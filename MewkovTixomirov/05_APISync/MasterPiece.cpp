/*****************************************
Модуль APISync
Copyright (c) 1997 Мешков А., Тихомиров Ю.
*****************************************/

#include <Windows.h> /* описание функций Win32 API*/

#define NUMBER_OF_THREADS	10
#define NUMBER_OF_RESOURCES	10

/* массив для хранения имени модуля */
TCHAR szModuleName[_MAX_PATH];

/* массивы для текстовых сообщений */
TCHAR szMessage[64 * NUMBER_OF_THREADS];
TCHAR szMessageCode[64];

/* дескрипторы событий и семафоров */
HANDLE hEmpty, hQuit, hResource;

/* объект "критическая секция" */
CRITICAL_SECTION section;

/* массив дескрипторов потоков */
HANDLE hThread[NUMBER_OF_THREADS];

/* массив для хранения количества полученных ресурсов */
DWORD dwCount[NUMBER_OF_THREADS];

/* количество ресурсов */
DWORD dwResources = NUMBER_OF_RESOURCES;

/* описание потоковой функции */
DWORD ThreadFunc(LPVOID lParam);



/* главная функция приложения */
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	int i;
	DWORD dwWait, dwExit, dwThreadId;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	/* проверяем, не была ли уже запущена копия приложения,
	будем использовать этот объект и для инициирования
	завершения потоков */
	hQuit = CreateEvent(
		NULL, /* используем атрибуты защиты по умолчанию */
		TRUE, /* создаём собятия с ручным сбросом */
		FALSE, /* начальное состояние - "сброшен" */
		TEXT("EventName1") /* уникальное имя */
	);

	/* если объект "событие" уже создан ранее -
	запущена другая копия приложения -
	завершаем выполнение с кодом 1 */
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return (1);
	/* запускаем копию процесса */

	/* заполняем структуру STARTUPINFO */
	GetStartupInfo(&si);

	/* получаем имя исполняемого модуля */
	GetModuleFileName(NULL, szModuleName, _MAX_PATH);

	/* создаём копию процесса */
	CreateProcess(
		NULL, /* имя модуля процесса передадим во втором параметре */
		szModuleName, /* командная строка процесса */
		NULL, /* используем атрибуты защиты ПРОЦЕССА по умолчанию */
		NULL,  /* используем атрибуты защиты ПОТОКА по умолчанию */
		FALSE, /* дескриптор ненаследуемый */
		CREATE_DEFAULT_ERROR_MODE, /* стандартная обработка ошибок */
		NULL, /* переменные окружения не нужны */
		NULL, /* текущий каталог не используем */
		&si, /* передаём копию собственной структуры STARTUPINFO */
		&pi /* получим дескриптор и другую информацию о процессе */
	);

	/* дочерний процесс должен завершиться немедленно, т.к. уже загружена его копия */

	/* ожидание завершения дочернего процесса */
	dwWait = WaitForSingleObject(pi.hProcess, 5000);
	
	//CloseHandle(pi.hProcess); //Added after Compiler Error
	//CloseHandle(pi.hThread); //Added after Compiler Error

	/* проводим диагностику результата завершения дочернего процесса */
	if (dwWait == WAIT_OBJECT_0)
	{
		GetExitCodeProcess(pi.hProcess, &dwExit);
		wsprintf(szMessage, (LPCWSTR)L"Копия процесса завершена.\nКод завершения: %d", dwExit);
		MessageBox(NULL, szMessage, (LPCWSTR)L"APISync", MB_OK);
	}
	else if (dwWait == WAIT_TIMEOUT)
		MessageBox(NULL,
			(LPCWSTR)L"Дочерний процесс не завершился по истечении 5 секунд", (LPCWSTR)L"APISync", MB_OK);
	else
		/* этот код никогда не выполняется */
	/* объект используется для сигнализации о том, что ресурсы исчерпаны */
		hEmpty = CreateEvent(NULL, FALSE, FALSE, NULL);

	/* создаём семафор для синхронизации доступа к ресурсам*/
	hResource = CreateSemaphore(
		NULL, /* используем атрибуты защиты по умолчанию */
		NUMBER_OF_RESOURCES, /* начальное значение счётчика */
		NUMBER_OF_RESOURCES, /* максимальное значение счётчика */
		NULL /* создаём объект безымянным */
	);

	/* инициализируем критическую секцию для защиты переменной,
	содержащей количество свободных ресурсов */
	InitializeCriticalSection(&section);

	/* создаём все потоки */
	for (i = 0; i < NUMBER_OF_THREADS; i++)
		hThread[i] = CreateThread(
			NULL, /* используем атрибуты защиты по умолчанию */
			0, /* начальный размер стека, как и у создающего потока */
			(LPTHREAD_START_ROUTINE)ThreadFunc, /* указатель на функцию потока */
			&dwCount[i], /* адрес переменной для хранения количества выделенных ресурсов */
			(i % 3) ? 0 : CREATE_SUSPENDED, /* часть потоков создаём, но не запускаем */
			&dwThreadId /* по завершении получим идентификатор потока, хотя в дальнейшем не используем его */
		);

	/* ждём момента, когда все ресурсы будут исчерпаны или пройдёт 5 секунд */
	WaitForSingleObject(hEmpty, 5000);

	/* инициируем окончание потоков */
	PulseEvent(hQuit);

	/* ждём завершения потоков, но не более 5 секунд */
	WaitForMultipleObjects(
		NUMBER_OF_THREADS, /* количество синхронизирующих объектов */
		hThread, /* массив дескрипторов синхронизурующих объектов */
		TRUE, /* ждём установки всех указанных объектов */
		5000 /* но не более 5 секунд */
	);

	/* формируем протокольное сообщение:
	- заголовок,
	- номер потока, код возврата и значение переменной,
	  хранящей количество выделенных потоку ресурсов
	  (должны быть равны - печатаем для проверки)
	- количество свободных ресурсов */

	lstrcpy(szMessage, (LPCWSTR)L"Протокол завершения потоков.\n\n");

	for (i = 0; i < NUMBER_OF_THREADS; i++)
	{
		/* получаем код возврата, содержащий количество выделенных потоку ресурсов */
		GetExitCodeThread(hThread[i], &dwExit);

		/* часть потоков могла не завершиться:
		одни потоки находились в приостановленном состоянии,
		другие в момент вызова функции PulseEvent
		ещё не вызвали в очередной раз ожидающую
		функцию WaitForMultipleObjects - удаляем их */
		if (dwExit == STILL_ACTIVE)
		{
			/* принудительно удаляем поток */
			TerminateThread(hThread[i], 0);

			if ((i % 3) == 0) /* такие потоки были приостановлены */
				wsprintf(szMessageCode, (LPCWSTR)L"\tПоток %d - удалён\n", i);
			else
				wsprintf(szMessageCode, (LPCWSTR)L"\tПоток %d - удалён (ресурсов %d)\n", i, dwCount[i]);
		}
		else
			wsprintf(szMessageCode, (LPCWSTR)L"\tПоток %d - ресурсов %d (%d)\n", i, dwExit, dwCount[i]);
		lstrcat(szMessage, szMessageCode);
	}

	wsprintf(szMessageCode, (LPCWSTR)L"\nСвободных ресурсов %d\n", dwResources);

	lstrcat(szMessage, szMessageCode);
	MessageBox(NULL, szMessage, (LPCWSTR)L"Win32(r) API", MB_OK);

	/* удаляем синхронизирующие объекты */
	DeleteCriticalSection(&section);

	CloseHandle(hResource);
	CloseHandle(hQuit);

	/* нормальное завершение процесса */
	return(0);
}



/* функция потока ожидает освобождения ресурса или
сигнала о завершении, если потоку уже выделено пять
и более ресурсов - освобождаем все */
DWORD ThreadFunc(LPVOID lParam)
{
	DWORD dwWait;
	/* преобразуем параметр в указатель на переменную
	для хранения количества выделенных потоку ресурсов */
	LPDWORD lpdwCount = (LPDWORD)lParam;

	/* формируем массив синхронизирующих объектовЖ
	- событие для сигнализации о необходимости завершения
	- дескриптор семафора, синхронизирующего выделение ресурсов */
	HANDLE hHandle[] = { hQuit, hResource };
	*lpdwCount = 0;

	for (;;)
	{
		dwWait = WaitForMultipleObjects(
			2, /* количество синхронизирующих объектов */
			hHandle, /* массив дескрипторов синхронизирующих объектов */
			FALSE, /* ждём установки хотя бы одного из указанных объектов */
			INFINITE /* ограничения по времени нет */
		);
		if (dwWait == WAIT_OBJECT_0) /* hQuit - установлен */
			break;
		else /* получили ресурс */
		{
			if ((*lpdwCount + 1) >= 5) /* освобождаем ресурсы */
			{
				/* изменяем переменную, содержащую количество свободных ресурсов */
				EnterCriticalSection(&section);

				/* увеличиваем значение счётчика семафора */
				ReleaseSemaphore(hResource, *lpdwCount + 1, NULL);

				/* увеличиваем значение счётчика свободных ресурсов */
				dwResources += *lpdwCount;
				LeaveCriticalSection(&section);

				*lpdwCount = 0;
			}
			else /* закрепляем ресурс за собой */
			{
				*lpdwCount += 1;

				/* изменяем переменную, содержащую количество свободных ресурсов */
				EnterCriticalSection(&section);

				if (--dwResources == 0)
					SetEvent(hEmpty); /* сигнализируем о том, что ресурсы исчерпаны */

				LeaveCriticalSection(&section);
			}
		}
	}

	/* завершаем поток и возвращаем количество захваченных ресурсов */
	return *lpdwCount;
}



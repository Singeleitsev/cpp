/*****************************************
������ APISync
Copyright (c) 1997 ������ �., ��������� �.
*****************************************/

#include <Windows.h> /* �������� ������� Win32 API*/

#define NUMBER_OF_THREADS	10
#define NUMBER_OF_RESOURCES	10

/* ������ ��� �������� ����� ������ */
TCHAR szModuleName[_MAX_PATH];

/* ������� ��� ��������� ��������� */
TCHAR szMessage[64 * NUMBER_OF_THREADS];
TCHAR szMessageCode[64];

/* ����������� ������� � ��������� */
HANDLE hEmpty, hQuit, hResource;

/* ������ "����������� ������" */
CRITICAL_SECTION section;

/* ������ ������������ ������� */
HANDLE hThread[NUMBER_OF_THREADS];

/* ������ ��� �������� ���������� ���������� �������� */
DWORD dwCount[NUMBER_OF_THREADS];

/* ���������� �������� */
DWORD dwResources = NUMBER_OF_RESOURCES;

/* �������� ��������� ������� */
DWORD ThreadFunc(LPVOID lParam);



/* ������� ������� ���������� */
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

	/* ���������, �� ���� �� ��� �������� ����� ����������,
	����� ������������ ���� ������ � ��� �������������
	���������� ������� */
	hQuit = CreateEvent(
		NULL, /* ���������� �������� ������ �� ��������� */
		TRUE, /* ������ ������� � ������ ������� */
		FALSE, /* ��������� ��������� - "�������" */
		TEXT("EventName1") /* ���������� ��� */
	);

	/* ���� ������ "�������" ��� ������ ����� -
	�������� ������ ����� ���������� -
	��������� ���������� � ����� 1 */
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return (1);
	/* ��������� ����� �������� */

	/* ��������� ��������� STARTUPINFO */
	GetStartupInfo(&si);

	/* �������� ��� ������������ ������ */
	GetModuleFileName(NULL, szModuleName, _MAX_PATH);

	/* ������ ����� �������� */
	CreateProcess(
		NULL, /* ��� ������ �������� ��������� �� ������ ��������� */
		szModuleName, /* ��������� ������ �������� */
		NULL, /* ���������� �������� ������ �������� �� ��������� */
		NULL,  /* ���������� �������� ������ ������ �� ��������� */
		FALSE, /* ���������� ������������� */
		CREATE_DEFAULT_ERROR_MODE, /* ����������� ��������� ������ */
		NULL, /* ���������� ��������� �� ����� */
		NULL, /* ������� ������� �� ���������� */
		&si, /* ������� ����� ����������� ��������� STARTUPINFO */
		&pi /* ������� ���������� � ������ ���������� � �������� */
	);

	/* �������� ������� ������ ����������� ����������, �.�. ��� ��������� ��� ����� */

	/* �������� ���������� ��������� �������� */
	dwWait = WaitForSingleObject(pi.hProcess, 5000);
	
	//CloseHandle(pi.hProcess); //Added after Compiler Error
	//CloseHandle(pi.hThread); //Added after Compiler Error

	/* �������� ����������� ���������� ���������� ��������� �������� */
	if (dwWait == WAIT_OBJECT_0)
	{
		GetExitCodeProcess(pi.hProcess, &dwExit);
		wsprintf(szMessage, (LPCWSTR)L"����� �������� ���������.\n��� ����������: %d", dwExit);
		MessageBox(NULL, szMessage, (LPCWSTR)L"APISync", MB_OK);
	}
	else if (dwWait == WAIT_TIMEOUT)
		MessageBox(NULL,
			(LPCWSTR)L"�������� ������� �� ���������� �� ��������� 5 ������", (LPCWSTR)L"APISync", MB_OK);
	else
		/* ���� ��� ������� �� ����������� */
	/* ������ ������������ ��� ������������ � ���, ��� ������� ��������� */
		hEmpty = CreateEvent(NULL, FALSE, FALSE, NULL);

	/* ������ ������� ��� ������������� ������� � ��������*/
	hResource = CreateSemaphore(
		NULL, /* ���������� �������� ������ �� ��������� */
		NUMBER_OF_RESOURCES, /* ��������� �������� �������� */
		NUMBER_OF_RESOURCES, /* ������������ �������� �������� */
		NULL /* ������ ������ ���������� */
	);

	/* �������������� ����������� ������ ��� ������ ����������,
	���������� ���������� ��������� �������� */
	InitializeCriticalSection(&section);

	/* ������ ��� ������ */
	for (i = 0; i < NUMBER_OF_THREADS; i++)
		hThread[i] = CreateThread(
			NULL, /* ���������� �������� ������ �� ��������� */
			0, /* ��������� ������ �����, ��� � � ���������� ������ */
			(LPTHREAD_START_ROUTINE)ThreadFunc, /* ��������� �� ������� ������ */
			&dwCount[i], /* ����� ���������� ��� �������� ���������� ���������� �������� */
			(i % 3) ? 0 : CREATE_SUSPENDED, /* ����� ������� ������, �� �� ��������� */
			&dwThreadId /* �� ���������� ������� ������������� ������, ���� � ���������� �� ���������� ��� */
		);

	/* ��� �������, ����� ��� ������� ����� ��������� ��� ������ 5 ������ */
	WaitForSingleObject(hEmpty, 5000);

	/* ���������� ��������� ������� */
	PulseEvent(hQuit);

	/* ��� ���������� �������, �� �� ����� 5 ������ */
	WaitForMultipleObjects(
		NUMBER_OF_THREADS, /* ���������� ���������������� �������� */
		hThread, /* ������ ������������ ���������������� �������� */
		TRUE, /* ��� ��������� ���� ��������� �������� */
		5000 /* �� �� ����� 5 ������ */
	);

	/* ��������� ������������ ���������:
	- ���������,
	- ����� ������, ��� �������� � �������� ����������,
	  �������� ���������� ���������� ������ ��������
	  (������ ���� ����� - �������� ��� ��������)
	- ���������� ��������� �������� */

	lstrcpy(szMessage, (LPCWSTR)L"�������� ���������� �������.\n\n");

	for (i = 0; i < NUMBER_OF_THREADS; i++)
	{
		/* �������� ��� ��������, ���������� ���������� ���������� ������ �������� */
		GetExitCodeThread(hThread[i], &dwExit);

		/* ����� ������� ����� �� �����������:
		���� ������ ���������� � ���������������� ���������,
		������ � ������ ������ ������� PulseEvent
		��� �� ������� � ��������� ��� ���������
		������� WaitForMultipleObjects - ������� �� */
		if (dwExit == STILL_ACTIVE)
		{
			/* ������������� ������� ����� */
			TerminateThread(hThread[i], 0);

			if ((i % 3) == 0) /* ����� ������ ���� �������������� */
				wsprintf(szMessageCode, (LPCWSTR)L"\t����� %d - �����\n", i);
			else
				wsprintf(szMessageCode, (LPCWSTR)L"\t����� %d - ����� (�������� %d)\n", i, dwCount[i]);
		}
		else
			wsprintf(szMessageCode, (LPCWSTR)L"\t����� %d - �������� %d (%d)\n", i, dwExit, dwCount[i]);
		lstrcat(szMessage, szMessageCode);
	}

	wsprintf(szMessageCode, (LPCWSTR)L"\n��������� �������� %d\n", dwResources);

	lstrcat(szMessage, szMessageCode);
	MessageBox(NULL, szMessage, (LPCWSTR)L"Win32(r) API", MB_OK);

	/* ������� ���������������� ������� */
	DeleteCriticalSection(&section);

	CloseHandle(hResource);
	CloseHandle(hQuit);

	/* ���������� ���������� �������� */
	return(0);
}



/* ������� ������ ������� ������������ ������� ���
������� � ����������, ���� ������ ��� �������� ����
� ����� �������� - ����������� ��� */
DWORD ThreadFunc(LPVOID lParam)
{
	DWORD dwWait;
	/* ����������� �������� � ��������� �� ����������
	��� �������� ���������� ���������� ������ �������� */
	LPDWORD lpdwCount = (LPDWORD)lParam;

	/* ��������� ������ ���������������� ���������
	- ������� ��� ������������ � ������������� ����������
	- ���������� ��������, ����������������� ��������� �������� */
	HANDLE hHandle[] = { hQuit, hResource };
	*lpdwCount = 0;

	for (;;)
	{
		dwWait = WaitForMultipleObjects(
			2, /* ���������� ���������������� �������� */
			hHandle, /* ������ ������������ ���������������� �������� */
			FALSE, /* ��� ��������� ���� �� ������ �� ��������� �������� */
			INFINITE /* ����������� �� ������� ��� */
		);
		if (dwWait == WAIT_OBJECT_0) /* hQuit - ���������� */
			break;
		else /* �������� ������ */
		{
			if ((*lpdwCount + 1) >= 5) /* ����������� ������� */
			{
				/* �������� ����������, ���������� ���������� ��������� �������� */
				EnterCriticalSection(&section);

				/* ����������� �������� �������� �������� */
				ReleaseSemaphore(hResource, *lpdwCount + 1, NULL);

				/* ����������� �������� �������� ��������� �������� */
				dwResources += *lpdwCount;
				LeaveCriticalSection(&section);

				*lpdwCount = 0;
			}
			else /* ���������� ������ �� ����� */
			{
				*lpdwCount += 1;

				/* �������� ����������, ���������� ���������� ��������� �������� */
				EnterCriticalSection(&section);

				if (--dwResources == 0)
					SetEvent(hEmpty); /* ������������� � ���, ��� ������� ��������� */

				LeaveCriticalSection(&section);
			}
		}
	}

	/* ��������� ����� � ���������� ���������� ����������� �������� */
	return *lpdwCount;
}



/********************************************************************
Файл APIShell.cpp
Copyright (c) 1997 Мешков А., Тихомиров Ю.
С. 43
********************************************************************/

#define _INC_OLE
#include <Windows.h>
#undef _INC_OLE

#include <windowsx.h>
#include "resource.h"

/* Определение номера сообщения */
#define WM_NOTIFYICON (WM_APP+100)

HINSTANCE g_hinst;
UINT g_uState;

/* Идентификатор пиктограммы */
#define IDC_NOTIFY 1000

/* Объявление используемых функций */

/* Оконная функция диалога */
BOOL CALLBACK TestAppDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* Функция настройки уведомляющей области */
BOOL TrayMessage(HWND hDlg, DWORD dwMessage, HICON hIcon, PSTR pszTip);

/* Функция изменения пиктограммы уведомления */
void StateChange(HWND hDlg);



/* Реализация функций приложения */

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* Сохранение дескриптора приложения без проверки наличия предыдущих копий */
	g_hinst = hInstance;

	/* Создание и вывод диалога настройки - основного окна приложения*/
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_APISHELL), NULL, (DLGPROC)TestAppDlgProc);

	/* Основное окно закрыто - завершение приложения */
	return(FALSE);
}


/* Функция настройки уведомляющей области */

BOOL CALLBACK TestAppDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			/* Инициализация окна диалога */
			TrayMessage(hDlg, NIM_ADD, NULL, NULL);
			StateChange(hDlg);
			break;

		case WM_DESTROY:
			/* Разрушение окна диалога */
			TrayMessage(hDlg, NIM_DELETE, NULL, NULL);
			break;

		case WM_COMMAND:
			/* Обработка сообщений от элементов окна диалога */
			switch (GET_WM_COMMAND_ID(wParam, lParam))
			{
				case IDCANCEL:
					EndDialog(hDlg, TRUE);
					break;

				case IDHIDE:
					ShowWindow(hDlg, SW_HIDE);
					break;
			}
			break;

		case WM_NOTIFYICON:
			/* Обработка сообщений от пиктограммы области уведомления */
			switch (lParam)
			{
				case WM_LBUTTONDOWN:
					switch (wParam)
					{
						case IDC_NOTIFY:
							/* Смена состояния с заменой пиктограммы*/
							g_uState = !g_uState;
							StateChange(hDlg);
							break;

						default:
							break;
					}
					break;

				case WM_RBUTTONDOWN:
					/* Активизация окна диалога */
					ShowWindow(hDlg, SW_SHOW);
					SetForegroundWindow(hDlg);
					break;

				default:
					break;
			}
			break;

		default:
			return(FALSE);

	}

return(TRUE);
}


BOOL TrayMessage(HWND hDlg, DWORD dwMessage, HICON hIcon, PSTR pszTip)
{
	BOOL retCode;
	NOTIFYICONDATA tnd;

	/* Формирование структуры для работы с областью уведомления */
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = hDlg;
	tnd.uID = IDC_NOTIFY;

	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = WM_NOTIFYICON;
	tnd.hIcon = hIcon;

	if (pszTip)
		lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
	else
		tnd.szTip[0] = '\0';

	/* Собственно настройка области уведомления */
	retCode = Shell_NotifyIcon(dwMessage, &tnd);

	if (hIcon)
		DestroyIcon(hIcon);

	return retCode;
}

void StateChange(HWND hDlg)
{
	LPCSTR pszIDIcon;
	char szTip[64];

	/* загрузка нужного текста и пиктограммы в зависимости от текущего состояния*/
	if (!LoadString(g_hinst, g_uState == 0 ? IDC_STATE1 : IDC_STATE2, szTip, sizeof(szTip)))
		*szTip = '\0';

	pszIDIcon = MAKEINTRESOURCE(g_uState ? IDI_STATE1 : IDI_STATE2);

	TrayMessage(hDlg, NIM_MODIFY, reinterpret_cast <HICON> (LoadImage(g_hinst, pszIDIcon, IMAGE_ICON, 16, 16, 0)), szTip);
}

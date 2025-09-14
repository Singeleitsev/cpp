#include <windows.h>
#include <windowsx.h>
#include "afxres.h"
#include "resource.h"

//----------------------
// Function Declarations
//----------------------

// Main Window Function
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//WM_CREATE Message Handler Function
BOOL WndProc_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
					  
//WM_LBUTTONDOWN Message Handler Function
void WndProc_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

//WM_DESTROY Message Handler Function
void WndProc_OnDestroy(HWND hWnd);

//-----------------
// Global Variables
//-----------------

HINSTANCE hInst;
char szAppName[] = "Window";
char szAppTitle[] = "Window Application Title";

//-----------------
// WinMain Function
//-----------------

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;

	hInst = hInstance;

	//Check for Previously Executed Applications
	hWnd = FindWindow(szAppName, NULL);
	if(hWnd)
	{
		if(IsIconic(hWnd))
			ShowWindow(hWnd, SW_RESTORE);
			SetForegroundWindow(hWnd);
		return FALSE;
	}

	//Register Window Class
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	//wc.hIconSm = LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON_SM), IMAGE_ICON, 16, 16, 0);
	wc.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON_SM));
	wc.style = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	//wc.hIcon = LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0);
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;
	if(!RegisterClassEx(&wc))
		if(!RegisterClass((LPWNDCLASS)&wc.style))
			return FALSE;

	hWnd = CreateWindow(szAppName, szAppTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);

	if(!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//-----------------
// WndProc Function
//-----------------

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, WndProc_OnCreate);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, WndProc_OnLButtonDown);
		HANDLE_MSG(hWnd, WM_DESTROY, WndProc_OnDestroy);

	default:
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
}

//------------------------
// Event Handler Functions
//------------------------

BOOL WndProc_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	return TRUE;
}

//Disable the Warning caused by the 'void' type function
//returning some value
//returend value is needed for the FORWARD_WM_BUTTONDOWN macro
//#pragma warning(disable: 4098)

void WndProc_OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
	//return FORWARD_WM_DESTROY(hWnd, DefWindowProc);
	FORWARD_WM_DESTROY(hWnd, DefWindowProc);
}

//Disable the Warning caused by the 'void' type function
//returning some value
//returend value is needed for the FORWARD_WM_BUTTONDOWN macro
//#pragma warning(disable: 4098)

void WndProc_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	MessageBox(NULL, "Hello, 32-bit World!", "Window", MB_OK);
	//return FORWARD_WM_LBUTTONDOWN(hWnd, fDoubleClick, x, y, keyFlags, DefWindowProc);
	FORWARD_WM_LBUTTONDOWN(hWnd, fDoubleClick, x, y, keyFlags, DefWindowProc);
}




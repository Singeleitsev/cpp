// oglTemplate.cpp : Определяет точку входа для приложения

#include <windows.h>
//#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>

#define _USE_MATH_DEFINES // for C++
#include <cmath>

#include "framework.h"
#include "oglTemplate.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define MAX_LOADSTRING 100


// Отправить объявления функций, включенных в этот модуль кода:
void EnableOpenGL();
void InitScene();
bool DrawGLScene();
void DisableOpenGL();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);


//const float PI_div_180 = 0.0174532925f;
const float PI_div_180 = M_PI / 180.0f;

WCHAR szTitle[MAX_LOADSTRING]; // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING]; // имя класса главного окна

//Vertex Definitions
float sb11_Top[]{ -110,-110,0, 110,-110,0, 110,110,0, -110,110,0 };
//Channel Positions
float xOffset[]{14,1, 20,1, 24,1, 28,1, 30,1, 32,1, 34,1, 36,1, 38,1, 40,1, 42,2, 44,2, 46,3, 48,7};

// Глобальные переменные:
HINSTANCE g_hInst; // текущий экземпляр
HWND g_hWnd;
HDC g_hDC;
HGLRC g_hRC;

MSG msg;
WNDCLASSEXW wcex;
RECT rc;
PIXELFORMATDESCRIPTOR pfd;

int i0, i1; //Counters
bool keys[256]; // Array Used For The Keyboard Routine
bool active = TRUE; // Window Active Flag Set To TRUE By Default
bool fullscreen = TRUE; // Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat aXY, aXZ, aYZ;
GLfloat xTrans, yTrans, zTrans;
GLfloat dA, dL;


//Arguments passed to wWinMain. Where have they been taken?
//Where is g_hInst = GetModuleHandleW(); ?

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    g_hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    LoadStringW(g_hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(g_hInst, IDC_OGLTEMPLATE, szWindowClass, MAX_LOADSTRING);

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = g_hInst;
    wcex.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_OGLTEMPLATE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_OGLTEMPLATE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    // Выполнить инициализацию приложения
    g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OGLTEMPLATE));

    ShowWindow(g_hWnd, nCmdShow);

    EnableOpenGL();

    InitScene();

    // Цикл основного сообщения:
	BOOL done = FALSE; // Bool Variable To Exit Loop

	while (!done) // Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT) // Have We Received A Quit Message?
			{
				done = TRUE; // If So done=TRUE
			}
			else // If Not, Deal With Window Messages
			{
				TranslateMessage(&msg); // Translate The Message
				DispatchMessage(&msg); // Dispatch The Message
			}
		}
		else // If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) // Active?  Was There A Quit Received?
			{
				done = TRUE; // ESC or DrawGLScene Signalled A Quit
			}
			else // Not Time To Quit, Update Screen
			{
				SwapBuffers(g_hDC); // Swap Buffers (Double Buffering)

				if (keys[0x57]) //W
				{
					aYZ -= dA;
				}
				if (keys[0x53]) //S
				{
					aYZ += dA;
				}
				if (keys[0x41]) //A
				{
					aXY += dA;				
				}
				if (keys[0x44]) //D
				{
					aXY -= dA;					
				}
				if (keys[VK_UP])
				{
                    xTrans += (float)sin(aXY * PI_div_180) * dL;
					yTrans += (float)cos(aXY * PI_div_180) * dL;					
				}
				if (keys[VK_DOWN])
				{
                    xTrans -= (float)sin(aXY * PI_div_180) * dL;
					yTrans -= (float)cos(aXY * PI_div_180) * dL;
				}
				if (keys[VK_LEFT])
				{
                    xTrans -= (float)cos(aXY * PI_div_180) * dL;
					yTrans += (float)sin(aXY * PI_div_180) * dL;
				}
				if (keys[VK_RIGHT])
				{
                    xTrans += (float)cos(aXY * PI_div_180) * dL;
					yTrans -= (float)sin(aXY * PI_div_180) * dL;
				}
                if (keys[VK_SPACE])
                {
                    InitScene();
                }
			}
		}
	}

    /* shutdown OpenGL */
    DisableOpenGL();
	/* destroy the window explicitly */
	DestroyWindow(g_hWnd);

    return (int) msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:							// Is A Key Being Held Down?
    {
        keys[wParam] = TRUE;					// If So, Mark It As TRUE
        return 0;								// Jump Back
    }

    case WM_KEYUP:								// Has A Key Been Released?
    {
        keys[wParam] = FALSE;					// If So, Mark It As FALSE
        return 0;								// Jump Back
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), g_hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void EnableOpenGL()
{
	/* get the device context (DC) */
	g_hDC = GetDC(g_hWnd);

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL + PFD_DOUBLEBUFFER + PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = 0; //PFD_TYPE_RGBA
    pfd.cColorBits = 24; //Color Depth = 24 bits per pixel
    pfd.cRedBits = 0; //Color Bits Ignored
    pfd.cRedShift = 0; //Color Bits Ignored
    pfd.cGreenBits = 0; //Color Bits Ignored
    pfd.cGreenShift = 0; //Color Bits Ignored
    pfd.cBlueBits = 0; //Color Bits Ignored
    pfd.cBlueShift = 0; //Color Bits Ignored
    pfd.cAlphaBits = 0; //No Alpha Buffer
    pfd.cAlphaShift = 0; //Shift Bit Ignored
    pfd.cAccumBits = 0; //No Accumulation Buffer
    pfd.cAccumRedBits = 0; //Accumulation Bits Ignored
    pfd.cAccumGreenBits = 0; //Accumulation Bits Ignored
    pfd.cAccumBlueBits = 0; //Accumulation Bits Ignored
    pfd.cAccumAlphaBits = 0; //Accumulation Bits Ignored
    pfd.cDepthBits = 16; //16 Bit z-Buffer (Depth Buffer) 
    pfd.cStencilBits = 0; //No Stencil Buffer
    pfd.cAuxBuffers = 0; //No Auxiliary Buffer
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.bReserved = 0; //Reserved
    pfd.dwLayerMask = 0; //Layer Masks Ignored
    pfd.dwVisibleMask = 0; //Layer Masks Ignored
    pfd.dwDamageMask = 0; //Layer Masks Ignored

   SetPixelFormat(g_hDC, ChoosePixelFormat(g_hDC, &pfd), &pfd);

    /* create and enable the render context (RC) */
    g_hRC = wglCreateContext(g_hDC);

    wglMakeCurrent(g_hDC, g_hRC);
}

void InitScene()
{
    aXY = 0.0f;
    aXZ = 0.0f;
    aYZ = 0.0f;
    xTrans = 0.0f;
    yTrans = 0.0f;
    zTrans = 0.0f;
    dA = 1.0f; //Rotation Moment
    dL = 100.0f; //Speed
};

bool DrawGLScene()
{
	/* OpenGL animation code goes here */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
    
    //glOrtho(-200, 200, -200, 200, -200, 200);

    glLoadIdentity();
    glFrustum(-7500,7500, -7500,7500, 20000,40000);

    glRotatef(aXY, 0.0f, 0.0f, 1.0f);
    glRotatef(aXZ, 0.0f, 1.0f, 0.0f);
    glRotatef(aYZ, 1.0f, 0.0f, 0.0f);
    glTranslatef(xTrans, yTrans, zTrans-30000);

    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &sb11_Top); //3 Coordinates per Vertex
        glColor3f(0.5, 0.5, 0.5);
        for (i0 = -24; i0 < 24; i0++)
            for (i1 = -24; i1 < 24; i1++)
            {
                glPushMatrix();
                glTranslatef(i0 * 250, i1 * 250, 0);
                    glDrawArrays(GL_QUADS, 0, 4); //Only 4 Points are Defined now
                glPopMatrix();
            }
    glDisableClientState(GL_VERTEX_ARRAY);

    return 1;
}

void DisableOpenGL()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(g_hRC);
	ReleaseDC(g_hWnd, g_hDC);
}



// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

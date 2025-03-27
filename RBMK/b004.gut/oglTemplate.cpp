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
void ReSizeScene();
void InitScene();
void rbmkInitLayout();
void Extrude(int nVertices, float htHeight, float pVertices[]);
void rbmkDrawSb11();
void rbmkDrawCap(float, float, float);
void rbmkDrawChannel();
void rbmkDrawGraphite();
bool DrawScene();
void DisableOpenGL();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);


//const float PI_div_180 = 0.0174532925f;
const float PI_div_180 = float(M_PI) / 180.0f;

WCHAR szTitle[MAX_LOADSTRING]; // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING]; // имя класса главного окна

// Глобальные переменные:
HINSTANCE g_hInst; // текущий экземпляр
HWND g_hWnd;
HDC g_hDC;
HGLRC g_hRC;

MSG msg;
WNDCLASSEXW wcex;
RECT rc;
PIXELFORMATDESCRIPTOR pfd;

int x,y,z,i0,i1; //Counters
bool keys[256]; // Array Used For The Keyboard Routine
bool active = TRUE; // Window Active Flag Set To TRUE By Default
bool fullscreen = TRUE; // Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat aYZ, aXZ, aXY;
GLfloat xTrans, yTrans, zTrans;
GLfloat dA, dL;

//Main Layout Array
byte nLayout[48][48] = { 0 };

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
    ReSizeScene();
    InitScene();

    rbmkInitLayout();

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
			if ((active && !DrawScene()) || keys[VK_ESCAPE]) // Active?  Was There A Quit Received?
			{
				done = TRUE; // ESC or DrawGLScene Signalled A Quit
			}
			else // Not Time To Quit, Update Screen
			{
				SwapBuffers(g_hDC); // Swap Buffers (Double Buffering)

				if (keys[0x57]) //W
				{
					aYZ -= dA; //Camera Pitch
				}
				if (keys[0x53]) //S
				{
					aYZ += dA; //Camera Pitch
				}
				if (keys[0x41]) //A
				{
					aXY += dA; //Camera Yaw
				}
				if (keys[0x44]) //D
				{
					aXY -= dA; //Camera Yaw
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
    case WM_KEYDOWN: // Is A Key Being Held Down?
    {
        keys[wParam] = TRUE; // If So, Mark It As TRUE
        return 0; // Jump Back
    }
    case WM_KEYUP: // Has A Key Been Released?
    {
        keys[wParam] = FALSE; // If So, Mark It As FALSE
        return 0; // Jump Back
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
    case WM_ACTIVATE: // Watch For Window Activate Message
    {
        // LoWord Can Be WA_INACTIVE, WA_ACTIVE, WA_CLICKACTIVE,
        // The High-Order Word Specifies The Minimized State Of The Window Being Activated Or Deactivated.
        // A NonZero Value Indicates The Window Is Minimized.
        if ((LOWORD(wParam) != WA_INACTIVE) && !((BOOL)HIWORD(wParam)))
            active = TRUE; // Program Is Active
        else
            active = FALSE; // Program Is No Longer Active
        return 0; // Return To The Message Loop
    }
    case WM_SIZE:
    {
        rc.right = LOWORD(lParam);
        rc.bottom = HIWORD(lParam);
        ReSizeScene();
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

void ReSizeScene()
{
    glViewport(0, 0, rc.right, rc.bottom);
}

void InitScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ScreenRatio = (float)rc.right / (float)rc.bottom;
    glOrtho(-15000.0f * ScreenRatio, 15000.0f * ScreenRatio, -15000.0f, 15000.0f, -15000.0f, 15000.0f); //Set zTrans = 10000.0f
    //glFrustum(-7500.0f,7500.0f, -7500.0f,7500.0f, 20000.0f,40000.0f); //Set zTrans = -30000.0f

    aYZ = 240.0f; // Camera Pitch
    aXZ = 0.0f; // Camera Yaw
    aXY = 30.0f; // Camera Roll
    xTrans = 0.0f;
    yTrans = 0.0f;
    zTrans = 10000.0f; //For glOrtho Mode
    //zTrans = -30000.0f; //For glFrustum Mode
    dA = 1.0f; //Rotation Moment
    dL = 100.0f; //Speed
}

void rbmkInitLayout()
{
    //Gray
    for (x = 16; x < 31; x++) nLayout[x][0] = 1;
    for (x = 14; x < 34; x++) { nLayout[x][1] = 1; nLayout[x][46] = 1; };
    for (x = 12; x < 36; x++) { nLayout[x][2] = 1; nLayout[x][45] = 1; };
    for (x = 10; x < 38; x++) { nLayout[x][3] = 1; nLayout[x][44] = 1; };
    for (x = 9; x < 39; x++) { nLayout[x][4] = 1; nLayout[x][43] = 1; };
    for (x = 8; x < 40; x++) { nLayout[x][5] = 1; nLayout[x][42] = 1; };
    for (x = 7; x < 41; x++) { nLayout[x][6] = 1; nLayout[x][41] = 1; };
    for (x = 6; x < 42; x++) { nLayout[x][7] = 1; nLayout[x][40] = 1; };
    for (x = 5; x < 43; x++) { nLayout[x][8] = 1; nLayout[x][39] = 1; };
    for (x = 4; x < 44; x++) { nLayout[x][9] = 1; nLayout[x][38] = 1; };
    for (x = 3; x < 45; x++) { nLayout[x][10] = 1; nLayout[x][11] = 1; nLayout[x][36] = 1; nLayout[x][37] = 1; };
    for (x = 2; x < 46; x++) { nLayout[x][12] = 1; nLayout[x][13] = 1; nLayout[x][34] = 1; nLayout[x][35] = 1; };
    for (x = 1; x < 47; x++) { nLayout[x][14] = 1; nLayout[x][15] = 1; nLayout[x][32] = 1; nLayout[x][33] = 1; };
    for (x = 1; x < 48; x++) nLayout[x][16] = 1;
    for (x = 0; x < 48; x++) for (y = 17; y < 31; y++) nLayout[x][y] = 1;
    for (x = 0; x < 47; x++) nLayout[x][31] = 1;
    for (x = 17; x < 32; x++) nLayout[x][47] = 1;

    //Green
    for (x = 1; x < 46; x += 4) { nLayout[x][30] = 2; nLayout[x][26] = 2; nLayout[x][22] = 2; nLayout[x][18] = 2; };
    for (x = 3; x < 44; x += 4) { nLayout[x][32] = 2; nLayout[x][24] = 2; nLayout[x][16] = 2; };
    for (x = 5; x < 42; x += 4) { nLayout[x][38] = 2; nLayout[x][34] = 2; nLayout[x][14] = 2; nLayout[x][10] = 2; };
    for (x = 7; x < 40; x += 4) { nLayout[x][40] = 2; nLayout[x][36] = 2; nLayout[x][28] = 2; nLayout[x][20] = 2; nLayout[x][12] = 2; nLayout[x][8] = 2; };
    for (x = 9; x < 38; x += 4) { nLayout[x][42] = 2; nLayout[x][6] = 2; };
    for (x = 15; x < 32; x += 8) { nLayout[x][44] = 2; nLayout[x][4] = 2; };
    for (x = 17; x < 30; x += 4) { nLayout[x][46] = 2; nLayout[x][2] = 2; };
    nLayout[33][2] = 2;

    //Yellow
    for (x = 3; x < 44; x += 8) for (y = 12; y < 37; y += 8) nLayout[x][y] = 3;
    for (x = 11; x < 36; x += 8) { nLayout[x][44] = 3; nLayout[x][4] = 3; };

    //Red
    nLayout[23][36] = 4;
    nLayout[15][32] = 4;
    nLayout[31][32] = 4;
    nLayout[23][28] = 4;
    nLayout[11][24] = 4;
    nLayout[19][24] = 4;
    nLayout[27][24] = 4;
    nLayout[35][24] = 4;
    nLayout[23][20] = 4;
    nLayout[15][16] = 4;
    nLayout[31][16] = 4;
    nLayout[23][12] = 4;


    //Blue
    nLayout[15][40] = 5;
    nLayout[31][40] = 5;
    nLayout[7][32] = 5;
    nLayout[23][32] = 5;
    nLayout[39][32] = 5;
    nLayout[15][24] = 5;
    nLayout[31][24] = 5;
    nLayout[7][16] = 5;
    nLayout[23][16] = 5;
    nLayout[39][16] = 5;
    nLayout[15][8] = 5;
    nLayout[31][8] = 5;
}

void Extrude(int nVertices, float htHeight, float pVertices[])
{
    glBegin(GL_QUADS);
        for (i0 = 0; i0 < (nVertices-1)*2; i0 += 2)
        {
            glVertex3f(pVertices[i0], pVertices[i0 + 1], 0.0f);
            glVertex3f(pVertices[i0 + 2], pVertices[i0 + 3], 0.0f);
            glVertex3f(pVertices[i0 + 2], pVertices[i0 + 3], htHeight);
            glVertex3f(pVertices[i0], pVertices[i0 + 1], htHeight);
        }
        glVertex3f(pVertices[i0], pVertices[i0 + 1], 0.0f);
        glVertex3f(pVertices[0], pVertices[1], 0.0f);
        glVertex3f(pVertices[0], pVertices[1], htHeight);
        glVertex3f(pVertices[i0], pVertices[i0 + 1], htHeight);
    glEnd();
}

void rbmkDrawSb11()
{
    float xyzSb11[12]{ -100.0f,-100.0f,0.0f, 100.0f,-100.0f,0.0f, 100.0f,100.0f,0.0f, -100.0f,100.0f,0.0f };
    glColor3f(0.55f, 0.55f, 0.55f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &xyzSb11); //3 Coordinates per Vertex
    glDrawArrays(GL_QUADS, 0, 4); //Only 4 Points are Defined now
}

void rbmkDrawCap(float red, float green, float blue)
{
    float xyzCap[12]{ -120.0f,-120.0f,0.0f, 120.0f,-120.0f,0.0f, 120.0f,120.0f,0.0f, -120.0f,120.0f,0.0f };
    glColor3f(red, green, blue);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &xyzCap); //3 Coordinates per Vertex
    glDrawArrays(GL_QUADS, 0, 4); //Only 4 Points are Defined now
}

void rbmkDrawChannel()
{
    float xyChannel[8]{ -20.0f,-20.0f, 20.0f,-20.0f, 20.0f,20.0f, -20.0f,20.0f };
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -20000.0f);
        glColor3f(0.75f, 0.75f, 0.75f);
        Extrude(4, 19200, xyChannel);
    glPopMatrix();
}

void rbmkDrawGraphite()
{
    float xyGraphite[8]{ -120.0f,-120.0f, 120.0f,-120.0f, 120.0f,120.0f, -120.0f,120.0f };
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -16800.0f);
        glColor3f(0.25f, 0.25f, 0.25f);
        Extrude(4, 8000, xyGraphite);
    glPopMatrix();
}

bool DrawScene()
{
    /* OpenGL animation code goes here */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(aYZ, 1.0f, 0.0f, 0.0f); //Camera Pitch
    glRotatef(aXZ, 0.0f, 1.0f, 0.0f); //Camera Yaw
    glRotatef(aXY, 0.0f, 0.0f, 1.0f); //Camera Roll
    glTranslatef(xTrans, yTrans, zTrans);

    for (y = 0; y < 48; y++)
        for (x = 0; x < 48; x++)
        {
            glPushMatrix();
            glTranslatef((float)x * 250.0f - 5875.0f, (float)y * 250.0f - 5875.0f, 0.0f);
            byte bType = nLayout[x][y];
            switch (bType)
            {
            case 1:
                rbmkDrawSb11();
                break;
            case 2:
                rbmkDrawCap(0.0f, 0.7f, 0.0f); //Green
                break;
            case 3:
                rbmkDrawCap(1.0f, 0.9f, 0.0f); //Yellow
                break;
            case 4:
                rbmkDrawCap(0.85f, 0.0f, 0.0f); //Red
                break;
            case 5:
                rbmkDrawCap(0.0f, 0.0f, 1.0f); //Blue
                break;
            }
            if (bType != 0)
            {
                rbmkDrawChannel();
                rbmkDrawGraphite();
            }
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

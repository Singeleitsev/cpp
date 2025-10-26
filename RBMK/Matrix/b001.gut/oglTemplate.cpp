#include "framework.h" // Quotation marks for custom headers
#include "oglTemplate.h"

// Application
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutProc(HWND, UINT, WPARAM, LPARAM);
void ResizeWndProc();
void CloseWndProc();

// OpenGL
void InitializeGL();
void SetView();
void DrawGLScene();
void CheckKeys();
void RefreshStatus();

// Object Geometry
void DrawAxes();
void DrawObject();
void rbmkDrawSb11();
void rbmkDrawCap();
void rbmkDrawChannel();
void rbmkDrawGraphite();
void Extrude(int nVertices, float htHeight, float pVertices[]);

//Const
const float PI_OVER_180 = float(M_PI) / 180.0f;
const DWORD DEFAULT_SCREEN_WIDTH = 1024;
const DWORD DEFAULT_SCREEN_HEIGHT = 768;

// Structures
WNDCLASSEXW wcx;
MSG msg;
RECT rc;
PIXELFORMATDESCRIPTOR pfd;
INITCOMMONCONTROLSEX icce;

// Global Handles
HINSTANCE ghInst;
HWND ghWnd;

// Menu
HACCEL hAccelTable;

// Status Bar
HWND hwndStatusBar;
int xStatusParts[12] = {0,0,0,0,0,0,0,0,0,0,0,-1}; //12 Indicated Variables
//Proportions Empirically Counted for Width = 1024
int xStatusProportions [11] = {80,160,240,320,400,480,560,640,720,820,920}; //11 Separators for 12 Fields

// Flags
BYTE isActive = 1;
BYTE isFullscreen = 0;
BYTE isInitialPosition = 1;
BYTE isRefreshed = 0;
BYTE nMouseMode = MOUSE_MODE_NO_ACTION;
unsigned int nFrame = 0;
int nLastError = 0;

// OpenGL
HDC ghDC;
HGLRC ghRC;
DWORD iPixelFormat = 0;
DWORD RectWidth, RectHeight;
GLdouble RectAspect;

// Cursor Position
int xScrCenter = 512;
int yScrCenter = 384;
int xPrevPos, xCurPos;
int yPrevPos, yCurPos;

// Motion
float dStep, dAngle;
float LinearSpeed = 10.0f;
float AngularSpeed = 0.1f;
float LinearBoost = 10.0f;
float AngularBoost = 10.0f;
float dxMouse, dyMouse;

// Matrices
float mtxProjectionBuffer[16];

//Lights
GLfloat light_position[] = { -12000.0f, -12000.0f, 24000.0f, 1.0f };
GLfloat light_ambient[] = { 0.04f, 0.04f, 0.035f, 0.0f };
GLfloat light_diffuse[] = { 0.04f, 0.04f, 0.035f, 0.0f };
//Materials
GLfloat matGrayAmbDiff[] = { 0.62f, 0.64f, 0.69f, 1.0f };
GLfloat matGreenAmbDiff[] = { 0.15f, 0.75f, 0.15f, 1.0f };
GLfloat matYellowAmbDiff[] = { 0.85f, 0.85f, 0.15f, 1.0f };
GLfloat matRedAmbDiff[] = { 0.85f, 0.15f, 0.15f, 1.0f };
GLfloat matBlueAmbDiff[] = { 0.25f, 0.4f, 0.85f, 1.0f };

// Keyboard
BYTE key[256] = { 0 };

// Strings
LPCTSTR szMainWndTitle{ L"C++ OpenGL Environment - [Frame " };
LPCTSTR szMainWndClass{ L"MainWndClass" };

LPCTSTR szMsgCloseText{ L"Close?" };

LPCTSTR szManualMsgTitle{ L"Manual" };
LPCTSTR szManualMsgText{
    L"Camera Motion:\n"
    "Left Mouse Button - Look Left - Right, Up - Down\n"
    "Mouse Wheel Down - Move Camera Left-Right, Up-Down\n"
    "Right Mouse Button - Roll Camera Left-Right, Look Up-Down\n"
    "Mouse Scroll - Move Camera Forward-Backward\n"
    "\n"
    "Arrow Up - Move Forward\n"
    "Arrow Down - Move Backward\n"
    "Arrow Left - Move Left\n"
    "Arrow Right - Move Right\n"
    "Page Up - Move Up\n"
    "Page Down - Move Down\n"
    "\n"
    "Object Rotation:\n"
    "W - Tilt from the Camera\n"
    "S - Tilt to the Camera\n"
    "A - Turn Left\n"
    "D - Turn Right\n"
    "Q - Tilt Left\n"
    "E - Tilt Right\n"
    "\n"
    "Tab - Turn the Object Clockwise Quick\n"
    "\n"
    "Space, Esc - Reset Scene\n"
    "Shift - Boost" };

//https://learn.microsoft.com/en-us/windows/win32/api/strsafe/nf-strsafe-stringcbprintfw
WCHAR pszDest[64]; //arraysize = 64;
size_t cbDest = 128; // arraysize * sizeof(WCHAR);
//https://learn.microsoft.com/en-us/cpp/c-runtime-library/format-specification-syntax-printf-and-wprintf-functions?view=msvc-170
LPCTSTR pszFormatTitle = TEXT("%s%u]");
LPCTSTR pszFormatStatus = TEXT("%s%.*f");

LPCTSTR pszAxisXX = TEXT("x.x="); //mtxProjectionBuffer[0] - Vector X, Coordinate X
LPCTSTR pszAxisYX = TEXT("y.x="); //mtxProjectionBuffer[1] - Vector Y, Coordinate X
LPCTSTR pszAxisZX = TEXT("z.x="); //mtxProjectionBuffer[2] - Vector Z, Coordinate X
LPCTSTR pszAxisXY = TEXT("x.y="); //mtxProjectionBuffer[4] - Vector X, Coordinate Y
LPCTSTR pszAxisYY = TEXT("y.y="); //mtxProjectionBuffer[5] - Vector Y, Coordinate Y
LPCTSTR pszAxisZY = TEXT("z.y="); //mtxProjectionBuffer[6] - Vector Z, Coordinate Y
LPCTSTR pszAxisXZ = TEXT("x.z="); //mtxProjectionBuffer[8] - Vector X, Coordinate Z
LPCTSTR pszAxisYZ = TEXT("y.z="); //mtxProjectionBuffer[9] - Vector Y, Coordinate Z
LPCTSTR pszAxisZZ = TEXT("z.z="); //mtxProjectionBuffer[10] - Vector Z, Coordinate Z
LPCTSTR pszAxisXW = TEXT("x.w="); //mtxProjectionBuffer[12] - Vector X, Coordinate W
LPCTSTR pszAxisYW = TEXT("y.w="); //mtxProjectionBuffer[13] - Vector Y, Coordinate W
LPCTSTR pszAxisZW = TEXT("z.w="); //mtxProjectionBuffer[14] - Vector Z, Coordinate W


// Main Layout Array
const BYTE nLayout[48][48] = { 
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,2,1,1,1,2,1,1,1,5,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,5,1,1,1,2,1,1,1,2,1,1,0,0,0,0,0,0,
0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
0,0,0,0,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,4,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,0,0,0,
0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
0,0,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,1,0,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,2,1,1,1,5,1,1,1,2,1,1,1,4,1,1,1,2,1,1,1,5,1,1,1,2,1,1,1,4,1,1,1,2,1,1,1,5,1,1,1,2,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,4,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,2,1,1,1,2,1,1,1,4,1,1,1,5,1,1,1,4,1,1,1,2,1,1,1,4,1,1,1,5,1,1,1,4,1,1,1,2,1,1,1,2,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,4,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
0,1,1,2,1,1,1,5,1,1,1,2,1,1,1,4,1,1,1,2,1,1,1,5,1,1,1,2,1,1,1,4,1,1,1,2,1,1,1,5,1,1,1,2,1,1,1,1,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,0,
0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
0,0,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,4,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,0,0,0,
0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
0,0,0,0,0,1,1,2,1,1,1,2,1,1,1,5,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,5,1,1,1,2,1,1,1,2,1,1,1,0,0,0,0,0,
0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// 10_WinMain.asm
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    // Arguments passed to wWinMain. Where have they been taken?
    UNREFERENCED_PARAMETER(hPrevInstance); // Why this expression?
    UNREFERENCED_PARAMETER(lpCmdLine); // Why this expression?

    ghInst = hInstance; // Where is GetModuleHandleW(); - ?

    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
    wcx.lpfnWndProc = WndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = ghInst;
    wcx.hIcon = LoadIcon(ghInst, MAKEINTRESOURCE(IDI_APPLICATION));
    wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcx.lpszMenuName = MAKEINTRESOURCEW(IDC_OGLTEMPLATE);
    wcx.lpszClassName = szMainWndClass;
    wcx.hIconSm = LoadIcon(wcx.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    RegisterClassEx(&wcx);

    // Выполнить инициализацию приложения
    ghWnd = CreateWindow(szMainWndClass, szMainWndTitle, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, 0, hInstance, 0);

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OGLTEMPLATE));

    InitializeGL();

    ShowWindow(ghWnd, nCmdShow);
    UpdateWindow(ghWnd);

    WinMainLoop:
        if (!PeekMessage(&msg, 0, 0, 0, PM_REMOVE) && isActive)
        {
            DrawGLScene(); // We'll draw the OpenGL Scene outside the WndProc   
        }
        else
        {
            if (msg.message == WM_QUIT)
            {
                CloseWndProc();
                return (int)msg.wParam;
            }
            else
            {
                if (!TranslateAccelerator(ghWnd, hAccelTable, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

        }
    goto WinMainLoop;
}

// 11_InitializeGL.asm
void InitializeGL()
{
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

    ghDC = GetDC(ghWnd);
    iPixelFormat = ChoosePixelFormat(ghDC, &pfd);
    SetPixelFormat(ghDC, iPixelFormat, &pfd);

    ghRC = wglCreateContext(ghDC);
    wglMakeCurrent(ghDC, ghRC);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    // glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

// 20_WndProc.asm
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_MOUSEMOVE:
        {
            switch (nMouseMode)
            {
                case MOUSE_MODE_CAMERA_ROTATION: //WM_LBUTTONDOWN
                {
                    xCurPos = GET_X_LPARAM(lParam);
                    yCurPos = GET_Y_LPARAM(lParam);
                    glMatrixMode(GL_PROJECTION);
                    dAngle = float((xPrevPos - xCurPos) / (-8));
                    glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
                    glTranslatef(-mtxProjectionBuffer[12], -mtxProjectionBuffer[13], -mtxProjectionBuffer[14]); //Camera UnMove
                    glRotatef(dAngle, mtxProjectionBuffer[1], mtxProjectionBuffer[5], mtxProjectionBuffer[9]); //World Y
                    glTranslatef(mtxProjectionBuffer[12], mtxProjectionBuffer[13], mtxProjectionBuffer[14]); //Camera Move
                    dAngle = float((yPrevPos - yCurPos) / 8); 
                    glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
                    glTranslatef(-mtxProjectionBuffer[12], -mtxProjectionBuffer[13], -mtxProjectionBuffer[14]); //Camera UnMove
                    glRotatef(-dAngle, mtxProjectionBuffer[0], mtxProjectionBuffer[4], mtxProjectionBuffer[8]); //World X
                    glTranslatef(mtxProjectionBuffer[12], mtxProjectionBuffer[13], mtxProjectionBuffer[14]); //Camera Move
                    //Store Values
                    xPrevPos = xCurPos;
                    yPrevPos = yCurPos;
                    //Set Flags
                    isInitialPosition = 0;
                    isRefreshed = 0;
                    return 0;
                }
                case MOUSE_MODE_CAMERA_PAN: //WM_MBUTTONDOWN
                {
                    xCurPos = GET_X_LPARAM(lParam);
                    yCurPos = GET_Y_LPARAM(lParam);
                    glMatrixMode(GL_PROJECTION);
                    dStep =  float((xPrevPos - xCurPos) * (-16));
                    glTranslatef(dStep * mtxProjectionBuffer[0], dStep * mtxProjectionBuffer[4], dStep * mtxProjectionBuffer[8]);
                    dStep =  float((yPrevPos - yCurPos) * 16);
                    glTranslatef(dStep * mtxProjectionBuffer[1], dStep * mtxProjectionBuffer[5], dStep * mtxProjectionBuffer[9]);
                    //Store Values
                    xPrevPos = xCurPos;
                    yPrevPos = yCurPos;
                    //Set Flags
                    isInitialPosition = 0;
                    isRefreshed = 0;
                    return 0;
                }
                case MOUSE_MODE_CAMERA_ROLL: //WM_RBUTTONDOWN
                {
                    xCurPos = GET_X_LPARAM(lParam);
                    yCurPos = GET_Y_LPARAM(lParam);
                    glMatrixMode(GL_PROJECTION);
                    dAngle = float((xPrevPos - xCurPos) / (-8));
                    glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
                    glTranslatef(-mtxProjectionBuffer[12], -mtxProjectionBuffer[13], -mtxProjectionBuffer[14]); //Camera UnMove
                    glRotatef(dAngle, mtxProjectionBuffer[1], mtxProjectionBuffer[5], mtxProjectionBuffer[9]); //World Y
                    glTranslatef(mtxProjectionBuffer[12], mtxProjectionBuffer[13], mtxProjectionBuffer[14]); //Camera Move
                    dAngle = float((yPrevPos - yCurPos) / 8);
                    glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
                    glTranslatef(-mtxProjectionBuffer[12], -mtxProjectionBuffer[13], -mtxProjectionBuffer[14]); //Camera UnMove
                    glRotatef(dAngle, mtxProjectionBuffer[2], mtxProjectionBuffer[6], mtxProjectionBuffer[10]); //World Z
                    glTranslatef(mtxProjectionBuffer[12], mtxProjectionBuffer[13], mtxProjectionBuffer[14]); //Camera Move
                    //Store Values
                    xPrevPos = xCurPos;
                    yPrevPos = yCurPos;
                    //Set Flags
                    isInitialPosition = 0;
                    isRefreshed = 0;
                    return 0;
                }
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        case WM_KEYDOWN: // Is A Key Being Held Down?
        {
            key[wParam] = 1; // If So, Mark It As TRUE
            switch LOWORD(wParam)
            {
                case VK_ESCAPE:
                {
                    if (isInitialPosition == 0)
                    {
                        SetView();
                        return 0; // Return To The Message Loop
                    }
                    else
                    {
                        CloseWndProc();
                        return 0; // Return To The Message Loop
                    }
                }
                case VK_SPACE:
                {
                    SetView();
                    return 0; // Return To The Message Loop
                }
                case VK_RETURN: //Enter
                {
                    MessageBox(ghWnd, (LPCWSTR)szManualMsgText, (LPCWSTR)szManualMsgTitle, MB_OK);
                    return 0; // Return To The Message Loop
                }
                case VK_F1:
                {
                    MessageBox(ghWnd, (LPCWSTR)szManualMsgText, (LPCWSTR)szManualMsgTitle, MB_OK);
                    return 0; // Return To The Message Loop
                }
                case VK_TAB:
                {
                    //Turn the Camera Counter-Clockwise 30 Degrees About the Model Z-Axis
                    glMatrixMode(GL_PROJECTION);
                    glRotatef(30.0f, 0.0f, 0.0f, 1.0f); 
                    isRefreshed = 0;
                    return 0; // Return To The Message Loop
                }
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        case WM_KEYUP: // Has A Key Been Released?
        {
            key[wParam] = 0; // If So, Mark It As FALSE
            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            nMouseMode = MOUSE_MODE_CAMERA_ROTATION;
            xPrevPos = GET_X_LPARAM(lParam);
            yPrevPos = GET_Y_LPARAM(lParam);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            nMouseMode = MOUSE_MODE_NO_ACTION;
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            nMouseMode = MOUSE_MODE_CAMERA_ROLL;
            xPrevPos = GET_X_LPARAM(lParam);
            yPrevPos = GET_Y_LPARAM(lParam);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            nMouseMode = MOUSE_MODE_NO_ACTION;
            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            nMouseMode = MOUSE_MODE_CAMERA_PAN;
            xPrevPos = GET_X_LPARAM(lParam);
            yPrevPos = GET_Y_LPARAM(lParam);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            nMouseMode = MOUSE_MODE_NO_ACTION;
            return 0;
        }
        case WM_MBUTTONDBLCLK:
        {
            SetView();
            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            dStep = float(GET_WHEEL_DELTA_WPARAM(wParam) * 8);
            glMatrixMode(GL_PROJECTION);
            glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
            glTranslatef(dStep * mtxProjectionBuffer[2], dStep * mtxProjectionBuffer[6], dStep * mtxProjectionBuffer[10]);
            isRefreshed = 0;
            return 0;
        }
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
                case IDM_ABOUT:
                {
                    DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUTBOX), ghWnd, AboutProc);
                    return 0;
                }
                case IDM_EXIT:
                {
                    CloseWndProc();
                    return 0;
                }
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
            {
                isActive = 1; // Program Is Active
            }
            else
            {
                isActive = 0; // Program Is No Longer Active
            }
            return 0;
        }
        case WM_SIZE:
        {
            ResizeWndProc();
            return 0;
        }
        case WM_CREATE:
        {
            ghWnd = hWnd;
            InitCommonControlsEx(&icce);
            hwndStatusBar = CreateWindowEx(0, STATUSCLASSNAME, (PCTSTR)0, SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, ghWnd, (HMENU)1, ghInst, 0);
            return 0;
        }
        case WM_CLOSE:
        {
            CloseWndProc();
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 23_GLResize.asm
void ResizeWndProc()
{
    int i;
    GetClientRect(ghWnd, &rc);
    RectWidth = rc.right - rc.left;
    RectHeight = rc.bottom - rc.top;
    if (RectHeight > 0)
    {
        RectAspect = (GLdouble)RectWidth / (GLdouble)RectHeight;
    }
    //Main Viewport
    glViewport(0, 0, RectWidth, RectHeight);
    SetView();
    //Status Bar
    SendMessage(hwndStatusBar, WM_SIZE, 0, 0);
    for (i = 0; i < 11; i++) //Divide into 12 Parts
    {
        //Since the Proportions were Empirically Counted for Width = 1024,
        //We must Divide (Proportions*Width) by 1024
        //Or just shift left by 10 bits
        xStatusParts[i] = (xStatusProportions[i] * RectWidth) >> 10;
    }
    SendMessage(hwndStatusBar, SB_SETPARTS, 12, (LPARAM)&xStatusParts); //Divide into 12 Parts
    isRefreshed = 0;
}

// 28_AboutProc.asm
INT_PTR CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
        {
            return 1;
        }
        case WM_COMMAND:
        {
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return 1;
            }
        }
        default:
            return 0;
    }
    return 0;
}

// 29_CloseProc.asm
void CloseWndProc()
{
    if (MessageBox(ghWnd, szMsgCloseText, szMainWndTitle, MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(ghRC);
        ReleaseDC(ghWnd, ghDC);
        DestroyWindow(ghWnd);
    }
}

// 30_DrawGLSceneProc.asm
void DrawGLScene()
{
    CheckKeys();
    if (isRefreshed == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawAxes();
        DrawObject();
        SwapBuffers(ghDC); // Double Buffering
        nFrame += 1;
        StringCbPrintf(pszDest, cbDest, pszFormatTitle, szMainWndTitle, nFrame);
        SetWindowText(ghWnd, pszDest);
        RefreshStatus();
        isRefreshed = 1;
    }
}

// 31_CheckKeys.asm
void CheckKeys()
{
    // Return to Normal Speed
    dStep = LinearSpeed;
    dAngle = AngularSpeed;
    //Boost
    if (key[VK_SHIFT])
    {
        dStep *= LinearBoost;
        dAngle *= AngularBoost;
    }

    glMatrixMode(GL_PROJECTION);

    // Camera Rotation - Keyboard Input
    if (key[0x57]) { //W - Camera Tilt Down
        glRotatef(-dAngle, 1.0f, 0.0f, 0.0f);
        isRefreshed = 0; }
    if (key[0x53]) { //S - Camera Tilt Up
        glRotatef(dAngle, 1.0f, 0.0f, 0.0f);
        isRefreshed = 0; }
    if (key[0x41]) { //A - Camera Turn Counter-Clockwise
        glRotatef(-dAngle, 0.0f, 0.0f, 1.0f);
        isRefreshed = 0; }
    if (key[0x44]) { //D - Camera Turn Clockwise
        glRotatef(dAngle, 0.0f, 0.0f, 1.0f);
        isRefreshed = 0; }
    if (key[0x51]) { //Q - Camera Roll Counter-Clockwise
        glRotatef(dAngle, 0.0f, 1.0f, 0.0f);
        isRefreshed = 0; }
    if (key[0x45]) { //E - Camera Roll Clockwise
        glRotatef(-dAngle, 0.0f, 1.0f, 0.0f);
        isRefreshed = 0; }

    //Camera Move
    if (key[VK_UP]) { //Arrow Up = Camera Move Forward
        glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
        glTranslatef(-dStep * mtxProjectionBuffer[2], -dStep * mtxProjectionBuffer[6], -dStep * mtxProjectionBuffer[10]);
        isRefreshed = 0; } 
    if (key[VK_DOWN]) { //Arrow Down = Camera Move Backward
        glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
        glTranslatef(dStep * mtxProjectionBuffer[2], dStep * mtxProjectionBuffer[6], dStep * mtxProjectionBuffer[10]);
        isRefreshed = 0; } 
    if (key[VK_LEFT]) { //Arrow Left = Camera Move Left
        glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
        glTranslatef(dStep * mtxProjectionBuffer[0], dStep * mtxProjectionBuffer[4], dStep * mtxProjectionBuffer[8]);
        isRefreshed = 0; }
    if (key[VK_RIGHT]) { //Arrow Right = Camera Move Right
        glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
        glTranslatef(-dStep * mtxProjectionBuffer[0], -dStep * mtxProjectionBuffer[4], -dStep * mtxProjectionBuffer[8]);
        isRefreshed = 0; }
    if (key[VK_PRIOR]) { //PageUp = Camera Move Up
        glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
        glTranslatef(-dStep * mtxProjectionBuffer[1], -dStep * mtxProjectionBuffer[5], -dStep * mtxProjectionBuffer[9]);
        isRefreshed = 0; }
    if (key[VK_NEXT]) { //PageDown = Camera Move Down
        glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
        glTranslatef(dStep * mtxProjectionBuffer[1], dStep * mtxProjectionBuffer[5], dStep * mtxProjectionBuffer[9]);
        isRefreshed = 0; }
}

// 36_SetView.asm
void SetView()
{
    //Set Object
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(20.0f, 0.0f, 0.0f, 1.0f); // Model Turn
    //Set Camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, RectAspect, 1.0, 1000000.0);
    glTranslatef(0.0f, 1500.0f, -9000.0f); //Camera Move
    glRotatef(300.0f, 1.0f, 0.0f, 0.0f);  //Camera Tilt
    // Set Flags
    isInitialPosition = 1;
    isRefreshed = 0;
}

// 40_RefreshStatus.asm
void RefreshStatus()
{
    glGetFloatv(GL_PROJECTION_MATRIX, mtxProjectionBuffer);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisXX, 4, mtxProjectionBuffer[0]); //Vector X, Coordinate X
    SendMessage(hwndStatusBar, SB_SETTEXTW, 0, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisYX, 4, mtxProjectionBuffer[1]); //Vector Y, Coordinate X
    SendMessage(hwndStatusBar, SB_SETTEXTW, 1, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisZX, 4, mtxProjectionBuffer[2]); //Vector Z, Coordinate X
    SendMessage(hwndStatusBar, SB_SETTEXTW, 2, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisXY, 4, mtxProjectionBuffer[4]); //Vector X, Coordinate Y
    SendMessage(hwndStatusBar, SB_SETTEXTW, 3, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisYY, 4, mtxProjectionBuffer[5]); //Vector Y, Coordinate Y
    SendMessage(hwndStatusBar, SB_SETTEXTW, 4, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisZY, 4, mtxProjectionBuffer[6]); //Vector Z, Coordinate Y
    SendMessage(hwndStatusBar, SB_SETTEXTW, 5, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisXZ, 4, mtxProjectionBuffer[8]); //Vector X, Coordinate Z
    SendMessage(hwndStatusBar, SB_SETTEXTW, 6, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisYZ, 4, mtxProjectionBuffer[9]); //Vector Y, Coordinate Z
    SendMessage(hwndStatusBar, SB_SETTEXTW, 7, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisZZ, 4, mtxProjectionBuffer[10]); //Vector Z, Coordinate Z
    SendMessage(hwndStatusBar, SB_SETTEXTW, 8, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisXW, 4, mtxProjectionBuffer[12]); //Vector X, Coordinate W
    SendMessage(hwndStatusBar, SB_SETTEXTW, 9, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisYW, 4, mtxProjectionBuffer[13]); //Vector Y, Coordinate W
    SendMessage(hwndStatusBar, SB_SETTEXTW, 10, (LPARAM)pszDest);
    StringCbPrintf(pszDest, cbDest, pszFormatStatus, pszAxisZW, 4, mtxProjectionBuffer[14]); //Vector Z, Coordinate W
    SendMessage(hwndStatusBar, SB_SETTEXTW, 11, (LPARAM)pszDest);
}

// 50_DrawAxesProc.asm
void DrawAxes()
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0); // Red
        glVertex3f(-7000, 0, 0);
        glVertex3f(7000, 0, 0);
        glVertex3f(6900, -50, 0); // Arrow
        glVertex3f(7000, 0, 0);
        glVertex3f(7000, 0, 0);
        glVertex3f(6900, 50, 0);
        glColor3f(0, 1, 0); // Green
        glVertex3f(0, -7000, 0);
        glVertex3f(0, 7000, 0);
        glVertex3f(-50, 6900, 0); // Arrow
        glVertex3f(0, 7000, 0);
        glVertex3f(0, 7000, 0);
        glVertex3f(50, 6900, 0);
        glColor3f(0, 0, 1); // Blue
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1000);
        glVertex3f(-50, 0, 900); // Arrow
        glVertex3f(0, 0, 1000);
        glVertex3f(0, 0, 1000);
        glVertex3f(50, 0, 900);
    glEnd();
}

// 51_DrawObjectProc.asm
void DrawObject()
{
    //Counters
    BYTE x, y, bType;
    //Light On
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    //Scheme G
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(-5875.0f, 5875.0f, 0.0f);
    for (y = 0; y < 48; y++)
    {
        glPushMatrix();
        for (x = 0; x < 48; x++)
        {
            bType = nLayout[x][y];
            switch (bType)
            {
                case 1:
                {
                    //glColor3f(0.65f, 0.65f, 0.65f); //Gray
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matGrayAmbDiff);
                    rbmkDrawSb11();
                    break;
                }
                case 2:
                {
                    //glColor3f(0.15f, 0.75f, 0.15f); //Green
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matGreenAmbDiff);
                    rbmkDrawCap();
                    break;
                }
                case 3:
                {
                    //glColor3f(0.85f, 0.85f, 0.15f); //Yellow
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matYellowAmbDiff);
                    rbmkDrawCap();
                    break;
                }
                case 4:
                {
                    //glColor3f(0.85f, 0.15f, 0.15f); //Red
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matRedAmbDiff);
                    rbmkDrawCap();
                    break;
                }
                case 5:
                {
                    //glColor3f(0.25f, 0.4f, 0.85f); //Blue
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matBlueAmbDiff);
                    rbmkDrawCap();
                    break;
                }
            }
            if (bType != 0)
            {
                rbmkDrawChannel();
                rbmkDrawGraphite();
            }
            glTranslatef(250.0f, 0.0f, 0.0f);
        }
        glPopMatrix();
        glTranslatef(0.0f, -250.0f, 0.0f);
    }
    glPopMatrix();
}

// 52_DrawCapProc.asm
void rbmkDrawSb11()
{
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f); //0 - Top
    glVertex3f(-110, -110, 0);
    glVertex3f(110, -110, 0);
    glVertex3f(110, 110, 0);
    glVertex3f(-110, 110, 0);
    glNormal3f(0.0f, 1.0f, 1.0f); //1 - North
    glVertex3f(120, 120, -10);
    glVertex3f(-120, 120, -10);
    glVertex3f(-110, 110, 0);
    glVertex3f(110, 110, 0);
    glNormal3f(-1.0f, 0.0f, 1.0f); // 2 - West
    glVertex3f(-120, 120, -10);
    glVertex3f(-120, -120, -10);
    glVertex3f(-110, -110, 0);
    glVertex3f(-110, 110, 0);
    glNormal3f(0.0f, -1.0f, 1.0f); //3 - South
    glVertex3f(-120, -120, -10);
    glVertex3f(120, -120, -10);
    glVertex3f(110, -110, 0);
    glVertex3f(-110, -110, 0);
    glNormal3f(1.0f, 0.0f, 1.0f); //4 - East
    glVertex3f(120, -120, -10);
    glVertex3f(120, 120, -10);
    glVertex3f(110, 110, 0);
    glVertex3f(110, -110, 0);
    glEnd();
}

void rbmkDrawCap()
{
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f); //0 - Top
    glVertex3f(-110, -110, 0);
    glVertex3f(110, -110, 0);
    glVertex3f(110, 110, 0);
    glVertex3f(-110, 110, 0);
    glNormal3f(0.0f, 1.0f, 1.0f); //1 - North
    glVertex3f(120, 120, -10);
    glVertex3f(-120, 120, -10);
    glVertex3f(-110, 110, 0);
    glVertex3f(110, 110, 0);
    glNormal3f(-1.0f, 0.0f, 1.0f); // 2 - West
    glVertex3f(-120, 120, -10);
    glVertex3f(-120, -120, -10);
    glVertex3f(-110, -110, 0);
    glVertex3f(-110, 110, 0);
    glNormal3f(0.0f, -1.0f, 1.0f); //3 - South
    glVertex3f(-120, -120, -10);
    glVertex3f(120, -120, -10);
    glVertex3f(110, -110, 0);
    glVertex3f(-110, -110, 0);
    glNormal3f(1.0f, 0.0f, 1.0f); //4 - East
    glVertex3f(120, -120, -10);
    glVertex3f(120, 120, -10);
    glVertex3f(110, 110, 0);
    glVertex3f(110, -110, 0);
    glEnd();
}

void rbmkDrawChannel()
{
    float xyChannel[8]{ -20.0f,-20.0f, 20.0f,-20.0f, 20.0f,20.0f, -20.0f,20.0f };
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -20000.0f);
        glColor3f(0.75f, 0.75f, 0.75f);
        Extrude(4, 19200.0f, xyChannel);
    glPopMatrix();
}

void rbmkDrawGraphite()
{
    float xyGraphite[8]{ -120.0f,-120.0f, 120.0f,-120.0f, 120.0f,120.0f, -120.0f,120.0f };
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -16800.0f);
        glColor3f(0.25f, 0.25f, 0.25f);
        Extrude(4, 8000.0f, xyGraphite);
    glPopMatrix();
}

void Extrude(int nVertices, float htHeight, float pVertices[])
{
    BYTE i;
    glBegin(GL_QUADS);
    for (i = 0; i < (nVertices - 1) * 2; i += 2)
    {
        glVertex3f(pVertices[i], pVertices[i + 1], 0.0f);
        glVertex3f(pVertices[i + 2], pVertices[i + 3], 0.0f);
        glVertex3f(pVertices[i + 2], pVertices[i + 3], htHeight);
        glVertex3f(pVertices[i], pVertices[i + 1], htHeight);
    }
    glVertex3f(pVertices[i], pVertices[i + 1], 0.0f);
    glVertex3f(pVertices[0], pVertices[1], 0.0f);
    glVertex3f(pVertices[0], pVertices[1], htHeight);
    glVertex3f(pVertices[i], pVertices[i + 1], htHeight);
    glEnd();
}

#include "framework.h" // Quotation marks for custom headers
#include "oglTemplate.h"

// Application
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ResizeWndProc();
void CloseWndProc();
INT_PTR CALLBACK AboutProc(HWND, UINT, WPARAM, LPARAM);

// OpenGL
void InitializeGL();
void ReAssign();
void DrawGLScene();
void CheckKeys();
float CheckAngle(float Angle);
float CheckDistance(float Distance);
void CameraMove();
void SetView();
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
int xStatusParts[9] = {0,0,0,0,0,0,0,0,-1}; //9 Indicated Variables
//Proportions Empirically Counted for Width = 1024
int xStatusProportions [8] = {102,204,307,430,552,675,788,901}; //8 Separators for 9 Fields

// Flags
BYTE isActive = 1;
BYTE isFullscreen = 0;
BYTE isRefreshed = 1;
BYTE nMouse = MOUSE_MODE_NO_ACTION;
int nLastError = 0;

// OpenGL
HDC ghDC;
HGLRC ghRC;
DWORD iPixelFormat = 0;
DWORD RectWidth, RectHeight;
float RectAspect;

// Cursor Position
int xScrCenter = 512;
int yScrCenter = 384;
int xPrevPos, xCurPos;
int yPrevPos, yCurPos;

// Model Scale
float GlobalScale = 0.001f;
// Model Angle
float aYZ_Model = 0.0f;
float aXY_Model = 20.0f;
float aXZ_Model = 0.0f;

// Camera Angle
float aYZ_Cam = 300.0f;
float aXY_Cam = 0.0f;
float aXZ_Cam = 0.0f;
// Camera Position
float xCam = 0.0f;
float yCam = 9.0f; //Move the World 9 Meters Forward = Move the Camera 9 Meters Back
float zCam = -4.5f; //Move the World 4.5 Meters Down = Move the Camera 4.5 Meters Up

// Motion
float dStep = 0.1f;
float dAngle = 1.0f;
float LinearSpeed = 0.01f;
float AngularSpeed = 0.1f;
float LinearBoost = 10.0f;
float AngularBoost = 10.0f;
float dTab = 30.0;
float dxMouse, dyMouse;

// Affine Transformations
float aRad = 0.0f;
float cosA = 0.0f;
float sinA = 0.0f;
float dxCam0 = 0.0f;
float dyCam0 = 0.0f;
float dzCam0 = 0.0f;
float dxCam1 = 0.0f;
float dyCam1 = 0.0f;
float dzCam1 = 0.0f;
float dxCam2 = 0.0f;
float dyCam2 = 0.0f;
float dzCam2 = 0.0f;
float dxCam3 = 0.0f;
float dyCam3 = 0.0f;
float dzCam3 = 0.0f;

// Keyboard
BYTE key[128] = { 0 };

// Strings
LPCTSTR szMainWndTitle{ L"C++ OpenGL Environment" };
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

WCHAR pszDest[20]; //arraysize = 20;
size_t cbDest = 40; // arraysize * sizeof(WCHAR);
LPCTSTR pszFormat = TEXT("%s%f");

LPCTSTR psz_xCam = TEXT("xCam = ");
LPCTSTR psz_yCam = TEXT("yCam = ");
LPCTSTR psz_zCam = TEXT("zCam = ");
LPCTSTR psz_aYZ_Model = TEXT("aYZ_Model = ");
LPCTSTR psz_aXY_Model = TEXT("aXY_Model = ");
LPCTSTR psz_aXZ_Model = TEXT("aXZ_Model = ");
LPCTSTR psz_aYZ_Cam = TEXT("aYZ_Cam = ");
LPCTSTR psz_aXY_Cam = TEXT("aXY_Cam = ");
LPCTSTR psz_aXZ_Cam = TEXT("aXZ_Cam = ");



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
    wcx.hIcon = LoadIcon(ghInst, MAKEINTRESOURCE(IDI_OGLTEMPLATE));
    wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcx.lpszMenuName = MAKEINTRESOURCEW(IDC_OGLTEMPLATE);
    wcx.lpszClassName = szMainWndClass;
    wcx.hIconSm = LoadIcon(wcx.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

    /* get the device context (DC) */
    ghDC = GetDC(ghWnd);

    iPixelFormat = ChoosePixelFormat(ghDC, &pfd);
    SetPixelFormat(ghDC, iPixelFormat, &pfd);

    /* create and enable the render context (RC) */
    ghRC = wglCreateContext(ghDC);

    wglMakeCurrent(ghDC, ghRC);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    ReAssign();
}

// 12_ReAssignProc.asm
void ReAssign()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    LinearSpeed = 0.01f;
    AngularSpeed = 0.1f;
    GlobalScale = 0.001f;
    LinearBoost = 10.0f;
    AngularBoost = 10.0f;
    dStep = LinearSpeed;
    dAngle = AngularSpeed;
    aYZ_Model = 0.0f;
    aXY_Model = 20.0f; // Model Yaw Left = World Yaw Left Over World's Center
    aXZ_Model = 0.0f;
    aYZ_Cam = 300.0f; // Camera Pitch Up = World Pitch Down
    aXY_Cam = 0.0f; // Camera Yaw Left = World Yaw Right Over Camera's Center
    aXZ_Cam = 0.0f; // Cameta Roll Left = World Roll Right
    xCam = 0.0f;
    yCam = 9.0f; // Camera Move Back = World Move Forward
    zCam = -4.5f; // Camera Move Up = World Move Down
    isRefreshed = 1; // Set Flag
}

// 20_WndProc.asm
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_MOUSEMOVE:
        {
            switch (nMouse)
            {
                case MOUSE_MODE_CAMERA_ROTATION:
                {
                    xCurPos = GET_X_LPARAM(lParam);
                    yCurPos = GET_Y_LPARAM(lParam);
                    dxMouse = float(xPrevPos - xCurPos) / 20;
                    dyMouse = float(yPrevPos - yCurPos) / 20;
                    aXY_Cam = CheckAngle(aXY_Cam + dxMouse);
                    aYZ_Cam = CheckAngle(aYZ_Cam + dyMouse);
                    xPrevPos = xCurPos;
                    yPrevPos = yCurPos;
                    return 0;
                }
                case MOUSE_MODE_CAMERA_ROLL:
                {
                    xCurPos = GET_X_LPARAM(lParam);
                    yCurPos = GET_Y_LPARAM(lParam);
                    dxMouse = float(xPrevPos - xCurPos) / 20;
                    dyMouse = float(yPrevPos - yCurPos) / 20;
                    aXZ_Cam = CheckAngle(aXZ_Cam + dxMouse);
                    aYZ_Cam = CheckAngle(aYZ_Cam + dyMouse);
                    xPrevPos = xCurPos;
                    yPrevPos = yCurPos;
                    return 0;
                }
                case MOUSE_MODE_CAMERA_PAN:
                {
                    xCurPos = GET_X_LPARAM(lParam);
                    yCurPos = GET_Y_LPARAM(lParam);
                    dxMouse = float(xPrevPos - xCurPos) / 20;
                    dyMouse = float(yPrevPos - yCurPos) / 20;
                    dxCam0 = CheckDistance(-dxMouse);
                    dyCam0 = CheckDistance(dyMouse);
                    dzCam0 = 0;
                    CameraMove();
                    xPrevPos = xCurPos;
                    yPrevPos = yCurPos;
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
                    if (isRefreshed)
                    {
                        CloseWndProc();
                        return 0; // Return To The Message Loop
                    }
                    else
                    {
                        ReAssign();
                        return 0; // Return To The Message Loop
                    }
                }
                case VK_SPACE:
                {
                    ReAssign();
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
                    aXY_Model = CheckAngle(aXY_Model + 30); //Object Turn Counter-Clockwise 30 degrees
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
            nMouse = MOUSE_MODE_CAMERA_ROTATION;
            xPrevPos = GET_X_LPARAM(lParam);
            yPrevPos = GET_Y_LPARAM(lParam);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            nMouse = MOUSE_MODE_NO_ACTION;
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            nMouse = MOUSE_MODE_CAMERA_ROLL;
            xPrevPos = GET_X_LPARAM(lParam);
            yPrevPos = GET_Y_LPARAM(lParam);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            nMouse = MOUSE_MODE_NO_ACTION;
            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            nMouse = MOUSE_MODE_CAMERA_PAN;
            xPrevPos = GET_X_LPARAM(lParam);
            yPrevPos = GET_Y_LPARAM(lParam);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            nMouse = MOUSE_MODE_NO_ACTION;
            return 0;
        }
        case WM_MBUTTONDBLCLK:
        {
            ReAssign();
            return 0;
        }
        case WM_MOUSEWHEEL:
        {
            dxCam0 = 0;
            dyCam0 = 0;
            dzCam0 = float(GET_WHEEL_DELTA_WPARAM(wParam) / 60);
            CameraMove();
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
            SendMessage(hwndStatusBar, SB_SETPARTS, (WPARAM)9, (LPARAM)&xStatusParts);
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
        RectAspect = (float)RectWidth / (float)RectHeight;
    }
    //Main Viewport
    glViewport(0, 0, RectWidth, RectHeight);
    //Status Bar
    SendMessage(hwndStatusBar, WM_SIZE, 0, 0);
    for (i = 0; i < 8; i++)
    {
        //Since the Proportions were Empirically Counted for Width = 1024,
        //We must Divide (Proportions*Width) by 1024
        //Or just shift left by 10 bits
        xStatusParts[i] = (xStatusProportions[i] * RectWidth) >> 10;
    }
    SendMessage(hwndStatusBar, SB_SETPARTS, 9, (LPARAM)&xStatusParts);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CheckKeys();
    RefreshStatus();
    SetView();
    DrawAxes();
    DrawObject();
    SwapBuffers(ghDC); // Swap Buffers (Double Buffering)
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
        dStep = dStep * LinearBoost;
        dAngle = dAngle * AngularBoost;
    }
// Model Rotation - Keyboard Input
    if (key[0x57]) {aYZ_Model = CheckAngle(aYZ_Model - dAngle);} //W
    if (key[0x53]) {aYZ_Model = CheckAngle(aYZ_Model + dAngle);} //S
    if (key[0x41]) {aXY_Model = CheckAngle(aXY_Model + dAngle);} //A - Model Turn Counter-Clockwise
    if (key[0x44]) {aXY_Model = CheckAngle(aXY_Model - dAngle);} //D - Model Turn Clockwise
    if (key[0x51]) {aXZ_Model = CheckAngle(aXZ_Model - dAngle);} //Q
    if (key[0x45]) {aXZ_Model = CheckAngle(aXZ_Model + dAngle);} //E

    //Camera Move Forward and Backward
    if (key[VK_UP])
    {
        dxCam0 = 0;
        dyCam0 = 0;
        dzCam0 = dStep;
        CameraMove();
    }
    if (key[VK_DOWN])
    {
        dxCam0 = 0;
        dyCam0 = 0;
        dzCam0 = -dStep;
        CameraMove();
    }
// Camera Move Left and Right
    if (key[VK_LEFT])
    {
        dxCam0 = dStep;
        dyCam0 = 0;
        dzCam0 = 0;
        CameraMove();
    }
    if (key[VK_RIGHT])
    {
        dxCam0 = -dStep;
        dyCam0 = 0;
        dzCam0 = 0;
        CameraMove();
    }
//Camera Move Up and Down
    if (key[VK_PRIOR]) //PageUp
    {
        dxCam0 = 0;
        dyCam0 = -dStep;
        dzCam0 = 0;
        CameraMove();
    }
    if (key[VK_NEXT]) //PageDown
    {
        dxCam0 = 0;
        dyCam0 = dStep;
        dzCam0 = 0;
        CameraMove();
    }
}

// 32_CheckAngleProc.asm
float CheckAngle(float Angle)
{
    if (Angle > 360)
    {
        return (Angle - 360);
    }
    else if (Angle < 0)
    {
        return (Angle + 360);
    }
    else
    {
        return Angle;
    }
    isRefreshed = 0; //Set Flag
}

// 33_CheckDistanceProc.asm
float CheckDistance(float Distance)
{
    if (Distance > 1000)
    {
        return 1000;
    }
    else if (Distance < -1000)
    {
        return -1000;
    }
    else
    {
        return Distance;
    }
}

// 34_CameraMoveProc.asm
void CameraMove()
{
// RotX - Tilt
    aRad = -aYZ_Cam * PI_OVER_180; // Negative Angle
    cosA = cos(aRad);
    sinA = sin(aRad);
    dxCam1 = dxCam0;
    dyCam1 = dyCam0 * cosA - dzCam0 * sinA;
    dzCam1 = dyCam0 * sinA + dzCam0 * cosA;
// RotZ - Turn
    aRad = -aXY_Cam * PI_OVER_180; // Negative Angle
    cosA = cos(aRad);
    sinA = sin(aRad);
    dxCam2 = dxCam1 * cosA - dyCam1 * sinA;
    dyCam2 = dxCam1 * sinA + dyCam1 * cosA;
    dzCam2 = dzCam1;
// RotY - Roll
    aRad = -aXZ_Cam * PI_OVER_180; // Negative Angle
    cosA = cos(aRad);
    sinA = sin(aRad);
    dxCam3 = dxCam2 * cosA + dzCam2 * sinA;
    dyCam3 = dyCam2;
    dzCam3 = dzCam2 * cosA - dxCam2 * sinA;
// Move
    xCam = CheckDistance(xCam + dxCam3);
    yCam = CheckDistance(yCam + dyCam3);
    zCam = CheckDistance(zCam + dzCam3);
// Set Flag
    isRefreshed = 0;
}

// 35_SetView.asm
void SetView()
{
//Set Object
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(GlobalScale, GlobalScale, GlobalScale); // 1 - Model Scale
    glRotatef(aYZ_Model, 1, 0, 0); // 2 - Model Pitch
    glRotatef(aXY_Model, 0, 0, 1); // 3 - Model Yaw
    glRotatef(aXZ_Model, 0, 1, 0); // 4 - Model Roll
//Set Camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, RectAspect, 0.1, 1000);
    glRotatef(aYZ_Cam, 1.0f, 0.0f, 0.0f); // 1 - Camera Pitch
    glRotatef(aXY_Cam, 0.0f, 0.0f, 1.0f); // 2 - Camera Yaw
    glRotatef(aXZ_Cam, 0.0f, 1.0f, 0.0f); // 3 - Camera Roll
    glTranslatef(xCam, yCam, zCam); // 4 - Camera Move
}

// 40_RefreshStatus.asm
void RefreshStatus()
{
    StringCbPrintf(pszDest, cbDest, pszFormat, psz_xCam, xCam);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 0, (LPARAM)pszDest);

    StringCbPrintf(pszDest, cbDest, pszFormat, psz_yCam, yCam);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 1, (LPARAM)pszDest);

    StringCbPrintf(pszDest, cbDest, pszFormat, psz_zCam, zCam);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 2, (LPARAM)pszDest);

    StringCbPrintf(pszDest, cbDest, pszFormat, psz_aYZ_Model, aYZ_Model);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 3, (LPARAM)pszDest);

    StringCbPrintf(pszDest, cbDest, pszFormat, psz_aXY_Model, aXY_Model);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 4, (LPARAM)pszDest);

    StringCbPrintf(pszDest, cbDest, pszFormat, psz_aXZ_Model, aXZ_Model);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 5, (LPARAM)pszDest);

    StringCbPrintf(pszDest, cbDest, pszFormat, psz_aYZ_Cam, aYZ_Cam);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 6, (LPARAM)pszDest);

    StringCbPrintf(pszDest, cbDest, pszFormat, psz_aXY_Cam, aXY_Cam);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 7, (LPARAM)pszDest);

    StringCbPrintf(pszDest, cbDest, pszFormat, psz_aXZ_Cam, aXZ_Cam);
    SendMessage(hwndStatusBar, SB_SETTEXTW, 8, (LPARAM)pszDest);
}

// 50_DrawAxesProc.asm
void DrawAxes()
{
    glColor3f(1, 0, 0); // Red
    glBegin(GL_LINES);
        glVertex3f(-7000, 0, 0);
        glVertex3f(7000, 0, 0);
        glVertex3f(6900, -50, 0); // Arrow
        glVertex3f(7000, 0, 0);
        glVertex3f(7000, 0, 0);
        glVertex3f(6900, 50, 0);
    glEnd();
    glColor3f(0, 1, 0); // Green
    glBegin(GL_LINES);
        glVertex3f(0, -7000, 0);
        glVertex3f(0, 7000, 0);
        glVertex3f(-50, 6900, 0); // Arrow
        glVertex3f(0, 7000, 0);
        glVertex3f(0, 7000, 0);
        glVertex3f(50, 6900, 0);
    glEnd();
    glColor3f(0, 0, 1); // Blue
    glBegin(GL_LINES);
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
    // Counters
    BYTE x, y, bType; 
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
                    glColor3f(0.65f, 0.65f, 0.65f); //Gray
                    rbmkDrawSb11();
                    break;
                }
                case 2:
                {
                    glColor3f(0.15f, 0.75f, 0.15f); //Green
                    rbmkDrawCap();
                    break;
                }
                case 3:
                {
                    glColor3f(0.85f, 0.85f, 0.15f); //Yellow
                    rbmkDrawCap();
                    break;
                }
                case 4:
                {
                    glColor3f(0.85f, 0.15f, 0.15f); //Red
                    rbmkDrawCap();
                    break;
                }
                case 5:
                {
                    glColor3f(0.25f, 0.4f, 0.85f); //Blue
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
        glVertex3f(-100, -100, 0);
        glVertex3f(100, -100, 0);
        glVertex3f(100, 100, 0);
        glVertex3f(-100, 100, 0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-120, -120, -20);
        glVertex3f(-100, -100, 0);
        glVertex3f(120, -120, -20);
        glVertex3f(100, -100, 0);
        glVertex3f(120, 120, -20);
        glVertex3f(100, 100, 0);
        glVertex3f(-120, 120, -20);
        glVertex3f(-100, 100, 0);
        glVertex3f(-120, -120, -20);
        glVertex3f(-100, -100, 0);
    glEnd();
   glBegin(GL_QUAD_STRIP);
        glVertex3f(-120, -120, -20);
        glVertex3f(-100, -100, -600);
        glVertex3f(120, -120, -20);
        glVertex3f(100, -100, -600);
        glVertex3f(120, 120, -20);
        glVertex3f(100, 100, -600);
        glVertex3f(-120, 120, -20);
        glVertex3f(-100, 100, -600);
        glVertex3f(-120, -120, -20);
        glVertex3f(-100, -100, -600);
    glEnd();
}

void rbmkDrawCap()
{
    float xyzCap[12]{ -120.0f,-120.0f,0.0f, 120.0f,-120.0f,0.0f, 120.0f,120.0f,0.0f, -120.0f,120.0f,0.0f };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &xyzCap); //3 Coordinates per Vertex
    glDrawArrays(GL_QUADS, 0, 4); //Only 4 Points are Defined now
    glDisableClientState(GL_VERTEX_ARRAY);
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


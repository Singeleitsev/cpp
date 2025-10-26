// header.h: включаемый файл для стандартных системных включаемых файлов

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN // Исключите редко используемые компоненты из заголовков Windows

// Файлы заголовков Windows
#include <windows.h>
#include <windowsx.h>

// Файлы заголовков среды выполнения C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <commctrl.h>
//#include <tchar.h>
//#include <wchar.h>
#include <strsafe.h>

// OpenGL
#include <gl/gl.h>
#include <gl/glu.h>

//Math
#define _USE_MATH_DEFINES
#include <cmath>

//pragma comment
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

/********************************************************************
Файл APIShell.cpp
Copyright (c) 1997 Мешков А., Тихомиров Ю.
********************************************************************/

#define _INC_OLE
#include <Windows.h>
#undef _INC_OLE

#include <windowsx.h>
#include "resource.h"

/* Определение номера сообщения */
#define WM_NOTIFYICON (WM_APP+100)
HINSTANCE hInst;
UINT g_uState;


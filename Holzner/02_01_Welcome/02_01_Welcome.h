
// 02_01_Welcome.h: основной файл заголовка для приложения 02_01_Welcome
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CWelcomeApp:
// Сведения о реализации этого класса: 02_01_Welcome.cpp
//

class CWelcomeApp : public CWinAppEx
{
public:
	CWelcomeApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWelcomeApp theApp;

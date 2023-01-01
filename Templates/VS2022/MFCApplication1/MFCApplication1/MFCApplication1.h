// Этот исходный код примеров MFC демонстрирует функционирование пользовательского интерфейса Fluent на основе MFC в Microsoft Office
// ("Fluent UI") и предоставляется исключительно как справочный материал в качестве дополнения к
// справочнику по пакету Microsoft Foundation Classes и связанной электронной документации,
// включенной в программное обеспечение библиотеки MFC C++.
// Условия лицензионного соглашения на копирование, использование или распространение Fluent UI доступны отдельно.
// Для получения дополнительных сведений о нашей программе лицензирования Fluent UI посетите веб-сайт
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// (C) Корпорация Майкрософт (Microsoft Corp.)
// Все права защищены.

// MFCApplication1.h: основной файл заголовка для приложения MFCApplication1
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CBIM23App:
// Сведения о реализации этого класса: MFCApplication1.cpp
//

class CBIM23App : public CWinAppEx
{
public:
	CBIM23App() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	COleTemplateServer m_server;
		// Объект сервера для создания документа
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBIM23App theApp;

// Этот исходный код MFC Samples демонстрирует функционирование пользовательского интерфейса Fluent на основе MFC в Microsoft Office
// ("Fluent UI") и предоставляется исключительно как справочный материал в качестве дополнения к
// справочнику по пакету Microsoft Foundation Classes и связанной электронной документации,
// включенной в программное обеспечение библиотеки MFC C++.  
// Условия лицензионного соглашения на копирование, использование или распространение Fluent UI доступны отдельно.  
// Для получения дополнительных сведений о нашей лицензионной программе Fluent UI посетите веб-узел
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// (C) Корпорация Майкрософт (Microsoft Corp.)
// Все права защищены.

// IpFrame.cpp: реализация класса CInPlaceFrame
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"

#include "IpFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInPlaceFrame

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleDocIPFrameWnd)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleDocIPFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// Создание или уничтожение CInPlaceFrame

CInPlaceFrame::CInPlaceFrame()
{
	EnableActiveAccessibility();
}

CInPlaceFrame::~CInPlaceFrame()
{
}

int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleDocIPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CResizeBar реализует изменение размеров по месту.
	if (!m_wndResizeBar.Create(this))
	{
		TRACE0("Failed to create resize bar\n");
		return -1;      // не удалось создать
	}

	// По умолчанию рекомендуется регистрировать раскрывающиеся конечные объекты, которые не изменяют
	//  окно рамки.  Это позволяет избежать
	//  передачи управления в контейнер, поддерживающий перетаскивание.
	m_dropTarget.Register(this);

	return 0;
}

// OnCreateControlBars вызывается рабочей средой для создания панелей элементов управления в
//  окнах приложения контейнера.  pWndFrame – окно рамки верхнего уровня для
//  контейнера и никогда не принимает значение NULL.  pWndDoc — окно рамки уровня документа
//  и может принимать значение NULL, когда контейнер является приложением SDI. Сервер
//  может размещать панели управления MFC в любом окне.
BOOL CInPlaceFrame::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc)
{
	// Удалите этот код, если вы используете pWndDoc
	UNREFERENCED_PARAMETER(pWndDoc);

	// Задайте владельца окна, чтобы сообщения доставлялись соответствующему приложению
	m_wndToolBar.SetOwner(this);

	if (!COleDocIPFrameWnd::OnCreateControlBars(pWndFrame, pWndDoc))
	{
		return FALSE;
	}

	COleCntrFrameWndEx* pNewFrame = DYNAMIC_DOWNCAST(COleCntrFrameWndEx, m_pMainFrame);
	ASSERT_VALID(pNewFrame);

	// Создайте панель инструментов в окне рамки клиента
	if (!m_wndToolBar.CreateEx(pNewFrame, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MFCApplication1TYPE_SRVR_IP))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}

	// TODO: Удалите эти три строки, если не собираетесь закреплять панель инструментов
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	pNewFrame->EnableDocking(CBRS_ALIGN_ANY);
	pNewFrame->DockPane(&m_wndToolBar);

	// TODO: Удалите этот код, если вам не нужны всплывающие подсказки
	m_wndToolBar.SetControlBarStyle(m_wndToolBar.GetPaneStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	return TRUE;
}

BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: измените класс Window или стили посредством изменения CREATESTRUCT cs

	return COleDocIPFrameWnd::PreCreateWindow(cs);
}


// Диагностика CInPlaceFrame

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
	COleDocIPFrameWnd::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
	COleDocIPFrameWnd::Dump(dc);
}
#endif //_DEBUG


// Команды CInPlaceFrame


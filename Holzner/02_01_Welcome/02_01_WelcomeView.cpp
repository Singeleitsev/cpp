
// 02_01_WelcomeView.cpp: реализация класса CMy0201WelcomeView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "02_01_Welcome.h"
#endif

#include "02_01_WelcomeDoc.h"
#include "02_01_WelcomeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy0201WelcomeView

IMPLEMENT_DYNCREATE(CMy0201WelcomeView, CView)

BEGIN_MESSAGE_MAP(CMy0201WelcomeView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy0201WelcomeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CMy0201WelcomeView

CMy0201WelcomeView::CMy0201WelcomeView() noexcept
{
	// TODO: добавьте код создания

}

CMy0201WelcomeView::~CMy0201WelcomeView()
{
}

BOOL CMy0201WelcomeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CMy0201WelcomeView

void CMy0201WelcomeView::OnDraw(CDC* pDC)
{
	CString szWelcome = CString(_T("Hello World!")); // <-----

	CMy0201WelcomeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
	pDC->TextOut(0, 0, szWelcome);
}


// Печать CMy0201WelcomeView


void CMy0201WelcomeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy0201WelcomeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CMy0201WelcomeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CMy0201WelcomeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}

void CMy0201WelcomeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy0201WelcomeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CMy0201WelcomeView

#ifdef _DEBUG
void CMy0201WelcomeView::AssertValid() const
{
	CView::AssertValid();
}

void CMy0201WelcomeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy0201WelcomeDoc* CMy0201WelcomeView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy0201WelcomeDoc)));
	return (CMy0201WelcomeDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CMy0201WelcomeView

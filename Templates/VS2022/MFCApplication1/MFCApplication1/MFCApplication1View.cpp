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

// MFCApplication1View.cpp: реализация класса CBIM23View
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBIM23View

IMPLEMENT_DYNCREATE(CBIM23View, CView)

BEGIN_MESSAGE_MAP(CBIM23View, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, &CBIM23View::OnCancelEditSrvr)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBIM23View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CBIM23View

CBIM23View::CBIM23View() noexcept
{
	EnableActiveAccessibility();
	// TODO: добавьте код создания

}

CBIM23View::~CBIM23View()
{
}

BOOL CBIM23View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CBIM23View

void CBIM23View::OnDraw(CDC* /*pDC*/)
{
	CBIM23Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: добавьте здесь код отрисовки для собственных данных
}


// Печать CBIM23View


void CBIM23View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBIM23View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CBIM23View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CBIM23View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}

void CBIM23View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBIM23View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// Поддержка сервера OLE

// Следующий обработчик команд предоставляет стандартный клавиатурный
//  интерфейс пользователя для отмены сеанса редактирования по месту.  Здесь
//  сервер (не контейнер) вызывает деактивацию
void CBIM23View::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}


// Диагностика CBIM23View

#ifdef _DEBUG
void CBIM23View::AssertValid() const
{
	CView::AssertValid();
}

void CBIM23View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBIM23Doc* CBIM23View::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBIM23Doc)));
	return (CBIM23Doc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CBIM23View

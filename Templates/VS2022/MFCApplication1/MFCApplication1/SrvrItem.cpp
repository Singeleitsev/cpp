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

// SrvrItem.cpp: реализация класса CBIM23SrvrItem
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"

#include "MFCApplication1Doc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Реализация CBIM23SrvrItem

IMPLEMENT_DYNAMIC(CBIM23SrvrItem, CDocObjectServerItem)

CBIM23SrvrItem::CBIM23SrvrItem(CBIM23Doc* pContainerDoc)
	: CDocObjectServerItem(pContainerDoc, TRUE)
{
	// TODO: добавьте код для одноразового вызова конструктора
	//  (например, добавление дополнительных форматов для буфера обмена в источнике данных элемента)
}

CBIM23SrvrItem::~CBIM23SrvrItem()
{
	// TODO: добавьте код очистки
}

void CBIM23SrvrItem::Serialize(CArchive& ar)
{
	// CBIM23SrvrItem::Serialize вызывается платформой, если
	//  элемент копируется в буфер обмена.  Это может произойти автоматически
	//  посредством обратного вызова OLE OnGetClipboardData.  Для внедренного элемента
	//  по умолчанию рекомендуется просто делегировать в функцию Serialize
	//  документа.  В случае поддержки ссылок возможно потребуется сериализовать
	//  отдельную часть документа.

	if (!IsLinkedItem())
	{
		CBIM23Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
			pDoc->Serialize(ar);
	}
}

BOOL CBIM23SrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// Большинство приложений (например, это) обрабатывают только отрисовку аспектов содержимого
	//  элемента. Для поддержки других аспектов, например
	//  DVASPECT_THUMBNAIL (с помощью переопределения OnDrawEx)
	//  измените реализацию OnGetExtent для поддержки
	//  дополнительных аспектов.

	if (dwDrawAspect != DVASPECT_CONTENT)
		return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CBIM23SrvrItem::OnGetExtent вызывается для получения величины в
	//  единицах HIMETRIC целого элемента.  В реализации по умолчанию
	//  просто возвращается количество единиц.

	// TODO: замените этот случайный размер

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC единиц

	return TRUE;
}

BOOL CBIM23SrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	if (!pDC)
		return FALSE;

	// Удалите этот код, если вы используете rSize
	UNREFERENCED_PARAMETER(rSize);

	// TODO: задайте режим отображения и величину
	//  (Величина обычно совпадает с размером, возвращенным из OnGetExtent)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(3000, 3000);

	// TODO: добавьте код отрисовки.  Кроме того, заполните величину HIMETRIC.
	//  Все рисунки производятся в контексте устройства метафайла (pDC).

	return TRUE;
}


// Диагностика CBIM23SrvrItem

#ifdef _DEBUG
void CBIM23SrvrItem::AssertValid() const
{
	CDocObjectServerItem::AssertValid();
}

void CBIM23SrvrItem::Dump(CDumpContext& dc) const
{
	CDocObjectServerItem::Dump(dc);
}
#endif


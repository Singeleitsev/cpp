
// 02_01_WelcomeDoc.cpp: реализация класса CMy0201WelcomeDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "02_01_Welcome.h"
#endif

#include "02_01_WelcomeDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMy0201WelcomeDoc

IMPLEMENT_DYNCREATE(CMy0201WelcomeDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy0201WelcomeDoc, CDocument)
END_MESSAGE_MAP()


// Создание или уничтожение CMy0201WelcomeDoc

CMy0201WelcomeDoc::CMy0201WelcomeDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CMy0201WelcomeDoc::~CMy0201WelcomeDoc()
{
}

BOOL CMy0201WelcomeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CMy0201WelcomeDoc

void CMy0201WelcomeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMy0201WelcomeDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMy0201WelcomeDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMy0201WelcomeDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CMy0201WelcomeDoc

#ifdef _DEBUG
void CMy0201WelcomeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy0201WelcomeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CMy0201WelcomeDoc

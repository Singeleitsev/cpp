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

// MFCApplication1Doc.cpp: реализация класса CBIM23Doc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "SrvrItem.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBIM23Doc

IMPLEMENT_DYNCREATE(CBIM23Doc, COleServerDoc)

BEGIN_MESSAGE_MAP(CBIM23Doc, COleServerDoc)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CBIM23Doc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CBIM23Doc::OnUpdateFileSendMail)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBIM23Doc, COleServerDoc)
END_DISPATCH_MAP()

// Примечание. Мы добавили поддержку для IID_IMFCApplication1, чтобы обеспечить безопасную с точки зрения типов привязку
//  из VBA.  Этот IID должен соответствовать GUID, связанному с
//  disp-интерфейсом в файле .IDL.

// {29cd6450-a91e-4ced-a064-9a2664db7d86}
static const IID IID_IMFCApplication1 =
{0x29cd6450,0xa91e,0x4ced,{0xa0,0x64,0x9a,0x26,0x64,0xdb,0x7d,0x86}};

BEGIN_INTERFACE_MAP(CBIM23Doc, COleServerDoc)
	INTERFACE_PART(CBIM23Doc, IID_IMFCApplication1, Dispatch)
END_INTERFACE_MAP()


// Создание или уничтожение CBIM23Doc

CBIM23Doc::CBIM23Doc() noexcept
{
	// Используйте объединенные файлы OLE
	EnableCompoundFile();

	// TODO: добавьте код для одноразового вызова конструктора

	EnableAutomation();

	AfxOleLockApp();
}

CBIM23Doc::~CBIM23Doc()
{
	AfxOleUnlockApp();
}

BOOL CBIM23Doc::OnNewDocument()
{
	if (!COleServerDoc::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}


// Реализация сервера CBIM23Doc

COleServerItem* CBIM23Doc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem вызывается рабочей средой для получения COleServerItem,
	//  который связан с этим документом.  Вызывается при необходимости.

	CBIM23SrvrItem* pItem = new CBIM23SrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}


// Реализация сервера активных документов CBIM23Doc

CDocObjectServer *CBIM23Doc::GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite)
{
	return new CDocObjectServer(this, pDocSite);
}




// Сериализация CBIM23Doc

void CBIM23Doc::Serialize(CArchive& ar)
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
void CBIM23Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CBIM23Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CBIM23Doc::SetSearchContent(const CString& value)
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

// Диагностика CBIM23Doc

#ifdef _DEBUG
void CBIM23Doc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CBIM23Doc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG


// Команды CBIM23Doc

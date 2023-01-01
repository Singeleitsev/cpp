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

// MFCApplication1Doc.h: интерфейс класса CBIM23Doc 
//


#pragma once


class CBIM23SrvrItem;

class CBIM23Doc : public COleServerDoc
{
protected: // создать только из сериализации
	CBIM23Doc() noexcept;
	DECLARE_DYNCREATE(CBIM23Doc)

// Атрибуты
public:
	CBIM23SrvrItem* GetEmbeddedItem()
		{ return reinterpret_cast<CBIM23SrvrItem*>(COleServerDoc::GetEmbeddedItem()); }

// Операции
public:

// Переопределение
protected:
	virtual COleServerItem* OnGetEmbeddedItem();
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CBIM23Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

	// Автоматически созданные функции диспетчерской карты OLE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

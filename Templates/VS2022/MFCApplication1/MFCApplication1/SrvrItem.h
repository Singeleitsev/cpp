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

// SrvrItem.h: интерфейс класса CBIM23SrvrItem
//

#pragma once

class CBIM23SrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CBIM23SrvrItem)

// Конструкторы
public:
	CBIM23SrvrItem(CBIM23Doc* pContainerDoc);

// Атрибуты
	CBIM23Doc* GetDocument() const
		{ return reinterpret_cast<CBIM23Doc*>(CDocObjectServerItem::GetDocument()); }

// Переопределение
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);

// Реализация
public:
	~CBIM23SrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // перезапись для входа/выхода документа
};


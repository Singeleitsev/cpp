
// 02_01_WelcomeView.h: интерфейс класса CMy0201WelcomeView
//

#pragma once


class CMy0201WelcomeView : public CView
{
protected: // создать только из сериализации
	CMy0201WelcomeView() noexcept;
	DECLARE_DYNCREATE(CMy0201WelcomeView)

// Атрибуты
public:
	CMy0201WelcomeDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CMy0201WelcomeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в 02_01_WelcomeView.cpp
inline CMy0201WelcomeDoc* CMy0201WelcomeView::GetDocument() const
   { return reinterpret_cast<CMy0201WelcomeDoc*>(m_pDocument); }
#endif


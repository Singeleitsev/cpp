// PreviewHandler.h : объявление заголовочного файла предварительного просмотра

#pragma once
#include "resource.h"       // основные символы

#include <atlhandler.h>
#include <atlhandlerimpl.h>
#include <afxext.h>
#include "..\MFCApplication1\MFCApplication1Doc.h"
#include "..\MFCApplication1\MFCApplication1View.h"
#include "MFCApplication1Handlers_i.h"

using namespace ATL;

// CPreviewHandler

class ATL_NO_VTABLE CPreviewHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPreviewHandler, &CLSID_Preview>,
	public CPreviewHandlerImpl <CPreviewHandler>
{
public:
	CPreviewHandler()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PREVIEW_HANDLER)
DECLARE_NOT_AGGREGATABLE(CPreviewHandler)

BEGIN_COM_MAP(CPreviewHandler)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IOleWindow)
	COM_INTERFACE_ENTRY(IInitializeWithStream)
	COM_INTERFACE_ENTRY(IPreviewHandler)
	COM_INTERFACE_ENTRY(IPreviewHandlerVisuals)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		CPreviewHandlerImpl<CPreviewHandler>::FinalRelease();
	}

protected:
	virtual IPreviewCtrl* CreatePreviewControl()
	{
		// создайте собственный элемент управления предварительным просмотром
		CMFCPreviewCtrlImpl *pPreviewCtrl = nullptr;
		ATLTRY(pPreviewCtrl = new CMFCPreviewCtrlImpl());
		return pPreviewCtrl;
	}

	DECLARE_DOCUMENT(CBIM23Doc)

public:
	virtual HRESULT InitializeDocumentPreview(HWND hWndParent, RECT* prc)
	{
		m_pPreviewCtrl = CreatePreviewControl();
		CMFCPreviewCtrlImpl* pCtrl = DYNAMIC_DOWNCAST(CMFCPreviewCtrlImpl, (CObject*) m_pPreviewCtrl);
		if (pCtrl == nullptr)
		{
			ATLTRACE2(atlTraceGeneral, 4, L"InitializeDocumentPreview: указатель на элемент управления предварительным просмотром равен NULL.\n");
			return E_POINTER;
		}

		ASSERT_VALID(pCtrl);

		CCreateContext ctx;
		ctx.m_pNewViewClass = RUNTIME_CLASS(CBIM23View);

		m_pDocument = CreateDocument();

		if (m_pDocument == nullptr)
		{
			ATLTRACE2(atlTraceGeneral, 4, L"InitializeDocumentPreview: указатель на документ равен NULL.\n");
			return E_POINTER;
		}

		m_pDocument->AddRef();
		ctx.m_pCurrentDoc = DYNAMIC_DOWNCAST(CBIM23Doc, (CObject*) m_pDocument->GetContainer());

		if (!pCtrl->Create(hWndParent, prc, &ctx))
		{
			ATLTRACE2(atlTraceGeneral, 4, L"InitializeDocumentPreview: не удалось создать элемент управления предварительным просмотром. Код ошибки: %d\n", GetLastError());
			return E_FAIL;
		}
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(Preview), CPreviewHandler)

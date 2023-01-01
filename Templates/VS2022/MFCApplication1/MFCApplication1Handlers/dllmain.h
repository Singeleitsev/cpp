// dllmain.h: объявление класса модуля.

class CMFCApplication1HandlersModule : public ATL::CAtlDllModuleT<CMFCApplication1HandlersModule>
{
public :
	DECLARE_LIBID(LIBID_MFCApplication1HandlersLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MFCAPPLICATION1HANDLERS, "{783008a0-d25b-4b8d-84be-2a663198b0ca}")
};

extern class CMFCApplication1HandlersModule _AtlModule;

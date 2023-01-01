// оболочка для dlldata.c

#define REGISTER_PROXY_DLL //DllRegisterServer и т. п.

#define USE_STUBLESS_PROXY	//определено только с ключом MIDL /Oicf

#pragma comment(lib, "rpcns4.lib")
#pragma comment(lib, "rpcrt4.lib")

#define ENTRY_PREFIX	Prx

#include "dlldata.c"
#include "MFCApplication1Handlers_p.c"



#ifndef __CAAFInProcServer_h__
#define __CAAFInProcServer_h__


#ifndef __CAAFServer_h__
#include "CAAFServer.h"
#endif

#ifndef __unknwn_h__
#include <unknwn.h>	
#endif

#ifndef __CAAFClassFactory_h__
#include "CAAFClassFactory.h"
#endif

typedef struct tagAAFComObjectInfo
{
	const CLSID* pCLSID;
	LPCOLESTR pClassName;
	AAFCreateComObjectProc pfnCreate;;
} AAFComObjectInfo_t;



#define AAF_BEGIN_OBJECT_MAP(x) static AAFComObjectInfo_t x[] = {
#define AAF_END_OBJECT_MAP()   { NULL, NULL, NULL } };
#define AAF_OBJECT_ENTRY(class) { &CLSID_##class, OLESTR(#class), &C##class##::COMCreate },


class CAAFInProcServer : 
	public CAAFServer
{
public:
	CAAFInProcServer();
	~CAAFInProcServer();

	HRESULT Init( AAFComObjectInfo_t *, HINSTANCE );
	HRESULT Term();

	HRESULT GetClassObject( REFCLSID rclsid, REFIID riid, void ** ppv );
	HRESULT CanUnloadNow( );
	HRESULT RegisterServer( BOOL bRegTypeLib = FALSE );
	HRESULT UnregisterServer( );


protected:
	AAFComObjectInfo_t *_pObjectInfo;
	HINSTANCE _hInstance;
};


#endif // __CAAFInProcServer_h__


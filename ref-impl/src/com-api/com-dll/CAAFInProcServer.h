

#ifndef __CAAFInProcServer_h__
#define __CAAFInProcServer_h__


#ifndef __CAAFServer_h__
#include "CAAFServer.h"
#endif

#ifndef __AAFCOMPlatform_h__
#include "AAFCOMPlatform.h"
#endif

#ifndef __CAAFClassFactory_h__
#include "CAAFClassFactory.h"
#endif

typedef struct tagAAFComObjectInfo
{
	const CLSID* pCLSID;
	LPCOLESTR pClassName;
	AAFCreateComObjectProc pfnCreate;
	bool bRegisterClass;
} AAFComObjectInfo_t;



#define AAF_BEGIN_OBJECT_MAP(x) static AAFComObjectInfo_t x[] = {
#define AAF_LAST_ENTRY() { NULL, NULL, NULL, false }
#define AAF_END_OBJECT_MAP()  AAF_LAST_ENTRY() };
#define AAF_OBJECT_ENTRYX(class,reg) { &CLSID_##class, OLESTR(#class), &C##class##::COMCreate, reg },

// Define standard entries do not register the class.
#define AAF_OBJECT_ENTRY(class) AAF_OBJECT_ENTRYX(class,false)

// Plugins/Class extensions are external and may have a registry component...
#define AAF_PLUGIN_OBJECT_ENTRY(class) AAF_OBJECT_ENTRYX(class,true)



class CAAFInProcServer : 
	public CAAFServer
{
public:
	CAAFInProcServer();
	~CAAFInProcServer();

	HRESULT Init( AAFComObjectInfo_t *, HINSTANCE );
	HRESULT Term();
  
	//
	// Normal COM In Proc Server entrypoints.
	//
	HRESULT GetClassObject( REFCLSID rclsid, REFIID riid, void ** ppv );
	HRESULT CanUnloadNow( );
	HRESULT RegisterServer( BOOL bRegTypeLib = FALSE );
	HRESULT UnregisterServer( );

	//
	// Calls that need to be available for platforms that do 
	// not support COM or Registry.
	//

	// Return the number of coclasses exported from this dll.
	ULONG GetClassCount( );

	// Get the nth implementation coclass id.
	HRESULT GetClassObjectID(ULONG index, CLSID *pClassID);


protected:
	long GetRegisterIndex(long index);

protected:
	AAFComObjectInfo_t *_pObjectInfo;
	HINSTANCE _hInstance;
};


#endif // __CAAFInProcServer_h__


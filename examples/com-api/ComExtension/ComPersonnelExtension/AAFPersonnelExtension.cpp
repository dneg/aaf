//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

////////////////////////////////////////////////////////////////////////////////
// @doc
//
// @module AAFPersonnelExtension.cpp - AAF COM InProc Server DLL |
//
// Implementation of DLL Exports
// 
// @end
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __CAAFServer_h__
#include "CAAFServer.h"
#endif

#ifndef __CAAFClassFactory_h__
#include "CAAFClassFactory.h"
#endif

#ifndef __CAAFPersonnelResource_h__
#include "CAAFPersonnelResource.h"
#endif
#ifndef __CAAFAdminMob_h__
#include "CAAFAdminMob.h"
#endif

#include <string.h>

// Structure to define 
typedef struct tagAAFPluginObjectInfo_t
{
	const CLSID* pCLSID;
	const char * pClassName; // for debugging.
	AAFCreateComObjectProc pfnCreate;
} AAFPluginObjectInfo_t;


#define AAF_BEGIN_PLUGIN_MAP(x) static AAFPluginObjectInfo_t x[] = {
#define AAF_LAST_ENTRY() { NULL, NULL, NULL }
#define AAF_END_PLUGIN_MAP()  AAF_LAST_ENTRY() };
#define AAF_PLUGIN_ENTRY(class) { &CLSID_##class, #class, &C##class::COMCreate },


class CAAFPluginServer : 
	public CAAFServer
{
public:
	CAAFPluginServer();
	~CAAFPluginServer();

	HRESULT Init(HINSTANCE );
	HRESULT Term();
  
	//
	// Normal COM In Proc Server entrypoints that we support
	//
	HRESULT GetClassObject( REFCLSID rclsid, REFIID riid, void ** ppv );
	HRESULT CanUnloadNow( );

	//
	// Calls that need to be available for platforms that do 
	// not support COM or Registry.
	//

	// Return the number of coclasses exported from this dll.
	ULONG GetClassCount( );

	// Get the nth implementation coclass id.
	HRESULT GetClassObjectID(ULONG index, CLSID *pClassID);

private:
	HINSTANCE _hInstance;
};



//
// Include all supported plugins in the following table:
//
AAF_BEGIN_PLUGIN_MAP(AAFPluginObjectMap)
	AAF_PLUGIN_ENTRY(AAFPersonnelResource)
	AAF_PLUGIN_ENTRY(AAFAdminMob)
AAF_END_PLUGIN_MAP()


CAAFPluginServer g_AAFPluginServer;
CAAFServer* g_pAAFServer = &g_AAFPluginServer;



//
// Initialize the AUID's.
//
#define INIT_AUID
#include "AAFStoredObjectIDs.h"
#undef INIT_AUID

//
// Define plugin constants.
//
#include "AAFPlugin_i.c"

//
// Define Personnel Extension constants.
//
#include "AAFPersonnelExtension_i.c"


#if defined( OS_MACOS )

// Make sure we have defined IID_IUnknown and IID_IClassFactory.
#include <initguid.h>
#include <coguid.h>	


// Define struc
typedef struct
{
	CFragInitBlock		InitBlock;	// CFM init Block
	Boolean				Inited;		// True if we've been inited
	short				ResRefNum;	// ResRefNum of the dll
	UInt32				DLLVersion; // holds DLL's version number from resource
}
dlldata;

static dlldata	DllData;




enum 
{
	uppDllInitRoutineProcInfo = kPascalStackBased
		 | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CFragInitBlockPtr)))
};

ProcInfoType __procinfo = uppDllInitRoutineProcInfo;


extern "C" 
{
OSErr pascal __initialize(CFragInitBlockPtr initBlkPtr);
OSErr pascal DllInitializationRoutine(CFragInitBlockPtr initBlkPtr);
OSErr pascal DllExecutionRoutine();
void pascal __terminate();
void pascal DllTerminationRoutine();
}



#pragma export on
#endif // #if defined( OS_MACOS )



#if defined( OS_WINDOWS )
// Include the entry point for the windows dll.
/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{	
		// Initialize the inproc server object.
		g_AAFPluginServer.Init(hInstance);

		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		g_AAFPluginServer.Term();
	}
	return TRUE;    // ok
}

#endif  // OS_WINDOWS


/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return g_AAFPluginServer.CanUnloadNow();
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return g_AAFPluginServer.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// AAF Plugins do not use the COM registry.
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	// AAF Plugins do not use the COM registry.
	return S_OK;
}



//
// Calls that need to be available for platforms that do 
// not support COM or Registry.
//

// Return the number of coclasses exported from this dll.
STDAPI_(ULONG) AAFGetClassCount(void)
{
	return g_AAFPluginServer.GetClassCount();
}

// Get the nth implementation coclass id.
STDAPI AAFGetClassObjectID(ULONG index, CLSID *pClassID)
{
	return g_AAFPluginServer.GetClassObjectID(index, pClassID);
}



#if defined( OS_MACOS )
//
//  DllGetVersion
//
//  gets the controls version
//

STDAPI
DllGetVersion(UInt32* pVersion)
{
	Handle	hResource = NULL;
	HRESULT	Error = E_FAIL;
	
	if (!DllData.Inited && DllData.InitBlock.fragLocator.where == kDataForkCFragLocator)
	{
		short	InResFile = ::CurResFile();
		short	ResRefNum;

		//	open up the resource fork of the dll and save it off
		ResRefNum = ::FSpOpenResFile(DllData.InitBlock.fragLocator.u.onDisk.fileSpec, fsRdPerm);

		hResource = Get1Resource('vers', 1);
		
		if (hResource && GetHandleSize(hResource) > sizeof(UInt32))
		{
			//  Extract the version number from the resource, stripping off
			//  the release stage byte.
			DllData.DLLVersion = *pVersion = (*((LPDWORD)(*hResource))) & 0xFFFF00FF;
			Error = S_OK;
		}
		if(hResource)
			ReleaseResource(hResource);
		
		::UseResFile(InResFile);
		CloseResFile(ResRefNum);
	}
	else
		*pVersion = DllData.DLLVersion;

	return Error;
}

//
//  DllInitializationRoutine
//
//  Initialization routine called by COM
//

OSErr pascal
DllInitializationRoutine(CFragInitBlockPtr initBlkPtr)
{
	DllData.InitBlock = *initBlkPtr;
	DllData.Inited = false;
	DllData.ResRefNum = -1;


	__initialize(initBlkPtr);

	// Initialize the inproc server object with a copy of the contents of the initBlkPtr.
	g_AAFPluginServer.Init((HINSTANCE)&DllData.InitBlock);

	if (!DllData.Inited && DllData.InitBlock.fragLocator.where == kDataForkCFragLocator)
	{
		short	InResFile = ::CurResFile();
		Handle	hResource = NULL;

		DllData.Inited = true;
		//	open up the resource fork of the dll and save it off
		DllData.ResRefNum = ::FSpOpenResFile(DllData.InitBlock.fragLocator.u.onDisk.fileSpec, fsRdPerm);

		hResource = Get1Resource('vers', 1);
		
		if (hResource && GetHandleSize(hResource) > sizeof(UInt32))
		{
			//  Extract the version number from the resource, stripping off
			//  the release stage byte.
			DllData.DLLVersion = (*((LPDWORD)(*hResource))) & 0xFFFF00FF;
		}
		if (hResource)
			ReleaseResource(hResource);

		::UseResFile(InResFile);
	}



    return noErr;
}


//
//  DllExecutionRoutine
//
//  Execution routine called by COM
//

OSErr pascal
DllExecutionRoutine()
{
	//	this routine isn't getting called by anybody
    return noErr;
}


//
//  DllTerminationRoutine
//
//  Termination routine called by COM
//

void pascal
DllTerminationRoutine()
{
  // Terminate the inproc server object.
	g_AAFPluginServer.Term();
	g_pAAFServer	= NULL;

	if (DllData.Inited)
	{
		if (DllData.ResRefNum != -1)
		{
			CloseResFile(DllData.ResRefNum);
			DllData.ResRefNum = -1;
		}
		
		DllData.Inited = false;
	}
	
	__terminate();
}


#pragma export off
#endif // #if defined(OS_MACOS)



CAAFPluginServer::CAAFPluginServer() :
  CAAFServer(),
  _hInstance(NULL)
{
}

CAAFPluginServer::~CAAFPluginServer()
{
}


HRESULT CAAFPluginServer::Init
(
  HINSTANCE hInstance
)
{
  _hInstance = hInstance;
 
  return S_OK;
}

HRESULT CAAFPluginServer::Term()
{
  return S_OK;
}



HRESULT CAAFPluginServer::GetClassObject
( 
  REFCLSID rclsid,
  REFIID riid,
  void ** ppv )
{
  HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
  if (NULL == ppv)
    return E_INVALIDARG;
  *ppv = NULL;

  // Search the object table for the given class id.
  // Lookup the class id in the com object table.
  AAFPluginObjectInfo_t *pResult = NULL;
  for (aafUInt32 i = 0; i < GetClassCount(); ++i)
  {
    if (!memcmp(&rclsid,AAFPluginObjectMap[i].pCLSID,sizeof(rclsid)))
    {
      pResult = &AAFPluginObjectMap[i];
      break;
    }
  }
  
  if (NULL == pResult)
    return hr;

  // We found the requested class id so attempt to create a class factory.
  CAAFClassFactory *pAAFClassFactory = new CAAFClassFactory(pResult->pfnCreate);
  if (NULL == pAAFClassFactory)
    return E_OUTOFMEMORY;

  // We now have a local reference.
  pAAFClassFactory->AddRef();

  // See of the factory supports the requested interface. If this succeeds
  // then the reference count will be two. If it fails the reference
  // count will be one and the following release will delete the factory
  // object.
  hr = pAAFClassFactory->QueryInterface(riid, ppv);

  // We are through with this object.
  pAAFClassFactory->Release();

  return hr;
}

HRESULT CAAFPluginServer::CanUnloadNow( )
{
  // If there are any outstanding locks or active objects then we cannot 
  // unload the server.
  if (0 < GetLockCount() || 0 < GetActiveObjectCount()) 
    return S_FALSE;
  else
    return S_OK;
}


// Return the number of coclasses exported from this dll.
ULONG CAAFPluginServer::GetClassCount( )
{	
	// Just return our global count computed by the Init() method.
	return ((sizeof(AAFPluginObjectMap) / sizeof(AAFPluginObjectInfo_t)) - 1);
}

// Get the nth implementation coclass id.
HRESULT CAAFPluginServer::GetClassObjectID(ULONG index, CLSID *pClassID)
{
	// Validate pre-conditions and parameters.
  if (0 == GetClassCount())
		return CLASS_E_CLASSNOTAVAILABLE;
	if (index >= GetClassCount() || NULL == pClassID)
		return E_INVALIDARG;

	// Lookup the corresponding object info pointer from our table.
	memcpy(pClassID, AAFPluginObjectMap[index].pCLSID, sizeof(CLSID));
	
	return S_OK;
}

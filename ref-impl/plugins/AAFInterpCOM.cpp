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
// @module AAFCOM.CPP - AAF COM InProc Server DLL |
//
// Implementation of DLL Exports
// 
// @end
//
////////////////////////////////////////////////////////////////////////////////



// WIN32 Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f aafcomps.mk in the project directory.


#include "AAFTypes.h"
#include "CAAFInProcServer.h"

static CAAFInProcServer g_AAFInProcServer;
CAAFServer* g_pAAFServer = &g_AAFInProcServer;

// Include the table the associates all of the CLSID's with class names and factory methods.
#include "AAFInterpObjectTable_i.cpp"

// Include the definitions for all of the interface IID's.
#include "AAFPlugin_i.c"


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

#if defined( OS_UNIX )

class InterpCOMInitialize {
public:
	InterpCOMInitialize();
	~InterpCOMInitialize();
};

InterpCOMInitialize::InterpCOMInitialize()
{
	// Initialize the inproc server object.
	// tbd: calling this function causes all module tests to fail (why?)
	g_AAFInProcServer.Init(AAFPluginObjectMap,(HINSTANCE)"libaafintp.so");
}

InterpCOMInitialize::~InterpCOMInitialize()
{}

InterpCOMInitialize init;

#endif  // OS_UNIX

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
		g_AAFInProcServer.Init(AAFPluginObjectMap, hInstance);

		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		g_AAFInProcServer.Term();
	}
	return TRUE;    // ok
}

#endif  // OS_WINDOWS


/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return g_AAFInProcServer.CanUnloadNow();
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return g_AAFInProcServer.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers objects, typelib and all interfaces in typelib
	return g_AAFInProcServer.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	// Unregisters all objects.
	return g_AAFInProcServer.UnregisterServer();
}



//
// Calls that need to be available for platforms that do 
// not support COM or Registry.
//

// Return the number of coclasses exported from this dll.
STDAPI_(ULONG) AAFGetClassCount(void)
{
	return g_AAFInProcServer.GetClassCount();
}

// Get the nth implementation coclass id.
STDAPI AAFGetClassObjectID(ULONG index, CLSID *pClassID)
{
	return g_AAFInProcServer.GetClassObjectID(index, pClassID);
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
	g_AAFInProcServer.Init(AAFPluginObjectMap, (HINSTANCE)&DllData.InitBlock);

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
	g_AAFInProcServer.Term();
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
#endif // #if defined( OS_MACOS )


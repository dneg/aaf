////////////////////////////////////////////////////////////////////////////////
// @doc
//
// @module AAFCOM.CPP - AAF COM InProc Server DLL |
//
// Implementation of DLL Exports
// 
// @end
//
// Tom Ransdell, Avid AAF Development Team
//
////////////////////////////////////////////////////////////////////////////////

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Prototypes that must be implemented but NOT exported.
extern "C" const char * AAFGetLibraryDirectory();
extern "C" const char * AAFGetLibraryPath();
extern "C" const char * AAFGetLibrarySharedDirectoryName();
extern "C" const char * AAFGetLibraryPluginPrefix();

#include "AAFTypes.h"
#include "CAAFInProcServer.h"
#include "ImplAAFContext.h"

static CAAFInProcServer g_AAFInProcServer;
CAAFServer* g_pAAFServer = &g_AAFInProcServer;


// Include the table the associates all of the CLSID's with class names and factory methods.
#include "AAFObjectTable_i.cpp"

// Include the definitions for all of the interface IID's.
#include "AAF_i.c"
#include "AAFPrivate_i.c"

#include <string.h>

#if defined( OS_UNIX )

class Initialize {
public:
	Initialize();
	~Initialize();
};

Initialize::Initialize()
{
	// Initialize the inproc server object.
	g_AAFInProcServer.Init(AAFObjectMap, 0);
    ImplAAFContext::GetInstance();
}

Initialize::~Initialize()
{
	g_AAFInProcServer.Term();
    ImplAAFContext::DeleteInstance();
}

Initialize init;

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
		g_AAFInProcServer.Init(AAFObjectMap, hInstance);

    // Attempt to intialize the global AAF context singleton.
    // This is a thread safe because this initialization
    // is only called when the dll is first loaded by the 
    // main thread of the current process. 
    // NOTE:This object is not reference counted!
    if (NULL == ImplAAFContext::GetInstance ())
    {
      // Return the most liked error code.
      return E_OUTOFMEMORY;
    }

    // We don't need to be notified everytime a thread is created
    // or destroyed.
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		g_AAFInProcServer.Term();
		ImplAAFContext::DeleteInstance();
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
// Platform dependent file system information needed by the reference
// implementation to load AAF plugins "relative" to this shared library/dll.
//
const char * AAFGetLibraryDirectory()
{
	return g_AAFInProcServer.GetServerDirectory();
}

const char * AAFGetLibraryPath()
{
	return g_AAFInProcServer.GetServerPath();
}

// For DR3 just use a fixed, hard-coded string. For RC1 we need to store
// this string and possibly the plugin prefix in external resources for
// each platform. transdel-2000-JUL-27
const char * AAFGetLibrarySharedDirectoryName()
{
    static char g_SharedDirectoryName[260];

    strcpy(g_SharedDirectoryName, "aafext");
    
    return g_SharedDirectoryName;
}


const char * AAFGetLibraryPluginPrefix()
{
    static char g_PluginPrefix[4];

    strcpy(g_PluginPrefix, "aaf");
    
    return g_PluginPrefix;
}

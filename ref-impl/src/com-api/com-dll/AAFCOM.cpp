////////////////////////////////////////////////////////////////////////////////
// @doc
//
// @module AAFCOM.CPP - AAF COM InProc Server DLL |
//
// Implementation of DLL Exports
// 
// @end
//
// Copyright (c)1998 Avid Technologies, Inc. All Rights Reserved
//
// Tom Ransdell, Avid AAF Development Team
//
////////////////////////////////////////////////////////////////////////////////


// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f aafcomps.mk in the project directory.


#include "CAAFInProcServer.h"

CAAFInProcServer g_AAFInProcServer;
CAAFServer* g_pAAFServer = &g_AAFInProcServer;

#ifdef AAF_MODULE_TEST
// Include the module test object in the object table.
#ifndef __CAAFModuleTest_h__
#include "CAAFModuleTest.h"
#endif
#endif // AAF_MODULE_TEST

// Include the table the associates all of the CLSID's with class names and factory methods.
#include "AAFObjectTable_i.cpp"


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{	
		// Initialize the inproc server object.
		g_AAFInProcServer.Init(AAFObjectMap, hInstance);

		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		g_AAFInProcServer.Term();
	}
	return TRUE;    // ok
}

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



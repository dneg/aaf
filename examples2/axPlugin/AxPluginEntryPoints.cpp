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
//=---------------------------------------------------------------------=

#include "AxPlugin.h"

#include "AAFCOMPlatformTypes.h"

// This file implements the four basic entry points required by the AAF
// SDK.  The SDK will look for these four symbols when it loads the
// library:
//
//    DllCanUnloadNow
//    DllGetClassObject
//    AAFGetClassCount
//    AAFGetClassObjectID
//
// Platform independent implementations of these four routines are
// provded here.
//
// If platform specific library (.dll or .so) entry points must be
// added, the code should be placed in the appropriate
// AAFPluginPLATFORM.cpp file.  Where, of course, PLATFORM is: Win,
// Unix, or Mac.


// Return S_OK if it is safe to unload the library.

STDAPI DllCanUnloadNow(void)
{
	if ( AxPlugin::CanUnloadNow() ) {
		return S_OK;
	}
	else {
		return S_FALSE;
	}
}

// Return the number of components implemented by this library.

STDAPI AAFGetClassCount(void)
{
	return AxPlugin::GetClassCount();
}


// Get the class id of the "i'th" component implemented by this library.

STDAPI AAFGetClassObjectID(ULONG index, CLSID *pClassID)
{
	return AxPlugin::GetClassObjectID( index, pClassID );
}

// Create an instance of the class factory for the component
// identified by "rcslid".

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return AxPlugin::GetClassObject( rclsid, riid, ppv );
}

#ifndef __AxPlugin_h__
#define __AxPlugin_h__

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

#include "AAFCOMPlatformTypes.h"

// Just a convient place to put a collection of static functions
// that are used by the library entry points.

class AxPlugin {
public:
	static bool CanUnloadNow();
	static unsigned int GetClassCount();
	static HRESULT GetClassObjectID(ULONG index, CLSID *pClassID);
	static HRESULT GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
};

#endif

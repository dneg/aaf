#ifndef __AxPluginFctryPtrcl_h__
#define __AxPluginFctryPtrcl_h__

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

#include <AAFCOMPlatformTypes.h>
#include <AAFSmartPointer.h>

// Implementations of AxPluginFctryPrtcl are registered with
// the AxPluginRegistry singleton.  Implementations of this
// protocol are responsible for creating an instance of a
// class factory (i.e., IClassFactory) for the underlying component.

// Please refer the UML.

class AxPluginFctryPrtcl {
public:

	virtual ~AxPluginFctryPrtcl() = 0;

	virtual CLSID GetClassID() = 0;

	// True if clsid matches the id of this instance.
	virtual bool IsClassId( const CLSID& clsid ) = 0;

	// Create a factory instance, but return it as an IUnkown.
	virtual HRESULT CreateClassObject( IUnknown** ppv ) = 0;
};

#endif

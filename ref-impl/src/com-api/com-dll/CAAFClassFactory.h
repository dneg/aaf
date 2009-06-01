//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __CAAFClassFactory_h__
#define __CAAFClassFactory_h__

#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif

// We need the following definition for a bool type since
// the Win32 used BOOL as an int and ActiveX SDK, MacOLE use
// unsigned int for OLEBOOL.
// NOTE: We may have to move this definition to AAFTypes.h.
#define AAFBOOL BOOL

// Define the object creation callback function that should be
// implemented as a static method for every concrete AAF object.
// 
typedef HRESULT (*AAFCreateComObjectProc)(IUnknown *, void **); 

class CAAFClassFactory : 
	public IClassFactory,
	public CAAFUnknown
{
public:
	CAAFClassFactory(AAFCreateComObjectProc );

    // IUnknown methods  
	AAF_DECLARE_STANDARD_UNKNOWN()
	
protected:
	// CAAFUnknown override
    STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppv);

  // Wrapper to call the private _pfnCreate proc.
  HRESULT CallClassFactoryFunction(IUnknown *, void **);

public:
    // IClassFactory methods 
    STDMETHOD(CreateInstance)(IUnknown * pUnkOuter, REFIID riid, void ** ppv); 
    STDMETHOD(LockServer)(AAFBOOL fLock);     

private:
	AAFCreateComObjectProc _pfnCreate;
};


#endif // __CAAFClassFactory_h__

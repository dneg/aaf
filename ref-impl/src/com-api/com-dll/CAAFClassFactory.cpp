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

//
// File: AAFClassFactory.cpp
// 
// Implementation for the AAFClassFactory class.
//

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __CAAFClassFactory_h__
#include "CAAFClassFactory.h"
#endif

#ifndef __CAAFServer_h__
#include "CAAFServer.h"
#endif

#include <string.h>

// Implementation
CAAFClassFactory::CAAFClassFactory(AAFCreateComObjectProc pfnCreate)
	: CAAFUnknown(0),
	  _pfnCreate(pfnCreate)
{
}

inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}

HRESULT CAAFClassFactory::InternalQueryInterface 
(
	REFIID riid,
	void **ppvObj)
{
	if (!ppvObj)
		return E_INVALIDARG;

    if (EQUAL_UID(riid,IID_IClassFactory)) 
    { 
        *ppvObj = (IClassFactory *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

	// Always delegate back to base implementation.
	return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}


// Object creation is delegated to the callback function passed into
// the constructor.
STDMETHODIMP CAAFClassFactory::CreateInstance
(
	LPUNKNOWN pUnkOuter,
	REFIID riid,
	void * * ppvObj)
{
	HRESULT hr = S_OK;

	if (!ppvObj)
		return E_INVALIDARG;

	*ppvObj = 0;
	
	// "Note that the nondelegating versions of QueryInteface, 
	// AddRef, and Relase are used. If a stand-alone identity is
	// being created, this is certainly appropriate. If an aggregate
	// is being created, this is necessary to ensure that the inner
	// object is Addrefed, not the outer object. Also note that 
	// the outer object must request IUnknown as the initial 
	// inteface. This is mandated by the COM Specification.
	// If the outer object could request any initial inteface, then
	// the inner object would essentially need to keep two duplicate
	// sets of vptrs, one set that delegated its QueryInterface, 
	// AddRef, and Release implementations and another set that did
	// not. By restricting the initial interface to IUnknown, the 
	// object implementor needs to isolate only the one vptr to act
	// as the nondelegating IUnknown." (p. 194, "Essensial COM", by
	// Don Box, Addison Wesley, 2nd Printing Feb. 1998)
	if (pUnkOuter && !EQUAL_UID(IID_IUnknown,riid))
		return E_INVALIDARG; //CLASS_E_NOAGGREGATION;

	// Ask the callback function to create the object instance.
	CAAFUnknown* pUnknown = 0;
	hr = CallClassFactoryFunction(pUnkOuter, (void **)&pUnknown);
	if (FAILED(hr))
		return hr;

	// Bump the reference count.
	pUnknown->InternalAddRef();
	
	// See if the object implements the requested interface. if 
	// this call succeeds than the reference count should be two if 
	// it fails then the reference count will still be one and the 
	// following release will delete the object.
	hr = pUnknown->InternalQueryInterface(riid, ppvObj);

	pUnknown->InternalRelease();

	return hr;
}

// Wrapper to call the private _pfnCreate proc.
HRESULT CAAFClassFactory::CallClassFactoryFunction(IUnknown *pUnkOuter, void **ppvObj)
{
	return (*_pfnCreate)(pUnkOuter, ppvObj);
}


STDMETHODIMP CAAFClassFactory::LockServer
(
	AAFBOOL fLock
)
{
  g_pAAFServer->Lock((fLock) ? kAAFTrue : kAAFFalse);
	return S_OK;
}

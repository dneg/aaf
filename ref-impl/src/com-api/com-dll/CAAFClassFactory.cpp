/***********************************************************************
*
*            Copyright (c) 1998-1999 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
*  prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

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
	HRESULT hr = S_OK;

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

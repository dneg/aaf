/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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



// Implementation
CAAFClassFactory::CAAFClassFactory(AAFCreateComObjectProc pfnCreate)
	: CAAFUnknown(NULL),
	  _pfnCreate(pfnCreate)
{
}

HRESULT CAAFClassFactory::InternalQueryInterface 
(
	REFIID riid,
	void **ppvObj)
{
	HRESULT hr = S_OK;

	if (NULL == ppvObj)
		return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (riid == IID_IClassFactory) 
    { 
        *ppvObj = this; 
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

	if (NULL == ppvObj)
		return E_INVALIDARG;

	*ppvObj = NULL;
	
	// Ask the callback function to create the object instance.
	IUnknown* pUnknown = NULL;
	hr = _pfnCreate(pUnkOuter, (void **)&pUnknown);
	if (FAILED(hr))
		return hr;
		
	// See if the object implements the requested interface. if 
	// this call succeeds than the reference count should be two if 
	// it fails then the reference count will still be one and the 
	// following spObject release will delete the object.
	hr = pUnknown->QueryInterface(riid, ppvObj);

	pUnknown->Release();

	return hr;
}


STDMETHODIMP CAAFClassFactory::LockServer
(
	AAFBOOL fLock
)
{
	g_pAAFServer->Lock(static_cast<aafBool>(fLock));
	return S_OK;
}




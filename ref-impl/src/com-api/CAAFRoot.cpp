//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#include "CAAFRoot.h"
#include "ImplAAFRoot.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>

// CLSID for AAFRoot 
// {7C3712C1-390D-11D2-841B-00600832ACB8}
EXTERN_C const CLSID CLSID_AAFRoot = { 0x7C3712C1, 0x390D, 0x11D2, { 0x84, 0x1B, 0x00, 0x60, 0x08, 0x32, 0xAC, 0xB8 } };



CAAFRoot::CAAFRoot (IUnknown * pControllingUnknown, aafBool /*doInit*/)
  : CAAFUnknown(pControllingUnknown),
    _rep(NULL)
{}


CAAFRoot::~CAAFRoot ()
{
  // The base class destructor of ImplAAFRoot must be declared as virtual.
  // Eventhough the ImplAAFRoot is reference counted we call delete 
  // directly since the ImplAAFRoot::ReleaseReference() delegates its
  // release to CAAFRoot::Release().
  if (_rep)
  {
    delete _rep;
    _rep = NULL;
  } 
}


void CAAFRoot::InitRep (ImplAAFRoot * newRep)
{
  assert (newRep);
  assert (!_rep);
  _rep = newRep;
  newRep->InitContainer (this);
}


ImplAAFRoot * CAAFRoot::GetRepObject ()
{
  assert (_rep);
  return _rep;
}


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFRoot::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFRoot)) 
    { 
        *ppvObj = (IAAFRoot *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Return private implementation pointer for delegation.
// NOTE: This is NOT the pointer to the COM object's implementation
// object!
//
HRESULT STDMETHODCALLTYPE
    CAAFRoot::GetImplRep (void **ppRep)
{
    if (!ppRep)
        return E_INVALIDARG;  
    *ppRep = static_cast<void*>(GetRepObject());
    return S_OK;
}

//
// Intialize class extensions. This method is called after the
// contained Impl object has been completely initialized. This
// allows the aggregated extension object access to all of the
// interfaces and property data of its controlling unknown.
//
HRESULT STDMETHODCALLTYPE
    CAAFRoot::InitializeExtension(REFCLSID /*clsid*/)
{
  // Extensions are not supported for the shared
  // CAAFRoot module.
  return AAFRESULT_NOT_IMPLEMENTED;
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFRoot)




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



#include "CAAFTimecodeStream12M.h"
#include "ImplAAFTimecodeStream12M.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>



// CLSID for AAFTimecodeStream12M 
// {EBF5EDB-2D3A-11d2-8043-006008143E6F}
EXTERN_C const CLSID CLSID_AAFTimecodeStream12M = { 0xEBF5EDB, 0x2D3A, 0x11d2, { 0x80, 0x43, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };





CAAFTimecodeStream12M::CAAFTimecodeStream12M (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTimecodeStream (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTimecodeStream12M * newRep;
      newRep = new ImplAAFTimecodeStream12M;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTimecodeStream12M::~CAAFTimecodeStream12M ()
{
}





//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFTimecodeStream12M::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTimecodeStream12M)) 
    { 
        *ppvObj = (IAAFTimecodeStream12M *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTimecodeStream::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTimecodeStream12M)


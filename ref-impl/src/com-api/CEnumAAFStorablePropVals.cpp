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



#include "CEnumAAFStorablePropVals.h"
#include "ImplEnumAAFStorablePropVals.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


// CLSID for EnumAAFStorablePropVals 
// {5096c201-831b-11d4-a812-8a70df17ed53}
EXTERN_C const CLSID CLSID_EnumAAFStorablePropVals = { 0x5096c201, 0x831b, 0x11d4, { 0xa8, 0x12, 0x8a, 0x70, 0xdf, 0x17, 0xed, 0x53 } };





CEnumAAFStorablePropVals::CEnumAAFStorablePropVals (IUnknown * pControllingUnknown, aafBool doInit)
  : CEnumAAFPropertyValues (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFStorablePropVals * newRep;
      newRep = new ImplEnumAAFStorablePropVals;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFStorablePropVals::~CEnumAAFStorablePropVals ()
{
}


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CEnumAAFStorablePropVals::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFStorablePropVals)) 
    { 
        *ppvObj = (IEnumAAFStorablePropVals *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CEnumAAFPropertyValues::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFStorablePropVals)


//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "CAAFPlainEssenceData.h"
#include "ImplAAFPlainEssenceData.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


// CLSID for AAFPlainEssenceData 
// {38b4b20d-d7b8-4005-ae7c-a2dab4785c8d}
EXTERN_C const CLSID CLSID_AAFPlainEssenceData = { 0x38b4b20d, 0xd7b8, 0x4005, { 0xae, 0x7c, 0xa2, 0xda, 0xb4, 0x78, 0x5c, 0x8d } };





CAAFPlainEssenceData::CAAFPlainEssenceData (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFEssenceData (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFPlainEssenceData * newRep;
      newRep = new ImplAAFPlainEssenceData;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFPlainEssenceData::~CAAFPlainEssenceData ()
{
}



//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFPlainEssenceData::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFPlainEssenceData)) 
    { 
        *ppvObj = (IAAFPlainEssenceData *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFEssenceData::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFPlainEssenceData)


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



#include "CAAFTypeDefMXFEssenceStream.h"
#include "ImplAAFTypeDefMXFEssenceStream.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


// CLSID for AAFTypeDefMXFEssenceStream 
// {6760c4b4-129c-4356-8048-bd69c6aba4a4}
EXTERN_C const CLSID CLSID_AAFTypeDefMXFEssenceStream = { 0x6760c4b4, 0x129c, 0x4356, { 0x80, 0x48, 0xbd, 0x69, 0xc6, 0xab, 0xa4, 0xa4 } };





CAAFTypeDefMXFEssenceStream::CAAFTypeDefMXFEssenceStream (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDefStream (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefMXFEssenceStream * newRep;
      newRep = new ImplAAFTypeDefMXFEssenceStream;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefMXFEssenceStream::~CAAFTypeDefMXFEssenceStream ()
{
}



//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFTypeDefMXFEssenceStream::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefMXFEssenceStream)) 
    { 
        *ppvObj = (IAAFTypeDefMXFEssenceStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDefStream::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefMXFEssenceStream)


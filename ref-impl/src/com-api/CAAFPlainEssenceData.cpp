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

HRESULT STDMETHODCALLTYPE
    CAAFPlainEssenceData::GetEssenceElementKey (aafUID_t *  pEssenceElementKey)
{
  HRESULT hr;

  ImplAAFPlainEssenceData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainEssenceData*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetEssenceElementKey (pEssenceElementKey);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }


  return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFPlainEssenceData::SetEssenceElementKey (aafUID_constref  key)
{
  HRESULT hr;

  ImplAAFPlainEssenceData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainEssenceData*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetEssenceElementKey (key);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }


  return hr;
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

    if (EQUAL_UID(riid,IID_IAAFKLVEssenceDataParameters)) 
    { 
        *ppvObj = (IAAFKLVEssenceDataParameters *)this; 
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

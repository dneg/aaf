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



#include "CEnumAAFTaggedValues.h"
#include "ImplEnumAAFTaggedValues.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>

#include "CAAFTaggedValue.h"
#include "ImplAAFTaggedValue.h"

// CLSID for EnumAAFTaggedValues 
// {5f7916f0-0f87-11d3-8a3e-0050040ef7d2}
EXTERN_C const CLSID CLSID_EnumAAFTaggedValues = { 0x5f7916f0, 0x0f87, 0x11d3, { 0x8a, 0x3e, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2 } };





CEnumAAFTaggedValues::CEnumAAFTaggedValues (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFTaggedValues * newRep;
      newRep = new ImplEnumAAFTaggedValues;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFTaggedValues::~CEnumAAFTaggedValues ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFTaggedValues::NextOne (IAAFTaggedValue ** ppTaggedValues)
{
  HRESULT hr;

  ImplEnumAAFTaggedValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFTaggedValues*> (pO);
  assert (ptr);

  //
  // set up for ppTaggedValues
  //
  ImplAAFTaggedValue * internalppTaggedValues = NULL;
  ImplAAFTaggedValue ** pinternalppTaggedValues = NULL;
  if (ppTaggedValues)
    {
      pinternalppTaggedValues = &internalppTaggedValues;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppTaggedValues);
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

  //
  // cleanup for ppTaggedValues
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppTaggedValues)
        {
          pUnknown = static_cast<IUnknown *> (internalppTaggedValues->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFTaggedValue, (void **)ppTaggedValues);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppTaggedValues->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFTaggedValues::Next (aafUInt32  count,
        IAAFTaggedValue ** ppTaggedValues,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFTaggedValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFTaggedValues*> (pO);
  assert (ptr);

  //
  // set up for ppTaggedValues
  //
  ImplAAFTaggedValue ** internalppTaggedValues = NULL;
  assert (count >= 0);
  internalppTaggedValues = new ImplAAFTaggedValue*[count];
  assert (internalppTaggedValues);

  ImplAAFTaggedValue ** pinternalppTaggedValues = NULL;
  if (ppTaggedValues)
    {
      pinternalppTaggedValues = internalppTaggedValues;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppTaggedValues,
        pNumFetched);
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

  //
  // cleanup for ppTaggedValues
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppTaggedValues[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFTaggedValue, (void **)(ppTaggedValues+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppTaggedValues[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppTaggedValues;
  internalppTaggedValues = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFTaggedValues::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFTaggedValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFTaggedValues*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Skip
       (count);
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
    CEnumAAFTaggedValues::Reset ()
{
  ImplEnumAAFTaggedValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFTaggedValues*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Reset();
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
    CEnumAAFTaggedValues::Clone (IEnumAAFTaggedValues ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFTaggedValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFTaggedValues*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFTaggedValues * internalppEnum = NULL;
  ImplEnumAAFTaggedValues ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->Clone
       (pinternalppEnum);
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

  //
  // cleanup for ppEnum
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEnum)
        {
          pUnknown = static_cast<IUnknown *> (internalppEnum->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IEnumAAFTaggedValues, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
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
HRESULT CEnumAAFTaggedValues::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFTaggedValues)) 
    { 
        *ppvObj = (IEnumAAFTaggedValues *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFTaggedValues)


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



#include "CEnumAAFPropertyValues.h"
#include "ImplEnumAAFPropertyValues.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

// CLSID for EnumAAFPropertyValues 
// {809c8ca9-9884-11d2-841f-00600832acb8}
EXTERN_C const CLSID CLSID_EnumAAFPropertyValues = { 0x809c8ca9, 0x9884, 0x11d2, { 0x84, 0x1f, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CEnumAAFPropertyValues::CEnumAAFPropertyValues (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFPropertyValues * newRep;
      newRep = new ImplEnumAAFPropertyValues;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFPropertyValues::~CEnumAAFPropertyValues ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFPropertyValues::NextOne (IAAFPropertyValue ** ppPropertyValue)
{
  HRESULT hr;

  ImplEnumAAFPropertyValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyValues*> (pO);
  assert (ptr);

  //
  // set up for ppPropertyValue
  //
  ImplAAFPropertyValue * internalppPropertyValue = NULL;
  ImplAAFPropertyValue ** pinternalppPropertyValue = NULL;
  if (ppPropertyValue)
    {
      pinternalppPropertyValue = &internalppPropertyValue;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppPropertyValue);
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
  // cleanup for ppPropertyValue
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPropertyValue)
        {
          pUnknown = static_cast<IUnknown *> (internalppPropertyValue->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppPropertyValue);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPropertyValue->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFPropertyValues::Next (aafUInt32  count,
        IAAFPropertyValue ** ppPropertyValues,
        aafUInt32 *  pFetched)
{
  HRESULT hr;

  ImplEnumAAFPropertyValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyValues*> (pO);
  assert (ptr);

  //
  // set up for ppPropertyValues
  //
  ImplAAFPropertyValue ** internalppPropertyValues = NULL;
  assert (count >= 0);
  internalppPropertyValues = new ImplAAFPropertyValue*[count];
  assert (internalppPropertyValues);

  ImplAAFPropertyValue ** pinternalppPropertyValues = NULL;
  if (ppPropertyValues)
    {
      pinternalppPropertyValues = internalppPropertyValues;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppPropertyValues,
        pFetched);
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
  // cleanup for ppPropertyValues
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppPropertyValues[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)(ppPropertyValues+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppPropertyValues[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppPropertyValues;
  internalppPropertyValues = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFPropertyValues::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFPropertyValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyValues*> (pO);
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
    CEnumAAFPropertyValues::Reset ()
{
  ImplEnumAAFPropertyValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyValues*> (pO);
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
    CEnumAAFPropertyValues::Clone (IEnumAAFPropertyValues ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFPropertyValues * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyValues*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFPropertyValues * internalppEnum = NULL;
  ImplEnumAAFPropertyValues ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFPropertyValues, (void **)ppEnum);
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
HRESULT CEnumAAFPropertyValues::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFPropertyValues)) 
    { 
        *ppvObj = (IEnumAAFPropertyValues *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFPropertyValues)


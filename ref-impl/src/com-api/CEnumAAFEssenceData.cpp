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



#include "CEnumAAFEssenceData.h"
#include "ImplEnumAAFEssenceData.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>



#include "CAAFEssenceData.h"
#include "ImplAAFEssenceData.h"



// CLSID for EnumAAFEssenceData 
// {B1A21393-1A7D-11D2-BF78-00104BC9156D}
EXTERN_C const CLSID CLSID_EnumAAFEssenceData = { 0xB1A21393, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CEnumAAFEssenceData::CEnumAAFEssenceData (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFEssenceData * newRep;
      newRep = new ImplEnumAAFEssenceData;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFEssenceData::~CEnumAAFEssenceData ()
{
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFEssenceData::NextOne (IAAFEssenceData ** ppEssenceData)
{
  HRESULT hr;

  ImplEnumAAFEssenceData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFEssenceData*> (pO);
  assert (ptr);

  //
  // set up for ppEssenceData
  //
  ImplAAFEssenceData * internalppEssenceData = NULL;
  ImplAAFEssenceData ** pinternalppEssenceData = NULL;
  if (ppEssenceData)
    {
      pinternalppEssenceData = &internalppEssenceData;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppEssenceData);
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
  // cleanup for ppEssenceData
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEssenceData)
        {
          pUnknown = static_cast<IUnknown *> (internalppEssenceData->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFEssenceData, (void **)ppEssenceData);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEssenceData->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFEssenceData::Next (aafUInt32  count,
        IAAFEssenceData ** ppEssenceData,
        aafUInt32 *  pFetched)
{
  HRESULT hr;

  ImplEnumAAFEssenceData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFEssenceData*> (pO);
  assert (ptr);

  //
  // set up for ppEssenceData
  //
  ImplAAFEssenceData ** internalppEssenceData = NULL;
  assert (count >= 0);
  internalppEssenceData = new ImplAAFEssenceData*[count];
  assert (internalppEssenceData);

  ImplAAFEssenceData ** pinternalppEssenceData = NULL;
  if (ppEssenceData)
    {
      pinternalppEssenceData = internalppEssenceData;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppEssenceData,
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
  // cleanup for ppEssenceData
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppEssenceData[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFEssenceData, (void **)(ppEssenceData+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppEssenceData[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppEssenceData;
  internalppEssenceData = 0;
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFEssenceData::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFEssenceData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFEssenceData*> (pO);
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
    CEnumAAFEssenceData::Reset ()
{
  ImplEnumAAFEssenceData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFEssenceData*> (pO);
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
    CEnumAAFEssenceData::Clone (IEnumAAFEssenceData ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFEssenceData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFEssenceData*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFEssenceData * internalppEnum = NULL;
  ImplEnumAAFEssenceData ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFEssenceData, (void **)ppEnum);
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
HRESULT CEnumAAFEssenceData::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFEssenceData)) 
    { 
        *ppvObj = (IEnumAAFEssenceData *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFEssenceData)


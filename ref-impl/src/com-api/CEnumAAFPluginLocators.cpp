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



#include "CEnumAAFPluginLocators.h"
#include "ImplEnumAAFPluginLocators.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFLocator.h"
#include "ImplAAFLocator.h"

// CLSID for EnumAAFPluginLocators 
// {AD1BB860-DBB2-11d2-809C-006008143E6F}
EXTERN_C const CLSID CLSID_EnumAAFPluginLocators = { 0xAD1BB860, 0xDBB2, 0x11d2, { 0x80, 0x9C, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };





CEnumAAFPluginLocators::CEnumAAFPluginLocators (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFPluginLocators * newRep;
      newRep = new ImplEnumAAFPluginLocators;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFPluginLocators::~CEnumAAFPluginLocators ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFPluginLocators::NextOne (IAAFLocator ** ppAAFLocator)
{
  HRESULT hr;

  ImplEnumAAFPluginLocators * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPluginLocators*> (pO);
  assert (ptr);

  //
  // set up for ppAAFLocator
  //
  ImplAAFLocator * internalppAAFLocator = NULL;
  ImplAAFLocator ** pinternalppAAFLocator = NULL;
  if (ppAAFLocator)
    {
      pinternalppAAFLocator = &internalppAAFLocator;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppAAFLocator);
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
  // cleanup for ppAAFLocator
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppAAFLocator)
        {
          pUnknown = static_cast<IUnknown *> (internalppAAFLocator->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFLocator, (void **)ppAAFLocator);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppAAFLocator->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFPluginLocators::Next (aafUInt32  count,
        IAAFLocator ** ppAAFLocators,
        aafUInt32 *  pFetched)
{
  HRESULT hr;

  ImplEnumAAFPluginLocators * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPluginLocators*> (pO);
  assert (ptr);

  //
  // set up for ppAAFLocators
  //
  ImplAAFLocator ** internalppAAFLocators = NULL;
  assert (count >= 0);
  internalppAAFLocators = new ImplAAFLocator*[count];
  assert (internalppAAFLocators);

  ImplAAFLocator ** pinternalppAAFLocators = NULL;
  if (ppAAFLocators)
    {
      pinternalppAAFLocators = internalppAAFLocators;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppAAFLocators,
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
  // cleanup for ppAAFLocators
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppAAFLocators[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFLocator, (void **)(ppAAFLocators+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppAAFLocators[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppAAFLocators;
  internalppAAFLocators = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFPluginLocators::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFPluginLocators * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPluginLocators*> (pO);
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
    CEnumAAFPluginLocators::Reset ()
{
  ImplEnumAAFPluginLocators * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPluginLocators*> (pO);
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
    CEnumAAFPluginLocators::Clone (IEnumAAFPluginLocators ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFPluginLocators * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPluginLocators*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFPluginLocators * internalppEnum = NULL;
  ImplEnumAAFPluginLocators ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFPluginLocators, (void **)ppEnum);
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
HRESULT CEnumAAFPluginLocators::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFPluginLocators)) 
    { 
        *ppvObj = (IEnumAAFPluginLocators *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFPluginLocators)


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



#include "CAAFSelector.h"
#include "ImplAAFSelector.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFSegment.h"
#include "ImplAAFSegment.h"

#include "CEnumAAFSegments.h"
#include "ImplEnumAAFSegments.h"

// CLSID for AAFSelector 
// {e684d787-b935-11d2-bf9d-00104bc9156d}
EXTERN_C const CLSID CLSID_AAFSelector = { 0xe684d787, 0xb935, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };





CAAFSelector::CAAFSelector (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFSegment (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFSelector * newRep;
      newRep = new ImplAAFSelector;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFSelector::~CAAFSelector ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFSelector::GetSelectedSegment (IAAFSegment ** ppSelSegment)
{
  HRESULT hr;

  ImplAAFSelector * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSelector*> (pO);
  assert (ptr);

  //
  // set up for ppSelSegment
  //
  ImplAAFSegment * internalppSelSegment = NULL;
  ImplAAFSegment ** pinternalppSelSegment = NULL;
  if (ppSelSegment)
    {
      pinternalppSelSegment = &internalppSelSegment;
    }

  try
    {
      hr = ptr->GetSelectedSegment
       (pinternalppSelSegment);
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
  // cleanup for ppSelSegment
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppSelSegment)
        {
          pUnknown = static_cast<IUnknown *> (internalppSelSegment->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSegment, (void **)ppSelSegment);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppSelSegment->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFSelector::SetSelectedSegment (IAAFSegment * pSelSegment)
{
  HRESULT hr;

  ImplAAFSelector * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSelector*> (pO);
  assert (ptr);

  //
  // set up for pSelSegment
  //
  ImplAAFSegment * internalpSelSegment = NULL;
  if (pSelSegment)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSelSegment->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSelSegment = static_cast<ImplAAFSegment*>(arg);
      assert (internalpSelSegment);
    }

  try
    {
      hr = ptr->SetSelectedSegment
       (internalpSelSegment);
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
  // no cleanup necessary for pSelSegment
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFSelector::AppendAlternateSegment (IAAFSegment * pSegment)
{
  HRESULT hr;

  ImplAAFSelector * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSelector*> (pO);
  assert (ptr);

  //
  // set up for pSegment
  //
  ImplAAFSegment * internalpSegment = NULL;
  if (pSegment)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSegment->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSegment = static_cast<ImplAAFSegment*>(arg);
      assert (internalpSegment);
    }

  try
    {
      hr = ptr->AppendAlternateSegment
       (internalpSegment);
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
  // no cleanup necessary for pSegment
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFSelector::GetNumAlternateSegments (aafInt32 *  pNumSegments)
{
  HRESULT hr;

  ImplAAFSelector * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSelector*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetNumAlternateSegments
       (pNumSegments);
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
    CAAFSelector::EnumAlternateSegments (IEnumAAFSegments ** ppEnum)
{
  HRESULT hr;

  ImplAAFSelector * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSelector*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFSegments * internalppEnum = NULL;
  ImplEnumAAFSegments ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->EnumAlternateSegments
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFSegments, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFSelector::RemoveAlternateSegment (IAAFSegment * pSegment)
{
  HRESULT hr;

  ImplAAFSelector * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSelector*> (pO);
  assert (ptr);

  //
  // set up for pSegment
  //
  ImplAAFSegment * internalpSegment = NULL;
  if (pSegment)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSegment->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSegment = static_cast<ImplAAFSegment*>(arg);
      assert (internalpSegment);
    }

  try
    {
      hr = ptr->RemoveAlternateSegment
       (internalpSegment);
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
  // no cleanup necessary for pSegment
  //
  return hr;
}


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFSelector::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFSelector)) 
    { 
        *ppvObj = (IAAFSelector *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFSegment::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFSelector)


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



#include "CAAFNestedScope.h"
#include "ImplAAFNestedScope.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>



#include "CAAFSegment.h"
#include "ImplAAFSegment.h"

#include "CEnumAAFSegments.h"
#include "ImplEnumAAFSegments.h"

// CLSID for AAFNestedScope 
// {e44be452-2099-11d2-bf98-006097116212}
EXTERN_C const CLSID CLSID_AAFNestedScope = { 0xe44be452, 0x2099, 0x11d2, { 0xbf, 0x98, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFNestedScope::CAAFNestedScope (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFSegment (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFNestedScope * newRep;
      newRep = new ImplAAFNestedScope;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFNestedScope::~CAAFNestedScope ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFNestedScope::AppendSegment (IAAFSegment * pSegment)
{
  HRESULT hr;

  ImplAAFNestedScope * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFNestedScope*> (pO);
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
      hr = ptr->AppendSegment
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
    CAAFNestedScope::PrependSegment (IAAFSegment * pSegment)
{
  HRESULT hr;

  ImplAAFNestedScope * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFNestedScope*> (pO);
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
      hr = ptr->PrependSegment
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
    CAAFNestedScope::InsertSegmentAt (aafUInt32  index,
        IAAFSegment * pSegment)
{
  HRESULT hr;

  ImplAAFNestedScope * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFNestedScope*> (pO);
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
      hr = ptr->InsertSegmentAt
       (index,
        internalpSegment);
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
    CAAFNestedScope::RemoveSegmentAt (aafUInt32  index)
{
  HRESULT hr;

  ImplAAFNestedScope * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFNestedScope*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->RemoveSegmentAt
       (index);
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
    CAAFNestedScope::CountSegments (aafUInt32 *  pResult)
{
  HRESULT hr;

  ImplAAFNestedScope * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFNestedScope*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountSegments
       (pResult);
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
    CAAFNestedScope::GetSegmentAt (aafUInt32  index,
        IAAFSegment ** ppSegment)
{
  HRESULT hr;

  ImplAAFNestedScope * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFNestedScope*> (pO);
  assert (ptr);

  //
  // set up for ppSegment
  //
  ImplAAFSegment * internalppSegment = NULL;
  ImplAAFSegment ** pinternalppSegment = NULL;
  if (ppSegment)
    {
      pinternalppSegment = &internalppSegment;
    }

  try
    {
      hr = ptr->GetSegmentAt
       (index,
        pinternalppSegment);
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
  // cleanup for ppSegment
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppSegment)
        {
          pUnknown = static_cast<IUnknown *> (internalppSegment->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSegment, (void **)ppSegment);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppSegment->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFNestedScope::GetSegments (IEnumAAFSegments ** ppEnum)
{
  HRESULT hr;

  ImplAAFNestedScope * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFNestedScope*> (pO);
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
      hr = ptr->GetSegments
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

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFNestedScope::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFNestedScope)) 
    { 
        *ppvObj = (IAAFNestedScope *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFSegment::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFNestedScope)


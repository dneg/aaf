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



#include "CAAFPulldown.h"
#include "ImplAAFPulldown.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


// CLSID for AAFPulldown 
// {92B88032-2B2B-11d2-BF7E-00104BC9156D}
EXTERN_C const CLSID CLSID_AAFPulldown = { 0x92B88032, 0x2B2B, 0x11d2, { 0xBF, 0x7E, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CAAFPulldown::CAAFPulldown (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFSegment (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFPulldown * newRep;
      newRep = new ImplAAFPulldown;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFPulldown::~CAAFPulldown ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFPulldown::GetInputSegment (IAAFSegment ** ppInputSegment)
{
  HRESULT hr;

  ImplAAFPulldown * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPulldown*> (pO);
  assert (ptr);

  //
  // set up for ppInputSegment
  //
  ImplAAFSegment * internalppInputSegment = NULL;
  ImplAAFSegment ** pinternalppInputSegment = NULL;
  if (ppInputSegment)
    {
      pinternalppInputSegment = &internalppInputSegment;
    }

  try
    {
      hr = ptr->GetInputSegment
       (pinternalppInputSegment);
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
  // cleanup for ppInputSegment
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppInputSegment)
        {
          pUnknown = static_cast<IUnknown *> (internalppInputSegment->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSegment, (void **)ppInputSegment);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppInputSegment->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPulldown::SetInputSegment (IAAFSegment * pInputSegment)
{
  HRESULT hr;

  ImplAAFPulldown * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPulldown*> (pO);
  assert (ptr);

  //
  // set up for pInputSegment
  //
  ImplAAFSegment * internalpInputSegment = NULL;
  if (pInputSegment)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pInputSegment->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpInputSegment = static_cast<ImplAAFSegment*>(arg);
      assert (internalpInputSegment);
    }

  try
    {
      hr = ptr->SetInputSegment
       (internalpInputSegment);
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
  // no cleanup necessary for pInputSegment
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPulldown::GetPulldownKind (aafPulldownKind_t *  pPulldownKind)
{
  HRESULT hr;

  ImplAAFPulldown * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPulldown*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetPulldownKind
       (pPulldownKind);
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
    CAAFPulldown::SetPulldownKind (aafPulldownKind_t  pulldownKind)
{
  HRESULT hr;

  ImplAAFPulldown * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPulldown*> (pO);
  assert (ptr);

  //
  // set up for pulldownKind
  //
  if (! Is_aafPulldownKind_t_Valid(pulldownKind))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->SetPulldownKind
       (pulldownKind);
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
    CAAFPulldown::GetPulldownDirection (aafPulldownDir_t *  pPulldownDirection)
{
  HRESULT hr;

  ImplAAFPulldown * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPulldown*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetPulldownDirection
       (pPulldownDirection);
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
    CAAFPulldown::SetPulldownDirection (aafPulldownDir_t  pulldownDirection)
{
  HRESULT hr;

  ImplAAFPulldown * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPulldown*> (pO);
  assert (ptr);

  //
  // set up for pulldownDirection
  //
  if (! Is_aafPulldownDir_t_Valid(pulldownDirection))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->SetPulldownDirection
       (pulldownDirection);
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
    CAAFPulldown::GetPhaseFrame (aafPhaseFrame_t *  pPhaseFrame)
{
  HRESULT hr;

  ImplAAFPulldown * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPulldown*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetPhaseFrame
       (pPhaseFrame);
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
    CAAFPulldown::SetPhaseFrame (aafPhaseFrame_t  phaseFrame)
{
  HRESULT hr;

  ImplAAFPulldown * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPulldown*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetPhaseFrame
       (phaseFrame);
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
HRESULT CAAFPulldown::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFPulldown)) 
    { 
        *ppvObj = (IAAFPulldown *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFSegment::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFPulldown)


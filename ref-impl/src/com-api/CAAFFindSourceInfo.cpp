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



#include "CAAFFindSourceInfo.h"
#include "ImplAAFFindSourceInfo.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/



#include "CAAFMob.h"
#include "ImplAAFMob.h"


// CLSID for AAFFindSourceInfo 
// {9346ACD0-2713-11d2-8035-006008143E6F}
EXTERN_C const CLSID CLSID_AAFFindSourceInfo = { 0x9346ACD0, 0x2713, 0x11d2, { 0x80, 0x35, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };




CAAFFindSourceInfo::CAAFFindSourceInfo (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFFindSourceInfo * newRep;
      newRep = new ImplAAFFindSourceInfo;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFFindSourceInfo::~CAAFFindSourceInfo ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFFindSourceInfo::GetMob (IAAFMob ** ppMob)
{
  HRESULT hr;

  ImplAAFFindSourceInfo * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFindSourceInfo*> (pO);
  assert (ptr);

  //
  // set up for ppMob
  //
  ImplAAFMob * internalppMob = NULL;
  ImplAAFMob ** pinternalppMob = NULL;
  if (ppMob)
    {
      pinternalppMob = &internalppMob;
    }

  try
    {
      hr = ptr->GetMob
       (pinternalppMob);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
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
  // cleanup for ppMob
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppMob)
        {
          pUnknown = static_cast<IUnknown *> (internalppMob->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFMob, (void **)ppMob);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppMob->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFFindSourceInfo::GetSourceReference (aafSourceRef_t *  pSourceRef)
{
  HRESULT hr;

  ImplAAFFindSourceInfo * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFindSourceInfo*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetSourceReference
       (pSourceRef);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
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
    CAAFFindSourceInfo::GetLength (aafLength_t *  pLength)
{
  HRESULT hr;

  ImplAAFFindSourceInfo * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFindSourceInfo*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetLength
       (pLength);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
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
    CAAFFindSourceInfo::GetEditRate (aafRational_t *  pEditRate)
{
  HRESULT hr;

  ImplAAFFindSourceInfo * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFindSourceInfo*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetEditRate
       (pEditRate);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
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
HRESULT CAAFFindSourceInfo::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFFindSourceInfo)) 
    { 
        *ppvObj = (IAAFFindSourceInfo *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFFindSourceInfo)


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



#include "CEnumAAFMobs.h"
#include "ImplEnumAAFMobs.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFMob.h"
#include "ImplAAFMob.h"

// CLSID for EnumAAFMobs 
// {B1A21387-1A7D-11D2-BF78-00104BC9156D}
EXTERN_C const CLSID CLSID_EnumAAFMobs = { 0xB1A21387, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CEnumAAFMobs::CEnumAAFMobs (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFMobs * newRep;
      newRep = new ImplEnumAAFMobs;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFMobs::~CEnumAAFMobs ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFMobs::NextOne (IAAFMob ** ppMob)
{
  HRESULT hr;

  ImplEnumAAFMobs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobs*> (pO);
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
      hr = ptr->NextOne
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
    CEnumAAFMobs::Next (aafUInt32  count,
        IAAFMob ** ppMobs,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFMobs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobs*> (pO);
  assert (ptr);

  //
  // set up for ppMobs
  //
  ImplAAFMob ** internalppMobs = NULL;
  assert (count >= 0);
  internalppMobs = new ImplAAFMob*[count];
  assert (internalppMobs);

  ImplAAFMob ** pinternalppMobs = NULL;
  if (ppMobs)
    {
      pinternalppMobs = internalppMobs;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppMobs,
        pNumFetched);
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
  // cleanup for ppMobs
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppMobs[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFMob, (void **)(ppMobs+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppMobs[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppMobs;
  internalppMobs = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFMobs::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFMobs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobs*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Skip
       (count);
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
    CEnumAAFMobs::Reset ()
{
  ImplEnumAAFMobs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobs*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Reset();
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
    CEnumAAFMobs::Clone (IEnumAAFMobs ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFMobs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobs*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFMobs * internalppEnum = NULL;
  ImplEnumAAFMobs ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->Clone
       (pinternalppEnum);
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
  // cleanup for ppEnum
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEnum)
        {
          pUnknown = static_cast<IUnknown *> (internalppEnum->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IEnumAAFMobs, (void **)ppEnum);
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
HRESULT CEnumAAFMobs::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFMobs)) 
    { 
        *ppvObj = (IEnumAAFMobs *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFMobs)


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



#include "CEnumAAFPropertyDefs.h"
#include "ImplEnumAAFPropertyDefs.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>

#include "CAAFPropertyDef.h"
#include "ImplAAFPropertyDef.h"

#include "CAAFTypeDef.h"
#include "ImplAAFTypeDef.h"

// CLSID for EnumAAFPropertyDefs 
// {245343C2-2D59-11D2-BF80-00104BC9156D}
EXTERN_C const CLSID CLSID_EnumAAFPropertyDefs = { 0x245343C2, 0x2D59, 0x11D2, { 0xBF, 0x80, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CEnumAAFPropertyDefs::CEnumAAFPropertyDefs (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFPropertyDefs * newRep;
      newRep = new ImplEnumAAFPropertyDefs;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFPropertyDefs::~CEnumAAFPropertyDefs ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFPropertyDefs::NextOne (IAAFPropertyDef ** ppPropertyDefs)
{
  HRESULT hr;

  ImplEnumAAFPropertyDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyDefs*> (pO);
  assert (ptr);

  //
  // set up for ppPropertyDefs
  //
  ImplAAFPropertyDef * internalppPropertyDefs = NULL;
  ImplAAFPropertyDef ** pinternalppPropertyDefs = NULL;
  if (ppPropertyDefs)
    {
      pinternalppPropertyDefs = &internalppPropertyDefs;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppPropertyDefs);
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
  // cleanup for ppPropertyDefs
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPropertyDefs)
        {
          pUnknown = static_cast<IUnknown *> (internalppPropertyDefs->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyDef, (void **)ppPropertyDefs);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPropertyDefs->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFPropertyDefs::Next (aafUInt32  count,
        IAAFPropertyDef ** ppPropertyDefs,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFPropertyDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyDefs*> (pO);
  assert (ptr);

  //
  // set up for ppPropertyDefs
  //
  ImplAAFPropertyDef ** internalppPropertyDefs = NULL;
  assert (count >= 0);
  internalppPropertyDefs = new ImplAAFPropertyDef*[count];
  assert (internalppPropertyDefs);

  ImplAAFPropertyDef ** pinternalppPropertyDefs = NULL;
  if (ppPropertyDefs)
    {
      pinternalppPropertyDefs = internalppPropertyDefs;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppPropertyDefs,
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
  // cleanup for ppPropertyDefs
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppPropertyDefs[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFPropertyDef, (void **)(ppPropertyDefs+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppPropertyDefs[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppPropertyDefs;
  internalppPropertyDefs = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFPropertyDefs::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFPropertyDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyDefs*> (pO);
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
    CEnumAAFPropertyDefs::Reset ()
{
  ImplEnumAAFPropertyDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyDefs*> (pO);
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
    CEnumAAFPropertyDefs::Clone (IEnumAAFPropertyDefs ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFPropertyDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFPropertyDefs*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFPropertyDefs * internalppEnum = NULL;
  ImplEnumAAFPropertyDefs ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFPropertyDefs, (void **)ppEnum);
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
HRESULT CEnumAAFPropertyDefs::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFPropertyDefs)) 
    { 
        *ppvObj = (IEnumAAFPropertyDefs *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFPropertyDefs)


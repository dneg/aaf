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



#include "CEnumAAFDataDefs.h"
#include "ImplEnumAAFDataDefs.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>

#include "CAAFDataDef.h"
#include "ImplAAFDataDef.h"

// CLSID for EnumAAFDataDefs 
// {D8000381-498E-11D2-841B-00600832ACB8}
EXTERN_C const CLSID CLSID_EnumAAFDataDefs = { 0xD8000381, 0x498E, 0x11D2, { 0x84, 0x1B, 0x00, 0x60, 0x08, 0x32, 0xAC, 0xB8 } };





CEnumAAFDataDefs::CEnumAAFDataDefs (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFDataDefs * newRep;
      newRep = new ImplEnumAAFDataDefs;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFDataDefs::~CEnumAAFDataDefs ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFDataDefs::NextOne (IAAFDataDef ** ppDataDefs)
{
  HRESULT hr;

  ImplEnumAAFDataDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFDataDefs*> (pO);
  assert (ptr);

  //
  // set up for ppDataDefs
  //
  ImplAAFDataDef * internalppDataDefs = NULL;
  ImplAAFDataDef ** pinternalppDataDefs = NULL;
  if (ppDataDefs)
    {
      pinternalppDataDefs = &internalppDataDefs;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppDataDefs);
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
  // cleanup for ppDataDefs
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppDataDefs)
        {
          pUnknown = static_cast<IUnknown *> (internalppDataDefs->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFDataDef, (void **)ppDataDefs);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppDataDefs->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFDataDefs::Next (aafUInt32  count,
        IAAFDataDef ** ppDataDefs,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFDataDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFDataDefs*> (pO);
  assert (ptr);

  //
  // set up for ppDataDefs
  //
  ImplAAFDataDef ** internalppDataDefs = NULL;
  assert (count >= 0);
  internalppDataDefs = new ImplAAFDataDef*[count];
  assert (internalppDataDefs);

  ImplAAFDataDef ** pinternalppDataDefs = NULL;
  if (ppDataDefs)
    {
      pinternalppDataDefs = internalppDataDefs;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppDataDefs,
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
  // cleanup for ppDataDefs
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppDataDefs[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFDataDef, (void **)(ppDataDefs+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppDataDefs[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppDataDefs;
  internalppDataDefs = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFDataDefs::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFDataDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFDataDefs*> (pO);
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
    CEnumAAFDataDefs::Reset ()
{
  ImplEnumAAFDataDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFDataDefs*> (pO);
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
    CEnumAAFDataDefs::Clone (IEnumAAFDataDefs ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFDataDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFDataDefs*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFDataDefs * internalppEnum = NULL;
  ImplEnumAAFDataDefs ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFDataDefs, (void **)ppEnum);
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
HRESULT CEnumAAFDataDefs::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFDataDefs)) 
    { 
        *ppvObj = (IEnumAAFDataDefs *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFDataDefs)


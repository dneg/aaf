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



#include "CEnumAAFInterpolationDefs.h"
#include "ImplEnumAAFInterpolationDefs.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>

#include "CAAFInterpolationDef.h"
#include "ImplAAFInterpolationDef.h"

// CLSID for EnumAAFInterpolationDefs 
// {293600A4-0D37-11d3-80A9-006008143E6F}
EXTERN_C const CLSID CLSID_EnumAAFInterpolationDefs = { 0x293600A4, 0x0D37, 0x11d3, { 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };





CEnumAAFInterpolationDefs::CEnumAAFInterpolationDefs (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFInterpolationDefs * newRep;
      newRep = new ImplEnumAAFInterpolationDefs;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFInterpolationDefs::~CEnumAAFInterpolationDefs ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFInterpolationDefs::NextOne (IAAFInterpolationDef ** ppInterpolationDefs)
{
  HRESULT hr;

  ImplEnumAAFInterpolationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFInterpolationDefs*> (pO);
  assert (ptr);

  //
  // set up for ppInterpolationDefs
  //
  ImplAAFInterpolationDef * internalppInterpolationDefs = NULL;
  ImplAAFInterpolationDef ** pinternalppInterpolationDefs = NULL;
  if (ppInterpolationDefs)
    {
      pinternalppInterpolationDefs = &internalppInterpolationDefs;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppInterpolationDefs);
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
  // cleanup for ppInterpolationDefs
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppInterpolationDefs)
        {
          pUnknown = static_cast<IUnknown *> (internalppInterpolationDefs->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFInterpolationDef, (void **)ppInterpolationDefs);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppInterpolationDefs->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFInterpolationDefs::Next (aafUInt32  count,
        IAAFInterpolationDef ** ppInterpolationDefs,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFInterpolationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFInterpolationDefs*> (pO);
  assert (ptr);

  //
  // set up for ppInterpolationDefs
  //
  ImplAAFInterpolationDef ** internalppInterpolationDefs = NULL;
  assert (count >= 0);
  internalppInterpolationDefs = new ImplAAFInterpolationDef*[count];
  assert (internalppInterpolationDefs);

  ImplAAFInterpolationDef ** pinternalppInterpolationDefs = NULL;
  if (ppInterpolationDefs)
    {
      pinternalppInterpolationDefs = internalppInterpolationDefs;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppInterpolationDefs,
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
  // cleanup for ppInterpolationDefs
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppInterpolationDefs[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFInterpolationDef, (void **)(ppInterpolationDefs+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppInterpolationDefs[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppInterpolationDefs;
  internalppInterpolationDefs = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFInterpolationDefs::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFInterpolationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFInterpolationDefs*> (pO);
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
    CEnumAAFInterpolationDefs::Reset ()
{
  ImplEnumAAFInterpolationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFInterpolationDefs*> (pO);
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
    CEnumAAFInterpolationDefs::Clone (IEnumAAFInterpolationDefs ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFInterpolationDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFInterpolationDefs*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFInterpolationDefs * internalppEnum = NULL;
  ImplEnumAAFInterpolationDefs ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFInterpolationDefs, (void **)ppEnum);
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
HRESULT CEnumAAFInterpolationDefs::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFInterpolationDefs)) 
    { 
        *ppvObj = (IEnumAAFInterpolationDefs *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFInterpolationDefs)


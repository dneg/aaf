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



#include "CEnumAAFContainerDefs.h"
#include "ImplEnumAAFContainerDefs.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>

#include "CAAFContainerDef.h"
#include "ImplAAFContainerDef.h"

// CLSID for EnumAAFContainerDefs 
// {3B2C9AD1-022A-11d3-80A6-006008143E6F}
EXTERN_C const CLSID CLSID_EnumAAFContainerDefs = { 0x3B2C9AD1, 0x022A, 0x11d3, { 0x80, 0xA6, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };





CEnumAAFContainerDefs::CEnumAAFContainerDefs (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFContainerDefs * newRep;
      newRep = new ImplEnumAAFContainerDefs;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFContainerDefs::~CEnumAAFContainerDefs ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFContainerDefs::NextOne (IAAFContainerDef ** ppContainerDefs)
{
  HRESULT hr;

  ImplEnumAAFContainerDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFContainerDefs*> (pO);
  assert (ptr);

  //
  // set up for ppContainerDefs
  //
  ImplAAFContainerDef * internalppContainerDefs = NULL;
  ImplAAFContainerDef ** pinternalppContainerDefs = NULL;
  if (ppContainerDefs)
    {
      pinternalppContainerDefs = &internalppContainerDefs;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppContainerDefs);
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
  // cleanup for ppContainerDefs
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppContainerDefs)
        {
          pUnknown = static_cast<IUnknown *> (internalppContainerDefs->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFContainerDef, (void **)ppContainerDefs);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppContainerDefs->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFContainerDefs::Next (aafUInt32  count,
        IAAFContainerDef ** ppContainerDefs,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFContainerDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFContainerDefs*> (pO);
  assert (ptr);

  //
  // set up for ppContainerDefs
  //
  ImplAAFContainerDef ** internalppContainerDefs = NULL;
  assert (count >= 0);
  internalppContainerDefs = new ImplAAFContainerDef*[count];
  assert (internalppContainerDefs);

  ImplAAFContainerDef ** pinternalppContainerDefs = NULL;
  if (ppContainerDefs)
    {
      pinternalppContainerDefs = internalppContainerDefs;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppContainerDefs,
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
  // cleanup for ppContainerDefs
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppContainerDefs[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFContainerDef, (void **)(ppContainerDefs+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppContainerDefs[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppContainerDefs;
  internalppContainerDefs = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFContainerDefs::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFContainerDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFContainerDefs*> (pO);
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
    CEnumAAFContainerDefs::Reset ()
{
  ImplEnumAAFContainerDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFContainerDefs*> (pO);
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
    CEnumAAFContainerDefs::Clone (IEnumAAFContainerDefs ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFContainerDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFContainerDefs*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFContainerDefs * internalppEnum = NULL;
  ImplEnumAAFContainerDefs ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFContainerDefs, (void **)ppEnum);
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
HRESULT CEnumAAFContainerDefs::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFContainerDefs)) 
    { 
        *ppvObj = (IEnumAAFContainerDefs *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFContainerDefs)


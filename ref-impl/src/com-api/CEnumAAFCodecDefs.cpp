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



#include "CEnumAAFCodecDefs.h"
#include "ImplEnumAAFCodecDefs.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>

#include "CAAFCodecDef.h"
#include "ImplAAFCodecDef.h"


// CLSID for EnumAAFCodecDefs 
// {3B2C9AD3-022A-11d3-80A6-006008143E6F}
EXTERN_C const CLSID CLSID_EnumAAFCodecDefs = { 0x3B2C9AD3, 0x022A, 0x11d3, { 0x80, 0xA6, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };





CEnumAAFCodecDefs::CEnumAAFCodecDefs (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFCodecDefs * newRep;
      newRep = new ImplEnumAAFCodecDefs;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFCodecDefs::~CEnumAAFCodecDefs ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFCodecDefs::NextOne (IAAFCodecDef ** ppCodecDefs)
{
  HRESULT hr;

  ImplEnumAAFCodecDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFCodecDefs*> (pO);
  assert (ptr);

  //
  // set up for ppCodecDefs
  //
  ImplAAFCodecDef * internalppCodecDefs = NULL;
  ImplAAFCodecDef ** pinternalppCodecDefs = NULL;
  if (ppCodecDefs)
    {
      pinternalppCodecDefs = &internalppCodecDefs;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppCodecDefs);
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
  // cleanup for ppCodecDefs
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppCodecDefs)
        {
          pUnknown = static_cast<IUnknown *> (internalppCodecDefs->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFCodecDef, (void **)ppCodecDefs);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppCodecDefs->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFCodecDefs::Next (aafUInt32  count,
        IAAFCodecDef ** ppCodecDefs,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFCodecDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFCodecDefs*> (pO);
  assert (ptr);

  //
  // set up for ppCodecDefs
  //
  ImplAAFCodecDef ** internalppCodecDefs = NULL;
  assert (count >= 0);
  internalppCodecDefs = new ImplAAFCodecDef*[count];
  assert (internalppCodecDefs);

  ImplAAFCodecDef ** pinternalppCodecDefs = NULL;
  if (ppCodecDefs)
    {
      pinternalppCodecDefs = internalppCodecDefs;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppCodecDefs,
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
  // cleanup for ppCodecDefs
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppCodecDefs[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFCodecDef, (void **)(ppCodecDefs+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppCodecDefs[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppCodecDefs;
  internalppCodecDefs = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFCodecDefs::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFCodecDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFCodecDefs*> (pO);
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
    CEnumAAFCodecDefs::Reset ()
{
  ImplEnumAAFCodecDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFCodecDefs*> (pO);
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
    CEnumAAFCodecDefs::Clone (IEnumAAFCodecDefs ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFCodecDefs * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFCodecDefs*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFCodecDefs * internalppEnum = NULL;
  ImplEnumAAFCodecDefs ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFCodecDefs, (void **)ppEnum);
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
HRESULT CEnumAAFCodecDefs::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFCodecDefs)) 
    { 
        *ppvObj = (IEnumAAFCodecDefs *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFCodecDefs)


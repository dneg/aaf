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



#include "CAAFCompositionMob.h"
#include "ImplAAFCompositionMob.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


// CLSID for AAFCompositionMob 
// {91920d64-2a2e-11D2-bfA3-006097116212}
EXTERN_C const CLSID CLSID_AAFCompositionMob = { 0x91920d64, 0x2a2e, 0x11D2, { 0xbf, 0xA3, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFCompositionMob::CAAFCompositionMob (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFMob (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFCompositionMob * newRep;
      newRep = new ImplAAFCompositionMob;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFCompositionMob::~CAAFCompositionMob ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFCompositionMob::Initialize (aafCharacter_constptr  pName)
{
  HRESULT hr;

  ImplAAFCompositionMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFCompositionMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Initialize
       (pName);
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
    CAAFCompositionMob::GetDefaultFade (aafDefaultFade_t *  pResult)
{
  HRESULT hr;

  ImplAAFCompositionMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFCompositionMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetDefaultFade
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
    CAAFCompositionMob::SetDefaultFade (aafLength_t  fadeLength,
        aafFadeType_t  fadeType,
        aafRational_t  fadeEditUnit)
{
  HRESULT hr;

  ImplAAFCompositionMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFCompositionMob*> (pO);
  assert (ptr);

  //
  // set up for fadeType
  //
  if (! Is_aafFadeType_t_Valid(fadeType))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->SetDefaultFade
       (fadeLength,
        fadeType,
        fadeEditUnit);
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
HRESULT CAAFCompositionMob::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFCompositionMob)) 
    { 
        *ppvObj = (IAAFCompositionMob *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFMob::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFCompositionMob)


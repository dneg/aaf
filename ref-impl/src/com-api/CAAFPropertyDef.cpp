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



#include "CAAFPropertyDef.h"
#include "ImplAAFPropertyDef.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CEnumAAFPropertyValues.h"
#include "ImplEnumAAFPropertyValues.h"

#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

#include "CAAFTypeDef.h"
#include "ImplAAFTypeDef.h"

// CLSID for AAFPropertyDef 
// {dfbd6522-1d81-11d2-bf96-006097116212}
EXTERN_C const CLSID CLSID_AAFPropertyDef = { 0xdfbd6522, 0x1d81, 0x11d2, { 0xbf, 0x96, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
 




CAAFPropertyDef::CAAFPropertyDef (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFMetaDefinition (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFPropertyDef * newRep;
      newRep = new ImplAAFPropertyDef;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFPropertyDef::~CAAFPropertyDef ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFPropertyDef::GetTypeDef (IAAFTypeDef ** ppTypeDef)
{
  HRESULT hr;

  ImplAAFPropertyDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPropertyDef*> (pO);
  assert (ptr);

  //
  // set up for ppTypeDef
  //
  ImplAAFTypeDef * internalppTypeDef = NULL;
  ImplAAFTypeDef ** pinternalppTypeDef = NULL;
  if (ppTypeDef)
    {
      pinternalppTypeDef = &internalppTypeDef;
    }

  try
    {
      hr = ptr->GetTypeDef
       (pinternalppTypeDef);
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
  // cleanup for ppTypeDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppTypeDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppTypeDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFTypeDef, (void **)ppTypeDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppTypeDef->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPropertyDef::GetName (aafCharacter *  pName,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFPropertyDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPropertyDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetName
       (pName,
        bufSize);
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
    CAAFPropertyDef::GetNameBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFPropertyDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPropertyDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetNameBufLen
       (pBufSize);
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
    CAAFPropertyDef::GetIsOptional (aafBoolean_t *  pIsOptional)
{
  HRESULT hr;

  ImplAAFPropertyDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPropertyDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetIsOptional
       (pIsOptional);
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
    CAAFPropertyDef::GetIsUniqueIdentifier (aafBoolean_t *  pIsUniqueIdentifier)
{
  HRESULT hr;

  ImplAAFPropertyDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPropertyDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetIsUniqueIdentifier
       (pIsUniqueIdentifier);
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
    CAAFPropertyDef::SetDescription (aafCharacter_constptr  pDescription)
{
  HRESULT hr;

  ImplAAFPropertyDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPropertyDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetDescription
       (pDescription);
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
    CAAFPropertyDef::GetDescription (aafCharacter *  pDescription,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFPropertyDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPropertyDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetDescription
       (pDescription,
        bufSize);
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
    CAAFPropertyDef::GetDescriptionBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFPropertyDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPropertyDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetDescriptionBufLen
       (pBufSize);
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
HRESULT CAAFPropertyDef::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFPropertyDef)) 
    { 
        *ppvObj = (IAAFPropertyDef *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFMetaDefinition::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFPropertyDef)


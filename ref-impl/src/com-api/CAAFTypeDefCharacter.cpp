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



#include "CAAFTypeDefCharacter.h"
#include "ImplAAFTypeDefCharacter.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"


// CLSID for AAFTypeDefCharacter 
// {FE717CA9-F04C-11d3-800B-00104BC9156D}
EXTERN_C const CLSID CLSID_AAFTypeDefCharacter = { 0xFE717CA9, 0xF04C, 0x11d3, { 0x80, 0x0B, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CAAFTypeDefCharacter::CAAFTypeDefCharacter (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefCharacter * newRep;
      newRep = new ImplAAFTypeDefCharacter;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefCharacter::~CAAFTypeDefCharacter ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefCharacter::CreateValueFromCharacter (aafCharacter  character,
        IAAFPropertyValue ** ppCharacterValue)
{
  HRESULT hr;

  ImplAAFTypeDefCharacter * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefCharacter*> (pO);
  assert (ptr);

  //
  // set up for ppCharacterValue
  //
  ImplAAFPropertyValue * internalppCharacterValue = NULL;
  ImplAAFPropertyValue ** pinternalppCharacterValue = NULL;
  if (ppCharacterValue)
    {
      pinternalppCharacterValue = &internalppCharacterValue;
    }

  try
    {
      hr = ptr->CreateValueFromCharacter
       (character,
        pinternalppCharacterValue);
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
  // cleanup for ppCharacterValue
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppCharacterValue)
        {
          pUnknown = static_cast<IUnknown *> (internalppCharacterValue->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppCharacterValue);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppCharacterValue->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefCharacter::GetCharacter (IAAFPropertyValue * pCharacterValue,
        aafCharacter *  pCharacter)
{
  HRESULT hr;

  ImplAAFTypeDefCharacter * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefCharacter*> (pO);
  assert (ptr);

  //
  // set up for pCharacterValue
  //
  ImplAAFPropertyValue * internalpCharacterValue = NULL;
  if (pCharacterValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pCharacterValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpCharacterValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpCharacterValue);
    }

  try
    {
      hr = ptr->GetCharacter
       (internalpCharacterValue,
        pCharacter);
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
  // no cleanup necessary for pCharacterValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefCharacter::SetCharacter (IAAFPropertyValue * pCharacterValue,
        aafCharacter  character)
{
  HRESULT hr;

  ImplAAFTypeDefCharacter * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefCharacter*> (pO);
  assert (ptr);

  //
  // set up for pCharacterValue
  //
  ImplAAFPropertyValue * internalpCharacterValue = NULL;
  if (pCharacterValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pCharacterValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpCharacterValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpCharacterValue);
    }

  try
    {
      hr = ptr->SetCharacter
       (internalpCharacterValue,
        character);
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
  // no cleanup necessary for pCharacterValue
  //
  return hr;
}




//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFTypeDefCharacter::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefCharacter)) 
    { 
        *ppvObj = (IAAFTypeDefCharacter *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefCharacter)


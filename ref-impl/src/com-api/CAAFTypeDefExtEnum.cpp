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



#include "CAAFTypeDefExtEnum.h"
#include "ImplAAFTypeDefExtEnum.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

// CLSID for AAFTypeDefExtEnum 
// {a5456f02-0ef2-11d3-842e-00600832acb8}
EXTERN_C const CLSID CLSID_AAFTypeDefExtEnum = { 0xa5456f02, 0x0ef2, 0x11d3, { 0x84, 0x2e, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CAAFTypeDefExtEnum::CAAFTypeDefExtEnum (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefExtEnum * newRep;
      newRep = new ImplAAFTypeDefExtEnum;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefExtEnum::~CAAFTypeDefExtEnum ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefExtEnum::Initialize (aafUID_constref  id,
        aafCharacter_constptr  pTypeName)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Initialize
       (id,
        pTypeName);
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
    CAAFTypeDefExtEnum::CreateValueFromName (aafCharacter_constptr  Name,
        IAAFPropertyValue ** ppPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);

  //
  // set up for ppPropVal
  //
  ImplAAFPropertyValue * internalppPropVal = NULL;
  ImplAAFPropertyValue ** pinternalppPropVal = NULL;
  if (ppPropVal)
    {
      pinternalppPropVal = &internalppPropVal;
    }

  try
    {
      hr = ptr->CreateValueFromName
       (Name,
        pinternalppPropVal);
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
  // cleanup for ppPropVal
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPropVal)
        {
          pUnknown = static_cast<IUnknown *> (internalppPropVal->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppPropVal);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPropVal->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}

  

HRESULT STDMETHODCALLTYPE
    CAAFTypeDefExtEnum::CountElements (aafUInt32 *  pCount)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountElements
       (pCount);
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
    CAAFTypeDefExtEnum::GetElementValue (aafUInt32  index,
        aafUID_t *  pOutValue)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetElementValue
       (index,
        pOutValue);
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
    CAAFTypeDefExtEnum::GetElementName (aafUInt32  index,
        aafCharacter *  pOutValue,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetElementName
       (index,
        pOutValue,
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
    CAAFTypeDefExtEnum::GetElementNameBufLen (aafUInt32  index,
        aafUInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetElementNameBufLen
       (index,
        pLen);
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
    CAAFTypeDefExtEnum::GetNameFromValue (IAAFPropertyValue * pValue,
        aafCharacter *  pName,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);

  //
  // set up for pValue
  //
  ImplAAFPropertyValue * internalpValue = NULL;
  if (pValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpValue);
    }

  try
    {
      hr = ptr->GetNameFromValue
       (internalpValue,
        pName,
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

  //
  // no cleanup necessary for pValue
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefExtEnum::GetNameBufLenFromValue (IAAFPropertyValue * pValue,
        aafUInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);

  //
  // set up for pValue
  //
  ImplAAFPropertyValue * internalpValue = NULL;
  if (pValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpValue);
    }

  try
    {
      hr = ptr->GetNameBufLenFromValue
       (internalpValue,
        pLen);
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
  // no cleanup necessary for pValue
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefExtEnum::GetNameFromAUID (aafUID_constref  value,
        aafCharacter *  pName,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetNameFromAUID
       (value,
        pName,
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
    CAAFTypeDefExtEnum::GetNameBufLenFromAUID (aafUID_constref  value,
        aafUInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetNameBufLenFromAUID
       (value,
        pLen);
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
    CAAFTypeDefExtEnum::GetAUIDValue (IAAFPropertyValue * pPropValIn,
        aafUID_t *  pValueOut)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);

  //
  // set up for pPropValIn
  //
  ImplAAFPropertyValue * internalpPropValIn = NULL;
  if (pPropValIn)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pPropValIn->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpPropValIn = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpPropValIn);
    }

  try
    {
      hr = ptr->GetAUIDValue
       (internalpPropValIn,
        pValueOut);
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
  // no cleanup necessary for pPropValIn
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefExtEnum::SetAUIDValue (IAAFPropertyValue * pPropValToSet,
        aafUID_constref  valueIn)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);

  //
  // set up for pPropValToSet
  //
  ImplAAFPropertyValue * internalpPropValToSet = NULL;
  if (pPropValToSet)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pPropValToSet->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpPropValToSet = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpPropValToSet);
    }

  try
    {
      hr = ptr->SetAUIDValue
       (internalpPropValToSet,
        valueIn);
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
  // no cleanup necessary for pPropValToSet
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefExtEnum::AppendElement (aafUID_constref  value,
        aafCharacter_constptr  pName)
{
  HRESULT hr;

  ImplAAFTypeDefExtEnum * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefExtEnum*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->AppendElement
       (value,
        pName);
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
HRESULT CAAFTypeDefExtEnum::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefExtEnum)) 
    { 
        *ppvObj = (IAAFTypeDefExtEnum *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefExtEnum)


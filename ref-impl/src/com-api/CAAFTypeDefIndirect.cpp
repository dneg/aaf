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



#include "CAAFTypeDefIndirect.h"
#include "ImplAAFTypeDefIndirect.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"


// CLSID for AAFTypeDefIndirect 
// {FE717CA7-F04C-11d3-800B-00104BC9156D}
EXTERN_C const CLSID CLSID_AAFTypeDefIndirect = { 0xFE717CA7, 0xF04C, 0x11d3, { 0x80, 0x0B, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CAAFTypeDefIndirect::CAAFTypeDefIndirect (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefIndirect * newRep;
      newRep = new ImplAAFTypeDefIndirect;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefIndirect::~CAAFTypeDefIndirect ()
{
}




HRESULT STDMETHODCALLTYPE
    CAAFTypeDefIndirect::CreateValueFromActualValue (IAAFPropertyValue * pActualValue,
        IAAFPropertyValue ** ppIndirectPropertyValue)
{
  HRESULT hr;

  ImplAAFTypeDefIndirect * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefIndirect*> (pO);
  assert (ptr);

  //
  // set up for pActualValue
  //
  ImplAAFPropertyValue * internalpActualValue = NULL;
  if (pActualValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pActualValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpActualValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpActualValue);
    }
  //
  // set up for ppIndirectPropertyValue
  //
  ImplAAFPropertyValue * internalppIndirectPropertyValue = NULL;
  ImplAAFPropertyValue ** pinternalppIndirectPropertyValue = NULL;
  if (ppIndirectPropertyValue)
    {
      pinternalppIndirectPropertyValue = &internalppIndirectPropertyValue;
    }

  try
    {
      hr = ptr->CreateValueFromActualValue
       (internalpActualValue,
        pinternalppIndirectPropertyValue);
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
  // no cleanup necessary for pActualValue
  //
  //
  // cleanup for ppIndirectPropertyValue
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppIndirectPropertyValue)
        {
          pUnknown = static_cast<IUnknown *> (internalppIndirectPropertyValue->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppIndirectPropertyValue);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppIndirectPropertyValue->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefIndirect::CreateValueFromActualData (IAAFTypeDef * pActualType,
        aafMemPtr_t  pInitData,
        aafUInt32  initDataSize,
        IAAFPropertyValue ** ppIndirectPropertyValue)
{
  HRESULT hr;

  ImplAAFTypeDefIndirect * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefIndirect*> (pO);
  assert (ptr);

  //
  // set up for pActualType
  //
  ImplAAFTypeDef * internalpActualType = NULL;
  if (pActualType)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pActualType->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpActualType = static_cast<ImplAAFTypeDef*>(arg);
      assert (internalpActualType);
    }
  //
  // set up for ppIndirectPropertyValue
  //
  ImplAAFPropertyValue * internalppIndirectPropertyValue = NULL;
  ImplAAFPropertyValue ** pinternalppIndirectPropertyValue = NULL;
  if (ppIndirectPropertyValue)
    {
      pinternalppIndirectPropertyValue = &internalppIndirectPropertyValue;
    }

  try
    {
      hr = ptr->CreateValueFromActualData
       (internalpActualType,
        pInitData,
        initDataSize,
        pinternalppIndirectPropertyValue);
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
  // no cleanup necessary for pActualType
  //
  //
  // cleanup for ppIndirectPropertyValue
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppIndirectPropertyValue)
        {
          pUnknown = static_cast<IUnknown *> (internalppIndirectPropertyValue->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppIndirectPropertyValue);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppIndirectPropertyValue->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefIndirect::GetActualValue (IAAFPropertyValue * pIndirectPropertyValue,
        IAAFPropertyValue ** ppActualPropertyValue)
{
  HRESULT hr;

  ImplAAFTypeDefIndirect * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefIndirect*> (pO);
  assert (ptr);

  //
  // set up for pIndirectPropertyValue
  //
  ImplAAFPropertyValue * internalpIndirectPropertyValue = NULL;
  if (pIndirectPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pIndirectPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpIndirectPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpIndirectPropertyValue);
    }
  //
  // set up for ppActualPropertyValue
  //
  ImplAAFPropertyValue * internalppActualPropertyValue = NULL;
  ImplAAFPropertyValue ** pinternalppActualPropertyValue = NULL;
  if (ppActualPropertyValue)
    {
      pinternalppActualPropertyValue = &internalppActualPropertyValue;
    }

  try
    {
      hr = ptr->GetActualValue
       (internalpIndirectPropertyValue,
        pinternalppActualPropertyValue);
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
  // no cleanup necessary for pIndirectPropertyValue
  //
  //
  // cleanup for ppActualPropertyValue
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppActualPropertyValue)
        {
          pUnknown = static_cast<IUnknown *> (internalppActualPropertyValue->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppActualPropertyValue);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppActualPropertyValue->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}

  //)

HRESULT STDMETHODCALLTYPE
    CAAFTypeDefIndirect::GetActualSize (IAAFPropertyValue * pIndirectPropertyValue,
        aafUInt32 *  pActualSize)
{
  HRESULT hr;

  ImplAAFTypeDefIndirect * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefIndirect*> (pO);
  assert (ptr);

  //
  // set up for pIndirectPropertyValue
  //
  ImplAAFPropertyValue * internalpIndirectPropertyValue = NULL;
  if (pIndirectPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pIndirectPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpIndirectPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpIndirectPropertyValue);
    }

  try
    {
      hr = ptr->GetActualSize
       (internalpIndirectPropertyValue,
        pActualSize);
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
  // no cleanup necessary for pIndirectPropertyValue
  //
  return hr;
}

HRESULT STDMETHODCALLTYPE
    CAAFTypeDefIndirect::GetActualType (IAAFPropertyValue * pIndirectPropertyValue,
        IAAFTypeDef ** pActualType)
{
  HRESULT hr;

  ImplAAFTypeDefIndirect * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefIndirect*> (pO);
  assert (ptr);

  //
  // set up for pIndirectPropertyValue
  //
  ImplAAFPropertyValue * internalpIndirectPropertyValue = NULL;
  if (pIndirectPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pIndirectPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpIndirectPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpIndirectPropertyValue);
    }
  //
  // set up for pActualType
  //
  ImplAAFTypeDef * internalpActualType = NULL;
  ImplAAFTypeDef ** pinternalpActualType = NULL;
  if (pActualType)
    {
      pinternalpActualType = &internalpActualType;
    }

  try
    {
      hr = ptr->GetActualType
       (internalpIndirectPropertyValue,
        pinternalpActualType);
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
  // no cleanup necessary for pIndirectPropertyValue
  //
  //
  // cleanup for pActualType
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalpActualType)
        {
          pUnknown = static_cast<IUnknown *> (internalpActualType->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFTypeDef, (void **)pActualType);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalpActualType->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefIndirect::GetActualData (IAAFPropertyValue * pPropVal,
        aafMemPtr_t  pData,
        aafUInt32  dataSize)
{
  HRESULT hr;

  ImplAAFTypeDefIndirect * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefIndirect*> (pO);
  assert (ptr);

  //
  // set up for pPropVal
  //
  ImplAAFPropertyValue * internalpPropVal = NULL;
  if (pPropVal)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pPropVal->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpPropVal = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpPropVal);
    }

  try
    {
      hr = ptr->GetActualData
       (internalpPropVal,
        pData,
        dataSize);
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
  // no cleanup necessary for pPropVal
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
HRESULT CAAFTypeDefIndirect::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefIndirect)) 
    { 
        *ppvObj = (IAAFTypeDefIndirect *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefIndirect)


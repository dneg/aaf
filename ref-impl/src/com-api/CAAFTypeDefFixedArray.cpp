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



#include "CAAFTypeDefFixedArray.h"
#include "ImplAAFTypeDefFixedArray.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

#include "CEnumAAFPropertyValues.h"
#include "ImplEnumAAFPropertyValues.h"

// CLSID for AAFTypeDefFixedArray 
// {c3274645-9467-11d2-841f-00600832acb8}
EXTERN_C const CLSID CLSID_AAFTypeDefFixedArray = { 0xc3274645, 0x9467, 0x11d2, { 0x84, 0x1f, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CAAFTypeDefFixedArray::CAAFTypeDefFixedArray (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefFixedArray * newRep;
      newRep = new ImplAAFTypeDefFixedArray;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefFixedArray::~CAAFTypeDefFixedArray ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefFixedArray::Initialize (aafUID_constref  id,
        IAAFTypeDef * pTypeDef,
        aafUInt32  nElements,
        aafCharacter_constptr  pTypeName)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
  assert (ptr);

  //
  // set up for pTypeDef
  //
  ImplAAFTypeDef * internalpTypeDef = NULL;
  if (pTypeDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pTypeDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpTypeDef = static_cast<ImplAAFTypeDef*>(arg);
      assert (internalpTypeDef);
    }

  try
    {
      hr = ptr->Initialize
       (id,
        internalpTypeDef,
        nElements,
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

  //
  // no cleanup necessary for pTypeDef
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefFixedArray::GetType (IAAFTypeDef ** ppTypeDef)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
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
      hr = ptr->GetType
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
    CAAFTypeDefFixedArray::GetCount (aafUInt32 *  pCount)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetCount
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
    CAAFTypeDefFixedArray::CreateValueFromValues (IAAFPropertyValue ** ppElementValues,
        aafUInt32  numElements,
        IAAFPropertyValue ** ppPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
  assert (ptr);

  //
  // set up for ppElementValues
  //
  ImplAAFPropertyValue ** internalppElementValues = NULL;
  if (ppElementValues)
    {
      aafUInt32 localIdx;
	  assert (numElements >= 0);
	  internalppElementValues = new ImplAAFPropertyValue*[numElements];
	  assert (internalppElementValues);

	  for (localIdx = 0; localIdx < numElements; localIdx++)
		{
		  HRESULT hStat;
		  IAAFRoot * iObj;
		  ImplAAFRoot *arg;
		  hStat = ppElementValues[localIdx]->QueryInterface (IID_IAAFRoot, (void **)&iObj);
		  assert (SUCCEEDED (hStat));
		  assert (iObj);
		  hStat = iObj->GetImplRep((void **)&arg);
		  assert (SUCCEEDED (hStat));
		  iObj->Release(); // we are through with this interface pointer.
		  internalppElementValues[localIdx] = static_cast<ImplAAFPropertyValue*>(arg);
		  assert (internalppElementValues[localIdx]);
		}
    }
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
      hr = ptr->CreateValueFromValues
       (internalppElementValues,
        numElements,
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
  // cleanup for ppElementValues
  //
  if (internalppElementValues)
    {
      delete[] internalppElementValues;
      internalppElementValues = 0;
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
    CAAFTypeDefFixedArray::CreateValueFromCArray (aafMemPtr_t  pInitData,
        aafUInt32  initDataSize,
        IAAFPropertyValue ** ppPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
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
      hr = ptr->CreateValueFromCArray
       (pInitData,
        initDataSize,
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
    CAAFTypeDefFixedArray::GetElementValue (IAAFPropertyValue * pInPropVal,
        aafUInt32  index,
        IAAFPropertyValue ** ppOutPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
  assert (ptr);

  //
  // set up for pInPropVal
  //
  ImplAAFPropertyValue * internalpInPropVal = NULL;
  if (pInPropVal)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pInPropVal->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpInPropVal = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpInPropVal);
    }
  //
  // set up for ppOutPropVal
  //
  ImplAAFPropertyValue * internalppOutPropVal = NULL;
  ImplAAFPropertyValue ** pinternalppOutPropVal = NULL;
  if (ppOutPropVal)
    {
      pinternalppOutPropVal = &internalppOutPropVal;
    }

  try
    {
      hr = ptr->GetElementValue
       (internalpInPropVal,
        index,
        pinternalppOutPropVal);
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
  // no cleanup necessary for pInPropVal
  //
  //
  // cleanup for ppOutPropVal
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppOutPropVal)
        {
          pUnknown = static_cast<IUnknown *> (internalppOutPropVal->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppOutPropVal);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppOutPropVal->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefFixedArray::GetCArray (IAAFPropertyValue * pPropVal,
        aafMemPtr_t  pData,
        aafUInt32  dataSize)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
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
      hr = ptr->GetCArray
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



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefFixedArray::SetElementValue (IAAFPropertyValue * pPropVal,
        aafUInt32  index,
        IAAFPropertyValue * pMemberPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
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
  //
  // set up for pMemberPropVal
  //
  ImplAAFPropertyValue * internalpMemberPropVal = NULL;
  if (pMemberPropVal)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pMemberPropVal->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpMemberPropVal = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpMemberPropVal);
    }

  try
    {
      hr = ptr->SetElementValue
       (internalpPropVal,
        index,
        internalpMemberPropVal);
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
  //
  // no cleanup necessary for pMemberPropVal
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefFixedArray::SetCArray (IAAFPropertyValue * pPropVal,
        aafMemPtr_t  pData,
        aafUInt32  dataSize)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
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
      hr = ptr->SetCArray
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





HRESULT STDMETHODCALLTYPE
    CAAFTypeDefFixedArray::GetElements (IAAFPropertyValue * PSetPropVal,
        IEnumAAFPropertyValues ** ppEnum)
{
  HRESULT hr;

  ImplAAFTypeDefFixedArray * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefFixedArray*> (pO);
  assert (ptr);

  //
  // set up for PSetPropVal
  //
  ImplAAFPropertyValue * internalPSetPropVal = NULL;
  if (PSetPropVal)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = PSetPropVal->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalPSetPropVal = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalPSetPropVal);
    }
  //
  // set up for ppEnum
  //
  ImplEnumAAFPropertyValues * internalppEnum = NULL;
  ImplEnumAAFPropertyValues ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetElements
       (internalPSetPropVal,
        pinternalppEnum);
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
  // no cleanup necessary for PSetPropVal
  //
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFPropertyValues, (void **)ppEnum);
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
HRESULT CAAFTypeDefFixedArray::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefFixedArray)) 
    { 
        *ppvObj = (IAAFTypeDefFixedArray *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefFixedArray)


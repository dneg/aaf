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



#include "CAAFTypeDefSet.h"
#include "ImplAAFTypeDefSet.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

#include "CEnumAAFPropertyValues.h"
#include "ImplEnumAAFPropertyValues.h"

// CLSID for AAFTypeDefSet 
// {381e1f21-e7bb-11d2-842a-00600832acb8}
EXTERN_C const CLSID CLSID_AAFTypeDefSet = { 0x381e1f21, 0xe7bb, 0x11d2, { 0x84, 0x2a, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CAAFTypeDefSet::CAAFTypeDefSet (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefSet * newRep;
      newRep = new ImplAAFTypeDefSet;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefSet::~CAAFTypeDefSet ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefSet::Initialize (aafUID_constref  id,
        IAAFTypeDef * pTypeDef,
        aafCharacter_constptr  pTypeName)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
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
    CAAFTypeDefSet::GetElementType (IAAFTypeDef ** ppTypeDef)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
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
      hr = ptr->GetElementType
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
    CAAFTypeDefSet::AddElement (IAAFPropertyValue * pSetPropertyValue,
        IAAFPropertyValue * pElementPropertyValue)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
  assert (ptr);

  //
  // set up for pSetPropertyValue
  //
  ImplAAFPropertyValue * internalpSetPropertyValue = NULL;
  if (pSetPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSetPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSetPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpSetPropertyValue);
    }
  //
  // set up for pElementPropertyValue
  //
  ImplAAFPropertyValue * internalpElementPropertyValue = NULL;
  if (pElementPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pElementPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpElementPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpElementPropertyValue);
    }

  try
    {
      hr = ptr->AddElement
       (internalpSetPropertyValue,
        internalpElementPropertyValue);
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
  // no cleanup necessary for pSetPropertyValue
  //
  //
  // no cleanup necessary for pElementPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefSet::RemoveElement (IAAFPropertyValue * pSetPropertyValue,
        IAAFPropertyValue * pElementPropertyValue)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
  assert (ptr);

  //
  // set up for pSetPropertyValue
  //
  ImplAAFPropertyValue * internalpSetPropertyValue = NULL;
  if (pSetPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSetPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSetPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpSetPropertyValue);
    }
  //
  // set up for pElementPropertyValue
  //
  ImplAAFPropertyValue * internalpElementPropertyValue = NULL;
  if (pElementPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pElementPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpElementPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpElementPropertyValue);
    }

  try
    {
      hr = ptr->RemoveElement
       (internalpSetPropertyValue,
        internalpElementPropertyValue);
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
  // no cleanup necessary for pSetPropertyValue
  //
  //
  // no cleanup necessary for pElementPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefSet::ContainsElement (IAAFPropertyValue * pSetPropertyValue,
        IAAFPropertyValue * pElementPropertyValue,
        aafBoolean_t*  pContainsElement)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
  assert (ptr);

  //
  // set up for pSetPropertyValue
  //
  ImplAAFPropertyValue * internalpSetPropertyValue = NULL;
  if (pSetPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSetPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSetPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpSetPropertyValue);
    }
  //
  // set up for pElementPropertyValue
  //
  ImplAAFPropertyValue * internalpElementPropertyValue = NULL;
  if (pElementPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pElementPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpElementPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpElementPropertyValue);
    }

  try
    {
      hr = ptr->ContainsElement
       (internalpSetPropertyValue,
        internalpElementPropertyValue,
        pContainsElement);
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
  // no cleanup necessary for pSetPropertyValue
  //
  //
  // no cleanup necessary for pElementPropertyValue
  //
  return hr;
}




HRESULT STDMETHODCALLTYPE
    CAAFTypeDefSet::GetCount (IAAFPropertyValue * pSetPropertyValue,
        aafUInt32 *  pCount)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
  assert (ptr);

  //
  // set up for pSetPropertyValue
  //
  ImplAAFPropertyValue * internalpSetPropertyValue = NULL;
  if (pSetPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSetPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSetPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpSetPropertyValue);
    }

  try
    {
      hr = ptr->GetCount
       (internalpSetPropertyValue,
        pCount);
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
  // no cleanup necessary for pSetPropertyValue
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefSet::CreateKey (aafDataBuffer_t  pKeyPtr,
        aafUInt32  length,
        IAAFPropertyValue ** ppKey)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
  assert (ptr);

  //
  // set up for ppKey
  //
  ImplAAFPropertyValue * internalppKey = NULL;
  ImplAAFPropertyValue ** pinternalppKey = NULL;
  if (ppKey)
    {
      pinternalppKey = &internalppKey;
    }

  try
    {
      hr = ptr->CreateKey
       (pKeyPtr,
        length,
        pinternalppKey);
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
  // cleanup for ppKey
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppKey)
        {
          pUnknown = static_cast<IUnknown *> (internalppKey->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppKey);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppKey->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefSet::LookupElement (IAAFPropertyValue * pSetPropertyValue,
        IAAFPropertyValue * pKey,
        IAAFPropertyValue ** ppElementPropertyValue)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
  assert (ptr);

  //
  // set up for pSetPropertyValue
  //
  ImplAAFPropertyValue * internalpSetPropertyValue = NULL;
  if (pSetPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSetPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSetPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpSetPropertyValue);
    }
  //
  // set up for pKey
  //
  ImplAAFPropertyValue * internalpKey = NULL;
  if (pKey)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pKey->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpKey = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpKey);
    }
  //
  // set up for ppElementPropertyValue
  //
  ImplAAFPropertyValue * internalppElementPropertyValue = NULL;
  ImplAAFPropertyValue ** pinternalppElementPropertyValue = NULL;
  if (ppElementPropertyValue)
    {
      pinternalppElementPropertyValue = &internalppElementPropertyValue;
    }

  try
    {
      hr = ptr->LookupElement
       (internalpSetPropertyValue,
        internalpKey,
        pinternalppElementPropertyValue);
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
  // no cleanup necessary for pSetPropertyValue
  //
  //
  // no cleanup necessary for pKey
  //
  //
  // cleanup for ppElementPropertyValue
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppElementPropertyValue)
        {
          pUnknown = static_cast<IUnknown *> (internalppElementPropertyValue->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppElementPropertyValue);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppElementPropertyValue->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefSet::ContainsKey (IAAFPropertyValue * pSetPropertyValue,
        IAAFPropertyValue * pKey,
        aafBoolean_t*  pContainsKey)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
  assert (ptr);

  //
  // set up for pSetPropertyValue
  //
  ImplAAFPropertyValue * internalpSetPropertyValue = NULL;
  if (pSetPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSetPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSetPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpSetPropertyValue);
    }
  //
  // set up for pKey
  //
  ImplAAFPropertyValue * internalpKey = NULL;
  if (pKey)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pKey->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpKey = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpKey);
    }

  try
    {
      hr = ptr->ContainsKey
       (internalpSetPropertyValue,
        internalpKey,
        pContainsKey);
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
  // no cleanup necessary for pSetPropertyValue
  //
  //
  // no cleanup necessary for pKey
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefSet::GetElements (IAAFPropertyValue * pSetPropertyValue,
        IEnumAAFPropertyValues ** ppEnum)
{
  HRESULT hr;

  ImplAAFTypeDefSet * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefSet*> (pO);
  assert (ptr);

  //
  // set up for pSetPropertyValue
  //
  ImplAAFPropertyValue * internalpSetPropertyValue = NULL;
  if (pSetPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSetPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSetPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpSetPropertyValue);
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
       (internalpSetPropertyValue,
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
  // no cleanup necessary for pSetPropertyValue
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
HRESULT CAAFTypeDefSet::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefSet)) 
    { 
        *ppvObj = (IAAFTypeDefSet *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefSet)


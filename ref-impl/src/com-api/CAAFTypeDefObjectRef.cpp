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



#include "CAAFTypeDefObjectRef.h"
#include "ImplAAFTypeDefObjectRef.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>



#include "CAAFObject.h"
#include "ImplAAFObject.h"

#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

#include "CAAFClassDef.h"
#include "ImplAAFClassDef.h"

// CLSID for AAFTypeDefObjectRef 
// {c3274643-9467-11d2-841f-00600832acb8}
EXTERN_C const CLSID CLSID_AAFTypeDefObjectRef = { 0xc3274643, 0x9467, 0x11d2, { 0x84, 0x1f, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CAAFTypeDefObjectRef::CAAFTypeDefObjectRef (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefObjectRef * newRep;
      newRep = new ImplAAFTypeDefObjectRef;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefObjectRef::~CAAFTypeDefObjectRef ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefObjectRef::GetObjectType (IAAFClassDef ** ppObjType)
{
  HRESULT hr;

  ImplAAFTypeDefObjectRef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefObjectRef*> (pO);
  assert (ptr);

  //
  // set up for ppObjType
  //
  ImplAAFClassDef * internalppObjType = NULL;
  ImplAAFClassDef ** pinternalppObjType = NULL;
  if (ppObjType)
    {
      pinternalppObjType = &internalppObjType;
    }

  try
    {
      hr = ptr->GetObjectType
       (pinternalppObjType);
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
  // cleanup for ppObjType
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppObjType)
        {
          pUnknown = static_cast<IUnknown *> (internalppObjType->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFClassDef, (void **)ppObjType);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppObjType->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefObjectRef::CreateValue (IUnknown * pObj,
        IAAFPropertyValue ** ppPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefObjectRef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefObjectRef*> (pO);
  assert (ptr);

  //
  // set up for pObj
  //
  ImplAAFRoot * internalpObj = NULL;
  if (pObj)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pObj->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      if (FAILED(hStat))
      {
        // If input IUnknown argument MUST supprt our private IAAFRoot interface.
        // If it does not than the argument is not one of our implementation objects!
        assert(E_NOINTERFACE == hStat);
        if (E_NOINTERFACE == hStat)
          return AAFRESULT_INVALID_PARAM;
        else
          return hStat;
      }
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpObj = static_cast<ImplAAFRoot*>(arg);
      assert (internalpObj);
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
      hr = ptr->CreateValue
       (internalpObj,
        pinternalppPropVal);
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
  // no cleanup necessary for pObj
  //
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
    CAAFTypeDefObjectRef::GetObject (IAAFPropertyValue * pPropVal,
        REFIID  iid,
        IUnknown ** ppObject)
{
  HRESULT hr = S_OK;

  try
    {
    

  ImplAAFTypeDefObjectRef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefObjectRef*> (pO);
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
  // set up for ppObject
  //
  ImplAAFRoot * internalppObject = NULL;
  ImplAAFRoot ** pinternalppObject = NULL;
  if (ppObject)
    {
      pinternalppObject = &internalppObject;
    }


  hr = ptr->GetObject
       (internalpPropVal,
        pinternalppObject);

  //
  // no cleanup necessary for pPropVal
  //

  //
  // cleanup for ppObject
  //
  if (internalppObject)
    {
      IUnknown *pUnknown = static_cast<IUnknown *> (internalppObject->GetContainer());
      HRESULT hStat = pUnknown->QueryInterface(iid, (void **)ppObject);
      if (SUCCEEDED(hr) && FAILED(hStat))
        hr = hStat;
      internalppObject->ReleaseReference(); // We are through with this pointer.
    }


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


  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefObjectRef::SetObject (IAAFPropertyValue * pPropVal,
        IUnknown * pObject)
{
  HRESULT hr;

  ImplAAFTypeDefObjectRef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefObjectRef*> (pO);
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
  // set up for pObject
  //
  ImplAAFRoot * internalpObject = NULL;
  if (pObject)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pObject->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      if (FAILED(hStat))
      {
        // If input IUnknown argument MUST supprt our private IAAFRoot interface.
        // If it does not than the argument is not one of our implementation objects!
        assert(E_NOINTERFACE == hStat);
        if (E_NOINTERFACE == hStat)
          return AAFRESULT_INVALID_PARAM;
        else
          return hStat;
      }
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpObject = static_cast<ImplAAFRoot*>(arg);
      assert (internalpObject);
    }

  try
    {
      hr = ptr->SetObject
       (internalpPropVal,
        internalpObject);
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
  // no cleanup necessary for pPropVal
  //
  //
  // no cleanup necessary for pObject
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
HRESULT CAAFTypeDefObjectRef::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefObjectRef)) 
    { 
        *ppvObj = (IAAFTypeDefObjectRef *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefObjectRef)


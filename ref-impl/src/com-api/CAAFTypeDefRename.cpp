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



#include "CAAFTypeDefRename.h"
#include "ImplAAFTypeDefRename.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

// CLSID for AAFTypeDefRename 
// {307e9e69-dcb2-11d2-8429-00600832acb8}
EXTERN_C const CLSID CLSID_AAFTypeDefRename = { 0x307e9e69, 0xdcb2, 0x11d2, { 0x84, 0x29, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CAAFTypeDefRename::CAAFTypeDefRename (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefRename * newRep;
      newRep = new ImplAAFTypeDefRename;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefRename::~CAAFTypeDefRename ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefRename::Initialize (aafUID_constref  id,
        IAAFTypeDef * pBaseType,
        aafCharacter_constptr  pTypeName)
{
  HRESULT hr;

  ImplAAFTypeDefRename * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRename*> (pO);
  assert (ptr);

  //
  // set up for pBaseType
  //
  ImplAAFTypeDef * internalpBaseType = NULL;
  if (pBaseType)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pBaseType->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpBaseType = static_cast<ImplAAFTypeDef*>(arg);
      assert (internalpBaseType);
    }

  try
    {
      hr = ptr->Initialize
       (id,
        internalpBaseType,
        pTypeName);
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
  // no cleanup necessary for pBaseType
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefRename::GetBaseType (IAAFTypeDef ** ppBaseType)
{
  HRESULT hr;

  ImplAAFTypeDefRename * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRename*> (pO);
  assert (ptr);

  //
  // set up for ppBaseType
  //
  ImplAAFTypeDef * internalppBaseType = NULL;
  ImplAAFTypeDef ** pinternalppBaseType = NULL;
  if (ppBaseType)
    {
      pinternalppBaseType = &internalppBaseType;
    }

  try
    {
      hr = ptr->GetBaseType
       (pinternalppBaseType);
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
  // cleanup for ppBaseType
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppBaseType)
        {
          pUnknown = static_cast<IUnknown *> (internalppBaseType->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFTypeDef, (void **)ppBaseType);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppBaseType->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefRename::GetBaseValue (IAAFPropertyValue * pInPropVal,
        IAAFPropertyValue ** ppOutPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefRename * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRename*> (pO);
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
      hr = ptr->GetBaseValue
       (internalpInPropVal,
        pinternalppOutPropVal);
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
    CAAFTypeDefRename::CreateValue (IAAFPropertyValue * pInPropVal,
        IAAFPropertyValue ** ppOutPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefRename * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRename*> (pO);
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
      hr = ptr->CreateValue
       (internalpInPropVal,
        pinternalppOutPropVal);
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



//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFTypeDefRename::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefRename)) 
    { 
        *ppvObj = (IAAFTypeDefRename *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefRename)


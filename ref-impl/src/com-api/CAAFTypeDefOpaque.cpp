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



#include "CAAFTypeDefOpaque.h"
#include "ImplAAFTypeDefOpaque.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"


// CLSID for AAFTypeDefOpaque 
// {FE717CAB-F04C-11d3-800B-00104BC9156D}
EXTERN_C const CLSID CLSID_AAFTypeDefOpaque = { 0xFE717CAB, 0xF04C, 0x11d3, { 0x80, 0x0B, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CAAFTypeDefOpaque::CAAFTypeDefOpaque (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDefIndirect (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefOpaque * newRep;
      newRep = new ImplAAFTypeDefOpaque;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefOpaque::~CAAFTypeDefOpaque ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefOpaque::GetActualTypeID (IAAFPropertyValue * pOpaquePropertyValue,
        aafUID_t *  pActualTypeID)
{
  HRESULT hr;

  ImplAAFTypeDefOpaque * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefOpaque*> (pO);
  assert (ptr);

  //
  // set up for pOpaquePropertyValue
  //
  ImplAAFPropertyValue * internalpOpaquePropertyValue = NULL;
  if (pOpaquePropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pOpaquePropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpOpaquePropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpOpaquePropertyValue);
    }

  try
    {
      hr = ptr->GetActualTypeID
       (internalpOpaquePropertyValue,
        pActualTypeID);
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
  // no cleanup necessary for pOpaquePropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefOpaque::GetHandle (IAAFPropertyValue * pPropVal,
        aafUInt32  handleSize,
        aafDataBuffer_t  pHandle,
        aafUInt32*  bytesRead)
{
  HRESULT hr;

  ImplAAFTypeDefOpaque * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefOpaque*> (pO);
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
      hr = ptr->GetHandle
       (internalpPropVal,
        handleSize,
        pHandle,
        bytesRead);
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
    CAAFTypeDefOpaque::GetHandleBufLen (IAAFPropertyValue * pPropVal,
        aafUInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFTypeDefOpaque * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefOpaque*> (pO);
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
      hr = ptr->GetHandleBufLen
       (internalpPropVal,
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
  // no cleanup necessary for pPropVal
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefOpaque::SetHandle (IAAFPropertyValue * pPropVal,
        aafUInt32  handleSize,
        aafDataBuffer_t  pHandle)
{
  HRESULT hr;

  ImplAAFTypeDefOpaque * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefOpaque*> (pO);
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
      hr = ptr->SetHandle
       (internalpPropVal,
        handleSize,
        pHandle);
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
    CAAFTypeDefOpaque::CreateValueFromHandle (aafMemPtr_t  pInitData,
        aafUInt32  initDataSize,
        IAAFPropertyValue ** ppOpaquePropertyValue)
{
  HRESULT hr;

  ImplAAFTypeDefOpaque * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefOpaque*> (pO);
  assert (ptr);

  //
  // set up for ppOpaquePropertyValue
  //
  ImplAAFPropertyValue * internalppOpaquePropertyValue = NULL;
  ImplAAFPropertyValue ** pinternalppOpaquePropertyValue = NULL;
  if (ppOpaquePropertyValue)
    {
      pinternalppOpaquePropertyValue = &internalppOpaquePropertyValue;
    }

  try
    {
      hr = ptr->CreateValueFromHandle
       (pInitData,
        initDataSize,
        pinternalppOpaquePropertyValue);
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
  // cleanup for ppOpaquePropertyValue
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppOpaquePropertyValue)
        {
          pUnknown = static_cast<IUnknown *> (internalppOpaquePropertyValue->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppOpaquePropertyValue);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppOpaquePropertyValue->ReleaseReference(); // We are through with this pointer.
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
HRESULT CAAFTypeDefOpaque::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefOpaque)) 
    { 
        *ppvObj = (IAAFTypeDefOpaque *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDefIndirect::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefOpaque)


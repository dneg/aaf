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



#include "CAAFTaggedValue.h"
#include "ImplAAFTaggedValue.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFTypeDef.h"
#include "ImplAAFTypeDef.h"

// CLSID for AAFTaggedValue 
// {e684d761-b935-11d2-bf9d-00104bc9156d}
EXTERN_C const CLSID CLSID_AAFTaggedValue = { 0xe684d761, 0xb935, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };





CAAFTaggedValue::CAAFTaggedValue (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFObject (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTaggedValue * newRep;
      newRep = new ImplAAFTaggedValue;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTaggedValue::~CAAFTaggedValue ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTaggedValue::Initialize (aafCharacter_constptr  pName,
        IAAFTypeDef * pTypeDef,
        aafUInt32  valueSize,
        aafDataBuffer_t  pValue)
{
  HRESULT hr;

  ImplAAFTaggedValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTaggedValue*> (pO);
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
       (pName,
        internalpTypeDef,
        valueSize,
        pValue);
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
  // no cleanup necessary for pTypeDef
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTaggedValue::GetName (aafCharacter *  pName,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFTaggedValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTaggedValue*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetName
       (pName,
        bufSize);
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
    CAAFTaggedValue::GetNameBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFTaggedValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTaggedValue*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetNameBufLen
       (pBufSize);
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
    CAAFTaggedValue::GetTypeDefinition (IAAFTypeDef ** ppTypeDef)
{
  HRESULT hr;

  ImplAAFTaggedValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTaggedValue*> (pO);
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
      hr = ptr->GetTypeDefinition
       (pinternalppTypeDef);
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
    CAAFTaggedValue::GetValue (aafUInt32  valueSize,
        aafDataBuffer_t  pValue,
        aafUInt32 *  bytesRead)
{
  HRESULT hr;

  ImplAAFTaggedValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTaggedValue*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetValue
       (valueSize,
        pValue,
        bytesRead);
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
    CAAFTaggedValue::GetValueBufLen (aafUInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFTaggedValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTaggedValue*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetValueBufLen
       (pLen);
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
    CAAFTaggedValue::SetValue (aafUInt32  valueSize,
        aafDataBuffer_t  pValue)
{
  HRESULT hr;

  ImplAAFTaggedValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTaggedValue*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetValue
       (valueSize,
        pValue);
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


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFTaggedValue::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTaggedValue)) 
    { 
        *ppvObj = (IAAFTaggedValue *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFObject::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTaggedValue)


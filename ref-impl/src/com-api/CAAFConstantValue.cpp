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



#include "CAAFConstantValue.h"
#include "ImplAAFConstantValue.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>



#include "CAAFParameterDef.h"
#include "ImplAAFParameterDef.h"

#include "CAAFTypeDef.h"
#include "ImplAAFTypeDef.h"


// CLSID for AAFConstantValue 
// {b8ddc081-2afa-11D2-bfA5-006097116212}
EXTERN_C const CLSID CLSID_AAFConstantValue = { 0xb8ddc081, 0x2afa, 0x11D2, { 0xbf, 0xA5, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFConstantValue::CAAFConstantValue (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFParameter (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFConstantValue * newRep;
      newRep = new ImplAAFConstantValue;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFConstantValue::~CAAFConstantValue ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFConstantValue::Initialize (IAAFParameterDef * pParameterDef,
        aafUInt32  valueSize,
        aafDataBuffer_t  pValue)
{
  HRESULT hr;

  ImplAAFConstantValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFConstantValue*> (pO);
  assert (ptr);

  //
  // set up for pParameterDef
  //
  ImplAAFParameterDef * internalpParameterDef = NULL;
  if (pParameterDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pParameterDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpParameterDef = static_cast<ImplAAFParameterDef*>(arg);
      assert (internalpParameterDef);
    }

  try
    {
      hr = ptr->Initialize
       (internalpParameterDef,
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
  // no cleanup necessary for pParameterDef
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFConstantValue::GetValue (aafUInt32  valueSize,
        aafDataBuffer_t  pValue,
        aafUInt32*  bytesRead)
{
  HRESULT hr;

  ImplAAFConstantValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFConstantValue*> (pO);
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
    CAAFConstantValue::GetValueBufLen (aafUInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFConstantValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFConstantValue*> (pO);
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
    CAAFConstantValue::GetTypeDefinition (IAAFTypeDef ** ppTypeDef)
{
  HRESULT hr;

  ImplAAFConstantValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFConstantValue*> (pO);
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
    CAAFConstantValue::SetValue (aafUInt32  valueSize,
        aafDataBuffer_t  pValue)
{
  HRESULT hr;

  ImplAAFConstantValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFConstantValue*> (pO);
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
HRESULT CAAFConstantValue::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFConstantValue)) 
    { 
        *ppvObj = (IAAFConstantValue *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFParameter::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFConstantValue)


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



#include "CAAFVaryingValue.h"
#include "ImplAAFVaryingValue.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFControlPoint.h"
#include "ImplAAFControlPoint.h"

#include "CEnumAAFControlPoints.h"
#include "ImplEnumAAFControlPoints.h"

#include "CAAFParameterDef.h"
#include "ImplAAFParameterDef.h"

#include "CAAFInterpolationDef.h"
#include "ImplAAFInterpolationDef.h"


// CLSID for AAFVaryingValue 
// {91920d61-2a2e-11D2-bfA3-006097116212}
EXTERN_C const CLSID CLSID_AAFVaryingValue = { 0x91920d61, 0x2a2e, 0x11D2, { 0xbf, 0xA3, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFVaryingValue::CAAFVaryingValue (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFParameter (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFVaryingValue * newRep;
      newRep = new ImplAAFVaryingValue;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFVaryingValue::~CAAFVaryingValue ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFVaryingValue::Initialize (IAAFParameterDef * pParameterDef,
        IAAFInterpolationDef * pInterpolation)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
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
  //
  // set up for pInterpolation
  //
  ImplAAFInterpolationDef * internalpInterpolation = NULL;
  if (pInterpolation)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pInterpolation->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpInterpolation = static_cast<ImplAAFInterpolationDef*>(arg);
      assert (internalpInterpolation);
    }

  try
    {
      hr = ptr->Initialize
       (internalpParameterDef,
        internalpInterpolation);
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
  //
  // no cleanup necessary for pInterpolation
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFVaryingValue::AddControlPoint (IAAFControlPoint * pControlPoint)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
  assert (ptr);

  //
  // set up for pControlPoint
  //
  ImplAAFControlPoint * internalpControlPoint = NULL;
  if (pControlPoint)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pControlPoint->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpControlPoint = static_cast<ImplAAFControlPoint*>(arg);
      assert (internalpControlPoint);
    }

  try
    {
      hr = ptr->AddControlPoint
       (internalpControlPoint);
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
  // no cleanup necessary for pControlPoint
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFVaryingValue::GetControlPoints (IEnumAAFControlPoints ** ppEnum)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFControlPoints * internalppEnum = NULL;
  ImplEnumAAFControlPoints ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetControlPoints
       (pinternalppEnum);
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
  // cleanup for ppEnum
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEnum)
        {
          pUnknown = static_cast<IUnknown *> (internalppEnum->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IEnumAAFControlPoints, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFVaryingValue::CountControlPoints (aafUInt32 *  pResult)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountControlPoints
       (pResult);
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
    CAAFVaryingValue::GetControlPointAt (aafUInt32  index,
        IAAFControlPoint ** ppControlPoint)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
  assert (ptr);

  //
  // set up for ppControlPoint
  //
  ImplAAFControlPoint * internalppControlPoint = NULL;
  ImplAAFControlPoint ** pinternalppControlPoint = NULL;
  if (ppControlPoint)
    {
      pinternalppControlPoint = &internalppControlPoint;
    }

  try
    {
      hr = ptr->GetControlPointAt
       (index,
        pinternalppControlPoint);
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
  // cleanup for ppControlPoint
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppControlPoint)
        {
          pUnknown = static_cast<IUnknown *> (internalppControlPoint->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFControlPoint, (void **)ppControlPoint);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppControlPoint->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFVaryingValue::RemoveControlPointAt (aafUInt32  index)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->RemoveControlPointAt
       (index);
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
    CAAFVaryingValue::GetInterpolationDefinition (IAAFInterpolationDef ** ppInterpolation)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
  assert (ptr);

  //
  // set up for ppInterpolation
  //
  ImplAAFInterpolationDef * internalppInterpolation = NULL;
  ImplAAFInterpolationDef ** pinternalppInterpolation = NULL;
  if (ppInterpolation)
    {
      pinternalppInterpolation = &internalppInterpolation;
    }

  try
    {
      hr = ptr->GetInterpolationDefinition
       (pinternalppInterpolation);
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
  // cleanup for ppInterpolation
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppInterpolation)
        {
          pUnknown = static_cast<IUnknown *> (internalppInterpolation->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFInterpolationDef, (void **)ppInterpolation);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppInterpolation->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFVaryingValue::GetValueBufLen (aafInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
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
    CAAFVaryingValue::GetInterpolatedValue (aafRational_t  inputValue,
        aafInt32  valueSize,
        aafDataBuffer_t  pValue,
        aafInt32 *  bytesRead)
{
  HRESULT hr;

  ImplAAFVaryingValue * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFVaryingValue*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetInterpolatedValue
       (inputValue,
        valueSize,
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

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFVaryingValue::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFVaryingValue)) 
    { 
        *ppvObj = (IAAFVaryingValue *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFParameter::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFVaryingValue)


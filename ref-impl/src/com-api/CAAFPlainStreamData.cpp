//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "CAAFPlainStreamData.h"
#include "ImplAAFPlainStreamData.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

// CLSID for AAFPlainStreamData 
// {6760c4b4-129c-4356-8048-bd69c6aba4a4}
EXTERN_C const CLSID CLSID_AAFPlainStreamData = { 0x6760c4b4, 0x129c, 0x4356, { 0x80, 0x48, 0xbd, 0x69, 0xc6, 0xab, 0xa4, 0xa4 } };





CAAFPlainStreamData::CAAFPlainStreamData (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFPlainStreamData * newRep;
      newRep = new ImplAAFPlainStreamData;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFPlainStreamData::~CAAFPlainStreamData ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::GetSize (IAAFPropertyValue * pStreamPropertyValue,
        aafInt64 *  pSize)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->GetSize
       (internalpStreamPropertyValue,
        pSize);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::SetSize (IAAFPropertyValue * pStreamPropertyValue,
        aafInt64  newSize)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->SetSize
       (internalpStreamPropertyValue,
        newSize);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::GetPosition (IAAFPropertyValue * pStreamPropertyValue,
        aafInt64 *  pPosition)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->GetPosition
       (internalpStreamPropertyValue,
        pPosition);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::SetPosition (IAAFPropertyValue * pStreamPropertyValue,
        aafInt64  newPosition)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->SetPosition
       (internalpStreamPropertyValue,
        newPosition);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::Read (IAAFPropertyValue * pStreamPropertyValue,
        aafUInt32  dataSize,
        aafMemPtr_t  pData,
        aafUInt32 *  bytesRead)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->Read
       (internalpStreamPropertyValue,
        dataSize,
        pData,
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
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::Write (IAAFPropertyValue * pStreamPropertyValue,
        aafUInt32  dataSize,
        aafMemPtr_t  pData)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->Write
       (internalpStreamPropertyValue,
        dataSize,
        pData);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::Append (IAAFPropertyValue * pStreamPropertyValue,
        aafUInt32  dataSize,
        aafMemPtr_t  pData)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->Append
       (internalpStreamPropertyValue,
        dataSize,
        pData);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::HasStoredByteOrder (IAAFPropertyValue * pStreamPropertyValue,
        aafBoolean_t *  pHasByteOrder)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->HasStoredByteOrder
       (internalpStreamPropertyValue,
        pHasByteOrder);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::GetStoredByteOrder (IAAFPropertyValue * pStreamPropertyValue,
        eAAFByteOrder_t *  pByteOrder)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->GetStoredByteOrder
       (internalpStreamPropertyValue,
        pByteOrder);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::SetStoredByteOrder (IAAFPropertyValue * pStreamPropertyValue,
        eAAFByteOrder_t  byteOrder)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }
  //
  // set up for byteOrder
  //
  if (! Is_eAAFByteOrder_t_Valid(byteOrder))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->SetStoredByteOrder
       (internalpStreamPropertyValue,
        byteOrder);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::ClearStoredByteOrder (IAAFPropertyValue * pStreamPropertyValue)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }

  try
    {
      hr = ptr->ClearStoredByteOrder
       (internalpStreamPropertyValue);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::ReadElements (IAAFPropertyValue * pStreamPropertyValue,
        IAAFTypeDef * pElementType,
        aafUInt32  dataSize,
        aafMemPtr_t  pData,
        aafUInt32 *  pBytesRead)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }
  //
  // set up for pElementType
  //
  ImplAAFTypeDef * internalpElementType = NULL;
  if (pElementType)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pElementType->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpElementType = static_cast<ImplAAFTypeDef*>(arg);
      assert (internalpElementType);
    }

  try
    {
      hr = ptr->ReadElements
       (internalpStreamPropertyValue,
        internalpElementType,
        dataSize,
        pData,
        pBytesRead);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  //
  // no cleanup necessary for pElementType
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::WriteElements (IAAFPropertyValue * pStreamPropertyValue,
        IAAFTypeDef * pElementType,
        aafUInt32  dataSize,
        aafMemPtr_t  pData)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }
  //
  // set up for pElementType
  //
  ImplAAFTypeDef * internalpElementType = NULL;
  if (pElementType)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pElementType->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpElementType = static_cast<ImplAAFTypeDef*>(arg);
      assert (internalpElementType);
    }

  try
    {
      hr = ptr->WriteElements
       (internalpStreamPropertyValue,
        internalpElementType,
        dataSize,
        pData);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  //
  // no cleanup necessary for pElementType
  //
  return hr;
}





HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::AppendElements (IAAFPropertyValue * pStreamPropertyValue,
        IAAFTypeDef * pElementType,
        aafUInt32  dataSize,
        aafMemPtr_t  pData)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }
  //
  // set up for pElementType
  //
  ImplAAFTypeDef * internalpElementType = NULL;
  if (pElementType)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pElementType->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpElementType = static_cast<ImplAAFTypeDef*>(arg);
      assert (internalpElementType);
    }

  try
    {
      hr = ptr->AppendElements
       (internalpStreamPropertyValue,
        internalpElementType,
        dataSize,
        pData);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //
  //
  // no cleanup necessary for pElementType
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::GetEssenceElementKey (IAAFPropertyValue * pStreamPropertyValue,
        aafUID_t *  pEssenceElementKey)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }


  try
    {
      hr = ptr->GetEssenceElementKey (internalpStreamPropertyValue,
    pEssenceElementKey);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
  //


  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPlainStreamData::SetEssenceElementKey (IAAFPropertyValue * pStreamPropertyValue,
        aafUID_constref  key)
{
  HRESULT hr;

  ImplAAFPlainStreamData * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPlainStreamData*> (pO);
  assert (ptr);

  //
  // set up for pStreamPropertyValue
  //
  ImplAAFPropertyValue * internalpStreamPropertyValue = NULL;
  if (pStreamPropertyValue)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStreamPropertyValue->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStreamPropertyValue = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpStreamPropertyValue);
    }


  try
    {
      hr = ptr->SetEssenceElementKey (internalpStreamPropertyValue,
    key);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UHANDLED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNHANDLED_EXCEPTION);
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
  // no cleanup necessary for pStreamPropertyValue
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
HRESULT CAAFPlainStreamData::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFPlainStreamData)) 
    { 
        *ppvObj = (IAAFPlainStreamData *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    if (EQUAL_UID(riid,IID_IAAFKLVStreamParameters)) 
    { 
        *ppvObj = (IAAFKLVStreamParameters *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFPlainStreamData)

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



#include "CAAFTypeDefStream.h"
#include "ImplAAFTypeDefStream.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

// CLSID for AAFTypeDefStream 
// {307e9e6d-dcb2-11d2-8429-00600832acb8}
EXTERN_C const CLSID CLSID_AAFTypeDefStream = { 0x307e9e6d, 0xdcb2, 0x11d2, { 0x84, 0x29, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CAAFTypeDefStream::CAAFTypeDefStream (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefStream * newRep;
      newRep = new ImplAAFTypeDefStream;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefStream::~CAAFTypeDefStream ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefStream::GetSize (IAAFPropertyValue * pStreamPropertyValue,
        aafInt64 *  pSize)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::SetSize (IAAFPropertyValue * pStreamPropertyValue,
        aafInt64  newSize)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::GetPosition (IAAFPropertyValue * pStreamPropertyValue,
        aafInt64 *  pPosition)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::SetPosition (IAAFPropertyValue * pStreamPropertyValue,
        aafInt64  newPosition)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::Read (IAAFPropertyValue * pStreamPropertyValue,
        aafUInt32  dataSize,
        aafMemPtr_t  pData,
        aafUInt32 *  bytesRead)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::Write (IAAFPropertyValue * pStreamPropertyValue,
        aafUInt32  dataSize,
        aafMemPtr_t  pData)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::Append (IAAFPropertyValue * pStreamPropertyValue,
        aafUInt32  dataSize,
        aafMemPtr_t  pData)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::HasStoredByteOrder (IAAFPropertyValue * pStreamPropertyValue,
        aafBoolean_t *  pHasByteOrder)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::GetStoredByteOrder (IAAFPropertyValue * pStreamPropertyValue,
        eAAFByteOrder_t *  pByteOrder)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::SetStoredByteOrder (IAAFPropertyValue * pStreamPropertyValue,
        eAAFByteOrder_t  byteOrder)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::ClearStoredByteOrder (IAAFPropertyValue * pStreamPropertyValue)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::ReadElements (IAAFPropertyValue * pStreamPropertyValue,
        IAAFTypeDef * pElementType,
        aafUInt32  dataSize,
        aafMemPtr_t  pData,
        aafUInt32 *  pBytesRead)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::WriteElements (IAAFPropertyValue * pStreamPropertyValue,
        IAAFTypeDef * pElementType,
        aafUInt32  dataSize,
        aafMemPtr_t  pData)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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
    CAAFTypeDefStream::AppendElements (IAAFPropertyValue * pStreamPropertyValue,
        IAAFTypeDef * pElementType,
        aafUInt32  dataSize,
        aafMemPtr_t  pData)
{
  HRESULT hr;

  ImplAAFTypeDefStream * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStream*> (pO);
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


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFTypeDefStream::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefStream)) 
    { 
        *ppvObj = (IAAFTypeDefStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefStream)


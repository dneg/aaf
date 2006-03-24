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



#include "CAAFBWFImportDescriptor.h"
#include "ImplAAFBWFImportDescriptor.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>



// CLSID for AAFBWFImportDescriptor 
// {dc86b358-86a6-4d2d-8b09-eff3ce3e66db}
EXTERN_C const CLSID CLSID_AAFBWFImportDescriptor = { 0xdc86b358, 0x86a6, 0x4d2d, { 0x8b, 0x09, 0xef, 0xf3, 0xce, 0x3e, 0x66, 0xdb } };






CAAFBWFImportDescriptor::CAAFBWFImportDescriptor (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFImportDescriptor (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFBWFImportDescriptor * newRep;
      newRep = new ImplAAFBWFImportDescriptor;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFBWFImportDescriptor::~CAAFBWFImportDescriptor ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFBWFImportDescriptor::Initialize ()
{
  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Initialize();
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
    CAAFBWFImportDescriptor::SetFileSecurityReport (aafUInt32  fileSecurityReport)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetFileSecurityReport
       (fileSecurityReport);
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
    CAAFBWFImportDescriptor::GetFileSecurityReport (aafUInt32 *  pFileSecurityReport)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetFileSecurityReport
       (pFileSecurityReport);
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
    CAAFBWFImportDescriptor::SetFileSecurityWave (aafUInt32  fileSecurityWave)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetFileSecurityWave
       (fileSecurityWave);
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
    CAAFBWFImportDescriptor::GetFileSecurityWave (aafUInt32 *  pFileSecurityWave)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetFileSecurityWave
       (pFileSecurityWave);
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
    CAAFBWFImportDescriptor::AppendUnknownBWFChunks (IAAFRIFFChunk * pData)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);

  //
  // set up for pData
  //
  ImplAAFRIFFChunk * internalpData = NULL;
  if (pData)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pData->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpData = static_cast<ImplAAFRIFFChunk*>(arg);
      assert (internalpData);
    }

  try
    {
      hr = ptr->AppendUnknownBWFChunks
       (internalpData);
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
  // no cleanup necessary for pData
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFBWFImportDescriptor::CountUnknownBWFChunks (aafUInt32 *  pNumData)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountUnknownBWFChunks
       (pNumData);
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
    CAAFBWFImportDescriptor::GetUnknownBWFChunks (IEnumAAFRIFFChunks ** ppEnum)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFRIFFChunks * internalppEnum = NULL;
  ImplEnumAAFRIFFChunks ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetUnknownBWFChunks
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFRIFFChunks, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFBWFImportDescriptor::RemoveUnknownBWFChunks (IAAFRIFFChunk * pData)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);

  //
  // set up for pData
  //
  ImplAAFRIFFChunk * internalpData = NULL;
  if (pData)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pData->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpData = static_cast<ImplAAFRIFFChunk*>(arg);
      assert (internalpData);
    }

  try
    {
      hr = ptr->RemoveUnknownBWFChunks
       (internalpData);
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
  // no cleanup necessary for pData
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFBWFImportDescriptor::SetBextCodingHistory (aafCharacter_constptr  pBextCodingHistory)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetBextCodingHistory
       (pBextCodingHistory);
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
    CAAFBWFImportDescriptor::GetBextCodingHistory (aafCharacter *  pBextCodingHistory,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetBextCodingHistory
       (pBextCodingHistory,
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
    CAAFBWFImportDescriptor::GetBextCodingHistoryBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetBextCodingHistoryBufLen
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
    CAAFBWFImportDescriptor::SetQltyBasicData (aafCharacter_constptr  pQltyBasicData)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetQltyBasicData
       (pQltyBasicData);
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
    CAAFBWFImportDescriptor::GetQltyBasicData (aafCharacter *  pQltyBasicData,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyBasicData
       (pQltyBasicData,
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
    CAAFBWFImportDescriptor::GetQltyBasicDataBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyBasicDataBufLen
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
    CAAFBWFImportDescriptor::SetQltyStartOfModulation (aafCharacter_constptr  pQltyStartOfModulation)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetQltyStartOfModulation
       (pQltyStartOfModulation);
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
    CAAFBWFImportDescriptor::GetQltyStartOfModulation (aafCharacter *  pQltyStartOfModulation,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyStartOfModulation
       (pQltyStartOfModulation,
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
    CAAFBWFImportDescriptor::GetQltyStartOfModulationBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyStartOfModulationBufLen
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
    CAAFBWFImportDescriptor::SetQltyQualityEvent (aafCharacter_constptr  pQltyQualityEvent)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetQltyQualityEvent
       (pQltyQualityEvent);
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
    CAAFBWFImportDescriptor::GetQltyQualityEvent (aafCharacter *  pQltyQualityEvent,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyQualityEvent
       (pQltyQualityEvent,
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
    CAAFBWFImportDescriptor::GetQltyQualityEventBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyQualityEventBufLen
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
    CAAFBWFImportDescriptor::SetQltyEndOfModulation (aafCharacter_constptr  pQltyEndOfModulation)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetQltyEndOfModulation
       (pQltyEndOfModulation);
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
    CAAFBWFImportDescriptor::GetQltyEndOfModulation (aafCharacter *  pQltyEndOfModulation,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyEndOfModulation
       (pQltyEndOfModulation,
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
    CAAFBWFImportDescriptor::GetQltyEndOfModulationBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyEndOfModulationBufLen
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
    CAAFBWFImportDescriptor::SetQltyQualityParameter (aafCharacter_constptr  pQltyQualityParameter)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetQltyQualityParameter
       (pQltyQualityParameter);
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
    CAAFBWFImportDescriptor::GetQltyQualityParameter (aafCharacter *  pQltyQualityParameter,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyQualityParameter
       (pQltyQualityParameter,
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
    CAAFBWFImportDescriptor::GetQltyQualityParameterBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyQualityParameterBufLen
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
    CAAFBWFImportDescriptor::SetQltyOperatorComment (aafCharacter_constptr  pQltyOperatorComment)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetQltyOperatorComment
       (pQltyOperatorComment);
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
    CAAFBWFImportDescriptor::GetQltyOperatorComment (aafCharacter *  pQltyOperatorComment,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyOperatorComment
       (pQltyOperatorComment,
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
    CAAFBWFImportDescriptor::GetQltyOperatorCommentBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyOperatorCommentBufLen
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
    CAAFBWFImportDescriptor::SetQltyCueSheet (aafCharacter_constptr  pQltyCueSheet)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetQltyCueSheet
       (pQltyCueSheet);
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
    CAAFBWFImportDescriptor::GetQltyCueSheet (aafCharacter *  pQltyCueSheet,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyCueSheet
       (pQltyCueSheet,
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
    CAAFBWFImportDescriptor::GetQltyCueSheetBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFBWFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFBWFImportDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetQltyCueSheetBufLen
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


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFBWFImportDescriptor::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFBWFImportDescriptor)) 
    { 
        *ppvObj = (IAAFBWFImportDescriptor *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFImportDescriptor::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFBWFImportDescriptor)


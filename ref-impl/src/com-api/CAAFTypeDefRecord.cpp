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



#include "CAAFTypeDefRecord.h"
#include "ImplAAFTypeDefRecord.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

#include "CEnumAAFPropertyValues.h"
#include "ImplEnumAAFPropertyValues.h"

// CLSID for AAFTypeDefRecord 
// {809c8ca5-9884-11d2-841f-00600832acb8}
EXTERN_C const CLSID CLSID_AAFTypeDefRecord = { 0x809c8ca5, 0x9884, 0x11d2, { 0x84, 0x1f, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CAAFTypeDefRecord::CAAFTypeDefRecord (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefRecord * newRep;
      newRep = new ImplAAFTypeDefRecord;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefRecord::~CAAFTypeDefRecord ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefRecord::Initialize (aafUID_constref  id,
        IAAFTypeDef ** ppMemberTypes,
        aafString_t *  pMemberNames,
        aafUInt32  numMembers,
        aafCharacter_constptr  pTypeName)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
  assert (ptr);

  //
  // set up for ppMemberTypes
  //
  ImplAAFTypeDef ** internalppMemberTypes = NULL;
  if (ppMemberTypes)
    {
      aafUInt32 localIdx;
	  assert (numMembers >= 0);
	  internalppMemberTypes = new ImplAAFTypeDef*[numMembers];
	  assert (internalppMemberTypes);

	  for (localIdx = 0; localIdx < numMembers; localIdx++)
		{
		  HRESULT hStat;
		  IAAFRoot * iObj;
		  ImplAAFRoot *arg;
		  hStat = ppMemberTypes[localIdx]->QueryInterface (IID_IAAFRoot, (void **)&iObj);
		  assert (SUCCEEDED (hStat));
		  assert (iObj);
		  hStat = iObj->GetImplRep((void **)&arg);
		  assert (SUCCEEDED (hStat));
		  iObj->Release(); // we are through with this interface pointer.
		  internalppMemberTypes[localIdx] = static_cast<ImplAAFTypeDef*>(arg);
		  assert (internalppMemberTypes[localIdx]);
		}
    }

  try
    {
      hr = ptr->Initialize
       (id,
        internalppMemberTypes,
        pMemberNames,
        numMembers,
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
  // cleanup for ppMemberTypes
  //
  if (internalppMemberTypes)
    {
      delete[] internalppMemberTypes;
      internalppMemberTypes = 0;
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefRecord::GetMemberType (aafUInt32  index,
        IAAFTypeDef ** ppTypeDef)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
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
      hr = ptr->GetMemberType
       (index,
        pinternalppTypeDef);
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
    CAAFTypeDefRecord::GetMemberName (aafUInt32  index,
        aafCharacter *  pName,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetMemberName
       (index,
        pName,
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
    CAAFTypeDefRecord::GetMemberNameBufLen (aafUInt32  index,
        aafUInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetMemberNameBufLen
       (index,
        pLen);
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
    CAAFTypeDefRecord::CreateValueFromValues (IAAFPropertyValue ** pMemberValues,
        aafUInt32  numMembers,
        IAAFPropertyValue ** ppPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
  assert (ptr);

  //
  // set up for pMemberValues
  //
  ImplAAFPropertyValue ** internalpMemberValues = NULL;
  if (pMemberValues)
    {
      aafUInt32 localIdx;
	  assert (numMembers >= 0);
	  internalpMemberValues = new ImplAAFPropertyValue*[numMembers];
	  assert (internalpMemberValues);

	  for (localIdx = 0; localIdx < numMembers; localIdx++)
		{
		  HRESULT hStat;
		  IAAFRoot * iObj;
		  ImplAAFRoot *arg;
		  hStat = pMemberValues[localIdx]->QueryInterface (IID_IAAFRoot, (void **)&iObj);
		  assert (SUCCEEDED (hStat));
		  assert (iObj);
		  hStat = iObj->GetImplRep((void **)&arg);
		  assert (SUCCEEDED (hStat));
		  iObj->Release(); // we are through with this interface pointer.
		  internalpMemberValues[localIdx] = static_cast<ImplAAFPropertyValue*>(arg);
		  assert (internalpMemberValues[localIdx]);
		}
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
      hr = ptr->CreateValueFromValues
       (internalpMemberValues,
        numMembers,
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
  // cleanup for pMemberValues
  //
  if (internalpMemberValues)
    {
      delete[] internalpMemberValues;
      internalpMemberValues = 0;
    }
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
    CAAFTypeDefRecord::CreateValueFromStruct (aafMemPtr_t  pInitData,
        aafUInt32  initDataSize,
        IAAFPropertyValue ** ppPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
  assert (ptr);

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
      hr = ptr->CreateValueFromStruct
       (pInitData,
        initDataSize,
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
    CAAFTypeDefRecord::GetValue (IAAFPropertyValue * pInPropVal,
        aafUInt32  index,
        IAAFPropertyValue ** ppOutPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
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
      hr = ptr->GetValue
       (internalpInPropVal,
        index,
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
    CAAFTypeDefRecord::GetStruct (IAAFPropertyValue * pPropVal,
        aafMemPtr_t  pData,
        aafUInt32  dataSize)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
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
      hr = ptr->GetStruct
       (internalpPropVal,
        pData,
        dataSize);
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
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefRecord::SetValue (IAAFPropertyValue * pPropVal,
        aafUInt32  index,
        IAAFPropertyValue * pMemberPropVal)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
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
  // set up for pMemberPropVal
  //
  ImplAAFPropertyValue * internalpMemberPropVal = NULL;
  if (pMemberPropVal)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pMemberPropVal->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpMemberPropVal = static_cast<ImplAAFPropertyValue*>(arg);
      assert (internalpMemberPropVal);
    }

  try
    {
      hr = ptr->SetValue
       (internalpPropVal,
        index,
        internalpMemberPropVal);
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
  // no cleanup necessary for pMemberPropVal
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefRecord::SetStruct (IAAFPropertyValue * pPropVal,
        aafMemPtr_t  pData,
        aafUInt32  dataSize)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
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
      hr = ptr->SetStruct
       (internalpPropVal,
        pData,
        dataSize);
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
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTypeDefRecord::GetCount (aafUInt32 *  pCount)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetCount
       (pCount);
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
    CAAFTypeDefRecord::RegisterMembers (aafUInt32 *  pOffsets,
        aafUInt32  numMembers,
        aafUInt32  structSize)
{
  HRESULT hr;

  ImplAAFTypeDefRecord * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefRecord*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->RegisterMembers
       (pOffsets,
        numMembers,
        structSize);
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
HRESULT CAAFTypeDefRecord::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefRecord)) 
    { 
        *ppvObj = (IAAFTypeDefRecord *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefRecord)


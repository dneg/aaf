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



#include "CAAFClassDef.h"
#include "ImplAAFClassDef.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>



#include "CAAFObject.h"
#include "ImplAAFObject.h"

#include "CEnumAAFPropertyDefs.h"
#include "ImplEnumAAFPropertyDefs.h"

#include "CAAFPropertyDef.h"
#include "ImplAAFPropertyDef.h"

#include "CAAFMetaDefinition.h"
#include "ImplAAFMetaDefinition.h"

#include "CAAFTypeDef.h"
#include "ImplAAFTypeDef.h"



// CLSID for AAFClassDef 
// {dfbd6526-1d81-11d2-bf96-006097116212}
EXTERN_C const CLSID CLSID_AAFClassDef = { 0xdfbd6526, 0x1d81, 0x11d2, { 0xbf, 0x96, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFClassDef::CAAFClassDef (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFMetaDefinition (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFClassDef * newRep;
      newRep = new ImplAAFClassDef;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFClassDef::~CAAFClassDef ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFClassDef::Initialize (aafUID_constref  classID,
        IAAFClassDef * pParentClass,
        aafCharacter_constptr  pClassName,
        aafBoolean_t  isConcrete)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);

  //
  // set up for pParentClass
  //
  ImplAAFClassDef * internalpParentClass = NULL;
  if (pParentClass)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pParentClass->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpParentClass = static_cast<ImplAAFClassDef*>(arg);
      assert (internalpParentClass);
    }

  try
    {
      hr = ptr->Initialize
       (classID,
        internalpParentClass,
        pClassName,
        isConcrete);
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
  // no cleanup necessary for pParentClass
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFClassDef::GetPropertyDefs (IEnumAAFPropertyDefs ** ppEnum)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFPropertyDefs * internalppEnum = NULL;
  ImplEnumAAFPropertyDefs ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetPropertyDefs
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFPropertyDefs, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFClassDef::CountPropertyDefs (aafUInt32 *  pCount)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountPropertyDefs
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
    CAAFClassDef::RegisterNewPropertyDef (aafUID_constref  id,
        aafCharacter_constptr  pName,
        IAAFTypeDef * pTypeDef,
        aafBoolean_t  isOptional,
        aafBoolean_t  isUniqueIdentifier,
        IAAFPropertyDef ** ppPropDef)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
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
  //
  // set up for isOptional
  //
  if (! Is_aafBoolean_t_Valid(isOptional))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for isUniqueIdentifier
  //
  if (! Is_aafBoolean_t_Valid(isUniqueIdentifier))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for ppPropDef
  //
  ImplAAFPropertyDef * internalppPropDef = NULL;
  ImplAAFPropertyDef ** pinternalppPropDef = NULL;
  if (ppPropDef)
    {
      pinternalppPropDef = &internalppPropDef;
    }

  try
    {
      hr = ptr->RegisterNewPropertyDef
       (id,
        pName,
        internalpTypeDef,
        isOptional,
        isUniqueIdentifier,
        pinternalppPropDef);
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
  //
  // cleanup for ppPropDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPropDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppPropDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyDef, (void **)ppPropDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPropDef->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFClassDef::RegisterOptionalPropertyDef (aafUID_constref  id,
        aafCharacter_constptr  pName,
        IAAFTypeDef * pTypeDef,
        IAAFPropertyDef ** ppPropDef)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
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
  //
  // set up for ppPropDef
  //
  ImplAAFPropertyDef * internalppPropDef = NULL;
  ImplAAFPropertyDef ** pinternalppPropDef = NULL;
  if (ppPropDef)
    {
      pinternalppPropDef = &internalppPropDef;
    }

  try
    {
      hr = ptr->RegisterOptionalPropertyDef
       (id,
        pName,
        internalpTypeDef,
        pinternalppPropDef);
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
  //
  // cleanup for ppPropDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPropDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppPropDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyDef, (void **)ppPropDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPropDef->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFClassDef::LookupPropertyDef (aafUID_constref  propID,
        IAAFPropertyDef ** ppPropDef)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);

  //
  // set up for ppPropDef
  //
  ImplAAFPropertyDef * internalppPropDef = NULL;
  ImplAAFPropertyDef ** pinternalppPropDef = NULL;
  if (ppPropDef)
    {
      pinternalppPropDef = &internalppPropDef;
    }

  try
    {
      hr = ptr->LookupPropertyDef
       (propID,
        pinternalppPropDef);
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
  // cleanup for ppPropDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPropDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppPropDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyDef, (void **)ppPropDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPropDef->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFClassDef::GetName (aafCharacter *  pName,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
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
    CAAFClassDef::GetNameBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
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
    CAAFClassDef::GetParent (IAAFClassDef ** ppClassDef)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);

  //
  // set up for ppClassDef
  //
  ImplAAFClassDef * internalppClassDef = NULL;
  ImplAAFClassDef ** pinternalppClassDef = NULL;
  if (ppClassDef)
    {
      pinternalppClassDef = &internalppClassDef;
    }

  try
    {
      hr = ptr->GetParent
       (pinternalppClassDef);
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
  // cleanup for ppClassDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppClassDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppClassDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFClassDef, (void **)ppClassDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppClassDef->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFClassDef::IsConcrete (aafBoolean_t*  pResult)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->IsConcrete
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
    CAAFClassDef::IsRoot (aafBoolean_t*  isRoot)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->IsRoot
       (isRoot);
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
    CAAFClassDef::IsUniquelyIdentified (aafBoolean_t*  pIsUniquelyIdentified)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->IsUniquelyIdentified
       (pIsUniquelyIdentified);
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
    CAAFClassDef::GetUniqueIdentifier (IAAFPropertyDef ** ppUniqueIdentifier)
{
  HRESULT hr;

  ImplAAFClassDef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFClassDef*> (pO);
  assert (ptr);

  //
  // set up for ppUniqueIdentifier
  //
  ImplAAFPropertyDef * internalppUniqueIdentifier = NULL;
  ImplAAFPropertyDef ** pinternalppUniqueIdentifier = NULL;
  if (ppUniqueIdentifier)
    {
      pinternalppUniqueIdentifier = &internalppUniqueIdentifier;
    }

  try
    {
      hr = ptr->GetUniqueIdentifier
       (pinternalppUniqueIdentifier);
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
  // cleanup for ppUniqueIdentifier
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppUniqueIdentifier)
        {
          pUnknown = static_cast<IUnknown *> (internalppUniqueIdentifier->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyDef, (void **)ppUniqueIdentifier);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppUniqueIdentifier->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



//
// Creates an object of this class, and returns it by reference in
  // the location specified by the ppObject argument.
  // 
  // Succeeds if:
  // - The ppObject pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppObject arg is NULL.
HRESULT CAAFClassDef::CreateInstance
   (// Reference to the identifier of the interface
    /*[in, ref]*/ REFIID riid,

    // Address of output variable that receives the 
    // interface pointer requested in riid
    /*[out, iid_is(riid)]*/ IUnknown ** ppvObject)
{
  HRESULT hr = S_OK;
  AAFRESULT result = AAFRESULT_SUCCESS;

  // Validate the input arguments.
  if (NULL == ppvObject)
    return E_INVALIDARG;

  // Find the factory on the corresponding impl class.
  ImplAAFRoot * pImplRoot = GetRepObject();
  assert(pImplRoot);
  ImplAAFClassDef * pImplAAFClassDef = dynamic_cast<ImplAAFClassDef *>(pImplRoot);
  if (NULL == pImplAAFClassDef)
  { // This should never happen.
    return AAFRESULT_INTERNAL_ERROR;
  }

  // Get the class id and make sure then this is not a private class...
  aafUID_t id;
  result = pImplAAFClassDef->GetAUID(&id);
  if (AAFRESULT_SUCCESS != result)
    return result;
      
  
  // Make sure that the client fails when they attempt to create a "private"
  // toolkit object instance.
  const AAFObjectModel * pObjectModel = AAFObjectModel::singleton();
  assert(pObjectModel); // this is statically contructed so it should not fail.
  const ClassDefinition * classDefinition = pObjectModel->findClassDefinition(&id);
  if (NULL != classDefinition)
  {
    if (classDefinition->privateClass())
      return AAFRESULT_INVALID_CLASS_ID;
  }


  // Create the corresponding impl object.
  ImplAAFObject *pObject = NULL;
  result = pImplAAFClassDef->CreateInstance(&pObject);
  if (AAFRESULT_SUCCESS != result)
    return result;

  // Get the COM container for the new Impl object and see if
  // it supports the requested interface pointer.
  IUnknown *pUnknown = static_cast<IUnknown *>(pObject->GetContainer());
  assert(pUnknown);
  hr = pUnknown->QueryInterface(riid, (void **)ppvObject);
  
  // Release the impl object pointer. If the previous QI failed
  // then the object will be deleted.
  pObject->ReleaseReference();

  return hr;
}

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFClassDef::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFClassDef)) 
    { 
        *ppvObj = (IAAFClassDef *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFMetaDefinition::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFClassDef)


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



#include "CAAFObject.h"
#include "ImplAAFObject.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>



#include "CAAFClassDef.h"
#include "ImplAAFClassDef.h"

#include "CAAFDictionary.h"
#include "ImplAAFDictionary.h"

#include "CAAFIdentification.h"
#include "ImplAAFIdentification.h"

#include "CAAFPropertyDef.h"
#include "ImplAAFPropertyDef.h"

#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

#include "CEnumAAFProperties.h"
#include "ImplEnumAAFProperties.h"


// CLSID for AAFObject 
// {B1A213AC-1A7D-11D2-BF78-00104BC9156D}
EXTERN_C const CLSID CLSID_AAFObject = { 0xB1A213AC, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };




// Private extension wrapper.
CAAFObject::Extension::Extension(REFCLSID rclsid) :
  pNext(NULL),
  clsid(rclsid),
  pExtensionUnknown(NULL),
  bExtensionQueryInterfaceInProgress(false)
{
}

CAAFObject::Extension::~Extension() 
{
  if (NULL != pExtensionUnknown)
  {
    pExtensionUnknown->Release();
    pExtensionUnknown = NULL;
  }
}

// Return the class id of this extension.
REFCLSID CAAFObject::Extension::GetCLSID(void) const
{ 
  return clsid;
}

// Set or get the next extension.
void CAAFObject::Extension::SetNext(CAAFObject::Extension *next)
{ 
  pNext = next;
}

CAAFObject::Extension * CAAFObject::Extension::GetNext(void) const
{ 
  return pNext;
}


// Attempt to create the given plugin extension.
HRESULT CAAFObject::Extension::InitializeExtension(IUnknown *pControllingUnknown)
{
  assert(NULL == pExtensionUnknown); // precondition.
  HRESULT hr = S_OK;
  IAAFPluginManager *pPluginMgr = NULL;


  try
  {
    // Get the plugin manager to create the plugin instance.
    hr = ::AAFGetPluginManager(&pPluginMgr);
    if (SUCCEEDED(hr))
    {
      hr = pPluginMgr->CreateInstance(clsid, 
                                      pControllingUnknown, 
                                      IID_IUnknown, 
                                      (void **)&pExtensionUnknown);

      if (pPluginMgr)
        pPluginMgr->Release();
    }
  }
  catch (...)
  {
    // cleanup
    if (pPluginMgr)
        pPluginMgr->Release();

    throw;
  }


  return hr;
}

HRESULT CAAFObject::Extension::QueryInterface(REFIID riid, void **ppvObjOut)
{
  assert(NULL != pExtensionUnknown); // precondition.
  HRESULT hr = E_NOINTERFACE;

  // We use state variable to prevent the endless cycle
  // that would occur if the extension's QueryInterface
  // called it's controlling IUnknown's QueryInterface
  // (i.e. CAAFObject::InternalQueryInterface)
  // for an interface that is not supported for a built-in
  // class.
  // (QI cycle: Object::QI -> Extension::QI -> Object::QI)
  if (NULL != pExtensionUnknown && !bExtensionQueryInterfaceInProgress)
  {
    bExtensionQueryInterfaceInProgress = true;
    hr = pExtensionUnknown->QueryInterface(riid, ppvObjOut);
    bExtensionQueryInterfaceInProgress = false;
  }

  return hr;
}


CAAFObject::CAAFObject (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse),
  pExtension(NULL)
{
  if (doInit)
    {
      ImplAAFObject * newRep;
      newRep = new ImplAAFObject;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFObject::~CAAFObject ()
{
  // Cleanup any extensions
  if (NULL != pExtension)
  {
    CAAFObject::Extension *pCurrentExtension = pExtension;
    CAAFObject::Extension *pNextExtension;

    // Set the refcount to 1 do avoid destruction when
    // releasing the aggregated extension object.
    InternalAddRef();

    while (NULL != pCurrentExtension)
    {
      pNextExtension = pCurrentExtension->GetNext();
      pCurrentExtension->SetNext(NULL);
      delete pCurrentExtension;
      pCurrentExtension = pNextExtension;
    }

    pExtension = NULL;
  }
}


HRESULT STDMETHODCALLTYPE
    CAAFObject::GetGeneration (IAAFIdentification ** ppGeneration)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);

  //
  // set up for ppGeneration
  //
  ImplAAFIdentification * internalppGeneration = NULL;
  ImplAAFIdentification ** pinternalppGeneration = NULL;
  if (ppGeneration)
    {
      pinternalppGeneration = &internalppGeneration;
    }

  try
    {
      hr = ptr->GetGeneration
       (pinternalppGeneration);
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
  // cleanup for ppGeneration
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppGeneration)
        {
          pUnknown = static_cast<IUnknown *> (internalppGeneration->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFIdentification, (void **)ppGeneration);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppGeneration->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFObject::GetGenerationAUID (aafUID_t *  pResult)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetGenerationAUID
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
    CAAFObject::GetDefinition (IAAFClassDef ** ppClassDef)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
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
      hr = ptr->GetDefinition
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
    CAAFObject::GetProperties (IEnumAAFProperties ** ppEnum)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFProperties * internalppEnum = NULL;
  ImplEnumAAFProperties ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetProperties
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFProperties, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFObject::CountProperties (aafUInt32 *  pCount)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountProperties
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
    CAAFObject::GetPropertyValue (IAAFPropertyDef * pPropDef,
        IAAFPropertyValue ** ppPropVal)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);

  //
  // set up for pPropDef
  //
  ImplAAFPropertyDef * internalpPropDef = NULL;
  if (pPropDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pPropDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpPropDef = static_cast<ImplAAFPropertyDef*>(arg);
      assert (internalpPropDef);
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
      hr = ptr->GetPropertyValue
       (internalpPropDef,
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
  // no cleanup necessary for pPropDef
  //
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
    CAAFObject::SetPropertyValue (IAAFPropertyDef * pPropDef,
        IAAFPropertyValue * pPropVal)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);

  //
  // set up for pPropDef
  //
  ImplAAFPropertyDef * internalpPropDef = NULL;
  if (pPropDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pPropDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpPropDef = static_cast<ImplAAFPropertyDef*>(arg);
      assert (internalpPropDef);
    }
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
      hr = ptr->SetPropertyValue
       (internalpPropDef,
        internalpPropVal);
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
  // no cleanup necessary for pPropDef
  //
  //
  // no cleanup necessary for pPropVal
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFObject::IsPropertyPresent (IAAFPropertyDef * pPropDef,
        aafBoolean_t*  pResult)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);

  //
  // set up for pPropDef
  //
  ImplAAFPropertyDef * internalpPropDef = NULL;
  if (pPropDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pPropDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpPropDef = static_cast<ImplAAFPropertyDef*>(arg);
      assert (internalpPropDef);
    }

  try
    {
      hr = ptr->IsPropertyPresent
       (internalpPropDef,
        pResult);
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
  // no cleanup necessary for pPropDef
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFObject::RemoveOptionalProperty (IAAFPropertyDef * pPropDef)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);

  //
  // set up for pPropDef
  //
  ImplAAFPropertyDef * internalpPropDef = NULL;
  if (pPropDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pPropDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpPropDef = static_cast<ImplAAFPropertyDef*>(arg);
      assert (internalpPropDef);
    }

  try
    {
      hr = ptr->RemoveOptionalProperty
       (internalpPropDef);
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
  // no cleanup necessary for pPropDef
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFObject::CreateOptionalPropertyValue (IAAFPropertyDef * pPropDef,
        IAAFPropertyValue ** ppPropVal)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);

  //
  // set up for pPropDef
  //
  ImplAAFPropertyDef * internalpPropDef = NULL;
  if (pPropDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pPropDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpPropDef = static_cast<ImplAAFPropertyDef*>(arg);
      assert (internalpPropDef);
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
      hr = ptr->CreateOptionalPropertyValue
       (internalpPropDef,
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
  // no cleanup necessary for pPropDef
  //
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
    CAAFObject::GetDictionary (IAAFDictionary ** ppDictionary)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);

  //
  // set up for ppDictionary
  //
  ImplAAFDictionary * internalppDictionary = NULL;
  ImplAAFDictionary ** pinternalppDictionary = NULL;
  if (ppDictionary)
    {
      pinternalppDictionary = &internalppDictionary;
    }

  try
    {
      hr = ptr->GetDictionary
       (pinternalppDictionary);
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
  // cleanup for ppDictionary
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppDictionary)
        {
          pUnknown = static_cast<IUnknown *> (internalppDictionary->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFDictionary, (void **)ppDictionary);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppDictionary->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFObject::EnableGenerationTracking ()
{
  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->EnableGenerationTracking();
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
    CAAFObject::DisableGenerationTracking ()
{
  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->DisableGenerationTracking();
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
    CAAFObject::IsGenerationTracked (aafBoolean_t *  pResult)
{
  HRESULT hr;

  ImplAAFObject * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFObject*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->IsGenerationTracked
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

//
// Intialize class extensions. This method is called after the
// contained Impl object has been completely initialized. This
// allows the aggregated extension object access to all of the
// interfaces and property data of its controlling unknown.
//
inline int EQUAL_CLSID(const CLSID & a, const CLSID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (CLSID)));
}
HRESULT STDMETHODCALLTYPE
    CAAFObject::InitializeExtension(REFCLSID rclsid)
{
  HRESULT hr = S_OK;
  CAAFObject::Extension *pNextExtension = NULL;
  CAAFObject::Extension *pLastExtension = NULL;
  CAAFObject::Extension *pNewExtension = NULL;

  // Increment the reference count to keep this object
  // from being inadvertantly deleted by/with the aggregate
  // extension object.
  InternalAddRef();

  try
    {
          // Make sure that we do not already have an instantiation
      // for the given clsid/plugin. Walk our linked list and 
      // abort if we have already instantiated a plugin with
      // the same code class id.
      //
      pNextExtension = pExtension;
      while (NULL != pNextExtension)
      {
        if (EQUAL_CLSID(rclsid,pNextExtension->GetCLSID()))
        {
          hr = AAFRESULT_EXTENSION_ALREADY_INITIALIZED;
          break;
        }

        // Remember the last extension so that we can add the
        // new extension onto the end of the list.
        pLastExtension = pNextExtension;
        pNextExtension = pLastExtension->GetNext();
      }

      if (SUCCEEDED(hr)) // there were no duplicates...
      {
        pNewExtension = new CAAFObject::Extension(rclsid);
        if (NULL == pNewExtension)
        {
          hr = E_OUTOFMEMORY;
        }
        else
        {
          hr = pNewExtension->InitializeExtension(GetPrivateUnknown());
          if (SUCCEEDED(hr))
          {
            // Save the extension in the list.
            if (NULL != pLastExtension)
              pLastExtension->SetNext(pNewExtension); // add to the end
            else
              pExtension = pNewExtension; // this is the first one.
            pNewExtension = NULL; // This pointer is now owned by the list.
          }
        }
      } 
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

  // Cleanup
  InternalRelease();

  if (pNewExtension)
    delete pNewExtension;

  return hr;
}

//
// 
//
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
} 
HRESULT CAAFObject::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFObject)) 
    { 
        *ppvObj = (IAAFObject *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    hr = CAAFRoot::InternalQueryInterface(riid, ppvObj);

    //
    // If our built-in class did not handle the requested interface
    // then delegate the request to extension objects if they exists.
    //
    CAAFObject::Extension *pNextExtension = pExtension;
    while (E_NOINTERFACE == hr && NULL != pNextExtension)
    {
        hr = pNextExtension->QueryInterface(riid, ppvObj);
        pNextExtension = pNextExtension->GetNext();
    }

    return hr;
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFObject)


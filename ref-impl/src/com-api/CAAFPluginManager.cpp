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



#include "CAAFPluginManager.h"
#include "ImplAAFPluginManager.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CEnumAAFLoadedPlugins.h"
#include "ImplEnumAAFLoadedPlugins.h"

#include "CAAFDictionary.h"
#include "ImplAAFDictionary.h"

// CLSID for AAFPluginManager 
// {AD1BB85B-DBB2-11d2-809C-006008143E6F}
EXTERN_C const CLSID CLSID_AAFPluginManager = { 0xAD1BB85B, 0xDBB2, 0x11d2, { 0x80, 0x9C, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };





CAAFPluginManager::CAAFPluginManager (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFPluginManager * newRep;
      newRep = new ImplAAFPluginManager;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFPluginManager::~CAAFPluginManager ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFPluginManager::RegisterSharedPlugins ()
{
  ImplAAFPluginManager * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPluginManager*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->RegisterSharedPlugins();
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
    CAAFPluginManager::RegisterPluginDirectory (aafCharacter_constptr  directoryName)
{
  HRESULT hr;

  ImplAAFPluginManager * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPluginManager*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->RegisterPluginDirectory
       (directoryName);
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
    CAAFPluginManager::RegisterPluginFile (aafCharacter_constptr  fileName)
{
  HRESULT hr;

  ImplAAFPluginManager * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPluginManager*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->RegisterPluginFile
       (fileName);
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
    CAAFPluginManager::EnumLoadedPlugins (aafUID_constref  categoryID,
        IEnumAAFLoadedPlugins ** ppEnum)
{
  HRESULT hr;

  ImplAAFPluginManager * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPluginManager*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFLoadedPlugins * internalppEnum = NULL;
  ImplEnumAAFLoadedPlugins ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->EnumLoadedPlugins
       (categoryID,
        pinternalppEnum);
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFLoadedPlugins, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPluginManager::CreatePluginDefinition (aafUID_constref  pluginDefID,
        IAAFDictionary * pDictionary,
        IAAFDefObject**  ppPluginDef)
{
  HRESULT hr;

  ImplAAFPluginManager * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPluginManager*> (pO);
  assert (ptr);

  //
  // set up for pDictionary
  //
  ImplAAFDictionary * internalpDictionary = NULL;
  if (pDictionary)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pDictionary->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpDictionary = static_cast<ImplAAFDictionary*>(arg);
      assert (internalpDictionary);
    }

  try
    {
      hr = ptr->CreatePluginDefinition
       (pluginDefID,
        internalpDictionary,
        ppPluginDef);
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
  // no cleanup necessary for pDictionary
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFPluginManager::CreateInstance (REFCLSID  rclsid,
        IUnknown *  pUnkOuter,
        REFIID  riid,
        void **  ppPlugin)
{
  HRESULT hr;

  ImplAAFPluginManager * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFPluginManager*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CreateInstance
       (rclsid,
        pUnkOuter,
        riid,
        ppPlugin);
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
HRESULT CAAFPluginManager::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFPluginManager)) 
    { 
        *ppvObj = (IAAFPluginManager *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFPluginManager)


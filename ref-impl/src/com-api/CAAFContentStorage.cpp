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



#include "CAAFContentStorage.h"
#include "ImplAAFContentStorage.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>

// CLSID for AAFContentStorage 
// {54D4C481-5F8B-11d2-8073-006008143E6F}
EXTERN_C const CLSID CLSID_AAFContentStorage = { 0x54D4C481, 0x5F8B, 0x11d2, { 0x80, 0x73, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };



#include "CEnumAAFMobs.h"
#include "ImplEnumAAFMobs.h"

#include "CEnumAAFEssenceData.h"
#include "ImplEnumAAFEssenceData.h"




CAAFContentStorage::CAAFContentStorage (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFObject (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFContentStorage * newRep;
      newRep = new ImplAAFContentStorage;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFContentStorage::~CAAFContentStorage ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFContentStorage::LookupMob (aafMobID_constref  mobID,
        IAAFMob ** ppMob)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for ppMob
  //
  ImplAAFMob * internalppMob = NULL;
  ImplAAFMob ** pinternalppMob = NULL;
  if (ppMob)
    {
      pinternalppMob = &internalppMob;
    }

  try
    {
      hr = ptr->LookupMob
       (mobID,
        pinternalppMob);
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
  // cleanup for ppMob
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppMob)
        {
          pUnknown = static_cast<IUnknown *> (internalppMob->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFMob, (void **)ppMob);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppMob->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFContentStorage::CountMobs (aafMobKind_t  mobKind,
        aafNumSlots_t *  pResult)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for mobKind
  //
  if (! Is_aafMobKind_t_Valid(mobKind))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->CountMobs
       (mobKind,
        pResult);
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
    CAAFContentStorage::GetMobs (aafSearchCrit_t *  pSearchCriteria,
        IEnumAAFMobs ** ppEnum)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFMobs * internalppEnum = NULL;
  ImplEnumAAFMobs ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetMobs
       (pSearchCriteria,
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFMobs, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFContentStorage::AddMob (IAAFMob * pMob)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for pMob
  //
  ImplAAFMob * internalpMob = NULL;
  if (pMob)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pMob->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpMob = static_cast<ImplAAFMob*>(arg);
      assert (internalpMob);
    }

  try
    {
      hr = ptr->AddMob
       (internalpMob);
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
  // no cleanup necessary for pMob
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFContentStorage::RemoveMob (IAAFMob * pMob)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for pMob
  //
  ImplAAFMob * internalpMob = NULL;
  if (pMob)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pMob->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpMob = static_cast<ImplAAFMob*>(arg);
      assert (internalpMob);
    }

  try
    {
      hr = ptr->RemoveMob
       (internalpMob);
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
  // no cleanup necessary for pMob
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFContentStorage::CountEssenceData (aafUInt32 *  pResult)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountEssenceData
       (pResult);
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
    CAAFContentStorage::IsEssenceDataPresent (aafMobID_constref  fileMobID,
        aafFileFormat_t  fmt,
        aafBoolean_t *  pResult)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for fmt
  //
  if (! Is_aafFileFormat_t_Valid(fmt))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->IsEssenceDataPresent
       (fileMobID,
        fmt,
        pResult);
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
    CAAFContentStorage::EnumEssenceData (IEnumAAFEssenceData ** ppEnum)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFEssenceData * internalppEnum = NULL;
  ImplEnumAAFEssenceData ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->EnumEssenceData
       (pinternalppEnum);
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFEssenceData, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFContentStorage::AddEssenceData (IAAFEssenceData * pEssenceData)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for pEssenceData
  //
  ImplAAFEssenceData * internalpEssenceData = NULL;
  if (pEssenceData)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pEssenceData->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpEssenceData = static_cast<ImplAAFEssenceData*>(arg);
      assert (internalpEssenceData);
    }

  try
    {
      hr = ptr->AddEssenceData
       (internalpEssenceData);
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
  // no cleanup necessary for pEssenceData
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFContentStorage::RemoveEssenceData (IAAFEssenceData * pEssenceData)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for pEssenceData
  //
  ImplAAFEssenceData * internalpEssenceData = NULL;
  if (pEssenceData)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pEssenceData->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpEssenceData = static_cast<ImplAAFEssenceData*>(arg);
      assert (internalpEssenceData);
    }

  try
    {
      hr = ptr->RemoveEssenceData
       (internalpEssenceData);
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
  // no cleanup necessary for pEssenceData
  //
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFContentStorage::LookupEssenceData (aafMobID_constref  mobID,
        IAAFEssenceData ** ppEssenceData)
{
  HRESULT hr;

  ImplAAFContentStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFContentStorage*> (pO);
  assert (ptr);

  //
  // set up for ppEssenceData
  //
  ImplAAFEssenceData * internalppEssenceData = NULL;
  ImplAAFEssenceData ** pinternalppEssenceData = NULL;
  if (ppEssenceData)
    {
      pinternalppEssenceData = &internalppEssenceData;
    }

  try
    {
      hr = ptr->LookupEssenceData
       (mobID,
        pinternalppEssenceData);
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
  // cleanup for ppEssenceData
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEssenceData)
        {
          pUnknown = static_cast<IUnknown *> (internalppEssenceData->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFEssenceData, (void **)ppEssenceData);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEssenceData->ReleaseReference(); // We are through with this pointer.
        }
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
HRESULT CAAFContentStorage::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFContentStorage)) 
    { 
        *ppvObj = (IAAFContentStorage *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFObject::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFContentStorage)


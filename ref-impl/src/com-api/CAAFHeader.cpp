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



#include "CAAFHeader.h"
#include "ImplAAFHeader.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFMob.h"
#include "ImplAAFMob.h"

#include "CAAFEssenceData.h"
#include "ImplAAFEssenceData.h"

#include "CEnumAAFMobs.h"
#include "ImplEnumAAFMobs.h"

#include "CEnumAAFEssenceData.h"
#include "ImplEnumAAFEssenceData.h"

#include "CAAFDictionary.h"
#include "ImplAAFDictionary.h"

#include "CAAFIdentification.h"
#include "ImplAAFIdentification.h"

#include "CEnumAAFIdentifications.h"
#include "ImplEnumAAFIdentifications.h"

// CLSID for AAFHeader 
// {B1A21383-1A7D-11D2-BF78-00104BC9156D}
EXTERN_C const CLSID CLSID_AAFHeader = { 0xB1A21383, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CAAFHeader::CAAFHeader (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFObject (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFHeader * newRep;
      newRep = new ImplAAFHeader;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFHeader::~CAAFHeader ()
{
}

HRESULT STDMETHODCALLTYPE
    CAAFHeader::LookupMob (aafMobID_constref  mobID,
        IAAFMob ** ppMob)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::CountMobs (aafMobKind_t  mobKind,
        aafNumSlots_t *  pResult)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::GetMobs (aafSearchCrit_t *  pSearchCriteria,
        IEnumAAFMobs ** ppEnum)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::AddMob (IAAFMob * pMob)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::RemoveMob (IAAFMob * pMob)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::CountEssenceData (aafUInt32 *  pResult)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::IsEssenceDataPresent (aafMobID_constref  fileMobID,
        aafFileFormat_t  fmt,
        aafBoolean_t *  pResult)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::EnumEssenceData (IEnumAAFEssenceData ** ppEnum)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::AddEssenceData (IAAFEssenceData * pEssenceData)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::RemoveEssenceData (IAAFEssenceData * pEssenceData)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::LookupEssenceData (aafMobID_constref  mobID,
        IAAFEssenceData ** ppEssenceData)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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


HRESULT STDMETHODCALLTYPE
    CAAFHeader::GetDictionary (IAAFDictionary ** ppDictionary)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
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
    CAAFHeader::GetLastIdentification (IAAFIdentification ** ppIdentification)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);

  //
  // set up for ppIdentification
  //
  ImplAAFIdentification * internalppIdentification = NULL;
  ImplAAFIdentification ** pinternalppIdentification = NULL;
  if (ppIdentification)
    {
      pinternalppIdentification = &internalppIdentification;
    }

  try
    {
      hr = ptr->GetLastIdentification
       (pinternalppIdentification);
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
  // cleanup for ppIdentification
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppIdentification)
        {
          pUnknown = static_cast<IUnknown *> (internalppIdentification->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFIdentification, (void **)ppIdentification);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppIdentification->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFHeader::LookupIdentification (aafUID_constref  generation,
        IAAFIdentification ** ppIdentification)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);

  //
  // set up for ppIdentification
  //
  ImplAAFIdentification * internalppIdentification = NULL;
  ImplAAFIdentification ** pinternalppIdentification = NULL;
  if (ppIdentification)
    {
      pinternalppIdentification = &internalppIdentification;
    }

  try
    {
      hr = ptr->LookupIdentification
       (generation,
        pinternalppIdentification);
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
  // cleanup for ppIdentification
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppIdentification)
        {
          pUnknown = static_cast<IUnknown *> (internalppIdentification->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFIdentification, (void **)ppIdentification);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppIdentification->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFHeader::CountIdentifications (aafUInt32 *  pResult)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountIdentifications
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
    CAAFHeader::GetIdentifications (IEnumAAFIdentifications ** ppEnum)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFIdentifications * internalppEnum = NULL;
  ImplEnumAAFIdentifications ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetIdentifications
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFIdentifications, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFHeader::AppendIdentification (IAAFIdentification * pIdent)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);

  //
  // set up for pIdent
  //
  ImplAAFIdentification * internalpIdent = NULL;
  if (pIdent)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pIdent->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpIdent = static_cast<ImplAAFIdentification*>(arg);
      assert (internalpIdent);
    }

  try
    {
      hr = ptr->AppendIdentification
       (internalpIdent);
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
  // no cleanup necessary for pIdent
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFHeader::GetIdentificationAt (aafUInt32  index,
        IAAFIdentification ** ppIdentification)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);

  //
  // set up for ppIdentification
  //
  ImplAAFIdentification * internalppIdentification = NULL;
  ImplAAFIdentification ** pinternalppIdentification = NULL;
  if (ppIdentification)
    {
      pinternalppIdentification = &internalppIdentification;
    }

  try
    {
      hr = ptr->GetIdentificationAt
       (index,
        pinternalppIdentification);
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
  // cleanup for ppIdentification
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppIdentification)
        {
          pUnknown = static_cast<IUnknown *> (internalppIdentification->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFIdentification, (void **)ppIdentification);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppIdentification->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFHeader::GetRefImplVersion (aafProductVersion_t *  pVersion)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetRefImplVersion
       (pVersion);
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
    CAAFHeader::GetFileRevision (aafVersionType_t *  pRevision)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetFileRevision
       (pRevision);
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
    CAAFHeader::GetLastModified (aafTimeStamp_t *  pTimeStamp)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetLastModified
       (pTimeStamp);
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
    CAAFHeader::GetContentStorage (IAAFContentStorage ** ppStorage)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);

  //
  // set up for ppStorage
  //
  ImplAAFContentStorage * internalppStorage = NULL;
  ImplAAFContentStorage ** pinternalppStorage = NULL;
  if (ppStorage)
    {
      pinternalppStorage = &internalppStorage;
    }

  try
    {
      hr = ptr->GetContentStorage
       (pinternalppStorage);
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
  // cleanup for ppStorage
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppStorage)
        {
          pUnknown = static_cast<IUnknown *> (internalppStorage->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFContentStorage, (void **)ppStorage);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppStorage->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFHeader::GetStoredByteOrder (eAAFByteOrder_t *  pOrder)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);


  hr = ptr->GetStoredByteOrder (pOrder);

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFHeader::GetNativeByteOrder (eAAFByteOrder_t *  pOrder)
{
  HRESULT hr;

  ImplAAFHeader * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFHeader*> (pO);
  assert (ptr);


  hr = ptr->GetNativeByteOrder (pOrder);

  return hr;
}

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFHeader::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFHeader)) 
    { 
        *ppvObj = (IAAFHeader *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    if (EQUAL_UID(riid,IID_IAAFEndian)) 
    { 
        *ppvObj = (IAAFEndian *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFObject::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFHeader)

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



#include "CAAFMasterMob.h"
#include "ImplAAFMasterMob.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFSourceClip.h"
#include "ImplAAFSourceClip.h"

#include "CAAFSourceMob.h"
#include "ImplAAFSourceMob.h"

#include "CAAFComponent.h"
#include "ImplAAFComponent.h"

#include "CAAFDataDef.h"
#include "ImplAAFDataDef.h"

#include "CAAFFindSourceInfo.h"
#include "ImplAAFFindSourceInfo.h"

#include "CAAFEssenceAccess.h"
#include "ImplAAFEssenceAccess.h"

// CLSID for AAFMasterMob 
// {b1a21373-1a7d-11d2-bf78-00104bc9156d}
EXTERN_C const CLSID CLSID_AAFMasterMob = { 0xb1a21373, 0x1a7d, 0x11d2, { 0xbf, 0x78, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };





CAAFMasterMob::CAAFMasterMob (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFMob (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFMasterMob * newRep;
      newRep = new ImplAAFMasterMob;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFMasterMob::~CAAFMasterMob ()
{
}

HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::Initialize ()
{
  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Initialize();
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
    CAAFMasterMob::AddMasterSlot (IAAFDataDef * pDataDef,
        aafSlotID_t  sourceSlotID,
        IAAFSourceMob * pSourceMob,
        aafSlotID_t  masterSlotID,
        aafCharacter_constptr  pSlotName)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for pDataDef
  //
  ImplAAFDataDef * internalpDataDef = NULL;
  if (pDataDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pDataDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpDataDef = static_cast<ImplAAFDataDef*>(arg);
      assert (internalpDataDef);
    }
  //
  // set up for pSourceMob
  //
  ImplAAFSourceMob * internalpSourceMob = NULL;
  if (pSourceMob)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSourceMob->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSourceMob = static_cast<ImplAAFSourceMob*>(arg);
      assert (internalpSourceMob);
    }

  try
    {
      hr = ptr->AddMasterSlot
       (internalpDataDef,
        sourceSlotID,
        internalpSourceMob,
        masterSlotID,
        pSlotName);
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
  // no cleanup necessary for pDataDef
  //
  //
  // no cleanup necessary for pSourceMob
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::GetTapeName (aafUInt32  masterSlotID,
        aafCharacter *  pTapeName,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetTapeName
       (masterSlotID,
        pTapeName,
        bufSize);
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
    CAAFMasterMob::GetTapeNameBufLen (aafUInt32  masterSlotID,
        aafUInt32 *  pLen)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetTapeNameBufLen
       (masterSlotID,
        pLen);
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
    CAAFMasterMob::GetNumRepresentations (aafSlotID_t  slotID,
        aafNumSlots_t *  pNumReps)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetNumRepresentations
       (slotID,
        pNumReps);
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
    CAAFMasterMob::GetRepresentation (aafSlotID_t  slotID,
        aafUInt32  index,
        IAAFSegment ** ppSourceClip)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for ppSourceClip
  //
  ImplAAFSegment * internalppSourceClip = NULL;
  ImplAAFSegment ** pinternalppSourceClip = NULL;
  if (ppSourceClip)
    {
      pinternalppSourceClip = &internalppSourceClip;
    }

  try
    {
      hr = ptr->GetRepresentation
       (slotID,
        index,
        pinternalppSourceClip);
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
  // cleanup for ppSourceClip
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppSourceClip)
        {
          pUnknown = static_cast<IUnknown *> (internalppSourceClip->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSegment, (void **)ppSourceClip);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppSourceClip->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::GetCriteriaSegment (aafSlotID_t  slotID,
        aafMediaCriteria_t *  pCriteria,
        IAAFSegment ** ppSegment)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for ppSegment
  //
  ImplAAFSegment * internalppSegment = NULL;
  ImplAAFSegment ** pinternalppSegment = NULL;
  if (ppSegment)
    {
      pinternalppSegment = &internalppSegment;
    }

  try
    {
      hr = ptr->GetCriteriaSegment
       (slotID,
        pCriteria,
        pinternalppSegment);
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
  // cleanup for ppSegment
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppSegment)
        {
          pUnknown = static_cast<IUnknown *> (internalppSegment->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSegment, (void **)ppSegment);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppSegment->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::AppendPhysSourceRef (aafRational_t  editrate,
        aafSlotID_t  aMobSlot,
        IAAFDataDef * pEssenceKind,
        aafSourceRef_t  ref,
        aafLength_t  srcRefLength)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for pEssenceKind
  //
  ImplAAFDataDef * internalpEssenceKind = NULL;
  if (pEssenceKind)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pEssenceKind->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpEssenceKind = static_cast<ImplAAFDataDef*>(arg);
      assert (internalpEssenceKind);
    }

  try
    {
      hr = ptr->AppendPhysSourceRef
       (editrate,
        aMobSlot,
        internalpEssenceKind,
        ref,
        srcRefLength);
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
  // no cleanup necessary for pEssenceKind
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::NewPhysSourceRef (aafRational_t  editrate,
        aafSlotID_t  aMobSlot,
        IAAFDataDef * pEssenceKind,
        aafSourceRef_t  ref,
        aafLength_t  srcRefLength)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for pEssenceKind
  //
  ImplAAFDataDef * internalpEssenceKind = NULL;
  if (pEssenceKind)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pEssenceKind->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpEssenceKind = static_cast<ImplAAFDataDef*>(arg);
      assert (internalpEssenceKind);
    }

  try
    {
      hr = ptr->NewPhysSourceRef
       (editrate,
        aMobSlot,
        internalpEssenceKind,
        ref,
        srcRefLength);
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
  // no cleanup necessary for pEssenceKind
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::CreateEssence (aafSlotID_t  masterSlotID,
        IAAFDataDef * pMediaKind,
        aafUID_constref  codecID,
        aafRational_t  editRate,
        aafRational_t  samplerate,
        aafCompressEnable_t  Enable,
        IAAFLocator * destination,
        aafUID_constref  fileFormat,
        IAAFEssenceAccess ** access)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for pMediaKind
  //
  ImplAAFDataDef * internalpMediaKind = NULL;
  if (pMediaKind)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pMediaKind->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpMediaKind = static_cast<ImplAAFDataDef*>(arg);
      assert (internalpMediaKind);
    }
  //
  // set up for Enable
  //
  if (! Is_aafCompressEnable_t_Valid(Enable))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for destination
  //
  ImplAAFLocator * internaldestination = NULL;
  if (destination)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = destination->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internaldestination = static_cast<ImplAAFLocator*>(arg);
      assert (internaldestination);
    }
  //
  // set up for access
  //
  ImplAAFEssenceAccess * internalaccess = NULL;
  ImplAAFEssenceAccess ** pinternalaccess = NULL;
  if (access)
    {
      pinternalaccess = &internalaccess;
    }

  try
    {
      hr = ptr->CreateEssence
       (masterSlotID,
        internalpMediaKind,
        codecID,
        editRate,
        samplerate,
        Enable,
        internaldestination,
        fileFormat,
        pinternalaccess);
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
  // no cleanup necessary for pMediaKind
  //
  //
  // no cleanup necessary for destination
  //
  //
  // cleanup for access
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalaccess)
        {
          pUnknown = static_cast<IUnknown *> (internalaccess->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFEssenceAccess, (void **)access);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalaccess->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::CreateMultiEssence (aafUID_constref  codecID,
        aafUInt16  arrayElemCount,
        aafmMultiCreate_t *  mediaArray,
        aafCompressEnable_t  Enable,
        IAAFLocator * destination,
        aafUID_constref  fileFormat,
        IAAFEssenceMultiAccess**  access)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for Enable
  //
  if (! Is_aafCompressEnable_t_Valid(Enable))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for destination
  //
  ImplAAFLocator * internaldestination = NULL;
  if (destination)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = destination->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internaldestination = static_cast<ImplAAFLocator*>(arg);
      assert (internaldestination);
    }

  try
    {
      hr = ptr->CreateMultiEssence
       (codecID,
        arrayElemCount,
        mediaArray,
        Enable,
        internaldestination,
        fileFormat,
        access);
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
  // no cleanup necessary for destination
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::OpenEssence (aafSlotID_t  slotID,
        aafMediaCriteria_t*  mediaCrit,
        aafMediaOpenMode_t  openMode,
        aafCompressEnable_t  compEnable,
        IAAFEssenceAccess ** access)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for openMode
  //
  if (! Is_aafMediaOpenMode_t_Valid(openMode))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for compEnable
  //
  if (! Is_aafCompressEnable_t_Valid(compEnable))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for access
  //
  ImplAAFEssenceAccess * internalaccess = NULL;
  ImplAAFEssenceAccess ** pinternalaccess = NULL;
  if (access)
    {
      pinternalaccess = &internalaccess;
    }

  try
    {
      hr = ptr->OpenEssence
       (slotID,
        mediaCrit,
        openMode,
        compEnable,
        pinternalaccess);
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
  // cleanup for access
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalaccess)
        {
          pUnknown = static_cast<IUnknown *> (internalaccess->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFEssenceAccess, (void **)access);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalaccess->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}

	

HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::OpenMultiEssence (aafSlotID_t  slotID,
        aafMediaCriteria_t*  mediaCrit,
        aafMediaOpenMode_t  openMode,
        aafCompressEnable_t  compEnable,
        IAAFEssenceMultiAccess**  access)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for openMode
  //
  if (! Is_aafMediaOpenMode_t_Valid(openMode))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for compEnable
  //
  if (! Is_aafCompressEnable_t_Valid(compEnable))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->OpenMultiEssence
       (slotID,
        mediaCrit,
        openMode,
        compEnable,
        access);
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
    CAAFMasterMob::CountChannels (aafSlotID_t  slotID,
        aafMediaCriteria_t*  mediaCrit,
        IAAFDataDef * pMediaKind,
        aafUInt16*  numCh)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);

  //
  // set up for pMediaKind
  //
  ImplAAFDataDef * internalpMediaKind = NULL;
  if (pMediaKind)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pMediaKind->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpMediaKind = static_cast<ImplAAFDataDef*>(arg);
      assert (internalpMediaKind);
    }

  try
    {
      hr = ptr->CountChannels
       (slotID,
        mediaCrit,
        internalpMediaKind,
        numCh);
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
  // no cleanup necessary for pMediaKind
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMasterMob::SearchSource (aafSlotID_t  slotID,
        aafPosition_t  offset,
        aafMobKind_t  mobKind,
        aafMediaCriteria_t *  pMediaCrit,
        aafOperationChoice_t *  pOperationChoice,
        IAAFFindSourceInfo ** ppSourceInfo)
{
  HRESULT hr;

  ImplAAFMasterMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMasterMob*> (pO);
  assert (ptr);



  //
  // set up for mobKind
  //
  if (! Is_aafMobKind_t_Valid(mobKind))
    return AAFRESULT_INVALID_ENUM_VALUE;



  //
  // set up for ppSourceInfo
  //
  ImplAAFFindSourceInfo * internalppSourceInfo = NULL;
  ImplAAFFindSourceInfo ** pinternalppSourceInfo = NULL;
  if (ppSourceInfo)
    {
      pinternalppSourceInfo = &internalppSourceInfo;
    }

  hr = ptr->SearchSource (slotID,
    offset,
    mobKind,
    pMediaCrit,
    pOperationChoice,
    pinternalppSourceInfo);





  //
  // cleanup for ppSourceInfo
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppSourceInfo)
        {
          pUnknown = static_cast<IUnknown *> (internalppSourceInfo->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFindSourceInfo, (void **)ppSourceInfo);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppSourceInfo->ReleaseReference(); // We are through with this pointer.
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
HRESULT CAAFMasterMob::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFMasterMob)) 
    { 
        *ppvObj = (IAAFMasterMob *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    if (EQUAL_UID(riid,IID_IAAFSearchSource)) 
    { 
        *ppvObj = (IAAFSearchSource *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFMob::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFMasterMob)

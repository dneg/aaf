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



#include "CAAFSourceMob.h"
#include "ImplAAFSourceMob.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFEssenceDescriptor.h"
#include "ImplAAFEssenceDescriptor.h"

#include "CAAFDataDef.h"
#include "ImplAAFDataDef.h"

#include "CAAFFindSourceInfo.h"
#include "ImplAAFFindSourceInfo.h"

#include "CAAFComponent.h"
#include "ImplAAFComponent.h"

#include "CAAFSegment.h"
#include "ImplAAFSegment.h"

// CLSID for AAFSourceMob 
// {B1A2137D-1A7D-11D2-BF78-00104BC9156D}
EXTERN_C const CLSID CLSID_AAFSourceMob = { 0xB1A2137D, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CAAFSourceMob::CAAFSourceMob (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFMob (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFSourceMob * newRep;
      newRep = new ImplAAFSourceMob;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFSourceMob::~CAAFSourceMob ()
{
}

HRESULT STDMETHODCALLTYPE
    CAAFSourceMob::Initialize ()
{
  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
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
    CAAFSourceMob::GetEssenceDescriptor (IAAFEssenceDescriptor ** ppEssence)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
  assert (ptr);

  //
  // set up for ppEssence
  //
  ImplAAFEssenceDescriptor * internalppEssence = NULL;
  ImplAAFEssenceDescriptor ** pinternalppEssence = NULL;
  if (ppEssence)
    {
      pinternalppEssence = &internalppEssence;
    }

  try
    {
      hr = ptr->GetEssenceDescriptor
       (pinternalppEssence);
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
  // cleanup for ppEssence
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEssence)
        {
          pUnknown = static_cast<IUnknown *> (internalppEssence->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFEssenceDescriptor, (void **)ppEssence);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEssence->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFSourceMob::SetEssenceDescriptor (IAAFEssenceDescriptor * pEssence)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
  assert (ptr);

  //
  // set up for pEssence
  //
  ImplAAFEssenceDescriptor * internalpEssence = NULL;
  if (pEssence)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pEssence->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpEssence = static_cast<ImplAAFEssenceDescriptor*>(arg);
      assert (internalpEssence);
    }

  try
    {
      hr = ptr->SetEssenceDescriptor
       (internalpEssence);
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
  // no cleanup necessary for pEssence
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFSourceMob::AddNilReference (aafSlotID_t  slotID,
        aafLength_t  length,
        IAAFDataDef * pDataDef,
        aafRational_t  editRate)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
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

  try
    {
      hr = ptr->AddNilReference
       (slotID,
        length,
        internalpDataDef,
        editRate);
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
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFSourceMob::AppendTimecodeSlot (aafRational_t  editrate,
        aafInt32  slotID,
        aafTimecode_t  startTC,
        aafFrameLength_t  length32)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->AppendTimecodeSlot
       (editrate,
        slotID,
        startTC,
        length32);
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
    CAAFSourceMob::AppendEdgecodeSlot (aafRational_t  editrate,
        aafInt32  slotID,
        aafFrameOffset_t  startEC,
        aafFrameLength_t  length32,
        aafFilmType_t  filmKind,
        aafEdgeType_t  codeFormat,
        aafEdgecodeHeader_t  header)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
  assert (ptr);

  //
  // set up for filmKind
  //
  if (! Is_aafFilmType_t_Valid(filmKind))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for codeFormat
  //
  if (! Is_aafEdgeType_t_Valid(codeFormat))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->AppendEdgecodeSlot
       (editrate,
        slotID,
        startEC,
        length32,
        filmKind,
        codeFormat,
        header);
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
    CAAFSourceMob::SpecifyValidCodeRange (IAAFDataDef * pEssenceKind,
        aafSlotID_t  slotID,
        aafRational_t  editrate,
        aafFrameOffset_t  startOffset,
        aafFrameLength_t  length32)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
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
      hr = ptr->SpecifyValidCodeRange
       (internalpEssenceKind,
        slotID,
        editrate,
        startOffset,
        length32);
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
    CAAFSourceMob::AppendPhysSourceRef (aafRational_t  editrate,
        aafSlotID_t  aMobSlot,
        IAAFDataDef * pEssenceKind,
        aafSourceRef_t  ref,
        aafLength_t  srcRefLength)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
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
    CAAFSourceMob::NewPhysSourceRef (aafRational_t  editrate,
        aafSlotID_t  aMobSlot,
        IAAFDataDef * pEssenceKind,
        aafSourceRef_t  ref,
        aafLength_t  srcRefLength)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
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
    CAAFSourceMob::AddPulldownRef (aafAppendOption_t  addType,
        aafRational_t  editrate,
        aafSlotID_t  aMobSlot,
        IAAFDataDef * pEssenceKind,
        aafSourceRef_t  ref,
        aafLength_t  srcRefLength,
        aafPulldownKind_t  pulldownKind,
        aafPhaseFrame_t  phaseFrame,
        aafPulldownDir_t  direction)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
  assert (ptr);

  //
  // set up for addType
  //
  if (! Is_aafAppendOption_t_Valid(addType))
    return AAFRESULT_INVALID_ENUM_VALUE;
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
  //
  // set up for pulldownKind
  //
  if (! Is_aafPulldownKind_t_Valid(pulldownKind))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for direction
  //
  if (! Is_aafPulldownDir_t_Valid(direction))
    return AAFRESULT_INVALID_ENUM_VALUE;

  try
    {
      hr = ptr->AddPulldownRef
       (addType,
        editrate,
        aMobSlot,
        internalpEssenceKind,
        ref,
        srcRefLength,
        pulldownKind,
        phaseFrame,
        direction);
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
    CAAFSourceMob::SearchSource (aafSlotID_t  slotID,
        aafPosition_t  offset,
        aafMobKind_t  mobKind,
        aafMediaCriteria_t *  pMediaCrit,
        aafOperationChoice_t *  pOperationChoice,
        IAAFFindSourceInfo ** ppSourceInfo)
{
  HRESULT hr;

  ImplAAFSourceMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFSourceMob*> (pO);
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
HRESULT CAAFSourceMob::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFSourceMob)) 
    { 
        *ppvObj = (IAAFSourceMob *)this; 
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
AAF_DEFINE_FACTORY(AAFSourceMob)


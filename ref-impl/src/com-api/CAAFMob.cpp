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



#include "CAAFMob.h"
#include "ImplAAFMob.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFSegment.h"
#include "ImplAAFSegment.h"

#include "CAAFMobSlot.h"
#include "ImplAAFMobSlot.h"

#include "CAAFTimelineMobSlot.h"
#include "ImplAAFTimelineMobSlot.h"

#include "CAAFFile.h"
#include "ImplAAFFile.h"

#include "CEnumAAFMobSlots.h"
#include "ImplEnumAAFMobSlots.h"

#include "CEnumAAFTaggedValues.h"
#include "ImplEnumAAFTaggedValues.h"

#include "CAAFKLVData.h"
#include "ImplAAFKLVData.h"

#include "CEnumAAFKLVData.h"
#include "ImplEnumAAFKLVData.h"


// CLSID for AAFMob 
// {B1A21375-1A7D-11d2-BF78-00104BC9156D}
EXTERN_C const CLSID CLSID_AAFMob = { 0xB1A21375, 0x1A7D, 0x11d2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CAAFMob::CAAFMob (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFObject (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFMob * newRep;
      newRep = new ImplAAFMob;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFMob::~CAAFMob ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFMob::GetMobID (aafMobID_t *  pMobID)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetMobID
       (pMobID);
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
    CAAFMob::SetMobID (aafMobID_constref  mobID)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetMobID
       (mobID);
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
    CAAFMob::SetName (aafCharacter_constptr  pName)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetName
       (pName);
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
    CAAFMob::GetName (aafCharacter *  pName,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetName
       (pName,
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
    CAAFMob::GetNameBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetNameBufLen
       (pBufSize);
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
    CAAFMob::CountSlots (aafNumSlots_t *  pNumSlots)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountSlots
       (pNumSlots);
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
    CAAFMob::AppendSlot (IAAFMobSlot * pSlot)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for pSlot
  //
  ImplAAFMobSlot * internalpSlot = NULL;
  if (pSlot)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSlot->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSlot = static_cast<ImplAAFMobSlot*>(arg);
      assert (internalpSlot);
    }

  try
    {
      hr = ptr->AppendSlot
       (internalpSlot);
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
  // no cleanup necessary for pSlot
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::PrependSlot (IAAFMobSlot * pSlot)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for pSlot
  //
  ImplAAFMobSlot * internalpSlot = NULL;
  if (pSlot)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSlot->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSlot = static_cast<ImplAAFMobSlot*>(arg);
      assert (internalpSlot);
    }

  try
    {
      hr = ptr->PrependSlot
       (internalpSlot);
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
  // no cleanup necessary for pSlot
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::InsertSlotAt (aafUInt32  index,
        IAAFMobSlot * pSlot)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for pSlot
  //
  ImplAAFMobSlot * internalpSlot = NULL;
  if (pSlot)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSlot->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSlot = static_cast<ImplAAFMobSlot*>(arg);
      assert (internalpSlot);
    }

  try
    {
      hr = ptr->InsertSlotAt
       (index,
        internalpSlot);
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
  // no cleanup necessary for pSlot
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::RemoveSlotAt (aafUInt32  index)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->RemoveSlotAt
       (index);
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
    CAAFMob::GetSlotAt (aafUInt32  index,
        IAAFMobSlot ** ppSlot)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for ppSlot
  //
  ImplAAFMobSlot * internalppSlot = NULL;
  ImplAAFMobSlot ** pinternalppSlot = NULL;
  if (ppSlot)
    {
      pinternalppSlot = &internalppSlot;
    }

  try
    {
      hr = ptr->GetSlotAt
       (index,
        pinternalppSlot);
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
  // cleanup for ppSlot
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppSlot)
        {
          pUnknown = static_cast<IUnknown *> (internalppSlot->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFMobSlot, (void **)ppSlot);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppSlot->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::GetSlots (IEnumAAFMobSlots ** ppEnum)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFMobSlots * internalppEnum = NULL;
  ImplEnumAAFMobSlots ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetSlots
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFMobSlots, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::GetModTime (aafTimeStamp_t *  pLastModified)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetModTime
       (pLastModified);
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
    CAAFMob::SetModTime (aafTimeStamp_constref  modTime)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetModTime
       (modTime);
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
    CAAFMob::GetCreateTime (aafTimeStamp_t *  pCreationTime)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetCreateTime
       (pCreationTime);
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
    CAAFMob::SetCreateTime (aafTimeStamp_constref  createTime)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetCreateTime
       (createTime);
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
    CAAFMob::AppendComment (aafCharacter *  pCategory,
        aafCharacter_constptr  pComment)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->AppendComment
       (pCategory,
        pComment);
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
    CAAFMob::CountComments (aafUInt32 *  pNumComments)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountComments
       (pNumComments);
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
    CAAFMob::GetComments (IEnumAAFTaggedValues ** ppEnum)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFTaggedValues * internalppEnum = NULL;
  ImplEnumAAFTaggedValues ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetComments
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFTaggedValues, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::RemoveComment (IAAFTaggedValue * pComment)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for pComment
  //
  ImplAAFTaggedValue * internalpComment = NULL;
  if (pComment)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pComment->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpComment = static_cast<ImplAAFTaggedValue*>(arg);
      assert (internalpComment);
    }

  try
    {
      hr = ptr->RemoveComment
       (internalpComment);
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
  // no cleanup necessary for pComment
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::AppendNewTimelineSlot (aafRational_t  editRate,
        IAAFSegment * pSegment,
        aafSlotID_t  slotID,
        aafCharacter_constptr  pSlotName,
        aafPosition_t  origin,
        IAAFTimelineMobSlot ** ppNewSlot)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for pSegment
  //
  ImplAAFSegment * internalpSegment = NULL;
  if (pSegment)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pSegment->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpSegment = static_cast<ImplAAFSegment*>(arg);
      assert (internalpSegment);
    }
  //
  // set up for ppNewSlot
  //
  ImplAAFTimelineMobSlot * internalppNewSlot = NULL;
  ImplAAFTimelineMobSlot ** pinternalppNewSlot = NULL;
  if (ppNewSlot)
    {
      pinternalppNewSlot = &internalppNewSlot;
    }

  try
    {
      hr = ptr->AppendNewTimelineSlot
       (editRate,
        internalpSegment,
        slotID,
        pSlotName,
        origin,
        pinternalppNewSlot);
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
  // no cleanup necessary for pSegment
  //
  //
  // cleanup for ppNewSlot
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewSlot)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewSlot->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFTimelineMobSlot, (void **)ppNewSlot);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewSlot->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::GetMobInfo (aafTimeStamp_t *  pLastModified,
        aafTimeStamp_t *  pCreationTime,
        aafCharacter *  pName,
        aafInt32  bufSize)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetMobInfo
       (pLastModified,
        pCreationTime,
        pName,
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
    CAAFMob::OffsetToMobTimecode (IAAFSegment * pTcSeg,
        aafPosition_t *  pOffset,
        aafTimecode_t *  pResult)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for pTcSeg
  //
  ImplAAFSegment * internalpTcSeg = NULL;
  if (pTcSeg)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pTcSeg->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpTcSeg = static_cast<ImplAAFSegment*>(arg);
      assert (internalpTcSeg);
    }

  try
    {
      hr = ptr->OffsetToMobTimecode
       (internalpTcSeg,
        pOffset,
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

  //
  // no cleanup necessary for pTcSeg
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::LookupSlot (aafSlotID_t  slotId,
        IAAFMobSlot ** ppDestSlot)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for ppDestSlot
  //
  ImplAAFMobSlot * internalppDestSlot = NULL;
  ImplAAFMobSlot ** pinternalppDestSlot = NULL;
  if (ppDestSlot)
    {
      pinternalppDestSlot = &internalppDestSlot;
    }

  try
    {
      hr = ptr->LookupSlot
       (slotId,
        pinternalppDestSlot);
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
  // cleanup for ppDestSlot
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppDestSlot)
        {
          pUnknown = static_cast<IUnknown *> (internalppDestSlot->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFMobSlot, (void **)ppDestSlot);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppDestSlot->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::ChangeRef (aafMobID_constref  oldMobID,
        aafMobID_constref  newMobID)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->ChangeRef
       (oldMobID,
        newMobID);
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
    CAAFMob::CloneExternal (aafDepend_t  resolveDependencies,
        aafIncMedia_t  includeMedia,
        IAAFFile * pDestFile,
        IAAFMob ** ppDestMob)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for resolveDependencies
  //
  if (! Is_aafDepend_t_Valid(resolveDependencies))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for includeMedia
  //
  if (! Is_aafIncMedia_t_Valid(includeMedia))
    return AAFRESULT_INVALID_ENUM_VALUE;
  //
  // set up for pDestFile
  //
  ImplAAFFile * internalpDestFile = NULL;
  if (pDestFile)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pDestFile->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpDestFile = static_cast<ImplAAFFile*>(arg);
      assert (internalpDestFile);
    }
  //
  // set up for ppDestMob
  //
  ImplAAFMob * internalppDestMob = NULL;
  ImplAAFMob ** pinternalppDestMob = NULL;
  if (ppDestMob)
    {
      pinternalppDestMob = &internalppDestMob;
    }

  try
    {
      hr = ptr->CloneExternal
       (resolveDependencies,
        includeMedia,
        internalpDestFile,
        pinternalppDestMob);
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
  // no cleanup necessary for pDestFile
  //
  //
  // cleanup for ppDestMob
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppDestMob)
        {
          pUnknown = static_cast<IUnknown *> (internalppDestMob->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFMob, (void **)ppDestMob);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppDestMob->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::Copy (aafCharacter_constptr  pDestMobName,
        IAAFMob ** ppDestMob)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for ppDestMob
  //
  ImplAAFMob * internalppDestMob = NULL;
  ImplAAFMob ** pinternalppDestMob = NULL;
  if (ppDestMob)
    {
      pinternalppDestMob = &internalppDestMob;
    }

  try
    {
      hr = ptr->Copy
       (pDestMobName,
        pinternalppDestMob);
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
  // cleanup for ppDestMob
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppDestMob)
        {
          pUnknown = static_cast<IUnknown *> (internalppDestMob->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFMob, (void **)ppDestMob);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppDestMob->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFMob::AppendKLVData (IAAFKLVData * pData)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for pData
  //
  ImplAAFKLVData * internalpData = NULL;
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
      internalpData = static_cast<ImplAAFKLVData*>(arg);
      assert (internalpData);
    }

  try
    {
      hr = ptr->AppendKLVData
       (internalpData);
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
    CAAFMob::CountKLVData (aafUInt32 *  pNumData)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountKLVData
       (pNumData);
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
    CAAFMob::GetKLVData (IEnumAAFKLVData ** ppEnum)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFKLVData * internalppEnum = NULL;
  ImplEnumAAFKLVData ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetKLVData
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFKLVData, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFMob::RemoveKLVData (IAAFKLVData * pData)
{
  HRESULT hr;

  ImplAAFMob * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFMob*> (pO);
  assert (ptr);

  //
  // set up for pData
  //
  ImplAAFKLVData * internalpData = NULL;
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
      internalpData = static_cast<ImplAAFKLVData*>(arg);
      assert (internalpData);
    }

  try
    {
      hr = ptr->RemoveKLVData
       (internalpData);
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

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFMob::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFMob)) 
    { 
        *ppvObj = (IAAFMob *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFObject::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFMob)


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



#include "CAAFEssenceGroup.h"
#include "ImplAAFEssenceGroup.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFSourceClip.h"
#include "ImplAAFSourceClip.h"

#include "CAAFDataDef.h"
#include "ImplAAFDataDef.h"

// CLSID for AAFEssenceGroup 
// {e58a8568-2a3e-11D2-bfa4-006097116212}
EXTERN_C const CLSID CLSID_AAFEssenceGroup = { 0xe58a8568, 0x2a3e, 0x11D2, { 0xbf, 0xa4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFEssenceGroup::CAAFEssenceGroup (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFSegment (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFEssenceGroup * newRep;
      newRep = new ImplAAFEssenceGroup;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFEssenceGroup::~CAAFEssenceGroup ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceGroup::SetStillFrame (IAAFSourceClip * pStillFrame)
{
  HRESULT hr;

  ImplAAFEssenceGroup * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceGroup*> (pO);
  assert (ptr);

  //
  // set up for pStillFrame
  //
  ImplAAFSourceClip * internalpStillFrame = NULL;
  if (pStillFrame)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pStillFrame->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpStillFrame = static_cast<ImplAAFSourceClip*>(arg);
      assert (internalpStillFrame);
    }

  try
    {
      hr = ptr->SetStillFrame
       (internalpStillFrame);
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
  // no cleanup necessary for pStillFrame
  //
  return hr;
}

    

HRESULT STDMETHODCALLTYPE
    CAAFEssenceGroup::GetStillFrame (IAAFSourceClip ** ppStillFrame)
{
  HRESULT hr;

  ImplAAFEssenceGroup * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceGroup*> (pO);
  assert (ptr);

  //
  // set up for ppStillFrame
  //
  ImplAAFSourceClip * internalppStillFrame = NULL;
  ImplAAFSourceClip ** pinternalppStillFrame = NULL;
  if (ppStillFrame)
    {
      pinternalppStillFrame = &internalppStillFrame;
    }

  try
    {
      hr = ptr->GetStillFrame
       (pinternalppStillFrame);
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
  // cleanup for ppStillFrame
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppStillFrame)
        {
          pUnknown = static_cast<IUnknown *> (internalppStillFrame->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSourceClip, (void **)ppStillFrame);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppStillFrame->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceGroup::AppendChoice (IAAFSegment * pChoice)
{
  HRESULT hr;

  ImplAAFEssenceGroup * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceGroup*> (pO);
  assert (ptr);

  //
  // set up for pChoice
  //
  ImplAAFSegment * internalpChoice = NULL;
  if (pChoice)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pChoice->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpChoice = static_cast<ImplAAFSegment*>(arg);
      assert (internalpChoice);
    }

  try
    {
      hr = ptr->AppendChoice
       (internalpChoice);
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
  // no cleanup necessary for pChoice
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceGroup::PrependChoice (IAAFSegment * pChoice)
{
  HRESULT hr;

  ImplAAFEssenceGroup * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceGroup*> (pO);
  assert (ptr);

  //
  // set up for pChoice
  //
  ImplAAFSegment * internalpChoice = NULL;
  if (pChoice)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pChoice->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpChoice = static_cast<ImplAAFSegment*>(arg);
      assert (internalpChoice);
    }

  try
    {
      hr = ptr->PrependChoice
       (internalpChoice);
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
  // no cleanup necessary for pChoice
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceGroup::InsertChoiceAt (aafUInt32  index,
        IAAFSegment * pChoice)
{
  HRESULT hr;

  ImplAAFEssenceGroup * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceGroup*> (pO);
  assert (ptr);

  //
  // set up for pChoice
  //
  ImplAAFSegment * internalpChoice = NULL;
  if (pChoice)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pChoice->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpChoice = static_cast<ImplAAFSegment*>(arg);
      assert (internalpChoice);
    }

  try
    {
      hr = ptr->InsertChoiceAt
       (index,
        internalpChoice);
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
  // no cleanup necessary for pChoice
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceGroup::CountChoices (aafUInt32*   pCount)
{
  HRESULT hr;

  ImplAAFEssenceGroup * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceGroup*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountChoices
       (pCount);
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
    CAAFEssenceGroup::GetChoiceAt (aafUInt32  index,
        IAAFSegment  ** ppChoice)
{
  HRESULT hr;

  ImplAAFEssenceGroup * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceGroup*> (pO);
  assert (ptr);

  //
  // set up for ppChoice
  //
  ImplAAFSegment  * internalppChoice = NULL;
  ImplAAFSegment  ** pinternalppChoice = NULL;
  if (ppChoice)
    {
      pinternalppChoice = &internalppChoice;
    }

  try
    {
      hr = ptr->GetChoiceAt
       (index,
        pinternalppChoice);
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
  // cleanup for ppChoice
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppChoice)
        {
          pUnknown = static_cast<IUnknown *> (internalppChoice->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSegment , (void **)ppChoice);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppChoice->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceGroup::RemoveChoiceAt (aafUInt32  index)
{
  HRESULT hr;

  ImplAAFEssenceGroup * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceGroup*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->RemoveChoiceAt
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

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFEssenceGroup::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFEssenceGroup)) 
    { 
        *ppvObj = (IAAFEssenceGroup *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFSegment::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFEssenceGroup)


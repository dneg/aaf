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



#include "CAAFTransition.h"
#include "ImplAAFTransition.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>



#include "CAAFOperationGroup.h"
#include "ImplAAFOperationGroup.h"

#include "CAAFDataDef.h"
#include "ImplAAFDataDef.h"

#include "CAAFParameter.h"
#include "ImplAAFParameter.h"

#include "CAAFSourceReference.h"
#include "ImplAAFSourceReference.h"

// CLSID for AAFTransition 
// {E3F38311-2D3C-11d2-8043-006008143E6F}
EXTERN_C const CLSID CLSID_AAFTransition = { 0xE3F38311, 0x2D3C, 0x11d2, { 0x80, 0x43, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };





CAAFTransition::CAAFTransition (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFComponent (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTransition * newRep;
      newRep = new ImplAAFTransition;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTransition::~CAAFTransition ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTransition::Initialize (IAAFDataDef * pDataDef,
        aafLength_t  length,
        aafPosition_t  cutPoint,
        IAAFOperationGroup * op)
{
  HRESULT hr;

  ImplAAFTransition * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTransition*> (pO);
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
  // set up for op
  //
  ImplAAFOperationGroup * internalop = NULL;
  if (op)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = op->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalop = static_cast<ImplAAFOperationGroup*>(arg);
      assert (internalop);
    }

  try
    {
      hr = ptr->Initialize
       (internalpDataDef,
        length,
        cutPoint,
        internalop);
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
  // no cleanup necessary for op
  //
  return hr;
}
	 


HRESULT STDMETHODCALLTYPE
    CAAFTransition::GetCutPoint (aafPosition_t *  cutPoint)
{
  HRESULT hr;

  ImplAAFTransition * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTransition*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetCutPoint
       (cutPoint);
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
    CAAFTransition::GetOperationGroup (IAAFOperationGroup ** groupObj)
{
  HRESULT hr;

  ImplAAFTransition * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTransition*> (pO);
  assert (ptr);

  //
  // set up for groupObj
  //
  ImplAAFOperationGroup * internalgroupObj = NULL;
  ImplAAFOperationGroup ** pinternalgroupObj = NULL;
  if (groupObj)
    {
      pinternalgroupObj = &internalgroupObj;
    }

  try
    {
      hr = ptr->GetOperationGroup
       (pinternalgroupObj);
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
  // cleanup for groupObj
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalgroupObj)
        {
          pUnknown = static_cast<IUnknown *> (internalgroupObj->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFOperationGroup, (void **)groupObj);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalgroupObj->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFTransition::SetCutPoint (aafPosition_t  cutPoint)
{
  HRESULT hr;

  ImplAAFTransition * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTransition*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetCutPoint
       (cutPoint);
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
    CAAFTransition::SetOperationGroup (IAAFOperationGroup * opgroup)
{
  HRESULT hr;

  ImplAAFTransition * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTransition*> (pO);
  assert (ptr);

  //
  // set up for opgroup
  //
  ImplAAFOperationGroup * internalopgroup = NULL;
  if (opgroup)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = opgroup->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalopgroup = static_cast<ImplAAFOperationGroup*>(arg);
      assert (internalopgroup);
    }

  try
    {
      hr = ptr->SetOperationGroup
       (internalopgroup);
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
  // no cleanup necessary for opgroup
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
HRESULT CAAFTransition::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTransition)) 
    { 
        *ppvObj = (IAAFTransition *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFComponent::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTransition)


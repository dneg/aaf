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



#include "CAAFCommentMarker.h"
#include "ImplAAFCommentMarker.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>




#include "CAAFSourceReference.h"
#include "ImplAAFSourceReference.h"


// CLSID for AAFCommentMarker 
// {e684d789-b935-11d2-bf9d-00104bc9156d}
EXTERN_C const CLSID CLSID_AAFCommentMarker = { 0xe684d789, 0xb935, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };





CAAFCommentMarker::CAAFCommentMarker (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFEvent (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFCommentMarker * newRep;
      newRep = new ImplAAFCommentMarker;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFCommentMarker::~CAAFCommentMarker ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFCommentMarker::GetAnnotation (IAAFSourceReference ** ppResult)
{
  HRESULT hr;

  ImplAAFCommentMarker * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFCommentMarker*> (pO);
  assert (ptr);

  //
  // set up for ppResult
  //
  ImplAAFSourceReference * internalppResult = NULL;
  ImplAAFSourceReference ** pinternalppResult = NULL;
  if (ppResult)
    {
      pinternalppResult = &internalppResult;
    }

  try
    {
      hr = ptr->GetAnnotation
       (pinternalppResult);
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
  // cleanup for ppResult
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppResult)
        {
          pUnknown = static_cast<IUnknown *> (internalppResult->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSourceReference, (void **)ppResult);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppResult->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFCommentMarker::SetAnnotation (IAAFSourceReference * pAnnotation)
{
  HRESULT hr;

  ImplAAFCommentMarker * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFCommentMarker*> (pO);
  assert (ptr);

  //
  // set up for pAnnotation
  //
  ImplAAFSourceReference * internalpAnnotation = NULL;
  if (pAnnotation)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pAnnotation->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpAnnotation = static_cast<ImplAAFSourceReference*>(arg);
      assert (internalpAnnotation);
    }

  try
    {
      hr = ptr->SetAnnotation
       (internalpAnnotation);
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
  // no cleanup necessary for pAnnotation
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
HRESULT CAAFCommentMarker::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFCommentMarker)) 
    { 
        *ppvObj = (IAAFCommentMarker *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFEvent::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFCommentMarker)


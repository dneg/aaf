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



#include "CEnumAAFControlPoints.h"
#include "ImplEnumAAFControlPoints.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>

#include "CAAFControlPoint.h"
#include "ImplAAFControlPoint.h"

// CLSID for EnumAAFControlPoints 
// {B1A2139D-1A7D-11D2-BF78-00104BC9156D}
EXTERN_C const CLSID CLSID_EnumAAFControlPoints = { 0xB1A2139D, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CEnumAAFControlPoints::CEnumAAFControlPoints (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFControlPoints * newRep;
      newRep = new ImplEnumAAFControlPoints;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFControlPoints::~CEnumAAFControlPoints ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFControlPoints::NextOne (IAAFControlPoint ** ppControlPoints)
{
  HRESULT hr;

  ImplEnumAAFControlPoints * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFControlPoints*> (pO);
  assert (ptr);

  //
  // set up for ppControlPoints
  //
  ImplAAFControlPoint * internalppControlPoints = NULL;
  ImplAAFControlPoint ** pinternalppControlPoints = NULL;
  if (ppControlPoints)
    {
      pinternalppControlPoints = &internalppControlPoints;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppControlPoints);
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
  // cleanup for ppControlPoints
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppControlPoints)
        {
          pUnknown = static_cast<IUnknown *> (internalppControlPoints->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFControlPoint, (void **)ppControlPoints);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppControlPoints->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFControlPoints::Next (aafUInt32  count,
        IAAFControlPoint ** ppControlPoints,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFControlPoints * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFControlPoints*> (pO);
  assert (ptr);

  //
  // set up for ppControlPoints
  //
  ImplAAFControlPoint ** internalppControlPoints = NULL;
  assert (count >= 0);
  internalppControlPoints = new ImplAAFControlPoint*[count];
  assert (internalppControlPoints);

  ImplAAFControlPoint ** pinternalppControlPoints = NULL;
  if (ppControlPoints)
    {
      pinternalppControlPoints = internalppControlPoints;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppControlPoints,
        pNumFetched);
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
  // cleanup for ppControlPoints
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppControlPoints[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFControlPoint, (void **)(ppControlPoints+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppControlPoints[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppControlPoints;
  internalppControlPoints = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFControlPoints::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFControlPoints * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFControlPoints*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Skip
       (count);
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
    CEnumAAFControlPoints::Reset ()
{
  ImplEnumAAFControlPoints * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFControlPoints*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Reset();
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
    CEnumAAFControlPoints::Clone (IEnumAAFControlPoints ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFControlPoints * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFControlPoints*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFControlPoints * internalppEnum = NULL;
  ImplEnumAAFControlPoints ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->Clone
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFControlPoints, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
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
HRESULT CEnumAAFControlPoints::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFControlPoints)) 
    { 
        *ppvObj = (IEnumAAFControlPoints *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFControlPoints)


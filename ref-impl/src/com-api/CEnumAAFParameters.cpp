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



#include "CEnumAAFParameters.h"
#include "ImplEnumAAFParameters.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFParameter.h"
#include "ImplAAFParameter.h"

// CLSID for EnumAAFParameters 
// {7227261D-A8C9-11D3-A3ED-0004AC96A937}
EXTERN_C const CLSID CLSID_EnumAAFParameters = { 0x7227261D, 0xA8C9, 0x11D3, { 0xA3, 0xED, 0x00, 0x04, 0xAC, 0x96, 0xA9, 0x37 } };





CEnumAAFParameters::CEnumAAFParameters (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFParameters * newRep;
      newRep = new ImplEnumAAFParameters;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFParameters::~CEnumAAFParameters ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFParameters::NextOne (IAAFParameter ** ppParameter)
{
  HRESULT hr;

  ImplEnumAAFParameters * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFParameters*> (pO);
  assert (ptr);

  //
  // set up for ppParameter
  //
  ImplAAFParameter * internalppParameter = NULL;
  ImplAAFParameter ** pinternalppParameter = NULL;
  if (ppParameter)
    {
      pinternalppParameter = &internalppParameter;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppParameter);
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
  // cleanup for ppParameter
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppParameter)
        {
          pUnknown = static_cast<IUnknown *> (internalppParameter->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFParameter, (void **)ppParameter);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppParameter->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFParameters::Next (aafUInt32  count,
        IAAFParameter ** ppParameters,
        aafUInt32 *  pFetched)
{
  HRESULT hr;

  ImplEnumAAFParameters * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFParameters*> (pO);
  assert (ptr);

  //
  // set up for ppParameters
  //
  ImplAAFParameter ** internalppParameters = NULL;
  assert (count >= 0);
  internalppParameters = new ImplAAFParameter*[count];
  assert (internalppParameters);

  ImplAAFParameter ** pinternalppParameters = NULL;
  if (ppParameters)
    {
      pinternalppParameters = internalppParameters;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppParameters,
        pFetched);
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
  // cleanup for ppParameters
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppParameters[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFParameter, (void **)(ppParameters+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppParameters[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppParameters;
  internalppParameters = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFParameters::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFParameters * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFParameters*> (pO);
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
    CEnumAAFParameters::Reset ()
{
  ImplEnumAAFParameters * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFParameters*> (pO);
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
    CEnumAAFParameters::Clone (IEnumAAFParameters ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFParameters * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFParameters*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFParameters * internalppEnum = NULL;
  ImplEnumAAFParameters ** pinternalppEnum = NULL;
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFParameters, (void **)ppEnum);
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
HRESULT CEnumAAFParameters::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFParameters)) 
    { 
        *ppvObj = (IEnumAAFParameters *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFParameters)


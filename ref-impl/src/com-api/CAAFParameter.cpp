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



#include "CAAFParameter.h"
#include "ImplAAFParameter.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>



#include "CAAFParameterDef.h"
#include "ImplAAFParameterDef.h"

#include "CAAFTypeDef.h"
#include "ImplAAFTypeDef.h"

// CLSID for AAFParameter 
// {9c568a82-2096-11d2-bf98-00609711621}
EXTERN_C const CLSID CLSID_AAFParameter = { 0x9c568a82, 0x2096, 0x11d2, { 0xbf, 0x98, 0x00, 0x60, 0x97, 0x11, 0x62, 0x1 } };
 




CAAFParameter::CAAFParameter (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFObject (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFParameter * newRep;
      newRep = new ImplAAFParameter;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFParameter::~CAAFParameter ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFParameter::GetParameterDefinition (IAAFParameterDef ** ppParmDef)
{
  HRESULT hr;

  ImplAAFParameter * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFParameter*> (pO);
  assert (ptr);

  //
  // set up for ppParmDef
  //
  ImplAAFParameterDef * internalppParmDef = NULL;
  ImplAAFParameterDef ** pinternalppParmDef = NULL;
  if (ppParmDef)
    {
      pinternalppParmDef = &internalppParmDef;
    }

  try
    {
      hr = ptr->GetParameterDefinition
       (pinternalppParmDef);
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
  // cleanup for ppParmDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppParmDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppParmDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFParameterDef, (void **)ppParmDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppParmDef->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFParameter::GetTypeDefinition (IAAFTypeDef ** ppTypeDef)
{
  HRESULT hr;

  ImplAAFParameter * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFParameter*> (pO);
  assert (ptr);

  //
  // set up for ppTypeDef
  //
  ImplAAFTypeDef * internalppTypeDef = NULL;
  ImplAAFTypeDef ** pinternalppTypeDef = NULL;
  if (ppTypeDef)
    {
      pinternalppTypeDef = &internalppTypeDef;
    }

  try
    {
      hr = ptr->GetTypeDefinition
       (pinternalppTypeDef);
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
  // cleanup for ppTypeDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppTypeDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppTypeDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFTypeDef, (void **)ppTypeDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppTypeDef->ReleaseReference(); // We are through with this pointer.
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
HRESULT CAAFParameter::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFParameter)) 
    { 
        *ppvObj = (IAAFParameter *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFObject::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFParameter)


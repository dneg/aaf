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



#include "CAAFProperty.h"
#include "ImplAAFProperty.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyDef.h"
#include "ImplAAFPropertyDef.h"

#include "CAAFTypeDef.h"
#include "ImplAAFTypeDef.h"

#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

// CLSID for AAFProperty 
// {83057d73-9691-11d2-aa7f-80e6aa000000}
EXTERN_C const CLSID CLSID_AAFProperty = { 0x83057d73, 0x9691, 0x11d2, { 0xaa, 0x7f, 0x80, 0xe6, 0xaa, 0x00, 0x00, 0x00 } };





CAAFProperty::CAAFProperty (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFProperty * newRep;
      newRep = new ImplAAFProperty;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFProperty::~CAAFProperty ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFProperty::GetDefinition (IAAFPropertyDef ** ppPropDef)
{
  HRESULT hr;

  ImplAAFProperty * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFProperty*> (pO);
  assert (ptr);

  //
  // set up for ppPropDef
  //
  ImplAAFPropertyDef * internalppPropDef = NULL;
  ImplAAFPropertyDef ** pinternalppPropDef = NULL;
  if (ppPropDef)
    {
      pinternalppPropDef = &internalppPropDef;
    }

  try
    {
      hr = ptr->GetDefinition
       (pinternalppPropDef);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
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
  // cleanup for ppPropDef
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPropDef)
        {
          pUnknown = static_cast<IUnknown *> (internalppPropDef->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyDef, (void **)ppPropDef);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPropDef->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFProperty::GetValue (IAAFPropertyValue ** ppValue)
{
  HRESULT hr;

  ImplAAFProperty * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFProperty*> (pO);
  assert (ptr);

  //
  // set up for ppValue
  //
  ImplAAFPropertyValue * internalppValue = NULL;
  ImplAAFPropertyValue ** pinternalppValue = NULL;
  if (ppValue)
    {
      pinternalppValue = &internalppValue;
    }

  try
    {
      hr = ptr->GetValue
       (pinternalppValue);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
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
  // cleanup for ppValue
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppValue)
        {
          pUnknown = static_cast<IUnknown *> (internalppValue->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPropertyValue, (void **)ppValue);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppValue->ReleaseReference(); // We are through with this pointer.
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
HRESULT CAAFProperty::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFProperty)) 
    { 
        *ppvObj = (IAAFProperty *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFProperty)


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



#include "CAAFEssenceDescriptor.h"
#include "ImplAAFEssenceDescriptor.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFLocator.h"
#include "ImplAAFLocator.h"

#include "CEnumAAFLocators.h"
#include "ImplEnumAAFLocators.h"

// CLSID for AAFEssenceDescriptor 
// {94659784-2a3c-11D2-bfA4-006097116212}
EXTERN_C const CLSID CLSID_AAFEssenceDescriptor = { 0x94659784, 0x2a3c, 0x11D2, { 0xbf, 0xA4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFEssenceDescriptor::CAAFEssenceDescriptor (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFObject (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFEssenceDescriptor * newRep;
      newRep = new ImplAAFEssenceDescriptor;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFEssenceDescriptor::~CAAFEssenceDescriptor ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDescriptor::CountLocators (aafUInt32 *  pResult)
{
  HRESULT hr;

  ImplAAFEssenceDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountLocators
       (pResult);
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
    CAAFEssenceDescriptor::AppendLocator (IAAFLocator * pLocator)
{
  HRESULT hr;

  ImplAAFEssenceDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceDescriptor*> (pO);
  assert (ptr);

  //
  // set up for pLocator
  //
  ImplAAFLocator * internalpLocator = NULL;
  if (pLocator)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pLocator->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpLocator = static_cast<ImplAAFLocator*>(arg);
      assert (internalpLocator);
    }

  try
    {
      hr = ptr->AppendLocator
       (internalpLocator);
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
  // no cleanup necessary for pLocator
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceDescriptor::PrependLocator (IAAFLocator * pLocator)
{
  HRESULT hr;

  ImplAAFEssenceDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceDescriptor*> (pO);
  assert (ptr);

  //
  // set up for pLocator
  //
  ImplAAFLocator * internalpLocator = NULL;
  if (pLocator)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pLocator->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpLocator = static_cast<ImplAAFLocator*>(arg);
      assert (internalpLocator);
    }

  try
    {
      hr = ptr->PrependLocator
       (internalpLocator);
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
  // no cleanup necessary for pLocator
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceDescriptor::InsertLocatorAt (aafUInt32  index,
        IAAFLocator * pLocator)
{
  HRESULT hr;

  ImplAAFEssenceDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceDescriptor*> (pO);
  assert (ptr);

  //
  // set up for pLocator
  //
  ImplAAFLocator * internalpLocator = NULL;
  if (pLocator)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pLocator->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpLocator = static_cast<ImplAAFLocator*>(arg);
      assert (internalpLocator);
    }

  try
    {
      hr = ptr->InsertLocatorAt
       (index,
        internalpLocator);
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
  // no cleanup necessary for pLocator
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceDescriptor::GetLocatorAt (aafUInt32  index,
        IAAFLocator ** ppLocator)
{
  HRESULT hr;

  ImplAAFEssenceDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceDescriptor*> (pO);
  assert (ptr);

  //
  // set up for ppLocator
  //
  ImplAAFLocator * internalppLocator = NULL;
  ImplAAFLocator ** pinternalppLocator = NULL;
  if (ppLocator)
    {
      pinternalppLocator = &internalppLocator;
    }

  try
    {
      hr = ptr->GetLocatorAt
       (index,
        pinternalppLocator);
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
  // cleanup for ppLocator
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppLocator)
        {
          pUnknown = static_cast<IUnknown *> (internalppLocator->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFLocator, (void **)ppLocator);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppLocator->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceDescriptor::RemoveLocatorAt (aafUInt32  index)
{
  HRESULT hr;

  ImplAAFEssenceDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->RemoveLocatorAt
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
    CAAFEssenceDescriptor::GetLocators (IEnumAAFLocators ** ppEnum)
{
  HRESULT hr;

  ImplAAFEssenceDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceDescriptor*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFLocators * internalppEnum = NULL;
  ImplEnumAAFLocators ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->GetLocators
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
          hStat = pUnknown->QueryInterface(IID_IEnumAAFLocators, (void **)ppEnum);
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
HRESULT CAAFEssenceDescriptor::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFEssenceDescriptor)) 
    { 
        *ppvObj = (IAAFEssenceDescriptor *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFObject::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFEssenceDescriptor)


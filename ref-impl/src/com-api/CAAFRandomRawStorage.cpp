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



#include "CAAFRandomRawStorage.h"
#include "ImplAAFRandomRawStorage.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


// CLSID for AAFRandomRawStorage 
// {B1341781-71FE-11d4-B80D-0000863F2C27}
EXTERN_C const CLSID CLSID_AAFRandomRawStorage = { 0xB1341781, 0x71FE, 0x11d4, { 0xB8, 0x0D, 0x00, 0x00, 0x86, 0x3F, 0x2C, 0x27 } };





CAAFRandomRawStorage::CAAFRandomRawStorage (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRawStorage (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFRandomRawStorage * newRep;
      newRep = new ImplAAFRandomRawStorage;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFRandomRawStorage::~CAAFRandomRawStorage ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFRandomRawStorage::ReadAt (aafUInt64  position,
        aafMemPtr_t  buf,
        aafUInt32  bufSize,
        aafUInt32 *  pNumRead)
{
  HRESULT hr;

  ImplAAFRandomRawStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomRawStorage*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->ReadAt
       (position,
        buf,
        bufSize,
        pNumRead);
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
    CAAFRandomRawStorage::WriteAt (aafUInt64  position,
        aafMemConstPtr_t  buf,
        aafUInt32  bufSize,
        aafUInt32 *  pNumWritten)
{
  HRESULT hr;

  ImplAAFRandomRawStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomRawStorage*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->WriteAt
       (position,
        buf,
        bufSize,
        pNumWritten);
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
    CAAFRandomRawStorage::GetSize (aafUInt64 *  pSize)
{
  HRESULT hr;

  ImplAAFRandomRawStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomRawStorage*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetSize
       (pSize);
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
    CAAFRandomRawStorage::IsExtendable (aafBoolean_t *  pResult)
{
  HRESULT hr;

  ImplAAFRandomRawStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomRawStorage*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->IsExtendable
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
    CAAFRandomRawStorage::GetExtent (aafUInt64 *  pExtent)
{
  HRESULT hr;

  ImplAAFRandomRawStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomRawStorage*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetExtent
       (pExtent);
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
    CAAFRandomRawStorage::SetExtent (aafUInt64  extent)
{
  HRESULT hr;

  ImplAAFRandomRawStorage * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomRawStorage*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetExtent
       (extent);
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
HRESULT CAAFRandomRawStorage::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFRandomRawStorage)) 
    { 
        *ppvObj = (IAAFRandomRawStorage *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRawStorage::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFRandomRawStorage)


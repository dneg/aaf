//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "CAAFAuxiliaryDescriptor.h"
#include "ImplAAFAuxiliaryDescriptor.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>





// CLSID for AAFAuxiliaryDescriptor 
// {989d79d1-9aca-43f6-8be1-03ad2ad075d5}
EXTERN_C const CLSID CLSID_AAFAuxiliaryDescriptor = { 0x989d79d1, 0x9aca, 0x43f6, { 0x8b, 0xe1, 0x03, 0xad, 0x2a, 0xd0, 0x75, 0xd5 } };






CAAFAuxiliaryDescriptor::CAAFAuxiliaryDescriptor (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFPhysicalDescriptor (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFAuxiliaryDescriptor * newRep;
      newRep = new ImplAAFAuxiliaryDescriptor;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFAuxiliaryDescriptor::~CAAFAuxiliaryDescriptor ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFAuxiliaryDescriptor::Initialize ()
{
  ImplAAFAuxiliaryDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFAuxiliaryDescriptor*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Initialize();
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

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFAuxiliaryDescriptor::SetMimeType (aafCharacter_constptr  pMimeType)
{
  HRESULT hr;

  ImplAAFAuxiliaryDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFAuxiliaryDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetMimeType
       (pMimeType);
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

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFAuxiliaryDescriptor::GetMimeType (aafCharacter *  pMimeType,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFAuxiliaryDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFAuxiliaryDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetMimeType
       (pMimeType,
        bufSize);
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

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFAuxiliaryDescriptor::GetMimeTypeBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFAuxiliaryDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFAuxiliaryDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetMimeTypeBufLen
       (pBufSize);
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

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFAuxiliaryDescriptor::SetCharSet (aafCharacter_constptr  pCharSet)
{
  HRESULT hr;

  ImplAAFAuxiliaryDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFAuxiliaryDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetCharSet
       (pCharSet);
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

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFAuxiliaryDescriptor::GetCharSet (aafCharacter *  pCharSet,
        aafUInt32  bufSize)
{
  HRESULT hr;

  ImplAAFAuxiliaryDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFAuxiliaryDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetCharSet
       (pCharSet,
        bufSize);
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

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFAuxiliaryDescriptor::GetCharSetBufLen (aafUInt32 *  pBufSize)
{
  HRESULT hr;

  ImplAAFAuxiliaryDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFAuxiliaryDescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetCharSetBufLen
       (pBufSize);
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

  return hr;
}


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFAuxiliaryDescriptor::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFAuxiliaryDescriptor)) 
    { 
        *ppvObj = (IAAFAuxiliaryDescriptor *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFPhysicalDescriptor::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFAuxiliaryDescriptor)


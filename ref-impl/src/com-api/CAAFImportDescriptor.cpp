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



#include "CAAFImportDescriptor.h"
#include "ImplAAFImportDescriptor.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>



// CLSID for AAFImportDescriptor 
// {6c2d9e60-54a3-4c84-ae87-5f2fd3581de5}
EXTERN_C const CLSID CLSID_AAFImportDescriptor = { 0x6c2d9e60, 0x54a3, 0x4c84, { 0xae, 0x87, 0x5f, 0x2f, 0xd3, 0x58, 0x1d, 0xe5 } };





CAAFImportDescriptor::CAAFImportDescriptor (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFPhysicalDescriptor (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFImportDescriptor * newRep;
      newRep = new ImplAAFImportDescriptor;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFImportDescriptor::~CAAFImportDescriptor ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFImportDescriptor::Initialize ()
{
  ImplAAFImportDescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFImportDescriptor*> (pO);
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


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFImportDescriptor::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFImportDescriptor)) 
    { 
        *ppvObj = (IAAFImportDescriptor *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFPhysicalDescriptor::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFImportDescriptor)


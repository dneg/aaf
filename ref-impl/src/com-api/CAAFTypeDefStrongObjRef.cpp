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



#include "CAAFTypeDefStrongObjRef.h"
#include "ImplAAFTypeDefStrongObjRef.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFPropertyValue.h"
#include "ImplAAFPropertyValue.h"

#include "CAAFClassDef.h"
#include "ImplAAFClassDef.h"

// CLSID for AAFTypeDefStrongObjRef 
// {381e1f25-e7bb-11d2-842a-00600832acb8}
EXTERN_C const CLSID CLSID_AAFTypeDefStrongObjRef = { 0x381e1f25, 0xe7bb, 0x11d2, { 0x84, 0x2a, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };





CAAFTypeDefStrongObjRef::CAAFTypeDefStrongObjRef (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFTypeDefObjectRef (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFTypeDefStrongObjRef * newRep;
      newRep = new ImplAAFTypeDefStrongObjRef;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFTypeDefStrongObjRef::~CAAFTypeDefStrongObjRef ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFTypeDefStrongObjRef::Initialize (aafUID_constref  id,
        IAAFClassDef * pObjType,
        aafCharacter_constptr  pTypeName)
{
  HRESULT hr;

  ImplAAFTypeDefStrongObjRef * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFTypeDefStrongObjRef*> (pO);
  assert (ptr);

  //
  // set up for pObjType
  //
  ImplAAFClassDef * internalpObjType = NULL;
  if (pObjType)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pObjType->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpObjType = static_cast<ImplAAFClassDef*>(arg);
      assert (internalpObjType);
    }

  try
    {
      hr = ptr->Initialize
       (id,
        internalpObjType,
        pTypeName);
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
  // no cleanup necessary for pObjType
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
HRESULT CAAFTypeDefStrongObjRef::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFTypeDefStrongObjRef)) 
    { 
        *ppvObj = (IAAFTypeDefStrongObjRef *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFTypeDefObjectRef::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFTypeDefStrongObjRef)


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



#include "CAAFRandomFile.h"
#include "ImplAAFRandomFile.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>


#include "CAAFGetFileBits.h"
#include "ImplAAFGetFileBits.h"

#include "CAAFSetFileBits.h"
#include "ImplAAFSetFileBits.h"

// CLSID for AAFRandomFile 
// {3CC80283-72A8-11D4-B80D-0000863F2C27}
EXTERN_C const CLSID CLSID_AAFRandomFile = { 0x3CC80283, 0x72A8, 0x11D4, { 0xB8, 0x0D, 0x00, 0x00, 0x86, 0x3F, 0x2C, 0x27 } };





CAAFRandomFile::CAAFRandomFile (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFFile (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFRandomFile * newRep;
      newRep = new ImplAAFRandomFile;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFRandomFile::~CAAFRandomFile ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFRandomFile::GetFileBits (IAAFGetFileBits ** ppGetFileBits)
{
  HRESULT hr;

  ImplAAFRandomFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomFile*> (pO);
  assert (ptr);

  //
  // set up for ppGetFileBits
  //
  ImplAAFGetFileBits * internalppGetFileBits = NULL;
  ImplAAFGetFileBits ** pinternalppGetFileBits = NULL;
  if (ppGetFileBits)
    {
      pinternalppGetFileBits = &internalppGetFileBits;
    }

  try
    {
      hr = ptr->GetFileBits
       (pinternalppGetFileBits);
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
  // cleanup for ppGetFileBits
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppGetFileBits)
        {
          pUnknown = static_cast<IUnknown *> (internalppGetFileBits->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFGetFileBits, (void **)ppGetFileBits);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppGetFileBits->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFRandomFile::Revert ()
{
  ImplAAFRandomFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomFile*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Revert();
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
    CAAFRandomFile::SaveAsFile (IAAFFile * pDestFile)
{
  HRESULT hr;

  ImplAAFRandomFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomFile*> (pO);
  assert (ptr);

  //
  // set up for pDestFile
  //
  ImplAAFFile * internalpDestFile = NULL;
  if (pDestFile)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pDestFile->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpDestFile = static_cast<ImplAAFFile*>(arg);
      assert (internalpDestFile);
    }

  try
    {
      hr = ptr->SaveAsFile
       (internalpDestFile);
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
  // no cleanup necessary for pDestFile
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFRandomFile::SetFileBits (IAAFSetFileBits ** ppSetFileBits)
{
  HRESULT hr;

  ImplAAFRandomFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRandomFile*> (pO);
  assert (ptr);

  //
  // set up for ppSetFileBits
  //
  ImplAAFSetFileBits * internalppSetFileBits = NULL;
  ImplAAFSetFileBits ** pinternalppSetFileBits = NULL;
  if (ppSetFileBits)
    {
      pinternalppSetFileBits = &internalppSetFileBits;
    }

  try
    {
      hr = ptr->SetFileBits
       (pinternalppSetFileBits);
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
  // cleanup for ppSetFileBits
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppSetFileBits)
        {
          pUnknown = static_cast<IUnknown *> (internalppSetFileBits->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFSetFileBits, (void **)ppSetFileBits);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppSetFileBits->ReleaseReference(); // We are through with this pointer.
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
HRESULT CAAFRandomFile::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFRandomFile)) 
    { 
        *ppvObj = (IAAFRandomFile *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFFile::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFRandomFile)


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



#include "CAAFFile.h"
#include "ImplAAFFile.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>


#include "CAAFHeader.h"
#include "ImplAAFHeader.h"

// CLSID for AAFFile 
// {9346ACD2-2713-11d2-8035-006008143E6F}
EXTERN_C const CLSID CLSID_AAFFile = { 0x9346ACD2, 0x2713, 0x11d2, { 0x80, 0x35, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };





CAAFFile::CAAFFile (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFFile * newRep;
      newRep = new ImplAAFFile;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFFile::~CAAFFile ()
{
}


HRESULT STDMETHODCALLTYPE
    CAAFFile::Open ()
{
  ImplAAFFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFile*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Open();
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
    CAAFFile::SaveCopyAs (IAAFFile * pDestFile)
{
  HRESULT hr;

  ImplAAFFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFile*> (pO);
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
      hr = ptr->SaveCopyAs
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
    CAAFFile::GetHeader (IAAFHeader ** ppHeader)
{
  HRESULT hr;

  ImplAAFFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFile*> (pO);
  assert (ptr);

  //
  // set up for ppHeader
  //
  ImplAAFHeader * internalppHeader = NULL;
  ImplAAFHeader ** pinternalppHeader = NULL;
  if (ppHeader)
    {
      pinternalppHeader = &internalppHeader;
    }

  try
    {
      hr = ptr->GetHeader
       (pinternalppHeader);
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
  // cleanup for ppHeader
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppHeader)
        {
          pUnknown = static_cast<IUnknown *> (internalppHeader->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFHeader, (void **)ppHeader);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppHeader->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFFile::GetRevision (aafFileRev_t *  pRev)
{
  HRESULT hr;

  ImplAAFFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFile*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetRevision
       (pRev);
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
    CAAFFile::Save ()
{
  ImplAAFFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFile*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Save();
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
    CAAFFile::Close ()
{
  ImplAAFFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFile*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Close();
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
    CAAFFile::GetDictionary (IAAFDictionary ** ppDictionary)
{
  HRESULT hr;

  ImplAAFFile * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFFile*> (pO);
  assert (ptr);

  //
  // set up for ppDictionary
  //
  ImplAAFDictionary * internalppDictionary = NULL;
  ImplAAFDictionary ** pinternalppDictionary = NULL;
  if (ppDictionary)
    {
      pinternalppDictionary = &internalppDictionary;
    }

  try
    {
      hr = ptr->GetDictionary
       (pinternalppDictionary);
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
  // cleanup for ppDictionary
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppDictionary)
        {
          pUnknown = static_cast<IUnknown *> (internalppDictionary->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFDictionary, (void **)ppDictionary);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppDictionary->ReleaseReference(); // We are through with this pointer.
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
HRESULT CAAFFile::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFFile)) 
    { 
        *ppvObj = (IAAFFile *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFFile)


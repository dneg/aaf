

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



#include "ImplAAFModule.h"
#include "AAFResult.h"

#include "OMAssertions.h"

#include <assert.h>

#if defined(_MAC)
#pragma export on
#endif // #if defined(_MAC)











STDAPI
    AAFLoad (const char *  dllname)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFLoad
       (dllname);
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










STDAPI
    AAFFileOpenExistingRead (aafCharacter_constptr  pFileName,
        aafUInt32  modeFlags,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenExistingRead
       (pFileName,
        modeFlags,
        pinternalppFile);
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
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileOpenExistingModify (aafCharacter_constptr  pFileName,
        aafUInt32  modeFlags,
        aafProductIdentification_t *  pIdent,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenExistingModify
       (pFileName,
        modeFlags,
        pIdent,
        pinternalppFile);
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
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileOpenNewModify (aafCharacter_constptr  pFileName,
        aafUInt32  modeFlags,
        aafProductIdentification_t *  pIdent,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenNewModify
       (pFileName,
        modeFlags,
        pIdent,
        pinternalppFile);
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
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileOpenTransient (aafProductIdentification_t *  pIdent,
        IAAFFile ** ppFile)
{
  HRESULT hr;

  //
  // set up for ppFile
  //
  ImplAAFFile * internalppFile = NULL;
  ImplAAFFile ** pinternalppFile = NULL;
  if (ppFile)
    {
      pinternalppFile = &internalppFile;
    }

  try
    {
      hr = ImplAAFFileOpenTransient
       (pIdent,
        pinternalppFile);
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
  // cleanup for ppFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFFileIsAAFFile (aafCharacter_constptr  pFileName,
        aafUID_t *  pAAFFileKind,
        aafBool *  pFileIsAAFFile)
{
  HRESULT hr;


  try
    {
      hr = ImplAAFFileIsAAFFile
       (pFileName,
        pAAFFileKind,
        pFileIsAAFFile);
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



STDAPI
    AAFGetPluginManager (IAAFPluginManager ** ppPluginManager)
{
  HRESULT hr;

  //
  // set up for ppPluginManager
  //
  ImplAAFPluginManager * internalppPluginManager = NULL;
  ImplAAFPluginManager ** pinternalppPluginManager = NULL;
  if (ppPluginManager)
    {
      pinternalppPluginManager = &internalppPluginManager;
    }

  try
    {
      hr = ImplAAFGetPluginManager
       (pinternalppPluginManager);
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
  // cleanup for ppPluginManager
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppPluginManager)
        {
          pUnknown = static_cast<IUnknown *> (internalppPluginManager->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFPluginManager, (void **)ppPluginManager);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppPluginManager->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFCreateRawStorageMemory (aafFileAccess_t  access,
        IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr;

  //
  // set up for ppNewRawStorage
  //
  ImplAAFRawStorage * internalppNewRawStorage = NULL;
  ImplAAFRawStorage ** pinternalppNewRawStorage = NULL;
  if (ppNewRawStorage)
    {
      pinternalppNewRawStorage = &internalppNewRawStorage;
    }

  try
    {
      hr = ImplAAFCreateRawStorageMemory
       (access,
        pinternalppNewRawStorage);
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
  // cleanup for ppNewRawStorage
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewRawStorage)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewRawStorage->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFRawStorage, (void **)ppNewRawStorage);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewRawStorage->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFCreateRawStorageDisk (aafCharacter_constptr  pFilename,
        aafFileExistence_t  existence,
        aafFileAccess_t  access,
        IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr;

  //
  // set up for ppNewRawStorage
  //
  ImplAAFRawStorage * internalppNewRawStorage = NULL;
  ImplAAFRawStorage ** pinternalppNewRawStorage = NULL;
  if (ppNewRawStorage)
    {
      pinternalppNewRawStorage = &internalppNewRawStorage;
    }

  try
    {
      hr = ImplAAFCreateRawStorageDisk
       (pFilename,
        existence,
        access,
        pinternalppNewRawStorage);
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
  // cleanup for ppNewRawStorage
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewRawStorage)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewRawStorage->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFRawStorage, (void **)ppNewRawStorage);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewRawStorage->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



STDAPI
    AAFCreateAAFFileOnRawStorage (IAAFRawStorage *  pRawStorage,
        aafFileExistence_t  existence,
        aafFileAccess_t  access,
        aafUID_constptr  pFileKind,
        aafUInt32  modeFlags,
        aafProductIdentification_constptr  pIdent,
        IAAFFile ** ppNewFile)
{
  HRESULT hr;

  //
  // set up for ppNewFile
  //
  ImplAAFFile * internalppNewFile = NULL;
  ImplAAFFile ** pinternalppNewFile = NULL;
  if (ppNewFile)
    {
      pinternalppNewFile = &internalppNewFile;
    }

  try
    {
      hr = ImplAAFCreateAAFFileOnRawStorage
       (pRawStorage,
        existence,
        access,
        pFileKind,
        modeFlags,
        pIdent,
        pinternalppNewFile);
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
  // cleanup for ppNewFile
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppNewFile)
        {
          pUnknown = static_cast<IUnknown *> (internalppNewFile->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFFile, (void **)ppNewFile);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppNewFile->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}

#if defined(_MAC)
#pragma export off
#endif // #if defined(_MAC)


//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Declare the public interface that must be implemented.
#include "aaflib.h"

#include "AAFResult.h"
#include "AAFSDKBuild.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//
// Define the platform specific default dll name.
//
#if defined( OS_WINDOWS )
#define DEFAULT_AAFDLL_NAME "AAFCOAPI.dll"
#elif defined( OS_DARWIN )
#define DEFAULT_AAFDLL_NAME "libcom-api.dylib"
#elif defined( OS_UNIX )
#define DEFAULT_AAFDLL_NAME "libcom-api.so"
#else
#error Unknown operating system
#endif



// Dynamic loading can be disabled using the Debug-static build.
#ifndef DISABLE_DYNAMIC_LOADING


//
// Initialize the AUID's.
//
#define INIT_AUID
#include "AAFStoredObjectIDs.h"
#undef INIT_AUID


//
// Initialize static singleton data.
//
AAFDLL * AAFDLL::_singleton = NULL;


//
// Helper class to make sure that the AAFDLL is unloaded properly.
// When the C++ runtime exits the destructor will be called and
// the AAFUnload() will be called. This "feature" has been added
// so that the client code is not required to call AAFLoad() or
// AAFUnload(). 
//
struct AAFUnloadWrapper
{
  ~AAFUnloadWrapper() { AAFUnload(); }
};

static AAFUnloadWrapper g_GuarenteeAAFUnload;


//***********************************************************
//
// AAFLoad()
//
// Utility to load the appropriate implementation DLL and
// initialize the following functions to call the corresponding
// entry point exported from the DLL.
// 
STDAPI AAFLoad(const char * dllname)
{
  AAFDLL *pAAFDLL = AAFDLL::GetAAFDLL();
  if (NULL != pAAFDLL)
    return AAFRESULT_ALREADY_INITIALIZED;

  // Create the appropriate dll wrapper
  pAAFDLL = AAFDLL::MakeAAFDLL();
  if (NULL == pAAFDLL)
    return AAFRESULT_NOMEMORY;

  // Attempt to load the dll and initialize the entry points.
  // If the load fails cleanup immediately.
  HRESULT hr = pAAFDLL->Load(dllname);
  if (S_OK != hr)
    AAFUnload();
  return hr;
}


//***********************************************************
//
// AAFUnload()
//
// Utility to cleanup the loaded entry points and unload the
// dll.
// 
STDAPI AAFUnload()
{
  // Get the dll wrapper
  AAFDLL *pAAFDLL = AAFDLL::GetAAFDLL();
  if (NULL != pAAFDLL)
  {
    // Attempt to unload the dll and cleanup the entry points
    HRESULT hr = pAAFDLL->Unload();
    if (SUCCEEDED(hr))
    {
      delete pAAFDLL;
    }
    return hr;
  }
  else
    return S_OK;
}



//***********************************************************
//
// LoadIfNecessary()
// 
static HRESULT LoadIfNecessary(AAFDLL **ppAAFDLL)
{
  HRESULT hr = S_OK;

  // Get the dll wrapper
  AAFDLL *pAAFDLL = AAFDLL::GetAAFDLL();
  if (NULL == pAAFDLL)
  {
    hr = AAFLoad(NULL);
    if (SUCCEEDED(hr))
    {
      pAAFDLL = AAFDLL::GetAAFDLL();
      assert(pAAFDLL != 0); // "Valid AAFDLL wrapper"
    }
  }
  
  *ppAAFDLL = pAAFDLL;
  return hr;
}


//***********************************************************
//
// AAFGetStaticLibraryVersion()
//
// Provides the version of this static library.
//
static const aafProductVersion_t impl_version = {
  AAF_MAJOR_VERSION,
  AAF_MINOR_VERSION,
  AAF_MAINT_RELEASE,
  AAF_PATCH_LEVEL,
  AAF_RELEASE_STAGE
};
STDAPI AAFGetStaticLibraryVersion
  (aafProductVersion_t *  pVersion)
{
  if (pVersion == NULL)
    return AAFRESULT_NULL_PARAM;
  
  *pVersion = impl_version;
  return S_OK;
}

//***********************************************************
//
// AAFFileOpenExistingRead()
// 
STDAPI AAFFileOpenExistingRead (
  const wchar_t *  pFileName,
  aafUInt32  modeFlags,
  IAAFFile ** ppFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
 
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenExistingRead(pFileName, modeFlags, ppFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}


//***********************************************************
//
// AAFFileOpenExistingModify()
// 
STDAPI AAFFileOpenExistingModify (
  const wchar_t *  pFileName,
  aafUInt32  modeFlags,
  aafProductIdentification_t *  pIdent,
  IAAFFile ** ppFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenExistingModify(pFileName, modeFlags, pIdent, ppFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFFileOpenNewModify()
//
// 
STDAPI AAFFileOpenNewModify (
  const wchar_t *  pFileName,
  aafUInt32  modeFlags,
  aafProductIdentification_t *  pIdent,
  IAAFFile ** ppFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenNewModify(pFileName, modeFlags, pIdent, ppFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFFileOpenNewModifyEx()
//
// 
STDAPI AAFFileOpenNewModifyEx (
  const wchar_t *  pFileName,
  aafUID_constptr  pFileKind,
  aafUInt32  modeFlags,
  aafProductIdentification_t *  pIdent,
  IAAFFile ** ppFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenNewModifyEx(pFileName, pFileKind, modeFlags, pIdent, ppFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFFileOpenTransient()
//
STDAPI AAFFileOpenTransient (
  aafProductIdentification_t *  pIdent,
  IAAFFile ** ppFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenTransient(pIdent, ppFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

//***********************************************************
//
// AAFFileIsAAFFile()
//
//
STDAPI AAFFileIsAAFFile (
    aafCharacter_constptr  pFileName,
    aafUID_t *  pAAFFileKind,
    aafBool *  pFileIsAAFFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->IsAAFFile(pFileName, pAAFFileKind, pFileIsAAFFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

//***********************************************************
//
// AAFRawStorageIsAAFFile()
//
//
STDAPI AAFRawStorageIsAAFFile (
    IAAFRawStorage * pStorage,
    aafUID_t *  pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->RawStorageIsAAFFile(pStorage, pAAFFileKind, pRawStorageIsAAFFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFFileIsAAFFileKind()
//
STDAPI AAFFileIsAAFFileKind (
  aafCharacter_constptr  pFileName,
  aafUID_constptr pAAFFileKind,
  aafBool *  pFileIsAAFFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->FileIsAAFFileKind(pFileName, pAAFFileKind, pFileIsAAFFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFRawStorageIsAAFFileKind()
//
//
STDAPI AAFRawStorageIsAAFFileKind (
  IAAFRawStorage *  pStorage,
  aafUID_constptr pAAFFileKind,
  aafBool *  pRawStorageIsAAFFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->RawStorageIsAAFFileKind(pStorage, pAAFFileKind, pRawStorageIsAAFFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFCreateRawStorageMemory()
//
STDAPI AAFCreateRawStorageMemory (
  aafFileAccess_t  access,
  IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->CreateRawStorageMemory(access,
										 ppNewRawStorage);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFCreateRawStorageDisk()
//
STDAPI AAFCreateRawStorageDisk (
  aafCharacter_constptr  filename,
  aafFileExistence_t  existence,
  aafFileAccess_t  access,
  IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->CreateRawStorageDisk
	  (filename,
	   existence,
	   access,
	   ppNewRawStorage);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFCreateRawStorageCachedDisk()
//
STDAPI AAFCreateRawStorageCachedDisk (
  aafCharacter_constptr  filename,
  aafFileExistence_t  existence,
  aafFileAccess_t  access,
  aafUInt32  pageCount,
  aafUInt32  pageSize,
  IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->CreateRawStorageCachedDisk
	  (filename,
	   existence,
	   access,
	   pageCount,
	   pageSize,
	   ppNewRawStorage);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

//***********************************************************
//
// AAFCreateRawStorageCached()
//
STDAPI AAFCreateRawStorageCached (
  IAAFRawStorage * pRawStorage,
  aafUInt32  pageCount,
  aafUInt32  pageSize,
  IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->CreateRawStorageCached
	  (pRawStorage,
	   pageCount,
	   pageSize,
	   ppNewRawStorage);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

//***********************************************************
//
// AAFCreateRawStorageCached2()
//
STDAPI AAFCreateRawStorageCached2 (
  IAAFRawStorage * pRawStorage,
  aafUInt32  pageCount,
  aafUInt32  pageSize,
  IAAFCachePageAllocator*  pCachePageAllocator,
  IAAFRawStorage ** ppNewRawStorage)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->CreateRawStorageCached2
	  (pRawStorage,
	   pageCount,
	   pageSize,
       pCachePageAllocator,
	   ppNewRawStorage);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFCreateAAFFileOnRawStorage()
//
STDAPI AAFCreateAAFFileOnRawStorage (
  IAAFRawStorage * pRawStorage,
  aafFileExistence_t  existence,
  aafFileAccess_t  access,
  aafUID_constptr  pFileKind,
  aafUInt32  modeFlags,
  aafProductIdentification_constptr  pIdent,
  IAAFFile ** ppNewFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->CreateAAFFileOnRawStorage
	  (pRawStorage,
	   existence,
	   access,
	   pFileKind,
	   modeFlags,
	   pIdent,
	   ppNewFile);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFGetPluginManager()
//
STDAPI AAFGetPluginManager (
  IAAFPluginManager ** ppPluginManager)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;


  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->GetPluginManager(ppPluginManager);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}


//***********************************************************
//
// AAFSetProgressCallback()
//
STDAPI AAFSetProgressCallback (
  IAAFProgress * pProgress)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;


  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->SetProgressCallback(pProgress);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



//***********************************************************
//
// AAFGetFileEncodings()
//
STDAPI AAFGetFileEncodings (
  IEnumAAFFileEncodings ** ppFileEncodings)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->GetFileEncodings
	  (ppFileEncodings);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}






STDAPI
AAFGetLibraryVersion
  (aafProductVersion_t *  pVersion)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->GetLibraryVersion
	  (pVersion);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

STDAPI AAFGetLibraryPathNameBufLen
  (aafUInt32 *  pBufSize)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->GetLibraryPathNameBufLen
	  (pBufSize);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

STDAPI AAFGetLibraryPathName
  (aafCharacter *  pLibraryPathName,
   aafUInt32  bufSize)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->GetLibraryPathName
	  (pLibraryPathName,
       bufSize);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

STDAPI AAFResultToTextBufLen (
    AAFRESULT  result,
    aafUInt32 *   pResultTextSize)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->ResultToTextBufLen
	  (result,
           pResultTextSize);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

STDAPI AAFResultToText (
    AAFRESULT  result,
    aafCharacter *  pResultText,
    aafUInt32  resultTextSize)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->ResultToText
	  (result,
           pResultText,
           resultTextSize);
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

//***********************************************************
//
// Constructor for the base class
AAFDLL::AAFDLL() :
  _libHandle(NULL)
{
  assert(_singleton == 0);
  _singleton = this;
  
  // Initialize all funtions pointers
  ClearEntrypoints();
}



//***********************************************************
//
// Destructor for the base class
AAFDLL::~AAFDLL()
{
  assert(this == _singleton);

  // Reset the entry point function pointers to NULL.
  ClearEntrypoints();

  // Clear the reference to the singleton
  _singleton = NULL;
}



//***********************************************************
//
// Static accessor method for the singleton.
//
AAFDLL * AAFDLL::GetAAFDLL()
{
  return _singleton;
}


//***********************************************************
//
// Factory function just returns an instance of the currect platform
// dll wrapper object.
AAFDLL * AAFDLL::MakeAAFDLL()
{
  AAFDLL *pAAFDLL =  new AAFDLL;
  return pAAFDLL;
}


//***********************************************************
//
HRESULT AAFDLL::Load(const char *dllname)
{
  HRESULT rc = S_OK;


  if (NULL == dllname)
  { // use a realistic default name.
    dllname = DEFAULT_AAFDLL_NAME;
  }

  // Attempt to load the library.
  rc = ::AAFLoadLibrary(dllname, &_libHandle);

  if (AAFRESULT_SUCCESS != rc)
    return rc;

  //
  // Attempt to initialize the entry points...
  //
  rc = ::AAFFindSymbol(_libHandle, "AAFFileOpenExistingRead", (AAFSymbolAddr *)&_pfnOpenExistingRead);
  if (AAFRESULT_FAILED(rc))
    return rc;

  rc = ::AAFFindSymbol(_libHandle, "AAFFileOpenExistingModify", (AAFSymbolAddr *)&_pfnOpenExistingModify);
  if (AAFRESULT_FAILED(rc))
    return rc;

  rc = ::AAFFindSymbol(_libHandle, "AAFFileOpenNewModify", (AAFSymbolAddr *)&_pfnOpenNewModify);
  if (AAFRESULT_FAILED(rc))
    return rc;

  rc = ::AAFFindSymbol(_libHandle, "AAFFileOpenNewModifyEx", (AAFSymbolAddr *)&_pfnOpenNewModifyEx);
  // Ignore failure

  rc = ::AAFFindSymbol(_libHandle, "AAFFileOpenTransient", (AAFSymbolAddr *)&_pfnOpenTransient);
  if (AAFRESULT_FAILED(rc))
    return rc;

  rc = ::AAFFindSymbol(_libHandle, "AAFFileIsAAFFile", (AAFSymbolAddr *)&_pfnIsAAFFile);
  if (AAFRESULT_FAILED(rc))
    return rc;

  rc = ::AAFFindSymbol(_libHandle, "AAFRawStorageIsAAFFile", (AAFSymbolAddr *)&_pfnRawStorageIsAAFFile);
  // Ignore failure

  rc = ::AAFFindSymbol(_libHandle, "AAFGetPluginManager", (AAFSymbolAddr *)&_pfnGetPluginManager);
  if (AAFRESULT_FAILED(rc))
    return rc;

  // These callbacks did not exist in DR4 or earlier toolkits. Ignore the error
  // return if the first new exported function cannot be found. If the first
  // exported function is present then all three must be present for the dll
  // to be valid.
  rc = ::AAFFindSymbol(_libHandle,
					   "AAFCreateRawStorageMemory",
					   (AAFSymbolAddr *)&_pfnCreateRawStorageMemory);
  if (AAFRESULT_SUCCEEDED(rc))
  {
    rc = ::AAFFindSymbol(_libHandle,
  					   "AAFCreateRawStorageDisk",
  					   (AAFSymbolAddr *)&_pfnCreateRawStorageDisk);
    if (AAFRESULT_FAILED(rc))
      return rc;

    rc = ::AAFFindSymbol(_libHandle,
  					   "AAFCreateAAFFileOnRawStorage",
  					   (AAFSymbolAddr *)&_pfnCreateAAFFileOnRawStorage);
    if (AAFRESULT_FAILED(rc))
      return rc;
  }

  rc = ::AAFFindSymbol(_libHandle,
  					   "AAFSetProgressCallback",
  					   (AAFSymbolAddr *)&_pfnSetProgressCallback);

  rc = ::AAFFindSymbol(_libHandle,
  					   "AAFCreateRawStorageCachedDisk",
  					   (AAFSymbolAddr *)&_pfnCreateRawStorageCachedDisk);
  // Ignore failure

  rc = ::AAFFindSymbol(_libHandle,
  					   "AAFCreateRawStorageCached",
  					   (AAFSymbolAddr *)&_pfnCreateRawStorageCached);
  // Ignore failure
  rc = ::AAFFindSymbol(_libHandle,
  					   "AAFCreateRawStorageCached2",
  					   (AAFSymbolAddr *)&_pfnCreateRawStorageCached2);
  // Ignore failure

  rc = ::AAFFindSymbol(_libHandle,
					   "AAFGetFileEncodings",
  					   (AAFSymbolAddr *)&_pfnGetFileEncodings);
  // Ignore failure

  rc = ::AAFFindSymbol(_libHandle,
					   "AAFFileIsAAFFileKind",
  					   (AAFSymbolAddr *)&_pfnFileIsAAFFileKind);

  rc = ::AAFFindSymbol(_libHandle,
					   "AAFRawStorageIsAAFFileKind",
  					   (AAFSymbolAddr *)&_pfnRawStorageIsAAFFileKind);

  rc = ::AAFFindSymbol(_libHandle,
					   "AAFGetLibraryVersion",
  					   (AAFSymbolAddr *)&_pfnGetLibraryVersion);

  rc = ::AAFFindSymbol(_libHandle,
					   "AAFGetLibraryPathNameBufLen",
  					   (AAFSymbolAddr *)&_pfnGetLibraryPathNameBufLen);

  rc = ::AAFFindSymbol(_libHandle,
					   "AAFGetLibraryPathName",
  					   (AAFSymbolAddr *)&_pfnGetLibraryPathName);

  rc = ::AAFFindSymbol(_libHandle,
					   "AAFResultToTextBufLen",
  					   (AAFSymbolAddr *)&_pfnResultToTextBufLen);
  rc = ::AAFFindSymbol(_libHandle,
					   "AAFResultToText",
  					   (AAFSymbolAddr *)&_pfnResultToText);

  return AAFRESULT_SUCCESS;
}



//***********************************************************
//
HRESULT AAFDLL::Unload()
{
  HRESULT rc = S_OK;

  if (_libHandle)
  {
    rc = ::AAFUnloadLibrary(_libHandle);
    if (AAFRESULT_SUCCEEDED(rc))
    {
      // Reset the entry point function pointers to NULL.
      ClearEntrypoints();

      _libHandle = NULL;
    }
  }
  
  return rc;
}



//***********************************************************
//
// Resets all entry point function pointers to NULL.
void AAFDLL::ClearEntrypoints()
{
  _pfnOpenExistingRead = NULL;
  _pfnOpenExistingModify = NULL;
  _pfnOpenNewModify = NULL;
  _pfnOpenNewModifyEx = 0;
  _pfnOpenTransient = NULL;
  _pfnIsAAFFile = 0;
  _pfnRawStorageIsAAFFile = 0;
  _pfnFileIsAAFFileKind = 0;
  _pfnRawStorageIsAAFFileKind= 0;
  _pfnGetPluginManager = NULL;
  _pfnCreateRawStorageMemory = 0;
  _pfnCreateRawStorageDisk = 0;
  _pfnCreateRawStorageCachedDisk = 0;
  _pfnCreateRawStorageCached = 0;
  _pfnCreateRawStorageCached2 = 0;
  _pfnCreateAAFFileOnRawStorage = 0;
  _pfnSetProgressCallback = 0;
  _pfnGetFileEncodings = 0;
  _pfnGetLibraryVersion = 0;
  _pfnGetLibraryPathNameBufLen = 0;
  _pfnGetLibraryPathName = 0;
  _pfnResultToTextBufLen = 0;
  _pfnResultToText = 0;
}



//***********************************************************
//
// Wrapper functions for calling member entry points.
//

HRESULT AAFDLL::OpenExistingRead (
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile)
{
  assert(_pfnOpenExistingRead != 0); // "Valid dll callback function"
  return _pfnOpenExistingRead(pFileName, modeFlags, ppFile);  
}
  
HRESULT AAFDLL::OpenExistingModify (
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  assert(_pfnOpenExistingModify != 0); // "Valid dll callback function"
  return _pfnOpenExistingModify(pFileName, modeFlags, pIdent, ppFile);  
}

HRESULT AAFDLL::OpenNewModify (
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  assert(_pfnOpenNewModify != 0); // "Valid dll callback function"
  return _pfnOpenNewModify(pFileName, modeFlags, pIdent, ppFile);  
}

HRESULT AAFDLL::OpenNewModifyEx (
    const wchar_t *  pFileName,
	aafUID_constptr  pFileKind,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  // This entry point is not present in some versions of the DLL
  if (NULL == _pfnOpenNewModifyEx)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;

  return _pfnOpenNewModifyEx(pFileName, pFileKind, modeFlags, pIdent, ppFile);
}

HRESULT AAFDLL::OpenTransient (
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  assert(_pfnOpenTransient != 0); // "Valid dll callback function"
  return _pfnOpenTransient(pIdent, ppFile);  
}

HRESULT AAFDLL::IsAAFFile (
    aafCharacter_constptr  pFileName,
    aafUID_t *  pAAFFileKind,
    aafBool *  pFileIsAAFFile)
{
  // This function was previously implemented here but has now
  // been moved to the DLL. There was previously a stub in the
  // DLL that returned AAFRESULT_NOT_IMPLEMENTED (this stub
  // was not previously called). On Unix and Macintosh this stub
  // is exported from old DLLs, on Windows the stub is not
  // exported from old DLLs.
  // If we can't find the symbol we're using an old DLL on Windows.
  // If we can find the symbol and when we call into the DLL the
  // result is AAFRESULT_NOT_IMPLEMENTED we are using an old DLL
  // on Unix or Macintosh. In this case we turn the result into
  // AAFRESULT_DLL_SYMBOL_NOT_FOUND.
  //
  if (NULL == _pfnIsAAFFile)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;

  HRESULT hr = _pfnIsAAFFile( pFileName, pAAFFileKind, pFileIsAAFFile);
  if (hr == AAFRESULT_NOT_IMPLEMENTED)
    hr = AAFRESULT_DLL_SYMBOL_NOT_FOUND;
  return hr;
}

HRESULT AAFDLL::GetPluginManager (
  IAAFPluginManager ** ppPluginManager)
{
  assert(_pfnGetPluginManager != 0); // "Valid dll callback function"
  return _pfnGetPluginManager(ppPluginManager);  
}

HRESULT AAFDLL::RawStorageIsAAFFile (
    IAAFRawStorage * pStorage,
    aafUID_t *  pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile)
{
  // This entry point is not present in some versions of the DLL
  if (NULL == _pfnRawStorageIsAAFFile)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;

  return _pfnRawStorageIsAAFFile( pStorage, pAAFFileKind, pRawStorageIsAAFFile);
}

HRESULT AAFDLL::FileIsAAFFileKind (
    aafCharacter_constptr  pFileName,
    aafUID_constptr pAAFFileKind,
    aafBool *  pFileIsAAFFile)
{
  // This entry point is not present in some versions of the DLL
  if (NULL == _pfnFileIsAAFFileKind)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;

  return _pfnFileIsAAFFileKind(pFileName, pAAFFileKind, pFileIsAAFFile);
}

HRESULT AAFDLL::RawStorageIsAAFFileKind (
    IAAFRawStorage *  pStorage,
    aafUID_constptr pAAFFileKind,
    aafBool *  pRawStorageIsAAFFile)
{
  // This entry point is not present in some versions of the DLL
  if (NULL == _pfnRawStorageIsAAFFileKind)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;

  return _pfnRawStorageIsAAFFileKind(pStorage, pAAFFileKind, pRawStorageIsAAFFile);
}

HRESULT AAFDLL::CreateRawStorageMemory (
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage)
{
  // This callback did not exist in DR4 or earlier toolkits.
  if (NULL == _pfnCreateRawStorageMemory)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnCreateRawStorageMemory(access, ppNewRawStorage);
}

HRESULT AAFDLL::CreateRawStorageDisk (
    aafCharacter_constptr  filename,
    aafFileExistence_t  existence,
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage)
{
  // This callback did not exist in DR4 or earlier toolkits.
  if (NULL == _pfnCreateRawStorageDisk)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnCreateRawStorageDisk(filename, existence, access, ppNewRawStorage);  
}

HRESULT AAFDLL::CreateRawStorageCachedDisk (
    aafCharacter_constptr  filename,
    aafFileExistence_t  existence,
	aafFileAccess_t  access,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
	IAAFRawStorage ** ppNewRawStorage)
{
  // This callback did not exist in DR4 or earlier toolkits.
  if (NULL == _pfnCreateRawStorageCachedDisk)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnCreateRawStorageCachedDisk
	(filename,
	 existence,
	 access,
	 pageCount,
	 pageSize,
	 ppNewRawStorage);  
}

HRESULT AAFDLL::CreateRawStorageCached (
    IAAFRawStorage * pRawStorage,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
	IAAFRawStorage ** ppNewRawStorage)
{
  // This callback did not exist in DR4 or earlier toolkits.
  if (NULL == _pfnCreateRawStorageCached)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnCreateRawStorageCached
	(pRawStorage,
	 pageCount,
	 pageSize,
	 ppNewRawStorage);  
}

HRESULT AAFDLL::CreateRawStorageCached2 (
    IAAFRawStorage * pRawStorage,
    aafUInt32  pageCount,
    aafUInt32  pageSize,
    IAAFCachePageAllocator*  pCachePageAllocator,
	IAAFRawStorage ** ppNewRawStorage)
{
  if (NULL == _pfnCreateRawStorageCached2)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnCreateRawStorageCached2
	(pRawStorage,
	 pageCount,
	 pageSize,
     pCachePageAllocator,
	 ppNewRawStorage);  
}

HRESULT AAFDLL::CreateAAFFileOnRawStorage (
    IAAFRawStorage * pRawStorage,
	aafFileExistence_t  existence,
	aafFileAccess_t  access,
	aafUID_constptr  pFileKind,
	aafUInt32  modeFlags,
	aafProductIdentification_constptr  pIdent,
	IAAFFile ** ppNewFile)
{
  // This callback did not exist in DR4 or earlier toolkits.
  if (NULL == _pfnCreateAAFFileOnRawStorage)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnCreateAAFFileOnRawStorage
	(pRawStorage,
	 existence,
	 access,
	 pFileKind,
	 modeFlags,
	 pIdent,
	 ppNewFile);
}

HRESULT AAFDLL::SetProgressCallback (
  IAAFProgress * pProgress)
{
//  ASSERT("Valid dll callback function", _pfnCreateAAFFileOnRawStorage);
  // This callback did not exist in DR4 or earlier toolkits.
  if (NULL == _pfnSetProgressCallback)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnSetProgressCallback(pProgress);
}

HRESULT AAFDLL::GetFileEncodings (
		    IEnumAAFFileEncodings ** ppFileEncodings)
{
  // This callback did not exist in DR4 or earlier toolkits.
  if (NULL == _pfnGetFileEncodings)
      return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
      
    return _pfnGetFileEncodings
    	(ppFileEncodings);
}


HRESULT AAFDLL::GetLibraryVersion
  (aafProductVersion_t *  pVersion)
{
  // This callback did not exist earlier toolkits.
  if (NULL == _pfnGetLibraryVersion)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnGetLibraryVersion
	(pVersion);
}

HRESULT AAFDLL::GetLibraryPathNameBufLen
  (aafUInt32 *  pBufSize)
{
  // This callback did not exist earlier toolkits.
  if (NULL == _pfnGetLibraryPathNameBufLen)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnGetLibraryPathNameBufLen
	(pBufSize);
}

HRESULT AAFDLL::GetLibraryPathName
  (aafCharacter *  pLibraryPathName,
   aafUInt32  bufSize)
{
  // This callback did not exist earlier toolkits.
  if (NULL == _pfnGetLibraryPathName)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnGetLibraryPathName
	(pLibraryPathName,
     bufSize);
}

HRESULT AAFDLL::ResultToTextBufLen (
    AAFRESULT  result,
    aafUInt32 *   pResultTextSize)
{
  // This function did not exist earlier toolkits.
  if (NULL == _pfnResultToTextBufLen)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;


  return _pfnResultToTextBufLen(result, pResultTextSize);
}

HRESULT AAFDLL::ResultToText (
    AAFRESULT  result,
    aafCharacter *  pResultText,
    aafUInt32  resultTextSize)
{
  // This function did not exist earlier toolkits.
  if (NULL == _pfnResultToText)
    return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
    
  return _pfnResultToText(result, pResultText, resultTextSize);
}

#else

// Static builds.
//
// AAFLoad not needed since it is defined in CAAFModule.cpp
// But AAFUnload is not.
STDAPI AAFUnload()
{
  return S_OK;
}

#endif // DISABLE_DYNAMIC_LOADING

//=---------------------------------------------------------------------=
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



// Declare the public interface that must be implemented.
#include "aaflib.h"

#include "AAFResult.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//
// Define the platform specific default dll name.
//
#if defined( OS_WINDOWS )
#define DEFAULT_AAFDLL_NAME "AAFCOAPI.dll"
#elif defined( OS_MACOS )
#define DEFAULT_AAFDLL_NAME "AAFCOAPI.DLL (PPC)"
#elif defined( OS_UNIX )
#define DEFAULT_AAFDLL_NAME "libcom-api.so"
#else
#error Unknown operating system
#endif


// Use conditional to control inclusion of stream code.
#ifndef USE_IOSTREAM
#define USE_IOSTREAM 0
#endif


// ASSERT code copied from OM...

#ifdef _DEBUG

#if USE_IOSTREAM

#include <iostream.h>

void reportAssertionFailure(char* kind,
                            char* name,
                            char* expressionString,
                            char* routine,
                            char* fileName,
                            size_t lineNumber)
{
  cerr << kind << " \"" << name << "\" failed in routine \""
       << routine  << "\"." << endl;
  cerr << "The failure occurred at line " << lineNumber
       << " in file \"" << fileName << "\"." << endl;
  cerr << "The condition \"" << expressionString << "\" was violated." << endl;
  throw AAFRESULT_ASSERTION_VIOLATION;
}

#else // #if USE_IOSTREAM

void reportAssertionFailure(char*,
                            char*,
                            char*,
                            char*,
                            char*,
                            size_t)
{
  throw AAFRESULT_ASSERTION_VIOLATION;
}

#endif // #else // #if USE_IOSTREAM



#endif


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
  TRACE("LoadIfNecessary");
  HRESULT hr = S_OK;

  // Get the dll wrapper
  AAFDLL *pAAFDLL = AAFDLL::GetAAFDLL();
  if (NULL == pAAFDLL)
  {
    hr = AAFLoad(NULL);
    if (SUCCEEDED(hr))
    {
      pAAFDLL = AAFDLL::GetAAFDLL();
      ASSERT("Valid AAFDLL wrapper", pAAFDLL);
    }
  }
  
  *ppAAFDLL = pAAFDLL;
  return hr;
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
  TRACE("AAFFileOpenExistingRead");
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
  TRACE("AAFFileOpenExistingModify");
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
  TRACE("AAFFileOpenNewModify");
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
// AAFFileOpenTransient()
//
STDAPI AAFFileOpenTransient (
  aafProductIdentification_t *  pIdent,
  IAAFFile ** ppFile)
{
  TRACE("AAFFileOpenTransient");
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
  TRACE("AAFFileIsAAFFile");
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
// Constructor for the base class
AAFDLL::AAFDLL() :
  _libHandle(NULL)
{
  TRACE("AAFDLL::AAFDLL");
  ASSERT("There Can Be Only One!", NULL == _singleton);
  _singleton = this;
  
  // Initialize all funtions pointers
  ClearEntrypoints();
}



//***********************************************************
//
// Destructor for the base class
AAFDLL::~AAFDLL()
{
  TRACE("AAFDLL::~AAFDLL");
  ASSERT("There Can Be Only One!", this == _singleton);

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

  rc = ::AAFFindSymbol(_libHandle, "AAFFileOpenTransient", (AAFSymbolAddr *)&_pfnOpenTransient);
  if (AAFRESULT_FAILED(rc))
    return rc;

  rc = ::AAFFindSymbol(_libHandle, "AAFFileIsAAFFile", (AAFSymbolAddr *)&_pfnIsAAFFile);
  if (AAFRESULT_FAILED(rc))
    return rc;

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
  					   "AAFCreateRawStorageCachedDisk",
  					   (AAFSymbolAddr *)&_pfnCreateRawStorageCachedDisk);
  // Ignore failure

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
  _pfnOpenTransient = NULL;
  _pfnIsAAFFile = 0;
  _pfnGetPluginManager = NULL;
  _pfnCreateRawStorageMemory = 0;
  _pfnCreateRawStorageDisk = 0;
  _pfnCreateRawStorageCachedDisk = 0;
  _pfnCreateAAFFileOnRawStorage = 0;
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
  TRACE("AAFDLL::OpenExistingRead");
  ASSERT("Valid dll callback function", _pfnOpenExistingRead);
  
  return _pfnOpenExistingRead(pFileName, modeFlags, ppFile);  
}
  
HRESULT AAFDLL::OpenExistingModify (
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  TRACE("AAFDLL::OpenExistingModify");
  ASSERT("Valid dll callback function", _pfnOpenExistingModify);
  return _pfnOpenExistingModify(pFileName, modeFlags, pIdent, ppFile);  
}

HRESULT AAFDLL::OpenNewModify (
    const wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  TRACE("AAFDLL::OpenNewModify");
  ASSERT("Valid dll callback function", _pfnOpenNewModify);
  return _pfnOpenNewModify(pFileName, modeFlags, pIdent, ppFile);  
}

HRESULT AAFDLL::OpenTransient (
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  TRACE("AAFDLL::OpenTransient");
  ASSERT("Valid dll callback function", _pfnOpenTransient);
  return _pfnOpenTransient(pIdent, ppFile);  
}

HRESULT AAFDLL::IsAAFFile (
    aafCharacter_constptr  pFileName,
    aafUID_t *  pAAFFileKind,
    aafBool *  pFileIsAAFFile)
{
  TRACE("AAFDLL::IsAAFFile");
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
  TRACE("AAFDLL::GetPluginManager");
  ASSERT("Valid dll callback function", _pfnGetPluginManager);
  return _pfnGetPluginManager(ppPluginManager);  
}

HRESULT AAFDLL::CreateRawStorageMemory (
	aafFileAccess_t  access,
	IAAFRawStorage ** ppNewRawStorage)
{
  TRACE("AAFDLL::CreateRawStorageMemory");
//  ASSERT("Valid dll callback function", _pfnCreateRawStorageMemory);
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
  TRACE("AAFDLL::CreateRawStorageDisk");
//  ASSERT("Valid dll callback function", _pfnCreateRawStorageDisk);
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
  TRACE("AAFDLL::CreateRawStorageCachedDisk");
//  ASSERT("Valid dll callback function", _pfnCreateRawStorageCachedDisk);
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

HRESULT AAFDLL::CreateAAFFileOnRawStorage (
    IAAFRawStorage * pRawStorage,
	aafFileExistence_t  existence,
	aafFileAccess_t  access,
	aafUID_constptr  pFileKind,
	aafUInt32  modeFlags,
	aafProductIdentification_constptr  pIdent,
	IAAFFile ** ppNewFile)
{
  TRACE("AAFDLL::CreateAAFFileOnRawStorage");
//  ASSERT("Valid dll callback function", _pfnCreateAAFFileOnRawStorage);
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

/***********************************************************************
 *
 *              Copyright (c) 1998-2001 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/



// Define this to 1 to use raw storage implementations of the
// traditional AAFFileOpenXXX(filename) methods.
#ifndef USE_RAW_STORAGE
#define USE_RAW_STORAGE 0
#endif // ! USE_RAW_STORAGE


// Declare the public interface that must be implemented.
#include "aaflib.h"

#include "AAFResult.h"
#include "AAFFileKinds.h"
#include "AAFFileSignatures.h"

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

  // Check that the file is an AAF file
  aafUID_t fileKind;
  aafBool isAnAAFFile;
  hr = AAFFileIsAAFFile(pFileName, &fileKind, &isAnAAFFile);
  if (FAILED(hr))
    return hr;

  if (isAnAAFFile == kAAFFalse)
    return AAFRESULT_NOT_AAF_FILE;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
 
  try
  {
#if USE_RAW_STORAGE
	IAAFRawStorage * pRawStg = 0;
	hr = AAFCreateRawStorageDisk
	  (pFileName,
	   kAAFFileExistence_existing,
	   kAAFFileAccess_read,
	   &pRawStg);
	if (AAFRESULT_SUCCEEDED (hr))
	  {
		hr = AAFCreateAAFFileOnRawStorage
		  (pRawStg,
		   kAAFFileExistence_existing,
		   kAAFFileAccess_read,
		   &aafFileKindAafSSBinary,
		   modeFlags,
		   0,
		   ppFile);
		if (AAFRESULT_SUCCEEDED (hr))
		  {
			ASSERT ("Valid ppFile pointer", ppFile);
			ASSERT ("Valid *ppFile", *ppFile);
			hr = (*ppFile)->Open ();
		  }
	  }
	if (pRawStg)
	  {
		pRawStg->Release ();
	  }
#else // ! USE_RAW_STORAGE
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenExistingRead(pFileName, modeFlags, ppFile);
#endif // USE_RAW_STORAGE
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

  // Check that the file is an AAF file
  aafUID_t fileKind;
  aafBool isAnAAFFile;
  hr = AAFFileIsAAFFile(pFileName, &fileKind, &isAnAAFFile);
  if (FAILED(hr))
    return hr;

  if (isAnAAFFile == kAAFFalse)
    return AAFRESULT_NOT_AAF_FILE;

  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  try
  {
#if USE_RAW_STORAGE
	IAAFRawStorage * pRawStg = 0;
	hr = AAFCreateRawStorageDisk
	  (pFileName,
	   kAAFFileExistence_existing,
	   kAAFFileAccess_modify,
	   &pRawStg);
	if (AAFRESULT_SUCCEEDED (hr))
	  {
		hr = AAFCreateAAFFileOnRawStorage
		  (pRawStg,
		   kAAFFileExistence_existing,
		   kAAFFileAccess_modify,
		   &aafFileKindAafSSBinary,
		   modeFlags,
		   pIdent,
		   ppFile);
		if (AAFRESULT_SUCCEEDED (hr))
		  {
			ASSERT ("Valid ppFile pointer", ppFile);
			ASSERT ("Valid *ppFile", *ppFile);
			hr = (*ppFile)->Open ();
		  }
	  }
	if (pRawStg)
	  {
		pRawStg->Release ();
	  }
#else // ! USE_RAW_STORAGE
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenExistingModify(pFileName, modeFlags, pIdent, ppFile);
#endif // USE_RAW_STORAGE
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
#if USE_RAW_STORAGE
	IAAFRawStorage * pRawStg = 0;
	hr = AAFCreateRawStorageDisk
	  (pFileName,
	   kAAFFileExistence_new,
	   kAAFFileAccess_modify,
	   &pRawStg);
	if (AAFRESULT_SUCCEEDED (hr))
	  {
		hr = AAFCreateAAFFileOnRawStorage
		  (pRawStg,
		   kAAFFileExistence_new,
		   kAAFFileAccess_modify,
		   &aafFileKindAafSSBinary,
		   modeFlags,
		   pIdent,
		   ppFile);
		if (AAFRESULT_SUCCEEDED (hr))
		  {
			ASSERT ("Valid ppFile pointer", ppFile);
			ASSERT ("Valid *ppFile", *ppFile);
			hr = (*ppFile)->Open ();
		  }
	  }
	if (pRawStg)
	  {
		pRawStg->Release ();
	  }
#else // ! USE_RAW_STORAGE
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenNewModify(pFileName, modeFlags, pIdent, ppFile);
#endif // USE_RAW_STORAGE
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
#if USE_RAW_STORAGE
	IAAFRawStorage * pRawStg = 0;
	hr = AAFCreateRawStorageMemory
	  (kAAFFileAccess_modify,
	   &pRawStg);
	if (AAFRESULT_SUCCEEDED (hr))
	  {
		hr = AAFCreateAAFFileOnRawStorage
		  (pRawStg,
		   kAAFFileExistence_new,
		   kAAFFileAccess_modify,
		   &aafFileKindAafSSBinary,
		   0,
		   pIdent,
		   ppFile);
		if (AAFRESULT_SUCCEEDED (hr))
		  {
			ASSERT ("Valid ppFile pointer", ppFile);
			ASSERT ("Valid *ppFile", *ppFile);
			hr = (*ppFile)->Open ();
		  }
	  }
	if (pRawStg)
	  {
		pRawStg->Release ();
	  }
#else // ! USE_RAW_STORAGE
    // Attempt to call the dll's exported function...
    hr = pAAFDLL->OpenTransient(pIdent, ppFile);
#endif // USE_RAW_STORAGE
  }
  catch (...)
  {
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}

// Helpers for AAFFileIsAAFFile().
//

// Table mapping valid signatures to file kinds.
// Signatures are found at the beginning of the file and are variable in size.
// There are multiple signatures since there are multiple file kinds
// e.g. "AAF structured storage binary" and "AAF XML text".
//
struct mapSignatureToFileKind {
  const aafUInt8* signature;
  size_t signatureSize;
  const aafUID_t* fileKind;
} fileKindTable[] = {
  {aafFileSignatureAafSSBinary,
   sizeof(aafFileSignatureAafSSBinary),
   &aafFileKindAafSSBinary},
  {aafFileSignatureMxfSSBinary,
   sizeof(aafFileSignatureMxfSSBinary),
   &aafFileKindMxfSSBinary},
  {aafFileSignatureAafXmlText,
   sizeof(aafFileSignatureAafXmlText),
   &aafFileKindAafXmlText},
  {aafFileSignatureMxfXmlText,
   sizeof(aafFileSignatureMxfXmlText),
   &aafFileKindMxfXmlText}
};

static FILE* wfopen(const wchar_t* fileName, const wchar_t* mode);
static HRESULT readSignature(FILE* file,
                             unsigned char* signature,
                             size_t signatureSize);
static size_t signatureSize(void);
static aafBool isRecognizedSignature(unsigned char* signature,
                                     size_t signatureSize,
                                     aafUID_t* fileKind);

static const size_t maxSignatureSize = 256; //signatureSize();

// Just like fopen() except for wchar_t* file names.
//
FILE* wfopen(const wchar_t* fileName, const wchar_t* mode)
{
  TRACE("wfopen");
  ASSERT("Valid file name", fileName != 0);
  ASSERT("Valid mode", mode != 0);

  FILE* result = 0;
#if defined( OS_WINDOWS )
  result = _wfopen(fileName, mode);
#else
  char cFileName[FILENAME_MAX];
  size_t status = wcstombs(cFileName, fileName, FILENAME_MAX);
  ASSERT("Convert succeeded", status != (size_t)-1);

  char cMode[FILENAME_MAX];
  status = wcstombs(cMode, mode, FILENAME_MAX);
  ASSERT("Convert succeeded", status != (size_t)-1);

  result = fopen(cFileName, cMode);
#endif
  return result;
}

// Read the file signature. Assumes that no valid file may be shorter
// than the longest signature.
//
HRESULT readSignature(FILE* file,
                      unsigned char* signature,
                      size_t signatureSize)
{
  TRACE("readSignature");
  ASSERT("Valid file", file != 0);
  ASSERT("Valid signature buffer", signature != 0);
  ASSERT("Valid signature buffer size", signatureSize != 0 
                      && signatureSize <= maxSignatureSize);

  HRESULT hr = S_OK;
  unsigned char sig[maxSignatureSize];
  if (sig == 0) {
    hr = AAFRESULT_NOMEMORY;
  } else {
    size_t status = fread(sig, signatureSize, 1, file);
    if (status == 1) {
      memcpy(signature, sig, signatureSize);
    } else {
      hr = AAFRESULT_NOT_AAF_FILE;  // Can't read signature
    }
  }
  return hr;
}

// The number of bytes to read to be sure of getting the signature.
//
size_t signatureSize(void)
{
  size_t result = 0;
  for (size_t i = 0; i < sizeof(fileKindTable)/sizeof(fileKindTable[0]); i++) {
    if (fileKindTable[i].signatureSize > result) {
      result = fileKindTable[i].signatureSize;
    }
  }
  return result;
}

// Try to recognize a file signature. Assumes that no signature is a
// prefix of any other signature.
//
aafBool isRecognizedSignature(unsigned char* signature,
                              size_t signatureSize,
                              aafUID_t* fileKind)
{
  TRACE("isRecognizedSignature");
  ASSERT("Valid signature buffer", signature != 0);
  ASSERT("Valid signature buffer size", signatureSize != 0);
  ASSERT("Valid file kind", fileKind != 0);

  aafBool result = kAAFFalse;

  for (size_t i = 0; i < sizeof(fileKindTable)/sizeof(fileKindTable[0]); i++) {
    if (fileKindTable[i].signatureSize <= signatureSize) {
      if (memcmp(fileKindTable[i].signature,
                 signature,
                 fileKindTable[i].signatureSize) == 0) {
        result = kAAFTrue;
        memcpy(fileKind, fileKindTable[i].fileKind, sizeof(CLSID));
        break;
      }
    }
  }
  return result;
}

//***********************************************************
//
// AAFFileIsAAFFile()
//
// This function is implemented here so that it can be called without
// having to load the DLL.
//
STDAPI AAFFileIsAAFFile (
    aafCharacter_constptr  pFileName,
    aafUID_t *  pAAFFileKind,
    aafBool *  pFileIsAAFFile)
{
  TRACE("AAFFileIsAAFFile");
  if (pFileName == 0)
    return AAFRESULT_NULL_PARAM;

  if (pAAFFileKind == 0)
    return AAFRESULT_NULL_PARAM;

  if (pFileIsAAFFile == 0)
    return AAFRESULT_NULL_PARAM;

  ASSERT("Valid signature buffer size", signatureSize() <= maxSignatureSize);


  HRESULT hr = S_OK;
  unsigned char signature[maxSignatureSize];
  if (signature == 0) {
    hr = AAFRESULT_NOMEMORY;
  } else {
    FILE* f = wfopen(pFileName, L"rb");
    if (f != 0) {
      hr = readSignature(f, signature, signatureSize());
      if (SUCCEEDED(hr)) {
        *pFileIsAAFFile = isRecognizedSignature(signature,
                                                signatureSize(),
                                                pAAFFileKind);
      } else {
        // The file exists but we can't read the signature
        *pFileIsAAFFile = kAAFFalse;
        hr = S_OK;
      }
      fclose(f);
    } else {
      hr = AAFRESULT_FILE_NOT_FOUND; // Can't open file
    }
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
  _pfnGetPluginManager = NULL;
  _pfnCreateRawStorageMemory = 0;
  _pfnCreateRawStorageDisk = 0;
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

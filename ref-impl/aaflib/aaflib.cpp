/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/



// Declare the public interface that must be implemented.
#include "aaflib.h"

#include "AAFResult.h"

//
// Define the platform specific default dll name.
//
#if defined(WIN32) || defined(_WIN32)
#define DEFAULT_AAFDLL_NAME "AAFCOAPI.dll"
#elif defined(macintosh) || defined(_MAC)
#define DEFAULT_AAFDLL_NAME "AAFCOAPI.DLL (PPC)"
#else
#define DEFAULT_AAFDLL_NAME "aafcoapi.so"
#endif




// ASSERT code copied from OM...

#ifdef _DEBUG

#include <iostream.h>
#include <stdlib.h>

#define FAILURE -1

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
  abort();
}


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
  wchar_t *  pFileName,
  aafUInt32  modeFlags,
  IAAFFile ** ppFile)
{
  HRESULT hr = S_OK;
  AAFDLL *pAAFDLL = NULL;


  // Get the dll wrapper
  hr = LoadIfNecessary(&pAAFDLL);
  if (FAILED(hr))
    return hr;
  
  // Attempt to call the dll's exported function...
  hr = pAAFDLL->OpenExistingRead(pFileName, modeFlags, ppFile);

  return hr;
}


//***********************************************************
//
// AAFFileOpenExistingModify()
// 
STDAPI AAFFileOpenExistingModify (
  wchar_t *  pFileName,
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
  
  // Attempt to call the dll's exported function...
  hr = pAAFDLL->OpenExistingModify(pFileName, modeFlags, pIdent, ppFile);

  return hr;
}



//***********************************************************
//
// AAFFileOpenNewModify()
//
// 
STDAPI AAFFileOpenNewModify (
  wchar_t *  pFileName,
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
  
  // Attempt to call the dll's exported function...
  hr = pAAFDLL->OpenNewModify(pFileName, modeFlags, pIdent, ppFile);

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
  
  // Attempt to call the dll's exported function...
  hr = pAAFDLL->OpenTransient(pIdent, ppFile);

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
  
  // Attempt to call the dll's exported function...
  hr = pAAFDLL->GetPluginManager(ppPluginManager);

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

  return rc;
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
}



//***********************************************************
//
// Wrapper functions for calling member entry points.
//

HRESULT AAFDLL::OpenExistingRead (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile)
{
  TRACE("AAFDLL::OpenExistingRead");
  ASSERT("Valid dll callback function", _pfnOpenExistingRead);
  
  return _pfnOpenExistingRead(pFileName, modeFlags, ppFile);  
}
  
HRESULT AAFDLL::OpenExistingModify (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  TRACE("AAFDLL::OpenExistingModify");
  ASSERT("Valid dll callback function", _pfnOpenExistingModify);
  return _pfnOpenExistingModify(pFileName, modeFlags, pIdent, ppFile);  
}

HRESULT AAFDLL::OpenNewModify (
    wchar_t *  pFileName,
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

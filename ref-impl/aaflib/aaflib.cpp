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

// Declare all of the public functions.
#include "AAF.h"
#include "AAFResult.h"
#include <assert.h>


// 
#ifdef __cplusplus
extern "C"{
#endif 

STDAPI AAFLoad(wchar_t * dllname);
STDAPI AAFUnload();

#ifdef __cplusplus
}
#endif




//***********************************************************
// Define function prototypes in a manner consistent with the 
// ActiveX and OLE SDK's.

#if !defined(_MSC_VER)

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENEXISTINGREAD)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENEXISTINGMODIFY)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENNEWMODIFY)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFFILEOPENTRANSIENT)(
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef STDAPICALLTYPE HRESULT (* LPFNAAFGETPLUGINMANAGER)(
    IAAFPluginManager ** ppPluginManager);

#else

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENEXISTINGREAD)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENEXISTINGMODIFY)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENNEWMODIFY)(
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFFILEOPENTRANSIENT)(
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

typedef HRESULT (STDAPICALLTYPE * LPFNAAFGETPLUGINMANAGER)(
    IAAFPluginManager ** ppPluginManager);

#endif

//***********************************************************
// Abstract helper class to hide the platform dependent
// details for handling dll entrypoints.
//
class AAFDLL;

class AAFDLL
{
public:
  // Constructor and destructor for base class.
  AAFDLL();
  virtual ~AAFDLL();

  // Return the singleton instance.
  static AAFDLL * GetAAFDLL();


  // Abstract method that must be overridden to implement
  // platform specific initialization of dll and entry points.
  virtual HRESULT Load(wchar_t *dllname) = 0;
  
  // Abstract method that must be overridden to implement
  // platform specific cleanup of dll and entry points.
  virtual HRESULT Unload() = 0;


  // Wrapper functions for calling member entry points.
  //
  HRESULT OpenExistingRead (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile);
  
  HRESULT OpenExistingModify (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT OpenNewModify (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT OpenTransient (
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile);

  HRESULT GetPluginManager (
    IAAFPluginManager ** ppPluginManager);
  
  // Make the global factory a friend.
  friend AAFDLL * MakeAAFDLL();

protected:
  // The single instance of the dll wrapper.
  static AAFDLL *_singleton;

  // Callback function member data loaded by overridden versions
  // of the Load() method:
  LPFNAAFFILEOPENEXISTINGREAD _pfnOpenExistingRead;
  LPFNAAFFILEOPENEXISTINGMODIFY _pfnOpenExistingModify;
  LPFNAAFFILEOPENNEWMODIFY _pfnOpenNewModify;
  LPFNAAFFILEOPENTRANSIENT _pfnOpenTransient;
  LPFNAAFGETPLUGINMANAGER _pfnGetPluginManager;
};

//
// Initialize static data.
//
AAFDLL * AAFDLL::_singleton = NULL;


//
// Forward declaration for factory function just returns an instance 
// of the currect platform dll wrapper object.
//
AAFDLL * MakeAAFDLL();



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
STDAPI AAFLoad(wchar_t * dllname)
{
  AAFDLL *pAAFDLL = AAFDLL::GetAAFDLL();
  if (NULL != pAAFDLL)
    return AAFRESULT_ALREADY_INITIALIZED;

  // Create the appropriate dll wrapper
  pAAFDLL = MakeAAFDLL();
  if (NULL == pAAFDLL)
    return AAFRESULT_NOMEMORY;

  if (NULL == dllname)
  { // use a realistic default name.
    dllname = L"AAFCOAPI.dll";
  }

  // Attempt to load the dll and initialize the entry points.
  // If the load fails the cleanup immediately.
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
      assert(pAAFDLL);
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



// Constructor for the base class
AAFDLL::AAFDLL()
{
  // There Can Be Only One!
  assert(NULL == _singleton);
  _pfnOpenExistingRead = NULL;
  _pfnOpenExistingModify = NULL;
  _pfnOpenNewModify = NULL;
  _pfnOpenTransient = NULL;
  _pfnGetPluginManager = NULL;
}

// Destructor for the base class
AAFDLL::~AAFDLL()
{
  // There Can Be Only One!
  assert(this == _singleton);

  _singleton = NULL;
}

AAFDLL * AAFDLL::GetAAFDLL()
{
  return _singleton;
}

HRESULT AAFDLL::OpenExistingRead (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    IAAFFile ** ppFile)
{
  assert(_pfnOpenExistingRead);
  return _pfnOpenExistingRead(pFileName, modeFlags, ppFile);  
}
  
HRESULT AAFDLL::OpenExistingModify (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  assert(_pfnOpenExistingModify);
  return _pfnOpenExistingModify(pFileName, modeFlags, pIdent, ppFile);  
}

HRESULT AAFDLL::OpenNewModify (
    wchar_t *  pFileName,
    aafUInt32  modeFlags,
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  assert(_pfnOpenNewModify);
  return _pfnOpenNewModify(pFileName, modeFlags, pIdent, ppFile);  
}

HRESULT AAFDLL::OpenTransient (
    aafProductIdentification_t *  pIdent,
    IAAFFile ** ppFile)
{
  assert(_pfnOpenTransient);
  return _pfnOpenTransient(pIdent, ppFile);  
}

HRESULT AAFDLL::GetPluginManager (
  IAAFPluginManager ** ppPluginManager)
{
  assert(_pfnGetPluginManager);
  return _pfnGetPluginManager(ppPluginManager);  
}
 





#if defined(macintosh) || defined(_MAC)
class MacAAFDLL : public AAFDLL
{
public:
  // Constructor and destructor.
  MacAAFDLL();
  ~MacAAFDLL();

  // Implements Mac specific initialization of dll and entry points.
  virtual HRESULT Load(wchar_t *);
  
  // Implements Mac specific cleanup of dll and entry points.
  virtual HRESULT Unload();

private:
  // The handle to the dll's module.
  long _libref;
};

// Factory function just returns an instance of the currect platform
// dll wrapper object.
AAFDLL * MakeAAFDLL()
{
  AAFDLL *pAAFDLL =  new MacAAFDLL;
  AAFDLL::_singleton = pAAFDLL;
  return pAAFDLL;
}


HRESULT MacAAFDLL::Load(wchar_t *)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT MacAAFDLL::Unload()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}
 

#elif defined(WIN32) || defined(_WIN32)

class Win32AAFDLL : public AAFDLL
{
public:
  // Constructor and destructor.
  Win32AAFDLL();
  ~Win32AAFDLL();

  // Implements Win32 specific initialization of dll and entry points.
  virtual HRESULT Load(wchar_t *);
  
  // Implements Win32 specific cleanup of dll and entry points.
  virtual HRESULT Unload();

private:
  // The handle to the dll's module instance.
  HINSTANCE _hInstance;
};


// Factory function just returns an instance of the currect platform
// dll wrapper object.
AAFDLL * MakeAAFDLL()
{
  assert(NULL == AAFDLL::_singleton);
  AAFDLL *pAAFDLL =  new Win32AAFDLL;
  AAFDLL::_singleton = pAAFDLL;
  return pAAFDLL;
}


Win32AAFDLL::Win32AAFDLL()
{
  _hInstance = NULL;
}

Win32AAFDLL::~Win32AAFDLL()
{
}


HRESULT Win32AAFDLL::Load(wchar_t *dllname)
{
  // Attempt to load the library.
  _hInstance = ::LoadLibraryW(dllname);

  if (NULL == _hInstance)
  {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  //
  // Attempt to initialize the entry points...
  //
  _pfnOpenExistingRead = (LPFNAAFFILEOPENEXISTINGREAD)
                         GetProcAddress(_hInstance, "AAFFileOpenExistingRead");
  if (NULL == _pfnOpenExistingRead)
    return HRESULT_FROM_WIN32(GetLastError());

  _pfnOpenExistingModify = (LPFNAAFFILEOPENEXISTINGMODIFY)
                           GetProcAddress(_hInstance, "AAFFileOpenExistingModify");
  if (NULL == _pfnOpenExistingModify)
    return HRESULT_FROM_WIN32(GetLastError());

  _pfnOpenNewModify = (LPFNAAFFILEOPENNEWMODIFY)
                      GetProcAddress(_hInstance, "AAFFileOpenNewModify");
  if (NULL == _pfnOpenNewModify)
    return HRESULT_FROM_WIN32(GetLastError());

  _pfnOpenTransient = (LPFNAAFFILEOPENTRANSIENT)
                         GetProcAddress(_hInstance, "AAFFileOpenTransient");
  if (NULL == _pfnOpenTransient)
    return HRESULT_FROM_WIN32(GetLastError());

  _pfnGetPluginManager = (LPFNAAFGETPLUGINMANAGER)
                         GetProcAddress(_hInstance, "AAFGetPluginManager");
  if (NULL == _pfnGetPluginManager)
    return HRESULT_FROM_WIN32(GetLastError());

  return S_OK;
}


HRESULT Win32AAFDLL::Unload()
{
  if (_hInstance)
  {
    _pfnOpenExistingRead = NULL;
    _pfnOpenExistingModify = NULL;
    _pfnOpenNewModify = NULL;
    _pfnOpenTransient = NULL;

    if (!::FreeLibrary(_hInstance))
      return HRESULT_FROM_WIN32(GetLastError());

    _hInstance = NULL;
  }
  
  return S_OK;
}

#else

class UnknownAAFDLL : public AAFDLL
{
public:
  // Implements Unknown platform specific initialization of dll and entry points.
  virtual HRESULT Load(wchar_t *aafdll);
  
  // Implements Unknown platform specific cleanup of dll and entry points.
  virtual HRESULT Unload();

private:
  // The handle to the dll's module instance.
};

// Factory function just returns an instance of the currect platform
// dll wrapper object.
AAFDLL * MakeAAFDLL()
{
  assert(NULL == AAFDLL::_singleton);
  AAFDLL *pAAFDLL =  new UnknownAAFDLL;
  AAFDLL::_singleton = pAAFDLL;
  return pAAFDLL;
}


HRESULT UnknownAAFDLL::Load(wchar_t *)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT UnknownAAFDLL::Unload()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}
 
#endif

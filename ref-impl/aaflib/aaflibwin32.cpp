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


//
// Use include guard so that the file can be included in every 
// platform build without causing any errors in the build.
//
#if defined(WIN32) || defined(_WIN32)


// Declare the public interface that must be implemented.
#include "aaflib.h"

#include "AAFResult.h"



class Win32AAFDLL : public AAFDLL
{
public:
  // Constructor and destructor.
  Win32AAFDLL();
  ~Win32AAFDLL();

  // Implements Win32 specific initialization of dll and entry points.
  virtual HRESULT Load(const char *);
  
  // Implements Win32 specific cleanup of dll and entry points.
  virtual HRESULT Unload();

private:
  // The handle to the dll's module instance.
  HINSTANCE _hInstance;
};



//***********************************************************
//
// Factory function just returns an instance of the currect platform
// dll wrapper object.
AAFDLL * MakeAAFDLL()
{
  return new Win32AAFDLL;
}



//***********************************************************
//
Win32AAFDLL::Win32AAFDLL()
{
  _hInstance = NULL;
}



//***********************************************************
//
Win32AAFDLL::~Win32AAFDLL()
{
}



//***********************************************************
//
HRESULT Win32AAFDLL::Load(const char *dllname)
{
  if (NULL == dllname)
  { // use a realistic default name.
    dllname = "AAFCOAPI.dll";
  }

  // Attempt to load the library.
  _hInstance = ::LoadLibraryA(dllname);

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



//***********************************************************
//
HRESULT Win32AAFDLL::Unload()
{
  if (_hInstance)
  {
    if (!::FreeLibrary(_hInstance))
      return HRESULT_FROM_WIN32(GetLastError());

    // Reset the entry point function pointers to NULL.
    ClearEntrypoints();

    _hInstance = NULL;
  }
  
  return S_OK;
}

#endif /* #if defined(WIN32) || defined(_WIN32) */

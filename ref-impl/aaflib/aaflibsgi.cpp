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

// WARNING! WARNING! WARNING!
// This is untested code. It has not even been compiled!
// [trr:1999-04-20]
// At this time the irix port has not been maintained for several months.
// Even then it was using the MainWin environment.
//
// This code is provided as a template to the irix port engineer.
//


//
// Use include guard so that the file can be included in every 
// platform build without causing any errors in the build.
//
#if defined(sgi)


// Declare the public interface that must be implemented.
#include "aaflib.h"

#include "AAFResult.h"


#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>


class SGIAAFDLL : public AAFDLL
{
public:
  // Constructor and destructor.
  SGIAAFDLL();
  ~SGIAAFDLL();

  // Implements SGI specific initialization of dll and entry points.
  virtual HRESULT Load(const char *);
  
  // Implements SGI specific cleanup of dll and entry points.
  virtual HRESULT Unload();

private:
  // The handle to the dll's module instance.
  HINSTANCE _handle;
};



//***********************************************************
//
// Factory function just returns an instance of the currect platform
// dll wrapper object.
AAFDLL * MakeAAFDLL()
{
  return new SGIAAFDLL;
}



//***********************************************************
//
SGIAAFDLL::SGIAAFDLL()
{
  _handle = NULL;
}



//***********************************************************
//
SGIAAFDLL::~SGIAAFDLL()
{
}



//***********************************************************
//
HRESULT SGIAAFDLL::Load(const char *dllname)
{
  const char* fcnName[] =
  {
    "AAFFileOpenExistingRead",
    "AAFFileOpenExistingModify",
    "AAFFileOpenNewModify",
    "AAFFileOpenTransient",
    "AAFGetPluginManager"
  };

  if (NULL == dllname)
  { // use a realistic default name.
    dllname = "aafcoapi.so";
  }

  // Attempt to load the library.
  _handle = ::dlopen(dllname, RTLD_LAZY);
  if (NULL == _handle)
  {
    fprintf(stderr, "dlopen(\"%s\", RTLD_LAZY) failed.\n", dllname);
    return -1; // Need an AAFRESULT
  }

  //
  // Attempt to initialize the entry points...
  //
  _pfnOpenExistingRead = (LPFNAAFFILEOPENEXISTINGREAD)
                         ::dlsym(_handle, fcnName[0]);
  if (NULL == _pfnOpenExistingRead)
  {
    fprintf(stderr, "dlsym(\"%s\") failed. %s\n", fcnName[0]);
    return -2; // Need an AAFRESULT
  }

  _pfnOpenExistingModify = (LPFNAAFFILEOPENEXISTINGMODIFY)
                           ::dlsym(_handle, fcnName[1]);
  if (NULL == _pfnOpenExistingModify)
  {
    fprintf(stderr, "dlsym(\"%s\") failed.\n", fcnName[1]);
    return -2; // Need an AAFRESULT
  }

  _pfnOpenNewModify = (LPFNAAFFILEOPENNEWMODIFY)
                      ::dlsym(_handle, fcnName[2]);
  if (NULL == _pfnOpenNewModify)
  {
    dlmsg = dlerror();
    fprintf(stderr, "dlsym(\"%s\") failed.\n", fcnName[2]);
    return -2; // Need an AAFRESULT
  }

  _pfnOpenTransient = (LPFNAAFFILEOPENTRANSIENT)
                         ::dlsym(_handle, fcnName[3]);
  if (NULL == _pfnOpenTransient)
  {
    fprintf(stderr, "dlsym(\"%s\") failed.\n", fcnName[3]);
    return -2; // Need an AAFRESULT
  }

  _pfnGetPluginManager = (LPFNAAFGETPLUGINMANAGER)
                         ::dlsym(_handle, fcnName[4]);
  if (NULL == _pfnGetPluginManager)
  {
    fprintf(stderr, "dlsym(\"%s\") failed.\n", fcnName[4]);
    return -2; // Need an AAFRESULT
  }

  return S_OK;
}



//***********************************************************
//
HRESULT SGIAAFDLL::Unload()
{
  if (_handle)
  {
    ::dlclose(_handle))

    // Reset the entry point function pointers to NULL.
    ClearEntrypoints();

    _handle = NULL;
  }
  
  return S_OK;
}

#endif /* #if defined(sgi) */

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
#if defined(macintosh) || defined(_MAC)


// Declare the public interface that must be implemented.
#include "aaflib.h"

#include "AAFResult.h"

#include <Folders.h>
#include <Resources.h>
#include <CodeFragments.h>
#include <Errors.h>

#include <stdio.h>
#include <stdlib.h>


class MacAAFDLL : public AAFDLL
{
public:
  // Constructor and destructor.
  MacAAFDLL();
  ~MacAAFDLL();

  // Implements Mac specific initialization of dll and entry points.
  virtual HRESULT Load(const char *dllname);
  
  // Implements Mac specific cleanup of dll and entry points.
  virtual HRESULT Unload();

private:
  // The handle to dll's module.
	CFragConnectionID _connectionID;
};



//***********************************************************
//
// Factory function just returns an instance of the currect platform
// dll wrapper object.
AAFDLL * MakeAAFDLL()
{
  return new MacAAFDLL;
}



//***********************************************************
//
MacAAFDLL::MacAAFDLL()
{
  _connectionID = 0;
}



//***********************************************************
//
MacAAFDLL::~MacAAFDLL()
{
}



//***********************************************************
//
HRESULT MacAAFDLL::Load(const char *dllname)
{
  TRACE("MacAAFDLL::Load");
  
  if (NULL == dllname)
  { // use a realistic default name.
    dllname = "AAFCOAPI.DLL (PPC)";
  }
  
  
  ASSERT("Valid dllname", NULL != dllname && strlen(dllname) <= 63);
  
  HRESULT hr = CO_E_DLLNOTFOUND; // the default error code
  OSErr             err = noErr;
  CFragArchitecture archType = kCompiledCFragArch;
  CFragLoadOptions  loadFlags = kReferenceCFrag;  /* Try to load an existing one and update the reference count */
  Ptr               mainAddr = NULL;
  char              errMessage[256];
  StringPtr         psymName = NULL;
  Ptr               symAddr = NULL;
  CFragSymbolClass  symClass;
  Str63             libName;

  
  // Copy and convert the c null terminated string to a Str63.
  strncpy((char *)libName, dllname, sizeof(Str63));
  libName[sizeof(Str63) - 1] = 0; // force null termination
  c2pstr((char *)libName);
  
  // Attempt to load the library.
  err = GetSharedLibrary(libName, archType, loadFlags, &_connectionID, &mainAddr, (unsigned char *)errMessage);
  if (noErr != err)
  {
  	hr = err;
#ifdef _DEBUG
    p2cstr((unsigned char *)errMessage);
    fprintf(stderr, "\nGetSharedLibrary(\"%s\"...); \n\tfailed with an error code = %d and errMessage:\"%s\"\n", dllname, hr, errMessage);
    fflush(stderr);
#endif
  	return err;
  }



  //
  // Attempt to initialize the entry points...
  //
  err = FindSymbol(_connectionID, "\pAAFFileOpenExistingRead", (Ptr *)&_pfnOpenExistingRead, &symClass);
  if (noErr != err)
    return err;

  err = FindSymbol(_connectionID, "\pAAFFileOpenExistingModify", (Ptr *)&_pfnOpenExistingModify, &symClass);
  if (noErr != err)
    return err;

  err = FindSymbol(_connectionID, "\pAAFFileOpenNewModify", (Ptr *)&_pfnOpenNewModify, &symClass);
  if (noErr != err)
    return err;

  err = FindSymbol(_connectionID, "\pAAFFileOpenTransient", (Ptr *)&_pfnOpenTransient, &symClass);
  if (noErr != err)
    return err;

  err = FindSymbol(_connectionID, "\pAAFGetPluginManager", (Ptr *)&_pfnGetPluginManager, &symClass);
  if (noErr != err)
    return err;
  

  return S_OK;
}



//***********************************************************
//
HRESULT MacAAFDLL::Unload()
{
  TRACE("MacAAFDLL::Unload");
  
  HRESULT hr = S_OK;
  OSErr err = noErr;
  
  if (0 != _connectionID)
  {
    err = CloseConnection(&_connectionID);
  	ASSERT("CloseConnection succeeded", noErr == err);
    
    // Reset the entry point function pointers to NULL.
    ClearEntrypoints();
  
    // For now just return the Mac error code. This value will
    // always be < 0 so the FAILED and SUCCEEDED macros from COM
    // should still work.
    hr = err;
  }
  
  return hr;
}


#endif /* #if defined(macintosh) || defined(_MAC) */
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
#include "aafrdli.h"


#include "AAFResult.h"

#include <Folders.h>
#include <Resources.h>
#include <CodeFragments.h>
#include <Errors.h>

#include <stdio.h>
#include <stdlib.h>





AAFRDLIRESULT AAFLoadLibrary(const char* name, AAFLibraryHandle* pLibHandle)
{
  AAFRDLIRESULT result = AAFRESULT_SUCCESS;
  HRESULT hr = CO_E_DLLNOTFOUND; // the default error code
  OSErr             err = noErr;
  CFragArchitecture archType = kCompiledCFragArch;
  CFragLoadOptions  loadFlags = kReferenceCFrag;  /* Try to load an existing one and update the reference count */
  Ptr               mainAddr = NULL;
  char              errMessage[256];
  StringPtr         psymName = NULL;
  Ptr               symAddr = NULL;
  Str63             libName;
  CFragConnectionID connectionID = 0;

 
  if (NULL == name || NULL == pLibHandle || (NULL != name && 63 < strlen(name)))
    return AAFRESULT_NULL_PARAM;
  
  
  // Copy and convert the c null terminated string to a Str63.
  strncpy((char *)libName, name, sizeof(Str63));
  libName[sizeof(Str63) - 1] = 0; // force null termination
  c2pstr((char *)libName);
  
  // Attempt to load the library.
  err = GetSharedLibrary(libName, archType, loadFlags, &connectionID, &mainAddr, (unsigned char *)errMessage);
  if (noErr != err)
  {
  	result = err;
#ifdef _DEBUG
    p2cstr((unsigned char *)errMessage);
    fprintf(stderr, "\nGetSharedLibrary(\"%s\"...); \n\tfailed with an error code = %d and errMessage:\"%s\"\n", name, hr, errMessage);
    fflush(stderr);
#endif
  }
  else
  { // Library successfully loaded:
  	*pLibHandle = (AAFLibraryHandle *)connectionID;
  }
 
  return result;
}

AAFRDLIRESULT AAFUnloadLibrary(AAFLibraryHandle libHandle)
{
  AAFRDLIRESULT result = AAFRESULT_SUCCESS;
  CFragConnectionID connectionID = (CFragConnectionID)libHandle;
  
  if (0 == connectionID)
    return AAFRESULT_NULL_PARAM;
    
  result = CloseConnection(&connectionID);  

  return result;
}

AAFRDLIRESULT AAFFindSymbol(AAFLibraryHandle libHandle, const char* symbolName, AAFSymbolAddr* pSymbol)
{
  AAFRDLIRESULT result = AAFRESULT_SUCCESS;
  Str63 PSymbolName;
  Ptr theSymbol = NULL;
  CFragSymbolClass  symClass;

  
  if (NULL == libHandle || NULL == symbolName || NULL == pSymbol || (NULL != symbolName && 63 < strlen(symbolName)) )
    return AAFRESULT_NULL_PARAM;

  // Copy and convert the c null terminated string to a Str63.
  strncpy((char *)PSymbolName, symbolName, sizeof(Str63));
  PSymbolName[sizeof(Str63) - 1] = 0; // force null termination
  c2pstr((char *)PSymbolName);

  result = FindSymbol((CFragConnectionID)libHandle, PSymbolName, &theSymbol, &symClass);
  if (AAFRESULT_SUCCEEDED(result))
    *pSymbol = (AAFSymbolAddr)theSymbol;

  
  return result;
}




/* Limited set of platform independent directory searching functions.*/
AAFRDLIRESULT AAFFindLibrary(const char* name, LPFNAAFTESTFILEPROC testProc, void *userData)
{
	// Default implementation will just continue to use a hard-coded list of shared libaries.

	const char *pluginFileNames[] = 
	{
		"AAFPGAPI.DLL (PPC)",
		"AAFINTP.DLL (PPC)",
		NULL
	};
	AAFRDLIRESULT rc = AAFRESULT_SUCCESS;

	if (NULL == name || NULL == testProc)
		return AAFRESULT_NULL_PARAM;

	for (int i = 0; AAFRESULT_SUCCESS == rc && pluginFileNames[i]; ++i)
		rc = testProc(pluginFileNames[i], false /* not a directory */, userData);

	return rc;
}

#endif /* #if defined(macintosh) || defined(_MAC) */

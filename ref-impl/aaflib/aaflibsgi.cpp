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

//
// Use include guard so that the file can be included in every 
// platform build without causing any errors in the build.
//
#if defined( OS_UNIX )

// Declare the public interface that must be implemented.

#include "aafrdli.h"
#include "AAFResult.h"

#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

AAFRDLIRESULT AAFLoadLibrary(
   const char* name,
   AAFLibraryHandle* pLibHandle)
{
   if (NULL == name || NULL == pLibHandle) {
      return AAFRESULT_NULL_PARAM;
   }
  
   // Attempt to load the library.
   *pLibHandle = ::dlopen(name, RTLD_NOW );
    
   if (NULL == *pLibHandle) {
#if defined(_DEBUG)
      fprintf(stderr, "dlopen() failed for %s: <%s>\n", name, dlerror() );
#endif
		return -1; // Need an AAFRESULT
   }
   else {
   }	
   
   return AAFRESULT_SUCCESS;
}

AAFRDLIRESULT AAFUnloadLibrary(
   AAFLibraryHandle libHandle)
{
   if (NULL == libHandle) {
      return AAFRESULT_NULL_PARAM;
   }
    
   // FIXME - Calls ~CAAFInProcServer() when unloading libaffpgapi.so.
	// ND'A, April 11, 2000.  
	//   ::dlclose(libHandle);
   return AAFRESULT_SUCCESS;
}

AAFRDLIRESULT AAFFindSymbol(
   AAFLibraryHandle libHandle,
   const char* symbolName,
   AAFSymbolAddr* pSymbol)
{
   if (NULL == libHandle || NULL == symbolName || NULL == pSymbol)
      return AAFRESULT_NULL_PARAM;
   
   *pSymbol = ::dlsym(libHandle, symbolName );
   
   if (NULL == *pSymbol) {
#if defined(_DEBUG)
      fprintf(stderr, "dlsym(\"%s\") failed <%s>.\n", symbolName, dlerror());
#endif
      return -2; // Need an AAFRESULT
   }
 
   return AAFRESULT_SUCCESS;
}

/* Limited set of platform independent directory searching functions.*/
AAFRDLIRESULT AAFFindLibrary(
   const char* name,
   LPFNAAFTESTFILEPROC testProc,
   void *userData)
{
   // Default implementation will just continue to use a hard-coded list
	// of shared libaries.
   
   const char *pluginFileNames[] =  {
		"libaafpgapi.so",
		"libaafintp.so",
      0
   };
   
   AAFRDLIRESULT rc = AAFRESULT_SUCCESS;
   

	if (NULL == name || NULL == testProc) {
		return AAFRESULT_NULL_PARAM;
	}
		
	for (int i = 0; AAFRESULT_SUCCESS == rc && pluginFileNames[i]; ++i) {
		rc = testProc("",pluginFileNames[i], false , userData);
	}

   return rc;
}

#endif /* #if defined( OS_UNIX ) */


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>

static bool AAFIsValidFile(const char *name)
{
#if defined( OS_DARWIN )
   static const char pluginExt[] = "dylib";
#else
   static const char pluginExt[] = "so";
#endif

   if (name == NULL)
      return false;

   char *pExt = strrchr(name, '.');

   return (pExt && strcmp(++pExt, pluginExt) == 0);
}

// This operates on a new directory component and checks
// if it should be descended into during a library search (see AAFFindLibrary)
// Assumes 'dir' is non-NULL
static bool AAFIsValidDirectory(const char *dir)
{
   return (strcmp(dir, "..") != 0 && strcmp(dir, ".") != 0);
}



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
		return AAFRESULT_DLL_LOAD_FAILED;
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
#if defined(DLSYM_DEBUG)
     // This is expected if we loaded an older library and are looking for a newer symbol
      fprintf(stderr, "dlsym(\"%s\") failed <%s>.\n", symbolName, dlerror());
#endif
      return AAFRESULT_DLL_SYMBOL_NOT_FOUND;
   }
 
   return AAFRESULT_SUCCESS;
}

/* Limited set of platform independent directory searching functions.*/
AAFRDLIRESULT AAFFindLibrary(
   const char* name,
   LPFNAAFTESTFILEPROC testProc,
   void *userData)
{
   
   AAFRDLIRESULT rc = AAFRESULT_SUCCESS;
   DIR *dir = NULL;
   struct dirent *entry;
   struct stat statbuf;

   // Our buffer for building full paths.
   char findPath[PATH_MAX];

   

   if (NULL == name || NULL == testProc)
      return AAFRESULT_NULL_PARAM;

   const size_t nameLen = strlen(name);

   if ((dir = opendir(name)) == NULL)
      rc = AAFRESULT_FILE_NOT_FOUND;

   while (rc == AAFRESULT_SUCCESS && (entry = readdir(dir)) != NULL)
   {
      // We could assume readdir() would give us paths under PATH_MAX
      // in length, but let's err on the side of safety.
      if (strlen(entry->d_name) + nameLen > (PATH_MAX - 2))
      {
         rc = AAFRESULT_INVALID_PARAM;
         break;
      }

      // Copies and appends PATH_MAX - 2 at most (see above check),
      // which guarantees space for '/' and a null
      strcpy(findPath, name);
      strcat(findPath, "/");
      strcat(findPath, entry->d_name);

      if (lstat(findPath, &statbuf) != 0)
      {
         switch (errno)
         {
            case EACCES:
               // No permission - just skip this one.
               continue;
            case EBADF:
            case EFAULT:
               rc = AAFRESULT_BAD_FHDL;
               break;
            case ENOMEM:
               rc = AAFRESULT_NOMEMORY;
               break;
            case ENOENT:
            case ENOTDIR:
            case ELOOP:
            case ENAMETOOLONG:
            default:
               rc = AAFRESULT_FILE_NOT_FOUND;
               break;
         }
         // Fatal error - cut out
         break;
      }

      if (S_ISDIR(statbuf.st_mode))
      {
         if (AAFIsValidDirectory(entry->d_name))
            rc = testProc(findPath, entry->d_name, true, userData);
      }
      else if (S_ISREG(statbuf.st_mode)) // a file
      {
         if (AAFIsValidFile(findPath))
            rc = testProc(findPath, entry->d_name, false, userData);
      }
   }

   closedir(dir);

   return rc;
}

#endif /* #if defined( OS_UNIX ) */


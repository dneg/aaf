//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __AAFRDLI_H__
#define __AAFRDLI_H__

#if defined(macintosh) || defined(_MAC)
#include <Files.h>
#endif

/* 
 * The AAF Runtime Dynamic (Library) Loader Interface 
 */

/*
 * TODO: document interface
 */

/* Opaque type for platform dependent data. */
typedef void * AAFLibraryHandle;
typedef void * AAFSymbolAddr;
typedef void * AAFUserData;
typedef long AAFRDLIRESULT;




// Callback function signature for AAFFindLibrary.

typedef AAFRDLIRESULT (* LPFNAAFTESTFILEPROC)(
	const char* path,
    const char* name,
		char isDirectory,
		void * userData);



#ifdef __cplusplus
extern "C" {
#endif


AAFRDLIRESULT AAFLoadLibrary(const char* name, AAFLibraryHandle* pLibHandle);

AAFRDLIRESULT AAFUnloadLibrary(AAFLibraryHandle libHandle);

AAFRDLIRESULT AAFFindSymbol(AAFLibraryHandle libHandle, const char* symbolName, AAFSymbolAddr* pSymbol);

AAFRDLIRESULT AAFFindLibrary(const char* name, LPFNAAFTESTFILEPROC testProc, void *userData);

#if defined(macintosh) || defined(_MAC)
// Given just a file name this routine will use GetSharedLibrary() to load the code fragment.
AAFRDLIRESULT AAFLoadSharedLibrary(const char* name, AAFLibraryHandle* pLibHandle);

// Given a full path to a file this routine will use the file's cfrg resource to load
// the code fragment.
AAFRDLIRESULT AAFLoadLibraryFile(const char* name, AAFLibraryHandle* pLibHandle);

// Same as above except that it attempts to load the library from a mac file spec.
AAFRDLIRESULT AAFFSpLoadLibraryFile(FSSpec *pSpec, AAFLibraryHandle* pLibHandle);

// Given a pathname return the corresponding file spec.
AAFRDLIRESULT AAFPathToFSSpec(const char* name, FSSpec *pSpec);

// Given a pathname return the corresponding file spec.
AAFRDLIRESULT AAFFSSpecToPath(const FSSpec *pSpec, char *pathBuffer, int pathBufferSize);
#endif 



#ifdef __cplusplus
}
#endif

#endif /* __AAFRDLI_H__ */



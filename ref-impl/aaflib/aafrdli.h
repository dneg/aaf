/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
*  prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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



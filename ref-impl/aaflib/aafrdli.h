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
typedef int AAFRDLIRESULT;




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


#ifdef __cplusplus
}
#endif

#endif /* __AAFRDLI_H__ */

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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
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
#if defined( OS_WINDOWS )

// Declare the public interface that must be implemented.
#include "aafrdli.h"

// Win32 specific header files.
#include "windows.h"

// AAF header files.
#include "AAFResult.h"



AAFRDLIRESULT AAFLoadLibrary(const char* name, AAFLibraryHandle* pLibHandle)
{
  AAFRDLIRESULT result = AAFRESULT_SUCCESS;
  HINSTANCE hInstance = NULL;


  if (NULL == name || NULL == pLibHandle)
    return AAFRESULT_NULL_PARAM;


  // Attempt to load the library.
  hInstance = ::LoadLibraryA((LPCSTR)name);

  if (NULL == hInstance)
    result = HRESULT_FROM_WIN32(GetLastError());
  else
    *pLibHandle = (AAFLibraryHandle)hInstance;

  return result;
}

AAFRDLIRESULT AAFUnloadLibrary(AAFLibraryHandle libHandle)
{
  AAFRDLIRESULT result = AAFRESULT_SUCCESS;


  if (NULL == libHandle)
    return AAFRESULT_NULL_PARAM;

  if (!::FreeLibrary((HINSTANCE)libHandle))
    result = HRESULT_FROM_WIN32(GetLastError());

  return result;
}

AAFRDLIRESULT AAFFindSymbol(AAFLibraryHandle libHandle, const char* symbolName, AAFSymbolAddr* pSymbol)
{
  AAFRDLIRESULT result = AAFRESULT_SUCCESS;
  FARPROC pfn = NULL;

  
  if (NULL == libHandle || NULL == symbolName || NULL == pSymbol)
    return AAFRESULT_NULL_PARAM;

  pfn = ::GetProcAddress((HMODULE)libHandle, (LPCSTR)symbolName);
  if (NULL == pfn)
    result = HRESULT_FROM_WIN32(GetLastError());
  else
    *pSymbol = (AAFSymbolAddr)pfn;
  
  return result;
}


// This operates on a new directory component and checks
// if it should be descended into during a library search (see AAFFindLibrary)
static bool AAFIsValidDirectory(const char* name)
{
	// Do not allow any relative addressing in directory names.
	// Otherwise we may recurse in the find routines.
	if ( (0 == strcmp(".", name)) || (0 == strcmp("..", name)) )
		return false;
	else 
		return true;
}

static bool AAFIsValidFile(const char* name)
{
	if (NULL == name)
		return false;

	char fileExt[4];
	int i;
	const char *pExt = strrchr(name, '.');

	if (pExt && 4 == strlen(pExt))
	{
		++pExt; // advance past the "."
		fileExt[3] = 0;
		for (i = 0; 3 > i; ++i)
			fileExt[i] = toupper(pExt[i]);

		if (0 == strcmp("DLL", fileExt))
			return true;
	}

	return false;
}


AAFRDLIRESULT AAFFindLibrary(const char* name, LPFNAAFTESTFILEPROC testProc, void *userData)
{
  AAFRDLIRESULT result = AAFRESULT_SUCCESS;
	HANDLE hFindFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAA findData;
	
	size_t nameLength = 0;
	char findPath[(2 * MAX_PATH) + 2];
	bool done = false;
	bool isDirectory = false;

	
	if (NULL == name || NULL == testProc)
		return AAFRESULT_NULL_PARAM;

	nameLength = strlen(name);

	
	if (0 == nameLength)
	{
		// Temporary: If for some reason the directory is empty then
		// just iterate over the "known" plugins for the windows platform.
		const char *pluginFileNames[] = 
		{
			"AAFPGAPI.DLL",
			"AAFINTP.DLL",
			NULL
		};

		for (int i = 0; AAFRESULT_SUCCESS == result && pluginFileNames[i]; ++i)
			result = testProc(pluginFileNames[i], pluginFileNames[i], false /* not a directory */, userData);
	}
	else
	{

		// We need to append either a "\*" or a "*" to the end of the string.
		strcpy(findPath, name);
		if ('\\' == findPath[nameLength - 1])
		{
			if ((MAX_PATH - 2) <= nameLength) // Limitation of ansi version of FindFirstFile!
				return AAFRESULT_INVALID_PARAM;

			strcat(findPath, "*");
		}
		else
		{
			if ((MAX_PATH - 3) <= nameLength) // Limitation of ansi version of FindFirstFile!
				return AAFRESULT_INVALID_PARAM;

			strcat(findPath, "\\*");
		}



		// Attempt to find the file/directory.
		hFindFile = FindFirstFileA(findPath, &findData);
		if (NULL == hFindFile || INVALID_HANDLE_VALUE == hFindFile)
			result = AAFRESULT_FILE_NOT_FOUND;

		while (AAFRESULT_SUCCESS == result && !done)
		{
			// We have successfully find a file or directory.
			isDirectory = 0 != (FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes);

			// Build the full path.
			strcpy(&findPath[nameLength + 1], findData.cFileName);
			
			if (isDirectory)
			{
				if (AAFIsValidDirectory(findData.cFileName))
				{
					result = testProc(findPath, findData.cFileName, isDirectory, userData);
				}
			}
			else // its a file
			{
				// Check for valid AAFLibrary file extensions
				if (AAFIsValidFile(findPath))
				{
					result = testProc(findPath, findData.cFileName, isDirectory, userData);
				}
			}

			if (AAFRESULT_SUCCESS == result)
			{
				// Try to find another file or directory.
				if (!FindNextFileA(hFindFile, &findData))
				{
					done = true;
					result = GetLastError();
				}
			}
		} 

		// Remap successful result...
		if (ERROR_NO_MORE_FILES == result)
			result = AAFRESULT_SUCCESS;

		// Cleanup:
		if (NULL != hFindFile && INVALID_HANDLE_VALUE != hFindFile)
			FindClose(hFindFile);
	
	}

	return result;
}

#endif /* #if defined( OS_WINDOWS ) */


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
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
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



//
// Use include guard so that the file can be included in every 
// platform build without causing any errors in the build.
//
#if defined(WIN32) || defined(_WIN32)

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


static bool AAFIsValidDirectory(const char* name)
{
	if (NULL == name)
		return false;
	// Do not allow any relative addressing in directory names.
	// This is a simple security check that we might as well
	// perform to be on the "safe side"...
	else if ( (0 == strcmp(".", name)) || (0 == strcmp("..", name)) )
		return false;
	else if (NULL != strstr(name, ".."))
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
	char *pExt = strrchr(name, '.');

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
	
	int nameLength = 0;
	char findPath[(2 * MAX_PATH) + 2];
	bool done = false;
	bool isDirectory = false;

	
	if (NULL == name || NULL == testProc)
		return AAFRESULT_NULL_PARAM;

	if (!AAFIsValidDirectory(name))
		return AAFRESULT_INVALID_PARAM;

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
			result = testProc(pluginFileNames[i], false /* not a directory */, userData);
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
				if (AAFIsValidDirectory(findData.cFileName) && AAFIsValidDirectory(findPath))
					result = testProc(findPath, isDirectory, userData);
			}
			else // its a file
			{
				// Check for valid AAFLibrary file extensions
				if (AAFIsValidFile(findPath))
				{
					result = testProc(findPath, isDirectory, userData);
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

#endif /* #if defined(WIN32) || defined(_WIN32) */

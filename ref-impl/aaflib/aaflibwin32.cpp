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

AAFRDLIRESULT AAFGetLibraryInfo(AAFLibraryHandle libHandle, AAFLibInfo* pLibInfo)
{
  AAFRDLIRESULT result = AAFRESULT_SUCCESS;
  
  if (NULL == libHandle || NULL == pLibInfo || (NULL != pLibInfo || NULL == pLibInfo->nameBuffer))
    return AAFRESULT_NULL_PARAM;
  
  pLibInfo->actualNameSize = (int)::GetModuleFileNameA(
                                             (HMODULE)libHandle,
                                             (LPSTR)pLibInfo->nameBuffer, 
                                             (DWORD)pLibInfo->nameBufferSize);
  if (0 == pLibInfo->actualNameSize)
    result = HRESULT_FROM_WIN32(GetLastError());

  return result;
}


#endif /* #if defined(WIN32) || defined(_WIN32) */

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

#include "ImplAAFPluginFile.h"

// Include the AAF Runtime Dynamic (Library) Loader Interface  
#include "aafrdli.h"

#include "AAFResult.h"
#include <assert.h>
#include <string.h>

#if defined(WIN32)
// Include declarations for InterlockedIncrement() and InterlockcedDecrment().
#include <winbase.h>
#endif



ImplAAFPluginFile::ImplAAFPluginFile(const char *name) :
  _refCount(1),
  _name(name),
  _libHandle(NULL)
{
  ClearEntryPoints();
}


ImplAAFPluginFile::~ImplAAFPluginFile()
{
  delete[] const_cast<char *>(_name);
  _name = NULL;
}


// Factory method. Objects are created in a "loaded" (ie IsLoaded() will return true)
// A file is successfully loaded if ALL of the expected entry points could
// found.
HRESULT ImplAAFPluginFile::CreatePluginFile(
  const char* name,
  ImplAAFPluginFile** ppPluginFile)
{
  HRESULT result = S_OK;
  ImplAAFPluginFile* pPluginFile = NULL;


  if (NULL == name || NULL == ppPluginFile)
    return AAFRESULT_NULL_PARAM;

  // copy the given name. this will be owned by the plugin file object.
  int len = strlen(name) + 1;
  char *name_copy = new char[len];
  if (NULL == name_copy)
    result = AAFRESULT_NOMEMORY;
  else
  {
    strcpy(name_copy, name);
    pPluginFile = new ImplAAFPluginFile(name_copy);
    if (NULL == pPluginFile)
      result = AAFRESULT_NOMEMORY;
    else
    {
      // Attempt to load the entry points...
      result = pPluginFile->Load();
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppPluginFile = pPluginFile;
        pPluginFile = NULL;
      }      
    }
  }

  if (pPluginFile)
    pPluginFile->ReleaseReference();
  pPluginFile = 0;

  return result;
}


// Increment the object reference count.
aafUInt32 ImplAAFPluginFile::AcquireReference() const
{  
  ImplAAFPluginFile *nonConstThis = const_cast<ImplAAFPluginFile *>(this);
#if defined(WIN32)
	return ::InterlockedIncrement(reinterpret_cast<long *>(&nonConstThis->_refCount));
#else
  ++(nonConstThis->_refCount);
#endif
  return _refCount;
}

// Decrement the object reference count and delete the container.
aafUInt32 ImplAAFPluginFile::ReleaseReference()
{
#if defined(WIN32)
	aafUInt32 count = ::InterlockedDecrement(reinterpret_cast<long *>(&_refCount));
#else
	aafUInt32 count = --(_refCount);
#endif
  if (0 == count)
  {
    if (IsLoaded())
      Unload();

    delete this;
  }

  return count;
}

// Just return the count. (this could be inline in the header...)
aafUInt32 ImplAAFPluginFile::ReferenceCount() const
{ 
  return _refCount;
}



void ImplAAFPluginFile::ClearEntryPoints()
{
  _pfnCanUnloadNow = NULL;
  _pfnGetClassObject = NULL;
  _pfnGetClassCount = NULL;
  _pfnGetClassObjectID = NULL;
}


bool ImplAAFPluginFile::IsLoaded() const
{
  return (_libHandle          
		   && _pfnCanUnloadNow  && _pfnGetClassObject 
			 && _pfnGetClassCount && _pfnGetClassObjectID);
}

HRESULT ImplAAFPluginFile::Load()
{
  HRESULT result = AAFRESULT_SUCCESS;
 

  if (IsLoaded())
    return AAFRESULT_ALREADY_INITIALIZED;

  // First attempt to load the given file as the dynamic/shared library.
  // It this is successful then attempt to load all of the required symbols.
  result = ::AAFLoadLibrary(_name, (AAFLibraryHandle *)&_libHandle);
  if (AAFRESULT_SUCCEEDED(result))
  {
    result = ::AAFFindSymbol((AAFLibraryHandle)_libHandle,
                             "DllCanUnloadNow",
                             (AAFSymbolAddr *)&_pfnCanUnloadNow);
  }  

  if (AAFRESULT_SUCCEEDED(result))
  {
    result = ::AAFFindSymbol((AAFLibraryHandle)_libHandle,
                             "DllGetClassObject",
                             (AAFSymbolAddr *)&_pfnGetClassObject);
  }  

  if (AAFRESULT_SUCCEEDED(result))
  {
    result = ::AAFFindSymbol((AAFLibraryHandle)_libHandle,
                             "AAFGetClassCount",
                             (AAFSymbolAddr *)&_pfnGetClassCount);
  }  

  if (AAFRESULT_SUCCEEDED(result))
  {
    result = ::AAFFindSymbol((AAFLibraryHandle)_libHandle,
                             "AAFGetClassObjectID",
                             (AAFSymbolAddr *)&_pfnGetClassObjectID);
  }  

  
  // If we were not completely successful then unload the dll.
  if (AAFRESULT_FAILED(result))
    Unload();

  return result;
}

HRESULT ImplAAFPluginFile::Unload()
{
  HRESULT result = AAFRESULT_SUCCESS;

  if (NULL != _libHandle)
  {
    result = ::AAFUnloadLibrary((AAFLibraryHandle)_libHandle);

    if (AAFRESULT_SUCCEEDED(result))
    {
      ClearEntryPoints();
      _libHandle = NULL;
    }

  }

  return result;
}




HRESULT ImplAAFPluginFile::CanUnloadNow()
{
  if (!_libHandle)
    return AAFRESULT_NOT_INITIALIZED;
	else if (!_pfnCanUnloadNow) // If entrypoint was not loaded then CanUnloadNow should return ok.
		return S_OK;

  return _pfnCanUnloadNow();
}



HRESULT ImplAAFPluginFile::GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  if (!(_libHandle && _pfnGetClassObject))
    return AAFRESULT_NOT_INITIALIZED;

  return _pfnGetClassObject(rclsid, riid, ppv);
}



ULONG ImplAAFPluginFile::GetClassCount()
{
  if (!(_libHandle && _pfnGetClassCount))
    return 0;

  return _pfnGetClassCount();
}



HRESULT ImplAAFPluginFile::GetClassObjectID(ULONG index, CLSID* pClassID)
{
  if (!(_libHandle && _pfnGetClassObjectID))
    return AAFRESULT_NOT_INITIALIZED;

  return _pfnGetClassObjectID(index, pClassID);
}



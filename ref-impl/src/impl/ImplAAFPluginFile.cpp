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

#include "ImplAAFPluginFile.h"

// Include the AAF Runtime Dynamic (Library) Loader Interface  
#include "aafrdli.h"

#include "AAFTypes.h"
#include "AAFResult.h"
#include <string.h>

#if defined( OS_WINDOWS )
// Include declarations for InterlockedIncrement() and InterlockcedDecrment().
#include <winbase.h>
#endif



ImplAAFPluginFile::ImplAAFPluginFile(const char *name) :
  _refCount(1),
  _name(name),
  _libHandle(0)
{
  ClearEntryPoints();
}


ImplAAFPluginFile::~ImplAAFPluginFile()
{
  delete[] const_cast<char *>(_name);
  _name = 0;
}


// Factory method. Objects are created in a "loaded" (ie IsLoaded() will return true)
// A file is successfully loaded if ALL of the expected entry points could
// found.
HRESULT ImplAAFPluginFile::CreatePluginFile(
  const char* name,
  ImplAAFPluginFile** ppPluginFile)
{
  HRESULT result = S_OK;
  ImplAAFPluginFile* pPluginFile = 0;


  if (!name || !ppPluginFile)
    return AAFRESULT_NULL_PARAM;

  // copy the given name. this will be owned by the plugin file object.
  size_t len = strlen(name) + 1;
  char *name_copy = new char[len];
  if (!name_copy)
    result = AAFRESULT_NOMEMORY;
  else
  {
    strcpy(name_copy, name);
    pPluginFile = new ImplAAFPluginFile(name_copy);
    if (!pPluginFile)
      result = AAFRESULT_NOMEMORY;
    else
    {
      // Attempt to load the entry points...
      result = pPluginFile->Load();
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppPluginFile = pPluginFile;
        pPluginFile = 0;
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
#if defined( OS_WINDOWS )
	return ::InterlockedIncrement(reinterpret_cast<long *>(&nonConstThis->_refCount));
#else
  ++(nonConstThis->_refCount);
#endif
  return _refCount;
}

// Decrement the object reference count and delete the container.
aafUInt32 ImplAAFPluginFile::ReleaseReference()
{
#if defined( OS_WINDOWS )
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
  _pfnCanUnloadNow = 0;
  _pfnGetClassObject = 0;
  _pfnGetClassCount = 0;
  _pfnGetClassObjectID = 0;
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

  if (_libHandle)
  {
    result = ::AAFUnloadLibrary((AAFLibraryHandle)_libHandle);

    if (AAFRESULT_SUCCEEDED(result))
    {
      ClearEntryPoints();
      _libHandle = 0;
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



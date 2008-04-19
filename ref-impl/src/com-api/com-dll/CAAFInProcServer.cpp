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

#ifndef __CAAFInProcServer_h__
#include "CAAFInProcServer.h"
#endif

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined( OS_WINDOWS )
#include <olectl.h> // need ole control header for SELFREG_E_CLASS definition

#endif

#if defined( OS_WINDOWS )
# undef __TCHAR_DEFINED // why is this necessary!
# include <tchar.h>
#else
# ifndef _stprintf
#  define _stprintf sprintf
# endif
#endif




/////////////////////////////////////////////////////////////////////
// Begin: Block of includes and other things to access dladdr and 
// deal with its associated portability issues.

#if defined( OS_LINUX ) && !defined( GNU_SOURCE )

# define GNU_SOURCE
# include <dlfcn.h>
# undef GNU_SOURCE

#elif defined( OS_IRIX )

// IRIX requires that you implement dladdr() yourself(!) as follows.
// The following is taken from the IRIX man page dladdr(3C)
                                                                                
# include <rld_interface.h>
# ifndef _RLD_INTERFACE_DLFCN_H_DLADDR
#  define _RLD_INTERFACE_DLFCN_H_DLADDR
typedef struct Dl_info {
    const char * dli_fname;
    void       * dli_fbase;
    const char * dli_sname;
    void       * dli_saddr;
    int          dli_version;
    int          dli_reserved1;
    long         dli_reserved[4];
} Dl_info;
# endif
# define _RLD_DLADDR             14

int dladdr(void *address, Dl_info *dl)
{
    void *v = _rld_new_interface(_RLD_DLADDR,address,dl);
    return (int)v;
}

#elif defined( OS_UNIX )

# include <dlfcn.h>

#endif

//
// End block of includes and things for dladdr
/////////////////////////////////////////////////////////////////////



#include "AAFResult.h"

static const char * g_EmptyString = "";

// Implemented for each platform to get platform specific path information.
static HRESULT AAFGetLibraryInfo(HINSTANCE hInstance, char **pServerPath, char **pServerDirectory);




// Compare proc for sort and search.
static int CompareObjectInfo(const AAFComObjectInfo_t **elem1,
                             const AAFComObjectInfo_t **elem2)
{
  const CLSID& clsid1 = *((**elem1).pCLSID);
  const CLSID& clsid2 = *((**elem2).pCLSID);

  // Compare the unsigned int member
  if (clsid1.Data1 < clsid2.Data1)
    return -1;
  else if (clsid1.Data1 > clsid2.Data1)
    return 1;
  // Compare the first unsigned short member
  else if (clsid1.Data2 < clsid2.Data2)
    return -1;
  else if (clsid1.Data2 > clsid2.Data2)
    return 1;
  // Compare the second unsigned short member
  else if (clsid1.Data3 < clsid2.Data3)
    return -1;
  else if (clsid1.Data3 > clsid2.Data3)
    return 1;
  else
  // Compare the last 8 bytes.
    return memcmp(clsid1.Data4, clsid2.Data4, 8);
}


//
//
//



CAAFInProcServer::CAAFInProcServer() :
  CAAFServer(),
  _pObjectInfo(0),
  _hInstance(0),
  _serverPath(0),
  _serverDirectory(0),
  _ppObjectInfoKey(0),
  _objectCount(0)
{
}

CAAFInProcServer::~CAAFInProcServer()
{
  if (_ppObjectInfoKey)
  {
    delete [] _ppObjectInfoKey;
    _ppObjectInfoKey = 0;
  }

	if (_serverPath && _serverPath !=  g_EmptyString)
	{
		delete [] _serverPath;
		_serverPath = 0;
	}

	if (_serverDirectory && _serverDirectory !=  g_EmptyString)
	{
		delete [] _serverDirectory;
		_serverDirectory = 0;
	}
}


HRESULT CAAFInProcServer::Init
(
  AAFComObjectInfo_t *pObjectInfo,
  HINSTANCE hInstance
)
{
  _pObjectInfo = pObjectInfo;
  _hInstance = hInstance;

	// Initialize the platform dependent file path information.
	HRESULT rc = AAFGetLibraryInfo(hInstance, &_serverPath, &_serverDirectory);
	if (S_OK != rc)
	{ // ignore real errors for now...
		//
		//
		// Set reasonable defaults:
		_serverPath = const_cast<char *>(g_EmptyString);
		_serverDirectory = const_cast<char *>(g_EmptyString);
	}


  // Compute the size of the table.
  ULONG i = 0;
  while (pObjectInfo[i].pCLSID)
    ++i;
  _objectCount = i;

  // Allocate the key table to the object info table.
  _ppObjectInfoKey = new AAFComObjectInfo_t* [_objectCount];
  if (!_ppObjectInfoKey)
    return E_OUTOFMEMORY;

  // Initialize the table of pointers.
  for (i = 0; _objectCount > i; ++i)
    _ppObjectInfoKey[i] = &_pObjectInfo[i];

  // Sort the key for the table.
  qsort(_ppObjectInfoKey, _objectCount , sizeof(AAFComObjectInfo_t *),
        (int (*)(const void*, const void*))CompareObjectInfo); 
 
  return S_OK;
}

HRESULT CAAFInProcServer::Term()
{
  return S_OK;
}



HRESULT CAAFInProcServer::GetClassObject
( 
  REFCLSID rclsid,
  REFIID riid,
  void ** ppv )
{
  HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
  if (!ppv)
    return E_INVALIDARG;
  *ppv = 0;

  // Search the object table for the given class id.
  // Lookup the class id in the com object table.
  AAFComObjectInfo_t **ppResult = 0;
  AAFComObjectInfo_t key = {&rclsid, OLESTR("KEY"), 0, false};
  AAFComObjectInfo_t *pKey = &key;
  
  // Use standard library's binary search routine.
  ppResult = (AAFComObjectInfo_t **)bsearch(&pKey, _ppObjectInfoKey, _objectCount,
               sizeof(AAFComObjectInfo_t *),
               (int (*)(const void*, const void*))CompareObjectInfo);
  if (!ppResult)
    return hr;

  // We found the requested class id so attempt to create a class factory.
  CAAFClassFactory *pAAFClassFactory = new CAAFClassFactory((**ppResult).pfnCreate);
  if (!pAAFClassFactory)
    return E_OUTOFMEMORY;

  // We now have a local reference.
  pAAFClassFactory->AddRef();

  // See of the factory supports the requested interface. If this succeeds
  // then the reference count will be two. If it fails the reference
  // count will be one and the following release will delete the factory
  // object.
  hr = pAAFClassFactory->QueryInterface(riid, ppv);

  // We are through with this object.
  pAAFClassFactory->Release();

  return hr;
}

HRESULT CAAFInProcServer::CanUnloadNow( )
{
  // If there are any outstanding locks or active objects then we cannot 
  // unload the server.
  if (0 < GetLockCount() || 0 < GetActiveObjectCount()) 
    return S_FALSE;
  else
    return S_OK;
}






// Global table used by Register/UnregisterServer.
typedef struct tagAAFRegEntry
{
  unsigned int flags;
  LPCOLESTR pFormat;
} AAFRegEntry;

enum eAAFRegFlag
{
  AAF_REG_SUB_SKIP = 0,
  AAF_REG_SUB_NONE = 1,
  AAF_REG_SUB_CLSID = 2,
  AAF_REG_SUB_CLASSNAME = 3,
  AAF_REG_SUB_MODULE = 4
};


// Find the next valid index in the internal object table where
// bRegister matches the entry in the table...
const int kInvalidObjectIndex = -1;

int CAAFInProcServer::GetRegisterIndex(int index)
{
	int nextIndex = kInvalidObjectIndex; // termination signal

	if (index >= 0)
	{
		while ((index < (int)_objectCount) && _pObjectInfo[index].pCLSID)
		{
			if (_pObjectInfo[index].bRegisterClass)
			{
				nextIndex = index;
				break;
			}	
			++index;
		}
	}

	return nextIndex;
}


HRESULT CAAFInProcServer::RegisterServer
( 
  BOOL /*bRegTypeLib*/
)
{
  return S_OK;
}

HRESULT CAAFInProcServer::UnregisterServer
(
  void
)
{
  return S_OK;
}


// Return the number of coclasses exported from this dll.
ULONG CAAFInProcServer::GetClassCount( )
{	
	// Just return our global count computed by the Init() method.
	return _objectCount;
}

// Get the nth implementation coclass id.
HRESULT CAAFInProcServer::GetClassObjectID(ULONG index, CLSID *pClassID)
{
	// Validate pre-conditions and parameters.
  if (!_ppObjectInfoKey || 0 == GetClassCount())
		return CLASS_E_CLASSNOTAVAILABLE;
	if (index >= GetClassCount() || !pClassID)
		return E_INVALIDARG;

	// Lookup the corresponding object info pointer from our table.
	AAFComObjectInfo_t *pInfo = _ppObjectInfoKey[index];
	assert(pInfo!=0);
	memcpy(pClassID, pInfo->pCLSID, sizeof(CLSID));
	
	return S_OK;
}

// Platform dependent file system information needed by the reference
// implementation.
const char* CAAFInProcServer::GetServerPath() const
{
	return _serverPath;
}
const char* CAAFInProcServer::GetServerDirectory() const
{
	return _serverDirectory;
}

//
// AAFGetLibraryInfo()
// Implemented for each platform to get platform specific path information.
//
#if defined( OS_WINDOWS )

#ifdef _MSC_VER
#pragma warning (disable:4996)
#endif

HRESULT AAFGetLibraryInfo(HINSTANCE hInstance, char **pServerPath, char **pServerDirectory)
{
	HRESULT rc = S_OK;
	char path[MAX_PATH];
	size_t pathLen = 0;
	char *pDirSeparator = 0;


	if (!hInstance || !pServerPath || !pServerDirectory)
		return E_INVALIDARG;

	pathLen = ::GetModuleFileNameA((HMODULE)hInstance, path, MAX_PATH);
	if (0 == pathLen)
	{
		rc = HRESULT_FROM_WIN32(GetLastError());
	}
	else
	{
		*pServerPath = new char[pathLen + 1];
		if (!(*pServerPath))
			return E_OUTOFMEMORY;
		strcpy(*pServerPath, path);

		pDirSeparator = strrchr(path, '\\');
		assert(pDirSeparator);
		
		// Terminate the directory string.
		*pDirSeparator = 0;
		pathLen = strlen(path);

		*pServerDirectory = new char[pathLen + 1];
		if (!(*pServerDirectory))
		{
			delete [] *pServerPath;
			*pServerPath = 0;
			return E_OUTOFMEMORY;
		}
		strcpy(*pServerDirectory, path);
	}

	return rc;
}

#elif defined( OS_UNIX )

HRESULT AAFGetLibraryInfo(
   HINSTANCE hInstance,
   char **pServerPath,
   char **pServerDirectory)
{
    HRESULT rc = S_OK;
    Dl_info info;
    size_t len = 0;
    char *pDirSeparator = 0;


    if (!pServerPath || !pServerDirectory)
        return E_INVALIDARG;

    // Inquire about this dynamic library's whereabouts.
    // Don't use hInstance (it's NULL on UNIX anyway). Use
    // this function's address as a way to inquire about
    // the location of the enclosing library.
    if (dladdr((void *)AAFGetLibraryInfo, &info) == 0)
        return E_INVALIDARG;


    // Allocate and copy into new buffer for library path
    // We use this as a temporary string manipulation buffer
    // when deducing the directory portion (below),
    // but we restore the full value at the end.
    len = strlen(info.dli_fname);
    *pServerPath = new char[len + 3];
    if (!(*pServerPath))
        return E_OUTOFMEMORY;

    // Add "./" if path does not contain a '/' since the
    // path manipulation logic requires the presence of '/'
    if ( strrchr(info.dli_fname, '/') == NULL )
    {
        strcpy(*pServerPath, "./");
        strcat(*pServerPath, info.dli_fname);
    }
    else
        strcpy(*pServerPath, info.dli_fname);

    // This section cuts off the filename of the library
    // we've found, leaving only the directory.
    // This, like all code in this function,
    // parallels the Windows equivalent. A major difference
    // is that dladdr() doesn't always return a fully-qualified
    // (absolute) path like Win32's GetModuleFileName() does.
    pDirSeparator = strrchr(*pServerPath, '/');
    assert(pDirSeparator);

	
    // Terminate by overwriting the '/'	
    *pDirSeparator = '\0';


    // Allocate buffer for path directory information and copy into it.
    len = strlen(*pServerPath);
    *pServerDirectory = new char[len + 1];
    if (!(*pServerDirectory))
    {
        delete [] *pServerPath;
        *pServerPath = 0;
        return E_OUTOFMEMORY;
    }
    strcpy(*pServerDirectory, *pServerPath);


    // Restore the '/' we overwrote above to reconstruct full path for caller
    *pDirSeparator = '/';


    return rc;
}

#else // other platform?

HRESULT AAFGetLibraryInfo(HINSTANCE hInstance, char **pServerPath, char **pServerDirectory)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

#endif  // OS_WINDOWS

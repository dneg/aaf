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

#ifndef __CAAFInProcServer_h__
#include "CAAFInProcServer.h"
#endif

#if defined(macintosh) || defined(_MAC)
// Temporary.
#include "aafrdli.h"
#endif

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if ! defined (__sgi) && ! defined (__FreeBSD__)
#include <olectl.h> // need ole control header for SELFREG_E_CLASS definition
#endif

#if defined(_WIN32) || defined(WIN32)
# undef __TCHAR_DEFINED // why is this necessary!
# include <tchar.h>
#else
# ifndef _stprintf
#  define _stprintf sprintf
# endif
#endif

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

  // Compare the unsigned long member
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
  size_t i = 0;
  while (pObjectInfo[i].pCLSID)
    ++i;
  _objectCount = i;
  printf("this = %p, &_objectCount = %p, _objectCount = %d\n",
	this,&_objectCount,(int)_objectCount);


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
  unsigned long flags;
  LPCOLESTR pFormat;
} AAFRegEntry;

enum eAAFRegFlag
{
  AAF_REG_SUB_SKIP = 0,
  AAF_REG_SUB_NONE = 1,
  AAF_REG_SUB_CLSID = 2,
  AAF_REG_SUB_CLASSNAME = 3,
  AAF_REG_SUB_MODULE = 4
#if defined(_MAC)
  ,AAF_REG_SUB_ALIAS = 5
#endif
};


const AAFRegEntry g_AAFRegEntry[][3] = 
{
  {  // [0]
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") }
  },
#if defined(_MAC) 
  {  // [1]
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\InprocServer") },
    { AAF_REG_SUB_SKIP, 0 },
    { AAF_REG_SUB_MODULE, OLESTR("ALS2:%s") }
  },
#else
  {  // [1]
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\InprocServer32") },
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_MODULE, OLESTR("%s") }
  },
#endif 
  {  // [2] 
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\NotInsertable") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_SKIP, 0 }
  },
  {  // [3]
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\ProgID") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") }
  },
  {  // [4] 
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\VersionIndependentProgID") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s") }  
  },
  {  // [5]  
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") }    
  },
  {  // [6]
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s\\CLSID") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_CLSID, OLESTR("%s") } 
  },
  {  // [7]
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s\\CurVer") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") } 
  },
  {  // [8]
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") } 
  },
  {  // [9] 
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1\\CLSID") }, 
    { AAF_REG_SUB_SKIP, 0 }, 
    { AAF_REG_SUB_CLSID, OLESTR("%s") } 
  }
};


static int FormatRegBuffer 
(
  LPOLESTR pBuffer,
  const AAFRegEntry& entry,
  LPOLESTR pCLSIDString,
  LPCOLESTR pClassName,
  LPOLESTR pFileName
)
{
  LPCOLESTR pParam = 0;

  switch (entry.flags)
  {
    case AAF_REG_SUB_CLSID:
      pParam = pCLSIDString;
      break;

    case AAF_REG_SUB_CLASSNAME:
      pParam = pClassName;
      break;

    case AAF_REG_SUB_MODULE:
      pParam = pFileName;
      break;
    
#if defined(_MAC)
    case AAF_REG_SUB_ALIAS:
#endif
    case AAF_REG_SUB_SKIP:
      pBuffer[0] = 0; // set to empty string so that we set the 
      return 0;

    case AAF_REG_SUB_NONE:
      // No parameter substitution, just copy the format string.
      return _stprintf(pBuffer, entry.pFormat);      

    default: // signal end (and/or error)
      return -1;
  } // switch (entry.flags)
  
  // Format the buffer.
  assert(pParam!=0);
  return _stprintf(pBuffer, entry.pFormat, pParam);
  
}


// Find the next valid index in the internal object table where
// bRegister matches the entry in the table...
const long kInvalidObjectIndex = -1;

long CAAFInProcServer::GetRegisterIndex(long index)
{
	long nextIndex = kInvalidObjectIndex; // termination signal

	if (index >= 0)
	{
		while ((index < (long)_objectCount) && _pObjectInfo[index].pCLSID)
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
  HRESULT hr = S_OK;

#ifndef __sgi
#if defined(_MAC)
  // In the MAC version we store the fragment block pointer in the HINSTANCE member.
  CFragInitBlockPtr initBlkPtr = static_cast<CFragInitBlockPtr>(_hInstance);
  assert(initBlkPtr);
  
  // Create the alias that we will be storing in the registry instead of the module path.
  AliasHandle hAlias = 0;
  AliasPtr pAlias = 0;
  unsigned long aliasLength = 0;
  ::NewAlias (0, initBlkPtr->fragLocator.u.onDisk.fileSpec, &hAlias);
  
  // Alias could not be created: signal failure.
  if (!hAlias)
    return SELFREG_E_CLASS;
  
  aliasLength = GetHandleSize((Handle)hAlias);
  OSErr err = MemError();
  if (noErr != err)
  {
    DisposeHandle((Handle)hAlias);
    hr = E_UNEXPECTED; // need better error code.
    return hr;
  }
  HLock((Handle)hAlias);
  err = MemError();
  if (noErr != err)
  {
    DisposeHandle((Handle)hAlias);
    hr = E_UNEXPECTED; // need better error code.
    return hr;
  }
  pAlias = *hAlias;
  
  // Copy the file name as the "frag name"
  OLECHAR fileName[64];
  memcpy(fileName, &(initBlkPtr->fragLocator.u.onDisk.fileSpec->name[1]), (initBlkPtr->fragLocator.u.onDisk.fileSpec->name[0]));
  fileName[(initBlkPtr->fragLocator.u.onDisk.fileSpec->name[0])] = 0;
#else
  // For the following code to work either _UNICODE and UNICODE
  // must be defined and OLE2ANSI must NOT be defined or every symbol must be undefined 
  // or OLE2ANSI defined but _UNICODE and UNICODE are undefined.
  assert(sizeof(OLECHAR) == sizeof(TCHAR));


  // All of our objects need to register there module location.
  // NOTE: This code may be platform dependent.
  TCHAR fileName[MAX_PATH];

  int fileNameLength = (int)GetModuleFileName(_hInstance, fileName, MAX_PATH);
  if (0 == fileNameLength)
    return GetLastError();  // TODO: convert to HRESULT!  
#endif  

  // Buffer for string version of each object's class id.
  const int MAX_CLSID_SIZE = 40;
  OLECHAR pCLSIDbuffer[MAX_CLSID_SIZE];
  int clsidLength;

  
  // Allocate the buffers for the key, value name and value data.
  const int MAX_REG_BUFFER = 128;
  OLECHAR pRegBuffer[3][MAX_REG_BUFFER];
  int regLength[3];
  LPOLESTR pKeyName = pRegBuffer[0];
  LPOLESTR pValueName = pRegBuffer[1];
  LPOLESTR pValue= pRegBuffer[2];

  // Use g_AAFRegEntry data to register each object in the object info table.
  // Search the object table for the given class id.
  long int objectIndex = 0;
  while (kInvalidObjectIndex != (objectIndex = GetRegisterIndex(objectIndex)))
  {
    // Convert the object's class id into a string suitable for the 
    // registry.
    clsidLength = StringFromGUID2(*_pObjectInfo[objectIndex].pCLSID,
                    pCLSIDbuffer, 
                    MAX_CLSID_SIZE);
    // Note: The returned length includes the NULL character.
    if (0 == clsidLength)
    {
      hr = E_UNEXPECTED; // need better error code.
      break;
    }
    

    // Now run through the AAF Reg Entries and create the corresponding
    // entries in the registry.
    int nEntries = sizeof(g_AAFRegEntry)/sizeof(*g_AAFRegEntry);
    for (int keyIndex = 0; keyIndex < nEntries; ++keyIndex)
    {
      for (int formatIndex = 0; formatIndex < 3; ++formatIndex)
      {
        regLength[formatIndex] = FormatRegBuffer (
          pRegBuffer[formatIndex],
          g_AAFRegEntry[keyIndex][formatIndex],
          pCLSIDbuffer,
          _pObjectInfo[objectIndex].pClassName,
          fileName);
      }

      // Create the Key
      HKEY hkey;
      long err = RegCreateKey(HKEY_CLASSES_ROOT, pKeyName, &hkey);
      if (ERROR_SUCCESS == err)
      {
#if defined(_MAC) 
        // Set the value. Note: we need to include the null character
        // but tell the registry the actual number of bytes we are
        // writing. (Mac wintypes.h defines BYTE as unsigned char.)
        err = RegSetValueEx(hkey, pValueName, 0, 
            REG_SZ, (const char*)pValue,
            (regLength[2] + 1) * sizeof(OLECHAR));

        // The second key need to include an alias on the Macintosh.
        if (1 == keyIndex)
        {
          // Set the value. Note: 
          err = RegSetValueEx(hkey, OLESTR("Alias"), 0, 
              REG_BINARY, (const char*)pAlias,
              aliasLength);
        }
#else
        // Set the value. Note: we need to include the null character
        // but tell the registry the actual number of bytes we are
        // writing. (windows defines BYTE as signed char.)
        err = RegSetValueEx(hkey, pValueName, 0, 
            REG_SZ, (const BYTE*)pValue,
            (regLength[2] + 1) * sizeof(TCHAR));
#endif

        RegCloseKey(hkey);
      }
      
      if (ERROR_SUCCESS != err)
      {
        // If we could not add a key or value then back out and fail.
        UnregisterServer();
        hr = SELFREG_E_CLASS; // self registration failed!
      }
    }

    
    // Next object in the table...
    ++objectIndex;
  }

#if defined(_MAC)
  if (hAlias)
  {
    if (pAlias)
      HUnlock((Handle)hAlias);
    DisposeHandle((Handle)hAlias);
  }
#endif
#endif // #ifndef __sgi
  return hr;
}

HRESULT CAAFInProcServer::UnregisterServer
(
  void
)
{
#ifndef __sgi
  HRESULT hr = S_OK;

  // Buffer for string version of each object's class id.
  const int MAX_CLSID_SIZE = 40;
  OLECHAR pCLSIDbuffer[MAX_CLSID_SIZE];
  int clsidLength;

  // Allocate the buffers for the key, value name and value data.
  const int MAX_REG_BUFFER = 128;
  OLECHAR pRegBuffer[MAX_REG_BUFFER];
  int regLength;
  LPOLESTR pKeyName = pRegBuffer;

  // Use g_AAFRegEntry data to register each object in the object info table.
  // Search the object table for the given class id.
  long int objectIndex = 0;
  while (_pObjectInfo[objectIndex].pCLSID)
  {
    // Convert the object's class id into a string suitable for the 
    // registry.
    clsidLength = StringFromGUID2(*_pObjectInfo[objectIndex].pCLSID,
                    pCLSIDbuffer, 
                    MAX_CLSID_SIZE);
    // Note: The returned length includes the NULL character.
    if (0 == clsidLength)
      return E_UNEXPECTED; // need better error code.
    

    // Now run through the AAF Reg Entries and create the corresponding
    // entries in the registry.
    int nEntries = sizeof(g_AAFRegEntry)/sizeof(*g_AAFRegEntry);
    for (int keyIndex = nEntries - 1; keyIndex >= 0; --keyIndex)
    {
      regLength = FormatRegBuffer (
          pRegBuffer,
          g_AAFRegEntry[keyIndex][0],
          pCLSIDbuffer,
          _pObjectInfo[objectIndex].pClassName,
          0);

      // Delete the Key
      long err = RegDeleteKey(HKEY_CLASSES_ROOT, pKeyName);
      if (ERROR_SUCCESS != err)
        hr = S_FALSE; // intensionally overwrite error.        
    }
    
    // Next object in the table...
    ++objectIndex;
  }
#endif // #ifndef __sgi
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
#if defined(WIN32) || defined(_WIN32)

HRESULT AAFGetLibraryInfo(HINSTANCE hInstance, char **pServerPath, char **pServerDirectory)
{
	HRESULT rc = S_OK;
	char path[MAX_PATH];
	int pathLen = 0;
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

#elif defined(macintosh) || defined(_MAC)

HRESULT AAFGetLibraryInfo(HINSTANCE hInstance, char **pServerPath, char **pServerDirectory)
{
	HRESULT rc = S_OK;
	char saveChar;
	char *pDirSeparator = 0;
	int pathLen = 0;
	
	if (!pServerPath || !pServerDirectory)
	  return AAFRESULT_NULL_PARAM;
	
	*pServerPath = *pServerDirectory = 0;
	
	// In the MAC version we store the fragment block pointer in the HINSTANCE member.
	CFragInitBlockPtr initBlkPtr = static_cast<CFragInitBlockPtr>(hInstance);
	assert(initBlkPtr);
	
	
	//
	// Attempt to allocate and intialize the path to this code fragment.
	int pathBufferSize = sizeof(StrFileName);
	do
	{
		*pServerPath = new char[pathBufferSize];
		if (!(*pServerPath))
			return E_OUTOFMEMORY;
		rc = AAFFSSpecToPath(initBlkPtr->fragLocator.u.onDisk.fileSpec, *pServerPath, pathBufferSize);
		if (S_OK != rc)
		{
			delete [] *pServerPath;
			*pServerPath = 0;
		}
		pathBufferSize += sizeof(StrFileName);
	} while (AAFRESULT_SMALLBUF == rc);
	
	if (S_OK == rc)
	{
		pDirSeparator = strrchr(*pServerPath, ':');
		assert(pDirSeparator);
		
		// Copy the directory path.
		saveChar = pDirSeparator[1];
		pDirSeparator[1] = 0;
		pathLen = strlen(*pServerPath);
		
		*pServerDirectory = new char[pathLen + 1];
		if (!(*pServerDirectory))
		{
			delete [] *pServerPath;
			*pServerPath = 0;
			return E_OUTOFMEMORY;
		}
		strcpy(*pServerDirectory, *pServerPath);
		pDirSeparator[1] = saveChar; // restore saved character
	}	
	
	return rc;
}

#elif defined( __sgi )

HRESULT AAFGetLibraryInfo(
   HINSTANCE hInstance,
   char **pServerPath,
   char **pServerDirectory)
{
   HRESULT rc = S_OK;

   // Library directory is not used under SGI
   char pLibDirectory[] = "";

   // Under SGI, we make the HINSTANCE pointer simply contain the name of
   // the library.
   if(!hInstance)
   {
	char pLibName[]="libcom-api.so";
        *pServerPath = new char[strlen(pLibName)+1];
   	strcpy( *pServerPath, pLibName );
   }
   else
   {
	*pServerPath = new char[strlen((char*)hInstance)];
	strcpy( *pServerPath, (char*)hInstance);
   }

   *pServerDirectory = new char[strlen(pLibDirectory) + 1];
   strcpy( *pServerDirectory, pLibDirectory );
   return rc;
}

#else // other Unix?

HRESULT AAFGetLibraryInfo(HINSTANCE hInstance, char **pServerPath, char **pServerDirectory)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

#endif

#ifndef __CAAFInProcServer_h__
#include "CAAFInProcServer.h"
#endif


#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <olectl.h> // need ole control header for SELFREG_E_CLASS definition

#if defined(_WIN32) || defined(WIN32)
# undef __TCHAR_DEFINED // why is this necessary!
# include <tchar.h>
#else
# ifndef _stprintf
#  define _stprintf sprintf
# endif
#endif



// private key data for object info array. NOTE: This data is declared global to cause the
// least amount of impact to other AAF source code. This is safe since there is only
// a single CAAFInprocServer per dll.
static AAFComObjectInfo_t **g_ppObjectInfoKey = NULL;
static size_t g_objectCount = 0;

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



CAAFInProcServer::CAAFInProcServer() :
  CAAFServer(),
  _pObjectInfo(NULL),
  _hInstance(NULL)
{
}

CAAFInProcServer::~CAAFInProcServer()
{
  if (NULL != g_ppObjectInfoKey)
  {
    CoTaskMemFree(g_ppObjectInfoKey);
    g_ppObjectInfoKey = NULL;
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

  
  // Compute the size of the table.
  size_t i = 0;
  while (pObjectInfo[i].pCLSID)
    ++i;
  g_objectCount = i;


  // Allocate the key table to the object info table.
  g_ppObjectInfoKey = (AAFComObjectInfo_t **)CoTaskMemAlloc(g_objectCount * sizeof(AAFComObjectInfo_t *));
  if (NULL == g_ppObjectInfoKey)
    return E_OUTOFMEMORY;

  // Initialize the table of pointers.
  for (i = 0; g_objectCount > i; ++i)
    g_ppObjectInfoKey[i] = &_pObjectInfo[i];

  // Sort the key for the table.
  qsort(g_ppObjectInfoKey, g_objectCount , sizeof(AAFComObjectInfo_t *),
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
  if (NULL == ppv)
    return E_INVALIDARG;
  *ppv = NULL;

  // Search the object table for the given class id.
  // Lookup the class id in the com object table.
  AAFComObjectInfo_t **ppResult = NULL;
  AAFComObjectInfo_t key = {&rclsid, OLESTR("KEY"), NULL, false};
  AAFComObjectInfo_t *pKey = &key;
  
  // Use standard library's binary search routine.
  ppResult = (AAFComObjectInfo_t **)bsearch(&pKey, g_ppObjectInfoKey, g_objectCount,
               sizeof(AAFComObjectInfo_t *),
               (int (*)(const void*, const void*))CompareObjectInfo);
  if (NULL == ppResult)
    return hr;

  // We found the requested class id so attempt to create a class factory.
  CAAFClassFactory *pAAFClassFactory = new CAAFClassFactory((**ppResult).pfnCreate);
  if (NULL == pAAFClassFactory)
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

  return S_OK;
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
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") }
  },
#if defined(_MAC) 
  {  // [1]
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\InprocServer") },
    { AAF_REG_SUB_SKIP, NULL },
    { AAF_REG_SUB_MODULE, OLESTR("ALS2:%s") }
  },
#else
  {  // [1]
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\InprocServer32") },
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_MODULE, OLESTR("%s") }
  },
#endif 
  {  // [2] 
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\NotInsertable") }, 
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_SKIP, NULL }
  },
  {  // [3]
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\ProgID") }, 
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") }
  },
  {  // [4] 
    { AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\VersionIndependentProgID") }, 
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s") }  
  },
  {  // [5]  
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s") }, 
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") }    
  },
  {  // [6]
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s\\CLSID") }, 
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_CLSID, OLESTR("%s") } 
  },
  {  // [7]
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s\\CurVer") }, 
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") } 
  },
  {  // [8]
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") }, 
    { AAF_REG_SUB_SKIP, NULL }, 
    { AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") } 
  },
  {  // [9] 
    { AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1\\CLSID") }, 
    { AAF_REG_SUB_SKIP, NULL }, 
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
  LPCOLESTR pParam = NULL;

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
  assert(NULL != pParam);
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
		while ((index < (long)g_objectCount) && NULL != _pObjectInfo[index].pCLSID)
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


#if defined(_MAC)
  // In the MAC version we store the fragment block pointer in the HINSTANCE member.
  CFragInitBlockPtr initBlkPtr = static_cast<CFragInitBlockPtr>(_hInstance);
  assert(initBlkPtr);
  
  // Create the alias that we will be storing in the registry instead of the module path.
  AliasHandle hAlias = NULL;
  AliasPtr pAlias = NULL;
  unsigned long aliasLength = 0;
  ::NewAlias (NULL, initBlkPtr->fragLocator.u.onDisk.fileSpec, &hAlias);
  
  // Alias could not be created: signal failure.
  if (NULL == hAlias)
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
  return hr;
}

HRESULT CAAFInProcServer::UnregisterServer
(
  void
)
{
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
  while (NULL != _pObjectInfo[objectIndex].pCLSID)
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
          NULL);

      // Delete the Key
      long err = RegDeleteKey(HKEY_CLASSES_ROOT, pKeyName);
      if (ERROR_SUCCESS != err)
        hr = S_FALSE; // intensionally overwrite error.        
    }
    
    // Next object in the table...
    ++objectIndex;
  }

  return S_OK;
}


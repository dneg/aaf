

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __CAAFInProcServer_h__
#include "CAAFInProcServer.h"
#endif

#undef __TCHAR_DEFINED // why is this necessary!
#include <tchar.h> // include wide characters and routines mappings
#include <assert.h>

#include <olectl.h> // need ole control header for SELFREG_E_CLASS definition

CAAFInProcServer::CAAFInProcServer() :
	CAAFServer(),
	_pObjectInfo(NULL),
	_hInstance(NULL)
{
}

CAAFInProcServer::~CAAFInProcServer()
{
}

HRESULT CAAFInProcServer::Init
(
	AAFComObjectInfo_t *pObjectInfo,
	HINSTANCE hInstance
)
{
	_pObjectInfo = pObjectInfo;
	_hInstance = hInstance;
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
	long int index = 0;
	while (NULL != _pObjectInfo[index].pCLSID)
	{
		if (IsEqualCLSID(rclsid, *_pObjectInfo[index].pCLSID))
		{
			CAAFClassFactory *pAAFClassFactory = new CAAFClassFactory( _pObjectInfo[index].pfnCreate);
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

			return hr;
		}

		++index;
	}

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
};


const AAFRegEntry g_AAFRegEntry[][3] = 
{
	{	// [0]
		{ AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") }
	},
	{	// [1] 
		{ AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\InprocServer32") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_MODULE, OLESTR("%s") }
	},
	{	// [2] 
		{ AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\NotInsertable") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_SKIP, NULL }
	},
	{	// [3]
		{ AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\ProgID") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") }
	},
	{	// [4] 
		{ AAF_REG_SUB_CLSID, OLESTR("CLSID\\%s\\VersionIndependentProgID") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s") }	
	},
	{	// [5]  
		{ AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") }		
	},
	{	// [6]
		{ AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s\\CLSID") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_CLSID, OLESTR("%s") } 
	},
	{	// [7]
		{ AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s\\CurVer") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") } 
	},
	{	// [8]
		{ AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_CLASSNAME, OLESTR("%s Class") } 
	},
	{	// [9] 
		{ AAF_REG_SUB_CLASSNAME, OLESTR("AAF.%s.1\\CLSID") }, 
		{ AAF_REG_SUB_SKIP, NULL }, 
		{ AAF_REG_SUB_CLSID, OLESTR("%s") } 
	}
};


static int FormatRegBuffer 
(
	LPTSTR pBuffer,
	const AAFRegEntry& entry,
	LPCTSTR pCLSIDString,
	LPCTSTR pClassName,
	LPCTSTR pFileName
)
{
	LPCTSTR pParam = NULL;

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




HRESULT CAAFInProcServer::RegisterServer
( 
	BOOL bRegTypeLib
)
{
	HRESULT hr = S_OK;

	// For the following code to work either _UNICODE and UNICODE
	// must be defined and OLE2ANSI must NOT be defined or every symbol must be undefined 
	// or OLE2ANSI defined but _UNICODE and UNICODE are undefined.
	assert(sizeof(OLECHAR) == sizeof(TCHAR));


	// Buffer for string version of each object's class id.
	const int MAX_CLSID_SIZE = 40;
	OLECHAR pCLSIDbuffer[MAX_CLSID_SIZE];
	int clsidLength;

	// All of our objects need to register there module location.
	// NOTE: This code may be platform dependent.
	TCHAR fileName[MAX_PATH];

	int fileNameLength = (int)GetModuleFileName(_hInstance, fileName, MAX_PATH);
	if (0 == fileNameLength)
		return GetLastError();	// TODO: convert to HRESULT!	
	
	// Allocate the buffers for the key, value name and value data.
	const int MAX_REG_BUFFER = 128;
	TCHAR pRegBuffer[3][MAX_REG_BUFFER];
	int regLength[3];
	LPCTSTR pKeyName = pRegBuffer[0];
	LPCTSTR pValueName = pRegBuffer[1];
	LPCTSTR pValue= pRegBuffer[2];

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
				// Set the value. Note: we need to include the null character
				// but tell the registry the actual number of bytes we are
				// writing.
				err = RegSetValueEx(hkey, pValueName, 0, 
						REG_SZ, (const BYTE*)pValue,
						(regLength[2] + 1) * sizeof(TCHAR));

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
	TCHAR pRegBuffer[MAX_REG_BUFFER];
	int regLength;
	LPCTSTR pKeyName = pRegBuffer;

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

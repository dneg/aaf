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
#define __CAAFInProcServer_h__


#ifndef __CAAFServer_h__
#include "CAAFServer.h"
#endif

#ifndef __AAFCOMPlatform_h__
#include "AAFCOMPlatform.h"
#endif

#ifndef __CAAFClassFactory_h__
#include "CAAFClassFactory.h"
#endif

#include <stddef.h> // for size_t

typedef struct tagAAFComObjectInfo
{
	const CLSID* pCLSID;
	LPCOLESTR pClassName;
	AAFCreateComObjectProc pfnCreate;
	bool bRegisterClass;
} AAFComObjectInfo_t;



#define AAF_BEGIN_OBJECT_MAP(x) static AAFComObjectInfo_t x[] = {
#define AAF_LAST_ENTRY() { NULL, NULL, NULL, false }
#define AAF_END_OBJECT_MAP()  AAF_LAST_ENTRY() };
#define AAF_OBJECT_ENTRYX(class,reg) { &CLSID_##class, OLESTR(#class), &C##class##::COMCreate, reg },

// Define standard entries do not register the class.
#define AAF_OBJECT_ENTRY(class) AAF_OBJECT_ENTRYX(class,false)

// Plugins/Class extensions are external and may have a registry component...
#define AAF_PLUGIN_OBJECT_ENTRY(class) AAF_OBJECT_ENTRYX(class,true)



class CAAFInProcServer : 
	public CAAFServer
{
public:
	CAAFInProcServer();
	~CAAFInProcServer();

	HRESULT Init( AAFComObjectInfo_t *, HINSTANCE );
	HRESULT Term();
  
	//
	// Normal COM In Proc Server entrypoints.
	//
	HRESULT GetClassObject( REFCLSID rclsid, REFIID riid, void ** ppv );
	HRESULT CanUnloadNow( );
	HRESULT RegisterServer( BOOL bRegTypeLib = FALSE );
	HRESULT UnregisterServer( );

	//
	// Calls that need to be available for platforms that do 
	// not support COM or Registry.
	//

	// Return the number of coclasses exported from this dll.
	ULONG GetClassCount( );

	// Get the nth implementation coclass id.
	HRESULT GetClassObjectID(ULONG index, CLSID *pClassID);

	// Platform dependent file system information needed by the reference
	// implementation.
	const char* GetServerPath() const;
	const char* GetServerDirectory() const;

protected:
	long GetRegisterIndex(long index);

protected:
	AAFComObjectInfo_t *_pObjectInfo;
	HINSTANCE _hInstance;

	// Platform dependent file system information needed by the reference
	// implementation.
	char * _serverPath;
	char * _serverDirectory;

	// Private key data for object info array
	AAFComObjectInfo_t **_ppObjectInfoKey;
	size_t _objectCount;
};


#endif // __CAAFInProcServer_h__


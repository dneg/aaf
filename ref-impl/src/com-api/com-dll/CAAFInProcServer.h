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
#define AAF_OBJECT_ENTRYX(class,reg) { &CLSID_##class, OLESTR(#class), &C##class::COMCreate, reg },

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
	int GetRegisterIndex(int index);

protected:
	AAFComObjectInfo_t *_pObjectInfo;
	HINSTANCE _hInstance;

	// Platform dependent file system information needed by the reference
	// implementation.
	char * _serverPath;
	char * _serverDirectory;

	// Private key data for object info array
	AAFComObjectInfo_t **_ppObjectInfoKey;
	ULONG _objectCount;
};


#endif // __CAAFInProcServer_h__


// AAFShellExt.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AAFShellExt.h"
#include <initguid.h>
#include "AAFShellExt_i.c"
#include "AAFShellExtFolder.h"
#include "AAFShellExtView.h"
#include "AAFShellExtIDLIST.h"
#include "AAFShellExtIcon.h"
#include "AAFShellExtContextMenu.h"

#include "AAFShellExtDebug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtApp

BEGIN_MESSAGE_MAP(CAAFShellExtApp, CWinApp)
	//{{AFX_MSG_MAP(CAAFShellExtApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtApp construction

CAAFShellExtApp::CAAFShellExtApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	AAFShellExtLog( "CAAFShellExtApp::CAAFShellExtApp()\n" );
}

CAAFShellExtApp::~CAAFShellExtApp()
{
	AAFShellExtLog( "CAAFShellExtApp::~CAAFShellExtApp()\n" );
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CAAFShellExtApp object

CAAFShellExtApp theApp;

	
CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_AAFShellExtFolder, CAAFShellExtFolder)
OBJECT_ENTRY(CLSID_AAFShellExtView, CAAFShellExtView)
OBJECT_ENTRY(CLSID_AAFShellExtIDLIST, CAAFShellExtIDList)
OBJECT_ENTRY(CLSID_AAFShellExtIcon, CAAFShellExtIcon)
OBJECT_ENTRY(CLSID_AAFShellExtContextMenu, CAAFShellExtContextMenu)
END_OBJECT_MAP()

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount() == 0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}
/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	HRESULT hr = _Module.RegisterServer(TRUE);
    if ( SUCCEEDED( hr ) )
    {
        // Then we need to add our own binary data
        CString szVal;
        LPOLESTR lpolestr;
        if ( S_OK == StringFromCLSID( CLSID_AAFShellExtFolder, &lpolestr ) )
        {
            _bstr_t theVal = lpolestr;
            szVal.Format( _T("CLSID\\%s\\ShellFolder"), 
                static_cast<LPCTSTR>(theVal) );
            DWORD dwData = SFGAO_FOLDER | SFGAO_HASSUBFOLDER;
            SHSetValue( HKEY_CLASSES_ROOT, szVal, _T("Attributes"), REG_BINARY, &dwData, sizeof( dwData ) );
            CoTaskMemFree( lpolestr );
        }
    }
    return hr;
}
/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer(TRUE); //TRUE indicates that typelib is unreg'd
    // Then we need to add our own binary data
    CString szVal;
    LPOLESTR lpolestr;
    if ( S_OK == StringFromCLSID( CLSID_AAFShellExtFolder, &lpolestr ) )
    {
        _bstr_t theVal = lpolestr;
        szVal.Format( _T("CLSID\\%s\\ShellFolder"), 
            static_cast<LPCTSTR>(theVal) );
        SHDeleteValue( HKEY_CLASSES_ROOT, szVal, _T("Attributes") );
        CoTaskMemFree( lpolestr );
    }
	return S_OK;
}

BOOL CAAFShellExtApp::InitInstance()
{
	AAFShellExtLog( "CAAFShellExtApp::InitInstance()\n" );
	
	if (!InitATL())
		return FALSE;
    
    // If we load the icon from CAAFShellExtExtractIcon
    // we risk not being able to get the correct
    // icon if we develop more than one extension
    // using the library and wizard.
    m_iconOpen = reinterpret_cast<UINT>
        (AfxGetApp()->LoadIcon( IDI_FOLDEROPEN ));
    m_iconClosed = reinterpret_cast<UINT>
        (AfxGetApp()->LoadIcon( IDI_FOLDER ));

    return CWinApp::InitInstance();
}

int CAAFShellExtApp::ExitInstance()
{
	AAFShellExtLog( "CAAFShellExtApp::ExitInstance()\n" );

	_Module.Term();

	return CWinApp::ExitInstance();
}

BOOL CAAFShellExtApp::InitATL()
{
	_Module.Init(ObjectMap, AfxGetInstanceHandle());
	return TRUE;
}


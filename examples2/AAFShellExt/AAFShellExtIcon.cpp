// AAFShellExtIcon.cpp : Implementation of CAAFShellExtIcon
#include "stdafx.h"
#include "AAFShellExt.h"
#include "AAFShellExtIcon.h"
#include <PidlMgr.h>
/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtIcon

CAAFShellExtIcon::CAAFShellExtIcon() :
    m_type( PIDL_UNKNOWN )
{
}

void CAAFShellExtIcon::SetPIDL( LPITEMIDLIST pidl )
{
    CPidlMgr pidlMgr;
    AAFShellExtPidl* pData = NULL;
    LPITEMIDLIST lastPidl = pidlMgr.GetLastItem( pidl );
    if ( pidlMgr.GetDataPointer( lastPidl, &pData ) )
    {
        m_type = pData->eType;
    }
}


STDMETHODIMP CAAFShellExtIcon::Extract(LPCTSTR pszFile, 
    UINT nIconIndex, HICON *phiconLarge, 
    HICON *phiconSmall, UINT nIconSize)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // Load the large version
    *phiconLarge = reinterpret_cast<HICON>( nIconIndex );

    // Load the small version
    *phiconSmall = reinterpret_cast<HICON>( nIconIndex );
    return S_OK;
}

STDMETHODIMP CAAFShellExtIcon::GetIconLocation(UINT   uFlags,
    LPTSTR szIconFile, UINT cchMax,
    int *piIndex, UINT *pwFlags)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // Tell the caller that the icon is 
    // allocated on a per-folder basis.
    CAAFShellExtApp* pApp = 
        dynamic_cast<CAAFShellExtApp*>(AfxGetApp());

    *pwFlags |= GIL_NOTFILENAME;
    switch( m_type )
    {
    case PIDL_FOLDER:
        *piIndex = (( uFlags & GIL_OPENICON ) 
            ? pApp->OpenIcon() : pApp->ClosedIcon() );
        break; 
    case PIDL_DATAITEM:
        *piIndex = IDI_DATA;
        break;
    }
   
    return S_OK;
}

// AAFShellExtFolder.cpp : Implementation of CAAFShellExtFolder
#include "stdafx.h"
#include "AAFShellExt.h"
#include "AAFShellExtFolder.h"
#include "AAFShellExtView.h"
#include "AAFShellExtIDLIST.h"
#include "AAFShellExtPidl.h"
#include <PidlMgr.h>
#include "AAFShellExtIcon.h"
#include "AAFShellExtContextMenu.h"
#include "AAFShellExtDebug.h"

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtFolder

CAAFShellExtFolder::CAAFShellExtFolder()
: _bound(false)
{
	AAFShellExtLog( "CAAFShellExtFolder::CAAFShellExtFolder\n" );
}

STDMETHODIMP CAAFShellExtFolder::CreateViewObject( HWND hwndOwner, REFIID riid, void **ppv)
{
	AAFShellExtLog( "CreateViewObjects\n" );

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HRESULT hr = E_NOTIMPL;
    if ( IsEqualIID(IID_IShellView, riid ) )
    {
        CComObject<CAAFShellExtView> *pView = new CComObject<CAAFShellExtView>;
        *ppv = pView;
        pView->AddRef();
        pView->SetFolder( this, m_pidl );
        hr = S_OK;
    }
    return hr;
}

STDMETHODIMP CAAFShellExtFolder::EnumObjects( HWND hwnd, DWORD grfFlags, IEnumIDList **ppenumIDList)
{
	AAFShellExtLog( "EnumObjects\n" );

    CComObject<CAAFShellExtIDList> *pList = new CComObject<CAAFShellExtIDList>;
    pList->SetType( grfFlags );
    HRESULT hr = pList->QueryInterface( IID_IEnumIDList, (void**)ppenumIDList );


	// This must be called *before* SetShellFolder because SetShellFolder will
	// call through to CAAFShellExtIDLists::CreateIDLists().
	AAFShellExtLog( "EnumObjects - Create iterator\n" );
	AAFShellExtLog( "_initialPidl = \n" );
	DumpPidlDebugInfo( _initialPidl );
	AAFShellExtLog( "m_pidl = \n" );
	DumpPidlDebugInfo( m_pidl );

	LPCITEMIDLIST rootPidlCopy = m_pidlMgr.Concatenate( _initialPidl, GetFirstAAFPidl(m_pidl) );
	auto_ptr<AxShellExtIDList> idList(
		new AxShellExtIDList( rootPidlCopy ) );
	pList->SetAxShellExtIDList( idList );
	pList->SetBound( _bound );

    if ( !pList->SetShellFolder( this, m_pidl ) )
    {
        hr = S_FALSE;
    }

	return hr;
}

#if 0
STDMETHODIMP CAAFShellExtFolder::GetDisplayNameOf( LPCITEMIDLIST pidl, DWORD uFlags, LPSTRRET lpName)
{
	AAFShellExtLog( "GetDisplayNameOf:\n" );
	AAFShellExtLog( "\t CP ** " );
	AAFShellExtLog( CompletePathFromPidl( pidl ).c_str() );
	AAFShellExtLog( " **\n" );


    AAFShellExtPidl* pData = NULL;
    HRESULT hr = E_NOTIMPL;
    if ( m_pidlMgr.GetDataPointer( const_cast<LPITEMIDLIST>(pidl), &pData ) )
    {
        IMalloc* pMalloc = NULL;
        LPITEMIDLIST tempPidl = NULL;
        _bstr_t theString;
        if ( SUCCEEDED( SHGetMalloc( &pMalloc ) ) )
        {
            if ( uFlags & SHGDN_INFOLDER ) 
            {
                // This is an easy case.
                tempPidl = m_pidlMgr.GetLastItem( pidl );
                m_pidlMgr.GetDataPointer( tempPidl, & pData );
                theString = pData->pDisplayText;
            }
            else if ( ( uFlags & SHGDN_FORADDRESSBAR ) ||
                ( uFlags & SHGDN_FORPARSING ) )
            {
                _bstr_t tempStr;
                LPITEMIDLIST minorPidl = m_pidl;
                tempStr += pData->pDisplayText;

                while ( minorPidl->mkid.cb != 0 )
                {
                    // The root of the namespace
                    // is usually some unprintable
                    // string.  So, skip that part.
                    if ( minorPidl->mkid.cb >= 
                        sizeof( ITEMIDLIST ) + sizeof(AAFShellExtPidl))
                    {
                        if ( m_pidlMgr.GetDataPointer( minorPidl, &pData ) )
                        {
                            if ( theString.length() > 0 )
                            {
                                theString += _T("\\");
                            }
                            theString += pData->pDisplayText;
                        }
                    }
                    minorPidl = m_pidlMgr.GetNextItem( minorPidl );
                }

                theString += _T("\\");
                theString += tempStr;
            }
            lpName->uType = STRRET_WSTR;
            if ( theString.length() > 0 )
            {
                lpName->pOleStr = reinterpret_cast<LPWSTR>
                    (pMalloc->Alloc( (theString.length() + 1) *
                    sizeof( wchar_t )));
                wcscpy(lpName->pOleStr, theString);
                hr = S_OK;
            }		
		}
    }


    
	return hr;
}
#else

// MODIFIED VERSION
STDMETHODIMP CAAFShellExtFolder::GetDisplayNameOf( LPCITEMIDLIST pidl, DWORD uFlags, LPSTRRET lpName)
{
	AAFShellExtLog( "GetDisplayNameOf:\n" );
	DumpPidlDebugInfo( pidl );
	AAFShellExtLog( "\t CP **" );
	AAFShellExtLog( CompletePathFromPidl( pidl ).c_str() );
	AAFShellExtLog( " **\n" );


    AAFShellExtPidl* pData = NULL;
    HRESULT hr = E_NOTIMPL;
    if ( m_pidlMgr.GetDataPointer( const_cast<LPITEMIDLIST>(pidl), &pData ) )
    {
        IMalloc* pMalloc = NULL;
        LPITEMIDLIST tempPidl = NULL;
        _bstr_t theString;
        if ( SUCCEEDED( SHGetMalloc( &pMalloc ) ) )
        {
            if ( uFlags & SHGDN_INFOLDER ) 
            {
                // This is an easy case.
                tempPidl = m_pidlMgr.GetLastItem( pidl );
                m_pidlMgr.GetDataPointer( tempPidl, & pData );
                theString = pData->pDisplayText;
            }
            else if ( ( uFlags & SHGDN_FORADDRESSBAR ) ||
                ( uFlags & SHGDN_FORPARSING ) )
            {
				theString = CompletePathFromPidl( pidl ).c_str();
            }

            lpName->uType = STRRET_WSTR;
            if ( theString.length() > 0 )
            {
                lpName->pOleStr = reinterpret_cast<LPWSTR>
                    (pMalloc->Alloc( (theString.length() + 1) *
                    sizeof( wchar_t )));
                wcscpy(lpName->pOleStr, theString);
                hr = S_OK;
            }		
		}
    }


    
	return hr;
}
#endif

#if 0
STDMETHODIMP CAAFShellExtFolder::GetAttributesOf( UINT cidl, LPCITEMIDLIST * apidl, ULONG * rgfInOut)
{
	AAFShellExtLog( "GetAttributesOf\n" );

	DumpPidlDebugInfo( *apidl );
	AAFShellExtLog( "\t CP: **" );
	AAFShellExtLog( CompletePathFromPidl( *apidl ).c_str() );
	AAFShellExtLog( " **\n" );


    AAFShellExtPidl* pData = NULL;
    ASSERT( cidl == 1 );
    if ( m_pidlMgr.GetDataPointer( const_cast<LPITEMIDLIST>(*apidl), &pData ) )
    {
        switch ( pData->eType )
        {
        case PIDL_FOLDER:
            *rgfInOut =  SFGAO_HASSUBFOLDER | SFGAO_FOLDER;
            break;
        case PIDL_DATAITEM:
			*rgfInOut = 0;
            break;
        default:
             break;
        }

    }
    return S_OK;
}
#else
// USE THIS ONE!!! 
STDMETHODIMP CAAFShellExtFolder::GetAttributesOf( UINT cidl, LPCITEMIDLIST * apidl, ULONG * rgfInOut)
{
	AAFShellExtLog( "GetAttributesOf\n" );

	DumpPidlDebugInfo( *apidl );
	AAFShellExtLog( "\t CP: **" );
	AAFShellExtLog( CompletePathFromPidl( *apidl ).c_str() );
	AAFShellExtLog( " **\n" );


    AAFShellExtPidl* pData = NULL;
    ASSERT( cidl == 1 );
	
	LPITEMIDLIST lastPidl = m_pidlMgr.GetLastItem( *apidl );

	// Make sure we know what it is.
    if ( m_pidlMgr.GetDataPointer( const_cast<LPITEMIDLIST>(*apidl), &pData ) &&
		 memcmp( &pData->id, &AAFSHELL_PIDL_ID, sizeof(GUID) ) == 0 ){

		switch ( pData->eType )
			{
			case PIDL_FOLDER:
	            *rgfInOut =  SFGAO_HASSUBFOLDER | SFGAO_FOLDER;
				break;
			case PIDL_DATAITEM:
				*rgfInOut = 0;
	            break;
			default:
				*rgfInOut = 0;
				break;
			}

	    return S_OK;
    }
	else {
		return S_FALSE;
	}
}
#endif

STDMETHODIMP CAAFShellExtFolder::CompareIDs( LPARAM lParam, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	AAFShellExtLog( "CompareIDs\n" );

    AAFShellExtPidl* pData1 = NULL;
    AAFShellExtPidl* pData2 = NULL;
    HRESULT hr = 0;
    if ( !(m_pidlMgr.GetDataPointer( const_cast<LPITEMIDLIST>(pidl1), &pData1 ) &&
          m_pidlMgr.GetDataPointer( const_cast<LPITEMIDLIST>(pidl2), &pData2 ) ) )
    {
        return 0;
    }
    
    hr = _tcscmp( pData1->pDisplayText, pData2->pDisplayText );
    return hr;
}

STDMETHODIMP CAAFShellExtFolder::GetUIObjectOf( HWND hwndOwner, UINT cidl, LPCITEMIDLIST * apidl,
                                            REFIID riid, UINT * prgfInOut, void **ppv)
{
	AAFShellExtLog( "GetUIObjectOf\n" );
    
	if(IsEqualIID(riid, IID_IExtractIcon))
    {
        CComObject<CAAFShellExtIcon> *pei = new CComObject<CAAFShellExtIcon>;
        if ( !pei )
        {
            return E_OUTOFMEMORY;
        }
        LPITEMIDLIST   pidl;
        
        if ( m_pidl )
        {
            pidl = m_pidlMgr.Concatenate(m_pidl, apidl[0]);
        }
        else
        {
            pidl = const_cast<LPITEMIDLIST>(apidl[0]);
        }
        pei->SetPIDL( pidl );        
        
        //The temp PIDL can be deleted because the new CAAFShellExtIcon either failed or 
        //made its own copy of it.
        
        m_pidlMgr.Delete(pidl);
        return pei->QueryInterface( riid, ppv );
    }
    else if (IsEqualIID(riid, IID_IContextMenu))
    {
        CComObject<CAAFShellExtContextMenu> *pcm = new CComObject<CAAFShellExtContextMenu>;
        LPITEMIDLIST pidl = NULL;
        if ( !pcm )
        {
            return E_OUTOFMEMORY;
        }
        if ( m_pidl )
        {
            pidl = m_pidlMgr.Concatenate( m_pidl, apidl[0] );
        }
        else
        {
            pidl = const_cast<LPITEMIDLIST>(apidl[0]);
        }
        pcm->SetPIDL( pidl, this );
        //The temp PIDL can be deleted because the new CAAFShellExtContextMenu either failed or 
        //made its own copy of it.
        m_pidlMgr.Delete(pidl);
        return pcm->QueryInterface( riid, ppv );
    }
    return E_NOINTERFACE;
}

STDMETHODIMP CAAFShellExtFolder::Initialize(LPCITEMIDLIST pidl)
{
	AAFShellExtLog( "Initialize()\n" );

 	m_pidl = m_pidlMgr.Copy( pidl );

	_initialPidl = m_pidlMgr.Copy( pidl );
	
	return S_OK;
}

void CAAFShellExtFolder::FinalRelease()
{
 	AAFShellExtLog( "FinalRelease()\n" );

	m_pidlMgr.Delete( m_pidl );
    m_pidl = NULL;

	m_pidlMgr.Delete( _initialPidl );
    _initialPidl = NULL;

}


STDMETHODIMP CAAFShellExtFolder::BindToObject( LPCITEMIDLIST pidl, LPBC pbc, REFIID riid, void **ppv)
{
	AAFShellExtLog( "BindToObject\n" );
    
 	if ( !IsEqualIID(riid, IID_IShellFolder ) )
    {
        return E_NOINTERFACE;
    }

	AAFShellExtLog( "m_pidl = " );
	DumpPidlDebugInfo(m_pidl);
	AAFShellExtLog( "\npidl = " );
	DumpPidlDebugInfo(pidl);
	AAFShellExtLog( "\n" );

	LPITEMIDLIST newPidl = m_pidlMgr.Concatenate( m_pidl, pidl );

	DumpPidlDebugInfo( newPidl );

	AxString testFileName;

	vector<AxString> objPath;
	if ( !ObjectPathFromRootPidl( newPidl, objPath ) ) {
		m_pidlMgr.Delete( newPidl );
		AAFShellExtLog( "\tCan't bind.  Unable to get an object path newPidl\n" );
		return E_NOINTERFACE;
	}

	CComObject<CAAFShellExtFolder>* pnewFolder = new CComObject<CAAFShellExtFolder>;

    // BindToObject gets called on the folder at the root.
    // By doing this, we can properly execute Open and Explore commands
    //    pnewFolder->m_pidl = m_pidlMgr.Concatenate( m_pidl, pidl );  // Original
    pnewFolder->m_pidl = newPidl;
	pnewFolder->_initialPidl = m_pidlMgr.Copy( _initialPidl );
    pnewFolder->SetBound(true);

	HRESULT hr = pnewFolder->QueryInterface( riid, ppv );

	if ( SUCCEEDED(hr) ) {
		AAFShellExtLog( "BindToObject returning new interface for: \n\t" );
		AAFShellExtLog( CompletePathFromPidl( 
			m_pidlMgr.Concatenate( _initialPidl, GetFirstAAFPidl(newPidl) ) ).c_str() );
		AAFShellExtLog( "\n" );
	} 
	
   return hr;
}


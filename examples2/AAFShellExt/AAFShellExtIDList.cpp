// AAFShellExtIDLIST.cpp : Implementation of CAAFShellExtIDList
#include "stdafx.h"
#include "AAFShellExt.h"
#include "AAFShellExtIDLIST.h"
#include "AAFShellExtFolder.h"

#include "AAFShellExtDebug.h"

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtIDList
CAAFShellExtIDList::CAAFShellExtIDList() :
    m_pFolder( NULL ),
    m_currentIndex( 0 ),
    m_grfFlags( 0 ),
	_bound( false )
{
}

bool CAAFShellExtIDList::SetShellFolder(IShellFolder* pFolder, 
    LPCITEMIDLIST pItemIDList)
{
	AAFShellExtLog( "CAAFShellExtIDList::SetShellFolder()\n" );

    bool retval = true;

    m_pFolder = pFolder;
    m_pFolderPidl = m_pidlMgr.Copy( pItemIDList );
    retval = CreateIDList();
    m_currentIndex = m_lstPidl.begin();
    return retval;
}

STDMETHODIMP CAAFShellExtIDList::Clone( IEnumIDList **ppEnum )
{
	AAFShellExtLog( "CAAFShellExtIDList::Clone()\n" );

	CComObject<CAAFShellExtIDList> *pList = new CComObject<CAAFShellExtIDList>;
    HRESULT hr = QueryInterface(IID_IAAFShellExtIDLIST, (void**)ppEnum);

    if ( pList == NULL )
    {
        return S_FALSE;
    }
    else 
    {
        pList->SetShellFolder( m_pFolder, m_pFolderPidl );
        pList->SetType( m_grfFlags );
    }

    return S_OK;
}

STDMETHODIMP CAAFShellExtIDList::Reset()
{
	AAFShellExtLog( "CAAFShellExtIDList::Reset()\n" );

    m_currentIndex = m_lstPidl.begin();
    return S_OK;
}

STDMETHODIMP CAAFShellExtIDList::Skip( ULONG celt )
{
	AAFShellExtLog( "CAAFShellExtIDList::Skip()\n" );

    for ( int i = 0; ( i < celt ) && 
        ( m_currentIndex != m_lstPidl.end() ); 
        ++m_currentIndex, ++i );
    return S_OK;
}

void CAAFShellExtIDList::FinalRelease()
{
    if ( m_pFolder )
    {
        for ( AAFShellExtListType::iterator it = m_lstPidl.begin();
        it != m_lstPidl.end(); ++it )
        {
            m_pidlMgr.Delete( *it );
        }
    }
    CComObjectRootEx<CComSingleThreadModel>::FinalRelease();
}

STDMETHODIMP CAAFShellExtIDList::Next( ULONG celt, LPITEMIDLIST *rgelt, 
    ULONG *pceltFetched )
{
	AAFShellExtLog( "CAAFShellExtIDList::Next()\n" );

    if ( m_currentIndex == m_lstPidl.end() )
    {
        *pceltFetched = 0;
		AAFShellExtLog( "\tS_FALSE\n" );
        return S_FALSE;
    }
    for ( int i = 0; (i < celt) && ( m_currentIndex != m_lstPidl.end() ); 
        ++i, ++m_currentIndex )
    {
        rgelt[i] = m_pidlMgr.Copy( *m_currentIndex );
    }
    *pceltFetched = i;
		AAFShellExtLog( "\tS_OK\n" );
    return S_OK;
}


void CAAFShellExtIDList::SetType(DWORD grfFlags)
{
    m_grfFlags = grfFlags;
}

bool CAAFShellExtIDList::CreateIDList()
{
	AAFShellExtLog( "CAAFShellExtIDList::CreateIDList()\n" );
	
#if 0

		// TODO: You will probably delete most of this
    // code and start from scratch.  This does
    // get something displaying in the view immediately.
    IMalloc* pMalloc = NULL;
    LPITEMIDLIST pidl = NULL;
    AAFShellExtPidl* pData = NULL;
    if ( FAILED(SHGetMalloc( &pMalloc ) ) )
    {
        return false;
    }
    USHORT theSize = sizeof(ITEMIDLIST) + sizeof(AAFShellExtPidl);

    // Include folder items
    if (m_grfFlags & SHCONTF_FOLDERS )
    {
        // The extra ITEMIDLIST is to NULL terminate the 
        // list.
        pidl = reinterpret_cast<LPITEMIDLIST>
            (pMalloc->Alloc( theSize + sizeof(ITEMIDLIST) ));
        ZeroMemory( pidl, theSize + sizeof(ITEMIDLIST) );
    
        if ( m_pidlMgr.GetDataPointer( pidl, &pData ) )
        {
            pData->eType = PIDL_FOLDER;
            _tcscpy( pData->pDisplayText, _T("Folder") );
            pidl->mkid.cb = theSize;
        }
        m_lstPidl.insert( m_lstPidl.end(), pidl );
    }

    // Include non-folder items
    if ( m_grfFlags & SHCONTF_NONFOLDERS )
    {
        pidl = reinterpret_cast<LPITEMIDLIST>
            (pMalloc->Alloc( theSize + sizeof(ITEMIDLIST) ));
        ZeroMemory( pidl, theSize + sizeof(ITEMIDLIST) );
        pData = NULL;
        if ( m_pidlMgr.GetDataPointer( pidl, &pData ) )
        {
            pData->eType = PIDL_DATAITEM;
            _tcscpy( pData->pDisplayText, _T("Data item") );
            pidl->mkid.cb = theSize;
        }
        m_lstPidl.insert( m_lstPidl.end(), pidl );
    }

	return retVal;
#else

	AAFShellExtLog( "\tGetList For: " );
	AAFShellExtLog( CompletePathFromPidl( m_pFolderPidl ).c_str() );
	AAFShellExtLog( "\n" );

	IMalloc *pMalloc = NULL;
    LPITEMIDLIST pidl = NULL;
    AAFShellExtPidl* pData = NULL;
    if ( FAILED(SHGetMalloc( &pMalloc ) ) )
    {
        return false;
    }
    USHORT theSize = sizeof(ITEMIDLIST) + sizeof(AAFShellExtPidl);

	bool notAtEnd;
	AxString nextDesc( L"" );
	bool     nextIsFolder = false;


	_axShellExtIDList->Initialize();

	bool retVal = false;

	int lid;

	for( notAtEnd = _axShellExtIDList->Next( nextDesc, nextIsFolder, lid );
		 notAtEnd;
		 notAtEnd = _axShellExtIDList->Next( nextDesc, nextIsFolder, lid ) ) {

		 AAFShellExtLog( nextIsFolder ? L"NextIsFolder\n" : L"NextIsNotFolder\n" );

	   // Include folder items
		if ( (m_grfFlags & SHCONTF_FOLDERS) && nextIsFolder)
		{
			// The extra ITEMIDLIST is to NULL terminate the 
			// list.
			pidl = reinterpret_cast<LPITEMIDLIST>
			 (pMalloc->Alloc( theSize + sizeof(ITEMIDLIST) ));
			ZeroMemory( pidl, theSize + sizeof(ITEMIDLIST) );

			if ( m_pidlMgr.GetDataPointer( pidl, &pData ) )
			{
				pData->id = AAFSHELL_PIDL_ID;
				pData->lid = lid;
				pData->eType = PIDL_FOLDER;
		        _tcscpy( pData->pDisplayText, nextDesc.c_str() );
				pidl->mkid.cb = theSize;

				// In CAAFShellExtListView::OnDblclk we need an absolute pidl,
				// Save it now.
				// FIXME !  Saving a pointer in a pidl is not a good idea.  It
				// cannot be made persistant.  Find a better way to do this.
				pData->absPidl = m_pidlMgr.Concatenate( m_pFolderPidl, pidl );			
			}

			// *Always* provide a pidl relative to current location. (i.e. *Don't*
			//	append it to m_pFolderPidl.)
			m_lstPidl.insert( m_lstPidl.end(), pidl );

			retVal = true;

			AAFShellExtLog( "\tPidl insert: " );
			AAFShellExtLog( CompletePathFromPidl( m_pidlMgr.Concatenate( m_pFolderPidl, pidl ) ).c_str() );
			AAFShellExtLog( "\n" );

			AAFShellExtLog( L"\tInsert folder: " );
			AAFShellExtLog( ((AAFShellExtPidl*)pidl->mkid.abID)->pDisplayText );
			AAFShellExtLog( L"\n" );
		}

		else if ( (m_grfFlags & SHCONTF_NONFOLDERS) && !nextIsFolder )
		{
			pidl = reinterpret_cast<LPITEMIDLIST>
				(pMalloc->Alloc( theSize + sizeof(ITEMIDLIST) ));
			ZeroMemory( pidl, theSize + sizeof(ITEMIDLIST) );

			if ( m_pidlMgr.GetDataPointer( pidl, &pData ) )
			{
				pData->id = AAFSHELL_PIDL_ID;
				pData->lid = lid;
				pData->eType = PIDL_DATAITEM;
		        _tcscpy( pData->pDisplayText, nextDesc.c_str() );
				pidl->mkid.cb = theSize;

				pData->absPidl = m_pidlMgr.Concatenate( m_pFolderPidl, pidl );
			}

			m_lstPidl.insert( m_lstPidl.end(), pidl );

			retVal = true;

			AAFShellExtLog( L"\tInsert nonfolder: " );
			AAFShellExtLog( nextDesc.c_str() );
			AAFShellExtLog( L"\n" );
		}
		else {
			pidl = reinterpret_cast<LPITEMIDLIST>
				(pMalloc->Alloc( theSize + sizeof(ITEMIDLIST) ));
			ZeroMemory( pidl, theSize + sizeof(ITEMIDLIST) );

			if ( m_pidlMgr.GetDataPointer( pidl, &pData ) )
			{
				pData->id = AAFSHELL_PIDL_ID;
				pData->lid = lid;
				pData->eType = PIDL_UNKNOWN;
		        _tcscpy( pData->pDisplayText, nextDesc.c_str() );
				pidl->mkid.cb = theSize;

				pData->absPidl = m_pidlMgr.Concatenate( m_pFolderPidl, pidl );			
			}

			m_lstPidl.insert( m_lstPidl.end(), pidl );

			AAFShellExtLog( "\tInsert UNKOWN\n");
			retVal = true;
		}
	}

 	_axShellExtIDList->Finalize();

	return retVal;
#endif
}

void CAAFShellExtIDList::SetAxShellExtIDList( std::auto_ptr<AxShellExtIDList> idList )
{
	AAFShellExtLog( "CAAFShellExtIDList::SetAxShellExtIDList to " );

	if ( idList.get() ) {
		AAFShellExtLog( "non null\n" );
	}
	else {
		AAFShellExtLog( "null\n" );
	}

	_axShellExtIDList = idList;
}

void CAAFShellExtIDList::SetBound( bool bound )
{
	_bound = bound;
}


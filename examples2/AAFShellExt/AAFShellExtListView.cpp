// AAFShellExtListView.cpp : implementation file
//

#include "stdafx.h"
#include "AAFShellExt.h"
#include "AAFShellExtFolder.h"
#include "AAFShellExtListView.h"
#include "AAFShellExtPidl.h"
#include "AboutDlg.h"
#include <PidlMgr.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "AAFShellExtDebug.h"
#include "AxShellExt.h"

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtListView
const UINT CAAFShellExtListView::m_iconIDs[] = {
    IDI_FOLDER,
    IDI_DATA,
    0
};

// Initial style for the list control.
static const DWORD KdwBasicStyle = WS_CLIPCHILDREN|WS_CLIPSIBLINGS|
    WS_HSCROLL|WS_TABSTOP|WS_CHILDWINDOW|LVS_EDITLABELS|
    WS_VISIBLE|WS_BORDER|LVS_SHOWSELALWAYS ;

// Uniquely identifies the list.
static const int KnListID = 2;

enum IconIDs { FOLDER, DATAITEM };

CAAFShellExtListView::CAAFShellExtListView() :
    CommandHandler<CWnd>( IDR_TBEXTENSION,
        IDR_EXTENSION )
{
}

CAAFShellExtListView::~CAAFShellExtListView()
{
}


// The CAAFShellExtListView class does not actually
// implement the ON_WM_MENUSELECT or ON_WM_INITMENUPOPUP
// handler.  Instead these handlers are implemented
// by CommandHandler<T>.
BEGIN_MESSAGE_MAP(CAAFShellExtListView, CWnd)
	//{{AFX_MSG_MAP(CAAFShellExtListView)
	ON_WM_SIZE()
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THEDETAILS, OnUpdateViewDetails)
	ON_COMMAND(ID_VIEW_THEDETAILS, OnViewDetails)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THELARGEICONS, OnUpdateViewLargeicons)
	ON_COMMAND(ID_VIEW_THELARGEICONS, OnViewLargeicons)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THELIST, OnUpdateViewList)
	ON_COMMAND(ID_VIEW_THELIST, OnViewList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THESMALLICONS, OnUpdateViewSmallicons)
	ON_COMMAND(ID_VIEW_THESMALLICONS, OnViewSmallicons)
	ON_WM_MENUSELECT()
    ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, KnListID, OnDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtListView message handlers

bool CAAFShellExtListView::Init(LPSV2CVW2_PARAMS lpParams)
{
    bool retval = true;
    HWND browserWnd;
    HRESULT hr = lpParams->psbOwner->GetWindow( &browserWnd );
    if ( FAILED( hr ) )
    {
        retval = false;
        return retval;
    }
    CWnd theBrowser;
    theBrowser.Attach( browserWnd );
    DWORD dwStyle = KdwBasicStyle;
    switch ( lpParams->pfs->ViewMode )
    {
        case FVM_SMALLICON:
            dwStyle |= LVS_SMALLICON;
            break;
        case FVM_LIST:
            dwStyle |= LVS_LIST;
            break;
        case FVM_DETAILS:
            dwStyle |= LVS_REPORT;
            break;
        case FVM_ICON:
        default:
            dwStyle |= LVS_ICON;
            break;
    }

    CRect theRect = *lpParams->prcView;
    if ( !Create( NULL, NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP
        | WS_CLIPCHILDREN, theRect, &theBrowser, 1 ) )
    {
        return false;
    }
    else if ( !m_list.Create( dwStyle, CRect( 0, 0, 
        theRect.Width(), theRect.Height() ), this, KnListID ) )
    {
        retval = false;
    }
    theBrowser.Detach();

    if ( FVM_DETAILS == lpParams->pfs->ViewMode )
    {
        SetupReportView();
    }

    if ( NULL == m_smallImageList.m_hImageList )
    {
        // only initialize once.  If we are instantiated in 
        // multiple apartments, this code will save
        // no memory since it will get called several times.
        m_smallImageList.Create( 16, 16, 0, 1, 1 );
        m_largeImageList.Create( 32, 32, 0, 1, 1 );
        m_largeImageList.SetBkColor( GetSysColor( COLOR_WINDOW ));
        m_smallImageList.SetBkColor( GetSysColor( COLOR_WINDOW ));
        for ( int i = 0; m_iconIDs[i] != 0; ++i )
        {
            m_smallImageList.Add( AfxGetApp()->LoadIcon( m_iconIDs[i] ) );
            m_largeImageList.Add( AfxGetApp()->LoadIcon( m_iconIDs[i] ) );
        }
        if ( lpParams->pfs->ViewMode == FVM_ICON )
        {
            m_list.SetImageList( &m_largeImageList, LVSIL_NORMAL );
        }
        else
        {
            m_list.SetImageList( &m_smallImageList,LVSIL_SMALL );
        }
    }
    if ( lpParams->pfs->ViewMode == FVM_ICON )
    {
        m_list.SetImageList( &m_largeImageList, LVSIL_NORMAL );
    }
    else
    {
        m_list.SetImageList( &m_smallImageList,LVSIL_SMALL );
    }

    retval = CommandHandler<CWnd>::Init( lpParams );
    return retval;
}



void CAAFShellExtListView::Refresh()
{
    // Clear the list
    m_list.DeleteAllItems();
    RemoveCachedPIDLs();
    // Enumerate the items;
    CComPtr<IEnumIDList> pEnum = NULL;
    m_pFolder->EnumObjects( GetSafeHwnd(), SHCONTF_FOLDERS | SHCONTF_NONFOLDERS,
        &pEnum );
    CPidlMgr pidlMgr;
    if ( pEnum )
    {
        ULONG numReturned = 0;
        CPidlMgr pidlMgr;
        LPITEMIDLIST pIDList = NULL;
        STRRET strRet;
        pEnum->Next( 1, &pIDList, &numReturned );
        int i = 0;
        AAFShellExtPidl* pData = NULL;
        UINT iconIndex = 0;
        int newIndex = 0;
        CString szData;
        while ( numReturned > 0 )
        {
            m_pFolder->GetDisplayNameOf( pIDList, 
                SHGDN_INFOLDER, &strRet );
            szData = strRet.pOleStr;
            if ( pidlMgr.GetDataPointer( pIDList, &pData ) )
            {
                if ( pData->eType == PIDL_DATAITEM )
                {
                    iconIndex = DATAITEM;
                }
                else
                {
                    iconIndex = FOLDER;
                }
            }
            newIndex = m_list.InsertItem( i, szData, iconIndex );
            m_list.SetItemData( newIndex, 
                reinterpret_cast<DWORD>(pIDList) );
            ++i;
            pEnum->Next( 1, &pIDList, &numReturned );
        }
    }
}

void CAAFShellExtListView::SetupReportView()
{
    int nColumnCount = m_list.GetHeaderCtrl()->GetItemCount();

    // Delete all of the columns.
    for (int i=0; i < nColumnCount; i++)
    {
       m_list.DeleteColumn(0);
    }
    m_list.InsertColumn( 0, _T("Name"), LVCFMT_LEFT, 150, 0 );
}

BOOL CAAFShellExtListView::DestroyWindow() 
{
    RemoveCachedPIDLs();
    m_smallImageList.DeleteImageList();
    m_largeImageList.DeleteImageList();
	return CommandHandler<CWnd>::DestroyWindow();
}

void CAAFShellExtListView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
    if ( m_list.GetSafeHwnd() )
    {
	    m_list.MoveWindow( CRect( 0, 0, cx, cy ) );
    }
    Invalidate();	
}

void CAAFShellExtListView::RemoveCachedPIDLs()
{
    int count = m_list.GetItemCount();
    CPidlMgr pidlMgr;
    LPITEMIDLIST pidl = NULL;
    for ( int i = 0; i < count; ++i )
    {
        pidl = reinterpret_cast<LPITEMIDLIST>( m_list.GetItemData( i ) );
        pidlMgr.Delete( pidl );
    }
}


void CAAFShellExtListView::OnHelpAbout() 
{
    CAboutDlg(this).DoModal();	
}

void CAAFShellExtListView::OnUpdateViewDetails(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio( GetViewMode() == FVM_DETAILS );
}

void CAAFShellExtListView::OnViewDetails() 
{
    DWORD dwStyle = KdwBasicStyle | LVS_REPORT;
    m_list.ModifyStyle( 0xFFFFFFFF, dwStyle, SWP_SHOWWINDOW );
    m_list.SetImageList( &m_smallImageList,LVSIL_SMALL );
    SetupReportView();
    Refresh();
}

void CAAFShellExtListView::OnUpdateViewLargeicons(CCmdUI* pCmdUI) 
{
    pCmdUI->SetRadio( GetViewMode() == FVM_ICON );
}

void CAAFShellExtListView::OnViewLargeicons() 
{
    DWORD dwStyle = KdwBasicStyle | LVS_ICON;
    m_list.ModifyStyle( 0xFFFFFFFF, dwStyle, SWP_SHOWWINDOW );
    m_list.SetImageList( &m_largeImageList,LVSIL_NORMAL );
    Refresh();
}

void CAAFShellExtListView::OnUpdateViewList(CCmdUI* pCmdUI) 
{
    pCmdUI->SetRadio( GetViewMode() == FVM_LIST );
}

void CAAFShellExtListView::OnViewList() 
{
    DWORD dwStyle = KdwBasicStyle | LVS_LIST;
    m_list.ModifyStyle( 0xFFFFFFFF, dwStyle, SWP_SHOWWINDOW );
    m_list.SetImageList( &m_smallImageList,LVSIL_SMALL );
    Refresh();
}

void CAAFShellExtListView::OnUpdateViewSmallicons(CCmdUI* pCmdUI) 
{
    pCmdUI->SetRadio( GetViewMode() == FVM_SMALLICON );
}

void CAAFShellExtListView::OnViewSmallicons() 
{
    DWORD dwStyle = KdwBasicStyle | LVS_SMALLICON;
    m_list.ModifyStyle( 0xFFFFFFFF, dwStyle, SWP_SHOWWINDOW );
    m_list.SetImageList( &m_smallImageList,LVSIL_SMALL );
    Refresh();
}

FOLDERVIEWMODE CAAFShellExtListView::GetViewMode()
{
    DWORD dwStyle = m_list.GetStyle() & 0x03;

    // Eliminate one test.  If none of the other styles
    // check out, it must be a detail view.
    FOLDERVIEWMODE viewMode = FVM_DETAILS;
    if ( dwStyle == LVS_SMALLICON )
    {
        viewMode = FVM_SMALLICON;
    }
    else if ( dwStyle == LVS_LIST )
    {
        viewMode = FVM_LIST;
    }
    else if ( dwStyle == LVS_ICON )
    {
        viewMode = FVM_ICON;
    }
    return viewMode;
}

void CAAFShellExtListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
    int item = m_list.GetSelectionMark();
    LPITEMIDLIST pidl = reinterpret_cast<LPITEMIDLIST>(m_list.GetItemData(item));
    CPidlMgr pidlMgr;
    AAFShellExtPidl *pData = NULL;
    if ( pidlMgr.GetDataPointer( pidl, &pData ) )
    {
		AAFShellExtLog( "CAAFShellExtListView::OnDblclk:\n" );
	    DumpPidlDebugInfo( pidl );
		AAFShellExtLog( "CAAFShellExtListView::OnDblclk dump end\n" );

		m_psBrowser->BrowseObject(
			GetAbsolutePidl(pidl), SBSP_DEFBROWSER | SBSP_DEFMODE | SBSP_ABSOLUTE );
	}

	*pResult = 0;
}

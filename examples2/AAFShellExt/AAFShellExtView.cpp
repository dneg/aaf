// AAFShellExtView.cpp : Implementation of CAAFShellExtView
#include "stdafx.h"
#include "AAFShellExt.h"
#include "AAFShellExtView.h"

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtView

CAAFShellExtView::CAAFShellExtView()
{
}

STDMETHODIMP CAAFShellExtView::GetView( SHELLVIEWID *pvid, ULONG uView )
{
    HRESULT hr = S_OK;

    // We are the default view.  Unless you create
    // more views, this return false for any other
    // requests.
    if ( uView == SV2GV_DEFAULTVIEW )
    {
        *pvid = GetObjectCLSID();
    }
    else
    {
        hr = S_FALSE;
    }
    return hr;
}

STDMETHODIMP CAAFShellExtView::CreateViewWindow2( LPSV2CVW2_PARAMS lpParams)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // Defer creation to the list control
    return m_theView.Init( lpParams ) ? S_OK : E_NOTIMPL;
}

STDMETHODIMP CAAFShellExtView::UIActivate( UINT uState)
{
    HRESULT hr = S_OK;

    // Pass activation to the list control.
    if ( !m_theView.Activate( uState ) )
    {
        hr = E_FAIL;
    }
    return hr;
}

STDMETHODIMP CAAFShellExtView::DestroyViewWindow()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HRESULT hr = S_OK;

    // Get rid of the view.
    if ( !m_theView.DestroyWindow() )
    {
        hr = S_FALSE;
    }
    return hr;
}

STDMETHODIMP CAAFShellExtView::GetItemObject( UINT uItem, REFIID riid,
                void **ppv)
{
    HRESULT hr = E_NOINTERFACE;
    
    switch ( uItem )
    {
    case SVGIO_BACKGROUND:
        break;
    case SVGIO_SELECTION:
        break;
    case SVGIO_ALLVIEW:
        break;
    default: 
        hr = S_FALSE;
        break;
    }
    return hr;
}


void CAAFShellExtView::SetFolder(IShellFolder *pFolder, LPITEMIDLIST pidl)
{
    // Only the view itself really cares.  This COM
    // object just passes messages to and from the
    // MFC world.
    m_theView.SetFolder( pFolder, pidl );
}

STDMETHODIMP CAAFShellExtView::GetWindow( HWND * lphwnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    *lphwnd = m_theView.GetSafeHwnd();
    return S_OK;
}

STDMETHODIMP CAAFShellExtView::Refresh()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    m_theView.Refresh();
    return S_OK;
}

STDMETHODIMP CAAFShellExtView::GetCurrentInfo( LPFOLDERSETTINGS lpfs)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    lpfs->ViewMode = m_theView.GetViewMode();

    lpfs->fFlags = m_folderFlags;
    return S_OK;
}

STDMETHODIMP CAAFShellExtView::SaveViewState()
{
    // TODO: If you have any state information that
    // you want to persist for this view, now is 
    // the time to do it.
    return S_OK;
}

#if 1
// JPT Added  
STDMETHODIMP CShellViewImpl::CreateViewWindow( IShellView  *lpPrevView,
                    LPCFOLDERSETTINGS lpfs, IShellBrowser  * psb,
                    RECT * prcView, HWND  *phWnd)
{
	SV2CVW2_PARAMS params;

	params.cbSize = sizeof( params );
	params.hwndView = *phWnd;
	params.pfs = lpfs;
	params.psbOwner = psb;
	params.prcView = prcView;
	params.psvPrev = lpPrevView;

	// Hmmm ... this is not in the book (page 313)
	params.pvid = 0;

	HRESULT hr = CreateViewWindow2( &params );
	*phWnd = params.hwndView;
	m_folderFlags = lpfs->fFlags;
	return hr;

}
#endif
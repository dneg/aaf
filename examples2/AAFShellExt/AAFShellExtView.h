//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

// AAFShellExtView.h : Declaration of the CAAFShellExtView

#ifndef AAFSHELLEXTVIEW_H
#define AAFSHELLEXTVIEW_H

#include "resource.h"       // main symbols
#ifndef SHELLVIEWIMPL_H
    #include <ShellViewImpl.h>
#endif // SHELLVIEWIMPL_H
#ifndef OLECOMMANDTARGETIMPL_H
    #include <OleCommandTargetImpl.h>
#endif // OLECOMMANDTARGETIMPL_H
#include "AAFShellExtListView.h"

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtView
class ATL_NO_VTABLE CAAFShellExtView : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAAFShellExtView, &CLSID_AAFShellExtView>,
    public CShellViewImpl,
    public COleCommandTargetImpl,
	public IAAFShellExtView
{
public:
	CAAFShellExtView();

DECLARE_REGISTRY_RESOURCEID(IDR_AAFSHELLEXTVIEW)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAAFShellExtView)
	COM_INTERFACE_ENTRY(IAAFShellExtView)
    COM_INTERFACE_ENTRY_IID(IID_IShellView, IShellView)
	COM_INTERFACE_ENTRY_IID(IID_IOleWindow, IOleWindow)
    COM_INTERFACE_ENTRY_IID(IID_IShellView2, IShellView2)
    COM_INTERFACE_ENTRY_IID(IID_IOleCommandTarget, IOleCommandTarget)
END_COM_MAP()

// IAAFShellExtView
public:
    // From IOleWindow
    STDMETHOD(GetWindow) ( HWND * lphwnd);
    
    // From IShellView
    STDMETHOD(UIActivate) ( UINT uState);
    STDMETHOD(DestroyViewWindow)();
    STDMETHOD(GetItemObject)( UINT uItem, REFIID riid,
                    void **ppv);
    STDMETHOD(Refresh) ();
    STDMETHOD(GetCurrentInfo)( LPFOLDERSETTINGS lpfs);
    STDMETHOD(SaveViewState)();

    // From IShellView2
    STDMETHOD(GetView)( SHELLVIEWID	*pvid, ULONG uView );
    STDMETHOD(CreateViewWindow2)( LPSV2CVW2_PARAMS lpParams);

    // Others
	void SetFolder( IShellFolder* pFolder, LPITEMIDLIST pidl );

private:
    // Keeps the view close to home.
    CAAFShellExtListView m_theView;
};

#endif //AAFSHELLEXTVIEW_H

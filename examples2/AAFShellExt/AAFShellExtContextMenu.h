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

// AAFShellExtContextMenu.h : Declaration of the CAAFShellExtContextMenu

#ifndef __AAFShellExtContextMenu_H_
#define __AAFShellExtContextMenu_H_

#include "resource.h"       // main symbols
#ifndef CONTEXTMENUIMPL_H
    #include <ContextMenuImpl.h>
#endif // CONTEXTMENUIMPL_H

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtContextMenu
class ATL_NO_VTABLE CAAFShellExtContextMenu : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAAFShellExtContextMenu, &CLSID_AAFShellExtContextMenu>,
	public IAAFShellExtContextMenu,
    public CContextMenuImpl
{
public:
	CAAFShellExtContextMenu();

DECLARE_REGISTRY_RESOURCEID(IDR_AAFSHELLEXTCONTEXTMENU)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAAFShellExtContextMenu)
	COM_INTERFACE_ENTRY(IAAFShellExtContextMenu)
    COM_INTERFACE_ENTRY_IID( IID_IContextMenu, IContextMenu )
END_COM_MAP()

    // IContextMenu methods
    STDMETHOD(QueryContextMenu)(HMENU hMenu, 
        UINT indexMenu, UINT idCmdFirst, 
        UINT idCmdLast, UINT uFlags);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpcmi);
    STDMETHOD(GetCommandString)(UINT idCmd, UINT uFlags, 
        UINT* pwReserved, LPSTR pszName, UINT cchMax);

// IAAFShellExtContextMenu
public:
    void SetPIDL( LPCITEMIDLIST pidl, IShellFolder* pFolder );

private:
	void ShowProperties(LPCMINVOKECOMMANDINFO lpcmi);
	void OpenFolder(LPCMINVOKECOMMANDINFO lpcmi, LPTSTR verb);
    LPITEMIDLIST m_pidl;
    CComPtr<IShellFolder> m_pFolder;
};

#endif //__AAFShellExtContextMenu_H_

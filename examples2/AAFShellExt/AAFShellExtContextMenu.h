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

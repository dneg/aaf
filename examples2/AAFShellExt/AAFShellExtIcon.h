// AAFShellExtIcon.h : Declaration of the CAAFShellExtIcon

#ifndef AAFSHELLEXTIcon_H_
#define AAFSHELLEXTIcon_H_

#include "resource.h"       // main symbols
#ifndef EXTRACTICONIMPL_H
    #include <ExtractIconImpl.h>
#endif // EXTRACTICONIMPL_H
#ifndef AAFSHELLEXTPIDL_H
    #include "AAFShellExtPidl.h"
#endif // AAFSHELLEXTPIDL_H
/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtIcon
class ATL_NO_VTABLE CAAFShellExtIcon : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAAFShellExtIcon, &CLSID_AAFShellExtIcon>,
	public IAAFShellExtIcon,
    public CExtractIconImpl
{
public:
	CAAFShellExtIcon();

DECLARE_REGISTRY_RESOURCEID(IDR_AAFSHELLEXTICON)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAAFShellExtIcon)
	COM_INTERFACE_ENTRY(IAAFShellExtIcon)
    COM_INTERFACE_ENTRY_IID(IID_IExtractIcon, IExtractIcon)
END_COM_MAP()

// IAAFShellExtIcon
public:
    // CExtractIconImpl overrides
    STDMETHOD (Extract)(LPCTSTR pszFile, UINT nIconIndex,
        HICON *phiconLarge, HICON *phiconSmall,
        UINT nIconSize);
    
    STDMETHOD(GetIconLocation)(
        UINT   uFlags, LPTSTR  szIconFile,
        UINT   cchMax, int   * piIndex,
        UINT  * pwFlags);

    void SetPIDL( LPITEMIDLIST pidl );

private:
    AAFShellExtPidlType m_type;
};

#endif //AAFSHELLEXTIcon_H_

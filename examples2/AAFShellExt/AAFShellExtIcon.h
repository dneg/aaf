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

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

// AAFShellExtFolder.h : Declaration of the CAAFShellExtShellFolder

#ifndef __AAFSHELLEXTFolder_H_
#define __AAFSHELLEXTFolder_H_

#include "resource.h"       // main symbols
#ifndef SHELLFOLDERIMPL_H
    #include <ShellFolderImpl.h>
#endif // SHELLFOLDERIMPL_H
#ifndef PERSISTFOLDERIMPL_H
    #include <PersistFolderImpl.h>
#endif // PERSISTFOLDERIMPL_H

#include "AxShellExt.h"
#include <memory>

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtFolder
class ATL_NO_VTABLE CAAFShellExtFolder : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAAFShellExtFolder, &CLSID_AAFShellExtFolder>,
    public CShellFolderImpl,
    public CPersistFolderImpl<CAAFShellExtFolder>,
	public IAAFShellExtFolder
{
public:
	CAAFShellExtFolder();

    // CShellFolderImpl overrides
    STDMETHOD(CreateViewObject)( HWND hwndOwner, REFIID riid, void **ppv);
    STDMETHOD(EnumObjects)( HWND hwnd, DWORD grfFlags, IEnumIDList **ppenumIDList);
    STDMETHOD(GetDisplayNameOf)( LPCITEMIDLIST pidl, DWORD uFlags, LPSTRRET lpName);
    STDMETHOD(GetAttributesOf)( UINT cidl, LPCITEMIDLIST * apidl, ULONG * rgfInOut);
    STDMETHOD(CompareIDs)( LPARAM lParam, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
    STDMETHOD(GetUIObjectOf)( HWND hwndOwner, UINT cidl, LPCITEMIDLIST * apidl,
                             REFIID riid, UINT * prgfInOut, void **ppv);
    STDMETHOD(BindToObject)( LPCITEMIDLIST pidl, LPBC pbc, REFIID riid, void **ppv);

    // CPersistFolderImpl overrides
    STDMETHOD(Initialize)(LPCITEMIDLIST pidl);

    virtual void FinalRelease();
    
DECLARE_REGISTRY_RESOURCEID(IDR_AAFSHELLEXTFOLDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAAFShellExtFolder)
	COM_INTERFACE_ENTRY(IAAFShellExtFolder)
	COM_INTERFACE_ENTRY_IID(IID_IShellFolder, IShellFolder)
	COM_INTERFACE_ENTRY_IID(IID_IPersistFolder, IPersistFolder)
	COM_INTERFACE_ENTRY_IID(IID_IPersist, IPersist)
    
END_COM_MAP()

public:
	void SetBound( bool bound )
	{ _bound = bound; }
private:
	// used... to be removed
	bool _bound;

public:
	LPITEMIDLIST _initialPidl;
};

#endif //__AAFSHELLEXTFolder_H_

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

// AAFShellExtIDLIST.h : Declaration of the CAAFShellExtIDList

#ifndef AAFSHELLEXTIDLIST_H_
#define AAFSHELLEXTIDLIST_H_

#include "resource.h"       // main symbols
#ifndef ENUMIDLISTIMPL_H
    #include <EnumIDListImpl.h>
#endif // ENUMIDLISTIMPL_H
#ifndef _LIST_
    #include <list>
#endif // _LIST_
#ifndef AAFSHELLEXTPIDL_H
    #include "AAFShellExtPidl.h"
#endif // AAFSHELLEXTPIDL_H

#include "AxShellExt.h"

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtIDList

class CAAFShellExtFolder;

class CAAFShellExtIDList : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAAFShellExtIDList, &CLSID_AAFShellExtIDLIST>,
    public IAAFShellExtIDLIST,
    public CEnumIDListImpl
{
public:
	void SetType( DWORD grfFlags );
	CAAFShellExtIDList();
    
DECLARE_REGISTRY_RESOURCEID(IDR_AAFSHELLEXTIDLIST)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAAFShellExtIDList)
	COM_INTERFACE_ENTRY(IAAFShellExtIDLIST)
    COM_INTERFACE_ENTRY_IID(IID_IEnumIDList, IEnumIDList)
END_COM_MAP()

// IAAFShellExtIDLIST
	bool SetShellFolder(IShellFolder* pFolder, 
        LPCITEMIDLIST pItemIDList);
    void FinalRelease();

    // IEnumIDList functions
    STDMETHOD(Clone)( IEnumIDList **ppEnum );
    STDMETHOD(Next)( ULONG celt, LPITEMIDLIST *rgelt, 
        ULONG *pceltFetched );
    STDMETHOD(Reset)();
    STDMETHOD(Skip)( ULONG celt );

	// FIXME JPT - This is to avoid messing with the default
	// ctor... that cause an ATL compile error.. didn't want
	// to deal with it.
	void SetAxShellExtIDList( std::auto_ptr<AxShellExtIDList> idlist );
	void SetBound( bool bound );

private:
	DWORD m_grfFlags;
	bool CreateIDList();
    CComPtr<IShellFolder> m_pFolder;
    typedef std::list<LPITEMIDLIST> AAFShellExtListType;
    AAFShellExtListType m_lstPidl;
    AAFShellExtListType::iterator m_currentIndex;

	std::auto_ptr<AxShellExtIDList> _axShellExtIDList;
	bool _bound;	
};

#endif //AAFSHELLEXTIDLIST_H_

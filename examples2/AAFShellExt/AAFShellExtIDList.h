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

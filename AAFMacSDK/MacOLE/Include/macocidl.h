/////////////////////////////////////////////////////////////////////////////
//
// macocidl.h 	OLE Control interfaces (this file is used by MAC only)
//
//              OLE Version 2.0
//
//              Copyright (c) 1992-1997, Microsoft Corp. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////


#ifndef _MACOCIDL_H_
#define _MACOCIDL_H_

#ifndef INITGUID
//
//  Interface IDs for data binding interfaces
//

DEFINE_GUID(IID_IPropertyNotifySink,
	0x9BFBBC02,0xEFF1,0x101A,0x84,0xED,0x00,0xAA,0x00,0x34,0x1D,0x07);

//
//  Interface IDs for licensing interfaces
//

DEFINE_GUID(IID_IClassFactory2,
	0xB196B28F,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);

//
//  Interface IDs for connection interfaces
//

DEFINE_GUID(IID_IProvideClassInfo,
	0xB196B283,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_IProvideClassInfo2,
    0xA6BC3AC0,0xDBAA,0x11CE,0x9D,0xE3,0x00,0xAA,0x00,0x4B,0xB8,0x51);
DEFINE_GUID(IID_IConnectionPointContainer,
	0xB196B284,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_IEnumConnectionPoints,
	0xB196B285,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_IConnectionPoint,
	0xB196B286,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_IEnumConnections,
	0xB196B287,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);

//
//  Interface IDs for OLE Control interfaces
//

DEFINE_GUID(IID_IOleControl,
	0xB196B288,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_IOleControlSite,
	0xB196B289,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_ISimpleFrameSite,
	0x742B0E01,0x14E6,0x101B,0x91,0x4E,0x00,0xAA,0x00,0x30,0x0C,0xAB);
DEFINE_GUID(IID_IPersistStreamInit,
	0x7FD52380,0x4E07,0x101B,0xAE,0x2D,0x08,0x00,0x2B,0x2E,0xC7,0x13);
DEFINE_GUID(IID_IPersistMemory, 
	0xBD1AE5E0,0xA6AE,0x11CE,0xBD,0x37,0x50,0x42,0x00,0xC1,0x00,0x00);
DEFINE_GUID(IID_IPersistPropertyBag,
	0x37D84F60,0x42CB,0x11CE,0x81,0x35,0x00,0xAA,0x00,0x4B,0xB8,0x51);
DEFINE_GUID(IID_IPropertyBag, 
	0x55272A00,0x42CB,0x11CE,0x81,0x35,0x00,0xAA,0x00,0x4B,0xB8,0x51);
DEFINE_GUID(IID_IErrorLog, 
	0x3127CA40,0x446E,0x11CE,0x81,0x35,0x00,0xAA,0x00,0x4B,0xB8,0x51);

//
//  Interface IDs for property sheet interfaces
//

DEFINE_GUID(IID_ISpecifyPropertyPages,
    0xB196B28B,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_IPerPropertyBrowsing,
    0x376BD3AA,0x3845,0x101B,0x84,0xED,0x08,0x00,0x2B,0x2E,0xC7,0x13);
DEFINE_GUID(IID_IPropertyPageSite,
    0xB196B28C,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_IPropertyPage,
    0xB196B28D,0xBAB4,0x101A,0xB6,0x9C,0x00,0xAA,0x00,0x34,0x1D,0x07);
DEFINE_GUID(IID_IPropertyPage2,
    0x01E44665,0x24AC,0x101B,0x84,0xED,0x08,0x00,0x2B,0x2E,0xC7,0x13);

//
//  Interface IDs for font objects
//

DEFINE_GUID(IID_IFont,
    0xBEF6E002,0xA874,0x101A,0x8B,0xBA,0x00,0xAA,0x00,0x30,0x0C,0xAB);
DEFINE_GUID(IID_IFontDisp,
    0xBEF6E003,0xA874,0x101A,0x8B,0xBA,0x00,0xAA,0x00,0x30,0x0C,0xAB);

//
//  Interface IDs for picture objects
//

DEFINE_GUID(IID_IPicture,
    0x7BF80980,0xBF32,0x101A,0x8B,0xBB,0x00,0xAA,0x00,0x30,0x0C,0xAB);
DEFINE_GUID(IID_IPictureDisp,
    0x7Bf80981,0xBF32,0x101A,0x8B,0xBB,0x00,0xAA,0x00,0x30,0x0C,0xAB);

#endif // INITGUID

/////////////////////////////////////////////////////////////////////////////
// Typedefs for characters and strings in interfaces

typedef char OLECHAR;
typedef OLECHAR FAR* LPOLESTR;
typedef const OLECHAR FAR* LPCOLESTR;

/////////////////////////////////////////////////////////////////////////////
// Typedefs for TEXTMETRIC structures

#if defined(_WIN32) && !defined(OLE2ANSI)
typedef TEXTMETRICW TEXTMETRICOLE;
#else
typedef TEXTMETRIC TEXTMETRICOLE;
#endif

typedef TEXTMETRICOLE FAR* LPTEXTMETRICOLE;


/////////////////////////////////////////////////////////////////////////////
// Typedefs for interfaces

#ifdef __cplusplus
interface IOleControl;
interface IOleControlSite;
interface ISimpleFrameSite;
interface IErrorLog;
interface IPropertyBag;
interface IPersistPropertyBag;
interface IPersistStreamInit;
interface IPersistMemory;
interface IPropertyNotifySink;
interface IProvideClassInfo;
interface IProvideClassInfo2;
interface IConnectionPointContainer;
interface IEnumConnectionPoints;
interface IConnectionPoint;
interface IEnumConnections;
interface IClassFactory2;
interface ISpecifyPropertyPages;
interface IPerPropertyBrowsing;
interface IPropertyPageSite;
interface IPropertyPage;
interface IPropertyPage2;
interface IFont;
interface IPicture;
interface IFontDisp;
interface IPictureDisp;
#else
typedef interface IOleControl IOleControl;
typedef interface IOleControlSite IOleControlSite;
typedef interface ISimpleFrameSite ISimpleFrameSite;
typedef interface IErrorLog IErrorLog;
typedef interface IPropertyBag IPropertyBag;
typedef interface IPersistPropertyBag IPersistPropertyBag;
typedef interface IPersistStreamInit IPersistStreamInit;
typedef interface IPersistMemory IPersistMemory;
typedef interface IPropertyNotifySink IPropertyNotifySink;
typedef interface IProvideClassInfo IProvideClassInfo;
typedef interface IProvideClassInfo2 IProvideClassInfo2;
typedef interface IConnectionPointContainer IConnectionPointContainer;
typedef interface IEnumConnectionPoints IEnumConnectionPoints;
typedef interface IConnectionPoint IConnectionPoint;
typedef interface IEnumConnections IEnumConnections;
typedef interface IClassFactory2 IClassFactory2;
typedef interface ISpecifyPropertyPages ISpecifyPropertyPages;
typedef interface IPerPropertyBrowsing IPerPropertyBrowsing;
typedef interface IPropertyPageSite IPropertyPageSite;
typedef interface IPropertyPage IPropertyPage;
typedef interface IPropertyPage2 IPropertyPage2;
typedef interface IFont IFont;
typedef interface IPicture IPicture;
typedef interface IFontDisp IFontDisp;
typedef interface IPictureDisp IPictureDisp;
#endif

typedef IOleControl FAR* LPOLECONTROL;
typedef IOleControlSite FAR* LPOLECONTROLSITE;
typedef ISimpleFrameSite FAR* LPSIMPLEFRAMESITE;
typedef IErrorLog FAR* LPERRORLOG;
typedef IPropertyBag FAR* LPPROPERTYBAG;
typedef IPersistPropertyBag FAR* LPPERSISTPROPERTYBAG;
typedef IPersistStreamInit FAR* LPPERSISTSTREAMINIT;
typedef IPersistMemory FAR* LPPERSISTMEMORY;
typedef interface IPropertyNotifySink FAR* LPPROPERTYNOTIFYSINK;
typedef IProvideClassInfo FAR* LPPROVIDECLASSINFO;
typedef IProvideClassInfo2 FAR* LPPROVIDECLASSINFO2;
typedef IConnectionPointContainer FAR* LPCONNECTIONPOINTCONTAINER;
typedef IEnumConnectionPoints FAR* LPENUMCONNECTIONPOINTS;
typedef IConnectionPoint FAR* LPCONNECTIONPOINT;
typedef IEnumConnections FAR* LPENUMCONNECTIONS;
typedef IClassFactory2 FAR* LPCLASSFACTORY2;
typedef ISpecifyPropertyPages FAR* LPSPECIFYPROPERTYPAGES;
typedef IPerPropertyBrowsing FAR* LPPERPROPERTYBROWSING;
typedef IPropertyPageSite FAR* LPPROPERTYPAGESITE;
typedef IPropertyPage FAR* LPPROPERTYPAGE;
typedef IPropertyPage2 FAR* LPPROPERTYPAGE2;
typedef IFont FAR* LPFONT;
typedef IPicture FAR* LPPICTURE;
typedef IFontDisp FAR* LPFONTDISP;
typedef IPictureDisp FAR* LPPICTUREDISP;


/////////////////////////////////////////////////////////////////////////////
// Typedefs for structs

typedef struct tagPOINTF FAR* LPPOINTF;
typedef struct tagCONTROLINFO FAR* LPCONTROLINFO;
typedef struct tagCONNECTDATA FAR* LPCONNECTDATA;
typedef struct tagLICINFO FAR* LPLICINFO;
typedef struct tagCAUUID FAR* LPCAUUID;
typedef struct tagCALPOLESTR FAR* LPCALPOLESTR;
typedef struct tagCADWORD FAR* LPCADWORD;
typedef struct tagPROPPAGEINFO FAR* LPPROPPAGEINFO;

typedef DWORD OLE_COLOR;
typedef UINT OLE_HANDLE;
typedef long OLE_XPOS_HIMETRIC;
typedef long OLE_YPOS_HIMETRIC;
typedef long OLE_XSIZE_HIMETRIC;
typedef long OLE_YSIZE_HIMETRIC;


/////////////////////////////////////////////////////////////////////////////
// POINTF structure

typedef struct tagPOINTF
{
	float x;
	float y;

} POINTF;


/////////////////////////////////////////////////////////////////////////////
// CONTROLINFO structure

typedef struct tagCONTROLINFO
{
	ULONG cb;       // Structure size
	HACCEL hAccel;  // Control mnemonics
	USHORT cAccel;  // Number of entries in mnemonics table
	DWORD dwFlags;  // Flags chosen from below

} CONTROLINFO;

#define CTRLINFO_EATS_RETURN    1   // Control doesn't send Return to container
#define CTRLINFO_EATS_ESCAPE    2   // Control doesn't send Escape to container


/////////////////////////////////////////////////////////////////////////////
// IOleControl interface

#undef  INTERFACE
#define INTERFACE IOleControl

DECLARE_INTERFACE_(IOleControl, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IOleControl methods
	STDMETHOD(GetControlInfo)(THIS_ LPCONTROLINFO pCI) PURE;
	STDMETHOD(OnMnemonic)(THIS_ LPMSG pMsg) PURE;
	STDMETHOD(OnAmbientPropertyChange)(THIS_ DISPID dispid) PURE;
	STDMETHOD(FreezeEvents)(THIS_ BOOL bFreeze) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IOleControlSite interface

#undef  INTERFACE
#define INTERFACE IOleControlSite

DECLARE_INTERFACE_(IOleControlSite, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IOleControlSite methods
	STDMETHOD(OnControlInfoChanged)(THIS) PURE;
	STDMETHOD(LockInPlaceActive)(THIS_ BOOL fLock) PURE;
	STDMETHOD(GetExtendedControl)(THIS_ LPDISPATCH FAR* ppDisp) PURE;
	STDMETHOD(TransformCoords)(THIS_ POINTL FAR* lpptlHimetric,
		POINTF FAR* lpptfContainer, DWORD flags) PURE;
	STDMETHOD(TranslateAccelerator)(THIS_ LPMSG lpMsg, DWORD grfModifiers)
		PURE;
	STDMETHOD(OnFocus)(THIS_ BOOL fGotFocus) PURE;
	STDMETHOD(ShowPropertyFrame)(THIS) PURE;
};

#define XFORMCOORDS_POSITION            0x1
#define XFORMCOORDS_SIZE                0x2
#define XFORMCOORDS_HIMETRICTOCONTAINER 0x4
#define XFORMCOORDS_CONTAINERTOHIMETRIC 0x8


/////////////////////////////////////////////////////////////////////////////
// ISimpleFrameSite interface

#undef  INTERFACE
#define INTERFACE ISimpleFrameSite

DECLARE_INTERFACE_(ISimpleFrameSite, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// ISimpleFrameSite methods
	STDMETHOD(PreMessageFilter)(THIS_ HWND hwnd, UINT msg, WPARAM wp,
		LPARAM lp, LRESULT FAR* lplResult, DWORD FAR * lpdwCookie) PURE;
	STDMETHOD(PostMessageFilter)(THIS_ HWND hwnd, UINT msg, WPARAM wp,
		LPARAM lp, LRESULT FAR* lplResult, DWORD dwCookie) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IErrorLog interface

#undef  INTERFACE
#define INTERFACE IErrorLog

DECLARE_INTERFACE_(IErrorLog, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IErrorLog methods
	STDMETHOD(AddError)(THIS_ LPCOLESTR pszPropName, LPEXCEPINFO pExcepInfo) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IPropertyBag interface

#undef  INTERFACE
#define INTERFACE IPropertyBag

DECLARE_INTERFACE_(IPropertyBag, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPropertyBag methods
	STDMETHOD(Read)(THIS_ LPCOLESTR pszPropName, LPVARIANT pVar,
		LPERRORLOG pErrorLog) PURE;
	STDMETHOD(Write)(THIS_ LPCOLESTR pszPropName, LPVARIANT pVar) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IPersistPropertyBag interface

#undef  INTERFACE
#define INTERFACE IPersistPropertyBag

DECLARE_INTERFACE_(IPersistPropertyBag, IPersist)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPersist methods
	STDMETHOD(GetClassID)(THIS_ LPCLSID lpClassID) PURE;

	// IPersistPropertyBag methods
	STDMETHOD(InitNew)(THIS) PURE;
	STDMETHOD(Load)(THIS_ LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog) PURE;
	STDMETHOD(Save)(THIS_ LPPROPERTYBAG pPropBag, BOOL fClearDirty,
		BOOL fSaveAllProperties) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IPersistStreamInit interface

#undef  INTERFACE
#define INTERFACE IPersistStreamInit

DECLARE_INTERFACE_(IPersistStreamInit, IPersist)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPersist methods
	STDMETHOD(GetClassID)(THIS_ LPCLSID lpClassID) PURE;

	// IPersistStreamInit methods
	STDMETHOD(IsDirty)(THIS) PURE;
	STDMETHOD(Load)(THIS_ LPSTREAM pStm) PURE;
	STDMETHOD(Save)(THIS_ LPSTREAM pStm, BOOL fClearDirty) PURE;
	STDMETHOD(GetSizeMax)(THIS_ ULARGE_INTEGER FAR* pcbSize) PURE;
	STDMETHOD(InitNew)(THIS) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IPersistMemory interface

#undef  INTERFACE
#define INTERFACE IPersistMemory

DECLARE_INTERFACE_(IPersistMemory, IPersist)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPersist methods
	STDMETHOD(GetClassID)(THIS_ LPCLSID lpClassID) PURE;

	// IPersistMemory methods
	STDMETHOD(IsDirty)(THIS) PURE;
	STDMETHOD(Load)(THIS_ LPVOID lpStream, ULONG cbSize) PURE;
	STDMETHOD(Save)(THIS_ LPVOID lpStream, BOOL fClearDirty, ULONG cbSize) PURE;
	STDMETHOD(GetSizeMax)(THIS_ ULONG* pcbSize) PURE;
	STDMETHOD(InitNew)(THIS) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IPropertyNotifySink interface

#undef  INTERFACE
#define INTERFACE IPropertyNotifySink

DECLARE_INTERFACE_(IPropertyNotifySink, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPropertyNotifySink methods
	STDMETHOD(OnChanged)(THIS_ DISPID dispid) PURE;
	STDMETHOD(OnRequestEdit)(THIS_ DISPID dispid) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IProvideClassInfo interface

#undef  INTERFACE
#define INTERFACE IProvideClassInfo

DECLARE_INTERFACE_(IProvideClassInfo, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IProvideClassInfo methods
	STDMETHOD(GetClassInfo)(THIS_ LPTYPEINFO FAR* ppTI) PURE;
};

/////////////////////////////////////////////////////////////////////////////
// IProvideClassInfo2 interface

#undef  INTERFACE
#define INTERFACE IProvideClassInfo2

DECLARE_INTERFACE_(IProvideClassInfo2, IProvideClassInfo)
{
	BEGIN_INTERFACE
    // IUnknown methods
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    // IProvideClassInfo methods
    STDMETHOD(GetClassInfo)(THIS_ LPTYPEINFO FAR* ppTI) PURE;

    // IProvideClassInfo2 methods
    STDMETHOD(GetGUID)(THIS_ DWORD dwGuidKind, GUID FAR* pGUID) PURE;
};

#define GUIDKIND_DEFAULT_SOURCE_DISP_IID    1

/////////////////////////////////////////////////////////////////////////////
// IConnectionPointContainer interface

#undef  INTERFACE
#define INTERFACE IConnectionPointContainer

DECLARE_INTERFACE_(IConnectionPointContainer, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IConnectionPointContainer methods
	STDMETHOD(EnumConnectionPoints)(THIS_ LPENUMCONNECTIONPOINTS FAR* ppEnum)
		PURE;
	STDMETHOD(FindConnectionPoint)(THIS_ REFIID iid,
		LPCONNECTIONPOINT FAR* ppCP) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IEnumConnectionPoint interface

#undef  INTERFACE
#define INTERFACE IEnumConnectionPoints

DECLARE_INTERFACE_(IEnumConnectionPoints, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IEnumConnectionPoints methods
	STDMETHOD(Next)(THIS_ ULONG cConnections, LPCONNECTIONPOINT FAR* rgpcn,
		ULONG FAR* lpcFetched) PURE;
	STDMETHOD(Skip)(THIS_ ULONG cConnections) PURE;
	STDMETHOD(Reset)(THIS) PURE;
	STDMETHOD(Clone)(THIS_ LPENUMCONNECTIONPOINTS FAR* ppEnum) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IConnectionPoint interface

#undef  INTERFACE
#define INTERFACE IConnectionPoint

DECLARE_INTERFACE_(IConnectionPoint, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IConnectionPoint methods
	STDMETHOD(GetConnectionInterface)(THIS_ IID FAR* pIID) PURE;
	STDMETHOD(GetConnectionPointContainer)(THIS_
		IConnectionPointContainer FAR* FAR* ppCPC) PURE;
	STDMETHOD(Advise)(THIS_ LPUNKNOWN pUnkSink, DWORD FAR* pdwCookie) PURE;
	STDMETHOD(Unadvise)(THIS_ DWORD dwCookie) PURE;
	STDMETHOD(EnumConnections)(THIS_ LPENUMCONNECTIONS FAR* ppEnum) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// CONNECTDATA structure

typedef struct tagCONNECTDATA
{
	LPUNKNOWN pUnk;
	DWORD dwCookie;

} CONNECTDATA;


/////////////////////////////////////////////////////////////////////////////
// IEnumConnections interface

#undef  INTERFACE
#define INTERFACE IEnumConnections

DECLARE_INTERFACE_(IEnumConnections, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IEnumConnections methods
	STDMETHOD(Next)(THIS_ ULONG cConnections, LPCONNECTDATA rgcd,
		ULONG FAR* lpcFetched) PURE;
	STDMETHOD(Skip)(THIS_ ULONG cConnections) PURE;
	STDMETHOD(Reset)(THIS) PURE;
	STDMETHOD(Clone)(THIS_ LPENUMCONNECTIONS FAR* ppecn) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// LICINFO structure

typedef struct tagLICINFO
{
	long cbLicInfo;
	BOOL fRuntimeKeyAvail;
	BOOL fLicVerified;

} LICINFO;


/////////////////////////////////////////////////////////////////////////////
// IClassFactory2 interface

#undef  INTERFACE
#define INTERFACE IClassFactory2

DECLARE_INTERFACE_(IClassFactory2, IClassFactory)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IClassFactory methods
	STDMETHOD(CreateInstance)(THIS_ LPUNKNOWN pUnkOuter, REFIID riid,
		LPVOID FAR* ppvObject) PURE;
	STDMETHOD(LockServer)(THIS_ BOOL fLock) PURE;

	//  IClassFactory2 methods
	STDMETHOD(GetLicInfo)(THIS_ LPLICINFO pLicInfo) PURE;
	STDMETHOD(RequestLicKey)(THIS_ DWORD dwResrved, BSTR FAR* pbstrKey) PURE;
	STDMETHOD(CreateInstanceLic)(THIS_ LPUNKNOWN pUnkOuter,
		LPUNKNOWN pUnkReserved, REFIID riid, BSTR bstrKey,
		LPVOID FAR* ppvObject) PURE;
};


/////////////////////////////////////////////////////////////////////////////
//  CAUUID structure - a counted array of UUIDs

#ifndef _tagCAUUID_DEFINED
#define _tagCAUUID_DEFINED
#define _CAUUID_DEFINED

typedef struct tagCAUUID
{
	ULONG cElems;
	GUID FAR* pElems;

} CAUUID;

#endif


/////////////////////////////////////////////////////////////////////////////
//  CALPOLESTR structure - a counted array of LPOLESTRs

#ifndef _tagCALPOLESTR_DEFINED
#define _tagCALPOLESTR_DEFINED
#define _CALPOLESTR_DEFINED

typedef struct tagCALPOLESTR
{
	ULONG cElems;
	LPOLESTR FAR* pElems;

} CALPOLESTR;

#endif


/////////////////////////////////////////////////////////////////////////////
//  CAUUID structure - a counted array of DWORDs

#ifndef _tagCADWORD_DEFINED
#define _tagCADWORD_DEFINED
#define _CADWORD_DEFINED

typedef struct tagCADWORD
{
	ULONG cElems;
	DWORD FAR* pElems;

} CADWORD;

#endif


/////////////////////////////////////////////////////////////////////////////
// PROPPAGEINFO structure - information about a property page

typedef struct tagPROPPAGEINFO
{
	size_t cb;
	LPOLESTR pszTitle;
	SIZE size;
	LPOLESTR pszDocString;
	LPOLESTR pszHelpFile;
	DWORD dwHelpContext;

} PROPPAGEINFO;


/////////////////////////////////////////////////////////////////////////////
// ISpecifyPropertyPages interface

#undef  INTERFACE
#define INTERFACE ISpecifyPropertyPages

DECLARE_INTERFACE_(ISpecifyPropertyPages, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// ISpecifyPropertyPages interface
	STDMETHOD(GetPages)(THIS_ CAUUID FAR* pPages) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IPerPropertyBrowsing interface

#undef  INTERFACE
#define INTERFACE IPerPropertyBrowsing

DECLARE_INTERFACE_(IPerPropertyBrowsing, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPerPropertyBrowsing interface
	STDMETHOD(GetDisplayString)(THIS_ DISPID dispid, BSTR FAR* lpbstr) PURE;
	STDMETHOD(MapPropertyToPage)(THIS_ DISPID dispid, LPCLSID lpclsid) PURE;
	STDMETHOD(GetPredefinedStrings)(THIS_ DISPID dispid,
		CALPOLESTR FAR* lpcaStringsOut, CADWORD FAR* lpcaCookiesOut) PURE;
	STDMETHOD(GetPredefinedValue)(THIS_ DISPID dispid, DWORD dwCookie,
		VARIANT FAR* lpvarOut) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IPropertyPageSite interface

#undef  INTERFACE
#define INTERFACE IPropertyPageSite

DECLARE_INTERFACE_(IPropertyPageSite, IUnknown)
{
BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPropertyPageSite methods
	STDMETHOD(OnStatusChange)(THIS_ DWORD flags) PURE;
	STDMETHOD(GetLocaleID)(THIS_ LCID FAR* pLocaleID) PURE;
	STDMETHOD(GetPageContainer)(THIS_ LPUNKNOWN FAR* ppUnk) PURE;
	STDMETHOD(TranslateAccelerator)(THIS_ LPMSG lpMsg) PURE;
};

#define PROPPAGESTATUS_DIRTY    0x1 // Values in page have changed
#define PROPPAGESTATUS_VALIDATE 0x2 // Appropriate time to validate/apply


/////////////////////////////////////////////////////////////////////////////
// IPropertyPage interface

#undef  INTERFACE
#define INTERFACE IPropertyPage

DECLARE_INTERFACE_(IPropertyPage, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPropertyPage methods
	STDMETHOD(SetPageSite)(THIS_ LPPROPERTYPAGESITE pPageSite) PURE;
	STDMETHOD(Activate)(THIS_ HWND hwndParent, LPCRECT lprc, BOOL bModal) PURE;
	STDMETHOD(Deactivate)(THIS) PURE;
	STDMETHOD(GetPageInfo)(THIS_ LPPROPPAGEINFO pPageInfo) PURE;
	STDMETHOD(SetObjects)(THIS_ ULONG cObjects, LPUNKNOWN FAR* ppunk) PURE;
	STDMETHOD(Show)(THIS_ UINT nCmdShow) PURE;
	STDMETHOD(Move)(THIS_ LPCRECT prect) PURE;
	STDMETHOD(IsPageDirty)(THIS) PURE;
	STDMETHOD(Apply)(THIS) PURE;
	STDMETHOD(Help)(THIS_ LPCOLESTR lpszHelpDir) PURE;
	STDMETHOD(TranslateAccelerator)(THIS_ LPMSG lpMsg) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IPropertyPage2 interface

#undef  INTERFACE
#define INTERFACE IPropertyPage2

DECLARE_INTERFACE_(IPropertyPage2, IPropertyPage)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IPropertyPage methods
	STDMETHOD(SetPageSite)(THIS_ LPPROPERTYPAGESITE pPageSite) PURE;
	STDMETHOD(Activate)(THIS_ HWND hwndParent, LPCRECT lprc, BOOL bModal) PURE;
	STDMETHOD(Deactivate)(THIS) PURE;
	STDMETHOD(GetPageInfo)(THIS_ LPPROPPAGEINFO pPageInfo) PURE;
	STDMETHOD(SetObjects)(THIS_ ULONG cObjects, LPUNKNOWN FAR* ppunk) PURE;
	STDMETHOD(Show)(THIS_ UINT nCmdShow) PURE;
	STDMETHOD(Move)(THIS_ LPCRECT prect) PURE;
	STDMETHOD(IsPageDirty)(THIS) PURE;
	STDMETHOD(Apply)(THIS) PURE;
	STDMETHOD(Help)(THIS_ LPCOLESTR lpszHelpDir) PURE;
	STDMETHOD(TranslateAccelerator)(THIS_ LPMSG lpMsg) PURE;

	// IPropertyPage2 methods
	STDMETHOD(EditProperty)(THIS_ DISPID dispid) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// IFont interface

#undef  INTERFACE
#define INTERFACE IFont

DECLARE_INTERFACE_(IFont, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	// IFont methods
	STDMETHOD(get_Name)(THIS_ BSTR FAR* pname) PURE;
	STDMETHOD(put_Name)(THIS_ BSTR name) PURE;
	STDMETHOD(get_Size)(THIS_ CY FAR* psize) PURE;
	STDMETHOD(put_Size)(THIS_ CY size) PURE;
	STDMETHOD(get_Bold)(THIS_ BOOL FAR* pbold) PURE;
	STDMETHOD(put_Bold)(THIS_ BOOL bold) PURE;
	STDMETHOD(get_Italic)(THIS_ BOOL FAR* pitalic) PURE;
	STDMETHOD(put_Italic)(THIS_ BOOL italic) PURE;
	STDMETHOD(get_Underline)(THIS_ BOOL FAR* punderline) PURE;
	STDMETHOD(put_Underline)(THIS_ BOOL underline) PURE;
	STDMETHOD(get_Strikethrough)(THIS_ BOOL FAR* pstrikethrough) PURE;
	STDMETHOD(put_Strikethrough)(THIS_ BOOL strikethrough) PURE;
	STDMETHOD(get_Weight)(THIS_ short FAR* pweight) PURE;
	STDMETHOD(put_Weight)(THIS_ short weight) PURE;
	STDMETHOD(get_Charset)(THIS_ short FAR* pcharset) PURE;
	STDMETHOD(put_Charset)(THIS_ short charset) PURE;
	STDMETHOD(get_hFont)(THIS_ HFONT FAR* phfont) PURE;
	STDMETHOD(Clone)(THIS_ IFont FAR* FAR* lplpfont) PURE;
	STDMETHOD(IsEqual)(THIS_ IFont FAR * lpFontOther) PURE;
	STDMETHOD(SetRatio)(THIS_ long cyLogical, long cyHimetric) PURE;
	STDMETHOD(QueryTextMetrics)(THIS_ LPTEXTMETRICOLE lptm) PURE;
	STDMETHOD(AddRefHfont)(THIS_ HFONT hfont) PURE;
	STDMETHOD(ReleaseHfont)(THIS_ HFONT hfont) PURE;
	STDMETHOD(SetHdc)(THIS_ HDC hdc) PURE;
};


/////////////////////////////////////////////////////////////////////////////
// Picture attributes

#define PICTURE_SCALABLE        0x1l
#define PICTURE_TRANSPARENT     0x2l

/////////////////////////////////////////////////////////////////////////////
// IPicture interface

#undef  INTERFACE
#define INTERFACE IPicture

DECLARE_INTERFACE_(IPicture, IUnknown)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	// IPicture methods
	STDMETHOD(get_Handle)(THIS_ OLE_HANDLE FAR* phandle) PURE;
	STDMETHOD(get_hPal)(THIS_ OLE_HANDLE FAR* phpal) PURE;
	STDMETHOD(get_Type)(THIS_ short FAR* ptype) PURE;
	STDMETHOD(get_Width)(THIS_ OLE_XSIZE_HIMETRIC FAR* pwidth) PURE;
	STDMETHOD(get_Height)(THIS_ OLE_YSIZE_HIMETRIC FAR* pheight) PURE;
	STDMETHOD(Render)(THIS_ HDC hdc, long x, long y, long cx, long cy,
		OLE_XPOS_HIMETRIC xSrc, OLE_YPOS_HIMETRIC ySrc,
		OLE_XSIZE_HIMETRIC cxSrc, OLE_YSIZE_HIMETRIC cySrc,
		LPCRECT lprcWBounds) PURE;
	STDMETHOD(set_hPal)(THIS_ OLE_HANDLE hpal) PURE;
	STDMETHOD(get_CurDC)(THIS_ HDC FAR * phdcOut) PURE;
	STDMETHOD(SelectPicture)(THIS_
		HDC hdcIn, HDC FAR * phdcOut, OLE_HANDLE FAR * phbmpOut) PURE;
	STDMETHOD(get_KeepOriginalFormat)(THIS_ BOOL * pfkeep) PURE;
	STDMETHOD(put_KeepOriginalFormat)(THIS_ BOOL fkeep) PURE;
	STDMETHOD(PictureChanged)(THIS) PURE;
	STDMETHOD(SaveAsFile)(THIS_ LPSTREAM lpstream, BOOL fSaveMemCopy,
		LONG FAR * lpcbSize) PURE;
	STDMETHOD(get_Attributes)(THIS_ DWORD FAR * lpdwAttr) PURE;

};

/////////////////////////////////////////////////////////////////////////////
// IFontDisp interface

#undef  INTERFACE
#define INTERFACE IFontDisp

DECLARE_INTERFACE_(IFontDisp, IDispatch)
{
	  BEGIN_INTERFACE
    // IUnknown methods
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // IDispatch methods
    STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR* pctinfo) PURE;
    STDMETHOD(GetTypeInfo)(THIS_ UINT itinfo, LCID lcid,
        ITypeInfo FAR* FAR* pptinfo) PURE;
    STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR FAR* rgszNames,
        UINT cNames, LCID lcid, DISPID FAR* rgdispid) PURE;
    STDMETHOD(Invoke)(THIS_ DISPID dispidMember, REFIID riid, LCID lcid,
        WORD wFlags, DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
        EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr) PURE;
};

/////////////////////////////////////////////////////////////////////////////
// IPictureDisp interface

#undef  INTERFACE
#define INTERFACE IPictureDisp

DECLARE_INTERFACE_(IPictureDisp, IDispatch)
{
	BEGIN_INTERFACE
	// IUnknown methods
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	// IDispatch methods
	STDMETHOD(GetTypeInfoCount)(THIS_ UINT FAR* pctinfo) PURE;
	STDMETHOD(GetTypeInfo)(THIS_ UINT itinfo, LCID lcid,
		ITypeInfo FAR* FAR* pptinfo) PURE;
	STDMETHOD(GetIDsOfNames)(THIS_ REFIID riid, LPOLESTR FAR* rgszNames,
		UINT cNames, LCID lcid, DISPID FAR* rgdispid) PURE;
	STDMETHOD(Invoke)(THIS_ DISPID dispidMember, REFIID riid, LCID lcid,
		WORD wFlags, DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
		EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr) PURE;
};

#endif // _MACOCIDL_H_

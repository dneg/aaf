/*****************************************************************************\
*                                                                             *
* dvobj.h -		Data/View object definitions								  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/

#if !defined( __DVOBJ__ ) && !defined( _DVOBJ_H_ )
#define __DVOBJ__
#define _DVOBJ_H_

/****** DV value types ******************************************************/

//      forward type declarations
#if defined(__cplusplus)
interface IStorage;
interface IStream;
interface IAdviseSink;
interface IMoniker;
#else 
//typedef interface IStorage IStorage;
//typedef interface IStream IStream;
//typedef interface IAdviseSink IAdviseSink;
//typedef interface IMoniker IMoniker;
#endif

typedef         interface IStorage * LPSTORAGE, ** LPLPSTORAGE;
typedef         interface IAdviseSink * LPADVISESINK, ** LPLPADVISESINK;
typedef         interface IMoniker * LPMONIKER, ** LPLPMONIKER;


// Data/View aspect; specifies the desired aspect of the object when 
// drawing or getting data.
typedef enum tagDVASPECT
{
    DVASPECT_CONTENT = 1,
    DVASPECT_THUMBNAIL = 2,
    DVASPECT_ICON = 4,
    DVASPECT_DOCPRINT = 8
	,DVASPECT_FORCELONG	=	2147483647
} DVASPECT;


// target device flags
typedef enum tagTDFLAGS
{
    TD_PRINTER = 1,		 	// if set, td describes a printer
    TD_POSTSCRIPT = 2,		// if set, td has postscript
    TD_GX = 4				// if set, td has GX
	,TD_FORCELONG	=	2147483647
} TDFLAGS;

// Data/View target device; determines the device for drawing or getting data
typedef struct FARSTRUCT tagDVTARGETDEVICE
{
    long	tdSize;
	// Windows definition
    unsigned short tdDriverNameOffset;
    unsigned short tdDeviceNameOffset;
    unsigned short tdPortNameOffset;
    unsigned short tdExtDevmodeOffset;
    unsigned char  tdWinData[1];
	unsigned char 	byAlign;

	// MAC Definition.
	long	dwSizeX;	// max length in pixels
	long	dwSizeY;	//
	short 	wDpiX;		// dots per inch x
	short	wDpiY;		// dots per inch y
	short	wColorDepth;// in color bits (eg 8 == 256 colors)
	short	wFlags;		// printer/screen,.i postscript, gx etc.
	short	wMisc;		// offset to app specific data or NULL (usually null)
    unsigned char tdData[1];
} DVTARGETDEVICE;


// Format, etc.; completely specifices the kind of data desired, including tymed
typedef struct FARSTRUCT tagFORMATETC
{
    unsigned long          cfFormat;
    DVTARGETDEVICE * ptd;
    unsigned long               dwAspect;
    long                lindex;
    unsigned long               tymed;
} FORMATETC, * LPFORMATETC;


// TYpes of storage MEDiums; determines how data is stored or passed around
typedef enum tagTYMED
{
    TYMED_HGLOBAL = 1,
    TYMED_FILE = 2,
    TYMED_ISTREAM = 4,
    TYMED_ISTORAGE = 8,
    TYMED_GDI = 16,
    TYMED_MFPICT = 32,
	TYMED_PICT = 32,
    TYMED_FSP	= 64,
    TYMED_NULL = 0
	,TYMED_FORCELONG	=	2147483647
} TYMED;


// DATA format DIRection
typedef enum tagDATADIR
{
    DATADIR_GET = 1,
    DATADIR_SET = 2
	,DATADIR_FORCELONG	=	2147483647
} DATADIR;


// SToraGe MEDIUM; a block of data on a particular medium 
typedef struct FARSTRUCT tagSTGMEDIUM
{
    unsigned long   tymed;
    union
    {
        Handle  hGlobal;
        char *   lpszFileName;
        LPSTREAM pstm;
        LPSTORAGE pstg;
		FSSpec *pFSSpec;
    }
#ifdef NONAMELESSUNION
    u       // add a tag when name less unions not supported
#endif
#if !defined(__cplusplus) && !defined(_MSC_VER)
	u		// add a tag for Mac
#endif
    ;
    IUnknown * pUnkForRelease;
} STGMEDIUM, * LPSTGMEDIUM;


// Advise Flags
typedef enum tagADVF
{
    ADVF_NODATA = 1,
    ADVF_PRIMEFIRST = 2,
    ADVF_ONLYONCE = 4,
    ADVF_DATAONSTOP = 64,
    ADVFCACHE_NOHANDLER = 8,
    ADVFCACHE_FORCEBUILTIN = 16,
    ADVFCACHE_ONSAVE = 32
	,ADVF_FORCELONG	=	2147483647
} ADVF;


// Stats for data; used by several enumerations and by at least one 
// implementation of IDataAdviseHolder; if a field is not used, it
// will be NULL.
typedef struct FARSTRUCT tagSTATDATA
{                                   // field used by:
    FORMATETC formatetc;            // EnumAdvise, EnumData (cache), EnumFormats
    unsigned long advf;                     // EnumAdvise, EnumData (cache)
    LPADVISESINK pAdvSink;      // EnumAdvise
    unsigned long dwConnection;             // EnumAdvise
} STATDATA;
    
typedef  STATDATA * LPSTATDATA;



/****** DV Interfaces ***************************************************/


#undef  INTERFACE
#define INTERFACE   IEnumFORMATETC

DECLARE_INTERFACE_(IEnumFORMATETC, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppv) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IEnumFORMATETC methods ***
    STDMETHOD(Next) (THIS_ unsigned long celt, FORMATETC  * rgelt, unsigned long * pceltFetched) PURE;
    STDMETHOD(Skip) (THIS_ unsigned long celt) PURE;
    STDMETHOD(Reset) (THIS) PURE;
    STDMETHOD(Clone) (THIS_ IEnumFORMATETC * * ppenum) PURE;
};
typedef        IEnumFORMATETC * LPENUMFORMATETC, ** LPLPENUMFORMATETC;


#undef  INTERFACE
#define INTERFACE   IEnumSTATDATA

DECLARE_INTERFACE_(IEnumSTATDATA, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppv) PURE;
    STDMETHOD_(unsigned long, AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long, Release) (THIS) PURE;

    // *** IEnumSTATDATA methods ***
    STDMETHOD(Next) (THIS_ unsigned long celt, STATDATA  * rgelt, unsigned long * pceltFetched) PURE;
    STDMETHOD(Skip) (THIS_ unsigned long celt) PURE;
    STDMETHOD(Reset) (THIS) PURE;
    STDMETHOD(Clone) (THIS_ IEnumSTATDATA * * ppenum) PURE;
};
typedef        IEnumSTATDATA * LPENUMSTATDATA, ** LPLPENUMSTATDATA;



#undef  INTERFACE
#define INTERFACE   IDataObject

#define DATA_E_FORMATETC        DV_E_FORMATETC
#ifndef _WINERROR_
#define DATA_S_SAMEFORMATETC    (DATA_S_FIRST + 0)
#endif

DECLARE_INTERFACE_(IDataObject, IUnknown)
{ 
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long, Release) (THIS) PURE;

    // *** IDataObject methods ***
    STDMETHOD(GetData) (THIS_ LPFORMATETC pformatetcIn,
                            LPSTGMEDIUM pmedium ) PURE;
    STDMETHOD(GetDataHere) (THIS_ LPFORMATETC pformatetc,
                            LPSTGMEDIUM pmedium ) PURE;
    STDMETHOD(QueryGetData) (THIS_ LPFORMATETC pformatetc ) PURE;
    STDMETHOD(GetCanonicalFormatEtc) (THIS_ LPFORMATETC pformatetc,
                            LPFORMATETC pformatetcOut) PURE;
    STDMETHOD(SetData) (THIS_ LPFORMATETC pformatetc, STGMEDIUM  * pmedium,
                            OLEBOOL fRelease) PURE;
    STDMETHOD(EnumFormatEtc) (THIS_ unsigned long dwDirection,
                            LPLPENUMFORMATETC ppenumFormatEtc) PURE;

    STDMETHOD(DAdvise) (THIS_ FORMATETC * pFormatetc, unsigned long advf, 
                    LPADVISESINK pAdvSink, unsigned long * pdwConnection) PURE;
    STDMETHOD(DUnadvise) (THIS_ unsigned long dwConnection) PURE;
    STDMETHOD(EnumDAdvise) (THIS_ LPLPENUMSTATDATA ppenumAdvise) PURE;
};                 
typedef      IDataObject * LPDATAOBJECT, ** LPLPDATAOBJECT;



#undef  INTERFACE
#define INTERFACE   IViewObject

#ifndef _WINERROR_
#define VIEW_E_DRAW         (VIEW_E_FIRST)
#endif
#define E_DRAW              VIEW_E_DRAW

#ifndef _WINERROR_
#define VIEW_S_ALREADY_FROZEN   (VIEW_S_FIRST)
#endif

DECLARE_INTERFACE_(IViewObject, IUnknown)
{ 
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IViewObject methods ***
    STDMETHOD(Draw) (THIS_ unsigned long dwDrawAspect, long lindex,
                    void * pvAspect, DVTARGETDEVICE  * ptd,
                    GrafPtr pGrafTargetDev,
                    GrafPtr pGrafDraw, 
                    LPCRECTL lprcBounds, 
                    LPCRECTL lprcWBounds,
					void *, 
                    unsigned long dwContinue) PURE;

    STDMETHOD(GetColorSet) (THIS_ unsigned long dwDrawAspect, long lindex,
                    void * pvAspect, DVTARGETDEVICE  * ptd,
                    GrafPtr pGrafTargetDev,
                    void * * ppColorSet) PURE;
                    
    STDMETHOD(Freeze)(THIS_ unsigned long dwDrawAspect, long lindex, 
                    void * pvAspect,
                    unsigned long * pdwFreeze) PURE;
    STDMETHOD(Unfreeze) (THIS_ unsigned long dwFreeze) PURE;
    STDMETHOD(SetAdvise) (THIS_ unsigned long aspects, unsigned long advf, 
                    LPADVISESINK pAdvSink) PURE;
    STDMETHOD(GetAdvise) (THIS_ unsigned long * pAspects, unsigned long * pAdvf, 
                    LPLPADVISESINK ppAdvSink) PURE;
};
typedef      IViewObject * LPVIEWOBJECT, ** LPLPVIEWOBJECT;


#undef  INTERFACE
#define INTERFACE   IViewObject2

DECLARE_INTERFACE_(IViewObject2, IViewObject)
{ 
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IViewObject methods ***
    STDMETHOD(Draw) (THIS_ unsigned long dwDrawAspect, long lindex,
                    void * pvAspect, DVTARGETDEVICE  * ptd,
                    GrafPtr pGrafTargetDev,
                    GrafPtr pGrafDraw, 
                    LPCRECTL lprcBounds, 
                    LPCRECTL lprcWBounds,
					void *, 
                    unsigned long dwContinue) PURE;

    STDMETHOD(GetColorSet) (THIS_ unsigned long dwDrawAspect, long lindex,
                    void * pvAspect, DVTARGETDEVICE  * ptd,
                    GrafPtr pGrafTargetDev,
                    void * * ppColorSet) PURE;
                    
    STDMETHOD(Freeze)(THIS_ unsigned long dwDrawAspect, long lindex, 
                    void * pvAspect,
                    unsigned long * pdwFreeze) PURE;
    STDMETHOD(Unfreeze) (THIS_ unsigned long dwFreeze) PURE;
    STDMETHOD(SetAdvise) (THIS_ unsigned long aspects, unsigned long advf, 
                    LPADVISESINK pAdvSink) PURE;
    STDMETHOD(GetAdvise) (THIS_ unsigned long * pAspects, unsigned long * pAdvf, 
                    LPLPADVISESINK ppAdvSink) PURE;
					
    // *** IViewObject2 methods ***
    STDMETHOD(GetExtent) (THIS_ unsigned long dwDrawAspect, long lindex,
                    DVTARGETDEVICE *ptd, LPSIZEL lpsizel) PURE;
					
};
typedef      IViewObject2 * LPVIEWOBJECT2, ** LPLPVIEWOBJECT2;



#undef  INTERFACE
#define INTERFACE   IAdviseSink

DECLARE_INTERFACE_(IAdviseSink, IUnknown)
{ 
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppv) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IAdviseSink methods ***
    STDMETHOD_(void,OnDataChange)(THIS_ FORMATETC * pFormatetc, 
                            STGMEDIUM * pStgmed) PURE;
    STDMETHOD_(void,OnViewChange)(THIS_ unsigned long dwAspect, long lindex) PURE;
    STDMETHOD_(void,OnRename)(THIS_ LPMONIKER pmk) PURE;
    STDMETHOD_(void,OnSave)(THIS) PURE;
    STDMETHOD_(void,OnClose)(THIS) PURE;
};
//typedef      IAdviseSink * LPADVISESINK, ** LPLPADVISESINK;



#undef  INTERFACE
#define INTERFACE   IAdviseSink2

DECLARE_INTERFACE_(IAdviseSink2, IAdviseSink)
{ 
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppv) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IAdviseSink methods ***
    STDMETHOD_(void,OnDataChange)(THIS_ FORMATETC * pFormatetc, 
                            STGMEDIUM * pStgmed) PURE;
    STDMETHOD_(void,OnViewChange)(THIS_ unsigned long dwAspect, long lindex) PURE;
    STDMETHOD_(void,OnRename)(THIS_ LPMONIKER pmk) PURE;
    STDMETHOD_(void,OnSave)(THIS) PURE;
    STDMETHOD_(void,OnClose)(THIS) PURE;

    // *** IAdviseSink2 methods ***
    STDMETHOD_(void,OnLinkSrcChange)(THIS_ LPMONIKER pmk) PURE;
};
typedef      IAdviseSink2 * LPADVISESINK2, ** LPLPADVISESINK2;



#undef  INTERFACE
#define INTERFACE   IDataAdviseHolder

DECLARE_INTERFACE_(IDataAdviseHolder, IUnknown)
{ 
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppv) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IDataAdviseHolder methods ***
    STDMETHOD(Advise)(THIS_ LPDATAOBJECT pDataObject, FORMATETC * pFetc, 
            unsigned long advf, LPADVISESINK pAdvise, unsigned long * pdwConnection) PURE;
    STDMETHOD(Unadvise)(THIS_ unsigned long dwConnection) PURE;
    STDMETHOD(EnumAdvise)(THIS_ LPLPENUMSTATDATA ppenumAdvise) PURE;

    STDMETHOD(SendOnDataChange)(THIS_ LPDATAOBJECT pDataObject, unsigned long dwReserved, unsigned long advf) PURE;
};
typedef      IDataAdviseHolder * LPDATAADVISEHOLDER, ** LPLPDATAADVISEHOLDER;



#undef  INTERFACE
#define INTERFACE   IOleCache

#ifndef _WINERROR_
#define CACHE_E_NOCACHE_UPDATED         (CACHE_E_FIRST)

#define CACHE_S_FORMATETC_NOTSUPPORTED  (CACHE_S_FIRST)
#define CACHE_S_SAMECACHE               (CACHE_S_FIRST+1)
#define CACHE_S_SOMECACHES_NOTUPDATED   (CACHE_S_FIRST+2)
#endif


DECLARE_INTERFACE_(IOleCache, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleCache methods ***
    STDMETHOD(Cache) (THIS_ LPFORMATETC lpFormatetc, unsigned long advf, unsigned long	* lpdwConnection) PURE;
    STDMETHOD(Uncache) (THIS_ unsigned long dwConnection) PURE;
    STDMETHOD(EnumCache) (THIS_ LPLPENUMSTATDATA ppenumStatData) PURE;
    STDMETHOD(InitCache) (THIS_ LPDATAOBJECT pDataObject) PURE;
    STDMETHOD(SetData) (THIS_ LPFORMATETC pformatetc, STGMEDIUM  * pmedium,
                            OLEBOOL fRelease) PURE;
};
typedef         IOleCache * LPOLECACHE, ** LPLPOLECACHE;




// Cache update Flags

#define	UPDFCACHE_NODATACACHE			0x00000001
#define UPDFCACHE_ONSAVECACHE			0x00000002
#define	UPDFCACHE_ONSTOPCACHE			0x00000004
#define	UPDFCACHE_NORMALCACHE			0x00000008
#define	UPDFCACHE_IFBLANK				0x00000010
#define UPDFCACHE_ONLYIFBLANK			0x80000000

#define UPDFCACHE_IFBLANKORONSAVECACHE	(UPDFCACHE_IFBLANK | UPDFCACHE_ONSAVECACHE )
#define UPDFCACHE_ALL					(~UPDFCACHE_ONLYIFBLANK)
#define UPDFCACHE_ALLBUTNODATACACHE		(UPDFCACHE_ALL & ~UPDFCACHE_NODATACACHE)


// IOleCache2::DiscardCache options
typedef enum tagDISCARDCACHE
{
	DISCARDCACHE_SAVEIFDIRTY =	0,	// Save all dirty cache before discarding
	DISCARDCACHE_NOSAVE		 =	1	// Don't save dirty caches before 
									// discarding
	,DISCARDCACHE_FORCELONG	=	2147483647
} DISCARDCACHE;


#undef  INTERFACE
#define INTERFACE   IOleCache2

DECLARE_INTERFACE_(IOleCache2, IOleCache)
{ 
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleCache methods ***
    STDMETHOD(Cache) (THIS_ LPFORMATETC lpFormatetc, unsigned long advf, unsigned long	* lpdwConnection) PURE;
    STDMETHOD(Uncache) (THIS_ unsigned long dwConnection) PURE;
    STDMETHOD(EnumCache) (THIS_ LPLPENUMSTATDATA ppenumStatData) PURE;
    STDMETHOD(InitCache) (THIS_ LPDATAOBJECT pDataObject) PURE;
    STDMETHOD(SetData) (THIS_ LPFORMATETC pformatetc, STGMEDIUM  * pmedium,
                            OLEBOOL fRelease) PURE;

    // *** IOleCache2 methods ***							
    STDMETHOD(UpdateCache) (THIS_ LPDATAOBJECT pDataObject, unsigned long grfUpdf, 
							void *pReserved) PURE;
    STDMETHOD(DiscardCache) (THIS_ unsigned long dwDiscardOptions) PURE;
						
};
typedef      IOleCache2 * LPOLECACHE2, ** LPLPOLECACHE2;


#undef  INTERFACE
#define INTERFACE   IOleCacheControl

DECLARE_INTERFACE_(IOleCacheControl, IUnknown)
{ 
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IDataObject methods ***
    STDMETHOD(OnRun) (THIS_ LPDATAOBJECT pDataObject) PURE;
    STDMETHOD(OnStop) (THIS) PURE;
};                 
typedef      IOleCacheControl * LPOLECACHECONTROL, ** LPLPOLECACHECONTROL;


/****** DV APIs ***********************************************************/


STDAPI CreateDataAdviseHolder(LPLPDATAADVISEHOLDER ppDAHolder);

STDAPI CreateDataCache(LPUNKNOWN pUnkOuter, REFCLSID rclsid,
					REFIID iid, void ** ppv);
					
#endif /* !__DVOBJ__ */

/*****************************************************************************\
*                                                                             *
* ole2.h - 		Main OLE2 header; includes all subcomponents				  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/

#if !defined(__OLE2__) && !defined(_OLE2_H_)
#define __OLE2__
#define _OLE2_H_


#ifdef _WIN32
#include "macname1.h"
#endif // _WIN32


#if !defined(__MACPUB__)
#include <macpub.h>
#endif


/****** Standard Object Definitions *****************************************/

#ifndef __COMPOBJ__
#include <compobj.h>
#endif


// *************** FACILITY_ITF scodes common to all interfaces ************
//
// By convention, OLE interfaces divide the FACILITY_ITF range of errors
// into nonoverlapping subranges.  If an interface returns a FACILITY_ITF
// scode, it must be from the range associated with that interface or from
// the shared range: OLE_E_FIRST...OLE_E_LAST.
//

// error codes

#ifndef _WINERROR_

#define OLE_E_OLEVERB               (OLE_E_FIRST)
// invalid OLEVERB structure

#define OLE_E_ADVF                  (OLE_E_FIRST+1)
// invalid advise flags

#define OLE_E_ENUM_NOMORE           (OLE_E_FIRST+2)
// you can't enuemrate any more, because the associated data is missing

#define OLE_E_ADVISENOTSUPPORTED    (OLE_E_FIRST+3)
// this implementation doesn't take advises

#define OLE_E_NOCONNECTION          (OLE_E_FIRST+4)
// there is no connection for this connection id

#define OLE_E_NOTRUNNING            (OLE_E_FIRST+5)
// need run the object to perform this operation

#define OLE_E_NOCACHE               (OLE_E_FIRST+6)
// there is no cache to operate on

#define OLE_E_BLANK                 (OLE_E_FIRST+7)
// Uninitialized object

#define OLE_E_CLASSDIFF             (OLE_E_FIRST+8)
// linked object's source class has changed

#define OLE_E_CANT_GETMONIKER       (OLE_E_FIRST+9)
// not able to get the moniker of the object

#define OLE_E_CANT_BINDTOSOURCE     (OLE_E_FIRST+10)
// not able to bind to the source

#define OLE_E_STATIC                (OLE_E_FIRST+11)
// object is static, operation not allowed

#define OLE_E_PROMPTSAVECANCELLED   (OLE_E_FIRST+12)
// user cancelled out of save dialog

#define OLE_E_INVALIDRECT			(OLE_E_FIRST+13)
// invalid rectangle

#define OLE_E_WRONGCOMPOBJ          (OLE_E_FIRST+14)
// compobj.dll is too old for the ole2.dll initialized

#define OLE_E_INVALIDHWND           (OLE_E_FIRST+15)
// invalid window handle

#define OLE_E_NOT_INPLACEACTIVE     (OLE_E_FIRST+16)
// object is not in any of the inplace active states

#define OLE_E_CANTCONVERT			(OLE_E_FIRST+17)
// not able to convert the object

#define OLE_E_NOSTORAGE				(OLE_E_FIRST+18)
// not able to perform the operation because object is not given storage yet.
#endif // _WINERROR_

typedef enum {
	OLE_E_NOEXTENSION	= OLE_E_NOSTORAGE +1,	// no extension at startup
	OLE_E_VERSEXTENSION,						// extension has wrong version #
	OLE_E_IPBUSY,								// cannot get inplace resource
	OLE_E_NOT_FRONT_PROCESS,					// inplace container unexpectedly in background
	OLE_E_WRONG_MENU,							// holemenu != prev holemenu
	OLE_E_MENU_NOT_PATCHED,						// menubar is not hashed
	OLE_E_MENUID_NOT_HASHED,					// id was never hashed
	OLE_E_foo
} inplace_errors;


#ifndef _WINERROR_


#define DVGEN_E_FIRST               (OLE_E_FIRST+100)

#define DV_E_FORMATETC              (DVGEN_E_FIRST)
// invalid FORMATETC structure

#define DV_E_DVTARGETDEVICE         (DVGEN_E_FIRST+1)
// invalid DVTARGETDEVICE structure

#define DV_E_STGMEDIUM              (DVGEN_E_FIRST+2)
// invalid STDGMEDIUM structure

#define DV_E_STATDATA               (DVGEN_E_FIRST+3)
// invalid STATDATA structure

#define DV_E_LINDEX                 (DVGEN_E_FIRST+4)
// invalid lindex

#define DV_E_TYMED                  (DVGEN_E_FIRST+5)
// invalid tymed

#define DV_E_CLIPFORMAT             (DVGEN_E_FIRST+6)
// invalid clipboard format

#define DV_E_DVASPECT               (DVGEN_E_FIRST+7)
// invalid aspect(s)

#define DV_E_DVTARGETDEVICE_SIZE    (DVGEN_E_FIRST+8)
// tdSize paramter of the DVTARGETDEVICE structure is invalid

#define DV_E_NOIVIEWOBJECT          (DVGEN_E_FIRST+9)
// object doesn't support IViewObject interface


// Success codes

#define OLE_S_USEREG                (OLE_S_FIRST)
// use the reg database to provide the requested info

#define OLE_S_STATIC                (OLE_S_FIRST+1)
// success, but static

#define OLE_S_MAC_CLIPFORMAT        (OLE_S_FIRST+2)
// macintosh clipboard format

#define OLE_S_WIN_CLIPFORMAT        (OLE_S_FIRST+3)
// macintosh clipboard format

//*************************** Interface or API specific scodes *************

// Errors for OleConvertOLESTREAMToIStorage and OleConvertIStorageToOLESTREAM

// OLESTREAM Get method failed
#define CONVERT10_E_OLESTREAM_GET       (CONVERT10_E_FIRST + 0)

// OLESTREAM Put method failed
#define CONVERT10_E_OLESTREAM_PUT       (CONVERT10_E_FIRST + 1)

// Contents of the OLESTREAM not in correct format
#define CONVERT10_E_OLESTREAM_FMT       (CONVERT10_E_FIRST + 2)

// There was in an error in a Windows GDI call while converting the bitmap
// to a DIB.
#define CONVERT10_E_OLESTREAM_BITMAP_TO_DIB (CONVERT10_E_FIRST + 3)

// Contents of the IStorage not in correct format
#define CONVERT10_E_STG_FMT             (CONVERT10_E_FIRST + 4)

// Contents of IStorage is missing one of the standard streams ("\1CompObj",
// "\1Ole", "\2OlePres000").  This may be the storage for a DLL object, or a
// class that does not use the def handler.
#define CONVERT10_E_STG_NO_STD_STREAM   (CONVERT10_E_FIRST + 5)

// There was in an error in a Windows GDI call while converting the DIB
// to a bitmap.
#define CONVERT10_E_STG_DIB_TO_BITMAP   (CONVERT10_E_FIRST + 6)


// Returned by either API, this scode indicates that the original object
//  had no presentation, therefore the converted object does not either.
#define CONVERT10_S_NO_PRESENTATION     (CONVERT10_S_FIRST + 0)


// Errors for Clipboard functions

// OpenClipboard Failed
#define CLIPBRD_E_CANT_OPEN     (CLIPBRD_E_FIRST + 0)

// EmptyClipboard Failed
#define CLIPBRD_E_CANT_EMPTY        (CLIPBRD_E_FIRST + 1)

// SetClipboard Failed
#define CLIPBRD_E_CANT_SET          (CLIPBRD_E_FIRST + 2)

// Data on clipboard is invalid
#define CLIPBRD_E_BAD_DATA          (CLIPBRD_E_FIRST + 3)

// CloseClipboard Failed
#define CLIPBRD_E_CANT_CLOSE        (CLIPBRD_E_FIRST + 4)
#endif //  _WINERROR_


/****** OLE value types *****************************************************/

/* rendering options */
typedef enum tagOLERENDER
{
    OLERENDER_NONE   = 0,
    OLERENDER_DRAW   = 1,
    OLERENDER_FORMAT = 2,
    OLERENDER_ASIS   = 3

	,OLERENDER_FORCELONG	=	2147483647
} OLERENDER;
typedef  OLERENDER * LPOLERENDER;

// OLE verb; returned by IEnumOLEVERB
typedef struct FARSTRUCT tagOLEVERB
{
    long			lVerb;
    char *			lpszVerbName;
    unsigned long	fuFlags;
    unsigned long	grfAttribs;
} OLEVERB, * LPOLEVERB;


// Bitwise verb attributes used in OLEVERB.grfAttribs
typedef enum tagOLEVERBATTRIB // bitwise
{
    OLEVERBATTRIB_NEVERDIRTIES = 1,
    OLEVERBATTRIB_ONCONTAINERMENU = 2

	,OLEVERBATTRIB_FORCELONG	=	2147483647
} OLEVERBATTRIB;


// IOleObject::GetUserType optons; determines which form of the string to use
typedef enum tagUSERCLASSTYPE
{
    USERCLASSTYPE_FULL = 1,
    USERCLASSTYPE_SHORT= 2,
    USERCLASSTYPE_APPNAME= 3
	,USERCLASSTYPE_FORCELONG	=	2147483647
} USERCLASSTYPE;


// bits returned from IOleObject::GetMistStatus
typedef enum tagOLEMISC // bitwise
{
    OLEMISC_RECOMPOSEONRESIZE = 1,
    OLEMISC_ONLYICONIC        = 2,
    OLEMISC_INSERTNOTREPLACE  = 4,
    OLEMISC_STATIC            = 8,
    OLEMISC_CANTLINKINSIDE    = 16,
    OLEMISC_CANLINKBYOLE1       = 32,
    OLEMISC_ISLINKOBJECT        = 64,
    OLEMISC_INSIDEOUT           = 128,
    OLEMISC_ACTIVATEWHENVISIBLE = 256
	,OLEMISC_FORCELONG	=	2147483647
} OLEMISC;


// IOleObject::Close options
typedef enum tagOLECLOSE
{
    OLECLOSE_SAVEIFDIRTY = 0,
    OLECLOSE_NOSAVE      = 1,
    OLECLOSE_PROMPTSAVE  = 2
	,OLECLOSE_FORCELONG	=	2147483647
} OLECLOSE;


// IOleObject::GetMoniker and IOleClientSite::GetMoniker options; determines
// if and how monikers should be assigned.
typedef enum tagOLEGETMONIKER
{
    OLEGETMONIKER_ONLYIFTHERE=1,
    OLEGETMONIKER_FORCEASSIGN=2,
    OLEGETMONIKER_UNASSIGN=3,
    OLEGETMONIKER_TEMPFORUSER=4
	,OLEGETMONIKER_FORCELONG	=	2147483647
} OLEGETMONIKER;


// IOleObject::GetMoniker, IOleObject::SetMoniker and
// IOleClientSite::GetMoniker options; determines which moniker to use
typedef enum tagOLEWHICHMK
{
    OLEWHICHMK_CONTAINER=1,
    OLEWHICHMK_OBJREL=2,
    OLEWHICHMK_OBJFULL=3
	,OLEWHICHMK_FORCELONG	=	2147483647
} OLEWHICHMK;

// DoDragDropMac()
typedef struct tagDRAG_FLAVORINFO {
	unsigned long	cfFormat;
	unsigned long	FlavorFlags;		// Flavor Flags as specified by the DragManager
	OLEBOOL			fAddData;			// If true, the data is added with the flavor
	long			reserved;			// must be NULL
} DRAG_FLAVORINFO;


#ifndef _WINDEF_

typedef struct FARSTRUCT tagSIZEL
{
    long cx;
    long cy;
} SIZEL;

typedef struct FARSTRUCT tagRECTL
{
    long    left;
    long    top;
    long    right;
    long    bottom;
} RECTL;

typedef struct FARSTRUCT tagPOINTL {
    long x;
    long y;
} POINTL;

#endif

typedef SIZEL * LPSIZEL;
typedef RECTL * LPRECTL;

#ifndef LPCRECTL
typedef const RECTL * LPCRECTL;
#endif

// for OleCreateEmbeddingHelper flags; roles in low word; options in high word
#define EMBDHLP_INPROC_HANDLER   0x0000L // role is handler; implementation is
										 // default handler; pCF can be NULL
#define EMBDHLP_INPROC_SERVER    0x0001L // role is server; pCF can't be NULL

#define EMBDHLP_CREATENOW    0x00000000L // create using pCF immediately; if pCF
										 // is NULL, uses std remoting handler
#define EMBDHLP_DELAYCREATE  0x00010000L // delayed create; must supply pCF


// NOTE: OleCreateEmbeddingHelper(clsid, pUnkOuter,
// 		EMBDHLP_INPROC_HANDLER | EMBDHLP_CREATENOW, NULL, riid, lplpObj)
// is the same as OleCreateDefaultHandler(clsid, pUnkOuter, riid, lplpObj);
// i.e., the embedding helper is the default handler in various roles.


/***** OLE 1.0 OLESTREAM declarations *************************************/

typedef struct _OLESTREAM *  LPOLESTREAM;

typedef struct _OLESTREAMVTBL
{
#ifndef _MSC_VER
	pascal unsigned long (* Get)(LPOLESTREAM, void *, unsigned long);
	pascal unsigned long (* Put)(LPOLESTREAM, const void *, unsigned long);
#else
	#ifdef __powerc
	unsigned long (* Get)(LPOLESTREAM, void *, unsigned long);
	unsigned long (* Put)(LPOLESTREAM, const void *, unsigned long);
	#else
	unsigned long ( __pascal * Get)(LPOLESTREAM, void *, unsigned long);
	unsigned long ( __pascal * Put)(LPOLESTREAM, const void *, unsigned long);
	#endif
#endif
} OLESTREAMVTBL;
typedef  OLESTREAMVTBL *  LPOLESTREAMVTBL;

typedef struct _OLESTREAM
{
    LPOLESTREAMVTBL lpstbl;
} OLESTREAM;


/****** Clipboard Data structures *****************************************/

typedef struct tagOBJECTDESCRIPTOR
{
	unsigned long	cbSize;         	// Size of structure in bytes
	CLSID			clsid;            	// CLSID of data being transferred
	unsigned long	dwDrawAspect;  		// Display aspect of the object
										//     normally DVASPECT_CONTENT or ICON.
										//     dwDrawAspect will be 0 (which is NOT
										//     DVASPECT_CONTENT) if the copier or
										//     dragsource didn't draw the object to
										//     begin with.
	SIZEL			sizel;           	// size of the object in HIMETRIC
										//    sizel is opt.: will be (0,0) for apps
										//    which don't draw the object being
										//    transferred
	unsigned long	dwOutline;			// Offset from beginning of structure to
										//    Region describing outline of object
										//    Like sizel, this is only defined for
										//    drawable objects, else it is zero.
										//	  To turn this data at this offset into
										//	  a region handle, allocate a handle
										//    the size of the first word of the region data
										//	  and then copy those n bytes into the handle
										//	  (including the first word)
										//			char *p = ((char*) pOD) + pOD->dwOutline; // ptr to region data
										//			short size = *(short *) p;
										//			h = NewHandle(size);
										//			BlockMove(p, *h, size);
	POINTL   		pointl;        		// Offset in HIMETRIC units from the
										//    upper-left corner of the obj where the
										//    mouse went down for the drag.
										//    NOTE: y coordinates increase downward.
										//          x coordinates increase to right
										//    pointl is opt.; it is only meaningful
										//    if object is transfered via drag/drop.
                                 		//    (0, 0) if mouse position is unspecified
                                		//    (eg. when obj transfered via clipboard)
	unsigned long	dwStatus; 			// Misc. status flags for object. Flags are
							     		//    defined by OLEMISC enum. these flags
										//    are as would be returned
                                 		//    by IOleObject::GetMiscStatus.
	unsigned long	dwFullUserTypeName;	// Offset from beginning of structure to
										//    null-terminated string that specifies
										//    Full User Type Name of the object.
										//    0 indicates string not present.
	unsigned long	dwSrcOfCopy;   		// Offset from beginning of structure to
										//    null-terminated string that specifies
										//    source of the transfer.
										//    dwSrcOfCOpy is normally implemented as
										//    the display name of the temp-for-user
										//    moniker which identifies the source of
										//    the data.
										//    0 indicates string not present.
										//    NOTE: moniker assignment is NOT forced.
										//    see IOleObject::GetMoniker(
										//                OLEGETMONIKER_TEMPFORUSER)
	unsigned long	dwExtra;			// must be null
 /* variable sized string data may appear here */

} OBJECTDESCRIPTOR,  *POBJECTDESCRIPTOR,   *LPOBJECTDESCRIPTOR,
  LINKSRCDESCRIPTOR, *PLINKSRCDESCRIPTOR,  *LPLINKSRCDESCRIPTOR;



/* verbs */
#define OLEIVERB_PRIMARY     (0L)
#define OLEIVERB_SHOW        (-1L)
#define OLEIVERB_OPEN        (-2L)
#define OLEIVERB_HIDE        (-3L)
#define OLEIVERB_UIACTIVATE         (-4L)
#define OLEIVERB_INPLACEACTIVATE    (-5L)
#define OLEIVERB_DISCARDUNDOSTATE   (-6L)


//      forward type declarations
#if defined(__cplusplus)
interface IOleClientSite;
interface IOleContainer;
interface IOleObject;
#else
//typedef interface IOleClientSite IOleClientSite;
//typedef interface IOleContainer IOleContainer;
//typedef interface IOleObject IOleObject;
#endif

typedef   interface IOleObject * LPOLEOBJECT, ** LPLPOLEOBJECT;
typedef   interface IOleClientSite * LPOLECLIENTSITE, ** LPLPOLECLIENTSITE;
typedef   interface IOleContainer * LPOLECONTAINER, ** LPLPOLECONTAINER;


/****** OLE GUIDs *********************************************************/

#ifndef INITGUID
#include "oleguid.h"
#endif


/****** Other Major Interfaces ********************************************/

#ifndef __DVOBJ__
#include <dvobj.h>
#endif

#ifndef __STORAGE__
#include <storage.h>
#endif



/****** IDrop??? Interfaces ********************************************/



#define DROPEFFECT_NONE     0
#define DROPEFFECT_COPY     1
#define DROPEFFECT_MOVE     2
#define DROPEFFECT_LINK     4
#define DROPEFFECT_SCROLL   0x80000000

// default inset-width of the hot zone, in pixels
#define DD_DEFSCROLLINSET 15

// default delay before scrolling, in ticks (sixtieths of a second)
// Better is to use GetDblTime()
#define DD_DEFSCROLLDELAY		10

// default scroll interval, in ticks
#define DD_DEFSCROLLINTERVAL	2

// default delay before dragging should start, in ticks
#define DD_DEFDRAGDELAY     	10

// default minimum distance (radius) before dragging should start, in pixels
#define DD_DEFDRAGMINDIST		2


#ifndef _WINERROR_

/* Dragdrop specific error codes */

#define	DRAGDROP_E_NOTREGISTERED		(DRAGDROP_E_FIRST)
// trying to revoke a drop target that has not been registered

#define	DRAGDROP_E_ALREADYREGISTERED	(DRAGDROP_E_FIRST+1)
// this window has already been registered as a drop target

#define DRAGDROP_E_INVALIDHWND			(DRAGDROP_E_FIRST+2)
// invalid Handle

#define DRAGDROP_S_DROP                 (DRAGDROP_S_FIRST + 0)
// successful drop took place

#define DRAGDROP_S_CANCEL               (DRAGDROP_S_FIRST + 1)
// drag-drop operation canceled

#define DRAGDROP_S_USEDEFAULTCURSORS    (DRAGDROP_S_FIRST + 2)
// use the default cursor
#endif //  _WINERROR_


#undef INTERFACE
#define INTERFACE   IDropTarget


DECLARE_INTERFACE_(IDropTarget, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IDropTarget methods ***
    STDMETHOD(DragEnter) (THIS_ LPDATAOBJECT pDataObj,
				unsigned long grfKeyState, POINTL pt,
				unsigned long * pdwEffect) PURE;
    STDMETHOD(DragOver) (THIS_ unsigned long grfKeyState, POINTL pt, unsigned long * pdwEffect) PURE;
    STDMETHOD(DragLeave) (THIS) PURE;
    STDMETHOD(Drop) (THIS_ LPDATAOBJECT pDataObj, unsigned long grfKeyState, POINTL pt, unsigned long * pdwEffect) PURE;
};
typedef         IDropTarget * LPDROPTARGET, ** LPLPDROPTARGET;



#undef INTERFACE
#define INTERFACE   IDropSource

DECLARE_INTERFACE_(IDropSource, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IDropSource methods ***
    STDMETHOD(QueryContinueDrag) (THIS_ OLEBOOL fEscapePressed, unsigned long grfKeyState) PURE;
    STDMETHOD(GiveFeedback) (THIS_ unsigned long dwEffect) PURE;
};
typedef         IDropSource * LPDROPSOURCE, ** LPLPDROPSOURCE;



#undef INTERFACE
#define INTERFACE   IMacDragHelper

DECLARE_INTERFACE_(IMacDragHelper, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IMacDragHelper methods ***
    STDMETHOD(GetDragReference) (THIS_  DragReference *theDragRef) PURE;
    STDMETHOD(ShowDragHilite) (THIS_ GrafPtr port, RgnHandle hiliteFrame,OLEBOOL inside, unsigned long refCon) PURE;
    STDMETHOD(UpdateDragHilite) (THIS_ GrafPtr port, RgnHandle updateRgn, unsigned long refCon) PURE;
    STDMETHOD(HideDragHilite) (THIS_ GrafPtr port) PURE;
    STDMETHOD(GetHiliteRefCon) (THIS_ GrafPtr port, unsigned long *prefCon) PURE;
    STDMETHOD(DragPreScroll) (THIS_  GrafPtr port, unsigned long dH,unsigned long dV) PURE;
    STDMETHOD(DragPostScroll) (THIS_ GrafPtr port) PURE;
    STDMETHOD(IsOLEDrag) (THIS) PURE;
};
typedef			IMacDragHelper * LPMACDRAGHELPER;

/****** IPersist??? Interfaces ********************************************/


#undef INTERFACE
#define INTERFACE   IPersist

DECLARE_INTERFACE_(IPersist, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IPersist methods ***
    STDMETHOD(GetClassID) (THIS_ LPCLSID lpClassID) PURE;
};
typedef            IPersist * LPPERSIST, ** LPLPPERSIST;



#undef INTERFACE
#define INTERFACE   IPersistStorage

DECLARE_INTERFACE_(IPersistStorage, IPersist)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IPersist methods ***
    STDMETHOD(GetClassID) (THIS_ LPCLSID lpClassID) PURE;

    // *** IPersistStorage methods ***
    STDMETHOD(IsDirty) (THIS) PURE;
    STDMETHOD(InitNew) (THIS_ LPSTORAGE pStg) PURE;
    STDMETHOD(Load) (THIS_ LPSTORAGE pStg) PURE;
    STDMETHOD(Save) (THIS_ LPSTORAGE pStgSave, OLEBOOL fSameAsLoad) PURE;
    STDMETHOD(SaveCompleted) (THIS_ LPSTORAGE pStgNew) PURE;
    STDMETHOD(HandsOffStorage) (THIS) PURE;
};
typedef         IPersistStorage * LPPERSISTSTORAGE, ** LPLPPERSISTSTORAGE;



#undef INTERFACE
#define INTERFACE   IPersistStream

DECLARE_INTERFACE_(IPersistStream, IPersist)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IPersist methods ***
    STDMETHOD(GetClassID) (THIS_ LPCLSID lpClassID) PURE;

    // *** IPersistStream methods ***
    STDMETHOD(IsDirty) (THIS) PURE;
    STDMETHOD(Load) (THIS_ LPSTREAM pStm) PURE;
    STDMETHOD(Save) (THIS_ LPSTREAM pStm,
                    OLEBOOL fClearDirty) PURE;
    STDMETHOD(GetSizeMax) (THIS_ ULARGE_INTEGER  * pcbSize) PURE;
};
typedef          IPersistStream * LPPERSISTSTREAM, ** LPLPPERSISTSTREAM;



#undef INTERFACE
#define INTERFACE   IPersistFile

DECLARE_INTERFACE_(IPersistFile, IPersist)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IPersist methods ***
    STDMETHOD(GetClassID) (THIS_ LPCLSID lpClassID) PURE;

    // *** IPersistFile methods ***
    STDMETHOD(IsDirty) (THIS) PURE;
    STDMETHOD(Load) (THIS_ const char * lpszFileName, unsigned long grfMode) PURE;
    STDMETHOD(Save) (THIS_ const char * lpszFileName, OLEBOOL fRemember) PURE;
    STDMETHOD(SaveCompleted) (THIS_ const char * lpszFileName) PURE;
    STDMETHOD(GetCurFile) (THIS_ char *  * lplpszFileName) PURE;

 	STDMETHOD(LoadFSP) (THIS_ const FSSpec *pFSSpec, unsigned long grfMode) PURE;
    STDMETHOD(SaveFSP) (THIS_ const FSSpec *pFSSpec, OLEBOOL fRemember) PURE;
    STDMETHOD(SaveCompletedFSP) (THIS_ const FSSpec *pFSSpec) PURE;
    STDMETHOD(GetCurFSP) (THIS_ FSSpec *pFSSpec) PURE;
};
typedef            IPersistFile * LPPERSISTFILE, ** LPLPPERSISTFILE;


/****** Moniker Object Interfaces ******************************************/

#ifndef __MONIKER__
#include <moniker.h>
#endif


/****** OLE Object Interfaces ******************************************/


#undef  INTERFACE
#define INTERFACE   IEnumOLEVERB

DECLARE_INTERFACE_(IEnumOLEVERB, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IEnumOLEVERB methods ***
    STDMETHOD(Next) (THIS_ unsigned long celt, LPOLEVERB rgelt, unsigned long * pceltFetched) PURE;
    STDMETHOD(Skip) (THIS_ unsigned long celt) PURE;
    STDMETHOD(Reset) (THIS) PURE;
    STDMETHOD(Clone) (THIS_ IEnumOLEVERB * * ppenm) PURE;
};
typedef         IEnumOLEVERB * LPENUMOLEVERB, ** LPLPENUMOLEVERB;




#undef  INTERFACE
#define INTERFACE   IOleObject

#ifndef _WINERROR_
#define OLEOBJ_E_NOVERBS                (OLEOBJ_E_FIRST + 0)

#define OLEOBJ_E_INVALIDVERB            (OLEOBJ_E_FIRST + 1)

#define OLEOBJ_S_INVALIDVERB            (OLEOBJ_S_FIRST + 0)

#define OLEOBJ_S_CANNOT_DOVERB_NOW      (OLEOBJ_S_FIRST + 1)
// verb number is valid but verb cannot be done now, for instance
// hiding a link or hiding a visible OLE 1.0 server

#define OLEOBJ_S_INVALIDHWND            (OLEOBJ_S_FIRST + 2)
// invalid hwnd passed

#endif // _WINERROR_

DECLARE_INTERFACE_(IOleObject, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleObject methods ***
    STDMETHOD(SetClientSite) (THIS_ LPOLECLIENTSITE pClientSite) PURE;
    STDMETHOD(GetClientSite) (THIS_ LPLPOLECLIENTSITE ppClientSite) PURE;
    STDMETHOD(SetHostNames) (THIS_ const char * szContainerApp, const char * szContainerObj) PURE;
    STDMETHOD(Close) (THIS_ unsigned long dwSaveOption) PURE;
    STDMETHOD(SetMoniker) (THIS_ unsigned long dwWhichMoniker, LPMONIKER pmk) PURE;
    STDMETHOD(GetMoniker) (THIS_ unsigned long dwAssign, unsigned long dwWhichMoniker,
                LPLPMONIKER ppmk) PURE;
    STDMETHOD(InitFromData) (THIS_ LPDATAOBJECT pDataObject,
                OLEBOOL fCreation,
                unsigned long dwReserved) PURE;
    STDMETHOD(GetClipboardData) (THIS_ unsigned long dwReserved,
                LPLPDATAOBJECT ppDataObject) PURE;
    STDMETHOD(DoVerb) (THIS_ long iVerb,
                EventRecord * pEvt,
                LPOLECLIENTSITE pActiveSite,
                long lindex,
                WindowPtr pWndParent,
                const Rect* lprcPosRect) PURE;
    STDMETHOD(EnumVerbs) (THIS_ LPLPENUMOLEVERB ppenumOleVerb) PURE;
    STDMETHOD(Update) (THIS) PURE;
    STDMETHOD(IsUpToDate) (THIS) PURE;
    STDMETHOD(GetUserClassID) (THIS_ CLSID * pClsid) PURE;
    STDMETHOD(GetUserType) (THIS_ unsigned long dwFormOfType, char * * pszUserType) PURE;
    STDMETHOD(SetExtent) (THIS_ unsigned long dwDrawAspect, LPSIZEL lpsizel) PURE;
    STDMETHOD(GetExtent) (THIS_ unsigned long dwDrawAspect, LPSIZEL lpsizel) PURE;
    STDMETHOD(Advise)(THIS_ LPADVISESINK pAdvSink, unsigned long * pdwConnection) PURE;
    STDMETHOD(Unadvise)(THIS_ unsigned long dwConnection) PURE;
    STDMETHOD(EnumAdvise) (THIS_ LPLPENUMSTATDATA ppenumAdvise) PURE;
    STDMETHOD(GetMiscStatus) (THIS_ unsigned long dwAspect, unsigned long * pdwStatus) PURE;
    STDMETHOD(SetColorScheme) (THIS_ LPOLECOLORSCHEME lpColorScheme) PURE;
};
// MAC Think C support
//typedef      IOleObject * LPOLEOBJECT, ** LPLPOLEOBJECT;



#undef  INTERFACE
#define INTERFACE   IOleClientSite

DECLARE_INTERFACE_(IOleClientSite, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleClientSite methods ***
    STDMETHOD(SaveObject) (THIS) PURE;
    STDMETHOD(GetMoniker) (THIS_ unsigned long dwAssign, unsigned long dwWhichMoniker,
                LPLPMONIKER ppmk) PURE;
    STDMETHOD(GetContainer) (THIS_ LPLPOLECONTAINER ppContainer) PURE;
    STDMETHOD(ShowObject) (THIS) PURE;
    STDMETHOD(OnShowWindow) (THIS_ OLEBOOL fShow) PURE;
    STDMETHOD(RequestNewObjectLayout) (THIS) PURE;
};
// MAC Think C support
//typedef      IOleClientSite * LPOLECLIENTSITE, ** LPLPOLECLIENTSITE;

/****** OLE Runnable Object Interface **********************************/

#undef  INTERFACE
#define INTERFACE	IRunnableObject

DECLARE_INTERFACE_(IRunnableObject, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

	// *** IRunnableObject methods ***
    STDMETHOD(GetRunningClass) (THIS_ LPCLSID lpClsid) PURE;
    STDMETHOD(Run) (THIS_ LPBINDCTX pbc) PURE;
    STDMETHOD_(OLEBOOL, IsRunning) (THIS) PURE;
	STDMETHOD(LockRunning)(THIS_ OLEBOOL fLock, OLEBOOL fLastUnlockCloses) PURE;
	STDMETHOD(SetContainedObject)(THIS_ OLEBOOL fContained) PURE;
};
typedef      IRunnableObject * LPRUNNABLEOBJECT, ** LPLPRUNNABLEOBJECT;


/****** OLE Container Interfaces ***************************************/


#undef  INTERFACE
#define INTERFACE   IParseDisplayName

DECLARE_INTERFACE_(IParseDisplayName, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IParseDisplayName method ***
    STDMETHOD(ParseDisplayName) (THIS_ LPBC pbc, char * lpszDisplayName,
        unsigned long * pchEaten, LPLPMONIKER ppmkOut) PURE;
};
typedef       IParseDisplayName * LPPARSEDISPLAYNAME, ** LPLPPARSEDISPLAYNAME;


#undef  INTERFACE
#define INTERFACE   IOleContainer

DECLARE_INTERFACE_(IOleContainer, IParseDisplayName)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IParseDisplayName method ***
    STDMETHOD(ParseDisplayName) (THIS_ LPBC pbc, char * lpszDisplayName,
        unsigned long * pchEaten, LPLPMONIKER ppmkOut) PURE;

    // *** IOleContainer methods ***
    STDMETHOD(EnumObjects) ( THIS_ unsigned long grfFlags, LPLPENUMUNKNOWN ppenumUnknown) PURE;
    STDMETHOD(LockContainer) (THIS_ OLEBOOL fLock) PURE;
};
//typedef IOleContainer * LPOLECONTAINER, ** LPLPOLECONTAINER;


typedef enum tagBINDSPEED
{
    BINDSPEED_INDEFINITE    = 1,
    BINDSPEED_MODERATE      = 2,
    BINDSPEED_IMMEDIATE     = 3
	,BINDSPEED_FORCELONG	=	2147483647
} BINDSPEED;

typedef enum tagOLECONTF
{
	OLECONTF_EMBEDDINGS		=  1,
	OLECONTF_LINKS 			=  2,
	OLECONTF_OTHERS 		=  4,
	OLECONTF_ONLYUSER 		=  8,
	OLECONTF_ONLYIFRUNNING 	= 16
	,OLECONTF_FORCELONG	=	2147483647
} OLECONTF;


#undef  INTERFACE
#define INTERFACE   IOleItemContainer

DECLARE_INTERFACE_(IOleItemContainer, IOleContainer)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IParseDisplayName method ***
    STDMETHOD(ParseDisplayName) (THIS_ LPBC pbc, char * lpszDisplayName,
        unsigned long * pchEaten, LPLPMONIKER ppmkOut) PURE;

    // *** IOleContainer methods ***
    STDMETHOD(EnumObjects) (THIS_ unsigned long grfFlags, LPLPENUMUNKNOWN ppenumUnknown) PURE;
    STDMETHOD(LockContainer) (THIS_ OLEBOOL fLock) PURE;

    // *** IOleItemContainer methods ***
    STDMETHOD(GetObject) (THIS_ char * lpszItem, unsigned long dwSpeedNeeded,
        LPBINDCTX pbc, REFIID riid, void * * ppvObject) PURE;
    STDMETHOD(GetObjectStorage) (THIS_ char * lpszItem, LPBINDCTX pbc,
        REFIID riid, void * * ppvStorage) PURE;
    STDMETHOD(IsRunning) (THIS_ char * lpszItem) PURE;
};
typedef       IOleItemContainer * LPOLEITEMCONTAINER, ** LPLPOLEITEMCONTAINER;


/****** OLE Advise Holder Interface ***************************************/


#undef  INTERFACE
#define INTERFACE   IOleAdviseHolder

DECLARE_INTERFACE_(IOleAdviseHolder, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppv) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleAdviseHolder methods ***
    STDMETHOD(Advise)(THIS_ LPADVISESINK pAdvise, unsigned long * pdwConnection) PURE;
    STDMETHOD(Unadvise)(THIS_ unsigned long dwConnection) PURE;
    STDMETHOD(EnumAdvise)(THIS_ LPLPENUMSTATDATA ppenumAdvise) PURE;

    STDMETHOD(SendOnRename)(THIS_ LPMONIKER pmk) PURE;
    STDMETHOD(SendOnSave)(THIS) PURE;
    STDMETHOD(SendOnClose)(THIS) PURE;
};
typedef      IOleAdviseHolder * LPOLEADVISEHOLDER, ** LPLPOLEADVISEHOLDER;


/****** OLE Link Interface ************************************************/

/* Link update options */
typedef enum tagOLEUPDATE
{
    OLEUPDATE_ALWAYS=1,
    OLEUPDATE_ONCALL=3
	,OLEUPDATE_FORCELONG	=	2147483647
} OLEUPDATE;
typedef  OLEUPDATE * LPOLEUPDATE;


// for IOleLink::BindToSource
typedef enum tagOLELINKBIND
{
    OLELINKBIND_EVENIFCLASSDIFF = 1
	,OLELINKBIND_FORCELONG	=	2147483647
} OLELINKBIND;


#undef  INTERFACE
#define INTERFACE   IOleLink

DECLARE_INTERFACE_(IOleLink, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleLink methods ***
    STDMETHOD(SetUpdateOptions) (THIS_ unsigned long dwUpdateOpt) PURE;
    STDMETHOD(GetUpdateOptions) (THIS_ unsigned long * pdwUpdateOpt) PURE;
    STDMETHOD(SetSourceMoniker) (THIS_ LPMONIKER pmk, REFCLSID rclsid) PURE;
    STDMETHOD(GetSourceMoniker) (THIS_ LPLPMONIKER ppmk) PURE;
    STDMETHOD(SetSourceDisplayName) (THIS_ const char * lpszDisplayName) PURE;
    STDMETHOD(GetSourceDisplayName) (THIS_ char * * lplpszDisplayName) PURE;
    STDMETHOD(BindToSource) (THIS_ unsigned long bindflags, LPBINDCTX pbc) PURE;
    STDMETHOD(BindIfRunning) (THIS) PURE;
    STDMETHOD(GetBoundSource) (THIS_ LPLPUNKNOWN ppUnk) PURE;
    STDMETHOD(UnbindSource) (THIS) PURE;
    STDMETHOD(Update) (THIS_ LPBINDCTX pbc) PURE;
};
typedef         IOleLink * LPOLELINK, ** LPLPOLELINK;


/****** OLE InPlace Editing Interfaces ************************************/


typedef struct FARSTRUCT tagOIFI          // OleInPlaceFrameInfo
{
	long				recordLength;	// (2) == sizeof(OLEINPLACEFRAMEINFO)
	long				version;		// (2) vers == 1
    WindowPtr  			frameWindow;	// (4)
	OSType				signature;		// (4) creator signature
	long				refcon;			// (4) anything you want
	ProcessSerialNumber	psn;			// (8) psn
    Handle 				hCmdKeys;		// (4) must be null
    short				numCmds;		// (2) must be zero
	short				growHandles;	// (2) which grow handles allowed in server
	short				dragConstraint;	// (2) which direction dragging allowed in server
    Boolean   			fAdjustMenus;	// (1) cont needs ::AdjustMenus called?
    Boolean   			unused;			// (1) unused
} OLEINPLACEFRAMEINFO, * LPOLEINPLACEFRAMEINFO;


typedef struct tagOleMBarRec
{
	long reserved;
} OleMBarRec, *OleMBarPtr, **OleMBarHandle;

#define noAxis	3		// indicates no grow or size in any direction
// see mac #defines noConstraint, hAxisOnly, vAxisOnly

typedef Rect			BORDERWIDTHS;
typedef Rect *			LPBORDERWIDTHS;
typedef const Rect *	LPCBORDERWIDTHS;

/* Inplace editing specific error codes */

#ifndef _WINERROR_

#define	INPLACE_E_NOTUNDOABLE		(INPLACE_E_FIRST)
// undo is not avaiable

#define INPLACE_E_NOTOOLSPACE		(INPLACE_E_FIRST+1)
// Space for tools is not available

#define INPLACE_S_TRUNCATED			(INPLACE_S_FIRST)
// Message is too long, some of it had to be truncated before displaying

#endif // _WINERROR_

//misc definitions
#define INPLACE_DEFBORDERWIDTH		4

//      forward type declarations
#if defined(__cplusplus)
interface IOleInPlaceUIWindow;
#else
//typedef interface IOleInPlaceUIWindow IOleInPlaceUIWindow;
#endif

typedef   interface  IOleInPlaceUIWindow * LPOLEINPLACEUIWINDOW, ** LPLPOLEINPLACEUIWINDOW;


#undef  INTERFACE
#define INTERFACE   IOleWindow

DECLARE_INTERFACE_(IOleWindow, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleWindow methods ***
    STDMETHOD(GetWindow) (THIS_ WindowPtr * ppWnd) PURE;
    STDMETHOD(ContextSensitiveHelp) (THIS_ OLEBOOL fEnterMode) PURE;
};

typedef         IOleWindow * LPOLEWINDOW, ** LPLPOLEWINDOW;



#undef  INTERFACE
#define INTERFACE   IOleInPlaceObject

DECLARE_INTERFACE_(IOleInPlaceObject, IOleWindow)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleWindow methods ***
    STDMETHOD(GetWindow) (THIS_ WindowPtr * ppWnd) PURE;
    STDMETHOD(ContextSensitiveHelp) (THIS_ OLEBOOL fEnterMode) PURE;

    // *** IOleInPlaceObject methods ***
    STDMETHOD(InPlaceDeactivate) (THIS) PURE;
    STDMETHOD(UIDeactivate) (THIS) PURE;
    STDMETHOD(SetObjectRects) (THIS_ const Rect* lprcPosRect,
					RgnHandle clipRgn,		// cliping area within containing window
					RgnHandle frameRgn,		// entire structure rgn of containing window
					RgnHandle cliRgn		// union of all structure rgns of all container app windows	
					) PURE;
    STDMETHOD(ReactivateAndUndo) (THIS) PURE;
};
typedef         IOleInPlaceObject * LPOLEINPLACEOBJECT, ** LPLPOLEINPLACEOBJECT;



#undef  INTERFACE
#define INTERFACE   IOleInPlaceActiveObject

DECLARE_INTERFACE_(IOleInPlaceActiveObject, IOleWindow)
{
    BEGIN_INTERFACE
   // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleWindow methods ***
    STDMETHOD(GetWindow) (THIS_ WindowPtr * ppWnd) PURE;
    STDMETHOD(ContextSensitiveHelp) (THIS_ OLEBOOL fEnterMode) PURE;

    // *** IOleInPlaceActiveObject methods ***
    STDMETHOD(TranslateAccelerator) (THIS_ EventRecord * pEvt) PURE;
    STDMETHOD(OnFrameWindowActivate) (THIS_ OLEBOOL fActivate) PURE;
    STDMETHOD(OnDocWindowActivate) (THIS_ OLEBOOL fActivate) PURE;
    STDMETHOD(ResizeBorder) (THIS_ const Rect* lprectBorder, LPOLEINPLACEUIWINDOW lpUIWindow, OLEBOOL fFrameWindow) PURE;
    STDMETHOD(EnableModeless) (THIS_ OLEBOOL fEnable) PURE;
};
typedef         IOleInPlaceActiveObject * LPOLEINPLACEACTIVEOBJECT, ** LPLPOLEINPLACEACTIVEOBJECT;

#undef  INTERFACE
#define INTERFACE   IOleInPlaceUIWindow

DECLARE_INTERFACE_(IOleInPlaceUIWindow, IOleWindow)
{
    BEGIN_INTERFACE
   // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleWindow methods ***
    STDMETHOD(GetWindow) (THIS_ WindowPtr * ppWnd) PURE;
    STDMETHOD(ContextSensitiveHelp) (THIS_ OLEBOOL fEnterMode) PURE;

    // *** IOleInPlaceUIWindow methods ***
    STDMETHOD(GetBorder) (THIS_ Rect * lprectBorder) PURE;
    STDMETHOD(RequestBorderSpace) (THIS_ LPCBORDERWIDTHS lpborderwidths) PURE;
    STDMETHOD(SetBorderSpace) (THIS_ LPCBORDERWIDTHS lpborderwidths) PURE;
    STDMETHOD(SetActiveObject) (THIS_ LPOLEINPLACEACTIVEOBJECT lpActiveObject,
                        const char * lpszObjName) PURE;
};
//typedef     IOleInPlaceUIWindow * LPOLEINPLACEUIWINDOW, ** LPLPOLEINPLACEUIWINDOW;



#undef  INTERFACE
#define INTERFACE   IOleInPlaceFrame


DECLARE_INTERFACE_(IOleInPlaceFrame, IOleInPlaceUIWindow)
{
    BEGIN_INTERFACE
   // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleWindow methods ***
    STDMETHOD(GetWindow) (THIS_ WindowPtr * ppWnd) PURE;
    STDMETHOD(ContextSensitiveHelp) (THIS_ OLEBOOL fEnterMode) PURE;

    // *** IOleInPlaceUIWindow methods ***
    STDMETHOD(GetBorder) (THIS_ Rect * lprectBorder) PURE;
    STDMETHOD(RequestBorderSpace) (THIS_ LPCBORDERWIDTHS lpborderwidths) PURE;
    STDMETHOD(SetBorderSpace) (THIS_ LPCBORDERWIDTHS lpborderwidths) PURE;
    STDMETHOD(SetActiveObject) (THIS_ LPOLEINPLACEACTIVEOBJECT lpActiveObject,
                    const char * lpszObjName) PURE;


    // *** IOleInPlaceFrame methods ***
    STDMETHOD(InsertMenus) (THIS_ OleMBarHandle hOleMBar) PURE;
    STDMETHOD(AdjustMenus) (THIS_ OleMBarHandle hOleMBar) PURE;
    STDMETHOD(RemoveMenus) (THIS_ OleMBarHandle hOleMBar) PURE;
    STDMETHOD(SetStatusText) (THIS_ const char * lpszStatusText) PURE;
    STDMETHOD(EnableModeless) (THIS_ OLEBOOL fEnable) PURE;
	STDMETHOD(TranslateAccelerator) (THIS_ EventRecord * pEvt, long ID) PURE;
};
typedef     IOleInPlaceFrame * LPOLEINPLACEFRAME, ** LPLPOLEINPLACEFRAME;


#undef  INTERFACE
#define INTERFACE   IOleInPlaceSite


DECLARE_INTERFACE_(IOleInPlaceSite, IOleWindow)
{
    BEGIN_INTERFACE
   // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IOleWindow methods ***
    STDMETHOD(GetWindow) (THIS_ WindowPtr * ppWnd) PURE;
    STDMETHOD(ContextSensitiveHelp) (THIS_ OLEBOOL fEnterMode) PURE;

    // *** IOleInPlaceSite methods ***
    STDMETHOD(CanInPlaceActivate) (THIS) PURE;
    STDMETHOD(OnInPlaceActivate) (THIS) PURE;
    STDMETHOD(OnUIActivate) (THIS) PURE;
	STDMETHOD(OnUIVisible) (THIS_ OLEBOOL fVisible) PURE;	// true if visibile

    STDMETHOD(GetObjectRects) (THIS_
                	Rect * lprcPosRect,
				// server allocs, container fills, server frees
					RgnHandle clipRgn,		// defines vis rgn
					RgnHandle frameRgn,		// containing window strucRgn
					RgnHandle cliRgn		// all container app strucRgns
                  	) PURE;
    STDMETHOD(GetWindowContext) (THIS_
						LPLPOLEINPLACEFRAME lplpFrame,
                        LPLPOLEINPLACEUIWINDOW lplpDoc,
						//WindowPtr	*ppWindow,
                        Rect *lprcPosRect,
					// server allocs, container fills, server frees
						RgnHandle clipRgn,		// defines vis rgn
						RgnHandle frameRgn,		// containing window strucRgn
						RgnHandle cliRgn,		// all container app strucRgns
                        LPOLEINPLACEFRAMEINFO lpFrameInfo) PURE;
    STDMETHOD(Scroll) (THIS_ long scrollExtent) PURE;
    STDMETHOD(OnUIDeactivate) (THIS_ OLEBOOL fUndoable) PURE;
    STDMETHOD(OnInPlaceDeactivate) (THIS) PURE;
    STDMETHOD(DiscardUndoState) (THIS) PURE;
    STDMETHOD(DeactivateAndUndo) (THIS) PURE;
    STDMETHOD(OnPosRectChange) (THIS_ const Rect* lprcPosRect) PURE;
};
typedef         IOleInPlaceSite * LPOLEINPLACESITE, ** LPLPOLEINPLACESITE;



/****** OLE API Prototypes ************************************************/

STDAPI_(unsigned long) OleBuildVersion( void );

/* helper functions */
STDAPI ReadClassStg(LPSTORAGE pStg, CLSID * pclsid);
STDAPI WriteClassStg(LPSTORAGE pStg, REFCLSID rclsid);
STDAPI ReadClassStm(LPSTREAM pStm, CLSID * pclsid);
STDAPI WriteClassStm(LPSTREAM pStm, REFCLSID rclsid);
STDAPI WriteFmtUserTypeStg (LPSTORAGE pstg, unsigned long cf, char * lpszUserType);
STDAPI ReadFmtUserTypeStg (LPSTORAGE pstg, unsigned long * pcf, char * * lplpszUserType);


/* init/term */


typedef enum tagOLEMANAGER
{
    OLEMGR_BIND_NORMAL = 0,	  	// normal app, launch, bind, and addref
	OLEMGR_BIND_IF_THERE = 1,	// app only wishes to bind if applet already running
								// useful only for mini-servers
    OLEMGR_NO_ADDREF = 2,	 	// do not refct the applet, will bind only if applet running
								// useful only for addins
	OLEMGR_ASYNC = 4,			// not yet supported
	OLEMGR_FORCELONG	=	2147483647
} OLEMANAGER;


STDAPI_(unsigned long) IsOleManagerRunning(void);		// returns ref ct	
STDAPI InitOleManager(long dwFlags);					// dwReserved must be NULL
STDAPI_(unsigned long) UninitOleManager(void);			// returns true/false

STDAPI OleInitialize(LPMALLOC pMalloc);
STDAPI_(void) OleUninitialize(void);


/* APIs to query whether (Embedded/Linked) object can be created from
   the data object */

STDAPI  OleQueryLinkFromData(LPDATAOBJECT pSrcDataObject);
STDAPI  OleQueryCreateFromData(LPDATAOBJECT pSrcDataObject);


/* Object creation APIs */

STDAPI  OleCreate(REFCLSID rclsid, REFIID riid, unsigned long renderopt,
                LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite,
                LPSTORAGE pStg, void * * ppvObj);

STDAPI  OleCreateFromData(LPDATAOBJECT pSrcDataObj, REFIID riid,
                unsigned long renderopt, LPFORMATETC pFormatEtc,
                LPOLECLIENTSITE pClientSite, LPSTORAGE pStg,
                void * * ppvObj);

STDAPI  OleCreateLinkFromData(LPDATAOBJECT pSrcDataObj, REFIID riid,
                unsigned long renderopt, LPFORMATETC pFormatEtc,
                LPOLECLIENTSITE pClientSite, LPSTORAGE pStg,
                void * * ppvObj);

STDAPI  OleCreateStaticFromData(LPDATAOBJECT pSrcDataObj, REFIID iid,
                unsigned long renderopt, LPFORMATETC pFormatEtc,
                LPOLECLIENTSITE pClientSite, LPSTORAGE pStg,
                void * * ppvObj);


STDAPI  OleCreateLink(LPMONIKER pmkLinkSrc, REFIID riid,
            unsigned long renderopt, LPFORMATETC lpFormatEtc,
            LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void * * ppvObj);

// MAC Also has the API's OleCreateLinkToFSp and OleCreateFromFSp defined.

STDAPI  OleCreateLinkToFile(const char * lpszFileName, REFIID riid,
            unsigned long renderopt, LPFORMATETC lpFormatEtc,
            LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void * * ppvObj);

STDAPI  OleCreateFromFile(REFCLSID rclsid, const char * lpszFileName, REFIID riid,
            unsigned long renderopt, LPFORMATETC lpFormatEtc,
            LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, void * * ppvObj);
								

STDAPI  OleLoad(LPSTORAGE pStg, REFIID riid, LPOLECLIENTSITE pClientSite,
            void * * ppvObj);

STDAPI  OleSave(LPPERSISTSTORAGE pPS, LPSTORAGE pStg, OLEBOOL fSameAsLoad);

STDAPI  OleLoadFromStream( LPSTREAM pStm, REFIID iidInterface, void * * ppvObj);
STDAPI  OleSaveToStream( LPPERSISTSTREAM pPStm, LPSTREAM pStm );


STDAPI  OleSetContainedObject(LPUNKNOWN pUnknown, OLEBOOL fContained);
STDAPI  OleNoteObjectVisible(LPUNKNOWN pUnknown, OLEBOOL fVisible);


/* Drag/Drop APIs */

STDAPI  RegisterDragDrop(WindowPtr pWnd, LPDROPTARGET pDropTarget);
STDAPI  RevokeDragDrop(WindowPtr pWnd);
STDAPI  DoDragDrop(LPDATAOBJECT pDataObj, LPDROPSOURCE pDropSource,
			unsigned long dwOKEffects, unsigned long *pdwEffect);
STDAPI DoDragDropMac(LPDATAOBJECT pDataObject,LPDROPSOURCE pDropSource,
			unsigned long dwOKEffects,EventRecord *pTheEvent,
			RgnHandle dragRegion,
			short numTypes, DRAG_FLAVORINFO *pFlavorInfo,
			unsigned long reserved,
			unsigned long *pdwEffect); 
/* Clipboard APIs */

STDAPI  OleSetClipboard(LPDATAOBJECT pDataObj);
STDAPI  OleGetClipboard(LPLPDATAOBJECT ppDataObj);
STDAPI  OleFlushClipboard(void);
STDAPI  OleIsCurrentClipboard(LPDATAOBJECT pDataObj);


/* InPlace Editing APIs */


STDAPI OleSendLLE(OSType sig, EventRecord *pevt);
STDAPI OleSendLowLevelEvent(ProcessSerialNumber *ppsn, EventRecord *pevt);

STDAPI	OleNewMBar(OleMBarHandle *phOleMBar);

STDAPI	OleDisposeMBar(OleMBarHandle hOleMBar);

STDAPI  OleInsertMenus(OleMBarHandle hOleMBar, short beforeID1, short beforeID3, short beforeID5);
	// OleInsertMenus inserts the hOleMbar menus into the server's menu list.
	// each group of menus is inserted before the menu whose
	// menu ID equals beforeID#. If beforeID# is 0 (or isn't the ID of any menu
	// in the menu list), the new menu is added after all others. If the menu
	// is already in the menu list or the menu list is already full, InsertMenu
	// does nothing. Be sure to call DrawMenuBar to update the menu bar.

typedef enum tagINSERTMENUS
{
	INSMENUS_INTOSERVER    = 0,	// inserts container menus into server
	INSMENUS_INTOCONTAINER = 1,	// inserts server menus into container
	INSMENUS_FORCELONG     = 2147483647
} INSERTMENUS;

STDAPI  OleInsertMenusEx(OleMBarHandle hOleMBar, short groupID1, short groupID2, short groupID3, unsigned long dwFlags);
	// OleInsertMenus inserts the hOleMbar menus into either the server
	// menu list or the container menu list, depending on the flags.
	// each group of menus is inserted before the menu whose
	// menu ID equals groupID#. If groupID# is 0 (or isn't the ID of any menu
	// in the menu list), the new menu is added after all others. If the menu
	// is already in the menu list or the menu list is already full, InsertMenu
	// does nothing. Be sure to call DrawMenuBar to update the menu bar.

STDAPI  OleHashMenuID(short *pMenuID);
	// container calls this passing menuid.  If this menuid is currently
	// hashed, then the hash value is returned.

STDAPI  OleUnhashMenuID(short *pMenuID);
	// the container calls this api passing a menuid returned by menu select
	// this id is unhashed and returned.  If menuid is not hashed,
	// then in menuID == out menuID

STDAPI  OlePatchGetMHandle(OleMBarHandle hOleMBar);
	// Container app calls this to patch GetMHandle.
	// the menu id's in hOleMbar will be hashed to a range in 15000-17000.
	// This call should be balanced by call to OleUnpatchGetMHandle.

STDAPI  OleUnpatchGetMHandle(OleMBarHandle hOleMBar);
	// Container app calls this to unhash menus and remove GetMHandle patch.
	// Menus are unhashed and patch is removed if and only if there are
	// no more unbalanced calls to OlePatchGetMHandle

STDAPI  OleAddMBarMenu(OleMBarHandle hOleMBar, MenuHandle hMenu, short group);
	// OleAddMBarMenu adds hMenu to the Ole MBar.  This is called only
	// by the client and group is 0,2,4,-1.  -1 is for Hierarchical menus.

STDAPI OleSetInFrontOf(ProcessSerialNumber *pPsn);

STDAPI	OleSetInPlaceWindow(WindowPtr pWndObject, LPOLEINPLACEFRAMEINFO lpFrameInfo);
	// OleSetInPlaceWindow is called by the server to tell the
	// dlls which window is serving the inplace object.  The dlls
	// will subclass this window's wdef to remove all structure
	// rgn from the window.  This is the official request to begin
	// inplace UI.  Server may change any fields of the frameinfo
	// before calling this api (eg, growhandles)

STDAPI	OleUnSetInPlaceWindow(OLEBOOL fInvalidate);
	// OleUnSetInPlaceWindow is called by the server to indicate
	// that the inplace window should be un-subclassed and all
	// the clipped windows should be unclipped.
	// if fInvalidate is true, implies server must invalidate rgns
	// NOTE: only pass TRUE if container is NOT pulling itself forward
	// or you will get flicker!

STDAPI  OleClipWindows(WindowPtr pWndClip);
	// OleClipWindows is called by the server app to clip all windows
	// in its windowlist behind and including pWndClip.
	// This makes the server app appear to remain in the background.
	// NOTE, you must call OleSetInPlaceWindow() first.

STDAPI  OleClipWindow(WindowPtr pWndClip);
STDAPI  OleUnclipWindow(WindowPtr pWndClip, Boolean fPaint);


STDAPI_(void) OleMoveWindow(WindowPtr pWnd,
							short hPosition, short vPosition,
							Boolean fActivate);
	// called by container or server on window and this does the right thing

STDAPI_(void) OleDragParentWindow(WindowPtr pWnd,
					Point startPt, const Rect *pLimitRect);
	// Same as mac trap, but removes server windows from
	// clipping region

STDAPI_(long) OleDragObjectWindow(WindowPtr pWnd,
					Point startPt, const Rect *pLimitRect, const Rect *pSlopRect,
					short constrain, ProcPtr actionProc);
	// bounds is intersected w/ the parent window rect as drag limit
	// constrain limits direction of drag
	// draggrayrgn


STDAPI_(void) OleSizeParentWindow(WindowPtr pWnd, short h, short v, Boolean fUpdate);
	// Same as mac trap, but clips ip server window


STDAPI_(void) OleSizeObjectWindow(WindowPtr pWnd, const Rect *prcNewBounds, Boolean fUpdate);
	// does a movewindow/sizewindow to effectively "zoom" window to new location/size

STDAPI_(long) OleZoomParentWindow(WindowPtr pWnd, short wPart, Boolean fMakeFront);
	// same as mac trap

STDAPI_(long) OleGrowParentWindow(WindowPtr pWnd, Point pt, const Rect *prcMinMax);
	// same as mac trap


STDAPI_(long) OleGrowObjectWindow(WindowPtr pWnd, Point pt, const Rect *prcMinMax, Rect *prcNewBounds);
	// OleGrowObjectWindow is called by the server app to track the resize handles
	// of the inplace window.  pass the inplace windowptr, global point
	// that the mouse went down, constraining min/max lengths in a rect
	// like growwindow top left is min height/width bottom right is
	// max height/width, the output rect prcNewBounds is
	// dims or rect to pass to olesizewindow and SetInplaceRects

STDAPI_(short) OleWhichGrowHandle(WindowPtr pWnd, Point pt);
	// pt is in global coordinates
	// returns -2, this window is not inplace active
	// returns -1, not in grow rgn
	// 0 == standard grow box
	// 1..8 handles numbered from top left to bot right
	// this api calls FindWindow to ensure window part is
	// visible

STDAPI	OleGetCursor(CursPtr pCursor, WindowPtr pWndObject);
	// server calls this to get cursor info from container.
	// objectWindow is server's current ip window
	// pCursor is ptr to 68 bytes to receive the cursor data

STDAPI	OleSetCursor(CursPtr pCursor, WindowPtr pWndParent);
	// container optionally calls this to set up cursor info.
	// if container does not set up custom cursors, standard
	// arrow will be assumed.
	// if pWndParent is not the current ip parent window
	// this api return error

STDAPI	OleUpdateCursor(WindowPtr pWndObject);
	// Server calls this to change cursor for container while
	// in foreground.  This is a short cut for calling
	// OleGetCursor(), OleWhichGrowHandle(), SetCursor().
	// If no cursor information was supplied by container
	// the standard arrow cursor will be used
	// if the cursor is over the drag/resize area of the object window
	// standard resize cursors will be drawn.


STDAPI  OleSetInPlaceRects(
		RgnHandle posRgn,
		RgnHandle clipRgn, RgnHandle frameRgn,
		RgnHandle cliRgn, RgnHandle clickRgn);
	// OleSetInPlaceRects is called by the server app to inform the
	// inplace window how to clip itself to client app windows
	// the 1st 3 params are the same as SetObjectRects,
	// all rect/rgn coords are GLOBAL
	// posRgn - obj pos Rgn
	// clipRgn - rgn in which object may display itself visibly
	// frameRgn - strucRgn of obj's containing window
	// cliRgn - the union of all open/visible client app windows strucRgns
	// clickRgn - if non-null is sub'ed from posRgn and passed to ole2init to
	// catch mouse clicks	(obsolete now?) bugbug

STDAPI  OleSetParentRgns(RgnHandle clipRgn,
	RgnHandle frameRgn, RgnHandle clientRgn);
//

STDAPI  OleSetObjectRgn(RgnHandle posRgn);
// this call is optional


STDAPI	OleMaskMouse(Boolean bEnable);
	// server calls this api to turn mouseDown->app1Evt coercion on or off
	// server calls OleMaskMouse(false) to put up modal dbox
	// and OleMaskMouse(true) when the dbox is done

STDAPI	OleTranslateAccelerator (LPOLEINPLACEFRAME lpFrame,
			LPOLEINPLACEFRAMEINFO lpFrameInfo, EventRecord * pEvt);
	// calls menukey to get menuid, then calls lpFrame->TranslateAccelerator,
	// then calls HiliteMenu(0);



/* Helper APIs */
STDAPI_(Handle) OleDuplicateData (Handle hSrc, unsigned long cfFormat,
                        unsigned short uiFlags);

STDAPI          OleDraw (LPUNKNOWN pUnknown, unsigned long dwAspect, GrafPtr pGrafDraw,
                    const Rect* lprcBounds);

STDAPI          OleRun(LPUNKNOWN pUnknown);
STDAPI_(OLEBOOL)   OleIsRunning(LPOLEOBJECT pObject);

STDAPI          OleLockRunning(LPUNKNOWN pUnknown, OLEBOOL fLock, OLEBOOL fLastUnlockCloses);

STDAPI_(void)   ReleaseStgMedium(LPSTGMEDIUM);
STDAPI          CreateOleAdviseHolder(LPLPOLEADVISEHOLDER ppOAHolder);

STDAPI          OleCreateDefaultHandler(REFCLSID clsid, LPUNKNOWN pUnkOuter,
					REFIID riid, void * * lplpObj);

STDAPI          OleCreateEmbeddingHelper(REFCLSID clsid, LPUNKNOWN pUnkOuter,
                    unsigned long flags, LPCLASSFACTORY pCF,
					REFIID riid, void ** lplpObj);

#ifdef MERGE
STDAPI_(OLEBOOL)	IsAccelerator(HACCEL hAccel, int cAccelEntries, LPMSG lpMsg,
					WORD FAR* lpwCmd);
#endif

					
/* Icon extraction Helper APIs */
					
STDAPI_(PicHandle) OleGetIconOfFile(char * lpszPath, OLEBOOL fUseFileAsLabel);
STDAPI_(PicHandle) OleGetIconOfClass(REFCLSID rclsid,	char * lpszLabel,
									OLEBOOL fUseTypeAsLabel);


/* Registration Database Helper APIs */

STDAPI 			OleRegGetUserType (REFCLSID clsid, unsigned long dwFormOfType,
					char ** pszUserType);

STDAPI 			OleRegGetMiscStatus	(REFCLSID clsid, unsigned long dwAspect,
					unsigned long* pdwStatus);

STDAPI 			OleRegEnumFormatEtc	(REFCLSID clsid, unsigned long dwDirection,
					LPLPENUMFORMATETC ppenum);

STDAPI 			OleRegEnumVerbs	(REFCLSID clsid, LPLPENUMOLEVERB ppenum);



/* OLE 1.0 conversion APIS */

STDAPI OleConvertIStorageToOLESTREAM
    (LPSTORAGE			pstg,
	 LPOLESTREAM		polestm);

STDAPI OleConvertOLESTREAMToIStorage
    (LPOLESTREAM                polestm,
    LPSTORAGE                   pstg,
    const DVTARGETDEVICE *   ptd);

STDAPI OleConvertIStorageToOLESTREAMEx
    (LPSTORAGE			pstg,
									// Presentation data to OLESTREAM
	 unsigned long		cfFormat,	//		format
	 long				lWidth,		//		width
	 long				lHeight,	//		height
	 unsigned long		dwSize,		//		size in bytes
     LPSTGMEDIUM		pmedium,	//		bits
	 LPOLESTREAM		polestm);

STDAPI OleConvertOLESTREAMToIStorageEx
    (LPOLESTREAM		polestm,
	 LPSTORAGE			pstg,
									// Presentation data from OLESTREAM
	 unsigned long *	pcfFormat,	//		format
	 long *				plwWidth,	//		width
	 long *				plHeight,	//		height
	 unsigned long *	pdwSize,	//		size in bytes
	 LPSTGMEDIUM		pmedium);	//		bits



/* Storage Utility APIs */
STDAPI GetHGlobalFromILockBytes (LPLOCKBYTES plkbyt, Handle * phglobal);
STDAPI CreateILockBytesOnHGlobal (Handle hGlobal, OLEBOOL fDeleteOnRelease,
                                    LPLPLOCKBYTES pplkbyt);

STDAPI GetHGlobalFromStream (LPSTREAM pstm, Handle * phglobal);
STDAPI CreateStreamOnHGlobal (Handle hGlobal, OLEBOOL fDeleteOnRelease,
                                LPLPSTREAM ppstm);


/* ConvertTo APIS */

STDAPI OleDoAutoConvert(LPSTORAGE pStg, LPCLSID pClsidNew);
STDAPI OleGetAutoConvert(REFCLSID clsidOld, LPCLSID pClsidNew);
STDAPI OleSetAutoConvert(REFCLSID clsidOld, REFCLSID clsidNew);
STDAPI GetConvertStg(LPSTORAGE pStg);
STDAPI SetConvertStg(LPSTORAGE pStg, OLEBOOL fConvert);

#ifndef __MACAPI__
#include <macapi.h>
#endif

#ifdef _WIN32
#include "macname2.h"
#endif // _WIN32

#endif /* !__OLE2__ */


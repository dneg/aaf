/*****************************************************************************\
*                                                                             *
* moniker.h - 	Moniker and related interfaces and APIs						  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/


#if !defined(__MONIKER__) && !defined(_MONIKER_H_)
#define __MONIKER__
#define _MONIKER_H_

#ifndef _WINERROR_

#define MK_E_CONNECTMANUALLY        MK_E_FIRST
#define MK_E_EXCEEDEDDEADLINE       (MK_E_FIRST + 1)
#define MK_E_NEEDGENERIC            (MK_E_FIRST + 2)
#define MK_E_UNAVAILABLE            (MK_E_FIRST + 3)
#define MK_E_SYNTAX                 (MK_E_FIRST + 4)
#define MK_E_NOOBJECT               (MK_E_FIRST + 5)
#define MK_E_INVALIDEXTENSION       (MK_E_FIRST + 6)
#define MK_E_INTERMEDIATEINTERFACENOTSUPPORTED (MK_E_FIRST + 7)
#define MK_E_NOTBINDABLE            (MK_E_FIRST + 8)
#define MK_E_NOTBOUND               (MK_E_FIRST + 9)
                            // called IBindCtx->RevokeObjectBound for an
                            // object which was not bound
#define MK_E_CANTOPENFILE           (MK_E_FIRST + 10)
#define MK_E_MUSTBOTHERUSER         (MK_E_FIRST + 11)
#define MK_E_NOINVERSE              (MK_E_FIRST + 12)
#define MK_E_NOSTORAGE              (MK_E_FIRST + 13)
#define MK_E_NOPREFIX               (MK_E_FIRST + 14)


// reserved                     MK_S_FIRST
// reserved                     (MK_S_FIRST + 1)
#define MK_S_REDUCED_TO_SELF    (MK_S_FIRST + 2)
// reserved                     (MK_S_FIRST + 3)
#define MK_S_ME                 (MK_S_FIRST + 4)
#define MK_S_HIM                (MK_S_FIRST + 5)
#define MK_S_US                 (MK_S_FIRST + 6)
#define MK_S_MONIKERALREADYREGISTERED (MK_S_FIRST + 7)

#endif // _WINERROR_


// bind options; variable sized
typedef struct FARSTRUCT tagBIND_OPTS
{
    unsigned long       cbStruct;       //  sizeof(BIND_OPTS)
    unsigned long       grfFlags;
    unsigned long       grfMode;
    unsigned long       dwTickCountDeadline;
} BIND_OPTS, * LPBIND_OPTS;


// bind flags; controls binding; stored in bind options above
typedef enum
{
    BIND_MAYBOTHERUSER = 1,
    BIND_JUSTTESTEXISTENCE = 2
	,BIND_FLAGS_FORCELONG	=	2147483647
} BIND_FLAGS;


// system moniker types; returned from IsSystemMoniker.
typedef enum tagMKSYS
{
    MKSYS_NONE = 0,
    MKSYS_GENERICCOMPOSITE = 1,
    MKSYS_FILEMONIKER = 2,
    MKSYS_ANTIMONIKER = 3,
    MKSYS_ITEMMONIKER = 4,
    MKSYS_POINTERMONIKER = 5
	,MKSYS_FORCELONG	=	2147483647
}MKSYS;


typedef enum tagMKREDUCE
{
    MKRREDUCE_ONE           =   3<<16,
    MKRREDUCE_TOUSER        =   2<<16,
    MKRREDUCE_THROUGHUSER   =   1<<16,
    MKRREDUCE_ALL           =   0
	,MKRREDUCE_FORCELONG	=	2147483647
} MKRREDUCE;


#if defined(__cplusplus)
interface IEnumMoniker;
interface IRunningObjectTable;
#else 
//typedef interface IEnumMoniker IEnumMoniker;
//typedef interface IRunningObjectTable IRunningObjectTable;
#endif

typedef       interface IEnumMoniker * LPENUMMONIKER, ** LPLPENUMMONIKER;
typedef       interface IRunningObjectTable * LPRUNNINGOBJECTTABLE, ** LPLPRUNNINGOBJECTTABLE;



#undef  INTERFACE
#define INTERFACE   IBindCtx

DECLARE_INTERFACE_(IBindCtx, IUnknown)
{
	BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IBindCtx methods ***
    STDMETHOD(RegisterObjectBound) (THIS_ LPUNKNOWN punk) PURE;
    STDMETHOD(RevokeObjectBound) (THIS_ LPUNKNOWN punk) PURE;
    STDMETHOD(ReleaseBoundObjects) (THIS) PURE;
    
    STDMETHOD(SetBindOptions) (THIS_ LPBIND_OPTS pbindopts) PURE;
    STDMETHOD(GetBindOptions) (THIS_ LPBIND_OPTS pbindopts) PURE;
    STDMETHOD(GetRunningObjectTable) (THIS_ LPLPRUNNINGOBJECTTABLE
        pprot) PURE;
    STDMETHOD(RegisterObjectParam) (THIS_ char * lpszKey, LPUNKNOWN punk) PURE;
    STDMETHOD(GetObjectParam) (THIS_ char * lpszKey, LPLPUNKNOWN ppunk) PURE;
    STDMETHOD(EnumObjectParam) (THIS_ LPLPENUMSTRING ppenum) PURE;
    STDMETHOD(RevokeObjectParam) (THIS_ char * lpszKey) PURE;
};
typedef             IBindCtx * LPBC, ** LPLPBC;
typedef         IBindCtx * LPBINDCTX, ** LPLPBINDCTX;



#undef  INTERFACE
#define INTERFACE   IMoniker

DECLARE_INTERFACE_(IMoniker, IPersistStream)
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

    // *** IMoniker methods ***
    STDMETHOD(BindToObject) (THIS_ LPBC pbc, LPMONIKER pmkToLeft,
        REFIID riidResult, void * * ppvResult) PURE;
    STDMETHOD(BindToStorage) (THIS_ LPBC pbc, LPMONIKER pmkToLeft,
        REFIID riid, void * * ppvObj) PURE;
    STDMETHOD(Reduce) (THIS_ LPBC pbc, unsigned long dwReduceHowFar, LPLPMONIKER
        ppmkToLeft, LPLPMONIKER ppmkReduced) PURE;
    STDMETHOD(ComposeWith) (THIS_ LPMONIKER pmkRight, OLEBOOL fOnlyIfNotGeneric,
        LPLPMONIKER ppmkComposite) PURE;
    STDMETHOD(Enum) (THIS_ OLEBOOL fForward, LPLPENUMMONIKER ppenumMoniker)
        PURE;
    STDMETHOD(IsEqual) (THIS_ LPMONIKER pmkOtherMoniker) PURE;
    STDMETHOD(Hash) (THIS_ unsigned long	* pdwHash) PURE;
    STDMETHOD(IsRunning) (THIS_ LPBC pbc, LPMONIKER pmkToLeft, LPMONIKER
        pmkNewlyRunning) PURE;
    STDMETHOD(GetTimeOfLastChange) (THIS_ LPBC pbc, LPMONIKER pmkToLeft,
        FILETIME * pfiletime) PURE;
    STDMETHOD(Inverse) (THIS_ LPLPMONIKER ppmk) PURE;
    STDMETHOD(CommonPrefixWith) (THIS_ LPMONIKER pmkOther, LPLPMONIKER
        ppmkPrefix) PURE;
    STDMETHOD(RelativePathTo) (THIS_ LPMONIKER pmkOther, LPLPMONIKER
        ppmkRelPath) PURE;
    STDMETHOD(GetDisplayName) (THIS_ LPBC pbc, LPMONIKER pmkToLeft,
        char * * lplpszDisplayName) PURE;
    STDMETHOD(ParseDisplayName) (THIS_ LPBC pbc, LPMONIKER pmkToLeft,
        char * lpszDisplayName, unsigned long * pchEaten,
        LPLPMONIKER ppmkOut) PURE;
    STDMETHOD(IsSystemMoniker) (THIS_ unsigned long	* pdwMksys) PURE;
};
//typedef         IMoniker * LPMONIKER, ** LPLPMONIKER;

//  IRunningObjectTable::Register flags
#define ROTFLAGS_REGISTRATIONKEEPSALIVE 1

#undef  INTERFACE
#define INTERFACE   IRunningObjectTable

DECLARE_INTERFACE_(IRunningObjectTable, IUnknown)
{
	BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IRunningObjectTable methods ***
    STDMETHOD(Register) (THIS_ unsigned long grfFlags, LPUNKNOWN punkObject, 
        LPMONIKER pmkObjectName, unsigned long  * pdwRegister) PURE;
    STDMETHOD(Revoke) (THIS_ unsigned long dwRegister) PURE;
    STDMETHOD(IsRunning) (THIS_ LPMONIKER pmkObjectName) PURE;
    STDMETHOD(GetObject) (THIS_ LPMONIKER pmkObjectName,
        LPLPUNKNOWN ppunkObject) PURE;
    STDMETHOD(NoteChangeTime) (THIS_ unsigned long dwRegister, FILETIME  * pfiletime) PURE;
    STDMETHOD(GetTimeOfLastChange) (THIS_ LPMONIKER pmkObjectName, FILETIME  * pfiletime) PURE;
    STDMETHOD(EnumRunning) (THIS_ LPLPENUMMONIKER ppenumMoniker ) PURE;
};
//typedef         IRunningObjectTable * LPRUNNINGOBJECTTABLE, ** LPLPRUNNINGOBJECTTABLE;



#undef  INTERFACE
#define INTERFACE   IEnumMoniker

DECLARE_INTERFACE_(IEnumMoniker, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IEnumOleDataObject methods ***
    STDMETHOD(Next) (THIS_ unsigned long celt, LPLPMONIKER rgelt, unsigned long * pceltFetched) PURE;
    STDMETHOD(Skip) (THIS_ unsigned long celt) PURE;
    STDMETHOD(Reset) (THIS) PURE;
    STDMETHOD(Clone) (THIS_ IEnumMoniker * * ppenm) PURE;
};
//typedef       IEnumMoniker * LPENUMMONIKER, ** LPLPENUMMONIKER;




STDAPI  BindMoniker(LPMONIKER pmk, unsigned long grfOpt, REFIID iidResult, void * * ppvResult);
STDAPI  MkParseDisplayName(LPBC pbc, char * szUserName, 
                unsigned long  * pchEaten, LPLPMONIKER ppmk);
STDAPI  MonikerRelativePathTo(LPMONIKER pmkSrc, LPMONIKER pmkDest, LPMONIKER
                * ppmkRelPath, OLEBOOL fCalledFromMethod);
STDAPI  MonikerCommonPrefixWith(LPMONIKER pmkThis, LPMONIKER pmkOther,
                LPLPMONIKER ppmkCommon);
STDAPI  CreateBindCtx(unsigned long reserved, LPLPBC ppbc);
STDAPI  CreateGenericComposite(LPMONIKER pmkFirst, LPMONIKER pmkRest,
    						LPLPMONIKER ppmkComposite);
								
// MAC has the API's GetClassFSp and CreateFileMonikerFSp also defined.

STDAPI  GetClassFile (const char *  szFilename, CLSID * pclsid);
STDAPI  CreateFileMoniker(char *  lpszPathName, LPLPMONIKER ppmk);

STDAPI  CreateItemMoniker(char * lpszDelim, char * lpszItem,
    LPLPMONIKER ppmk);
STDAPI  CreateAntiMoniker(LPLPMONIKER ppmk);
STDAPI  CreatePointerMoniker(LPUNKNOWN punk, LPLPMONIKER ppmk);

STDAPI  GetRunningObjectTable( unsigned long reserved, LPLPRUNNINGOBJECTTABLE pprot);


#endif /* !__MONIKER__ */

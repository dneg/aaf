/*****************************************************************************\
*                                                                             *
* compobj.h - 	Component object model definitions							  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/


#if !defined(__COMPOBJ__) && !defined(_COMPOBJ_H_)
#define __COMPOBJ__
#define _COMPOBJ_H_


/****** Linkage Definitions *************************************************/

/*
 *      These are macros for declaring methods/functions.  They exist so that
 *      control over the use of keywords (CDECL, PASCAL, __export,
 *      extern "C") resides in one place, and because this is the least
 *      intrusive way of writing function declarations that do not have
 *      to be modified in order to port to the Mac.
 *
 *      The macros without the trailing underscore are for functions/methods
 *      which a return value of type HRESULT; this is by far the most common
 *      case in OLE. The macros with a trailing underscore take a return
 *      type as a parameter.
 *
 * WARNING: STDAPI is hard coded into the LPFNGETCLASSOBJECT typedef below.
 */

#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif

#if !defined( __MACPUB__ )
#include <macpub.h>
#endif

#ifndef __CODEFRAGMENTS__
#include <codefrag.h>
#endif

#if !defined(_MSC_VER)
  #ifdef __SC__
	#define STDMETHODCALLTYPE		_cdecl
  #else
	#define STDMETHODCALLTYPE
  #endif
	#define STDAPICALLTYPE          pascal
	#define STDAPI                  EXTERN_C STDAPICALLTYPE HRESULT
	#define _STDAPI                 EXTERN_C STDAPICALLTYPE HRESULT
	#define STDAPI_(type)           EXTERN_C STDAPICALLTYPE type
	#define _STDAPI_(type)          EXTERN_C STDAPICALLTYPE type
#else /*  */
	#ifndef __powerc
	
	#define STDMETHODCALLTYPE		  __cdecl
	#define STDAPICALLTYPE          __pascal
	#define STDAPI                  EXTERN_C HRESULT STDAPICALLTYPE
	#define _STDAPI                 EXTERN_C HRESULT STDAPICALLTYPE
	#define STDAPI_(type)           EXTERN_C type STDAPICALLTYPE
	#define _STDAPI_(type)          EXTERN_C type STDAPICALLTYPE
	
	#else

	#define STDMETHODCALLTYPE		
	#define STDAPICALLTYPE
	#define STDAPI                  EXTERN_C HRESULT STDAPICALLTYPE
	#define _STDAPI                 EXTERN_C HRESULT STDAPICALLTYPE
	#define STDAPI_(type)           EXTERN_C type STDAPICALLTYPE
	#define _STDAPI_(type)          EXTERN_C type STDAPICALLTYPE

	#endif // __powerc

#endif /*  */


#define STDMETHODIMP            HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)     type STDMETHODCALLTYPE


/****** Interface Declaration ***********************************************/

/*
 *      These are macros for declaring interfaces.  They exist so that
 *      a single definition of the interface is simulataneously a proper
 *      declaration of the interface structures (C++ abstract classes)
 *      for both C and C++.
 *
 *      DECLARE_INTERFACE(iface) is used to declare an interface that does
 *      not derive from a base interface.
 *      DECLARE_INTERFACE_(iface, baseiface) is used to declare an interface
 *      that does derive from a base interface.
 *
 *      By default if the source file has a .c extension the C version of
 *      the interface declaratations will be expanded; if it has a .cpp
 *      extension the C++ version will be expanded. if you want to force
 *      the C version expansion even though the source file has a .cpp
 *      extension, then define the macro "CINTERFACE".
 *      eg.     cl -DCINTERFACE file.cpp
 *
 *      Example Interface declaration:
 *
 *          #undef  INTERFACE
 *          #define INTERFACE   IClassFactory
 *
 *          DECLARE_INTERFACE_(IClassFactory, IUnknown)
 *          {
 *              // *** IUnknown methods ***
 *              STDMETHOD(QueryInterface) (THIS_
 *                                        REFIID riid,
 *                                        void * * ppvObj) PURE;
 *              STDMETHOD_(unsigned long,AddRef) (THIS) PURE;
 *              STDMETHOD_(unsigned long,Release) (THIS) PURE;
 *
 *              // *** IClassFactory methods ***
 *              STDMETHOD(CreateInstance) (THIS_
 *                                        LPUNKNOWN pUnkOuter,
 *                                        REFIID riid,
 *                                        void * * ppvObject) PURE;
 *          };
 *
 *      Example C++ expansion:
 *
 *          struct  IClassFactory : public IUnknown
 *          {
 *              virtual HRESULT STDMETHODCALLTYPE QueryInterface(
 *                                                  IID & riid,
 *                                                  void * * ppvObj) = 0;
 *              virtual HRESULT STDMETHODCALLTYPE AddRef(void) = 0;
 *              virtual HRESULT STDMETHODCALLTYPE Release(void) = 0;
 *              virtual HRESULT STDMETHODCALLTYPE CreateInstance(
 *                                              LPUNKNOWN pUnkOuter,
 *                                              IID & riid,
 *                                              void * * ppvObject) = 0;
 *          };
 *
 *          NOTE: Our documentation says '#define interface class' but we use
 *          'struct' instead of 'class' to keep a lot of 'public:' lines
 *          out of the interfaces.  The '' forces the 'this' pointers to
 *          be far, which is what we need.
 *
 *      Example C expansion:
 *
 *          typedef struct IClassFactory
 *          {
 *              const struct IClassFactoryVtbl * lpVtbl;
 *          } IClassFactory;
 *
 *          typedef struct IClassFactoryVtbl IClassFactoryVtbl;
 *
 *          struct IClassFactoryVtbl
 *          {
 *              HRESULT (STDMETHODCALLTYPE * QueryInterface) (
 *                                                  IClassFactory * This,
 *                                                  IID * riid,
 *                                                  void * * ppvObj) ;
 *              HRESULT (STDMETHODCALLTYPE * AddRef) (IClassFactory * This) ;
 *              HRESULT (STDMETHODCALLTYPE * Release) (IClassFactory * This) ;
 *              HRESULT (STDMETHODCALLTYPE * CreateInstance) (
 *                                                  IClassFactory * This,
 *                                                  LPUNKNOWN pUnkOuter,
 *                                                  IID * riid,
 *                                                  void * * ppvObject);
 *              HRESULT (STDMETHODCALLTYPE * LockServer) (
 *                                                  IClassFactory * This,
 *                                                  OLEBOOL   fLock);
 *          };
 */


#if defined(__cplusplus) && !defined(CINTERFACE)
#ifdef __TURBOC__
#define interface               struct huge
#else
#define interface               struct
#endif

#define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method

#define PURE                    = 0
#define THIS_
#define THIS                    void
#define DECLARE_INTERFACE(iface)    interface iface
#define DECLARE_INTERFACE_(iface, baseiface)    interface iface : public baseiface

#if (defined(_MSC_VER) || defined(__SC__)) && !defined(NO_NULL_VTABLE_ENTRY)
#define BEGIN_INTERFACE STDMETHOD_(void,a) () {}
#else
#define BEGIN_INTERFACE
#endif

#define END_INTERFACE

#else // !__cplusplus

#define interface               struct


#define STDMETHOD(method)       HRESULT (STDMETHODCALLTYPE * method)
#define STDMETHOD_(type,method) type (STDMETHODCALLTYPE * method)

#define BEGIN_INTERFACE			void    *b;
#define END_INTERFACE			// void    *b;

#define PURE
#define THIS_                   INTERFACE * This,
#define THIS                    INTERFACE * This
#ifdef CONST_VTABLE
#define DECLARE_INTERFACE(iface)    typedef interface iface { \
                                    const struct iface##Vtbl * lpVtbl; \
                                } iface; \
                                typedef const struct iface##Vtbl iface##Vtbl; \
                                const struct iface##Vtbl
#else
#define DECLARE_INTERFACE(iface)    typedef interface iface { \
                                    struct iface##Vtbl * lpVtbl; \
                                } iface; \
                                typedef struct iface##Vtbl iface##Vtbl; \
                                struct iface##Vtbl
#endif
#define DECLARE_INTERFACE_(iface, baseiface)    DECLARE_INTERFACE(iface)

#endif


/****** Additional basic types **********************************************/


#ifndef FARSTRUCT
#ifdef __cplusplus
#define FARSTRUCT
#else
#define FARSTRUCT
#endif  // __cplusplus
#endif  // FARSTRUCT



#ifndef WIN32
#ifndef _WINNT_
typedef struct FARSTRUCT _LARGE_INTEGER {
    unsigned long LowPart;
    long  HighPart;
} LARGE_INTEGER, *PLARGE_INTEGER;
#endif
#endif
#define LISet32(li, v) ((li).HighPart = ((long)(v)) < 0 ? -1 : 0, (li).LowPart = (v))

#ifndef WIN32
#ifndef _WINNT_
typedef struct FARSTRUCT _ULARGE_INTEGER {
    unsigned long LowPart;
    unsigned long HighPart;
} ULARGE_INTEGER, *PULARGE_INTEGER;
#endif
#endif
#define ULISet32(li, v) ((li).HighPart = 0, (li).LowPart = (v))

#ifndef _WINDOWS_
#ifndef _FILETIME_
#define _FILETIME_
typedef struct FARSTRUCT tagFILETIME
{
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
} FILETIME;
#endif
#endif

#ifndef __SCODE__
#include "scode.h"
#endif


// *********************** Compobj errors **********************************

#ifndef _WINERROR_

#define CO_E_NOTINITIALIZED         (CO_E_FIRST + 0x0)
// CoInitialize has not been called and must be

#define CO_E_ALREADYINITIALIZED     (CO_E_FIRST + 0x1)
// CoInitialize has already been called and cannot be called again (temporary)

#define CO_E_CANTDETERMINECLASS     (CO_E_FIRST + 0x2)
// can't determine clsid (e.g., extension not in reg.dat)

#define CO_E_CLASSSTRING            (CO_E_FIRST + 0x3)
// the string form of the clsid is invalid (including ole1 classes)

#define CO_E_IIDSTRING              (CO_E_FIRST + 0x4)
// the string form of the iid is invalid

#define CO_E_APPNOTFOUND            (CO_E_FIRST + 0x5)
// application not found

#define CO_E_APPSINGLEUSE           (CO_E_FIRST + 0x6)
// application cannot be run more than once

#define CO_E_ERRORINAPP             (CO_E_FIRST + 0x7)
// some error in the app program file

#define CO_E_DLLNOTFOUND            (CO_E_FIRST + 0x8)
// dll not found

#define CO_E_ERRORINDLL             (CO_E_FIRST + 0x9)
// some error in the dll file

#define CO_E_WRONGOSFORAPP          (CO_E_FIRST + 0xa)
// app written for other version of OS or other OS altogether

#define CO_E_OBJNOTREG              (CO_E_FIRST + 0xb)
// object is not registered

#define CO_E_OBJISREG               (CO_E_FIRST + 0xc)
// object is already registered

#define CO_E_OBJNOTCONNECTED        (CO_E_FIRST + 0xd)
// handler is not connected to server

#define CO_E_APPDIDNTREG            (CO_E_FIRST + 0xe)
// app was launched, but didn't registered a class factory


// ********************* ClassObject errors ********************************

#define CLASS_E_NOAGGREGATION       (CLASSFACTORY_E_FIRST + 0x0)
// class does not support aggregation (or class object is remote)

#define CLASS_E_CLASSNOTAVAILABLE   (CLASSFACTORY_E_FIRST + 0x1)
// dll doesn't support that class (returned from DllGetClassObject)


// *********************** Reg.dat errors **********************************

#define REGDB_E_READREGDB           (REGDB_E_FIRST + 0x0)
// some error reading the registration database

#define REGDB_E_WRITEREGDB          (REGDB_E_FIRST + 0x1)
// some error reading the registration database

#define REGDB_E_KEYMISSING          (REGDB_E_FIRST + 0x2)
// some error reading the registration database

#define REGDB_E_INVALIDVALUE        (REGDB_E_FIRST + 0x3)
// some error reading the registration database

#define REGDB_E_CLASSNOTREG         (REGDB_E_FIRST + 0x4)
// some error reading the registration database

#define REGDB_E_IIDNOTREG           (REGDB_E_FIRST + 0x5)
// some error reading the registration database


// *************************** RPC errors **********************************

#define RPC_E_FIRST    MAKE_SCODE(SEVERITY_ERROR, FACILITY_RPC,  0x000)

// call was rejected by callee, either by MF::HandleIncomingCall or
#define RPC_E_CALL_REJECTED             (RPC_E_FIRST + 0x1)

// call was canceld by call - returned by MessagePending
// this code only occurs if MessagePending return cancel
#define RPC_E_CALL_CANCELED             (RPC_E_FIRST + 0x2)

// the caller is dispatching an intertask SendMessage call and
// can NOT call out via PostMessage
#define RPC_E_CANTPOST_INSENDCALL       (RPC_E_FIRST + 0x3)

// the caller is dispatching an asynchronus call can NOT
// make an outgoing call on behalf of this call
#define RPC_E_CANTCALLOUT_INASYNCCALL   (RPC_E_FIRST + 0x4)

// the caller is not in a state where an outgoing call can be made
// this is the case if the caller has an outstandig call and
// another incoming call was excepted by HIC; now the caller is
// not allowed to call out again
#define RPC_E_CANTCALLOUT_INEXTERNALCALL (RPC_E_FIRST + 0x5)

// the connection terminated or is in a bogus state
// and can not be used any more. Other connections
// are still valid.
#define RPC_E_CONNECTION_TERMINATED     (RPC_E_FIRST + 0x6)

// the callee (server [not server application]) is not available
// and disappeared; all connections are invalid
#define RPC_E_SERVER_DIED               (RPC_E_FIRST + 0x7)

// the caller (client ) disappeared while the callee (server) was
// processing a call
#define RPC_E_CLIENT_DIED               (RPC_E_FIRST + 0x8)

// the date paket with the marshalled parameter data is
// incorrect
#define RPC_E_INVALID_DATAPACKET        (RPC_E_FIRST + 0x9)

// the call was not transmitted properly; the message queue
// was full and was not emptied after yielding
#define RPC_E_CANTTRANSMIT_CALL         (RPC_E_FIRST + 0xa)

// the client (caller) can not marshall the parameter data
// or unmarshall the return data - low memory etc.
#define RPC_E_CLIENT_CANTMARSHAL_DATA   (RPC_E_FIRST + 0xb)
#define RPC_E_CLIENT_CANTUNMARSHAL_DATA (RPC_E_FIRST + 0xc)

// the server (caller) can not unmarshall the parameter data
// or marshall the return data - low memory
#define RPC_E_SERVER_CANTMARSHAL_DATA   (RPC_E_FIRST + 0xd)
#define RPC_E_SERVER_CANTUNMARSHAL_DATA (RPC_E_FIRST + 0xe)

// received data are invalid; can be server or
// client data
#define RPC_E_INVALID_DATA              (RPC_E_FIRST + 0xf)

// a particular parameter is invalid and can not be un/marshalled
#define RPC_E_INVALID_PARAMETER         (RPC_E_FIRST + 0x10)

// DDE conversation - no second outgoing call on same channel
#define RPC_E_CANTCALLOUT_AGAIN			(RPC_E_FIRST + 0x11)

// a internal error occured
#define RPC_E_UNEXPECTED                (RPC_E_FIRST + 0xFFFF)

#endif // _WINERROR_

/****** Globally Unique Ids *************************************************/

#ifndef GUID_DEFINED
#define GUID_DEFINED
#ifdef __cplusplus

struct  GUID
{
    unsigned long Data1;
    unsigned short  Data2;
    unsigned short  Data3;
    unsigned char  Data4[8];

    unsigned long operator==(const GUID& iidOther) const

        {	return ((Data1 == iidOther.Data1) &&
					(Data2 == iidOther.Data2) &&
					(Data3 == iidOther.Data3) &&
					(* (long*) Data4 == *(long*)iidOther.Data4) &&
					(* (long*) (Data4+4) == *( (long*) (iidOther.Data4+4)) ) );
		}
    unsigned long operator!=(const GUID& iidOther) const
        { return !((*this) == iidOther); }
};

#else
typedef struct GUID
{
    unsigned long Data1;
    unsigned short  Data2;
    unsigned short  Data3;
    unsigned char  Data4[8];
} GUID;
#endif
#endif /* GUID_DEFINED */

typedef                GUID * LPGUID;


// macros to define byte pattern for a GUID.
//      Example: DEFINE_GUID(GUID_XXX, a, b, c, ...);
//
// Each dll/exe must initialize the GUIDs once.  This is done in one of
// two ways.  If you are not using precompiled headers for the file(s) which
// initializes the GUIDs, define INITGUID before including compobj.h.  This
// is how OLE builds the initialized versions of the GUIDs which are included
// in compobj.dll.
//
// The alternative (which some versions of the compiler don't handle properly;
// they wind up with the initialized GUIDs in a data, not a text segment),
// is to use a precompiled version of compobj.h and then include initguid.h
// after compobj.h followed by one or more of the guid defintion files.

// Bug in the DR2 release of Metrowerks does not allow for extern "C" of data
#if defined(__MWERKS__)				

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    extern const GUID name

#else

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID name

#endif

#ifdef INITGUID
#include "initguid.h"
#endif


#define DEFINE_OLEGUID(name, l, w1, w2) \
    DEFINE_GUID(name, l, w1, w2, 0xC0,0,0,0,0,0,0,0x46)


// Interface ID are just a kind of GUID
typedef GUID IID;
typedef                IID * LPIID;
#define IID_NULL            GUID_NULL
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)


// Class ID are just a kind of GUID
typedef GUID CLSID;
typedef              CLSID * LPCLSID;
#define CLSID_NULL          GUID_NULL
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)


#if defined(__cplusplus)
#define REFGUID             const GUID &
#define REFIID              const IID &
#define REFCLSID            const CLSID &
#else
#define REFGUID             const GUID * const
#define REFIID              const IID * const
#define REFCLSID            const CLSID * const
#endif


#ifndef INITGUID
#include "coguid.h"
#endif


/****** Other value types ***************************************************/

// memory context values; passed to CoGetMalloc
typedef enum tagMEMCTX
{
    MEMCTX_TASK = 1,            // task (private) memory
    MEMCTX_SHARED = 2,           // shared memory (between processes)

    // these are mostly for internal use...
    MEMCTX_UNKNOWN = -1,        // unknown context (when asked about it)
    MEMCTX_SAME = -2           // same context (as some other pointer)
	,MEMCTX_FORCELONG	=	2147483647
} MEMCTX;



// class context: used to determine what scope and kind of class object to use
// NOTE: this is a bitwise enum
typedef enum tagCLSCTX
{
    CLSCTX_INPROC_SERVER = 1,   // server dll (runs in same process as caller)
    CLSCTX_INPROC_HANDLER = 2,  // handler dll (runs in same process as caller)
    CLSCTX_LOCAL_SERVER = 4     // server exe (runs on same machine; diff proc)
	,CLSCTX_FORCELONG	=	2147483647
} CLSCTX;

#define CLSCTX_ALL              (CLSCTX_INPROC_SERVER| \
                                 CLSCTX_INPROC_HANDLER| \
                                 CLSCTX_LOCAL_SERVER)

#define CLSCTX_INPROC           (CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER)

#define CLSCTX_SERVER           (CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER)


// class registration flags; passed to CoRegisterClassObject
typedef enum tagREGCLS
{
    REGCLS_SINGLEUSE = 0,       // class object only generates one instance
    REGCLS_MULTIPLEUSE = 1,     // same class object genereates multiple inst.
								// and local automatically goes into inproc tbl.
    REGCLS_MULTI_SEPARATE = 2   // multiple use, but separate control over each
								// context.

	// NOTE: CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE is the same as
	// (CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER), REGCLS_MULTI_SEPARATE, but
	// not the same as CLSCTX_LOCAL_SERVER, REGCLS_MULTI_SEPARATE.
	,REGCLS_FORCELONG	=	2147483647
} REGCLS;


// interface marshaling definitions
#define MARSHALINTERFACE_MIN 40 // minimum number of bytes for interface marshl

// marshaling flags; passed to CoMarshalInterface
typedef enum tagMSHLFLAGS
{
    MSHLFLAGS_NORMAL = 0,       // normal marshaling via proxy/stub
    MSHLFLAGS_TABLESTRONG = 1,  // keep object alive; must explicitly release
    MSHLFLAGS_TABLEWEAK = 2     // doesn't hold object alive; still must release
	,MSHLFLAGS_FORCELONG	=	2147483647
} MSHLFLAGS;

// marshal context: determines the destination context of the marshal operation
typedef enum tagMSHCTX
{
    MSHCTX_LOCAL = 0, 		    // unmarshal context is local (eg.shared memory)
    MSHCTX_NOSHAREDMEM = 1     // unmarshal context has no shared memory access
	,MSHCTX_FORCELONG	=	2147483647
} MSHCTX;


// call type used by IMessageFilter::HandleIncommingMessage
typedef enum tagCALLTYPE
{
    CALLTYPE_TOPLEVEL = 1,      // toplevel call - no outgoing call
    CALLTYPE_NESTED   = 2,      // callback on behalf of previous outgoing call - should always handle
    CALLTYPE_ASYNC    = 3,	    // aysnchronous call - can NOT be rejected
    CALLTYPE_TOPLEVEL_CALLPENDING = 4,  // new toplevel call with new LID
    CALLTYPE_ASYNC_CALLPENDING    = 5   // async call - can NOT be rejected
	,CALLTYPE_FORCELONG	=	2147483647
} CALLTYPE;


typedef struct tagINTERFACEINFO
{
    interface IUnknown *pUnk;       // the pointer to the object
    IID      			iid;            // interface id
    short        		wMethod;        // interface methode
} INTERFACEINFO, * LPINTERFACEINFO;

// status of server call - returned by IMessageFilter::HandleIncommingCall
// and passed to  IMessageFilter::RetryRejectedCall
typedef enum tagSERVERCALL
{
    SERVERCALL_ISHANDLED    = 0,
    SERVERCALL_REJECTED     = 1,
    SERVERCALL_RETRYLATER   = 2
	,SERVERCALL_FORCELONG	=	2147483647
} SERVERCALL;

// Pending type indicates the level of nesting
typedef enum tagPENDINGTYPE
{
    PENDINGTYPE_TOPLEVEL = 1, // toplevel call
    PENDINGTYPE_NESTED   = 2  // nested call
	,PENDINGTYPEE_FORCELONG	=	2147483647
} PENDINGTYPE;

// return values of MessagePending
typedef enum tagPENDINGMSG
{
	PENDINGMSG_CANCELCALL  = 0, // cancel the outgoing call
	PENDINGMSG_WAITNOPROCESS  = 1, // wait for the return and don't dispatch the message
	PENDINGMSG_WAITDEFPROCESS = 2  // wait and dispatch the message 	
	,PENDINGMSG_FORCELONG	=	2147483647
} PENDINGMSG;

// bit flags for IExternalConnection
typedef enum tagEXTCONN
{
	EXTCONN_STRONG		= 0x0001	// strong connection
	,EXTCONN_FORCELONG	=	2147483647
} EXTCONN;



/****** IUnknown Interface **************************************************/


#undef  INTERFACE
#define INTERFACE   IUnknown

#if defined(applec) || defined(__MWERKS__)
// MPW declaration to force SI vtable construction.
DECLARE_INTERFACE_(IUnknown, SingleObject)
#else
DECLARE_INTERFACE(IUnknown)
#endif
{
    BEGIN_INTERFACE
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;
};
typedef        IUnknown * LPUNKNOWN, ** LPLPUNKNOWN;


/****** Class Factory Interface *******************************************/


#undef  INTERFACE
#define INTERFACE   IClassFactory

DECLARE_INTERFACE_(IClassFactory, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IClassFactory methods ***
    STDMETHOD(CreateInstance) (THIS_ LPUNKNOWN pUnkOuter,
                              REFIID riid,
                              void * * ppvObject) PURE;
    STDMETHOD(LockServer) (THIS_ OLEBOOL fLock) PURE;

};
typedef       IClassFactory * LPCLASSFACTORY, ** LPLPCLASSFACTORY;


/****** Memory Allocation Interface ***************************************/


#undef  INTERFACE
#define INTERFACE   IMalloc

DECLARE_INTERFACE_(IMalloc, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IMalloc methods ***
    STDMETHOD_(void *, Alloc) (THIS_ unsigned long cb) PURE;
    STDMETHOD_(void *, Realloc) (THIS_ void * pv, unsigned long cb) PURE;
    STDMETHOD_(void, Free) (THIS_ void * pv) PURE;
    STDMETHOD_(unsigned long, GetSize) (THIS_ void * pv) PURE;
    STDMETHOD_(int, DidAlloc) (THIS_ void * pv) PURE;
    STDMETHOD_(void, HeapMinimize) (THIS) PURE;
};
typedef       IMalloc * LPMALLOC, ** LPLPMALLOC;

DECLARE_INTERFACE_(IDebugMalloc, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IDebugMalloc methods ***
    STDMETHOD_(void, ListAllBlocks)(THIS) PURE;
    STDMETHOD_(void, ListTaggedBlocks)(THIS_ unsigned long dwTag) PURE;
    STDMETHOD_(void, ListMySharedBlocks)(THIS_ unsigned long dwTag) PURE;
    STDMETHOD_(unsigned long, SetBlockTag)(THIS_ unsigned long dwTag) PURE;
    STDMETHOD_(unsigned long, GetBlockTag)(THIS) PURE;
    STDMETHOD_(unsigned long, ResetBlockNum)(THIS_ unsigned long dwNextBlkNum) PURE;
    STDMETHOD_(unsigned long, GetBlockNum)(THIS) PURE;
    STDMETHOD_(void, MonitorBlocks)(THIS_ unsigned long nBlocks, unsigned long *vBlockNums, unsigned long bAssert, unsigned long bFail) PURE;
    STDMETHOD_(void, ListAllHeaps)(THIS) PURE;
    STDMETHOD_(unsigned long, IsBlockCorrupt)(THIS_ void *pBlock) PURE;
    STDMETHOD_(unsigned long, CountAllBlocks)(THIS) PURE;
    STDMETHOD_(unsigned long, CountTaggedBlocks)(THIS_ unsigned long dwTag) PURE;
    STDMETHOD_(unsigned long, CountMySharedBlocks)(THIS_ unsigned long dwTag) PURE;
    STDMETHOD_(unsigned long, IsOleShared)(THIS) PURE;
    STDMETHOD_(unsigned long, IsOleTask)(THIS) PURE;
    STDMETHOD_(unsigned long, GetMemctx)(THIS) PURE;
};

typedef       IDebugMalloc * LPDEBUGMALLOC, ** LPLPDEBUGMALLOC;

/****** IMarshal Interface ************************************************/

// forward declaration for IStream; must include storage.h later to use
#ifdef __cplusplus
interface IStream;
#endif
typedef  interface IStream * LPSTREAM, ** LPLPSTREAM;


#undef  INTERFACE
#define INTERFACE   IMarshal

DECLARE_INTERFACE_(IMarshal, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IMarshal methods ***
    STDMETHOD(GetUnmarshalClass)(THIS_ REFIID riid, void * pv,
						unsigned long dwDestContext, void * pvDestContext,
						unsigned long mshlflags, LPCLSID pCid) PURE;
    STDMETHOD(GetMarshalSizeMax)(THIS_ REFIID riid, void * pv,
						unsigned long dwDestContext, void * pvDestContext,
						unsigned long mshlflags, unsigned long	* pSize) PURE;
    STDMETHOD(MarshalInterface)(THIS_ LPSTREAM pStm, REFIID riid,
                        void * pv, unsigned long dwDestContext, void * pvDestContext,
                        unsigned long mshlflags) PURE;
    STDMETHOD(UnmarshalInterface)(THIS_ LPSTREAM pStm, REFIID riid,
                        void * * ppv) PURE;
    STDMETHOD(ReleaseMarshalData)(THIS_ LPSTREAM pStm) PURE;
    STDMETHOD(DisconnectObject)(THIS_ unsigned long dwReserved) PURE;
};
typedef         IMarshal * LPMARSHAL, ** LPLPMARSHAL;


#undef  INTERFACE
#define INTERFACE   IStdMarshalInfo

DECLARE_INTERFACE_(IStdMarshalInfo, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IStdMarshalInfo methods ***
    STDMETHOD(GetClassForHandler)(THIS_ unsigned long dwDestContext,
						void * pvDestContext, LPCLSID pClsid) PURE;
};
typedef         IStdMarshalInfo * LPSTDMARSHALINFO, ** LPLPSTDMARSHALINFO;


/****** Message Filter Interface *******************************************/


#undef  INTERFACE
#define INTERFACE   IMessageFilter

DECLARE_INTERFACE_(IMessageFilter, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IMessageFilter methods ***
    STDMETHOD_(unsigned long, HandleInComingCall) (THIS_ unsigned long dwCallType,
                                ProcessSerialNumber *pPSNCaller, unsigned long dwTickCount,
                                LPINTERFACEINFO lpInterfaceInfo ) PURE;
    STDMETHOD_(unsigned long, RetryRejectedCall) (THIS_
                                ProcessSerialNumber *pPSNCallee, unsigned long dwTickCount,
                                unsigned long dwRejectType ) PURE;
    STDMETHOD_(unsigned long, MessagePending) (THIS_
                                ProcessSerialNumber *pPSNCallee, unsigned long dwTickCount,
                                unsigned long dwPendingType  ) PURE;
};
typedef       IMessageFilter * LPMESSAGEFILTER, ** LPLPMESSAGEFILTER;


/****** External Connection Information ***********************************/

#undef  INTERFACE
#define INTERFACE   IExternalConnection

DECLARE_INTERFACE_(IExternalConnection, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IExternalConnection methods ***
    STDMETHOD_(unsigned long, AddConnection) (THIS_ unsigned long extconn, unsigned long reserved) PURE;
    STDMETHOD_(unsigned long, ReleaseConnection) (THIS_ unsigned long extconn, unsigned long reserved, OLEBOOL fLastReleaseCloses) PURE;
};
typedef       IExternalConnection * LPEXTERNALCONNECTION, ** LPLPEXTERNALCONNECTION;


/****** Enumerator Interfaces *********************************************/

/*
 *  Since we don't use parametrized types, we put in explicit declarations
 *  of the enumerators we need.
 */


#undef  INTERFACE
#define INTERFACE   IEnumString

DECLARE_INTERFACE_(IEnumString, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IEnumString methods ***
    STDMETHOD(Next) (THIS_ unsigned long celt,
                       char * * rgelt,
                       unsigned long * pceltFetched) PURE;
    STDMETHOD(Skip) (THIS_ unsigned long celt) PURE;
    STDMETHOD(Reset) (THIS) PURE;
    STDMETHOD(Clone) (THIS_ IEnumString * * ppenm) PURE;
};
typedef      IEnumString * LPENUMSTRING, ** LPLPENUMSTRING;


#undef  INTERFACE
#define INTERFACE   IEnumUnknown

DECLARE_INTERFACE_(IEnumUnknown, IUnknown)
{
    BEGIN_INTERFACE
    // *** IUnknown methods ***
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, void * * ppvObj) PURE;
    STDMETHOD_(unsigned long,AddRef) (THIS)  PURE;
    STDMETHOD_(unsigned long,Release) (THIS) PURE;

    // *** IEnumUnknown methods ***
    STDMETHOD(Next) (THIS_ unsigned long celt, LPLPUNKNOWN rgelt, unsigned long * pceltFetched) PURE;
    STDMETHOD(Skip) (THIS_ unsigned long celt) PURE;
    STDMETHOD(Reset) (THIS) PURE;
    STDMETHOD(Clone) (THIS_ IEnumUnknown * * ppenm) PURE;
};
typedef         IEnumUnknown * LPENUMUNKNOWN, ** LPLPENUMUNKNOWN;


/****** STD Object API Prototypes *****************************************/

STDAPI_(unsigned long) CoBuildVersion( void );

/* init/uninit */

STDAPI  CoInitialize(LPMALLOC pMalloc);
STDAPI_(void)  CoUninitialize(void);
STDAPI  CoGetMalloc(unsigned long dwMemContext, LPLPMALLOC ppMalloc);
STDAPI_(unsigned long) CoGetCurrentProcess(void);
STDAPI  CoCreateStandardMalloc(unsigned long memctx, IMalloc **ppMalloc);

STDAPI CoGetState(IUnknown **ppunk);
STDAPI CoSetState(IUnknown  *punk);

STDAPI_(void *) CoTaskMemAlloc(unsigned long size);
STDAPI_(void *) CoTaskMemRealloc(void * lpv, unsigned long size);
STDAPI_(void) CoTaskMemFree(void * lpv);

STDAPI_(void *) CoSharedMemAlloc(unsigned long size);
STDAPI_(void *) CoSharedMemRealloc(void * lpv, unsigned long size);
STDAPI_(void) CoSharedMemFree(void * lpv);

STDAPI CoSetTaskNewPtr(Ptr(*newPtrFunc)(Size));


/* register/revoke/get class objects */

STDAPI  CoGetClassObject(REFCLSID rclsid, unsigned long dwClsContext, void * pvReserved,
                    REFIID riid, void * * ppv);
STDAPI  CoRegisterClassObject(REFCLSID rclsid, LPUNKNOWN pUnk,
                    unsigned long dwClsContext, unsigned long flags, unsigned long * lpdwRegister);
STDAPI  CoRevokeClassObject(unsigned long dwRegister);


STDAPI	CoGetClassFrag(REFCLSID rclsid, unsigned long dwClsContext, 
					   ConnectionID *pConnectionID);

STDAPI  CoGetFragVersion(ConnectionID connectionID, unsigned long *pdwVersion);
STDAPI  CoDllMacNativeHandshake(ConnectionID connectionID, unsigned long dwReserved);

STDAPI DllMacNativeHandshake(unsigned long dwReserved);
STDAPI DllGetVersion(unsigned long *pdwVersion);


/* marshaling interface pointers */

STDAPI CoMarshalInterface(LPSTREAM pStm, REFIID riid, LPUNKNOWN pUnk,
                    unsigned long dwDestContext, void * pvDestContext, unsigned long mshlflags);
STDAPI CoUnmarshalInterface(LPSTREAM pStm, REFIID riid, void * * ppv);
STDAPI CoMarshalHresult(LPSTREAM pstm, HRESULT hresult);
STDAPI CoUnmarshalHresult(LPSTREAM pstm, HRESULT  * phresult);
STDAPI CoReleaseMarshalData(LPSTREAM pStm);
STDAPI CoDisconnectObject(LPUNKNOWN pUnk, unsigned long dwReserved);
STDAPI CoLockObjectExternal(LPUNKNOWN pUnk, OLEBOOL fLock, OLEBOOL fLastUnlockReleases);
STDAPI CoGetStandardMarshal(REFIID riid, LPUNKNOWN pUnk,
                    unsigned long dwDestContext, void * pvDestContext, unsigned long mshlflags,
					LPLPMARSHAL ppMarshal);

STDAPI_(OLEBOOL) CoIsHandlerConnected(LPUNKNOWN pUnk);

/* dll loading helpers; keeps track of ref counts and unloads all on exit */

STDAPI_(long) CoLoadLibrary(char * lpszLibName, OLEBOOL bAutoFree);
STDAPI_(void) CoFreeLibrary(long hInst);
STDAPI_(void) CoFreeAllLibraries(void);
STDAPI_(void) CoFreeUnusedLibraries(void);


/* helper for creating instances */

STDAPI CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter,
                    unsigned long dwClsContext, REFIID riid, void * * ppv);


/* other helpers */

STDAPI_(OLEBOOL) IsEqualGUID(REFGUID rguid1, REFGUID rguid2);
STDAPI StringFromCLSID(REFCLSID rclsid, char * * lplpsz);
STDAPI CLSIDFromString(char * lpsz, LPCLSID pclsid);
STDAPI StringFromIID(REFIID rclsid, char * * lplpsz);
STDAPI IIDFromString(char * lpsz, LPIID lpiid);
STDAPI_(OLEBOOL) CoIsOle1Class(REFCLSID rclsid);
STDAPI ProgIDFromCLSID (REFCLSID clsid, char * * lplpszProgID);
STDAPI CLSIDFromProgID (const char * lpszProgID, LPCLSID lpclsid);

STDAPI_(int) StringFromGUID2(REFGUID rguid, char *lpsz, int cbMax);

STDAPI CoCreateGuid(GUID *pguid);


STDAPI_(OLEBOOL) CoFileTimeToMacDateTime(FILETIME * lpFileTime, unsigned long *psecs);
STDAPI_(OLEBOOL) CoMacDateTimeToFileTime(unsigned long secs, FILETIME * lpFileTime);

STDAPI CoFileTimeNow( FILETIME * lpFileTime );

STDAPI CoRegisterMessageFilter( LPMESSAGEFILTER lpMessageFilter,
                                LPLPMESSAGEFILTER lplpMessageFilter );


/* TreatAs APIS */

STDAPI CoGetTreatAsClass(REFCLSID clsidOld, LPCLSID pClsidNew);
STDAPI CoTreatAsClass(REFCLSID clsidOld, REFCLSID clsidNew);


/* the server dlls must define their DllGetClassObject and DllCanUnloadNow
 * to match these; the typedefs are located here to ensure all are changed at
 * the same time.
 */

STDAPI  DllGetClassObject(REFCLSID rclsid, REFIID riid, void * * ppv);
STDAPI  DllCanUnloadNow(void);

#if !defined(_MSC_VER)
typedef STDAPICALLTYPE HRESULT (* LPFNGETCLASSOBJECT) (REFCLSID, REFIID, void * *);
typedef STDAPICALLTYPE HRESULT (* LPFNCANUNLOADNOW)(void);
#else
typedef HRESULT (STDAPICALLTYPE * LPFNGETCLASSOBJECT) (REFCLSID, REFIID, void * *);
typedef HRESULT (STDAPICALLTYPE * LPFNCANUNLOADNOW)(void);

typedef HRESULT (STDAPICALLTYPE * LPFNDLLMACNATIVEHANDSHAKE)(unsigned long);
typedef HRESULT (STDAPICALLTYPE * LPFNDLLGETVERSION)(unsigned long *);

#endif



/****** Debugging Helpers *************************************************/

#ifdef _DEBUG
// writes to the debug port and displays a message box
STDAPI FnAssert(char * lpstrExpr, char * lpstrMsg, char * lpstrFileName, unsigned short iLine);
#endif /* _DEBUG */

#endif /* !__COMPOBJ__ */

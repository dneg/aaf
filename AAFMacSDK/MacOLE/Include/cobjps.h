/*****************************************************************************\
*                                                                             *
* cobjps.h -    Definitions for writing standard proxies and stubs			  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/

#if !defined( __COBJPS__ ) && !defined( _COBJPS_H_ )
#define __COBJPS__
#define _COBJPS_H_


/****** IRpcChannel Interface ***********************************************/
#undef  INTERFACE
#define INTERFACE   IRpcChannel

DECLARE_INTERFACE_(IRpcChannel, IUnknown)
{
	BEGIN_INTERFACE
    STDMETHOD(GetStream)(REFIID iid, int iMethod, OLEBOOL  fSend,
                     OLEBOOL  fNoWait, unsigned long size, IStream * * ppIStream) PURE;
    STDMETHOD(Call)(IStream * pIStream) PURE;
    STDMETHOD(GetDestCtx)(unsigned long * lpdwDestCtx, void * * lplpvDestCtx) PURE;
    STDMETHOD(IsConnected)(void) PURE;
};


/****** IRpcProxy Interface *************************************************/

// IRpcProxy is an interface implemented by proxy objects.  A proxy object has
// exactly the same interfaces as the real object in addition to IRpcProxy.
//

#undef  INTERFACE
#define INTERFACE   IRpcProxy

DECLARE_INTERFACE_(IRpcProxy, IUnknown)
{
	BEGIN_INTERFACE
    STDMETHOD(Connect)(IRpcChannel * pRpcChannel) PURE;
    STDMETHOD_(void, Disconnect)(void) PURE;
};


/****** IRpcStub Interface **************************************************/

// IRpcStub is an interface implemented by stub objects.  
//

#undef  INTERFACE
#define INTERFACE   IRpcStub

DECLARE_INTERFACE_(IRpcStub, IUnknown)
{
 	BEGIN_INTERFACE
    STDMETHOD(Connect)(IUnknown * pUnk) PURE;
    STDMETHOD_(void, Disconnect)(void) PURE;
    STDMETHOD(Invoke)(REFIID iid, int iMethod, IStream * pIStream,
            unsigned long dwDestCtx, void * lpvDestCtx) PURE;
    STDMETHOD_(OLEBOOL, IsIIDSupported)(REFIID iid) PURE;
    STDMETHOD_(unsigned long, CountRefs)(void) PURE;
};


/****** IPSFactory Interface ************************************************/

// IPSFactory - creates proxies and stubs
//

#undef  INTERFACE
#define INTERFACE   IPSFactory

DECLARE_INTERFACE_(IPSFactory, IUnknown)
{
	BEGIN_INTERFACE
    STDMETHOD(CreateProxy)(IUnknown * pUnkOuter, REFIID riid, 
        IRpcProxy * * ppProxy, void * * ppv) PURE;
    STDMETHOD(CreateStub)(REFIID riid, IUnknown * pUnkServer,
        IRpcStub * * ppStub) PURE;
};

#endif /* !__COBJPS__ */


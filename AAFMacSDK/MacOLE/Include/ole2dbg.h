/*
	ole2dbg.h:	This header file contains the function declarations for the publicly
	exported debugging interfaces.

	Include *after* standard OLE2 includes.
	
	Copyright (c) 1992-1997, Microsoft Corp. All rights reserved.
*/

#if !defined(__OLE2DBG__) && !defined(_OLE2DBG_H_)
#define __OLE2DBG__
#define _OLE2DBG_H_

STDAPI_(void) DbgDumpObject( IUnknown *pUnk, unsigned long dwReserved);
STDAPI_(void) DbgDumpExternalObject( IUnknown *pUnk, unsigned long  dwReserved );

STDAPI_(unsigned long ) DbgIsObjectValid( IUnknown  * pUnk );
STDAPI_(void) DbgDumpClassName( IUnknown * pUnk );

#endif /* !__OLE2DBG__ */

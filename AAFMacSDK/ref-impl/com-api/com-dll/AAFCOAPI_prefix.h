// >>> © 1998 Avid Technology, Inc.  All rights reserved. <<<
#define __dest_os	__mac_os

//#define CHECKMEMORYLEAKS // Uncomment this line to turn leak checking on

#define _MAC
#define NONAMELESSUNION

#include "MacHeaders.h"


#if !defined(CDECL) && defined(_MSC_VER)
#define CDECL	_cdecl
#endif // CDECL

#if !defined(FAR)
#define FAR
#endif

#include <ole2.h>
#include "dispatch.h"
#include "wintypes.h"
#include <olectl.h>

// Define to enable the module testing of the shared library.
#define AAF_MODULE_TEST
//#include "AXControlHeaders.h"
#ifndef _DEBUG
#define _DEBUG
#endif


// AAF OM symbols
//#define OM_ENABLE_DEBUG
//#define OM_ENABLE_TRACE

#define BOB_TEST
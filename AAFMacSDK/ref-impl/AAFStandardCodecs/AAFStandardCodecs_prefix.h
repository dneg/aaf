// >>> © 1998 Avid Technology, Inc.  All rights reserved. <<<
#define __dest_os	__mac_os

//#define CHECKMEMORYLEAKS // Uncomment this line to turn leak checking on

// Symbols needed for OLE/ActiveX headers.
#define _MAC
#define NONAMELESSUNION

// We need to include some of the mac headers before the ole/ActiveX headers
// so that certain include guards will be properly defined. This will keep
// the OLE/ActiveX headers from attempting to include files that do not
// exist on the macintosh such as <codefrag.h>.
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

//#ifndef _DEBUG
//#define _DEBUG
//#endif


// AAF OM symbols
//#define OM_ENABLE_DEBUG
//#define OM_ENABLE_TRACE

#define BOB_TEST

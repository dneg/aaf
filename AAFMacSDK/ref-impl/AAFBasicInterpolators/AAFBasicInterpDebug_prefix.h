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

// Mac OLE/ActiveX headers do not define IID, CLSID, GUID in multiple header
// files so they do not need to have any special include guards to keep
// the symbols from being multiply defined. For compatibility with other WIN32
// OLE/COM we need to define the following symbolic include guards to keep
// the compiler from redefining IID, CLSID and GUID.
#ifndef __IID_DEFINED__
#define __IID_DEFINED__
#endif // __IID_DEFINED__
#ifndef CLSID_DEFINED
#define CLSID_DEFINED
#endif // CLSID_DEFINED
#ifndef GUID_DEFINED
#define GUID_DEFINED
#endif // GUID_DEFINED

// Define to enable the module testing of the shared library.
#define AAF_MODULE_TEST

//#ifndef _DEBUG
//#define _DEBUG
//#endif


// AAF OM symbols
//#define OM_ENABLE_DEBUG
//#define OM_ENABLE_TRACE

// Define "debug" symbol for compatibility with WIN32.
#define _DEBUG
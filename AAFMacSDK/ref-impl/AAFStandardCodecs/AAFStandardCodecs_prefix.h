//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#define __dest_os	__mac_os

//#define CHECKMEMORYLEAKS // Uncomment this line to turn leak checking on

// Symbols needed for OLE/ActiveX headers.
#define _MAC
#define NONAMELESSUNION

#if (__MWERKS__ >= 0x2400)
// required for CW6 ShLib (DLL) targets
#define _MSL_IMP_EXP __declspec(dllimport)
#define _MSL_NO_LOCALE
#endif

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
//#define OM_DEBUG
//#define OM_TRACE

#define BOB_TEST


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

#define _MSL_USING_NAMESPACE

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

// Conditional debugging and ansi assertions disabled
#ifndef NDEBUG
#define NDEBUG
#endif

// Enable PRECONDITION, POSTCONDITION, ASSERT, INVARIANT and IMPLIES macros.
//#define OM_ENABLE_DEBUG

// Uncomment the following line if you want to see an exhausted list
// of the calling sequence within the Object Manager.
//#define OM_TRACE

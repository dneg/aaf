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

#if (__MWERKS__ >= 0x2400)
// required for CW6 ShLib (DLL) targets
#define _MSL_IMP_EXP __declspec(dllimport)
#define _MSL_NO_LOCALE
#endif

#include "MacHeaders.h"


// Conditional debugging and ansi assertions disabled
#ifndef NDEBUG
#define NDEBUG
#endif

// Enable PRECONDITION, POSTCONDITION, ASSERT, INVARIANT and IMPLIES macros.
//#define OM_ENABLE_DEBUG

// Uncomment the following line if you want to see an exhausted list
// of the calling sequence within the Object Manager.
//#define OM_ENABLE_TRACE

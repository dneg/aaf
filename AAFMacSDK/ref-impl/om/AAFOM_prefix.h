
// >>> © 1998 Avid Technology, Inc.  All rights reserved. <<<
#define __dest_os	__mac_os

#define _MSL_USING_NAMESPACE

//#define CHECKMEMORYLEAKS // Uncomment this line to turn leak checking on

#define _MAC
#define NONAMELESSUNION

#include "MacHeaders.h"


// Define to enable the module testing of the shared library.
#define AAF_MODULE_TEST
//#ifndef _DEBUG
//#define _DEBUG
//#endif

// Enable PRECONDITION, POSTCONDITION, ASSERT, INVARIANT and IMPLIES macros.
#define OM_ENABLE_DEBUG

// Uncomment the following line if you want to see an exhausted list
// of the calling sequence within the Object Manager.
//#define OM_ENABLE_TRACE
#ifndef __AAFCOMPlatform_h__
#define __AAFCOMPlatform_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/
// Files that contain standard types for windows/com interfaces.
// Files that contain DECLARE_INTERFACE macros, IUnknown and IClassFactory.

// NOTE: This file may have to be modified to support other COM platforms.

#if defined(_MAC) || defined(_MAC_) || defined(macintosh)
  // Include files form ActiveX SDK for the Macintosh
  #include <wintypes.h>
  #include <compobj.h>
#elif defined(WIN32) || defined(_WIN32)
  #include <unknwn.h>
  #include <objbase.h>
#else
  // Default to the Structured Storage reference implementation.
  #include "REF.HXX"
  
  // Make sure begin/end interface are at least defined. These macros are required
  // for all AAF interface files.
  #ifndef BEGIN_INTERFACE
    #define BEGIN_INTERFACE
  #endif
  #ifndef END_INTERFACE
    #define END_INTERFACE
  #endif
#endif

// Include our Types here so that we do not have to reference it from every
// CoClass header file.
#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif


#endif // __AAFCOMPlatform_h__


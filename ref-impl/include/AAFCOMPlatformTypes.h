#ifndef __AAFCOMPlatformTypes_h__
#define __AAFCOMPlatformTypes_h__

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
  // Include file form ActiveX SDK for the Macintosh. Otherwise the 
  // OLE (2.08, 2.24) interfaces cannot be used since they use standard
  // window's types and defines...
  #include <wintypes.h>
#elif defined(WIN32) || defined(_WIN32)
  #include <wtypes.h>
#else
  // Default to the reference implementation.
  #include "REF.HXX"
#endif


#endif // __AAFCOMPlatformTypes_h__


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
  // Include file form ActiveX SDK for the Macintosh
  #include <wintypes.h>
#elif defined(WIN32) || defined(_WIN32)
  #include <wtypes.h>
#else
  // Default to the reference implementation.
  #include "REF.HXX"
#endif


#endif // __AAFCOMPlatform_h__


#ifndef __AAFCOMPlatform_h__
#define __AAFCOMPlatform_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=
// Files that contain standard types for windows/com interfaces.
// Files that contain DECLARE_INTERFACE macros, IUnknown and IClassFactory.

// NOTE: This file may have to be modified to support other COM platforms.

#ifndef __AAFCOMPlatformTypes_h__
#include "AAFCOMPlatformTypes.h"
#endif

#if defined( OS_WINDOWS )
#  include <unknwn.h>
#  include <objbase.h>


#elif defined( OS_UNIX )

  STDAPI  DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);

#  ifndef INITGUID
#    define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)      \
        EXTERN_C const GUID name                                          
#  else                                                                     
#    define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)      \
      EXTERN_C const GUID name;                                         \
      EXTERN_C const GUID name                                          \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } } 
#  endif // INITGUID

  // Make sure begin/end interface are at least defined. These macros are required
  // for all AAF interface files.
#  ifndef BEGIN_INTERFACE
#    define BEGIN_INTERFACE
#  endif
  #ifndef END_INTERFACE
    #define END_INTERFACE
#  endif

  // Stolen from wintype.h (from vc++ 6.0)
#  define WINAPI

  // Stolen from winbase.h (from vc++ 6.0)
#  define WINBASEAPI

  WINBASEAPI
  DWORD
  WINAPI
  GetModuleFileNameW(
      HMODULE hModule,
      LPWSTR lpFilename,
      DWORD nSize
      );
 
# define GetModuleFileName  GetModuleFileNameW

#else
#error Unknown operating system


#endif


#endif // __AAFCOMPlatform_h__





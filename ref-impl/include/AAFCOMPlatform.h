#ifndef __AAFCOMPlatform_h__
#define __AAFCOMPlatform_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/
// Files that contain standard types for windows/com interfaces.
// Files that contain DECLARE_INTERFACE macros, IUnknown and IClassFactory.

// NOTE: This file may have to be modified to support other COM platforms.

#ifndef __AAFCOMPlatformTypes_h__
#include "AAFCOMPlatformTypes.h"
#endif

#if defined(_MAC) || defined(_MAC_) || defined(macintosh)
  // Include files from ActiveX SDK for the Macintosh
#  include <compobj.h>
#elif defined(WIN32) || defined(_WIN32)
#  include <unknwn.h>
#  include <objbase.h>
#elif defined (__sgi) || defined (__FreeBSD__)

  EXTERN_C STDAPI  DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);

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
  // Default to the Structured Storage reference implementation.
#  include "REF.HXX"
  
  // Make sure begin/end interface are at least defined. These macros are required
  // for all AAF interface files.
#  ifndef BEGIN_INTERFACE
#    define BEGIN_INTERFACE
#  endif
#  ifndef END_INTERFACE
#    define END_INTERFACE
#  endif
#endif


#endif // __AAFCOMPlatform_h__





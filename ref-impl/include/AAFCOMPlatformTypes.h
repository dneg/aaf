#ifndef __AAFCOMPlatformTypes_h__
#define __AAFCOMPlatformTypes_h__

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

#if defined(_MAC) || defined(_MAC_) || defined(macintosh)
  // Include file form ActiveX SDK for the Macintosh. Otherwise the 
  // OLE (2.08, 2.24) interfaces cannot be used since they use standard
  // window's types and defines...
  #include <wintypes.h>
#elif defined(WIN32) || defined(_WIN32)
  #include <wtypes.h>
#else
  // Default to the reference implementation.
  #include "ref.hxx"
#endif


#endif // __AAFCOMPlatformTypes_h__


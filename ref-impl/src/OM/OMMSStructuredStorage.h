/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

// @doc OMEXTERNAL
#ifndef OMMSSTRUCTUREDSTORAGE_H
#define OMMSSTRUCTUREDSTORAGE_H

#include "OMDataTypes.h"

// Figure out which Microsoft supplied Structured Storage implementation
// to use. Here there are three different implementations to consider.
//
// 1) The standard implementation on Windows platforms
//    for which USE_WINDOWS_SS is defined
// 2) The implementation on Macintosh (pre OSX)
//    for which USE_MACINTOSH_SS is defined
// 3) The reference implementation
//    for which USE_REFERENCE_SS is defined
//
#if defined(_WIN32)
#define USE_WINDOWS_SS
#elif defined(_MAC) || defined(macintosh)
#define USE_MACINTOSH_SS
#elif defined(__sgi) || defined(__linux__) || defined (__FreeBSD__)
#define USE_REFERENCE_SS
#endif

// Each Microsoft supplied Structured Storage implementation requires
// us to include different header files.
// 
#if defined(USE_WINDOWS_SS)
#include <objbase.h>
#elif defined(USE_MACINTOSH_SS)
#include "wintypes.h"
#include <storage.h>
#elif defined (USE_REFERENCE_SS)
#include "storage.h"
#else
#error "Don't know which structured storage implementation to use."
#endif

#if defined(USE_MACINTOSH_SS) || defined(USE_REFERENCE_SS)

// The Macintosh and reference implementation declarations
// for LARGE_INTEGER and ULARGE_INTEGER don't have a QuadPart.
// On Macintosh this is probably because the HighPart and LowPart
// components on that platform are not in the natural platform order.
// Here we hide these differences behind a couple of functions.

static inline OMUInt64 toOMUInt64(const ULARGE_INTEGER &x)
{
  OMUInt64 result = (((OMUInt64)x.HighPart) << 32) + (x.LowPart);
  return result;
}

static inline ULARGE_INTEGER fromOMUInt64(const OMUInt64& x)
{
  ULARGE_INTEGER result;
  result.HighPart = (unsigned long)(((OMUInt64)x) >> 32);
  result.LowPart  = (unsigned long) (0x00000000FFFFFFFF & (OMUInt64)x);
  return result;
}

#else

static inline OMUInt64 toOMUInt64(const ULARGE_INTEGER &x)
{
  OMUInt64 result;
  result = x.QuadPart;
  return result;
}


static inline ULARGE_INTEGER fromOMUInt64(const OMUInt64& x)
{
  ULARGE_INTEGER result;
  result.QuadPart = x;
  return result;
}

#endif

#endif

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
#ifndef OMDATATYPES_H
#define OMDATATYPES_H

// Figure out the compiler and define a symbol of the
// form COMPILER_<compiler name>_<processor>_<operating system>
//
// If your compiler is not listed please add code to detect it here
// and add a section defining the OM[U]Int{8|16|32}64} types below.
//
#if defined (_MSC_VER) && defined(_M_IX86) && defined(_WIN32)
#define COMPILER_MSC_INTEL_WINDOWS
#elif defined(__MWERKS__) && defined(__POWERPC__) && defined(macintosh)
#define COMPILER_MWERKS_PPC_MACOS
#elif defined(__GNUC__) && defined(__mips__) && defined(__sgi__)
#define COMPILER_GCC_MIPS_SGI
#elif defined(__GNUC__) && defined(__i386__) && defined(__linux__)
#define COMPILER_GCC_INTEL_LINUX
#elif defined(__GNUC__) && defined(__i386__) && defined(__FreeBSD__)
#define COMPILER_GCC_INTEL_FREEBSD
#elif defined(mips) && defined(sgi)
#define COMPILER_SGICC_MIPS_SGI
#else
#error "Unknown compiler"
#endif

#if defined(COMPILER_MSC_INTEL_WINDOWS)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef _int64                 OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned _int64        OMUInt64;

#elif defined(COMPILER_MWERKS_PPC_MACOS)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef long long int          OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned long long int OMUInt64;

#elif defined(COMPILER_GCC_MIPS_SGI)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef long long int          OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned long long int OMUInt64;

#elif defined(COMPILER_GCC_INTEL_LINUX)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef long long int          OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned long long int OMUInt64;

#elif defined(COMPILER_GCC_INTEL_FREEBSD)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef long long int          OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned long long int OMUInt64;

#elif defined(COMPILER_SGICC_MIPS_SGI)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef long long int          OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned long long int OMUInt64;

#endif

typedef OMUInt8                OMByte;
typedef OMUInt16               OMCharacter;

typedef OMUInt16 OMPropertyId;
typedef OMUInt16 OMPropertySize;
typedef OMUInt16 OMPropertyCount;
typedef OMUInt32 OMPropertyOffset;
typedef OMUInt16 OMStoredForm;
typedef OMUInt8  OMVersion;
typedef OMUInt8  OMKeySize;

typedef OMUInt8 OMByteOrder;

const OMByteOrder littleEndian      = 0x4c; // 'L' for little
const OMByteOrder bigEndian         = 0x42; // 'B' for big
const OMByteOrder unspecified       = 0x55; // 'U' for unspecified

#include <string.h>  // For use of memcmp() below.

// A GUID in all but name.
//
struct OMObjectIdentification
{
    OMUInt32 Data1;
    OMUInt16 Data2;
    OMUInt16 Data3;
    OMUInt8  Data4[8];
};

// OMObjectIdentification comparison operators.
//
inline bool operator == (const OMObjectIdentification& lhs,
                         const OMObjectIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMObjectIdentification)) == 0;
}

inline bool operator != (const OMObjectIdentification& lhs,
                         const OMObjectIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMObjectIdentification)) != 0;
}

inline bool operator < (const OMObjectIdentification& lhs,
                        const OMObjectIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMObjectIdentification)) < 0;
}

inline bool operator > (const OMObjectIdentification& lhs,
                        const OMObjectIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMObjectIdentification)) > 0;
}

// For identifying the class of a persisted object.
//
typedef OMObjectIdentification OMStoredObjectIdentification;

// For uniquely identifying an object instance.
//
typedef OMObjectIdentification OMUniqueObjectIdentification;
const OMUniqueObjectIdentification nullOMUniqueObjectIdentification = {0};

// For backwards compatibility - all instances of OMClassId should
// eventually be replaced with OMStoredObjectIdentification.
//
typedef OMStoredObjectIdentification OMClassId;

const OMClassId nullOMClassId = {0};

// A UMID in all but name.
//
struct OMMaterialIdentification
{
  OMUInt8 SMPTELabel[12];
  OMUInt8 length;
  OMUInt8 instanceHigh;
  OMUInt8 instanceMid;
  OMUInt8 instanceLow;
  OMObjectIdentification material;
};

// OMMaterialIdentification comparison operators.
//
inline bool operator == (const OMMaterialIdentification& lhs,
                         const OMMaterialIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMMaterialIdentification)) == 0;
}

inline bool operator != (const OMMaterialIdentification& lhs,
                         const OMMaterialIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMMaterialIdentification)) != 0;
}

inline bool operator < (const OMMaterialIdentification& lhs,
                        const OMMaterialIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMMaterialIdentification)) < 0;
}

inline bool operator > (const OMMaterialIdentification& lhs,
                        const OMMaterialIdentification& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(OMMaterialIdentification)) > 0;
}

// For uniquely identifying material instances.
//
typedef OMMaterialIdentification OMUniqueMaterialIdentification;

// File signatures
//
typedef OMObjectIdentification OMFileSignature;

// Property instance tags.
//
typedef OMUInt16 OMPropertyTag;

const OMPropertyTag nullOMPropertyTag = ~(OMPropertyTag)0;

#endif

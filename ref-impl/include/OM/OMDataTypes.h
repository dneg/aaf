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

// @doc OMEXTERNAL
#ifndef OMDATATYPES_H
#define OMDATATYPES_H

// @module OMDataTypes | Host independent data type definitions used
//         by the Object Manager.
//   @mauthor Tim Bingham | tjb | Avid Technology, Inc.

// Figure out the compiler and define a symbol of the
// form COMPILER_<compiler name>_<processor>_<operating system>
//
// If your compiler is not listed please add code to detect it here
// and add a section defining the OM[U]Int{8|16|32}64} types below.
// This allows new platforms to be added without disturbing the code
// for existing platforms.
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
#elif defined(__GNUC__) && defined(__ppc__) && defined(__APPLE__)
#define COMPILER_GCC_PPC_MACOSX
#elif defined(__GNUC__) && defined(__sparc__) && defined(__sun__)
#define COMPILER_GCC_SPARC_SUNOS
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

#elif defined(COMPILER_GCC_PPC_MACOSX)
typedef signed char            OMInt8;
typedef short int              OMInt16;
typedef long int               OMInt32;
typedef long long int          OMInt64;

typedef unsigned char          OMUInt8;
typedef unsigned short int     OMUInt16;
typedef unsigned long int      OMUInt32;
typedef unsigned long long int OMUInt64;

#elif defined(COMPILER_GCC_SPARC_SUNOS)
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
const OMFileSignature nullOMFileSignature = {0};

// Property instance tags.
//
typedef OMUInt16 OMPropertyTag;

const OMPropertyTag nullOMPropertyTag = ~(OMPropertyTag)0;

#endif

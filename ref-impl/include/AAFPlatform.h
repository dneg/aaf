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
#ifndef __AAFPlatform_h__
#define __AAFPlatform_h__


/* wchar_t definition */
#include <stddef.h>



/****************************************************************
 *
 *  Platform definition
 *
 *  This section defines platform the SDK is built on.
 *  The platform definition consists of three components:
 *      compiler, processor type and operating system.
 *  Together with platform definition macro, for each component
 *  the separate macro is defined so as result there are four
 *  definitions:
 *      COMPILER_*, CPU_*, OS_*, PLATFORM_*.
 *
 ***************************************************************/

/*
 *  Compiler:   Microsoft Visual C++
 *  Processor:  Intel x86
 *  OS:         Windows NT
 */
#if   defined(_MSC_VER) && defined(_M_IX86) && defined(_WIN32)
#define CPU_INTEL
#define OS_WINDOWS
#define COMPILER_MSC
#define PLATFORM_MSC_INTEL_WINDOWS
    
/*
 *  Compiler:   Metrowerks CodeWarrior
 *  Processor:  PowerPC
 *  OS:         Classic MacOS
 */
#elif defined(__MWERKS__) && defined(__POWERPC__) && defined(macintosh)
#define CPU_POWERPC
#define OS_MACOS
#define COMPILER_MWERKS
#define PLATFORM_MWERKS_POWERPC_MACOS
    
/*
 *  Compiler:   GNU C++
 *  Processor:  MIPS
 *  OS:         IRIX
 */
#elif defined(__GNUC__) && defined(__mips__) && defined(__sgi__)
#define CPU_MIPS
#define OS_IRIX
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_MIPS_IRIX
    
/*
 *  Compiler:   MIPSpro C++
 *  Processor:  MIPS
 *  OS:         IRIX
 */
#elif defined(mips) && defined(sgi)
#define CPU_MIPS
#define OS_IRIX
#define OS_UNIX
#define COMPILER_MIPSPRO
#define PLATFORM_MIPSPRO_MIPS_IRIX
    
/*
 *  Compiler:   GNU C++
 *  Processor:  Intel x86
 *  OS:         Classic MacOS
 */
#elif defined(__GNUC__) && defined(__i386__) && defined(__linux__)
#define CPU_INTEL
#define OS_LINUX
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_INTEL_LINUX
    
/*
 *  Compiler:   GNU C++
 *  Processor:  Intel x86
 *  OS:         FreeBSD
 */
#elif defined(__GNUC__) && defined(__i386__) && defined(__FreeBSD__)
#define CPU_INTEL
#define OS_FREEBSD
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_INTEL_FREEBSD
 
/*
 *  Compiler:   GNU C++
 *  Processor:  PowerPC
 *  OS:         MacOS 10
 */
#elif defined(__GNUC__) && defined(__ppc__) && defined(__APPLE__) && defined(__APPLE_CC__)
#define CPU_POWERPC
#define OS_MACOS10
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_POWERPC_MACOS10

#else
#error Unknown platform

#endif
/* End of Platform definition */



/****************************************************************
 *
 *  Basic types
 *
 ***************************************************************/
    
/* Reset definition */
#ifdef AAF_INT64_NATIVE
#undef AAF_INT64_NATIVE
#endif 


/*
 *  Classic MacOS
 */
#if defined( PLATFORM_MWERKS_POWERPC_MACOS )
typedef signed char		aafInt8;
typedef signed short int	aafInt16;
typedef signed long int		aafInt32;

typedef unsigned char		aafUInt8;
typedef unsigned short int	aafUInt16;
typedef unsigned long int	aafUInt32;

#if (__MWERKS__ >= 0x1100)
typedef signed long long int	aafInt64;
typedef unsigned long long int	aafUInt64;
#define AAF_INT64_NATIVE	1
#endif

#if !__option(wchar_type)
typedef aafUInt16		aafWChar;
typedef aafUInt16		aafCharacter;

#else
typedef wchar_t			aafWChar;
typedef wchar_t			aafCharacter;

#endif


/*
 *  Windows
 */
#elif defined( PLATFORM_MSC_INTEL_WINDOWS )
typedef signed char		aafInt8;
typedef signed short int	aafInt16;
typedef signed long int		aafInt32;

typedef unsigned char		aafUInt8;
typedef unsigned short int	aafUInt16;
typedef unsigned long int	aafUInt32;

typedef __int64			aafInt64;
typedef unsigned __int64	aafUInt64;
#define AAF_INT64_NATIVE	1

typedef wchar_t			aafWChar;
typedef wchar_t			aafCharacter;


/*
 *  IRIX
 */
#elif defined(PLATFORM_MIPSPRO_MIPS_IRIX) || defined(PLATFORM_GCC_MIPS_IRIX)
typedef signed char		aafInt8;
typedef signed short int	aafInt16;
typedef signed long int		aafInt32;

typedef unsigned char		aafUInt8;
typedef unsigned short int	aafUInt16;
typedef unsigned long int	aafUInt32;

#if defined( __LONGLONG ) || defined( _LONGLONG )
typedef signed long long int	aafInt64;
typedef unsigned long long int	aafUInt64;
#define AAF_INT64_NATIVE	1
#endif

typedef wchar_t			aafWChar;
typedef wchar_t			aafCharacter;


/*
 *  Linux, FreeBSD, Darwin
 */
#elif defined(PLATFORM_GCC_INTEL_LINUX) || defined(PLATFORM_GCC_INTEL_FREEBSD) || defined(PLATFORM_GCC_POWERPC_MACOS10)
typedef signed char		aafInt8;
typedef signed short int	aafInt16;
typedef signed long int		aafInt32;

typedef unsigned char		aafUInt8;
typedef unsigned short int	aafUInt16;
typedef unsigned long int	aafUInt32;

typedef signed long long int	aafInt64;
typedef unsigned long long int	aafUInt64;
#define AAF_INT64_NATIVE	1

typedef wchar_t			aafWChar;
typedef wchar_t			aafCharacter;


#endif



/*
 *  Default definitions of 64-bit types
 */
#ifndef AAF_INT64_NATIVE
/* Treated as an integer, defined as array of 4 shorts if needed */
typedef struct _aafInt64_t
{
	aafUInt16       words[4];
} aafInt64;
typedef aafInt64_t              aafUInt64;
#define AAF_INT64_NATIVE        0

#endif


/*
 *  AAFCONSTINT64() AAFCONSTUINT64()
 *
 *  Some compilers require 64-bit integer constants to have a suffix letter
 *  indicating its type. In case of g++ it looks like this: 
 *    0xFFFF1111FFFF1111LL
 *  Here is declaration of AAFCONSTINT64() and AAFCONSTUINT64(). Whenever
 *  64-bit constant appears in the code it should be inside this macro.
 */
#ifdef __GNUC__
#define AAFCONSTINT64(i)	(i##LL)
#define AAFCONSTUINT64(i)	(i##ULL)

#else
#define AAFCONSTINT64(i)	(i)
#define AAFCONSTUINT64(i)	(i)

#endif


#endif // #ifndef __AAFPlatform_h__


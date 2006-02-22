//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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
 *  OS:         MS Windows NT/2000/XP
 */
#if   defined(_MSC_VER) && defined(_M_IX86) && defined(_WIN32)
#define CPU_INTEL
#define OS_WINDOWS
#define COMPILER_MSC
#define PLATFORM_MSC_INTEL_WINDOWS
    
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
 *  OS:         Linux
 */
#elif defined(__GNUC__) && defined(__i386__) && defined(__linux__)
#define CPU_INTEL
#define OS_LINUX
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_INTEL_LINUX

/*
 *  Compiler:   GNU C++
 *  Processor:  x86-64 (both AMD and Intel x86-64)
 *  OS:         Linux
 */
#elif defined(__GNUC__) && defined(__x86_64__) && defined(__linux__)
#define CPU_INTEL
#define OS_LINUX
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_X86_64_LINUX

/*
 *  Compiler:   GNU C++
 *  Processor:  ppc64 (Apple G5 and IBM Power64)
 *  OS:         Linux
 */
#elif defined(__GNUC__) && defined(__powerpc64__) && defined(__linux__)
#define CPU_POWERPC64
#define OS_LINUX
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_POWERPC64_LINUX

/*
 *  Compiler:   GNU C++
 *  Processor:  SPARC
 *  OS:         Solaris
 */
#elif defined(__GNUC__) && defined(__sun) 
#define CPU_SPARC
#define OS_SOLARIS
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_SPARC_SOLARIS
    
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
 *  Processor:  Intel x86
 *  OS:         OpenBSD
 */
#elif defined(__GNUC__) && defined(__i386__) && defined(__OpenBSD__)
#define CPU_INTEL
#define OS_OPENBSD
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_INTEL_OPENBSD
 
/*
 *  Compiler:   GNU C++
 *  Processor:  PowerPC
 *  OS:         MacOS 10 (Darwin)
 */
#elif defined(__GNUC__) && defined(__ppc__) && defined(__APPLE__) && defined(__APPLE_CC__)
#define CPU_POWERPC
#define OS_DARWIN
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_POWERPC_DARWIN

/*
 *  Compiler:   GNU C++
 *  Processor:  Intel x86
 *  OS:         MacOS 10 (Darwin)
 */
#elif defined(__GNUC__) && defined(__i386__) && defined(__APPLE__) && defined(__APPLE_CC__)
#define CPU_INTEL
#define OS_DARWIN
#define OS_UNIX
#define COMPILER_GCC
#define PLATFORM_GCC_INTEL_DARWIN

/*   
 *  Compiler:   Metrowerks CodeWarrior
 *  Processor:  PowerPC  
 *  OS:         Darwin
 *
 * NOTE: CW support untested/incomplete. (cae 24-Feb-2004)
 *
 * NOTE: CW support demands macro PPC_DARWIN be manually defined, usually
 *       in a prefix header. If this is not done, confusion will result with
 *       regard to the intended target OS, MacOS or Darwin (cae 25-Mar-2004)
 */
#elif defined(__MWERKS__) && defined(__POWERPC__) && defined(PPC_DARWIN)
#define CPU_POWERPC
#define OS_DARWIN
#define OS_UNIX
#define COMPILER_MWERKS
#define PLATFORM_MWERKS_POWERPC_DARWIN

/*
 *  Compiler:   GNU C++
 *  Processor:  Intel x86
 *  OS:         Windows/Cygwin
 */
#elif defined(__GNUC__) && defined(__i386__) && defined(__CYGWIN__)
#define CPU_INTEL
#define OS_WINDOWS
#define COMPILER_GCC
#define PLATFORM_GCC_INTEL_WINDOWS

#else
#error Unknown platform

#endif
/* End of Platform definition */



/****************************************************************
 *
 *  Platform Identifying Strings. Human readable.
 *
 ***************************************************************/

#if defined( OS_WINDOWS )
# define PLATFORM_NAME L"Win32"

#elif defined( OS_IRIX )
# define PLATFORM_NAME L"IRIX"

#elif defined( OS_LINUX )
# define PLATFORM_NAME L"Linux"

#elif defined( OS_SOLARIS )
# define PLATFORM_NAME L"Solaris"

#elif defined( OS_FREEBSD )
# define PLATFORM_NAME L"FreeBSD"

#elif defined( OS_OPENBSD )
# define PLATFORM_NAME L"OpenBSD"

#elif defined( OS_DARWIN )
# define PLATFORM_NAME L"MacOS X"

#else
# error "Unknown platform - add platform string";

#endif


    
/****************************************************************
 *
 *  Basic types
 *
 ***************************************************************/
    
/*
 *  MS Windows
 */
#if defined( PLATFORM_MSC_INTEL_WINDOWS )
typedef signed char			aafInt8;
typedef signed short int	aafInt16;
typedef signed long int		aafInt32;

typedef unsigned char		aafUInt8;
typedef unsigned short int	aafUInt16;
typedef unsigned long int	aafUInt32;

typedef __int64				aafInt64;
typedef unsigned __int64	aafUInt64;

typedef wchar_t			aafWChar;
typedef wchar_t			aafCharacter;


/*
 *  Linux, Irix, Darwin, Solaris, FreeBSD, CygWin
 */
#elif defined(PLATFORM_GCC_INTEL_LINUX) \
	|| defined(PLATFORM_GCC_X86_64_LINUX) \
	|| defined(PLATFORM_GCC_POWERPC64_LINUX) \
	|| defined(PLATFORM_GCC_POWERPC_DARWIN) \
	|| defined(PLATFORM_GCC_INTEL_DARWIN) \
	|| defined(PLATFORM_MWERKS_POWERPC_DARWIN) \
	|| defined(PLATFORM_MIPSPRO_MIPS_IRIX) \
	|| defined(PLATFORM_GCC_MIPS_IRIX) \
	|| defined(PLATFORM_GCC_SPARC_SOLARIS) \
	|| defined(PLATFORM_GCC_INTEL_FREEBSD) \
	|| defined(PLATFORM_GCC_INTEL_OPENBSD) \
	|| defined(PLATFORM_GCC_INTEL_WINDOWS)

// Use ISO C99 (also ANSI and POSIX) fixed size integers
#include <inttypes.h>
typedef int8_t			aafInt8;
typedef int16_t			aafInt16;
typedef int32_t			aafInt32;
typedef int64_t			aafInt64;

typedef uint8_t			aafUInt8;
typedef uint16_t		aafUInt16;
typedef uint32_t		aafUInt32;
typedef uint64_t		aafUInt64;

typedef wchar_t			aafWChar;
typedef wchar_t			aafCharacter;

#endif


/*
 *  AAFCONSTINT64() AAFCONSTUINT64()
 *
 *  POSIX compilers require 64-bit integer constants to have a suffix letter
 *  indicating its width. In case of g++ it looks like this: 
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

/*
 *  printf-style formatting for 64bit integers
 *
 *  The POSIX standard does not specify a printf() length modifier for 64bit
 *  integers.  Instead, experiment has revealed the following modifiers
 *  work on their respective platforms.
 */
#if defined(_MSC_VER)
#define AAFFMT64 "I64"
#elif defined(__x86_64__) || defined(__powerpc64__)
#define AAFFMT64 "l"
#else			// all 32bit platforms using POSIX compilers
#define AAFFMT64 "ll"
#endif


#endif // #ifndef __AAFPlatform_h__

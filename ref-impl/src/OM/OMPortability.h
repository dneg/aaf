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

// @doc OMINTERNAL
#ifndef OMPORTABILITY_H
#define OMPORTABILITY_H

// @module OMPortability | Definitions supporting the portability
//         of the Object Manager.
//   @mauthor Tim Bingham | tjb | Avid Technology, Inc.

// Figure out the compiler and define symbols of the form
// OM_COMPILER_<compiler name>_<processor>_<operating system>
// OM_OS_<operating system>
//
#if defined (_MSC_VER) && defined(_M_IX86) && defined(_WIN32)
#define OM_COMPILER_MSC_INTEL_WINDOWS
#define OM_OS_WINDOWS
#elif defined(__MWERKS__) && defined(__POWERPC__) && defined(macintosh)
#define OM_COMPILER_MWERKS_PPC_MACOS
#define OM_OS_MACOS
#elif defined(__GNUC__) && defined(__mips__) && defined(__sgi__)
#define OM_COMPILER_GCC_MIPS_SGI
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__i386__) && defined(__linux__)
#define OM_COMPILER_GCC_INTEL_LINUX
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__i386__) && defined(__FreeBSD__)
#define OM_COMPILER_GCC_INTEL_FREEBSD
#define OM_OS_UNIX
#elif defined(mips) && defined(sgi)
#define OM_COMPILER_SGICC_MIPS_SGI
#define OM_OS_UNIX
#elif defined(__GNUC__) && defined(__ppc__) && defined(__APPLE__)
#define OM_COMPILER_GCC_PPC_MACOSX
#define OM_OS_MACOSX
#elif defined(__GNUC__) && defined(__sparc__) && defined(__sun__)
#define OM_COMPILER_GCC_SPARC_SUNOS
#define OM_OS_UNIX
#else
#error "Unknown compiler"
#endif

#endif


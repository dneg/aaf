/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/

/*************************************************************************
   FILE portvars.h  -- Porting Variables
**************************************************************************

   This file defines porting "variables". These are identifiers whose 
   defined values are derived from the current portkey choices.

   All identifiers are defined.  Check the state of a variable by using
   "#if PORT_xxx", not "#ifdef" or "#if defined".

   All porting variables begin with "PORT_" to avoid any naming conflicts.

   Note that NO macros are to defined in this file.  All platform-
   dependent macros are defined in other files and are based on porting
   variables (NOT on portkey choices).

   Note also that auto-defined identifiers such as __cplusplus, THINK_C,
   __MS_DOS__, unix, etc. should NEVER be referred to in the code.  Each 
   of these has a porting variable equivalent which may change its status
   independent of the auto-defined counterpart.


   CONTRIBUTORS:
      MJV - Michael Vigneau
      JW  - Joe Wisnewski
      CvD - Casper van Dijk, ELECTROGIG Technology BV

   REVISION HISTORY:
      25-Nov-96  CvD  Added little endian byte sex for Linux
      22-Mar-93  MJV  Added var for IEEE 80-bit support
      12-Feb-93  MJV  Added vars for well-known Mob generation (temporary?)
      ??-Feb-93  JW   Added var for shared globals (exeception hdl)
      26-Jan-93  MJV  Redefined BYTESEX vars in terms of "endian";
		                  added FILESYS vars
      21-Dec-92  MJV  File Created

*************************************************************************/
#ifndef portvars_h
#define portvars_h

/* Byte Sex*/
#if defined(PORTKEY_CPU_80386_16) || defined(PORTKEY_CPU_80386_32)
#define PORT_BYTESEX_LITTLE_ENDIAN  1
#define PORT_BYTESEX_BIG_ENDIAN     0
#define CM_80x86    /* for Bento */
#elif defined(PORTKEY_OS_ULTRIX) || defined(NEXT) || defined(linux)
#define PORT_BYTESEX_LITTLE_ENDIAN  1
#define PORT_BYTESEX_BIG_ENDIAN     0
#else
#define PORT_BYTESEX_LITTLE_ENDIAN  0
#define PORT_BYTESEX_BIG_ENDIAN     1
#endif

#if defined(PORTKEY_COMP_MS_CPP)
#define CM_MICROSOFT 1
#endif

#if defined(PORTKEY_COMP_BORLAND_CPP)
#define CM_BORLAND 1
#endif

#if defined(PORTKEY_COMP_UNIX_GCC)
#define PORT_NEEDS_MEMMOVE 1
#define NEEDS_U_LONG 1
#endif

/* Integer Size (in bits)*/
#if defined(PORTKEY_CPU_80386_16)
#define PORT_INTSIZE 16
#else
#define PORT_INTSIZE 32
#endif

/* Maximum allocation amount*/
#if defined(PORTKEY_CPU_80386_16)
#define PORT_MAXALLOC 65536
#else
#define PORT_MAXALLOC MAXINT
#endif



/*--------------------------------*/

/* Language*/
#if defined(PORTKEY_COMP_THINK_C)
#define PORT_LANG_CPLUSMINUS  1
#define PORT_LANG_CPLUSPLUS   0
#elif defined (__cplusplus)
#define PORT_LANG_CPLUSMINUS  0
#define PORT_LANG_CPLUSPLUS   1
#elif defined (PORTKEY_COMP_UNIX_ANSIC) || defined (PORTKEY_COMP_UNIX_GCC)
#define PORT_LANG_CPLUSMINUS  0
#define PORT_LANG_CPLUSPLUS   0
#else
#define PORT_LANG_CPLUSMINUS  0
#define PORT_LANG_CPLUSPLUS   0
#endif

/* Pragmas*/
#define PORT_PRAGMAS_MSOFT      0
#define PORT_PRAGMAS_BORLAND    0
#define PORT_PRAGMAS_SYMANTEC   0
#define PORT_PRAGMAS_SGI        0

#if defined(PORTKEY_COMP_MS_CPP)
#undef  PORT_PRAGMAS_MSOFT
#define PORT_PRAGMAS_MSOFT      1
#elif defined(PORTKEY_COMP_BORLAND_CPP)
#undef  PORT_PRAGMAS_BORLAND
#define PORT_PRAGMAS_BORLAND    1
#elif defined(PORTKEY_COMP_THINK_C)
#undef  PORT_PRAGMAS_SYMANTEC
#define PORT_PRAGMAS_SYMANTEC   1
#elif defined(PORTKEY_COMP_UNIX_CPP)
#undef  PORT_PRAGMAS_SGI
#define PORT_PRAGMAS_SGI        1
#endif


/* Standard Headers*/
#if defined(PORTKEY_OS_UNIX) || defined(PORTKEY_OS_ULTRIX)
#define PORT_UNIX_STD_HDRS  1
#else
#define PORT_UNIX_STD_HDRS  0
#endif

#if defined(PORTKEY_OS_UNIX) || defined(PORTKEY_OS_ULTRIX)
#define PORT_INC_NEEDS_SYSTIME	1
#else
#define PORT_INC_NEEDS_SYSTIME	0
#endif

#if PORT_MEM_DOS16 || defined(_WIN32)
#define PORT_INC_NEEDS_TIMEH	1
#else
#define PORT_INC_NEEDS_TIMEH	0
#endif

/* File System*/
#define PORT_SYS_MAC    0
#define PORT_SYS_UNIX   0
#define PORT_SYS_DOS    0
#define PORT_SYS_HPFS   0
#define PORT_SYS_NTFS   0

#if defined(PORTKEY_OS_MACSYS)
#undef  PORT_SYS_MAC
#define PORT_SYS_MAC    1
#elif defined(PORTKEY_OS_UNIX) || defined(PORTKEY_OS_ULTRIX)
#undef  PORT_SYS_UNIX
#define PORT_SYS_UNIX   1
#elif defined(PORTKEY_OS_DOS)    || defined(PORTKEY_OS_WIN16) || \
      defined(PORTKEY_OS_WIN32s) || defined(PORTKEY_OS_WIN32)
#undef  PORT_SYS_DOS
#define PORT_SYS_DOS    1
#endif

#if defined(THINK_C) || defined(__MWERKS__)
#define PORT_MAC_HAS_CCOMMAND	1
#else
#define PORT_MAC_HAS_CCOMMAND	0
#endif

#if defined(MSDOS)
#define PORT_MEM_DOS16	1
#else
#define PORT_MEM_DOS16	0
#endif

#if defined(MSDOS)
#define PORT_SUPPORTS_DLL	0
#define PORT_COMP_MICROSOFT	1
#elif defined(_WIN32)
#define PORT_SUPPORTS_DLL	1
#define PORT_COMP_MICROSOFT	1
#else
#define PORT_SUPPORTS_DLL	0
#define PORT_COMP_MICROSOFT	0
#endif

#if defined(THINK_C)
#define PORT_MAC_QD_SEPGLOBALS			1		/* Quickdraw globals not in structure */
#else
#define PORT_MAC_QD_SEPGLOBALS			0
#endif

/* Is IEEE 80-bit supported directly?*/
#if defined(PORTKEY_CPU_80386_16) || defined(PORTKEY_CPU_80386_32)
#define PORT_IEEE_80BIT_IS_NATIVE  1
#else
#define PORT_IEEE_80BIT_IS_NATIVE  0
#endif

#if PORTKEY_INT64_NATIVE && !defined(OMF_DONT_USE_NATIVE64)
#define PORT_USE_NATIVE64	1
#else
#define PORT_USE_NATIVE64	0
#endif


#endif /* portvars_h*/
/****************************** END OF FILE *****************************/




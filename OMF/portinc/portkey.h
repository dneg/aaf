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
   FILE portkey.h  -- Portability Keyfile
**************************************************************************

   This file holds the "keys" to the platform-specific porting variables.
   Portkey identifiers begin with "PORTKEY_" to avoid any naming
   conflicts.
  
   The identifiers are defined to be equal to their version numbers 
   (1.00 = 100); these should be looked at only when absolutely required.


   CONTRIBUTORS:
      MJV - Michael Vigneau
      JMC - Jacqueline Caldwell
      JLB - Jeffrey Bedell
      CvD - Casper van Dijk, ELECTROGIG Technology BV

   REVISION HISTORY:
      21-Dec-92  MJV  File Created
      15-Dec-95  JMC  Combined WIN/NT, UNIX, MAC into one file.
      27-Nov-96  CvD  Added sections for HP and Linux

**************************************************************************/
#ifndef portkey_h
#define portkey_h


/*----------------------------------------*/
/*   PLATFORM KEYS*/
/*----------------------------------------*/

/* PORTKEY: CPU choice*/
/* one of:*/
/* */
/* PORTKEY_CPU_80386_32*/
/* PORTKEY_CPU_80386_16*/
/* PORTKEY_CPU_68020*/
/* PORTKEY_CPU_R3000*/
/* PORTKEY_CPU_R4000*/
/* PORTKEY_CPU_ALPHA*/
/* PORTKEY_CPU_*/

/* PORTKEY: OS choice*/
/* one of:*/
/* */
/* PORTKEY_OS_MACSYS*/
/* PORTKEY_OS_MACAUX*/
/* PORTKEY_OS_UNIX*/
/* PORTKEY_OS_XENIX*/
/* PORTKEY_OS_POSIX*/
/* PORTKEY_OS_OS2*/
/* PORTKEY_OS_WIN32*/
/* PORTKEY_OS_WIN32s*/
/* PORTKEY_OS_WIN16*/
/* PORTKEY_OS_DOS*/
/* PORTKEY_OS_*/

/*----------------------------------------*/
/*   DEVELOPMENT ENV. KEYS*/
/*----------------------------------------*/

/* PORTKEY: COMPiler choice*/
/* one of:*/
/* */
/* PORTKEY_COMP_THINK_C*/
/* PORTKEY_COMP_UNIX_CPP*/
/* PORTKEY_COMP_MS_CPP*/
/* PORTKEY_COMP_BORLAND_CPP*/
/* PORTKEY_COMP_*/

/*----------------------------------------*/
/*   DEVELOPMENT ENV. KEYS*/
/*----------------------------------------*/

/* PORTKEY: Application uses multiprocessing, shared memory multiprocessing*/
/*           or no multiprocessing*/
/* one of:*/
/* */
/* PORTKEY_APP_SM_MULTIPROC*/
/* PORTKEY_APP_MULTIPROC*/
/* PORTKEY_APP_SINGLEPROC*/
/* PORTKEY_APP_*/


/*-------------------------------------------------*/
/* Create the proper environment for each platform */
/* 						   */
/* Note the sun and sgi definitions came from how  */
/*   X deals with port specific stuff in Xos.h     */
/*-------------------------------------------------*/

/* MAC Port definitions */
#if defined(THINK_C)

#define PORTKEY_CPU_68020     1
#define PORTKEY_OS_MACSYS 7.1
#define PORTKEY_COMP_THINK_C 6.0
#define PORTKEY_APP_SINGLEPROC

#endif

#if defined(__MWERKS__)

#if powerc
#define PORTKEY_CPU_PPC     1
#else
#define PORTKEY_CPU_68020     1
#endif
#define PORTKEY_OS_MACSYS 7.1
#define PORTKEY_COMP_METROWERKS DR9
#define PORTKEY_APP_SINGLEPROC
#if (__MWERKS__ >= 0x1100)
#define PORTKEY_INT64_NATIVE	1
#define PORTKEY_INT64_TYPE		long long
#endif
#endif

/* SGI Port definitions */
#ifdef sgi

#define PORTKEY_CPU_R4000
#define PORTKEY_OS_UNIX Irix5.3
#define PORTKEY_COMP_UNIX_ANSIC 1
#define PORTKEY_APP_MULTIPROC
#if defined(__LONGLONG) || defined(_LONGLONG)
#define PORTKEY_INT64_NATIVE	1
#define PORTKEY_INT64_TYPE		long long
#endif
#endif

#ifdef NEXT
#define PORTKEY_OS_UNIX NEXT
#endif

/* Sun Port Definitions */
#ifdef sun

#define PORTKEY_CPU_SPARC     1
#define PORTKEY_OS_UNIX SUNOS
#define PORTKEY_COMP_UNIX_GCC 1
#define PORTKEY_APP_MULTIPROC

/* overrides for solaris */
#ifdef solaris
#define PORTKEY_OS_UNIX SOLARIS2.2
#define PORTKEY_COMP_UNIX_ANSIC 1
#endif

#endif

/* PC MSDOS Port Definitions */
#if defined(MSDOS) 

#define PORTKEY_CPU_80386_32
#define PORTKEY_OS_WIN16 1
#define PORTKEY_COMP_MS_CPP 1
#define PORTKEY_APP_SINGLEPROC

#endif 

/* PC Windows 32 Port Definitions */
#if defined(_WIN32)

#define PORTKEY_CPU_80386_32
#define PORTKEY_OS_WIN32 1
/* #define PORTKEY_COMP_MS_CPP 1*/
#define PORTKEY_APP_SINGLEPROC
/*#if (defined(_MSC_VER) && (_MSC_VER > 0x1100))	*/
#if (!defined(MIDL_PASS) || defined(__midl)) && (!defined(_M_IX86) || (defined(_INTEGRAL_MAX_BITS) && _INTEGRAL_MAX_BITS >= 64))
#define PORTKEY_INT64_NATIVE 1
#define PORTKEY_INT64_TYPE   __int64
#endif
#endif

/* HP-UX Port Definitions */
#if defined(hpux)

#define PORTKEY_CPU_
#define PORTKEY_OS_UNIX HP-UX
#define PORTKEY_COMP_UNIX_ANSIC 1
#define PORTKEY_APP_MULTIPROC

#endif

/* Linux Port Definitions */
#if defined(linux)

#define PORTKEY_CPU_
#define PORTKEY_OS_UNIX LINUX
#define PORTKEY_COMP_UNIX_ANSIC 1
#define PORTKEY_APP_MULTIPROC

#if defined (__GNUC__)
#define PORTKEY_INT64_NATIVE 1
#define PORTKEY_INT64_TYPE long long
#endif

#endif

#endif /* portkey_h*/

/****************************** END OF FILE *****************************/

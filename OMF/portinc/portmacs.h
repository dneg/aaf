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
   FILE portmacs.h  -- Porting Macros
**************************************************************************

   This file defines porting macros that are based on port variables.

   CONTRIBUTORS:
		MJV - Michael Vigneau
		CvD - Casper van Dijk, ELECTROGIG Technology BV

   REVISION HISTORY:
		25-Nov-96  CvD  Avoid PORT_MAXALLOC check on Linux
   		25-Mar-93  MJV  Added OVER64K macro
		26-Jan-93  MJV  Added CHAR4 macro, "unused" placeholders
		21-Dec-92  MJV  File Created (from masterhd)

*************************************************************************/
#ifndef portmacs_h
#define portmacs_h


/* ThinkC does not support pure virtual or inline functions and so...*/

#if PORT_LANG_CPLUSMINUS
#define PURE_VIRTUAL
#define INLINE 
#else
#define PURE_VIRTUAL = 0
#define INLINE inline
#endif


/* Put 4 chars together into a long (that works as a char string)*/

#if PORT_BYTESEX_LITTLE_ENDIAN

#define CHAR4(ch0, ch1, ch2, ch3) (long) \
   ( (u_long)(u_char)(ch0)        | ((u_long)(u_char)(ch1) << 8 ) |  \
    ((u_long)(u_char)(ch2) << 16) | ((u_long)(u_char)(ch3) << 24) )

#else  /* big endian version*/

#define CHAR4(ch0, ch1, ch2, ch3) (long) \
   ( (u_long)(u_char)(ch3)        | ((u_long)(u_char)(ch2) << 8 ) |  \
    ((u_long)(u_char)(ch1) << 16) | ((u_long)(u_char)(ch0) << 24) )

#endif /* PORT_BYTESEX_LITTLE_ENDIAN*/



/* Special keyword for pointers (and arrays) to access more than 64K*/
/* Added extra linux check to circumvent foul '==' check, 25-11-96, CvD */

#if defined(linux)
#define OVER64K
#else
#if PORT_MAXALLOC == 65536
#define OVER64K _huge
#else
#define OVER64K
#endif
#endif

/* ceil implemented differently in different environments */
/* #if defined(PORTKEY_COMP_MS_CPP) || defined(THINK_C) */
#define CEIL ceil
/*#else
#define CEIL ceilf
#endif
*/

#endif /* portmacs_h*/
/****************************** END OF FILE *****************************/

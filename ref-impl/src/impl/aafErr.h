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

#ifndef OMError_h_
#define OMError_h_ 1

#ifdef AAF_ERROR_TRACE
#include <stdio.h>
#endif

#include "AAFTypes.h"
#include "AAFResult.h"

class AAFFile;

typedef AAFRESULT aafErr_t;

/************************************************************
 *
 * Externally visible routines
 *
 *************************************************************/
char    *aafGetErrorString(aafErr_t code);
char		*aafGetExpandedErrorString(AAFFile * file,
											aafErr_t code,
											aafInt16 buflen,
											char *buffer);
void     aafErrorInit(void);
void     aafRegErr(aafErr_t msgcode);
void     aafReregErrDebug(aafErr_t ec, 
								 char *filename, 
								 aafInt16 line);
void     aafRegErrDebug(aafErr_t msgcode, 
							   char *filename, 
							   aafInt16 line);


/************************************************************
 *
 * Assertions.  Those assertions defined using XASSERT or aafAssert
 * don't need to be keyed off of AAF_NO_ASSERTS, as the base macros will
 * evaluate to a NULL string if AAF_NO_ASSERTS is set.
 *
 *************************************************************/

#ifdef AAF_NO_ASSERTS
#define aafAssertValidFHdl()
#define aafAssertValidFHdlBool(errparam, ret)
#define aafAssert(b, msgcode)
#define aafAssertBool(b, msgcode, msgparam, retval)

#define aafAssertIterMore(ihdl)
#define aafAssertSTrackHdl(thdl)
#define aafAssertIterHdl(ihdl)
#define aafAssertMediaHdl(mhdl)

#define aafAssertIsAAF()
#define aafAssertMediaInitComplete()

#else /* AAF_NO_ASSERTS */    				
#define aafAssertIterMore(ihdl) \
	if((ihdl == NULL) || (ihdl->_iterType == kIterDone)) \
          return(AAFRESULT_NO_MORE_OBJECTS)
			  
#define aafAssertSTrackHdl(thdl) \
	if((thdl == NULL) || (thdl->_cookie != SIMPLETRAK_COOKIE)) \
          return(AAFRESULT_BAD_STRACKHDL)
          
	
#define aafAssertIterHdl(ihdl) \
	if((ihdl == NULL) || (ihdl->_cookie != ITER_COOKIE)) \
           return(AAFRESULT_BAD_ITHDL)
          
#define aafAssert(b, file, msgcode) \
	if (!(b)) { aafRegErrorReturn(msgcode); }

#define aafAssertBool(b, file, msgcode, msgparam, retval) \
    if (!(b)) { aafRegErrReturnBool(msgcode, msgparam, retval); }

#define aafAssertMediaHdl(mhdl) \
	aafAssert((mhdl != NULL) && (mhdl->_cookie == MEDIA_COOKIE), \
			  AAFRESULT_BAD_MDHDL)
#define aafAssertIsAAF(file) \
	aafAssert((file->_fmt == kAAFMedia), file, AAFRESULT_WRONG_FILETYPE)
#define aafAssertMediaInitComplete(file) \
	aafAssert(file->_session->_mediaLayerInitComplete, file, AAFRESULT_MEDIA_NOT_INIT);

#endif /* AAF_NO_ASSERTS */

	/************************************************************
	 *
	 * Old-style error macros.  These should be phased out
	 * (except aafPrintStackTrace should be kept)
	 *
	 *************************************************************/
	 
#ifdef AAF_ERROR_TRACE

AAF_EXPORT void aafPrintStackTrace(AAFFile * file);

#define aafRegErrorReturn(file, ec) { \
   aafRegErrDebug(file, ec, __FILE__, __LINE__); return(ec); }

#define aafRegErrReturnBool(ec, errparam, retval) { \
        aafRegErr(ec); *errparam = ec; return(retval); }

#else /* AAF_ERROR_TRACE */

#define aafRegErrorReturn(ec) { aafRegErr(ec); return(ec); }
#define aafRegErrReturnBool(ec, errparam, retval) { \
        aafRegErr(ec); *errparam = ec; return(retval); }

#endif /* AAF_ERROR_TRACE */

	/************************************************************
	 *
	 * New exception-style error handling
	 *
	 *************************************************************/
#define XPROTECT()	{ aafErr_t zzOmfEcode = AAFRESULT_SUCCESS; \
						aafBool zzOmfPropagate = AAFTrue;

#define CHECK(a) { if((zzOmfEcode = (a)) != AAFRESULT_SUCCESS) goto zzOmfCleanup; }

#if defined(AAF_ERROR_TRACE)
#define REG_ERR(ecode)	   aafRegErrDebug(ecode, __FILE__, __LINE__)
#define REREG_ERR(ecode)  aafReregErrDebug(ecode, __FILE__, __LINE__)
#else 
#define REG_ERR(ecode)
#define REREG_ERR(ecode)
#endif

#ifdef AAF_NO_ASSERTS
#define XASSERT(b, msgcode)
#else
#define XASSERT(b, msgcode) \
	if (!(b)) { RAISE(msgcode); }
#endif

#define RAISE(a)		{ zzOmfEcode = (a); \
							REG_ERR(zzOmfEcode); goto zzOmfCleanup;\
						}
						
#define RERAISE(a)		zzOmfEcode = (a)
#define NO_PROPAGATE()	zzOmfPropagate = AAFFalse
#define XCODE()			zzOmfEcode
#define XEXCEPT			goto zzOmfExit; \
						zzOmfCleanup:
#define XEND			if(zzOmfPropagate) { REREG_ERR(zzOmfEcode);\
										   return(zzOmfEcode); } } zzOmfExit:
#define XEND_VOID		if(zzOmfPropagate) { REREG_ERR(zzOmfEcode);\
											   return; } } zzOmfExit:
#define XEND_SPECIAL(a)	if(zzOmfPropagate) { REREG_ERR(zzOmfEcode);\
											   return(a); } } zzOmfExit:
// Prevent warnings of the form ...
//
// warning C4102: 'zzOmfCleanup' : unreferenced label
//
// when XEXCEPT is used without RAISE
//
#ifdef PORTKEY_OS_WIN32
#pragma warning( disable : 4102)
#endif

#endif

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

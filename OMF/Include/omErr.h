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

#ifdef OMFI_ERROR_TRACE
#include <stdio.h>
#endif

#include "omTypes.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

/************************************************************
 *
 * Error codes
 *
 *************************************************************/
typedef enum
{
	OM_ERR_NONE = 0, 

/* COMPATIBILITY ONLY error codes */
	OM_ERR_BADOBJ, OM_ERR_INULLMOBIDPTR, OM_ERR_INULLMTPTR, 
	OM_ERR_INULLDESCPTR, OM_ERR_IBADMEDIAINDEX,
	OM_ERR_LRC_DISABLED, OM_ERR_LRC_BADSAMPLESIZE, OM_ERR_LRC_MONOONLY, 
	OM_ERR_BADSESSION, OM_ERR_INTERNAL_BADOBJ, 

/* SESSION/FILE Error Codes */
	OM_ERR_BAD_SESSION, OM_ERR_BADSESSIONOPEN, OM_ERR_BADSESSIONMETA,
	OM_ERR_BADSESSIONCLOSE, OM_ERR_BADCONTAINER, 
	OM_ERR_FILEREV_NOT_SUPP, OM_ERR_FILEREV_DIFF,
	OM_ERR_BADOPEN, OM_ERR_BADCLOSE, OM_ERR_BAD_FHDL, 
	OM_ERR_BADHEAD, OM_ERR_NOBYTEORDER, OM_ERR_INVALID_BYTEORDER, 
	OM_ERR_NOTOMFIFILE, OM_ERR_WRONG_FILETYPE, OM_ERR_WRONG_OPENMODE, 
	OM_ERR_CONTAINERWRITE, OM_ERR_FILE_NOT_FOUND,
	OM_ERR_CANNOT_SAVE_CLSD, OM_ERR_CANNOT_LOAD_CLSD,
	OM_ERR_FILE_REV_200, OM_ERR_NEED_PRODUCT_IDENT,

/* MEDIA Error Codes */
	OM_ERR_DESCSAMPRATE, OM_ERR_SOURCEMOBLIST, OM_ERR_DESCLENGTH, 
	OM_ERR_INTERNAL_MDO, OM_ERR_3COMPONENT, OM_ERR_INTERNAL_CORRUPTVINFO, 
	OM_ERR_BADSAMPLEOFFSET, OM_ERR_ONESAMPLEREAD, OM_ERR_ONESAMPLEWRITE,
	OM_ERR_DECOMPRESS, OM_ERR_NODATA, OM_ERR_SMALLBUF, 
	OM_ERR_BADCOMPR, OM_ERR_BADPIXFORM, OM_ERR_BADLAYOUT,
	OM_ERR_COMPRLINESWR, OM_ERR_COMPRLINESRD, OM_ERR_BADMEDIATYPE, 
	OM_ERR_BADDATAADDRESS, OM_ERR_BAD_MDHDL, OM_ERR_MEDIA_NOT_FOUND,
	OM_ERR_ILLEGAL_MEMFMT, OM_ERR_ILLEGAL_FILEFMT, OM_ERR_SWABBUFSIZE,
	OM_ERR_MISSING_SWAP_PROC, OM_ERR_NULL_STREAMPROC,
	OM_ERR_NULLBUF, OM_ERR_SWAP_SETUP, OM_ERR_INVALID_FILE_MOB,
	OM_ERR_SINGLE_CHANNEL_OP, OM_ERR_INVALID_CACHE_SIZE, OM_ERR_NOT_FILEMOB,
	OM_ERR_TRANSLATE_SAMPLE_SIZE, OM_ERR_TRANSLATE_NON_INTEGRAL_RATE,
	OM_ERR_MISSING_MEDIA_REP, OM_ERR_NOT_LONGWORD, OM_ERR_XFER_DUPCH,
	OM_ERR_MEDIA_NOT_INIT, OM_ERR_BLOCKING_SIZE, OM_ERR_WRONG_MEDIATYPE, 
	OM_ERR_MULTI_WRITELEN, OM_ERR_STREAM_REOPEN, OM_ERR_TOO_MANY_FMT_OPS, 
	OM_ERR_MEDIASTREAM_NOTALLOWED, OM_ERR_STILLFRAME_BADLENGTH,
	OM_ERR_DATA_NONCONTIG, OM_ERR_OPLIST_OVERFLOW, OM_ERR_STREAM_CLOSED, 
	OM_ERR_USE_MULTI_CREATE, OM_ERR_MEDIA_OPENMODE, OM_ERR_MEDIA_CANNOT_CLOSE, 
	OM_ERR_CODEC_INVALID, OM_ERR_INVALID_OP_CODEC, OM_ERR_BAD_CODEC_REV, 
	OM_ERR_CODEC_CHANNELS, OM_ERR_INTERN_TOO_SMALL, 
	OM_ERR_INTERNAL_UNKNOWN_LOC, OM_ERR_TRANSLATE, OM_ERR_EOF,
	OM_ERR_TIFFVERSION, OM_ERR_BADTIFFCOUNT, OM_ERR_24BITVIDEO,
	OM_ERR_JPEGBASELINE, OM_ERR_BADJPEGINFO, OM_ERR_BADQTABLE, 
	OM_ERR_BADACTABLE, OM_ERR_BADDCTABLE, OM_ERR_NOFRAMEINDEX, 
	OM_ERR_BADFRAMEOFFSET, OM_ERR_JPEGPCM, OM_ERR_JPEGDISABLED,
	OM_ERR_JPEGPROBLEM,	OM_ERR_BADEXPORTPIXFORM, OM_ERR_BADEXPORTLAYOUT,
	OM_ERR_BADRWLINES, OM_ERR_BADAIFCDATA, OM_ERR_BADWAVEDATA, 
	OM_ERR_NOAUDIOCONV, OM_ERR_XFER_NOT_BYTES, OM_ERR_CODEC_NAME_SIZE,
	OM_ERR_ZERO_SAMPLESIZE, OM_ERR_ZERO_PIXELSIZE, OM_ERR_BAD_VARIETY,

/* OBJECT Error Codes */
	OM_ERR_NULLOBJECT, OM_ERR_BADINDEX,	OM_ERR_INVALID_LINKAGE,
	OM_ERR_BAD_PROP, OM_ERR_BAD_TYPE, OM_ERR_SWAB, OM_ERR_END_OF_DATA, 
	OM_ERR_PROP_NOT_PRESENT, OM_ERR_INVALID_DATAKIND, OM_ERR_DATAKIND_EXIST,
	OM_ERR_TOO_MANY_TYPES,
	
/* MOB Error Codes */
	OM_ERR_NOT_SOURCE_CLIP, OM_ERR_FILL_FOUND, OM_ERR_BAD_LENGTH, 
	OM_ERR_BADRATE, OM_ERR_INVALID_ROUNDING, OM_ERR_TIMECODE_NOT_FOUND,	
	OM_ERR_NO_TIMECODE,	OM_ERR_INVALID_TIMECODE, 
	OM_ERR_TRACK_NOT_FOUND, OM_ERR_BAD_SLOTLENGTH, OM_ERR_MISSING_TRACKID, 
	OM_ERR_TRACK_EXISTS, OM_ERR_MOB_NOT_FOUND, OM_ERR_NO_MORE_MOBS,
	OM_ERR_DUPLICATE_MOBID, OM_ERR_MISSING_MOBID, OM_ERR_EFFECTDEF_EXIST, 
	OM_ERR_INVALID_EFFECTDEF, OM_ERR_INVALID_EFFECT, OM_ERR_INVALID_EFFECTARG, 
	OM_ERR_INVALID_CVAL, OM_ERR_RENDER_NOT_FOUND,
	OM_ERR_BAD_ITHDL, OM_ERR_NO_MORE_OBJECTS,
	OM_ERR_ITER_WRONG_TYPE, OM_ERR_INVALID_SEARCH_CRIT,	
	OM_ERR_INTERNAL_ITERATOR, OM_ERR_NULL_MATCHFUNC, OM_ERR_NULL_CALLBACKFUNC,
	OM_ERR_TRAVERSAL_NOT_POSS, OM_ERR_INVALID_TRAN_EFFECT, 
	OM_ERR_ADJACENT_TRAN, OM_ERR_LEADING_TRAN, OM_ERR_INSUFF_TRAN_MATERIAL, 
	OM_ERR_PULLDOWN_DIRECTION, OM_ERR_PULLDOWN_FUNC, OM_ERR_PULLDOWN_KIND,
	OM_ERR_BAD_SRCH_ITER, OM_ERR_NOT_COMPOSITION, OM_ERR_NOT_A_TRACK,
	OM_ERR_PARSE_EFFECT_AMBIGUOUS,

/* SIMPLE COMPOSITION Error Codes */
	OM_ERR_BAD_STRACKHDL, OM_ERR_STRACK_APPEND_ILLEGAL,

/* GENERIC Error Codes ***/
	OM_ERR_NOMEMORY, OM_ERR_OFFSET_SIZE, OM_ERR_INTERNAL_NEG64,
	OM_ERR_OVERFLOW64, OM_ERR_NOT_IN_15, OM_ERR_NOT_IN_20, 
	OM_ERR_NOT_IMPLEMENTED,	OM_ERR_NULL_PARAM, OM_ERR_ZERO_DIVIDE,
	
/* SEMANTIC CHECKING Error Codes */
	OM_ERR_REQUIRED_POSITIVE, OM_ERR_INVALID_TRACKKIND,	
	OM_ERR_INVALID_EDGETYPE, OM_ERR_INVALID_FILMTYPE, OM_ERR_INVALID_MOBTYPE, 
	OM_ERR_INVALID_TAPECASETYPE, OM_ERR_INVALID_VIDEOSIGNALTYPE, 
	OM_ERR_INVALID_TAPEFORMATTYPE, OM_ERR_INVALID_EDITHINT, 
	OM_ERR_INVALID_INTERPKIND,
	OM_ERR_INVALID_TRACK_REF, OM_ERR_INVALID_OBJ, OM_ERR_BAD_VIRTUAL_CREATE, 
	OM_ERR_INVALID_CLASS_ID, OM_ERR_OBJECT_SEMANTIC,
	OM_ERR_DATA_IN_SEMANTIC, OM_ERR_DATA_OUT_SEMANTIC, OM_ERR_TYPE_SEMANTIC, 
	OM_ERR_INVALID_ATTRIBUTEKIND, OM_ERR_DATA_MDES_DISAGREE,
	OM_ERR_CODEC_SEMANTIC_WARN, OM_ERR_INVALID_BOOLTYPE,

/* INTERNAL Error Codes */
	OM_ERR_TABLE_DUP_KEY, OM_ERR_TABLE_MISSING_COMPARE,
	OM_ERR_TABLE_BAD_HDL, OM_ERR_TABLE_BAD_ITER,
	OM_ERR_BENTO_PROBLEM, OM_ERR_BENTO_HANDLER,
	OM_ERR_PROPID_MATCH, OM_ERR_INTERNAL_DIVIDE,

/* Testing Error Codes */
	OM_ERR_TEST_FAILED,
	
	OM_ERR_MAXCODE
}               omfErr_t;

/************************************************************
 *
 * Externally visible routines
 *
 *************************************************************/
OMF_EXPORT char    *omfsGetErrorString(omfErr_t code);
OMF_EXPORT char		*omfsGetExpandedErrorString(omfHdl_t file,
											omfErr_t code,
											omfInt16 buflen,
											char *buffer);
OMF_EXPORT void     omfsErrorInit(void);
OMF_EXPORT void     omfRegErr(omfHdl_t file, omfErr_t msgcode);
OMF_EXPORT void     omfReregErrDebug(omfHdl_t file, 
								 omfErr_t ec, 
								 char *filename, 
								 omfInt16 line);
OMF_EXPORT void     omfRegErrDebug(omfHdl_t file, 
							   omfErr_t msgcode, 
							   char *filename, 
							   omfInt16 line);


/************************************************************
 *
 * Assertions.  Those assertions defined using XASSERT or omfAssert
 * don't need to be keyed off of OMFI_NO_ASSERTS, as the base macros will
 * evaluate to a NULL string if OMFI_NO_ASSERTS is set.
 *
 *************************************************************/

#ifdef OMFI_NO_ASSERTS
#define omfAssertValidFHdl(file)
#define omfAssertValidFHdlBool(file, errparam, ret)
#define omfAssert(b, file, msgcode)
#define omfAssertBool(b, file, msgcode, msgparam, retval)

#define omfAssertIterMore(ihdl)
#define omfAssertSTrackHdl(thdl)
#define omfAssertIterHdl(ihdl)
#define omfAssertMediaHdl(mhdl)

#define omfAssertNot1x(file)
#define omfAssertNot1xBool(file, errparam, ret)
#define omfAssertIsOMFI(file)
#define omfAssertMediaInitComplete(file)

#else /* OMFI_NO_ASSERTS */
#define omfAssertValidFHdlBool(file, errparam, ret) \
	if (!((file != NULL) && (file->cookie == FILE_COOKIE))) \
           {omfRegErrReturnBool(file, OM_ERR_BAD_FHDL, errparam, ret);}
           
    				
#define omfAssertIterMore(ihdl) \
	if((ihdl == NULL) || (ihdl->iterType == kIterDone)) \
          return(OM_ERR_NO_MORE_OBJECTS)
			  
#define omfAssertSTrackHdl(thdl) \
	if((thdl == NULL) || (thdl->cookie != SIMPLETRAK_COOKIE)) \
          return(OM_ERR_BAD_STRACKHDL)
          
	
#define omfAssertIterHdl(ihdl) \
	if((ihdl == NULL) || (ihdl->cookie != ITER_COOKIE)) \
           return(OM_ERR_BAD_ITHDL)
          
#define omfAssert(b, file, msgcode) \
	if (!(b)) { omfRegErrorReturn(file, (omfErr_t)msgcode); }

#define omfAssertBool(b, file, msgcode, msgparam, retval) \
    if (!(b)) { omfRegErrReturnBool(file, (omfErr_t)msgcode, msgparam, retval); }

#define omfAssertNot1x(file) \
    omfAssert((file->setrev != kOmfRev1x) && (file->setrev != kOmfRevIMA), \
			  file, OM_ERR_NOT_IN_15)
#define omfAssertNot1xBool(file, errparam, retval) \
    omfAssertBool((file->setrev != kOmfRev1x) && (file->setrev != kOmfRevIMA),\
			  file, OM_ERR_NOT_IN_15, errparam, retval)

#define omfAssertMediaHdl(mhdl) \
	omfAssert((mhdl != NULL) && (mhdl->cookie == MEDIA_COOKIE), \
			  media->mainFile, OM_ERR_BAD_MDHDL)
#define omfAssertValidFHdl(file) \
	omfAssert((file != NULL) && (file->session != NULL) && \
			  (file->cookie == FILE_COOKIE), file, OM_ERR_BAD_FHDL);
#define omfAssertIsOMFI(file) \
	omfAssert((file->fmt == kOmfiMedia), file, OM_ERR_WRONG_FILETYPE)
#define omfAssertMediaInitComplete(file) \
	omfAssert(file->session->mediaLayerInitComplete, file, OM_ERR_MEDIA_NOT_INIT);

#endif /* OMFI_NO_ASSERTS */

	/************************************************************
	 *
	 * Old-style error macros.  These should be phased out
	 * (except omfPrintStackTrace should be kept)
	 *
	 *************************************************************/
	 
#ifdef OMFI_ERROR_TRACE

OMF_EXPORT void omfPrintStackTrace(omfHdl_t file);

#define omfRegErrorReturn(file, ec) { \
   omfRegErrDebug(file, (omfErr_t)ec, __FILE__, (omfInt16)__LINE__); return(ec); }

#define omfCheckBentoRaiseError(file, ec) { if (file->BentoErrorRaised) { \
    printf("<Bento %1ld>%s @ %d in '%s'\n", file->BentoErrorNumber, \
		   file->BentoErrorString, __LINE__, __FILE__);  \
			RAISE(ec); \
    } }
#define omfRegErrReturnBool(file, ec, errparam, retval) { \
        omfRegErr(file, (omfErr_t)ec); *errparam = ec; return(retval); }

#else /* OMFI_ERROR_TRACE */

#define omfRegErrorReturn(file, ec) { omfRegErr(file, (omfErr_t)ec); return(ec); }
#define omfRegErrReturnBool(file, ec, errparam, retval) { \
        omfRegErr(file, (omfErr_t)ec); *errparam = ec; return(retval); }
#define omfCheckBentoRaiseError(file, ec) { if(file->BentoErrorRaised) \
											  RAISE(ec); }

#endif /* OMFI_ERROR_TRACE */

	/************************************************************
	 *
	 * New exception-style error handling
	 *
	 *************************************************************/
#define XPROTECT(a)	{ omfErr_t zzOmfEcode = OM_ERR_NONE; \
						omfHdl_t zzOmfFile = a; \
						omfBool zzOmfPropagate = TRUE;

#define CHECK(a) { if((zzOmfEcode = (a)) != OM_ERR_NONE) goto zzOmfCleanup; }

#if defined(OMFI_ERROR_TRACE)
#define REG_ERR(file,ecode)	   omfRegErrDebug(file,(omfErr_t)ecode, __FILE__, (omfInt16)__LINE__)
#define REREG_ERR(file,ecode)  omfReregErrDebug(file,(omfErr_t)ecode, __FILE__, (omfInt16)__LINE__)
#else 
#define REG_ERR(file,ecode)
#define REREG_ERR(file,ecode)
#endif

#ifdef OMFI_NO_ASSERTS
#define XASSERT(b, msgcode)
#else
#define XASSERT(b, msgcode) \
	if (!(b)) { RAISE(msgcode); }
#endif

#define RAISE(a)		{ zzOmfEcode = (a); \
							REG_ERR(zzOmfFile, zzOmfEcode); goto zzOmfCleanup;\
						}
						
#define RERAISE(a)		zzOmfEcode = (a)
#define NO_PROPAGATE()	zzOmfPropagate = FALSE
#define XCODE()			zzOmfEcode
#define XEXCEPT			goto zzOmfExit; \
						zzOmfCleanup:
#define XEND			if(zzOmfPropagate) { REREG_ERR(zzOmfFile, zzOmfEcode);\
										   return(zzOmfEcode); } } zzOmfExit:
#define XEND_VOID		if(zzOmfPropagate) { REREG_ERR(zzOmfFile, zzOmfEcode);\
											   return; } } zzOmfExit:
#define XEND_SPECIAL(a)	if(zzOmfPropagate) { REREG_ERR(zzOmfFile, zzOmfEcode);\
											   return(a); } } zzOmfExit:

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

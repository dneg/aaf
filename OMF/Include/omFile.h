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

#ifndef _OMF_FILE_API_
#define _OMF_FILE_API_ 1

#include "omErr.h"
#include "omFile.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

/************************************************************************
 *
 * Session and File Management
 *
 ************************************************************************/

typedef enum
{
  kOmfRev1x, kOmfRevIMA, kOmfRev2x
}               omfFileRev_t;

typedef void   *fileHandleType;

/*
 * The call to set up all read-only data.  This routine is not
 * thread-safe and MUST therefore not be called before threads.
 */
OMF_EXPORT omfErr_t omfsBeginSession(
			omfProductIdentification_t	*ident, 		/* IN - Open a session */
			omfSessionHdl_t * result);
OMF_EXPORT omfErr_t omfsEndSession(omfSessionHdl_t session);

OMF_EXPORT omfErr_t omfsFileGetRev(omfHdl_t file, 
								omfFileRev_t *fileRev);

OMF_EXPORT omfErr_t omfsCreateFile(fileHandleType stream, 
								omfSessionHdl_t session, 
								omfFileRev_t rev, 
								omfHdl_t * result);
OMF_EXPORT omfErr_t omfsOpenFile(fileHandleType stream, 
							 omfSessionHdl_t session, 
							 omfHdl_t * result);
OMF_EXPORT omfErr_t omfsModifyFile(fileHandleType stream, 
								omfSessionHdl_t session, 
								omfHdl_t * result);
OMF_EXPORT omfErr_t omfsCloseFile(omfHdl_t file);
OMF_EXPORT omfErr_t omfsGetErrorCode(omfHdl_t file);

OMF_EXPORT omfErr_t omfsOpenRawFile(fileHandleType	fh, 
								 omfInt16			fhSize,
								 omfSessionHdl_t session, 
								 omfHdl_t * result);
OMF_EXPORT omfErr_t omfsAppendRawFile(fileHandleType stream, 
								omfSessionHdl_t session, 
								omfFileRev_t rev, 
								omfHdl_t * result);

OMF_EXPORT omfErr_t omfsCreateRawFile(fileHandleType fh, 
									omfInt16 fhSize,
									omfSessionHdl_t session, 
									omfCodecID_t		codecID,
									omfHdl_t * result);

OMF_EXPORT omfErr_t omfsPatch1xMobs(omfHdl_t file);

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _OMF_FILE_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/

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

#ifndef _OMF_CODEC_ID_
#define _OMF_CODEC_ID_ 1
#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif				/* Registered codecs */
#define	CODEC_TIFF_VIDEO	"TIFF"
#define	CODEC_RGBA_VIDEO	"RGBA"
#define	CODEC_CDCI_VIDEO	"CDCI"
#define	CODEC_JPEG_VIDEO	"JPEG"
#define	CODEC_AVID_VIDEO_HW	"AVHW"	/* Hardware accelerated AVID video
					 * type */
#define	CODEC_AIFC_AUDIO	"AIFC"
#define	CODEC_WAVE_AUDIO	"WAVE"
#define	CODEC_SD2_AUDIO		"SDII"
#define	CODEC_LRC_AUDIO		"LRC "
#define CODEC_QUICKTIME		"QUICKTIME"

/*#define	CODEC_TIFF_INDEX	0
#define	CODEC_AIFC_INDEX	1
#define	CODEC_WAVE_INDEX	2
*/
#define	CODEC_RGBA_INDEX	3

#if (PORT_BYTESEX_BIG_ENDIAN)
#define	CODEC_NATIVE_AUDIO	CODEC_AIFC_AUDIO
#else
#define	CODEC_NATIVE_AUDIO	CODEC_WAVE_AUDIO
#endif

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _OMF_CODEC_ID_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

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

#if FULL_TOOLKIT
#include "masterhd.h"
#endif
#include <string.h>

#include "aafErr.h"
#include "AAFFile.h"
#include "AAFSession.h"
//#include "omPvt.h"

static char    *localErrorStrings[300];
/* A 75-column ruler
         111111111122222222223333333333444444444455555555556666666666777777
123456789012345678901234567890123456789012345678901234567890123456789012345
*/

/************************
 * Function: aafGetErrorString
 *
 * 		Given an error code, returns an error string.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Always returns an error string of some sort, although
 *		it may just be "unknown error code".
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
char *aafGetErrorString(
		aafErr_t code)
{
	if (code < OM_ERR_MAXCODE && code >= 0 && localErrorStrings[0] != '\0')
		return (localErrorStrings[code]);
	else
		return ("AAF_ERR: Unknown error code");
}

/************************
 * Function: aafGetExpandedErrorString
 *
 * 		Given an error code, returns an error string.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Always returns an error string of some sort, although
 *		it may just be "unknown error code".
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
char *aafGetExpandedErrorString(AAFFile * file,
											aafErr_t code,
											aafInt16 buflen,
											char *buffer)
{
#if FULL_TOOLKIT
	char	*basic = aafGetErrorString(code);
	
	strncpy(buffer, basic, buflen);
	if((strlen(basic) < (aafUInt16)buflen) && (file != NULL))
	{
		if(code == OM_ERR_BENTO_PROBLEM ||
		   code == OM_ERR_BADOPEN ||
		   code == OM_ERR_NOTAAFFILE ||
		   code == OM_ERR_FILE_NOT_FOUND)
		{
			if(file->ContainerErrorRaised())
			{
				strncat(buffer, "\n[", buflen - strlen(buffer));
				strncat(buffer, file->GetContainerErrString(), buflen - strlen(buffer));
				strncat(buffer, "]", buflen - strlen(buffer));
			}
			else if(file->_session->GetBentoError() != 0)
			{
				strncat(buffer, "\n[", buflen - strlen(buffer));
				strncat(buffer, file->_session->GetContainerErrString(), buflen - strlen(buffer));
				strncat(buffer, "]", buflen - strlen(buffer));
			}
		}
	}

	return(buffer);
#else
	return(NULL);
#endif
}

/************************
 * Function: aafRegErr			(CALLED BY MACROS)
 *
 * 	Internal routine to log an error.  Should not used anymore.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
void aafRegErr(
			AAFFile *		file,
			aafErr_t		msgcode)
{
}

#ifdef AAF_ERROR_TRACE
/************************
 * Function: aafRegErrDebug		(CALLED BY MACROS)
 *
 * 	Debug version of omfRefErr.  This routine is used ONLY if AAF_ERROR_TRACE
 *		is enabled, and prints information about the error raised to a stack trace
 *		buffer, which may be printed.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		<none>.
 *
 * Possible Errors:
 *		<none>.
 */
void aafRegErrDebug(
			AAFFile *		file,
			aafErr_t		ec,
			char			*filename,
			aafInt16		line)
{
	if (file->isValid())
	{
		if(file->_stackTrace != NULL)
			sprintf(file->_stackTrace, "Error Raised <%1d>%s @ %d in '%s'\n", ec, aafGetErrorString(ec), line, filename);
	}
}

/************************
 * Function: aafReregErrDebug	(CALLED BY MACROS)
 *
 * 	Called when an error is reraised (as opposed to being
 *		initially raised).  This is to allow errors being propagated
 *		up the stack looking different in the stack trace from
 *		Errors initially raised.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		<none>.
 */
void aafReregErrDebug(
			AAFFile * file,
			aafErr_t msgcode,
			char *filename,
			aafInt16 line)
{
	char	buf[256];
	aafInt32	left;
		
	if (file->isValid())
	{
		sprintf(buf, "   Error <%1d>%s propagated  @ %d in '%s'\n", msgcode,
					aafGetErrorString(msgcode), line, filename);
		left = file->_stackTraceSize - (strlen(file->_stackTrace) + 1);
		strncat(file->_stackTrace, buf, left);
	}
}


/************************
 * Function: aafPrintStackTrace
 *
 * 	Put this routine in the top-level exception block of the test
 *		application to print out the trace buffer when an error occurs.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		<none>.
 *
 * Possible Errors:
 *		<none>.
 */
void aafPrintStackTrace(
			AAFFile * file)
{
   if (file != NULL)
	  {
		 printf("***Stack trace from last error****\n");
		 printf("%s\n", file->_stackTrace);
	  }
}
#endif

/************************
 * Function: aafErrorInit	(INTERNAL)
 *
 * 	Called on session open to load up the error message table.
 *		A routine is used asopposed to static data, because Mac code
 *		resources can't statically initialize pointers.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		<none>.
 *
 * Possible Errors:
 *		<none>.
 */
void aafErrorInit(void)
{
	localErrorStrings[OM_ERR_NONE] =
		"AAF: Success";

/*** 1.0 COMPATIBILITY ONLY Error Codes ***/
	localErrorStrings[OM_ERR_BADOBJ] =
		"AAF_ERR: Null AAF/Bento object";

	localErrorStrings[OM_ERR_INULLMOBIDPTR] =
		"AAF_ERR(Import): NULL Mob ID pointer in GetNthMediaDesc/SetMediaDesc";

	localErrorStrings[OM_ERR_INULLMTPTR] =
		"AAF_ERR(Import): NULL omfMediaType_t pointer in GetNthMediaDesc";

	localErrorStrings[OM_ERR_INULLDESCPTR] =
		"AAF_ERR(Import): NULL descriptor pointer in GetNthMediaDesc";

	localErrorStrings[OM_ERR_IBADMEDIAINDEX] =
		"AAF_ERR(Import): Input AAF file does not have enough media objects";

	localErrorStrings[OM_ERR_LRC_DISABLED] =
		"AAF_ERR: LRC codec disabled";

	localErrorStrings[OM_ERR_LRC_BADSAMPLESIZE] =
		"AAF_ERR: Only 16 bit samples supported for LRC";

	localErrorStrings[OM_ERR_LRC_MONOONLY] =
	   "AAF_ERR: Only one channel audio supported for LRC";

	localErrorStrings[OM_ERR_BADSESSION] =
		"AAF_INTERNAL_ERR: Bad Bento Session operation";

	localErrorStrings[OM_ERR_INTERNAL_BADOBJ] =
		"AAF_INTERNAL_ERR: Error creating AAF/Bento object";

	localErrorStrings[OM_ERR_BADSAMPLEOFFSET] =
		"AAF_ERR: Sample Index offset out of range";


/*** SESSION/FILE Error Codes ***/
	/* Session Error Codes */
	localErrorStrings[OM_ERR_BAD_SESSION] =
		"AAF_ERR: Bad Session Handle";

	localErrorStrings[OM_ERR_BADSESSIONOPEN] =
		"AAF_ERR: Container Session open failed";

	localErrorStrings[OM_ERR_BADSESSIONMETA] =
		"AAF_ERR: Container Session meta-handler operation failed";

	localErrorStrings[OM_ERR_BADSESSIONCLOSE] =
		"AAF_ERR: Container Session close failed";

	localErrorStrings[OM_ERR_BADCONTAINER] =
		"AAF_ERR: Cannot find Bento container";

	/* File Error Codes */
	localErrorStrings[OM_ERR_FILEREV_NOT_SUPP] =
		"AAF_ERR: AAF File Revision > 2.0 not supported";

	localErrorStrings[OM_ERR_FILEREV_DIFF] = 
		"AAF_ERR: Files must have same file revisions";

	localErrorStrings[OM_ERR_BADOPEN] =
		"AAF_ERR: Cannot open file";

	localErrorStrings[OM_ERR_BADCLOSE] =
		"AAF_ERR: Cannot close file";

	localErrorStrings[OM_ERR_BAD_FHDL] =
		"AAF_ERR: Bad File Handle";

	localErrorStrings[OM_ERR_BADHEAD] =
		"AAF_ERR: Invalid AAF HEAD object";

	localErrorStrings[OM_ERR_NOBYTEORDER] =
		"AAF_ERR: No File Byte Order Specified";

	localErrorStrings[OM_ERR_INVALID_BYTEORDER] =
		"AAF_ERR: Invalid byte order";

	localErrorStrings[OM_ERR_NOTAAFFILE] =
		"AAF_ERR: Invalid AAF file";

	localErrorStrings[OM_ERR_WRONG_FILETYPE] =
		"AAF_ERR: Invalid file type (Raw vs. AAF)";

	localErrorStrings[OM_ERR_WRONG_OPENMODE] =
		"AAF_ERR: File opened in wrong mode (readonly vs. readwrite)";

	localErrorStrings[OM_ERR_CONTAINERWRITE] = 
		"AAF_ERR: Error writing to container media (Possibly media is full).";
		
	localErrorStrings[OM_ERR_FILE_NOT_FOUND] = 
		"AAF_ERR: File not found.";
		
	/* Class Dictionary Error Codes */
	localErrorStrings[OM_ERR_CANNOT_SAVE_CLSD] = 
		"AAF_ERR: Error updating the class dictionary in the AAF file";

	localErrorStrings[OM_ERR_CANNOT_LOAD_CLSD] = 
		"AAF_ERR: Error loading the class dictionary from the file";
		
	localErrorStrings[OM_ERR_FILE_REV_200] = 
		"AAF_ERR: Please use toolkit revision 2.0.1 or later to write 2.x compliant files";

	localErrorStrings[OM_ERR_NEED_PRODUCT_IDENT] = 
		"AAF_ERR: You must supply a product identification struct when creating or updating files";


/*** MEDIA Error Codes ***/
	/* General Media Error Codes */
	localErrorStrings[OM_ERR_DESCSAMPRATE] =
		"AAF_ERR: Error getting sample rate from file descriptor";

	localErrorStrings[OM_ERR_SOURCEMOBLIST] =
		"AAF_ERR: Error processing source mob list";

	localErrorStrings[OM_ERR_DESCLENGTH] =
		"AAF_ERR: Error getting length from file descriptor";

	localErrorStrings[OM_ERR_INTERNAL_MDO] =
		"AAF_ERR: Internal Media Data system not initialized";

	localErrorStrings[OM_ERR_3COMPONENT] =
		"AAF_ERR: Only 3-component video allowed in AAF files";

	localErrorStrings[OM_ERR_INTERNAL_CORRUPTVINFO] =
		"AAF_INTERNAL_ERR: Corrupt Video Info Structure";

	localErrorStrings[OM_ERR_BADSAMPLEOFFSET] =
		"AAF_ERR: Sample Index offset out of range";

	localErrorStrings[OM_ERR_ONESAMPLEREAD] =
		"AAF_ERR: Only one sample allowed for reading compressed data";

	localErrorStrings[OM_ERR_ONESAMPLEWRITE] =
		"AAF_ERR: Only one sample allowed for writing compressed data";

	localErrorStrings[OM_ERR_DECOMPRESS] =
		"AAF_ERR: Software Decompression Failed";

	localErrorStrings[OM_ERR_NODATA] =
		"AAF_ERR: No data read";

	localErrorStrings[OM_ERR_SMALLBUF] =
		"AAF_ERR: Sample too large to fit in given buffer";

	localErrorStrings[OM_ERR_INTERN_TOO_SMALL] =
		"AAF_ERR: Buffer is not large enough to hold data";

	localErrorStrings[OM_ERR_INTERNAL_UNKNOWN_LOC] =
		"AAF_ERR: Unknown locator type";

	localErrorStrings[OM_ERR_TRANSLATE] =
		"AAF_ERR: Can't translate to expected memory format";

	localErrorStrings[OM_ERR_EOF] =
		"AAF_ERR: End of file";

	localErrorStrings[OM_ERR_BADCOMPR] =
		"AAF_ERR: Unrecognized compression type in AAF file";

	localErrorStrings[OM_ERR_BADPIXFORM] =
		"AAF_ERR: Unrecognized pixel format in AAF file";

	localErrorStrings[OM_ERR_BADLAYOUT] =
		"AAF_ERR: Unrecognized frame layout in AAF file";

	localErrorStrings[OM_ERR_COMPRLINESWR] =
		"AAF_ERR: omfWriteLines not allowed with compression";

	localErrorStrings[OM_ERR_COMPRLINESRD] =
		"AAF_ERR: omfReadLines not allowed with compression";

	localErrorStrings[OM_ERR_BADMEDIATYPE] =
		"AAF_ERR: Unrecognized Media Type";

	localErrorStrings[OM_ERR_BADDATAADDRESS] =
		"AAF_ERR: Null data address for transfer operation";

	localErrorStrings[OM_ERR_BAD_MDHDL] =
		"AAF_ERR: Bad Media Handle";

	localErrorStrings[OM_ERR_MEDIA_NOT_FOUND] =
		"AAF_ERR: Cannot locate media data";

	localErrorStrings[OM_ERR_ILLEGAL_MEMFMT] =
		"AAF_ERR: Illegal memory format code";

	localErrorStrings[OM_ERR_ILLEGAL_FILEFMT] =
		"AAF_ERR: Illegal file format code";

	localErrorStrings[OM_ERR_SWABBUFSIZE] =
		"AAF_ERR: Invalid swab buffer size";

	localErrorStrings[OM_ERR_MISSING_SWAP_PROC] =
		"AAF_ERR: Missing stream swap proc";

	localErrorStrings[OM_ERR_NULL_STREAMPROC] = 
		"AAF_ERR: A Stream Callback is NULL";

	localErrorStrings[OM_ERR_NULLBUF] =
		"AAF_ERR: Null transfer buffer";

	localErrorStrings[OM_ERR_SWAP_SETUP] =
		"AAF_ERR: Need to set up file & memory format before calling this function";

	localErrorStrings[OM_ERR_INVALID_FILE_MOB] =
		"AAF_ERR: Invalid file mob reference";
		
	localErrorStrings[OM_ERR_SINGLE_CHANNEL_OP] =
		"AAF_ERR: Operation not valid on multi-channel media streams";

	localErrorStrings[OM_ERR_INVALID_CACHE_SIZE] = 
		"AAF_ERR: Stream cache size must be positive or zero";

	localErrorStrings[OM_ERR_NOT_FILEMOB] = 
		"AAF_ERR: Operation requires a file mob";

	localErrorStrings[OM_ERR_TRANSLATE_SAMPLE_SIZE] = 
		"AAF_ERR: Codec can't translate to/from that sample size";
		
	localErrorStrings[OM_ERR_TRANSLATE_NON_INTEGRAL_RATE] =
		"AAF_ERR: Codec can't translate to/from rates which are not multiples";

	localErrorStrings[OM_ERR_MISSING_MEDIA_REP] =
		"AAF_ERR: Media representation not present in the mob";

	localErrorStrings[OM_ERR_NOT_LONGWORD] = 
		"AAF_ERR: Buffer must be longword aligned for this translation";
		
	localErrorStrings[OM_ERR_XFER_DUPCH] = 
		"AAF_ERR: Cannot specify the same channel twice in multi-xfers";

	localErrorStrings[OM_ERR_MEDIA_NOT_INIT] = 
		"AAF_ERR: omfmInit() must be called before making this call";
		
	localErrorStrings[OM_ERR_BLOCKING_SIZE] = 
		"AAF_ERR: Blocking size must be >= 0 bytes";
		
	localErrorStrings[OM_ERR_WRONG_MEDIATYPE] = 
		"AAF_ERR: Incorrect media type for this operation";

	localErrorStrings[OM_ERR_MULTI_WRITELEN] = 
		"AAF_ERR: Writelengths in a writeMultiple must be uniform";

	localErrorStrings[OM_ERR_STREAM_REOPEN] = 
		"AAF_ERROR: Stream reopened without being closed";

	localErrorStrings[OM_ERR_TOO_MANY_FMT_OPS] = 
		"AAF_ERROR: Too many format specifiers for this codec";
		
	localErrorStrings[OM_ERR_MEDIASTREAM_NOTALLOWED] =
		"AAF_ERR: An object with a media stream datakind is not allowed";

	localErrorStrings[OM_ERR_STILLFRAME_BADLENGTH] =
		"AAF_ERR: Length of a still frame in a media group must be 1";

	localErrorStrings[OM_ERR_DATA_NONCONTIG] = 
		"AAF_ERR: Calling this function now will produce non-contiguous media data";
	   
	localErrorStrings[OM_ERR_OPLIST_OVERFLOW] =
	   "AAF_ERR: Operation list overflow";
	   
	localErrorStrings[OM_ERR_STREAM_CLOSED] = 
		"AAF_ERR: Stream must be open for this operation";
		
	localErrorStrings[OM_ERR_USE_MULTI_CREATE] = 
		"AAF_ERR: Use multiCreate to create this many interleaved channels";

	localErrorStrings[OM_ERR_MEDIA_OPENMODE] =
		"AAF_ERR: Media object opened in wrong mode (readonly vs. readwrite)";

	localErrorStrings[OM_ERR_MEDIA_CANNOT_CLOSE] =
		"AAF_ERR: No proc to close media";

	localErrorStrings[OM_ERR_XFER_NOT_BYTES] =
		"AAF_ERR: Sample transfers must be an integral number of bytes";
		
	localErrorStrings[OM_ERR_ZERO_SAMPLESIZE] =
		"AAF_ERR: Sample size of zero not allowed";
		
	localErrorStrings[OM_ERR_ZERO_PIXELSIZE] =
		"AAF_ERR: Pixel size of zero not allowed";
		
	/* Codec Error Codes */
	localErrorStrings[OM_ERR_CODEC_INVALID] =
		"CODEC invalid or not loaded";

	localErrorStrings[OM_ERR_INVALID_OP_CODEC] =
		"AAF_ERR: Operation not valid on this codec";

	localErrorStrings[OM_ERR_BAD_CODEC_REV] =
		"AAF_ERR: Out of date codec";

	localErrorStrings[OM_ERR_CODEC_CHANNELS] =
		"AAF_ERR: Channel out of range for codec";
		
	localErrorStrings[OM_ERR_BAD_VARIETY] = 
		"AAF_ERR: Badly formed codec variety string";

	localErrorStrings[OM_ERR_CODEC_NAME_SIZE] = 
		"AAF_ERR: Codec name string too large";

	/* Image Error Codes */
	localErrorStrings[OM_ERR_TIFFVERSION] =
		"AAF_ERR: Error reading tiff version";

	localErrorStrings[OM_ERR_BADTIFFCOUNT] =
		"AAF_ERR: Video: TIFF count less than 1";

	localErrorStrings[OM_ERR_24BITVIDEO] =
		"AAF_ERR: 24 Bit (8-8-8) video only";

	localErrorStrings[OM_ERR_JPEGBASELINE] =
		"AAF_ERR: Only baseline JPEG allowed in AAF files";

	localErrorStrings[OM_ERR_BADJPEGINFO] =
		"AAF_ERR: JPEG TIFF fields not allowed without COMPRESSION == JPEG";

	localErrorStrings[OM_ERR_BADQTABLE] =
		"AAF_ERR: Bad JPEG Quantization Table";

	localErrorStrings[OM_ERR_BADACTABLE] =
		"AAF_ERR: Bad JPEG AC Huffman Table";

	localErrorStrings[OM_ERR_BADDCTABLE] =
		"AAF_ERR: Bad JPEG DC Huffman Table";

	localErrorStrings[OM_ERR_NOFRAMEINDEX] =
		"AAF_ERR: No JPEG Video Frame Index";

	localErrorStrings[OM_ERR_BADFRAMEOFFSET] =
		"AAF_ERR: Frame Index offset out of range";

	localErrorStrings[OM_ERR_JPEGPCM] =
		"AAF_ERR: OMJPEG.c: put_color_map called";

	localErrorStrings[OM_ERR_JPEGDISABLED] =
		"AAF_ERR: JPEG codec disabled";

	localErrorStrings[OM_ERR_JPEGPROBLEM] =
		"AAF_ERR: Unspecified JPEG codec problem";

	localErrorStrings[OM_ERR_BADEXPORTPIXFORM] =
		"AAF_ERR: Unrecognized Pixel Format";

	localErrorStrings[OM_ERR_BADEXPORTLAYOUT] =
		"AAF_ERR: Unrecognized Frame Layout";

	localErrorStrings[OM_ERR_BADRWLINES] =
		"AAF_ERR: Read/Write Lines only enabled for common video format";

	/* Audio Error Codes */
	localErrorStrings[OM_ERR_BADAIFCDATA] =
		"AAF_ERR: Error reading AIFC data";

	localErrorStrings[OM_ERR_BADWAVEDATA] =
		"AAF_ERR: Error reading WAVE data";

	localErrorStrings[OM_ERR_NOAUDIOCONV] =
		"AAF_ERR: Audio Read Samples: Can't convert to requested sample size";


/*** OBJECT Error Codes ***/
	localErrorStrings[OM_ERR_NULLOBJECT] =
		"AAF_ERR: Null Object not allowed";

	localErrorStrings[OM_ERR_BADINDEX] =
		"AAF_ERR: Array Index Out of Range";

	localErrorStrings[OM_ERR_INVALID_LINKAGE] =
		"AAF_ERR: Invalid object attached to property";

	localErrorStrings[OM_ERR_BAD_PROP] =
		"AAF_ERR: Property code out of range";

	localErrorStrings[OM_ERR_BAD_TYPE] =
		"AAF_ERR: Type code out of range";

	localErrorStrings[OM_ERR_SWAB] =
		"AAF_ERR: Cannot swab that data size";

	localErrorStrings[OM_ERR_END_OF_DATA] =
		"AAF_ERR: Read past end of data";

	localErrorStrings[OM_ERR_PROP_NOT_PRESENT] =
		"AAF_ERR: Property missing from the file";

	localErrorStrings[OM_ERR_INVALID_DATAKIND] =
		"AAF_ERR: Datakind invalid or nonexistant";

	localErrorStrings[OM_ERR_DATAKIND_EXIST] =
		"AAF_ERR: A Datakind Definition with this ID already exists";
		
	localErrorStrings[OM_ERR_TOO_MANY_TYPES] =
		"AAF_ERR: Too many types for a single property";

/*** MOB Error Codes ***/
	/* General Segment Error Codes */
	localErrorStrings[OM_ERR_NOT_SOURCE_CLIP] =
		"AAF_ERR: This property must be a Source Clip";

	localErrorStrings[OM_ERR_FILL_FOUND] =
		"AAF_ERR: An unexpected fill property was found";

	localErrorStrings[OM_ERR_BAD_LENGTH] = 
		"AAF_ERR: Segment has an illegal length";

	localErrorStrings[OM_ERR_BADRATE] = 
		"AAF_ERR: Illegal value for edit rate";

	localErrorStrings[OM_ERR_INVALID_ROUNDING] = 
		"AAF_ERR: Editrate rounding must be either Floor or Ceiling";

	localErrorStrings[OM_ERR_PULLDOWN_DIRECTION] = 
		"AAF_ERR: Illegal pulldown direction";
	
	localErrorStrings[OM_ERR_PULLDOWN_FUNC] = 
		"AAF_ERR: use AddPulldownRef() instead of AddPhysSourceRef() for pulldown";
	
	localErrorStrings[OM_ERR_NOT_COMPOSITION] =
		"AAF_ERR: This property must be a Composition";

	/* Timecode Error Codes */
	localErrorStrings[OM_ERR_TIMECODE_NOT_FOUND] =
		"AAF_ERR: Timecode was not found in the mob chain";

	localErrorStrings[OM_ERR_NO_TIMECODE] = 
		"AAF_ERR: Cannot find timecode on given track";

	localErrorStrings[OM_ERR_INVALID_TIMECODE] = 
		"AAF_ERR: Timecode value is invalid";

	/* Track Error Codes */
	localErrorStrings[OM_ERR_TRACK_NOT_FOUND] =
		"AAF_ERR: Track not found";

	localErrorStrings[OM_ERR_BAD_SLOTLENGTH] =
		"AAF_ERR: Bad Slot Length";

	localErrorStrings[OM_ERR_MISSING_TRACKID] =
		"AAF_ERR: TrackID not present in the mob";

	localErrorStrings[OM_ERR_TRACK_EXISTS] =
		"AAF_ERR: A Track with this trackID already exists";

	localErrorStrings[OM_ERR_NOT_A_TRACK] =
		"AAF_ERR: This function requires a track, not a mob slot.";

	/* MOBJ Error Codes */
	localErrorStrings[OM_ERR_MOB_NOT_FOUND] =
		"AAF_ERR: Mob does not exist in this file";

	localErrorStrings[OM_ERR_NO_MORE_MOBS] =
		"AAF_ERR: The end of the mob chain has been reached";

	localErrorStrings[OM_ERR_DUPLICATE_MOBID] =
		"AAF_ERR: MobID already exists in the file";

	localErrorStrings[OM_ERR_MISSING_MOBID] =
		"AAF_ERR: MobID not present in the file";

	/* Effect Error Codes */
	localErrorStrings[OM_ERR_EFFECTDEF_EXIST] =
		"AAF_ERR: An Effect Definition with this Effect ID already exists";

	localErrorStrings[OM_ERR_INVALID_EFFECTDEF] = 
		"AAF_ERR: Effect Definition invalid or nonexistant";

	localErrorStrings[OM_ERR_INVALID_EFFECT] = 
		"AAF_ERR: Effect is invalid or non-existent";

	localErrorStrings[OM_ERR_INVALID_EFFECTARG] =
		"AAF_ERR: The given effect argument is not valid for this effect";

	localErrorStrings[OM_ERR_INVALID_CVAL] = 
		"AAF_ERR: Constant value is invalid or nonexistent";

	localErrorStrings[OM_ERR_RENDER_NOT_FOUND] = 
		"AAF_ERR: Effect Rendering does not exist";

	/* Iterator Error Codes */
	localErrorStrings[OM_ERR_BAD_ITHDL] =
		"AAF_ERR: Bad Iterator handle";

	localErrorStrings[OM_ERR_NO_MORE_OBJECTS] =
		"AAF_ERR: No More Objects";

	localErrorStrings[OM_ERR_ITER_WRONG_TYPE] =
		"AAF_ERR: Wrong iterator type for this function";

	localErrorStrings[OM_ERR_INVALID_SEARCH_CRIT] =
		"AAF_ERR: Invalid search criteria for this kind of iterator";

	localErrorStrings[OM_ERR_INTERNAL_ITERATOR] =
		"AAF_INTERNAL_ERR: Internal error with iterator";

	localErrorStrings[OM_ERR_BAD_SRCH_ITER] =
		"AAF_ERR: This iterator handle must be allocated by omfiMobOpenSearch()";

	/* Traversal Error Codes */
	localErrorStrings[OM_ERR_NULL_MATCHFUNC] = 
		"AAF_ERR: Match function to traversal routine is null";

	localErrorStrings[OM_ERR_NULL_CALLBACKFUNC] = 
		"AAF_ERR: Callback function to traversal routine is null";

	localErrorStrings[OM_ERR_TRAVERSAL_NOT_POSS] =
		"AAF_ERR: Mob traversal failed";

	localErrorStrings[OM_ERR_PARSE_EFFECT_AMBIGUOUS] =
		"AAF_ERR: Need more information to parse further through an effect";

	/* Transition Error Codes */
	localErrorStrings[OM_ERR_INVALID_TRAN_EFFECT] =
		"AAF_ERR: Effect is not a transition effect";

	localErrorStrings[OM_ERR_ADJACENT_TRAN] =
		"AAF_ERR: Adjacent transitions in a sequence are illegal";

	localErrorStrings[OM_ERR_LEADING_TRAN] =
		"AAF_ERR: Beginning a sequence with a transition is illegal";

	localErrorStrings[OM_ERR_INSUFF_TRAN_MATERIAL] =
		"AAF_ERR: Not enough material as input to transition";


/*** SIMPLE COMPOSITION Error Codes ***/
	localErrorStrings[OM_ERR_BAD_STRACKHDL] =
		"AAF_ERR: Bad Simple Track handle";

	localErrorStrings[OM_ERR_STRACK_APPEND_ILLEGAL] =
		"AAF_ERR: This track does not contain a sequence, appending is illegal";


/*** GENERIC Error Codes ***/
	localErrorStrings[OM_ERR_NOMEMORY] =
		"AAF_ERR: Memory allocation failed, no more heap memory";

	localErrorStrings[OM_ERR_OFFSET_SIZE] =
		"AAF_ERR: 64-bit truncation error";

	localErrorStrings[OM_ERR_INTERNAL_NEG64] =
		"AAF_ERR: Negative 64-bit number";

	localErrorStrings[OM_ERR_OVERFLOW64] = 
		"AAF_ERR: Overflow on a 64-bit operation";

	localErrorStrings[OM_ERR_NOT_IN_15] =
		"AAF_ERR: Function not available in 1.x compatability mode";

	localErrorStrings[OM_ERR_NOT_IN_20] =
		"AAF_ERR: Function not available in 2.x native mode";

	localErrorStrings[OM_ERR_NOT_IMPLEMENTED] =
		"AAF_ERR: Not Implemented";

	localErrorStrings[OM_ERR_NULL_PARAM] =
		"AAF_ERR: NULL Actual parameter to function call";

	localErrorStrings[OM_ERR_ZERO_DIVIDE] = 
		"AAF_ERR: Divide by zero";
		
/*** SEMANTIC CHECKING Error Codes ***/
	localErrorStrings[OM_ERR_REQUIRED_POSITIVE] =
		"AAF_ERR: Value should be positive or zero";

	localErrorStrings[OM_ERR_INVALID_TRACKKIND] =
		"AAF_ERR: Invalid Track Kind";

	localErrorStrings[OM_ERR_INVALID_EDGETYPE] =
		"AAF_ERR: Invalid Edge Code Format Kind";

	localErrorStrings[OM_ERR_INVALID_FILMTYPE] =
		"AAF_ERR: Invalid Film Type";

	localErrorStrings[OM_ERR_INVALID_TAPECASETYPE] =
		"AAF_ERR: Invalid Tape Case Type";

	localErrorStrings[OM_ERR_INVALID_VIDEOSIGNALTYPE] =
		"AAF_ERR: Invalid Video Signal Type";

	localErrorStrings[OM_ERR_INVALID_TAPEFORMATTYPE] =
		"AAF_ERR: Invalid Tape Format Type";

	localErrorStrings[OM_ERR_INVALID_EDITHINT] =
		"AAF_ERR: Invalid Effect Edit Hint";

	localErrorStrings[OM_ERR_INVALID_INTERPKIND] =
		"AAF_ERR: Invalid Effect Interpolation Kind";

	localErrorStrings[OM_ERR_INVALID_MOBTYPE] =
		"AAF_ERR: Invalid Mob Type";

	localErrorStrings[OM_ERR_INVALID_TRACK_REF] =
		"AAF_ERR: Positive Relative Track Reference not allowed";

	localErrorStrings[OM_ERR_INVALID_OBJ] =
		"AAF_ERR: Invalid object for this operation";

	localErrorStrings[OM_ERR_BAD_VIRTUAL_CREATE] =
		"AAF_ERR: Creation of virtual objects not allowed";

	localErrorStrings[OM_ERR_INVALID_CLASS_ID] =
		"AAF_ERR: Invalid Object Class ID";

	localErrorStrings[OM_ERR_OBJECT_SEMANTIC] =
		"AAF_ERR: Failed a semantic check on an input obj";

	localErrorStrings[OM_ERR_DATA_IN_SEMANTIC] =
		"AAF_ERR: Failed a semantic check on an input data";

	localErrorStrings[OM_ERR_DATA_OUT_SEMANTIC] =
		"AAF_ERR: Failed a semantic check on an output data";

	localErrorStrings[OM_ERR_TYPE_SEMANTIC] = 
		"AAF_ERR: Property and type do not match";

	localErrorStrings[OM_ERR_INVALID_ATTRIBUTEKIND] =
		"AAF_ERR: Invalid Attribute Kind";
		
	localErrorStrings[OM_ERR_DATA_MDES_DISAGREE] =
		"AAF_ERR: Media descriptor summary data does not agree with media";

	localErrorStrings[OM_ERR_CODEC_SEMANTIC_WARN] =
		"AAF_ERR: A semantic check warning occured when checking media";
		
	localErrorStrings[OM_ERR_INVALID_BOOLTYPE] =
		"AAF_ERR: Invalid Boolean Value";

/*** INTERNAL Error Codes ***/
	localErrorStrings[OM_ERR_TABLE_DUP_KEY] =
		"AAF_INTERNAL_ERR: Duplicate Key detected in internal table";
	
	localErrorStrings[OM_ERR_TABLE_MISSING_COMPARE] =
		"AAF_INTERNAL_ERR: Missing compare function on table";

	localErrorStrings[OM_ERR_TABLE_BAD_HDL] =
		"AAF_INTERNAL_ERR: Bad table handle";
		
	localErrorStrings[OM_ERR_TABLE_BAD_ITER] =
		"AAF_INTERNAL_ERR: Bad table iterator handle";
		
	localErrorStrings[OM_ERR_BENTO_PROBLEM] =
		"AAF_ERR: Bento container error";

	localErrorStrings[OM_ERR_BENTO_HANDLER] = 
		"AAF_INTERNAL_ERR: Can't locate Bento handler";
		
	localErrorStrings[OM_ERR_PROPID_MATCH] =
		"AAF_INTERNAL_ERR: Property ID code doesn't match between revs";

	localErrorStrings[OM_ERR_INTERNAL_DIVIDE] = 
		"AAF_ERR: Internal division error";
		
/*** Testing Error Codes ***/
	localErrorStrings[OM_ERR_TEST_FAILED] =
		"AAF_TESTING_ERR: Test Failed";
}

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/

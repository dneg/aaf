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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include <string.h>
#include <wchar.h>
#include "aafErr.h"

struct errorTableTag {
  aafUInt16 _code;
  wchar_t* _name;
  wchar_t* _desc;
} errorTable[] = {
#define AAF_DEFINE_ERROR(name, val, desc) \
  {val, L ## "AAFRESULT_" L ## #name, L ## desc},
#include "AAFMetaResult.h"
};

size_t errorTableEntryCount = sizeof(errorTable) / sizeof(errorTable[0]);

static bool isAAFError(AAFRESULT code)
{
  bool result = false;

  aafUInt16 facility = ((code >> 16) & 0x1fff);
  if (facility == 0x0012) {
    result = true;
  }
  return result;
}

static bool findEntry(size_t& index, aafUInt16 code)
{
  bool result = false;
  for (size_t i = 0; i < errorTableEntryCount; i++) {
    if (errorTable[i]._code == code) {
      index = i;
      result = true;
      break;
    }
  }
  return result;
}

size_t length(size_t index)
{
  size_t result;
  if (wcscmp(errorTable[index]._desc, L"") == 0) {
    result  = (wcslen(errorTable[index]._name) + 1) * sizeof(wchar_t);
  } else {
    result  = (wcslen(errorTable[index]._name) +
               wcslen(errorTable[index]._desc) + 
               wcslen(L" - ") + 1) * sizeof(wchar_t);
  }
  return result;
}

AAFRESULT ResultToTextBufLen (
    AAFRESULT  result,
    aafUInt32 *   pResultTextSize)
{
  HRESULT hr;

  if (isAAFError(result)) {
    aafUInt16 x = result;
    size_t i;

    if (findEntry(i, x)) {
      *pResultTextSize = length(i);
      hr = AAFRESULT_SUCCESS;
    } else {
      hr = AAFRESULT_RESULT_NOT_RECOGNIZED;
    }
  } else {
    hr = AAFRESULT_RESULT_NOT_AAF;
  }
  return hr;
}

AAFRESULT ResultToText (
    AAFRESULT  result,
    aafCharacter *  pResultText,
    aafUInt32  resultTextSize)
{
  HRESULT hr;

  if (isAAFError(result)) {
    aafUInt16 x = result;
    size_t i;

    if (findEntry(i, x)) {
      size_t len = length(i);
      if (resultTextSize >= len) {
        if (wcscmp(errorTable[i]._desc, L"") == 0) {
          wcscpy(pResultText, errorTable[i]._name);
        } else {
          wcscpy(pResultText, errorTable[i]._name);
          wcscat(pResultText, L" - ");
          wcscat(pResultText, errorTable[i]._desc);
        }
        hr = AAFRESULT_SUCCESS;
      } else {
        hr = AAFRESULT_SMALLBUF;
      }
    } else {
      hr = AAFRESULT_RESULT_NOT_RECOGNIZED;
    }
  } else {
    hr = AAFRESULT_RESULT_NOT_AAF;
  }
  return hr;
}

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
	if (code < 300 && code >= 0 && localErrorStrings[0] != '\0')
		return (localErrorStrings[code]);
	else
		return ("AAF_ERR: Unknown error code");
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
			aafErr_t		/* msgcode */)
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
//!!!void aafPrintStackTrace(
//			AAFFile * file)
//{
  // if (file != NULL)
	//  {
//		 printf("***Stack trace from last error****\n");
//		 printf("%s\n", file->_stackTrace);
//	  }
//}
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
	localErrorStrings[AAFRESULT_SUCCESS] =
		"AAF: Success";

/*** 1.0 COMPATIBILITY ONLY Error Codes ***/
//	localErrorStrings[AAFRESULT_BADOBJ] =
//		"AAF_ERR: Null AAF object";

//	localErrorStrings[AAFRESULT_INULLMOBIDPTR] =
//		"AAF_ERR(Import): NULL Mob ID pointer in GetNthMediaDesc/SetMediaDesc";

//	localErrorStrings[AAFRESULT_INULLMTPTR] =
//		"AAF_ERR(Import): NULL omfMediaType_t pointer in GetNthMediaDesc";

//	localErrorStrings[AAFRESULT_INULLDESCPTR] =
//		"AAF_ERR(Import): NULL descriptor pointer in GetNthMediaDesc";

//	localErrorStrings[AAFRESULT_IBADMEDIAINDEX] =
//		"AAF_ERR(Import): Input AAF file does not have enough media objects";

//	localErrorStrings[AAFRESULT_LRC_DISABLED] =
//		"AAF_ERR: LRC codec disabled";

//	localErrorStrings[AAFRESULT_LRC_BADSAMPLESIZE] =
//		"AAF_ERR: Only 16 bit samples supported for LRC";

//	localErrorStrings[AAFRESULT_LRC_MONOONLY] =
//	   "AAF_ERR: Only one channel audio supported for LRC";

//	localErrorStrings[AAFRESULT_BADSAMPLEOFFSET] =
//		"AAF_ERR: Sample Index offset out of range";


/*** SESSION/FILE Error Codes ***/
	/* Session Error Codes */
//	localErrorStrings[AAFRESULT_BAD_SESSION] =
//		"AAF_ERR: Bad Session Handle";

//	localErrorStrings[AAFRESULT_BADSESSIONOPEN] =
//		"AAF_ERR: Container Session open failed";

//	localErrorStrings[AAFRESULT_BADSESSIONMETA] =
//		"AAF_ERR: Container Session meta-handler operation failed";

//	localErrorStrings[AAFRESULT_BADSESSIONCLOSE] =
//		"AAF_ERR: Container Session close failed";

	/* File Error Codes */
//	localErrorStrings[AAFRESULT_FILEREV_NOT_SUPP] =
//		"AAF_ERR: AAF File Revision > 2.0 not supported";

//	localErrorStrings[AAFRESULT_FILEREV_DIFF] = 
//		"AAF_ERR: Files must have same file revisions";

//	localErrorStrings[AAFRESULT_BADOPEN] =
//		"AAF_ERR: Cannot open file";

//	localErrorStrings[AAFRESULT_BADCLOSE] =
//		"AAF_ERR: Cannot close file";

//	localErrorStrings[AAFRESULT_BAD_FHDL] =
//		"AAF_ERR: Bad File Handle";

//	localErrorStrings[AAFRESULT_BADHEAD] =
//		"AAF_ERR: Invalid AAF HEAD object";

//	localErrorStrings[AAFRESULT_NOBYTEORDER] =
//		"AAF_ERR: No File Byte Order Specified";

//	localErrorStrings[AAFRESULT_INVALID_BYTEORDER] =
//		"AAF_ERR: Invalid byte order";

//	localErrorStrings[AAFRESULT_NOTAAFFILE] =
//		"AAF_ERR: Invalid AAF file";

//	localErrorStrings[AAFRESULT_WRONG_FILETYPE] =
//		"AAF_ERR: Invalid file type (Raw vs. AAF)";

//	localErrorStrings[AAFRESULT_WRONG_OPENMODE] =
//		"AAF_ERR: File opened in wrong mode (readonly vs. readwrite)";

//	localErrorStrings[AAFRESULT_CONTAINERWRITE] = 
//		"AAF_ERR: Error writing to container media (Possibly media is full).";
		
//	localErrorStrings[AAFRESULT_FILE_NOT_FOUND] = 
//		"AAF_ERR: File not found.";
		
	/* Class Dictionary Error Codes */
//	localErrorStrings[AAFRESULT_CANNOT_SAVE_CLSD] = 
//		"AAF_ERR: Error updating the class dictionary in the AAF file";

//	localErrorStrings[AAFRESULT_CANNOT_LOAD_CLSD] = 
//		"AAF_ERR: Error loading the class dictionary from the file";
		
//	localErrorStrings[AAFRESULT_FILE_REV_200] = 
//		"AAF_ERR: Please use toolkit revision 2.0.1 or later to write 2.x compliant files";

//	localErrorStrings[AAFRESULT_NEED_PRODUCT_IDENT] = 
//		"AAF_ERR: You must supply a product identification struct when creating or updating files";


/*** MEDIA Error Codes ***/
	/* General Media Error Codes */
//	localErrorStrings[AAFRESULT_DESCSAMPRATE] =
//		"AAF_ERR: Error getting sample rate from file descriptor";

//	localErrorStrings[AAFRESULT_SOURCEMOBLIST] =
//		"AAF_ERR: Error processing source mob list";

//	localErrorStrings[AAFRESULT_DESCLENGTH] =
//		"AAF_ERR: Error getting length from file descriptor";

//	localErrorStrings[AAFRESULT_INTERNAL_MDO] =
//		"AAF_ERR: Internal Media Data system not initialized";

//	localErrorStrings[AAFRESULT_3COMPONENT] =
//		"AAF_ERR: Only 3-component video allowed in AAF files";

//	localErrorStrings[AAFRESULT_INTERNAL_CORRUPTVINFO] =
//		"AAF_INTERNAL_ERR: Corrupt Video Info Structure";

//	localErrorStrings[AAFRESULT_BADSAMPLEOFFSET] =
//		"AAF_ERR: Sample Index offset out of range";

//	localErrorStrings[AAFRESULT_ONESAMPLEREAD] =
//		"AAF_ERR: Only one sample allowed for reading compressed data";

//	localErrorStrings[AAFRESULT_ONESAMPLEWRITE] =
//		"AAF_ERR: Only one sample allowed for writing compressed data";

//	localErrorStrings[AAFRESULT_DECOMPRESS] =
//		"AAF_ERR: Software Decompression Failed";

//	localErrorStrings[AAFRESULT_NODATA] =
//		"AAF_ERR: No data read";

//	localErrorStrings[AAFRESULT_SMALLBUF] =
//		"AAF_ERR: Sample too large to fit in given buffer";

//	localErrorStrings[AAFRESULT_INTERN_TOO_SMALL] =
//		"AAF_ERR: Buffer is not large enough to hold data";

//	localErrorStrings[AAFRESULT_INTERNAL_UNKNOWN_LOC] =
//		"AAF_ERR: Unknown locator type";

//	localErrorStrings[AAFRESULT_TRANSLATE] =
//		"AAF_ERR: Can't translate to expected memory format";

//	localErrorStrings[AAFRESULT_EOF] =
//		"AAF_ERR: End of file";

//	localErrorStrings[AAFRESULT_BADCOMPR] =
//		"AAF_ERR: Unrecognized compression type in AAF file";

//	localErrorStrings[AAFRESULT_BADPIXFORM] =
//		"AAF_ERR: Unrecognized pixel format in AAF file";

//	localErrorStrings[AAFRESULT_BADLAYOUT] =
//		"AAF_ERR: Unrecognized frame layout in AAF file";

//	localErrorStrings[AAFRESULT_COMPRLINESWR] =
//		"AAF_ERR: omfWriteLines not allowed with compression";

//	localErrorStrings[AAFRESULT_COMPRLINESRD] =
//		"AAF_ERR: omfReadLines not allowed with compression";

//	localErrorStrings[AAFRESULT_BADMEDIATYPE] =
//		"AAF_ERR: Unrecognized Media Type";

//	localErrorStrings[AAFRESULT_BADDATAADDRESS] =
//		"AAF_ERR: Null data address for transfer operation";

//	localErrorStrings[AAFRESULT_BAD_MDHDL] =
//		"AAF_ERR: Bad Media Handle";

//	localErrorStrings[AAFRESULT_MEDIA_NOT_FOUND] =
//		"AAF_ERR: Cannot locate media data";

//	localErrorStrings[AAFRESULT_ILLEGAL_MEMFMT] =
//		"AAF_ERR: Illegal memory format code";

//	localErrorStrings[AAFRESULT_ILLEGAL_FILEFMT] =
//		"AAF_ERR: Illegal file format code";

//	localErrorStrings[AAFRESULT_SWABBUFSIZE] =
//		"AAF_ERR: Invalid swab buffer size";

//	localErrorStrings[AAFRESULT_MISSING_SWAP_PROC] =
//		"AAF_ERR: Missing stream swap proc";

//	localErrorStrings[AAFRESULT_NULL_STREAMPROC] = 
//		"AAF_ERR: A Stream Callback is NULL";

//	localErrorStrings[AAFRESULT_NULLBUF] =
//		"AAF_ERR: Null transfer buffer";

//	localErrorStrings[AAFRESULT_SWAP_SETUP] =
//		"AAF_ERR: Need to set up file & memory format before calling this function";

//	localErrorStrings[AAFRESULT_INVALID_FILE_MOB] =
//		"AAF_ERR: Invalid file mob reference";
		
//	localErrorStrings[AAFRESULT_SINGLE_CHANNEL_OP] =
//		"AAF_ERR: Operation not valid on multi-channel media streams";

//	localErrorStrings[AAFRESULT_INVALID_CACHE_SIZE] = 
//		"AAF_ERR: Stream cache size must be positive or zero";

//	localErrorStrings[AAFRESULT_NOT_FILEMOB] = 
//		"AAF_ERR: Operation requires a file mob";

//	localErrorStrings[AAFRESULT_TRANSLATE_SAMPLE_SIZE] = 
//		"AAF_ERR: Codec can't translate to/from that sample size";
		
//	localErrorStrings[AAFRESULT_TRANSLATE_NON_INTEGRAL_RATE] =
//		"AAF_ERR: Codec can't translate to/from rates which are not multiples";

//	localErrorStrings[AAFRESULT_MISSING_MEDIA_REP] =
//		"AAF_ERR: Media representation not present in the mob";

//	localErrorStrings[AAFRESULT_NOT_LONGWORD] = 
//		"AAF_ERR: Buffer must be longword aligned for this translation";
		
//	localErrorStrings[AAFRESULT_XFER_DUPCH] = 
//		"AAF_ERR: Cannot specify the same channel twice in multi-xfers";

//	localErrorStrings[AAFRESULT_MEDIA_NOT_INIT] = 
//		"AAF_ERR: omfmInit() must be called before making this call";
		
//	localErrorStrings[AAFRESULT_BLOCKING_SIZE] = 
//		"AAF_ERR: Blocking size must be >= 0 bytes";
		
//	localErrorStrings[AAFRESULT_WRONG_MEDIATYPE] = 
//		"AAF_ERR: Incorrect media type for this operation";

//	localErrorStrings[AAFRESULT_MULTI_WRITELEN] = 
//		"AAF_ERR: Writelengths in a writeMultiple must be uniform";

//	localErrorStrings[AAFRESULT_STREAM_REOPEN] = 
//		"AAF_ERROR: Stream reopened without being closed";

//	localErrorStrings[AAFRESULT_TOO_MANY_FMT_OPS] = 
//		"AAF_ERROR: Too many format specifiers for this codec";
		
//	localErrorStrings[AAFRESULT_MEDIASTREAM_NOTALLOWED] =
//		"AAF_ERR: An object with a media stream datakind is not allowed";

//	localErrorStrings[AAFRESULT_STILLFRAME_BADLENGTH] =
//		"AAF_ERR: Length of a still frame in a media group must be 1";

//	localErrorStrings[AAFRESULT_DATA_NONCONTIG] = 
//		"AAF_ERR: Calling this function now will produce non-contiguous media data";
	   
//	localErrorStrings[AAFRESULT_OPLIST_OVERFLOW] =
//	   "AAF_ERR: Operation list overflow";
	   
//	localErrorStrings[AAFRESULT_STREAM_CLOSED] = 
//		"AAF_ERR: Stream must be open for this operation";
		
//	localErrorStrings[AAFRESULT_USE_MULTI_CREATE] = 
//		"AAF_ERR: Use multiCreate to create this many interleaved channels";

//	localErrorStrings[AAFRESULT_MEDIA_OPENMODE] =
//		"AAF_ERR: Media object opened in wrong mode (readonly vs. readwrite)";

//	localErrorStrings[AAFRESULT_MEDIA_CANNOT_CLOSE] =
//		"AAF_ERR: No proc to close media";

//	localErrorStrings[AAFRESULT_XFER_NOT_BYTES] =
//		"AAF_ERR: Sample transfers must be an integral number of bytes";
		
//	localErrorStrings[AAFRESULT_ZERO_SAMPLESIZE] =
//		"AAF_ERR: Sample size of zero not allowed";
		
//	localErrorStrings[AAFRESULT_ZERO_PIXELSIZE] =
//		"AAF_ERR: Pixel size of zero not allowed";
		
	/* Codec Error Codes */
//	localErrorStrings[AAFRESULT_CODEC_INVALID] =
//		"CODEC invalid or not loaded";

//	localErrorStrings[AAFRESULT_INVALID_OP_CODEC] =
//		"AAF_ERR: Operation not valid on this codec";

//	localErrorStrings[AAFRESULT_BAD_CODEC_REV] =
//		"AAF_ERR: Out of date codec";

//	localErrorStrings[AAFRESULT_CODEC_CHANNELS] =
//		"AAF_ERR: Channel out of range for codec";
		
//	localErrorStrings[AAFRESULT_BAD_VARIETY] = 
//		"AAF_ERR: Badly formed codec variety string";

//	localErrorStrings[AAFRESULT_CODEC_NAME_SIZE] = 
//		"AAF_ERR: Codec name string too large";

	/* Image Error Codes */
//	localErrorStrings[AAFRESULT_TIFFVERSION] =
//		"AAF_ERR: Error reading tiff version";

//	localErrorStrings[AAFRESULT_BADTIFFCOUNT] =
//		"AAF_ERR: Video: TIFF count less than 1";

//	localErrorStrings[AAFRESULT_24BITVIDEO] =
//		"AAF_ERR: 24 Bit (8-8-8) video only";

//	localErrorStrings[AAFRESULT_JPEGBASELINE] =
//		"AAF_ERR: Only baseline JPEG allowed in AAF files";

//	localErrorStrings[AAFRESULT_BADJPEGINFO] =
//		"AAF_ERR: JPEG TIFF fields not allowed without COMPRESSION == JPEG";

//	localErrorStrings[AAFRESULT_BADQTABLE] =
//		"AAF_ERR: Bad JPEG Quantization Table";

//	localErrorStrings[AAFRESULT_BADACTABLE] =
//		"AAF_ERR: Bad JPEG AC Huffman Table";

//	localErrorStrings[AAFRESULT_BADDCTABLE] =
//		"AAF_ERR: Bad JPEG DC Huffman Table";

//	localErrorStrings[AAFRESULT_NOFRAMEINDEX] =
//		"AAF_ERR: No JPEG Video Frame Index";

//	localErrorStrings[AAFRESULT_BADFRAMEOFFSET] =
//		"AAF_ERR: Frame Index offset out of range";

//	localErrorStrings[AAFRESULT_JPEGPCM] =
//		"AAF_ERR: OMJPEG.c: put_color_map called";

//	localErrorStrings[AAFRESULT_JPEGDISABLED] =
//		"AAF_ERR: JPEG codec disabled";

//	localErrorStrings[AAFRESULT_JPEGPROBLEM] =
//		"AAF_ERR: Unspecified JPEG codec problem";

//	localErrorStrings[AAFRESULT_BADEXPORTPIXFORM] =
//		"AAF_ERR: Unrecognized Pixel Format";

//	localErrorStrings[AAFRESULT_BADEXPORTLAYOUT] =
//		"AAF_ERR: Unrecognized Frame Layout";

//	localErrorStrings[AAFRESULT_BADRWLINES] =
//		"AAF_ERR: Read/Write Lines only enabled for common video format";

	/* Audio Error Codes */
//	localErrorStrings[AAFRESULT_BADAIFCDATA] =
//		"AAF_ERR: Error reading AIFC data";

//	localErrorStrings[AAFRESULT_BADWAVEDATA] =
//		"AAF_ERR: Error reading WAVE data";

//	localErrorStrings[AAFRESULT_NOAUDIOCONV] =
//		"AAF_ERR: Audio Read Samples: Can't convert to requested sample size";


/*** OBJECT Error Codes ***/
//	localErrorStrings[AAFRESULT_NULLOBJECT] =
//		"AAF_ERR: Null Object not allowed";

//	localErrorStrings[AAFRESULT_BADINDEX] =
//		"AAF_ERR: Array Index Out of Range";

//	localErrorStrings[AAFRESULT_INVALID_LINKAGE] =
//		"AAF_ERR: Invalid object attached to property";

//	localErrorStrings[AAFRESULT_BAD_PROP] =
//		"AAF_ERR: Property code out of range";

//	localErrorStrings[AAFRESULT_BAD_TYPE] =
//		"AAF_ERR: Type code out of range";

//	localErrorStrings[AAFRESULT_SWAB] =
//		"AAF_ERR: Cannot swab that data size";

//	localErrorStrings[AAFRESULT_END_OF_DATA] =
//		"AAF_ERR: Read past end of data";

//	localErrorStrings[AAFRESULT_PROP_NOT_PRESENT] =
//		"AAF_ERR: Property missing from the file";

//	localErrorStrings[AAFRESULT_INVALID_DATAKIND] =
//		"AAF_ERR: Datakind invalid or nonexistant";

//	localErrorStrings[AAFRESULT_DATAKIND_EXIST] =
//		"AAF_ERR: A Datakind Definition with this ID already exists";
		
//	localErrorStrings[AAFRESULT_TOO_MANY_TYPES] =
//		"AAF_ERR: Too many types for a single property";

/*** MOB Error Codes ***/
	/* General Segment Error Codes */
//	localErrorStrings[AAFRESULT_NOT_SOURCE_CLIP] =
//		"AAF_ERR: This property must be a Source Clip";

//	localErrorStrings[AAFRESULT_FILL_FOUND] =
//		"AAF_ERR: An unexpected fill property was found";

//	localErrorStrings[AAFRESULT_BAD_LENGTH] = 
//		"AAF_ERR: Segment has an illegal length";

//	localErrorStrings[AAFRESULT_BADRATE] = 
//		"AAF_ERR: Illegal value for edit rate";

//	localErrorStrings[AAFRESULT_INVALID_ROUNDING] = 
//		"AAF_ERR: Editrate rounding must be either Floor or Ceiling";

//	localErrorStrings[AAFRESULT_PULLDOWN_DIRECTION] = 
//		"AAF_ERR: Illegal pulldown direction";
	
//	localErrorStrings[AAFRESULT_PULLDOWN_FUNC] = 
//		"AAF_ERR: use AddPulldownRef() instead of AddPhysSourceRef() for pulldown";
	
//	localErrorStrings[AAFRESULT_NOT_COMPOSITION] =
//		"AAF_ERR: This property must be a Composition";

	/* Timecode Error Codes */
//	localErrorStrings[AAFRESULT_TIMECODE_NOT_FOUND] =
//		"AAF_ERR: Timecode was not found in the mob chain";

//	localErrorStrings[AAFRESULT_NO_TIMECODE] = 
//		"AAF_ERR: Cannot find timecode on given track";

//	localErrorStrings[AAFRESULT_INVALID_TIMECODE] = 
//		"AAF_ERR: Timecode value is invalid";

	/* Track Error Codes */
//	localErrorStrings[AAFRESULT_TRACK_NOT_FOUND] =
//		"AAF_ERR: Track not found";

//	localErrorStrings[AAFRESULT_BAD_SLOTLENGTH] =
//		"AAF_ERR: Bad Slot Length";

//	localErrorStrings[AAFRESULT_MISSING_TRACKID] =
//		"AAF_ERR: TrackID not present in the mob";

//	localErrorStrings[AAFRESULT_TRACK_EXISTS] =
//		"AAF_ERR: A Track with this trackID already exists";

//	localErrorStrings[AAFRESULT_NOT_A_TRACK] =
//		"AAF_ERR: This function requires a track, not a mob slot.";

	/* MOBJ Error Codes */
//	localErrorStrings[AAFRESULT_MOB_NOT_FOUND] =
//		"AAF_ERR: Mob does not exist in this file";

//	localErrorStrings[AAFRESULT_NO_MORE_MOBS] =
//		"AAF_ERR: The end of the mob chain has been reached";

//	localErrorStrings[AAFRESULT_DUPLICATE_MOBID] =
//		"AAF_ERR: MobID already exists in the file";

//	localErrorStrings[AAFRESULT_MISSING_MOBID] =
//		"AAF_ERR: MobID not present in the file";

	/* Effect Error Codes */
//	localErrorStrings[AAFRESULT_EFFECTDEF_EXIST] =
//		"AAF_ERR: An Effect Definition with this Effect ID already exists";

//	localErrorStrings[AAFRESULT_INVALID_EFFECTDEF] = 
//		"AAF_ERR: Effect Definition invalid or nonexistant";

//	localErrorStrings[AAFRESULT_INVALID_EFFECT] = 
//		"AAF_ERR: Effect is invalid or non-existent";

//	localErrorStrings[AAFRESULT_INVALID_EFFECTARG] =
//		"AAF_ERR: The given effect argument is not valid for this effect";

//	localErrorStrings[AAFRESULT_INVALID_CVAL] = 
//		"AAF_ERR: Constant value is invalid or nonexistent";

//	localErrorStrings[AAFRESULT_RENDER_NOT_FOUND] = 
//		"AAF_ERR: Effect Rendering does not exist";

	/* Iterator Error Codes */
//	localErrorStrings[AAFRESULT_BAD_ITHDL] =
//		"AAF_ERR: Bad Iterator handle";

//	localErrorStrings[AAFRESULT_NO_MORE_OBJECTS] =
//		"AAF_ERR: No More Objects";

//	localErrorStrings[AAFRESULT_ITER_WRONG_TYPE] =
//		"AAF_ERR: Wrong iterator type for this function";

//	localErrorStrings[AAFRESULT_INVALID_SEARCH_CRIT] =
//		"AAF_ERR: Invalid search criteria for this kind of iterator";

//	localErrorStrings[AAFRESULT_INTERNAL_ITERATOR] =
//		"AAF_INTERNAL_ERR: Internal error with iterator";

//	localErrorStrings[AAFRESULT_BAD_SRCH_ITER] =
//		"AAF_ERR: This iterator handle must be allocated by omfiMobOpenSearch()";

	/* Traversal Error Codes */
//	localErrorStrings[AAFRESULT_NULL_MATCHFUNC] = 
//		"AAF_ERR: Match function to traversal routine is null";

//	localErrorStrings[AAFRESULT_NULL_CALLBACKFUNC] = 
//		"AAF_ERR: Callback function to traversal routine is null";

//	localErrorStrings[AAFRESULT_TRAVERSAL_NOT_POSS] =
//		"AAF_ERR: Mob traversal failed";

//	localErrorStrings[AAFRESULT_PARSE_EFFECT_AMBIGUOUS] =
//		"AAF_ERR: Need more information to parse further through an effect";

	/* Transition Error Codes */
//	localErrorStrings[AAFRESULT_INVALID_TRAN_EFFECT] =
//		"AAF_ERR: Effect is not a transition effect";

//	localErrorStrings[AAFRESULT_ADJACENT_TRAN] =
//		"AAF_ERR: Adjacent transitions in a sequence are illegal";

//	localErrorStrings[AAFRESULT_LEADING_TRAN] =
//		"AAF_ERR: Beginning a sequence with a transition is illegal";

//	localErrorStrings[AAFRESULT_INSUFF_TRAN_MATERIAL] =
//		"AAF_ERR: Not enough material as input to transition";


/*** SIMPLE COMPOSITION Error Codes ***/
//	localErrorStrings[AAFRESULT_BAD_STRACKHDL] =
//		"AAF_ERR: Bad Simple Track handle";

//	localErrorStrings[AAFRESULT_STRACK_APPEND_ILLEGAL] =
//		"AAF_ERR: This track does not contain a sequence, appending is illegal";


/*** GENERIC Error Codes ***/
//	localErrorStrings[AAFRESULT_NOMEMORY] =
//		"AAF_ERR: Memory allocation failed, no more heap memory";

//	localErrorStrings[AAFRESULT_OFFSET_SIZE] =
//		"AAF_ERR: 64-bit truncation error";

//	localErrorStrings[AAFRESULT_INTERNAL_NEG64] =
//		"AAF_ERR: Negative 64-bit number";

//	localErrorStrings[AAFRESULT_OVERFLOW64] = 
//		"AAF_ERR: Overflow on a 64-bit operation";

//	localErrorStrings[AAFRESULT_NOT_IN_15] =
//		"AAF_ERR: Function not available in 1.x compatability mode";

//	localErrorStrings[AAFRESULT_NOT_IN_20] =
//		"AAF_ERR: Function not available in 2.x native mode";

//	localErrorStrings[AAFRESULT_NOT_IMPLEMENTED] =
//		"AAF_ERR: Not Implemented";

//	localErrorStrings[AAFRESULT_NULL_PARAM] =
//		"AAF_ERR: NULL Actual parameter to function call";

//	localErrorStrings[AAFRESULT_ZERO_DIVIDE] = 
//		"AAF_ERR: Divide by zero";
		
/*** SEMANTIC CHECKING Error Codes ***/
//	localErrorStrings[AAFRESULT_REQUIRED_POSITIVE] =
//		"AAF_ERR: Value should be positive or zero";

//	localErrorStrings[AAFRESULT_INVALID_TRACKKIND] =
//		"AAF_ERR: Invalid Track Kind";

//	localErrorStrings[AAFRESULT_INVALID_EDGETYPE] =
//		"AAF_ERR: Invalid Edge Code Format Kind";

//	localErrorStrings[AAFRESULT_INVALID_FILMTYPE] =
//		"AAF_ERR: Invalid Film Type";

//	localErrorStrings[AAFRESULT_INVALID_TAPECASETYPE] =
//		"AAF_ERR: Invalid Tape Case Type";

//	localErrorStrings[AAFRESULT_INVALID_VIDEOSIGNALTYPE] =
//		"AAF_ERR: Invalid Video Signal Type";

//	localErrorStrings[AAFRESULT_INVALID_TAPEFORMATTYPE] =
//		"AAF_ERR: Invalid Tape Format Type";

//	localErrorStrings[AAFRESULT_INVALID_EDITHINT] =
//		"AAF_ERR: Invalid Effect Edit Hint";

//	localErrorStrings[AAFRESULT_INVALID_INTERPKIND] =
//		"AAF_ERR: Invalid Effect Interpolation Kind";

//	localErrorStrings[AAFRESULT_INVALID_MOBTYPE] =
//		"AAF_ERR: Invalid Mob Type";

//	localErrorStrings[AAFRESULT_INVALID_TRACK_REF] =
//		"AAF_ERR: Positive Relative Track Reference not allowed";

//	localErrorStrings[AAFRESULT_INVALID_OBJ] =
//		"AAF_ERR: Invalid object for this operation";

//	localErrorStrings[AAFRESULT_BAD_VIRTUAL_CREATE] =
//		"AAF_ERR: Creation of virtual objects not allowed";

//	localErrorStrings[AAFRESULT_INVALID_CLASS_ID] =
//		"AAF_ERR: Invalid Object Class ID";

//	localErrorStrings[AAFRESULT_OBJECT_SEMANTIC] =
//		"AAF_ERR: Failed a semantic check on an input obj";

//	localErrorStrings[AAFRESULT_DATA_IN_SEMANTIC] =
//		"AAF_ERR: Failed a semantic check on an input data";

//	localErrorStrings[AAFRESULT_DATA_OUT_SEMANTIC] =
//		"AAF_ERR: Failed a semantic check on an output data";

//	localErrorStrings[AAFRESULT_TYPE_SEMANTIC] = 
//		"AAF_ERR: Property and type do not match";

//	localErrorStrings[AAFRESULT_INVALID_ATTRIBUTEKIND] =
//		"AAF_ERR: Invalid Attribute Kind";
		
//	localErrorStrings[AAFRESULT_DATA_MDES_DISAGREE] =
//		"AAF_ERR: Media descriptor summary data does not agree with media";

//	localErrorStrings[AAFRESULT_CODEC_SEMANTIC_WARN] =
//		"AAF_ERR: A semantic check warning occured when checking media";
		
//	localErrorStrings[AAFRESULT_INVALID_BOOLTYPE] =
//		"AAF_ERR: Invalid Boolean Value";

/*** INTERNAL Error Codes ***/
//	localErrorStrings[AAFRESULT_TABLE_DUP_KEY] =
//		"AAF_INTERNAL_ERR: Duplicate Key detected in internal table";
	
//	localErrorStrings[AAFRESULT_TABLE_MISSING_COMPARE] =
//		"AAF_INTERNAL_ERR: Missing compare function on table";

//	localErrorStrings[AAFRESULT_TABLE_BAD_HDL] =
//		"AAF_INTERNAL_ERR: Bad table handle";
		
//	localErrorStrings[AAFRESULT_TABLE_BAD_ITER] =
//		"AAF_INTERNAL_ERR: Bad table iterator handle";
				
//	localErrorStrings[AAFRESULT_PROPID_MATCH] =
//		"AAF_INTERNAL_ERR: Property ID code doesn't match between revs";

//	localErrorStrings[AAFRESULT_INTERNAL_DIVIDE] = 
//		"AAF_ERR: Internal division error";
		
/*** Testing Error Codes ***/
//	localErrorStrings[AAFRESULT_TEST_FAILED] =
//		"AAF_TESTING_ERR: Test Failed";
}

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:3 ***
;;; End: ***
*/

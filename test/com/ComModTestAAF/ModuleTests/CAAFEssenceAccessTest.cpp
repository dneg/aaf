// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/***********************************************\
*  Stub only.   Implementation not yet added    *
\***********************************************/












#include "CAAFEssenceAccess.h"
#include "CAAFEssenceAccess.h"
#ifndef __CAAFEssenceAccess_h__
#error - improperly defined include guard
#endif

#include <iostream.h>

HRESULT CAAFEssenceAccess::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEssenceAccess *pObject = NULL;

  try
	{
	  // Attempt to create an AAFEssenceAccess.
	  hr =  CoCreateInstance(
							 CLSID_AAFEssenceAccess,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFEssenceAccess, (void **)&pObject);
	  if (FAILED(hr))
		{
		  cerr << "CAAFEssenceAccess::test...FAILED!";
		  cerr << hr;
		  cerr << "\tCoCreateInstance(&CLSID_AAFEssenceAccess, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFEssenceAccess, ...);" <<
			  endl;
		  return hr;
		}

	  // module-specific tests go here

      if (pObject)
        pObject->Release();
	  return AAFRESULT_NOT_IMPLEMENTED;

	}
  catch (...)
	{
	  cerr << "CAAFEssenceAccess::test...Caught general C++"
		" exception!" << endl; 
	}

  // Cleanup our object if it exists.
  if (pObject)
	pObject->Release();

  return hr;
}


/****/

//@comm Creates either a single stream of audio essence, or interleaved
// audio-only data.  A separate call (MediaMultiCreate) exists 
// in order to create interleaved audio and video data.
// 
//@comm The essence object initialized from this call can be used with
// WriteDataSamples or WriteMultiSamples but NOT with 
// or WriteDataLines.
// 
// Argument Notes:
//@comm If you are creating the essence, and then attaching it to a master
// mob, then the "masterMob" field may be left NULL.
// 
//@comm The numChannels field refers to the number of interleaved
// channels on a single data stream.  
// 
//@comm The sample rate should be the actual samples per second, not the
// edit rate.
//@comm omfmAudioMediaCreate

/****/

//@comm Creates a single stream of video essence.  A separate call
// (MediaMultiCreate) exists to create interleaved audio and
// video data.
//@comm The essence object initialized from this call can be used with
// WriteDataSamples or WriteDataLines, but NOT with
// WriteMultiSamples.
//@comm If you are creating the essence, and then attaching it to a master
// mob, then the "masterMob" field may be left NULL.
//@comm The storedHeight and storedWidth are the dimensions of the frame
// as stored on disk (or as it should be restored by the codec.  The
// displayRect and sampledRect are set to:
// 	(0,0 @ sampledWidth, sampledHeight).
//@comm If the displayed rectangle is not the same as the stored rectangle
// (as with the old leadingLines and trailingLines), then you should
// call SetDisplayRect().
//@comm The frame layout contains the number of fields and whether they are
// interlaced, but does not specify field dominance.
//@comm Replaces omfmVideoMediaCreate

/****/

	//@comm Creates a single channel stream of essence.  Convenience functions
	// exist to create audio or video essence, and a separate call
	// (MultiCreate) exists to create interleaved audio and
	// video data.
	//@comm The essence handle from this call can be used with
	// WriteDataSamples  and possibly WriteDataLines, but NOT with
	// WriteMultiSamples.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	// For video, the sampleRate should be the edit rate of the file mob.
	// For audio, the sample rate should be the actual samples per second.
	//@comm Replaces omfmMediaCreate
	
/****/

	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	// or WriteDataLines.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	//@comm Replaces omfmMediaMultiCreate
	
/****/

	//@comm Allocating the space in this fashion ensures that the data will be
	// contiguous on disk (for at least numBytes bytes) even if other
	// disk operations allocate space on the disk.  If the data written
	// exceeds numBytes, then another disk block of numBytes size will be
	// allocated.
	//@comm Takes a essence handle, so the essence must have been opened or created.
	// The space is allocated in terms of bytes.
	//@comm Replaces omfmSetBlockingSize

/****/

	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces omfmSetVideoLineMap.

/****/

	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces omfmGetVideoTopField
	
/****/

	//@comm Handles the case where the displayed rectangle is not the same as
	// the stored rectangle (as with the old leadingLines and trailingLines).
	// A positive "leadingLines" (from 1.5) becomes a positive yOffset, and
	// decreases the display height.
	// A positive "trailingLines" (from 1.5) also decreases the display height.
	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces  omfmSetDisplayRect

/****/

	//@comm Takes a essence handle, so the essence must have been opened or created
	//@comm Replaces omfmSetSampledRect
	
/****/

	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmWriteMultiSamples

/****/

	//@comm Takes a essence handle, so the essence must have been opened or created.
	// A single video frame is ONE sample.
	// Buflen must be large enough to hold nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_SINGLE_CHANNEL_OP -- Tried to write to an interleaved stream.
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmWriteDataSamples

/****/

	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold
	// nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmWriteRawData
	
/****/

	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmReadRawData
	
/****/

	//@comm Writes single lines of video to a file.  This function allows writing
	// video frames in pieces, for low-memory situations.  When enough lines
	// have been written to constitute a frame, then the number of samples will
	// be incremented by one.
	//@comm This function works only for video essence.
	//@comm The buffer must be large enough to hold an entire line of video. 
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_BADRWLINES -- This function only works for video essence.
	//@comm Replaces omfmWriteDataLines
	//@devnote Previous version did not have nBytesPerLine, otherwise the actual size
	// of the buffer cannot be known.
	
/****/

	//@comm This function should be called whether the essence was opened or created.
	//@comm Replaces omfmMediaClose

/****/

	//@comm This function needs to be called once for each component of the video.
	//@comm Replaces omfmSetJPEGTables
	
/****/

	//@comm Replaces omfmSetAudioCompressParms
/****/

	//@comm Replaces omfmGetAudioCompressParms
	
/****/

	//@parm IN -- .
	//@comm The parameter block should be defined in the
	// "h" file of the codec, and must be included by the application in
	// order to use this call.
	//@comm NOTE: All CODECs should default to reasonable parameters,
	// in case the application doesn't know about a given codec.
	//@comm Replaces omfmCodecSendPrivateData*/

/****/

	//@comm The parameter block should be defined in the
	// "h" file of the codec, and must be included by the application in
	// order to use this call.
	//@comm NOTE: All CODECs should default to reasonable parameters,
	// in case the application doesn't know about a given codec.
	//@comm Replaces omfmGetPrivateMediaData*/
	//@devnote Added bytesRead 

/****/

	//@comm Returns the number of interleaved essence channels of a given type in the essence stream referenced by the given file mob
	//@comm If the data format is not interleaved, then the answer will
	// always be zero or one.  This function correctly returns zero
	// for essence types not handled by a given codec, and handles codecs
	// which work with multiple essence types.
	//@comm Replaces omfmGetNumChannels*/

/****/

	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Replaces omfmGetLargestSampleSize*/

/****/

	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	OM_ERR_NULL_PARAM -- A return parameter was NULL.
	//@comm Replaces omfmGetSampleFrameSize*/

/****/

	//@comm If the essence is interleaved,
	// then it will be di-interleaved when samples are read.  This routine
	// follows the locator, and may call the locator failure callback if
	// the essence can not be found.  If the failure callback finds the essence,
	// then this routine will return normally.
	//@comm The essence handle from this call can be used with
	// ReadDataSamples  and possibly ReadDataLines, but NOT with
	// ReadMultiSamples.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	OM_ERR_NOMEMORY -- couldn't allocate memory for the essence handle
	//@comm NOTE: If a locator is followed, then essencePtr may reference ANOTHER file
	// object, which must be closed on file close.
	//@comm Replaces omfmMediaOpen*/
	
/****/

	//@comm This routine
	// follows the locator, and may call the locator failure callback if
	// the essence can not be found.  If the failure callback finds the essence,
	// then this routine will return normally.
	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	//  WriteDataLines.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	OM_ERR_NOMEMORY -- couldn't allocate memory for the essence handle
	//@comm Replaces omfmMediaMultiOpen*/

/****/

	//@comm This is the format expected on writes and produced on reads.
	//@comm On writes, the data will be written in this format, except
	// where a software codec may be used.
	// On reads, the data will be translated to this format.
	//@comm The current CODECs should support rgb888 and YUV as formats
	// and all of the standard layouts.  A special format of
	// kVmFmtStd says to use the file's native format & layout.
	//@comm Replaces omfmSetVideoMemFormat*/

/****/

	//@comm This is the format expected on writes and produced on reads.
	//@comm On writes, the data will be written in this format, except
	// where a software codec may be used.
	// On reads, the data will be translated to this format.
	//@comm The current CODECs should support different sample sizes and rates
	// A special format of kAmFmtStd says to use the file's native
	// size and rate.
 	//@comm Replaces omfmSetAudioMemFormat

/****/

	//@comm Handles the case where the displayed rectangle is not the same as
	// the stored rectangle (as with the old leadingLines and
	// trailingLines).<nl>
	// A positive "leadingLines" (from 1.5) becomes a positive yOffset, and
	// decreases the display height.<nl>
	// A positive "trailingLines" (from 1.5) also decreases the display
	// height.
	//@comm Takes a essence handle, so the essence must have been opened or
	// created.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	//   OM_ERR_NULL_PARAM -- A NULL rectangle pointer.
	//   OM_ERR_INVALID_OP_CODEC -- This codec doesn't support display rect
	//								(may not be video essence)
 	//@comm Replaces omfmGetDisplayRect

/****/

	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Possible Errors:<nl>
	//   OM_ERR_NULL_PARAM -- A NULL rectangle pointer.<nl>
	//   OM_ERR_INVALID_OP_CODEC -- This codec doesn't support display rect<nl>
	//   							(may not be video essence)
 	//@comm Replaces omfmGetSampledRect

/****/

	//@comm A video sample is one frame.
	//@comm Replaces omfmGetSampleCount

/****/

	//@comm This call will only return a single channel of essence from an interleaved
	// stream.
	//@comm A video sample is a frame.
	// Buflen is in bytes, and should be large enough to hold the samples
	// in the requested memory format.
	//@comm Replaces omfmReadDataSamples

/****/

	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmReadMultiSamples
	
/****/

	//@comm This function allows reading
	// video frames in pieces, for low-memory situations.  When enough lines
	// have been read to constitute a frame, then the number of samples read
	// be incremented by one.
	//@comm The buffer must be large enough to hold an entire line of video.
	//@comm This function works only for video essence.
	//@comm Replaces omfmReadDataLines

/****/

	//@comm Useful only on reading, you
	// can't seek aound while writing essence.
	//@comm An audio frame is one sample across all open channels.
	//@comm Replaces omfmGotoFrameNumber

/****/

	//@comm Any parameters not required may have the pointers set to NULL.
	//@comm Possible Errors:<nl>
	// Standard errors (see top of file).<nl>
	// OM_ERR_WRONG_MEDIATYPE -- Not video essence.<nl>
	//@comm Replaces omfmGetVideoInfo */
			
			
/****/

	//@comm Replaces omfmGetVideoInfoArray */

/****/

	//@comm Replaces omfmPutVideoInfoArray */

/****/

	//@comm Replaces omfmGetNumRepresentations */

/****/

	//@comm Replaces omfmGetRepresentationSourceClip */

/****/

	//@comm Any parameters not required may have the pointers set to NULL.
	//@comm Possible Errors:<nl>
	//	Standard errors (see top of file).<nl>
	//	OM_ERR_WRONG_MEDIATYPE -- Not video essence.
	//@comm Replaces omfmGetAudioInfo */


/****/

	//@comm Replaces omfmGetAudioInfoArray */

/****/


/****/

	//@comm That is, does
	// it contain calls to a particular hardware device which speeds
	// up transfer and compression/decompression.
	//@comm If the hardware is not present in the system, this call should
	// return FALSE.
	//@comm If the parameters in the essence descriptor are out-of-bounds
	// for the hardware, then this call will return FALSE.
	//@comm Replaces omfmIsHardwareAssistedCodec */

/****/

	//@comm No other call uses this name, so it may be fully descriptive, esp. of limitations.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Possible Errors:<nl>
	// 	Standard errors (see top of file).<nl>
	//	OM_ERR_CODEC_INVALID - The given codec ID is not loaded.
	//@comm Replaces omfmCodecGetName */
	//@devnote Shouldn't we use a unicode aafString_t since AAF's public interface for
	// strings is supposed to be unicode?(TomR)

/****/

	//@comm This will be required in order to send private data to the codec.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Replaces omfmMediaGetCodecID */

/****/

	//@comm This function should NOT be called when essence is passed to
	//the reference implementation in an uncompressed format.
	//@comm Possible Errors:<nl>
	//	Standard errors (see top of file).<nl>
	//	OM_ERR_INVALID_OP_CODEC -- This kind of essence doesn't have a frame index<nl>
	//	OM_ERR_MEDIA_OPENMODE -- The essence is open for read-only.
	//@comm Replaces omfmAddFrameIndexEntry */

/****/

	//@comm Replaces omfmSetStreamCacheSize */

/****/

	//@comm Replaces omfmSourceGetVideoSignalType */








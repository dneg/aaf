// @doc INTERNAL
// @com This file implements the module test for CAAFEssenceAccess
/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/

/************************************************\
*											    *
* Advanced Authoring Format					    *
*										        *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*										        *
\************************************************/











#include "AAFTypes.h" //Use #include "AAF.h" for functional module test.
#include "AAFResult.h"


extern "C" HRESULT CAAFEssenceAccess_test()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}




	//@comm An example of a flavour would be a number of standard starting JPEG tables.

/****/

	// A single video frame is ONE sample.
	// Buflen must be large enough to hold the total sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// AAFRESULT_SINGLE_CHANNEL_OP -- Tried to write to an interleaved stream.
	// AAFRESULT_BADDATAADDRESS -- The buffer must not be a NULL pointer.


/****/

	//@comm arrayElemCount is the number of elementss in the array of transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.  Some of the fields in the xferArray structure are status results
	// like bytesXferred and samplesXferred.

/****/

	//@comm Writes fractional samples of essence to the stream. When enough bytes
	// have been written to constitute one sample, then the number of samples will
	// be incremented by one. This function is useful in low-memory situations.  
	//@comm Possible Errors:
	// Standard errors (see top of file).
	
/****/

	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	// Add illegal media kind error.


/****/

	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	AAFRESULT_NULL_PARAM -- A return parameter was NULL.

/****/

	//@comm This is the format expected on writes and produced on reads.
	//@comm On writes, the data will be written in this format, except
	// where a software codec may be used.
	// On reads, the data will be translated to this format.
	//@comm The order of the parameters does matter, as transformations will be applied in
	// that order to get from the on-disk format to the in-memory format.
	 
/****/

	//@comm A video sample is one frame.
	//Invalid essence type

/****/

	//@comm This call will only return a single channel of essence from an interleaved
	// stream.
	//@comm A video sample is a frame.
	// Buflen is in bytes, and should be large enough to hold the samples
	// after the requested transforms have been applied.
	//@comm Possible Errors:
	// 	Standard errors \\\(see top of file\\\).
	// 	AAFRESULT_END_OF_ESSENCE -- Hit the end of the essence \\\(like EOF\\\) while reading.
	// 	AAFRESULT_SMALLBUF -- Buffer too small for samples.



/****/

// The multiXfer_t structure has the following fields\, which specify one channel of data:
//		essenceDef		[IN] -- The essence type definition
//		physical		[IN] - The physical input-output channel
//		numSamples		[IN] -- The number of samples to transfer
//		buflen			[IN] -- The size of the buffer
//		buffer			[IN] -- The buffer for this
// The multiResult_t structure has the following fields\, which return result for one channel of data:
//		bytesXfered		[OUT] -- The total number of bytes transferred
//		samplesXfered	[OUT] -- The total number of samples transferred
	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.  Some of the fields in the xferArray structure are status results
	// like bytesXferred and samplesXferred.
	//@comm Possible Errors:
	// 	Standard errors \(see top of file\).
	// 	AAFRESULT_END_OF_ESSENCE -- Hit the end of the essence \(like EOF\) while reading.
	



/****/

	//@comm This function allows reading
	// video frames in pieces, for low-memory situations.
	//@comm The buffer must be large enough to hold the specified number of bytes.
	//@comm Possible Errors:
	// 	Standard errors \\\(see top of file\\\).
	// 	AAFRESULT_END_OF_ESSENCE -- Hit the end of the essence \\\(like EOF\\\) while reading.


/****/

	//@comm Useful only on reading, you can't seek aound while writing essence.
	//@comm An audio frame is one sample across all channels.
	// 	AAFRESULT_END_OF_ESSENCE -- Hit the end of the essence \\\(like EOF\\\) while reading.


/****/


/****/


/****/


/****/





/****/

	//@comm No other call uses this name, so it may be fully descriptive, esp. of limitations.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Possible Errors:<nl>
	// 	Standard errors (see top of file).<nl>
	//	AAFRESULT_CODEC_INVALID - The given codec ID is not loaded.

/****/

	//@comm This will be required in order to send private data to the codec.
	//@comm The name will be truncated to fit within "buflen" bytes.

/****/

	//@comm This function should NOT be called when essence is passed to
	//the reference implementation in an uncompressed format.
	//@comm Possible Errors:<nl>
	//	Standard errors (see top of file).<nl>
	//	AAFRESULT_INVALID_OP_CODEC -- This kind of essence doesn't have a frame index<nl>
	//	AAFRESULT_MEDIA_OPENMODE -- The essence is open for read-only.

/****/


/****/

	//@comm Allocating the space in this fashion ensures that the data will be
	// contiguous on disk (for at least numBytes bytes) even if other
	// disk operations allocate space on the disk.  If the data written
	// exceeds numBytes, then another disk block of numBytes size will be
	// allocated.
	//@comm Takes a essence handle, so the essence must have been opened or created.
	// The space is allocated in terms of bytes.

/****/

	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold
	// nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// AAFRESULT_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	
/****/

	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// AAFRESULT_BADDATAADDRESS -- The buffer must not be a NULL pointer.

/****/

	//@comm Must be called before releasing a write essence access.
	// Standard errors (see top of file).




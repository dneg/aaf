//@doc
//@class    AAFEssenceAccess | Implementation class for AAFEssenceAccess
#ifndef __ImplAAFEssenceAccess_h__
#define __ImplAAFEssenceAccess_h__


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


class ImplAAFDataDef;

class ImplAAFMasterMob;

class ImplAAFSourceMob;

class ImplAAFSourceClip;

class ImplAAFEssenceFormat;

class ImplAAFEssenceStream;

class ImplAAFFileDescriptor;

class ImplAAFLocator;


#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __AAFPlugin_h__
#include "AAFPlugin.h"
#endif

typedef struct
{
//	aafPosition_t	dataOffset;
	aafUID_t		mediaKind;
	aafInt32		trackID;
	aafInt16		physicalOutChan;	/* 1->N */
//	aafRational_t	sampleRate;
//	aafLength_t		numSamples;
}               aafSubChannel_t;

typedef enum { kAAFCreated, kAAFAppended, kAAFReadOnly } aafOpenType_t;


class ImplAAFEssenceAccess : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEssenceAccess ();

protected:
  virtual ~ImplAAFEssenceAccess ();

public:

/****/
  //****************
  // CreateAudioEssence()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateAudioEssence
        (// @parm [in] on this master mob
         ImplAAFMasterMob * masterMob,

         // @parm [in] 
         aafSlotID_t  masterSlotID,

         // @parm [in] and this file mob create audio
         ImplAAFSourceMob * fileMob,

         // @parm [in] with this sample rate
         aafRational_t  samplerate,

         // @parm [in] optionally compressing it
         aafCompressEnable_t  Enable,

         // @parm [in] with this sample size
         aafInt16  sampleSize,

         // @parm [in] and this many channels
         aafInt16  obj);
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
  //****************
  // VideoMediaCreate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    VideoMediaCreate
        (// @parm [in] on this master mob
         ImplAAFMasterMob * masterMob,

         // @parm [in] masterSlotID
         aafSlotID_t  masterSlotID,

         // @parm [in] and this file mob create video
         ImplAAFSourceMob * fileMob,

         // @parm [in] optionally compressing it
         aafCompressEnable_t  Enable,

         // @parm [in] editrate
         aafRational_t  editrate,

         // @parm [in] with this height
         aafUInt32  StoredHeight,

         // @parm [in] and this width
         aafUInt32  StoredWidth,

         // @parm [in] and this frame layout.
         aafFrameLayout_t  layout,

         // @parm [in] image aspect ratio
         aafRational_t  ratio);
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
  virtual AAFRESULT STDMETHODCALLTYPE
	SetEssenceDestination(
				ImplAAFLocator		*destination,
				aafFileFormat_t		fileFormat);

 //Sets which variety of the codec ID is to be used.)
  virtual AAFRESULT STDMETHODCALLTYPE
	SetEssenceCodecVariety(aafUID_t variety);

  //****************
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
        (// @parm [in] on this master mob
         ImplAAFMasterMob * masterMob,

         // @parm [in] 
         aafSlotID_t  masterSlotID,

         // @parm [in] create essence of this type
         aafUID_t	mediaKind,

 		 aafUID_t			codecID,
		 aafRational_t	editRate,
		 aafRational_t	sampleRate,

         // @parm [in] optionally compressing it
         aafCompressEnable_t  Enable);
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
  //****************
  // MultiCreate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    MultiCreate
        (// @parm [in] on this master mob
         ImplAAFMasterMob * masterMob,

 							aafUID_t codecID,
                          aafInt16  /*arrayElemCount*/,
                           aafmMultiCreate_t *  /*mediaArray*/,
                           aafCompressEnable_t  /*Enable*/);
	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	// or WriteDataLines.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	//@comm Replaces omfmMediaMultiCreate
	
/****/
  //****************
  // SetBlockingSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBlockingSize
        // @parm [in] Preallocate this many bytes
        (aafInt32  numBytes);
	//@comm Allocating the space in this fashion ensures that the data will be
	// contiguous on disk (for at least numBytes bytes) even if other
	// disk operations allocate space on the disk.  If the data written
	// exceeds numBytes, then another disk block of numBytes size will be
	// allocated.
	//@comm Takes a essence handle, so the essence must have been opened or created.
	// The space is allocated in terms of bytes.
	//@comm Replaces omfmSetBlockingSize

/****/
  //****************
  // SetVideoLineMap()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetVideoLineMap
        (// @parm [in] set this starting video line
         aafInt16  startLine,

         // @parm [in] and this top field
         aafFieldTop_t  type);
	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces omfmSetVideoLineMap.

/****/
  //****************
  // GetVideoTopField()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetVideoTopField
        // @parm [out] and this top field
        (aafFieldTop_t *  type);
	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces omfmGetVideoTopField
	
/****/
  //****************
  // SetDisplayRect()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDisplayRect
        // @parm [in,ref] set display rectangle
        (aafRect_t *  DisplayRect);
	//@comm Handles the case where the displayed rectangle is not the same as
	// the stored rectangle (as with the old leadingLines and trailingLines).
	// A positive "leadingLines" (from 1.5) becomes a positive yOffset, and
	// decreases the display height.
	// A positive "trailingLines" (from 1.5) also decreases the display height.
	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces  omfmSetDisplayRect

/****/
  //****************
  // SetSampledRect()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSampledRect
        // @parm [in,ref] set sampled rectangle
        (aafRect_t *  SampledRect);
	//@comm Takes a essence handle, so the essence must have been opened or created
	//@comm Replaces omfmSetSampledRect
	
/****/
  //****************
  // WriteMultiSamples()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteMultiSamples
        (// @parm [in] Do this many transfers
         aafInt16  arrayElemCount,

         // @parm [out,size_is(arrayElemCount)] referencing this array
         aafmMultiXfer_t *  xferArray);
	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmWriteMultiSamples

/****/
  //****************
  // WriteDataSamples()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteDataSamples
        (// @parm [in] write this many samples
         aafInt32  nSamples,

         // @parm [in,size_is(buflen)] to a buffer
         aafDataBuffer_t  buffer,

         // @parm [in] of this size
         aafInt32  buflen);
	//@comm Takes a essence handle, so the essence must have been opened or created.
	// A single video frame is ONE sample.
	// Buflen must be large enough to hold nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_SINGLE_CHANNEL_OP -- Tried to write to an interleaved stream.
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmWriteDataSamples

/****/
  //****************
  // WriteRawData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteRawData
        (// @parm [in] write this many samples
         aafInt32  nSamples,

         // @parm [in, size_is(nSamples * sampleSize)] to a buffer
         aafDataBuffer_t  buffer,

         // @parm [in] of this size
         aafInt32  sampleSize);
	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold
	// nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmWriteRawData
	
/****/
  //****************
  // ReadRawData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadRawData
        (// @parm [in] write this many samples
         aafInt32  nSamples,

         // @parm [in] to a buffer of this size
         aafUInt32  buflen,

         // @parm [out, size_is(buflen), length_is(*bytesRead)] here is the buffer
         aafDataBuffer_t  buffer,

         // @parm [out,ref] 
         aafUInt32 *  bytesRead,

         // @parm [out,ref] 
         aafUInt32 *  samplesRead);
	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmReadRawData
	
/****/
  //****************
  // WriteDataLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteDataLines
        (// @parm [in] write this many lines on video
         aafUInt32  nLines,

         // @parm [in] 
         aafUInt32  nBytesPerLine,

         // @parm [in, size_is(nLines * nBytesPerLine)] from a buffer
         aafDataBuffer_t  buffer,

         // @parm [out,ref] of this size
         aafInt32 *  bytesWritten);
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
  //****************
  // Close()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Close ();

	//@comm This function should be called whether the essence was opened or created.
	//@comm Replaces omfmMediaClose

/****/
  //****************
  // SetJPEGTables()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetJPEGTables
        (// @parm [in] change this component
         aafJPEGcomponent_t  JPEGcomp,

         // @parm [in] to have these Q-tables
         aafUInt8 *  QTables,

         // @parm [in] and these AC-tables
         aafUInt8 *  ACTables,

         // @parm [in] and these DC-tables
         aafUInt8 *  DCTables,

         // @parm [in] size in bytes
         aafInt16  QTableSize,

         // @parm [in] size in bytes
         aafInt16  ACTableSize,

         // @parm [in] size in bytes
         aafInt16  DCTableSize);
	//@comm This function needs to be called once for each component of the video.
	//@comm Replaces omfmSetJPEGTables
	
/****/
  //****************
  // SetAudioBlockLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAudioBlockLength
        // @parm [in] the block length
        (aafInt16  blockLength);
	//@comm Replaces omfmSetAudioCompressParms
/****/
  //****************
  // GetAudioBlockLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAudioBlockLength
        // @parm [out] the block length
        (aafInt16*  blockLength);
	//@comm Replaces omfmGetAudioCompressParms
	
/****/
  //****************
  // SetPrivateMediaData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPrivateMediaData
        (// @parm [in] a block of this size
         aafUInt32  parmBlockSize,

         // @parm [in,size_is(parmBlockSize)] with these values
         aafDataBuffer_t  ParameterBlock);
	//@parm IN -- .
	//@comm The parameter block should be defined in the
	// "h" file of the codec, and must be included by the application in
	// order to use this call.
	//@comm NOTE: All CODECs should default to reasonable parameters,
	// in case the application doesn't know about a given codec.
	//@comm Replaces omfmCodecSendPrivateData*/

/****/
  //****************
  // GetPrivateMediaData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPrivateMediaData
        (// @parm [in] 
         aafUInt32  blocksize,

         // @parm [out, size_is(blocksize)] 
         aafDataBuffer_t  buffer);
	//@comm The parameter block should be defined in the
	// "h" file of the codec, and must be included by the application in
	// order to use this call.
	//@comm NOTE: All CODECs should default to reasonable parameters,
	// in case the application doesn't know about a given codec.
	//@comm Replaces omfmGetPrivateMediaData*/
	//@devnote Added bytesRead 

/****/
  //****************
  // GetNumChannels()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumChannels
        (// @parm [in] In this master mob
         ImplAAFMasterMob * masterMob,

         // @parm [in] On this slot
         aafSlotID_t  slotID,

         // @parm [in] using this essence criteria
         aafMediaCriteria_t*  mediaCrit,

         // @parm [in] for this essence type
         ImplAAFDataDef * mediaKind,

         // @parm [out] How many channels?
         aafInt16*  numCh);
	//@comm Returns the number of interleaved essence channels of a given type in the essence stream referenced by the given file mob
	//@comm If the data format is not interleaved, then the answer will
	// always be zero or one.  This function correctly returns zero
	// for essence types not handled by a given codec, and handles codecs
	// which work with multiple essence types.
	//@comm Replaces omfmGetNumChannels*/

/****/
  //****************
  // GetLargestSampleSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLargestSampleSize
        (// @parm [in] and this essence type
         ImplAAFDataDef * mediaKind,

         // @parm [out] the largest sample size
         aafInt32*  maxSize);
	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Replaces omfmGetLargestSampleSize*/

/****/
  //****************
  // GetSampleFrameSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleFrameSize
        (// @parm [in] and this essence type
         ImplAAFDataDef * mediaKind,

         // @parm [in] for this [1-based] sample frame number
         aafPosition_t  frameNum,

         // @parm [out] How big is the sample frame?
         aafLength_t*  frameSize);
	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	OM_ERR_NULL_PARAM -- A return parameter was NULL.
	//@comm Replaces omfmGetSampleFrameSize*/

/****/
  //****************
  // Open()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Open
        (// @parm [in] In this master mob
         ImplAAFMasterMob * masterMob,

         // @parm [in] On this slot
         aafSlotID_t  slotID,

         // @parm [in] using this essence criteria
         aafMediaCriteria_t*  mediaCrit,

         // @parm [in] ReadOnly or Append
         aafMediaOpenMode_t  openMode,

         // @parm [in] optionally decompressing
         aafCompressEnable_t  compEnable);
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
  //****************
  // MultiOpen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    MultiOpen
        (// @parm [in] In this master mob
         ImplAAFMasterMob * masterMob,

         // @parm [in] On this slot
         aafSlotID_t  slotID,

         // @parm [in] using this essence criteria
         aafMediaCriteria_t*  mediaCrit,

         // @parm [in] ReadOnly or Append
         aafMediaOpenMode_t  openMode,

         // @parm [in] optionally decompressing
         aafCompressEnable_t  compEnable);
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
  //****************
  // SetVideoMemFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetVideoMemFormat
        // @parm [in] 
        (ImplAAFEssenceFormat * op);
	//@comm This is the format expected on writes and produced on reads.
	//@comm On writes, the data will be written in this format, except
	// where a software codec may be used.
	// On reads, the data will be translated to this format.
	//@comm The current CODECs should support rgb888 and YUV as formats
	// and all of the standard layouts.  A special format of
	// kVmFmtStd says to use the file's native format & layout.
	//@comm Replaces omfmSetVideoMemFormat*/

/****/
  //****************
  // SetAudioMemFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAudioMemFormat
        // @parm [in] 
        (ImplAAFEssenceFormat * op);
	//@comm This is the format expected on writes and produced on reads.
	//@comm On writes, the data will be written in this format, except
	// where a software codec may be used.
	// On reads, the data will be translated to this format.
	//@comm The current CODECs should support different sample sizes and rates
	// A special format of kAmFmtStd says to use the file's native
	// size and rate.
 	//@comm Replaces omfmSetAudioMemFormat

/****/
  //****************
  // GetDisplayRect()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDisplayRect
        // @parm [out] Get the display rectangle
        (aafRect_t*  result);
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
  //****************
  // GetSampledRect()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampledRect
        // @parm [out] Get the sampled rectangle
        (aafRect_t*  result);
	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Possible Errors:<nl>
	//   OM_ERR_NULL_PARAM -- A NULL rectangle pointer.<nl>
	//   OM_ERR_INVALID_OP_CODEC -- This codec doesn't support display rect<nl>
	//   							(may not be video essence)
 	//@comm Replaces omfmGetSampledRect

/****/
  //****************
  // GetSampleCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleCount
        // @parm [out] 
        (aafLength_t *  result);
	//@comm A video sample is one frame.
	//@comm Replaces omfmGetSampleCount

/****/
  //****************
  // ReadDataSamples()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadDataSamples
        (// @parm [in] 
         aafInt32  nSamples,

         // @parm [in] 
         aafInt32  buflen,

         // @parm [out, size_is(buflen),length_is(*bytesRead)] 
         aafDataBuffer_t  buffer,

         // @parm [out,ref] 
         aafUInt32*  bytesRead);
	//@comm This call will only return a single channel of essence from an interleaved
	// stream.
	//@comm A video sample is a frame.
	// Buflen is in bytes, and should be large enough to hold the samples
	// in the requested memory format.
	//@comm Replaces omfmReadDataSamples

/****/
  //****************
  // ReadMultiSamples()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadMultiSamples
        (// @parm [in] 
         aafInt16  elemCount,

         // @parm [out, size_is(elemCount)] 
         aafmMultiXfer_t *  xferArray);
	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmReadMultiSamples
	
/****/
  //****************
  // ReadDataLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadDataLines
        (// @parm [in] 
         aafInt32  nLines,

         // @parm [in] 
         aafInt32  bufLen,

         // @parm [out, size_is(bufLen),length_is(*bytesRead)] 
         aafDataBuffer_t  buffer,

         // @parm [out,ref] 
         aafInt32*  bytesRead);
	//@comm This function allows reading
	// video frames in pieces, for low-memory situations.  When enough lines
	// have been read to constitute a frame, then the number of samples read
	// be incremented by one.
	//@comm The buffer must be large enough to hold an entire line of video.
	//@comm This function works only for video essence.
	//@comm Replaces omfmReadDataLines

/****/
  //****************
  // GotoFrameNumber()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GotoFrameNumber
        // @parm [in] 
        (aafInt64  frameNum);
	//@comm Useful only on reading, you
	// can't seek aound while writing essence.
	//@comm An audio frame is one sample across all open channels.
	//@comm Replaces omfmGotoFrameNumber

/****/
  //****************
  // GetVideoInfo()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetVideoInfo
        (// @parm [out] 
         aafFrameLayout_t *  layout,

         // @parm [out] 
         aafInt32 *  fieldWidth,

         // @parm [out] 
         aafInt32 *  fieldHeight,

         // @parm [out] 
         aafRational_t *  editrate,

         // @parm [out] 
         aafInt16 *  bitsPerPixel,

         // @parm [out] 
         aafPixelFormat_t *  defaultPixelFmt);
	//@comm Any parameters not required may have the pointers set to NULL.
	//@comm Possible Errors:<nl>
	// Standard errors (see top of file).<nl>
	// OM_ERR_WRONG_MEDIATYPE -- Not video essence.<nl>
	//@comm Replaces omfmGetVideoInfo */
			
			
/****/
  //****************
  // GetVideoInfoArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetVideoInfoArray
        // @parm [out] 
        (ImplAAFEssenceFormat ** ops);
	//@comm Replaces omfmGetVideoInfoArray */

/****/
  //****************
  // PutVideoInfoArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PutVideoInfoArray
        // @parm [in] 
        (ImplAAFEssenceFormat * ops);
	//@comm Replaces omfmPutVideoInfoArray */

/****/
  //****************
  // GetNumRepresentations()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumRepresentations
        (// @parm [in] 
         ImplAAFMasterMob * masterMob,

         // @parm [in] 
         aafSlotID_t  slotID,

         // @parm [out] 
         aafInt32*  numReps);
	//@comm Replaces omfmGetNumRepresentations */

/****/
  //****************
  // GetRepresentationSourceClip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRepresentationSourceClip
        (// @parm [in] 
         ImplAAFMasterMob * masterMob,

         // @parm [in] 
         aafSlotID_t  slotID,

         // @parm [in] 
         aafInt32  index,

         // @parm [out] 
         ImplAAFSourceClip ** sourceClip);
	//@comm Replaces omfmGetRepresentationSourceClip */

/****/
  //****************
  // GetAudioInfo()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAudioInfo
        (// @parm [out] 
         aafRational_t*  rate,

         // @parm [out] 
         aafInt32*  sampleSize,

         // @parm [out] 
         aafInt32*  numChannels);
	//@comm Any parameters not required may have the pointers set to NULL.
	//@comm Possible Errors:<nl>
	//	Standard errors (see top of file).<nl>
	//	OM_ERR_WRONG_MEDIATYPE -- Not video essence.
	//@comm Replaces omfmGetAudioInfo */


/****/
  //****************
  // GetAudioInfoArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAudioInfoArray
        // @parm [out] 
        (ImplAAFEssenceFormat ** ops);
	//@comm Replaces omfmGetAudioInfoArray */

/****/
  //****************
  // PutAudioInfoArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PutAudioInfoArray
        // @parm [in] 
        (ImplAAFEssenceFormat * ops);

/****/
  //****************
  // IsHardwareAssistedCodec()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsHardwareAssistedCodec
        (// @parm [in] 
         aafCodecID_t  codecID,

         // @parm [in] 
         ImplAAFSourceMob * mob,

         // @parm [out] 
         aafBool*  result);
	//@comm That is, does
	// it contain calls to a particular hardware device which speeds
	// up transfer and compression/decompression.
	//@comm If the hardware is not present in the system, this call should
	// return FALSE.
	//@comm If the parameters in the essence descriptor are out-of-bounds
	// for the hardware, then this call will return FALSE.
	//@comm Replaces omfmIsHardwareAssistedCodec */

/****/
  //****************
  // GetCodecName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCodecName
        (// @parm [in] 
         aafCodecID_t  codecID,

         // @parm [in] 
         aafInt32  namelen,

         // @parm [out,size_is(namelen)] 
         aafUInt8 *  name);
	//@comm No other call uses this name, so it may be fully descriptive, esp. of limitations.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Possible Errors:<nl>
	// 	Standard errors (see top of file).<nl>
	//	OM_ERR_CODEC_INVALID - The given codec ID is not loaded.
	//@comm Replaces omfmCodecGetName */
	//@devnote Shouldn't we use a unicode aafString_t since AAF's public interface for
	// strings is supposed to be unicode?(TomR)

/****/
  //****************
  // GetCodecID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCodecID
        // @parm [out] 
        (aafCodecID_t*  codecID);
	//@comm This will be required in order to send private data to the codec.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Replaces omfmMediaGetCodecID */

/****/
  //****************
  // AddFrameIndexEntry()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddFrameIndexEntry
        // @parm [in] add a frame offset to it's frame index
        (aafInt64  frameOffset);
	//@comm This function should NOT be called when essence is passed to
	//the reference implementation in an uncompressed format.
	//@comm Possible Errors:<nl>
	//	Standard errors (see top of file).<nl>
	//	OM_ERR_INVALID_OP_CODEC -- This kind of essence doesn't have a frame index<nl>
	//	OM_ERR_MEDIA_OPENMODE -- The essence is open for read-only.
	//@comm Replaces omfmAddFrameIndexEntry */

/****/
  //****************
  // SetStreamCacheSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStreamCacheSize
        // @parm [in] make the cache this many bytes long
        (aafUInt32  cacheSize);
	//@comm Replaces omfmSetStreamCacheSize */

/****/
  //****************
  // SourceGetVideoSignalType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SourceGetVideoSignalType
        // @parm [out] 
        (aafVideoSignalType_t*  signalType);
	//@comm Replaces omfmSourceGetVideoSignalType */


  //***********************************************************
  // METHOD NAME: GetStoredByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetStoredByteOrder |
  // Returns the "Endian-ness" in which the current object was or will
  // be stored.  If this is a transient object (i.e., one which has
  // not been persisted) then it will return the native byte order of
  // the platform on which this is running.
  // 
  // Succeeds if all of the following are true:
  // - the pOrder pointer is valid.
  // 
  // If this method fails nothing is written to *pOrder.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pOrder is null.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetStoredByteOrder (
    // @parm [out] eAAFByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    eAAFByteOrder_t *  pOrder
  );



  //***********************************************************
  // METHOD NAME: GetNativeByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetNativeByteOrder |
  // Returns the native "Endian-ness" of the platform on which this is
  // running.
  // 
  // Succeeds if all of the following are true:
  // - the pOrder pointer is valid.
  // 
  // If this method fails nothing is written to *pOrder.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pOrder is null.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetNativeByteOrder (
    // @parm [out] eAAFByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    eAAFByteOrder_t *  pOrder
  );



public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFEssenceAccessTest.cpp.
  static AAFRESULT test();

private:
	aafUID_t			_codecID;
	aafUID_t			_variety;
	ImplAAFLocator		*_destination;
	aafFileFormat_t		_fileFormat;
	ImplAAFSourceMob	*_fileMob;
	aafInt32			_numChannels;
	aafSubChannel_t		*_channels;
	aafPosition_t		_dataStart;
	ImplAAFMasterMob	*_masterMob;
	ImplAAFFileDescriptor *_mdes;
	IAAFEssenceCodec	*_codec;
	IAAFEssenceStream	 *_stream;
	aafOpenType_t		_openType;
};

#endif // ! __ImplAAFEssenceAccess_h__


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

class ImplAAFMasterMob;
class ImplAAFSourceMob;
class ImplAAFSourceClip;
class ImplAAFEssenceFormat;
class ImplAAFEssenceStream;
class ImplAAFEssenceSampleStream;
class ImplAAFFileDescriptor;
class ImplAAFFile;
class ImplAAFLocator;
class ImplAAFContainerDef;
class ImplAAFPluginDescriptor;
class ImplAAFHeader;
class ImplAAFDictionary;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __AAFPlugin_h__
#include "AAFPlugin.h"
#endif

typedef struct
{
	aafUID_t		mediaKind;
	aafInt32		trackID;
	aafInt16		physicalOutChan;	/* 1->N */
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
  virtual AAFRESULT STDMETHODCALLTYPE
	SetEssenceDestination(
				ImplAAFLocator		*destination,
				aafUID_t			fileFormat);

 //Sets which variety of the codec ID is to be used.)
  virtual AAFRESULT STDMETHODCALLTYPE
	SetEssenceCodecVariety(aafUID_t variety);

  //****************
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
        (ImplAAFMasterMob *masterMob,
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
        (					ImplAAFMasterMob *masterMob,
 							aafUID_t codecID,
                          aafInt16  /*arrayElemCount*/,
                           aafmMultiCreate_t *  /*mediaArray*/,
                           aafCompressEnable_t  /*Enable*/t);
	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	// or WriteDataLines.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	//@comm Replaces omfmMediaMultiCreate

    //****************
  // Open()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Open
        (ImplAAFMasterMob *masterMob,
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
        (ImplAAFMasterMob *masterMob,
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
  // SetBlockingSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBlockingSize
        // @parm [in] Preallocate this many bytes
        (aafUInt32  numBytes);
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
    WriteSamples
        (// @parm [in] write this many samples
         aafUInt32  nSamples,

         // @parm [in,size_is(buflen)] to a buffer
         aafDataBuffer_t  buffer,

         // @parm [in] of this size
         aafUInt32  buflen);
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
         aafUInt32  nSamples,

         // @parm [in, size_is(nSamples * sampleSize)] to a buffer
         aafDataBuffer_t  buffer,

         // @parm [in] of this size
         aafUInt32  sampleSize);
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
         aafUInt32  nSamples,

         // @parm [in] to a buffer of this size
         aafUInt32  buflen,

         // @parm [out, size_is(buflen), length_is(*bytesRead)] here is the buffer
         aafDataBuffer_t  buffer,

         // @parm [out,ref] 
         aafUInt32 *  samplesRead,

         // @parm [out,ref] 
         aafUInt32 *  bytesRead);
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
    WriteFractionalSample
        (// @parm [in] 
         aafUInt32  nBytes,

         // @parm [in, size_is(nLines * nBytesPerLine)] from a buffer
         aafDataBuffer_t  buffer,

         // @parm [out,ref] of this size
         aafUInt32 *  bytesWritten);
	
/****/
  //****************
  // CompleteWrite()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CompleteWrite ();

	//@comm This function should be called whether the essence was opened for append or created.
	//@comm Replaces omfmMediaClose


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
         aafUID_t mediaKind,

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
         aafUID_t mediaKind,

         // @parm [out] the largest sample size
         aafUInt32*  maxSize);
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
         aafUID_t  mediaKind,

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

/****/
  //****************
  // SetTransformParameters()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTransformParameters
        // @parm [in] 
        (ImplAAFEssenceFormat * op);


/****/
  //****************
  // GetSampleCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleCount
        (// @parm [in] and this essence type
         aafUID_t mediaKind,

        // @parm [out] 
        aafLength_t *  result);
	//@comm A video sample is one frame.
	//@comm Replaces omfmGetSampleCount

/****/
  //****************
  // ReadDataSamples()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadSamples
        (// @parm [in] 
         aafUInt32  nSamples,

         // @parm [in] 
         aafUInt32  buflen,

         // @parm [out, size_is(buflen),length_is(*bytesRead)] 
         aafDataBuffer_t  buffer,

         // @parm [out,ref] 
         aafUInt32*  samplesRead,
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
    ReadFractionalSample
        (aafUInt32	nBytes,
		// @parm [in] 
         aafUInt32  bufLen,

         // @parm [out, size_is(bufLen),length_is(*bytesRead)] 
         aafDataBuffer_t  buffer,

         // @parm [out,ref] 
         aafUInt32*  bytesRead);

/****/
  //****************
  // GotoFrameNumber()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SeektoEditFrame
        // @parm [in] 
        (aafInt64  frameNum);
	//@comm Useful only on reading, you
	// can't seek aound while writing essence.
	//@comm An audio frame is one sample across all open channels.
	//@comm Replaces omfmGotoFrameNumber
			
/****/
  //****************
  // GetParameters()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileFormat
        (ImplAAFEssenceFormat * opsTemplate,
         ImplAAFEssenceFormat ** opsResult);
	//@comm Replaces omfmGetVideoInfoArray */

  //****************
  // GetParameterList()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileFormatParameterList
        // @parm [out] 
        (ImplAAFEssenceFormat ** ops);
  //****************
  // GetParameterList()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEmptyFileFormat
        // @parm [out] 
        (ImplAAFEssenceFormat ** ops);

/****/
  //****************
  // PutParameters()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PutFileFormat
        // @parm [in] 
        (ImplAAFEssenceFormat * ops);
	//@comm Replaces omfmPutVideoInfoArray */


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
         aafUInt32  namelen,

         // @parm [out,size_is(namelen)] 
         wchar_t *  name);
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

public:
	//Toolkit private functions
	AAFRESULT MakeAAFContainerDef(ImplAAFHeader *head, ImplAAFContainerDef **result);
	AAFRESULT CreateContainerDef (ImplAAFHeader *head);
	AAFRESULT CreateCodecDef(ImplAAFHeader *head, aafUID_t codecDef, IAAFPluginDescriptor **newDesc);
	AAFRESULT CreateEssenceFileFromLocator (ImplAAFHeader *srcHead, ImplAAFLocator *loc, ImplAAFFile **result);
	AAFRESULT ModifyEssenceFileFromLocator (ImplAAFHeader *srcHead, ImplAAFLocator *loc, ImplAAFFile **result);
	AAFRESULT CreateFileMob (ImplAAFHeader *newHead, aafSlotID_t slotID, aafUID_t *newMobID,
							aafUID_t mediaKind, aafRational_t editRate,aafRational_t sampleRate,
							ImplAAFLocator *addLocator, ImplAAFSourceMob **result);

private:
	aafUID_t				_codecID;
	aafUID_t				_variety;
	ImplAAFLocator			*_destination;
	aafUID_t				_fileFormat;
	ImplAAFSourceMob		*_compFileMob;
	aafInt32				_numChannels;
	aafSubChannel_t			*_channels;
	ImplAAFMasterMob		*_masterMob;
	ImplAAFFileDescriptor	*_mdes;
	IAAFEssenceCodec		*_codec;
	IAAFEssenceStream		*_stream;
	aafOpenType_t			_openType;
	IAAFPluginDescriptor	*_codecDescriptor;
	ImplAAFFile				*_dataFile;
	ImplAAFSourceMob		*_dataFileMob;
};

#endif // ! __ImplAAFEssenceAccess_h__


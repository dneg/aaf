//@doc
//@class    AAFEssenceAccess | Implementation class for AAFEssenceAccess
#ifndef __ImplAAFEssenceAccess_h__
#define __ImplAAFEssenceAccess_h__


//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

class ImplAAFContainerDef;
class ImplAAFDataDef;
class ImplAAFDictionary;
class ImplAAFEssenceFormat;
class ImplAAFEssenceSampleStream;
class ImplAAFEssenceStream;
class ImplAAFFile;
class ImplAAFFileDescriptor;
class ImplAAFHeader;
class ImplAAFLocator;
class ImplAAFMasterMob;
class ImplAAFPluginDef;
class ImplAAFSourceClip;
class ImplAAFSourceMob;
class ImplAAFEssenceSampleIndex;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __AAFPlugin_h__
#include "AAFPlugin.h"
#endif

#ifndef OMVECTOR_H
#include "OMVector.h"
#endif

#ifndef OMVECTORITERATOR_H
#include "OMVectorIterator.h"
#endif

typedef struct
{
  aafUID_t mediaKind;
  aafUInt32 trackID;
  aafUInt16 physicalOutChan;	/* 1->N */
} aafSubChannel_t;

typedef enum { kAAFCreated, kAAFAppended, kAAFReadOnly } aafOpenType_t;

#include <vector>
#include <iterator>

typedef struct _tagAccessor_t
{
	ImplAAFSourceMob 	    *fileMob;
	ImplAAFFileDescriptor	*mdes;
	IAAFEssenceCodec		*codec;
	IAAFMultiEssenceCodec *multicodec;
	IAAFEssenceData			 *internalEssenceData;
	IAAFEssenceStream		*stream;
	ImplAAFFile					 *dataFile;
	aafUID_t					 containerDefID;
	aafPosition_t				offset, pos;
	aafLength_t					length;
	bool operator ==(_tagAccessor_t accessor)
	{
		return 0 == memcmp(this, &accessor, sizeof(_tagAccessor_t));
	}
} aafAccessor_t;

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
	SetEssenceCodecFlavour(aafUID_t variety);

  //****************
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
        (ImplAAFMasterMob *masterMob,
		// @parm [in] 
		 aafSlotID_t masterSlotID,

         // @parm [in] create essence of this type
         const aafUID_t & mediaKind,

 		 const aafUID_t & codecID,
		 const aafRational_t & editRate,
		 const aafRational_t & sampleRate,

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
 							aafUID_constref codecID,
                          aafUInt16  /*arrayElemCount*/,
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
	
  //****************
  // Append()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Append
        (ImplAAFMasterMob *masterMob,
		// @parm [in] 
		 aafSlotID_t masterSlotID,

         // @parm [in] create essence of this type
         const aafUID_t & mediaKind,

 		 const aafUID_t & codecID,
		 const aafRational_t & editRate,
		 const aafRational_t & sampleRate,

         // @parm [in] optionally compressing it
         aafCompressEnable_t  Enable);
	//@comm Appends a single channel stream of essence.  Convenience functions
	// exist to create audio or video essence, and a separate call
	// (MultiAppend) exists to create interleaved audio and
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
  // MultiAppend()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    MultiAppend
        (					ImplAAFMasterMob *masterMob,
 							aafUID_constref codecID,
                          aafUInt16  /*arrayElemCount*/,
                           aafmMultiCreate_t *  /*mediaArray*/,
                           aafCompressEnable_t  /*Enable*/t);
	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	// or WriteDataLines.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	//@comm Replaces omfmMediaMultiCreate

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
  // WriteMultiSamples()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteMultiSamples
        (// @parm [in] Do this many transfers
         aafUInt16  arrayElemCount,

         // @parm [out,size_is(arrayElemCount)] referencing this array
         aafmMultiXfer_t *  xferArray, aafmMultiResult_t *resultArray);
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

         // @parm [in] of this size
         aafUInt32  buflen,

         // @parm [in,size_is(buflen)] to a buffer
         aafDataBuffer_t  buffer,

         // @parm [out] samples actually written
         aafUInt32 * samplesWritten,

         // @parm [out] bytes actually written
         aafUInt32 * bytesWritten);

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
  // CompleteWrite()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CompleteWrite ();

	//@comm This function should be called whether the essence was opened for append or created.
	//@comm Replaces omfmMediaClose


/****/
  //****************
  // CountChannels()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountChannels
        (// @parm [in] In this master mob
         ImplAAFMasterMob * masterMob,

         // @parm [in] On this slot
         aafSlotID_t  slotID,

         // @parm [in] using this essence criteria
         aafMediaCriteria_t*  mediaCrit,

         // @parm [in] for this essence type
         aafUID_t mediaKind,

         // @parm [out] How many channels?
         aafUInt16*  numCh);
	//@comm Returns the number of interleaved essence channels of a given type in the essence stream referenced by the given file mob
	//@comm If the data format is not interleaved, then the answer will
	// always be zero or one.  This function correctly returns zero
	// for essence types not handled by a given codec, and handles codecs
	// which work with multiple essence types.
	//@comm Replaces omfmGetNumChannels*/

  // INTERNAL to the SDK
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSelectInfo (ImplAAFSourceMob *fileMob,
                      aafSelectInfo_t* pSelectInfo);

/****/
  //****************
  // GetLargestSampleSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLargestSampleSize
        (// @parm [in] and this essence type
         ImplAAFDataDef * pMediaKind,

         // @parm [out] the largest sample size
         aafLength_t*  maxSize);
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
    GetIndexedSampleSize
        (// @parm [in] and this essence type
         ImplAAFDataDef *  pMediaKind,

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
  // CountSamples()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountSamples
        (// @parm [in] and this essence type
         ImplAAFDataDef * pMediaKind,

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
         aafUInt16  elemCount,

         // @parm [out, size_is(elemCount)] 
         aafmMultiXfer_t *  xferArray, aafmMultiResult_t *resultArray);
	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmReadMultiSamples
	
/****/
  //****************
  // GotoFrameNumber()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Seek
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



public:
	//Toolkit private functions
	AAFRESULT MakeAAFContainerDef(ImplAAFHeader *head, ImplAAFContainerDef **result);
	AAFRESULT CreateContainerDef (ImplAAFHeader *head);
	AAFRESULT CreateCodecDef(ImplAAFHeader *head,
							 const aafUID_t & codecDef,
							 IAAFPluginDef **newDesc);
	AAFRESULT CreateEssenceFileFromLocator (ImplAAFHeader *srcHead,
											ImplAAFLocator *loc,
											ImplAAFFile **result);
	AAFRESULT ModifyEssenceFileFromLocator (ImplAAFHeader *srcHead,
											ImplAAFLocator *loc,
											ImplAAFFile **result);
	AAFRESULT CreateFileMob (ImplAAFHeader *newHead,
							 aafBool addSlots,
							 aafSlotID_t slotID,
							 aafMobID_constptr newMobID, /* optional */
							 const aafUID_t & mediaKind,
							 const aafUID_t &	   codecID,
							 const aafRational_t & editRate,
							 const aafRational_t & sampleRate,
							 ImplAAFLocator *addLocator,
							 ImplAAFSourceMob **result);

  AAFRESULT InstallEssenceAccessIntoCodec();

private:
	aafUID_t				_codecID;
	aafUID_t				_flavour;
	ImplAAFLocator			*_destination;
	aafUID_t				_containerDefID;
	ImplAAFSourceMob		*_compFileMob;
	aafUInt32				_numChannels;
	aafSubChannel_t			*_channels;
	ImplAAFMasterMob		*_masterMob;
	ImplAAFFileDescriptor	*_mdes;
	IAAFEssenceCodec		*_codec;
  IAAFMultiEssenceCodec *_multicodec;
  IAAFEssenceData *_internalEssenceData;
	IAAFEssenceStream		*_stream;
	aafOpenType_t			_openType;
	IAAFPluginDef		*_codecDescriptor;
	ImplAAFFile				*_dataFile;
	ImplAAFSourceMob		*_dataFileMob;
	OMVector<aafAccessor_t> _codecList;
	OMVectorIterator<aafAccessor_t> _cur;
};

#endif // ! __ImplAAFEssenceAccess_h__


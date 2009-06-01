//@doc
//@class    AAFEssenceCodec | Implementation class for AAFEssenceCodec
#ifndef __CAAFWAVECodec_h__
#define __CAAFWAVECodec_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAF.h"
#include "AAFPlugin.h"
#include "CAAFUnknown.h"
#include "AAFPluginUtils.h"

EXTERN_C const CLSID CLSID_AAFWaveCodec;

typedef struct
{
	void			*buf;
	aafUInt32		buflen;
	aafUInt32		samplesLeft;
	aafUInt32		bytesXfered;
	aafmMultiXfer_t	*xfer;
} interleaveBuf_t;

class CAAFWaveCodec : 
  public IAAFMultiEssenceCodec,
  public IAAFEssenceCodec,
	public IAAFPlugin,
  public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFWaveCodec (IUnknown * pControllingUnknown);
  virtual ~CAAFWaveCodec ();

public:

		
  // Create a media data object, and attach the correct type of
			//EssenceDescriptor to the fileMob
  STDMETHOD (MultiCreate)
    (/*[in]*/ IAAFSourceMob *fileMob, // Create the essence attached to this file mob
     /*[in]*/ aafUID_constref flavour, // which flavour of the codec to use
     /*[in]*/ IAAFEssenceStream * stream, // Here is an essence stream for the data
     /*[in]*/ aafCompressEnable_t compEnable, // whether or not samples should be written compressed
     /*[in]*/ aafUInt32 numParms, // Here are the number of create parameters
     /*[in]*/ aafmMultiCreate_t *createParms); // Here are the create parameters

  // Open a media data object.
  STDMETHOD (MultiOpen)
    (/*[in]*/ IAAFSourceMob *fileMob, // Open the essence attached to this file mob
     /*[in]*/ aafMediaOpenMode_t  openMode, // In this mode
     /*[in]*/ IAAFEssenceStream * stream,  // Here is an essence stream with the raw data 
     /*[in]*/ aafCompressEnable_t compEnable); // whether or not samples should be read compressed

  // Write blocks from one or more buffers, interleaving if needed.
  STDMETHOD (WriteBlocks)
    (/*[in]*/ aafDeinterleave_t  inter, // Whether the material will be de-interleaved on read
     /*[in]*/ aafUInt16  xferBlockCount, // How many aafMultiXfer blocks follow
     /*[in]*/ aafmMultiXfer_t *  xferBlock, // One or more blocks containing buffer pointer and length 
     /*[in]*/ aafmMultiResult_t *  resultBlock);

  // Read blocks into one or more buffers, de-interleaving if needed.
  STDMETHOD (ReadBlocks)
    (/*[in]*/ aafDeinterleave_t  inter, // Whether the material will be de-interleaved on read
     /*[in]*/ aafUInt16  xferBlockCount, // How many aafmMultiXfer blocks follow
     /*[in]*/ aafmMultiXfer_t *  xferBlock, // One or more blocks containing buffer pointer and length 
     /*[in]*/ aafmMultiResult_t *  resultBlock);



  STDMETHOD (CountDefinitions)(aafUInt32 *pDefCount);
  STDMETHOD (GetIndexedDefinitionID)(aafUInt32 index, aafUID_t *result);
  STDMETHOD (GetPluginDescriptorID)(aafUID_t *result);
  STDMETHOD (GetEssenceDescriptorID)(aafUID_t *result);
  STDMETHOD (GetEssenceDataID)(aafUID_t *result);
  STDMETHOD (GetIndexedDefinitionObject)(aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def);
  STDMETHOD (CreateDescriptor)(IAAFDictionary *dict, IAAFPluginDef **desc);

  STDMETHOD (SetEssenceAccess)
    (/*[in]*/ IAAFEssenceAccess *access); // Set the AAFEssenceAccess used as a factory for AAFEssenceFormat

 STDMETHOD (CountFlavours)
    (/*[in]*/ aafUInt32  *pCount);

	// Some codecs have variants handled by a single codec.
	// (For example, the Avid AVR codec handles multiple AVRs.)
	// The number of variants is returned by GetMetaInfo, and cached
	// by the AAFPluginManager.
	//
  STDMETHOD (GetIndexedFlavourID)
    (/*[in]*/ aafUInt32  index, // Which flavour to get the ID for
     /*[out]*/ aafUID_t *  pVariant); // The returned flavour ID 

  STDMETHOD (CountDataDefinitions)
        (/* [out] */ aafUInt32 *pCount);

			// // All codecs handle at least one kind of media (picture, sound, control)
	// but some handle more than one.  The kind of media is specified by an
	// AAFDataDefinition.  The numnber of data definitions is returned by
	// GetMetaInfo, and cached by the AAFPluginManager.
  STDMETHOD (GetIndexedDataDefinition)
    (/*[in]*/ aafUInt32  index, // Which data definition to get the ID for
     /*[out]*/ aafUID_t * pVariant); // The returned dataDefinition 

  STDMETHOD (GetMaxCodecDisplayNameLength)
    (aafUInt32  *bufSize);

	// Given a flavour ID, return the human readable name
  STDMETHOD (GetCodecDisplayName)
    (/*[in]*/ aafUID_constref flavour, // which flavour of the codec to use
     /*[in,string]*/ aafCharacter *  pName, // Human-readable name of the flavour
     /*[in]*/ aafUInt32  bufSize); // length of the buffer to hold flavour Name 
	
  // Returns the number of channels which this codec can handle
			// of the given essence kind
  STDMETHOD (CountChannels)
    (/*[in]*/ IAAFSourceMob *fileMob, // Get the number of processable channels on this file mob
     /*[in]*/ aafUID_constref essenceKind, // This is the type of essence to open
	 IAAFEssenceStream *stream,
     /*[out]*/ aafUInt16 *  pNumChannels); // The number of channels present 

  STDMETHOD (GetSelectInfo)
   (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
        aafSelectInfo_t *  pSelectInfo);

  // Returns the number of samples which this codec can find on the
			// given slot.
  STDMETHOD (CountSamples)
     (/*[in]*/ aafUID_constref essenceKind, // This is the type of essence to check
	/*[out]*/ aafLength_t *  pNumSamples); // The number of samples present of that type

	 
  // Returns a (possibly zero-length) string listing any problems
			//found with the essence, or the relationship between variables
			// in the essence descriptor, and any such values contained within
			// the essence data
  STDMETHOD (ValidateEssence)
    (/*[in]*/ IAAFSourceMob *fileMob, // Run a check on this file mob
         IAAFEssenceStream *stream,
    /*[in]*/ aafCheckVerbose_t  verbose, // This is the verbosity level of the output
     /*[out]*/ aafCheckWarnings_t warning, // This determines whether the output contains warnings
         aafUInt32  bufSize,
		wchar_t *  pName,
        aafUInt32  *bytesWritten);
		
  // Create a media data object, and attach the correct type of
			//EssenceDescriptor to the fileMob
  STDMETHOD (Create)
    (/*[in]*/ IAAFSourceMob *fileMob, // Create the essence attached to this file mob
     /*[in]*/ aafUID_constref flavour, // which flavour of the codec to use
     /*[in]*/ aafUID_constref essenceKind, // This is the type of essence to create
     /*[in]*/ aafRational_constref sampleRate,
     /*[in]*/ IAAFEssenceStream * stream, // stream to write the data
     /*[in]*/ aafCompressEnable_t compEnable); // whether or not samples should be written compressed

  // Open a media data object.
  STDMETHOD (Open)
    (/*[in]*/ IAAFSourceMob *fileMob, // Open the essence attached to this file mob
     /*[in]*/ aafMediaOpenMode_t  openMode, // In this mode
     /*[in]*/ IAAFEssenceStream * stream, // Here is an essence stream with the raw data 
     /*[in]*/ aafCompressEnable_t compEnable); // whether or not samples should be read compressed
	

  STDMETHOD (WriteSamples) (
    /*[in]*/ aafUInt32  nSamples, // write this many samples
    /*[in]*/ aafUInt32  buflen,  // size of buffer
    /*[in,size_is(buflen)]*/ aafDataBuffer_t  buffer, // from a buffer
    /*[out]*/ aafUInt32 *samplesWritten, // samples actually written
    /*[out]*/ aafUInt32 *bytesWritten);  // bytes actually written

  STDMETHOD (ReadSamples) (  
    /*[in]*/ aafUInt32  nSamples, // Read this many samples
    /*[in]*/ aafUInt32  buflen, // into a buffer of this size
    /*[out]*/ aafDataBuffer_t  buffer, // The transfer buffer
    /*[out, ref]*/ aafUInt32 *  samplesRead, // The number of samples actually read
    /*[out, ref]*/ aafUInt32 *  bytesRead); // The number of bytes actually read

  // Seek to a particular sample frame on the media.  The
			// sample frame is one frame for picture, and one sample
			// times the number of interleaved channels for audio.
  STDMETHOD (Seek)
    (/*[in]*/ aafPosition_t  sampleFrame); // The sample frame to seek to. 

  // Close the media stream, ready to open another or Finish.
  STDMETHOD (CompleteWrite)
     (IAAFSourceMob *desc);


	
  // Given some raw essence (like a WAVE file), create
			// an AAFEssenceDescriptor to match, with all fields filled in.
  STDMETHOD (CreateDescriptorFromStream)
    (/*[in]*/ IAAFEssenceStream * pStream, // A raw file stream
     /*[in]*/ IAAFSourceMob *fileMob); // Put the finished descriptor on this source mob 



  // Return the current essence stream.
  STDMETHOD (GetCurrentEssenceStream)
    (/*[out]*/ IAAFEssenceStream ** ppStream); // The current essence stream 
		
  // Set the format of the data.
  STDMETHOD (PutEssenceFormat)
    (/*[in]*/ IAAFEssenceFormat * pFormat); // An essence format object 

  // Supply an essence format object specifying what data is
			// required, and it is filled in with the data values.
  STDMETHOD (GetEssenceFormat)
    (/*[in]*/ IAAFEssenceFormat *pTemplate, // An essence format template object 
    /*[out]*/ IAAFEssenceFormat **pResult); // An essence format result object 

  STDMETHOD (GetDefaultEssenceFormat)
    (/*[out]*/ IAAFEssenceFormat **pFormat); // An essence format result object 

  STDMETHOD (GetIndexedSampleSize)
    (aafUID_constref dataDefID,
	 aafPosition_t pos,
	 aafLength_t *pResult);

  STDMETHOD (GetLargestSampleSize)
    (aafUID_constref dataDefID,
	 aafLength_t *pResult);
		


  // Write some number of bytes to the stream exactly and with no formatting or compression.
  STDMETHOD (WriteRawData)
    (/*[in]*/ aafUInt32 nSamples,	//number of samples to read
	/*[in,size_is(buflen)]*/ aafDataBuffer_t  buffer, // to a buffer
     /*[in]*/ aafUInt32  buflen); // of this size 



  // Read some number of bytes from the stream exactly and with no formatting or compression.
  STDMETHOD (ReadRawData)
    (/*[in]*/ aafUInt32 nSamples,	//number of samples to read
	 /*[in]*/ aafUInt32  buflen, // to a buffer of this size
     /*[out, size_is(buflen), length_is(*bytesRead)]*/ aafDataBuffer_t  buffer, // here is the buffer
     /*[out,ref]*/ aafUInt32 *  bytesRead,	// Return bytes actually read 
     /*[out,ref]*/ aafUInt32 *  samplesRead); // Return samples actually read 


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********

  // Declare the module test method. The implementation of the will be be
  // in /test/CAAFEssenceCodecTest.cpp.
  static HRESULT test();

private:
	IAAFEssenceStream	*_stream;
	IAAFWAVEDescriptor	*_mdes;
	IAAFEssenceAccess	*_access;
	AAFByteOrder		_nativeByteOrder;
	aafRational_t		_sampleRate;
	aafUInt16			_bitsPerSample;
	aafUInt16			_numCh;
	aafUInt16			_bytesPerFrame;
	aafPosition_t		_sampleFrames;
	aafBoolean_t				_headerLoaded;
	interleaveBuf_t		*_interleaveBuf;
	aafInt64			_dataStartOffset;
	aafInt64			_dataSizeOffset;
	aafBoolean_t				_readOnly;
	aafBoolean_t				_sampleDataHeaderWritten;
	aafBoolean_t				_initialSeekPerformed;

	// Sets a value indicating whether the SDK is handling the compression.
	AAFRESULT SetCompressionEnabled(aafCompressEnable_t compEnable);
	AAFRESULT fillSwappedWAVEData(aafUInt8 **destBufHdl, aafUInt32 maxsize, void *data);
	AAFRESULT scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafUInt32 maxsize, void *data);
	AAFRESULT CreateWAVEheader(aafUInt8 *buffer, aafUInt32 bufsize, aafUInt16 numCh, aafUInt32 *hdrSize);
	AAFRESULT loadWAVEHeader(void);
	AAFRESULT GetWAVEData(aafUInt32 len, void *buf);

	AAFRESULT ComputeWriteChunkSize(aafInt64 sizeOff, aafInt64 end);
	AAFRESULT CreateAudioDataEnd(void);
};

#endif // ! __CAAFEssenceCodec_h__


//@doc
//@class    CAAFJPEGCodec | Implementation class for AAFEssenceCodec
#ifndef __CAAFJPEGCodec_h__
#define __CAAFJPEGCodec_h__

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


// Include the interfaces that our plugin needs to support.
#include "AAFPlugin.h"

// Include our base class for support of IUnknown methods.
#include "CAAFUnknown.h"


#ifndef __CAAFJPEGDescriptorHelper_h__
#include "CAAFJPEGDescriptorHelper.h"
#endif

#include "AAFUtils.h" // AAFByteOrder

#include "jpegesdata.h"

// ID for this Plugin's CoClass.
EXTERN_C const CLSID CLSID_AAFJPEGCodec;





class CAAFJPEGCodec
  : public IAAFEssenceCodec,
	  public IAAFPlugin,
    public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFJPEGCodec (IUnknown * pControllingUnknown);
  virtual ~CAAFJPEGCodec ();

public:

  //
  // IAAFPlugin interface methods
	//

  STDMETHOD (CountDefinitions)
	  (/*[out]*/ aafUInt32 *pDefCount);

  STDMETHOD (GetIndexedDefinitionID)
	  (/*[in] */ aafUInt32 index, 
		 /*[out]*/ aafUID_t *result);

  STDMETHOD (GetPluginDescriptorID)
	  (/*[out]*/ aafUID_t *result);

  STDMETHOD (GetIndexedDefinitionObject)
	  (/*[in] */ aafUInt32 index, 
		 /*[in] */ IAAFDictionary *dict, 
		 /*[out]*/ IAAFDefObject **def);

  STDMETHOD (CreateDescriptor)
	  (/*[in] */ IAAFDictionary *dict,
		 /*[out]*/ IAAFPluginDef **desc);


  //
  // IAAFEssenceCodec interface methods
	//
	
	// Set the AAFEssenceAccess used as a factory for AAFEssenceFormat 
  STDMETHOD (SetEssenceAccess)
    (/*[in] */ IAAFEssenceAccess *access);

  STDMETHOD (CountFlavours)
    (/*[in] */ aafUInt32  *pCount);

	// Some codecs have variants handled by a single codec.
	// (For example, the Avid AVR codec handles multiple AVRs.)
	// The number of variants is returned by GetIndexedFlavourID, and cached
	// by the AAFPluginManager.
	//
  STDMETHOD (GetIndexedFlavourID)
    (/*[in] */ aafUInt32  index, // Which flavour to get the ID for
     /*[out]*/ aafUID_t *  pFlavour); // The returned flavour ID 

  STDMETHOD (CountDataDefinitions)
    (/*[out] */ aafUInt32 *pCount);

	// All codecs handle at least one kind of media (picture, sound, control)
	    // but some handle more than one.  The kind of media is specified by an
	    // AAFDataDefinition.  The numnber of data definitions is returned by
	    // GetIndexedDataDefinition, and cached by the AAFPluginManager.
  STDMETHOD (GetIndexedDataDefinition)
    (/*[in] */ aafUInt32  index, // Which data definition to get the ID for
     /*[out]*/ aafUID_t * pFlavour); // The returned dataDefinition 

  STDMETHOD (GetMaxCodecDisplayNameLength)
    (/*[out]*/ aafUInt32  *bufSize);

	// Given a flavour ID, return the human readable name
  STDMETHOD (GetCodecDisplayName)
    (/*[in] */ aafUID_constref  flavour, // which flavour of the codec to use
     /*[in,string]*/ aafCharacter *  pName, // Human-readable name of the flavour
     /*[in] */ aafUInt32  bufSize); // length of the buffer to hold flavour Name 
	
  // Returns the number of channels which this codec can handle
			// of the given essence kind
  STDMETHOD (CountChannels)
    (/*[in] */ IAAFSourceMob *fileMob, // Get the number of processable channels on this file mob
     /*[in] */ aafUID_constref essenceKind, // This is the type of essence to open
     /*[in] */ IAAFEssenceStream *stream,
     /*[out]*/ aafUInt16 *  pNumChannels); // The number of channels present 

  STDMETHOD (GetSelectInfo)
	  (/*[in] */ IAAFSourceMob *fileMob, 
		 /*[in] */ IAAFEssenceStream *stream, 
		 /*[out]*/ aafSelectInfo_t *  pSelectInfo);
	 
  // Returns a (possibly zero-length) string listing any problems
			//found with the essence, or the relationship between variables
			// in the essence descriptor, and any such values contained within
			// the essence data
  STDMETHOD (ValidateEssence)
    (/*[in] */ IAAFSourceMob *fileMob, // Run a check on this file mob
     /*[in] */ IAAFEssenceStream *stream,
     /*[in] */ aafCheckVerbose_t  verbose, // This is the verbosity level of the output
     /*[out]*/ aafCheckWarnings_t warning, // This determines whether the output contains warnings
     /*[in] */ aafUInt32  bufSize,
		 /*[in, string] */ wchar_t *  pName,
     /*[out]*/ aafUInt32  *bytesWritten);
		
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

  // Returns the number of samples which this codec can find on the
			// given slot.
  STDMETHOD (CountSamples)
    (/*[in] */ aafUID_constref essenceKind, // This is the type of essence to check
	   /*[out]*/ aafLength_t *  pNumSamples); // The number of samples present of that type

  STDMETHOD (WriteSamples) (
    /*[in]*/ aafUInt32  nSamples, // write this many samples
    /*[in]*/ aafUInt32  buflen,  // size of buffer
    /*[in,size_is(buflen)]*/ aafDataBuffer_t  buffer, // from a buffer
    /*[out]*/ aafUInt32 *samplesWritten, // write this many samples
    /*[out]*/ aafUInt32 *bytesWritten);  // size of buffer

  // Write blocks from one or more buffers, interleaving if needed.
  STDMETHOD (WriteBlocks)
    (/*[in]*/ aafDeinterleave_t  inter, // Whether the material will be de-interleaved on read
     /*[in]*/ aafUInt16  xferBlockCount, // How many aafMultiXfer blocks follow
     /*[in]*/ aafmMultiXfer_t *  xferBlock, // One or more blocks containing buffer pointer and length 
     /*[in]*/ aafmMultiResult_t *  resultBlock);

  STDMETHOD (ReadSamples) (  
    /*[in]*/ aafUInt32  nSamples, // Read this many samples
    /*[in]*/ aafUInt32  buflen, // into a buffer of this size
    /*[out]*/ aafDataBuffer_t  buffer, // The transfer buffer
    /*[out, ref]*/ aafUInt32 *  samplesRead, // The number of samples actually read
    /*[out, ref]*/ aafUInt32 *  bytesRead); // The number of bytes actually read

  // Read blocks into one or more buffers, de-interleaving if needed.
  STDMETHOD (ReadBlocks)
    (/*[in]*/ aafDeinterleave_t  inter, // Whether the material will be de-interleaved on read
     /*[in]*/ aafUInt16  xferBlockCount, // How many aafmMultiXfer blocks follow
     /*[in]*/ aafmMultiXfer_t *  xferBlock, // One or more blocks containing buffer pointer and length 
     /*[in]*/ aafmMultiResult_t *  resultBlock);

  // Seek to a particular sample frame on the media.  The
			// sample frame is one frame for picture, and one sample
			// times the number of interleaved channels for audio.
  STDMETHOD (Seek)
    (/*[in]*/ aafPosition_t  sampleFrame); // The sample frame to seek to. 

  // Close the media stream, ready to open another or Finish.
  STDMETHOD (CompleteWrite)
     (IAAFSourceMob *desc);


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
	
  // Given some raw essence (like a JPEG file), create
			// an AAFEssenceDescriptor to match, with all fields filled in.
  STDMETHOD (CreateDescriptorFromStream)
    (/*[in] */ IAAFEssenceStream * pStream, // A raw file stream
     /*[in] */ IAAFSourceMob *fileMob); // Put the finished descriptor on this source mob 

  // Return the current essence stream.
  STDMETHOD (GetCurrentEssenceStream)
    (/*[out]*/ IAAFEssenceStream ** ppStream); // The current essence stream 
		
  // Set the format of the data.
  STDMETHOD (PutEssenceFormat)
    (/*[in] */ IAAFEssenceFormat * pFormat); // An essence format object 

  // Supply an essence format object specifying what data is
			// required, and it is filled in with the data values.
  STDMETHOD (GetEssenceFormat)
    (/*[in] */ IAAFEssenceFormat *pTemplate, // An essence format template object 
     /*[out]*/ IAAFEssenceFormat **pResult); // An essence format result object 

  STDMETHOD (GetDefaultEssenceFormat)
    (/*[out]*/ IAAFEssenceFormat **pFormat); // An essence format result object 

  STDMETHOD (GetEssenceDescriptorID)
	  (/*[out]*/ aafUID_t *result);

  STDMETHOD (GetEssenceDataID)
	  (/*[out]*/ aafUID_t *result);

  STDMETHOD (GetIndexedSampleSize)
    (/*[in] */ aafUID_constref dataDefID,
		 /*[in] */aafPosition_t pos, 
		 /*[out]*/aafLength_t *pResult);

  STDMETHOD (GetLargestSampleSize)
    (/*[in] */aafUID_constref dataDefID,
		 /*[out]*/aafLength_t *pResult);


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


private:
	void SetEssenceStream(IAAFEssenceStream *stream);
  void SetCompressionEnabled(aafCompressEnable_t compEnable);
  HRESULT AddSampleIndexEntry(aafPosition_t pos);
	void SetNumberOfSamples(const aafLength_t& numberOfSamples);
	void SetCurrentIndex(aafUInt32 currentIndex);
	void SetWriteIndex(aafUInt32 writeIndex);
	aafUInt32 GetSampleSizeFromIndex(const aafPosition_t& pos); // throw HRESULT
	void CreateLegacyPropDefs(IAAFDictionary *p_dict);

	typedef struct _aafCompressionParams
	{
		aafUInt32 imageWidth;
		aafUInt32 imageHeight;

		aafUInt16 components;
		aafColorSpace_t colorSpace;
		aafUInt32 horizontalSubsampling;
		aafUInt32 verticalSubsampling;

		aafUInt32 blackReferenceLevel;
		aafUInt32 whiteReferenceLevel;
		aafUInt32 colorRange;

		int quality; 

		aafUInt32 rowBytes;
		aafDataBuffer_t buffer;
		aafUInt32 bufferSize;

	} aafCompressionParams;

	void DumpSampleImage(const aafCompressionParams& param, JSAMPIMAGE rawSampleImage);

	JSAMPIMAGE GetRawSampleImage(const aafCompressionParams& param); // throw HRESULT
	aafUInt32 CopyDataToSampleImage(const aafCompressionParams& param, aafUInt32 startingOffset, JSAMPIMAGE rawSampleImage); // throw HRESULT
	aafUInt32 CopyDataFromSampleImage(JSAMPIMAGE rawSampleImage, aafUInt32 startingOffset, aafCompressionParams& param); // throw HRESULT

	// Compress a single image data from the given buffer. Return the actual
	// number of bytes written.
	HRESULT CompressImage(const aafCompressionParams& param, struct jpeg_compress_struct& cinfo);

	// Decompress a single image from the current position in the stream returning
	// the image data in buffer and the actual number of bytes written. Note: bufLen must
	// be large enough to hold all of the decompressed data
	HRESULT DecompressImage(aafCompressionParams& param, struct jpeg_decompress_struct& cinfo);

	// Utility to get the current offset in the stream and add
	// it the the sample index
	void AddNewCompressedSample(); // throw HRESULT

	// SampleIndex access methods : may be temporary. We need these methods
	// to read/write the index because the SampleIndex property in EssenceData
	// is not implemented. Also, these methods will allow all essence streams
	// to be treated the sample, whether in an AAF file or not.

	HRESULT ReadNumberOfSamples(IAAFEssenceStream *stream,
		                         aafLength_t& numberOfSamples);
	HRESULT AllocateSampleIndex(const aafLength_t& numberOfSamples);
	HRESULT ReadSampleIndex();
	HRESULT ReadSampleIndex32(aafUInt16 fileByteOrder);
	HRESULT WriteSampleIndex();
	HRESULT WriteSampleIndex32();

	// Helper utility to make sure the given descriptor information is synchronized
	// with the current information in the codec. Called in CompleteWrite method.
	void UpdateDescriptor (CAAFJPEGDescriptorHelper& descriptorHelper);

	// Routine to keep calculated member data up-to-date.
	void UpdateCalculatedData(void);

private:
	AAFByteOrder		_nativeByteOrder;

	IAAFEssenceAccess	*_access;
	IAAFEssenceStream	*_stream; // stream for jpeg sample data.

	CAAFJPEGDescriptorHelper _descriptorHelper;

	aafMediaOpenMode_t _openMode; // Either read-only or for append.
	aafCompressEnable_t _compressEnable;   // if true then should receive and deliver uncompressed data.


	// Data from/to FileDescriptor
	aafLength_t _length; // total size of file (informational?)
	aafRational_t _sampleRate; // same for all samples.
	aafUID_t _containerFormat;	// set by refimpl

	// Data from/to DigitalImageDescriptor
	aafUID_t _compression;

	aafUID_t _codecID; // always CodecJPEG.

	aafUInt32 _storedHeight;
	aafUInt32 _storedWidth;

	aafUInt32 _sampledHeight; // SampledView
	aafUInt32 _sampledWidth;
	aafInt32 _sampledXOffset;
	aafInt32 _sampledYOffset;

	aafUInt32 _displayHeight; // DisplayView
	aafUInt32 _displayWidth;
	aafInt32 _displayXOffset;
	aafInt32 _displayYOffset;

	aafFrameLayout_t _frameLayout;

	aafUInt32 _videoLineMapSize; // s/b 2, VideoLineMap
	aafVideoLineMap_t  _videoLineMap;

	aafRational_t _imageAspectRatio;
	aafAlphaTransparency_t _alphaTransparency;
	aafRational_t _gamma;
	aafInt32 _imageAlignmentFactor; // padding for "sector size" on disk for possibly faster access.

	// Data from/to CDCIDescriptor
	aafInt32 _componentWidth; // should always be 8 (no support for 12 bit data).
	aafUInt32 _horizontalSubsampling;
	aafUInt32 _verticalSubsampling; // not currently a property (default to 1 in codec).
	aafColorSiting_t _colorSiting;
	aafUInt32 _blackReferenceLevel;
	aafUInt32 _whiteReferenceLevel;
	aafUInt32 _colorRange;
	aafInt16 _paddingBits;

	// Misc. data copied from omf codec
	aafUInt32 _imageHeight; 
	aafUInt32 _imageWidth;
	aafUInt32 _fileBytesPerSample;
	aafBoolean_t _descriptorFlushed;
	aafUInt32 _startingIndex; // index that we cannot write before...
	aafUInt32 _currentIndex;
	aafUInt32 _writeIndex;
	aafUInt32 _maxIndex;
	aafPosition_t *_sampleIndex;

	aafColorSpace_t _pixelFormat;
	aafCompArray_t _compArray;

	//jpeg_tables     compressionTables[3];
	//aafBoolean_t _customTables;
	//omfJPEGInfo_t   jpeg;
	aafFieldDom_t	_fieldDominance;
	aafInt16 _memBitsPerPixel;
	aafInt16 _bitsPerPixelAvg;
	aafInt32 _memBytesPerSample;
	aafUInt32 _bitsPerSample;

	// Copied from WaveCodec...(may be renamed...)
	aafLength_t	_numberOfSamples; /* was _sampleFrames in WaveCodec) */

	aafUInt16 _padBytesPerRow;
	aafUInt32 _compression_IJG_Quality;
	aafInt32 _resolutionID;


	// Data do define and hold the sample image buffer used for raw sub-sampled
	// YUV data (4-2-2 YCbCr). This is the buffer used with the jpeg_write_raw_data
	// and jpeg_read_raw_data functions.
	JSAMPIMAGE _rawSampleImage; /* a 3-D sample array: top index is color */
	JSAMPARRAY _rawSampleArray; // array of all sample row pointers in raw sample image.
	JSAMPLE * _rawSampleImageBuffer; // ultimate buffer for _sampleImage.
	aafUInt32 _rawSampleImageBufferSize;

};

#endif // ! __CAAFJPEGCodec_h__


//@doc
//@class    CAAFJPEGCodec | Implementation class for AAFEssenceCodec
#ifndef __CAAFJPEGCodec_h__
#define __CAAFJPEGCodec_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


// Include the interfaces that our plugin needs to support.
#include "AAFPlugin.h"

// Include our base class for support of IUnknown methods.
#include "CAAFUnknown.h"


#ifndef __CAAFJPEGDescriptorHelper_h__
#include "CAAFJPEGDescriptorHelper.h"
#endif

#include "AAFUtils.h" // AAFByteOrder



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

  // Set up a codec.
  STDMETHOD (Start)
     (void);

  // Tear down a codec.
  STDMETHOD (Finish)
     (void);

  STDMETHOD (GetNumDefinitions)
	  (/*[out]*/ aafInt32 *pDefCount);

  STDMETHOD (GetIndexedDefinitionID)
	  (/*[in] */ aafInt32 index, 
		 /*[out]*/ aafUID_t *result);

  STDMETHOD (GetPluginDescriptorID)
	  (/*[out]*/ aafUID_t *result);

  STDMETHOD (GetIndexedDefinitionObject)
	  (/*[in] */ aafInt32 index, 
		 /*[in] */ IAAFDictionary *dict, 
		 /*[out]*/ IAAFDefObject **def);

  STDMETHOD (CreateDescriptor)
	  (/*[in] */ IAAFDictionary *dict,
		 /*[out]*/ IAAFPluginDescriptor **desc);


  //
  // IAAFEssenceCodec interface methods
	//
	
	// Set the AAFEssenceAccess used as a factory for AAFEssenceFormat 
  STDMETHOD (SetEssenceAccess)
    (/*[in] */ IAAFEssenceAccess *access);

  STDMETHOD (GetFlavourCount)
    (/*[in] */ aafInt32  *pCount);

	// Some codecs have variants handled by a single codec.
	// (For example, the Avid AVR codec handles multiple AVRs.)
	// The number of variants is returned by GetIndexedFlavourID, and cached
	// by the AAFPluginManager.
	//
  STDMETHOD (GetIndexedFlavourID)
    (/*[in] */ aafInt32  index, // Which variant to get the ID for
     /*[out]*/ aafUID_t *  pVariant); // The returned variant ID 

	
  // Sets a value indicating whether the SDK is handling the compression.
  STDMETHOD (SetCompressionEnabled)
    (/*[in] */ aafBool  enable); // isCompressionEnabled 

  STDMETHOD (GetDataDefinitionCount)
    (/*[out] */ aafInt32 *pCount);

	// All codecs handle at least one kind of media (picture, sound, control)
	    // but some handle more than one.  The kind of media is specified by an
	    // AAFDataDefinition.  The numnber of data definitions is returned by
	    // GetIndexedDataDefinition, and cached by the AAFPluginManager.
  STDMETHOD (GetIndexedDataDefinition)
    (/*[in] */ aafInt32  index, // Which data definition to get the ID for
     /*[out]*/ aafUID_t *  pVariant); // The returned dataDefinition 

  STDMETHOD (GetMaxCodecDisplayNameLength)
    (/*[out]*/ aafInt32  *bufSize);

	// Given a variant ID, return the human readable name
  STDMETHOD (GetCodecDisplayName)
    (/*[in] */ aafUID_t  variant, // which variant of the codec to use
     /*[in,string]*/ wchar_t *  pName, // Human-readable name of the variant
     /*[in] */ aafInt32  bufSize); // length of the buffer to hold variant Name 
	
  // Returns the number of channels which this codec can handle
			// of the given essence kind
  STDMETHOD (GetNumChannels)
    (/*[in] */ IAAFSourceMob *fileMob, // Get the number of processable channels on this file mob
     /*[in] */ aafUID_t essenceKind, // This is the type of essence to open
     /*[in] */ IAAFEssenceStream *stream,
     /*[out]*/ aafInt16 *  pNumChannels); // The number of channels present 

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
     /*[in] */ aafInt32  bufSize,
		 /*[in, string] */ wchar_t *  pName,
     /*[out]*/ aafInt32  *bytesWritten);
		
  // Create a media data object, and attach the correct type of
			//EssenceDescriptor to the fileMob
  STDMETHOD (Create)
    (/*[in]*/ IAAFSourceMob *fileMob, // Create the essence attached to this file mob
     /*[in]*/ aafUID_t  variant, // which variant of the codec to use
     /*[in]*/ IAAFEssenceStream * stream,
     /*[in]*/ aafInt32 numParms,
     /*[in]*/ aafmMultiCreate_t *createParms);

  // Open a media data object.
  STDMETHOD (Open)
    (/*[in]*/ IAAFSourceMob *fileMob, // Open the essence attached to this file mob
     /*[in]*/ aafSlotID_t	slotID,
     /*[in]*/ aafMediaOpenMode_t  openMode, // In this mode
     /*[in]*/ IAAFEssenceStream * stream); // Here is an essence stream with the raw data 
	
  // Returns the number of samples which this codec can find on the
			// given slot.
  STDMETHOD (GetNumSamples)
    (/*[in] */ aafUID_t  essenceKind, // This is the type of essence to check
	   /*[out]*/ aafLength_t *  pNumSamples); // The number of samples present of that type

  // Write blocks from one or more buffers, interleaving if needed.
  STDMETHOD (WriteBlocks)
    (/*[in]*/ aafDeinterleave_t  inter, // Whether the material will be de-interleaved on read
     /*[in]*/ aafInt16  xferBlockCount, // How many aafMultiXfer blocks follow
     /*[in]*/ aafmMultiXfer_t *  xferBlock, // One or more blocks containing buffer pointer and length 
     /*[in]*/ aafmMultiResult_t *  resultBlock);

  // Read blocks into one or more buffers, de-interleaving if needed.
  STDMETHOD (ReadBlocks)
    (/*[in]*/ aafDeinterleave_t  inter, // Whether the material will be de-interleaved on read
     /*[in]*/ aafInt16  xferBlockCount, // How many aafmMultiXfer blocks follow
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

  // Write some number of bytes to the stream,
			// formatting it as usual.  When the number of bytes passes
			//the size of a sample frame, then the number of sample frames is
			// bumped by one.
  STDMETHOD (WriteFractionalSample)
    (/*[in,size_is(buflen)]*/ aafDataBuffer_t  buffer, // to a buffer
     /*[in]*/ aafInt32  buflen); // of this size 

  // Read some number of bytes to the stream, removing any
			// formatting.  When the number of bytes passes
			//the size of a sample frame, then the number of sample frames read is
			// bumped by one.
  STDMETHOD (ReadFractionalSample)
    (/*[in]*/ aafUInt32  buflen, // to a buffer of this size
     /*[out, size_is(buflen), length_is(*bytesRead)]*/ aafDataBuffer_t  buffer, // here is the buffer
     /*[out,ref]*/ aafUInt32 *  bytesRead); // Return bytes actually read 

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
    (/*[in] */aafUID_t dataDefID, 
		 /*[in] */aafPosition_t pos, 
		 /*[out]*/aafLength_t *pResult);

  STDMETHOD (GetLargestSampleSize)
    (/*[in] */aafUID_t dataDefID, 
		 /*[out]*/aafLength_t *pResult);

  STDMETHOD (AddSampleIndexEntry)
    (/*[in] */aafPosition_t pos);


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  virtual HRESULT InternalQueryInterface(REFIID riid, void **ppvObjOut);


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
	void SetNumberOfSamples(const aafLength_t& numberOfSamples);
	void SetCurrentIndex(aafUInt32 currentIndex);
	void SetWriteIndex(aafUInt32 writeIndex);

	typedef struct _aafCompressionParams
	{
		aafUInt32 imageWidth;
		aafUInt32 imageHeight;

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

	// Compress a single image data from the given buffer. Return the actual
	// number of bytes written.
	HRESULT CompressImage(const aafCompressionParams& param);

	// Decompress a single image from the current position in the stream returning
	// the image data in buffer and the actual number of bytes written. Note: bufLen must
	// be large enough to hold all of the decompressed data
	HRESULT DecompressImage(aafCompressionParams& param);

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
	HRESULT WriteSampleIndex();

private:
	AAFByteOrder		_nativeByteOrder;

	IAAFEssenceAccess	*_access;
	IAAFEssenceStream	*_stream; // stream for jpeg sample data.

	CAAFJPEGDescriptorHelper _descriptorHelper;

	aafMediaOpenMode_t _openMode; // Either read-only or for append.
	aafCompressEnable_t _compressEnable;   // if true then should receive and deliver uncompressed data.


	// Data from/to FileDescriptor
	aafLength_t _length; // total size of file (informational?)
	aafBool _isInAAFContainer; // set to true by refimpl if the essence stream is actually in an essence data.
	aafRational_t _sampleRate; // same for all samples.
	aafUID_t _containerFormat;	// set by refimpl

	// Data from/to DigitalImageDescriptor
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
	aafBool _descriptorFlushed;
	aafUInt32 _startingIndex; // index that we cannot write before...
	aafUInt32 _currentIndex;
	aafUInt32 _writeIndex;
	aafUInt32 _maxIndex;
	aafPosition_t *_sampleIndex;

	aafColorSpace_t _pixelFormat;
	aafCompArray_t _compArray;

	//jpeg_tables     compressionTables[3];
	//aafBool _customTables;
	//omfJPEGInfo_t   jpeg;
	aafFieldDom_t	_fieldDominance;
	aafInt16 _memBitsPerPixel;
	aafInt16 _bitsPerPixelAvg;
	aafInt32 _memBytesPerSample;
	aafUInt32 _bitsPerSample;

	// Copied from WaveCodec...(may be renamed...)
	aafLength_t	_numberOfSamples; /* was _sampleFrames in WaveCodec) */

	aafUInt16 _padBytesPerRow;


	bool _headerLoaded; // has the jpeg header been loaded?

};

#endif // ! __CAAFJPEGCodec_h__


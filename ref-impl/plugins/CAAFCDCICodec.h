//@doc
//@class    CAAFCDCICodec | Implementation class for AAFEssenceCodec
#ifndef __CAAFCDCICodec_h__
#define __CAAFCDCICodec_h__

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
 * prior written permission of Avid Technology, Inc.
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

#include "AAFUtils.h" // AAFByteOrder

#ifndef __CAAFCDCIDescriptorHelper_h__
#include "CAAFCDCIDescriptorHelper.h"
#endif

// ID for this Plugin's CoClass.
EXTERN_C const CLSID CLSID_AAFCDCICodec;





class CAAFCDCICodec
  : public IAAFEssenceCodec,
	  public IAAFPlugin,
    public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFCDCICodec (IUnknown * pControllingUnknown);
  virtual ~CAAFCDCICodec ();

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


  // Given some raw essence (like a CDCI file), create
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
  	void SetNumberOfSamples(const aafLength_t& numberOfSamples);


	// Helper utility to make sure the current information in the codec is 
	// synchronized with the given descriptor information . 
	// Called in Open and Create methods.
	STDMETHOD( ReadDescriptor )(CAAFCDCIDescriptorHelper& descriptorHelper);

	// Helper utility to make sure the given descriptor information is synchronized
	// with the current information in the codec. Called in 
	// CompleteWrite and Create methods.
	void UpdateDescriptor (CAAFCDCIDescriptorHelper& descriptorHelper);

	// Routine to keep calculated member data up-to-date.
	void UpdateCalculatedData(void);

	STDMETHOD( CreateLegacyPropDefs )(IAAFDictionary *p_dict);

private:
	AAFByteOrder		_nativeByteOrder;

	IAAFEssenceAccess	*_access;
	IAAFEssenceStream	*_stream; // stream for CDCI sample data.


	aafMediaOpenMode_t _openMode; // Either read-only or for append.


	// Data from/to FileDescriptor
	aafLength_t _length; // total size of file (informational?)
	aafRational_t _sampleRate; // same for all samples.
	aafUID_t _containerFormat;	// set by refimpl

	// Data from/to DigitalImageDescriptor
	aafUID_t _compression;

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

	CAAFCDCIDescriptorHelper _descriptorHelper;

	aafUInt32 _videoLineMapSize; // s/b 2, VideoLineMap
	aafVideoLineMap_t  _videoLineMap;

	aafRational_t _imageAspectRatio;
	aafAlphaTransparency_t _alphaTransparency;
	aafUID_t _gamma;
	aafUInt32 _imageAlignmentFactor; // padding for "sector size" on disk for possibly faster access.

	// Data from/to CDCIDescriptor
	aafUInt32 _componentWidth; // should always be 8 (no support for 12 bit data).
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

	aafColorSpace_t _pixelFormat;
	aafCompArray_t _compArray;

	aafFieldDom_t	_fieldDominance;
	aafUInt32	_fieldStartOffset;
	aafUInt32	_fieldEndOffset;

	aafUInt16 _bitsPerPixelAvg;
	aafUInt32 _bitsPerSample;

	// Copied from WaveCodec...(may be renamed...)
	aafLength_t	_numberOfSamples; /* was _sampleFrames in WaveCodec) */

	aafUInt16 _padBytesPerRow;
};

#endif // ! __CAAFCDCICodec_h__


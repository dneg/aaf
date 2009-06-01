//@doc
//@class    CAAFDNxHDCodec | Implementation class for AAFEssenceCodec
#ifndef __CAAFDNxHDCodec_h__
#define __CAAFDNxHDCodec_h__

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


// Include the interfaces that our plugin needs to support.
#include "AAFPlugin.h"

// Include our base class for support of IUnknown methods.
#include "CAAFUnknown.h"

#include "AAFPluginUtils.h" // AAFByteOrder

#ifndef __CAAFCDCIDescriptorHelper_h__
#include "CAAFCDCIDescriptorHelper.h"
#endif

// Buffer Layouts
enum _BufferLayout{
	Layout_Unknown,
	Layout_Raw,			// compressed, per m_EssenceFormat
	Layout_YUV,			// really 3 bytes per pixel
	Layout_RGB,			// 3 bytes per pixel
	Layout_UYVY,		// 4 bytes per 2 pixels
	Layout_P_UYVY,		// frame of packed 2 lsbs (1 byte per 2 pixels), then 4 bytes per 2 pixels
	Layout_UYVYI,		// 8 bytes per 2 pixels, msb-aligned, little-endian
	Layout_UYVYM,		// 8 bytes per 2 pixels, msb-aligned, big-endian
	Layout_BGRA,		// 4 bytes per pixel
	Layout_BGRAI,		// 8 bytes per pixel, msb-aligned, little-endian
};
typedef enum _BufferLayout BufferLayout_t;

#include "AAFCompressionDefs.h"
#include "AAFContainerDefs.h"

#ifdef USE_DNxHD_CODEC

// for access to the Avid DNxHD Codec SDK
// please visit http://www.avid.com/DNxHD
// or contact DNxHDSupport@avid.com

#include "AvidDNXCodecParams.h"

#ifdef WIN32
#include "AvidHDCodecSDKdll.h"
#else
#include "AvidHDCodecSDK.h"
#endif

#endif //USE_DNxHD_CODEC

// ID for this Plugin's CoClass.
EXTERN_C const CLSID CLSID_AAFDNxHDCodec;

#define DNxHDHEADERSIZE 640

#define MAX_DNxHD_UNCOMPRESSED_FRAME (1080*1920*8)
#define MAX_DNxHD_COMPRESSED_FRAME (917504)

class CAAFDNxHDCodec
  : public IAAFEssenceCodec2,
	public IAAFEssenceCodec,
	public IAAFPlugin,
	public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFDNxHDCodec (IUnknown * pControllingUnknown);
  virtual ~CAAFDNxHDCodec ();

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

	// Switches the codec into the specified flavour.
  STDMETHOD (SetFlavour)
    (/*[in]*/ aafUID_constref flavour);

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


  // Given some raw essence (like a DNxHD file), create
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

	// EqualDegenerateAUID() could be moved to AAFPluginUtils.cpp and/or AAFUtils.cpp
	// it appears in plugins/CAAFVC3Codec.cpp and plugins/CAAFDNxHDCodec.cpp and impl/ImplAAFEssenceAccess.cpp
	// it is kept here for 1.1.3 because it is used only by the patch to accept MXF files with no CodecID
	static aafBool EqualDegenerateAUID(const aafUID_t *uid1, const aafUID_t *uid2)
	{
		// does not test any bytes that are zero in uid2
		// allows comparing a specific UL against a family of ULs

		int i = sizeof(aafUID_t);

		const char* u1= (const char*)uid1;
		const char* u2= (const char*)uid2;

		char b;
		do
			if( *u1++ != (b = *u2++) && b ) return kAAFFalse;
		while( --i ); 

		return kAAFTrue;
	}

	static bool IsDNxHD(const aafUID_t &compId)
	{
		if( EqualAUID(&compId,&kAAFCompressionDef_Avid_DNxHD_Legacy) ) return true; 
		else if( EqualDegenerateAUID(&compId,&kAAFCompressionDef_VC3_1) ) return true; 
		else return false;
	}

	void SetEssenceStream(IAAFEssenceStream *stream);
	void SetCompressionEnabled(aafCompressEnable_t compEnable);

  	void SetNumberOfSamples(const aafLength_t& numberOfSamples);


	// Helper utility to make sure the current information in the codec is 
	// synchronized with the given descriptor information . 
	// Called in Open and Create methods.
	STDMETHOD( ReadDescriptor )(CAAFCDCIDescriptorHelper& descriptorHelper);

	// Helper utility to make sure the given descriptor information is synchronized
	// with the current information in the codec. Called in 
	// CompleteWrite and Create methods.
	void UpdateDescriptor (CAAFCDCIDescriptorHelper& descriptorHelper);

	// Prepares codec internals for writing samples
	void SetCodecState(void);

	// Routine to keep calculated member data up-to-date.
	void UpdateCalculatedData(void);

	STDMETHOD( CreateLegacyPropDefs )(IAAFDictionary *p_dict);
	
	bool IsInterlaced()
	{
		if(	   _ComprID==1241 || _ComprID==1242 
			|| _ComprID==1243 || _ComprID==1244 )
			return true;
		return false;
	}

	virtual aafUInt32 GetBytesPerSample()
	{
		switch(_ComprID)
		{
		case 1235:
		case 1238:
		case 1241:
		case 1243:
			return 917504;
			break;
		case 1237:
		case 1242:
		case 1244:
			return 606208;
			break;
		case 1250:
		case 1251:
			return 458752;
			break;
		case 1252:
			return 303104;
			break;
		case 1253:
			return 188416;
			break;
		case 1254:
			return 131072;	// FIXME: is this correct?
			break;
		default:
			return 0;
			break;
		}
		return 0; // avoid unnecessary warning
	}

	aafUInt32 GetUncomprFrameSize()
	{
		if(GetLinesPerField()==720)
		{
			return 720*1280*4;
		}
		return MAX_DNxHD_UNCOMPRESSED_FRAME;
	}

	bool Is10Bit()
	{
		if( _ComprID==1235 || _ComprID==1241 || _ComprID==1250)
			return true;
		return false;
	}

	//! Get Is720p for the selected CompressionID
	bool Is720p()
	{
		if(_ComprID==1250 || _ComprID==1251 || _ComprID==1252 )
			return true;
		return false;
	}

	int GetLinesPerField()
	{
		switch(_ComprID)
		{
		case 1235:
		case 1237:
		case 1238:
			return 1080;
			break;
		case 1241:
		case 1243:
		case 1242:
		case 1244:
			return 540;
			break;
		case 1250:
		case 1251:
		case 1252:
			return 720;
			break;
		case 1253:
		case 1254:
			return 1080;
			break;
		default:
			return 0;
			break;
		}
		return 0; // avoid unnecessary warning
	}

	int GetPixelsPerLine()
	{
		switch(_ComprID)
		{
		case 1235:
		case 1237:
		case 1238:
		case 1241:
		case 1243:
		case 1242:
			return 1920;
			break;
		case 1250:
		case 1251:
		case 1252:
			return 1280;
			break;
		case 1253:
		case 1254:
			return 1920;
			break;
		case 1244:
			return 1440;
		default:
			return 0;
			break;
		}
		return 0; // avoid unnecessary warning
	}

	inline aafUInt32 GetComprID(const aafUID_t &compId, const aafUID_t &container)
	{
		aafUInt32 c = 0;

#ifdef USE_DNxHD_CODEC
		if( EqualAUID(&compId,&kAAFCompressionDef_Avid_DNxHD_Legacy) )
		{
			// derive Compression ID from Container Def
				 if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_220X_1080p ) ) c = DNX_220X_1080p_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_145_1080p ) ) c = DNX_145_1080p_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_220_1080p ) ) c = DNX_220_1080p_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_220X_1080i ) ) c = DNX_220X_1080i_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_145_1080i ) ) c = DNX_145_1080i_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_220_1080i ) ) c = DNX_220_1080i_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_145_1440_1080i ) ) c = DNX_145_1440_1080i_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_220X_720p ) ) c = DNX_220X_720p_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_220_720p ) ) c = DNX_220_720p_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_145_720p ) ) c = DNX_145_720p_COMPRESSION_ID;
			else if( EqualAUID(&container,&kAAFContainerDef_MXFGC_Avid_DNX_36_1080p ) ) c = DNX_36_1080p_COMPRESSION_ID;
			// else if( EqualAUID(&container,& ) ) c = DNX_50_1080i_COMPRESSION_ID;
			else							    c = 0;
		}
		else if( EqualDegenerateAUID(&compId,&kAAFCompressionDef_VC3_1) ) 
		{
			// derive ComprID from Compression 
			switch(compId.Data2)
			{
			case 0x7101: c = DNX_220X_1080p_COMPRESSION_ID; break;
			case 0x7103: c = DNX_145_1080p_COMPRESSION_ID; break;
			case 0x7104: c = DNX_220_1080p_COMPRESSION_ID; break;
			case 0x7107: c = DNX_220X_1080i_COMPRESSION_ID; break;
			case 0x7108: c = DNX_145_1080i_COMPRESSION_ID; break;
			case 0x7109: c = DNX_220_1080i_COMPRESSION_ID; break;
			case 0x710a: c = DNX_145_1440_1080i_COMPRESSION_ID; break;
			case 0x7110: c = DNX_220X_720p_COMPRESSION_ID; break;
			case 0x7111: c = DNX_220_720p_COMPRESSION_ID; break;
			case 0x7112: c = DNX_145_720p_COMPRESSION_ID; break;
			case 0x7113: c = DNX_36_1080p_COMPRESSION_ID; break;
			//case 0x7114: c = DNX_50_1080i_COMPRESSION_ID; break;
			default:	 c = 0;
			}
		}
#endif
		return c;
	}

private:
	AAFByteOrder		_nativeByteOrder;

	IAAFEssenceAccess	*_access;
	IAAFEssenceStream	*_stream; // stream for DNxHD sample data.


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

	// Data from/to DNxHDDescriptor
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

	BufferLayout_t _pixelFormat;
	aafCompArray_t _compArray;

	aafFieldDom_t	_fieldDominance;
	aafUInt32	_fieldStartOffset;
	aafUInt32	_fieldEndOffset;

	aafUInt16 _bitsPerPixelAvg;

	// Copied from WaveCodec...(may be renamed...)
	aafLength_t	_numberOfSamples; /* was _sampleFrames in WaveCodec) */

	aafUInt16 _padBytesPerRow;

#ifdef USE_DNxHD_CODEC
	

	aafDataBuffer_t	_DNxHD_buffer;		// enough for PAL or NTSC frame
	AvidHDCodecSDK * _pAdvDNxCodec;     // The Codec if available
	bool             _CodecInitialized;

#endif

	aafCompressEnable_t _compressEnable;

	aafUInt8 _Header[DNxHDHEADERSIZE];
	aafUInt32 _HeaderFilled;

	aafUInt64 _TotalBytesWritten;


	aafUInt32 _ComprID;
	bool	  _UseLegacyAvidUIDs; //true if the original Avid UIDs are to be used
	aafUInt32 _nCodecThreads;

	aafUInt32 _phys;		// physical slot number - small integer, default = 1
							// will be expanded by the codec for MXF GC

	aafUID_t  _eek;			// MXF GC Essence Element Key
							// codec will set default, taking _UseLegacyAvidUIDs nto account
							// application may override

	//variables from decoding header on read
	int _ALPF;
	int _SPL;
	int _SBD;

};

#endif // ! __CAAFDNxHDCodec_h__


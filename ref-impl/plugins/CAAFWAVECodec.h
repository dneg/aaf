//@doc
//@class    AAFEssenceCodec | Implementation class for AAFEssenceCodec
#ifndef __CAAFWAVECodec_h__
#define __CAAFWAVECodec_h__

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


#include "AAFUtils.h"
#include "AAF.h"
#include "AAFPlugin.h"
#include "CAAFUnknown.h"

EXTERN_C const CLSID CLSID_AAFWaveCodec;

typedef struct
{
	void			*buf;
	aafInt32		buflen;
	aafInt32		samplesLeft;
	aafInt32		bytesXfered;
	aafmMultiXfer_t	*xfer;
} interleaveBuf_t;

class CAAFWaveCodec
  : public IAAFEssenceCodec,
	public IAAFPlugin,
    public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFWaveCodec (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFWaveCodec ();

public:


  // Set up a codec.
  STDMETHOD (Start)
     (void);

  // Tear down a codec.
  STDMETHOD (Finish)
     (void);

  STDMETHOD (GetNumDefinitions)(aafInt32 *pDefCount);
  STDMETHOD (GetIndexedDefinitionID)(aafInt32 index, aafUID_t *result);
  STDMETHOD (GetPluginDescriptorID)(aafUID_t *result);
  STDMETHOD (GetEssenceDescriptorID)(aafUID_t *result);
  STDMETHOD (GetEssenceDataID)(aafUID_t *result);
  STDMETHOD (GetIndexedDefinitionObject)(aafInt32 index, IAAFDictionary *dict, IAAFDefObject **def);
  STDMETHOD (CreateDescriptor)(IAAFDictionary *dict, IAAFPluginDescriptor **desc);

  STDMETHOD (SetEssenceAccess)
    (/*[in]*/ IAAFEssenceAccess *access); // Set the AAFEssenceAccess used as a factory for AAFEssenceFormat

 STDMETHOD (GetFlavourCount)
    (/*[in]*/ aafInt32  *pCount);

	// Some codecs have variants handled by a single codec.
	// (For example, the Avid AVR codec handles multiple AVRs.)
	// The number of variants is returned by GetMetaInfo, and cached
	// by the AAFPluginManager.
	//
  STDMETHOD (GetIndexedFlavourID)
    (/*[in]*/ aafInt32  index, // Which variant to get the ID for
     /*[out]*/ aafUID_t *  pVariant); // The returned variant ID 

	
  // Sets a value indicating whether the SDK is handling the compression.
  STDMETHOD (SetCompressionEnabled)
    (/*[in]*/ aafBool  enable); // isCompressionEnabled 

  STDMETHOD (GetDataDefinitionCount)
        (/* [out] */ aafInt32 *pCount);

			// // All codecs handle at least one kind of media (picture, sound, control)
	// but some handle more than one.  The kind of media is specified by an
	// AAFDataDefinition.  The numnber of data definitions is returned by
	// GetMetaInfo, and cached by the AAFPluginManager.
  STDMETHOD (GetIndexedDataDefinition)
    (/*[in]*/ aafInt32  index, // Which data definition to get the ID for
     /*[out]*/ aafUID_t * pVariant); // The returned dataDefinition 

  STDMETHOD (GetMaxCodecDisplayNameLength)
    (aafUInt32  *bufSize);

	// Given a variant ID, return the human readable name
  STDMETHOD (GetCodecDisplayName)
    (/*[in]*/ aafUID_constref variant, // which variant of the codec to use
     /*[in,string]*/ aafCharacter *  pName, // Human-readable name of the variant
     /*[in]*/ aafUInt32  bufSize); // length of the buffer to hold variant Name 
	
  // Returns the number of channels which this codec can handle
			// of the given essence kind
  STDMETHOD (GetNumChannels)
    (/*[in]*/ IAAFSourceMob *fileMob, // Get the number of processable channels on this file mob
     /*[in]*/ aafUID_constref essenceKind, // This is the type of essence to open
	 IAAFEssenceStream *stream,
     /*[out]*/ aafInt16 *  pNumChannels); // The number of channels present 

  STDMETHOD (GetSelectInfo)
   (IAAFSourceMob *fileMob,
        IAAFEssenceStream *stream,
        aafSelectInfo_t *  pSelectInfo);

  // Returns the number of samples which this codec can find on the
			// given slot.
  STDMETHOD (GetNumSamples)
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
         aafInt32  bufSize,
		wchar_t *  pName,
        aafInt32  *bytesWritten);
		
  // Create a media data object, and attach the correct type of
			//EssenceDescriptor to the fileMob
  STDMETHOD (Create)
    (/*[in]*/ IAAFSourceMob *fileMob, // Create the essence attached to this file mob
     /*[in]*/ aafUID_constref variant, // which variant of the codec to use
        IAAFEssenceStream * stream,
        aafInt32 numParms,
        aafmMultiCreate_t *createParms);

  // Open a media data object.
  STDMETHOD (Open)
    (/*[in]*/ IAAFSourceMob *fileMob, // Open the essence attached to this file mob
        aafSlotID_t	slotID,
     /*[in]*/ aafMediaOpenMode_t  openMode, // In this mode
     /*[in]*/ IAAFEssenceStream * stream); // Here is an essence stream with the raw data 
	


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

  STDMETHOD (AddSampleIndexEntry)
    (aafPosition_t pos);


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
	aafUInt32			_sampleFrames;
	aafBool				_headerLoaded;
	interleaveBuf_t		*_interleaveBuf;
	aafInt64			_dataStartOffset;
	aafInt64			_dataSizeOffset;
	aafBool				_readOnly;
	aafBool				_sampleDataHeaderWritten;
	aafBool				_initialSeekPerformed;

	AAFRESULT fillSwappedWAVEData(aafUInt8 **destBufHdl, aafInt32 maxsize, void *data);
	AAFRESULT scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data);
	AAFRESULT CreateWAVEheader(aafUInt8 *buffer, aafInt32 bufsize, aafInt16 numCh);
	AAFRESULT loadWAVEHeader(void);
	AAFRESULT GetWAVEData(aafUInt32 len, void *buf);

	AAFRESULT ComputeWriteChunkSize(aafInt64 sizeOff, aafInt64 end);
	AAFRESULT CreateAudioDataEnd(void);
};

#endif // ! __CAAFEssenceCodec_h__


//@doc
//@class    AAFEssenceCodec | Implementation class for AAFEssenceCodec
#ifndef __CAAFWAVECodec_h__
#define __CAAFWAVECodec_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __AAF_h__
#include "AAF.h"
#endif

#include "AAFPlugin.h"

#ifndef __AAFDefaultCodec_h__
#include "CAAFDefaultCodec.h"
#endif

#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif

EXTERN_C const CLSID CLSID_AAFWaveCodec;

#if 0
// A Codec has a generic "plugin" interface, and a more specific "codec"
// interface.  Both of which are described in this .h file.
class CAAFWaveCodec
  : public CAAFDefaultCodec
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFWaveCodec (IUnknown * pControllingUnknown, aafBool doInit = AAFTrue);
  virtual ~CAAFWaveCodec ();

public:


  // Set up a codec.
  STDMETHOD (Start)
     ();

  // Tear down a codec.
  STDMETHOD (Finish)
     ();



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

  // Declare the module test method. The implementation of the will be be
  // in /test/CAAFPluginTest.cpp.
  static HRESULT test();
};
#endif

#if 1 
class CAAFWaveCodec
  : public CAAFDefaultCodec
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFWaveCodec (IUnknown * pControllingUnknown, aafBool doInit = AAFTrue);
  virtual ~CAAFWaveCodec ();

public:




  // Some codecs have variants handled by a single codec.
	// (For example, the Avid AVR codec handles multiple AVRs.)
	// The number of variants is returned by GetMetaInfo, and cached
	// by the AAFPluginManager.
	//
  STDMETHOD (GetIndexedVariantID)
    (/*[in]*/ aafInt32  index, // Which variant to get the ID for
     /*[out]*/ aafUID_t *  pVariant); // The returned variant ID 

	
  // Sets a value indicating whether the SDK is handling the compression.
  STDMETHOD (SetCompression)
    (/*[in]*/ aafCompressEnable_t  enable); // isCompressionEnabled 

  // // All codecs handle at least one kind of media (picture, sound, control)
	// but some handle more than one.  The kind of media is specified by an
	// AAFDataDefinition.  The numnber of data definitions is returned by
	// GetMetaInfo, and cached by the AAFPluginManager.
  STDMETHOD (GetIndexedDataDefinition)
    (/*[in]*/ aafInt32  index, // Which data definition to get the ID for
     /*[out]*/ aafUID_t *  pVariant); // The returned dataDefinition 

  // This function is called at startup by the plugin manager
  STDMETHOD (GetMetaInfo)
    (/*[out]*/ aafCodecMetaInfo_t *  pInfo); // The returned meta information about this codec 

  // Given a variant ID, return the human readable name
  STDMETHOD (GetCodecDisplayName)
    (/*[in]*/ aafUID_t  variant, // which variant of the codec to use
     /*[in,string]*/ wchar_t *  pName, // Human-readable name of the variant
     /*[in]*/ aafInt32  bufSize); // length of the buffer to hold variant Name 

  // This function is called when opening a media stream on a file mob.
  STDMETHOD (GetSelectInfo)
    (/*[in]*/ aafUID_t  fileMobID, // See if the current codec can process data from this file mob
     /*[out]*/ aafCodecSelectInfo_t *  pInfo); // The returned meta information about this codec 
	

	
  // Returns the number of channels which this codec can handle
			// of the given essence kind
  STDMETHOD (GetNumChannels)
    (/*[in]*/ aafUID_t  fileMobID, // Get the number of processable channels on this file mob
     /*[in]*/ aafUID_t  essenceKind, // This is the type of essence to open
     /*[out]*/ aafInt16 *  pNumChannels); // The number of channels present 

  // Returns a (possibly zero-length) string listing any problems
			//found with the essence, or the relationship between variables
			// in the essence descriptor, and any such values contained within
			// the essence data
  STDMETHOD (SemanticCheck)
    (/*[in]*/ aafUID_t  fileMobID, // Run a check on this file mob
     /*[in]*/ aafCheckVerbose_t  verbose, // This is the verbosity level of the output
     /*[out]*/ aafCheckWarnings_t *  warning, // This determines whether the output contains warnings
     /*[in,string]*/ wchar_t *  pName, // Human-readable text describing problems (or lack therof) with the media
     /*[in]*/ aafInt32  bufSize); // length of the buffer to hold variant Name 
		
  // Create a media data object, and attach the correct type of
			//EssenceDescriptor to the fileMob
  STDMETHOD (Create)
    (/*[in]*/ aafUID_t  fileMobID, // Create the essence attached to this file mob
     /*[in]*/ aafUID_t  variant, // which variant of the codec to use
     /*[in]*/ IAAFEssenceStream * stream); // Here is an essence stream with the raw data 

  // Open a media data object.
  STDMETHOD (Open)
    (/*[in]*/ aafUID_t  fileMobID, // Open the essence attached to this file mob
     /*[in]*/ aafMediaOpenMode_t  openMode, // In this mode
     /*[in]*/ IAAFEssenceStream * stream); // Here is an essence stream with the raw data 
	


  // Write blocks from one or more buffers, interleaving if needed.
  STDMETHOD (WriteBlocks)
    (/*[in]*/ aafDeinterleave_t  inter, // Whether the material will be de-interleaved on read
     /*[in]*/ aafInt16  xferBlockCount, // How many aafMultiXfer blocks follow
     /*[in]*/ aafmMultiXfer_t *  xferBlock); // One or more blocks containing buffer pointer and length 

  // Read blocks into one or more buffers, de-interleaving if needed.
  STDMETHOD (ReadBlocks)
    (/*[in]*/ aafDeinterleave_t  inter, // Whether the material will be de-interleaved on read
     /*[in]*/ aafInt16  xferBlockCount, // How many aafmMultiXfer blocks follow
     /*[in]*/ aafmMultiXfer_t *  xferBlock); // One or more blocks containing buffer pointer and length 

  // Seek to a particular sample frame on the media.  The
			// sample frame is one frame for picture, and one sample
			// times the number of interleaved channels for audio.
  STDMETHOD (Seek)
    (/*[in]*/ aafPosition_t  sampleFrame); // The sample frame to seek to. 

  // Close the media stream, ready to open another or Finish.
  STDMETHOD (Close)
     ();

		


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
    (/*[in,size_is(buflen)]*/ aafDataBuffer_t  buffer, // to a buffer
     /*[in]*/ aafInt32  buflen); // of this size 



  // Read some number of bytes from the stream exactly and with no formatting or compression.
  STDMETHOD (ReadRawData)
    (/*[in]*/ aafUInt32  buflen, // to a buffer of this size
     /*[out, size_is(buflen), length_is(*bytesRead)]*/ aafDataBuffer_t  buffer, // here is the buffer
     /*[out,ref]*/ aafUInt32 *  bytesRead); // Return bytes actually read 


	
  // Given some raw essence (like a WAVE file), create
			// an AAFEssenceDescriptor to match, with all fields filled in.
  STDMETHOD (CreateDescriptorFromStream)
    (/*[in]*/ IAAFEssenceStream * pStream, // A raw file stream
     /*[in]*/ aafUID_t  pSourceMobID); // Put the finished descriptor on this source mob 



  // Return the current essence stream.
  STDMETHOD (GetCurrentEssenceStream)
    (/*[out]*/ IAAFEssenceStream ** ppStream); // The current essence stream 
		
  // Set the format of the data.
  STDMETHOD (PutEssenceFormat)
    (/*[in]*/ IAAFEssenceFormat * pFormat); // An essence format object 

  // Supply an essence format object specifying what data is
			// required, and it is filled in with the data values.
  STDMETHOD (GetEssenceFormat)
    (/*[in,out]*/ IAAFEssenceFormat ** ppFormat); // An essence format object 


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

  // Declare the module test method. The implementation of the will be be
  // in /test/CAAFEssenceCodecTest.cpp.
  static HRESULT test();

private:
	IAAFEssenceStream	*_stream;
};
#endif

#endif // ! __CAAFEssenceCodec_h__


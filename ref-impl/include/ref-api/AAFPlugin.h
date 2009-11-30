//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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

//=--------------------------------------------------------------------------=
// AAF Plugin Interfaces.
//=--------------------------------------------------------------------------=
//
#ifndef __AAFPlugin_h__
#define __AAFPlugin_h__

#ifndef __AAF_h__
#include "AAF.h"
#endif

#ifndef __AAFPluginTypes_h__
#include "AAFPluginTypes.h"
#endif

#ifdef __cplusplus
interface IAAFClassExtension;
interface IAAFEssenceCodec;
interface IAAFEssenceCodec2;
interface IAAFEssenceCodec3;
interface IAAFEssenceContainer;
interface IAAFEssenceDataStream;
interface IAAFEssenceDataStream2;
interface IAAFEssenceStream;
interface IAAFInterpolator;
interface IAAFMultiEssenceCodec;
interface IAAFPlugin;
#else
typedef interface IAAFClassExtension IAAFClassExtension;
typedef interface IAAFEssenceCodec IAAFEssenceCodec;
typedef interface IAAFEssenceCodec2 IAAFEssenceCodec2;
typedef interface IAAFEssenceCodec3 IAAFEssenceCodec3;
typedef interface IAAFEssenceContainer IAAFEssenceContainer;
typedef interface IAAFEssenceDataStream IAAFEssenceDataStream;
typedef interface IAAFEssenceDataStream2 IAAFEssenceDataStream2;
typedef interface IAAFEssenceStream IAAFEssenceStream;
typedef interface IAAFInterpolator IAAFInterpolator;
typedef interface IAAFMultiEssenceCodec IAAFMultiEssenceCodec;
typedef interface IAAFPlugin IAAFPlugin;
#endif

// IAAFClassExtension

// ************************
//
// Interface IAAFClassExtension
//
// ************************






#ifndef __IAAFClassExtension_INTERFACE_DEFINED__
#define __IAAFClassExtension_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFClassExtension;

#undef  INTERFACE
#define INTERFACE   IAAFClassExtension

DECLARE_INTERFACE_(IAAFClassExtension, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFClassExtension methods *** */


  //***********************************************************
  //
  // RegisterDefinitions()
  //
  /// This method should register all type, class and property definitions
  /// into the given dictionary necessary to create instances of this 
  /// extension object..
  ///
  /// Succeeds if all of the following are true:
  /// - the pDictionary pointer is valid and all 
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDictionary arg is NULL.
  ///
  /// @param pDictionary [in] The dictionary to use to register definitions
  ///
  STDMETHOD(RegisterDefinitions) (THIS_
    IAAFDictionary * pDictionary) PURE;


  END_INTERFACE
};
#endif // __IAAFClassExtension_INTERFACE_DEFINED__



// IAAFEssenceCodec

// ************************
//
// Interface IAAFEssenceCodec
//
// ************************






#ifndef __IAAFEssenceCodec_INTERFACE_DEFINED__
#define __IAAFEssenceCodec_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFEssenceCodec;

#undef  INTERFACE
#define INTERFACE   IAAFEssenceCodec

DECLARE_INTERFACE_(IAAFEssenceCodec, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFEssenceCodec methods *** */




  //***********************************************************
  //
  // SetEssenceAccess()
  //
  /// Set the essence access associated with the codec.  This routine
  /// is called by the SDK when creating or opening an essence stream,
  /// in order to give the codec access to the AAFEssenceAccess
  /// object, because this is the factory object for creating
  /// AAFEssenceFormats.  It would be best to store away this pointer
  /// inside of the codec.  In order to avoid circular reference counting
  /// problems DO NOT AddRef this interface pointer. Also, DO NOT save
  /// any interface derived from the the saved IAAFEssenceAccess 
  /// pointer using QueryInterface unless the reference count is restored
  /// by a call to ReleaseReference.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// @param pEssenceAccess [in] The associated IAAFEssenceAccess
  ///
  STDMETHOD(SetEssenceAccess) (THIS_
    IAAFEssenceAccess * pEssenceAccess) PURE;



  //***********************************************************
  //
  // CountFlavours()
  //
  /// Some codecs have different variants or flavours of the media
  /// handled by a single codec.  (For example, the Avid AVR codec
  /// handles multiple AVRs.)  This call returns the number of
  /// flavours (usually one) supported by this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  ///
  /// @param pCount [out] Number of flavours supported
  ///
  STDMETHOD(CountFlavours) (THIS_
    aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedFlavourID()
  //
  /// Some codecs have flavour handled by a single codec.  (For
  /// example, the Avid AVR codec handles multiple AVRs.)  The number
  /// of flavours is returned by CountFlavours(), and cached by
  /// the AAFPluginManager. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pVariant is null.
  ///
  /// AAFRESULT_OUT_OF_RANGE
  ///   - index must be >= 0 and < the number of flavours returned
  ///     in CountFlavours().
  ///
  /// @param index [in] Which flavour to get the ID for
  /// @param pVariant [out] The returned flavour ID
  ///
  STDMETHOD(GetIndexedFlavourID) (THIS_
    aafUInt32  index,
    aafUID_t *  pVariant) PURE;


  //***********************************************************
  //
  // CountDataDefinitions()
  //
  /// Some codecs support formats which can encode multiple types of
  /// essence.  For example, interleaved video and audio.  This call
  /// returns the number of essence data definitions (usually one)
  /// supported by this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  ///
  /// @param pCount [out] Number of essence data definitions supported
  ///
  STDMETHOD(CountDataDefinitions) (THIS_
    aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedDataDefinition()
  //
  /// All codecs handle at least one kind of media (picture, sound,
  /// control) but some handle more than one.  The kind of media is
  /// specified by an AUID representing an AAFDataDef.  The number of
  /// data definitions is returned by CountDataDefinitions, and
  /// cached by the AAFPluginManager.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppDataDef is null.
  ///
  /// @param index [in] Which data definition to get
  /// @param pDataDefID [out] The returned dataDefinition ID
  ///
  STDMETHOD(GetIndexedDataDefinition) (THIS_
    aafUInt32  index,
    aafUID_t *  pDataDefID) PURE;


  //***********************************************************
  //
  // GetMaxCodecDisplayNameLength()
  //
  /// This method should return the buffer size required to hold the
  /// largest display name (and optional flavour) including the
  /// terminating NUL character.  The client code does not expect this
  /// to be equal to any particular display name.
  ///
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pBufSize is null.
  ///
  /// @param pBufSize [out] length of the buffer required to hold the longest flavour Name
  ///
  STDMETHOD(GetMaxCodecDisplayNameLength) (THIS_
    aafUInt32*  pBufSize) PURE;


  //***********************************************************
  //
  // GetCodecDisplayName()
  //
  /// Given a flavour ID, return the human readable name.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pName is null.
  ///
  /// @param flavour [in, ref] which flavour of the codec to use, or kNoCodecFlavour
  /// @param pName [out, string, size_is(bufSize)] Human-readable name of the flavour
  /// @param bufSize [in] length of the buffer to hold flavour Name
  ///
  STDMETHOD(GetCodecDisplayName) (THIS_
    aafUID_constref  flavour,
    aafCharacter *  pName,
    aafUInt32  bufSize) PURE;

	



  //***********************************************************
  //
  // CountChannels()
  //
  /// Returns the number of channels which this codec can handle of the
  /// given essence kind.  If the essence kind is not handled by this
  /// codec at all, then return zero channels through pNumChannels,
  /// and return AAFRESULT_SUCCESS.
  ///
  /// The AAFEssenceStream is owned by IAAFEssenceAccess, and
  /// therefore should not AddRef() it.  The codec may seek and read
  /// the supplied open stream as much as required to determine the
  /// number of channels, but should not assume that the Open method
  /// on the codec has been called (ie: it must parse any header
  /// itself). If the given IAAFEssenceStream pointer is NULL then
  /// the codec should look at the information in then source mob and 
  /// essence descriptor (in the mob).
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumChannels is null.
  ///
  /// @param fileMob [in] Get the number of processable channels on this file mob
  /// @param essenceKind [in, ref] This is the type of essence to open
  /// @param stream [in] Here is an essence stream with the data
  /// @param pNumChannels [out] The number of channels present
  ///
  STDMETHOD(CountChannels) (THIS_
    IAAFSourceMob * fileMob,
    aafUID_constref  essenceKind,
    IAAFEssenceStream * stream,
    aafUInt16 *  pNumChannels) PURE;


  //***********************************************************
  //
  // GetSelectInfo()
  //
  /// Returns a block of information required to select essence by
  /// criteria.  The SDK will use this criteria to find the best
  /// essence to fit requirements of size or speed.
  ///
  /// The AAFEssenceStream is owned by IAAFEssenceAccess, and
  /// therefore should not AddRef() it.  The codec may seek and read
  /// the supplied open stream as much as required to determine the
  /// number of channels, but should not assume that the Open method
  /// on the codec has been called (ie: it must parse any header
  /// itself). If the given IAAFEssenceStream pointer is NULL then
  /// the codec should look at the information in then source mob and 
  /// essence descriptor (in the mob).
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSelectInfo is null.
  ///
  /// @param fileMob [in] Get the number of processable channels on this file mob
  /// @param stream [in] Here is an essence stream with the data
  /// @param pSelectInfo [out] Information required to select by media criteria
  ///
  STDMETHOD(GetSelectInfo) (THIS_
    IAAFSourceMob * fileMob,
    IAAFEssenceStream * stream,
    aafSelectInfo_t *  pSelectInfo) PURE;


  //***********************************************************
  //
  // ValidateEssence()
  //
  /// This method validates the metadata associated with the essence
  /// data.  It may compare fields within the data, and also compare
  /// essence data against values in the IAAFFileDescriptor.
  /// 
  /// The fields bufSize, pErrorText, and pBytes read are used to
  /// return a textual description of any problems encountered.  If no
  /// problems were encountered, pBytesRead should be zero.
  /// 
  /// This method is used during client and SDK validation, and is not
  /// intended for general use.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pErrorText is null.
  ///
  /// @param fileMob [in] Run a check on this file mob
  /// @param stream [in] and this essence data stream
  /// @param verbose [in] This is the verbosity level of the output
  /// @param outputWarnings [in] This determines whether the output contains warnings
  /// @param bufSize [in] length of the buffer to hold the error result
  /// @param pErrorText [out, size_is(bufSize), length_is(*pBytesRead)] Human-readable text describing problems (or lack therof) with
  /// the essence
  /// @param pBytesRead [out] The number of result bytes returned
  ///
  STDMETHOD(ValidateEssence) (THIS_
    IAAFSourceMob * fileMob,
    IAAFEssenceStream * stream,
    aafCheckVerbose_t  verbose,
    aafCheckWarnings_t  outputWarnings,
    aafUInt32  bufSize,
    aafCharacter *  pErrorText,
    aafUInt32*  pBytesRead) PURE;


  //***********************************************************
  //
  // Create()
  //
  /// Initalize the stream to be of the correct format with no data and
  /// default metadata, attach the correct type of EssenceDescriptor
  /// to the fileMob, and file in the default metadata on the
  /// descriptor also.  Prepare the stream for writing the first sample
  /// of data.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// AAFRESULT_STREAM_FULL
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param fileMob [in] Create the essence attached to this file mob
  /// @param flavour [in, ref] which flavour of the codec to use
  /// @param essenceKind [in, ref] This is the type of essence to create
  /// @param sampleRate [in, ref] The sample rate for this essence
  /// @param stream [in] Here is an essence stream with the data
  /// @param compEnable [in] optionally compressing
  ///
  STDMETHOD(Create) (THIS_
    IAAFSourceMob * fileMob,
    aafUID_constref  flavour,
    aafUID_constref  essenceKind,
    aafRational_constref  sampleRate,
    IAAFEssenceStream * stream,
    aafCompressEnable_t  compEnable) PURE;


  //***********************************************************
  //
  // Open()
  //
  /// Read the metadata from the essence descriptor and/or the
  /// formatted data, and prepare the stream for reading the first
  /// sample of data.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// AAFRESULT_ESSENCE_NOT_FOUND
  ///   - The essence could not be located inside of the current file,
  ///     or by following any of the locators.
  ///
  /// @param fileMob [in] Open the essence attached to this file mob
  /// @param openMode [in] In this mode
  /// @param stream [in] Here is an essence stream with the raw data
  /// @param compEnable [in] optionally decompressing
  ///
  STDMETHOD(Open) (THIS_
    IAAFSourceMob * fileMob,
    aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t  compEnable) PURE;
	




  //***********************************************************
  //
  // CountSamples()
  //
  /// Return the number of samples present on a given essenceKind.  The
  /// assumption is made that all slots of the same essence kind have
  /// the same number of samples.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumSamples is null.
  ///
  /// @param essenceKind [in, ref] Tell how many samples of this type are on the mob
  /// @param pNumSamples [out] The number of samples present of the given essence kind
  ///
  STDMETHOD(CountSamples) (THIS_
    aafUID_constref  essenceKind,
    aafLength_t *  pNumSamples) PURE;


  //***********************************************************
  //
  // WriteSamples()
  //
  /// Writes data the given essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ZERO_SAMPLESIZE
  ///	 - The sample size of the stream has not been set.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - buffer or pResultParm is null.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - One of the supplied buffers is not large enough to hold the
  ///     given number of samples.
  ///
  /// AAFRESULT_CODEC_CHANNELS
  ///   - SPecified channel numbers are out of range
  ///
  /// AAFRESULT_CONTAINERWRITE
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param nSamples [in] write this many samples
  /// @param buflen [in] from a buffer of this size
  /// @param buffer [in,size_is(buflen)] from a buffer
  /// @param samplesWritten [out, ref] number of samples actually written
  /// @param bytesWritten [out, ref] number of bytes actually written
  ///
  STDMETHOD(WriteSamples) (THIS_
    aafUInt32  nSamples,
    aafUInt32  buflen,
    aafDataBuffer_t  buffer,
    aafUInt32 *  samplesWritten,
    aafUInt32 *  bytesWritten) PURE;


  //***********************************************************
  //
  // ReadSamples()
  //
  /// Read a given number of samples from an opened essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one  encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTransferParm is null.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - The buffer is not large enough to hold the data
  ///
  /// AAFRESULT_CODEC_CHANNELS
  ///   - SPecified channel numbers are out of range
  ///
  /// AAFRESULT_END_OF_DATA
  ///   - Hit either the end-of-file on a raw essence file, or the
  ///     end of the essence property.  The bytesRead fields inside of
  ///     pTransferParm correctly reflect the number of bytes and
  ///     complete samples on each track.
  ///
  /// @param nSamples [in] Read this many samples
  /// @param buflen [in] into a buffer of this size
  /// @param buffer [out, size_is(buflen),length_is(*bytesRead)] The transfer buffer
  /// @param samplesRead [out, ref] The number of samples actually read
  /// @param bytesRead [out, ref] The number of bytes actually read
  ///
  STDMETHOD(ReadSamples) (THIS_
    aafUInt32  nSamples,
    aafUInt32  buflen,
    aafDataBuffer_t  buffer,
    aafUInt32 *  samplesRead,
    aafUInt32 *  bytesRead) PURE;


  //***********************************************************
  //
  // Seek()
  //
  /// Seek to a particular sample frame on the media.  The sample frame
  /// is one frame for picture, and one sample times the number of
  /// interleaved channels for audio or other interleaved formats.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  ///
  /// AAFRESULT_BADSAMPLEOFFSET
  ///   - The supplied sample offset is out of range
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.) 
  ///
  /// @param sampleFrame [in] The sample frame to seek to.
  ///
  STDMETHOD(Seek) (THIS_
    aafPosition_t  sampleFrame) PURE;


  //***********************************************************
  //
  // CompleteWrite()
  //
  /// Close the essence stream, ready to open another or Finish.  In
  /// the case  where the client has created essence in an external AAF
  /// file, two essence descriptors will need to be updated.  After
  /// updating the essence descriptor for the file which you are
  /// writing (like updating the number of samples), find the
  /// essence descriptor associated with pFileMob, and make the same
  /// changes to that essence desriptor.
  ///
  /// NOTE that pFileMob may be NULL in all cases except writing
  /// essence to an external AAF file.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescriptor is null.
  ///
  /// AAFRESULT_STREAM_FULL
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param pFileMob [in] Optional second essence descriptor to also update
  ///
  STDMETHOD(CompleteWrite) (THIS_
    IAAFSourceMob * pFileMob) PURE;



	

  //***********************************************************
  //
  // CreateDescriptorFromStream()
  //
  /// Given some raw essence (like a WAVE file), create an
  /// AAFEssenceDescriptor to match, with all fields filled in. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pStream or pSourceMob is null.
  ///
  /// @param pStream [in] A raw file stream
  /// @param pSourceMob [in] Put the finished descriptor on this source mob
  ///
  STDMETHOD(CreateDescriptorFromStream) (THIS_
    IAAFEssenceStream * pStream,
    IAAFSourceMob * pSourceMob) PURE;


  //***********************************************************
  //
  // GetCurrentEssenceStream()
  //
  /// Return the current essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppStream is null.
  ///
  /// @param ppStream [out] The current essence stream
  ///
  STDMETHOD(GetCurrentEssenceStream) (THIS_
    IAAFEssenceStream ** ppStream) PURE;

	
  //***********************************************************
  //
  // PutEssenceFormat()
  //
  /// Set the format of the one or more parameters.  The client should
  /// have called GetDefaultEssenceFormat() in order to find out what
  /// format codes are supported, but you may still get format codes
  /// out of range.
  ///
  /// The expected bevhavior of this routine is to pre-scan the list
  /// for valid format codes and lengths before setting any data, and
  /// returning AAFRESULT_INVALID_OP_CODEC.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_FILEFMT
  ///   - One of the format codes is not supported by this codec.  Call
  ///   GetDefaultEssenceFormat() in order to find the list of
  ///   supported format codes.  No data will have been transferred.
  ///
  /// AAFRESULT_INVALID_PARM_SIZE
  ///   - The data supplied with the parameter is not of the correct
  ///     size.  No data will have been transferred.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFormat is null.
  ///
  /// @param pFormat [in] An essence format object
  ///
  STDMETHOD(PutEssenceFormat) (THIS_
    IAAFEssenceFormat * pFormat) PURE;


  //***********************************************************
  //
  // GetEssenceFormat()
  //
  /// Get the format of the one or more parameters by scanning the list
  /// of format codes supplied, and filling in the data values.  The
  /// client should have called GetDefaultEssenceFormat() in order to
  /// find out what format codes are supported, but you may still get
  /// format codes out of range.
  ///
  /// The expected bevhavior of this routine is to pre-scan the list
  /// for valid format codes before setting any data, and returning
  /// AAFRESULT_INVALID_OP_CODEC.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_FILEFMT
  ///   - One of the format codes is not supported by this codec.  Call
  ///     GetDefaultEssenceFormat() in order to find the list of
  ///     supported format codes.  No data will have been transferred.
  ///
  /// AAFRESULT_INVALID_PARM_SIZE
  ///   - The data supplied with the parameter is not of the correct
  ///     size.  No data will have been transferred.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFormatTemplate or ppNewFormat is null.
  ///
  /// @param pFormatTemplate [in] An essence format object to use as a template of which fields to read
  /// @param ppNewFormat [out] A new essence format object with the data filled in
  ///
  STDMETHOD(GetEssenceFormat) (THIS_
    IAAFEssenceFormat * pFormatTemplate,
    IAAFEssenceFormat ** ppNewFormat) PURE;

		
  //***********************************************************
  //
  // GetDefaultEssenceFormat()
  //
  /// Returns an essence format object specifying which contains the
  /// complete list of essence formats supported by this codec, along
  /// with the default values.  Previous systems required you to
  /// "know" what parameters a given codec could support.  With this
  /// function, a client application can check what the default
  /// parameter would be, and change only those which are required.
  /// It differs from GetEssenceFormat() in that it
  /// GetEssenceFormat() returns the current client-set settings.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppNewFormat is null.
  ///
  /// @param ppNewFormat [out] An essence format with all valid parameter names and values
	/// filled in
  ///
  STDMETHOD(GetDefaultEssenceFormat) (THIS_
    IAAFEssenceFormat ** ppNewFormat) PURE;


  //***********************************************************
  //
  // GetEssenceDescriptorID()
  //
  /// Returns the stored object AUID of the essence descriptor class
  /// associated with this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescriptorID is null.
  ///
  /// @param pDescriptorID [out] The stored object AUID of the essence descriptor class
  ///
  STDMETHOD(GetEssenceDescriptorID) (THIS_
    aafUID_t *  pDescriptorID) PURE;


  //***********************************************************
  //
  // GetEssenceDataID()
  //
  /// Returns the stored object AUID of the essence data class
  /// associated with this codec. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceDataID is null.
  ///
  /// @param pEssenceDataID [out] The stored object AUID of the essence data class
  ///
  STDMETHOD(GetEssenceDataID) (THIS_
    aafUID_t *  pEssenceDataID) PURE;


  //***********************************************************
  //
  // GetIndexedSampleSize()
  //
  /// Returns the size of the sample at the given offset, and of the
  /// given data definition.  For formats with fixed sample sizes
  /// (without a sample offset table), this method should return the
  /// standard sample size.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLength is null.
  ///
  /// @param essenceDefID [in, ref] The ID of the data definition describing the essence
  /// @param sampleOffset [in] The zero-based offset to get the size of
  /// @param pLength [out] The returned length of the given sample
  ///
  STDMETHOD(GetIndexedSampleSize) (THIS_
    aafUID_constref  essenceDefID,
    aafPosition_t  sampleOffset,
    aafLength_t *  pLength) PURE;


  //***********************************************************
  //
  // GetLargestSampleSize()
  //
  /// Returns the size of the largest sample, and of the given data
  /// definition.  For formats with fixed sample sizes (without a
  /// sample offset table), this method should return the standard
  /// sample size.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLength is null.
  ///
  /// @param essenceDefID [in, ref] The ID of the data definition of the essence
  /// @param pLength [out] The returned length of the largest sample
  ///
  STDMETHOD(GetLargestSampleSize) (THIS_
    aafUID_constref  essenceDefID,
    aafLength_t *  pLength) PURE;








  END_INTERFACE
};
#endif // __IAAFEssenceCodec_INTERFACE_DEFINED__


// IAAFEssenceCodec2

// ************************
//
// Interface IAAFEssenceCodec2
//
// ************************







#ifndef __IAAFEssenceCodec2_INTERFACE_DEFINED__
#define __IAAFEssenceCodec2_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFEssenceCodec2;

#undef  INTERFACE
#define INTERFACE   IAAFEssenceCodec2

DECLARE_INTERFACE_(IAAFEssenceCodec2, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFEssenceCodec2 methods *** */





  //***********************************************************
  //
  // SetEssenceAccess()
  //
  /// Set the essence access associated with the codec.  This routine
  /// is called by the SDK when creating or opening an essence stream,
  /// in order to give the codec access to the AAFEssenceAccess
  /// object, because this is the factory object for creating
  /// AAFEssenceFormats.  It would be best to store away this pointer
  /// inside of the codec.  In order to avoid circular reference counting
  /// problems DO NOT AddRef this interface pointer. Also, DO NOT save
  /// any interface derived from the the saved IAAFEssenceAccess 
  /// pointer using QueryInterface unless the reference count is restored
  /// by a call to ReleaseReference.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// @param pEssenceAccess [in] The associated IAAFEssenceAccess
  ///
  STDMETHOD(SetEssenceAccess) (THIS_
    IAAFEssenceAccess * pEssenceAccess) PURE;



  //***********************************************************
  //
  // CountFlavours()
  //
  /// Some codecs have different variants or flavours of the media
  /// handled by a single codec.  (For example, the Avid AVR codec
  /// handles multiple AVRs.)  This call returns the number of
  /// flavours (usually one) supported by this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  ///
  /// @param pCount [out] Number of flavours supported
  ///
  STDMETHOD(CountFlavours) (THIS_
    aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedFlavourID()
  //
  /// Some codecs have flavour handled by a single codec.  (For
  /// example, the Avid AVR codec handles multiple AVRs.)  The number
  /// of flavours is returned by CountFlavours(), and cached by
  /// the AAFPluginManager. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pVariant is null.
  ///
  /// AAFRESULT_OUT_OF_RANGE
  ///   - index must be >= 0 and < the number of flavours returned
  ///     in CountFlavours().
  ///
  /// @param index [in] Which flavour to get the ID for
  /// @param pVariant [out] The returned flavour ID
  ///
  STDMETHOD(GetIndexedFlavourID) (THIS_
    aafUInt32  index,
    aafUID_t *  pVariant) PURE;


  //***********************************************************
  //
  // CountDataDefinitions()
  //
  /// Some codecs support formats which can encode multiple types of
  /// essence.  For example, interleaved video and audio.  This call
  /// returns the number of essence data definitions (usually one)
  /// supported by this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  ///
  /// @param pCount [out] Number of essence data definitions supported
  ///
  STDMETHOD(CountDataDefinitions) (THIS_
    aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedDataDefinition()
  //
  /// All codecs handle at least one kind of media (picture, sound,
  /// control) but some handle more than one.  The kind of media is
  /// specified by an AUID representing an AAFDataDef.  The number of
  /// data definitions is returned by CountDataDefinitions, and
  /// cached by the AAFPluginManager.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppDataDef is null.
  ///
  /// @param index [in] Which data definition to get
  /// @param pDataDefID [out] The returned dataDefinition ID
  ///
  STDMETHOD(GetIndexedDataDefinition) (THIS_
    aafUInt32  index,
    aafUID_t *  pDataDefID) PURE;


  //***********************************************************
  //
  // GetMaxCodecDisplayNameLength()
  //
  /// This method should return the buffer size required to hold the
  /// largest display name (and optional flavour) including the
  /// terminating NUL character.  The client code does not expect this
  /// to be equal to any particular display name.
  ///
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pBufSize is null.
  ///
  /// @param pBufSize [out] length of the buffer required to hold the longest flavour Name
  ///
  STDMETHOD(GetMaxCodecDisplayNameLength) (THIS_
    aafUInt32*  pBufSize) PURE;


  //***********************************************************
  //
  // GetCodecDisplayName()
  //
  /// Given a flavour ID, return the human readable name.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pName is null.
  ///
  /// @param flavour [in, ref] which flavour of the codec to use, or kNoCodecFlavour
  /// @param pName [out, string, size_is(bufSize)] Human-readable name of the flavour
  /// @param bufSize [in] length of the buffer to hold flavour Name
  ///
  STDMETHOD(GetCodecDisplayName) (THIS_
    aafUID_constref  flavour,
    aafCharacter *  pName,
    aafUInt32  bufSize) PURE;

	



  //***********************************************************
  //
  // CountChannels()
  //
  /// Returns the number of channels which this codec can handle of the
  /// given essence kind.  If the essence kind is not handled by this
  /// codec at all, then return zero channels through pNumChannels,
  /// and return AAFRESULT_SUCCESS.
  ///
  /// The AAFEssenceStream is owned by IAAFEssenceAccess, and
  /// therefore should not AddRef() it.  The codec may seek and read
  /// the supplied open stream as much as required to determine the
  /// number of channels, but should not assume that the Open method
  /// on the codec has been called (ie: it must parse any header
  /// itself). If the given IAAFEssenceStream pointer is NULL then
  /// the codec should look at the information in then source mob and 
  /// essence descriptor (in the mob).
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumChannels is null.
  ///
  /// @param fileMob [in] Get the number of processable channels on this file mob
  /// @param essenceKind [in, ref] This is the type of essence to open
  /// @param stream [in] Here is an essence stream with the data
  /// @param pNumChannels [out] The number of channels present
  ///
  STDMETHOD(CountChannels) (THIS_
    IAAFSourceMob * fileMob,
    aafUID_constref  essenceKind,
    IAAFEssenceStream * stream,
    aafUInt16 *  pNumChannels) PURE;


  //***********************************************************
  //
  // GetSelectInfo()
  //
  /// Returns a block of information required to select essence by
  /// criteria.  The SDK will use this criteria to find the best
  /// essence to fit requirements of size or speed.
  ///
  /// The AAFEssenceStream is owned by IAAFEssenceAccess, and
  /// therefore should not AddRef() it.  The codec may seek and read
  /// the supplied open stream as much as required to determine the
  /// number of channels, but should not assume that the Open method
  /// on the codec has been called (ie: it must parse any header
  /// itself). If the given IAAFEssenceStream pointer is NULL then
  /// the codec should look at the information in then source mob and 
  /// essence descriptor (in the mob).
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSelectInfo is null.
  ///
  /// @param fileMob [in] Get the number of processable channels on this file mob
  /// @param stream [in] Here is an essence stream with the data
  /// @param pSelectInfo [out] Information required to select by media criteria
  ///
  STDMETHOD(GetSelectInfo) (THIS_
    IAAFSourceMob * fileMob,
    IAAFEssenceStream * stream,
    aafSelectInfo_t *  pSelectInfo) PURE;


  //***********************************************************
  //
  // ValidateEssence()
  //
  /// This method validates the metadata associated with the essence
  /// data.  It may compare fields within the data, and also compare
  /// essence data against values in the IAAFFileDescriptor.
  /// 
  /// The fields bufSize, pErrorText, and pBytes read are used to
  /// return a textual description of any problems encountered.  If no
  /// problems were encountered, pBytesRead should be zero.
  /// 
  /// This method is used during client and SDK validation, and is not
  /// intended for general use.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pErrorText is null.
  ///
  /// @param fileMob [in] Run a check on this file mob
  /// @param stream [in] and this essence data stream
  /// @param verbose [in] This is the verbosity level of the output
  /// @param outputWarnings [in] This determines whether the output contains warnings
  /// @param bufSize [in] length of the buffer to hold the error result
  /// @param pErrorText [out, size_is(bufSize), length_is(*pBytesRead)] Human-readable text describing problems (or lack therof) with
  /// the essence
  /// @param pBytesRead [out] The number of result bytes returned
  ///
  STDMETHOD(ValidateEssence) (THIS_
    IAAFSourceMob * fileMob,
    IAAFEssenceStream * stream,
    aafCheckVerbose_t  verbose,
    aafCheckWarnings_t  outputWarnings,
    aafUInt32  bufSize,
    aafCharacter *  pErrorText,
    aafUInt32*  pBytesRead) PURE;


  //***********************************************************
  //
  // Create()
  //
  /// Initalize the stream to be of the correct format with no data and
  /// default metadata, attach the correct type of EssenceDescriptor
  /// to the fileMob, and file in the default metadata on the
  /// descriptor also.  Prepare the stream for writing the first sample
  /// of data.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// AAFRESULT_STREAM_FULL
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param fileMob [in] Create the essence attached to this file mob
  /// @param flavour [in, ref] which flavour of the codec to use
  /// @param essenceKind [in, ref] This is the type of essence to create
  /// @param sampleRate [in, ref] The sample rate for this essence
  /// @param stream [in] Here is an essence stream with the data
  /// @param compEnable [in] optionally compressing
  ///
  STDMETHOD(Create) (THIS_
    IAAFSourceMob * fileMob,
    aafUID_constref  flavour,
    aafUID_constref  essenceKind,
    aafRational_constref  sampleRate,
    IAAFEssenceStream * stream,
    aafCompressEnable_t  compEnable) PURE;


  //***********************************************************
  //
  // Open()
  //
  /// Read the metadata from the essence descriptor and/or the
  /// formatted data, and prepare the stream for reading the first
  /// sample of data.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// AAFRESULT_ESSENCE_NOT_FOUND
  ///   - The essence could not be located inside of the current file,
  ///     or by following any of the locators.
  ///
  /// @param fileMob [in] Open the essence attached to this file mob
  /// @param openMode [in] In this mode
  /// @param stream [in] Here is an essence stream with the raw data
  /// @param compEnable [in] optionally decompressing
  ///
  STDMETHOD(Open) (THIS_
    IAAFSourceMob * fileMob,
    aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t  compEnable) PURE;
	




  //***********************************************************
  //
  // CountSamples()
  //
  /// Return the number of samples present on a given essenceKind.  The
  /// assumption is made that all slots of the same essence kind have
  /// the same number of samples.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumSamples is null.
  ///
  /// @param essenceKind [in, ref] Tell how many samples of this type are on the mob
  /// @param pNumSamples [out] The number of samples present of the given essence kind
  ///
  STDMETHOD(CountSamples) (THIS_
    aafUID_constref  essenceKind,
    aafLength_t *  pNumSamples) PURE;


  //***********************************************************
  //
  // WriteSamples()
  //
  /// Writes data the given essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ZERO_SAMPLESIZE
  ///	 - The sample size of the stream has not been set.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - buffer or pResultParm is null.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - One of the supplied buffers is not large enough to hold the
  ///     given number of samples.
  ///
  /// AAFRESULT_CODEC_CHANNELS
  ///   - SPecified channel numbers are out of range
  ///
  /// AAFRESULT_CONTAINERWRITE
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param nSamples [in] write this many samples
  /// @param buflen [in] from a buffer of this size
  /// @param buffer [in,size_is(buflen)] from a buffer
  /// @param samplesWritten [out, ref] number of samples actually written
  /// @param bytesWritten [out, ref] number of bytes actually written
  ///
  STDMETHOD(WriteSamples) (THIS_
    aafUInt32  nSamples,
    aafUInt32  buflen,
    aafDataBuffer_t  buffer,
    aafUInt32 *  samplesWritten,
    aafUInt32 *  bytesWritten) PURE;


  //***********************************************************
  //
  // ReadSamples()
  //
  /// Read a given number of samples from an opened essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one  encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTransferParm is null.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - The buffer is not large enough to hold the data
  ///
  /// AAFRESULT_CODEC_CHANNELS
  ///   - SPecified channel numbers are out of range
  ///
  /// AAFRESULT_END_OF_DATA
  ///   - Hit either the end-of-file on a raw essence file, or the
  ///     end of the essence property.  The bytesRead fields inside of
  ///     pTransferParm correctly reflect the number of bytes and
  ///     complete samples on each track.
  ///
  /// @param nSamples [in] Read this many samples
  /// @param buflen [in] into a buffer of this size
  /// @param buffer [out, size_is(buflen),length_is(*bytesRead)] The transfer buffer
  /// @param samplesRead [out, ref] The number of samples actually read
  /// @param bytesRead [out, ref] The number of bytes actually read
  ///
  STDMETHOD(ReadSamples) (THIS_
    aafUInt32  nSamples,
    aafUInt32  buflen,
    aafDataBuffer_t  buffer,
    aafUInt32 *  samplesRead,
    aafUInt32 *  bytesRead) PURE;


  //***********************************************************
  //
  // Seek()
  //
  /// Seek to a particular sample frame on the media.  The sample frame
  /// is one frame for picture, and one sample times the number of
  /// interleaved channels for audio or other interleaved formats.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  ///
  /// AAFRESULT_BADSAMPLEOFFSET
  ///   - The supplied sample offset is out of range
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.) 
  ///
  /// @param sampleFrame [in] The sample frame to seek to.
  ///
  STDMETHOD(Seek) (THIS_
    aafPosition_t  sampleFrame) PURE;


  //***********************************************************
  //
  // CompleteWrite()
  //
  /// Close the essence stream, ready to open another or Finish.  In
  /// the case  where the client has created essence in an external AAF
  /// file, two essence descriptors will need to be updated.  After
  /// updating the essence descriptor for the file which you are
  /// writing (like updating the number of samples), find the
  /// essence descriptor associated with pFileMob, and make the same
  /// changes to that essence desriptor.
  ///
  /// NOTE that pFileMob may be NULL in all cases except writing
  /// essence to an external AAF file.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescriptor is null.
  ///
  /// AAFRESULT_STREAM_FULL
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param pFileMob [in] Optional second essence descriptor to also update
  ///
  STDMETHOD(CompleteWrite) (THIS_
    IAAFSourceMob * pFileMob) PURE;



	

  //***********************************************************
  //
  // CreateDescriptorFromStream()
  //
  /// Given some raw essence (like a WAVE file), create an
  /// AAFEssenceDescriptor to match, with all fields filled in. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pStream or pSourceMob is null.
  ///
  /// @param pStream [in] A raw file stream
  /// @param pSourceMob [in] Put the finished descriptor on this source mob
  ///
  STDMETHOD(CreateDescriptorFromStream) (THIS_
    IAAFEssenceStream * pStream,
    IAAFSourceMob * pSourceMob) PURE;


  //***********************************************************
  //
  // GetCurrentEssenceStream()
  //
  /// Return the current essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppStream is null.
  ///
  /// @param ppStream [out] The current essence stream
  ///
  STDMETHOD(GetCurrentEssenceStream) (THIS_
    IAAFEssenceStream ** ppStream) PURE;

	
  //***********************************************************
  //
  // PutEssenceFormat()
  //
  /// Set the format of the one or more parameters.  The client should
  /// have called GetDefaultEssenceFormat() in order to find out what
  /// format codes are supported, but you may still get format codes
  /// out of range.
  ///
  /// The expected bevhavior of this routine is to pre-scan the list
  /// for valid format codes and lengths before setting any data, and
  /// returning AAFRESULT_INVALID_OP_CODEC.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_FILEFMT
  ///   - One of the format codes is not supported by this codec.  Call
  ///   GetDefaultEssenceFormat() in order to find the list of
  ///   supported format codes.  No data will have been transferred.
  ///
  /// AAFRESULT_INVALID_PARM_SIZE
  ///   - The data supplied with the parameter is not of the correct
  ///     size.  No data will have been transferred.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFormat is null.
  ///
  /// @param pFormat [in] An essence format object
  ///
  STDMETHOD(PutEssenceFormat) (THIS_
    IAAFEssenceFormat * pFormat) PURE;


  //***********************************************************
  //
  // GetEssenceFormat()
  //
  /// Get the format of the one or more parameters by scanning the list
  /// of format codes supplied, and filling in the data values.  The
  /// client should have called GetDefaultEssenceFormat() in order to
  /// find out what format codes are supported, but you may still get
  /// format codes out of range.
  ///
  /// The expected bevhavior of this routine is to pre-scan the list
  /// for valid format codes before setting any data, and returning
  /// AAFRESULT_INVALID_OP_CODEC.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_FILEFMT
  ///   - One of the format codes is not supported by this codec.  Call
  ///     GetDefaultEssenceFormat() in order to find the list of
  ///     supported format codes.  No data will have been transferred.
  ///
  /// AAFRESULT_INVALID_PARM_SIZE
  ///   - The data supplied with the parameter is not of the correct
  ///     size.  No data will have been transferred.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFormatTemplate or ppNewFormat is null.
  ///
  /// @param pFormatTemplate [in] An essence format object to use as a template of which fields to read
  /// @param ppNewFormat [out] A new essence format object with the data filled in
  ///
  STDMETHOD(GetEssenceFormat) (THIS_
    IAAFEssenceFormat * pFormatTemplate,
    IAAFEssenceFormat ** ppNewFormat) PURE;

		
  //***********************************************************
  //
  // GetDefaultEssenceFormat()
  //
  /// Returns an essence format object specifying which contains the
  /// complete list of essence formats supported by this codec, along
  /// with the default values.  Previous systems required you to
  /// "know" what parameters a given codec could support.  With this
  /// function, a client application can check what the default
  /// parameter would be, and change only those which are required.
  /// It differs from GetEssenceFormat() in that it
  /// GetEssenceFormat() returns the current client-set settings.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppNewFormat is null.
  ///
  /// @param ppNewFormat [out] An essence format with all valid parameter names and values
	/// filled in
  ///
  STDMETHOD(GetDefaultEssenceFormat) (THIS_
    IAAFEssenceFormat ** ppNewFormat) PURE;


  //***********************************************************
  //
  // GetEssenceDescriptorID()
  //
  /// Returns the stored object AUID of the essence descriptor class
  /// associated with this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescriptorID is null.
  ///
  /// @param pDescriptorID [out] The stored object AUID of the essence descriptor class
  ///
  STDMETHOD(GetEssenceDescriptorID) (THIS_
    aafUID_t *  pDescriptorID) PURE;


  //***********************************************************
  //
  // GetEssenceDataID()
  //
  /// Returns the stored object AUID of the essence data class
  /// associated with this codec. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceDataID is null.
  ///
  /// @param pEssenceDataID [out] The stored object AUID of the essence data class
  ///
  STDMETHOD(GetEssenceDataID) (THIS_
    aafUID_t *  pEssenceDataID) PURE;


  //***********************************************************
  //
  // GetIndexedSampleSize()
  //
  /// Returns the size of the sample at the given offset, and of the
  /// given data definition.  For formats with fixed sample sizes
  /// (without a sample offset table), this method should return the
  /// standard sample size.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLength is null.
  ///
  /// @param essenceDefID [in, ref] The ID of the data definition describing the essence
  /// @param sampleOffset [in] The zero-based offset to get the size of
  /// @param pLength [out] The returned length of the given sample
  ///
  STDMETHOD(GetIndexedSampleSize) (THIS_
    aafUID_constref  essenceDefID,
    aafPosition_t  sampleOffset,
    aafLength_t *  pLength) PURE;


  //***********************************************************
  //
  // GetLargestSampleSize()
  //
  /// Returns the size of the largest sample, and of the given data
  /// definition.  For formats with fixed sample sizes (without a
  /// sample offset table), this method should return the standard
  /// sample size.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLength is null.
  ///
  /// @param essenceDefID [in, ref] The ID of the data definition of the essence
  /// @param pLength [out] The returned length of the largest sample
  ///
  STDMETHOD(GetLargestSampleSize) (THIS_
    aafUID_constref  essenceDefID,
    aafLength_t *  pLength) PURE;


  //***********************************************************
  //
  // SetFlavour()
  //
  /// Switches the codec into the specified flavour.  Flavour is
  /// codec dependent.  For example, the DV codec could use a
  /// flavour to switch into a legacy mode causing legacy properties
  /// to be added which are needed by legacy applications.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// HRESULT_NOT_IMPLEMENTED
  ///   - flavour not implemented by the codec
  ///
  /// @param flavour [in, ref] the codec flavour to use
  ///
  STDMETHOD(SetFlavour) (THIS_
    aafUID_constref  flavour) PURE;


  END_INTERFACE
};
#endif // __IAAFEssenceCodec2_INTERFACE_DEFINED__



// IAAFEssenceCodec3

// ************************
//
// Interface IAAFEssenceCodec3
//
// ************************







#ifndef __IAAFEssenceCodec3_INTERFACE_DEFINED__
#define __IAAFEssenceCodec3_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFEssenceCodec3;

#undef  INTERFACE
#define INTERFACE   IAAFEssenceCodec3

DECLARE_INTERFACE_(IAAFEssenceCodec3, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFEssenceCodec3 methods *** */





  //***********************************************************
  //
  // SetEssenceAccess()
  //
  /// Set the essence access associated with the codec.  This routine
  /// is called by the SDK when creating or opening an essence stream,
  /// in order to give the codec access to the AAFEssenceAccess
  /// object, because this is the factory object for creating
  /// AAFEssenceFormats.  It would be best to store away this pointer
  /// inside of the codec.  In order to avoid circular reference counting
  /// problems DO NOT AddRef this interface pointer. Also, DO NOT save
  /// any interface derived from the the saved IAAFEssenceAccess 
  /// pointer using QueryInterface unless the reference count is restored
  /// by a call to ReleaseReference.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// @param pEssenceAccess [in] The associated IAAFEssenceAccess
  ///
  STDMETHOD(SetEssenceAccess) (THIS_
    IAAFEssenceAccess * pEssenceAccess) PURE;



  //***********************************************************
  //
  // CountFlavours()
  //
  /// Some codecs have different variants or flavours of the media
  /// handled by a single codec.  (For example, the Avid AVR codec
  /// handles multiple AVRs.)  This call returns the number of
  /// flavours (usually one) supported by this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  ///
  /// @param pCount [out] Number of flavours supported
  ///
  STDMETHOD(CountFlavours) (THIS_
    aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedFlavourID()
  //
  /// Some codecs have flavour handled by a single codec.  (For
  /// example, the Avid AVR codec handles multiple AVRs.)  The number
  /// of flavours is returned by CountFlavours(), and cached by
  /// the AAFPluginManager. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pVariant is null.
  ///
  /// AAFRESULT_OUT_OF_RANGE
  ///   - index must be >= 0 and < the number of flavours returned
  ///     in CountFlavours().
  ///
  /// @param index [in] Which flavour to get the ID for
  /// @param pVariant [out] The returned flavour ID
  ///
  STDMETHOD(GetIndexedFlavourID) (THIS_
    aafUInt32  index,
    aafUID_t *  pVariant) PURE;


  //***********************************************************
  //
  // CountDataDefinitions()
  //
  /// Some codecs support formats which can encode multiple types of
  /// essence.  For example, interleaved video and audio.  This call
  /// returns the number of essence data definitions (usually one)
  /// supported by this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  ///
  /// @param pCount [out] Number of essence data definitions supported
  ///
  STDMETHOD(CountDataDefinitions) (THIS_
    aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedDataDefinition()
  //
  /// All codecs handle at least one kind of media (picture, sound,
  /// control) but some handle more than one.  The kind of media is
  /// specified by an AUID representing an AAFDataDef.  The number of
  /// data definitions is returned by CountDataDefinitions, and
  /// cached by the AAFPluginManager.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppDataDef is null.
  ///
  /// @param index [in] Which data definition to get
  /// @param pDataDefID [out] The returned dataDefinition ID
  ///
  STDMETHOD(GetIndexedDataDefinition) (THIS_
    aafUInt32  index,
    aafUID_t *  pDataDefID) PURE;


  //***********************************************************
  //
  // GetMaxCodecDisplayNameLength()
  //
  /// This method should return the buffer size required to hold the
  /// largest display name (and optional flavour) including the
  /// terminating NUL character.  The client code does not expect this
  /// to be equal to any particular display name.
  ///
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pBufSize is null.
  ///
  /// @param pBufSize [out] length of the buffer required to hold the longest flavour Name
  ///
  STDMETHOD(GetMaxCodecDisplayNameLength) (THIS_
    aafUInt32*  pBufSize) PURE;


  //***********************************************************
  //
  // GetCodecDisplayName()
  //
  /// Given a flavour ID, return the human readable name.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pName is null.
  ///
  /// @param flavour [in, ref] which flavour of the codec to use, or kNoCodecFlavour
  /// @param pName [out, string, size_is(bufSize)] Human-readable name of the flavour
  /// @param bufSize [in] length of the buffer to hold flavour Name
  ///
  STDMETHOD(GetCodecDisplayName) (THIS_
    aafUID_constref  flavour,
    aafCharacter *  pName,
    aafUInt32  bufSize) PURE;

	



  //***********************************************************
  //
  // CountChannels()
  //
  /// Returns the number of channels which this codec can handle of the
  /// given essence kind.  If the essence kind is not handled by this
  /// codec at all, then return zero channels through pNumChannels,
  /// and return AAFRESULT_SUCCESS.
  ///
  /// The AAFEssenceStream is owned by IAAFEssenceAccess, and
  /// therefore should not AddRef() it.  The codec may seek and read
  /// the supplied open stream as much as required to determine the
  /// number of channels, but should not assume that the Open method
  /// on the codec has been called (ie: it must parse any header
  /// itself). If the given IAAFEssenceStream pointer is NULL then
  /// the codec should look at the information in then source mob and 
  /// essence descriptor (in the mob).
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumChannels is null.
  ///
  /// @param fileMob [in] Get the number of processable channels on this file mob
  /// @param essenceKind [in, ref] This is the type of essence to open
  /// @param stream [in] Here is an essence stream with the data
  /// @param pNumChannels [out] The number of channels present
  ///
  STDMETHOD(CountChannels) (THIS_
    IAAFSourceMob * fileMob,
    aafUID_constref  essenceKind,
    IAAFEssenceStream * stream,
    aafUInt16 *  pNumChannels) PURE;


  //***********************************************************
  //
  // GetSelectInfo()
  //
  /// Returns a block of information required to select essence by
  /// criteria.  The SDK will use this criteria to find the best
  /// essence to fit requirements of size or speed.
  ///
  /// The AAFEssenceStream is owned by IAAFEssenceAccess, and
  /// therefore should not AddRef() it.  The codec may seek and read
  /// the supplied open stream as much as required to determine the
  /// number of channels, but should not assume that the Open method
  /// on the codec has been called (ie: it must parse any header
  /// itself). If the given IAAFEssenceStream pointer is NULL then
  /// the codec should look at the information in then source mob and 
  /// essence descriptor (in the mob).
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pSelectInfo is null.
  ///
  /// @param fileMob [in] Get the number of processable channels on this file mob
  /// @param stream [in] Here is an essence stream with the data
  /// @param pSelectInfo [out] Information required to select by media criteria
  ///
  STDMETHOD(GetSelectInfo) (THIS_
    IAAFSourceMob * fileMob,
    IAAFEssenceStream * stream,
    aafSelectInfo_t *  pSelectInfo) PURE;


  //***********************************************************
  //
  // ValidateEssence()
  //
  /// This method validates the metadata associated with the essence
  /// data.  It may compare fields within the data, and also compare
  /// essence data against values in the IAAFFileDescriptor.
  /// 
  /// The fields bufSize, pErrorText, and pBytes read are used to
  /// return a textual description of any problems encountered.  If no
  /// problems were encountered, pBytesRead should be zero.
  /// 
  /// This method is used during client and SDK validation, and is not
  /// intended for general use.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pErrorText is null.
  ///
  /// @param fileMob [in] Run a check on this file mob
  /// @param stream [in] and this essence data stream
  /// @param verbose [in] This is the verbosity level of the output
  /// @param outputWarnings [in] This determines whether the output contains warnings
  /// @param bufSize [in] length of the buffer to hold the error result
  /// @param pErrorText [out, size_is(bufSize), length_is(*pBytesRead)] Human-readable text describing problems (or lack therof) with
  /// the essence
  /// @param pBytesRead [out] The number of result bytes returned
  ///
  STDMETHOD(ValidateEssence) (THIS_
    IAAFSourceMob * fileMob,
    IAAFEssenceStream * stream,
    aafCheckVerbose_t  verbose,
    aafCheckWarnings_t  outputWarnings,
    aafUInt32  bufSize,
    aafCharacter *  pErrorText,
    aafUInt32*  pBytesRead) PURE;


  //***********************************************************
  //
  // Create()
  //
  /// Initalize the stream to be of the correct format with no data and
  /// default metadata, attach the correct type of EssenceDescriptor
  /// to the fileMob, and file in the default metadata on the
  /// descriptor also.  Prepare the stream for writing the first sample
  /// of data.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// AAFRESULT_STREAM_FULL
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param fileMob [in] Create the essence attached to this file mob
  /// @param flavour [in, ref] which flavour of the codec to use
  /// @param essenceKind [in, ref] This is the type of essence to create
  /// @param sampleRate [in, ref] The sample rate for this essence
  /// @param stream [in] Here is an essence stream with the data
  /// @param compEnable [in] optionally compressing
  ///
  STDMETHOD(Create) (THIS_
    IAAFSourceMob * fileMob,
    aafUID_constref  flavour,
    aafUID_constref  essenceKind,
    aafRational_constref  sampleRate,
    IAAFEssenceStream * stream,
    aafCompressEnable_t  compEnable) PURE;


  //***********************************************************
  //
  // Open()
  //
  /// Read the metadata from the essence descriptor and/or the
  /// formatted data, and prepare the stream for reading the first
  /// sample of data.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceAccess is null.
  ///
  /// AAFRESULT_ESSENCE_NOT_FOUND
  ///   - The essence could not be located inside of the current file,
  ///     or by following any of the locators.
  ///
  /// @param fileMob [in] Open the essence attached to this file mob
  /// @param openMode [in] In this mode
  /// @param stream [in] Here is an essence stream with the raw data
  /// @param compEnable [in] optionally decompressing
  ///
  STDMETHOD(Open) (THIS_
    IAAFSourceMob * fileMob,
    aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t  compEnable) PURE;
	




  //***********************************************************
  //
  // CountSamples()
  //
  /// Return the number of samples present on a given essenceKind.  The
  /// assumption is made that all slots of the same essence kind have
  /// the same number of samples.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumSamples is null.
  ///
  /// @param essenceKind [in, ref] Tell how many samples of this type are on the mob
  /// @param pNumSamples [out] The number of samples present of the given essence kind
  ///
  STDMETHOD(CountSamples) (THIS_
    aafUID_constref  essenceKind,
    aafLength_t *  pNumSamples) PURE;


  //***********************************************************
  //
  // WriteSamples()
  //
  /// Writes data the given essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ZERO_SAMPLESIZE
  ///	 - The sample size of the stream has not been set.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - buffer or pResultParm is null.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - One of the supplied buffers is not large enough to hold the
  ///     given number of samples.
  ///
  /// AAFRESULT_CODEC_CHANNELS
  ///   - SPecified channel numbers are out of range
  ///
  /// AAFRESULT_CONTAINERWRITE
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param nSamples [in] write this many samples
  /// @param buflen [in] from a buffer of this size
  /// @param buffer [in,size_is(buflen)] from a buffer
  /// @param samplesWritten [out, ref] number of samples actually written
  /// @param bytesWritten [out, ref] number of bytes actually written
  ///
  STDMETHOD(WriteSamples) (THIS_
    aafUInt32  nSamples,
    aafUInt32  buflen,
    aafDataBuffer_t  buffer,
    aafUInt32 *  samplesWritten,
    aafUInt32 *  bytesWritten) PURE;


  //***********************************************************
  //
  // ReadSamples()
  //
  /// Read a given number of samples from an opened essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one  encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTransferParm is null.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - The buffer is not large enough to hold the data
  ///
  /// AAFRESULT_CODEC_CHANNELS
  ///   - SPecified channel numbers are out of range
  ///
  /// AAFRESULT_END_OF_DATA
  ///   - Hit either the end-of-file on a raw essence file, or the
  ///     end of the essence property.  The bytesRead fields inside of
  ///     pTransferParm correctly reflect the number of bytes and
  ///     complete samples on each track.
  ///
  /// @param nSamples [in] Read this many samples
  /// @param buflen [in] into a buffer of this size
  /// @param buffer [out, size_is(buflen),length_is(*bytesRead)] The transfer buffer
  /// @param samplesRead [out, ref] The number of samples actually read
  /// @param bytesRead [out, ref] The number of bytes actually read
  ///
  STDMETHOD(ReadSamples) (THIS_
    aafUInt32  nSamples,
    aafUInt32  buflen,
    aafDataBuffer_t  buffer,
    aafUInt32 *  samplesRead,
    aafUInt32 *  bytesRead) PURE;


  //***********************************************************
  //
  // Seek()
  //
  /// Seek to a particular sample frame on the media.  The sample frame
  /// is one frame for picture, and one sample times the number of
  /// interleaved channels for audio or other interleaved formats.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  ///
  /// AAFRESULT_BADSAMPLEOFFSET
  ///   - The supplied sample offset is out of range
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.) 
  ///
  /// @param sampleFrame [in] The sample frame to seek to.
  ///
  STDMETHOD(Seek) (THIS_
    aafPosition_t  sampleFrame) PURE;


  //***********************************************************
  //
  // CompleteWrite()
  //
  /// Close the essence stream, ready to open another or Finish.  In
  /// the case  where the client has created essence in an external AAF
  /// file, two essence descriptors will need to be updated.  After
  /// updating the essence descriptor for the file which you are
  /// writing (like updating the number of samples), find the
  /// essence descriptor associated with pFileMob, and make the same
  /// changes to that essence desriptor.
  ///
  /// NOTE that pFileMob may be NULL in all cases except writing
  /// essence to an external AAF file.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescriptor is null.
  ///
  /// AAFRESULT_STREAM_FULL
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param pFileMob [in] Optional second essence descriptor to also update
  ///
  STDMETHOD(CompleteWrite) (THIS_
    IAAFSourceMob * pFileMob) PURE;



	

  //***********************************************************
  //
  // CreateDescriptorFromStream()
  //
  /// Given some raw essence (like a WAVE file), create an
  /// AAFEssenceDescriptor to match, with all fields filled in. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pStream or pSourceMob is null.
  ///
  /// @param pStream [in] A raw file stream
  /// @param pSourceMob [in] Put the finished descriptor on this source mob
  ///
  STDMETHOD(CreateDescriptorFromStream) (THIS_
    IAAFEssenceStream * pStream,
    IAAFSourceMob * pSourceMob) PURE;


  //***********************************************************
  //
  // GetCurrentEssenceStream()
  //
  /// Return the current essence stream.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppStream is null.
  ///
  /// @param ppStream [out] The current essence stream
  ///
  STDMETHOD(GetCurrentEssenceStream) (THIS_
    IAAFEssenceStream ** ppStream) PURE;

	
  //***********************************************************
  //
  // PutEssenceFormat()
  //
  /// Set the format of the one or more parameters.  The client should
  /// have called GetDefaultEssenceFormat() in order to find out what
  /// format codes are supported, but you may still get format codes
  /// out of range.
  ///
  /// The expected bevhavior of this routine is to pre-scan the list
  /// for valid format codes and lengths before setting any data, and
  /// returning AAFRESULT_INVALID_OP_CODEC.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_FILEFMT
  ///   - One of the format codes is not supported by this codec.  Call
  ///   GetDefaultEssenceFormat() in order to find the list of
  ///   supported format codes.  No data will have been transferred.
  ///
  /// AAFRESULT_INVALID_PARM_SIZE
  ///   - The data supplied with the parameter is not of the correct
  ///     size.  No data will have been transferred.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFormat is null.
  ///
  /// @param pFormat [in] An essence format object
  ///
  STDMETHOD(PutEssenceFormat) (THIS_
    IAAFEssenceFormat * pFormat) PURE;


  //***********************************************************
  //
  // GetEssenceFormat()
  //
  /// Get the format of the one or more parameters by scanning the list
  /// of format codes supplied, and filling in the data values.  The
  /// client should have called GetDefaultEssenceFormat() in order to
  /// find out what format codes are supported, but you may still get
  /// format codes out of range.
  ///
  /// The expected bevhavior of this routine is to pre-scan the list
  /// for valid format codes before setting any data, and returning
  /// AAFRESULT_INVALID_OP_CODEC.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ILLEGAL_FILEFMT
  ///   - One of the format codes is not supported by this codec.  Call
  ///     GetDefaultEssenceFormat() in order to find the list of
  ///     supported format codes.  No data will have been transferred.
  ///
  /// AAFRESULT_INVALID_PARM_SIZE
  ///   - The data supplied with the parameter is not of the correct
  ///     size.  No data will have been transferred.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFormatTemplate or ppNewFormat is null.
  ///
  /// @param pFormatTemplate [in] An essence format object to use as a template of which fields to read
  /// @param ppNewFormat [out] A new essence format object with the data filled in
  ///
  STDMETHOD(GetEssenceFormat) (THIS_
    IAAFEssenceFormat * pFormatTemplate,
    IAAFEssenceFormat ** ppNewFormat) PURE;

		
  //***********************************************************
  //
  // GetDefaultEssenceFormat()
  //
  /// Returns an essence format object specifying which contains the
  /// complete list of essence formats supported by this codec, along
  /// with the default values.  Previous systems required you to
  /// "know" what parameters a given codec could support.  With this
  /// function, a client application can check what the default
  /// parameter would be, and change only those which are required.
  /// It differs from GetEssenceFormat() in that it
  /// GetEssenceFormat() returns the current client-set settings.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppNewFormat is null.
  ///
  /// @param ppNewFormat [out] An essence format with all valid parameter names and values
	/// filled in
  ///
  STDMETHOD(GetDefaultEssenceFormat) (THIS_
    IAAFEssenceFormat ** ppNewFormat) PURE;


  //***********************************************************
  //
  // GetEssenceDescriptorID()
  //
  /// Returns the stored object AUID of the essence descriptor class
  /// associated with this codec.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDescriptorID is null.
  ///
  /// @param pDescriptorID [out] The stored object AUID of the essence descriptor class
  ///
  STDMETHOD(GetEssenceDescriptorID) (THIS_
    aafUID_t *  pDescriptorID) PURE;


  //***********************************************************
  //
  // GetEssenceDataID()
  //
  /// Returns the stored object AUID of the essence data class
  /// associated with this codec. 
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceDataID is null.
  ///
  /// @param pEssenceDataID [out] The stored object AUID of the essence data class
  ///
  STDMETHOD(GetEssenceDataID) (THIS_
    aafUID_t *  pEssenceDataID) PURE;


  //***********************************************************
  //
  // GetIndexedSampleSize()
  //
  /// Returns the size of the sample at the given offset, and of the
  /// given data definition.  For formats with fixed sample sizes
  /// (without a sample offset table), this method should return the
  /// standard sample size.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLength is null.
  ///
  /// @param essenceDefID [in, ref] The ID of the data definition describing the essence
  /// @param sampleOffset [in] The zero-based offset to get the size of
  /// @param pLength [out] The returned length of the given sample
  ///
  STDMETHOD(GetIndexedSampleSize) (THIS_
    aafUID_constref  essenceDefID,
    aafPosition_t  sampleOffset,
    aafLength_t *  pLength) PURE;


  //***********************************************************
  //
  // GetLargestSampleSize()
  //
  /// Returns the size of the largest sample, and of the given data
  /// definition.  For formats with fixed sample sizes (without a
  /// sample offset table), this method should return the standard
  /// sample size.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pLength is null.
  ///
  /// @param essenceDefID [in, ref] The ID of the data definition of the essence
  /// @param pLength [out] The returned length of the largest sample
  ///
  STDMETHOD(GetLargestSampleSize) (THIS_
    aafUID_constref  essenceDefID,
    aafLength_t *  pLength) PURE;


  //***********************************************************
  //
  // SetFlavour()
  //
  /// Switches the codec into the specified flavour.  Flavour is
  /// codec dependent.  For example, the DV codec could use a
  /// flavour to switch into a legacy mode causing legacy properties
  /// to be added which are needed by legacy applications.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// HRESULT_NOT_IMPLEMENTED
  ///   - flavour not implemented by the codec
  ///
  /// @param flavour [in, ref] the codec flavour to use
  ///
  STDMETHOD(SetFlavour) (THIS_
    aafUID_constref  flavour) PURE;


  //***********************************************************
  //
  // IsCompressionSupported()
  //
  /// Returns AAFTrue if the codec supports opening essence with
  /// the specified compression ID.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pIsSupported pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pIsSupported.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pIsSupported is null.
  ///
  /// @param compression [in] The compression to test for support
  /// @param pIsSupported [out,retval] Is this compression supported
  ///
  STDMETHOD(IsCompressionSupported) (THIS_
    aafUID_constref  compression,
    aafBoolean_t*  pIsSupported) PURE;


  END_INTERFACE
};
#endif // __IAAFEssenceCodec3_INTERFACE_DEFINED__



// IAAFEssenceContainer

// ************************
//
// Interface IAAFEssenceContainer
//
// ************************


#ifndef __IAAFEssenceContainer_INTERFACE_DEFINED__
#define __IAAFEssenceContainer_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFEssenceContainer;

#undef  INTERFACE
#define INTERFACE   IAAFEssenceContainer

DECLARE_INTERFACE_(IAAFEssenceContainer, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFEssenceContainer methods *** */

  //***********************************************************
  //
  // CreateEssenceStream()
  //
  /// Attempt to create an essence stream in the container 
  /// for reading and writing.
  /// Returns one of the following:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - both pName and pMobID cannot be NULL.
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - pName or pMobID (or both) are invalid
  ///
  /// AAFRESULT_FILE_EXISTS
  ///   - the given path and or mobID  already points to an existing stream 
  ///     within the container.
  ///
  /// @param pName [in, string] String key to identify the new essence within container
  /// @param pMobID [in] Optional mobID identifying the essence
  /// @param ppEssenceStream [out, retval] address of local variable to contain the essence stream pointer.
  ///
  STDMETHOD(CreateEssenceStream) (THIS_
    aafCharacter_constptr  pName,
    aafMobID_constptr  pMobID,
    IAAFEssenceStream ** ppEssenceStream) PURE;


  //***********************************************************
  //
  // CreateEssenceStreamWriteOnly()
  //
  /// Attempt to create an essence stream in the container 
  /// for writing.
  /// Returns one of the following:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - both pName and pMobID cannot be NULL.
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - pName or pMobID (or both) are invalid
  ///
  /// AAFRESULT_FILE_EXISTS
  ///   - the given path and or mobID  already points to an existing stream 
  ///     within the container.
  ///
  /// @param pPath [in, string] String key to identify the new essence within container
  /// @param pMobID [in] Optional mobID identifying the essence
  /// @param ppEssenceStream [out, retval] address of local variable to contain the essence stream pointer.
  ///
  STDMETHOD(CreateEssenceStreamWriteOnly) (THIS_
    aafCharacter_constptr  pPath,
    aafMobID_constptr  pMobID,
    IAAFEssenceStream ** ppEssenceStream) PURE;


  //***********************************************************
  //
  // OpenEssenceStreamReadOnly()
  //
  /// Attempt to open an essence stream for reading.
  /// Returns one of the following:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - both pName and pMobID cannot be NULL.
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - pName or pMobID (or both) are invalid
  ///
  /// AAFRESULT_MEDIA_NOT_FOUND
  ///   - the given path does not point to an existing stream within 
  ///     the container.
  ///
  /// AAFRESULT_NOT_READABLE
  ///   - stream is write-only, cannot be opened for reading.
  ///
  /// @param pFilePath [in, string] String key to identify the existing essence within container
  /// @param pMobID [in] Optional mobID identifying the essence
  /// @param ppEssenceStream [out, retval] address of local variable to contain the essence stream pointer.
  ///
  STDMETHOD(OpenEssenceStreamReadOnly) (THIS_
    aafCharacter_constptr  pFilePath,
    aafMobID_constptr  pMobID,
    IAAFEssenceStream ** ppEssenceStream) PURE;


  //***********************************************************
  //
  // OpenEssenceStreamAppend()
  //
  /// Attempt to open an essence stream for appending.
  /// Returns one of the following:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - both pName and pMobID cannot be NULL.
  ///
  /// AAFRESULT_ILLEGAL_VALUE
  ///   - pName or pMobID (or both) are invalid
  ///
  /// AAFRESULT_MEDIA_NOT_FOUND
  ///   - the given path does not point to an existing stream within 
  ///     the container.
  ///
  /// AAFRESULT_NOT_WRITEABLE
  ///   - stream is read-only, cannot be opened for writing.
  ///
  /// @param pFilePath [in, string] String key to identify the existing essence within container
  /// @param pMobID [in] Optional mobID identifying the essence
  /// @param ppEssenceStream [out, retval] address of local variable to contain the essence stream pointer.
  ///
  STDMETHOD(OpenEssenceStreamAppend) (THIS_
    aafCharacter_constptr  pFilePath,
    aafMobID_constptr  pMobID,
    IAAFEssenceStream ** ppEssenceStream) PURE;

  END_INTERFACE
};
#endif // __IAAFEssenceContainer_INTERFACE_DEFINED__



// IAAFEssenceDataStream

// ************************
//
// Interface IAAFEssenceDataStream
//
// ************************





#ifndef __IAAFEssenceDataStream_INTERFACE_DEFINED__
#define __IAAFEssenceDataStream_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFEssenceDataStream;

#undef  INTERFACE
#define INTERFACE   IAAFEssenceDataStream

DECLARE_INTERFACE_(IAAFEssenceDataStream, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFEssenceDataStream methods *** */


  //***********************************************************
  //
  // Init()
  //
  /// Init the stream over a particular EssenceData.
  ///
  /// @param essenceData [in] The EssenceData to stream over
  ///
  STDMETHOD(Init) (THIS_
    IUnknown * essenceData) PURE;


  END_INTERFACE
};
#endif // __IAAFEssenceDataStream_INTERFACE_DEFINED__




// IAAFEssenceDataStream2

// ************************
//
// Interface IAAFEssenceDataStream2
//
// ************************





#ifndef __IAAFEssenceDataStream2_INTERFACE_DEFINED__
#define __IAAFEssenceDataStream2_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFEssenceDataStream2;


#undef  INTERFACE
#define INTERFACE   IAAFEssenceDataStream2

DECLARE_INTERFACE_(IAAFEssenceDataStream2, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFEssenceDataStream2 methods *** */


  //***********************************************************
  //
  // Init()
  //
  /// Init the stream over a particular EssenceData.
  ///
  /// @param essenceData [in] The EssenceData to stream over
  ///
  STDMETHOD(Init) (THIS_
    IUnknown * essenceData) PURE;

  //***********************************************************
  //
  // GetEssenceData()
  //
  /// Returns the EssenceData with which the EssenceDataStream2 was initialized, in
/// the ppEssenceData argument.  The returned EssenceData interface is AddRef()ed
/// before it is returned.
/// 
/// Succeeds if all of the following are true:
/// - the ppEssenceData pointer is valid.
/// 
/// If this method fails nothing will be written to *ppEssenceData.
/// 
/// This method will return the following codes.  If more than one of
/// the listed errors is in effect, it will return the first one
/// encountered in the order given below:
/// 
/// AAFRESULT_SUCCESS
///   - succeeded.  (This is the only code indicating success.)
///
/// AAFRESULT_NULL_PARAM
///   - ppEssenceData is null.
  ///
  /// @param ppEssenceData [out,retval] The EssenceData this stream applies to
  ///
  STDMETHOD(GetEssenceData) (THIS_
    IAAFEssenceData ** ppEssenceData) PURE;

  //***********************************************************
  //
  // SetEssenceElementKey()
  //
  /// Set the essence element key on the essence stream 
  /// and optionally sets the Physical Track Number on a slot of the associated File Mob.
  ///
  /// If the EssenceStream does not support essence element keys, does not update the Physical Num,
  /// fails gracefully and returns
  ///		AAFRESULT_OPERATION_NOT_PERMITTED
  /// this is NOT an error
  ///
  /// If the Essence Element Key input parameter is NULL, uses SMPTE 379M Generic Container keys
  /// 
  /// Constructs the 4 least significant bytes of the essence element key from the parameters
  ///		kind	- essence kind (ex. 0x15 for GC Picture - refer to SMPTE 379M)
  ///		count	- essence count (normally 1 for mono essence)
  ///		type	- essence type (ex. 0x0D for VC-3 clip-wrapped - refer to essence mapping documents)
  ///		index	- integer from 1 up to count (normally 1 for mon essence)
  /// if any of these is zero, does not override the 4 least significant bytes of the essence element key
  ///
  /// If the sourceSlotID input parameter is not 0, sets thte PhysicalNum of that slot to match
  /// the 4 least significant bytes of the essence element key
  ///
  /// Succeeds if all of the following are true:
  /// - the essence stream supports essence element keys.
  /// 
  /// If this method fails, the property will not be changed.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_OPERATION_NOT_PERMITTED
  ///   - the essence stream does not support essence element keys.
  ///
  /// @param eek [in, ref] essence element key
  /// @param eeKind [in] Generic Container essence kind
  /// @param eeCount [in] Generic Container essence element count
  /// @param eeType [in] Generic Container essence element type
  /// @param eeIndex [in] Generic Container essence element index
  /// @param sourceSlotID [in] Slot ID of the Source Mob slot to be adjusted
  ///
  STDMETHOD(SetEssenceElementKey) (THIS_
    aafUID_constref  eek,
    aafUInt8  eeKind,
    aafUInt8  eeCount,
    aafUInt8  eeType,
    aafUInt8  eeIndex,
    aafSlotID_t  sourceSlotID) PURE;

  //***********************************************************
  //
  // GetEssenceElementKey()
  //
  /// This method returns the essence element key associated
  /// with the essence stream.
  ///
  /// Succeeds if all of the following are true:
  /// - pEssenceElementKey is a valid pointer.
  /// - the essence stream supports essence element keys.
  ///
  /// If this method fails nothing will be written to *pEssenceElementKey.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - This object has not yet had Initialize() called on it.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pEssenceElementKey arg is NULL.
  ///
  /// AAFRESULT_OPERATION_NOT_PERMITTED
  ///   - the essence stream does not support essence element keys.
  ///
  /// @param pEssenceElementKey [out] Essence element key.
  ///
  STDMETHOD(GetEssenceElementKey) (THIS_
    aafUID_t *  pEssenceElementKey) PURE;


  END_INTERFACE
};
#endif // __IAAFEssenceDataStream2_INTERFACE_DEFINED__




// IAAFEssenceStream

// ************************
//
// Interface IAAFEssenceStream
//
// ************************




#ifndef __IAAFEssenceStream_INTERFACE_DEFINED__
#define __IAAFEssenceStream_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFEssenceStream;

#undef  INTERFACE
#define INTERFACE   IAAFEssenceStream

DECLARE_INTERFACE_(IAAFEssenceStream, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFEssenceStream methods *** */


  //***********************************************************
  //
  // Write()
  //
  /// Write some number of bytes to the stream exactly and with no formatting or compression.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NULL_PARAM
	///   - pBuffer or pBytesRead is null.
	///
	/// AAFRESULT_STREAM_FULL
	///   - The essence can not be written because of a fault such as a disk full error in the
	/// underlying operating system.
  ///
  /// @param bytes [in] the number of bytes to write
  /// @param buffer [out, size_is(bytes)] the buffer that contains at least bytes
  /// @param bytesWritten [out,ref] the number of bytes actually written from the buffer
  ///
  STDMETHOD(Write) (THIS_
    aafUInt32  bytes,
    aafDataBuffer_t  buffer,
    aafUInt32 *  bytesWritten) PURE;


  //***********************************************************
  //
  // Read()
  //
  /// Read some number of bytes from the stream exactly and with no formatting or compression.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NULL_PARAM
	///   - pBuffer or pBytesRead is null.
	///
	/// AAFRESULT_END_OF_ESSENCE
	///   - Hit either the end-of-file on a raw essence file, or the end of the essence property.
	///	The pBytesRead parameter correctly reflects the number of bytes actually read.
  ///
  /// @param buflen [in] to a buffer of this size
  /// @param pBuffer [out, size_is(buflen), length_is(*pBytesRead)] here is the buffer
  /// @param pBytesRead [out,ref] Return bytes actually read
  ///
  STDMETHOD(Read) (THIS_
    aafUInt32  buflen,
    aafDataBuffer_t  pBuffer,
    aafUInt32 *  pBytesRead) PURE;

  //***********************************************************
  //
  // Seek()
  //
  /// Seek to the absolute byte offset into the stream.
  ///
  /// @param byteOffset [in] The absolute byte offset into the stream.
  ///
  STDMETHOD(Seek) (THIS_
    aafPosition_t  byteOffset) PURE;
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect\\\, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  \\\(This is the only code indicating success.\\\)
	///
	/// AAFRESULT_STREAM_BOUNDS
	///   - The new position would be outside of the bounds of the stream.)

  //***********************************************************
  //
  // SeekRelative()
  //
  /// Seek forward or backward the given byte count.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_STREAM_BOUNDS
	///   - The new position would be outside of the bounds of the stream.
  ///
  /// @param byteOffset [in] The relative byte offset into the stream.
  ///
  STDMETHOD(SeekRelative) (THIS_
    aafInt32  byteOffset) PURE;

  //***********************************************************
  //
  // GetPosition()
  //
  /// Returns the position within the stream.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NULL_PARAM
	///   - pPosition is null.
  ///
  /// @param pPosition [out] The position within the stream.
  ///
  STDMETHOD(GetPosition) (THIS_
    aafPosition_t *  pPosition) PURE;

  //***********************************************************
  //
  // GetLength()
  //
  /// Returns the length of the stream.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NULL_PARAM
	///   - pLength is null.
  ///
  /// @param pLength [out] The length of the stream.
  ///
  STDMETHOD(GetLength) (THIS_
    aafLength_t *  pLength) PURE;



  //***********************************************************
  //
  // FlushCache()
  //
  /// Ensure that all bits are written.  The caller of this interface
	/// is required to call FlushCache before releasing the stream.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect\, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  \(This is the only code indicating success.\)
	/// 
	/// AAFRESULT_STREAM_FULL
	///   - The essence can not be written because of a fault such as a disk full error in the
	/// underlying operating system.
  STDMETHOD(FlushCache) (THIS) PURE;

  //***********************************************************
  //
  // SetCacheSize()
  //
  /// Sets the size of the cache buffer used for further operations.
			/// Destroys the current contents of the cache.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.) 
  ///
  /// @param itsSize [in] The size of the cache buffer.  Zero is a valid size, and means to turn caching off
  ///
  STDMETHOD(SetCacheSize) (THIS_
    aafUInt32  itsSize) PURE;


  END_INTERFACE
};
#endif // __IAAFEssenceStream_INTERFACE_DEFINED__




// IAAFInterpolator

// ************************
//
// Interface IAAFInterpolator
//
// ************************




#ifndef __IAAFInterpolator_INTERFACE_DEFINED__
#define __IAAFInterpolator_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFInterpolator;

#undef  INTERFACE
#define INTERFACE   IAAFInterpolator

DECLARE_INTERFACE_(IAAFInterpolator, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFInterpolator methods *** */




  //***********************************************************
  //
  // GetNumTypesSupported()
  //
  /// Each interpolator plugin will support a given number of interpolator
	/// types for a given number of data types.  This call returns the number of
	/// data types supported, so that the SDK can call GetIndexedSupportedType()
	/// repeatedly in order to find out what types this interpolator suppports.
	///
	/// An interpolator is expected to support all data types for each interpolation
	/// method which the interpolator supports.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NULL_PARAM
	///   - pCount is null.
  ///
  /// @param pCount [out] Number of data types supported
  ///
  STDMETHOD(GetNumTypesSupported) (THIS_
    aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedSupportedType()
  //
  /// Each interpolator plugin will support a given number of interpolator
	/// types for a given number of data types.  This call returns one of the data
	/// types supported using an index.  The index value should be between 0 and
	/// one less than the value returned by GetNumTypesSupported().
	/// The number of flavours is returned by GetNumTypesSupported(), and cached
	/// by the AAFPluginManager.
	///
	/// An interpolator is expected to support all data types for each interpolation
	/// method which the interpolator supports.
	/// 
	/// This method should return only the following codes.  If more than one of
	/// the listed errors is in effect, it should return the first one
	/// encountered in the order given below:
	/// 
	/// AAFRESULT_SUCCESS
	///   - succeeded.  (This is the only code indicating success.)
	///
	/// AAFRESULT_NULL_PARAM
	///   - pVariant is null.
	///
	/// AAFRESULT_OUT_OF_RANGE
	///   - index must be >= 0 and < the number of flavours returned in GetNumTypesSupported(). 
  ///
  /// @param index [in] Which flavour to get the ID for
  /// @param ppType [out] The returned type definition
  ///
  STDMETHOD(GetIndexedSupportedType) (THIS_
    aafUInt32  index,
    IAAFTypeDef ** ppType) PURE;

  //***********************************************************
  //
  // GetTypeDefinition()
  //
  /// Places the data definition object attached to this interpolator into the
  /// *ppTypeDef argument.  If none exists yet, NULL is placed into the
  /// *ppTypeDef argument.
  ///
  /// The returned data definition object, if it exists, is
  /// AddRef()ed before it is returned.
  ///
  /// Succeeds if all of the following are true:
  /// - the ppTypeDef pointer is valid.
  /// - A valid data definition exists.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppTypeDef is null.
  ///
  /// @param ppTypeDef [out] Returned data definition object
  ///
  STDMETHOD(GetTypeDefinition) (THIS_
    IAAFTypeDef ** ppTypeDef) PURE;


  //***********************************************************
  //
  // SetTypeDefinition()
  //
  /// Sets the data definition of this interpolator to be the given one.
  ///
  /// Succeeds if all of the following are true:
  /// - the pTypeDef pointer is valid.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTypeDef is null.
  ///
  /// @param pTypeDef [in] Data definition object
  ///
  STDMETHOD(SetTypeDefinition) (THIS_
    IAAFTypeDef * pTypeDef) PURE;


  //***********************************************************
  //
  // GetParameter()
  //
  /// Places the parameter object attached to this interpolator into the
  /// *ppParameter argument.  If none exists yet, NULL is placed into the
  /// *ppParameter argument.
  ///
  /// The returned parameter object, if it exists, is
  /// AddRef()ed before it is returned.
  ///
  /// Succeeds if all of the following are true:
  /// - the ppParameter pointer is valid.
  /// - A valid data definition exists.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - ppParameter is null.
  ///
  /// @param ppParameter [out] Returned data definition object
  ///
  STDMETHOD(GetParameter) (THIS_
    IAAFParameter ** ppParameter) PURE;


  //***********************************************************
  //
  // SetParameter()
  //
  /// Sets the parmeter of this interpolator to be the given one.
  ///
  /// Succeeds if all of the following are true:
  /// - the pParameter pointer is valid.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pParameter is null.
  ///
  /// @param pParameter [in] Parameter object
  ///
  STDMETHOD(SetParameter) (THIS_
    IAAFParameter * pParameter) PURE;


  //***********************************************************
  //
  // InterpolateOne()
  //
  /// Interpolates between known control points on the value, and
	/// creates a new AAFControlPoint which which can be imagined to exist
	/// at the given inputValue.
  ///
  /// Succeeds if all of the following are true:
  /// - the pInputValue pointer is valid.
  /// - the ppOutputValue pointer is valid.
  /// - The input value is between 0 (effect start), to 1 (effect end) inclusive.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_INPUT_RANGE
  ///   - The input value must be between 0 and 1, inclusive.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pInputValue or ppOutputValue is null.
  ///
  /// @param pInputValue [in] Input values are from 0 (effect start), to 1 (effect end) inclusive.
  /// @param valueSize [in] Size of preallocated buffer
  /// @param pValue [out, size_is(valueSize),length_is(*bytesRead)] A generated control point
  /// @param bytesRead [out] The number of bytes actually transferred
  ///
  STDMETHOD(InterpolateOne) (THIS_
    aafRational_t *  pInputValue,
    aafUInt32  valueSize,
    aafDataBuffer_t  pValue,
    aafUInt32*  bytesRead) PURE;


  //***********************************************************
  //
  // InterpolateMany()
  //
  /// Interpolates between known control points on the value, and
	/// creates an array of new AAFControlPoint which which can be imagined to exist
	/// at the given inputValue.  While InterpolateOne() generates a single point,
	/// this function starts at a given offset and generates interpolations until either
	/// generateCount is reached, or the next result would have an input value
	/// of greater than one. 
  ///
  /// Succeeds if all of the following are true:
  /// - the pInputValue pointer is valid.
  /// - the pInputStep pointer is valid.
  /// - the ppOutputValue pointer is valid.
  /// - the pResultCount pointer is valid.
  /// - The input value is between 0 (effect start), to 1 (effect end) inclusive.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_INPUT_RANGE
  ///   - The input value must be between 0 and 1, inclusive.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pInputValue or ppOutputValue or pInputStep or pResultCount is null.
  ///
  /// @param pStartInputValue [in] Input values are from 0 (effect start), to 1 (effect end) inclusive.
  /// @param pInputStep [in] Amount to add to *pStartInputValue to get the next point to interpolate
  /// @param generateCount [in] The number of points to interpolate
  /// @param pOutputValue [out] an array of generated control points
  /// @param pResultCount [out] The number of points actually generated
  ///
  STDMETHOD(InterpolateMany) (THIS_
    aafRational_t *  pStartInputValue,
    aafRational_t *  pInputStep,
    aafUInt32  generateCount,
    aafMemPtr_t  pOutputValue,
    aafUInt32 *  pResultCount) PURE;

  END_INTERFACE
};
#endif // __IAAFInterpolator_INTERFACE_DEFINED__



// IAAFMultiEssenceCodec

// ************************
//
// Interface IAAFMultiEssenceCodec
//
// ************************






#ifndef __IAAFMultiEssenceCodec_INTERFACE_DEFINED__
#define __IAAFMultiEssenceCodec_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFMultiEssenceCodec;

#undef  INTERFACE
#define INTERFACE   IAAFMultiEssenceCodec

DECLARE_INTERFACE_(IAAFMultiEssenceCodec, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFMultiEssenceCodec methods *** */




	



  //***********************************************************
  //
  // MultiCreate()
  //
  /// Initalize the stream to be of the correct format with no data and
  /// default metadata, attach the correct type of EssenceDescriptor
  /// to the fileMob, and file in the default metadata on the
  /// descriptor also.  Prepare the stream for writing the first sample
  /// of data.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - fileMob, stream or createParms is null.
  ///
  /// AAFRESULT_STREAM_FULL
  ///   - The essence can not be written because of a fault such as a
  ///     disk full error in the underlying operating system.
  ///
  /// @param fileMob [in] Create the essence attached to this file mob
  /// @param flavour [in, ref] which flavour of the codec to use
  /// @param stream [in] Here is an essence stream with the data
  /// @param compEnable [in] optionally compressing
  /// @param numParms [in] Here are the number of create parameters
  /// @param createParms [in, size_is(numParms)] Here are the create parameters
  ///
  STDMETHOD(MultiCreate) (THIS_
    IAAFSourceMob * fileMob,
    aafUID_constref  flavour,
    IAAFEssenceStream * stream,
    aafCompressEnable_t  compEnable,
    aafUInt32  numParms,
    aafmMultiCreate_t *  createParms) PURE;


  //***********************************************************
  //
  // MultiOpen()
  //
  /// Read the metadata from the essence descriptor and/or the
  /// formatted data, and prepare the stream for reading the first
  /// sample of data.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - fileMob or stream  is null.
  ///
  /// @param fileMob [in] Open the essence attached to this file mob
  /// @param openMode [in] In this mode
  /// @param stream [in] Here is an essence stream with the raw data
  /// @param compEnable [in] optionally decompressing
  ///
  STDMETHOD(MultiOpen) (THIS_
    IAAFSourceMob * fileMob,
    aafMediaOpenMode_t  openMode,
    IAAFEssenceStream * stream,
    aafCompressEnable_t  compEnable) PURE;
	






  //***********************************************************
  //
  // WriteBlocks()
  //
  /// Write blocks from one or more buffers, interleaving if needed.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ZERO_SAMPLESIZE
  ///	 - The sample size of the stream has not been set.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTransferParm or pResultParm is null.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - One of the supplied buffers is not large enough to hold the
  ///     given number of samples.
  ///
  /// AAFRESULT_CODEC_CHANNELS
  ///   - SPecified channel numbers are out of range
  ///
  /// AAFRESULT_XFER_DUPCH
  ///   - The SDK passed in the same channel number on two blocks
  ///
  /// AAFRESULT_MULTI_WRITELEN
  ///   - The length fields of the channels must specify an identical
  ///     length of clock time.
  ///
  /// @param inter [in] Whether the material will be de-interleaved on read
  /// @param xferBlockCount [in] How many aafMultiXfer blocks follow
  /// @param pTransferParm [in, size_is(xferBlockCount)] One or more blocks containing buffer pointer and length
  /// @param pResultParm [out, size_is(xferBlockCount)] One or more blocks containing results
  ///
  STDMETHOD(WriteBlocks) (THIS_
    aafDeinterleave_t  inter,
    aafUInt16  xferBlockCount,
    aafmMultiXfer_t *  pTransferParm,
    aafmMultiResult_t *  pResultParm) PURE;

  //***********************************************************
  //
  // ReadBlocks()
  //
  /// Read blocks into one or more buffers, de-interleaving if needed.
  /// 
  /// This method should return only the following codes.  If more than
  /// one of the listed errors is in effect, it should return the
  /// first one  encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pTransferParm is null.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - The buffer is not large enough to hold the data
  ///
  /// AAFRESULT_CODEC_CHANNELS
  ///   - SPecified channel numbers are out of range
  ///
  /// AAFRESULT_XFER_DUPCH
  ///   - The SDK passed in the same channel number on two blocks
  ///
  /// AAFRESULT_END_OF_DATA
  ///   - Hit either the end-of-file on a raw essence file, or the
  ///     end of the essence property.  The bytesRead fields inside of
  ///     pTransferParm correctly reflect the number of bytes and
  ///     complete samples on each track.
  ///
  /// @param inter [in] Whether the material will be de-interleaved on read
  /// @param xferBlockCount [in] How many aafmMultiXfer blocks follow
  /// @param pTransferParm [out, size_is(xferBlockCount)] One or more blocks containing buffer pointer and length
  /// @param pResultParm [out, size_is(xferBlockCount)] One or more blocks containing results
  ///
  STDMETHOD(ReadBlocks) (THIS_
    aafDeinterleave_t  inter,
    aafUInt16  xferBlockCount,
    aafmMultiXfer_t *  pTransferParm,
    aafmMultiResult_t *  pResultParm) PURE;

  END_INTERFACE
};
#endif // __IAAFMultiEssenceCodec_INTERFACE_DEFINED__



// IAAFPlugin

// ************************
//
// Interface IAAFPlugin
//
// ************************








#ifndef __IAAFPlugin_INTERFACE_DEFINED__
#define __IAAFPlugin_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFPlugin;

#undef  INTERFACE
#define INTERFACE   IAAFPlugin

DECLARE_INTERFACE_(IAAFPlugin, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFPlugin methods *** */


  //***********************************************************
  //
  // CountDefinitions()
  //
  /// This method returns the number of subclasses of AAFDefObject associated with this plugin,
	/// and will be called once at startup by the plugin manager.
	/// A non-zero index is used when a single piece of code implements more than one definition, which
	/// is not to be confused with multiple plugin interfaces existing in a single plugin file.
	/// An example where an index other than one would be useful is an interpolator which implements
	/// multiple types of interpolation (ex. linear, constant, etc...), but has one entry point
	/// for all types.  Codecs will have only one definition per interface, but may have many interfaces
	/// clumped together into a single file.
  ///
  /// If this method fails nothing will be written to *pDefCount.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDefCount arg is NULL.
  ///
  /// @param pDefCount [out] The number of definitions associated with this plugin
  ///
  STDMETHOD(CountDefinitions) (THIS_
    aafUInt32 *  pDefCount) PURE;

  //***********************************************************
  //
  // GetIndexedDefinitionID()
  //
  /// This method returns the unique ID of the AAFDefObject associated with this Plugin. 
 	/// This range of indices will be from 0 to one less than the total number of codecs,
	/// operation groups, classes, types, containers, etc.. implemented by this plugin.
	/// A non-zero index is used when a single piece of code implements more than one definition, which
	/// is not to be confused with multiple plugin interfaces existing in a single plugin file.
	/// An example where an index other than one would be useful is an interpolator which implements
	/// multiple types of interpolation (ex. linear, constant, etc...), but has one entry point
	/// for all types.  Codecs will have only one definition per interface, but may have many interfaces
	/// clumped together into a single file.
 ///
  /// Succeeds if all of the following are true:
  /// - the pPluginID pointer is valid.
  /// - Index is within range.
  /// 
  /// If this method fails nothing will be written to *pPluginID.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pPluginID arg is NULL.
  ///
  /// AAFRESULT_INDEX_RANGE
  ///   - The index value is out of range
  ///
  /// @param index [in] An index from 0 to the number of definitions - 1
  /// @param pPluginID [out] The unique media object id
  ///
  STDMETHOD(GetIndexedDefinitionID) (THIS_
    aafUInt32  index,
    aafUID_t *  pPluginID) PURE;

  //***********************************************************
  //
  // GetPluginDescriptorID()
  //
  /// This method returns the unique ID of the AAFPluginDescriptor associated with this Plugin.
  /// This method is called by the plugin manager to determine if a particular plugin descriptor
  /// (indicating a particular plugin) is already in the current file, so that the full create
  /// function does not need to be called.
  ///
  /// Succeeds if all of the following are true:
  /// - the pPluginID pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pPluginID.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pPluginID arg is NULL.
  ///
  /// @param pPluginID [out] The unique media object id
  ///
  STDMETHOD(GetPluginDescriptorID) (THIS_
    aafUID_t *  pPluginID) PURE;

  //***********************************************************
  //
  // GetIndexedDefinitionObject()
  //
  /// This method manufactures a definition object of the correct
  /// class for this plugin, and fills in the values.  You must call QueryInterface
  /// on the result in order to find the correct interface.  The dictionary supplied
  /// should be for the file where the definition will go, but the definition will not
  /// be added to the file by this function.  The supplied dictionary also needs no
  /// former knowledge of the new definition.  This function will be called by the plugin
  /// manager in order to add the correct definition objects to a file.
	/// A non-zero index is used when a single piece of code implements more than one definition, which
	/// is not to be confused with multiple plugin interfaces existing in a single plugin file.
	/// An example where an index other than one would be useful is an interpolator which implements
	/// multiple types of interpolation (ex. linear, constant, etc...), but has one entry point
	/// for all types.  Codecs will have only one definition per interface, but may have many interfaces
	/// clumped together into a single file.
  ///
  /// Succeeds if all of the following are true:
  /// - the pDefObject pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pDefObject.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDefObject arg is NULL.
  ///
  /// @param index [in] An index from 0 to the number of definitions - 1
  /// @param pDictionary [in] The dictionary to use when creating the definition
  /// @param pDefObject [out] The interface of the pluggable definition
  ///
  STDMETHOD(GetIndexedDefinitionObject) (THIS_
    aafUInt32  index,
    IAAFDictionary * pDictionary,
    IAAFDefObject ** pDefObject) PURE;

  //***********************************************************
  //
  // CreateDescriptor()
  //
  /// This method manufactures a plugin descriptor of the correct
  /// class for this plugin, and fills in the values.  You must call QueryInterface
  /// on the result in order to find the corret interface.  The dictionary supplied
  /// should be for the file where the descriptor will go, but the descriptor will not
  /// be added to the file by this function.  The supplied dictionary also needs no
  /// former knowledge of the new descriptor.  This function will be called by the plugin
  /// manager in order to add the correct plugin descriptors to a file.
  ///
  /// Succeeds if all of the following are true:
  /// - the pPluginDef pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pPluginDef.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pPluginDef arg is NULL.
  ///
  /// @param pDictionary [in] The dictionary to use when creating the descriptor
  /// @param pPluginDef [out] The interface of the pluggable definition
  ///
  STDMETHOD(CreateDescriptor) (THIS_
    IAAFDictionary * pDictionary,
    IAAFPluginDef ** pPluginDef) PURE;


  END_INTERFACE
};
#endif // __IAAFPlugin_INTERFACE_DEFINED__



#endif // __AAFPlugin_h__

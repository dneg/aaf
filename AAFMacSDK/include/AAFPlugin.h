//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
interface IAAFPlugin;
interface IAAFClassExtension;
interface IAAFEssenceStream;
interface IAAFEssenceDataStream;
interface IAAFEssenceCodec;
interface IAAFMultiEssenceCodec;
interface IAAFEssenceContainer;
interface IAAFInterpolator;
#else
typedef interface IAAFPlugin IAAFPlugin;
typedef interface IAAFClassExtension IAAFClassExtension;
typedef interface IAAFEssenceStream IAAFEssenceStream;
typedef interface IAAFEssenceDataStream IAAFEssenceDataStream;
typedef interface IAAFEssenceCodec IAAFEssenceCodec;
typedef interface IAAFMultiEssenceCodec IAAFMultiEssenceCodec;
typedef interface IAAFEssenceContainer IAAFEssenceContainer;
typedef interface IAAFInterpolator IAAFInterpolator;
#endif

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
  // This method returns the number of subclasses of AAFDefObject associated with this plugin,
	// and will be called once at startup by the plugin manager.
	// A non-zero index is used when a single piece of code implements more than one definition, which
	// is not to be confused with multiple plugin interfaces existing in a single plugin file.
	// An example where an index other than one would be useful is an interpolator which implements
	// multiple types of interpolation (ex. linear, constant, etc...), but has one entry point
	// for all types.  Codecs will have only one definition per interface, but may have many interfaces
	// clumped together into a single file.
  //
  // If this method fails nothing will be written to *pDefCount.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDefCount arg is NULL.
  //
  STDMETHOD(CountDefinitions) (THIS_
    // The number of definitions associated with this plugin
    /*[out]*/ aafUInt32 *  pDefCount) PURE;

  //***********************************************************
  //
  // GetIndexedDefinitionID()
  //
  // This method returns the unique ID of the AAFDefObject associated with this Plugin. 
 	// This range of indices will be from 0 to one less than the total number of codecs,
	// operation groups, classes, types, containers, etc.. implemented by this plugin.
	// A non-zero index is used when a single piece of code implements more than one definition, which
	// is not to be confused with multiple plugin interfaces existing in a single plugin file.
	// An example where an index other than one would be useful is an interpolator which implements
	// multiple types of interpolation (ex. linear, constant, etc...), but has one entry point
	// for all types.  Codecs will have only one definition per interface, but may have many interfaces
	// clumped together into a single file.
 //
  // Succeeds if all of the following are true:
  // - the pPluginID pointer is valid.
  // - Index is within range.
  // 
  // If this method fails nothing will be written to *pPluginID.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPluginID arg is NULL.
  //
  // AAFRESULT_INDEX_RANGE
  //   - The index value is out of range
  //
  STDMETHOD(GetIndexedDefinitionID) (THIS_
    // An index from 0 to the number of definitions - 1
    /*[in]*/ aafUInt32  index,

    // The unique media object id
    /*[out]*/ aafUID_t *  pPluginID) PURE;

  //***********************************************************
  //
  // GetPluginDescriptorID()
  //
  // This method returns the unique ID of the AAFPluginDescriptor associated with this Plugin.
  // This method is called by the plugin manager to determine if a particular plugin descriptor
  // (indicating a particular plugin) is already in the current file, so that the full create
  // function does not need to be called.
  //
  // Succeeds if all of the following are true:
  // - the pPluginID pointer is valid.
  // 
  // If this method fails nothing will be written to *pPluginID.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPluginID arg is NULL.
  //
  STDMETHOD(GetPluginDescriptorID) (THIS_
    // The unique media object id
    /*[out]*/ aafUID_t *  pPluginID) PURE;

  //***********************************************************
  //
  // GetIndexedDefinitionObject()
  //
  // This method manufactures a definition object of the correct
  // class for this plugin, and fills in the values.  You must call QueryInterface
  // on the result in order to find the correct interface.  The dictionary supplied
  // should be for the file where the definition will go, but the definition will not
  // be added to the file by this function.  The supplied dictionary also needs no
  // former knowledge of the new definition.  This function will be called by the plugin
  // manager in order to add the correct definition objects to a file.
	// A non-zero index is used when a single piece of code implements more than one definition, which
	// is not to be confused with multiple plugin interfaces existing in a single plugin file.
	// An example where an index other than one would be useful is an interpolator which implements
	// multiple types of interpolation (ex. linear, constant, etc...), but has one entry point
	// for all types.  Codecs will have only one definition per interface, but may have many interfaces
	// clumped together into a single file.
  //
  // Succeeds if all of the following are true:
  // - the pDefObject pointer is valid.
  // 
  // If this method fails nothing will be written to *pDefObject.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDefObject arg is NULL.
  //
  STDMETHOD(GetIndexedDefinitionObject) (THIS_
    // An index from 0 to the number of definitions - 1
    /*[in]*/ aafUInt32  index,

    // The dictionary to use when creating the definition
    /*[in]*/ IAAFDictionary * pDictionary,

    // The interface of the pluggable definition
    /*[out]*/ IAAFDefObject ** pDefObject) PURE;

  //***********************************************************
  //
  // CreateDescriptor()
  //
  // This method manufactures a plugin descriptor of the correct
  // class for this plugin, and fills in the values.  You must call QueryInterface
  // on the result in order to find the corret interface.  The dictionary supplied
  // should be for the file where the descriptor will go, but the descriptor will not
  // be added to the file by this function.  The supplied dictionary also needs no
  // former knowledge of the new descriptor.  This function will be called by the plugin
  // manager in order to add the correct plugin descriptors to a file.
  //
  // Succeeds if all of the following are true:
  // - the pPluginDef pointer is valid.
  // 
  // If this method fails nothing will be written to *pPluginDef.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPluginDef arg is NULL.
  //
  STDMETHOD(CreateDescriptor) (THIS_
    // The dictionary to use when creating the descriptor
    /*[in]*/ IAAFDictionary * pDictionary,

    // The interface of the pluggable definition
    /*[out]*/ IAAFPluginDef ** pPluginDef) PURE;


  END_INTERFACE
};
#endif // __IAAFPlugin_INTERFACE_DEFINED__



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
  // This method should register all type, class and property definitions
  // into the given dictionary necessary to create instances of this 
  // extension object..
  //
  // Succeeds if all of the following are true:
  // - the pDictionary pointer is valid and all 
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDictionary arg is NULL.
  //
  STDMETHOD(RegisterDefinitions) (THIS_
    // The dictionary to use to register definitions
    /*[in]*/ IAAFDictionary * pDictionary) PURE;


  END_INTERFACE
};
#endif // __IAAFClassExtension_INTERFACE_DEFINED__



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
  // Write some number of bytes to the stream exactly and with no formatting or compression.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  (This is the only code indicating success.)
	//
	// AAFRESULT_NULL_PARAM
	//   - pBuffer or pBytesRead is null.
	//
	// AAFRESULT_STREAM_FULL
	//   - The essence can not be written because of a fault such as a disk full error in the
	// underlying operating system.
  //
  STDMETHOD(Write) (THIS_
    // the number of bytes to write
    /*[in]*/ aafUInt32  bytes,

    // the buffer that contains at least bytes
    /*[out, size_is(bytes)]*/ aafDataBuffer_t  buffer,

    // the number of bytes actually written from the buffer
    /*[out,ref]*/ aafUInt32 *  bytesWritten) PURE;


  //***********************************************************
  //
  // Read()
  //
  // Read some number of bytes from the stream exactly and with no formatting or compression.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  (This is the only code indicating success.)
	//
	// AAFRESULT_NULL_PARAM
	//   - pBuffer or pBytesRead is null.
	//
	// AAFRESULT_END_OF_ESSENCE
	//   - Hit either the end-of-file on a raw essence file, or the end of the essence property.
	//	The pBytesRead parameter correctly reflects the number of bytes actually read.
  //
  STDMETHOD(Read) (THIS_
    // to a buffer of this size
    /*[in]*/ aafUInt32  buflen,

    // here is the buffer
    /*[out, size_is(buflen), length_is(*pBytesRead)]*/ aafDataBuffer_t  pBuffer,

    // Return bytes actually read
    /*[out,ref]*/ aafUInt32 *  pBytesRead) PURE;

  //***********************************************************
  //
  // Seek()
  //
  // Seek to the absolute byte offset into the stream.
  //
  STDMETHOD(Seek) (THIS_
    // The absolute byte offset into the stream.
    /*[in]*/ aafPosition_t  byteOffset) PURE;
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect\\\, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  \\\(This is the only code indicating success.\\\)
	//
	// AAFRESULT_STREAM_BOUNDS
	//   - The new position would be outside of the bounds of the stream.)

  //***********************************************************
  //
  // SeekRelative()
  //
  // Seek forward or backward the given byte count.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  (This is the only code indicating success.)
	//
	// AAFRESULT_STREAM_BOUNDS
	//   - The new position would be outside of the bounds of the stream.
  //
  STDMETHOD(SeekRelative) (THIS_
    // The relative byte offset into the stream.
    /*[in]*/ aafInt32  byteOffset) PURE;

  //***********************************************************
  //
  // GetPosition()
  //
  // Returns the position within the stream.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  (This is the only code indicating success.)
	//
	// AAFRESULT_NULL_PARAM
	//   - pPosition is null.
  //
  STDMETHOD(GetPosition) (THIS_
    // The position within the stream.
    /*[out]*/ aafPosition_t *  pPosition) PURE;

  //***********************************************************
  //
  // GetLength()
  //
  // Returns the length of the stream.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  (This is the only code indicating success.)
	//
	// AAFRESULT_NULL_PARAM
	//   - pLength is null.
  //
  STDMETHOD(GetLength) (THIS_
    // The length of the stream.
    /*[out]*/ aafLength_t *  pLength) PURE;



  //***********************************************************
  //
  // FlushCache()
  //
  // Ensure that all bits are written.  The caller of this interface
	// is required to call FlushCache before releasing the stream.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect\, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  \(This is the only code indicating success.\)
	// 
	// AAFRESULT_STREAM_FULL
	//   - The essence can not be written because of a fault such as a disk full error in the
	// underlying operating system.
  STDMETHOD(FlushCache) (THIS) PURE;

  //***********************************************************
  //
  // SetCacheSize()
  //
  // Sets the size of the cache buffer used for further operations.
			// Destroys the current contents of the cache.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  (This is the only code indicating success.) 
  //
  STDMETHOD(SetCacheSize) (THIS_
    // The size of the cache buffer.  Zero is a valid size, and means to turn caching off
    /*[in]*/ aafUInt32  itsSize) PURE;


  END_INTERFACE
};
#endif // __IAAFEssenceStream_INTERFACE_DEFINED__




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
  // Init the stream over a particular EssenceData.
  //
  STDMETHOD(Init) (THIS_
    // The EssenceData to stream over
    /*[in]*/ IUnknown * essenceData) PURE;


  END_INTERFACE
};
#endif // __IAAFEssenceDataStream_INTERFACE_DEFINED__




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
  // Set the essence access associated with the codec.  This routine
  // is called by the SDK when creating or opening an essence stream,
  // in order to give the codec access to the AAFEssenceAccess
  // object, because this is the factory object for creating
  // AAFEssenceFormats.  It would be best to store away this pointer
  // inside of the codec.  In order to avoid circular reference counting
  // problems DO NOT AddRef this interface pointer. Also, DO NOT save
  // any interface derived from the the saved IAAFEssenceAccess 
  // pointer using QueryInterface unless the reference count is restored
  // by a call to ReleaseReference.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceAccess is null.
  //
  STDMETHOD(SetEssenceAccess) (THIS_
    // The associated IAAFEssenceAccess
    /*[in]*/ IAAFEssenceAccess * pEssenceAccess) PURE;



  //***********************************************************
  //
  // CountFlavours()
  //
  // Some codecs have different variants or flavours of the media
  // handled by a single codec.  (For example, the Avid AVR codec
  // handles multiple AVRs.)  This call returns the number of
  // flavours (usually one) supported by this codec.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCount is null.
  //
  STDMETHOD(CountFlavours) (THIS_
    // Number of flavours supported
    /*[out]*/ aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedFlavourID()
  //
  // Some codecs have flavour handled by a single codec.  (For
  // example, the Avid AVR codec handles multiple AVRs.)  The number
  // of flavours is returned by CountFlavours(), and cached by
  // the AAFPluginManager. 
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pVariant is null.
  //
  // AAFRESULT_OUT_OF_RANGE
  //   - index must be >= 0 and < the number of flavours returned
  //     in CountFlavours().
  //
  STDMETHOD(GetIndexedFlavourID) (THIS_
    // Which flavour to get the ID for
    /*[in]*/ aafUInt32  index,

    // The returned flavour ID
    /*[out]*/ aafUID_t *  pVariant) PURE;


  //***********************************************************
  //
  // CountDataDefinitions()
  //
  // Some codecs support formats which can encode multiple types of
  // essence.  For example, interleaved video and audio.  This call
  // returns the number of essence data definitions (usually one)
  // supported by this codec.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCount is null.
  //
  STDMETHOD(CountDataDefinitions) (THIS_
    // Number of essence data definitions supported
    /*[out]*/ aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedDataDefinition()
  //
  // All codecs handle at least one kind of media (picture, sound,
  // control) but some handle more than one.  The kind of media is
  // specified by an AUID representing an AAFDataDef.  The number of
  // data definitions is returned by CountDataDefinitions, and
  // cached by the AAFPluginManager.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppDataDef is null.
  //
  STDMETHOD(GetIndexedDataDefinition) (THIS_
    // Which data definition to get
    /*[in]*/ aafUInt32  index,

    // The returned dataDefinition ID
    /*[out]*/ aafUID_t *  pDataDefID) PURE;


  //***********************************************************
  //
  // GetMaxCodecDisplayNameLength()
  //
  // This method should return the buffer size required to hold the
  // largest display name (including flavour).  The client code does
  // not expect this to be equal to any particular display name.
  //
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize is null.
  //
  STDMETHOD(GetMaxCodecDisplayNameLength) (THIS_
    // length of the buffer required to hold the longest flavour Name
    /*[out]*/ aafUInt32*  pBufSize) PURE;


  //***********************************************************
  //
  // GetCodecDisplayName()
  //
  // Given a flavour ID, return the human readable name.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pName is null.
  //
  STDMETHOD(GetCodecDisplayName) (THIS_
    // which flavour of the codec to use, or kNoCodecFlavour
    /*[in, ref]*/ aafUID_constref  flavour,

    // Human-readable name of the flavour
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pName,

    // length of the buffer to hold flavour Name
    /*[in]*/ aafUInt32  bufSize) PURE;

	



  //***********************************************************
  //
  // CountChannels()
  //
  // Returns the number of channels which this codec can handle of the
  // given essence kind.  If the essence kind is not handled by this
  // codec at all, then return zero channels through pNumChannels,
  // and return AAFRESULT_SUCCESS.
  //
  // The AAFEssenceStream is owned by IAAFEssenceAccess, and
  // therefore should not AddRef() it.  The codec may seek and read
  // the supplied open stream as much as required to determine the
  // number of channels, but should not assume that the Open method
  // on the codec has been called (ie: it must parse any header
  // itself). If the given IAAFEssenceStream pointer is NULL then
  // the codec should look at the information in then source mob and 
  // essence descriptor (in the mob).
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumChannels is null.
  //
  STDMETHOD(CountChannels) (THIS_
    // Get the number of processable channels on this file mob
    /*[in]*/ IAAFSourceMob * fileMob,

    // This is the type of essence to open
    /*[in, ref]*/ aafUID_constref  essenceKind,

    // Here is an essence stream with the data
    /*[in]*/ IAAFEssenceStream * stream,

    // The number of channels present
    /*[out]*/ aafUInt16 *  pNumChannels) PURE;


  //***********************************************************
  //
  // GetSelectInfo()
  //
  // Returns a block of information required to select essence by
  // criteria.  The SDK will use this criteria to find the best
  // essence to fit requirements of size or speed.
  //
  // The AAFEssenceStream is owned by IAAFEssenceAccess, and
  // therefore should not AddRef() it.  The codec may seek and read
  // the supplied open stream as much as required to determine the
  // number of channels, but should not assume that the Open method
  // on the codec has been called (ie: it must parse any header
  // itself). If the given IAAFEssenceStream pointer is NULL then
  // the codec should look at the information in then source mob and 
  // essence descriptor (in the mob).
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pSelectInfo is null.
  //
  STDMETHOD(GetSelectInfo) (THIS_
    // Get the number of processable channels on this file mob
    /*[in]*/ IAAFSourceMob * fileMob,

    // Here is an essence stream with the data
    /*[in]*/ IAAFEssenceStream * stream,

    // Information required to select by media criteria
    /*[out]*/ aafSelectInfo_t *  pSelectInfo) PURE;


  //***********************************************************
  //
  // ValidateEssence()
  //
  // This method validates the metadata associated with the essence
  // data.  It may compare fields within the data, and also compare
  // essence data against values in the IAAFFileDescriptor.
  // 
  // The fields bufSize, pErrorText, and pBytes read are used to
  // return a textual description of any problems encountered.  If no
  // problems were encountered, pBytesRead should be zero.
  // 
  // This method is used during client and SDK validation, and is not
  // intended for general use.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pErrorText is null.
  //
  STDMETHOD(ValidateEssence) (THIS_
    // Run a check on this file mob
    /*[in]*/ IAAFSourceMob * fileMob,

    // and this essence data stream
    /*[in]*/ IAAFEssenceStream * stream,

    // This is the verbosity level of the output
    /*[in]*/ aafCheckVerbose_t  verbose,

    // This determines whether the output contains warnings
    /*[in]*/ aafCheckWarnings_t  outputWarnings,

    // length of the buffer to hold the error result
    /*[in]*/ aafUInt32  bufSize,

    // Human-readable text describing problems (or lack therof) with
    // the essence
    /*[out, size_is(bufSize), length_is(*pBytesRead)]*/ aafCharacter *  pErrorText,

    // The number of result bytes returned
    /*[out]*/ aafUInt32*  pBytesRead) PURE;


  //***********************************************************
  //
  // Create()
  //
  // Initalize the stream to be of the correct format with no data and
  // default metadata, attach the correct type of EssenceDescriptor
  // to the fileMob, and file in the default metadata on the
  // descriptor also.  Prepare the stream for writing the first sample
  // of data.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceAccess is null.
  //
  // AAFRESULT_STREAM_FULL
  //   - The essence can not be written because of a fault such as a
  //     disk full error in the underlying operating system.
  //
  STDMETHOD(Create) (THIS_
    // Create the essence attached to this file mob
    /*[in]*/ IAAFSourceMob * fileMob,

    // which flavour of the codec to use
    /*[in, ref]*/ aafUID_constref  flavour,

    // This is the type of essence to create
    /*[in, ref]*/ aafUID_constref  essenceKind,

    // The sample rate for this essence
    /*[in, ref]*/ aafRational_constref  sampleRate,

    // Here is an essence stream with the data
    /*[in]*/ IAAFEssenceStream * stream,

    // optionally compressing
    /*[in]*/ aafCompressEnable_t  compEnable) PURE;


  //***********************************************************
  //
  // Open()
  //
  // Read the metadata from the essence descriptor and/or the
  // formatted data, and prepare the stream for reading the first
  // sample of data.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceAccess is null.
  //
  // AAFRESULT_ESSENCE_NOT_FOUND
  //   - The essence could not be located inside of the current file,
  //     or by following any of the locators.
  //
  STDMETHOD(Open) (THIS_
    // Open the essence attached to this file mob
    /*[in]*/ IAAFSourceMob * fileMob,

    // In this mode
    /*[in]*/ aafMediaOpenMode_t  openMode,

    // Here is an essence stream with the raw data
    /*[in]*/ IAAFEssenceStream * stream,

    // optionally decompressing
    /*[in]*/ aafCompressEnable_t  compEnable) PURE;
	




  //***********************************************************
  //
  // CountSamples()
  //
  // Return the number of samples present on a given essenceKind.  The
  // assumption is made that all slots of the same essence kind have
  // the same number of samples.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pNumSamples is null.
  //
  STDMETHOD(CountSamples) (THIS_
    // Tell how many samples of this type are on the mob
    /*[in, ref]*/ aafUID_constref  essenceKind,

    // The number of samples present of the given essence kind
    /*[out]*/ aafLength_t *  pNumSamples) PURE;


  //***********************************************************
  //
  // WriteSamples()
  //
  // Writes data the given essence stream.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ZERO_SAMPLESIZE
  //	 - The sample size of the stream has not been set.
  //
  // AAFRESULT_NULL_PARAM
  //   - buffer or pResultParm is null.
  //
  // AAFRESULT_SMALLBUF
  //   - One of the supplied buffers is not large enough to hold the
  //     given number of samples.
  //
  // AAFRESULT_CODEC_CHANNELS
  //   - SPecified channel numbers are out of range
  //
  // AAFRESULT_CONTAINERWRITE
  //   - The essence can not be written because of a fault such as a
  //     disk full error in the underlying operating system.
  //
  STDMETHOD(WriteSamples) (THIS_
    // write this many samples
    /*[in]*/ aafUInt32  nSamples,

    // from a buffer of this size
    /*[in]*/ aafUInt32  buflen,

    // from a buffer
    /*[in,size_is(buflen)]*/ aafDataBuffer_t  buffer,

    // number of samples actually written
    /*[out, ref]*/ aafUInt32 *  samplesWritten,

    // number of bytes actually written
    /*[out, ref]*/ aafUInt32 *  bytesWritten) PURE;


  //***********************************************************
  //
  // ReadSamples()
  //
  // Read a given number of samples from an opened essence stream.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one  encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTransferParm is null.
  //
  // AAFRESULT_SMALLBUF
  //   - The buffer is not large enough to hold the data
  //
  // AAFRESULT_CODEC_CHANNELS
  //   - SPecified channel numbers are out of range
  //
  // AAFRESULT_END_OF_DATA
  //   - Hit either the end-of-file on a raw essence file, or the
  //     end of the essence property.  The bytesRead fields inside of
  //     pTransferParm correctly reflect the number of bytes and
  //     complete samples on each track.
  //
  STDMETHOD(ReadSamples) (THIS_
    // Read this many samples
    /*[in]*/ aafUInt32  nSamples,

    // into a buffer of this size
    /*[in]*/ aafUInt32  buflen,

    // The transfer buffer
    /*[out, size_is(buflen),length_is(*bytesRead)]*/ aafDataBuffer_t  buffer,

    // The number of samples actually read
    /*[out, ref]*/ aafUInt32 *  samplesRead,

    // The number of bytes actually read
    /*[out, ref]*/ aafUInt32 *  bytesRead) PURE;


  //***********************************************************
  //
  // Seek()
  //
  // Seek to a particular sample frame on the media.  The sample frame
  // is one frame for picture, and one sample times the number of
  // interleaved channels for audio or other interleaved formats.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  //
  // AAFRESULT_BADSAMPLEOFFSET
  //   - The supplied sample offset is out of range
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.) 
  //
  STDMETHOD(Seek) (THIS_
    // The sample frame to seek to.
    /*[in]*/ aafPosition_t  sampleFrame) PURE;


  //***********************************************************
  //
  // CompleteWrite()
  //
  // Close the essence stream, ready to open another or Finish.  In
  // the case  where the client has created essence in an external AAF
  // file, two essence descriptors will need to be updated.  After
  // updating the essence descriptor for the file which you are
  // writing (like updating the number of samples), find the
  // essence descriptor associated with pFileMob, and make the same
  // changes to that essence desriptor.
  //
  // NOTE that pFileMob may be NULL in all cases except writing
  // essence to an external AAF file.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDescriptor is null.
  //
  // AAFRESULT_STREAM_FULL
  //   - The essence can not be written because of a fault such as a
  //     disk full error in the underlying operating system.
  //
  STDMETHOD(CompleteWrite) (THIS_
    // Optional second essence descriptor to also update
    /*[in]*/ IAAFSourceMob * pFileMob) PURE;



	

  //***********************************************************
  //
  // CreateDescriptorFromStream()
  //
  // Given some raw essence (like a WAVE file), create an
  // AAFEssenceDescriptor to match, with all fields filled in. 
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pStream or pSourceMob is null.
  //
  STDMETHOD(CreateDescriptorFromStream) (THIS_
    // A raw file stream
    /*[in]*/ IAAFEssenceStream * pStream,

    // Put the finished descriptor on this source mob
    /*[in]*/ IAAFSourceMob * pSourceMob) PURE;


  //***********************************************************
  //
  // GetCurrentEssenceStream()
  //
  // Return the current essence stream.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppStream is null.
  //
  STDMETHOD(GetCurrentEssenceStream) (THIS_
    // The current essence stream
    /*[out]*/ IAAFEssenceStream ** ppStream) PURE;

	
  //***********************************************************
  //
  // PutEssenceFormat()
  //
  // Set the format of the one or more parameters.  The client should
  // have called GetDefaultEssenceFormat() in order to find out what
  // format codes are supported, but you may still get format codes
  // out of range.
  //
  // The expected bevhavior of this routine is to pre-scan the list
  // for valid format codes and lengths before setting any data, and
  // returning AAFRESULT_INVALID_OP_CODEC.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ILLEGAL_FILEFMT
  //   - One of the format codes is not supported by this codec.  Call
  //   GetDefaultEssenceFormat() in order to find the list of
  //   supported format codes.  No data will have been transferred.
  //
  // AAFRESULT_INVALID_PARM_SIZE
  //   - The data supplied with the parameter is not of the correct
  //     size.  No data will have been transferred.
  //
  // AAFRESULT_NULL_PARAM
  //   - pFormat is null.
  //
  STDMETHOD(PutEssenceFormat) (THIS_
    // An essence format object
    /*[in]*/ IAAFEssenceFormat * pFormat) PURE;


  //***********************************************************
  //
  // GetEssenceFormat()
  //
  // Get the format of the one or more parameters by scanning the list
  // of format codes supplied, and filling in the data values.  The
  // client should have called GetDefaultEssenceFormat() in order to
  // find out what format codes are supported, but you may still get
  // format codes out of range.
  //
  // The expected bevhavior of this routine is to pre-scan the list
  // for valid format codes before setting any data, and returning
  // AAFRESULT_INVALID_OP_CODEC.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ILLEGAL_FILEFMT
  //   - One of the format codes is not supported by this codec.  Call
  //     GetDefaultEssenceFormat() in order to find the list of
  //     supported format codes.  No data will have been transferred.
  //
  // AAFRESULT_INVALID_PARM_SIZE
  //   - The data supplied with the parameter is not of the correct
  //     size.  No data will have been transferred.
  //
  // AAFRESULT_NULL_PARAM
  //   - pFormatTemplate or ppNewFormat is null.
  //
  STDMETHOD(GetEssenceFormat) (THIS_
    // An essence format object to use as a template of which fields to read
    /*[in]*/ IAAFEssenceFormat * pFormatTemplate,

    // A new essence format object with the data filled in
    /*[out]*/ IAAFEssenceFormat ** ppNewFormat) PURE;

		
  //***********************************************************
  //
  // GetDefaultEssenceFormat()
  //
  // Returns an essence format object specifying which contains the
  // complete list of essence formats supported by this codec, along
  // with the default values.  Previous systems required you to
  // "know" what parameters a given codec could support.  With this
  // function, a client application can check what the default
  // parameter would be, and change only those which are required.
  // It differs from GetEssenceFormat() in that it
  // GetEssenceFormat() returns the current client-set settings.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppNewFormat is null.
  //
  STDMETHOD(GetDefaultEssenceFormat) (THIS_
    // An essence format with all valid parameter names and values
	// filled in
    /*[out]*/ IAAFEssenceFormat ** ppNewFormat) PURE;


  //***********************************************************
  //
  // GetEssenceDescriptorID()
  //
  // Returns the stored object AUID of the essence descriptor class
  // associated with this codec.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDescriptorID is null.
  //
  STDMETHOD(GetEssenceDescriptorID) (THIS_
    // The stored object AUID of the essence descriptor class
    /*[out]*/ aafUID_t *  pDescriptorID) PURE;


  //***********************************************************
  //
  // GetEssenceDataID()
  //
  // Returns the stored object AUID of the essence data class
  // associated with this codec. 
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceDataID is null.
  //
  STDMETHOD(GetEssenceDataID) (THIS_
    // The stored object AUID of the essence data class
    /*[out]*/ aafUID_t *  pEssenceDataID) PURE;


  //***********************************************************
  //
  // GetIndexedSampleSize()
  //
  // Returns the size of the sample at the given offset, and of the
  // given data definition.  For formats with fixed sample sizes
  // (without a sample offset table), this method should return the
  // standard sample size.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLength is null.
  //
  STDMETHOD(GetIndexedSampleSize) (THIS_
    // The ID of the data definition describing the essence
    /*[in, ref]*/ aafUID_constref  essenceDefID,

    // The zero-based offset to get the size of
    /*[in]*/ aafPosition_t  sampleOffset,

    // The returned length of the given sample
    /*[out]*/ aafLength_t *  pLength) PURE;


  //***********************************************************
  //
  // GetLargestSampleSize()
  //
  // Returns the size of the largest sample, and of the given data
  // definition.  For formats with fixed sample sizes (without a
  // sample offset table), this method should return the standard
  // sample size.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLength is null.
  //
  STDMETHOD(GetLargestSampleSize) (THIS_
    // The ID of the data definition of the essence
    /*[in, ref]*/ aafUID_constref  essenceDefID,

    // The returned length of the largest sample
    /*[out]*/ aafLength_t *  pLength) PURE;

  END_INTERFACE
};
#endif // __IAAFEssenceCodec_INTERFACE_DEFINED__



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
  // Initalize the stream to be of the correct format with no data and
  // default metadata, attach the correct type of EssenceDescriptor
  // to the fileMob, and file in the default metadata on the
  // descriptor also.  Prepare the stream for writing the first sample
  // of data.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - fileMob, stream or createParms is null.
  //
  // AAFRESULT_STREAM_FULL
  //   - The essence can not be written because of a fault such as a
  //     disk full error in the underlying operating system.
  //
  STDMETHOD(MultiCreate) (THIS_
    // Create the essence attached to this file mob
    /*[in]*/ IAAFSourceMob * fileMob,

    // which flavour of the codec to use
    /*[in, ref]*/ aafUID_constref  flavour,

    // Here is an essence stream with the data
    /*[in]*/ IAAFEssenceStream * stream,

    // optionally compressing
    /*[in]*/ aafCompressEnable_t  compEnable,

    // Here are the number of create parameters
    /*[in]*/ aafUInt32  numParms,

    // Here are the create parameters
    /*[in, size_is(numParms)]*/ aafmMultiCreate_t *  createParms) PURE;


  //***********************************************************
  //
  // MultiOpen()
  //
  // Read the metadata from the essence descriptor and/or the
  // formatted data, and prepare the stream for reading the first
  // sample of data.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - fileMob or stream  is null.
  //
  STDMETHOD(MultiOpen) (THIS_
    // Open the essence attached to this file mob
    /*[in]*/ IAAFSourceMob * fileMob,

    // In this mode
    /*[in]*/ aafMediaOpenMode_t  openMode,

    // Here is an essence stream with the raw data
    /*[in]*/ IAAFEssenceStream * stream,

    // optionally decompressing
    /*[in]*/ aafCompressEnable_t  compEnable) PURE;
	






  //***********************************************************
  //
  // WriteBlocks()
  //
  // Write blocks from one or more buffers, interleaving if needed.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ZERO_SAMPLESIZE
  //	 - The sample size of the stream has not been set.
  //
  // AAFRESULT_NULL_PARAM
  //   - pTransferParm or pResultParm is null.
  //
  // AAFRESULT_SMALLBUF
  //   - One of the supplied buffers is not large enough to hold the
  //     given number of samples.
  //
  // AAFRESULT_CODEC_CHANNELS
  //   - SPecified channel numbers are out of range
  //
  // AAFRESULT_XFER_DUPCH
  //   - The SDK passed in the same channel number on two blocks
  //
  // AAFRESULT_MULTI_WRITELEN
  //   - The length fields of the channels must specify an identical
  //     length of clock time.
  //
  STDMETHOD(WriteBlocks) (THIS_
    // Whether the material will be de-interleaved on read
    /*[in]*/ aafDeinterleave_t  inter,

    // How many aafMultiXfer blocks follow
    /*[in]*/ aafUInt16  xferBlockCount,

    // One or more blocks containing buffer pointer and length
    /*[in, size_is(xferBlockCount)]*/ aafmMultiXfer_t *  pTransferParm,

    // One or more blocks containing results
    /*[out, size_is(xferBlockCount)]*/ aafmMultiResult_t *  pResultParm) PURE;

  //***********************************************************
  //
  // ReadBlocks()
  //
  // Read blocks into one or more buffers, de-interleaving if needed.
  // 
  // This method should return only the following codes.  If more than
  // one of the listed errors is in effect, it should return the
  // first one  encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTransferParm is null.
  //
  // AAFRESULT_SMALLBUF
  //   - The buffer is not large enough to hold the data
  //
  // AAFRESULT_CODEC_CHANNELS
  //   - SPecified channel numbers are out of range
  //
  // AAFRESULT_XFER_DUPCH
  //   - The SDK passed in the same channel number on two blocks
  //
  // AAFRESULT_END_OF_DATA
  //   - Hit either the end-of-file on a raw essence file, or the
  //     end of the essence property.  The bytesRead fields inside of
  //     pTransferParm correctly reflect the number of bytes and
  //     complete samples on each track.
  //
  STDMETHOD(ReadBlocks) (THIS_
    // Whether the material will be de-interleaved on read
    /*[in]*/ aafDeinterleave_t  inter,

    // How many aafmMultiXfer blocks follow
    /*[in]*/ aafUInt16  xferBlockCount,

    // One or more blocks containing buffer pointer and length
    /*[out, size_is(xferBlockCount)]*/ aafmMultiXfer_t *  pTransferParm,

    // One or more blocks containing results
    /*[out, size_is(xferBlockCount)]*/ aafmMultiResult_t *  pResultParm) PURE;

  END_INTERFACE
};
#endif // __IAAFMultiEssenceCodec_INTERFACE_DEFINED__



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
  // Attempt to create an essence stream in the container 
  // for reading and writing.
  // Returns one of the following:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - both pName and pMobID cannot be NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - pName or pMobID (or both) are invalid
  //
  // AAFRESULT_FILE_EXISTS
  //   - the given path and or mobID  already points to an existing stream 
  //     within the container.
  //
  STDMETHOD(CreateEssenceStream) (THIS_
    // String key to identify the new essence within container
    /*[in, string]*/ aafCharacter_constptr  pName,

    // Optional mobID identifying the essence
    /*[in]*/ aafMobID_constptr  pMobID,

    // address of local variable to contain the essence stream pointer.
    /*[out, retval]*/ IAAFEssenceStream ** ppEssenceStream) PURE;


  //***********************************************************
  //
  // CreateEssenceStreamWriteOnly()
  //
  // Attempt to create an essence stream in the container 
  // for writing.
  // Returns one of the following:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - both pName and pMobID cannot be NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - pName or pMobID (or both) are invalid
  //
  // AAFRESULT_FILE_EXISTS
  //   - the given path and or mobID  already points to an existing stream 
  //     within the container.
  //
  STDMETHOD(CreateEssenceStreamWriteOnly) (THIS_
    // String key to identify the new essence within container
    /*[in, string]*/ aafCharacter_constptr  pPath,

    // Optional mobID identifying the essence
    /*[in]*/ aafMobID_constptr  pMobID,

    // address of local variable to contain the essence stream pointer.
    /*[out, retval]*/ IAAFEssenceStream ** ppEssenceStream) PURE;


  //***********************************************************
  //
  // OpenEssenceStreamReadOnly()
  //
  // Attempt to open an essence stream for reading.
  // Returns one of the following:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - both pName and pMobID cannot be NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - pName or pMobID (or both) are invalid
  //
  // AAFRESULT_MEDIA_NOT_FOUND
  //   - the given path does not point to an existing stream within 
  //     the container.
  //
  // AAFRESULT_NOT_READABLE
  //   - stream is write-only, cannot be opened for reading.
  //
  STDMETHOD(OpenEssenceStreamReadOnly) (THIS_
    // String key to identify the existing essence within container
    /*[in, string]*/ aafCharacter_constptr  pFilePath,

    // Optional mobID identifying the essence
    /*[in]*/ aafMobID_constptr  pMobID,

    // address of local variable to contain the essence stream pointer.
    /*[out, retval]*/ IAAFEssenceStream ** ppEssenceStream) PURE;


  //***********************************************************
  //
  // OpenEssenceStreamAppend()
  //
  // Attempt to open an essence stream for appending.
  // Returns one of the following:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - both pName and pMobID cannot be NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - pName or pMobID (or both) are invalid
  //
  // AAFRESULT_MEDIA_NOT_FOUND
  //   - the given path does not point to an existing stream within 
  //     the container.
  //
  // AAFRESULT_NOT_WRITEABLE
  //   - stream is read-only, cannot be opened for writing.
  //
  STDMETHOD(OpenEssenceStreamAppend) (THIS_
    // String key to identify the existing essence within container
    /*[in, string]*/ aafCharacter_constptr  pFilePath,

    // Optional mobID identifying the essence
    /*[in]*/ aafMobID_constptr  pMobID,

    // address of local variable to contain the essence stream pointer.
    /*[out, retval]*/ IAAFEssenceStream ** ppEssenceStream) PURE;

  END_INTERFACE
};
#endif // __IAAFEssenceContainer_INTERFACE_DEFINED__



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
  // Each interpolator plugin will support a given number of interpolator
	// types for a given number of data types.  This call returns the number of
	// data types supported, so that the SDK can call GetIndexedSupportedType()
	// repeatedly in order to find out what types this interpolator suppports.
	//
	// An interpolator is expected to support all data types for each interpolation
	// method which the interpolator supports.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  (This is the only code indicating success.)
	//
	// AAFRESULT_NULL_PARAM
	//   - pCount is null.
  //
  STDMETHOD(GetNumTypesSupported) (THIS_
    // Number of data types supported
    /*[out]*/ aafUInt32*  pCount) PURE;


  //***********************************************************
  //
  // GetIndexedSupportedType()
  //
  // Each interpolator plugin will support a given number of interpolator
	// types for a given number of data types.  This call returns one of the data
	// types supported using an index.  The index value should be between 0 and
	// one less than the value returned by GetNumTypesSupported().
	// The number of flavours is returned by GetNumTypesSupported(), and cached
	// by the AAFPluginManager.
	//
	// An interpolator is expected to support all data types for each interpolation
	// method which the interpolator supports.
	// 
	// This method should return only the following codes.  If more than one of
	// the listed errors is in effect, it should return the first one
	// encountered in the order given below:
	// 
	// AAFRESULT_SUCCESS
	//   - succeeded.  (This is the only code indicating success.)
	//
	// AAFRESULT_NULL_PARAM
	//   - pVariant is null.
	//
	// AAFRESULT_OUT_OF_RANGE
	//   - index must be >= 0 and < the number of flavours returned in GetNumTypesSupported(). 
  //
  STDMETHOD(GetIndexedSupportedType) (THIS_
    // Which flavour to get the ID for
    /*[in]*/ aafUInt32  index,

    // The returned type definition
    /*[out]*/ IAAFTypeDef ** ppType) PURE;

  //***********************************************************
  //
  // GetTypeDefinition()
  //
  // Places the data definition object attached to this interpolator into the
  // *ppTypeDef argument.  If none exists yet, NULL is placed into the
  // *ppTypeDef argument.
  //
  // The returned data definition object, if it exists, is
  // AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppTypeDef pointer is valid.
  // - A valid data definition exists.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppTypeDef is null.
  //
  STDMETHOD(GetTypeDefinition) (THIS_
    // Returned data definition object
    /*[out]*/ IAAFTypeDef ** ppTypeDef) PURE;


  //***********************************************************
  //
  // SetTypeDefinition()
  //
  // Sets the data definition of this interpolator to be the given one.
  //
  // Succeeds if all of the following are true:
  // - the pTypeDef pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTypeDef is null.
  //
  STDMETHOD(SetTypeDefinition) (THIS_
    // Data definition object
    /*[in]*/ IAAFTypeDef * pTypeDef) PURE;


  //***********************************************************
  //
  // GetParameter()
  //
  // Places the parameter object attached to this interpolator into the
  // *ppParameter argument.  If none exists yet, NULL is placed into the
  // *ppParameter argument.
  //
  // The returned parameter object, if it exists, is
  // AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppParameter pointer is valid.
  // - A valid data definition exists.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppParameter is null.
  //
  STDMETHOD(GetParameter) (THIS_
    // Returned data definition object
    /*[out]*/ IAAFParameter ** ppParameter) PURE;


  //***********************************************************
  //
  // SetParameter()
  //
  // Sets the parmeter of this interpolator to be the given one.
  //
  // Succeeds if all of the following are true:
  // - the pParameter pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pParameter is null.
  //
  STDMETHOD(SetParameter) (THIS_
    // Parameter object
    /*[in]*/ IAAFParameter * pParameter) PURE;


  //***********************************************************
  //
  // InterpolateOne()
  //
  // Interpolates between known control points on the value, and
	// creates a new AAFControlPoint which which can be imagined to exist
	// at the given inputValue.
  //
  // Succeeds if all of the following are true:
  // - the pInputValue pointer is valid.
  // - the ppOutputValue pointer is valid.
  // - The input value is between 0 (effect start), to 1 (effect end) inclusive.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_INPUT_RANGE
  //   - The input value must be between 0 and 1, inclusive.
  //
  // AAFRESULT_NULL_PARAM
  //   - pInputValue or ppOutputValue is null.
  //
  STDMETHOD(InterpolateOne) (THIS_
    // Input values are from 0 (effect start), to 1 (effect end) inclusive.
    /*[in]*/ aafRational_t *  pInputValue,

    // Size of preallocated buffer
    /*[in]*/ aafUInt32  valueSize,

    // A generated control point
    /*[out, size_is(valueSize),length_is(*bytesRead)]*/ aafDataBuffer_t  pValue,

    // The number of bytes actually transferred
    /*[out]*/ aafUInt32*  bytesRead) PURE;


  //***********************************************************
  //
  // InterpolateMany()
  //
  // Interpolates between known control points on the value, and
	// creates an array of new AAFControlPoint which which can be imagined to exist
	// at the given inputValue.  While InterpolateOne() generates a single point,
	// this function starts at a given offset and generates interpolations until either
	// generateCount is reached, or the next result would have an input value
	// of greater than one. 
  //
  // Succeeds if all of the following are true:
  // - the pInputValue pointer is valid.
  // - the pInputStep pointer is valid.
  // - the ppOutputValue pointer is valid.
  // - the pResultCount pointer is valid.
  // - The input value is between 0 (effect start), to 1 (effect end) inclusive.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_INPUT_RANGE
  //   - The input value must be between 0 and 1, inclusive.
  //
  // AAFRESULT_NULL_PARAM
  //   - pInputValue or ppOutputValue or pInputStep or pResultCount is null.
  //
  STDMETHOD(InterpolateMany) (THIS_
    // Input values are from 0 (effect start), to 1 (effect end) inclusive.
    /*[in]*/ aafRational_t *  pStartInputValue,

    // Amount to add to *pStartInputValue to get the next point to interpolate
    /*[in]*/ aafRational_t *  pInputStep,

    // The number of points to interpolate
    /*[in]*/ aafUInt32  generateCount,

    // an array of generated control points
    /*[out]*/ aafMemPtr_t  pOutputValue,

    // The number of points actually generated
    /*[out]*/ aafUInt32 *  pResultCount) PURE;

  END_INTERFACE
};
#endif // __IAAFInterpolator_INTERFACE_DEFINED__



#endif // __AAFPlugin_h__

//@doc
//@class    AAFEssenceAccess | Implementation class for AAFEssenceAccess
#ifndef __CAAFEssenceAccess_h__
#define __CAAFEssenceAccess_h__

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



#ifndef __AAF_h__
#include "AAF.h"
#endif











#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFEssenceAccess
  : public IAAFEssenceAccess,
    public IAAFEssenceMultiAccess,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFEssenceAccess (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFEssenceAccess ();

public:



  //***********************************************************
  //
  // SetEssenceCodecFlavour()
  //
  // Sets which flavour of the codec ID is to be used.
  //
  // An example of a flavour would be a number of standard starting
  // JPEG tables.
  //
  STDMETHOD (SetEssenceCodecFlavour) (
    // The particular flavour 
    /*[in, ref]*/ aafUID_constref  flavour);


  //***********************************************************
  //
  // WriteSamples()
  //
  // Writes data to the given essence stream.
  //
  // A single video frame is ONE sample.
  //
  // Buflen must be large enough to hold the total sample size.
  //
  // Possible Errors:
  //   AAFRESULT_SINGLE_CHANNEL_OP -- Tried to write to an interleaved
  //                                  stream.
  //   AAFRESULT_BADDATAADDRESS    -- The buffer must not be a NULL
  //                                  pointer.
  //
  STDMETHOD (WriteSamples) (
    // write this many samples
    /*[in]*/ aafUInt32  nSamples,

    // from a buffer of this size
    /*[in]*/ aafUInt32  buflen,

    // from a buffer
    /*[in,size_is(buflen)]*/ aafDataBuffer_t  buffer,

    // The number of samples actually written
    /*[out, ref]*/ aafUInt32 *  samplesWritten,

    // The number of bytes actually written
    /*[out, ref]*/ aafUInt32 *  bytesWritten);


  //***********************************************************
  //
  // GetLargestSampleSize()
  //
  // Returns the size in bytes of the largest sample for a given
  // essence type.
  //
  // For uncompressed data, or the output of the software codec, the
  // sample size will propably be a constant.
  //
  // The essence type parameter exists to support codecs with multiple
  // interleaved essence types.
  //
  STDMETHOD (GetLargestSampleSize) (
    // for this essence type
    /*[in]*/ IAAFDataDef * pDataDef,

    // the largest sample size
    /*[out]*/ aafLength_t*  maxSize);


  //***********************************************************
  //
  // GetIndexedSampleSize()
  //
  // Returns the size in bytes of the given sample for a given essence
  // type.
  //
  // For uncompressed data, or the output of the software codec, the
  // sample size will propably be a constant.
  //
  // The essence type parameter exists to support codecs with multiple
  // interleaved essence types.
  // 
  // Possible Errors:
  //   AAFRESULT_NULL_PARAM      -- A return parameter was NULL.
  //   AAFRESULT_BADSAMPLEOFFSET -- The supplied sample offset is out
  //                                of range.
  //
  STDMETHOD (GetIndexedSampleSize) (
    // and this essence type
    /*[in]*/ IAAFDataDef * pDataDef,

    // for this [0-based] sample frame number
    /*[in]*/ aafPosition_t  frameNum,

    // How big is the sample frame?
    /*[out]*/ aafLength_t*  frameSize);


  //***********************************************************
  //
  // SetTransformParameters()
  //
  // Sets a series of format objects which will be used to describe
  // the in-memory format.
  //
  // This is the format expected on writes and produced on reads.
  //
  // On writes, the data will be written in this format, except
  // where a software codec may be used.  On reads, the data will be
  // translated to this format.
  //
  // The order of the parameters does matter, as transformations will
  // be applied in that order to get from the on-disk format to the
  // in-memory format.
  //
  STDMETHOD (SetTransformParameters) (
    // A set of transforms to apply to the essence if required 
    /*[in]*/ IAAFEssenceFormat * op);
	 

  //***********************************************************
  //
  // CountSamples()
  //
  // Returns the number of samples of the given essence type on the
  // given essence stream.
  // 
  // A video sample is one frame.
  //
  STDMETHOD (CountSamples) (
    // For this essence dataDef
    /*[in]*/ IAAFDataDef * pDataDef,

    // find out how many samples are present
    /*[out]*/ aafLength_t *  result);


  //***********************************************************
  //
  // ReadSamples()
  //
  // Read a given number of samples from an opened essence stream.
  //
  // This call will only return a single channel of essence from an
  // interleaved stream.
  //
  // A video sample is a frame.
  //
  // Buflen is in bytes, and should be large enough to hold the
  // samples after the requested transforms have been applied.
  //
  // Possible Errors:
  //   AAFRESULT_END_OF_DATA -- Hit the end of the essence (like
  //                            EOF) while reading.
  //   AAFRESULT_SMALLBUF    -- Buffer too small for samples.
  //
  STDMETHOD (ReadSamples) (
    // Read this many samples
    /*[in]*/ aafUInt32  nSamples,

    // into a buffer of this size
    /*[in]*/ aafUInt32  buflen,

    // The transfer buffer
    /*[out, size_is(buflen),length_is(*bytesRead)]*/ aafDataBuffer_t  buffer,

    // The number of samples actually read
    /*[out, ref]*/ aafUInt32 *  samplesRead,

    // The number of bytes actually read
    /*[out, ref]*/ aafUInt32 *  bytesRead);


  //***********************************************************
  //
  // Seek()
  //
  // The seek function for essence.
  //
  // Useful only on reading, you can't seek aound while writing
  // essence.
  //
  // An audio frame is one sample across all channels.
  //
  // Possible Errors:
  //   AAFRESULT_BADSAMPLEOFFSET -- Hit the end of the essence
  //                                (like EOF) while reading.
  //
  STDMETHOD (Seek) (
    // A 0-based offset in units of the sample rate to seek to. 
    /*[in]*/ aafPosition_t  sampleFrameNum);


  //***********************************************************
  //
  // GetFileFormat()
  //
  // Given an AAFEssenceFormat, read the essence parameters inside
  // and set the values from the file format.
  //
  STDMETHOD (GetFileFormat) (
    // An AAFEssenceFormat with parameter codes set but no values
    /*[in]*/ IAAFEssenceFormat * opsTemplate,

    // Another AAFEssenceFormat with values set
    /*[out]*/ IAAFEssenceFormat ** opsResult);


  //***********************************************************
  //
  // GetFileFormatParameterList()
  //
  // Returns an AAFEssenceFormat containing a list of all parameters
  // supported by the current codec.
  //
  STDMETHOD (GetFileFormatParameterList) (
    // An ampty AAFEssenceFormat object 
    /*[out]*/ IAAFEssenceFormat ** ops);


  //***********************************************************
  //
  // GetEmptyFileFormat()
  //
  // Returns an empty AAFEssenceFormat object.  This is the factory
  // method for AAFEssenceFormat.
  //
  STDMETHOD (GetEmptyFileFormat) (
    // An ampty AAFEssenceFormat object 
    /*[out]*/ IAAFEssenceFormat ** ops);


  //***********************************************************
  //
  // PutFileFormat()
  //
  // Given an AAFEssenceFormat, read the essence parameters inside
  // and change the file format.
  //
  STDMETHOD (PutFileFormat) (
    // An AAFEssenceFormat with one or more parameter/value pairs 
    /*[in]*/ IAAFEssenceFormat * ops);


  //***********************************************************
  //
  // GetCodecName()
  //
  // Returns the full name of the given codec expanded for human
  // consumption.
  //
  // No other call uses this name, so it may be fully
  // descriptive, esp. of limitations.
  //
  // The name will be truncated to fit within "buflen" bytes.
  //
  // Possible Errors:
  //   AAFRESULT_CODEC_INVALID - The given codec ID is not loaded.
  //   AAFRESULT_SMALLBUF      - The buffer is not large enough to
  //                             hold the data.
  //
  STDMETHOD (GetCodecName) (
    // The size of the name buffer to be filled
    /*[in]*/ aafUInt32  namelen,

    // The buffer to fill
    /*[out,size_is(namelen)]*/ aafCharacter *  name);


  //***********************************************************
  //
  // GetCodecID()
  //
  // Returns the codec ID being used to handle the specified essence.
  //
  // This will be required in order to send private data to the
  // codec.
  //
  // The name will be truncated to fit within "buflen" bytes.
  //
  STDMETHOD (GetCodecID) (
    // Returns the current codec ID 
    /*[out]*/ aafCodecID_t *  codecID);


  //***********************************************************
  //
  // CompleteWrite()
  //
  // Handle any format related writing at the end and adjust mob
  // lengths.  Must be called before releasing a write essence
  // access.
  //
  // Possible Errors:
  //   AAFRESULT_STREAM_FULL
  //   - The essence can not be written because of a fault such as a
  //     disk full error in the underlying operating system.
  //
  STDMETHOD (CompleteWrite)
     ();


  //***********************************************************
  // METHOD NAME: WriteMultiSamples()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceMultiAccess | WriteMultiSamples |
  // Writes multiple channels worth of sample data to an interleaved
  // data stream in the natural order for the CODEC.
  // @end
  // 
  STDMETHOD (WriteMultiSamples)
   (
    // @parm [in] aafUInt16 | arrayElemCount | number of elements in the array of transfer operations
    aafUInt16  arrayElemCount,

    // @parm [in,size_is(arrayElemCount)] aafmMultiXfer_t * | xferArray | points to an array of transfer parameters.  All fields in this
    // array except for bytesXferred must be set up before doing the
    // transfer.  Some of the fields in the xferArray structure are
    // status results like bytesXferred and samplesXferred.
    //
    // The multiXfer_t structure has the following fields, which
	// specify one channel of data: 
    //   essenceDef    [IN] -- The essence type definition
    //   physical      [IN] -- The physical input-output channel
    //   numSamples    [IN] -- The number of samples to transfer
    //   buflen        [IN] -- The size of the buffer
    //   buffer        [IN] -- The buffer for this
    aafmMultiXfer_t *  xferArray,

    // @parm [out,size_is(arrayElemCount)] aafmMultiResult_t * | resultArray | put results into this array.  It has the following fields,
    // which return result for one channel of data:
    //   bytesXfered   [OUT] -- The total number of bytes transferred
    //   samplesXfered [OUT] -- The total number of samples transferred
    aafmMultiResult_t *  resultArray
  );


  //***********************************************************
  // METHOD NAME: ReadMultiSamples()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEssenceMultiAccess | ReadMultiSamples |
  // Reads one or more channels from an interleaved data stream.
  // Possible Errors:
  //
  // Standard errors (see top of file).
  //   AAFRESULT_END_OF_ESSENCE -- Hit the end of the essence (like
  //                               EOF) while reading.
  // @end
  // 
  STDMETHOD (ReadMultiSamples)
   (
    // @parm [in] aafUInt16 | elemCount | The size of the array for transfer operations.
    aafUInt16  elemCount,

    // @parm [in, size_is(elemCount)] aafmMultiXfer_t * | xferArray | Points to an array of transfer parameters.  All fields in this
    // array except for bytesXferred must be set up before doing the
	// transfer.  Some of the fields in the xferArray structure are
	// status results like bytesXferred and samplesXferred.
    //
    // The multiXfer_t structure has the following fields, which
	// specify one channel of data:
    // essenceDef  [IN] -- The essence type definition
    // physical    [IN] -- The physical input-output channel
    // numSamples  [IN] -- The number of samples to transfer
    // buflen      [IN] -- The size of the buffer
    // buffer      [IN] -- The buffer for this
    aafmMultiXfer_t *  xferArray,

    // @parm [out, size_is(elemCount)] aafmMultiResult_t * | resultArray | Results go into this array.
    //
    // The aafmMultiResult_t structure has the following fields,
	// which return result for one channel of data: 
    // bytesXfered   [OUT] -- The total number of bytes transferred
    // samplesXfered [OUT] -- The total number of samples transferred
    aafmMultiResult_t *  resultArray
  );



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
};

#endif // ! __CAAFEssenceAccess_h__


//@doc
//@class    AAFTimecodeStream | Implementation class for AAFTimecodeStream
#ifndef __CAAFTimecodeStream_h__
#define __CAAFTimecodeStream_h__

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


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif




#ifndef __CAAFSegment_h__
#include "CAAFSegment.h"
#endif


class CAAFTimecodeStream
  : public IAAFTimecodeStream,
    public CAAFSegment
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTimecodeStream (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTimecodeStream ();

public:


  //***********************************************************
  //
  // GetPositionTimecode()
  //
  // Get the timecode fields at the given position.
  //
  STDMETHOD (GetPositionTimecode) (
    // Zero based offset to set the timecode at
    /*[in]*/ aafPosition_t  position,

    // Timecode [startFrame drop fps]
    /*[out]*/ aafTimecode_t *  timecode);


  //***********************************************************
  //
  // SetPositionTimecode()
  //
  // Set the timecode fields for a given frame.  The frame index must
  // be within the length of the object.
  //
  STDMETHOD (SetPositionTimecode) (
    // Zero-based offset to set the timecode at
    /*[in]*/ aafPosition_t  position,

    // Timecode [startFrame  drop  fps]
    /*[in]*/ aafTimecode_t  timecode);


  //***********************************************************
  //
  // GetUserDataLength()
  //
  // Gets the length of the user data for one frame.
  //
  STDMETHOD (GetUserDataLength) (
    // Fixed length of the user data [userbits] in bytes 
    /*[out]*/ aafInt32 *  length);


  //***********************************************************
  //
  // GetUserDataAtPosition()
  //
  // Gets the user data [userbits] for a particular frame.
  //
  STDMETHOD (GetUserDataAtPosition) (
    // Zero-based offset
    /*[in]*/ aafPosition_t  position,

    // Length of the buffer
    /*[in]*/ aafInt32  buflen,

    // Passed in and filled with user data
    /*[out]*/ aafDataBuffer_t  buffer);


  //***********************************************************
  //
  // SetUserDataAtPosition()
  //
  // Gets the user data [userbits] for a particular frame.
  //
  STDMETHOD (SetUserDataAtPosition) (
    // Zero-based offset to set the userbits at
    /*[in]*/ aafPosition_t  position,

    // Length of the buffer
    /*[in]*/ aafInt32  buflen,

    // user data for the given frame
    /*[in]*/ aafDataBuffer_t  buffer);
							 

  //***********************************************************
  //
  // SetSampleRate()
  //
  // Sets the sample rate of the timecode data.
  //
  STDMETHOD (SetSampleRate) (
    // The sample rate of the timecode data 
    /*[in]*/ aafRational_t  sampleRate);


  //***********************************************************
  //
  // GetSampleRate()
  //
  // Gets the sample rate of the timecode data.
  //
  STDMETHOD (GetSampleRate) (
    // The sample rate of the timecode data 
    /*[out]*/ aafRational_t *  pSampleRate);


  //***********************************************************
  //
  // SetSourceType()
  //
  // Sets the type (LTC or VITC) of the timecode data.
  //
  STDMETHOD (SetSourceType) (
    // The type (LTC or VITC) of the timecode data 
    /*[in]*/ aafTimecodeSourceType_t  sourceType);


  //***********************************************************
  //
  // GetSourceType()
  //
  // Gets the type (LTC or VITC) of the timecode data.
  //
  STDMETHOD (GetSourceType) (
    // The type (LTC or VITC) of the timecode data 
    /*[out]*/ aafTimecodeSourceType_t *  pSourceType);


  //***********************************************************
  //
  // GetSampleSize()
  //
  // Gets the size (in samples) of one sample of the timecode data.
  //
  STDMETHOD (GetSampleSize) (
    // The size (in bytes) of one sample of the timecode data 
    /*[out]*/ aafUInt32 *  pSampleSize);


  //***********************************************************
  //
  // GetSource()
  //
  // Writes the entire timecode data value into the pValue buffer.
  // The buffer is allocated by the caller, and the size of the
  // buffer is given by valueSize.
  // 
  // Caller may call GetValueBufLen() to determine the
  // required buffer size.
  // 
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
  // - valueSize indicates the buffer is large enough to hold the name.
  // 
  // If this method fails nothing will be written to *pValue.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pValue arg is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - valueSize indicates the buffer is too small to hold the value.
  //
  STDMETHOD (GetSource) (
    // Size of preallocated buffer
    /*[in]*/ aafUInt32  valueSize,

    // Preallocated buffer to hold value
    /*[out, size_is(valueSize),length_is(*bytesRead)]*/ aafDataBuffer_t  pValue,

    // Number of actual bytes read
    /*[out]*/ aafUInt32*  bytesRead);


  //***********************************************************
  //
  // GetSourceBufLen()
  //
  // Returns the length of buffer required for the GetValue()
  // method.  The value is placed into the location specified by
  // pLen.
  //
  // Succeeds if all of the following are true:
  // - the pLen pointer is valid.
  //
  // If this method fails nothing will be written to *pLen.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLen arg is NULL.
  //
  STDMETHOD (GetSourceBufLen) (
    // Pointer to an variable used to return the length 
    /*[out]*/ aafUInt32 *  pLen);


  //***********************************************************
  //
  // SetSource()
  //
  // The data value is set from a buffer of size valueSize.
  //
  // Succeeds if all of the following are true:
  // - the pValue pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pValue is null.
  //
  STDMETHOD (SetSource) (
    // Size of preallocated buffer
    /*[in]*/ aafUInt32  valueSize,

    // buffer containing value
    /*[in, size_is(valueSize)]*/ aafDataBuffer_t  pValue);







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

#endif // ! __CAAFTimecodeStream_h__



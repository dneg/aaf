//@doc
//@class    AAFTimecodeStream | Implementation class for AAFTimecodeStream
#ifndef __ImplAAFTimecodeStream_h__
#define __ImplAAFTimecodeStream_h__


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

#include "OMDataStreamProperty.h"


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif





#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class OMDataStreamPropertyFilter;


class ImplAAFTimecodeStream : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTimecodeStream ();

protected:
  virtual ~ImplAAFTimecodeStream ();

public:

  //****************
  // GetPositionTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPositionTimecode
        (// @parm [in] Zero based offset to set the timecode at
         aafPosition_t  position,

         // @parm [out] Timecode [startFrame drop fps]
         aafTimecode_t *  timecode);


  //****************
  // SetPositionTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPositionTimecode
        (// @parm [in] Zero-based offset to set the timecode at
         aafPosition_t  position,

         // @parm [in] Timecode [startFrame  drop  fps]
         aafTimecode_t  timecode);


  //****************
  // GetUserDataLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetUserDataLength
        // @parm [out] Fixed length of the user data [userbits] in bytes
        (aafInt32 *  length);


  //****************
  // GetUserDataAtPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetUserDataAtPosition
        (// @parm [in] Zero-based offset
         aafPosition_t  position,

         // @parm [in] Length of the buffer
         aafInt32  buflen,

         // @parm [out] Passed in and filled with user data
         aafDataBuffer_t  buffer);


  //****************
  // SetUserDataAtPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetUserDataAtPosition
        (// @parm [in] Zero-based offset to set the userbits at
         aafPosition_t  position,

         // @parm [in] Length of the buffer
         aafInt32  buflen,

         // @parm [in] user data for the given frame
         aafDataBuffer_t  buffer);
							 

  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC (/*[in]*/ aafPosition_t *  pOffset,
      /*[out]*/ aafTimecode_t *  pTimecode);

  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset (/*[in]*/ aafTimecode_t *  pTimecode,
      /*[in]*/ aafRational_t *  pEditRate,
      /*[out]*/ aafFrameOffset_t *  pOffset);


  //****************
  // SetSampleRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSampleRate
        // @parm [in] The sample rate of the timecode data
        (aafRational_t  sampleRate);

  //****************
  // GetSampleRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleRate
        // @parm [out] The sample rate of the timecode data
        (aafRational_t *  pSampleRate);

  //****************
  // SetSourceType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceType
        // @parm [in] The type (LTC or VITC) of the timecode data
        (aafTimecodeSourceType_t  sourceType);

  //****************
  // GetSourceType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceType
        // @parm [out] The type (LTC or VITC) of the timecode data
        (aafTimecodeSourceType_t *  pSourceType);

  //****************
  // GetSampleSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleSize
        // @parm [out] The size (in bytes) of one sample of the timecode data
        (aafUInt32 *  pSampleSize);

/****/
  //****************
  // GetSource()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSource
        (// @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [out, size_is(valueSize),length_is(*bytesRead)] Preallocated buffer to hold value
         aafDataBuffer_t  pValue,

         // @parm [out] Number of actual bytes read
         aafUInt32*  bytesRead);


  //****************
  // GetSourceBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceBufLen
        // @parm [out] Pointer to an variable used to return the length
        (aafUInt32 *  pLen);

/****/
  //****************
  // SetSource()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSource
        (// @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [in, size_is(valueSize)] buffer containing value
         aafDataBuffer_t  pValue);

public:
	// SDK-internal calls
  virtual AAFRESULT STDMETHODCALLTYPE
	UnpackTimecode(aafUInt8 *buffer, aafUInt32 buflen, aafUInt32 fps, aafTimecode_t *tc);
  virtual AAFRESULT STDMETHODCALLTYPE
	PackTimecode(aafTimecode_t *tc, aafUInt8 *buffer, aafUInt32 buflen);
  virtual AAFRESULT STDMETHODCALLTYPE
	UnpackUserBits(aafUInt8 *packedBuffer, aafUInt32 packedBuflen,
					aafUInt8 *unpackedBuffer, aafUInt32 unpackedBuflen);
  virtual AAFRESULT STDMETHODCALLTYPE
	PackUserBits(aafUInt8 *unpackedBuffer, aafUInt32 unpackedBuflen,
					aafUInt8 *packedBuffer, aafUInt32 packedBuflen);

private: 
  virtual AAFRESULT STDMETHODCALLTYPE
    Write(aafUInt32  bytes, aafDataBuffer_t  buffer, aafUInt32 *  bytesWritten);
  virtual AAFRESULT STDMETHODCALLTYPE
    Read(aafUInt32  bytes, aafDataBuffer_t  buffer, aafUInt32 *  bytesRead);
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPosition(aafPosition_t  offset);
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPosition(aafPosition_t  *pOffset);
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSize(aafLength_t *  pSize );

   //****************
	OMFixedSizeProperty<aafRational_t>				_sampleRate;
	OMDataStreamProperty							_source;
	OMFixedSizeProperty<aafTimecodeSourceType_t>	_sourceType;
	OMDataStreamPropertyFilter*						_sourceStreamFilter;
};

#endif // ! __ImplAAFTimecodeStream_h__



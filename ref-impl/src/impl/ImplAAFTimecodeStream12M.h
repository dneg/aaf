//@doc
//@class    AAFTimecodeStream12M | Implementation class for AAFTimecodeStream12M
#ifndef __ImplAAFTimecodeStream12M_h__
#define __ImplAAFTimecodeStream12M_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif






#ifndef __ImplAAFTimecodeStream_h__
#include "ImplAAFTimecodeStream.h"
#endif


class ImplAAFTimecodeStream12M : public ImplAAFTimecodeStream
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTimecodeStream12M ();

protected:
  virtual ~ImplAAFTimecodeStream12M ();

public:

#if 0

/****/
  // Override from AAFTimecodeStream
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPositionTimecode (/*[in]*/ aafPosition_t  position,
      /*[out]*/ aafTimecode_t *  timecode);

  // Override from AAFTimecodeStream
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPositionTimecode (/*[in]*/ aafPosition_t  position,
      /*[in]*/ aafTimecode_t  timecode);


  // Override from AAFTimecodeStream
  virtual AAFRESULT STDMETHODCALLTYPE
    GetUserDataLength (/*[out]*/ aafInt32 *  length);

  // Override from AAFTimecodeStream
  virtual AAFRESULT STDMETHODCALLTYPE
    GetUserDataAtPosition (/*[in]*/ aafPosition_t  position,
      /*[in]*/ aafInt32  buflen,
      /*[out]*/ aafDataBuffer_t  buffer);

  // Override from AAFTimecodeStream
  virtual AAFRESULT STDMETHODCALLTYPE
    SetUserDataAtPosition (/*[in]*/ aafPosition_t  position,
      /*[in]*/ aafInt32  buflen,
      /*[in]*/ aafDataBuffer_t  buffer);


/****/
  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC (/*[in]*/ aafPosition_t *  pOffset,
      /*[out]*/ aafTimecode_t *  pTimecode);

  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset (/*[in]*/ aafTimecode_t *  pTimecode,
      /*[in]*/ aafRational_t *  pEditRate,
      /*[out]*/ aafFrameOffset_t *  pOffset);
#endif
  /****/
  // Override from AAFTimecodeStream
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleSize (/*[out]*/ aafUInt32 *  pSampleSize);


public:
	// SDK-internal calls
  virtual AAFRESULT STDMETHODCALLTYPE
	UnpackTimecode(aafUInt8 *buffer, aafUInt32 buflen, aafTimecode_t *tc);
  virtual AAFRESULT STDMETHODCALLTYPE
	PackTimecode(aafTimecode_t *tc, aafUInt8 *buffer, aafUInt32 buflen);
  virtual AAFRESULT STDMETHODCALLTYPE
	UnpackUserBits(aafUInt8 *packedBuffer, aafUInt32 packedBuflen,
					aafUInt8 *unpackedBuffer, aafUInt32 unpackedBuflen);
  virtual AAFRESULT STDMETHODCALLTYPE
	PackUserBits(aafUInt8 *unpackedBuffer, aafUInt32 unpackedBuflen,
					aafUInt8 *packedBuffer, aafUInt32 packedBuflen);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTimecodeStream12M)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTimecodeStream12MTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFTimecodeStream12M_h__



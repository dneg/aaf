/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/






#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"


#ifndef __ImplAAFTimecodeStream_h__
#include "ImplAAFTimecodeStream.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"

ImplAAFTimecodeStream::ImplAAFTimecodeStream ()
: _sampleRate(		PID_TimecodeStream_SampleRate,	"SampleRate"),
  _source(			PID_TimecodeStream_Source,		"Source"),
  _sourceType(		PID_TimecodeStream_SourceType,	"SourceType")
{
  _persistentProperties.put(_sampleRate.address());
  _persistentProperties.put(_source.address());
  _persistentProperties.put(_sourceType.address());
}


ImplAAFTimecodeStream::~ImplAAFTimecodeStream ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetPositionTimecode (
      aafPosition_t  /*position*/,
      aafTimecode_t *  /*timecode*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetPositionTimecode (
      aafPosition_t  /*position*/,
      aafTimecode_t  /*timecode*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetUserDataLength (
      aafInt32 *  /*length*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetUserDataAtPosition (
      aafPosition_t  /*position*/,
      aafInt32  /*buflen*/,
      aafDataBuffer_t  /*buffer*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetUserDataAtPosition (
      aafPosition_t  /*position*/,
      aafInt32  /*buflen*/,
      aafDataBuffer_t  /*buffer*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}							 

  // Override from AAFSegment
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SegmentOffsetToTC (/*[in]*/ aafPosition_t *pOffset,
      /*[out]*/ aafTimecode_t *pTimecode)
  {
		return(GetPositionTimecode (*pOffset, pTimecode));
  }

  // Override from AAFSegment
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SegmentTCToOffset (/*[in]*/ aafTimecode_t *pTimecode,
      /*[in]*/ aafRational_t *pEditRate,
      /*[out]*/ aafFrameOffset_t *pOffset)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }


  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream::UnpackTimecode(aafUInt8 *buffer, aafUInt32 buflen, aafTimecode_t *tc)
{
    return AAFRESULT_NOT_IMPLEMENTED;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream::PackTimecode(aafTimecode_t *tc, aafUInt8 *buffer, aafUInt32 buflen)
{
    return AAFRESULT_NOT_IMPLEMENTED;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream::UnpackUserBits(aafUInt8 *packedBuffer, aafUInt32 packedBuflen,
									  aafUInt8 *unpackedBuffer, aafUInt32 unpackedBuflen)
{
    return AAFRESULT_NOT_IMPLEMENTED;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream::PackUserBits(aafUInt8 *unpackedBuffer, aafUInt32 unpackedBuflen,
									  aafUInt8 *packedBuffer, aafUInt32 packedBuflen)
{
    return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetSampleRate (
      aafRational_t  /*sampleRate*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSampleRate (
      aafRational_t *  /*pSampleRate*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetSourceType (
      aafTimecodeSourceType_t  /*sourceType*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSourceType (
      aafTimecodeSourceType_t *  /*pSourceType*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSampleSize (
      aafUInt32 *  /*pSampleSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSource (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue,
      aafUInt32*  bytesRead)
{
	if(pValue == NULL || bytesRead == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_source.size() > valueSize)
	  return AAFRESULT_SMALLBUF;

	_source.copyToBuffer(pValue, valueSize);
	*bytesRead  = _source.size();

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSourceBufLen (
      aafUInt32 *  pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pLen = _source.size();

	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetSource (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue)
{
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);

	_source.setValue(pValue, valueSize);

	return(AAFRESULT_SUCCESS); 
}

OMDEFINE_STORABLE(ImplAAFTimecodeStream, AUID_AAFTimecodeStream);



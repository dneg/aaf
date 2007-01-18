//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=






#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "OMDataStreamPropertyFilter.h"


#ifndef __ImplAAFTimecodeStream_h__
#include "ImplAAFTimecodeStream.h"
#endif

#include <string.h>
#include "aafErr.h"

ImplAAFTimecodeStream::ImplAAFTimecodeStream ()
: _sampleRate(		PID_TimecodeStream_SampleRate,	L"SampleRate"),
  _source(			PID_TimecodeStream_Source,		L"Source"),
  _sourceType(		PID_TimecodeStream_SourceType,	L"SourceType"),
  _sourceStreamFilter(_source.createFilter())
{
  _persistentProperties.put(_sampleRate.address());
  _persistentProperties.put(_source.address());
  _persistentProperties.put(_sourceType.address());
}


ImplAAFTimecodeStream::~ImplAAFTimecodeStream ()
{
	delete _sourceStreamFilter;
	_sourceStreamFilter = NULL;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetPositionTimecode (
      aafPosition_t  position,
      aafTimecode_t *timecode)
{
	aafUInt32		sampleSize, bytesRead, fps;
	aafUInt8		*buffer = NULL;
	aafRational_t	rate;
	double			floatRate;

	XPROTECT()
	{
		if(timecode == NULL)
			RAISE(AAFRESULT_NULL_PARAM);

		CHECK(GetSampleRate(&rate));
		//!!! Need a better algorithm here
		floatRate = (double)rate.numerator / (double)rate.denominator;
		if((floatRate >= 29.96) && (floatRate <= 30.0))
			fps = 30;
		else
			fps = (aafUInt32)floatRate;
		timecode->fps = (aafUInt16)fps;
		CHECK(GetSampleSize(&sampleSize));
		buffer = new aafUInt8[sampleSize];
		if(buffer == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(SetPosition(position * sampleSize));
		CHECK(Read(sampleSize, buffer, &bytesRead));
		CHECK(UnpackTimecode(buffer, sampleSize, fps, timecode));
		delete [] buffer;
	}
	XEXCEPT
	{
		if(buffer != NULL)
			delete [] buffer;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetPositionTimecode (
      aafPosition_t  position,
      aafTimecode_t  timecode)
{
	aafUInt32		sampleSize, bytesRead, bytesWritten;
	aafUInt8		*packedBuf = NULL;

	XPROTECT()
	{
		CHECK(GetSampleSize(&sampleSize));
		packedBuf = new aafUInt8[sampleSize];
		if(packedBuf == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(SetPosition(position * sampleSize));
		(void)Read(sampleSize, packedBuf, &bytesRead);
		CHECK(PackTimecode(&timecode, packedBuf, sampleSize));
		CHECK(SetPosition(position * sampleSize));
		CHECK(Write(sampleSize, packedBuf, &bytesWritten));
		if(sampleSize != bytesWritten)
			RAISE(AAFRESULT_CONTAINERWRITE);
		delete [] packedBuf;
	}
	XEXCEPT
	{
		if(packedBuf != NULL)
			delete [] packedBuf;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetUserDataAtPosition (
      aafPosition_t  position,
      aafInt32  buflen,
      aafDataBuffer_t  buffer)
{
	aafUInt32		sampleSize, bytesRead;
	aafUInt8		*packedBuf = NULL;

	XPROTECT()
	{
		if(buffer == NULL)
			RAISE(AAFRESULT_NULL_PARAM);

		CHECK(GetSampleSize(&sampleSize));
		packedBuf = new aafUInt8[sampleSize];
		if(packedBuf == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(SetPosition(position * sampleSize));
		CHECK(Read(sampleSize, packedBuf, &bytesRead));
		CHECK(UnpackUserBits(packedBuf, sampleSize, buffer, buflen));
		delete [] packedBuf;
	}
	XEXCEPT
	{
		if(packedBuf != NULL)
			delete [] packedBuf;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetUserDataAtPosition (
      aafPosition_t  position,
      aafInt32  buflen,
      aafDataBuffer_t  buffer)
{
	aafUInt32		sampleSize, bytesRead, bytesWritten;
	aafUInt8		*packedBuf = NULL;

	XPROTECT()
	{
		CHECK(GetSampleSize(&sampleSize));
		packedBuf = new aafUInt8[sampleSize];
		if(packedBuf == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(SetPosition(position * sampleSize));
		CHECK(Read(sampleSize, packedBuf, &bytesRead));
		CHECK(PackUserBits(buffer, buflen, packedBuf, sampleSize));
		CHECK(SetPosition(position * sampleSize));
		CHECK(Write(sampleSize, packedBuf, &bytesWritten));
		if(sampleSize != bytesWritten)
			RAISE(AAFRESULT_CONTAINERWRITE);
		delete [] packedBuf;
	}
	XEXCEPT
	{
		if(packedBuf != NULL)
			delete [] packedBuf;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
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
	//!!! Is there a better algorithm assuming "mostly increasing"?
	// Since timecode need not be contiguous, or even increasing, we can try to optimize the search,
	// but we may have to search linearly.
	aafPosition_t	testOffset, scanStart;
	aafTimecode_t	baseTimecode, testTimecode;
	aafPosition_t	increment = pTimecode->fps * 10;	// Skip at 10 second intervals
	aafBool			found = kAAFFalse;
	aafInt32		error;
	aafLength_t		len, numSamples;
	aafUInt32		sampleSize;

	XPROTECT()
	{
		if(pEditRate == NULL || pOffset == NULL)
			RAISE(AAFRESULT_NULL_PARAM);
		scanStart = 0;
		CHECK(GetSampleRate(pEditRate));
		CHECK(GetPositionTimecode (0, &baseTimecode));

		// Assume that TC is contiguous, and prove otherwise
		testOffset = pTimecode->startFrame - baseTimecode.startFrame;
		CHECK(GetSize (&len));
		CHECK(GetSampleSize(&sampleSize));
		numSamples = len / sampleSize;
		testTimecode.startFrame = baseTimecode.startFrame;
		if((testOffset >= 0) && (testOffset < numSamples))
		{
			CHECK(GetPositionTimecode (testOffset, &testTimecode));
			if(	(pTimecode->drop == testTimecode.drop) && 
				(pTimecode->fps == testTimecode.fps) && 
				(pTimecode->startFrame == testTimecode.startFrame))
			{
				// TEST: Path #1 through code
				*pOffset = testOffset;
				found = kAAFTrue;
			}
			else
			{
				// See if timecodes are monatonically increasing anywhere near here.
				// See if we can compute an offset from the error in reaching here.
				testOffset += pTimecode->startFrame - testTimecode.startFrame;
				if(testOffset >= 0)
				{
					CHECK(GetPositionTimecode (testOffset, &testTimecode));
					if(	(pTimecode->drop == testTimecode.drop) && 
						(pTimecode->fps == testTimecode.fps) && 
						(pTimecode->startFrame == testTimecode.startFrame))
					{
						// TEST: Path #2 through code
						*pOffset = testOffset;
						found = kAAFTrue;
					}
				}
			}
		}

		
		if(!found)
		{
			// Start at zero, and skip forward at increments, until a timecode is found within
			// (increment) of the goal.
			testOffset = increment;
			scanStart = 0;
			if(testOffset > numSamples)
			{
				// TEST: Path #3 through code
				testOffset = 0;				// It's short, skip the scan phase	
			}
			else
			{
				// TEST: Path #4 through code
				for( ; testOffset < numSamples; testOffset += increment)
				{
					CHECK(GetPositionTimecode (testOffset, &testTimecode));
					error = (aafInt32)(pTimecode->startFrame - testTimecode.startFrame);
					if((error >= -increment) && (error <= increment))
					{
						if(error < 0)
							testOffset -= increment;	// Need to get a running start
						scanStart = testOffset;
						break;
					}
				}

				if(testOffset >= numSamples)
				{
					// TEST: Path #5 through code
					testOffset -= increment;	// Need to get a running start
					scanStart = testOffset;
				}
			}

			// Now finished scanning, start looking at testOffset, incrementing until the end
			for( ; testOffset < numSamples; testOffset++)
			{
				CHECK(GetPositionTimecode (testOffset, &testTimecode));
				if(	(pTimecode->drop == testTimecode.drop) && 
					(pTimecode->fps == testTimecode.fps) && 
					(pTimecode->startFrame == testTimecode.startFrame))
				{
					// TEST: Path #6 through code
					*pOffset = testOffset;
					found = kAAFTrue;
					break;
				}
			}
		}

		if(!found)
		{
			// If not found
			testOffset = 0;
			for(testOffset = 0; testOffset < scanStart; testOffset++)
			{
				CHECK(GetPositionTimecode (testOffset, &testTimecode));
				if(	(pTimecode->drop == testTimecode.drop) && 
					(pTimecode->fps == testTimecode.fps) && 
					(pTimecode->startFrame == testTimecode.startFrame))
				{
					// TEST: Path #7 through code
					*pOffset = testOffset;
					found = kAAFTrue;
					break;
				}
			}
		}

		if(found == kAAFFalse)
		{
			// TEST: Path #8 through code
			return AAFRESULT_TIMECODE_NOT_FOUND;
		}
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}


//*** Abstract operations which must be overridden by a subclass of AAFTimecodeStream
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetUserDataLength (
      aafInt32 * /*length*/)
{
  return AAFRESULT_ABSTRACT_CLASS;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream::UnpackTimecode(aafUInt8 * /*buffer*/, aafUInt32 /*buflen*/, aafUInt32 /*fps*/, aafTimecode_t * /*tc*/)
{
    return AAFRESULT_ABSTRACT_CLASS;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream::PackTimecode(aafTimecode_t * /*tc*/, aafUInt8 * /*buffer*/, aafUInt32 /*buflen*/)
{
    return AAFRESULT_ABSTRACT_CLASS;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream::UnpackUserBits(aafUInt8 * /*packedBuffer*/, aafUInt32 /*packedBuflen*/,
									  aafUInt8 * /*unpackedBuffer*/, aafUInt32 /*unpackedBuflen*/)
{
    return AAFRESULT_ABSTRACT_CLASS;
}

  AAFRESULT STDMETHODCALLTYPE
	ImplAAFTimecodeStream::PackUserBits(aafUInt8 * /*unpackedBuffer*/, aafUInt32 /*unpackedBuflen*/,
									  aafUInt8 * /*packedBuffer*/, aafUInt32 /*packedBuflen*/)
{
    return AAFRESULT_ABSTRACT_CLASS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetSampleRate (
      aafRational_t  sampleRate)
{
	_sampleRate = sampleRate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSampleRate (
      aafRational_t *pSampleRate)
{
	if (pSampleRate == NULL)
		return AAFRESULT_NULL_PARAM;
	*pSampleRate = _sampleRate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetSourceType (
      aafTimecodeSourceType_t  sourceType)
{
	_sourceType = sourceType;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSourceType (
      aafTimecodeSourceType_t *pSourceType)
{
	if (pSourceType == NULL)
		return AAFRESULT_NULL_PARAM;
	*pSourceType = _sourceType;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSampleSize (
      aafUInt32 *  /*pSampleSize*/)
{
  return AAFRESULT_ABSTRACT_CLASS;
}

//!!! Don't we also need a GetSourceAt/PutSourceAt and a seek?
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSource (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue,
      aafUInt32*  bytesRead)
{
	if(pValue == NULL || bytesRead == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_sourceStreamFilter->size() > valueSize)
	  return AAFRESULT_SMALLBUF;

	XPROTECT()
	{
		CHECK(SetPosition (0));
		CHECK(Read (valueSize, pValue, bytesRead));
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSourceBufLen (
      aafUInt32 *  pLen)
{
	aafLength_t	result;
	
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		CHECK(GetSize (&result));
		*pLen = (aafUInt32)result;		//!!! Change API to be a aafLength_t
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetSource (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue)
{
	aafUInt32	bytesWritten;
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		CHECK(SetPosition (0));
		CHECK(Write(valueSize, pValue, &bytesWritten));
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS); 
}

/*************************** Private Methods ***********/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFTimecodeStream::Write (aafUInt32  bytes,
                           aafDataBuffer_t buffer,
                           aafUInt32 *bytesWritten)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == buffer || NULL == bytesWritten)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  try
  {
    _sourceStreamFilter->write(buffer, bytes, *bytesWritten);
    if (0 < bytes && 0 == *bytesWritten)
      result = AAFRESULT_CONTAINERWRITE;
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }
  return result;
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFTimecodeStream::Read (aafUInt32  bytes,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesRead)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == buffer || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  try
  {
    _sourceStreamFilter->read(buffer, bytes, *bytesRead);
    if (0 < bytes && 0 == *bytesRead)
      result = AAFRESULT_END_OF_DATA;
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }
  return result;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::SetPosition (aafPosition_t  offset)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  try
  {
    OMUInt64 tmpOffset = offset;
    _sourceStreamFilter->setPosition(tmpOffset);
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }

  return result;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetPosition (aafPosition_t  *pOffset)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pOffset)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  try
  {
    OMUInt64 tmpOffset;
    tmpOffset = _sourceStreamFilter->position();
    *pOffset = tmpOffset;
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }

  return result;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecodeStream::GetSize (aafLength_t *pSize)
{
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;


  AAFRESULT result = AAFRESULT_SUCCESS;

  try
  {
    *pSize = _sourceStreamFilter->size();
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }

  return result;
}



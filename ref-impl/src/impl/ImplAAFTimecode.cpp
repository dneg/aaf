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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFTimecode_h__
#include "ImplAAFTimecode.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFBuiltinDefs.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"
#include "AAFUtils.h"
#include "AAFDataDefs.h"

ImplAAFTimecode::ImplAAFTimecode ():
_start(	PID_Timecode_Start,	L"Start"),
_FPS(	PID_Timecode_FPS,	L"FPS"),
_drop(	PID_Timecode_Drop,	L"Drop")
{
	_persistentProperties.put(_start.address());
	_persistentProperties.put(_FPS.address());
	_persistentProperties.put(_drop.address());
}

ImplAAFTimecode::~ImplAAFTimecode ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::Initialize (aafLength_t length,
                           aafTimecode_t *timecode)
{
	XPROTECT()
	{
		if (isInitialized ()) 
		{
			return AAFRESULT_ALREADY_INITIALIZED;
		}

		if (timecode == NULL)
		{
			return AAFRESULT_NULL_PARAM;
		}
		if ((timecode->drop != kAAFTcDrop) && (timecode->drop != kAAFTcNonDrop))
		{
			return AAFRESULT_INVALID_TIMECODE;
		}

		ImplAAFDictionarySP pDict;
		CHECK(GetDictionary (&pDict));
		CHECK(SetNewProps(length,
						  pDict->GetBuiltinDefs()->ddkAAFTimecode()));
		_start = timecode->startFrame;
		if (timecode->drop == kAAFTcDrop)
		{
		  _drop = kAAFTrue;
		}
		else
		{
		  _drop = kAAFFalse;
		}
		_FPS = timecode->fps;
		
		setInitialized ();
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::GetTimecode (aafTimecode_t *timecode)
{
	if ( !isInitialized ()) 
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if (timecode == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}

	timecode->startFrame = _start;

	if (kAAFTrue == _drop)
	{
		timecode->drop = kAAFTcDrop;
	}
	else
	{
		timecode->drop = kAAFTcNonDrop;
	}
	timecode->fps = _FPS;
	return(AAFRESULT_SUCCESS);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::SetTimecode (aafTimecode_t  *timecode)
{
	if ( !isInitialized ()) 
	{
		return AAFRESULT_NOT_INITIALIZED;
	}
	if (timecode == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	if ((timecode->drop != kAAFTcDrop) && (timecode->drop != kAAFTcNonDrop))
	{
		return AAFRESULT_INVALID_TIMECODE;
	}
	_start = timecode->startFrame;
	if (timecode->drop == kAAFTcDrop)
	{
		_drop = kAAFTrue;
	}
	else
	{
		_drop = kAAFFalse;
	}
	_FPS = timecode->fps;

	return(AAFRESULT_SUCCESS);
}


/****/
  // Override from AAFSegment
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::SegmentOffsetToTC (/*[in]*/ aafPosition_t *pOffset,
	  /*[out]*/ aafTimecode_t *pTimecode)
{
  aafUInt32		frameOffset;

  	if(pOffset == NULL)
		return(AAFRESULT_NULL_PARAM);
  	if(pTimecode == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
    {
		CHECK(GetTimecode(pTimecode));
	  	CHECK(TruncInt64toUInt32(*pOffset, &frameOffset));
		pTimecode->startFrame += frameOffset;
    }

  XEXCEPT
    {
    }
  XEND;

  return(AAFRESULT_SUCCESS);
}

  // Override from AAFSegment
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::SegmentTCToOffset (/*[in]*/ aafTimecode_t *pTimecode,
	  /*[in]*/ aafRational_t *pEditRate,
	  /*[out]*/ aafFrameOffset_t *pOffset)
{
	aafTimecode_t	startTC;
	aafPosition_t	oldStart;
	aafLength_t	tcLen;
  
  	if(pTimecode == NULL)
		return(AAFRESULT_NULL_PARAM);
  	if(pEditRate == NULL)
		return(AAFRESULT_NULL_PARAM);
  	if(pOffset == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		CHECK(GetTimecode(&startTC));
		CHECK(GetLength(&tcLen));
		
		// Assume found at this point, so finish generating result
		CvtInt32toInt64((pTimecode->startFrame - startTC.startFrame), &oldStart);

// Since the new call works only on a segment, the offset is assumed to be in the units
// of the enclosing segment, so no conversion need to be done.  The old code took a mob
// pointer, and so had some context
// CHECK(AAFConvertEditRate(_editRate, oldStart,
//		  							*pEditRate , kRoundFloor, &newStart));
//!!!		CHECK(TruncInt64toInt32(_offset, &frameOffset));	/* OK FRAMEOFFSET */
		// BobT 3/26/99: change aafFrameOffset_t to 64-bit, so we need to use a
		// 32-bit temp for this function call.
		aafUInt32 offset32;
		CHECK(TruncInt64toUInt32(oldStart, &offset32));		/* OK FRAMEOFFSET */
		*pOffset = offset32;

	   /* check for out of bound timecode */
	   if (pTimecode->startFrame < startTC.startFrame) 
	   {
			  /* out of left bound */
		    RAISE(AAFRESULT_BADSAMPLEOFFSET);
	   }
	   else
	   {
		    aafUInt32 len;
		    CHECK(TruncInt64toUInt32(tcLen, &len));
		    if (pTimecode->startFrame > (startTC.startFrame + len))
		    {
					/* out of right bound */
			     RAISE(AAFRESULT_BADSAMPLEOFFSET);
		    }
	   }
	 }
  XEXCEPT
	{
	}
  XEND;
  
  return(AAFRESULT_SUCCESS);

}


aafErr_t ImplAAFTimecode::OffsetToTimecodeClip(aafPosition_t /* offset !!!*/, ImplAAFTimecode **result,
												aafPosition_t *tcStartPos)
{
  	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	*result = this;
	CvtInt32toInt64(0, tcStartPos);
	return(AAFRESULT_SUCCESS);
}



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




#ifndef __ImplAAFTimecode_h__
#include "ImplAAFTimecode.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"
#include "aafUtils.h"
#include "AAFDefUIDs.h"

ImplAAFTimecode::ImplAAFTimecode ():
_start(	PID_Timecode_Start,	"Start"),
_FPS(	PID_Timecode_FPS,	"FPS"),
_drop(	PID_Timecode_Drop,	"Drop")
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
	aafUID_t	tcddef = DDEF_Timecode;
	
	XPROTECT()
	{
		if (timecode == NULL)
		{
			return AAFRESULT_NULL_PARAM;
		}
		if ((timecode->drop != kTcDrop) && (timecode->drop != kTcNonDrop))
		{
			return AAFRESULT_INVALID_TIMECODE;
		}

		CHECK(SetNewProps(length, &tcddef));
		_start = timecode->startFrame;
		if (timecode->drop == kTcDrop)
		{
		  _drop = AAFTrue;
		}
		else
		{
		  _drop = AAFFalse;
		}
		_FPS = timecode->fps;
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::GetTimecode (aafTimecode_t *timecode)
{
	if (timecode == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}

	timecode->startFrame = _start;
	if (AAFTrue == _drop)
	{
		timecode->drop = kTcDrop;
	}
	else
	{
		timecode->drop = kTcNonDrop;
	}
	timecode->fps = _FPS;
	return(AAFRESULT_SUCCESS);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::SetTimecode (aafTimecode_t  *timecode)
{
	if (timecode == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	if ((timecode->drop != kTcDrop) && (timecode->drop != kTcNonDrop))
	{
		return AAFRESULT_INVALID_TIMECODE;
	}
	_start = timecode->startFrame;
	if (timecode->drop == kTcDrop)
	{
		_drop = AAFTrue;
	}
	else
	{
		_drop = AAFFalse;
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
		CHECK(TruncInt64toUInt32(oldStart, pOffset));		/* OK FRAMEOFFSET */

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


aafErr_t ImplAAFTimecode::OffsetToTimecodeClip(aafPosition_t offset, ImplAAFTimecode **result,
												aafPosition_t *tcStartPos)
{
  	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	*result = this;
	CvtInt32toInt64(0, tcStartPos);
	return(AAFRESULT_SUCCESS);
}


OMDEFINE_STORABLE(ImplAAFTimecode, AUID_AAFTimecode);

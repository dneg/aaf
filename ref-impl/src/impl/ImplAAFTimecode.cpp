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

#include <assert.h>
#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"
#include "aafUtils.h"
#include "AAFDefUIDs.h"

ImplAAFTimecode::ImplAAFTimecode ():
_timecode(	PID_TIMECODE_TC,	"timecode")
{
	_persistentProperties.put(_timecode.address());
}

ImplAAFTimecode::~ImplAAFTimecode ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimecode::InitTimecode (aafLength_t length,
                           aafTimecode_t *timecode)
{
	aafUID_t	tcddef = DDEF_Timecode;
	
	XPROTECT()
	{
		CHECK(SetNewProps(length, &tcddef));
		if (timecode == NULL)
		{
			return AAFRESULT_NULL_PARAM;
		}
		_timecode = *timecode;
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

	*timecode = _timecode;
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
	_timecode = *timecode;
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

  return(OM_ERR_NONE);
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
		    RAISE(OM_ERR_BADSAMPLEOFFSET);
	   }
	   else
	   {
		    aafUInt32 len;
		    CHECK(TruncInt64toUInt32(tcLen, &len));
		    if (pTimecode->startFrame > (startTC.startFrame + len))
		    {
					/* out of right bound */
			     RAISE(OM_ERR_BADSAMPLEOFFSET);
		    }
	   }
	 }
  XEXCEPT
	{
	}
  XEND;
  
  return(OM_ERR_NONE);

}


aafErr_t ImplAAFTimecode::OffsetToTimecodeClip(aafPosition_t offset, ImplAAFTimecode **result,
												aafPosition_t *tcStartPos)
{
	*result = this;
	CvtInt32toInt64(0, tcStartPos);
	return(OM_ERR_NONE);
}

extern "C" const aafClassID_t CLSID_AAFTimecode;

OMDEFINE_STORABLE(ImplAAFTimecode, CLSID_AAFTimecode);

#if FULL_TOOLKIT
/*************************************************************************
 * Function: omfiTimecodeNew()
 *
 *      This function creates a new timecode clip with the given
 *      property values.  The timecode value is represented with an
 *      aafTimecode_t struct consisting of startFrame, drop, and fps.
 *
 *      This function supports both 1.x and 2.x files.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
AAFTimecode::AAFTimecode(AAFFile *file, OMLObject obj)
{
	Init(file, obj);
}

AAFTimecode::AAFTimecode(
    AAFFile * file,               /* IN - File Handle */
	aafLength_t length,          /* IN - Length Property Value */
	aafTimecode_t timecode)      /* IN - Timecode Value (startFrame,
								  *      drop, fps)
								  */
{
	AAFObject * tmpTCClip = NULL;
	AAFDataKind * datakind = NULL;
	aafErr_t aafError = OM_ERR_NONE;
	
	XPROTECT(_file)
	  {
		CreatePersistant(file, "TCCP");

		/* Set datakind object for timecode from cache */
		if (!_head->DatakindLookup(TIMECODEKIND, &datakind, &aafError))
		  {
			RAISE(OM_ERR_INVALID_DATAKIND);
		  }

		CHECK(SetNewProps(length, datakind));
		_timecode = timecode;
	  }

	XEXCEPT
	  {
	  }
	XEND_VOID;
}

/*************************************************************************
 * Function: omfiTimecodeGetInfo()
 *************************************************************************/
aafErr_t AAFTimecode::GetTimecode(
	aafTimecode_t *timecode)  /* OUT - Timecode (startFrame, drop, fps) */
{
	aafAssertValidFHdl(_file);
	aafAssert(timecode != NULL, _file, OM_ERR_NULL_PARAM);

	*timecode = _timecode;

	return(OM_ERR_NONE);
}

aafErr_t AAFTimecode::SegmentOffsetToTC(aafPosition_t offset, aafTimecode_t *tc, aafBool *found)
{
}


aafErr_t AAFTimecode::SegmentTCToOffset(aafTimecode_t tc, aafRational_t editRate, aafFrameOffset_t *offset, aafBool *found)
{
}


#endif
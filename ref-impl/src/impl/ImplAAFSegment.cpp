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




#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include <assert.h>
#include <AAFResult.h>

#include "aafCvt.h" 
#include "ImplAAFTimecode.h"
#include "ImplAAFSequence.h"


ImplAAFSegment::ImplAAFSegment ()
{}


ImplAAFSegment::~ImplAAFSegment ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSegment::SegmentOffsetToTC (aafPosition_t *pOffset,
                           aafTimecode_t *pTimecode)
{
    AAFRESULT aafError = AAFRESULT_TIMECODE_NOT_FOUND;
	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSegment::SegmentTCToOffset (aafTimecode_t *pTimecode,
                           aafRational_t *pEditRate,
                           aafFrameOffset_t *pOffset)
{
    AAFRESULT aafError = AAFRESULT_TIMECODE_NOT_FOUND;
	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSegment::NumRepresentations (aafInt32 *pCount)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	*pCount = 1;
	return aafError;
}


AAFRESULT ImplAAFSegment::AccumulateLength( aafLength_t *length)
{

	aafLength_t		len;
	
	XPROTECT()
	  {
		CHECK(GetLength(&len));
		CHECK(AddInt64toInt64(len, length));
	  }
	XEXCEPT
	  {
	  }
	XEND;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFSegment::OffsetToTimecodeClip(aafPosition_t offset,
											   ImplAAFTimecode **result,
											   aafPosition_t *tcStartPos)
{
	*result = (ImplAAFTimecode*) NULL;
	CvtInt32toInt64(0, tcStartPos);
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFSegment::FindSubSegment(aafPosition_t offset, 
										 aafPosition_t *sequPosPtr,
										 ImplAAFSegment **subseg,
										 aafBool *found)
{
	aafLength_t	segLen;
	aafPosition_t begPos, endPos;
	
	XPROTECT( )
	{
		CHECK(GetLength(&segLen));
		CvtInt32toPosition(0, begPos);
		CHECK(AddInt64toInt64(segLen, &endPos));
		if (Int64LessEqual(begPos, offset) &&
			Int64Less(offset, endPos))
		{
			*found = AAFTrue;
			*subseg = this;
			*sequPosPtr = 0;
		}
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFSegment::TraverseToClip(aafLength_t length,
										 ImplAAFSegment **sclp,
										 ImplAAFPulldown **pulldownObj,
										 aafInt32 *pulldownPhase,
										 aafLength_t *sclpLen,
										 aafBool *isMask)
{
	return(AAFRESULT_TRAVERSAL_NOT_POSS);
}

AAFRESULT ImplAAFSegment::GenerateSequence(ImplAAFSequence **seq)
{
	ImplAAFSequence	*tmp;
// ***	ImplAAFDataDef	*datakind;
				
	XPROTECT( )
	{
// ***	CHECK(GetDatakind(&datakind));
		tmp = new ImplAAFSequence();
		CHECK(tmp->AppendCpnt(this));
		*seq = tmp;
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

extern "C" const aafClassID_t CLSID_AAFSegment;

OMDEFINE_STORABLE(ImplAAFSegment, CLSID_AAFSegment);


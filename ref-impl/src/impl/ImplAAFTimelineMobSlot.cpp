

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


////////////////////////////////////////////////////////////////////////////////
// Types required by this module:
//
// aafRational_t,
// aafPosition_t,



#ifndef __ImplAAFTimelineMobSlot_h__
#include "ImplAAFTimelineMobSlot.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <AAFResult.h>
#include "aafCvt.h" 
#include "AAFUtils.h"

ImplAAFTimelineMobSlot::ImplAAFTimelineMobSlot ():
	_editRate(	PID_TimelineMobSlot_EditRate,	"EditRate"),
	_origin(	PID_TimelineMobSlot_Origin,		"Origin")

{
	_persistentProperties.put( _editRate.address());
	_persistentProperties.put( _origin.address());

}


ImplAAFTimelineMobSlot::~ImplAAFTimelineMobSlot ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTimelineMobSlot::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetEditRate (aafRational_t *editRate)
{
	if (editRate == NULL)
		return AAFRESULT_NULL_PARAM;

	*editRate = _editRate;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::SetEditRate (aafRational_t *editRate)
{
	if (editRate == NULL)
		return AAFRESULT_NULL_PARAM;

	_editRate = *editRate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetOrigin (aafPosition_t *origin)
{
	if (origin == NULL)
		return AAFRESULT_NULL_PARAM;

	*origin = _origin;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::SetOrigin (aafPosition_t origin)
{
	_origin = origin;

	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFTimelineMobSlot::FindSegment(aafPosition_t offset,
					  ImplAAFSegment **segment,
					  aafRational_t *srcRate,
					  aafPosition_t *diffPos)
{
	aafBool					foundClip = AAFFalse;
	ImplAAFMobSlot			*tmpTrack = NULL;
	aafPosition_t begPos = CvtInt32toPosition(0, begPos);
	aafRational_t tmpSrcRate;
	aafPosition_t origin = 0;
	aafSlotID_t tmpTrackID;
	ImplAAFSegment	*tmpSegment = NULL;

	if(diffPos == NULL || segment == NULL || srcRate == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		/* Initialize return parameters */
		*segment = NULL;
				
		CHECK(GetEditRate(&tmpSrcRate));
		CHECK(GetOrigin(&origin));
		CHECK(GetSlotID(&tmpTrackID));
		CHECK(GetSegment(&tmpSegment));
		*srcRate = tmpSrcRate;
		
		/* Normalize the requested position on the track by adding
		* the StartPosition (1.x) or Origin (2.x) to it.  The origin
		* was acquired above with omfiTrackGetInfo().
		* The StartPosition/Origin will usually be 0.  It may be 
		* negative, if more data before the original "origin" was 
		* digitized later.
		*/
		if (!IsInt64Positive(origin))
			MultInt32byInt64(-1, origin, &origin);
		AddInt64toInt64(origin, &offset);
		
		CHECK(tmpSegment->FindSubSegment(offset, &begPos, segment, &foundClip));
		if(!foundClip)
			RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);

		/* Calculate diffPos - difference between requested offset and
		* the beginning of clip that contains it. 
		*/
		(*diffPos) = offset;
		CHECK(SubInt64fromInt64(begPos, diffPos));
		tmpSegment->ReleaseReference();
		
	} /* XPROTECT */
	XEXCEPT
	{
		if (tmpSegment)	
			tmpSegment->ReleaseReference();
	}
	XEND;
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFTimelineMobSlot::ConvertToEditRate(aafPosition_t srcPos,
										aafRational_t destRate,
										aafPosition_t *convertPos)
{
	aafRational_t	srcRate;

	XPROTECT()
	{
		CHECK(GetEditRate(&srcRate));
		CHECK(AAFConvertEditRate(srcRate, srcPos,
						destRate, kRoundFloor, convertPos));
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFTimelineMobSlot::ConvertToMyRate(aafPosition_t srcPos,
										  ImplAAFMobSlot *srcSlot,
										aafPosition_t *convertPos)
{
	aafRational_t	destRate;
	
	XPROTECT()
	{
		CHECK(GetEditRate(&destRate));
		CHECK(srcSlot->ConvertToEditRate(srcPos, destRate, convertPos));
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}


OMDEFINE_STORABLE(ImplAAFTimelineMobSlot, AUID_AAFTimelineMobSlot);


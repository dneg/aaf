

//=---------------------------------------------------------------------=
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
	_editRate(	PID_TimelineMobSlot_EditRate,	L"EditRate"),
	_origin(	PID_TimelineMobSlot_Origin,		L"Origin")

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
    ImplAAFTimelineMobSlot::SetEditRate (const aafRational_t & editRate)
{
	_editRate = editRate;
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
	aafBool					foundClip = kAAFFalse;
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
		tmpSegment = 0;
		
	} /* XPROTECT */
	XEXCEPT
	{
		if (tmpSegment)	
		  tmpSegment->ReleaseReference();
		tmpSegment = 0;
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




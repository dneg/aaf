

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
#include "AAFResult.h"
#include "aafCvt.h" 
#include "AAFUtils.h"

ImplAAFTimelineMobSlot::ImplAAFTimelineMobSlot ():
	_editRate(	PID_TimelineMobSlot_EditRate,	L"EditRate"),
	_origin(	PID_TimelineMobSlot_Origin,		L"Origin"),
	_markIn(	PID_TimelineMobSlot_MarkIn,		L"MarkIn"),
	_markOut(	PID_TimelineMobSlot_MarkOut,		L"MarkOut"),
	_userPos(	PID_TimelineMobSlot_UserPos,		L"UserPos")

{
	_persistentProperties.put( _editRate.address());
	_persistentProperties.put( _origin.address());
	_persistentProperties.put( _markIn.address());
	_persistentProperties.put( _markOut.address());
	_persistentProperties.put( _userPos.address());

	// Initialize the required properties with bogus values.
	const aafRational_t  null_rational = { 0, 0 };
	_editRate = null_rational;
	_origin = 0;
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

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::SetMarkIn (aafPosition_t  markIn)
{
	_markIn = markIn;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetMarkIn (aafPosition_t *markIn)
{
	if ( markIn == NULL )
		return AAFRESULT_NULL_PARAM;

	if ( !_markIn.isPresent() )
		return AAFRESULT_PROP_NOT_PRESENT;

	*markIn = _markIn;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::SetMarkOut (aafPosition_t  markOut)
{
	_markOut = markOut;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetMarkOut (aafPosition_t *markOut)
{
	if ( markOut == NULL )
		return AAFRESULT_NULL_PARAM;

	if ( !_markOut.isPresent() )
		return AAFRESULT_PROP_NOT_PRESENT;

	*markOut = _markOut;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::SetUserPos (aafPosition_t  userPos)
{
	_userPos = userPos;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetUserPos (aafPosition_t *userPos)
{
	if ( userPos == NULL )
		return AAFRESULT_NULL_PARAM;

	if ( !_userPos.isPresent() )
		return AAFRESULT_PROP_NOT_PRESENT;

	*userPos = _userPos;
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
		
		// The origin of a track is normally zero - ie. it is
		// at the first sample position.  If samples are
		// inserted at the start of the track, then the origin
		// shifts to the right and is greater than zero.  If
		// samples are removed from the start of the track the
		// origin shifts to the left and is less than zero.
		//
		// External sample references (by SourceClip's) into
		// the track are always relative to the origin.  This
		// ensures that external references do not have to be
		// adjusted when samples are inserted or removed.
		//
		// For example, if a SourceClip references sample
		// number 10, then someone inserts 5 samples, the
		// origin and the referenced sample shift 5 positions
		// to the right.  When the source clip fetches (what
		// it considers to be) sample 10, it actually gets
		// sample 10+5 = 15.
		// 
		// All that, to explain the following line of code:
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




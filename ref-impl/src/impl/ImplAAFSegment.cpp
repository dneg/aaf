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


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include <assert.h>
#include "aafErr.h"
#include "AAFResult.h"

#include "ImplAAFTimecode.h"
#include "ImplAAFSequence.h"

#include "AAFStoredObjectIDs.h"
#include "AAFClassIDs.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFBuiltinDefs.h"


ImplAAFSegment::ImplAAFSegment ()
{}


ImplAAFSegment::~ImplAAFSegment ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSegment::SegmentOffsetToTC (aafPosition_t *pOffset,
                           aafTimecode_t *pTimecode)
{
	AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (pOffset == NULL ||
		pTimecode == NULL)
		aafError = AAFRESULT_NULL_PARAM;
	else
		aafError = AAFRESULT_TIMECODE_NOT_FOUND;

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSegment::SegmentTCToOffset (aafTimecode_t *pTimecode,
                           aafRational_t * /*pEditRate*/,
                           aafFrameOffset_t *pOffset)
{
	AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (pOffset == NULL ||
		pTimecode == NULL)
		aafError = AAFRESULT_NULL_PARAM;
	else
		aafError = AAFRESULT_TIMECODE_NOT_FOUND;

	return aafError;
}


AAFRESULT ImplAAFSegment::NumRepresentations (aafUInt32 *pCount)
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
		*length += len;
	  }
	XEXCEPT
	  {
	  }
	XEND;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFSegment::OffsetToTimecodeClip(aafPosition_t /*offset*/,
											   ImplAAFTimecode **result,
											   aafPosition_t *tcStartPos)
{
	*result = (ImplAAFTimecode*) NULL;
	*tcStartPos = 0;
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFSegment::FindSubSegment(aafPosition_t offset, 
										 aafPosition_t *sequPosPtr,
										 ImplAAFSegment **subseg,
										 aafBool *found)
{
	aafLength_t	segLen;
	aafPosition_t begPos, endPos, zero;
	
	XPROTECT( )
	{
		CHECK(GetLength(&segLen));
		begPos = 0;
		zero = 0;
		endPos = begPos;
		endPos += segLen;
		if ((begPos <= offset) &&
			(offset < endPos))
		{
			*found = kAAFTrue;
			*subseg = this;
			// We are returning a reference to this object so bump the ref count
			AcquireReference();
			*sequPosPtr = 0;
		}
		else if ((begPos == endPos) && (offset == zero))	 //JeffB: Handle zero-length sourceClips
		{
			*found = kAAFTrue;
			*subseg = this;
			// We are returning a reference to this object so bump the ref count
			AcquireReference();
			*sequPosPtr = 0;
		}
		else
		{
			*found = kAAFFalse;
			*subseg = NULL;
			*sequPosPtr = 0;
		}
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFSegment::TraverseToClip(aafLength_t /*length*/,
										 ImplAAFSegment ** /*sclp*/,
										 ImplAAFPulldown ** /*pulldownObj*/,
										 aafInt32 * /*pulldownPhase*/,
										 aafLength_t * /*sclpLen*/,
										 aafBool * /*isMask*/)
{
	return(AAFRESULT_TRAVERSAL_NOT_POSS);
}

AAFRESULT ImplAAFSegment::GenerateSequence(ImplAAFSequence **seq)
{
  ImplAAFDictionary *pDictionary = NULL;
	ImplAAFSequence	*tmp = NULL;
// ***	ImplAAFDataDef	*datakind;
				
	XPROTECT( )
	{
// ***	CHECK(GetDatakind(&datakind));
    CHECK(GetDictionary(&pDictionary));
	CHECK(pDictionary->GetBuiltinDefs()->cdSequence()->
		  CreateInstance ((ImplAAFObject**) &tmp));
    pDictionary->ReleaseReference();
    pDictionary = NULL;

		CHECK(tmp->AppendComponent(this));
		*seq = tmp;
	} /* XPROTECT */
	XEXCEPT
	{
    if (tmp)
      tmp->ReleaseReference();
	tmp = 0;
    if(pDictionary != NULL)
      pDictionary->ReleaseReference();
	pDictionary = 0;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}




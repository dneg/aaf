/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/
#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include <assert.h>
#include <AAFResult.h>

#include "aafCvt.h" 
#include "ImplAAFTimecode.h"
#include "ImplAAFSequence.h"

#include "AAFStoredObjectIDs.h"
#include "AAFClassIDs.h"
#include "ImplAAFDictionary.h"


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
                           aafRational_t *pEditRate,
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


AAFRESULT ImplAAFSegment::NumRepresentations (aafInt32 *pCount)
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
		endPos = begPos;
		CHECK(AddInt64toInt64(segLen, &endPos));
		if (Int64LessEqual(begPos, offset) &&
			Int64Less(offset, endPos))
		{
			*found = AAFTrue;
			*subseg = this;
			// We are returning a reference to this object so bump the ref count
			AcquireReference();
			*sequPosPtr = 0;
		}
		else
		{
			*found = AAFFalse;
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
  ImplAAFDictionary *pDictionary = NULL;
	ImplAAFSequence	*tmp = NULL;
// ***	ImplAAFDataDef	*datakind;
				
	XPROTECT( )
	{
// ***	CHECK(GetDatakind(&datakind));
    CHECK(GetDictionary(&pDictionary));
    tmp = (ImplAAFSequence *)pDictionary->CreateImplObject(AUID_AAFSequence);
    if (NULL == tmp)
      RAISE(AAFRESULT_NOMEMORY);
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


OMDEFINE_STORABLE(ImplAAFSegment, AUID_AAFSegment);


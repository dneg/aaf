//@doc
//@class    AAFSegment | Implementation class for AAFSegment
#ifndef __ImplAAFSegment_h__
#define __ImplAAFSegment_h__

#include "OMStorable.h"

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

/******************************************\
*                                          *
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



#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

class ImplAAFPulldown;
class ImplAAFTimecode;
class ImplAAFSequence;

class ImplAAFSegment : public ImplAAFComponent
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSegment ();
  virtual ~ImplAAFSegment ();



  //****************
  // SegmentOffsetToTC()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC
	    (// @parm [in] Pointer to a Segment Offset to be convert to Timecode
         aafPosition_t *  pOffset,

		 // @parm [out] The converted timecode to be returned
		 aafTimecode_t *  pTimecode);

  //****************
  // SegmentTCToOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset
        (// @parm [in] Pointer to a timecode to be converted to Offset
		 aafTimecode_t *  pTimecode,

		 // @parm [in] The edit rate for the given timecode 
		 aafRational_t *  pEditRate,

		 // @parm [out] Frame Offset to be returned if found
         aafFrameOffset_t *  pOffset);



public:

	virtual AAFRESULT NumRepresentations(aafInt32 *  pCount);

	virtual AAFRESULT AccumulateLength(aafLength_t *length);

	virtual AAFRESULT FindSubSegment( aafPosition_t offset,
									  aafPosition_t *sequPosPtr,
									  ImplAAFSegment **subseg,
									  aafBool *found);

	virtual AAFRESULT TraverseToClip( aafLength_t length,
									  ImplAAFSegment **sclp,
									  ImplAAFPulldown **pulldownObj,
									  aafInt32 *pulldownPhase,
									  aafLength_t *sclpLen,
									  aafBool *isMask);

	virtual AAFRESULT OffsetToTimecodeClip( aafPosition_t offset, 
											ImplAAFTimecode **result,
											aafPosition_t *tcStartPos);

	virtual AAFRESULT GenerateSequence(ImplAAFSequence **seq);

	virtual AAFRESULT GetComponentType(implCompType_t* pType) {*pType = kSegment; return AAFRESULT_SUCCESS;}
};

#endif // ! __ImplAAFSegment_h__


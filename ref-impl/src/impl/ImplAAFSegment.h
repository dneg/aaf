//@doc
//@class    AAFSegment | Implementation class for AAFSegment
#ifndef __ImplAAFSegment_h__
#define __ImplAAFSegment_h__

#include "OMStorable.h"

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

	virtual AAFRESULT NumRepresentations(aafUInt32 *  pCount);

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

	// OM deep copy notification
	virtual void onCopy(void* clientContext) const;
};

#endif // ! __ImplAAFSegment_h__


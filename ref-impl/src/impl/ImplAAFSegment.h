//@doc
//@class    AAFSegment | Implementation class for AAFSegment
#ifndef __ImplAAFSegment_h__
#define __ImplAAFSegment_h__

#include "OMStorable.h"

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
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/





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
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFSegment)


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


//@doc
//@class    AAFSegment | Implementation class for AAFSegment
#ifndef __ImplAAFSegment_h__
#define __ImplAAFSegment_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
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
  ~ImplAAFSegment ();

  OMDECLARE_STORABLE(ImplAAFSegment);

  //****************
  // SegmentOffsetToTC()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC
        (aafPosition_t  offset,   //@parm [in] 
		 aafTimecode_t *  tc,   //@parm [out] 
         aafBool *  found);  //@parm [out] 


  //****************
  // SegmentTCToOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset
        (aafTimecode_t  tc,   //@parm [in] 
		 aafRational_t  editRate,   //@parm [in] 
         aafFrameOffset_t *  offset,   //@parm [out] 
		 aafBool *  found);  //@parm [out] 


  //****************
  // NumRepresentations()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NumRepresentations
        (aafInt32 *  count);  //@parm [out] 


public:
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
	
	// Declare the module test method. The implementation of the will be be
	// in /test/ImplAAFSegmentTest.cpp.
	static AAFRESULT test();
};

#endif // ! __ImplAAFSegment_h__


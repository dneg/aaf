//@doc
//@class    AAFPulldown | Implementation class for AAFPulldown
#ifndef __ImplAAFPulldown_h__
#define __ImplAAFPulldown_h__


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFPulldown : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFPulldown ();

protected:
  virtual ~ImplAAFPulldown ();

public:


  //****************
  // GetInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetInputSegment
        // @parm [out] Returns the input Segment object
        (ImplAAFSegment ** ppInputSegment);

  //****************
  // SetInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetInputSegment
        // @parm [in] A Segment object
        (ImplAAFSegment * pInputSegment);

  //****************
  // GetPulldownKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPulldownKind
        // @parm [out] Returns the pulldownKind property
        (aafPulldownKind_t *  pPulldownKind);



  //****************
  // SetPulldownKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPulldownKind
        // @parm [in] A Segment object
        (aafPulldownKind_t  pulldownKind);



  //****************
  // GetPulldownDirection()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPulldownDirection
        // @parm [out] Returns the input Segment object
        (aafPulldownDir_t *  pPulldownDirection);

  //****************
  // SetPulldownDirection()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPulldownDirection
        // @parm [in] A Segment object
        (aafPulldownDir_t  pulldownDirection);

  //****************
  // GetPhaseFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPhaseFrame
        // @parm [out] Returns the input Segment object
        (aafPhaseFrame_t *  pPhaseFrame);

  //****************
  // SetPhaseFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPhaseFrame
        // @parm [in] A Segment object
        (aafPhaseFrame_t  phaseFrame);

  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC (/*[in]*/ aafPosition_t *  pOffset,
      /*[out]*/ aafTimecode_t *  pTimecode);

  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset (/*[in]*/ aafTimecode_t *  pTimecode,
      /*[in]*/ aafRational_t *  pEditRate,
      /*[out]*/ aafFrameOffset_t *  pOffset);

// SDK-private functions
	/* Functions to support calculating dropping/doubling frames with masks */
	AAFRESULT MapOffset(aafPosition_t offset,
			   aafBool reverse,
			   aafLength_t *numFrames,
			   aafInt32 *srcPhase);
	AAFRESULT aafPvtGetPulldownMask(aafPulldownKind_t	pulldown,
			   aafUInt32 			*outMask,
			   aafInt32				*maskLen,
			   aafBool	 			*isOneToOne);
	virtual AAFRESULT intSegmentOffsetToTC(aafPosition_t offset, aafTimecode_t *tc, aafBool *found);
	virtual AAFRESULT TraverseToClip(aafLength_t length,
					ImplAAFSegment **sclp,
					 ImplAAFPulldown **pulldownObj,
					 aafInt32 *pulldownPhase,
					 aafLength_t *sclpLen,
					 aafBool *isMask);
private:
	AAFRESULT GetRemFramesDrop(aafUInt32 maskBits, 
							char ones, 
							aafUInt32 phase,
							aafUInt32 masksize,
							aafInt32 *result);

	AAFRESULT GetRemFramesDouble(aafUInt32 maskBits, 
							char ones,
							aafUInt32 phase,
							aafUInt32 masksize,
							aafInt32 *result);
	AAFRESULT MaskGetBits(aafUInt32 maskBits, char *maskones);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFPulldown)

private:
	OMStrongReferenceProperty<ImplAAFSegment>	_inputSegment;
	OMFixedSizeProperty<aafPulldownKind_t>		_pulldownKind;
	OMFixedSizeProperty<aafPulldownDir_t>		_pulldownDirection;
	OMFixedSizeProperty<aafPhaseFrame_t>		_phaseFrame;
};

#endif // ! __ImplAAFPulldown_h__



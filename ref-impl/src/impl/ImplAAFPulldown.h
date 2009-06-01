//@doc
//@class    AAFPulldown | Implementation class for AAFPulldown
#ifndef __ImplAAFPulldown_h__
#define __ImplAAFPulldown_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include "OMStrongRefProperty.h"

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
	virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);
	// Visit this component and its sub-components
	// with the specified visitor.
	virtual void Accept(AAFComponentVisitor& visitor);

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

private:
	OMStrongReferenceProperty<ImplAAFSegment>	_inputSegment;
	OMFixedSizeProperty<aafPulldownKind_t>		_pulldownKind;
	OMFixedSizeProperty<aafPulldownDir_t>		_pulldownDirection;
	OMFixedSizeProperty<aafPhaseFrame_t>		_phaseFrame;
};

#endif // ! __ImplAAFPulldown_h__



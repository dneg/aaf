//@doc
//@class    AAFPulldown | Implementation class for AAFPulldown
#ifndef __ImplAAFPulldown_h__
#define __ImplAAFPulldown_h__


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



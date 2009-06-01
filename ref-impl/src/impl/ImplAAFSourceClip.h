//@doc
//@class    AAFSourceClip | Implementation class for AAFSourceClip
#ifndef __ImplAAFSourceClip_h__
#define __ImplAAFSourceClip_h__

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


class ImplAAFDataDef;

class ImplAAFMob;

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif





class ImplAAFSourceClip : public ImplAAFSourceReference
{
public:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFSourceClip ();
  virtual ~ImplAAFSourceClip ();


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] Data Definition object
		 ImplAAFDataDef * pDataDef,

		 // @parm [in] Length property value
		 const aafLength_t & length,

		 // @parm [in] Source Reference
         const aafSourceRef_t & sourceRef);

  //****************
  // GetFade()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFade
        (// @parm [out] Fade In Length
		 aafLength_t *       fadeInLen     ,

		 // @parm [out] Fade In Type
		 aafFadeType_t *  fadeInType    ,

		 // @parm [out] Fade In Type
         aafBool *        fadeInPresent ,

		 // @parm [out] Fade Out Length
		 aafLength_t *     fadeOutLen    ,

		 // @parm [out] Fade Out Type
         aafFadeType_t *  fadeOutType   ,

		 // @parm [out] Fade In Type
		 aafBool *        fadeOutPresent);

  //****************
  // ResolveRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ResolveRef
		// @parm [out] Referenced mob
        (ImplAAFMob ** mob);

  //****************
  // GetSourceReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceReference
		// @parm [out] Source Reference
        (aafSourceRef_t *  sourceRef);

  //****************
  // SetFade()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFade
        (// @parm [in] Fade In Length
		 aafInt32       fadeInLen  ,

		 // @parm [in] Fade In Type
		 aafFadeType_t  fadeInType ,

		 // @parm [in] Fade Out Length
         aafInt32       fadeOutLen ,

		 // @parm [in] Fade Out Type
		 aafFadeType_t  fadeOutType);

  //****************
  // SetSourceReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceReference
		// @parm [in] Source Reference
        (aafSourceRef_t  sourceRef);


public:

	virtual AAFRESULT TraverseToClip(aafLength_t length,
					ImplAAFSegment **sclp,
					 ImplAAFPulldown **pulldownObj,
					 aafInt32 *pulldownPhase,
					 aafLength_t *sclpLen,
					 aafBool *isMask);

	// Visit this component and its sub-components
	// with the specified visitor.
	virtual void Accept(AAFComponentVisitor& visitor);

private:
	OMFixedSizeProperty<aafLength_t>	_fadeInLength;
	OMFixedSizeProperty<aafFadeType_t>	_fadeInType;
	OMFixedSizeProperty<aafLength_t> 	_fadeOutLength;
	OMFixedSizeProperty<aafFadeType_t>	_fadeOutType;
	OMFixedSizeProperty<aafPosition_t>	_startTime;

};

#endif // ! __ImplAAFSourceClip_h__


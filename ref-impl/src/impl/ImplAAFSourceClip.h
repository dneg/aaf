//@doc
//@class    AAFSourceClip | Implementation class for AAFSourceClip
#ifndef __ImplAAFSourceClip_h__
#define __ImplAAFSourceClip_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


class ImplAAFDataDef;

class ImplAAFMob;

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif





class ImplAAFSourceClip : public ImplAAFSourceReference
{
public:

	// Declare class storable

	OMDECLARE_STORABLE(ImplAAFSourceClip)
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
		 aafUID_t * pDatadef  ,

		 // @parm [in] Length property value
		 aafLength_t *     pLength   ,

		 // @parm [in] Source Reference
         aafSourceRef_t   sourceRef);

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

private:
	OMFixedSizeProperty<aafLength_t>	_fadeInLength;
	OMFixedSizeProperty<aafFadeType_t>	_fadeInType;
	OMFixedSizeProperty<aafLength_t> 	_fadeOutLength;
	OMFixedSizeProperty<aafFadeType_t>	_fadeOutType;
	OMFixedSizeProperty<aafPosition_t>	_startTime;

};

#endif // ! __ImplAAFSourceClip_h__


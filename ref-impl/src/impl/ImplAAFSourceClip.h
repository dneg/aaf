//@doc
//@class    AAFSourceClip | Implementation class for AAFSourceClip
#ifndef __ImplAAFSourceClip_h__
#define __ImplAAFSourceClip_h__

#include "OMStorable.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplAAFDataDef;

class ImplAAFMob;


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#include "ImplAAFObject.h"
#include "OMProperty.h"


// Persistent Property IDs

const int PID_SOURCECLIP_FADEINLEN	= 4;
const int PID_SOURCECLIP_FADEINTYPE	= 5;
const int PID_SOURCECLIP_FADEINPRESENT	= 6;
const int PID_SOURCECLIP_FADEOUTLEN	= 7;
const int PID_SOURCECLIP_FADEOUTTYPE	= 8;
const int PID_SOURCECLIP_FADEOUTPRESENT	= 9;
const int PID_SOURCECLIP_STARTTIME		= 10;


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
  // InitializeSourceClip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InitializeSourceClip
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
		 aafInt32 *       fadeInLen     ,

		 // @parm [out] Fade In Type
		 aafFadeType_t *  fadeInType    ,

		 // @parm [out] Fade In Type
         aafBool *        fadeInPresent ,

		 // @parm [out] Fade Out Length
		 aafInt32 *       fadeOutLen    ,

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

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFSourceClipTest.cpp.
  static AAFRESULT test();

  // Return this objects stored object class.
  virtual AAFRESULT STDMETHODCALLTYPE
	GetObjectClass(aafUID_t * pClass);

private:
	OMFixedSizeProperty<aafInt32>		_fadeInLen;
	OMFixedSizeProperty<aafFadeType_t>	_fadeInType;
	OMFixedSizeProperty<aafBool>		_fadeInPresent;
	OMFixedSizeProperty<aafInt32> 		_fadeOutLen;
	OMFixedSizeProperty<aafFadeType_t>	_fadeOutType;
	OMFixedSizeProperty<aafBool>		_fadeOutPresent;
	OMFixedSizeProperty<aafPosition_t>	_startTime;

};

#endif // ! __ImplAAFSourceClip_h__


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

const int PID_SOURCECLIP_FADEINLEN		= 1;
const int PID_SOURCECLIP_FADEINTYPE		= 2;
const int PID_SOURCECLIP_FADEINPRESENT	= 3;
const int PID_SOURCECLIP_FADEOUTLEN		= 4;
const int PID_SOURCECLIP_FADEOUTTYPE	= 5;
const int PID_SOURCECLIP_FADEOUTPRESENT	= 6;


class ImplAAFSourceClip : public ImplAAFSourceReference
{
public:

	// Declare class storable

	OMDECLARE_STORABLE(AAFSourceClip);
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
		 ImplAAFDataDef * datadef  ,

		 // @parm [in] Length property value
		 aafLength_t      length   ,

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
  // GetRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRef
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
  // SetRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetRef
		// @parm [in] Source Reference
        (aafSourceRef_t  sourceRef);


  // Override from AAFSourceReference
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceID (/*[retval][out]*/ aafUID_t *  pSourceID);

  // Override from AAFSourceReference
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceID (/*[in]*/ aafUID_t   sourceID);

  // Override from AAFSourceReference
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceMobSlotID (/*[retval][out]*/ aafTrackID_t *  pMobSlotID);

  // Override from AAFSourceReference
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceMobSlotID (/*[in]*/ aafTrackID_t   mobSlotID);





public:

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFSourceClipTest.cpp.
  static AAFRESULT test();

private:
	OMFixedSizeProperty<aafInt32>		_fadeInLen;
	OMFixedSizeProperty<aafFadeType_t>	_fadeInType;
	OMFixedSizeProperty<aafBool>		_fadeInPresent;
	OMFixedSizeProperty<aafInt32> 		_fadeOutLen;
	OMFixedSizeProperty<aafFadeType_t>	_fadeOutType;
	OMFixedSizeProperty<aafBool>		_fadeOutPresent;

};

#endif // ! __ImplAAFSourceClip_h__


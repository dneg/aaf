//@doc
//@class    AAFCompositionMob | Implementation class for AAFCompositionMob
#ifndef __ImplAAFCompositionMob_h__
#define __ImplAAFCompositionMob_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif



class ImplAAFCompositionMob : public ImplAAFMob
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCompositionMob ();
  virtual ~ImplAAFCompositionMob ();



  //@access Public Members
  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
	    (// @parm [in] Mob name [optional]
         const aafCharacter *  name);
  //@rdesc Error code [see below].

  //****************
  // GetDefaultFade()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDefaultFade
		// @parm [out] a default fade struct
        (aafDefaultFade_t *  result);
  //@rdesc Error code [see below].
  //@comm If there is no default fade, this function returns with no error,
  // but the VALID field of the structure is false.  This allows you to
  // pass this struct to omfiSourceClipGetFade() in all cases.
  //@comm Maps to omfiMobGetDefaultFade
	

  //****************
  // SetDefaultFade()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDefaultFade
        (// @parm [in] Default fade length
		 aafLength_t    fadeLength  ,

		 // @parm [in] default fade type
		 aafFadeType_t  fadeType    ,

		 // @parm [in] default fade edit unit
         aafRational_t  fadeEditUnit);
  //@rdesc Error code [see below].
  //@comm Maps to omfiMobSetDefaultFade

  //***********************************************************
  // METHOD NAME: GetMobKind()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFMobPrivate | GetMobKind |
  // This method returns the kind of this mob.
  // 
  // Succeeds if all of the following are true:
  // - the pMobKind pointer is valid.
  // 
  // If this method fails nothing is written to *pMobKind.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pMobKind is null.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetMobKind (
    // @parm [out] aafMobKind_t * | pMobKind | The kind of this mob
    aafMobKind_t *  pMobKind
  );

  //***********************************************************
  // METHOD NAME: SetRendering()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFCompositionMob2 | SetRendering |
  // set the MobID of a rendering of this CompositionMob.

  // This method will return the following code:
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  SetRendering (
    // @parm [in, ref] aafMobID_constref | mobID | Rendering MobID
    aafMobID_constref  mobID
  );

  //***********************************************************
  // METHOD NAME: GetRendering()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFCompositionMob2 | GetRendering |
  // return the MobID of a rendering of this CompositionMob.

  // Succeeds if all of the following are true:
  // - the pMobID pointer is valid.
  //
  // If this method fails nothing will be written to *pMobID.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pMobID arg is NULL.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetRendering (
    // @parm [out] aafMobID_t * | pMobID | Rendering MobID
    aafMobID_t *  pMobID
  );


private:
	OMFixedSizeProperty<aafLength_t>	_defaultFadeLen;
	OMFixedSizeProperty<aafFadeType_t>	_defaultFadeType;
	OMFixedSizeProperty<aafRational_t>	_defaultFadeEditUnit;
	OMFixedSizeProperty<aafMobID_t>		_rendering;

};

#endif // ! __ImplAAFCompositionMob_h__

//@doc
//@class    AAFCompositionMob | Implementation class for AAFCompositionMob
#ifndef __ImplAAFCompositionMob_h__
#define __ImplAAFCompositionMob_h__

#include "OMStorable.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMStorable.h"

const int PID_COMPOSITIONMOB_DEFAULTFADELENGTH	= 6;
const int PID_COMPOSITIONMOB_DEFAULTFADETYPE	= 7;
const int PID_COMPOSITIONMOB_DEFAULTFADEEDITUNIT= 8;


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
  // SetInitialValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetInitialValues
	    (// @parm [in] Mob name [optional]
         aafWChar *  name,

		 // @parm [in] Whether or not the mob is a primary mob
		 aafBool  isPrimary);
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


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFCompositionMob)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFCompositionMobTest.cpp.
  static AAFRESULT test();

private:
	OMFixedSizeProperty<aafLength_t>	_defaultFadeLen;
	OMFixedSizeProperty<aafFadeType_t>	_defaultFadeType;
	OMFixedSizeProperty<aafRational_t>	_defaultFadeEditUnit;

};

#endif // ! __ImplAAFCompositionMob_h__


//@doc
//@class    AAFCompositionMob | Implementation class for AAFCompositionMob
#ifndef __ImplAAFCompositionMob_h__
#define __ImplAAFCompositionMob_h__


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
         aafWChar *  name);
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

private:
	OMFixedSizeProperty<aafLength_t>	_defaultFadeLen;
	OMFixedSizeProperty<aafFadeType_t>	_defaultFadeType;
	OMFixedSizeProperty<aafRational_t>	_defaultFadeEditUnit;
};

#endif // ! __ImplAAFCompositionMob_h__

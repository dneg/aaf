/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFSourceClip_h__
#include "ImplAAFSourceClip.h"
#endif

#include <assert.h>
#include "aafCvt.h" 
#include <AAFResult.h>


ImplAAFSourceClip::ImplAAFSourceClip ():
	_fadeInLen(		PID_SOURCECLIP_FADEINLEN,		"fadeInLen"),
	_fadeInType(	PID_SOURCECLIP_FADEINTYPE,		"fadeInType"),
	_fadeInPresent( PID_SOURCECLIP_FADEINPRESENT,	"fadeInPresent"),
	_fadeOutLen(	PID_SOURCECLIP_FADEOUTLEN,		"fadeOutLen"),
	_fadeOutType(	PID_SOURCECLIP_FADEOUTTYPE,		"fadeOutType"),
	_fadeOutPresent(PID_SOURCECLIP_FADEOUTPRESENT,	"fadeOutPresent")
{
	_persistentProperties.put(		_fadeInLen.address());
	_persistentProperties.put(		_fadeInType.address());
	_persistentProperties.put(		_fadeInPresent.address());
	_persistentProperties.put(		_fadeOutLen.address());
	_persistentProperties.put(		_fadeOutType.address());
	_persistentProperties.put(		_fadeOutPresent.address());

}


ImplAAFSourceClip::~ImplAAFSourceClip ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::InitializeSourceClip(aafUID_t*		pDatadef,
											aafLength_t*	pLength,
											aafSourceRef_t	sourceRef)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pDatadef == NULL ||
		pLength == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		SetDataDef( pDatadef );
		SetLength( pLength );
		SetSourceID( sourceRef.sourceID );
		SetSourceMobSlotID( sourceRef.sourceSlotID );

		_fadeInLen		= 0;
		_fadeInType		= kFadeNone;
		_fadeInPresent	= AAFFalse;
	
		_fadeOutLen		= 0;
		_fadeOutType	= kFadeNone;
		_fadeOutPresent = AAFFalse;
}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFSourceClip::GetFade (aafInt32		*fadeInLen,
                           aafFadeType_t	*fadeInType,
                           aafBool			*fadeInPresent,
                           aafInt32			*fadeOutLen,
                           aafFadeType_t	*fadeOutType,
                           aafBool			*fadeOutPresent)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (fadeInLen == NULL ||
		fadeInType == NULL ||
		fadeInPresent == NULL ||
		fadeOutLen == NULL ||
		fadeOutType == NULL ||
		fadeOutPresent == NULL )
	{
		aafError = 	AAFRESULT_NULL_PARAM;
	}
	else
	{
	
		*fadeInLen		= _fadeInLen;
		*fadeInType		= _fadeInType;
		*fadeInPresent	= _fadeInPresent;

		*fadeOutLen		= _fadeOutLen;
		*fadeOutType	= _fadeOutType;
		*fadeOutPresent	= _fadeOutPresent;
	}

	return aafError;
}


/*************************************************************************
 * Function: ResolveRef()
 *
 * 		Given a source clip object, this function returns a pointer to
 *      the mob that it references.  If this mob does not exist, the
 *      error AAFRESULT_MOB_NOT_FOUND is returned.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code .
 *
 * Possible Errors:
 *		Standard errors .
 *************************************************************************/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::ResolveRef (ImplAAFMob ** mob)
{
    aafSourceRef_t sourceRef;
	ImplAAFObject * head = NULL;
	ImplAAFMob * tmpMob = NULL;
	aafInt32 index = 0;
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	*mob = NULL;

	XPROTECT()
	  {
		if (!mob)
		  {
			RAISE(AAFRESULT_NULL_PARAM);
		  }

		/* NOTE: This function should really be doing more checking,
		 * i.e., does track exist in mob, does position (adjusted by
		 * editrate) exist in track, etc.
		 */

		CHECK(GetRef(&sourceRef));

#ifdef FULL_TOOLKIT
		CHECK(_file->LookupMob(sourceRef.sourceID, mob));
#endif
	  } /* XPROTECT */

	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;

	return(aafError);
}


/*************************************************************************
 * Function: GetRef()
 *
 * 		This function returns the 3 properties on a source Clip that
 *      make of the "source reference" (sourceID, sourceTrackID, and
 *      startTime).
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code.
 *
 * Possible Errors:
 *		Standard errors.
 *************************************************************************/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::GetRef (aafSourceRef_t*	pSourceRef)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	XPROTECT()
	  {
		if (pSourceRef)
		  {
#ifdef FULL_TOOLKIT
			  *pSourceRef = _srcRef;
#endif
		  } /* if sourceRef */
		else
		  {
			RAISE(AAFRESULT_NULL_PARAM);
		  }
	  } /* XPROTECT */

	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;

	return(aafError);
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFSourceClip::SetFade (aafInt32		fadeInLen,
                           aafFadeType_t	fadeInType,
                           aafInt32			fadeOutLen,
                           aafFadeType_t	fadeOutType)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (fadeInLen > 0)
	{
		_fadeInLen	= fadeInLen;
		_fadeInType	= fadeInType;
		_fadeInPresent = AAFTrue;
	}

	if (fadeOutLen > 0)
	{
		_fadeOutLen		= fadeOutLen;
		_fadeOutType	= fadeOutType;
		_fadeOutPresent = AAFTrue;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::SetRef (aafSourceRef_t  /*sourceRef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



  // Override from AAFSourceReference
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::GetSourceID (/*[retval][out]*/ aafUID_t *  /*pSourceID*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  // Override from AAFSourceReference
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::SetSourceID (/*[in]*/ aafUID_t   /*sourceID*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  // Override from AAFSourceReference
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::GetSourceMobSlotID (/*[retval][out]*/ aafSlotID_t *  /*pMobSlotID*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  // Override from AAFSourceReference
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::SetSourceMobSlotID (/*[in]*/ aafSlotID_t   /*mobSlotID*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }




extern "C" const aafClassID_t CLSID_AAFSourceClip;

OMDEFINE_STORABLE(ImplAAFSourceClip, CLSID_AAFSourceClip);


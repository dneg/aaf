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
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "ImplAAFHeader.h"

ImplAAFSourceClip::ImplAAFSourceClip ():
	_fadeInLen(		PID_SOURCECLIP_FADEINLEN,		"fadeInLen"),
	_fadeInType(	PID_SOURCECLIP_FADEINTYPE,		"fadeInType"),
	_fadeInPresent( PID_SOURCECLIP_FADEINPRESENT,	"fadeInPresent"),
	_fadeOutLen(	PID_SOURCECLIP_FADEOUTLEN,		"fadeOutLen"),
	_fadeOutType(	PID_SOURCECLIP_FADEOUTTYPE,		"fadeOutType"),
	_fadeOutPresent(PID_SOURCECLIP_FADEOUTPRESENT,	"fadeOutPresent"),
	_startTime(		PID_SOURCECLIP_STARTTIME,		"startTime")
{
	_persistentProperties.put(		_fadeInLen.address());
	_persistentProperties.put(		_fadeInType.address());
	_persistentProperties.put(		_fadeInPresent.address());
	_persistentProperties.put(		_fadeOutLen.address());
	_persistentProperties.put(		_fadeOutType.address());
	_persistentProperties.put(		_fadeOutPresent.address());
	_persistentProperties.put(		_startTime.address());

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
		_startTime = sourceRef.startTime;

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
	ImplAAFMob * tmpMob = NULL;
	ImplAAFHeader *head = NULL;
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
		CHECK(MyHeadObject(&head));
		CHECK(head->LookupMob(&sourceRef.sourceID, mob));
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
	aafUID_t	sourceID;
	aafSlotID_t slotID;

	XPROTECT()
	{	
		if (pSourceRef)
		{
			GetSourceID( &sourceID );
			GetSourceMobSlotID( &slotID );
			pSourceRef->sourceID = sourceID;
			pSourceRef->sourceSlotID = slotID;
			pSourceRef->startTime = _startTime;
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
    ImplAAFSourceClip::SetRef (aafSourceRef_t  sourceRef)
{
    aafInt32	tmp1xSourcePosition = 0;
	aafInt16	tmp1xTrackNum = 0;
	AAFRESULT   aafError = AAFRESULT_SUCCESS;
	

	/* If UID is NUL - make the rest of the fields 0 too. */
	if( (sourceRef.sourceID.Data1 == NilMOBID.Data1) && 
		(sourceRef.sourceID.Data2 == NilMOBID.Data2) &&
		(sourceRef.sourceID.Data3 == NilMOBID.Data3) &&
		(sourceRef.sourceID.Data4[0] == NilMOBID.Data4[0]) &&
		(sourceRef.sourceID.Data4[1] == NilMOBID.Data4[1]) &&
		(sourceRef.sourceID.Data4[2] == NilMOBID.Data4[2]) &&
		(sourceRef.sourceID.Data4[3] == NilMOBID.Data4[3]) &&
		(sourceRef.sourceID.Data4[4] == NilMOBID.Data4[4]) &&
		(sourceRef.sourceID.Data4[5] == NilMOBID.Data4[5]) &&
		(sourceRef.sourceID.Data4[6] == NilMOBID.Data4[6]) &&
		(sourceRef.sourceID.Data4[7] == NilMOBID.Data4[7]) 	)
	{
		sourceRef.sourceSlotID = 0;
		CvtInt32toPosition(0, sourceRef.startTime);	
	}
	SetSourceID( sourceRef.sourceID );
	SetSourceMobSlotID( sourceRef.sourceSlotID) ;
	_startTime = sourceRef.startTime;

	return(aafError);
}





extern "C" const aafClassID_t CLSID_AAFSourceClip;

OMDEFINE_STORABLE(ImplAAFSourceClip, CLSID_AAFSourceClip);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFSourceClip::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFSourceClip, sizeof aafClassID_t);
  return AAFRESULT_SUCCESS;
}


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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "aafCvt.h" 
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "ImplAAFHeader.h"

ImplAAFSourceClip::ImplAAFSourceClip ():
	_fadeInLength(		PID_SourceClip_FadeInLength,		"fadeInLength"),
	_fadeInType(	PID_SourceClip_FadeInType,		"fadeInType"),
	_fadeOutLength(	PID_SourceClip_FadeOutLength,		"fadeOutLength"),
	_fadeOutType(	PID_SourceClip_FadeOutType,		"fadeOutType"),
	_startTime(		PID_SourceClip_StartTime,		"startTime")
{
	_persistentProperties.put(		_fadeInLength.address());
	_persistentProperties.put(		_fadeInType.address());
	_persistentProperties.put(		_fadeOutLength.address());
	_persistentProperties.put(		_fadeOutType.address());
	_persistentProperties.put(		_startTime.address());

}


ImplAAFSourceClip::~ImplAAFSourceClip ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::Initialize(aafUID_t*		pDatadef,
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

		_fadeInLength		= 0;
		_fadeInType		= kFadeNone;
	
		_fadeOutLength		= 0;
		_fadeOutType	= kFadeNone;
}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFSourceClip::GetFade (aafLength_t	*fadeInLen,
                           aafFadeType_t	*fadeInType,
                           aafBool			*fadeInPresent,
                           aafLength_t		*fadeOutLen,
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
	
        *fadeInLen      = _fadeInLength;
		*fadeInType		= _fadeInType;
		if (_fadeInLength > 0)
		{
			*fadeInPresent	= AAFTrue;
		}
		else
		{
			*fadeInPresent = AAFFalse;
		}

		*fadeOutLen		= _fadeOutLength;
		*fadeOutType	= _fadeOutType;
		if (_fadeOutLength > 0)
		{
			*fadeOutPresent	= AAFTrue;
		}
		else
		{
			*fadeOutPresent = AAFFalse;
		}
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

		CHECK(GetSourceReference(&sourceRef));
		CHECK(MyHeadObject(&head));
		CHECK(head->LookupMob(&sourceRef.sourceID, mob));

		head->ReleaseReference();
		head = NULL;
	  } /* XPROTECT */

	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;

	return(aafError);
}


/*************************************************************************
 * Function: GetSourceReference()
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
    ImplAAFSourceClip::GetSourceReference (aafSourceRef_t*	pSourceRef)
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
		_fadeInLength	= fadeInLen;
		_fadeInType	= fadeInType;
	}

	if (fadeOutLen > 0)
	{
		_fadeOutLength		= fadeOutLen;
		_fadeOutType	= fadeOutType;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::SetSourceReference (aafSourceRef_t  sourceRef)
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



AAFRESULT ImplAAFSourceClip::TraverseToClip(aafLength_t length,
											ImplAAFSegment **sclp,
											ImplAAFPulldown **pulldownObj,
											aafInt32 *pulldownPhase,
											aafLength_t *sclpLen,
											aafBool *isMask)
{
	XPROTECT()
	{
		*sclp = this;
		CHECK((*sclp)->GetLength(sclpLen));
		if (Int64Less(length, *sclpLen))
		{
			*sclpLen = length;
		}
	} /* XPROTECT */
	
	XEXCEPT
	{
	}
	XEND;
	
	return(OM_ERR_NONE);
}


OMDEFINE_STORABLE(ImplAAFSourceClip, AUID_AAFSourceClip);


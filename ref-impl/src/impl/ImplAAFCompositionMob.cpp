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



#ifndef __ImplAAFCompositionMob_h__
#include "ImplAAFCompositionMob.h"
#endif

#include <assert.h>
#include "AAFResult.h"

ImplAAFCompositionMob::ImplAAFCompositionMob ():
	_defaultFadeLen(	PID_COMPOSITIONMOB_DEFAULTFADELENGTH,		"defaultFadeLength"),
	_defaultFadeType(	PID_COMPOSITIONMOB_DEFAULTFADETYPE,			"defaultFadeType"),
	_defaultFadeEditUnit(PID_COMPOSITIONMOB_DEFAULTFADEEDITUNIT,	"defaultFadeEditUnit")
{
	_persistentProperties.put(		_defaultFadeLen.address());
	_persistentProperties.put(		_defaultFadeType.address());
	_persistentProperties.put(		_defaultFadeEditUnit.address());
	
}




ImplAAFCompositionMob::~ImplAAFCompositionMob ()
{}


  //@access Public Members
AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::SetInitialValues (aafWChar	*pName,
											 aafBool	isPrimary
											 )
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		aafError = SetName( pName );
		if (aafError == AAFRESULT_SUCCESS)
			aafError = SetPrimary( isPrimary );
	}

	return aafError;
}

  //@rdesc Error code [see below].

  //@rdesc Error code [see below].
  //@comm If there is no default fade, this function returns with no error,
  // but the VALID field of the structure is false.  This allows you to
  // pass this struct to SourceClip::GetFade() in all cases.
  //@comm Maps to AAFGetDefaultFade

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::GetDefaultFade (aafDefaultFade_t *pResult)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pResult == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		if (_defaultFadeType == kFadeNone)
			pResult->valid = AAFFalse;
		else
		{
			pResult->fadeLength = _defaultFadeLen;
			pResult->fadeType = _defaultFadeType;
			pResult->fadeEditUnit = _defaultFadeEditUnit;
			pResult->valid = AAFTrue;
		}
	}
	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::SetDefaultFade (
											aafLength_t		fadeLength,
											aafFadeType_t	fadeType,
											aafRational_t	fadeEditUnit
											)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	_defaultFadeLen = fadeLength;
	_defaultFadeType = fadeType;
	_defaultFadeEditUnit = fadeEditUnit;

	return aafError;
}

  //@rdesc Error code [see below].
  //@comm Maps to omfiMobSetDefaultFade

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::GetMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kCompMob;

	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_AAFCompositionMob;

OMDEFINE_STORABLE(ImplAAFCompositionMob, CLSID_AAFCompositionMob);


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

#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"

ImplAAFCompositionMob::ImplAAFCompositionMob ():
	_defaultFadeLen(	PID_CompositionMob_DefaultFadeLength,		"Default Fade Length"),
	_defaultFadeType(	PID_CompositionMob_DefFadeType,			"Default Fade Type"),
	_defaultFadeEditUnit(PID_CompositionMob_DefFadeEditUnit,	"Default Fade Edit Unit")
{
	_persistentProperties.put(		_defaultFadeLen.address());
	_persistentProperties.put(		_defaultFadeType.address());
	_persistentProperties.put(		_defaultFadeEditUnit.address());
	
}

ImplAAFCompositionMob::~ImplAAFCompositionMob ()
{}


  //@access Public Members
AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::Initialize (aafWChar	*pName)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		aafError = SetName( pName );
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

	if (fadeLength >= 0) 
	{
		if (fadeType >= kFadeNone ||
			fadeType <= kFadeLinearPower)
		{
			_defaultFadeLen = fadeLength;
			_defaultFadeType = fadeType;
			_defaultFadeEditUnit = fadeEditUnit;
		}
		else
			aafError = AAFRESULT_BAD_TYPE;
	}
	else
		aafError = AAFRESULT_BAD_LENGTH;

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

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFCompositionMob::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFCompositionMob, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}


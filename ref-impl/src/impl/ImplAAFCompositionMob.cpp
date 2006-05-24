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
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __ImplAAFCompositionMob_h__
#include "ImplAAFCompositionMob.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "AAFResult.h"

ImplAAFCompositionMob::ImplAAFCompositionMob ():
	_defaultFadeLen(	PID_CompositionMob_DefaultFadeLength,		L"DefaultFadeLength"),
	_defaultFadeType(	PID_CompositionMob_DefFadeType,			L"DefFadeType"),
	_defaultFadeEditUnit(PID_CompositionMob_DefFadeEditUnit,	L"DefFadeEditUnit"),
	_rendering(PID_CompositionMob_Rendering,	L"Rendering")
{
	_persistentProperties.put(		_defaultFadeLen.address());
	_persistentProperties.put(		_defaultFadeType.address());
	_persistentProperties.put(		_defaultFadeEditUnit.address());	
	_persistentProperties.put(		_rendering.address());	
}

ImplAAFCompositionMob::~ImplAAFCompositionMob ()
{}


  //@access Public Members
AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::Initialize (const aafCharacter * pName)
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
		if (!_defaultFadeLen.isPresent() ||
			!_defaultFadeType.isPresent() ||
			!_defaultFadeEditUnit.isPresent())
			aafError = AAFRESULT_PROP_NOT_PRESENT;
		else
		{
			if (_defaultFadeType == kAAFFadeNone)
				pResult->valid = kAAFFalse;
			else
			{
				pResult->fadeLength = _defaultFadeLen;
				pResult->fadeType = _defaultFadeType;
				pResult->fadeEditUnit = _defaultFadeEditUnit;
				pResult->valid = kAAFTrue;
			}
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
		if (fadeType >= kAAFFadeNone ||
			fadeType <= kAAFFadeLinearPower)
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
	*pMobKind = kAAFCompMob;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::SetRendering (aafMobID_constref mobID)
{
  _rendering = mobID;

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFCompositionMob::GetRendering (aafMobID_t *  pMobID)
{
  if(pMobID == NULL)
    return AAFRESULT_NULL_PARAM;

  *pMobID = _rendering;

  return(AAFRESULT_SUCCESS); 
}



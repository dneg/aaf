/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

/******************************************\
*                                          *
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

#ifndef __ImplAAFCompositionMob_h__
#include "ImplAAFCompositionMob.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"

ImplAAFCompositionMob::ImplAAFCompositionMob ():
	_defaultFadeLen(	PID_CompositionMob_DefaultFadeLength,		"DefaultFadeLength"),
	_defaultFadeType(	PID_CompositionMob_DefFadeType,			"DefFadeType"),
	_defaultFadeEditUnit(PID_CompositionMob_DefFadeEditUnit,	"DefFadeEditUnit")
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
		if (!_defaultFadeLen.isPresent() ||
			!_defaultFadeType.isPresent() ||
			!_defaultFadeEditUnit.isPresent())
			aafError = AAFRESULT_PROP_NOT_PRESENT;
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


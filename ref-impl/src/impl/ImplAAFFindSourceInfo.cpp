/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFFindSourceInfo_h__
#include "ImplAAFFindSourceInfo.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafCvt.h"
#include "ImplAAFMob.h"
#include "ImplAAFEffect.h"
#include "aafdefuids.h"

ImplAAFFindSourceInfo::ImplAAFFindSourceInfo ()
{
	_mob = NULL;
	_cpnt = NULL;
	_editRate.numerator = 0;
	_editRate.denominator = 1;
	CvtInt32toLength(0, _position);
	CvtInt32toLength(0, _length);
	_effect = NULL;
}


ImplAAFFindSourceInfo::~ImplAAFFindSourceInfo ()
{
	if (_mob)
	{
		_mob->ReleaseReference();
		_mob = NULL;
	}
	if (_cpnt)
	{
		_cpnt->ReleaseReference();
		_cpnt = NULL;
	}
	if (_effect)
	{
		_effect->ReleaseReference();
		_effect = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::Init(ImplAAFMob *mob, aafSlotID_t slotID, aafPosition_t position,
							aafRational_t editRate, aafLength_t length,
							ImplAAFComponent *cpnt)
{
	if (_mob)
		_mob->ReleaseReference();
	_mob = mob;
	if (mob)
		mob->AcquireReference();
	_slotID = slotID;
	_position = position;
	_editRate = editRate;
	_length = length;
	if (_cpnt)
		_cpnt->ReleaseReference();
	_cpnt = cpnt;
	if (cpnt)
		cpnt->AcquireReference();
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::AddPulldown(ImplAAFPulldown *pdwn)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::Duplicate(ImplAAFFindSourceInfo *result)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE ImplAAFFindSourceInfo::Clear(void)
{
	if (_mob)
		_mob->ReleaseReference();
	_mob = NULL;
	if (_cpnt)
		_cpnt->ReleaseReference();
	_cpnt = NULL;
	_editRate.numerator = 0;
	_editRate.denominator = 1;
//!!!	  (*sourceInfo).filmTapePdwn = NULL;
//!!!	  (*sourceInfo).tapeFilmPdwn = NULL;
//!!!	  (*sourceInfo).effeObject = NULL;
	CvtInt32toLength(0, _position);
	CvtInt32toLength(0, _length);
	if (_effect)
		_effect->ReleaseReference();
	_effect = NULL;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::SetEffect(
				ImplAAFEffect *effect)
{
	if (_effect)
		_effect->ReleaseReference();
	_effect = effect;
	if (effect)
	  effect->AcquireReference();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::SetComponent(
				ImplAAFComponent *cpnt)
{
	if (_cpnt)
		_cpnt->ReleaseReference();
	_cpnt = cpnt;
	if (cpnt)
	  cpnt->AcquireReference();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
		ImplAAFFindSourceInfo::GetReference(aafSourceRef_t *pSourceRef)
{
	if (pSourceRef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		pSourceRef->sourceSlotID = _slotID;
		pSourceRef->startTime = _position;
		if(_mob != NULL)
		{
			CHECK(_mob->GetMobID(&pSourceRef->sourceID));
		}
		else
		{
			pSourceRef->sourceID = NilMOBID;
		}
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::GetMob(ImplAAFMob **ppMob)
{
	*ppMob = _mob;
	if (*ppMob)
	  (*ppMob)->AcquireReference();
	else
	  return AAFRESULT_NULLOBJECT;
	return AAFRESULT_SUCCESS;
}

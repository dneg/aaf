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

 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/






#ifndef __ImplAAFFindSourceInfo_h__
#include "ImplAAFFindSourceInfo.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafCvt.h"
#include "ImplAAFMob.h"
#include "aafdefuids.h"

ImplAAFFindSourceInfo::ImplAAFFindSourceInfo ()
{}


ImplAAFFindSourceInfo::~ImplAAFFindSourceInfo ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::Init(ImplAAFMob *mob, aafSlotID_t slotID, aafPosition_t position,
							aafRational_t editRate, aafLength_t length,
							ImplAAFComponent *cpnt)
{
	_mob = mob;
	_slotID = slotID;
	_position = position;
	_editRate = editRate;
	_length = length;
	_cpnt = cpnt;
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
	_mob = NULL;
	_cpnt = NULL;
	_editRate.numerator = 0;
	_editRate.denominator = 1;
//!!!	  (*sourceInfo).filmTapePdwn = NULL;
//!!!	  (*sourceInfo).tapeFilmPdwn = NULL;
//!!!	  (*sourceInfo).effeObject = NULL;
	CvtInt32toLength(0, _position);
	CvtInt32toLength(0, _length);
	_effect = NULL;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::SetEffect(
										ImplAAFGroup *effect)
{
	_effect = effect;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::SetComponent(
										ImplAAFComponent *cpnt)
{
	_cpnt = cpnt;
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
	return AAFRESULT_SUCCESS;
}

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


#ifndef __ImplAAFFindSourceInfo_h__
#include "ImplAAFFindSourceInfo.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafCvt.h"
#include "ImplAAFMob.h"
#include "ImplAAFOperationGroup.h"
#include "AAFDefUIDs.h"

ImplAAFFindSourceInfo::ImplAAFFindSourceInfo ()
{
	_mob = NULL;
	_cpnt = NULL;
	_editRate.numerator = 0;
	_editRate.denominator = 1;
	CvtInt32toLength(0, _position);
	CvtInt32toLength(0, _length);
	_operationGroup = NULL;
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
	if (_operationGroup)
	{
		_operationGroup->ReleaseReference();
		_operationGroup = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::Init(ImplAAFMob *mob, aafSlotID_t slotID, aafPosition_t position,
							aafRational_t editRate, aafLength_t length,
							ImplAAFComponent *cpnt)
{
	if (_mob)
	  {
		_mob->ReleaseReference();
		_mob = 0;
	  }
	_mob = mob;
	if (mob)
		mob->AcquireReference();
	_slotID = slotID;
	_position = position;
	_editRate = editRate;
	_length = length;
	if (_cpnt)
	  {
		_cpnt->ReleaseReference();
		_cpnt = 0;
	  }
	_cpnt = cpnt;
	if (cpnt)
		cpnt->AcquireReference();
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::AddPulldown(ImplAAFPulldown * /*pdwn*/)
{
	return AAFRESULT_NOT_IN_CURRENT_VERSION;
}

//AAFRESULT STDMETHODCALLTYPE
//ImplAAFFindSourceInfo::Duplicate(ImplAAFFindSourceInfo * /*result*/)
//{
//	return AAFRESULT_NOT_IN_CURRENT_VERSION;
//}


AAFRESULT STDMETHODCALLTYPE ImplAAFFindSourceInfo::Clear(void)
{
	if (_mob)
	  {
		_mob->ReleaseReference();
		_mob = 0;
	  }
	_mob = NULL;
	if (_cpnt)
	  {
		_cpnt->ReleaseReference();
		_cpnt = 0;
	  }
	_cpnt = NULL;
	_editRate.numerator = 0;
	_editRate.denominator = 1;
//!!!	  (*sourceInfo).filmTapePdwn = NULL;
//!!!	  (*sourceInfo).tapeFilmPdwn = NULL;
//!!!	  (*sourceInfo).effeObject = NULL;
	CvtInt32toLength(0, _position);
	CvtInt32toLength(0, _length);
	if (_operationGroup)
		_operationGroup->ReleaseReference();
	_operationGroup = NULL;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::SetOperationGroup(
				ImplAAFOperationGroup *group)
{
	if (_operationGroup)
	  {
		_operationGroup->ReleaseReference();
		_operationGroup = 0;
	  }
	_operationGroup = group;
	if (group)
	  group->AcquireReference();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::SetComponent(
				ImplAAFComponent *cpnt)
{
	if (_cpnt)
	  {
		_cpnt->ReleaseReference();
		_cpnt = 0;
	  }
	_cpnt = cpnt;
	if (cpnt)
	  cpnt->AcquireReference();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
		ImplAAFFindSourceInfo::GetSourceReference(aafSourceRef_t *pSourceRef)
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
			memset(&pSourceRef->sourceID, 0, sizeof(pSourceRef->sourceID));
		}
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::GetMob(ImplAAFMob **ppMob)
{
	if(ppMob == NULL)
		return AAFRESULT_NULL_PARAM;
	*ppMob = _mob;
	if (*ppMob)
	  (*ppMob)->AcquireReference();
	else
	  return AAFRESULT_NULLOBJECT;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::GetEditRate(aafRational_t *pEditRate)
{
	if(pEditRate == NULL)
		return AAFRESULT_NULL_PARAM;
	*pEditRate = _editRate;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFFindSourceInfo::GetLength(aafLength_t *pLength)
{
	if(pLength == NULL)
		return AAFRESULT_NULL_PARAM;
	*pLength = _length;
	return AAFRESULT_SUCCESS;
}

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
 * prior written permission of Avid Technology, Inc.
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



#ifndef __ImplAAFOperationGroup_h__
#include "ImplAAFOperationGroup.h"
#endif

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#include "ImplAAFDictionary.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"
#include "AAFUtils.h"
#include "AAFDefUIDs.h"


#ifndef __ImplAAFTransition_h__
#include "ImplAAFTransition.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTransition::ImplAAFTransition ():
_operationGroup( PID_Transition_OperationGroup, L"OperationGroup"),
_cutPoint( PID_Transition_CutPoint, L"CutPoint")
{
	_persistentProperties.put(_operationGroup.address());
	_persistentProperties.put(_cutPoint.address());
}


ImplAAFTransition::~ImplAAFTransition ()
{
	ImplAAFOperationGroup *group = _operationGroup.clearValue();
	if (group)
	{
	  group->ReleaseReference();
	  group = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFTransition::Initialize (ImplAAFDataDef * pDataDef,
							   aafLength_t				length,
							   aafPosition_t			cutPoint,
							   ImplAAFOperationGroup*	pOperationGroup)
{
	HRESULT		rc = AAFRESULT_SUCCESS;

	if (pOperationGroup == NULL)
		return AAFRESULT_NULL_PARAM;

	if (pDataDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(SetNewProps(length, pDataDef));
		_cutPoint = cutPoint;
		if (_operationGroup)
		  _operationGroup->ReleaseReference();
		_operationGroup = 0;
		_operationGroup = pOperationGroup;
		if (pOperationGroup)
			pOperationGroup->AcquireReference();

	}
	XEXCEPT
	XEND;
	
	return rc;
}
	 

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTransition::GetCutPoint (aafPosition_t*  pCutPoint)
{
	HRESULT		rc = AAFRESULT_SUCCESS;

	if (pCutPoint == NULL )
		return AAFRESULT_NULL_PARAM;

	*pCutPoint = _cutPoint;
	return rc;
}

	//@comm Replaces part of omfsTransitionGetInfo

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTransition::GetOperationGroup (ImplAAFOperationGroup ** ppEffObj)
{
	if (ppEffObj == NULL)
		return AAFRESULT_NULL_PARAM;

	*ppEffObj = _operationGroup;
	if (*ppEffObj)
		(*ppEffObj)->AcquireReference();

	return AAFRESULT_SUCCESS; 
}

	//@comm Replaces part of omfsTransitionGetInfo

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTransition::SetCutPoint (aafPosition_t  cutPoint)
{
	_cutPoint = cutPoint;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTransition::SetOperationGroup (ImplAAFOperationGroup * pEffObj)
{

	if (pEffObj == NULL)
		return AAFRESULT_NULL_PARAM;
	
	if (_operationGroup)
	  _operationGroup->ReleaseReference();
	_operationGroup = 0;

	_operationGroup = pEffObj;
	_operationGroup->AcquireReference();
	return AAFRESULT_SUCCESS; 
}





AAFRESULT ImplAAFTransition::ChangeContainedReferences(aafMobID_constref from,
													aafMobID_constref to)
{
	ImplAAFSegment	*seg;
	
	seg = _operationGroup;

	if(seg != NULL)
		seg->ChangeContainedReferences(from, to);

	return AAFRESULT_SUCCESS;
}


//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



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
		{
		  if( pOperationGroup == _operationGroup )
			RAISE( AAFRESULT_SUCCESS );

		  _operationGroup->ReleaseReference();
		  _operationGroup = 0;
		}

		if (pOperationGroup->attached())
			RAISE(  AAFRESULT_OBJECT_ALREADY_ATTACHED );

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
	{
	  if( pEffObj == _operationGroup )
		return AAFRESULT_SUCCESS;

	  _operationGroup->ReleaseReference();
	  _operationGroup = 0;
	}

	if( pEffObj->attached() )
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

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


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



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


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFTransition_h__
#include "ImplAAFTransition.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTransition::ImplAAFTransition ():
_operationGroup( PID_Transition_OperationGroup, "OperationGroup"),
_cutPoint( PID_Transition_CutPoint, "CutPoint")
{
	_persistentProperties.put(_operationGroup.address());
	_persistentProperties.put(_cutPoint.address());
}


ImplAAFTransition::~ImplAAFTransition ()
{
	ImplAAFOperationGroup *group = _operationGroup.setValue(0);
	if (group)
	{
	  group->ReleaseReference();
	  group = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFTransition::Create (aafUID_t*				pDatadef,
							   aafLength_t				length,
							   aafPosition_t			cutPoint,
							   ImplAAFOperationGroup*	pOperationGroup)
{
	HRESULT		rc = AAFRESULT_SUCCESS;

	if (pDatadef == NULL || pOperationGroup == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(SetNewProps(length, pDatadef));
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



OMDEFINE_STORABLE(ImplAAFTransition, AUID_AAFTransition);



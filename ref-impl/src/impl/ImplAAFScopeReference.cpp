/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



/*************************************************************************
 * 
 * @class AAFScopeReference | AAFScopeReference refers to a section in the 
 *			specified AAFNestedScope slo tor AAFMobSLot.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/



#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFScopeReference_h__
#include "ImplAAFScopeReference.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFResult.h"


ImplAAFScopeReference::ImplAAFScopeReference ():
	_relativeScope(			PID_ScopeReference_RelativeScope,		"RelativeScope"),
	_relativeSlot(			PID_ScopeReference_RelativeSlot,		"RelativeSlot")
{
	_persistentProperties.put(		_relativeScope.address());
	_persistentProperties.put(		_relativeSlot.address());
}


ImplAAFScopeReference::~ImplAAFScopeReference ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::Create (aafUInt32 RelativeScope, aafUInt32 RelativeSlot)
{

	_relativeScope = RelativeScope;
	_relativeSlot = RelativeSlot;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::GetRelativeScope (aafUInt32* pnRelativeScope)
{
	if (pnRelativeScope == NULL)
		return AAFRESULT_NULL_PARAM;

	*pnRelativeScope = _relativeScope;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::GetRelativeSlot (aafUInt32* pnRelativeSlot)
{
	if (pnRelativeSlot == NULL)
		return AAFRESULT_NULL_PARAM;

	*pnRelativeSlot = _relativeSlot;

	return AAFRESULT_SUCCESS;
}






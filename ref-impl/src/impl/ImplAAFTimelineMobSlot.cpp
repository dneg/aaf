

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


////////////////////////////////////////////////////////////////////////////////
// Types required by this module:
//
// aafRational_t,
// aafPosition_t,



#ifndef __ImplAAFTimelineMobSlot_h__
#include "ImplAAFTimelineMobSlot.h"
#endif

#include <assert.h>
#include <AAFResult.h>
#include "aafCvt.h" 


ImplAAFTimelineMobSlot::ImplAAFTimelineMobSlot ():
	_editRate(	PID_TIMELINEMOBSLOT_EDITRATE,	"EditRate"),
	_origin(	PID_TIMELINEMOBSLOT_ORIGIN,		"Origin")

{
	_persistentProperties.put( _editRate.address());
	_persistentProperties.put( _origin.address());

}


ImplAAFTimelineMobSlot::~ImplAAFTimelineMobSlot ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetEditRate (aafRational_t *editRate)
{
	if (editRate == NULL)
		return AAFRESULT_NULL_PARAM;

	*editRate = _editRate;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::SetEditRate (aafRational_t *editRate)
{
	if (editRate == NULL)
		return AAFRESULT_NULL_PARAM;

	_editRate = *editRate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::GetOrigin (aafPosition_t *origin)
{
	if (origin == NULL)
		return AAFRESULT_NULL_PARAM;

	*origin = _origin;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTimelineMobSlot::SetOrigin (aafPosition_t origin)
{
	_origin = origin;

	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_AAFTimelineMobSlot;

OMDEFINE_STORABLE(ImplAAFTimelineMobSlot, CLSID_AAFTimelineMobSlot);


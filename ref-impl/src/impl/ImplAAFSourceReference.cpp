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




#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#include <assert.h>
#include "AAFResult.h"


ImplAAFSourceReference::ImplAAFSourceReference ():
	_sourceID(			PID_SOURCEREFERENCE_SOURCEID,		"sourceId"),
	_sourceMobSlotId(	PID_SOURCEREFERENCE_SOURCEMOBSLOTID,	"sourceMobSlotId")
{
	_persistentProperties.put(		_sourceID.address());
	_persistentProperties.put(		_sourceMobSlotId.address());
}


ImplAAFSourceReference::~ImplAAFSourceReference ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::GetSourceID (aafUID_t*	pSourceID)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pSourceID == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pSourceID = _sourceID;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::SetSourceID (aafUID_t	sourceID)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	_sourceID = sourceID;

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::GetSourceMobSlotID (aafSlotID_t*	pMobSlotID)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pMobSlotID == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pMobSlotID = _sourceMobSlotId;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::SetSourceMobSlotID (aafSlotID_t		mobSlotID)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	_sourceMobSlotId = mobSlotID;

	return aafError;
}



extern "C" const aafClassID_t CLSID_AAFSourceReference;

OMDEFINE_STORABLE(ImplAAFSourceReference, CLSID_AAFSourceReference);


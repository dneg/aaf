/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"
#include "aafUtils.h"

ImplAAFSourceReference::ImplAAFSourceReference ():
	_sourceID(			PID_SourceReference_SourceID,		"SourceID"),
	_sourceMobSlotId(	PID_SourceReference_SourceMobSlotID,	"SourceMobSlotID")
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
	else if (!_sourceID.isPresent())
	{
		aafError = AAFRESULT_PROP_NOT_PRESENT;
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


AAFRESULT ImplAAFSourceReference::ChangeContainedReferences(aafUID_t *from, aafUID_t *to)
{
	aafUID_t			myID;

	if(EqualAUID(&myID, from))
		SetSourceID(*to);

	return AAFRESULT_SUCCESS;
}


OMDEFINE_STORABLE(ImplAAFSourceReference, AUID_AAFSourceReference);


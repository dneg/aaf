/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include <assert.h>
#include "AAFResult.h"

ImplAAFFileDescriptor::ImplAAFFileDescriptor ()
: _sampleRate(			PID_FILE_DESC_SAMPLERATE,	"sampleRate"),
 _length(				PID_FILE_DESC_LENGTH,		"length"),
 _isInContainer(        PID_FILE_DESC_INCONTAINER,	"isInContainer"),
 _containerFmt(         PID_FILE_DESC_CONTAINERFMT,	"containerFormat"),
 _initialized(AAFFalse)
{
  _persistentProperties.put(_sampleRate.address());
  _persistentProperties.put(_length.address());
  _persistentProperties.put(_isInContainer.address());
  _persistentProperties.put(_containerFmt.address());
}


ImplAAFFileDescriptor::~ImplAAFFileDescriptor ()
{}


//@access Public Members

AAFRESULT STDMETHODCALLTYPE
	ImplAAFFileDescriptor::Initialize ()
{
  if (_initialized)
	{
	  return AAFRESULT_ALREADY_INITIALIZED;
	}
  _initialized = AAFTrue;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetLength (aafLength_t length)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	_length = length;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetLength (aafLength_t *pLength)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if(pLength == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pLength = _length;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetIsInContainer (aafBool isAAF)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	_isInContainer = isAAF;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetIsInContainer (aafBool* pIsAAF)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if(pIsAAF == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pIsAAF = _isInContainer;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetSampleRate (aafRational_t *pRate)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if(pRate == NULL)
		return(AAFRESULT_NULL_PARAM);
	_sampleRate = *pRate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetSampleRate (aafRational_t *pRate)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if(pRate == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pRate = _sampleRate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetContainerFormat (aafUID_t *pFormat)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if(pFormat == NULL)
		return(AAFRESULT_NULL_PARAM);
	_containerFmt = *pFormat;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetContainerFormat (aafUID_t *pFormat)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if(pFormat == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pFormat = _containerFmt;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetOwningMobKind (aafMobKind_t *pMobKind)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if(pMobKind  == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kFileMob;
	return(AAFRESULT_SUCCESS);
}


extern "C" const aafClassID_t CLSID_AAFFileDescriptor;

OMDEFINE_STORABLE(ImplAAFFileDescriptor, CLSID_AAFFileDescriptor);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFFileDescriptor::GetObjectClass(aafUID_t * pClass)
{
	if (! _initialized)
	{
		return AAFRESULT_NOT_INITIALIZED;
	}

	if (! pClass)
	{
		return AAFRESULT_NULL_PARAM;
	}
	memcpy (pClass, &CLSID_AAFFileDescriptor, sizeof (aafClassID_t));
	return AAFRESULT_SUCCESS;
}


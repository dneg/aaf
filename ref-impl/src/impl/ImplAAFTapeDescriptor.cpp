/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFTapeDescriptor_h__
#include "ImplAAFTapeDescriptor.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"


ImplAAFTapeDescriptor::ImplAAFTapeDescriptor ():
	_formFactor(		PID_TapeDescriptor_FormFactor,		"FormFactor"),
	_videoSignalType(	PID_TapeDescriptor_VideoSignal,		"VideoSignal"),
	_tapeFormat(		PID_TapeDescriptor_TapeFormat,		"TapeFormat"),
	_tapeLength(		PID_TapeDescriptor_Length,			"Length"),
	_manufacturer(		PID_TapeDescriptor_ManufacturerID,	"ManufacturerID"),
	_model(				PID_TapeDescriptor_Model,			"Model")	
{
	_persistentProperties.put(_formFactor.address());
	_persistentProperties.put(_videoSignalType.address());
	_persistentProperties.put(_tapeFormat.address());
	_persistentProperties.put(_tapeLength.address());
	_persistentProperties.put(_manufacturer.address());
	_persistentProperties.put(_model.address());
}


ImplAAFTapeDescriptor::~ImplAAFTapeDescriptor ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTapeDescriptor::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeManufacturer (aafWChar*  pName)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		_manufacturer = pName;
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeManufacturer (aafWChar*	pName,
												aafInt32	buflen)
{
    AAFRESULT	aafError = AAFRESULT_SUCCESS;
	bool		status;

	if (pName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if(!_manufacturer.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		status = _manufacturer.copyToBuffer(pName, buflen);
		if (!status)
			aafError = AAFRESULT_SMALLBUF;
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeManBufLen (aafInt32 *pLen)
{
    AAFRESULT	aafError = AAFRESULT_SUCCESS;

	if (pLen == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if(!_manufacturer.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pLen = _manufacturer.size();
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeModel (aafWChar*	pModelName)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pModelName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		_model = pModelName;
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeModel (aafWChar*	pModelName,
										 aafInt32	buflen)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	bool		status;

	if (pModelName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if(!_model.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		status = _model.copyToBuffer(pModelName, buflen);
		if (!status)
			aafError = AAFRESULT_SMALLBUF;
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeModelBufLen (aafInt32*	pLen)
{
    AAFRESULT	aafError = AAFRESULT_SUCCESS;

	if (pLen == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if(!_model.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pLen = _model.size();
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeFormFactor (aafTapeCaseType_t	formFactor)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	
	if ( formFactor < kTapeCaseNull ||
		 formFactor > kNagraAudioTape )
	{
		aafError = AAFRESULT_BAD_TYPE;
	}
	else
	{
		_formFactor = formFactor;
	}
	
	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeFormFactor (aafTapeCaseType_t*	pFormFactor)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	
	if (pFormFactor == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	
	else if(!_formFactor.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	

	else
	{
		*pFormFactor = _formFactor;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetSignalType (aafVideoSignalType_t	videoSignal)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (videoSignal < kVideoSignalNull ||
		videoSignal > kSECAMSignal)
	{
		aafError = AAFRESULT_BAD_TYPE;
	}
	else
	{
		_videoSignalType = videoSignal;
	}
	
	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetSignalType (aafVideoSignalType_t*	pVideoSignal)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (pVideoSignal == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	
	else if(!_videoSignalType.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	 
	else
	{
		*pVideoSignal = _videoSignalType;
	}

	return aafError;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeFormat (aafTapeFormatType_t	tapeFormat)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (tapeFormat < kTapeFormatNull ||
		tapeFormat > kHi8Format)
	{
		aafError = AAFRESULT_BAD_TYPE;
	}
	else
	{
		_tapeFormat = tapeFormat;
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeFormat (aafTapeFormatType_t*	pTapeFormat)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (pTapeFormat == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if(!_tapeFormat.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}	
	else
	{
		*pTapeFormat = _tapeFormat;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeLength (aafLength_t	tapeLength)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (tapeLength < 0)
	{
		aafError = AAFRESULT_BAD_LENGTH;
	}
	else
	{
		_tapeLength = tapeLength;
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeLength (aafLength_t*	pTapeLength)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (pTapeLength == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if(!_tapeLength.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pTapeLength = _tapeLength;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetOwningMobKind (aafMobKind_t *pMobKind)
{
	*pMobKind = kTapeMob;		// Abstract superclass, only match "all"
	return(AAFRESULT_SUCCESS);
}


OMDEFINE_STORABLE(ImplAAFTapeDescriptor, AUID_AAFTapeDescriptor);

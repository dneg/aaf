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




#ifndef __ImplAAFTapeDescriptor_h__
#include "ImplAAFTapeDescriptor.h"
#endif

#include <assert.h>
#include "AAFResult.h"


ImplAAFTapeDescriptor::ImplAAFTapeDescriptor ():
	_formFactor(		PID_TAPEDESCRIPTOR_FORMFACTOR,		"FormFactor"),
	_videoSignalType(	PID_TAPEDESCRIPTOR_VIDEOSIGNAL,		"VideoSignalType"),
	_tapeFormat(		PID_TAPEDESCRIPTOR_TAPEFORMAT,		"TapeFormat"),
	_tapeLength(		PID_TAPEDESCRIPTOR_LENGTH,			"TapeLength"),
	_manufacturer(		PID_TAPEDESCRIPTOR_MANUFACTURER,	"Manufacturer"),
	_model(				PID_TAPEDESCRIPTOR_MODEL,			"Model")	
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
	else
	{
		*pLen = _manufacturer.length()+1;
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
	else
	{
		*pLen = _model.length()+1;
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
	else
	{
		*pTapeLength = _tapeLength;
	}

	return aafError;
}



extern "C" const aafClassID_t CLSID_AAFTapeDescriptor;

OMDEFINE_STORABLE(ImplAAFTapeDescriptor, CLSID_AAFTapeDescriptor);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFTapeDescriptor::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFTapeDescriptor, sizeof aafClassID_t);
  return AAFRESULT_SUCCESS;
}


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFTapeDescriptor_h__
#include "ImplAAFTapeDescriptor.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <wchar.h>
#include "AAFResult.h"


ImplAAFTapeDescriptor::ImplAAFTapeDescriptor ():
	_formFactor(		PID_TapeDescriptor_FormFactor,		L"FormFactor"),
	_videoSignalType(	PID_TapeDescriptor_VideoSignal,		L"VideoSignal"),
	_tapeFormat(		PID_TapeDescriptor_TapeFormat,		L"TapeFormat"),
	_tapeLength(		PID_TapeDescriptor_Length,			L"Length"),
	_manufacturer(		PID_TapeDescriptor_ManufacturerID,	L"ManufacturerID"),
	_model(				PID_TapeDescriptor_Model,			L"Model")	
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
    ImplAAFTapeDescriptor::SetTapeManufacturer (const aafCharacter*  pName)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
		aafError = AAFRESULT_BAD_SIZE;
	}
	else
	{
		_manufacturer = pName;
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeManufacturer (aafCharacter*	pName,
												aafUInt32	buflen)
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
    ImplAAFTapeDescriptor::GetTapeManufacturerBufLen (aafUInt32 *pLen)
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
    ImplAAFTapeDescriptor::SetTapeModel (const aafCharacter*	pModelName)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pModelName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if (wcslen(pModelName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
		aafError = AAFRESULT_BAD_SIZE;
	}
	else
	{
		_model = pModelName;
	}

	return aafError;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeModel (aafCharacter*	pModelName,
										 aafUInt32	buflen)
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
    ImplAAFTapeDescriptor::GetTapeModelBufLen (aafUInt32*	pLen)
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
	
	if ( formFactor < kAAFTapeCaseNull ||
		 formFactor > kAAFNagraAudioTape )
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

	if (videoSignal < kAAFVideoSignalNull ||
		videoSignal > kAAFSECAMSignal)
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

	if (tapeFormat < kAAFTapeFormatNull ||
		tapeFormat > kAAFHi8Format)
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
    ImplAAFTapeDescriptor::SetTapeLength (aafUInt32	tapeLength)
{
	_tapeLength = tapeLength;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeLength (aafUInt32*	pTapeLength)
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
	*pMobKind = kAAFTapeMob;		// Abstract superclass, only match "all"
	return(AAFRESULT_SUCCESS);
}



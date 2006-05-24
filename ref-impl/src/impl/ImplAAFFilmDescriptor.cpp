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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFFilmDescriptor_h__
#include "ImplAAFFilmDescriptor.h"
#endif

#include "OMAssertions.h"
#include <string.h>
#include <wchar.h>
#include "aafErr.h"

ImplAAFFilmDescriptor::ImplAAFFilmDescriptor ()
: _manufacturer(	PID_FilmDescriptor_Manufacturer,		L"Manufacturer"),
  _model(			PID_FilmDescriptor_Model,				L"Model"),
  _format(			PID_FilmDescriptor_FilmFormat,			L"FilmFormat"),
  _frameRate(		PID_FilmDescriptor_FrameRate,			L"FrameRate"),
  _perfPerFrame(	PID_FilmDescriptor_PerforationsPerFrame,L"PerforationsPerFrame"),
  _aspectRatio(		PID_FilmDescriptor_FilmAspectRatio,		L"FilmAspectRatio")
{
	_persistentProperties.put(_manufacturer.address());
	_persistentProperties.put(_model.address());
	_persistentProperties.put(_format.address());
	_persistentProperties.put(_frameRate.address());
	_persistentProperties.put(_perfPerFrame.address());
	_persistentProperties.put(_aspectRatio.address());
}

ImplAAFFilmDescriptor::~ImplAAFFilmDescriptor ()
{
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmManufacturer (
      const aafCharacter *name)
{
	if(name == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(name)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

	_manufacturer = name;

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmManufacturer (
      aafCharacter *name, aafUInt32 bufSize)
{
	if(name == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_manufacturer.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	bool stat;

	stat = _manufacturer.copyToBuffer(name, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmManufacturerBufLen (
      aafUInt32 *bufSize)
{
	if(bufSize == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(!_manufacturer.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*bufSize = _manufacturer.size();
	return(AAFRESULT_SUCCESS); 
}
	

AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmModel (
      const aafCharacter *name)
{
	if(name == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(name)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

	_model = name;

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmModel (
      aafCharacter *name, aafUInt32 bufSize)
{
	if(name == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_model.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	bool stat;

	stat = _model.copyToBuffer(name, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmModelBufLen (
      aafUInt32 *bufSize)
{		
	if(bufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_model.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*bufSize = _model.size();
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmFormat (
      aafFilmType_t*filmFormat)
{
	aafAssert(filmFormat != NULL, NULL, AAFRESULT_NULL_PARAM);

	if(!_format.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*filmFormat = _format;
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFrameRate (
      aafUInt32*  rate)
{
	aafAssert(rate != NULL, NULL, AAFRESULT_NULL_PARAM);

	if(!_frameRate.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*rate = _frameRate;
	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetPerfPerFrame (
      aafUInt8* perfPerFrame)
{
	aafAssert(perfPerFrame != NULL, NULL, AAFRESULT_NULL_PARAM);
	
	if(!_perfPerFrame.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*perfPerFrame = _perfPerFrame;
	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmAspectRatio (
      aafRational_t*aspectRatio)
{
	aafAssert(aspectRatio != NULL, NULL, AAFRESULT_NULL_PARAM);
	
	if(!_aspectRatio.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*aspectRatio = _aspectRatio;
	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmFormat (
      aafFilmType_t  filmFormat)
{
	_format = filmFormat;
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFrameRate (
      aafUInt32 rate)
{
	_frameRate = rate;
	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetPerfPerFrame (
      aafUInt8 perfPerFrame)
{
	_perfPerFrame = perfPerFrame;
	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmAspectRatio (
      aafRational_t  aspectRatio)
{
	_aspectRatio = aspectRatio;
	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetOwningMobKind (aafMobKind_t *pMobKind)
{
	*pMobKind = kAAFFilmMob;
	return(AAFRESULT_SUCCESS);
}

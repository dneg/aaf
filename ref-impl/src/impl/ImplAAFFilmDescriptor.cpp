/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFFilmDescriptor_h__
#include "ImplAAFFilmDescriptor.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"

ImplAAFFilmDescriptor::ImplAAFFilmDescriptor ()
: _manufacturer(	PID_FilmDescriptor_Manufacturer,		"Manufacturer"),
  _model(			PID_FilmDescriptor_Model,				"Model"),
  _format(			PID_FilmDescriptor_FilmFormat,			"FilmFormat"),
  _frameRate(		PID_FilmDescriptor_FrameRate,			"FrameRate"),
  _perfPerFrame(	PID_FilmDescriptor_PerforationsPerFrame,"PerforationsPerFrame"),
  _aspectRatio(		PID_FilmDescriptor_FilmAspectRatio,		"FilmAspectRatio")
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


//@access Public Members

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmManufacturer (
      wchar_t *name)
{
	if(name == NULL)
		return(AAFRESULT_NULL_PARAM);

	_manufacturer = name;

	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmManufacturer (
      wchar_t *name, aafInt32 bufSize)
{
	if(!_manufacturer.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	bool stat;

	if(name == NULL)
		return(AAFRESULT_NULL_PARAM);

	stat = _manufacturer.copyToBuffer(name, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}

	//@comm Returns a zero-length string if the property was not present

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmManufacturerBufLen (
      aafInt32 *bufSize)
{
	if(bufSize == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(!_manufacturer.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*bufSize = _manufacturer.size();
	return(AAFRESULT_SUCCESS); 
}
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmModel (
      wchar_t *name)
{
	if(name == NULL)
		return(AAFRESULT_NULL_PARAM);

	_model = name;

	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmModel (
      wchar_t *name, aafInt32 bufSize)
{
	if(!_model.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	bool stat;

	if(name == NULL)
		return(AAFRESULT_NULL_PARAM);

	stat = _model.copyToBuffer(name, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmModelBufLen (
      aafInt32 *bufSize)
{		
	if(!_model.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	if(bufSize == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	*bufSize = _model.size();
	return(AAFRESULT_SUCCESS); 
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmFormat (
      aafFilmType_t*filmFormat)
{
	if(!_format.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	aafAssert(filmFormat != NULL, NULL, AAFRESULT_NULL_PARAM);
	*filmFormat = _format;
	return(AAFRESULT_SUCCESS); 
}

	//@comm Film format may be: kFt35MM, kFt16MM, kFt8MM, kFt65MM

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFrameRate (
      aafUInt32*  rate)
{
	if(!_frameRate.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	aafAssert(rate != NULL, NULL, AAFRESULT_NULL_PARAM);
	*rate = _frameRate;
	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetPerfPerFrame (
      aafUInt8* perfPerFrame)
{
	if(!_perfPerFrame.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	aafAssert(perfPerFrame != NULL, NULL, AAFRESULT_NULL_PARAM);
	*perfPerFrame = _perfPerFrame;
	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmAspectRatio (
      aafRational_t*aspectRatio)
{
	if(!_aspectRatio.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	aafAssert(aspectRatio != NULL, NULL, AAFRESULT_NULL_PARAM);
	*aspectRatio = _aspectRatio;
	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmFormat (
      aafFilmType_t  filmFormat)
{
	_format = filmFormat;
	return(AAFRESULT_SUCCESS); 
}

	//@comm kFt35MM, kFt16MM, kFt8MM, kFt65MM

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFrameRate (
      aafUInt32 rate)
{
	_frameRate = rate;
	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetPerfPerFrame (
      aafUInt8 perfPerFrame)
{
	_perfPerFrame = perfPerFrame;
	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmAspectRatio (
      aafRational_t  aspectRatio)
{
	_aspectRatio = aspectRatio;
	return(AAFRESULT_SUCCESS); 
}



OMDEFINE_STORABLE(ImplAAFFilmDescriptor, AUID_AAFFilmDescriptor);


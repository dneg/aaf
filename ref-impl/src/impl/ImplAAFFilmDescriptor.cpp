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


ImplAAFFilmDescriptor::ImplAAFFilmDescriptor ()
: _manufacturer(	PID_FilmDescriptor_Manufacturer,			"Manufacturer"),
  _model(			PID_FilmDescriptor_Model,			"Model"),
  _format(			PID_FilmDescriptor_FilmFormat,	"FilmFormat"),
  _frameRate(		PID_FilmDescriptor_FrameRate,	"FrameRate"),
  _perfPerFrame(	PID_FilmDescriptor_PerforationsPerFrame,			"PerforationsPerFrame"),
  _aspectRatio(		PID_FilmDescriptor_FilmAspectRatio,	"FilmAspectRatio")
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
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmManufacturer (
      wchar_t *name, aafInt32 bufSize)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Returns a zero-length string if the property was not present

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmManufacturerBufLen (
      aafInt32 *bufSize)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmModel (
      wchar_t *name)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmModel (
      wchar_t *name, aafInt32 bufSize)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmModelBufLen (
      aafInt32 *bufSize)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmFormat (
      aafFilmType_t*  /*filmFormat*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm Film format may be: kFt35MM, kFt16MM, kFt8MM, kFt65MM

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFrameRate (
      aafUInt32*  /*rate*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetPerfPerFrame (
      aafUInt8*  /*perfPerFrame*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::GetFilmAspectRatio (
      aafRational_t*  /*aspectRatio*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmFormat (
      aafFilmType_t  /*filmFormat*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	//@comm kFt35MM, kFt16MM, kFt8MM, kFt65MM

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFrameRate (
      aafUInt32  /*rate*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetPerfPerFrame (
      aafUInt8  /*perfPerFrame*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFilmDescriptor::SetFilmAspectRatio (
      aafRational_t  /*aspectRatio*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFFilmDescriptor, AUID_AAFFilmDescriptor);


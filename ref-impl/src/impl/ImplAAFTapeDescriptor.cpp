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
	_tapeLength(		PID_TAPEDESCRIPTOR_LENGTH,			"TapeLength"),
	_manufacturer(		PID_TAPEDESCRIPTOR_MANUFACTURER,	"Manufacturer"),
	_model(				PID_TAPEDESCRIPTOR_MODEL,			"Model")	
{
	_persistentProperties.put(_formFactor.address());
	_persistentProperties.put(_videoSignalType.address());
	_persistentProperties.put(_tapeLength.address());
	_persistentProperties.put(_manufacturer.address());
	_persistentProperties.put(_model.address());
}


ImplAAFTapeDescriptor::~ImplAAFTapeDescriptor ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeManufacturer (aafWChar*  /*pName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeManufacturer (aafWChar*  /*pName*/,
                           aafInt32  /*buflen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetManufacturerNameLen (aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeModel (aafWChar*  /*pModelName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeModel (aafWChar*  /*pModelName*/,
                           aafInt32  /*buflen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeModelLen (aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeFormFactor (aafTapeCaseType_t  /*formFactor*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeFormFactor (aafTapeCaseType_t*  /*formFactor*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetSignalType (aafVideoSignalType_t  /*videoSignal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetSignalType (aafVideoSignalType_t*  /*videoSignal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeFormat (aafTapeFormatType_t  /*tapeFormat*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeFormat (aafTapeFormatType_t*  /*tapeFormat*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::SetTapeLength (aafLength_t  /*tapeFormat*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTapeDescriptor::GetTapeLength (aafLength_t*  /*tapeFormat*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



extern "C" const aafClassID_t CLSID_AAFTapeDescriptor;

OMDEFINE_STORABLE(ImplAAFTapeDescriptor, CLSID_AAFTapeDescriptor);


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




#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include <assert.h>

ImplAAFFileDescriptor::ImplAAFFileDescriptor ()
: _sampleRate(			PID_FILE_DESC_SAMPLERATE,	"sampleRate"),
 _length(				PID_FILE_DESC_LENGTH,		"length"),
 _isInContainer(        PID_FILE_DESC_INCONTAINER,	"isInContainer"),
 _containerFmt(         PID_FILE_DESC_CONTAINERFMT,	"containerFormat")
{
  _persistentProperties.put(_sampleRate.address());
  _persistentProperties.put(_length.address());
  _persistentProperties.put(_isInContainer.address());
  _persistentProperties.put(_containerFmt.address());
}


ImplAAFFileDescriptor::~ImplAAFFileDescriptor ()
{}


//@access Public Members

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetLength (aafLength_t length)
{
	_length = length;
	return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetLength (aafLength_t *pLength)
{
	*pLength = _length;
	return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetIsInContainer (aafBool isAAF)
{
	_isInContainer = isAAF;
	return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetIsInContainer (aafBool* pIsAAF)
{
	*pIsAAF = _isInContainer;
	return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetSampleRate (aafRational_t *pRate)
{
	_sampleRate = *pRate;
	return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetSampleRate (aafRational_t *pRate)
{
	*pRate = _sampleRate;
	return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetContainerFormat (aafUID_t *pFormat)
{
	_containerFmt = *pFormat;
	return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetContainerFormat (aafUID_t *pFormat)
{
	*pFormat = _containerFmt;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetOwningMobKind (aafMobKind_t *pMobKind)
{
	*pMobKind = kFileMob;
	return(AAFRESULT_SUCCESS);
}


extern "C" const aafClassID_t CLSID_AAFFileDescriptor;

OMDEFINE_STORABLE(ImplAAFFileDescriptor, CLSID_AAFFileDescriptor);


//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=




#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"

// Declare these two functions to avoid including the whole ImplAAFDictionary.h
aafBoolean_t aafLookupCodecDef( ImplAAFObject *, ImplAAFCodecDef *p_codecdef );
aafBoolean_t aafLookupContainerDef( ImplAAFObject *,ImplAAFContainerDef * );

ImplAAFFileDescriptor::ImplAAFFileDescriptor ()
: _sampleRate(			PID_FileDescriptor_SampleRate,		L"SampleRate"),
 _length(				PID_FileDescriptor_Length,			L"Length"),
 _codecDef(				PID_FileDescriptor_CodecDefinition,		L"CodecDefinition", L"/Header/Dictionary/CodecDefinitions", PID_DefinitionObject_Identification),
 _containerFmt(         PID_FileDescriptor_ContainerFormat,	L"ContainerFormat", L"/Header/Dictionary/ContainerDefinitions", PID_DefinitionObject_Identification)
{
  _persistentProperties.put(_sampleRate.address());
  _persistentProperties.put(_length.address());
  _persistentProperties.put(_codecDef.address());
  _persistentProperties.put(_containerFmt.address());
}


ImplAAFFileDescriptor::~ImplAAFFileDescriptor ()
{}


//@access Public Members

AAFRESULT STDMETHODCALLTYPE
	ImplAAFFileDescriptor::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetLength (aafLength_t length)
{
	_length = length;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetLength (aafLength_t *pLength)
{
	if(pLength == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pLength = _length;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetCodecDef (ImplAAFCodecDef *pDef)
{
  if (! pDef)
    return AAFRESULT_NULL_PARAM;
  if (!pDef->attached())
    return AAFRESULT_OBJECT_NOT_ATTACHED;
  if (!aafLookupCodecDef( this, pDef ) )
    return AAFRESULT_INVALID_OBJ;

  _codecDef = pDef;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetCodecDef (ImplAAFCodecDef **ppDef)
{
	if(ppDef == NULL)
		return(AAFRESULT_NULL_PARAM);
	*ppDef = _codecDef;
	assert (*ppDef);
  if (*ppDef)
	 (*ppDef)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetSampleRate (const aafRational_t & rate)
{
	_sampleRate = rate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetSampleRate (aafRational_t *pRate)
{
	if(pRate == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pRate = _sampleRate;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::SetContainerFormat (ImplAAFContainerDef *pDef)
{
	if( pDef == NULL )
		return AAFRESULT_NULL_PARAM;
	if( !aafLookupContainerDef( this, pDef ) )
		return AAFRESULT_INVALID_OBJ;

	_containerFmt = pDef;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetContainerFormat (ImplAAFContainerDef **ppDef)
{
	if(ppDef == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_containerFmt.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;	
	
	*ppDef = _containerFmt;
	assert (*ppDef);
	 (*ppDef)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFFileDescriptor::GetOwningMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind  == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kAAFFileMob;
	return(AAFRESULT_SUCCESS);
}



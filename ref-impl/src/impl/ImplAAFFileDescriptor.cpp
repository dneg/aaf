/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/




#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"

ImplAAFFileDescriptor::ImplAAFFileDescriptor ()
: _sampleRate(			PID_FileDescriptor_SampleRate,		"SampleRate"),
 _length(				PID_FileDescriptor_Length,			"Length"),
 _codecDef(				PID_FileDescriptor_CodecDefinition,		"CodecDefinition", "/Dictionary/CodecDefinitions", PID_DefinitionObject_Identification),
 _containerFmt(         PID_FileDescriptor_ContainerFormat,	"ContainerFormat", "/Dictionary/ContainerDefinitions", PID_DefinitionObject_Identification)
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



/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/

#ifndef __ImplAAFAIFCDescriptor_h__
#include "ImplAAFAIFCDescriptor.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFAIFCDescriptor::ImplAAFAIFCDescriptor ()
:   _summary(PID_AIFCDescriptor_Summary,	"Summary")
{
	_persistentProperties.put(_summary.address());
}


ImplAAFAIFCDescriptor::~ImplAAFAIFCDescriptor ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFAIFCDescriptor::Initialize ()
{
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFAIFCDescriptor::GetSummary (aafUInt32	size, aafDataValue_t  pSummary)
{
	if(pSummary == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_summary.size() > size)
	  return AAFRESULT_SMALLBUF;

	_summary.copyToBuffer(pSummary, size);

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFAIFCDescriptor::GetSummaryBufferSize (aafUInt32 *pSize)
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pSize = _summary.size();

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFAIFCDescriptor::SetSummary (aafUInt32 size, aafDataValue_t pSummary)
{
	if(pSummary == NULL)
		return(AAFRESULT_NULL_PARAM);

	_summary.setValue(pSummary, size);

	return(AAFRESULT_SUCCESS); 
}

OMDEFINE_STORABLE(ImplAAFAIFCDescriptor, AUID_AAFAIFCDescriptor);



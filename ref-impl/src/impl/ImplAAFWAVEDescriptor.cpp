/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/






#ifndef __ImplAAFWAVEDescriptor_h__
#include "ImplAAFWAVEDescriptor.h"
#endif

#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFWAVEDescriptor::ImplAAFWAVEDescriptor ()
:   _summary(PID_WAVEDESCRIPTOR_SUMMARY,	"Summary")
{
	_persistentProperties.put(_summary.address());
}


ImplAAFWAVEDescriptor::~ImplAAFWAVEDescriptor ()
{
	// TODO: free summary
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFWAVEDescriptor::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFWAVEDescriptor::GetSummary (aafUInt32 size, aafDataValue_t pSummary)
{
	if(pSummary == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_summary.size() > size)
	  return AAFRESULT_SMALLBUF;

	_summary.copyToBuffer(pSummary, size);

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFWAVEDescriptor::GetSummaryBufferSize (aafUInt32* pSize)
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pSize = _summary.size();

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFWAVEDescriptor::SetSummary (aafUInt32 size, aafDataValue_t  pSummary)
{
	if(pSummary == NULL)
		return(AAFRESULT_NULL_PARAM);

	_summary.setValue(pSummary, size);

	return(AAFRESULT_SUCCESS); 
}



extern "C" const aafClassID_t CLSID_AAFWAVEDescriptor;

OMDEFINE_STORABLE(ImplAAFWAVEDescriptor, CLSID_AAFWAVEDescriptor);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFWAVEDescriptor::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFWAVEDescriptor, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}



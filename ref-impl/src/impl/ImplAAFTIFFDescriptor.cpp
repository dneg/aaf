/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFTIFFDescriptor_h__
#include "ImplAAFTIFFDescriptor.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTIFFDescriptor::ImplAAFTIFFDescriptor ():
_isUniform(PID_TIFFDescriptor_IsUniform,	"Is Uniform"),
_isContiguous(PID_TIFFDescriptor_IsContiguous,	"Is Contiguous"),
_leadingLines(PID_TIFFDescriptor_LeadingLines,	"Leading Lines"),
_trailingLines(PID_TIFFDescriptor_TrailingLines,	"Trailing Lines"),
_jpegTableID(PID_TIFFDescriptor_JPEGTableID,	"JPEGTableID"),
_summary(PID_TIFFDescriptor_Summary,	"Summary")
{
	_persistentProperties.put(_isUniform.address());
	_persistentProperties.put(_isContiguous.address());
	_persistentProperties.put(_leadingLines.address());
	_persistentProperties.put(_trailingLines.address());
	_persistentProperties.put(_jpegTableID.address());
	_persistentProperties.put(_summary.address());

	_isUniform = AAFFalse;
	_isContiguous = AAFFalse;
	_leadingLines = 0;
	_trailingLines = 0;
	_jpegTableID = 0;
}

ImplAAFTIFFDescriptor::~ImplAAFTIFFDescriptor ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::SetIsUniform (aafBool IsUniform)
{
	_isUniform = IsUniform;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::GetIsUniform (aafBool* pIsUniform)
{
	if (pIsUniform == NULL)
		return AAFRESULT_NULL_PARAM;
	else
	{
		*pIsUniform = _isUniform;
		return AAFRESULT_SUCCESS;
	}
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::SetIsContiguous (aafBool IsContiguous)
{
	_isContiguous = IsContiguous;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::GetIsContiguous (aafBool* pIsContiguous)
{
	if (pIsContiguous == NULL)
		return AAFRESULT_NULL_PARAM;
	else
	{
		*pIsContiguous = _isContiguous;
		return AAFRESULT_SUCCESS;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::SetLeadingLines (aafInt32 LeadingLines)
{
	if (LeadingLines< 0)
		return AAFRESULT_ILLEGAL_VALUE;
	else
	{
		_leadingLines = LeadingLines;
		return AAFRESULT_SUCCESS;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::GetLeadingLines (aafInt32* pLeadingLines)
{
	if (pLeadingLines == NULL)
		return AAFRESULT_NULL_PARAM;
	
	else if(!_leadingLines.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	else
	{
		*pLeadingLines = _leadingLines;
		return AAFRESULT_SUCCESS;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::SetTrailingLines (aafInt32 TrailingLines)
{
	if (TrailingLines< 0)
		return AAFRESULT_ILLEGAL_VALUE;
	else
	{
		_trailingLines = TrailingLines;
		return AAFRESULT_SUCCESS;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::GetTrailingLines (aafInt32* pTrailingLines)
{
	if (pTrailingLines == NULL)
		return AAFRESULT_NULL_PARAM;

	else if(!_trailingLines.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	else 
	{
		*pTrailingLines = _trailingLines;
		return AAFRESULT_SUCCESS;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::SetJPEGTableID (aafJPEGTableID_t JPEGTableID)
{
	_jpegTableID = JPEGTableID;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::GetJPEGTableID (aafJPEGTableID_t* pJPEGTableID)
{
	if (pJPEGTableID == NULL)
		return AAFRESULT_NULL_PARAM;
	
	else if(!_jpegTableID.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	else
	{
		*pJPEGTableID = _jpegTableID;
		return AAFRESULT_SUCCESS;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::GetSummary (aafUInt32 size, aafDataValue_t pSummary)
{
	if(pSummary == NULL)
		return AAFRESULT_NULL_PARAM;

	if (_summary.size() > size)
		return AAFRESULT_SMALLBUF;

	_summary.copyToBuffer(pSummary, size);

	return AAFRESULT_SUCCESS; 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::GetSummaryBufferSize (aafUInt32* pSize)
{
	if(pSize == NULL)
		return AAFRESULT_NULL_PARAM;

	*pSize = _summary.size();
	
	return AAFRESULT_SUCCESS; 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTIFFDescriptor::SetSummary (aafUInt32 size, aafDataValue_t pSummary)
{
	if(pSummary == NULL)
		return AAFRESULT_NULL_PARAM;

	_summary.setValue(pSummary, size);

	return AAFRESULT_SUCCESS; 
}



OMDEFINE_STORABLE(ImplAAFTIFFDescriptor, AUID_AAFTIFFDescriptor);



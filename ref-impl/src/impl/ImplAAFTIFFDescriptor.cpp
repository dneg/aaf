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


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFTIFFDescriptor_h__
#include "ImplAAFTIFFDescriptor.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTIFFDescriptor::ImplAAFTIFFDescriptor ():
_isUniform(PID_TIFFDescriptor_IsUniform,	L"IsUniform"),
_isContiguous(PID_TIFFDescriptor_IsContiguous,	L"IsContiguous"),
_leadingLines(PID_TIFFDescriptor_LeadingLines,	L"LeadingLines"),
_trailingLines(PID_TIFFDescriptor_TrailingLines,	L"TrailingLines"),
_jpegTableID(PID_TIFFDescriptor_JPEGTableID,	L"JPEGTableID"),
_summary(PID_TIFFDescriptor_Summary,	L"Summary")
{
	_persistentProperties.put(_isUniform.address());
	_persistentProperties.put(_isContiguous.address());
	_persistentProperties.put(_leadingLines.address());
	_persistentProperties.put(_trailingLines.address());
	_persistentProperties.put(_jpegTableID.address());
	_persistentProperties.put(_summary.address());

	_isUniform = kAAFFalse;
	_isContiguous = kAAFFalse;
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






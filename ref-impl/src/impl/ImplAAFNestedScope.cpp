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



/*************************************************************************
 * 
 * @class AAFNestedScope | an AAFNestedScope object defines a scope that 
 *			contains an ordered set of AAFSegments and produces the value
 *			specified by the last AAFSegement in the ordered set.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#ifndef __ImplEnumAAFSegments_h__
#include "ImplEnumAAFSegments.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "ImplAAFObjectCreation.h"

#ifndef __ImplAAFNestedScope_h__
#include "ImplAAFNestedScope.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "aafCvt.h"
#include "aafErr.h"

extern "C" const aafClassID_t CLSID_AAFSegment;
extern "C" const aafClassID_t CLSID_EnumAAFSegments;

ImplAAFNestedScope::ImplAAFNestedScope ()
:  _slots( PID_NestedScope_Slots, L"Slots")
{
	_persistentProperties.put(_slots.address());
}


ImplAAFNestedScope::~ImplAAFNestedScope ()
{
	// Release all of the slot(segments) pointers in the slot list.
	size_t size = _slots.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFSegment* pSegment = _slots.setValueAt(0, i);

		if (pSegment)
		{
		  pSegment->ReleaseReference();
		  pSegment = 0;
		}
	}

}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::AppendSegment (ImplAAFSegment* pSegment)
{
	if(pSegment == NULL)
		return(AAFRESULT_NULL_PARAM);

	_slots.appendValue(pSegment);
	pSegment->AcquireReference();

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::PrependSegment (ImplAAFSegment* pSegment)
{
	if(pSegment == NULL)
		return(AAFRESULT_NULL_PARAM);

	_slots.prependValue(pSegment);
	pSegment->AcquireReference();

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::InsertSegmentAt (aafUInt32 index,
										 ImplAAFSegment* pSegment)
{
  if(pSegment == NULL)
	return(AAFRESULT_NULL_PARAM);

  aafUInt32 count;
  AAFRESULT hr;
  hr = CountSegments (&count);
  if (AAFRESULT_FAILED (hr)) return hr;

  if (index > count)
	return AAFRESULT_BADINDEX;

  _slots.insertAt(pSegment,index);
  pSegment->AcquireReference();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::CountSegments (aafUInt32 * pResult)
{
  // Validate input pointer...
  if (NULL == pResult)
    return (AAFRESULT_NULL_PARAM);

  size_t numSegments;

	_slots.getSize(numSegments);
	
	*pResult = numSegments;

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::GetSegmentAt (aafUInt32 index,
									  ImplAAFSegment ** ppSegment)
{
  if(ppSegment == NULL)
	return(AAFRESULT_NULL_PARAM);

  aafUInt32 count;
  AAFRESULT hr;
  hr = CountSegments (&count);
  if (AAFRESULT_FAILED (hr)) return hr;

  if (index >= count)
	return AAFRESULT_BADINDEX;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::RemoveSegmentAt (
      aafUInt32 index)
{
	aafUInt32 count;
	AAFRESULT hr;
	ImplAAFSegment	*pSeg;
	
	hr = CountSegments (&count);
	if (AAFRESULT_FAILED (hr)) return hr;
	
	if (index >= count)
		return AAFRESULT_BADINDEX;
	
	pSeg = _slots.removeAt(index);
	if(pSeg)
		pSeg->ReleaseReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::GetSegments (ImplEnumAAFSegments** ppEnum)
{
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFSegments *)CreateImpl(CLSID_EnumAAFSegments);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);

	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFSegment>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFSegment>(_slots);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK((*ppEnum)->Initialize(&CLSID_EnumAAFSegments, this, iter));
	}
	XEXCEPT
	{
		if (*ppEnum)
		  (*ppEnum)->ReleaseReference();
		(*ppEnum) = 0;
		return(XCODE());
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFNestedScope::ChangeContainedReferences(aafMobID_constref from,
														aafMobID_constref to)
{
	ImplAAFComponent	*comp = NULL;
	
	XPROTECT()
	{
		size_t count = _slots.getSize();
		for (size_t n = 0; n < count; n++)
		{
			ImplAAFSegment	*pSegment;
			_slots.getValueAt(pSegment, n);
			CHECK(pSegment->ChangeContainedReferences(from, to));
			pSegment->ReleaseReference();
			pSegment = NULL;
		}
	}
	XEXCEPT
	{
		if(comp != NULL)
		  comp->ReleaseReference();
		comp = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}






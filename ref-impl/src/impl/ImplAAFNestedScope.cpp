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
	size_t count = _slots.count();
	for (size_t i = 0; i < count; i++)
	{
		ImplAAFSegment* pSegment = _slots.clearValueAt(i);

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

	if(pSegment->attached())
		return(AAFRESULT_OBJECT_ALREADY_ATTACHED);

	_slots.appendValue(pSegment);
	pSegment->AcquireReference();

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::PrependSegment (ImplAAFSegment* pSegment)
{
	if(pSegment == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(pSegment->attached())
		return(AAFRESULT_OBJECT_ALREADY_ATTACHED);

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

  if(pSegment->attached())
	return(AAFRESULT_OBJECT_ALREADY_ATTACHED);

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

	size_t numSegments = _slots.count();
	
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

  _slots.getValueAt(*ppSegment,index);

  assert(*ppSegment);
  (*ppSegment)->AcquireReference();

  return AAFRESULT_SUCCESS;
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
		size_t count = _slots.count();
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






/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


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
:  _slots( PID_NestedScope_Slots, "Slots")
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
    ImplAAFNestedScope::RemoveSegment (
      ImplAAFSegment * /*pSegment*/)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFNestedScope::GetSlots (ImplEnumAAFSegments** ppEnum)
{
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFSegments *)CreateImpl(CLSID_EnumAAFSegments);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);
	(*ppEnum)->SetEnumStrongProperty(this, &_slots);

	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFNestedScope::ChangeContainedReferences(aafUID_t *from, aafUID_t *to)
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






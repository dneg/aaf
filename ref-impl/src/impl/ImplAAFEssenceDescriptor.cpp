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


#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#ifndef __ImplEnumAAFLocators_h__
#include "ImplEnumAAFLocators.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"



#ifndef __ImplAAFEssenceDescriptor_h__
#include "ImplAAFEssenceDescriptor.h"
#endif

#include <assert.h>
#include "AAFResult.h"
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFLocators;

ImplAAFEssenceDescriptor::ImplAAFEssenceDescriptor ()
: _locators(         PID_EssenceDescriptor_Locator,          L"Locator")
{
  _persistentProperties.put(_locators.address());
}


ImplAAFEssenceDescriptor::~ImplAAFEssenceDescriptor ()
{
	// Release all of the locator pointers.
	size_t count = _locators.count();
	for (size_t i = 0; i < count; i++)
	{
		ImplAAFLocator *pLocator = _locators.clearValueAt(i);
		if (pLocator)
		{
		  pLocator->ReleaseReference();
		  pLocator = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::CountLocators (aafUInt32 *pCount)
{
	if (! pCount)
	{
		return AAFRESULT_NULL_PARAM;
	}

	*pCount = _locators.count();
	return(AAFRESULT_SUCCESS);
}

  //@comm The number of locators may be zero if the essence is in the current file.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::AppendLocator (ImplAAFLocator *pLocator)
{
	if(pLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(pLocator->attached())
		return(AAFRESULT_OBJECT_ALREADY_ATTACHED);

	_locators.appendValue(pLocator);
	pLocator->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

  //@comm    Use this function to add a locator to be scanned last when searching for
  // the essence (a new primary location).

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::PrependLocator (ImplAAFLocator *pLocator)
{
	if(pLocator == NULL)
		return(AAFRESULT_NULL_PARAM);
  if (pLocator->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  _locators.prependValue(pLocator);
	pLocator->AcquireReference();

	return AAFRESULT_SUCCESS;
}

  //@comm    Use this function to add a locator to be scanned first when searching for
  // the essence (a secondary location for the essence).


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::InsertLocatorAt (aafUInt32 index,
											   ImplAAFLocator *pLocator)
{
	if (NULL == pLocator)
		return AAFRESULT_NULL_PARAM;
  if (pLocator->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;
  if (index > _locators.count())
    return AAFRESULT_BADINDEX;

	_locators.insertAt(pLocator, index);
	pLocator->AcquireReference();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::GetLocatorAt (aafUInt32 index,
											ImplAAFLocator ** ppLocator)
{
	if (! ppLocator) return AAFRESULT_NULL_PARAM;
	
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountLocators (&count);
	if (AAFRESULT_FAILED (hr)) return hr;
	
	if (index >= count)
		return AAFRESULT_BADINDEX;
	
	_locators.getValueAt(*ppLocator, index);
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::RemoveLocatorAt (aafUInt32 index)
{
	if (index >= _locators.count())
	  return AAFRESULT_BADINDEX;
	
	ImplAAFLocator *pLocator = _locators.removeAt(index);
  if (pLocator)
  {
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    pLocator->ReleaseReference ();
  }
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::RemoveLocator (ImplAAFLocator *pLocator)
{
	if (NULL == pLocator)
		return AAFRESULT_NULL_PARAM;
  if (!pLocator->attached ()) // locator could not possibly be in _locators container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;

  size_t index;
  if (_locators.findIndex (pLocator, index))
	  return RemoveLocatorAt (index);
  else
    return AAFRESULT_OBJECT_NOT_FOUND;

	// return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::GetLocators (ImplEnumAAFLocators **ppEnum)
{
	if (ppEnum == NULL) return AAFRESULT_NULL_PARAM;

	ImplEnumAAFLocators		*theEnum = (ImplEnumAAFLocators *)CreateImpl (CLSID_EnumAAFLocators);
		
	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFLocator>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFLocator>(_locators);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFLocators, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

  //@comm The number of locators may be zero if the essence is in the current file.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::GetOwningMobKind (aafMobKind_t *pMobKind)
{
	*pMobKind = kAAFAllMob;		// Abstract superclass, only match "all"
	return(AAFRESULT_SUCCESS);
}

// Internal to the toolkit functions
AAFRESULT
    ImplAAFEssenceDescriptor::GetNthLocator (aafInt32 index, ImplAAFLocator **ppLocator)
{
	if(ppLocator == NULL)
		return(AAFRESULT_NULL_PARAM);
  if ((aafUInt32)index >= _locators.count())
		return AAFRESULT_NO_MORE_OBJECTS; // AAFRESULT_BADINDEX ???

	_locators.getValueAt(*ppLocator, index);
  assert(*ppLocator); // locator should never be NULL.
	(*ppLocator)->AcquireReference();

	return AAFRESULT_SUCCESS;
}


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
: _locators(         PID_EssenceDescriptor_Locator,          "Locator")
{
  _persistentProperties.put(_locators.address());
}


ImplAAFEssenceDescriptor::~ImplAAFEssenceDescriptor ()
{
	// Release all of the locator pointers.
	size_t size = _locators.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFLocator *pLocator = _locators.setValueAt(0, i);
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

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::GetLocators (ImplEnumAAFLocators **ppEnum)
{
	ImplEnumAAFLocators		*theEnum = (ImplEnumAAFLocators *)CreateImpl (CLSID_EnumAAFLocators);
		
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_locators));
		CHECK(theEnum->Reset());
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
		return(XCODE());
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


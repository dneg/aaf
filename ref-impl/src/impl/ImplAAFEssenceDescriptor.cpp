/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#ifndef __ImplEnumAAFLocators_h__
#include "ImplEnumAAFLocators.h"
#endif






#ifndef __ImplAAFEssenceDescriptor_h__
#include "ImplAAFEssenceDescriptor.h"
#endif

#include <assert.h>
#include "AAFResult.h"

ImplAAFEssenceDescriptor::ImplAAFEssenceDescriptor ()
: _locators(         PID_ESSENCE_DESC_LOCATORS,          "locators")
{
  _persistentProperties.put(_locators.address());
}


ImplAAFEssenceDescriptor::~ImplAAFEssenceDescriptor ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::GetNumLocators (aafInt32 *pCount)
{
	size_t	siz;
	if (! pCount)
	{
		return AAFRESULT_NULL_PARAM;
	}

	_locators.getSize(siz);
	*pCount = siz;
	return(AAFRESULT_SUCCESS);
}

  //@comm The number of locators may be zero if the essence is in the current file.

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::AppendLocator (ImplAAFLocator *pLocator)
{
	_locators.appendValue(pLocator);
	return(AAFRESULT_SUCCESS);
}

  //@comm    Use this function to add a locator to be scanned first when searching for
  // the essence (a new primary location).

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::PrependLocator (ImplAAFLocator * /*pLocator*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  //@comm    Use this function to add a locator to be scanned first when searching for
  // the essence (a secondary location for the essence).

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::RemoveLocator (ImplAAFLocator * /*pLocator*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceDescriptor::EnumAAFAllLocators (ImplEnumAAFLocators **ppEnum)
{
#if FULL_TOOLKIT
	ImplEnumAAFLocators		*theEnum = (ImplEnumAAFLocators *)CreateImpl (CLSID_EnumAAFLocators);
		
	XPROTECT()
	{
		CHECK(theEnum->SetEssenceDescriptor(this));
		CHECK(theEnum->Reset());
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

  //@comm The number of locators may be zero if the essence is in the current file.



extern "C" const aafClassID_t CLSID_AAFEssenceDescriptor;

OMDEFINE_STORABLE(ImplAAFEssenceDescriptor, CLSID_AAFEssenceDescriptor);


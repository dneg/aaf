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

#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif






#ifndef __ImplEnumAAFLocators_h__
#include "ImplEnumAAFLocators.h"
#endif

#include <assert.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"

extern "C" const aafClassID_t CLSID_EnumAAFLocators;

ImplEnumAAFLocators::ImplEnumAAFLocators ()
{
	_current = 0;
	_enumObj = NULL;
	_enumStrongProp = NULL;
}


ImplEnumAAFLocators::~ImplEnumAAFLocators ()
{
	if (_enumObj)
	{
		_enumObj->ReleaseReference();
		_enumObj = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::NextOne (ImplAAFLocator **ppLocator)
{
	aafUInt32			numElem;
	if(_enumStrongProp != NULL)
	{
		size_t	siz;
		
		_enumStrongProp->getSize(siz);
		numElem = siz;
	}
	else
		return(AAFRESULT_INCONSISTANCY);

	if(ppLocator == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(_current >= numElem)
		return AAFRESULT_NO_MORE_OBJECTS;
	XPROTECT()
	{
		if(_enumStrongProp != NULL)
		{
			_enumStrongProp->getValueAt(*ppLocator, _current);
			(*ppLocator)->AcquireReference();
		}
		else
			RAISE(AAFRESULT_INCONSISTANCY);
		_current++;
	}
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::Next (aafUInt32  count,
                           ImplAAFLocator **ppLocators,
                           aafUInt32 *pFetched)
{
	ImplAAFLocator		**	ppLoc;
	aafUInt32				numDefs;
	HRESULT					hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppLoc = ppLocators;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		hr = NextOne(ppLoc);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppLoc++;
	}
	
	if (pFetched)
		*pFetched = numDefs;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::Skip (aafUInt32 count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent;
	aafUInt32	numElem;

	if(_enumStrongProp != NULL)
	{
		size_t	siz;
		
		_enumStrongProp->getSize(siz);
		numElem = siz;
	}
	else
		return(AAFRESULT_INCONSISTANCY);

	newCurrent = _current + count;

	if(newCurrent < numElem)
	{
		_current = newCurrent;
		hr = AAFRESULT_SUCCESS;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::Clone (ImplEnumAAFLocators **ppEnum)
{
	ImplEnumAAFLocators		*result;
	AAFRESULT		hr;

	result = (ImplEnumAAFLocators *)CreateImpl(CLSID_EnumAAFLocators);
	if (result == NULL)
		return E_FAIL;

	if(_enumStrongProp != NULL)
		hr = result->SetEnumStrongProperty(_enumObj, _enumStrongProp);
	if (SUCCEEDED(hr))
	{
		result->_current = _current;
		*ppEnum = result;
	}
	else
	{
		result->ReleaseReference();
		*ppEnum = NULL;
	}
	
	return hr;
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFLocators::SetEnumStrongProperty( ImplAAFObject *pObj, locatorStrongRefArrayProp_t *pProp)
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	/**/
	_enumStrongProp = pProp;		// Don't refcount, same lifetime as the object.

	return AAFRESULT_SUCCESS;
}

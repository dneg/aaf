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


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif






#ifndef __ImplEnumAAFParameterDefs_h__
#include "ImplEnumAAFParameterDefs.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"

extern "C" const aafClassID_t CLSID_EnumAAFParameterDefs;

ImplEnumAAFParameterDefs::ImplEnumAAFParameterDefs ()
{
	_current = 0;
	_enumObj = NULL;
	_enumProp = NULL;
}


ImplEnumAAFParameterDefs::~ImplEnumAAFParameterDefs ()
{
	if (_enumObj)
	{
		_enumObj->ReleaseReference();
		_enumObj = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::NextOne (
      ImplAAFParameterDef **ppParameterDef)
{
	aafUInt32			numElem = _enumProp->size() / sizeof(aafUID_t);
	aafUID_t			*tmp;
	ImplAAFHeader		*head;
	ImplAAFDictionary	*dict;

	if(ppParameterDef == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(_current >= numElem)
		return AAFRESULT_NO_MORE_OBJECTS;
	XPROTECT()
	{
		tmp = new aafUID_t[_enumProp->size()];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);

		_enumProp->copyToBuffer(tmp, _enumProp->size());
		CHECK(_enumObj->MyHeadObject(&head));
		CHECK(head->GetDictionary (&dict));
		CHECK(dict->LookupPluggableDef((tmp + _current), (ImplAAFPluggableDef **)ppParameterDef));//!!!
		_current++;
		delete [] tmp;
	}
	XEXCEPT
	{
		if(tmp != NULL)
			delete [] tmp;
	}
	XEND;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::Next (
      aafUInt32  count,
      ImplAAFParameterDef **ppParameterDefs,
      aafUInt32 * pFetched)
{
	ImplAAFParameterDef**	ppDef;
	aafUInt32				numDefs;
	HRESULT					hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppParameterDefs;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		hr = NextOne(ppDef);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppDef++;
	}
	
	if (pFetched)
		*pFetched = numDefs;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::Skip (
      aafUInt32  count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent;
	aafUInt32	numElem = _enumProp->size() / sizeof(aafUID_t);

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
    ImplEnumAAFParameterDefs::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFParameterDefs::Clone (
      ImplEnumAAFParameterDefs **ppEnum)
{
	ImplEnumAAFParameterDefs	*result;
	AAFRESULT					hr;

	result = (ImplEnumAAFParameterDefs *)CreateImpl(CLSID_EnumAAFParameterDefs);
	if (result == NULL)
		return E_FAIL;

	hr = result->SetEnumProperty(_enumObj, _enumProp);
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
    ImplEnumAAFParameterDefs::SetEnumProperty( ImplAAFObject *pObj, parmDefWeakRefArrayProp_t *pProp)
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	_enumProp = pProp;				// Don't refcount, same lifetime as the object.

	return AAFRESULT_SUCCESS;
}







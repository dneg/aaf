/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

#ifndef __ImplAAFOperationDef_h__
#include "ImplAAFOperationDef.h"
#endif

#ifndef __ImplEnumAAFOperationDefs_h__
#include "ImplEnumAAFOperationDefs.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"

extern "C" const aafClassID_t CLSID_EnumAAFOperationDefs;

ImplEnumAAFOperationDefs::ImplEnumAAFOperationDefs ()
{
	_current = 0;
	_enumObj = NULL;
	_enumProp = NULL;
	_enumStrongProp = NULL;
}


ImplEnumAAFOperationDefs::~ImplEnumAAFOperationDefs ()
{
	if (_enumObj)
	{
		_enumObj->ReleaseReference();
		_enumObj = NULL;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFOperationDefs::NextOne (
      ImplAAFOperationDef **ppOperationDef)
{
	aafUInt32			numElem;
	aafUID_t			value;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;

	if(_enumProp != NULL)
		numElem = _enumProp->size() / sizeof(aafUID_t);
	else if(_enumStrongProp != NULL)
	{
		size_t	siz;
		
		_enumStrongProp->getSize(siz);
		numElem = siz;
	}
	//!!!Else assert

	if(ppOperationDef == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(_current >= numElem)
		return AAFRESULT_NO_MORE_OBJECTS;
	XPROTECT()
	{
		if(_enumProp != NULL)
		{
			_enumProp->getValueAt(&value, _current);
			CHECK(_enumObj->MyHeadObject(&head));
			CHECK(head->GetDictionary (&dict));
			CHECK(dict->LookupOperationDefinition(&value, ppOperationDef));
			head->ReleaseReference();
			head = NULL;
			dict->ReleaseReference();
			dict = NULL;
		}
		else if(_enumStrongProp != NULL)
		{
			_enumStrongProp->getValueAt(*ppOperationDef, _current);
			(*ppOperationDef)->AcquireReference();
		}
		//!!!Else assert
		_current++;
		if (head) {
			head->ReleaseReference();
			head = NULL;
		}
		if (dict) {
			dict->ReleaseReference();
			dict = NULL;
		}
	}
	XEXCEPT
	{
		if(head)
			head->ReleaseReference();
		if(dict)
			dict->ReleaseReference();
	}
	XEND;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFOperationDefs::Next (
      aafUInt32  count,
      ImplAAFOperationDef **ppOperationDefs,
      aafUInt32 *pFetched)
{
	ImplAAFOperationDef**	ppDef;
	aafUInt32			numDefs;
	HRESULT				hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppDef = ppOperationDefs;
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
    ImplEnumAAFOperationDefs::Skip (
      aafUInt32  count)
{
	AAFRESULT	hr;
	aafUInt32	newCurrent;
	aafUInt32	numElem;
	if(_enumProp != NULL)
		numElem = _enumProp->size() / sizeof(aafUID_t);
	else if(_enumStrongProp != NULL)
	{
		size_t	siz;
		
		_enumStrongProp->getSize(siz);
		numElem = siz;
	}
	//!!!Else assert

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
    ImplEnumAAFOperationDefs::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFOperationDefs::Clone (
      ImplEnumAAFOperationDefs **ppEnum)
{
	ImplEnumAAFOperationDefs	*result;
	AAFRESULT				hr;

	result = (ImplEnumAAFOperationDefs *)CreateImpl(CLSID_EnumAAFOperationDefs);
	if (result == NULL)
		return E_FAIL;

	if(_enumProp != NULL)
		hr = result->SetEnumProperty(_enumObj, _enumProp);
	else if(_enumStrongProp != NULL)
		hr = result->SetEnumStrongProperty(_enumObj, _enumStrongProp);
	// !!!Else assert
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
    ImplEnumAAFOperationDefs::SetEnumProperty( ImplAAFObject *pObj, OperationDefWeakRefArrayProp_t *pProp)
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = pObj;

	if (pObj)
		pObj->AcquireReference();
	/**/
	_enumProp = pProp;				// Don't refcount, same lifetime as the object.
	_enumStrongProp = NULL;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFOperationDefs::SetEnumStrongProperty( ImplAAFObject *pObj, OperationDefStrongRefArrayProp_t *pProp)
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	/**/
	_enumStrongProp = pProp;				// Don't refcount, same lifetime as the object.
	_enumProp = NULL;

	return AAFRESULT_SUCCESS;
}








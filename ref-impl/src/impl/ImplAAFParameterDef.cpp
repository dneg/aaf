/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFDictionary.h"

ImplAAFParameterDef::ImplAAFParameterDef ()
: _typeDef(			PID_ParameterDefinition_Type,					"Type"),
  _displayUnits(	PID_ParameterDefinition_DisplayUnits,			"DisplayUnits")
{
	_persistentProperties.put(_typeDef.address());
	_persistentProperties.put(_displayUnits.address());
}


ImplAAFParameterDef::~ImplAAFParameterDef ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetTypeDef (
      ImplAAFTypeDef **ppTypeDef)
{
	aafUID_t			uid;
	ImplAAFDictionary	*dict = NULL;

	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		uid = _typeDef;
		CHECK(GetDictionary(&dict));
		CHECK(dict->LookupType (&uid, ppTypeDef));
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(dict != NULL)
			dict->ReleaseReference();
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetTypeDef (
      ImplAAFTypeDef * pTypeDef)
{
	aafUID_t	uid;
	AAFRESULT	hr;

	if(pTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	hr = pTypeDef->GetAUID(&uid);
	if(hr == AAFRESULT_SUCCESS)
		_typeDef = uid;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnits (
      wchar_t *pDisplayUnits,
      aafInt32  bufSize)
{
	if(pDisplayUnits == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_displayUnits.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _displayUnits.copyToBuffer(pDisplayUnits, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnitsBufLen (
      aafInt32 * pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_displayUnits.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pLen = _displayUnits.size();
	return(AAFRESULT_SUCCESS); 
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetDisplayUnits (
      wchar_t *pDisplayUnits)
{
	if(pDisplayUnits == NULL)
		return(AAFRESULT_NULL_PARAM);

	_displayUnits = pDisplayUnits;

	return(AAFRESULT_SUCCESS); 
}

OMDEFINE_STORABLE(ImplAAFParameterDef, AUID_AAFParameterDef);



/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/


#ifndef __ImplAAFReferenceValue_h__
#include "ImplAAFReferenceValue.h"
#endif

#ifndef __ImplEnumAAFReferenceValues_h__
#include "ImplEnumAAFReferenceValues.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFParameterDef::ImplAAFParameterDef ()
: _typeDef(			PID_ParameterDefinition_Type,					"Type"),
  _refVal(			PID_ParameterDefinition_ReferenceValues,		"ReferenceValues"),
  _displayUnits(	PID_ParameterDefinition_DisplayUnits,			"DisplayUnits")
{
	_persistentProperties.put(_typeDef.address());
	_persistentProperties.put(_refVal.address());
	_persistentProperties.put(_displayUnits.address());
}


ImplAAFParameterDef::~ImplAAFParameterDef ()
{
	size_t size = _refVal.getSize();
	for (size_t i = 0; i < size; i++) {
		ImplAAFReferenceValue *pRefVal = _refVal.setValueAt(0, i);

		if (pRefVal) {
			pRefVal->ReleaseReference();
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetTypeDef (
      ImplAAFTypeDef **ppTypeDef)
{
	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetTypeDef (
      ImplAAFTypeDef * pTypeDef)
{
	if(pTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnits (
      wchar_t *pDisplayUnits,
      aafInt32  bufSize)
{
	bool stat;

	if(pDisplayUnits == NULL)
		return(AAFRESULT_NULL_PARAM);

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


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::AddReferenceValue (
      ImplAAFReferenceValue * /*pReferenceValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetRefValues (
      ImplEnumAAFReferenceValues ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFParameterDef, AUID_AAFParameterDef);



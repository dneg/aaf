/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/ 

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif





#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFControlPoint_h__
#include "ImplAAFControlPoint.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFDefObject.h"
#include "ImplAAFParameterDef.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFHeader.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"


ImplAAFControlPoint::ImplAAFControlPoint ()
: _type(			PID_ControlPoint_Type,			"Type"),
  _time(			PID_ControlPoint_Time,			"Time"),
  _value(			PID_ControlPoint_Value,			"Value"),
  _hint(			PID_ControlPoint_EditHint,		"EditHint")
{
	_persistentProperties.put(_type.address());
	_persistentProperties.put(_time.address());
	_persistentProperties.put(_value.address());
	_persistentProperties.put(_hint.address());
}


ImplAAFControlPoint::~ImplAAFControlPoint ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetTime (
      aafRational_t *pTime)
{
	if(pTime == NULL)
		return AAFRESULT_NULL_PARAM;
	*pTime = _time;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetEditHint (
      aafEditHint_t *pEditHint)
{
	aafUInt16	intHint;
	
	if(pEditHint == NULL)
		return AAFRESULT_NULL_PARAM;
	intHint = _hint;
	*pEditHint = (aafEditHint_t)intHint;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetValueBufLen (
      aafUInt32 * pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pLen = _value.size();

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetValue (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue,
      aafUInt32*  bytesRead)
{
	if(pValue == NULL || bytesRead == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_value.size() > valueSize)
	  return AAFRESULT_SMALLBUF;

	_value.copyToBuffer(pValue, valueSize);
	*bytesRead  = _value.size();

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetTime (
      aafRational_t pTime)
{
	_time = pTime;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetEditHint (
      aafEditHint_t  editHint)
{
	_hint = (aafUInt16)editHint;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetValue (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue)
{
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);

	_value.setValue(pValue, valueSize);

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetTypeDefinition (
      ImplAAFTypeDef*  pTypeDef)
{
	aafUID_t			newUID;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;

	if(pTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pTypeDef->GetAUID(&newUID));
		CHECK(pTypeDef->MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
//		if(dict->LookupType(&newUID, &def) == AAFRESULT_SUCCESS)
//			def->ReleaseReference();

		_type = newUID;
//		pTypeDef->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
			head->ReleaseReference();
		if(dict)
			dict->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef)
{
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict =  NULL;
	aafUID_t			typeID;

	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
		typeID = _type;
		CHECK(dict->LookupType(&typeID, ppTypeDef));
//		(*ppTypeDef)->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
			head->ReleaseReference();
		if(dict)
			dict->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

OMDEFINE_STORABLE(ImplAAFControlPoint, AUID_AAFControlPoint);




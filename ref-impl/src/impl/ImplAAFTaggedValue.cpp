/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIds.h"

#ifndef __ImplAAFTaggedValue_h__
#include "ImplAAFTaggedValue.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFHeader.h"

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "aafErr.h"


ImplAAFTaggedValue::ImplAAFTaggedValue ():
	_name(		PID_TaggedValue_Name,		"Name"),
	_type(		PID_TaggedValue_Type,		"Type"),
	_value(		PID_TaggedValue_Value,		"Value")
{
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_type.address());
	_persistentProperties.put(_value.address());
}


ImplAAFTaggedValue::~ImplAAFTaggedValue ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::Initialize (wchar_t* pName, aafUID_t*  pDataDef)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	if (pName == NULL || pDataDef == NULL)
		return AAFRESULT_NULL_PARAM;

	_type = *pDataDef;
	_name = pName;

	return rc;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::GetName (wchar_t* pName, aafInt32 bufSize)
{
    AAFRESULT	aafError = AAFRESULT_SUCCESS;
	bool		status;

	if (pName == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		status = _name.copyToBuffer(pName, bufSize);
		if (!status)
			aafError = AAFRESULT_SMALLBUF;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::GetNameBufLen (aafInt32* pLen)
{
    AAFRESULT	aafError = AAFRESULT_SUCCESS;

	if (pLen == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pLen = _name.size();
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::GetTypeDefinition (ImplAAFTypeDef** ppTypeDef)
{
	aafUID_t			defUID;
	ImplAAFDictionary	*dict = NULL;
	ImplAAFHeader		*head = NULL;

	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		defUID = _type;
		CHECK(MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
		CHECK(dict->LookupType(&defUID, ppTypeDef));
	}
	XEXCEPT
	{
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
		if(head != NULL)
		  head->ReleaseReference();
		head = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

  // 
  // Succeeds if all of the following are true:
  // - the ppTypeDef pointer is valid.
  // 
  // If this method fails nothing will be written to *ppTypeDef.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect\, it will return the first one
  // encountered in the order given below:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  \(This is the only code indicating success.\)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize\(\) called on it.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - This property does not exist in the file.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppTypeDef arg is NULL.)

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::GetValue (aafUInt32 valueSize, aafDataBuffer_t pValue, aafUInt32* bytesRead)
{
	if (pValue == NULL || bytesRead == NULL)
		return AAFRESULT_NULL_PARAM;

	if (_value.size() > valueSize)
	  return AAFRESULT_SMALLBUF;

	_value.copyToBuffer(pValue, valueSize);
	*bytesRead  = _value.size();

	return AAFRESULT_SUCCESS; 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::GetValueBufLen (aafUInt32* pLen)
{
	if(pLen == NULL)
		return AAFRESULT_NULL_PARAM;

	*pLen = _value.size();

	return AAFRESULT_SUCCESS; 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::SetValue (aafUInt32 valueSize, aafDataBuffer_t pValue)
{
	if(pValue == NULL)
		return AAFRESULT_NULL_PARAM;

	_value.setValue(pValue, valueSize);

	return AAFRESULT_SUCCESS; 
}



OMDEFINE_STORABLE(ImplAAFTaggedValue, AUID_AAFTaggedValue);



//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFTaggedValue_h__
#include "ImplAAFTaggedValue.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFDataDef.h"
#include "ImplAAFTypeDefIndirect.h"

#include <string.h>
#include <wchar.h>
#include "AAFResult.h"
#include "aafErr.h"


ImplAAFTaggedValue::ImplAAFTaggedValue ():
	_name(		PID_TaggedValue_Name,		L"Name"),
	_value(		PID_TaggedValue_Value,		L"Value"),
  _initialized(false),
  _cachedTypeDef(NULL)
{
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_value.address());
}


ImplAAFTaggedValue::~ImplAAFTaggedValue ()
{
  if (_cachedTypeDef)
    _cachedTypeDef->ReleaseReference ();
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::Initialize (
      const aafCharacter * pName,
      ImplAAFTypeDef * pType, 
      aafUInt32 valueSize, 
      aafDataBuffer_t pValue)
{
  AAFRESULT	result = AAFRESULT_SUCCESS;
  if (!pName || !pType || !pValue)
    return AAFRESULT_NULL_PARAM;
  if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
    return AAFRESULT_BAD_SIZE;
  if (_initialized)
    return AAFRESULT_ALREADY_INITIALIZED;

	_name = pName;

  // Save the type so that SetValue will know the type of the value data.
  _cachedTypeDef = pType;
  _cachedTypeDef->AcquireReference ();

	result = SetValue (valueSize, pValue);
  if (AAFRESULT_SUCCEEDED (result))
  {
    _initialized = true;
  }
  else
  {
    _cachedTypeDef->ReleaseReference ();
    _cachedTypeDef = NULL;
  }


  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::GetName (aafCharacter * pName, aafInt32 bufSize)
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
    ImplAAFTaggedValue::GetNameBufLen (aafUInt32* pLen)
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
	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	// Validate the property and get the actual type definition from the
  // indirect value.
	return (ImplAAFTypeDefIndirect::GetActualPropertyType (_value, ppTypeDef));
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

//	if (_value.size() > valueSize)
//	  return AAFRESULT_SMALLBUF;

//	_value.copyToBuffer(pValue, valueSize);
//	*bytesRead  = _value.size();
	*bytesRead = 0;

	// Validate the property and get the property definition and type definition, 
	// and the actual length of the data
	return (ImplAAFTypeDefIndirect::GetActualPropertyValue (_value, pValue, valueSize, bytesRead));
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::GetValueBufLen (aafUInt32* pLen)
{
	if(pLen == NULL)
		return AAFRESULT_NULL_PARAM;

//	*pLen = _value.size();

	// Validate the property and get the actual length of the data
	return (ImplAAFTypeDefIndirect::GetActualPropertySize (_value, pLen)); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValue::SetValue (aafUInt32 valueSize, aafDataBuffer_t pValue)
{
	if (!pValue)
		return AAFRESULT_NULL_PARAM;

	if (valueSize > OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

//	_value.setValue(pValue, valueSize);
  if (!_cachedTypeDef)
  {
    // Lookup the type definition from this constrol point. If it fails
    // then the control point is invalid!
    AAFRESULT result = GetTypeDefinition (&_cachedTypeDef);
    if (AAFRESULT_FAILED (result))
      return result;
  }

  // Validate the property and get the property definition and type definition, 
	// and the actual length of the data
  return (ImplAAFTypeDefIndirect::SetActualPropertyValue (_value, _cachedTypeDef, pValue, valueSize));
}






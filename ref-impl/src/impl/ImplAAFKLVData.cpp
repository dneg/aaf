//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFKLVData_h__
#include "ImplAAFKLVData.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFDataDef.h"
#include "ImplAAFTypeDefOpaque.h"
#include "ImplAAFTypeDefRename.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFBuiltinDefs.h"

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "aafErr.h"

extern "C" const aafClassID_t CLSID_AAFTypeDefRename;

ImplAAFKLVData::ImplAAFKLVData ():
	_value(		PID_KLVData_Value,		L"Value"),
  _initialized(false),
  _cachedRenameTypeDef(NULL)
{
	_persistentProperties.put(_value.address());
}


ImplAAFKLVData::~ImplAAFKLVData ()
{
//  if (_cachedRenameTypeDef)
//    _cachedRenameTypeDef->ReleaseReference ();
  _cachedRenameTypeDef = 0;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVData::Initialize (
      aafUID_t keyUID, 
      aafUInt32 valueSize, 
      aafDataBuffer_t pValue)
{
	ImplAAFDictionary		*pDict = NULL;
	ImplAAFTypeDef			*pDef = NULL;
	if (!pValue)
		return AAFRESULT_NULL_PARAM;
	if (_initialized)
		return AAFRESULT_ALREADY_INITIALIZED;
	
	XPROTECT()
	{
	// Save the type so that SetValue will know the type of the value data.
		CHECK(GetDictionary(&pDict));
		CHECK(pDict->LookupOpaqueTypeDef(keyUID, &pDef));
    pDef->ReleaseReference(); // This object is owned by the dictionary!

    _cachedRenameTypeDef = dynamic_cast<ImplAAFTypeDefRename*>(pDef);
		assert(_cachedRenameTypeDef);
    if (NULL == _cachedRenameTypeDef)
      RAISE(AAFRESULT_INVALID_OBJ);

		CHECK(SetValue (valueSize, pValue));
		_initialized = true;
		if(pDict)
		  pDict->ReleaseReference();
		pDict = 0;
	}
	XEXCEPT
	{
		if(pDict)
		  pDict->ReleaseReference();
		pDict = 0;
	}
	XEND
	
	
	return AAFRESULT_SUCCESS;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVData::GetKey (aafUID_t* pKey)
{
	AAFRESULT		hr = AAFRESULT_SUCCESS;

	if(pKey == NULL)
		return AAFRESULT_NULL_PARAM;

	// Validate the property and get the actual type id from the
  // indirect value.
	hr = ImplAAFTypeDefIndirect::GetActualPropertyTypeID (_value, pKey);

	return hr;
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
    ImplAAFKLVData::GetValue (aafUInt32 valueSize, aafDataBuffer_t pValue, aafUInt32* bytesRead)
{
//	ImplAAFDictionary	*pDict;
//	ImplAAFTypeDef		*pType;
//	AAFRESULT			hr;
//	aafUID_t			theKey;

	if (pValue == NULL || bytesRead == NULL)
		return AAFRESULT_NULL_PARAM;

//	GetKey(&theKey);
//	GetDictionary(&pDict);
//	hr = pDict->LookupOpaqueTypeDef(theKey, &pType);
//	if(pDict)
//	  pDict->ReleaseReference();
//	pDict = 0;
//	if(hr != AAFRESULT_SUCCESS)
//		return hr;

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
    ImplAAFKLVData::GetValueBufLen (aafUInt32* pLen)
{
	if(pLen == NULL)
		return AAFRESULT_NULL_PARAM;

//	*pLen = _value.size();

	// Validate the property and get the actual length of the data
	return (ImplAAFTypeDefIndirect::GetActualPropertySize (_value, pLen)); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVData::SetValue (aafUInt32 valueSize, aafDataBuffer_t pValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
	ImplAAFTypeDef		*pDef = NULL;
	ImplAAFDictionary	*pDict = NULL;
	aafUID_t			keyUID;

	if (!pValue)
		return AAFRESULT_NULL_PARAM;
	
	if (valueSize > OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	if (!_cachedRenameTypeDef)
	{
		// Lookup the type definition from this KLV data. If it fails
		// then the control point is invalid!
		result = GetKey(&keyUID);
    if (AAFRESULT_FAILED(result))
      return result;

    result = GetDictionary(&pDict);
    if (AAFRESULT_FAILED(result))
      return result;

    result = pDict->LookupTypeDef(keyUID, &pDef);
		pDict->ReleaseReference();
		pDict = 0;
    if (AAFRESULT_FAILED(result))
      return result;
    pDef->ReleaseReference(); // This object is owned by the dictionary!

    _cachedRenameTypeDef = dynamic_cast<ImplAAFTypeDefRename*>(pDef);
    if (NULL == _cachedRenameTypeDef)
      return AAFRESULT_INVALID_OBJ;
	}
	
	// Validate the property and get the property definition and type definition, 
	// and the actual length of the data
	return (ImplAAFTypeDefIndirect::SetActualPropertyValue (_value, _cachedRenameTypeDef, pValue, valueSize));
}






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

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFConstantValue_h__
#include "ImplAAFConstantValue.h"
#endif

#ifndef __ImplAAFTypeDefIndirect_h__
#include "ImplAAFTypeDefIndirect.h"
#endif


#include <assert.h>
#include <string.h>


ImplAAFConstantValue::ImplAAFConstantValue ()
: _value(			PID_ConstantValue_Value,			L"Value"),
  _cachedTypeDef(NULL)
{
	_persistentProperties.put(_value.address());
}


ImplAAFConstantValue::~ImplAAFConstantValue ()
{  
  if (_cachedTypeDef)
    _cachedTypeDef->ReleaseReference ();
}

  //****************
  // Initialize()
  //
AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstantValue::Initialize (
         ImplAAFParameterDef * pParameterDef,
         aafUInt32  valueSize,
         aafDataBuffer_t  pValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (!pParameterDef || !pValue)
    return (AAFRESULT_NULL_PARAM);

  result = SetParameterDefinition (pParameterDef);
  if (AAFRESULT_SUCCEEDED (result))
    result = SetValue (valueSize, pValue);

  return result;
}

	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstantValue::GetValue (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue,
      aafUInt32*  bytesRead)
{
	if(pValue == NULL || bytesRead == NULL)
		return(AAFRESULT_NULL_PARAM);

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
    ImplAAFConstantValue::GetValueBufLen (
      aafUInt32 *pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

//	*pLen = _value.size();
	*pLen = 0;

	// Validate the property and get the actual length of the data
	return (ImplAAFTypeDefIndirect::GetActualPropertySize (_value, pLen)); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstantValue::SetValue (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue)
{
	if (!pValue)
		return(AAFRESULT_NULL_PARAM);

//	_value.setValue(pValue, valueSize);

  if (!_cachedTypeDef)
  {
    // Lookup the type definition from the Parameter parent class.
    AAFRESULT result = ImplAAFParameter::GetTypeDefinition (&_cachedTypeDef);
    if (AAFRESULT_FAILED (result))
      return result;
  }

  // Validate the property and get the property definition and type definition, 
	// and the actual length of the data
	return (ImplAAFTypeDefIndirect::SetActualPropertyValue (_value, _cachedTypeDef, pValue, valueSize));
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstantValue::GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef)
{
	if (!ppTypeDef)
		return AAFRESULT_NULL_PARAM;

	// Validate the property and get the actual type definition from the
  // indirect value.
	return (ImplAAFTypeDefIndirect::GetActualPropertyType (_value, ppTypeDef));
}




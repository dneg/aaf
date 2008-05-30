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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFControlPoint_h__
#include "ImplAAFControlPoint.h"
#endif

#ifndef __ImplAAFVaryingValue_h__
#include "ImplAAFVaryingValue.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFDefObject.h"
#include "ImplAAFParameterDef.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFHeader.h"
#include "ImplAAFTypeDefIndirect.h"

#include <string.h>
#include "aafErr.h"

ImplAAFControlPoint::ImplAAFControlPoint ()
: _time(			PID_ControlPoint_Time,			L"Time"),
  _value(			PID_ControlPoint_Value,			L"Value"),
  _hint(			PID_ControlPoint_EditHint,		L"EditHint"),
  _initialized(false),
  _cachedTypeDef(NULL)
{
	_persistentProperties.put(_time.address());
	_persistentProperties.put(_value.address());
	_persistentProperties.put(_hint.address());
}


ImplAAFControlPoint::~ImplAAFControlPoint ()
{
  if (_cachedTypeDef)
    _cachedTypeDef->ReleaseReference ();
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::Initialize
    (ImplAAFVaryingValue * pVaryingValue,
     aafRational_constref  time,
     aafUInt32  valueSize,
     aafDataBuffer_t  pValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFTypeDef *pType = NULL;


  if (!pVaryingValue || !pValue)
    return (AAFRESULT_NULL_PARAM);
  if (_initialized)
    return (AAFRESULT_ALREADY_INITIALIZED);

  // First initialize the time.
  result = SetTime (time);
  if (AAFRESULT_SUCCEEDED (result))
  {
    // Lookup the type definition from this constrol point. If it fails
    // then the control point is invalid!
    result = pVaryingValue->GetTypeDefinition (&pType);
    if (AAFRESULT_SUCCEEDED (result))
    {
      _cachedTypeDef = pType;
      _cachedTypeDef->AcquireReference ();

      // Install the initial value.
      result = SetValue (valueSize, pValue);
    }
  }
  
  if (AAFRESULT_SUCCEEDED (result))
  {
    _initialized = true;
  }
  else
  {
    if (_cachedTypeDef)
    {
      _cachedTypeDef->ReleaseReference ();
      _cachedTypeDef = NULL;
    }
  }

  // Cleanup
  if (pType)
    pType->ReleaseReference ();

  return result;
}


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
	if(pEditHint == NULL)
		return AAFRESULT_NULL_PARAM;

	if (!_hint.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pEditHint = _hint;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetValueBufLen (
      aafUInt32 * pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

//	*pLen = _value.size();
	*pLen = 0;

	// Validate the property and get the actual length of the data
	return (ImplAAFTypeDefIndirect::GetActualPropertySize (_value, pLen)); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetValue (
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
	_hint = editHint;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetValue (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue)
{
	if (!pValue)
		return(AAFRESULT_NULL_PARAM);

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


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef)
{
	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	// Validate the property and get the actual type definition from the
  // indirect value.
	return (ImplAAFTypeDefIndirect::GetActualPropertyType (_value, ppTypeDef));
}





/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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

#include <assert.h>
#include <string.h>
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






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
#include "AAFPropertyIds.h"

#ifndef __ImplAAFTaggedValue_h__
#include "ImplAAFTaggedValue.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFDataDef.h"

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
    ImplAAFTaggedValue::Initialize (const aafCharacter * pName,
									ImplAAFTypeDef * pTypeDef)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	if (pName == NULL)
		return AAFRESULT_NULL_PARAM;

	if (! pTypeDef)
	  return AAFRESULT_NULL_PARAM;
	aafUID_t typeDef;
	AAFRESULT hr = pTypeDef->GetAUID(&typeDef);
	if (AAFRESULT_FAILED (hr))return hr;

	_type = typeDef;
	_name = pName;

	return rc;
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
	aafUID_t			defUID;
	ImplAAFDictionary	*dict = NULL;

	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		defUID = _type;
		CHECK(GetDictionary(&dict));
		CHECK(dict->LookupTypeDef(defUID, ppTypeDef));
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEXCEPT
	{
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
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






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
  if (_cachedRenameTypeDef)
    _cachedRenameTypeDef->ReleaseReference ();
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVData::Initialize (
      aafUID_t keyUID, 
      aafUInt32 valueSize, 
      aafDataBuffer_t pValue)
{
	ImplAAFDictionary		*pDict = NULL;
	ImplAAFTypeDef			*pDef = NULL;
	ImplAAFTypeDefRename	*pRenameDef = NULL;
	if (!pValue)
		return AAFRESULT_NULL_PARAM;
	if (_initialized)
		return AAFRESULT_ALREADY_INITIALIZED;
	
	XPROTECT()
	{
	// Save the type so that SetValue will know the type of the value data.
		CHECK(GetDictionary(&pDict));
		CHECK(pDict->LookupOpaqueTypeDef(keyUID, &pDef));
		_cachedRenameTypeDef = dynamic_cast<ImplAAFTypeDefRename*>(pDef);
		assert(_cachedRenameTypeDef);
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
	ImplAAFDictionary	*pDict;
	ImplAAFTypeDef		*pType;
	AAFRESULT			hr;
	aafUID_t			theKey;

	if (pValue == NULL || bytesRead == NULL)
		return AAFRESULT_NULL_PARAM;

	GetKey(&theKey);
	GetDictionary(&pDict);
	hr = pDict->LookupOpaqueTypeDef(theKey, &pType);
	if(pDict)
	  pDict->ReleaseReference();
	pDict = 0;
	if(hr != AAFRESULT_SUCCESS)
		return hr;

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
	ImplAAFTypeDef		*pDef = NULL;
	ImplAAFDictionary	*pDict = NULL;
	aafUID_t			keyUID;

	if (!pValue)
		return AAFRESULT_NULL_PARAM;
	
	if (!_cachedRenameTypeDef)
	{
		// Lookup the type definition from this KLV data. If it fails
		// then the control point is invalid!
		(void)GetDictionary(&pDict);
		(void)GetKey(&keyUID);
		if(pDict->LookupTypeDef(keyUID, &pDef) == AAFRESULT_SUCCESS)
			_cachedRenameTypeDef = dynamic_cast<ImplAAFTypeDefRename*>(pDef);
		pDict->ReleaseReference();
		pDict = 0;
	}
	
	// Validate the property and get the property definition and type definition, 
	// and the actual length of the data
	return (ImplAAFTypeDefIndirect::SetActualPropertyValue (_value, _cachedRenameTypeDef, pValue, valueSize));
}






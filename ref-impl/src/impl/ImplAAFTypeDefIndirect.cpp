/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
*  prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

#ifndef __ImplAAFTypeDefIndirect_h__
#include "ImplAAFTypeDefIndirect.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFTypeDefRename_h__
#include "ImplAAFTypeDefRename.h"
#endif

#ifndef __ImplAAFTypeDefVariableArray_h__
#include "ImplAAFTypeDefVariableArray.h"
#endif

#ifndef __ImplAAFTypeDefFixedArray_h__
#include "ImplAAFTypeDefFixedArray.h"
#endif

#include "ImplAAFObjectCreation.h"

#include "OMAssertions.h"
#include "OMUtilities.h"


#include <assert.h>
#include <string.h>
#include <iostream.h>



#include "AAFStoredObjectIDs.h"

// We should not have to include this declaration in every type definition
// file. A better alternative would be to have a single factory method that
// creates ImplAAFPropValData objects. Possibly a static method of 
// ImplAAFPropValData. (TRR 2000-MAR-03)
extern "C" const aafClassID_t CLSID_AAFPropValData;



//
// Base format for property with an indirect type:
//
// aafUInt16 (2 bytes) 
// aafUID_t (16 bytes)
// aafUInt8[] (array of bytes)
//
// Therefore the minimum valid size is 18 bytes.
//

static void formatError(DWORD errorCode)
{
#if defined(_WIN32) || defined(WIN32)
  char message[256];

  int status = FormatMessageA(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    errorCode,
    LANG_SYSTEM_DEFAULT,
    message, 256,
    NULL);

  if (status != 0) {
    int length = strlen(message);
    // zap cr/lf
    if (length >= 2) {
      message[length - 2] = '\0';
    }
    cerr << "Error text = \"" << message << "\"" << endl;
  } else {
    cerr << "Error code = " << hex << errorCode << dec << endl;
  }
#else
  cerr << "Error code = " << hex << errorCode << dec << endl;
#endif
}

static void checkResult(AAFRESULT resultCode)
{
  TRACE("checkFile");
  ASSERT("Valid program name", validString(getProgramName()));

  if (AAFRESULT_FAILED(resultCode)) {
    formatError(resultCode);
  }
  ASSERT("Succeeded", AAFRESULT_SUCCEEDED(resultCode)); // trr - error
}



//
// Utility to extract information from an "indirect" property.
// Hopefully this is temporary and this routine will be removed
// when there is a specific "indirect" OM property.
//


static AAFRESULT GetPropertyInfo (
	const OMProperty & indirectProperty,
  ImplAAFObject **ppObject, // NOT REFERENCE COUNTED!
	ImplAAFPropertyDef ** ppPropertyDef, // NOT REFERENCE COUNTED!
	ImplAAFTypeDefIndirect ** ppIndirectType, // NOT REFERENCE COUNTED!
	aafUInt32 * pActualValueSize)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
	ImplAAFPropertyDef * pPropertyDef = NULL; // NOT REFERENCE COUNTED!
	ImplAAFTypeDefIndirect * pIndirectType = NULL; // NOT REFERENCE COUNTED!
	aafUInt32 actualValueSize = 0;

  //
  // Get the ImplAAFObject from the given indirect property.
  //
  const OMPropertySet * propertySet = indirectProperty.propertySet ();
  if (!propertySet)
    return AAFRESULT_INVALID_PARAM;
  OMStorable *storable = propertySet->container ();
  if (!storable)
    return AAFRESULT_INVALID_PARAM;
  ImplAAFObject *pObject = dynamic_cast<ImplAAFObject *>(storable);
  if (!pObject)
    return AAFRESULT_INVALID_PARAM;

	// Get the property definition associated with the given property.
  pPropertyDef = const_cast<ImplAAFPropertyDef *>
		                  (dynamic_cast<const ImplAAFPropertyDef *>(indirectProperty.definition()));
	if (!pPropertyDef)
    return AAFRESULT_INVALID_PARAM;

	// The the indirect type definition from the property's definition.
	// The type MUST be an (the) indirect type.
  pIndirectType = const_cast<ImplAAFTypeDefIndirect *>
		                  (dynamic_cast<const ImplAAFTypeDefIndirect *>(pPropertyDef->type()));
	if (!pIndirectType)
    return AAFRESULT_INVALID_PARAM;
  
	// Compute the actual data size.
	//
  actualValueSize = indirectProperty.bitsSize();
	if (0 < actualValueSize)
	{
    actualValueSize -= pIndirectType->GetIndirectValueOverhead ();
    assert (0 <= actualValueSize);
		if (0 > actualValueSize)
			return (AAFRESULT_INVALID_OBJ);
  }

  if (ppObject)
		*ppObject = pObject;
  if (ppPropertyDef)
		*ppPropertyDef = pPropertyDef;
  if (ppIndirectType)
		*ppIndirectType = pIndirectType;
  if (pActualValueSize)
		*pActualValueSize = actualValueSize;

  return (result);
}


AAFRESULT ImplAAFTypeDefIndirect::GetActualPropertySize (
	const OMProperty & indirectProperty,
	aafUInt32 * pActualValueSize)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (!pActualValueSize)
    return (AAFRESULT_NULL_PARAM);

	// Validate the property and get the actual length of the data
  result = ::GetPropertyInfo (indirectProperty, NULL, NULL, NULL, pActualValueSize);
  return result;
}


AAFRESULT ImplAAFTypeDefIndirect::GetActualPropertyValue (
	const OMProperty & indirectProperty,
  aafDataBuffer_t value,
  aafUInt32 valueSize,
	aafUInt32 * bytesRead)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFObject *pObject = NULL; // NOT REFERENCE COUNTED!
	ImplAAFPropertyDef * pPropertyDef = NULL; // NOT REFERENCE COUNTED!
	ImplAAFTypeDefIndirect * pIndirectType = NULL; // NOT REFERENCE COUNTED!
	aafUInt32 actualValueSize = 0;

	if(value == NULL || bytesRead == NULL)
		return(AAFRESULT_NULL_PARAM);

//	if (_value.size() > valueSize)
//	  return AAFRESULT_SMALLBUF;

//	_value.copyToBuffer(pValue, valueSize);
//	*bytesRead  = _value.size();

	*bytesRead = 0;

	// Validate the property and get the property definition and type definition, 
	// and the actual length of the data
	result = ::GetPropertyInfo (indirectProperty, &pObject, &pPropertyDef, &pIndirectType, &actualValueSize);
  if (AAFRESULT_FAILED (result))
		return result;

  // Make sure the value buffer is large enough to hold all of the acutal data
  // within the indirect property.
	if (actualValueSize > valueSize)
	  return AAFRESULT_SMALLBUF;

  
	// Use the "direct access" methods for extracting the actual value from the
	// indirect property.
	ImplAAFPropertyValueSP pIndirectValue;
  result = pObject->GetPropertyValue (pPropertyDef, &pIndirectValue);
  if (AAFRESULT_FAILED (result))
		return result;
	// Copy the data. (NOTE: type def indirect needs a "public" method
	// to extract the actual data directly from the indirect property???)
  result = pIndirectType->GetActualData (pIndirectValue, value, valueSize);
  if (AAFRESULT_FAILED (result))
		return result;

	*bytesRead = valueSize;

	return(result); 
}


AAFRESULT ImplAAFTypeDefIndirect::SetActualPropertyValue (
	const OMProperty & indirectProperty,
  ImplAAFTypeDef *pActualType,
  aafDataBuffer_t value,
  aafUInt32 valueSize)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFObject *pObject = NULL; // NOT REFERENCE COUNTED!
	ImplAAFPropertyDef * pPropertyDef = NULL; // NOT REFERENCE COUNTED!
	ImplAAFTypeDefIndirect * pIndirectType = NULL; // NOT REFERENCE COUNTED!
	aafUInt32 actualValueSize = 0;

	if(!value || !pActualType)
		return(AAFRESULT_NULL_PARAM);

//	_value.setValue(pValue, valueSize);

	// Validate the property and get the property definition and type definition, 
	// and the actual length of the data
	result = ::GetPropertyInfo (indirectProperty, 
             &pObject, &pPropertyDef, &pIndirectType, &actualValueSize);
  if (AAFRESULT_FAILED (result))
		return result;

 	// Use the "direct access" methods for setting the actual value from the
	// indirect property.


	ImplAAFPropertyValueSP pIndirectValue;
  result = pIndirectType->CreateValueFromActualData (pActualType, 
		                                                 value, valueSize, &pIndirectValue);
  if (AAFRESULT_FAILED (result))
		return result;

  result = pObject->SetPropertyValue (pPropertyDef, pIndirectValue);

	return (result); 
}


AAFRESULT ImplAAFTypeDefIndirect::GetActualPropertyType (
	const OMProperty & indirectProperty,
	ImplAAFTypeDef ** ppActualType) // REFERENCE COUNTED!
{
	if(!ppActualType)
		return(AAFRESULT_NULL_PARAM);

  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFObject *pObject = NULL; // NOT REFERENCE COUNTED!
	ImplAAFPropertyDef * pPropertyDef = NULL; // NOT REFERENCE COUNTED!
	ImplAAFTypeDefIndirect * pIndirectType = NULL; // NOT REFERENCE COUNTED!


	// Validate the property and extract the type def from the indirect property.
	result = ::GetPropertyInfo (indirectProperty, &pObject, &pPropertyDef, &pIndirectType, NULL);
  if (AAFRESULT_FAILED (result))
		return result;

	// Use the "direct access" methods for extracting the actual value from the
	// indirect property.
	ImplAAFPropertyValueSP pIndirectValue;
  result = pObject->GetPropertyValue (pPropertyDef, &pIndirectValue);
  if (AAFRESULT_FAILED (result))
		return result;


  //
	// Validate the indirect value and extract common information...
	//
	aafUInt32  indirectValueSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  result = pIndirectType->GetIndirectValueInfo (pIndirectValue, 
             indirectValueSize, pIndirectValueDataBits, ppActualType);

  return result;
}


ImplAAFTypeDefIndirect::ImplAAFTypeDefIndirect () :
  _initialized(false),
  _dictionary(NULL),
  _typeDefAUID(NULL),
  _internalAUIDSize(0),
  _externalAUIDSize(0),
  _internalIndirectSize(0),
  _externalIndirectSize(0)
{}


ImplAAFTypeDefIndirect::~ImplAAFTypeDefIndirect ()
{}


//   Called when we initialize as one of the "builtin" types.
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefIndirect::pvtInitialize
      (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (_initialized)
    return AAFRESULT_ALREADY_INITIALIZED;

  // Initialize the type definition with its unique identifier and name.
  result = SetAUID (id);
  if (AAFRESULT_FAILED(result))
    return result;

  result = SetName (pTypeName);
  if (AAFRESULT_FAILED(result))
    return result;

  
  //
  // Lookup the type definition for auid and cache this reference.
  // 
  ImplAAFDictionary *pDictionary = NULL;
  result = GetDictionary (&pDictionary);
  if (AAFRESULT_SUCCEEDED(result))
  {
    result = pDictionary->LookupTypeDef (kAAFTypeID_AUID, &_typeDefAUID);
    if (AAFRESULT_SUCCEEDED(result))
    {
      // We don't want or need to reference count this definition...
      _typeDefAUID->ReleaseReference();

      // Cache thse values so that we do not have to re-compute them
      // in multiple OMType methods.
      _internalAUIDSize = sizeof(aafUID_t);
      _internalIndirectSize = _internalAUIDSize + sizeof(aafUInt16);
      _externalAUIDSize = _typeDefAUID->PropValSize ();
      _externalIndirectSize = _externalAUIDSize + sizeof(aafUInt16);

      _initialized = true; // 
    }

    // Do NOT reference count the dictionary since it is the container
    // of this type indirect instance.
    if (_initialized)
      _dictionary = pDictionary;

    pDictionary->ReleaseReference();
  }

  return result;
}

// Called internally by the dm because there is NO OM property to hide this
// from the DM.
aafUInt32 ImplAAFTypeDefIndirect::GetIndirectValueOverhead (void) const
{
  assert (_initialized);
  if (_initialized)
    return (_internalIndirectSize);
  else
    return (sizeof(aafUID_t) + sizeof(aafUInt16));
}


// Utility (possibly temporary) that returns true if the given 
// actual type can be used in an indirect type property.
bool ImplAAFTypeDefIndirect::supportedActualType (ImplAAFTypeDef *pActualType, aafUInt32 level)
{
  if (!pActualType)
    return false;

  eAAFTypeCategory_t typeCategory = kAAFTypeCatUnknown;
  AAFRESULT result = pActualType->GetTypeCategory (&typeCategory);
  if (AAFRESULT_FAILED(result))
    return false;

  switch (typeCategory)
  {
    case kAAFTypeCatRename:
    {
      // Make sure that this is actually one of our renamed types.
      ImplAAFTypeDefRename *pRenamedType;
      pRenamedType = dynamic_cast<ImplAAFTypeDefRename *>(pActualType);
      if (!pRenamedType)
        return false;

      ImplAAFTypeDefSP type;
      result = pRenamedType->GetBaseType (&type);
      if (AAFRESULT_FAILED(result))
        return false;

      // Recursively check that the element type is also supported.
      // We need to special case a "rename of a renamed type" etc.
      if (0 == level)
        return supportedActualType (type, 0); // start over.
      else
        return supportedActualType (type, level + 1);
    }

    case kAAFTypeCatFixedArray:
    {
      // Make sure that this is actually one of our fixed array types.
      ImplAAFTypeDefFixedArray *pFixedArrayType;
      pFixedArrayType = dynamic_cast<ImplAAFTypeDefFixedArray *>(pActualType);
      if (!pFixedArrayType)
        return false;

      ImplAAFTypeDefSP type;
      result = pFixedArrayType->GetType (&type);
      if (AAFRESULT_FAILED(result))
        return false;

      // Recursively check that the element type is also supported.
      return supportedActualType (type, level + 1);
    }

    case kAAFTypeCatVariableArray:
    {
      if (0 == level)
      {
        // Make sure that this is actually one of our variable array types.
        ImplAAFTypeDefVariableArray *pVariableArrayType;
        pVariableArrayType = dynamic_cast<ImplAAFTypeDefVariableArray *>(pActualType);
        if (!pVariableArrayType)
          return false;

        ImplAAFTypeDefSP type;
        result = pVariableArrayType->GetType (&type);
        if (AAFRESULT_FAILED(result))
          return false;

        // Recursively check that the element type is also supported.
        return supportedActualType (type, level + 1);
      }
      else
      {
        // The toolkit and the OM so not support variable type inside of another type.
        return false;
      }
    }

    case kAAFTypeCatInt:
    case kAAFTypeCatCharacter:
    case kAAFTypeCatEnum:
    case kAAFTypeCatRecord:
      return true;

    case kAAFTypeCatString:
    {
      if (0 == level)
        return true;
      else
        return false; // strings cannot be embedded in another type.
    }

    default:
      return false;
  }
}


// Find the actual type definition from the dictionary.

AAFRESULT ImplAAFTypeDefIndirect::LookupActualType (
  aafUID_constref actualTypeID, 
  ImplAAFTypeDef ** ppActualType) const
{
  assert (NULL != _dictionary);
  assert (NULL != ppActualType);

  AAFRESULT result = _dictionary->LookupTypeDef (actualTypeID, ppActualType);
  if (AAFRESULT_FAILED(result))
  { 
    // For some reason this method returns AAFRESULT_NO_MORE_OBJECTS
    // when it cannot find (or create) the requested type. This
    // code is supposed to only be returned from IEnumAAFXXXX::NextOne,Next
    // methods. If this is still the case then remap the result code
    // to a more appropriate value.
    if (AAFRESULT_NO_MORE_OBJECTS == result)
      result = AAFRESULT_TYPE_NOT_FOUND;
  }

  return (result);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefIndirect::CreateValueFromActualValue (
      ImplAAFPropertyValue * pActualValue,
      ImplAAFPropertyValue ** ppIndirectValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pActualValue || NULL == ppIndirectValue)
    return AAFRESULT_NULL_PARAM;
  
  // Initialize the return value.
  *ppIndirectValue = NULL;

  
  // Extract the type from the actual value. If this type is
  // registered then this type's id will be written into the
  // new indirect type value (*pPropVal).
  ImplAAFTypeDefSP pActualType;
  result = pActualValue->GetType (&pActualType);
  if (AAFRESULT_FAILED(result))
    return result;

  // Make sure the actual type can be used in an indirect type.
  if (!supportedActualType (pActualType))
    return AAFRESULT_ILLEGAL_VALUE;

  aafUID_t actualTypeID = {0};
  result = pActualType->GetAUID (&actualTypeID);
  if (AAFRESULT_FAILED(result))
    return result;
  else
  {
    ImplAAFTypeDefSP testType;
    result = LookupActualType (actualTypeID, &testType);
    if (AAFRESULT_FAILED(result))
      return result;
  }

  // Get the private value data interface so that we can access the bits.
  ImplAAFPropValData* pActualValueData = dynamic_cast<ImplAAFPropValData*>(pActualValue);
  if (NULL == pActualValueData)
    return AAFRESULT_INTERNAL_ERROR; // AAFRESULT_INVALID_OBJ?

  // Get the size of the actual value.
  aafUInt32 actualInternalSize = 0;
  result = pActualValueData->GetBitsSize (&actualInternalSize);
  if (AAFRESULT_FAILED(result))
    return result;

  // Extract the bits.
  aafMemPtr_t pActualValueDataBits = NULL;
  result = pActualValueData->GetBits (&pActualValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  //
  // Now we are ready to allocate and initialize the indirect value
  // that will contain a copy of the bits from the actaul value.
  //
  ImplAAFPropValDataSP pIndirectValueData;
  pIndirectValueData = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!pIndirectValueData)
    return AAFRESULT_NOMEMORY;
  pIndirectValueData->ReleaseReference(); // make sure reference count is one.

  // Initialize the new property value with this indirect type.
  result = pIndirectValueData->Initialize (this);
  if (AAFRESULT_FAILED(result))
    return result;

  aafMemPtr_t pIndirectValueDataBits = NULL;
  result = pIndirectValueData->AllocateBits (_internalIndirectSize + actualInternalSize,
                                             &pIndirectValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  // Copy the current byte order.
  aafUInt16 originalByteOrder = hostByteOrder();
  copy ((OMByte*)&originalByteOrder, (OMByte*)&pIndirectValueDataBits[0], 
        sizeof(originalByteOrder));

  // Copy the actual type id for this indirect type...
  copy ((OMByte*)&actualTypeID, (OMByte*)&pIndirectValueDataBits[sizeof(originalByteOrder)], _internalAUIDSize);

  // Copy the actual data value bits...
  copy ((OMByte*)pActualValueDataBits, (OMByte*)&pIndirectValueDataBits[_internalIndirectSize], 
        actualInternalSize);
  
  // Return the newly allocated and initialized indirect data value.
  *ppIndirectValue = pIndirectValueData;
  (*ppIndirectValue)->AcquireReference(); // refcount == 2, smartptr will reduce this to one.

  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefIndirect::CreateValueFromActualData (
      ImplAAFTypeDef * pActualType,
      aafMemPtr_t  pInitData,
      aafUInt32  initDataSize,
      ImplAAFPropertyValue ** ppIndirectValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pActualType || NULL == pInitData || NULL == ppIndirectValue)
    return AAFRESULT_NULL_PARAM;
  
  // Initialize the return value.
  *ppIndirectValue = NULL;


  // Make sure the actual type can be used in an indirect type.
  if (!supportedActualType (pActualType))
    return AAFRESULT_INVALID_PARAM;


  // If this type is registered then this type's id will be written into the
  // new indirect type value (*pPropVal).
  aafUID_t actualTypeID = {0};
  result = pActualType->GetAUID (&actualTypeID);
  if (AAFRESULT_FAILED(result))
    return result;
  else
  {
    ImplAAFTypeDefSP testType;
    result = LookupActualType (actualTypeID, &testType);
    if (AAFRESULT_FAILED(result))
      return result;
  }

  // If the actual type if a fixed size than make sure that initDataSize is 
  // correct for the type.
  if (pActualType->IsFixedSize())
  {
    aafUInt32 expectedSize = pActualType->NativeSize();
    if (expectedSize != initDataSize)
      return AAFRESULT_ILLEGAL_VALUE;
  }

  //
  // Now we are ready to allocate and initialize the indirect value
  // that will contain a copy of the bits from the actaul value.
  //
  ImplAAFPropValDataSP pIndirectValueData;
  pIndirectValueData = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!pIndirectValueData)
    return AAFRESULT_NOMEMORY;
  pIndirectValueData->ReleaseReference(); // make sure reference count is one.

  // Initialize the new property value with this indirect type.
  result = pIndirectValueData->Initialize (this);
  if (AAFRESULT_FAILED(result))
    return result;

  aafMemPtr_t pIndirectValueDataBits = NULL;
  result = pIndirectValueData->AllocateBits (_internalIndirectSize + initDataSize,
                                             &pIndirectValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  // Copy the current byte order.
  aafUInt16 originalByteOrder = hostByteOrder();
  copy ((OMByte*)&originalByteOrder, (OMByte*)&pIndirectValueDataBits[0], 
        sizeof(originalByteOrder));

  // Copy the actual type id for this indirect type...
  copy ((OMByte*)&actualTypeID, (OMByte*)&pIndirectValueDataBits[sizeof(originalByteOrder)], _internalAUIDSize);

  // Copy the actual data value bits...
  copy ((OMByte*)pInitData, (OMByte*)&pIndirectValueDataBits[_internalIndirectSize], 
        initDataSize);
  
  // Return the newly allocated and initialized indirect data value.
  *ppIndirectValue = pIndirectValueData;
  (*ppIndirectValue)->AcquireReference(); // refcount == 2, smartptr will reduce this to one.
  

  return result;
}


AAFRESULT ImplAAFTypeDefIndirect::GetIndirectValueInfo (
      ImplAAFPropertyValue * pIndirectValue,
			aafUInt32 & indirectValueSize,
			aafMemPtr_t & pIndirectValueDataBits,
      ImplAAFTypeDef ** ppActualType)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pIndirectValue || NULL == ppActualType)
    return AAFRESULT_NULL_PARAM;


	// Make sure that the input value is infact a valid indirect value.
	ImplAAFTypeDef *pTypeDef = NULL;
	result = pIndirectValue->GetType (&pTypeDef);
	if (AAFRESULT_FAILED (result))
		return result;
	ImplAAFTypeDefIndirect *pValidIndirectType = 
		                          dynamic_cast<ImplAAFTypeDefIndirect*>(pTypeDef);
  pTypeDef->ReleaseReference();
	pTypeDef = NULL;
	if (!pValidIndirectType)
		return AAFRESULT_ILLEGAL_VALUE;

  // Get the private value data interface so that we can access the bits.
  ImplAAFPropValData* pIndirectValueData = dynamic_cast<ImplAAFPropValData*>(pIndirectValue);
  if (NULL == pIndirectValueData)
    return AAFRESULT_INTERNAL_ERROR; // AAFRESULT_INVALID_OBJ?

  // Get the size and the bits from the indirect value.
  result = pIndirectValueData->GetBitsSize (&indirectValueSize);
  if (AAFRESULT_FAILED(result))
    return result;

  // Make sure that the indirect value data is at least large enough for the
  // actual auid and original byte order.
  if (indirectValueSize < _internalIndirectSize)
    return AAFRESULT_ILLEGAL_VALUE;

	// Get the pointer to the indirect value bits.
  result = pIndirectValueData->GetBits (&pIndirectValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  // Extract the actual type id and the original byte order from the indirect
  // value data bits.
  aafUID_t actualTypeID = {0};
  aafUInt16 originalByteOrder = 0;

  // Copy and validate the original byte order.
  copy ((OMByte*)&pIndirectValueDataBits[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  if ((originalByteOrder != littleEndian) && (originalByteOrder != bigEndian))
    return AAFRESULT_ILLEGAL_VALUE;

  // Copy the actual type id for this indirect type. Validate that the id does
  // belong to a type definition in the current dictionary.
  copy ((OMByte*)&pIndirectValueDataBits[sizeof(originalByteOrder)], (OMByte*)&actualTypeID, _internalAUIDSize);

  // Lookup the actual type in the dictionary (use impl smartptrs to hide reference
  // counting...this code should be moved down into the OM anyway...)
  result = LookupActualType (actualTypeID, ppActualType);
  if (AAFRESULT_FAILED(result))
    return result;

  // Make sure the actual type can be used in an indirect type.
  if (!supportedActualType (*ppActualType))
	{
		(*ppActualType)->ReleaseReference();
    return AAFRESULT_ILLEGAL_VALUE;
	}


  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefIndirect::GetActualValue (
      ImplAAFPropertyValue * pIndirectValue,
      ImplAAFPropertyValue ** ppActualValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pIndirectValue || NULL == ppActualValue)
    return AAFRESULT_NULL_PARAM;
  
  // Initialize the return value.
  *ppActualValue = NULL;

  //
	// Validate the indirect value and extract common information...
	//
  ImplAAFTypeDefSP pActualType;
	aafUInt32  indirectValueSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  result = GetIndirectValueInfo (pIndirectValue, indirectValueSize, pIndirectValueDataBits,
                                 &pActualType);
  if (AAFRESULT_FAILED(result))
    return result;

  //
  // Now we are ready to allocate and initialize the actual value
  // that will contain a copy of the bits from remaining bits of the
  // indirect value.
  //
  aafUInt32 actualValueDataSize = indirectValueSize - _internalIndirectSize;

  ImplAAFPropValDataSP pActualValueData;
  pActualValueData = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!pActualValueData)
    return AAFRESULT_NOMEMORY;
  pActualValueData->ReleaseReference(); // make sure reference count is one.

  // Initialize the new property value with the actual type from the indirect value.
  result = pActualValueData->Initialize (pActualType);
  if (AAFRESULT_FAILED(result))
    return result;

  aafMemPtr_t pActualValueDataBits = NULL;
  result = pActualValueData->AllocateBits (actualValueDataSize, &pActualValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

	// Copy the actual value bits from the indirect value to the actual value.
  copy ((OMByte*)&pIndirectValueDataBits[_internalIndirectSize], (OMByte*)pActualValueDataBits,
        actualValueDataSize);
  
  // Return the newly allocated and initialized indirect data value.
  *ppActualValue = pActualValueData;
  (*ppActualValue)->AcquireReference(); // refcount == 2, smartptr will reduce this to one.

  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefIndirect::GetActualSize (
      ImplAAFPropertyValue * pIndirectValue,
      aafUInt32 * pActualSize)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pIndirectValue || NULL == pActualSize)
    return AAFRESULT_NULL_PARAM;

  //
	// Validate the indirect value and extract common information...
	//
  ImplAAFTypeDefSP pActualType;
	aafUInt32  indirectValueSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  result = GetIndirectValueInfo (pIndirectValue, indirectValueSize, pIndirectValueDataBits,
                                 &pActualType);
  if (AAFRESULT_FAILED(result))
    return result;

  // Compute the actual size of the data (minus the internal overhead of the
  // indirect type)
  *pActualSize = indirectValueSize - _internalIndirectSize;
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefIndirect::GetActualType (
      ImplAAFPropertyValue * pIndirectValue,
      ImplAAFTypeDef ** ppActualType)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pIndirectValue || NULL == ppActualType)
    return AAFRESULT_NULL_PARAM;

  //
	// Validate the indirect value and extract common information...
	//
	aafUInt32  indirectValueSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  result = GetIndirectValueInfo (pIndirectValue, indirectValueSize, pIndirectValueDataBits,
                                 ppActualType);
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefIndirect::GetActualData (
      ImplAAFPropertyValue * pIndirectValue,
      aafMemPtr_t  pData,
      aafUInt32  dataSize)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pIndirectValue || NULL == pData)
    return AAFRESULT_NULL_PARAM;

  //
	// Validate the indirect value and extract common information...
	//
  ImplAAFTypeDefSP pActualType;
	aafUInt32  indirectValueSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  result = GetIndirectValueInfo (pIndirectValue, indirectValueSize, pIndirectValueDataBits,
                                 &pActualType);
  if (AAFRESULT_FAILED(result))
    return result;

  //
  // Now we are ready to copy the bits from remaining bits of the
  // indirect value into the given data pointer.
  //
  aafUInt32 actualValueDataSize = indirectValueSize - _internalIndirectSize;

	// Make sure that the given data size is large enough to copy the value
	// data. (Should it be exactly equal to the actual value size?)
	if (dataSize < actualValueDataSize)
    return AAFRESULT_ILLEGAL_VALUE;

	// Copy the actual value bits from the indirect value to the actual value.
  copy ((OMByte*)&pIndirectValueDataBits[_internalIndirectSize], (OMByte*)pData, dataSize);
  
  return result;
}


// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefIndirect::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid)
{
  if (!pTid)
    return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatIndirect;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefIndirect::RawAccessType (ImplAAFTypeDef ** ppRawTypeDef)
{
  // Return variable array of unsigned char
  return pvtGetUInt8Array8Type (ppRawTypeDef);
}





//
// Overrides for OMType
//

void ImplAAFTypeDefIndirect::reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const
{
  TRACE("ImplAAFTypeDefIndirect::reorder");
  PRECONDITION("Object has been initialized", _initialized);
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external byte size", externalBytesSize > 0);
  PRECONDITION("External byte size is large enough for id and byte order", 
               externalBytesSize >= _externalIndirectSize);

  // Get the auid's external size.
  aafUID_t actualTypeID = {0};
  aafUInt16 originalByteOrder = 0;


  // Get the original byte order saved in the property data.
  copy (&externalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));
  
  //
  // We need to read the id out of the externalBytes and be careful
  // to use the correct byte order.
  //
  if (originalByteOrder != hostByteOrder())
  {
    // We need to reorder the bytes first before we can internalize the id.
    _typeDefAUID->reorder (&externalBytes[sizeof(originalByteOrder)], _externalAUIDSize);
  }

  // "read" the actual type id from the externalized indirect value.
  _typeDefAUID->internalize (&externalBytes[sizeof(originalByteOrder)],
						                 _externalAUIDSize,
						                 (OMByte*)&actualTypeID,
						                 sizeof(actualTypeID),
						                 hostByteOrder());
  
  //
  // Lookup the actual type in the dictionary (use impl smartptrs to hide reference
  // counting...this code should be moved down into the OM anyway...)
	//
  ImplAAFTypeDefSP pActualType;
  checkResult (LookupActualType (actualTypeID, &pActualType)); // ASSERT

  if (originalByteOrder != hostByteOrder())
  {
    // reorder the rest of the buffer with actual type.
    aafInt32 actualExternalBytesSize = externalBytesSize - _externalIndirectSize;
    OMByte *actualExternalBytes = &externalBytes[_externalIndirectSize];
    pActualType->reorder (actualExternalBytes, actualExternalBytesSize);
	}
}

size_t ImplAAFTypeDefIndirect::externalSize(OMByte* internalBytes,
                              size_t internalBytesSize) const
{
  TRACE("ImplAAFTypeDefIndirect::externalSize");
  PRECONDITION("Object has been initialized", _initialized);
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal byte size", internalBytesSize > 0);
  PRECONDITION("Internal byte size is large enough for id and byte order", 
               internalBytesSize >= _internalIndirectSize);

  // return value...
  size_t externalBytesSize = 0;


  // Get the auid's internal size.
  aafUID_t actualTypeID = {0};
  aafUInt16 originalByteOrder = 0;



  // Add the external size of the original byte order data.
  externalBytesSize += sizeof(originalByteOrder);

  // Add the external size of auid.
  externalBytesSize += _typeDefAUID->externalSize (internalBytes, _internalAUIDSize);

  // Get the original byte order.
  copy (&internalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));

  // The data has already been internalized (i.e. in native byte order).
  copy (&internalBytes[sizeof(originalByteOrder)], (OMByte*)&actualTypeID, _internalAUIDSize);


  // Lookup the actual type in the dictionary (use impl smartptrs to hide reference
  // counting...this code should be moved down into the OM anyway...)
  ImplAAFTypeDefSP pActualType;
  checkResult (LookupActualType (actualTypeID, &pActualType)); // ASSERT

  // Add the external size of the actual type.
  aafInt32 actualInternalBytesSize = internalBytesSize - _internalIndirectSize;
  OMByte *actualInternalBytes = &internalBytes[_internalIndirectSize];
  externalBytesSize += pActualType->externalSize (actualInternalBytes, actualInternalBytesSize);

  return externalBytesSize;
}

void ImplAAFTypeDefIndirect::externalize(
  OMByte* internalBytes,
  size_t internalBytesSize,
  OMByte* externalBytes,
  size_t externalBytesSize,
  OMByteOrder byteOrder) const
{
  TRACE("ImplAAFTypeDefIndirect::externalize");
  PRECONDITION("Object has been initialized", _initialized);
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal byte size", internalBytesSize > 0);
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external byte size", externalBytesSize > 0);
  PRECONDITION("Internal byte size is large enough for id and byte order", 
               internalBytesSize >= _internalIndirectSize);
  PRECONDITION("External byte size is large enough for id and byte order", 
               externalBytesSize >= _externalIndirectSize);

  // Get the auid's internal size.
  aafUID_t actualTypeID = {0};
  aafUInt16 originalByteOrder = 0;


  // Get the original byte order.
  copy (&internalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));
  
  // The original byte order does not have to be externalized.
  copy ((OMByte*)&originalByteOrder, &externalBytes[0], sizeof(originalByteOrder));
 
  // The data has already been internalized (i.e. in native byte order).
  copy (&internalBytes[sizeof(originalByteOrder)], (OMByte*)&actualTypeID, _internalAUIDSize);
  
  // Write the external data for the auid.
  _typeDefAUID->externalize (&internalBytes[sizeof(originalByteOrder)], _internalAUIDSize, 
                             &externalBytes[sizeof(originalByteOrder)], _externalAUIDSize, originalByteOrder);


  // Lookup the actual type in the dictionary (use impl smartptrs to hide reference
  // counting...this code should be moved down into the OM anyway...)
  ImplAAFTypeDefSP pActualType;
  checkResult (LookupActualType (actualTypeID, &pActualType)); // ASSERT


  // Externalize the rest of the data using the actual type.
  aafInt32 actualInternalBytesSize = internalBytesSize - _internalIndirectSize;
  aafInt32 actualExternalBytesSize = externalBytesSize - _externalIndirectSize;
  OMByte *actualInternalBytes = &internalBytes[_internalIndirectSize];
  OMByte *actualExternalBytes = &externalBytes[_externalIndirectSize];
  pActualType->externalize (actualInternalBytes, actualInternalBytesSize, 
                            actualExternalBytes, actualExternalBytesSize, originalByteOrder);
}


size_t ImplAAFTypeDefIndirect::internalSize(
  OMByte* externalBytes,
  size_t externalSize) const
{
  TRACE("ImplAAFTypeDefIndirect::internalSize");
  PRECONDITION("Object has been initialized", _initialized);
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external size", externalSize > 0);
  PRECONDITION("External byte size is large enough for id and byte order", 
               externalSize >= _externalIndirectSize);

  // return value...
  size_t internalBytesSize = 0;


  // Add the external size of the original byte order data.
  aafUInt16 originalByteOrder = 0;
  internalBytesSize += sizeof(originalByteOrder);

  // Add the external size of auid.
  internalBytesSize += _internalAUIDSize;

  // 
  // Get the original byte order. (this is redundent since reorder should
  // have already been called)
  //
  copy (&externalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));


  //
  // Get the actual type id from the external bytes.
  // The data has already been reorder (i.e. in native byte order but may need padding).
  //
  aafUID_t actualTypeID = {0};
  _typeDefAUID->internalize (&externalBytes[sizeof(originalByteOrder)], _externalAUIDSize,
                             (OMByte*)&actualTypeID, _internalAUIDSize, hostByteOrder());
  
  //  
  // Lookup the actual type in the dictionary (use impl smartptrs to hide reference
  // counting...this code should be moved down into the OM anyway...)
  //
  ImplAAFTypeDefSP pActualType;
  checkResult (LookupActualType (actualTypeID, &pActualType)); // ASSERT

  // reorder the rest of the buffer with actual type.
  aafInt32 actualExternalBytesSize = externalSize - _externalIndirectSize;
  OMByte *actualExternalBytes = &externalBytes[_externalIndirectSize];
  internalBytesSize += pActualType->internalSize (actualExternalBytes, 
                                                  actualExternalBytesSize);

  return internalBytesSize;
}

void ImplAAFTypeDefIndirect::internalize(
  OMByte* externalBytes,
  size_t externalBytesSize,
  OMByte* internalBytes,
  size_t internalBytesSize,
  OMByteOrder byteOrder) const
{
  TRACE("ImplAAFTypeDefIndirect::internalize");
  PRECONDITION("Object has been initialized", _initialized);
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external byte size", externalBytesSize > 0);
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal byte size", internalBytesSize > 0);
  PRECONDITION("Internal byte size is large enough for id and byte order", 
               internalBytesSize >= _internalIndirectSize);
  PRECONDITION("External byte size is large enough for id and byte order", 
               externalBytesSize >= _externalIndirectSize);
  PRECONDITION("internalize byte order should always be the the host byte order",
		           byteOrder == hostByteOrder ()); // why do we need this argument?


  // Get the auid's internal size.

  

  // Get the original byte order.
  aafUInt16 originalByteOrder = 0;
  copy (&externalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));
  
  // Internalize the actual typed id.
  _typeDefAUID->internalize (&externalBytes[sizeof(originalByteOrder)], _externalAUIDSize,
                             &internalBytes[sizeof(originalByteOrder)], _internalAUIDSize, byteOrder);

  // The data has already been internalized.
  aafUID_t actualTypeID = {0};
  copy (&internalBytes[sizeof(originalByteOrder)], (OMByte*)&actualTypeID, _internalAUIDSize);
  // Just copy the original byte order.
  copy ((OMByte*)&originalByteOrder, &internalBytes[0], sizeof(originalByteOrder));


  //
  // Lookup the actual type in the dictionary (use impl smartptrs to hide reference
  // counting...this code should be moved down into the OM anyway...)
  //
  ImplAAFTypeDefSP pActualType;
  checkResult (LookupActualType (actualTypeID, &pActualType)); // ASSERT

  // Externalize the rest of the data using the actual type.
  aafInt32 actualInternalBytesSize = internalBytesSize - _internalIndirectSize;
  aafInt32 actualExternalBytesSize = externalBytesSize - _externalIndirectSize;
  OMByte *actualInternalBytes = &internalBytes[_internalIndirectSize];
  OMByte *actualExternalBytes = &externalBytes[_externalIndirectSize];
  pActualType->internalize (actualExternalBytes, actualExternalBytesSize, 
                            actualInternalBytes, actualInternalBytesSize, byteOrder);
}











//
// Overrides of ImplAAFTypeDef
//
//
// Same return values as ImplAAFTypeDefVariableArray.
//
// Override from AAFTypeDef

aafBool ImplAAFTypeDefIndirect::IsFixedSize (void) const
{
  return kAAFFalse;
}


size_t ImplAAFTypeDefIndirect::PropValSize (void) const
{
  assert (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDefIndirect::IsRegistered (void) const
{
  // Only depends on registration of basic type.
  return kAAFTrue;
}


size_t ImplAAFTypeDefIndirect::NativeSize (void) const
{
  assert (0);
  return 0; // not reached!
}


OMProperty * ImplAAFTypeDefIndirect::pvtCreateOMPropertyMBS
  (OMPropertyId pid,
   const char * name) const
{
  assert (name);

  OMProperty * result = 0;

  // We do not currently (TRR 2000-MAR-02) have a specific property
  // for indirect data so we just return a property for a variable
  // sized array of unsigned bytes.
	result = new OMVariableSizeProperty<aafUInt8> (pid, name);

  assert (result); // need better error handling!
  return result;
}

bool ImplAAFTypeDefIndirect::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefIndirect::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefIndirect::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefIndirect::IsVariableArrayable () const
{ return false; }

bool ImplAAFTypeDefIndirect::IsStringable () const
{ return false; }



//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

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
// OMByteOrder (1 byte) 
// aafUID_t (16 bytes)
// aafUInt8[] (array of bytes)
//
// Therefore the minimum valid size is 18 bytes.
//

static void formatError(DWORD errorCode)
{
#if defined( OS_WINDOWS )
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

#endif  // OS_WINDOWS
}

static void checkResult(AAFRESULT resultCode)
{
  TRACE("checkFile");

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
	    aafUInt32	valueOverhead = pIndirectType->GetIndirectValueOverhead ();
	    assert (actualValueSize >= valueOverhead);
	    if (actualValueSize < valueOverhead)
		return (AAFRESULT_INVALID_OBJ);
	    actualValueSize -= valueOverhead;
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
  ImplAAFObject *pObject = NULL; // NOT REFERENCE COUNTED!
	ImplAAFPropertyDef * pPropertyDef = NULL; // NOT REFERENCE COUNTED!
	ImplAAFTypeDefIndirect * pIndirectType = NULL; // NOT REFERENCE COUNTED!
	aafUInt32 actualValueSize = 0;
  if (!pActualValueSize)
    return (AAFRESULT_NULL_PARAM);


	// Validate the property and get the property definition and type definition, 
	// and the actual length of the data
	result = ::GetPropertyInfo (indirectProperty, &pObject, &pPropertyDef, &pIndirectType, &actualValueSize);
  if (AAFRESULT_FAILED (result))
		return result;

  
	// Use the "direct access" methods for extracting the actual value from the
	// indirect property.
	ImplAAFPropertyValueSP pIndirectValue;
  result = pObject->GetPropertyValue (pPropertyDef, &pIndirectValue);
  if (AAFRESULT_FAILED (result))
		return result;
  result = pIndirectType->GetActualSize (pIndirectValue, pActualValueSize);
	return(result); 
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


AAFRESULT ImplAAFTypeDefIndirect::GetActualPropertyTypeID (
	const OMProperty & indirectProperty,
	aafUID_t * pTypeID)
{
	if(!pTypeID)
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
             indirectValueSize, pIndirectValueDataBits, NULL, NULL, pTypeID);

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
AAFRESULT
  ImplAAFTypeDefIndirect::Initialize
      (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (_initialized)
    return AAFRESULT_ALREADY_INITIALIZED;

  // Initialize the type definition with its unique identifier and name.

  result = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (result))
    return result;

  
  //
  // Lookup the type definition for auid and cache this reference.
  // 
  ImplAAFDictionarySP pDictionary;
  result = GetDictionary (&pDictionary);
  if (AAFRESULT_FAILED (result))
    return result;

  ImplAAFTypeDefSP pTypeDefAUID;
  result = pDictionary->LookupTypeDef(kAAFTypeID_AUID, &pTypeDefAUID);
  if (AAFRESULT_FAILED (result))
    return result;

  result = pvtInitialize(id, pTypeName, pTypeDefAUID, pDictionary);

  return result;
}

//   Called when we initialize as one of the "builtin" types.
//
AAFRESULT
  ImplAAFTypeDefIndirect::pvtInitialize
      (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName,
       
       // @parm [in] the type definition for kAAFTypeID_AUID.
       ImplAAFTypeDef *pTypeDefAUID,

       // @parm [in] the dictionary for this instance
       ImplAAFDictionary *pDictionary)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (_initialized)
    return AAFRESULT_ALREADY_INITIALIZED;
  if (!pTypeName)
    return AAFRESULT_NULL_PARAM;
  if (!pTypeDefAUID)
    return AAFRESULT_NULL_PARAM;
  if (!pDictionary)
    return AAFRESULT_NULL_PARAM;


  // Make sure the given type is for an AUID.
  aafUID_t auid;
  result = pTypeDefAUID->GetAUID(&auid);
  if (AAFRESULT_FAILED (result))
    return result;
  if (memcmp(&kAAFTypeID_AUID, &auid, sizeof(aafUID_t)))
    return AAFRESULT_INVALID_PARAM;

  // Initialize the type definition with its unique identifier and name.
  result = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
  if (AAFRESULT_FAILED (result))
    return result;

  // Do NOT reference count the dictionary since it is the container
  // of this type indirect instance.
  _dictionary = pDictionary;
  
  // We don't want or need to reference count this definition...
  _typeDefAUID = pTypeDefAUID;

  // Cache thse values so that we do not have to re-compute them
  // in multiple OMType methods.
  _internalAUIDSize = sizeof(aafUID_t);
  _internalIndirectSize = _internalAUIDSize + sizeof(OMByteOrder);
  _externalAUIDSize = _typeDefAUID->PropValSize ();
  _externalIndirectSize = _externalAUIDSize + sizeof(OMByteOrder);

  _initialized = true; // 

  return result;
}


// Called internally by the dm because there is NO OM property to hide this
// from the DM.
aafUInt32 ImplAAFTypeDefIndirect::GetIndirectValueOverhead (void) const
{
  assert (_initialized);
  if (_initialized)
    return (_externalIndirectSize);
  else
    return (sizeof(aafUID_t) + sizeof(OMByteOrder));
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

  // Get the external size of the actual value.
  aafUInt32 actualExternalSize = 0;
	actualExternalSize = pActualType->externalSize((OMByte *)pActualValueDataBits, actualInternalSize);

  aafMemPtr_t pIndirectValueDataBits = NULL;
  result = pIndirectValueData->AllocateBits (_externalIndirectSize + actualExternalSize,
                                             &pIndirectValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  // Copy the current byte order.
  OMByteOrder originalByteOrder = hostByteOrder();
  copy ((OMByte*)&originalByteOrder, (OMByte*)&pIndirectValueDataBits[0], 
        sizeof(originalByteOrder));

	// Write the actual type id into the indirect value in its external form.
  _typeDefAUID->externalize((OMByte*)&actualTypeID, 
                            _internalAUIDSize,
                            (OMByte*)&pIndirectValueDataBits[sizeof(originalByteOrder)],
                            _externalAUIDSize,
                            originalByteOrder);

	// Write the actual value into the indirect value in its external form.
	pActualType->externalize((OMByte *)pActualValueDataBits,
                           actualInternalSize,
                           (OMByte*)&pIndirectValueDataBits[_externalIndirectSize],
                           actualExternalSize,
                           originalByteOrder);

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

  // Any padding and offsets must be registered to use this method.
  if (!pActualType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;

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

  // Get the external size of the actual value.
  aafUInt32 actualExternalSize = 0;
	actualExternalSize = pActualType->externalSize((OMByte *)pInitData, initDataSize);

  aafMemPtr_t pIndirectValueDataBits = NULL;
  result = pIndirectValueData->AllocateBits (_externalIndirectSize + actualExternalSize,
                                             &pIndirectValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  // Copy the current byte order.
  OMByteOrder originalByteOrder = hostByteOrder();
  copy ((OMByte*)&originalByteOrder, (OMByte*)&pIndirectValueDataBits[0], 
        sizeof(originalByteOrder));

	// Write the actual type id into the indirect value in its external form.
  _typeDefAUID->externalize((OMByte*)&actualTypeID, 
                            _internalAUIDSize,
                            (OMByte*)&pIndirectValueDataBits[sizeof(originalByteOrder)],
                            _externalAUIDSize,
                            originalByteOrder);

	// Write the actual value into the indirect value in its external form.
	pActualType->externalize((OMByte *)pInitData,
                           initDataSize,
                           (OMByte*)&pIndirectValueDataBits[_externalIndirectSize],
                           actualExternalSize,
                           originalByteOrder);

  // Return the newly allocated and initialized indirect data value.
  *ppIndirectValue = pIndirectValueData;
  (*ppIndirectValue)->AcquireReference(); // refcount == 2, smartptr will reduce this to one.
  

  return result;
}


AAFRESULT ImplAAFTypeDefIndirect::GetIndirectValueInfo (
      ImplAAFPropertyValue * pIndirectValue,
			aafUInt32 & indirectValueSize,
			aafMemPtr_t & pIndirectValueDataBits,
      ImplAAFTypeDef ** ppActualType,
			aafUInt32 * actualValueSize,
      aafUID_t *typeID)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pIndirectValue)
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
  if (indirectValueSize < _externalIndirectSize)
    return AAFRESULT_ILLEGAL_VALUE;

	// Get the pointer to the indirect value bits.
  result = pIndirectValueData->GetBits (&pIndirectValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  // Extract the actual type id and the original byte order from the indirect
  // value data bits.
  aafUID_t actualTypeID = {0};
  OMByteOrder originalByteOrder = 0;

  // Copy and validate the original byte order.
  copy ((OMByte*)&pIndirectValueDataBits[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  if ((originalByteOrder != littleEndian) && (originalByteOrder != bigEndian))
    return AAFRESULT_ILLEGAL_VALUE;

  if (typeID || ppActualType)
  {
		// First make sure that the actual type id is in the correct byte order.
		if (originalByteOrder != hostByteOrder())
		  _typeDefAUID->reorder((OMByte*)&pIndirectValueDataBits[sizeof(originalByteOrder)], _externalAUIDSize);

    // "read" the actual type id from the externalized indirect value.
    _typeDefAUID->internalize ((OMByte*)&pIndirectValueDataBits[sizeof(originalByteOrder)],
						                   _externalAUIDSize,
						                   (OMByte*)&actualTypeID,
						                   sizeof(actualTypeID),
						                   hostByteOrder());

		// Restore the original byte order (we may be able to optimize here...)
		if (originalByteOrder != hostByteOrder())
		  _typeDefAUID->reorder((OMByte*)&pIndirectValueDataBits[sizeof(originalByteOrder)], _externalAUIDSize);

    // Return the actual type id.
    if (typeID)
      *typeID = actualTypeID;
  }


  if (NULL != ppActualType)
  {
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
  }


	if (actualValueSize)
	{
    if (!ppActualType)
      return AAFRESULT_NULL_PARAM;

		aafUInt32 actualExternalBytesSize = indirectValueSize - _externalIndirectSize;
		OMByte *actualExternalBytes = (OMByte*)&pIndirectValueDataBits[_externalIndirectSize];

		// First make sure that the actual data is in the correct byte order.
		if (originalByteOrder != hostByteOrder())
			(*ppActualType)->reorder(actualExternalBytes, actualExternalBytesSize);

		// Ask the type to compute the internal size of the actual data.
		*actualValueSize = (*ppActualType)->internalSize(actualExternalBytes, actualExternalBytesSize);

		// Restore the original byte order (we may be able to optimize here...)
		if (originalByteOrder != hostByteOrder())
			(*ppActualType)->reorder(actualExternalBytes, actualExternalBytesSize);
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
  aafUInt32 actualValueDataSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  result = GetIndirectValueInfo (pIndirectValue, indirectValueSize, pIndirectValueDataBits,
                                 &pActualType, &actualValueDataSize);
  if (AAFRESULT_FAILED(result))
    return result;

  //
  // Now we are ready to allocate and initialize the actual value
  // that will contain a copy of the bits from remaining bits of the
  // indirect value.
  //

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

	OMByteOrder originalByteOrder = 0;
	copy ((OMByte*)&pIndirectValueDataBits[0], (OMByte *)&originalByteOrder, sizeof(originalByteOrder));

	aafUInt32 actualExternalBytesSize = indirectValueSize - _externalIndirectSize;
	OMByte *actualExternalBytes = (OMByte*)&pIndirectValueDataBits[_externalIndirectSize];

	// First make sure that the actual data is in the correct byte order.
	if (originalByteOrder != hostByteOrder())
		pActualType->reorder(actualExternalBytes, actualExternalBytesSize);

	// Ask the type to internalize into the actual value bytes.
	pActualType->internalize(actualExternalBytes,
                           actualExternalBytesSize,
                           (OMByte *)pActualValueDataBits,
                           actualValueDataSize,
                           hostByteOrder());

	// Restore the original byte order (we may be able to optimize here...)
	if (originalByteOrder != hostByteOrder())
		pActualType->reorder(actualExternalBytes, actualExternalBytesSize);

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
                                 &pActualType, pActualSize);
  if (AAFRESULT_FAILED(result))
    return result;

  
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
  aafUInt32 actualValueDataSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  result = GetIndirectValueInfo (pIndirectValue, indirectValueSize, pIndirectValueDataBits,
                                 &pActualType, &actualValueDataSize);
  if (AAFRESULT_FAILED(result))
    return result;


  // Any padding and offsets must be registered to use this method.
  if (!pActualType->IsRegistered())
    return AAFRESULT_NOT_REGISTERED;

  //
  // Now we are ready to copy the bits from remaining bits of the
  // indirect value into the given data pointer.
  //

	// Make sure that the given data size is large enough to copy the value
	// data. (Should it be exactly equal to the actual value size?)
	if (dataSize < actualValueDataSize)
    return AAFRESULT_ILLEGAL_VALUE;

	OMByteOrder originalByteOrder = 0;
	copy ((OMByte*)&pIndirectValueDataBits[0], (OMByte *)&originalByteOrder, sizeof(originalByteOrder));

	aafUInt32 actualExternalBytesSize = indirectValueSize - _externalIndirectSize;
	OMByte *actualExternalBytes = (OMByte*)&pIndirectValueDataBits[_externalIndirectSize];

	// First make sure that the actual data is in the correct byte order.
	if (originalByteOrder != hostByteOrder())
		pActualType->reorder(actualExternalBytes, actualExternalBytesSize);

	// Ask the type to internalize into the actual value bytes.
	pActualType->internalize(actualExternalBytes,
                           actualExternalBytesSize,
                           (OMByte *)pData,
                           actualValueDataSize,
                           hostByteOrder());

	// Restore the original byte order (we may be able to optimize here...)
	if (originalByteOrder != hostByteOrder())
		pActualType->reorder(actualExternalBytes, actualExternalBytesSize);
  
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
                       size_t ANAME(externalBytesSize)) const
{
  TRACE("ImplAAFTypeDefIndirect::reorder");
  PRECONDITION("Object has been initialized", _initialized);
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external byte size", externalBytesSize > 0);
  PRECONDITION("External byte size is large enough for id and byte order", 
               externalBytesSize >= _externalIndirectSize);


  //
  // Validate the fields in the property data.
  //
  OMByteOrder originalByteOrder = 0;
  copy (&externalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));
  
  //
  // Reordering of the data is defered until the data is actually 
  // read through an API method.
  // 
}

size_t ImplAAFTypeDefIndirect::externalSize(const OMByte* internalBytes,
                              size_t internalBytesSize) const
{
  TRACE("ImplAAFTypeDefIndirect::externalSize");
  PRECONDITION("Object has been initialized", _initialized);
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal byte size", internalBytesSize > 0);
  PRECONDITION("Internal byte size is large enough for id and byte order", 
               internalBytesSize >= _internalIndirectSize);


  //
  // Validate the fields in the property data.
  //
  OMByteOrder originalByteOrder = 0;
  copy (&internalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));

  //
  // The internal size is the same as the external size.
  // The data is only internalized when the data is requested
  // through the client API.
  //
  return internalBytesSize;
}

void ImplAAFTypeDefIndirect::externalize(
  const OMByte* internalBytes,
  size_t ANAME(internalBytesSize),
  OMByte* externalBytes,
  size_t externalBytesSize,
  OMByteOrder NNAME(byteOrder)) const
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
  PRECONDITION("External byte size is the same as the internal byte size", 
               externalBytesSize == internalBytesSize);


  //
  // Validate the fields in the property data.
  //
  OMByteOrder originalByteOrder = 0;
  copy (&internalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));

  //
  // The internal form of this data is the same as its external form.
  // The data is only internalized when the it is requested
  // through the client API. Therefore we can just copy the data.
  //
  ASSERT("External byte size is the same as the internal byte size", 
         externalBytesSize == internalBytesSize);
  copy (internalBytes, externalBytes, externalBytesSize);
}


size_t ImplAAFTypeDefIndirect::internalSize(
  const OMByte* externalBytes,
  size_t externalSize) const
{
  TRACE("ImplAAFTypeDefIndirect::internalSize");
  PRECONDITION("Object has been initialized", _initialized);
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external size", externalSize > 0);
  PRECONDITION("External byte size is large enough for id and byte order", 
               externalSize >= _externalIndirectSize);


  //
  // Validate the fields in the property data.
  //
  OMByteOrder originalByteOrder = 0;
  copy (&externalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));

  //
  // The internal size is the same as the external size.
  // The data is only internalized when the data is requested
  // through the client API.
  //
  return externalSize;
}

void ImplAAFTypeDefIndirect::internalize(
  const OMByte* externalBytes,
  size_t externalBytesSize,
  OMByte* internalBytes,
  size_t ANAME(internalBytesSize),
  OMByteOrder ANAME(byteOrder)) const
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


  //
  // Validate the fields in the property data.
  //
  OMByteOrder originalByteOrder = 0;
  copy (&externalBytes[0], (OMByte*)&originalByteOrder, sizeof(originalByteOrder));
  ASSERT("Valid byte order",
         (originalByteOrder == littleEndian) || (originalByteOrder == bigEndian));

  //
  // The internal form of this data is the same as its external form.
  // The data is only internalized when the it is requested
  // through the client API. Therefore we can just copy the data.
  //
  ASSERT("External byte size is the same as the internal byte size", 
         externalBytesSize == internalBytesSize);
  copy (externalBytes, internalBytes, externalBytesSize);
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


OMProperty * ImplAAFTypeDefIndirect::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
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






// override from OMStorable.
const OMClassId& ImplAAFTypeDefIndirect::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefIndirect));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefIndirect::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefIndirect::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}

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

#ifndef __ImplAAFTypeDefOpaque_h__
#include "ImplAAFTypeDefOpaque.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif


#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "ImplAAFObjectCreation.h"


#include <assert.h>
#include <string.h>


// We should not have to include this declaration in every type definition
// file. A better alternative would be to have a single factory method that
// creates ImplAAFPropValData objects. Possibly a static method of 
// ImplAAFPropValData. (TRR 2000-MAR-03)
extern "C" const aafClassID_t CLSID_AAFPropValData;


// The current version of the opaque handle data.
const OMVersion kAAFCurrentOpaqueHandleVersion = 1;
const size_t kAAFOpaqueHandleSignatureSize = 4;
const aafUInt8 kAAFOpaqueHandleSignature[] = { 'O', 'P', 'A', 'Q' };
const size_t kAAFOpaqueHandleOverhead = sizeof(OMVersion) + (2 * kAAFOpaqueHandleSignatureSize);


//
// Utility to make sure that the given property value is for an opaque type.
//
AAFRESULT ImplAAFTypeDefOpaque::ValidateOpaquePropertyValue(ImplAAFPropertyValue * pOpaquePropertyValue)
{
  //
	// Make sure that the input value is infact a valid indirect value.
  //
	ImplAAFTypeDef *pTypeDef = NULL;
	AAFRESULT result = pOpaquePropertyValue->GetType (&pTypeDef);
	if (AAFRESULT_FAILED (result))
		return result;
	ImplAAFTypeDefOpaque *pValidOpaqueType = 
		                          dynamic_cast<ImplAAFTypeDefOpaque*>(pTypeDef);
  pTypeDef->ReleaseReference();
	pTypeDef = NULL;
	if (!pValidOpaqueType)
		return AAFRESULT_ILLEGAL_VALUE;

  return AAFRESULT_SUCCESS;
}


//
// Utility to make sure that the given property value is for an opaque type.
//
AAFRESULT ImplAAFTypeDefOpaque::GetOpaqueHandleInfo(aafUInt32 handleSize, aafDataBuffer_t pHandle,
                                                    aafUInt32& opaqueDataSize, aafDataBuffer_t& opaqueDataBits)
{
  aafUInt32 offset;
  //
  // Check for the supported version.
  //
  if (handleSize < sizeof(OMVersion))
    return AAFRESULT_INVALID_PARAM; // not even large enough for the version.

  OMVersion handleVersion = 0;
  copy ((OMByte *)pHandle, (OMByte *)&handleVersion, sizeof(OMVersion));

  if (kAAFCurrentOpaqueHandleVersion == handleVersion)
  {
    // Check that the opaque handle is large enough for this version.
    if (handleSize < kAAFOpaqueHandleOverhead + GetIndirectValueOverhead())
      return AAFRESULT_INVALID_PARAM;

    // Check for the opaque signature at the beginning of the handle after
    // the version.
    offset = sizeof(OMVersion);
    if (0 != memcmp(&pHandle[offset], &kAAFOpaqueHandleSignature[0], kAAFOpaqueHandleSignatureSize))  
      return AAFRESULT_INVALID_PARAM;

    // Check for the opaque signature at the end of the handle
    // the version.
    offset = handleSize - kAAFOpaqueHandleSignatureSize;
    if (0 != memcmp(&pHandle[offset], &kAAFOpaqueHandleSignature[0], kAAFOpaqueHandleSignatureSize))  
      return AAFRESULT_INVALID_PARAM;

    //
    // Return the opaque data size and the pointer to the begining of the opaque data.
    //
    opaqueDataSize = handleSize - kAAFOpaqueHandleOverhead;
    offset = sizeof(OMVersion) + kAAFOpaqueHandleSignatureSize;
    opaqueDataBits = &pHandle[offset];
  }
  else
  {
    // not a recognized version.
    return AAFRESULT_INVALID_PARAM;
  }




  return AAFRESULT_SUCCESS;
}




ImplAAFTypeDefOpaque::ImplAAFTypeDefOpaque ()
{}


ImplAAFTypeDefOpaque::~ImplAAFTypeDefOpaque ()
{}

//   Called when we initialize as one of the "builtin" types.
//
AAFRESULT
  ImplAAFTypeDefOpaque::Initialize
      (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName)
{
  return ImplAAFTypeDefIndirect::Initialize(id, pTypeName);
}

//   Called when we initialize as one of the "builtin" types.
//
AAFRESULT
  ImplAAFTypeDefOpaque::pvtInitialize
      (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName,
       
       // @parm [in] the type definition for kAAFTypeID_AUID.
       ImplAAFTypeDef *pTypeDefAUID,

       // @parm [in] the dictionary for this instance
       ImplAAFDictionary *pDictionary)
{
  return ImplAAFTypeDefIndirect::pvtInitialize(id, pTypeName, pTypeDefAUID, pDictionary);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefOpaque::GetActualTypeID (
      ImplAAFPropertyValue * pOpaquePropertyValue,
      aafUID_t *  pActualTypeID)
{
  //
	// Validate the indirect value and extract common information...
	//
	aafUInt32  indirectValueSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  return (GetIndirectValueInfo (pOpaquePropertyValue, indirectValueSize, pIndirectValueDataBits,
                                NULL, NULL, pActualTypeID));
}


//***********************************************************
//
// GetHandle()
//
// Used to read and save an unknown property without having to know its contents.
// This call fills in a handle to a block of data which can be saved in you application, and
// later written to another file using SetHandle().  Writes the data into the pHandle buffer. 
// The buffer is allocated by the caller, and the size of the buffer is given by
// handleSize.
// 
// Caller may call GetHandleBufLen() to determine the
// required buffer size.
// 
// Succeeds if all of the following are true:
// - the pHandle pointer is valid.
// - handleSize indicates the buffer is large enough to hold the
//   name.
// 
// If this method fails nothing will be written to *pHandle.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pHandle arg is NULL.
//
// AAFRESULT_SMALLBUF
//   - handleSize indicates the buffer is too small to hold the
//     handle.
// 
AAFRESULT ImplAAFTypeDefOpaque::GetHandle (
    // value to get data from.
    ImplAAFPropertyValue * pOpaquePropertyValue,

    // Size of preallocated buffer
    aafUInt32  handleSize,

    // Preallocated buffer to hold handle
    aafDataBuffer_t  pHandle,

    // Number of actual bytes read
    aafUInt32*  bytesRead)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (!pOpaquePropertyValue || !pHandle || !bytesRead)
    return (AAFRESULT_NULL_PARAM);

  result = ValidateOpaquePropertyValue(pOpaquePropertyValue);
	if (AAFRESULT_FAILED (result))
		return result;


  // Get the private value data interface so that we can access the bits.
  ImplAAFPropValData* pOpaqueValueData = dynamic_cast<ImplAAFPropValData*>(pOpaquePropertyValue);
  assert (NULL != pOpaqueValueData);
  if (NULL == pOpaqueValueData)
    return AAFRESULT_INTERNAL_ERROR; // AAFRESULT_INVALID_OBJ?

  // Get the size and the bits from the opaque value.
  aafUInt32 opaqueValueDataSize = 0;
  result = pOpaqueValueData->GetBitsSize (&opaqueValueDataSize);
  if (AAFRESULT_FAILED(result))
    return result;

  // Make sure that the input size if big enough.
  if (handleSize < opaqueValueDataSize)
    return AAFRESULT_SMALLBUF;

	// Get the pointer to the opaque value bits.
  aafMemPtr_t pOpaqueValueDataBits = NULL;
  result = pOpaqueValueData->GetBits (&pOpaqueValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  //
  // Copy our opaque handle version
  // 
  aafUInt32 bytesCopied = 0;
  pHandle[bytesCopied++] = kAAFCurrentOpaqueHandleVersion;

  //
  // Copy the opaque signature before the opaque data.
  //
  copy ((OMByte *)kAAFOpaqueHandleSignature, (OMByte *)&pHandle[bytesCopied], sizeof(kAAFOpaqueHandleSignature));
  bytesCopied += kAAFOpaqueHandleSignatureSize;

  //
  // Copy the opaque data.
  //
  copy ((OMByte *)pOpaqueValueDataBits, (OMByte *)&pHandle[bytesCopied], opaqueValueDataSize);
  bytesCopied += opaqueValueDataSize;

  //
  // Copy the opaque signature again after the opaque data.
  //
  copy ((OMByte *)kAAFOpaqueHandleSignature, (OMByte *)&pHandle[bytesCopied], sizeof(kAAFOpaqueHandleSignature));
  bytesCopied += kAAFOpaqueHandleSignatureSize;

  // Return the actual number of bytes copied to the handle.
  *bytesRead = bytesCopied;

	return (result);
}


//***********************************************************
//
// GetHandleBufLen()
//
// Returns the length of buffer required for the GetHandle()
// method.  The handle is placed into the location specified by
// pLen.
//
// Succeeds if all of the following are true:
// - the pLen pointer is valid.
//
// If this method fails nothing will be written to *pLen.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pLen arg is NULL.
// 
AAFRESULT ImplAAFTypeDefOpaque::GetHandleBufLen (
    // value to set.
   ImplAAFPropertyValue * pOpaquePropertyValue,

    // Pointer to an variable used to return the length
   aafUInt32 *  pLength)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (!pOpaquePropertyValue || !pLength)
    return (AAFRESULT_NULL_PARAM);

  result = ValidateOpaquePropertyValue(pOpaquePropertyValue);
	if (AAFRESULT_FAILED (result))
		return result;


  // Get the private value data interface so that we can access the bits.
  ImplAAFPropValData* pOpaqueValueData = dynamic_cast<ImplAAFPropValData*>(pOpaquePropertyValue);
  if (NULL == pOpaqueValueData)
    return AAFRESULT_INTERNAL_ERROR; // AAFRESULT_INVALID_OBJ?

  // Get the size and the bits from the opaque value.
  result = pOpaqueValueData->GetBitsSize (pLength);
  if (AAFRESULT_FAILED(result))
    return result;

  //
  // Add the size of the extra version and validation information that
  // we added to the opaque data from the OM.
  //
  *pLength += kAAFOpaqueHandleOverhead;

	return (result);
}

//***********************************************************
//
// SetHandle()
//
// Used to read and save an unknown property without having to know its contents.
// This call fills takes a block of unknown data which has been saved in you application, and
// writes to another file. 
//
// Succeeds if all of the following are true:
// - the pHandle pointer is valid.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NOT_INITIALIZED
//   - This object has not yet had Initialize() called on it.
//
// AAFRESULT_NULL_PARAM
//   - pHandle is null.
// 
AAFRESULT ImplAAFTypeDefOpaque::SetHandle (
    // value to write data to.
    ImplAAFPropertyValue * pOpaquePropertyValue,

    // Size of preallocated buffer
	aafUInt32  handleSize,

    // buffer containing handle
	aafDataBuffer_t  pHandle)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (!pOpaquePropertyValue || !pHandle)
    return (AAFRESULT_NULL_PARAM);

  result = ValidateOpaquePropertyValue(pOpaquePropertyValue);
	if (AAFRESULT_FAILED (result))
		return result;

  //
  // Validate and extract the opaque data info from the given handle.
  //
  aafUInt32  opaqueDataSize = 0;
  aafDataBuffer_t opaqueDataBits = NULL;
  result = GetOpaqueHandleInfo(handleSize, pHandle, opaqueDataSize, opaqueDataBits);
	if (AAFRESULT_FAILED (result))
		return result;

  // Get the private value data interface so that we can access the bits.
  ImplAAFPropValData* pOpaqueValueData = dynamic_cast<ImplAAFPropValData*>(pOpaquePropertyValue);
  if (NULL == pOpaqueValueData)
    return AAFRESULT_INTERNAL_ERROR; // AAFRESULT_INVALID_OBJ?

  // Allocate the correct size value.
  aafMemPtr_t pOpaqueValueDataBits = NULL;
  result = pOpaqueValueData->AllocateBits (opaqueDataSize, &pOpaqueValueDataBits);
  if (AAFRESULT_FAILED(result))
    return result;

  //
  // Copy the opaque data from the handle into the newly resized opaque value.
  //
  copy ((OMByte *)opaqueDataBits, pOpaqueValueDataBits, opaqueDataSize);

  return (result);
}


  //***********************************************************
  //
  // CreateValueFromHandle()
  //
  // This call takes a block of unknown data created by GetHandle()
  // and saved in you application, and creates an opaque property value.
  // Returns the newly-created property value in ppOpaquePropertyValue.
  //
  // Succeeds if all of the following are true:
  // - the pInitData pointer is valid.
  // - the ppOpaquePropertyValue pointer is valid.
  // - initDataSize indicates pInitData is the correct size for 
  //   the actual type.
  // - compile-time struct has had its member offests registered.
  //
  // If this method fails nothing will be written to *ppOpaquePropertyValue.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - either pInitData or ppOpaquePropertyValue arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - initDataSize indicates pInitData is of the wrong size.
  // 
  HRESULT ImplAAFTypeDefOpaque::CreateValueFromHandle (
    // pointer to buffer containing handle to use
     aafMemPtr_t  pInitData,

    // size of handle in pInitData
    aafUInt32  initDataSize,

    // newly created property value
    ImplAAFPropertyValue ** ppOpaquePropertyValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (!pInitData || !ppOpaquePropertyValue)
    return (AAFRESULT_NULL_PARAM);

  //
  // Now we are ready to allocate and initialize the opaque value
  // that will contain a copy of the bits from the opaque handle.
  //
  ImplAAFPropValDataSP pOpaqueValueData;
  pOpaqueValueData = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!pOpaqueValueData)
    return AAFRESULT_NOMEMORY;
  pOpaqueValueData->ReleaseReference(); // make sure reference count is one.

  // Initialize the new property value with this opaque type.
  result = pOpaqueValueData->Initialize (this);
  if (AAFRESULT_FAILED(result))
    return result;

  // Attempt to initialize the opaque property value from the given opaque handle
  // data.
  result = SetHandle(pOpaqueValueData, initDataSize, pInitData);
  if (AAFRESULT_FAILED(result))
    return result;

  //
  // Return the newly allocated and initialized opaque data value.
  *ppOpaquePropertyValue = pOpaqueValueData;
  (*ppOpaquePropertyValue)->AcquireReference(); // refcount == 2, smartptr will reduce this to one.


  return (result);
}

// Find the actual type definition from the dictionary.
// This override of TypeDefIndirect will also look in the
// dictionary for opaque types.
AAFRESULT ImplAAFTypeDefOpaque::LookupActualType (
	aafUID_constref typeID, 
	ImplAAFTypeDef ** ppActualType) const
{
	if (!ppActualType)
		return (AAFRESULT_NULL_PARAM);

	// First see if this is a known type:
	AAFRESULT result = ImplAAFTypeDefIndirect::LookupActualType(typeID, ppActualType);
	if (AAFRESULT_FAILED(result))
	{
		// See if this is an opaque type.
		result = _dictionary->LookupOpaqueTypeDef(typeID, ppActualType);
	}

	return (result);
}



// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefOpaque::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid)
{
  if (!pTid)
    return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatOpaque;
  return AAFRESULT_SUCCESS;
}


OMUniqueObjectIdentification ImplAAFTypeDefOpaque::actualTypeId(
    const OMByte* externalBytes, size_t externalSize) const
{
    return ImplAAFTypeDefIndirect::actualTypeId(externalBytes, externalSize);
}

OMType* ImplAAFTypeDefOpaque::actualType(const OMByte* externalBytes, 
    size_t externalSize) const
{
    assert(false);
    return 0;
}
  
OMByteOrder ImplAAFTypeDefOpaque::byteOrder(const OMByte* externalBytes,
    size_t externalSize) const
{
    return ImplAAFTypeDefIndirect::byteOrder(externalBytes, externalSize);
}

void ImplAAFTypeDefOpaque::actualSize(const OMByte* externalBytes, 
    size_t externalSize, size_t& actualSize) const
{
    assert(false);
}

void ImplAAFTypeDefOpaque::actualData(const OMByte* externalBytes, size_t externalSize,
    OMByte* actualBytes, size_t& actualSize) const
{
    assert(false);
}

OMType* ImplAAFTypeDefOpaque::actualType(OMUniqueObjectIdentification id) const
{
    assert(false);
    return 0;
}

void ImplAAFTypeDefOpaque::externalData(const OMByte* externalBytes, size_t externalSize,
    const OMByte*& externalDataBytes, size_t& externalDataSize) const
{
    ImplAAFTypeDefIndirect::externalData(externalBytes, externalSize, externalDataBytes, 
        externalDataSize);
}

bool ImplAAFTypeDefOpaque::initialise(const OMUniqueObjectIdentification& id, 
    const wchar_t* name, const wchar_t* description)
{
    if (!ImplAAFMetaDefinition::initialise(id, name, description))
    {
        return false;
    }

    _initialized = true;
    //setInitialized();

    return true;    
}




// override from OMStorable.
const OMClassId& ImplAAFTypeDefOpaque::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefOpaque));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefOpaque::onSave(void* clientContext) const
{
  ImplAAFTypeDefIndirect::onSave(clientContext);
}

void ImplAAFTypeDefOpaque::onRestore(void* clientContext) const
{
  ImplAAFTypeDefIndirect::onRestore(clientContext);
}

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

#ifndef __ImplAAFTypeDefOpaque_h__
#include "ImplAAFTypeDefOpaque.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif



#include "AAFStoredObjectIDs.h"


#include <assert.h>
#include <string.h>


ImplAAFTypeDefOpaque::ImplAAFTypeDefOpaque ()
{}


ImplAAFTypeDefOpaque::~ImplAAFTypeDefOpaque ()
{}

//   Called when we initialize as one of the "builtin" types.
//
AAFRESULT
  ImplAAFTypeDefOpaque::pvtInitialize
      (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName)
{
  return ImplAAFTypeDefIndirect::pvtInitialize(id, pTypeName);
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
  aafUInt32 actualValueDataSize = 0;
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
    ImplAAFPropertyValue * pPropVal,

    // Size of preallocated buffer
    aafUInt32  handleSize,

    // Preallocated buffer to hold handle
    aafDataBuffer_t  pHandle,

    // Number of actual bytes read
    aafUInt32*  bytesRead)
{
	return AAFRESULT_NOT_IMPLEMENTED;
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
   ImplAAFPropertyValue * pPropVal,

    // Pointer to an variable used to return the length
   aafUInt32 *  pLen)
{
	return AAFRESULT_NOT_IMPLEMENTED;
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
    ImplAAFPropertyValue * pPropVal,

    // Size of preallocated buffer
	aafUInt32  handleSize,

    // buffer containing handle
	aafDataBuffer_t  pHandle)
{
	return AAFRESULT_NOT_IMPLEMENTED;
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
	return AAFRESULT_NOT_IMPLEMENTED;
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





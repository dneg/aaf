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






#ifndef __ImplAAFTypeDefStream_h__
#include "ImplAAFTypeDefStream.h"
#endif

#ifndef __ImplAAFStreamPropertyValue_h__
#include "ImplAAFStreamPropertyValue.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#include "OMPropertyDefinition.h"
#include "OMDataStreamProperty.h"
#include "OMAssertions.h"
#include "OMUtilities.h"


#include <assert.h>
#include <string.h>


extern "C" const aafClassID_t CLSID_AAFStreamPropertyValue;

ImplAAFTypeDefStream::ImplAAFTypeDefStream ()
{
}


ImplAAFTypeDefStream::~ImplAAFTypeDefStream ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::pvtInitialize (
      const aafUID_t & id,
      const aafCharacter * pTypeName)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (! pTypeName)
	return AAFRESULT_NULL_PARAM;

  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
 
   // Initialize the type definition with its unique identifier and name.
  result = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
  if (AAFRESULT_FAILED (result))
    return result; 

  // This type definition stream instance has been initialized.
  setInitialized();

  return AAFRESULT_SUCCESS;
}



// 
// Make sure that this is a valid stream property value.
//
AAFRESULT ImplAAFTypeDefStream::GetStreamPropertyValue(
  ImplAAFPropertyValue * pPropertyValue,
  ImplAAFStreamPropertyValue *& pStreamPropertyValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  pStreamPropertyValue = NULL; // init out parameter
  
  if (NULL == pPropertyValue)
    return AAFRESULT_NULL_PARAM;
  
  // The stream property value's type should be this instance of 
  // ImplAAFTypeDefStream.
  ImplAAFTypeDefSP pPropertyValueType;
  result = pPropertyValue->GetType(&pPropertyValueType);
  if ((ImplAAFTypeDef *)pPropertyValueType != (ImplAAFTypeDef *)this)
    return AAFRESULT_INVALID_PARAM;
  
  pStreamPropertyValue = dynamic_cast<ImplAAFStreamPropertyValue *>(pPropertyValue);
  if (NULL == pStreamPropertyValue)
    return AAFRESULT_INVALID_PARAM;
    
  return AAFRESULT_SUCCESS;
}

//
// Macro to encapsulate the convertions and validation of a ImplAAFPropertyValue
// into an ImplAAFStreamPropertyValue.
//
#define PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pv, spv)\
  AAFRESULT result = AAFRESULT_SUCCESS;\
  ImplAAFStreamPropertyValue * spv = NULL;\
  result = GetStreamPropertyValue(pv, spv);\
  if (AAFRESULT_FAILED(result))\
    return result










AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetSize (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64 *  pSize)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetSize(pSize);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::SetSize (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64  newSize)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetSize(newSize);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetPosition (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64 *  pPosition)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetPosition(pPosition);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::SetPosition (
      ImplAAFPropertyValue * pPropertyValue,
      aafInt64  newPosition)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetPosition(newPosition);
}





AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefStream::Read (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->Read(dataSize, pData, bytesRead);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::Write (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->Write(dataSize, pData);
}





AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::Append (
      ImplAAFPropertyValue * pPropertyValue,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->Append(dataSize, pData);
}






AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::HasStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      aafBoolean_t *  pHasByteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->HasStoredByteOrder(pHasByteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      eAAFByteOrder_t *  pByteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->GetStoredByteOrder(pByteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::SetStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue,
      eAAFByteOrder_t  byteOrder)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->SetStoredByteOrder(byteOrder);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::ClearStoredByteOrder (
      ImplAAFPropertyValue * pPropertyValue)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ClearStoredByteOrder();
}






AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefStream::ReadElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData,
      aafUInt32 *  bytesRead)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->ReadElements(pElementType, dataSize, pData, bytesRead);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::WriteElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->WriteElements(pElementType, dataSize, pData);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::AppendElements (
      ImplAAFPropertyValue * pPropertyValue,
      ImplAAFTypeDef * pElementType,
      aafUInt32  dataSize,
      aafMemPtr_t  pData)
{
  PROPERTYVALUE_TO_STREAMPROPERTYVALUE(pPropertyValue, pStreamPropertyValue);
  
  return pStreamPropertyValue->AppendElements(pElementType, dataSize, pData);
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetTypeCategory (
      eAAFTypeCategory_t *  pTid)
{
  if (NULL == pTid)
    return AAFRESULT_NULL_PARAM;
  
  *pTid = kAAFTypeCatStream;
  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::RawAccessType (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  if (NULL == ppRawTypeDef)
    return AAFRESULT_NULL_PARAM;
  
  // There is no "raw access type" for a stream. For now
  // just return this stream type and let the caller handle
  // reading the stream.
  *ppRawTypeDef = this;
  (*ppRawTypeDef)->AcquireReference();
  
  return AAFRESULT_SUCCESS;
}



// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefStream::CreatePropertyValue(
    OMProperty *property,
    ImplAAFPropertyValue ** ppPropertyValue ) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  assert (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
 
  ImplAAFStreamPropertyValue *pStreamValue = NULL;
  pStreamValue = (ImplAAFStreamPropertyValue*) CreateImpl (CLSID_AAFStreamPropertyValue);
  if (!pStreamValue) 
    return AAFRESULT_NOMEMORY;

  // Attempt to initialize the stream value. This will fail if given property is not a valid
  // stream property.
  result = pStreamValue->Initialize (this, property);
  if (AAFRESULT_SUCCEEDED(result))
  {
	*ppPropertyValue = pStreamValue; // The reference count is already 1.
	pStreamValue = NULL;
  }
  else
  {
    pStreamValue->ReleaseReference();
  }
  return result;
}





//
// Overrides of ImplAAFTypeDef
//

aafBool ImplAAFTypeDefStream::IsFixedSize (void) const
{
  return kAAFFalse;
}


size_t ImplAAFTypeDefStream::PropValSize (void) const
{
  assert (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDefStream::IsRegistered (void) const
{
  // This is an axiomatic type.
  return kAAFTrue;
}


size_t ImplAAFTypeDefStream::NativeSize (void) const
{
  assert (0);
  return 0; // not reached!
}


bool ImplAAFTypeDefStream::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefStream::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefStream::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefStream::IsVariableArrayable () const
{ return false; }

bool ImplAAFTypeDefStream::IsStringable () const
{ return false; }



OMProperty * ImplAAFTypeDefStream::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  // TEMPORARY: Need to use non-template class: Use existing OMDataStreamProperty
  // until new non-template typed-stream class is available.
  return new OMDataStreamProperty(pid, name);
}



// override from OMStorable.
const OMClassId& ImplAAFTypeDefStream::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefStream));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefStream::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefStream::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}

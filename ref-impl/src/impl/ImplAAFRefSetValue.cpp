/***********************************************************************
 *
 *              Copyright (c) 2000 Avid Technology, Inc.
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

#include "ImplAAFRefSetValue.h"

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif

#ifndef __ImplAAFRefValue_h__
#include "ImplAAFRefValue.h"
#endif

#ifndef __ImplAAFTypeDefRecord_h__
#include "ImplAAFTypeDefRecord.h"
#endif

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif


#include "OMProperty.h"
#include "OMRefSetProperty.h" // TBD: include header for class for singleton strong references.

#include <assert.h>
#include <string.h>


ImplAAFRefSetValue::ImplAAFRefSetValue () :
  _uidType(NULL),
  _keyBuffer(NULL),
  _keyBufferSize(0)
{}


ImplAAFRefSetValue::~ImplAAFRefSetValue ()
{
  if (NULL != _keyBuffer)
  {
    delete [] _keyBuffer;
    _keyBuffer = NULL;
  }
}







AAFRESULT ImplAAFRefSetValue::Initialize (
  const ImplAAFTypeDefSet *containerType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  // Concrete class' Initialize method should have already
  // validated the parameters.
  assert (NULL != containerType && NULL != property);
  
  if (NULL == dynamic_cast<OMReferenceSetProperty *>(property))
    return AAFRESULT_INVALID_PARAM;

  result = ImplAAFRefContainerValue::Initialize(containerType, property);
  if (AAFRESULT_FAILED(result))
    return result;

  
  // Cache the type definition for the unique identifier type
  // for the set.
  ImplAAFTypeDefObjectRef *pElementType = GetElementType();
  assert(NULL != pElementType);
  if (NULL == pElementType)
    return AAFRESULT_INVALID_OBJ;

  _uidType = containerType->GetUIDType((ImplAAFTypeDef *)pElementType, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  // Allocate key buffer used for extracting a key value from a property
  // valud for a key.
  _keyBufferSize = _uidType->NativeSize();
  assert(0 < _keyBufferSize);
  _keyBuffer = new aafUInt8[_keyBufferSize];
  if (NULL == _keyBuffer)
    return AAFRESULT_NOMEMORY;
  
  return result;
}

  
// Retrieve the property as an OMReferenceSetProperty.
OMReferenceSetProperty * ImplAAFRefSetValue::referenceSetProperty(void) const
{
  return static_cast<OMReferenceSetProperty *>(property());
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFRefSetValue::WriteTo(
  OMProperty* pOmProp)
{
  return (ImplAAFRefContainerValue::WriteTo(pOmProp));
}


// Remove the object identified by identification from the 
// reference set.
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFRefSetValue::Remove(
    void* identification)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == identification)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  OMReferenceSetProperty* pReferenceSetProperty = referenceSetProperty();
  assert(NULL != pReferenceSetProperty);
  // Satisfy the om precondition that the given identifier must exist
  // in the set for remove to succeed.
  if (!pReferenceSetProperty->contains(identification))
    return AAFRESULT_OBJECT_NOT_FOUND;

  OMObject* oldStorable = pReferenceSetProperty->remove(identification);
  if (oldStorable && usesReferenceCounting())
  {
    ReleaseOldObject(oldStorable);
  }


  return result;
}

// Does the reference set contain an object identified
// by identification?
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFRefSetValue::Contains(
    void* identification,
    aafBoolean_t* pResult) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == identification || NULL == pResult)
    return AAFRESULT_NULL_PARAM;
  *pResult = kAAFFalse;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  OMReferenceSetProperty* pReferenceSetProperty = referenceSetProperty();
  assert(NULL != pReferenceSetProperty);
  if (pReferenceSetProperty->contains(identification))
  {
    *pResult = kAAFTrue;
  }
  else
  {
    *pResult = kAAFFalse;
  }

  return result;
}


// Find the object in the reference set property identified by 
// identification.  If the object is found it is returned in object.
// If the object is not found the result is false.
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFRefSetValue::FindObject(
    void* identification,
    ImplAAFStorable **ppObject,
    aafBoolean_t* pResult) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == identification || NULL == ppObject || NULL == pResult)
    return AAFRESULT_NULL_PARAM;
  *ppObject = NULL;
  *pResult = kAAFFalse;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  OMReferenceSetProperty* pReferenceSetProperty = referenceSetProperty();
  assert(NULL != pReferenceSetProperty);
    
  OMObject *object = NULL;
  if (!pReferenceSetProperty->findObject(identification, object))
    return AAFRESULT_OBJECT_NOT_FOUND;
  
  *ppObject = ImplAAFRefValue::ConvertOMObjectToRoot(object);
  assert(NULL != *ppObject);
  if (NULL == *ppObject)
    return AAFRESULT_INVALID_OBJ;
  
  // We are returning a reference to the caller so we need to 
  // bump the reference count.
  (*ppObject)->AcquireReference();
  
  return result;
}

// Load the key (if valid) into the _keyBuffer
void ImplAAFRefSetValue::LoadKey(ImplAAFPropertyValue * pKey, AAFRESULT& result)
{

  assert (isInitialized());
  if (!isInitialized())
  {
    result = AAFRESULT_NOT_INITIALIZED;
    return;
  }

  assert(NULL != pKey);
  ImplAAFTypeDefSP pKeyType;
  result = pKey->GetType(&pKeyType);
  if (AAFRESULT_FAILED(result))
    return;
  
  // The key type must be the same as the type for the unique identifier
  // for the set.
  if (_uidType != (ImplAAFTypeDef*)(pKeyType))
  {
    result = AAFRESULT_BAD_TYPE;
    return;
  }
  
  // Use the unique type to extra the key
  result = _uidType->GetStruct(pKey, _keyBuffer, _keyBufferSize);
}


// Find the element associated with the given key.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFRefSetValue::LookupElement(
    ImplAAFPropertyValue * pKey,
    ImplAAFPropertyValue ** ppElementPropertyValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pKey || NULL == ppElementPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppElementPropertyValue = NULL;
  
  LoadKey(pKey, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  ImplAAFStorableSP pObject;
  aafBoolean_t bContainsKey;
  result = FindObject(_keyBuffer, &pObject, &bContainsKey);
  if (AAFRESULT_FAILED(result))
    return result;

  ImplAAFTypeDefObjectRef *pElementType = GetElementType();
  assert(NULL != pElementType);
  if (NULL == pElementType)
    return AAFRESULT_INVALID_OBJ;

  result = pElementType->CreateValue((ImplAAFStorable *)pObject, ppElementPropertyValue);
  if (AAFRESULT_FAILED(result))
    return result;
  

  return result;
}


// Test for containment by key.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFRefSetValue::ContainsKey(
    ImplAAFPropertyValue * pKey,
    aafBoolean_t*  pContainsKey)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pKey || NULL == pContainsKey)
    return AAFRESULT_NULL_PARAM;
  *pContainsKey = kAAFFalse;
  
  LoadKey(pKey, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  result = Contains(_keyBuffer, pContainsKey);

  return result;
}

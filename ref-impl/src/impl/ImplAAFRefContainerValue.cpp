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

#include "ImplAAFRefContainerValue.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFRefValue_h__
#include "ImplAAFRefValue.h"
#endif

#ifndef __ImplEnumAAFStorablePropVals_h__
#include "ImplEnumAAFStorablePropVals.h"
#endif


#include "OMProperty.h"
#include "OMContainerProperty.h" // TBD: include header for base class for singleton references.
#include "OMReferenceContainer.h"
#include "OMRefSetProperty.h"
#include "OMRefVectorProperty.h"
#include "OMPropertyDefinition.h"
#include "OMObject.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFStorablePropVals;


ImplAAFRefContainerValue::ImplAAFRefContainerValue ()
{}


ImplAAFRefContainerValue::~ImplAAFRefContainerValue ()
{
}







AAFRESULT ImplAAFRefContainerValue::Initialize (
  const ImplAAFTypeDef *containerType,    
  OMProperty *property)
{
  // Concrete class' Initialize method should have already
  // validated the parameters.
  assert (NULL != containerType && NULL != property);
  
  if (NULL == dynamic_cast<OMContainerProperty *>(property))
    return AAFRESULT_INVALID_PARAM;
    
  AAFRESULT result = ImplAAFPropertyValue::Initialize(containerType, property);
  return result;
}

  
// Retrieve the property as an OMReferenceContainer.
OMReferenceContainer* ImplAAFRefContainerValue::referenceContainer(void) const
{
  assert (isInitialized());
  OMProperty* p = property();
  OMContainerProperty* cp = dynamic_cast<OMContainerProperty*>(p);
  assert(cp != 0);
  OMReferenceContainer* result = cp->referenceContainer();
  assert(result != 0);
  return result;
}


//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::WriteTo(
  OMProperty* pOmProp)
{
  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  if (NULL != property())
  {
    // Make sure that the given property is the same one that was used to 
    // initialize this property value. NOTE: Copying an object reference to a 
    // different OMProperty should be handled through another interface.
    if (pOmProp != property())
      return AAFRESULT_INVALID_PARAM;
  }
  
  return AAFRESULT_SUCCESS;
//  return AAFRESULT_SUCCESS;
}


/* static */ ImplAAFStorable * ImplAAFRefContainerValue::GetStorableFromPropertyValue(
  ImplAAFPropertyValue* pPropertyValue,
  AAFRESULT & result)
{
  result = AAFRESULT_SUCCESS;
  assert(NULL != pPropertyValue);
  
  ImplAAFStorable* storable = NULL; // initialize the returned parameter
  
  // Make sure that the type definition can contain a single object
  // reference.
  ImplAAFTypeDefSP pPropertyValueType;
  result = pPropertyValue->GetType(&pPropertyValueType);
  if (AAFRESULT_FAILED(result))
    return NULL;
  ImplAAFTypeDefObjectRef* pObjectRefType = NULL;
  pObjectRefType = dynamic_cast<ImplAAFTypeDefObjectRef *>((ImplAAFTypeDef *)pPropertyValueType);
  if (NULL == pObjectRefType)
  {
    result = AAFRESULT_INVALID_PARAM;
    return NULL;
  }
  
  // Use the type to extract the object from the input property
  // value.
  ImplAAFRoot *pObject = NULL;
  result = pObjectRefType->GetObject(pPropertyValue, &pObject); // returns reference counted object!
  if (AAFRESULT_FAILED(result))
    return NULL;
    
  // the returned object is NOT reference counted.
  pObject->ReleaseReference();

  storable = ImplAAFRefValue::ConvertRootToOMStorable(pObject);
  assert(NULL != storable);
  if (NULL == storable)
    result = AAFRESULT_INVALID_OBJ; 

  
  return storable;  
}

void ImplAAFRefContainerValue::ReleaseOldObject(OMObject *object)
{
  assert(object && usesReferenceCounting());
  ImplAAFStorable *pOldObject = ImplAAFRefValue::ConvertOMObjectToRoot(object);
  assert(NULL != pOldObject);
  if (NULL == pOldObject)
    throw AAFRESULT_INVALID_OBJ; // ???
  pOldObject->ReleaseReference();
}


// Insert the given object into this contain property.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::InsertObject(
  ImplAAFStorable* pObject)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;
		
	result = ValidateNewObject(pObject);
	if (AAFRESULT_FAILED(result))
		return result;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
 
  // Hand off to the OMReferenceContainer
  OMReferenceContainer* pReferenceContainer = referenceContainer();

  // Object refernce containers should only contain a single reference
  // to an object.
  if (!pReferenceContainer->containsObject(pObject))
  {
    pReferenceContainer->insertObject(pObject);
    if (usesReferenceCounting())
    {
      pObject->AcquireReference();
    }
    
  }
    
  return result;
}


// Is the given object in the container property.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::ContainsObject(
  ImplAAFStorable* pObject,
  aafBoolean_t* pResult)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pObject || NULL == pResult)
    return AAFRESULT_NULL_PARAM;
  
  *pResult = kAAFFalse;  

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
 
  // Hand off to the OMReferenceContainer
  OMReferenceContainer* pReferenceContainer = referenceContainer();
  if (pReferenceContainer->containsObject(pObject))
  {
    *pResult = kAAFTrue;
  }
    
  return result;
}


// Remove the given object from the container property.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::RemoveObject(
  ImplAAFStorable* pObject)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
 
  // Hand off to the OMeferenceContainer
  OMReferenceContainer* pReferenceContainer = referenceContainer();
  
  // Object refernce containers should only contain a single reference
  // to an object.
  if (!pReferenceContainer->containsObject(pObject))
    result = AAFRESULT_OBJECT_NOT_FOUND;

  pReferenceContainer->removeObject(pObject);
  if (usesReferenceCounting())
    pObject->ReleaseReference();
    
  return result;
}


  
// Insert the given object into this contain property.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::InsertElement(
  ImplAAFPropertyValue* pPropertyValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pPropertyValue)
    return AAFRESULT_NULL_PARAM;
    
  ImplAAFStorable * pObject = GetStorableFromPropertyValue(pPropertyValue, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  result = InsertObject(pObject); 
  return result;
}

// Is the given object in the container property.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::ContainsElement(
  ImplAAFPropertyValue* pPropertyValue,
  aafBoolean_t* pResult)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pPropertyValue || NULL == pResult)
    return AAFRESULT_NULL_PARAM;
  
  *pResult = kAAFFalse;  
    
  ImplAAFStorable * pObject = GetStorableFromPropertyValue(pPropertyValue, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  result = ContainsObject(pObject, pResult); 
  return result;
}


// The number of objects in the container property
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::Count(
  aafUInt32* pCount)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pCount)
    return AAFRESULT_NULL_PARAM;
 
  *pCount = referenceContainer()->count(); 
  return result;
}
  

// Remove the given object from the container property.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::RemoveElement(
  ImplAAFPropertyValue* pPropertyValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pPropertyValue)
    return AAFRESULT_NULL_PARAM;
    
  ImplAAFStorable * pObject = GetStorableFromPropertyValue(pPropertyValue, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  result = RemoveObject(pObject); 
  return result;
}

  
// Get an enumerator for the given container property
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::GetElements(
  ImplEnumAAFPropertyValues** ppEnum)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == ppEnum)
    return AAFRESULT_NULL_PARAM;
  
  *ppEnum = NULL; // initialize the return parameter.

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;


  ImplAAFRoot* pRoot = CreateImpl(CLSID_EnumAAFStorablePropVals);
  if (NULL == pRoot)
    return AAFRESULT_NOMEMORY;
    
  ImplEnumAAFStorablePropVals* pNewEnum = dynamic_cast<ImplEnumAAFStorablePropVals*>(pRoot);
  if (NULL != pNewEnum)
  {
    OMReferenceContainerIterator* newIterator = referenceContainer()->createIterator();
    if (NULL != newIterator)
    {
      result = pNewEnum->Initialize(this, newIterator);
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppEnum = pNewEnum;
        pNewEnum->AcquireReference();
      }
    }
    else
    {
      // _containerIterator->copy() failed...
      result = AAFRESULT_NOMEMORY;
    } 
  }

  // This will free the newly created object if the initialzation has failed.
  pRoot->ReleaseReference();


  return result;
}

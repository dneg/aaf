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

#include "OMAssertions.h"
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFStorablePropVals;


ImplAAFRefContainerValue::ImplAAFRefContainerValue ()
{}


ImplAAFRefContainerValue::~ImplAAFRefContainerValue ()
{
}





AAFRESULT ImplAAFRefContainerValue::Initialize (
	  const ImplAAFTypeDef *propertyType)
{
  return ImplAAFPropertyValue::Initialize(propertyType);
}


AAFRESULT ImplAAFRefContainerValue::Initialize (
  const ImplAAFTypeDef *containerType,    
  OMProperty *property)
{
  // Concrete class' Initialize method should have already
  // validated the parameters.
  ASSERTU (NULL != containerType && NULL != property);
  
  if (NULL == dynamic_cast<OMContainerProperty *>(property))
    return AAFRESULT_INVALID_PARAM;
    
  AAFRESULT result = ImplAAFPropertyValue::Initialize(containerType, property);
  return result;
}

  
// Retrieve the property as an OMReferenceContainer.
OMReferenceContainer* ImplAAFRefContainerValue::referenceContainer(void) const
{
  ASSERTU (isInitialized());
  OMProperty* p = property();
  OMContainerProperty* cp = dynamic_cast<OMContainerProperty*>(p);
  ASSERTU(cp != 0);
  OMReferenceContainer* result = cp->referenceContainer();
  ASSERTU(result != 0);
  return result;
}


//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFRefContainerValue::WriteTo(
  OMProperty* pOmProp)
{
  ASSERTU (isInitialized());
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
  ASSERTU(NULL != pPropertyValue);
  
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
  ASSERTU(NULL != storable);
  if (NULL == storable)
    result = AAFRESULT_INVALID_OBJ; 

  
  return storable;  
}

void ImplAAFRefContainerValue::ReleaseOldObject(OMObject *object)
{
  ASSERTU(object && usesReferenceCounting());
  ImplAAFStorable *pOldObject = ImplAAFRefValue::ConvertOMObjectToRoot(object);
  ASSERTU(NULL != pOldObject);
  if (NULL == pOldObject)
    throw AAFRESULT_INVALID_OBJ; // ???
  pOldObject->ReleaseReference();
}

  
// Utility to release all old OMObjects from the given container.
AAFRESULT ImplAAFRefContainerValue::ReleaseAllObjects(OMReferenceContainer *pContainerProperty)
{
  ASSERTU(pContainerProperty && usesReferenceCounting());

  OMReferenceContainerIterator* containerIter = pContainerProperty->createIterator();
  if (NULL == containerIter)
    return AAFRESULT_NOMEMORY;
    
  AAFRESULT result = AAFRESULT_SUCCESS;
  while (AAFRESULT_SUCCEEDED(result) && (containerIter->before() || containerIter->valid()))
  {
    if (++(*containerIter))
    {
      OMObject *object = containerIter->currentObject();
      ImplAAFStorable *obj = dynamic_cast<ImplAAFStorable*>(object);
      ASSERTU(NULL != obj);
      if (NULL == obj)
      {
        result = AAFRESULT_INVALID_OBJ;
      }
      else
      {
        obj->ReleaseReference();
      }
    }
  }

  delete containerIter;
  containerIter = NULL;
  
  return AAFRESULT_SUCCESS;
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

  ASSERTU (isInitialized());
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

  ASSERTU (isInitialized());
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

  ASSERTU (isInitialized());
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

  ASSERTU (isInitialized());
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

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

#include "ImplAAFRefArrayValue.h"

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

#ifndef __ImplAAFTypeDefArray_h__
#include "ImplAAFTypeDefArray.h"
#endif

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif

#ifndef __ImplAAFRefValue_h__
#include "ImplAAFRefValue.h"
#endif


#include "OMProperty.h"
#include "OMRefVectorProperty.h"
#include "OMReferenceVector.h"

#include <assert.h>
#include <string.h>






ImplAAFRefArrayValue::ImplAAFRefArrayValue () :
  _fixedSize(false),
  _tempStorableVector(NULL)
{}


ImplAAFRefArrayValue::~ImplAAFRefArrayValue ()
{
	delete _tempStorableVector;
	_tempStorableVector = NULL;
}



bool ImplAAFRefArrayValue::fixedSize(void) const
{
  return _fixedSize;
}


AAFRESULT ImplAAFRefArrayValue::Initialize (
	  const ImplAAFTypeDefArray *containerType,
	  bool fixed)
{
  _fixedSize = fixed; // TBD: Add validation
  
  _tempStorableVector = new OMReferenceVector<ImplAAFStorable>;
  if (NULL == _tempStorableVector)
  {
    return AAFRESULT_NOMEMORY;
  }

  return (ImplAAFRefContainerValue::Initialize(containerType));
}


AAFRESULT ImplAAFRefArrayValue::Initialize (
  const ImplAAFTypeDefArray *containerType,    
  OMProperty *property,
  bool fixed)
{
  // Concrete class' Initialize method should have already
  // validated the parameters.
  assert (NULL != containerType && NULL != property);
  
  if (NULL == dynamic_cast<OMReferenceVectorProperty *>(property))
    return AAFRESULT_INVALID_PARAM;
    
  _fixedSize = fixed; // TBD: Add validation
  return (ImplAAFRefContainerValue::Initialize(containerType, property));
}

  
// Retrieve the property or temporary reference vector as an OMReferenceContainer.
OMReferenceContainer* ImplAAFRefArrayValue::referenceContainer(void) const
{
  assert (isInitialized());
  OMReferenceContainer* result = NULL;
  OMProperty* p = property();
  
  if (NULL != p)
  {
  	// If there is a property for "direct access" then use the container's
  	// implementation to extract the OMReferenceContainer.
  	result = ImplAAFRefContainerValue::referenceContainer();
  }
  else
  {
  	// There is no property for "direct access" so get the container interface from
  	// the temporary reference vector.
		result = (const_cast<ImplAAFRefArrayValue *>(this)->_tempStorableVector);
  }
  
  assert(result != 0);
  return result;
}

// Retrieve the property or temporary reference vector as an OMObjectVector.
OMObjectVector * ImplAAFRefArrayValue::referenceVector(void) const
{
	OMObjectVector * result = NULL;
  OMProperty* p = property();

  if (NULL != p)
  {
  	result = static_cast<OMReferenceVectorProperty *>(p);
 	}
 	else
 	{
  	// There is no property for "direct access" so get the vector interface from
  	// the temporary reference vector.
		result = (const_cast<ImplAAFRefArrayValue *>(this)->_tempStorableVector);
 	}
  
  assert(result != 0);
  return result;
}


//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::WriteTo(
  OMProperty* pOmProp)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  OMObject* object;
  ImplAAFStorable* obj;
  if (NULL == property())
  {
    // First empty the current property.
    OMContainerProperty * pNewContainerProperty = dynamic_cast<OMContainerProperty *>(pOmProp);
    if (NULL == pNewContainerProperty)
      return AAFRESULT_INVALID_OBJ;
    OMReferenceContainer *pNewReferenceContainer = pNewContainerProperty->referenceContainer();
    if (NULL == pNewReferenceContainer)
      return AAFRESULT_INVALID_OBJ;
    
    if (usesReferenceCounting())
    {
      result = ReleaseAllObjects(pNewReferenceContainer);
      if (AAFRESULT_FAILED(result))
        return result;
    }
      
    // Make sure that there are no elements in the new property container.
    pNewReferenceContainer->removeAllObjects();
    
    // Install the new "empty" container property.
    SetProperty(pOmProp);

    OMReferenceContainerIterator* tempIterator = _tempStorableVector->createIterator();
    if (NULL == tempIterator)
    {
      result = AAFRESULT_NOMEMORY;
    }
    else
    {
      while (AAFRESULT_SUCCEEDED(result) && (tempIterator->before() || tempIterator->valid()))
      {
        if (++(*tempIterator))
        {
          object = tempIterator->currentObject();
          obj = dynamic_cast<ImplAAFStorable*>(object);
          assert(NULL != obj);
          if (NULL == obj)
          {
            result = AAFRESULT_INVALID_OBJ;
          }
          else
          {
//            if (usesReferenceCounting())
//            {
//              obj->detach();
//            }
            
            // Add the element from the temporary property container to the property.
            result = InsertObject(obj); // reference counts if necessary...
            
            if (AAFRESULT_SUCCEEDED(result))
            {
              if (usesReferenceCounting())
              {
                obj->ReleaseReference();
              }
            }
          }
        }
      }
   
      delete tempIterator;
      tempIterator = NULL;
    }
    
    // Cleanup the temporary property now the the the pOmProp has been created and initialized.
		delete _tempStorableVector;
		_tempStorableVector = NULL;
    
    if (AAFRESULT_FAILED(result))
      return result;
  }

  return (ImplAAFRefContainerValue::WriteTo(pOmProp));
}





// Insert the given object into this contain property.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::InsertObject(
  ImplAAFStorable* pObject)
{
  // Cannot change size of a fixed array.
  assert(!fixedSize());
  if (fixedSize())
    return AAFRESULT_INVALID_OBJ;    
  return ImplAAFRefContainerValue::InsertObject(pObject);
}


// Remove the given object from the container property.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::RemoveObject(
  ImplAAFStorable* pObject)
{
  // Cannot change size of a fixed array.
  assert(!fixedSize());
  if (fixedSize())
    return AAFRESULT_INVALID_OBJ;    
  return ImplAAFRefContainerValue::RemoveObject(pObject);
}


  

//
// Methods to access the elements from the OMObjectVector
// 


// Set the value of the OMObjectVector at position index to pObject.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::SetObjectAt(
  ImplAAFStorable* pObject,
  aafUInt32 index)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;
  
  ImplAAFStorable* newStorable = ImplAAFRefValue::ConvertRootToOMStorable(pObject);
  assert(NULL != newStorable);
  if (NULL == newStorable)
    return AAFRESULT_INVALID_OBJ;
  
  // Validate the index...  
  OMObjectVector * pReferenceVector = referenceVector();
  assert (NULL != pReferenceVector);
  aafUInt32 elementCount = pReferenceVector->count();
  if (index >= elementCount)
    return AAFRESULT_BADINDEX;
    
  OMObject *oldStorable = pReferenceVector->setObjectAt(newStorable, index);
  if (usesReferenceCounting())
  {
    newStorable->AcquireReference();
    if (oldStorable)
      ReleaseOldObject(oldStorable);
  }
  
  
  return result;
}  


// Get the value of the OMObjectVector at position index.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::GetObjectAt(
  aafUInt32 index, 
  ImplAAFStorable** ppObject) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == ppObject)
    return AAFRESULT_NULL_PARAM;
  *ppObject = NULL;
  
  // Validate the index...  
  OMObjectVector * pReferenceVector = referenceVector();
  assert (NULL != pReferenceVector);
  aafUInt32 elementCount = pReferenceVector->count();
  if (index >= elementCount)
    return AAFRESULT_BADINDEX;
    
  OMObject *object = pReferenceVector->getObjectAt(index);
  assert(NULL != object);
  if (NULL == object)
    return AAFRESULT_INVALID_OBJ;
  
  *ppObject = ImplAAFRefValue::ConvertOMObjectToRoot(object);
  assert(NULL != *ppObject);
  if (NULL == *ppObject)
    return AAFRESULT_INVALID_OBJ;
  
  // We are returning a reference to the caller so we need to 
  // bump the reference count.
  (*ppObject)->AcquireReference();
  
  return result;
}  


// Append the given pObject to the OMObjectVector.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::AppendObject(
  ImplAAFStorable* pObject)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  // Cannot change size of a fixed array.
  assert(!fixedSize());
  if (fixedSize())
    return AAFRESULT_INVALID_OBJ;    

  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;

  ImplAAFStorable *storable = ImplAAFRefValue::ConvertRootToOMStorable(pObject);
  if (NULL == storable)
    result = AAFRESULT_INVALID_OBJ; 
    
  OMObjectVector * pReferenceVector = referenceVector();
  assert (NULL != pReferenceVector);
  pReferenceVector->appendObject(storable);

  if (usesReferenceCounting())
  {
    pObject->AcquireReference();
  }
  
  return result;
}  


// Prepend the given pObject> to the OMObjectVector.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::PrependObject(
  ImplAAFStorable* pObject)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;

  // Cannot change size of a fixed array.
  assert(!fixedSize());
  if (fixedSize())
    return AAFRESULT_INVALID_OBJ;    

  ImplAAFStorable *storable = ImplAAFRefValue::ConvertRootToOMStorable(pObject);
  if (NULL == storable)
    result = AAFRESULT_INVALID_OBJ; 
    
  OMObjectVector * pReferenceVector = referenceVector();
  assert (NULL != pReferenceVector);
  pReferenceVector->prependObject(storable);

  if (usesReferenceCounting())
  {
    pObject->AcquireReference();
  }
  
  return result;
}  


// Remove the object from the OMObjectVector at position index.
// Existing objects in the OMObjectVector at index + 1 and higher 
// are shifted down one index position.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::RemoveObjectAt(
  aafUInt32 index)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  // Cannot change size of a fixed array.
  assert(!fixedSize());
  if (fixedSize())
    return AAFRESULT_INVALID_OBJ;    
  
  // Validate the index...  
  OMObjectVector * pReferenceVector = referenceVector();
  assert (NULL != pReferenceVector);
  aafUInt32 elementCount = pReferenceVector->count();
  if (index >= elementCount)
    return AAFRESULT_BADINDEX;
    
    
  OMObject *oldStorable = pReferenceVector->removeObjectAt(index);
  if (usesReferenceCounting())
  {
    if (oldStorable)
      ReleaseOldObject(oldStorable);
  }
  
  return result;
}  

// Insert pObject into the OMObjectVector at position index. 
// Existing objects at index> and higher are shifted up one index position.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::InsertObjectAt(
  ImplAAFStorable* pObject,
  aafUInt32 index)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;

  // Cannot change size of a fixed array.
  assert(!fixedSize());
  if (fixedSize())
    return AAFRESULT_INVALID_OBJ;    
  
  // Validate the index...  
  OMObjectVector * pReferenceVector = referenceVector();
  assert (NULL != pReferenceVector);
  aafUInt32 elementCount = pReferenceVector->count();
  if (index > elementCount) // it is valid to insert after the last element
    return AAFRESULT_BADINDEX;

  pReferenceVector->insertObjectAt(pObject, index);
  
  if (usesReferenceCounting())
  {
    pObject->AcquireReference();
  }

  
  return result;
}  


// Set the value of the OMObjectVector at position index to the
// object inside the property value.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::SetElementAt(
  ImplAAFPropertyValue* pPropertyValue,
  aafUInt32 index)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pPropertyValue)
    return AAFRESULT_NULL_PARAM;
    
  ImplAAFStorable * pObject = GetStorableFromPropertyValue(pPropertyValue, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  result = SetObjectAt(pObject, index); 
  return result;
}


// Get the value of the OMObjectVector at position index.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::GetElementAt(
  aafUInt32 index,
  ImplAAFPropertyValue** ppPropertyValue) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL;
  
  ImplAAFStorableSP pObject;
  result = GetObjectAt(index, &pObject);
  if (AAFRESULT_FAILED(result))
    return result;

  ImplAAFTypeDefObjectRef *pElementType = GetElementType();
  assert(NULL != pElementType);
  if (NULL == pElementType)
    return AAFRESULT_INVALID_OBJ;
    
  result = pElementType->CreateValue((ImplAAFStorable *)pObject, ppPropertyValue);
  if (AAFRESULT_FAILED(result))
    return result;
  
  return result;  
}


// Append the given pObject to the OMObjectVector.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::AppendElement(
  ImplAAFPropertyValue* pPropertyValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pPropertyValue)
    return AAFRESULT_NULL_PARAM;
    
  ImplAAFStorable * pObject = GetStorableFromPropertyValue(pPropertyValue, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  result = AppendObject(pObject); 
  return result;
}


// Prepend the given pObject> to the OMObjectVector.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::PrependElement(
  ImplAAFPropertyValue* pPropertyValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pPropertyValue)
    return AAFRESULT_NULL_PARAM;
    
  ImplAAFStorable * pObject = GetStorableFromPropertyValue(pPropertyValue, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  result = PrependObject(pObject); 
  return result;
}

// Insert pObject into the OMObjectVector at position index. 
// Existing objects at index> and higher are shifted up one index position.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::InsertElementAt(
  ImplAAFPropertyValue* pPropertyValue,
  aafUInt32 index)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == pPropertyValue)
    return AAFRESULT_NULL_PARAM;
    
  ImplAAFStorable * pObject = GetStorableFromPropertyValue(pPropertyValue, result);
  if (AAFRESULT_FAILED(result))
    return result;
  
  result = InsertObjectAt(pObject, index); 
  return result;
}
  

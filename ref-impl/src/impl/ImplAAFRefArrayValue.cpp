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
#include "OMPropertyDefinition.h"

#include <assert.h>
#include <string.h>


static const OMClassId sTempStorableClassId =
{ 0xFFFFFFFF, 0xFFFF, 0xFFFF, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };

static const wchar_t * sTempVectorPropertyName = L"Temporary Vector Property";
static const OMPropertyId sTempVectorPropertyId = 0xFFFF;


class TempStorable : public OMStorable
{
public:
  TempStorable(void);
  virtual ~TempStorable(void);
  
  virtual const OMClassId& classId(void) const;
};


class TempPropertyDefinition : public OMPropertyDefinition
{
public:
  TempPropertyDefinition(const ImplAAFTypeDef *type);
  virtual ~TempPropertyDefinition(void);
  virtual const OMType* type(void) const;
  virtual const wchar_t* name(void) const;
  virtual OMPropertyId localIdentification(void) const;
  virtual bool isOptional(void) const;

  const ImplAAFTypeDef *_type;
};

TempStorable::TempStorable(void)
{}

TempStorable::~TempStorable(void)
{
  // If there are any properties they have been allocated
  // dynamically.
  OMProperty * tempProperty = propertySet()->get(sTempVectorPropertyId);
  assert(NULL != tempProperty);
  delete tempProperty;
}

const OMClassId& TempStorable::classId(void) const
{
  return sTempStorableClassId;
}


TempPropertyDefinition::TempPropertyDefinition(const ImplAAFTypeDef *type) :
  _type(type)
{}

TempPropertyDefinition::~TempPropertyDefinition()
{}

const OMType* TempPropertyDefinition::type(void) const
{
  return _type;
}

const wchar_t* TempPropertyDefinition::name(void) const
{
  return sTempVectorPropertyName;
}

OMPropertyId TempPropertyDefinition::localIdentification(void) const
{
  return sTempVectorPropertyId;
}

bool TempPropertyDefinition::isOptional(void) const
{
  return true;
}




ImplAAFRefArrayValue::ImplAAFRefArrayValue () :
  _fixedSize(false),
  _tempPropertyDefinition(NULL),
  _tempStorable(NULL)
{}


ImplAAFRefArrayValue::~ImplAAFRefArrayValue ()
{
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
  
  if (NULL == _tempPropertyDefinition)
  {
    _tempPropertyDefinition = new TempPropertyDefinition(containerType);
    if (NULL == _tempPropertyDefinition)
      return AAFRESULT_NOMEMORY;
  }

  if (NULL == _tempStorable)
  {
    _tempStorable = new TempStorable();
    if (NULL == _tempStorable)
      return AAFRESULT_NOMEMORY;
  }

    
  assert (NULL == property());
  
  // Create the temporary property...  
  OMProperty *tempProperty = containerType->pvtCreateOMProperty(_tempPropertyDefinition->localIdentification(), _tempPropertyDefinition->name());
  if (NULL == tempProperty)
    return AAFRESULT_NOMEMORY;
  
  // Attach the temporary property to our temporary storable. The property now has a "valid" container.  
  _tempStorable->propertySet()->put(tempProperty);
  
  // Make sure that the property has a "valid" definition.  
  tempProperty->initialize(_tempPropertyDefinition);
  
  // Install the new property into this property value.
  SetProperty(tempProperty);

  return (ImplAAFRefContainerValue::Initialize(containerType));
}

void ImplAAFRefArrayValue::CleanupTemporaryProperty(void)
{
  if (NULL != property() && _tempPropertyDefinition == property()->definition())
  {
    // Cleanup the property if it was temporarily allocated but never
    // attached to the real container's property set.
    // NOTE: The temporary property will be deleted along with the 
    // temporary storable container.
    SetProperty(NULL);
  }

  if (NULL != _tempStorable)
  {
    delete _tempStorable; // This will cleanup the temporary property
    _tempStorable = NULL;
  }
  
  if (NULL != _tempPropertyDefinition)
  {
    delete _tempPropertyDefinition;
    _tempPropertyDefinition = NULL;
  }
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

  
// Retrieve the property as an OMReferenceVectorProperty.
OMReferenceVectorProperty * ImplAAFRefArrayValue::referenceVectorProperty(void) const
{
  return static_cast<OMReferenceVectorProperty *>(property());
}


//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::WriteTo(
  OMProperty* pOmProp)
{
  // PATCH for DR4 CPR:
  // If we have created an unattached temporary vector property to hold the 
  // the new objects then we need to update the given property, pOmProp, to hold
  // the contents of the the temporary property.
  AAFRESULT result = AAFRESULT_SUCCESS;
  OMObject* object;
  ImplAAFStorable* obj;
  OMReferenceVectorProperty * pTempReferenceVectorProperty = referenceVectorProperty();
  assert (NULL != pTempReferenceVectorProperty);
  if (pTempReferenceVectorProperty->definition() == _tempPropertyDefinition)
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

    OMReferenceContainerIterator* tempIterator = pTempReferenceVectorProperty->createIterator();
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
            if (usesReferenceCounting())
            {
              obj->detach();
            }
            
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
    CleanupTemporaryProperty();
    
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
// Methods to access the elements from the OMReferenceVectorProperty
// 


// Set the value of the OMReferenceVectorProperty at position index to pObject.
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
  OMReferenceVectorProperty * pReferenceVectorProperty = referenceVectorProperty();
  assert (NULL != pReferenceVectorProperty);
  aafUInt32 elementCount = pReferenceVectorProperty->count();
  if (index >= elementCount)
    return AAFRESULT_BADINDEX;
    
  OMObject *oldStorable = pReferenceVectorProperty->setObjectAt(newStorable, index);
  if (usesReferenceCounting())
  {
    newStorable->AcquireReference();
    if (oldStorable)
      ReleaseOldObject(oldStorable);
  }
  
  
  return result;
}  


// Get the value of the OMReferenceVectorProperty at position index.
AAFRESULT STDMETHODCALLTYPE ImplAAFRefArrayValue::GetObjectAt(
  aafUInt32 index, 
  ImplAAFStorable** ppObject) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == ppObject)
    return AAFRESULT_NULL_PARAM;
  *ppObject = NULL;
  
  // Validate the index...  
  OMReferenceVectorProperty * pReferenceVectorProperty = referenceVectorProperty();
  assert (NULL != pReferenceVectorProperty);
  aafUInt32 elementCount = pReferenceVectorProperty->count();
  if (index >= elementCount)
    return AAFRESULT_BADINDEX;
    
  OMObject *object = pReferenceVectorProperty->getObjectAt(index);
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


// Append the given pObject to the OMReferenceVectorProperty.
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
    
  OMReferenceVectorProperty * pReferenceVectorProperty = referenceVectorProperty();
  assert (NULL != pReferenceVectorProperty);
  pReferenceVectorProperty->appendObject(storable);

  if (usesReferenceCounting())
  {
    pObject->AcquireReference();
  }
  
  return result;
}  


// Prepend the given pObject> to the OMReferenceVectorProperty.
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
    
  OMReferenceVectorProperty * pReferenceVectorProperty = referenceVectorProperty();
  assert (NULL != pReferenceVectorProperty);
  pReferenceVectorProperty->prependObject(storable);

  if (usesReferenceCounting())
  {
    pObject->AcquireReference();
  }
  
  return result;
}  


// Remove the object from the OMReferenceVectorProperty at position index.
// Existing objects in the OMReferenceVectorProperty at index + 1 and higher 
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
  OMReferenceVectorProperty * pReferenceVectorProperty = referenceVectorProperty();
  assert (NULL != pReferenceVectorProperty);
  aafUInt32 elementCount = pReferenceVectorProperty->count();
  if (index >= elementCount)
    return AAFRESULT_BADINDEX;
    
    
  OMObject *oldStorable = pReferenceVectorProperty->removeObjectAt(index);
  if (usesReferenceCounting())
  {
    if (oldStorable)
      ReleaseOldObject(oldStorable);
  }
  
  return result;
}  

// Insert pObject into the OMReferenceVectorProperty at position index. 
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
  OMReferenceVectorProperty * pReferenceVectorProperty = referenceVectorProperty();
  assert (NULL != pReferenceVectorProperty);
  aafUInt32 elementCount = pReferenceVectorProperty->count();
  if (index > elementCount) // it is valid to insert after the last element
    return AAFRESULT_BADINDEX;

  pReferenceVectorProperty->insertObjectAt(pObject, index);
  
  if (usesReferenceCounting())
  {
    pObject->AcquireReference();
  }

  
  return result;
}  


// Set the value of the OMReferenceVectorProperty at position index to the
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


// Get the value of the OMReferenceVectorProperty at position index.
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


// Append the given pObject to the OMReferenceVectorProperty.
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


// Prepend the given pObject> to the OMReferenceVectorProperty.
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

// Insert pObject into the OMReferenceVectorProperty at position index. 
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
  

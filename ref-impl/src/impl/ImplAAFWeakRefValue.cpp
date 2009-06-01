//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFWeakRefValue.h"

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

// TBD: Note the following include for ImplAAFClassDef.h really belongs
// in ImplAAFTypeDefWeakObjRef.h!
#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif


#include "OMProperty.h"
//#include "OMRefProperty.h" // TBD: include header for class for singleton strong references.

#include "OMAssertions.h"
#include <string.h>


ImplAAFWeakRefValue::ImplAAFWeakRefValue ()
{}


ImplAAFWeakRefValue::~ImplAAFWeakRefValue ()
{
}



AAFRESULT ImplAAFWeakRefValue::Initialize (
  const ImplAAFTypeDefWeakObjRef *referenceType)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  ASSERTU (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
    
  result = ImplAAFRefValue::Initialize(referenceType);
  if (AAFRESULT_SUCCEEDED(result))
  {
    // This instance is now fully initialized.
    setInitialized();
  }
  return result;
}


AAFRESULT ImplAAFWeakRefValue::Initialize (
  const ImplAAFTypeDefWeakObjRef *referenceType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  result = ImplAAFRefValue::Initialize(referenceType, property);
  
  if (AAFRESULT_SUCCEEDED(result))
  {
    // This instance is now fully initialized.
    setInitialized();
  }
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE ImplAAFWeakRefValue::GetObject(ImplAAFStorable **ppObject) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFStorable *pObject = NULL;
  
  if (NULL == ppObject)
    return AAFRESULT_NULL_PARAM;
  
  *ppObject = NULL;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  OMReferenceProperty* pProperty =  referenceProperty();
  if (NULL != pProperty)
  {
    if (! pProperty->isOptional() || pProperty->isPresent ())
    {
      OMObject *object = pProperty->getObject();
      if (NULL == object)
        return AAFRESULT_NULLOBJECT;
      pObject = ConvertOMObjectToRoot(object); 
      ASSERTU (NULL != pObject);
      if (NULL == pObject)
        return AAFRESULT_INVALID_OBJ; // ???
    }
    else
    {
      return AAFRESULT_PROP_NOT_PRESENT;
    }
  }
  else
  {
    // Use an "indirect access" method of retrieving a strong object reference.
    //
    // If there was not associated reference property then there MUST
    // be a local object pointer assigned to this instance.
    pObject = GetLocalObject();
    ASSERTU (NULL != pObject);
  }
  
  *ppObject = pObject;
  (*ppObject)->AcquireReference();
  
  return result;
}

static AAFRESULT SetNewObjectReference(
  OMReferenceProperty* refProperty,
  ImplAAFStorable* pNewObject)
{   
  ASSERTU (NULL != pNewObject);
  if (NULL == pNewObject)
    return AAFRESULT_INVALID_OBJ; // ???

  // TODO: Validation
  // The given storable must already be in the target strong reference set!

  // Save the reference in the OMReferenceProperty
  refProperty->setObject(pNewObject);
  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFWeakRefValue::SetObject(ImplAAFStorable *pObject)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;

  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  if (!pObject->attached())
  {
    return AAFRESULT_OBJECT_NOT_ATTACHED;
  }
 

  if (NULL != referenceProperty())
  {
    // Save the reference in the OMReferenceProperty
    result = SetNewObjectReference(referenceProperty(), pObject);
  }
  else
  {
    // Use an "indirect access" method of saving a weak object reference.
    //
    // If there was not associated reference property then we MUST
    // set the local object pointer for this instance.
    //
    // This weak reference should be an element of a weak reference set or
    // weak reference array.
    SetLocalObject(pObject);
  }
  
  
  return result;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFWeakRefValue::WriteTo(
  OMProperty* pOmProp)
{
  ASSERTU (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  AAFRESULT result = ImplAAFRefValue::WriteTo(pOmProp);
  if (AAFRESULT_SUCCEEDED(result))
  {
    // If there is a referenceProperty then the SetObject method has already
    // written the object reference to the property.
  
    if (NULL == referenceProperty())
    {
      OMReferenceProperty* refProperty = dynamic_cast<OMReferenceProperty *>(pOmProp);
      ASSERTU(refProperty);
      if (NULL == refProperty)
        return AAFRESULT_INVALID_OBJ; // ???

      // Use an "indirect access" method of saving a strong object reference.
      // The local object reference must exist! NOTE: GetLocalObject() does NOT increment
      // the reference count of the returned object!
      result = SetNewObjectReference(refProperty, GetLocalObject());
      if (AAFRESULT_SUCCEEDED(result))
      {
        // Install the property direct access and cleanup the local reference.
        SetProperty(pOmProp);
        SetLocalObject(NULL);
      }
    }
  }
  
  return result;
}

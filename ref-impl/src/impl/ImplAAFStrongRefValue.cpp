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

#include "ImplAAFStrongRefValue.h"


#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

// TBD: Note the following include for ImplAAFClassDef.h really belongs
// in ImplAAFTypeDefStrongObjRef.h!
#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif


#include "OMProperty.h"
#include "OMRefProperty.h" // Include header for base class for singleton references.

#include <assert.h>
#include <string.h>



ImplAAFStrongRefValue::ImplAAFStrongRefValue ()
{}


ImplAAFStrongRefValue::~ImplAAFStrongRefValue ()
{
}



AAFRESULT ImplAAFStrongRefValue::Initialize (
  const ImplAAFTypeDefStrongObjRef *referenceType)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  assert (!isInitialized());
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


AAFRESULT ImplAAFStrongRefValue::Initialize (
  const ImplAAFTypeDefStrongObjRef *referenceType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  assert (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
    
  result = ImplAAFRefValue::Initialize(referenceType, property);
  if (AAFRESULT_SUCCEEDED(result))
  {
    // This instance is now fully initialized.
    setInitialized();
  }
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE ImplAAFStrongRefValue::GetObject(ImplAAFStorable **ppObject) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFStorable *pObject = NULL;
  
  if (NULL == ppObject)
    return AAFRESULT_NULL_PARAM;
  
  *ppObject = NULL;

  assert (isInitialized());
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
      assert (NULL != pObject);
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
    assert (NULL != pObject);
  }
  
  *ppObject = pObject;
  (*ppObject)->AcquireReference();
  
  return result;
}

static AAFRESULT SetNewObjectReference(
  OMReferenceProperty* refProperty,
  ImplAAFStorable* pNewObject)
{   
  assert (NULL != pNewObject);
  if (NULL == pNewObject)
    return AAFRESULT_INVALID_OBJ; // ???
  OMObject *object = refProperty->setObject(pNewObject);

  // The new object reference is now owned by its containing object
  // so we need to increase the reference count.
  pNewObject->AcquireReference();

  // Release the old object reference if necessary...
  if (NULL != object)
  {    
    ImplAAFStorable *pOldObject = ImplAAFRefValue::ConvertOMObjectToRoot(object); 
    assert (NULL != pOldObject);
    if (NULL == pOldObject)
      return AAFRESULT_INVALID_OBJ; // ???
    
    pOldObject->ReleaseReference();
  }  
  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE ImplAAFStrongRefValue::SetObject(ImplAAFStorable *pObject)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  if (NULL != referenceProperty())
  {
    // Save the reference in the OMReferenceProperty
    result = SetNewObjectReference(referenceProperty(), pObject);
  }
  else
  {
    // Use an "indirect access" method of saving a strong object reference.
    //
    // If there was not associated reference property then we MUST
    // set the local object pointer for this instance.
    SetLocalObject(pObject);
  }
  
  
  return result;
}


//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFStrongRefValue::WriteTo(
  OMProperty* pOmProp)
{
  assert (isInitialized());
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
      assert(refProperty);
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

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

#include "ImplAAFRefValue.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif


#include "OMProperty.h"
//#include "OMRefProperty.h" // TBD: include header for base class for singleton references.
#include "OMPropertyDefinition.h"

#include <assert.h>
#include <string.h>


ImplAAFRefValue::ImplAAFRefValue () :
  _referenceProperty(NULL),
  _propertyContainer(NULL)
{}


ImplAAFRefValue::~ImplAAFRefValue ()
{
  _referenceProperty = NULL;
  
#if defined(REFERENCE_PROPERTY_CONTAINER)
  if (_propertyContainer)
  {
    _propertyContainer->ReleaseReference();
    _propertyContainer = NULL;
  }
#endif // #if defined(REFERENCE_PROPERTY_CONTAINER)
}







AAFRESULT ImplAAFRefValue::Initialize (
  const ImplAAFTypeDefObjectRef *referenceType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  assert (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
  assert (property);
  if (NULL == referenceType || NULL == property)
    return AAFRESULT_NULL_PARAM;

  // Get the type definition. This must be a stream type.
  assert (property->definition());
  if (NULL == property->definition())
    return AAFRESULT_INVALID_PARAM;
  const OMType *type = property->definition()->type();
  assert (type);
  
// TBD: The given property must be an OM reference property.  
// Until the OMReferenceProperty is checked in just use the given
// property as the referenceProperty
  OMProperty *referenceProperty = property;
//  OMReferenceProperty *referenceProperty = dynamic_cast<OMReferenceProperty *>(property);
//  assert (referenceProperty);
//  if (NULL == referenceProperty)
//    return AAFRESULT_INVALID_PARAM;

  // Get the storable container for this property. Since this is a "direct 
  // access" interface we need to hold onto a reference so tha the container
  // is not deleted.
  ImplAAFRoot * propertyContainer = dynamic_cast<ImplAAFRoot *>
                                      (property->propertySet()->container());
  assert (propertyContainer);
  if (NULL == propertyContainer)
    return AAFRESULT_INVALID_PARAM;
  
  // Save our initialized member data.
  SetType(const_cast<ImplAAFTypeDefObjectRef *>(referenceType));
  _referenceProperty = referenceProperty;
  _propertyContainer = propertyContainer;
#if defined(REFERENCE_PROPERTY_CONTAINER)
  _propertyContainer->AcquireReference();
#endif // #if defined(REFERENCE_PROPERTY_CONTAINER)
    
  return AAFRESULT_SUCCESS;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFRefValue::WriteTo(
  OMProperty* pOmProp)
{
  // Make sure that the given property is the same one that was used to 
  // initialize this property value. NOTE: Copying an object reference to a 
  // different OMProperty should be handled through another interface.
  if (pOmProp != _referenceProperty)
    return AAFRESULT_INVALID_PARAM;
  
  // The first version of this class defers to the older   
  return ImplAAFPropValData::WriteTo(pOmProp);
//  return AAFRESULT_SUCCESS;
}

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

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif


#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMProperty.h"
#include "OMPropertyDefinition.h"



#include <assert.h>
#include <string.h>


ImplAAFPropertyValue::ImplAAFPropertyValue ()
  : _pType (NULL),
  _property(NULL),
  _propertyContainer(NULL)
{}


ImplAAFPropertyValue::~ImplAAFPropertyValue ()
{
  // BobT: type is not to be reference counted.
  _pType = NULL;
  _property = NULL;
  
#if defined(REFERENCE_PROPERTY_CONTAINER)
  if (_propertyContainer)
  {
    _propertyContainer->ReleaseReference();
    _propertyContainer = NULL;
  }
#endif // #if defined(REFERENCE_PROPERTY_CONTAINER)
}

void ImplAAFPropertyValue::SetProperty(OMProperty * pProperty)
{
  _property = pProperty;
}

AAFRESULT ImplAAFPropertyValue::Initialize (
	  const ImplAAFTypeDef *propertyType)
{
  if (! propertyType) 
    return AAFRESULT_NULL_PARAM;
    
  // make sure we haven't been init'd yet
  assert (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
  
  assert (! _pType);

  _pType = propertyType;
  // BobT: type is not to be reference counted.
  return AAFRESULT_SUCCESS;
}



AAFRESULT ImplAAFPropertyValue::Initialize (
  const ImplAAFTypeDef *propertyType,    
  OMProperty *property)
{
  assert (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
  assert (property);
  if (NULL == propertyType || NULL == property)
    return AAFRESULT_NULL_PARAM;

  // Get the type definition. This must be a stream type.
  assert (property->definition());
  if (NULL == property->definition())
    return AAFRESULT_INVALID_PARAM;
  const OMType *type = property->definition()->type();
  assert (type);

  // Get the storable container for this property. Since this is a "direct 
  // access" interface we need to hold onto a reference so tha the container
  // is not deleted.
  ImplAAFRoot * propertyContainer = dynamic_cast<ImplAAFRoot *>
                                      (property->propertySet()->container());
  assert (propertyContainer);
  if (NULL == propertyContainer)
    return AAFRESULT_INVALID_PARAM;
  
  // Save our initialized member data.
  SetType(propertyType);
  SetProperty(property);
  _propertyContainer = propertyContainer;
#if defined(REFERENCE_PROPERTY_CONTAINER)
  _propertyContainer->AcquireReference();
#endif // #if defined(REFERENCE_PROPERTY_CONTAINER)
    
  return AAFRESULT_SUCCESS;
}



AAFRESULT ImplAAFPropertyValue::SetType (
	  const ImplAAFTypeDef * pType)
{
  return Initialize(pType);
}

// accessor methods for subclasses:
const ImplAAFTypeDef *ImplAAFPropertyValue::type(void) const
{
  return _pType;
}



OMProperty *ImplAAFPropertyValue::property(void) const
{
  return _property;
}



ImplAAFRoot *ImplAAFPropertyValue::propertyContainer(void) const
{
  return _propertyContainer;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyValue::GetType (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *ppTypeDef = const_cast<ImplAAFTypeDef *>(type());
  assert (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyValue::IsDefinedType (
      aafBool * pIsDefined)
{
  if (! pIsDefined)
	return AAFRESULT_NULL_PARAM;
  // Currently the type returned by GetType() is always the defined type.
  *pIsDefined = kAAFTrue;
  return AAFRESULT_SUCCESS;
}


const ImplAAFTypeDef * ImplAAFPropertyValue::pvtGetType (void) const
{
  return _pType;
}

AAFRESULT STDMETHODCALLTYPE ImplAAFPropertyValue::WriteTo(
      OMProperty* /* pOmProp */)
{
  // This method should be pure virtual but dodo won't allow it
  assert(false);
  return AAFRESULT_SUCCESS;
}

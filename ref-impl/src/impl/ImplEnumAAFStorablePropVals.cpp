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

#ifndef __ImplEnumAAFStorablePropVals_h__
#include "ImplEnumAAFStorablePropVals.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFRefContainerValue_h__
#include "ImplAAFRefContainerValue.h"
#endif

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif


#include "OMReferenceContainerIter.h"


#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFStorablePropVals;

ImplEnumAAFStorablePropVals::ImplEnumAAFStorablePropVals () :
  _containerValue(NULL),
  _elementType(NULL),
  _containerIterator(NULL)
{}


ImplEnumAAFStorablePropVals::~ImplEnumAAFStorablePropVals ()
{
  // Cleanup
  if (NULL != _containerValue)
  {
    _containerValue->ReleaseReference();
    _containerValue = NULL;
  }
  
  if (_containerIterator)
  {
    delete (_containerIterator);
    _containerIterator = NULL;
  }
}


  
AAFRESULT STDMETHODCALLTYPE 
  ImplEnumAAFStorablePropVals::Initialize(
    ImplAAFRefContainerValue* containerValue,
    OMReferenceContainerIterator* containerIterator)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (NULL == containerValue || NULL == containerIterator)
    return AAFRESULT_NULL_PARAM;
    
  assert(!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
  
  _elementType = containerValue->GetElementType();
  if (NULL == _elementType)
    return AAFRESULT_INVALID_OBJ;
  
  _containerIterator = containerIterator;
  _containerValue = containerValue;
  containerValue->AcquireReference();

  // This instance is now fully initialized.
  setInitialized();
  
  return result;
}
  



  

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFStorablePropVals::NextOne (
      ImplAAFPropertyValue ** ppPropertyValue)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  assert(isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  if (NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL;
  
  if (_containerIterator->before() || _containerIterator->valid())
  {
    if (++(*_containerIterator))
    {
      OMObject* object = _containerIterator->currentObject();
      ImplAAFStorable* obj = dynamic_cast<ImplAAFStorable*>(object);
      assert(NULL != obj);
      if (NULL == obj)
        return AAFRESULT_INVALID_OBJ;
      
      // Use the container's element type to create the proper subclass
      // of object reference.
      result = _elementType->CreateValue(obj, ppPropertyValue);
    }
    else
    {
      result = AAFRESULT_NO_MORE_OBJECTS;
    }
  }
  else
  {
    result = AAFRESULT_NO_MORE_OBJECTS;
  }
  
  return result;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFStorablePropVals::Next (
      aafUInt32  count,
      ImplAAFPropertyValue ** ppPropertyValues,
      aafUInt32 *  pFetched)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  assert(isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  if (NULL == ppPropertyValues || NULL == pFetched)
    return AAFRESULT_NULL_PARAM;
  
  if (0 == count)
    return AAFRESULT_INVALID_PARAM;

  aafUInt32      numItems;
  for (numItems = 0; numItems < count; numItems++)
  {
    result = NextOne(&ppPropertyValues[numItems]);
    if (FAILED(result))
      break;
  }
  
  *pFetched = numItems;

  return result;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFStorablePropVals::Skip (
      aafUInt32  count)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  assert(isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  if (0 == count)
    return AAFRESULT_INVALID_PARAM;

  aafUInt32 n;
  
  for(n =1 ; n <= count; n++)
  {
    // Defined behavior of skip is to NOT advance at all if it would push 
    // us off of the end
    if(!++(*_containerIterator))
    {
      // Off the end, decrement n and iterator back to the starting 
      // position
      while(n>=1)
      {
        --(*_containerIterator);
        n--;
      }
      result = AAFRESULT_NO_MORE_OBJECTS;
      break;
    }
  }

  return result;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFStorablePropVals::Reset ()
{
  assert(isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
    
  _containerIterator->reset();
  
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFStorablePropVals::Clone (
      ImplEnumAAFPropertyValues ** ppEnum)
{
  AAFRESULT result = AAFRESULT_SUCCESS;  
  if (NULL == ppEnum)
    return AAFRESULT_NULL_PARAM;
  *ppEnum = NULL;

  assert(isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  ImplAAFRoot* pRoot = CreateImpl(CLSID_EnumAAFStorablePropVals);
  if (NULL == pRoot)
    return AAFRESULT_NOMEMORY;
    
  ImplEnumAAFStorablePropVals* pNewEnum = dynamic_cast<ImplEnumAAFStorablePropVals*>(pRoot);
  if (NULL != pNewEnum)
  {
    OMReferenceContainerIterator* newIterator = _containerIterator->copy();
    if (NULL != newIterator)
    {
      result = pNewEnum->Initialize(_containerValue, newIterator);
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

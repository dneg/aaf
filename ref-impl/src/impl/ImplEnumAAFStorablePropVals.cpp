/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

	aafUInt32			numItems;
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

	AAFRESULT ar=AAFRESULT_SUCCESS;
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

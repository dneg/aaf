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

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif


#include "OMProperty.h"
#include "OMRefProperty.h" // Include header for base class for singleton references.
#include "OMPropertyDefinition.h"

#include <assert.h>
#include <string.h>

#ifndef USE_IMPLSTORABLE
#define USE_IMPLSTORABLE 1
#endif

// Utility routine
/* static */ ImplAAFStorable * ImplAAFRefValue::ConvertOMObjectToRoot(OMObject *object)
{
  ImplAAFStorable* result = NULL;
#if USE_IMPLSTORABLE
  ImplAAFStorable *pObject = dynamic_cast<ImplAAFStorable *>(object);
  result = pObject;
#else
  ImplAAFObject *pObject = dynamic_cast<ImplAAFObject *>(object);
  if (NULL != pObject)
  {
    result = pObject;
  }
  else
  {
    ImplAAFMetaDefinition* metaDefinition = dynamic_cast<ImplAAFMetaDefinition *>(object);
    if (NULL != metaDefinition)
    {
      result = metaDefinition;
    }
  }
#endif  
  // A strong reference MUST be either an ImplAAFObject or an ImplAAFMetaDefintion!!!
  assert (NULL != result);
  return result;
}

/* static */ ImplAAFStorable * ImplAAFRefValue::ConvertRootToOMStorable(ImplAAFRoot *object)
{
  ImplAAFStorable* result = NULL;
#if USE_IMPLSTORABLE
  ImplAAFStorable *pObject = dynamic_cast<ImplAAFStorable *>(object);
  result = pObject;
#else
  ImplAAFObject *pObject = dynamic_cast<ImplAAFObject *>(object);
  if (NULL != pObject)
  {
    result = pObject;
  }
  else
  {
    ImplAAFMetaDefinition* metaDefinition = dynamic_cast<ImplAAFMetaDefinition *>(object);
    if (NULL != metaDefinition)
    {
      result = metaDefinition;
    }
  }
#endif  
  
  // A strong reference MUST be either an ImplAAFObject or an ImplAAFMetaDefintion!!!
  assert (NULL != result);
  return result;
}

ImplAAFRefValue::ImplAAFRefValue () :
  _localObject(NULL)
{}


ImplAAFRefValue::~ImplAAFRefValue ()
{
  // Release the old object (if necessary)
  SetLocalObject(NULL);
}



AAFRESULT ImplAAFRefValue::Initialize (
  const ImplAAFTypeDefObjectRef *referenceType)
{
  AAFRESULT result = ImplAAFPropertyValue::Initialize(referenceType);
  return result;
}


AAFRESULT ImplAAFRefValue::Initialize (
  const ImplAAFTypeDefObjectRef *referenceType,    
  OMProperty *property)
{
  AAFRESULT result = ImplAAFPropertyValue::Initialize(referenceType, property);
  return result;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFRefValue::WriteTo(
  OMProperty* pOmProp)
{
  if (NULL != property())
  {
    // Make sure that the given property is the same one that was used to 
    // initialize this property value. NOTE: Copying an object reference to a 
    // different OMProperty should be handled through another interface.
    if (pOmProp != property())
      return AAFRESULT_INVALID_PARAM;
  }
  
  // The first version of this class defers to the older   
//  return ImplAAFPropValData::WriteTo(pOmProp);
  return AAFRESULT_SUCCESS;
}


// Return the instance's property as a reference property.
OMReferenceProperty * ImplAAFRefValue::referenceProperty(void) const
{
  return (static_cast<OMReferenceProperty *>(property()));
}


void ImplAAFRefValue::SetLocalObject(ImplAAFStorable * localObject)
{
  if (NULL != _localObject)
    _localObject->ReleaseReference();

  _localObject = localObject;

  if (NULL != localObject)
    localObject->AcquireReference();
}


ImplAAFStorable * ImplAAFRefValue::GetLocalObject(void) const // not reference counted
{
  return _localObject;
}

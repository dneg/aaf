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

#include "ImplAAFStrongRefSetValue.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

// TBD: Note the following include for ImplAAFClassDef.h really belongs
// in ImplAAFTypeDefStrongObjRef.h!
#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif


#include "OMProperty.h"

#include <assert.h>
#include <string.h>


ImplAAFStrongRefSetValue::ImplAAFStrongRefSetValue ()
{}


ImplAAFStrongRefSetValue::~ImplAAFStrongRefSetValue ()
{
}







AAFRESULT ImplAAFStrongRefSetValue::Initialize (
  const ImplAAFTypeDefSet *containerType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  
  result = ImplAAFRefSetValue::Initialize(containerType, property);
  
  if (AAFRESULT_SUCCEEDED(result))
  {
    // This instance is now fully initialized.
    setInitialized();
  }
  
  return result;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFStrongRefSetValue::WriteTo(
  OMProperty* pOmProp)
{
  return (ImplAAFRefSetValue::WriteTo(pOmProp));
}




  
//
// ImplAAFRefContainer overrides
//

// Return the type of object references in the container.
ImplAAFTypeDefObjectRef * ImplAAFStrongRefSetValue::GetElementType(void) const // the result is NOT reference counted.
{
  ImplAAFTypeDefObjectRef * pContainerElementType = NULL;
  ImplAAFTypeDefSet *pContainerType = NULL;
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFTypeDefSP pType, pElementType;
  
  result = GetType(&pType);
  assert(AAFRESULT_SUCCEEDED(result));
  if (AAFRESULT_SUCCEEDED(result))
  {
    pContainerType = dynamic_cast<ImplAAFTypeDefSet *>((ImplAAFTypeDef *)pType); // extract obj from smartptr
    assert(NULL != pContainerType);
    if (NULL != pContainerType)
    {
      result = pContainerType->GetElementType(&pElementType);
      assert(AAFRESULT_SUCCEEDED(result));
      if (AAFRESULT_SUCCEEDED(result))
      {
        pContainerElementType = dynamic_cast<ImplAAFTypeDefStrongObjRef *>((ImplAAFTypeDef *)pElementType); // extract obj from smartptr
      }
    }
  }
  
  assert(pContainerElementType);
  return pContainerElementType;
}


// Perform specialized validation for an object before it is added
// to a container.
AAFRESULT ImplAAFStrongRefSetValue::ValidateNewObject(ImplAAFStorable *pNewObject) const
{
  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  assert(NULL != pNewObject);
  if (pNewObject->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  // Hand off to the OMReferenceContainer
  OMReferenceContainer* pReferenceContainer = referenceContainer();

  // Object refernce containers should only contain a single reference
  // to an object.
  if (pReferenceContainer->containsObject(pNewObject))
  {
    return AAFRESULT_TABLE_DUP_KEY; // Object is already in the set!
  }

  return AAFRESULT_SUCCESS;
}


// Perform any specialized cleanup of any object after it has been removed
// from the container.
bool ImplAAFStrongRefSetValue::usesReferenceCounting(void) const
{
  return true;
}
  

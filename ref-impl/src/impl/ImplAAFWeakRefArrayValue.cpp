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

#include "ImplAAFWeakRefArrayValue.h"

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

// TBD: Note the following include for ImplAAFClassDef.h really belongs
// in ImplAAFTypeDefWeakObjRef.h!
#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFTypeDefArray_h__
#include "ImplAAFTypeDefArray.h"
#endif


#include "OMProperty.h"

#include <assert.h>
#include <string.h>


ImplAAFWeakRefArrayValue::ImplAAFWeakRefArrayValue ()
{}


ImplAAFWeakRefArrayValue::~ImplAAFWeakRefArrayValue ()
{
}





AAFRESULT ImplAAFWeakRefArrayValue::Initialize (
	  const ImplAAFTypeDefArray *containerType,
	  bool fixed)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  result = ImplAAFRefArrayValue::Initialize(containerType, fixed);
  
  if (AAFRESULT_SUCCEEDED(result))
  {
    // This instance is now fully initialized.
    setInitialized();
  }
  
  return result;
}


AAFRESULT ImplAAFWeakRefArrayValue::Initialize (
  const ImplAAFTypeDefArray *containerType,    
  OMProperty *property,
  bool fixedSize)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  result = ImplAAFRefArrayValue::Initialize(containerType, property, fixedSize);
  
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
AAFRESULT STDMETHODCALLTYPE ImplAAFWeakRefArrayValue::WriteTo(
  OMProperty* pOmProp)
{
  return (ImplAAFRefArrayValue::WriteTo(pOmProp));
}




  
//
// ImplAAFRefContainer overrides
//

// Return the type of object references in the container.
ImplAAFTypeDefObjectRef * ImplAAFWeakRefArrayValue::GetElementType(void) const // the result is NOT reference counted.
{
  ImplAAFTypeDefObjectRef * pContainerElementType = NULL;
  ImplAAFTypeDefArray *pContainerType = NULL;
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFTypeDefSP pType, pElementType;
  
  result = GetType(&pType);
  assert(AAFRESULT_SUCCEEDED(result));
  if (AAFRESULT_SUCCEEDED(result))
  {
    pContainerType = dynamic_cast<ImplAAFTypeDefArray *>((ImplAAFTypeDef *)pType); // extract obj from smartptr
    assert(NULL != pContainerType);
    if (NULL != pContainerType)
    {
      result = pContainerType->GetType(&pElementType);
      assert(AAFRESULT_SUCCEEDED(result));
      if (AAFRESULT_SUCCEEDED(result))
      {
        pContainerElementType = dynamic_cast<ImplAAFTypeDefWeakObjRef *>((ImplAAFTypeDef *)pElementType); // extract obj from smartptr
      }
    }
  }
  
  assert(pContainerElementType);
  return pContainerElementType;
}


// Perform specialized validation for an object before it is added
// to a container.
AAFRESULT ImplAAFWeakRefArrayValue::ValidateNewObject(ImplAAFStorable *pNewObject) const
{
  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  assert(NULL != pNewObject);
  if (!pNewObject->attached())
  {
    return AAFRESULT_OBJECT_NOT_ATTACHED;
  }

  return AAFRESULT_SUCCESS;
}


// Perform any specialized cleanup of any object after it has been removed
// from the container.
bool ImplAAFWeakRefArrayValue::usesReferenceCounting(void) const
{
  return false;
}
  

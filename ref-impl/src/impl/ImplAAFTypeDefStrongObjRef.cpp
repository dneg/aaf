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
#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif


#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFStrongRefValue_h__
#include "ImplAAFStrongRefValue.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFDictionary.h"


#include <assert.h>
#include <string.h>


extern "C" const aafClassID_t CLSID_AAFPropValData;
extern "C" const aafClassID_t CLSID_AAFStrongRefValue;

ImplAAFTypeDefStrongObjRef::ImplAAFTypeDefStrongObjRef ()
  : _referencedType ( PID_TypeDefinitionStrongObjectReference_ReferencedType,
                      L"ReferencedType",
                      L"/MetaDictionary/ClassDefinitions",
                      PID_MetaDefinition_Identification)
{
  _persistentProperties.put(_referencedType.address());
}


ImplAAFTypeDefStrongObjRef::~ImplAAFTypeDefStrongObjRef ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::Initialize (
      const aafUID_t & id,
      ImplAAFClassDef * pObjType,
      const aafCharacter * pTypeName)
{
  if (! pObjType)  return AAFRESULT_NULL_PARAM;
  if (! pTypeName)  return AAFRESULT_NULL_PARAM;
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
    
  // The given class definition must be in the directory.
  aafUID_t classID;
  AAFRESULT result = pObjType->GetAUID(&classID);
  if (AAFRESULT_SUCCEEDED(result))
  {
    ImplAAFDictionarySP pDictionary;
    result = GetDictionary(&pDictionary);
    if (AAFRESULT_SUCCEEDED(result))
    {
      ImplAAFClassDefSP pClassDef;
      result = pDictionary->LookupClassDef(classID, &pClassDef);
      if (AAFRESULT_SUCCEEDED(result))
      {
         result = pvtInitialize (id, pObjType, pTypeName);
      }
      else if (AAFRESULT_NO_MORE_OBJECTS == result)
      {
        result = AAFRESULT_CLASS_NOT_FOUND;
      }
    }
  }
  
  return result;
}


// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::pvtInitialize (
      const aafUID_t & id,
      const ImplAAFClassDef * pClassDef,
      const aafCharacter * pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  _referencedType = pClassDef;

  // This instance is now fully initialized.
  setInitialized();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::SetObject (ImplAAFPropertyValue * pPropVal,
										   ImplAAFRoot * pObj)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pObj) return AAFRESULT_NULL_PARAM;
  if (! isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  // Clients can only make strong references to data objects (not type or class definitions!)
  // transdel:2000-JUN-29
  ImplAAFObject *pObject = dynamic_cast<ImplAAFObject*>(pObj);
  if (NULL == pObject)
    return AAFRESULT_INVALID_PARAM;
  
  // If the given property value is a "direct" strong reference property
  // then just set the object directly.
  ImplAAFStrongRefValue* pStrongRefValue = dynamic_cast<ImplAAFStrongRefValue*>(pPropVal);
  if (NULL != pStrongRefValue)
  {
    return pStrongRefValue->SetObject(pObject);
  }

  OMStorable ** ppStorable = NULL;
  AAFRESULT hr;
  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  assert (pvd);

  hr = pvd->AllocateBits (sizeof (OMStorable*), (aafMemPtr_t*) &ppStorable);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (ppStorable);
  if (*ppStorable)
	{
	  // An object was already here.  Release it before we trash the
	  // reference to it.
	  ImplAAFObject * tmp = dynamic_cast<ImplAAFObject*>(*ppStorable);
	  assert (tmp);
	  tmp->ReleaseReference ();
	  tmp = 0;
	  *ppStorable = 0;
	}

  assert (! *ppStorable);
  *ppStorable = pObject;
  pObject->AcquireReference ();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefStrongObjRef::GetObject (ImplAAFPropertyValue * pPropVal,
									   ImplAAFRoot ** ppObject)
{
  AAFRESULT hr = AAFRESULT_SUCCESS;
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppObject) return AAFRESULT_NULL_PARAM;
  if (! isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  *ppObject = NULL;
  
  // If the given property value is a "direct" strong reference property
  // then just get the object directly.
  ImplAAFStrongRefValue* pStrongRefValue = dynamic_cast<ImplAAFStrongRefValue*>(pPropVal);
  if (NULL != pStrongRefValue)
  {
    ImplAAFStorable *pObject = NULL;
    hr = pStrongRefValue->GetObject((ImplAAFStorable **)&pObject);
    if (AAFRESULT_SUCCEEDED(hr))
      *ppObject = pObject;
    return hr;
  }

  OMStorable ** ppStorable = NULL;
  aafUInt32 bitsSize = 0;
  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  assert (pvd);

  hr = pvd->GetBitsSize (&bitsSize);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (bitsSize >= sizeof (ImplAAFRoot*));
  hr = pvd->GetBits ((aafMemPtr_t*) &ppStorable);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (*ppStorable);
  assert (ppObject);
  ImplAAFRoot * pObj;
  pObj = dynamic_cast<ImplAAFRoot*>(*ppStorable);
  assert (pObj);
  *ppObject = pObj;
  (*ppObject)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::GetObjectType (ImplAAFClassDef ** ppObjType)
{
  if (! ppObjType)
	return AAFRESULT_NULL_PARAM;
  if (! isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

   if(_referencedType.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFClassDef *pClassDef = _referencedType;

  *ppObjType = pClassDef;
  assert (*ppObjType);
  (*ppObjType)->AcquireReference ();
  return AAFRESULT_SUCCESS;

}

// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::CreateValue (/*[in]*/ ImplAAFRoot * pObj,
      /*[out]*/ ImplAAFPropertyValue ** ppPropVal)
{
  if (! pObj)
	return AAFRESULT_NULL_PARAM;
  if (! ppPropVal)
	return AAFRESULT_NULL_PARAM;
  if (! isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

#if 1

  ImplAAFStrongRefValue *pStrongRefValue = NULL;
  pStrongRefValue = (ImplAAFStrongRefValue*) CreateImpl (CLSID_AAFStrongRefValue);
  if (!pStrongRefValue) 
    return AAFRESULT_NOMEMORY;

  // Attempt to initialize the strong reference value. This "should" fail if the given 
  // property is not a valid strong reference property.
  AAFRESULT result = pStrongRefValue->Initialize (this);//, property);
  if (AAFRESULT_SUCCEEDED(result))
  {
    result = this->SetObject(pStrongRefValue, pObj);
    if (AAFRESULT_SUCCEEDED(result))
    {
      
    	*ppPropVal = pStrongRefValue; // The reference count is already 1.
    	pStrongRefValue = NULL;
    }
  }

  if (AAFRESULT_FAILED(result))
  {
    pStrongRefValue->ReleaseReference();
    return result;
  }
  
  return result;

#else

  ImplAAFPropValData * pvd = 0;
  pvd = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!pvd) return AAFRESULT_NOMEMORY;

  ImplAAFPropValDataSP spPvd;
  spPvd = pvd;
  // SmartPointer operator= will automatically
  // AddRef; CreateImpl *also* will addref, so we've got one too
  // many.  Put us back to normal.
  pvd->ReleaseReference ();
  pvd = 0;

  AAFRESULT hr;
  hr = spPvd->Initialize (this);
  if (AAFRESULT_FAILED (hr)) return hr;

  hr = SetObject (spPvd, pObj);
  if (AAFRESULT_FAILED (hr))
	return hr;
  assert (ppPropVal);
  *ppPropVal = spPvd;
  assert (*ppPropVal);
  (*ppPropVal)->AcquireReference ();
  return AAFRESULT_SUCCESS;
  
#endif
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatStrongObjRef;
  return AAFRESULT_SUCCESS;
}




// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefStrongObjRef::CreatePropertyValue(
    OMProperty *property,
    ImplAAFPropertyValue ** ppPropertyValue ) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  assert (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
  
  OMReferenceProperty* refProperty = dynamic_cast<OMReferenceProperty*>(property);
  assert(NULL != refProperty);
  if (NULL == refProperty)
    return AAFRESULT_INVALID_PARAM;
 
  ImplAAFStrongRefValue *pStrongRefValue = NULL;
  pStrongRefValue = (ImplAAFStrongRefValue*) CreateImpl (CLSID_AAFStrongRefValue);
  if (!pStrongRefValue) 
    return AAFRESULT_NOMEMORY;

  // Attempt to initialize the strong reference value. This "should" fail if the given 
  // property is not a valid strong reference property.
  result = pStrongRefValue->Initialize (this);//, property);
  if (AAFRESULT_SUCCEEDED (result))
  {
    // Bobt hack! This should be removed once we have proper
    // integration with OM property def support.
    if (! property->isOptional() || property->isPresent ())
    {
      // set the storage in the prop value
      OMObject* object = refProperty->getObject();
#if 1
	  // This code can be called during lazy initialization of the
	  // property set. The initialization takes place the first time
	  // any property of this object is given a value.
	  // That means we can't assert that this particular property always
	  // has a value - tjb 01/30/02.
	  ImplAAFStorable* pObject = 0;
	  if (object != 0)
		{
		  pObject = ImplAAFRefValue::ConvertOMObjectToRoot(object);
		}
#else
      assert (NULL != object);
      ImplAAFStorable* pObject = ImplAAFRefValue::ConvertOMObjectToRoot(object);
#endif
      result = pStrongRefValue->SetObject(pObject);
    }
  }

  if (AAFRESULT_SUCCEEDED(result))
  {
  	*ppPropertyValue = pStrongRefValue; // The reference count is already 1.
  	pStrongRefValue = NULL;
  }
  else
  {
    pStrongRefValue->ReleaseReference();
  }
  return result;
}


aafBool ImplAAFTypeDefStrongObjRef::IsFixedSize (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefStrongObjRef::PropValSize (void) const
{
  return sizeof (ImplAAFObject*);
}


aafBool ImplAAFTypeDefStrongObjRef::IsRegistered (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefStrongObjRef::NativeSize (void) const
{
  return sizeof (ImplAAFObject*);
}


OMProperty * ImplAAFTypeDefStrongObjRef::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  assert (name);

  OMProperty * result = new OMStrongReferenceProperty<ImplAAFObject> (pid, name);
  assert (result);
  return result;
}






// override from OMStorable.
const OMClassId& ImplAAFTypeDefStrongObjRef::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefStrongObjRef));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefStrongObjRef::onSave(void* clientContext) const
{
  ImplAAFTypeDefObjectRef::onSave(clientContext);
}

void ImplAAFTypeDefStrongObjRef::onRestore(void* clientContext) const
{
  ImplAAFTypeDefObjectRef::onRestore(clientContext);
}

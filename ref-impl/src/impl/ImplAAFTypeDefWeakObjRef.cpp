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


#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFWeakRefValue_h__
#include "ImplAAFWeakRefValue.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefRename_h__
#include "ImplAAFTypeDefRename.h"
#endif

#ifndef __ImplAAFTypeDefVariableArray_h__
#include "ImplAAFTypeDefVariableArray.h"
#endif

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#ifndef __AAFClassDefUIDs_h__
#include "AAFClassDefUIDs.h"
#endif

#include "ImplAAFDictionary.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyDefs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>



extern "C" const aafClassID_t CLSID_AAFWeakRefValue;

ImplAAFTypeDefWeakObjRef::ImplAAFTypeDefWeakObjRef ()
  : _referencedType ( PID_TypeDefinitionWeakObjectReference_ReferencedType, 
                      L"ReferencedType", 
                      L"/MetaDictionary/ClassDefinitions", 
                      PID_MetaDefinition_Identification),
    _targetSet     ( PID_TypeDefinitionWeakObjectReference_TargetSet,
                      L"TargetSet"),
    _targetPids(NULL),
    _targetPidCount(0),
    _uniqueIdentifierPid(0)
{
  _persistentProperties.put(_referencedType.address());
  _persistentProperties.put(_targetSet.address());
}


ImplAAFTypeDefWeakObjRef::~ImplAAFTypeDefWeakObjRef ()
{
  if (NULL != _targetPids)
  {
    delete [] _targetPids;
    _targetPids = NULL;
    _targetPidCount = 0;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::Initialize (
      const aafUID_t & id,
      ImplAAFClassDef * pObjType,
      const aafCharacter * pTypeName,
      aafUInt32  ids,
      aafUID_constptr  pTargetSet)
{
  if (! pObjType)  return AAFRESULT_NULL_PARAM;
  if (! pTypeName)  return AAFRESULT_NULL_PARAM;
  if (! pTargetSet)  return AAFRESULT_NULL_PARAM;
  if (0 == ids)
    return AAFRESULT_INVALID_PARAM;

  AAFRESULT result = pvtInitialize (id, pObjType, pTypeName, ids, pTargetSet);
  if (AAFRESULT_FAILED(result))
    return result;
  

  return result;
}


aafUInt32 ImplAAFTypeDefWeakObjRef::GetTargetPidCount(void) const
{
  if (0 == _targetPidCount)
  {
    AAFRESULT rc = const_cast<ImplAAFTypeDefWeakObjRef *>(this)->SyncTargetPidsFromTargetSet();
    if (AAFRESULT_FAILED(rc))
      return 0;
  }
  return _targetPidCount;
}

const OMPropertyId * ImplAAFTypeDefWeakObjRef::GetTargetPids(void) const
{
  if (NULL == _targetPids)
  {
    AAFRESULT rc = const_cast<ImplAAFTypeDefWeakObjRef *>(this)->SyncTargetPidsFromTargetSet();
    if (AAFRESULT_FAILED(rc))
      return NULL;
  }
  return _targetPids;
}


OMPropertyId ImplAAFTypeDefWeakObjRef::GetUniqueIdentifierPid(void) const
{
  if (0 == _uniqueIdentifierPid)
  {
    AAFRESULT rc = const_cast<ImplAAFTypeDefWeakObjRef *>(this)->SyncTargetPidsFromTargetSet();
    if (AAFRESULT_FAILED(rc))
      return 0;
  }
  return _uniqueIdentifierPid;
}

// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::pvtInitialize (
      const aafUID_t & id,
      const ImplAAFClassDef * pClassDef,
      const aafCharacter * pTypeName,
      aafUInt32  ids,
      aafUID_constptr  pTargetSet,
      OMPropertyId * targetPids,
      OMPropertyId uniqueIdentifierPid)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
  if (AAFRESULT_FAILED (hr))
    return hr;

  _referencedType = pClassDef;
  

  // Do not validate the given target hint until registration. If we attempt
  // to validate at this point the corresponding property definitions might
  // not be in the dictionary yet (one of the properties may be on a forward
  // reference class). 
  
  // Copy the given target hint array to the variable sized property.
  _targetSet.setElementValues(pTargetSet, ids);

	// Axiomatic classes can also call this method with the pTargetSet already
	// converted to the targetPids array and the uniqueIdentifierPid defined.
	if (NULL != targetPids)
	{
		_targetPids = new OMPropertyId[ids + 1]; // allocate space for the null pid.
		if (NULL == _targetPids)
			return AAFRESULT_NOMEMORY;
		
		for (aafUInt32 i = 0; i <= ids; i++)
		{
			_targetPids[i] = targetPids[i];
		}
	}

	if (0 != _uniqueIdentifierPid)
	{
		_uniqueIdentifierPid = uniqueIdentifierPid;
	}

  // This instance is now fully initialized.
  setInitialized();


  return AAFRESULT_SUCCESS;
}

// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefWeakObjRef::CreateValue (/*[in]*/ ImplAAFRoot * pObj,
    /*[out]*/ ImplAAFPropertyValue ** ppPropVal)
{
  if (! pObj)
	return AAFRESULT_NULL_PARAM;
  if (! ppPropVal)
	return AAFRESULT_NULL_PARAM;
  if (! isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  ImplAAFWeakRefValue *pWeakRefValue = NULL;
  pWeakRefValue = (ImplAAFWeakRefValue*) CreateImpl (CLSID_AAFWeakRefValue);
  if (!pWeakRefValue) 
    return AAFRESULT_NOMEMORY;

  // Attempt to initialize the strong reference value. This "should" fail if the given 
  // property is not a valid strong reference property.
  AAFRESULT result = pWeakRefValue->Initialize (this);//, property());
  if (AAFRESULT_SUCCEEDED(result))
  {
    result = this->SetObject(pWeakRefValue, pObj);
    if (AAFRESULT_SUCCEEDED(result))
    {
    	*ppPropVal = pWeakRefValue; // The reference count is already 1.
    	pWeakRefValue = NULL;
    }
  }

  if (AAFRESULT_FAILED(result))
  {
    pWeakRefValue->ReleaseReference();
    return result;
  }
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::SetObject (ImplAAFPropertyValue * pPropVal,
										   ImplAAFRoot * pObj)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pObj) return AAFRESULT_NULL_PARAM;

  if (! isInitialized())
    return AAFRESULT_NOT_INITIALIZED;

  ImplAAFStorable *pObject = dynamic_cast<ImplAAFStorable*>(pObj);
  if (NULL == pObject)
    return AAFRESULT_INVALID_PARAM;

  
  // If the given property value is a "direct" strong reference property
  // then just set the object directly.
  ImplAAFWeakRefValue* pWeakRefValue = dynamic_cast<ImplAAFWeakRefValue*>(pPropVal);
  if (NULL != pWeakRefValue)
  {
    return pWeakRefValue->SetObject(pObject);
  }
  else
  {
    return AAFRESULT_INVALID_PARAM;
  }
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefWeakObjRef::GetObject (ImplAAFPropertyValue * pPropVal,
									   ImplAAFRoot ** ppObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppObject) return AAFRESULT_NULL_PARAM;
  *ppObject = NULL;

  if (! isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  // If the given property value is a "direct" strong reference property
  // then just get the object directly.
  ImplAAFWeakRefValue* pWeakRefValue = dynamic_cast<ImplAAFWeakRefValue*>(pPropVal);
  if (NULL != pWeakRefValue)
  {
    ImplAAFStorable *pObject = NULL;
    AAFRESULT hr = pWeakRefValue->GetObject((ImplAAFStorable **)&pObject);
    if (AAFRESULT_SUCCEEDED(hr))
      *ppObject = pObject;
    return hr;
  }
  else
  {
    return AAFRESULT_INVALID_PARAM;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::GetObjectType (ImplAAFClassDef ** ppObjType)
{
  if (! ppObjType)
	return AAFRESULT_NULL_PARAM;

   if(_referencedType.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFClassDef *pClassDef = _referencedType;

  *ppObjType = pClassDef;
  assert (*ppObjType);
  (*ppObjType)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatWeakObjRef;
  return AAFRESULT_SUCCESS;
}




// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefWeakObjRef::CreatePropertyValue(
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
 
  ImplAAFWeakRefValue *pWeakRefValue = NULL;
  pWeakRefValue = (ImplAAFWeakRefValue*) CreateImpl (CLSID_AAFWeakRefValue);
  if (!pWeakRefValue) 
    return AAFRESULT_NOMEMORY;

  // Attempt to initialize the strong reference value. This "should" fail if the given 
  // property is not a valid strong reference property.
  result = pWeakRefValue->Initialize (this, property);

#if 0
  if (AAFRESULT_SUCCEEDED (result))
  {
    // Bobt hack! This should be removed once we have proper
    // integration with OM property def support.
    if (! property->isOptional() || property->isPresent ())
    {
      // set the storage in the prop value
      OMObject* object = refProperty->getObject();
      assert (NULL != object);
      ImplAAFStorable* pObject = ImplAAFRefValue::ConvertOMObjectToRoot(object);
      result = pWeakRefValue->SetObject(pObject);
    }
  }
#endif


  if (AAFRESULT_SUCCEEDED(result))
  {
  	*ppPropertyValue = pWeakRefValue; // The reference count is already 1.
  	pWeakRefValue = NULL;
  }
  else
  {
    pWeakRefValue->ReleaseReference();
  }
  return result;
}


// Recursively search a class definition for a pid for the given propery
// definition guid.
static bool findPropertyDefinition(
  ImplAAFClassDef *pClassDef,
  aafUID_constref propertyID,
  ImplAAFPropertyDef **ppPropertyDef)
{
  AAFRESULT result;
  
  // Try the current class definition before the parent.
  {
    ImplAAFPropertyDefSP pPropertyDef;
    result = pClassDef->LookupPropertyDef(propertyID, ppPropertyDef);
    assert(AAFRESULT_SUCCEEDED(result) || AAFRESULT_NO_MORE_OBJECTS == result || AAFRESULT_PROPERTY_NOT_FOUND == result);
    if (AAFRESULT_SUCCEEDED(result))
      return true;
  }
  
  // Now recursively look in the parent...
  ImplAAFClassDefSP pParent;
  result = pClassDef->GetParent(&pParent);
  assert(AAFRESULT_SUCCEEDED(result) || AAFRESULT_IS_ROOT_CLASS == result);
  if (AAFRESULT_SUCCEEDED(result))
    return findPropertyDefinition(pParent, propertyID, ppPropertyDef);
  else
    return false;
}

static bool findReferencedClassDefintion(
  ImplAAFTypeDef *pTypeDef, 
  ImplAAFClassDef **ppClassDef,
  OMPropertyId * uniqueIdentifierPid,
  eAAFTypeCategory_e expectedCategory = kAAFTypeCatUnknown)
{
  eAAFTypeCategory_t category = kAAFTypeCatUnknown;
  
  if (AAFRESULT_SUCCEEDED(pTypeDef->GetTypeCategory(&category)))
  {
    switch (category)
    {
      case kAAFTypeCatRename:
      {
        ImplAAFTypeDefRename * pRenameType = dynamic_cast<ImplAAFTypeDefRename *>(pTypeDef);
        assert(pRenameType);
        if (!pRenameType)
          return false;
          
        ImplAAFTypeDefSP pBaseType;
        if (AAFRESULT_SUCCEEDED(pRenameType->GetBaseType(&pBaseType)))
        {
          return findReferencedClassDefintion(pBaseType, ppClassDef, uniqueIdentifierPid);
        }
        else
        {
          return false;
        }
        break;
      }

#if 0      
      case kAAFTypeCatVariableArray:
      {
        // The OM requires that the last property definition that defines the target of
        // a weak reference must be a set that contains a strong reference.
        if ((expectedCategory != kAAFTypeCatUnknown) && (expectedCategory != kAAFTypeCatVariableArray))
          return false;
          
        ImplAAFTypeDefVariableArray * pArrayType = dynamic_cast<ImplAAFTypeDefVariableArray *>(pTypeDef);
        assert(pArrayType);
        if (!pArrayType)
          return false;
          
        ImplAAFTypeDefSP pElementType;
        if (AAFRESULT_SUCCEEDED(pArrayType->GetType(&pElementType)))
        {
          // The OM requires that the last property definition that defines the target of
          // a weak reference must be a set that contains a strong reference.
          return findReferencedClassDefintion(pElementType, ppClassDef, uniqueIdentifierPid, kAAFTypeCatStrongObjRef);
        }
        else
        {
          return false;
        }
        break;
      }
#endif      
      
      case kAAFTypeCatSet:
      {          
        if ((expectedCategory != kAAFTypeCatUnknown) && (expectedCategory != kAAFTypeCatSet))
          return false;

        ImplAAFTypeDefSet * pSetType = dynamic_cast<ImplAAFTypeDefSet *>(pTypeDef);
        assert(pSetType);
        if (!pSetType)
          return false;
          
        ImplAAFTypeDefSP pElementType;
        if (AAFRESULT_SUCCEEDED(pSetType->GetElementType(&pElementType)))
        {
          // The OM requires that the last property definition that defines the target of
          // a weak reference must be a set that contains a strong reference.
          return findReferencedClassDefintion(pElementType, ppClassDef, uniqueIdentifierPid, kAAFTypeCatStrongObjRef);
        }
        else
        {
          return false;
        }
        break;
      }

      case kAAFTypeCatStrongObjRef:
      {
        if ((expectedCategory != kAAFTypeCatUnknown) && (expectedCategory != kAAFTypeCatStrongObjRef))
          return false;
          
        ImplAAFTypeDefStrongObjRef * pReferenceType = dynamic_cast<ImplAAFTypeDefStrongObjRef *>(pTypeDef);
        assert(pReferenceType);
        if (!pReferenceType)
          return false;
          
        if (AAFRESULT_SUCCEEDED(pReferenceType->GetObjectType(ppClassDef)))
        {
          if (NULL == uniqueIdentifierPid)
            return true;
        
          // Find the uniqueIdentifierPid from the last class definition.
          ImplAAFPropertyDefSP pPropertyDef;
          if (AAFRESULT_SUCCEEDED((*ppClassDef)->GetUniqueIdentifier(&pPropertyDef)))
          {
            *uniqueIdentifierPid = pPropertyDef->OmPid();
            return true;
          }
          else
          {
            return false;
          }
        }
        else
        {
          return false;
        }
        break;
      }
      
      default:
      {
        // No other types are used to support weak references.
        return false;
      }
    }
  }
  else
  {
    return false;
  }

  return false;
}

// Find the referenced class definition for the given property definition.
static bool findReferencedClassDefintion(
  ImplAAFPropertyDef *pPropertyDef,
  ImplAAFClassDef **ppClassDef,
  OMPropertyId * uniqueIdentifierPid,
  eAAFTypeCategory_e expectedCategory = kAAFTypeCatUnknown)
{
  AAFRESULT result;
  ImplAAFTypeDefSP pPropertyType, pFundementalType;
  
  result = pPropertyDef->GetTypeDef(&pPropertyType);
  assert(AAFRESULT_SUCCEEDED(result));
  if (AAFRESULT_FAILED(result))
    return false;
  
  // Get the fundemental type of the property.
  return findReferencedClassDefintion(pPropertyType, ppClassDef, uniqueIdentifierPid, expectedCategory);
}

// Synchronize the array of OM pids with the current TargetSet property.
AAFRESULT ImplAAFTypeDefWeakObjRef::SyncTargetPidsFromTargetSet(void)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  assert (NULL == _targetPids); // this method should only be called once!
  
  // We need to have some guids to convert!
  assert(0 !=  _targetSet.count());
  if (0 == _targetSet.count())
    return AAFRESULT_NOT_INITIALIZED;
  
  OMPropertyId *targetPids = NULL;
  OMPropertyId uniqueIdentifierPid = 0;
  aafUInt32 targetPidCount = 0;
  aafUInt32 index = 0;
  aafUInt32 lastIndex = _targetSet.count() - 1;
  aafUID_t propertyID = {0};
  bool valid = false;
  
  // Replace with appropriate contants defined by OM:
  const OMPropertyId kOMDictionaryPid = 1;
  const OMPropertyId kOMDataPid = 2;

  ImplAAFDictionarySP pDictionary;
  result = GetDictionary(&pDictionary);
  if (AAFRESULT_SUCCEEDED(result))
  {
    targetPidCount = _targetSet.count();
    assert (0 != targetPidCount);
    
    // pid array for OM must be 0 terminated.
    targetPids = new OMPropertyId[targetPidCount + 1];
    if (NULL == targetPids)
      return AAFRESULT_NOMEMORY;
    // Initialize the new pid array. NOTE: This will also null terminates the array...
    for (index = 0; index <= targetPidCount; index++)
      targetPids[index] = 0;
     
    ImplAAFClassDefSP pClassDef;
    ImplAAFPropertyDefSP pPropertyDef;

    // First we need to determine if whether the first property
    // definition is in the MetaDictionary or the Header.
    _targetSet.getValueAt(&propertyID, 0);
    
    if (0 == memcmp(&propertyID, &kAAFPropID_Root_MetaDictionary, sizeof(aafUID_t)))
    {
      targetPids[0] = kOMDictionaryPid;
      result = pDictionary->LookupClassDef(AUID_AAFMetaDictionary, &pClassDef);
    } 
    else if (0 == memcmp(&propertyID, &kAAFPropID_Root_Header, sizeof(aafUID_t)))
    {
      targetPids[0] = kOMDataPid;
      result = pDictionary->LookupClassDef(AUID_AAFHeader, &pClassDef);
    }
    else
    {
      result = AAFRESULT_PROPERTY_NOT_FOUND; // ???
    }


    if (AAFRESULT_SUCCEEDED(result))
    { 
      for (index = 1; index < targetPidCount; ++index)
      {
        // Find the property definition that corresponds to the current propertyID.
        _targetSet.getValueAt(&propertyID, index);
        valid = findPropertyDefinition(pClassDef, propertyID, &pPropertyDef);
        if (!valid)
          break;
        
        // The OM requires that the last property definition that defines the target of
        // a weak reference must be a set that contains a strong reference.
        if (index < lastIndex)
          valid = findReferencedClassDefintion(pPropertyDef, &pClassDef, NULL, kAAFTypeCatStrongObjRef);
        else
          valid = findReferencedClassDefintion(pPropertyDef, &pClassDef, &uniqueIdentifierPid, kAAFTypeCatSet);

        // Find the next referenced class definition
        if (!valid)
          break;
        
        // Save the pid
        targetPids[index] = pPropertyDef->OmPid();
      }     
    }
  }
  
  
  // Remap result...
  if (!valid)
    result = AAFRESULT_INVALID_PARAM;

  
  // Save the target pid array.
  if (valid && AAFRESULT_SUCCEEDED(result))
  {
    _targetPids = targetPids;
    targetPids = NULL;
    
    _uniqueIdentifierPid = uniqueIdentifierPid;
  }
  
  
  // Cleanup and left over dynamic memory...
  if (NULL != targetPids)
  {
    delete [] targetPids;
    targetPids = NULL;
  }
  
  
  return result;
}


aafBool ImplAAFTypeDefWeakObjRef::IsFixedSize (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefWeakObjRef::PropValSize (void) const
{
  return sizeof (ImplAAFStorable*);
}


aafBool ImplAAFTypeDefWeakObjRef::IsRegistered (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefWeakObjRef::NativeSize (void) const
{
  return sizeof (ImplAAFStorable*);
}


OMProperty * ImplAAFTypeDefWeakObjRef::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  assert (name);
  
#if defined(USE_SIMPLEPROPERTY)

  size_t elemSize = PropValSize ();
  OMProperty * result = new OMSimpleProperty (pid, name, elemSize);
  
#else // #if defined(USE_SIMPLEPROPERTY)
  
  // TEMPORARY: The following code is obsolete. It only exists temporarily to satisfy the compiler...
  // This code will be updated when there is a non-template contructor for a weak reference.
  // transdel:2000-JUN-23.
  OMProperty * result = NULL;

  // Allocate and initialize a weak reference property.
  if (!_targetPids)
  {
    AAFRESULT rc = const_cast<ImplAAFTypeDefWeakObjRef *>(this)->SyncTargetPidsFromTargetSet();
    if (AAFRESULT_FAILED(rc))
      return NULL;
  }
  assert (_targetPids);
  
  switch (_uniqueIdentifierPid)
  {
    case PID_MetaDefinition_Identification:
      result = new OMWeakReferenceProperty<ImplAAFMetaDefinition>(pid, name, _uniqueIdentifierPid, _targetPids);
      break;
      
    case PID_DefinitionObject_Identification:
      result = new OMWeakReferenceProperty<ImplAAFDefObject>(pid, name, _uniqueIdentifierPid, _targetPids);
      break;
    
    default:
      // No support for other "key properties"
      assert (0);
      break;
  }

#endif // #else // #if defined(USE_SIMPLEPROPERTY)

  assert (result);
  return result;
}






// override from OMStorable.
const OMClassId& ImplAAFTypeDefWeakObjRef::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefWeakObjRef));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefWeakObjRef::onSave(void* clientContext) const
{
  ImplAAFTypeDefObjectRef::onSave(clientContext);
}

void ImplAAFTypeDefWeakObjRef::onRestore(void* clientContext) const
{
  ImplAAFTypeDefObjectRef::onRestore(clientContext);
}

// Method is called after associated class has been added to MetaDictionary.
// If this method fails the class is removed from the MetaDictionary and the
// registration method will fail.
HRESULT ImplAAFTypeDefWeakObjRef::CompleteClassRegistration(void)
{
  AAFRESULT rc = AAFRESULT_SUCCESS;
  if (!_targetPids)
  {
    rc = SyncTargetPidsFromTargetSet();
  }
  assert (_targetPids);

  return rc;
}

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
 * prior written permission of Avid Technology, Inc.
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


#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
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
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


// Weak references may not be in v1.0...
#ifndef ENABLE_WEAK_REFERENCES
#define ENABLE_WEAK_REFERENCES 0
#endif


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
  

  // Do not validate the given target hint until registration. If we attempt
  // to validate at this point the corresponding property definitions might
  // not be in the dictionary yet (one of the properties may be on a forward
  // reference class). 
  
  // Copy the given target hint array to the variable sized property.
  _targetSet.setElementValues(pTargetSet, ids);

  AAFRESULT result = pvtInitialize (id, pObjType, pTypeName);
  if (AAFRESULT_FAILED(result))
    return result;
  
#if ENABLE_WEAK_REFERENCES
  
  // TEMPORARY (for debugging): Allocate and initialize a weak reference property.
  if (!_targetPids)
  {
    result = const_cast<ImplAAFTypeDefWeakObjRef *>(this)->SyncTargetPidsFromTargetSet();
  }

  return result;

#else // #if ENABLE_WEAK_REFERENCES
    
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
    
#endif // #else // #if ENABLE_WEAK_REFERENCES
}



// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::pvtInitialize (
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

  return AAFRESULT_SUCCESS;
}

// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefWeakObjRef::CreateValue (/*[in]*/ ImplAAFRoot * /*pObj*/,
    /*[out]*/ ImplAAFPropertyValue ** /*ppPropVal*/)
{
#if ENABLE_WEAK_REFERENCES

  return AAFRESULT_NOT_IMPLEMENTED;

#else // #if ENABLE_WEAK_REFERENCES
    
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
    
#endif // #else // #if ENABLE_WEAK_REFERENCES
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::SetObject (ImplAAFPropertyValue * pPropVal,
										   ImplAAFRoot * pObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pObject) return AAFRESULT_NULL_PARAM;

#if ENABLE_WEAK_REFERENCES

  return AAFRESULT_NOT_IMPLEMENTED;

#else // #if ENABLE_WEAK_REFERENCES
    
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
    
#endif // #else // #if ENABLE_WEAK_REFERENCES
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefWeakObjRef::GetObject (ImplAAFPropertyValue * pPropVal,
									   ImplAAFRoot ** ppObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppObject) return AAFRESULT_NULL_PARAM;

#if ENABLE_WEAK_REFERENCES

  return AAFRESULT_NOT_IMPLEMENTED;

#else // #if ENABLE_WEAK_REFERENCES
    
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
    
#endif // #else // #if ENABLE_WEAK_REFERENCES
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
  OMPropertyId &uniqueIdentifierPid,
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
          // Find the uniqueIdentifierPid from the last class definition.
          ImplAAFPropertyDefSP pPropertyDef;
          if (AAFRESULT_SUCCEEDED((*ppClassDef)->GetUniqueIdentifier(&pPropertyDef)))
          {
            uniqueIdentifierPid = pPropertyDef->OmPid();
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
}

// Find the referenced class definition for the given property definition.
static bool findReferencedClassDefintion(
  ImplAAFPropertyDef *pPropertyDef,
  ImplAAFClassDef **ppClassDef,
  OMPropertyId &uniqueIdentifierPid,
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
  eAAFTypeCategory_e expectedCategory = kAAFTypeCatUnknown;
  bool valid = false;
  
  // Replace with appropriate contants defined by OM:
  const OMPropertyId kOMDictionaryPid = 1;
  const OMPropertyId kOMDataPid = 2;
  
  // We need to prepend the OM Pid for the data or dictionary
  // strong reference.
  targetPidCount = _targetSet.count() + 1;
  assert (0 != targetPidCount);
  targetPids = new OMPropertyId[targetPidCount + 1];
  if (NULL == targetPids)
    return AAFRESULT_NOMEMORY;
  // Initialize the new pid array. NOTE: This will also null terminates the array...
  for (index = 0; index <= targetPidCount; index++)
    targetPids[index] = 0;
    
  ImplAAFClassDefSP pClassDef;
  ImplAAFPropertyDefSP pPropertyDef;

  // We need to search the dictionary.
  index = 0;
  ImplAAFDictionarySP pDictionary;
  result = GetDictionary(&pDictionary);
  if (AAFRESULT_SUCCEEDED(result))
  {
    // First we need to determine if whether the first property
    // definition is in the MetaDictionary or the Header.
    _targetSet.getValueAt(&propertyID, 0);
    
    result = pDictionary->LookupClassDef(AUID_AAFMetaDictionary, &pClassDef);
    assert(AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_SUCCEEDED(result))
    {      
      if (findPropertyDefinition(pClassDef, propertyID, &pPropertyDef))
      {
        targetPids[0] = kOMDictionaryPid;
      }
      else
      {
        result = pDictionary->LookupClassDef(AUID_AAFHeader, &pClassDef);
        assert(AAFRESULT_SUCCEEDED(result));
        if (AAFRESULT_SUCCEEDED(result))
        {
          if (findPropertyDefinition(pClassDef, propertyID, &pPropertyDef))
          {
            targetPids[0] = kOMDataPid;
          }
          else
          {
            // The pTargetSet argument is not valid!
            result = AAFRESULT_INVALID_PARAM;
          }
        }
      }
    }
  }
  

  if (AAFRESULT_SUCCEEDED(result))
  { 
    for (index = 0; index < (targetPidCount - 1); ++index)
    {
      
      // Find the property definition that corresponds to the current propertyID.
      if (0 < index)
      {
        _targetSet.getValueAt(&propertyID, index);
        valid = findPropertyDefinition(pClassDef, propertyID, &pPropertyDef);
        if (!valid)
          break;
      }
      
      // The OM requires that the last property definition that defines the target of
      // a weak reference must be a set that contains a strong reference.
      if (index < lastIndex)
        valid = findReferencedClassDefintion(pPropertyDef, &pClassDef, uniqueIdentifierPid, kAAFTypeCatStrongObjRef);
      else
#ifdef LAST_ELEMENT_CAN_BE_A_STRONGREFERENCEVECTOR
        valid = findReferencedClassDefintion(pPropertyDef, &pClassDef, uniqueIdentifierPid, kAAFTypeCatVariableArray);
#else
        valid = findReferencedClassDefintion(pPropertyDef, &pClassDef, uniqueIdentifierPid, kAAFTypeCatSet);
#endif        
      // Find the next referenced class definition
      if (!valid)
        break;
      
      // Save the pid
      targetPids[index + 1] = pPropertyDef->OmPid();
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




// Synchronize the TargetSet property from the current targetPids OM pid array.
AAFRESULT ImplAAFTypeDefWeakObjRef::SyncTargetSetFromTargetPids(void)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


aafBool ImplAAFTypeDefWeakObjRef::IsFixedSize (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefWeakObjRef::PropValSize (void) const
{
  return sizeof (ImplAAFRoot*);
}


aafBool ImplAAFTypeDefWeakObjRef::IsRegistered (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefWeakObjRef::NativeSize (void) const
{
  return sizeof (ImplAAFRoot*);
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

  // Allocate and initialize a weak reference property.
  if (!_targetPids)
    const_cast<ImplAAFTypeDefWeakObjRef *>(this)->SyncTargetPidsFromTargetSet();
  
  // TEMPORARY: The following code is obsolete. It only exists temporarily to satisfy the compiler...
  // This code will be updated when there is a non-template contructor for a weak reference.
  // transdel:2000-JUN-23.
  OMProperty * result = NULL;
  
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

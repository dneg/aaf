/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif


#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFRefSetValue_h__
#include "ImplAAFRefSetValue.h"
#endif

#ifndef __ImplAAFStrongRefSetValue_h__
#include "ImplAAFStrongRefSetValue.h"
#endif

#ifndef __ImplAAFWeakRefSetValue_h__
#include "ImplAAFWeakRefSetValue.h"
#endif

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFTypeDefRecord_h__
#include "ImplAAFTypeDefRecord.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#include "OMStrongRefSetProperty.h"
#include "OMWeakRefSetProperty.h"
#include "OMDataTypes.h"


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFTypeDefObjectRef.h"
#include "ImplAAFObjectCreation.h"


#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFStrongRefSetValue;
extern "C" const aafClassID_t CLSID_AAFWeakRefSetValue;

ImplAAFTypeDefSet::ImplAAFTypeDefSet () :
_ElementType  ( PID_TypeDefinitionSet_ElementType, 
         L"ElementType", 
         L"/MetaDictionary/TypeDefinitions", 
         PID_MetaDefinition_Identification),
 _uidProperty(NULL),
 _uidType(NULL)
{
  _persistentProperties.put(_ElementType.address());
}


ImplAAFTypeDefSet::~ImplAAFTypeDefSet ()
{}

AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::Initialize (
      aafUID_constref  id,
      ImplAAFTypeDef * pTypeDef,
      aafCharacter_constptr  pTypeName)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (! pTypeName) 
    return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)
    return AAFRESULT_NULL_PARAM;

  assert (!isInitialized());
  if (isInitialized())
    return AAFRESULT_ALREADY_INITIALIZED;
    
  //
  // Validate: the given type definition exists in the
  // dictionary. Also, make sure that the associated class
  // definition contains a unique identifier!
  //
  
  // The given type must be in the current dictionary.
  aafUID_t typeID;
  result = pTypeDef->GetAUID(&typeID);
  if (AAFRESULT_FAILED(result))
    return result;
  ImplAAFDictionarySP pDictionary;
  result = GetDictionary(&pDictionary);
  if (AAFRESULT_FAILED(result))
    return result;
  ImplAAFTypeDefSP pType;
  result = pDictionary->LookupTypeDef(typeID, &pType);
  if (AAFRESULT_FAILED(result))
    return result;
  
   
  // The object reference must be for a class that has a unique
  // identifier property. NOTE: The utility GetUIDType performs the validation.
  GetUIDType(pTypeDef, result); // ignore return value (it is NOT reference counted).
  if (AAFRESULT_FAILED(result))
    return result;    
  
  result = pvtInitialize(id, pTypeDef, pTypeName);
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatSet;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::pvtInitialize (
                  aafUID_constref  id,
                  ImplAAFTypeDef * pTypeDef,
                  aafCharacter_constptr  pTypeName)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  if (! pTypeName) 
    return AAFRESULT_NULL_PARAM;
  if (! pTypeDef)
    return AAFRESULT_NULL_PARAM;
  
  
  result = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
  if (AAFRESULT_SUCCEEDED(result))
  {
    _ElementType = pTypeDef;

    // This instance is now fully initialized.
    setInitialized();
  }
  return result;
}

         
ImplAAFTypeDefRecord* STDMETHODCALLTYPE
  ImplAAFTypeDefSet::GetUIDType(ImplAAFTypeDef* pElementType, AAFRESULT& result) const
{
  ImplAAFTypeDefObjectRef  *objRef = NULL;
  result = AAFRESULT_SUCCESS;
  if (NULL != _uidType)
    return _uidType;
  
  if (NULL == pElementType)
  {
    result = AAFRESULT_NULL_PARAM;
    return NULL;
  }

  // JeffB: only allow strong and weak references to objects
  objRef = dynamic_cast<ImplAAFTypeDefObjectRef*>(pElementType);
  if(objRef == NULL)
  {
    result = AAFRESULT_ELEMENT_NOT_OBJECT;
    return NULL;
  }
  
  // The object reference must be for a class that has a unique
  // identifier property.
  ImplAAFClassDefSP pElementClass;
  result = objRef->GetObjectType(&pElementClass);
  if (AAFRESULT_FAILED(result))
    return NULL;
  ImplAAFPropertyDefSP pUIDPropertyDef;
  result = pElementClass->GetUniqueIdentifier(&pUIDPropertyDef);
  if (AAFRESULT_FAILED(result))
    return NULL;

	// TEMPORARY???: There are only a few types of object reference sets supported
	// at this time.
	switch (pUIDPropertyDef->localIdentification())
	{
    case PID_MetaDefinition_Identification:
      break;
    case PID_DefinitionObject_Identification:
      break;
		case PID_Mob_MobID:
			break;
		case PID_EssenceData_MobID:
			break;
			
		default:
			result = AAFRESULT_BAD_TYPE;
			return NULL;
			break;
	}
 
  // Preserve logical const-ness even though this method is bitwise non-const.
  ImplAAFTypeDefSet* pNonConstThis = const_cast<ImplAAFTypeDefSet *>(this);
  
  // Cache the pid for the uid. 
  pNonConstThis->_uidProperty = pUIDPropertyDef;
    
  ImplAAFTypeDefSP pUIDType;
  result = pUIDPropertyDef->GetTypeDef(&pUIDType);
  if (AAFRESULT_FAILED(result))
    return NULL;
  
  // We only support record types for unique identifiers of objects...
  // Preserve logical const-ness even though this method is bitwise non-const.
  pNonConstThis->_uidType = dynamic_cast<ImplAAFTypeDefRecord*>((ImplAAFTypeDef*)pUIDType);
  if (NULL == _uidType)
  {
    result = AAFRESULT_BAD_TYPE; // ??? TODO: Add support fo Renamed record...
    return NULL;
  }
  
  return _uidType;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::GetElementType (
              ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  if(_ElementType.isVoid())
    return AAFRESULT_OBJECT_NOT_FOUND;
  
  *ppTypeDef = _ElementType;
  assert (*ppTypeDef);
  
  (*ppTypeDef)->AcquireReference ();
  
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::AddElement (
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pElementPropertyValue)
{
  if (!pSetPropertyValue || !pElementPropertyValue)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->InsertElement(pElementPropertyValue);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::RemoveElement(
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pElementPropertyValue)
{
  if (!pSetPropertyValue || !pElementPropertyValue)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->RemoveElement(pElementPropertyValue);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}

//****************
// ContainsElement()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::ContainsElement(
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pElementPropertyValue,
      aafBoolean_t*  pContainsElement)
{
  if (!pSetPropertyValue || !pElementPropertyValue || !pContainsElement)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->ContainsElement(pElementPropertyValue, pContainsElement);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefSet::GetCount (
      ImplAAFPropertyValue * pSetPropertyValue,
      aafUInt32 *  pCount)
{
  if (!pSetPropertyValue || !pCount)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->Count(pCount);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}



//***********************************************************
// CreateKey()
//
AAFRESULT ImplAAFTypeDefSet::CreateKey (
      aafDataBuffer_t  pKeyPtr,
      aafUInt32  length,
      ImplAAFPropertyValue ** ppKey)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  if (!pKeyPtr || !ppKey)
    return AAFRESULT_NULL_PARAM;
  if (0 == length)
    return AAFRESULT_INVALID_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  // Use the type of the unique identifier to create the key
  // property value.
  ImplAAFTypeDefSP pElementType;
  result = GetElementType(&pElementType);
  if (AAFRESULT_FAILED(result))
    return result;
  ImplAAFTypeDefRecord* pUIDType = GetUIDType(pElementType, result);
  if (AAFRESULT_FAILED(result))
    return result;
  result = pUIDType->CreateValueFromStruct(pKeyPtr, length, ppKey);

  return result;
}


//***********************************************************
// LookupElement()
//
AAFRESULT ImplAAFTypeDefSet::LookupElement (
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pKey,
      ImplAAFPropertyValue ** ppElementPropertyValue)
{
  if (!pSetPropertyValue || !pKey || !ppElementPropertyValue)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->LookupElement(pKey, ppElementPropertyValue);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}

//***********************************************************
// ContainsKey()
//
AAFRESULT ImplAAFTypeDefSet::ContainsKey (
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplAAFPropertyValue * pKey,
      aafBoolean_t*  pContainsKey)
{
  if (!pSetPropertyValue || !pKey || !pContainsKey)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->ContainsKey(pKey, pContainsKey);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}


AAFRESULT
ImplAAFTypeDefSet::GetElements (
      ImplAAFPropertyValue * pSetPropertyValue,
      ImplEnumAAFPropertyValues ** ppEnum)
{
  if (!pSetPropertyValue || !ppEnum)
    return AAFRESULT_NULL_PARAM;

  assert (isInitialized());
  if (!isInitialized())
    return AAFRESULT_NOT_INITIALIZED;
  
  // Get the elements if the set property value is a object reference set.
  ImplAAFRefSetValue* pRefSet = dynamic_cast<ImplAAFRefSetValue*>(pSetPropertyValue);
  if (NULL != pRefSet)
  {
    return pRefSet->GetElements(ppEnum);
  }
  else
  {
    return AAFRESULT_ELEMENT_NOT_OBJECT;
  }
}


bool ImplAAFTypeDefSet::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefSet::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefSet::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefSet::IsVariableArrayable () const
{ return false; }

bool ImplAAFTypeDefSet::IsStringable () const
{ return false; }




OMProperty * ImplAAFTypeDefSet::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  assert (name);

  assert (isInitialized());
  if (!isInitialized())
    return NULL;

  ImplAAFTypeDefSP ptd;
  AAFRESULT rc = (const_cast<ImplAAFTypeDefSet*>(this))->GetElementType(&ptd);
	if (AAFRESULT_FAILED(rc))
		return NULL;
  assert (ptd);

  OMProperty * result = 0;

  if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) ptd))
	{
	  // element is strong ref
	  
//	  

		// TEMPORARY???: There are only a few types of object reference sets supported
		// at this time.
		switch (_uidProperty->localIdentification())
		{
//	    case PID_MetaDefinition_Identification:
//	  		return new OMStrongReferenceSetProperty<OMObjectIdentification, ImplAAFMetaDefinition> (pid, name, _uidProperty->localIdentification());

	    case PID_DefinitionObject_Identification:
	  		return new OMStrongReferenceSetProperty<OMObjectIdentification, ImplAAFDefObject> (pid, name, _uidProperty->localIdentification());

//			case PID_Mob_MobID:
//				return new OMStrongReferenceSetProperty<OMUniqueMaterialIdentification, ImplAAFMob> (pid, name, _uidProperty->localIdentification());

//			case PID_EssenceData_MobID:
//				return new OMStrongReferenceSetProperty<OMUniqueMaterialIdentification, ImplAAFEssenceData> (pid, name, _uidProperty->localIdentification());
		}

		return NULL;
	}
  else if (dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) ptd))
	{
	  // element is weak ref, hence implemented as AUID array.
	  // Specify a size of one element.
	  result = new OMSimpleProperty (pid, name, sizeof (aafUID_t));
	}

  else
	{
		// bad type	
  }

  assert (result);
  return result;
}





// Allocate and initialize the correct subclass of ImplAAFPropertyValue 
// for the given OMProperty.
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefSet::CreatePropertyValue(
    OMProperty *property,
    ImplAAFPropertyValue ** ppPropertyValue ) const
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  assert (property && ppPropertyValue);
  if (NULL == property || NULL == ppPropertyValue)
    return AAFRESULT_NULL_PARAM;
  *ppPropertyValue = NULL; // initialize out parameter
  
  OMReferenceSetProperty* pReferenceSetProperty = dynamic_cast<OMReferenceSetProperty*>(property);
  if (NULL != pReferenceSetProperty)
  {
    assert (property->definition());
    if (NULL == property->definition())
      return AAFRESULT_INVALID_PARAM;
    const OMType *type = property->definition()->type();
    assert (type);
    ImplAAFTypeDefSet *ptd = const_cast<ImplAAFTypeDefSet *> (dynamic_cast<const ImplAAFTypeDefSet *>(type));
    assert (ptd);
    if (NULL == ptd)
      return AAFRESULT_INVALID_PARAM;
      
    ImplAAFTypeDefSP pElementType;
    result = GetElementType(&pElementType);
    if (AAFRESULT_FAILED(result))
      return result;
      
    if (dynamic_cast<ImplAAFTypeDefStrongObjRef*>((ImplAAFTypeDef*) pElementType))
    {
      // element is strong ref
      ImplAAFStrongRefSetValue* pStrongRefSet = NULL;
      pStrongRefSet = (ImplAAFStrongRefSetValue*) CreateImpl (CLSID_AAFStrongRefSetValue);
      if (!pStrongRefSet) 
        return AAFRESULT_NOMEMORY;
      result = pStrongRefSet->Initialize(this, property);
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppPropertyValue = pStrongRefSet;
      }
      else
      {
        pStrongRefSet->ReleaseReference();
      }
    }
    else if (dynamic_cast<ImplAAFTypeDefWeakObjRef*>((ImplAAFTypeDef*) pElementType))
    {
      // element is weak ref
      ImplAAFWeakRefSetValue* pWeakRefSet = NULL;
      pWeakRefSet = (ImplAAFWeakRefSetValue*) CreateImpl (CLSID_AAFWeakRefSetValue);
      if (!pWeakRefSet) 
        return AAFRESULT_NOMEMORY;
      result = pWeakRefSet->Initialize(this, property);
      if (AAFRESULT_SUCCEEDED(result))
      {
        *ppPropertyValue = pWeakRefSet;
      }
      else
      {
        pWeakRefSet->ReleaseReference();
      }
    }
    else
    { 
//      assert (NULL != *ppPropertyValue);     
//      return AAFRESULT_INVALID_PARAM;

      // TEMPORARY HACK!
      // Weak reference arrays are still implemented as an array of records!
      
      // If the property is not a reference vector then use the "old" method
      // for creating a variable array property value.
      result = ImplAAFTypeDef::CreatePropertyValue(property, ppPropertyValue);
    }
  }
  else
  {
    // If the property is not a reference vector then use the "old" method
    // for creating a variable array property value.
    result = ImplAAFTypeDef::CreatePropertyValue(property, ppPropertyValue);
  }
  return result;
}




// override from OMStorable.
const OMClassId& ImplAAFTypeDefSet::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefSet));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefSet::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefSet::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "AAFTypeDefUIDs.h"


#include "OMAssertions.h"
#include <string.h>


ImplAAFPropertyDef::ImplAAFPropertyDef ()
  : _Type(PID_PropertyDefinition_Type, L"Type"),
    _IsOptional(PID_PropertyDefinition_IsOptional, L"IsOptional"),
    _pid(PID_PropertyDefinition_LocalIdentification, L"LocalIdentification"),
    _IsUniqueIdentifier(PID_PropertyDefinition_IsUniqueIdentifier, L"IsUniqueIdentifier"),
	_cachedType (0),
    _OMPropCreateFunc (0),
    _containingClass(0)
{
  _persistentProperties.put (_Type.address());
  _persistentProperties.put (_IsOptional.address());
  _persistentProperties.put (_pid.address());
  _persistentProperties.put (_IsUniqueIdentifier.address());
}


ImplAAFPropertyDef::~ImplAAFPropertyDef ()
{
  if (_cachedType)
    _cachedType->ReleaseReference();
  _cachedType = 0;
}


AAFRESULT ImplAAFPropertyDef::pvtInitialize (
      const aafUID_t & propertyAuid,
      OMPropertyId omPid,
      const aafCharacter * pPropName,
	  const aafUID_t & typeId,
      aafBoolean_t isOptional,
      aafBoolean_t isUniqueIdentifier,
      OMClassDefinition* pContainingClass)
{
  AAFRESULT hr;

  if (! pPropName) return AAFRESULT_NULL_PARAM;
  if (! pContainingClass)
    return AAFRESULT_NULL_PARAM;

  hr = ImplAAFMetaDefinition::Initialize(propertyAuid, pPropName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  _Type = typeId;
  _pid = omPid;
  _IsOptional = isOptional;

  if (isUniqueIdentifier)
  {
    // Only set this optional property if true.
    _IsUniqueIdentifier = isUniqueIdentifier;
  }

  _containingClass = pContainingClass;

  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFPropertyDef::pvtInitialize (
      aafUID_constref propertyAuid,
      OMPropertyId omPid,
      aafCharacter_constptr pPropName,
      ImplAAFTypeDef *pType,
      aafBoolean_t isOptional,
      aafBoolean_t isUniqueIdentifier,
      OMClassDefinition* pContainingClass)
{
  AAFRESULT hr;

  if (! pPropName) return AAFRESULT_NULL_PARAM;
  if (! pType)
    return AAFRESULT_NULL_PARAM;
  if (! pContainingClass)
    return AAFRESULT_NULL_PARAM;

  aafUID_t typeId;
  hr = pType->GetAUID(&typeId);
  if (AAFRESULT_FAILED (hr))
    return hr;

  hr = ImplAAFMetaDefinition::Initialize(propertyAuid, pPropName, NULL);
  if (AAFRESULT_FAILED (hr))
    return hr;

  _cachedType = pType;
  _cachedType->AcquireReference();


  _Type = typeId;
  _pid = omPid;
  _IsOptional = isOptional;

  if (isUniqueIdentifier)
  {
    // Only set this optional property if true.
    _IsUniqueIdentifier = isUniqueIdentifier;
  }

  _containingClass = pContainingClass;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetTypeDef (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  if (! _cachedType)
	{
	  ImplAAFDictionarySP pDict;
	  AAFRESULT hr;

	  hr = GetDictionary(&pDict);
	  if (AAFRESULT_FAILED (hr)) return hr;
	  ASSERTU (pDict);

	  ImplAAFPropertyDef * pNonConstThis =
		  (ImplAAFPropertyDef *) this;
	  aafUID_t typeId = _Type;

	  ImplAAFTypeDef * tmp = 0;
	  hr = pDict->LookupTypeDef (typeId, &tmp);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	  ASSERTU (tmp);
	  if (!_cachedType) {
		pNonConstThis->_cachedType = tmp;
		_cachedType->AcquireReference();
	  }
	  // If lookup caused this to already be put into the cache, just
	  // throw away the current copy (in tmp)
	  tmp->ReleaseReference ();
	  tmp = 0;
	}
  ASSERTU (ppTypeDef);
  *ppTypeDef = _cachedType;
  ASSERTU (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetIsOptional (
       aafBool * pIsOptional) const
{
  if (! pIsOptional)
	return AAFRESULT_NULL_PARAM;
  *pIsOptional = _IsOptional;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetIsUniqueIdentifier (
       aafBool * pIsUniqueIdentifier) const
{
  if (! pIsUniqueIdentifier)
	  return AAFRESULT_NULL_PARAM;

  if (! _IsUniqueIdentifier.isPresent())
	{
    // If the property is not present then this property
    // definition cannot be for a unique identifier! Just
    // return false.
    *pIsUniqueIdentifier = kAAFFalse;
//	  return AAFRESULT_PROP_NOT_PRESENT;
	}
  else
  {
    *pIsUniqueIdentifier = _IsUniqueIdentifier;
  }

  return AAFRESULT_SUCCESS;
}


OMPropertyId ImplAAFPropertyDef::OmPid (void) const
{
  return _pid;
}


OMClassDefinition* ImplAAFPropertyDef::containingClass(void) const
{
  TRACE( "ImplAAFPropertyDef::containingClass" );
  PRECONDITION( "Property definition is registered", _containingClass != 0 );

  return _containingClass;
}


const OMType* ImplAAFPropertyDef::type(void) const
{
  AAFRESULT hr;
  ImplAAFTypeDef * ptd = 0;

  hr = GetTypeDef (&ptd);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  ASSERTU (ptd);
  // Don't reference count these!
  aafUInt32 refCount;
  refCount = ptd->ReleaseReference ();
  // make sure our assumption (dict owns a ref) is correct
  ASSERTU (refCount > 0);
  return ptd->type();
}


const OMUniqueObjectIdentification&
ImplAAFPropertyDef::identification(void) const
{
  // to prevent ambiguity
  return ImplAAFMetaDefinition::identification();
}

const wchar_t* ImplAAFPropertyDef::name(void) const
{
  // to prevent ambiguity
  return ImplAAFMetaDefinition::name();
}


bool ImplAAFPropertyDef::hasDescription(void) const
{
  // to prevent ambiguity
  return ImplAAFMetaDefinition::hasDescription();
}

const wchar_t* ImplAAFPropertyDef::description(void) const
{
  // to prevent ambiguity
  return ImplAAFMetaDefinition::description();
}

bool ImplAAFPropertyDef::isPredefined(void) const
{
  // to prevent ambiguity
  return ImplAAFMetaDefinition::isPredefined();
}

OMPropertyId ImplAAFPropertyDef::localIdentification(void) const
{
  return _pid;
}

void ImplAAFPropertyDef::setLocalIdentification(OMPropertyId propertyId)
{
  _pid = propertyId;
}

bool ImplAAFPropertyDef::isOptional(void) const
{
  return (kAAFTrue == _IsOptional) ? true : false;
}

OMUniqueObjectIdentification ImplAAFPropertyDef::typeId(void) const
{
    aafUID_t tid = _Type;
    return *(reinterpret_cast<OMUniqueObjectIdentification*>(&tid));
}

bool ImplAAFPropertyDef::isUniqueIdentifier(void) const
{
  if (!_IsUniqueIdentifier.isPresent())
  {
      return false;
  }
  else
  {
      return _IsUniqueIdentifier == kAAFTrue;
  }
}



OMProperty * ImplAAFPropertyDef::CreateOMProperty () const
{
  OMProperty * result = 0;

  if (_OMPropCreateFunc)
	{
	  result = _OMPropCreateFunc (_pid, name());
	}

  if (! result)
	{
	  // Either there was no create func, or an existing one deferred
	  // to the type def.
	  ImplAAFTypeDefSP ptd;
	  AAFRESULT hr = GetTypeDef (&ptd);
	  ASSERTU (AAFRESULT_SUCCEEDED (hr));
	  ASSERTU (ptd);
	  result = ptd->pvtCreateOMProperty (_pid, name());
	}

  return result;
}


void ImplAAFPropertyDef::SetOMPropCreateFunc
(ImplAAFOMPropertyCreateFunc_t pFunc)
{
  ASSERTU (pFunc);
  _OMPropCreateFunc = pFunc;
}






// override from OMStorable.
const OMClassId& ImplAAFPropertyDef::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFPropertyDef));
}

// Override callbacks from OMStorable
void ImplAAFPropertyDef::onSave(void* clientContext) const
{
  ImplAAFMetaDefinition::onSave(clientContext);
}


#define AAF_BEGIN_TYPE_PATCHES()\
  ImplAAFPropertyDef *nonConstThis;\
  OMPropertyId pid = _pid;\
  aafUID_t typeId;\
  switch (pid)\
  {


#define AAF_PATCH_PROPETY_TYPE(pid, tid)\
    case pid:\
    {\
      nonConstThis = const_cast<ImplAAFPropertyDef *>(this);\
      typeId = _Type;\
      if (0 != memcmp(&typeId, &tid, sizeof(aafUID_t)))\
      {\
        nonConstThis->_Type = tid;\
      }\
    }\
    break;

      
#define AAF_END_TYPE_PATCHES()\
    default:\
      break;\
  }


void ImplAAFPropertyDef::onRestore(void* clientContext) const
{
  // NOTE: This is a patch for DR3 & DR4 and 
  // earlier files. Such files actually have the wrong type definition
  // associated with the property definition.

  AAF_BEGIN_TYPE_PATCHES()

    // All strong reference sets in DR3 files were incorrectly described as variable arrays.
    // DR4 now has an implementation for strong reference sets so we need to "remap" all of
    // the builtin types so the strong reference sets will be created instead of strong
    // reference arrays.
    AAF_PATCH_PROPETY_TYPE(PID_ClassDefinition_Properties, kAAFTypeID_PropertyDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_ContentStorage_Mobs, kAAFTypeID_MobStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_ContentStorage_EssenceData, kAAFTypeID_EssenceDataStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_Dictionary_OperationDefinitions, kAAFTypeID_OperationDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_Dictionary_ParameterDefinitions, kAAFTypeID_ParameterDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_Dictionary_DataDefinitions, kAAFTypeID_DataDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_Dictionary_PluginDefinitions, kAAFTypeID_PluginDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_Dictionary_CodecDefinitions, kAAFTypeID_CodecDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_Dictionary_ContainerDefinitions, kAAFTypeID_ContainerDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_Dictionary_InterpolationDefinitions, kAAFTypeID_InterpolationDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_MetaDictionary_ClassDefinitions, kAAFTypeID_ClassDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_MetaDictionary_TypeDefinitions, kAAFTypeID_TypeDefinitionStrongReferenceSet)
    AAF_PATCH_PROPETY_TYPE(PID_OperationDefinition_ParametersDefined, kAAFTypeID_ParameterDefinitionWeakReferenceSet)
  
    // The DataDefinitions property is implemented as a weak reference vector. The object model for 
    // DR4, AAFMetaDictionary.h, and earlier incorrectly described this type as a weak reference set.
    AAF_PATCH_PROPETY_TYPE(PID_CodecDefinition_DataDefinitions, kAAFTypeID_DataDefinitionWeakReferenceVector)    
   
    // The Definition property was incorrectly implemented as an AUID. The object model for 
    // DR4, AAFMetaDictionary.h, and earlier correctly described this type as a weak reference.
    // Since the file format if "frozen" the new AAFMetaDictionary describes this property
    // as an AUID to be consistent with the implementation.
    AAF_PATCH_PROPETY_TYPE(PID_Parameter_Definition, kAAFTypeID_AUID)
   
    // The Type property was incorrectly implemented as an AUID. The object model for 
    // DR4, AAFMetaDictionary.h, and earlier correctly described this type as a weak reference.
    // Since the file format if "frozen" the new AAFMetaDictionary describes this property
    // as an AUID to be consistent with the implementation.
    AAF_PATCH_PROPETY_TYPE(PID_PropertyDefinition_Type, kAAFTypeID_AUID)

    // In DR4 and earlier there were a couple of types that incorrectly used String instead of StringArray.
//    AAF_PATCH_PROPETY_TYPE(PID_TypeDefinitionExtendibleEnumeration_ElementNames, kAAFTypeID_StringArray)
//    AAF_PATCH_PROPETY_TYPE(PID_TypeDefinitionRecord_MemberNames, kAAFTypeID_StringArray)
    
  AAF_END_TYPE_PATCHES()


  ImplAAFMetaDefinition::onRestore(clientContext);
}


#undef AAF_BEGIN_TYPE_PATCHES
#undef AAF_PATCH_PROPETY_TYPE
#undef AAF_END_TYPE_PATCHES



// Method is called after class has been added to MetaDictionary.
// If this method fails the class is removed from the MetaDictionary and the
// registration method will fail.
HRESULT ImplAAFPropertyDef::CompleteClassRegistration(void)
{
  // Make sure the associated type definition can complete.
  //
  ImplAAFTypeDefSP pType;
  AAFRESULT hr = GetTypeDef (&pType);
  if (AAFRESULT_SUCCEEDED(hr))
  {
    hr = pType->CompleteClassRegistration();
  }
  return hr;
}



AAFRESULT ImplAAFPropertyDef::MergeTo( ImplAAFClassDef* pDestClassDef )
{
    ASSERTU( pDestClassDef );

    AAFRESULT hr = AAFRESULT_SUCCESS;

    // This property ID
    aafUID_t propertyID;
    GetAUID( &propertyID );

    if( ! pDestClassDef->PvtIsPropertyDefRegistered( propertyID ) )
    {
        ImplAAFDictionary* pDestDictionary = NULL;
        pDestClassDef->GetDictionary( &pDestDictionary );

        aafUInt32  nameBufLen = 0;
        GetNameBufLen( &nameBufLen );
        aafUInt8* pName = new aafUInt8[ nameBufLen ];
        GetName( (aafCharacter*)pName, nameBufLen );

        // Find the property type definition in the destination file
        ImplAAFTypeDef* pTypeDef = NULL;
        GetTypeDef( &pTypeDef );
        aafUID_t  typeID;
        pTypeDef->GetAUID( &typeID );
        pTypeDef->MergeTo( pDestDictionary );
        pTypeDef->ReleaseReference();
        pTypeDef = NULL;

        ImplAAFTypeDef* pDestTypeDef = NULL;
        pDestDictionary->LookupTypeDef( typeID, &pDestTypeDef );
        ASSERTU( pDestTypeDef != NULL );

        // Register the property definition.
        // The property registering method to use depends on whether
        // this class definition is attached to or detached from
        // the dictionary.
        ImplAAFPropertyDef* pDestPropertyDef = NULL;
        aafUID_t  classID;
        pDestClassDef->GetAUID( &classID );
        if( pDestDictionary->PvtIsClassPresent( classID ) )
        {
            // This class definition is in the dictionary - only
            // optional properties can be registered.
            ASSERTU( _IsOptional == kAAFTrue );

            hr = pDestClassDef->RegisterOptionalPropertyDef( propertyID,
                                                             (aafCharacter*)pName,
                                                             pDestTypeDef,
                                                             &pDestPropertyDef );
        }
        else
        {
            // This class definition is not in the dictionary -
            // any properties can be registered.
            aafBoolean_t isUniqueIdentifier = kAAFFalse;
            if( _IsUniqueIdentifier.isPresent() )
            {
                isUniqueIdentifier = _IsUniqueIdentifier;
            }

            hr = pDestClassDef->RegisterNewPropertyDef( propertyID,
                                                        (aafCharacter*)pName,
                                                        pDestTypeDef,
                                                        _IsOptional,
                                                        isUniqueIdentifier,
                                                        &pDestPropertyDef );
        }


        // If present, copy the property definition description.
        if( AAFRESULT_SUCCEEDED( hr ) )
        {
            aafUInt32  descriptionBufLen = 0;
            GetDescriptionBufLen( &descriptionBufLen );
            if( descriptionBufLen > 0 )
            {
                aafUInt8* pDescription = new aafUInt8[ descriptionBufLen ];
                GetDescription( (aafCharacter*)pDescription,
                                descriptionBufLen );

                hr = pDestPropertyDef->SetDescription(
                        (aafCharacter*)pDescription );

                delete[] pDescription;
                pDescription = NULL;
            }
        }

        // Because RegisterOptionalPropertyDef/RegisterNewPropertyDef can
        // fail (for example, if the property's already registered with a
        // different class), pDestPropertyDef may not be a valid pointer.
        if( pDestPropertyDef )
        {
            pDestPropertyDef->ReleaseReference();
            pDestPropertyDef = NULL;
        }

        pDestTypeDef->ReleaseReference();
        pDestTypeDef = NULL;

        delete[] pName;
        pName = NULL;

        pDestDictionary->ReleaseReference();
        pDestDictionary = NULL;
    }


    return hr;
}

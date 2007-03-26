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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFMetaDictionary_h__
#include "ImplAAFMetaDictionary.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplEnumAAFClassDefs_h__
#include "ImplEnumAAFClassDefs.h"
#endif


#ifndef __ImplEnumAAFTypeDefs_h__
#include "ImplEnumAAFTypeDefs.h"
#endif


#ifndef __ImplAAFBaseClassFactory_h__
#include "ImplAAFBaseClassFactory.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "OMReferenceSetIter.h"
#include "OMPropertySetIterator.h"

#include "ImplAAFObjectCreation.h"
#include "ImplAAFCloneResolver.h"
#include "aafErr.h"


#include "OMAssertions.h"


#include "AAFObjectModel.h"
#include "AAFObjectModelProcs.h"

#include "ImplAAFTypeDefCharacter.h"
#include "ImplAAFTypeDefEnum.h"
#include "ImplAAFTypeDefExtEnum.h"
#include "ImplAAFTypeDefFixedArray.h"
#include "ImplAAFTypeDefIndirect.h"
#include "ImplAAFTypeDefInt.h"
#include "ImplAAFTypeDefOpaque.h"
#include "ImplAAFTypeDefRecord.h"
#include "ImplAAFTypeDefRename.h"
#include "ImplAAFTypeDefSet.h"
#include "ImplAAFTypeDefStream.h"
#include "ImplAAFTypeDefString.h"
#include "ImplAAFTypeDefStrongObjRef.h"
#include "ImplAAFTypeDefVariableArray.h"
#include "ImplAAFTypeDefWeakObjRef.h"


extern "C" const aafClassID_t CLSID_AAFMetaDictionary;
extern "C" const aafClassID_t CLSID_AAFPropertyDef;
extern "C" const aafClassID_t CLSID_EnumAAFClassDefs;
extern "C" const aafClassID_t CLSID_EnumAAFTypeDefs;


ImplAAFMetaDictionary::ImplAAFMetaDictionary () :
  _typeDefinitions  (PID_MetaDictionary_TypeDefinitions,
                     L"TypeDefinitions", 
                     PID_MetaDefinition_Identification),
  _classDefinitions (PID_MetaDictionary_ClassDefinitions,
                     L"ClassDefinitions", 
                     PID_MetaDefinition_Identification),
  _dataDictionary(NULL)
{
  _persistentProperties.put (_typeDefinitions.address());
  _persistentProperties.put (_classDefinitions.address());
}


ImplAAFMetaDictionary::~ImplAAFMetaDictionary ()
{
  // Release the _opaqueTypeDefinitions
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef> opaqueTypeDefinitions(_opaqueTypeDefinitions);
  while(++opaqueTypeDefinitions)
  {
    ImplAAFTypeDef *pType = opaqueTypeDefinitions.value();
    if (pType)
    {
      pType->ReleaseReference();
      pType = 0;
    }
  }

  // Release the _axiomaticTypeDefinitions
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef> axiomaticTypeDefinitions(_axiomaticTypeDefinitions);
  while(++axiomaticTypeDefinitions)
  {
    ImplAAFTypeDef *pAxiomaticTypeDef = axiomaticTypeDefinitions.value();
    if (pAxiomaticTypeDef)
    {
      pAxiomaticTypeDef->ReleaseReference();
      pAxiomaticTypeDef = 0;
    }
  }

  // Release the _axiomaticPropertyDefinitions
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPropertyDef> axiomaticPropertyDefinitions(_axiomaticPropertyDefinitions);
  while(++axiomaticPropertyDefinitions)
  {
    ImplAAFPropertyDef *pAxiomaticPropertyDef = axiomaticPropertyDefinitions.value();
    if (pAxiomaticPropertyDef)
    {
      pAxiomaticPropertyDef->ReleaseReference();
      pAxiomaticPropertyDef = 0;
    }
  }

  // Release the _axiomaticClassDefinitions
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef> axiomaticClassDefinitions(_axiomaticClassDefinitions);
  while(++axiomaticClassDefinitions)
  {
    ImplAAFClassDef *pAxiomaticClassDef = axiomaticClassDefinitions.value();
    if (pAxiomaticClassDef)
    {
      pAxiomaticClassDef->ReleaseReference();
      pAxiomaticClassDef = 0;
    }
  }




  // Release _typeDefinitions
  OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>typeDefinitions(_typeDefinitions);
  while(++typeDefinitions)
  {
    ImplAAFTypeDef *pType = typeDefinitions.clearValue();
    if (pType)
    {
      pType->ReleaseReference();
      pType = 0;
    }
  }

  // Release _classDefinitions
  OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef>classDefinitions(_classDefinitions);
  while(++classDefinitions)
  {
    ImplAAFClassDef *pClass = classDefinitions.clearValue();
    if (pClass)
    {
      pClass->ReleaseReference();
      pClass = 0;
    }
  }

}




// Factory method for creating a MetaDictionary.
ImplAAFMetaDictionary *ImplAAFMetaDictionary::CreateMetaDictionary(void)
{
  ImplAAFMetaDictionary * pMetaDictionary = NULL;

  ImplAAFRoot *pObject = CreateImpl(CLSID_AAFMetaDictionary);
  if (pObject)
  {
    pMetaDictionary = dynamic_cast<ImplAAFMetaDictionary *>(pObject);
    ASSERTU(NULL != pMetaDictionary);
    if (NULL != pMetaDictionary)
    {
      // If we created a dictionary then give it a reference to a factory
      // (dictionary) to satisfy the OMStorable class invariant: Every OMStorabe
      // must have a reference to a factory. Since the meta dictionary is not created
      // by the OMClassFactory interface we just set the factory to "itself".
      //

      pMetaDictionary->setClassFactory(pMetaDictionary);
    }
    else
    {
      // Cleanup the "invalid" meta dictionary object.
      pObject->ReleaseReference();
      pObject = NULL;
    }
  }
  
  return pMetaDictionary;
}


bool ImplAAFMetaDictionary::isRegistered(const OMClassId& classId ) const
{
	ASSERTU( sizeof(OMClassId) == sizeof(aafUID_t) );
	return containsClass( reinterpret_cast<const aafUID_t&>(classId) );
}

//
// Create an instance of the appropriate derived class, given the class id.
//  This method implements the OMClassFactory interface.
//
OMStorable* ImplAAFMetaDictionary::create(const OMClassId& classId) const
{
  OMStorable* storable = NULL;

  // If we are creating the meta dictionary then just return this.
  if (0 == memcmp(&AUID_AAFMetaDictionary, &classId, sizeof(classId)))
  {
	ImplAAFClassDef* pClassDef = findAxiomaticClassDefinition(AUID_AAFMetaDictionary);
    ASSERTU(NULL != pClassDef);
    storable = const_cast<ImplAAFMetaDictionary *>(this);
  }
  else
  {
    //
    // Create the appropriate meta definition object for the given
    // classId.
    // transdel:2000-MAY-16
    //
    ImplAAFMetaDefinition *pMetaObject = NULL;
    ImplAAFMetaDictionary *pNonConstThis = const_cast<ImplAAFMetaDictionary *>(this);
    AAFRESULT result = pNonConstThis->CreateMetaInstance((*reinterpret_cast<const aafUID_t *>(&classId)), &pMetaObject);
    ASSERTU(AAFRESULT_SUCCEEDED(result));
    storable = pMetaObject;
  }

  // Set the class factory for the meta-data objects to this meta dictionary.
  storable->setClassFactory(this);

  return (storable);
}

void ImplAAFMetaDictionary::destroy(OMStorable* victim) const
{
  ImplAAFMetaDefinition* v = dynamic_cast<ImplAAFMetaDefinition*>(victim);
  ASSERTU(v != 0);
  v->ReleaseReference();
}

//
// Define the symbol for the stored object id
//
const OMClassId& ImplAAFMetaDictionary::classId(void) const
{
  return *reinterpret_cast<const OMClassId*>(&AUID_AAFMetaDictionary);
}



// Override callback from OMStorable
void ImplAAFMetaDictionary::onSave(void* /*clientContext*/) const
{
}



void ImplAAFMetaDictionary::onRestore(void* /* clientContext */) const
{
#if 0

	// Make sure that all of the axiomatic objects are in the dictionary.
	// Use only low-level OM methods so that we can eliminate side-effects
	// in the DM.
	OMUniqueObjectIdentification id = {0};
	ImplAAFMetaDictionary * nonConstThis = const_cast<ImplAAFMetaDictionary *>(this);
	
  // Make sure all of the _axiomaticClassDefinitions are in the persistent set.
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef> axiomaticClassDefinitions(_axiomaticClassDefinitions);
  while(++axiomaticClassDefinitions)
  {
    ImplAAFClassDef *pAxiomaticClassDef = axiomaticClassDefinitions.value();
    ASSERTU (pAxiomaticClassDef);
    if (pAxiomaticClassDef)
    {
      id = axiomaticClassDefinitions.identification();
      if (_classDefinitions.contains(id))
      {
      	nonConstThis->_classDefinitions.remove(&id);
      }

	    nonConstThis->_classDefinitions.appendValue(pAxiomaticClassDef);
	    pAxiomaticClassDef->AcquireReference(); // saving another reference...
    }
  }

  // Make sure all of the _axiomaticTypeDefinitions are in the persistent set.
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef> axiomaticTypeDefinitions(_axiomaticTypeDefinitions);
  while(++axiomaticTypeDefinitions)
  {
    ImplAAFTypeDef *pAxiomaticTypeDef = axiomaticTypeDefinitions.value();
    if (pAxiomaticTypeDef)
    {
      id = axiomaticTypeDefinitions.identification();
      if (_typeDefinitions.contains(id))
      {
      	nonConstThis->_typeDefinitions.remove(&id);
      }

	    nonConstThis->_typeDefinitions.appendValue(pAxiomaticTypeDef);
	    pAxiomaticTypeDef->AcquireReference(); // saving another reference... 
    }
  }
#endif // #if 0
}

// Use "propertyId" as the "local tag" (2 bytes) for "id" (16 bytes).
void
ImplAAFMetaDictionary::associate(const OMObjectIdentification& id,
                                 const OMPropertyId propertyId)
{
  ASSERTU(_dataDictionary);
  _dataDictionary->associate(*reinterpret_cast<const aafUID_t *>(&id), propertyId);
}

void ImplAAFMetaDictionary::newClass(const OMUniqueObjectIdentification& id,
                    const wchar_t* name,
                    const wchar_t* description,
                    const OMUniqueObjectIdentification& parent,
                    bool isConcrete)
{
    TRACE("ImplAAFMetaDictionary::newClass");
    
    AAFRESULT hr;
    
    // lookup the class def
    const aafUID_t* classDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFClassDefSP spClassDef;
    hr = dataDictionary()->LookupClassDef(*classDefAUID, &spClassDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // class def already exists
        return;
    }
    
    // Lookup the parent class def (we assume a new class cannot be a root class)
    const aafUID_t* parentClassDefAUID = reinterpret_cast<const aafUID_t*>(&parent);
    ImplAAFClassDefSP spParentClassDef;
    hr = dataDictionary()->LookupClassDef(*parentClassDefAUID, &spParentClassDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("Parent of new class def exists", false);
        return;
    }
    
    // create new class def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFClassDef, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New class def created", false);
        return;
    }
    
    // initialise class def
    ImplAAFClassDef* pClassDef = dynamic_cast<ImplAAFClassDef*>(pMetaDef);
    ASSERT("Meta def is a class def", pClassDef != 0);
    hr = pClassDef->Initialize(*classDefAUID, spParentClassDef, name, isConcrete);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New class def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pClassDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New class def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterClassDef(pClassDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New class def registered", false);
        return;
    }
    
}

const OMPropertyDefinition* ImplAAFMetaDictionary::newProperty(const OMUniqueObjectIdentification& id,
                       const wchar_t* name,
                       const wchar_t* description,
                       OMPropertyId localId,
                       const OMUniqueObjectIdentification& type,
                       bool& isOptional,
                       bool& isUniqueIdentifier,
                       const OMUniqueObjectIdentification& memberOf)
{
    TRACE("ImplAAFMetaDictionary::newProperty");
    
    AAFRESULT hr;
    
    // lookup the class def
    const aafUID_t* classDefAUID = reinterpret_cast<const aafUID_t*>(&memberOf);
    ImplAAFClassDefSP spClassDef;
    hr = dataDictionary()->LookupClassDef(*classDefAUID, &spClassDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("Class def for new property def exists", false);
        return 0;
    }
    
    // lookup property def
    const aafUID_t* propDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFPropertyDef* pPropertyDef = 0;
    hr = spClassDef->LookupPropertyDef(*propDefAUID, &pPropertyDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // property def already exists - return it
        return pPropertyDef;
    }
    
    // create a new property def
    // TODO: is it ok to use this "private" function?
    //       the non-private function RegisterNewPropertyDef fails because it doesn't allow the
    //       class to be attached
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&type);
    hr = spClassDef->pvtRegisterPropertyDef(*propDefAUID, name, *typeDefAUID,
        isOptional, isUniqueIdentifier, &pPropertyDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here?
        ASSERT("Created new property def", false);
        return 0;
    }
    
    // set the local Id if it is predefined, otherwise it is dynamic and should be ignored
    if (localId < 0x8000)
    {
        pPropertyDef->setLocalIdentification(localId);
    }

    // set the description
    if (description != 0)
    {
        hr = pPropertyDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New property def description set", false);
            return 0;
        }
    }
    
    return pPropertyDef;
}

void ImplAAFMetaDictionary::newIntegerType(const OMObjectIdentification& id,
                          const wchar_t* name,
                          const wchar_t* description,
                          OMUInt8 size,
                          bool isSigned)
{
    TRACE("ImplAAFMetaDictionary::newIntegerType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefInt, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New integer type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefInt* pTypeDef = dynamic_cast<ImplAAFTypeDefInt*>(pMetaDef);
    ASSERT("Meta def is a integer type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, size, isSigned, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New integer type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New interger type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New integer type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newCharacterType(const OMObjectIdentification& id,
                            const wchar_t* name,
                            const wchar_t* description)
{
    TRACE("ImplAAFMetaDictionary::newCharacterType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefCharacter, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New character type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefCharacter* pTypeDef = dynamic_cast<ImplAAFTypeDefCharacter*>(pMetaDef);
    ASSERT("Meta def is a character type def", pTypeDef != 0);
    hr = pTypeDef->pvtInitialize(*typeDefAUID, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New character type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New character type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New character type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newStrongReferenceType(
                             const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description,
                             const OMObjectIdentification& referencedType)
{
    TRACE("ImplAAFMetaDictionary::newStrongReferenceType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the referenced class def
    const aafUID_t* classDefAUID = reinterpret_cast<const aafUID_t*>(&referencedType);
    ImplAAFClassDefSP spClassDef;
    hr = dataDictionary()->LookupClassDef(*classDefAUID, &spClassDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("Referenced class def for strong ref type def exists", false);
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefStrongObjRef, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New strong ref type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefStrongObjRef* pTypeDef = dynamic_cast<ImplAAFTypeDefStrongObjRef*>(pMetaDef);
    ASSERT("Meta def is a strong ref type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, spClassDef, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New strong ref type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New strong ref type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New strong ref type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newWeakReferenceType(
                             const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description,
                             const OMObjectIdentification& referencedType,
                             const OMObjectIdentification* path,
                             OMUInt32 pathCount)
{
    TRACE("ImplAAFMetaDictionary::newWeakReferenceType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the referenced class def
    const aafUID_t* classDefAUID = reinterpret_cast<const aafUID_t*>(&referencedType);
    ImplAAFClassDefSP spClassDef;
    hr = dataDictionary()->LookupClassDef(*classDefAUID, &spClassDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("Referenced class def for weak ref type def exists", false);
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefWeakObjRef, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New weak ref type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefWeakObjRef* pTypeDef = dynamic_cast<ImplAAFTypeDefWeakObjRef*>(pMetaDef);
    ASSERT("Meta def is a weak ref type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, spClassDef, name, pathCount, 
        reinterpret_cast<const aafUID_t*>(path));
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New weak ref type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New weak ref type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New weak ref type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newRenamedType(const OMObjectIdentification& id,
                          const wchar_t* name,
                          const wchar_t* description,
                          const OMObjectIdentification& renamedType)
{
    TRACE("ImplAAFMetaDictionary::newRenamedType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the renamed type def
    const aafUID_t* renamedTypeDefAUID = reinterpret_cast<const aafUID_t*>(&renamedType);
    ImplAAFTypeDefSP spRenamedTypeDef;
    hr = dataDictionary()->LookupTypeDef(*renamedTypeDefAUID, &spRenamedTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("Renamed type def exists", false);
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefRename, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New renamed type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefRename* pTypeDef = dynamic_cast<ImplAAFTypeDefRename*>(pMetaDef);
    ASSERT("Meta def is a renamed type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, spRenamedTypeDef, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New renamed type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New renamed type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New renamed type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newEnumeratedType(const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description,
                             const OMObjectIdentification& elementType,
                             const wchar_t** elementNames,
                             OMInt64* elementValues,
                             OMUInt32 elementCount)
{
    TRACE("ImplAAFMetaDictionary::newEnumeratedType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the element type def
    const aafUID_t* elementTypeDefAUID = reinterpret_cast<const aafUID_t*>(&elementType);
    ImplAAFTypeDefSP spElementTypeDef;
    hr = dataDictionary()->LookupTypeDef(*elementTypeDefAUID, &spElementTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("Enum element type def exists", false);
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefEnum, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New enum type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefEnum* pTypeDef = dynamic_cast<ImplAAFTypeDefEnum*>(pMetaDef);
    ASSERT("Meta def is a enum type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, spElementTypeDef, elementValues, 
        const_cast<wchar_t**>(elementNames), elementCount, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New enum type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New enum type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New enum type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newFixedArrayType(const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description,
                             const OMObjectIdentification& elementType,
                             OMUInt32 elementCount)
{
    TRACE("ImplAAFMetaDictionary::newFixedArrayType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the element type def
    const aafUID_t* elementTypeDefAUID = reinterpret_cast<const aafUID_t*>(&elementType);
    ImplAAFTypeDefSP spElementTypeDef;
    hr = dataDictionary()->LookupTypeDef(*elementTypeDefAUID, &spElementTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("Fixed array type def exists", false);
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefFixedArray, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New fixed array type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefFixedArray* pTypeDef = dynamic_cast<ImplAAFTypeDefFixedArray*>(pMetaDef);
    ASSERT("Meta def is a fixed array type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, spElementTypeDef, elementCount, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New fixed array type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New fixed array type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New fixed array type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newVaryingArrayType(const OMObjectIdentification& id,
                               const wchar_t* name,
                               const wchar_t* description,
                               const OMObjectIdentification& elementType)
{
    TRACE("ImplAAFMetaDictionary::newVaryingArrayType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the element type def
    const aafUID_t* elementTypeDefAUID = reinterpret_cast<const aafUID_t*>(&elementType);
    ImplAAFTypeDefSP spElementTypeDef;
    hr = dataDictionary()->LookupTypeDef(*elementTypeDefAUID, &spElementTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("Variable array type def exists", false);
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefVariableArray, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New variable array type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefVariableArray* pTypeDef = dynamic_cast<ImplAAFTypeDefVariableArray*>(pMetaDef);
    ASSERT("Meta def is a variable array type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, spElementTypeDef, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New variable array type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New variable array type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New variable array type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newSetType(const OMObjectIdentification& id,
                      const wchar_t* name,
                      const wchar_t* description,
                      const OMObjectIdentification& elementType)
{
    TRACE("ImplAAFMetaDictionary::newSetType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the element type def
    const aafUID_t* elementTypeDefAUID = reinterpret_cast<const aafUID_t*>(&elementType);
    ImplAAFTypeDefSP spElementTypeDef;
    hr = dataDictionary()->LookupTypeDef(*elementTypeDefAUID, &spElementTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("Set type def exists", false);
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefSet, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New set type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefSet* pTypeDef = dynamic_cast<ImplAAFTypeDefSet*>(pMetaDef);
    ASSERT("Meta def is a set type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, spElementTypeDef, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New set type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New set type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New set type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newRecordType(const OMObjectIdentification& id,
                         const wchar_t* name,
                         const wchar_t* description,
                         const OMObjectIdentification* memberTypes,
                         const wchar_t** memberNames,
                         OMUInt32 memberCount)
{
    TRACE("ImplAAFMetaDictionary::newRecordType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the each member type
    OMVector<ImplAAFTypeDef*> memberTypeDefs;
    memberTypeDefs.grow(memberCount);
    OMUInt32 i;
    for (i = 0; i < memberCount; i++)
    {
        const aafUID_t* memberTypeDefAUID = reinterpret_cast<const aafUID_t*>(&memberTypes[i]);
        ImplAAFTypeDef* pMemberTypeDef;
        hr = dataDictionary()->LookupTypeDef(*memberTypeDefAUID, &pMemberTypeDef);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // class def does not exist
            ASSERT("Enum element type def exists", false);
            return;
        }
        memberTypeDefs.append(pMemberTypeDef);
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefRecord, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New record type def created", false);
        return;
    }
    
    // initialise type def
    // TODO: &memberTypeDefs.getAt(0) relies on vector implementation storing elements consecutively
    ImplAAFTypeDefRecord* pTypeDef = dynamic_cast<ImplAAFTypeDefRecord*>(pMetaDef);
    ASSERT("Meta def is a record type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, &memberTypeDefs.getAt(0), 
        const_cast<wchar_t**>(memberNames), memberCount, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New record type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New record type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New record type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newStreamType(const OMObjectIdentification& id,
                         const wchar_t* name,
                         const wchar_t* description)
{
    TRACE("ImplAAFMetaDictionary::newStreamType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefStream, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New stream type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefStream* pTypeDef = dynamic_cast<ImplAAFTypeDefStream*>(pMetaDef);
    ASSERT("Meta def is a stream type def", pTypeDef != 0);
    hr = pTypeDef->pvtInitialize(*typeDefAUID, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New stream type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New stream type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New stream type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newStringType(const OMObjectIdentification& id,
                         const wchar_t* name,
                         const wchar_t* description,
                         const OMObjectIdentification& elementType)
{
    TRACE("ImplAAFMetaDictionary::newStringType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // lookup the element type def
    const aafUID_t* elementTypeDefAUID = reinterpret_cast<const aafUID_t*>(&elementType);
    ImplAAFTypeDefSP spElementTypeDef;
    hr = dataDictionary()->LookupTypeDef(*elementTypeDefAUID, &spElementTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // class def does not exist
        ASSERT("String type def exists", false);
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefString, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New string type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefString* pTypeDef = dynamic_cast<ImplAAFTypeDefString*>(pMetaDef);
    ASSERT("Meta def is a string type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, spElementTypeDef, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New string type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New string type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New string type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newExtendibleEnumeratedType(
                             const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description)
{
    TRACE("ImplAAFMetaDictionary::newExtendibleEnumeratedType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefExtEnum, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New ext enum type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefExtEnum* pTypeDef = dynamic_cast<ImplAAFTypeDefExtEnum*>(pMetaDef);
    ASSERT("Meta def is a ext enum type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New ext enum type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New ext enum type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New ext enum type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newExtendibleEnumeratedTypeElement(
                             const OMObjectIdentification& elementOf,
                             const wchar_t* name,
                             const OMObjectIdentification& value)
{
    TRACE("ImplAAFMetaDictionary::newExtendibleEnumeratedType");
    
    AAFRESULT hr;
    
    // try lookup the ext enum type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&elementOf);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("Ext enum type def exists", false);
        return;
    }
    
    // try lookup the value
    ImplAAFTypeDefExtEnum* pExtEnumTypeDef = dynamic_cast<ImplAAFTypeDefExtEnum*>((ImplAAFTypeDef*)spTypeDef);
    ASSERT("Type def is a ext enum type def", pExtEnumTypeDef != 0);
    const aafUID_t* valueAUID = reinterpret_cast<const aafUID_t*>(&value);
    aafUInt32 nameLen = 0;
    hr = pExtEnumTypeDef->GetNameBufLenFromAUID(*valueAUID, &nameLen);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // value already exists
        return;
    }
    
    // register value
    hr = pExtEnumTypeDef->AppendElement(*valueAUID, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New ext enum type def element appended", false);
        return;
    }
}

void ImplAAFMetaDictionary::newIndirectType(const OMObjectIdentification& id,
                           const wchar_t* name,
                           const wchar_t* description)
{
    TRACE("ImplAAFMetaDictionary::newIndirectType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefIndirect, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New indirect type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefIndirect* pTypeDef = dynamic_cast<ImplAAFTypeDefIndirect*>(pMetaDef);
    ASSERT("Meta def is a indirect type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New indirect type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New indirect type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New indirect type def registered", false);
        return;
    }
    
}

void ImplAAFMetaDictionary::newOpaqueType(const OMObjectIdentification& id,
                         const wchar_t* name,
                         const wchar_t* description)
{
    TRACE("ImplAAFMetaDictionary::newOpaqueType");
    
    AAFRESULT hr;
    
    // try lookup the type 
    const aafUID_t* typeDefAUID = reinterpret_cast<const aafUID_t*>(&id);
    ImplAAFTypeDefSP spTypeDef;
    hr = dataDictionary()->LookupTypeDef(*typeDefAUID, &spTypeDef);
    if (AAFRESULT_SUCCEEDED(hr))
    {
        // type def already exist
        return;
    }
    
    // create new type def
    ImplAAFMetaDefinition* pMetaDef;
    hr = dataDictionary()->CreateMetaInstance(AUID_AAFTypeDefOpaque, &pMetaDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New indirect type def created", false);
        return;
    }
    
    // initialise type def
    ImplAAFTypeDefOpaque* pTypeDef = dynamic_cast<ImplAAFTypeDefOpaque*>(pMetaDef);
    ASSERT("Meta def is a opaque type def", pTypeDef != 0);
    hr = pTypeDef->Initialize(*typeDefAUID, name);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New opaque type def initialised", false);
        return;
    }
    
    // set the description
    if (description != 0)
    {
        hr = pTypeDef->SetDescription(description);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            // TODO: should throw an exception here
            ASSERT("New opaque type def description set", false);
            return;
        }
    }
    
    // register in dictionary
    hr = RegisterTypeDef(pTypeDef);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
        // TODO: should throw an exception here
        ASSERT("New opaque type def registered", false);
        return;
    }
    
}

bool ImplAAFMetaDictionary::registerClassDef(const OMUniqueObjectIdentification& classId)
{
    const aafUID_t* auid = reinterpret_cast<const aafUID_t*>(&classId);
    ImplAAFClassDefSP spClassDef;
    AAFRESULT hr = dataDictionary()->LookupClassDef(*auid, &spClassDef);
    spClassDef->AssurePropertyTypesLoaded();
    return AAFRESULT_SUCCEEDED(hr);
}

bool ImplAAFMetaDictionary::registerTypeDef(const OMUniqueObjectIdentification& typeId)
{
    const aafUID_t* auid = reinterpret_cast<const aafUID_t*>(&typeId);
    ImplAAFTypeDefSP pTypeDef;
    AAFRESULT hr = dataDictionary()->LookupTypeDef(*auid, &pTypeDef);
    return AAFRESULT_SUCCEEDED(hr);
}

ClassDefinitionsIterator* ImplAAFMetaDictionary::classDefinitions(void) const
{
  return _classDefinitions.createIterator();
}

void ImplAAFMetaDictionary::typeDefinitions(OMVector<OMType*>& typeDefs) const
{
    if (_typeDefinitions.count() > 0)
    {
        OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef> 
            iter(_typeDefinitions);
        typeDefs.grow(iter.count());
        while(++iter)
        {
            typeDefs.append(iter.value()->type());
        }
    }
}

//TypeDefinitionsIterator* ImplAAFMetaDictionary::typeDefinitions(void) const
//{
//  return _typeDefinitions.createIterator();
//}


// Temporary method to set the 
void ImplAAFMetaDictionary::setDataDictionary(ImplAAFDictionary *dataDictionary)
{
  ASSERTU(!_dataDictionary);
  _dataDictionary = dataDictionary;
}

ImplAAFDictionary * ImplAAFMetaDictionary::dataDictionary(void) const
{
  ASSERTU (NULL != _dataDictionary);
  return (_dataDictionary);
}


//////////////////////////////
//
//////////////////////////////




bool ImplAAFMetaDictionary::containsForwardClassReference(aafUID_constref classId)
{
  const OMUniqueObjectIdentification* id = reinterpret_cast<const OMUniqueObjectIdentification*>(&classId);
  return (_forwardClassReferences.contains(*id));
}

// Remove the give classId from the foward class reference set.
void ImplAAFMetaDictionary::RemoveForwardClassReference(aafUID_constref classId)
{
  ASSERTU(containsForwardClassReference(classId)); // classId must already be a forward reference!
  const OMUniqueObjectIdentification* id = reinterpret_cast<const OMUniqueObjectIdentification*>(&classId);
  _forwardClassReferences.remove(*id);
}

//////////////////////////////
//
//////////////////////////////


ImplAAFTypeDef * ImplAAFMetaDictionary::findOpaqueTypeDefinition(aafUID_constref typeId) const
{
  // NOTE: The following type cast is temporary. It should be removed as soon
  // as the OM has a declarative sytax to include the type
  // of the key used in the set. (trr:2000-FEB-29)
  ImplAAFTypeDef* opaqueTypeDefinition;
  if (_opaqueTypeDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&typeId)),
                                  opaqueTypeDefinition))
  {
    return opaqueTypeDefinition;
  }
  else
  {
    return NULL;
  }
}


// Add the given class definition to the set of axiomatic class definitions.
void ImplAAFMetaDictionary::addAxiomaticClassDefinition(ImplAAFClassDef *pClassDef)
{
  // Save the new axiomatic class definition in a non-persistent set.
  _axiomaticClassDefinitions.appendValue(pClassDef);
  pClassDef->AcquireReference();
  
  // Save the new axiomatic class definition in a persistent set.
#if 0
  _classDefinitions.appendValue(pClassDef);
  pClassDef->AcquireReference();
#endif
}


// Add the given property definition to the set of axiomatic property definitions.
void ImplAAFMetaDictionary::addAxiomaticPropertyDefinition(ImplAAFPropertyDef *pPropertyDef)
{
  // Save the new axiomatic property definition in a non-persistent set.
  _axiomaticPropertyDefinitions.appendValue(pPropertyDef);
  pPropertyDef->AcquireReference();
}


// Add the given type definition to the set of axiomatic type definitions.
void ImplAAFMetaDictionary::addAxiomaticTypeDefinition(ImplAAFTypeDef *pTypeDef)
{
  // Save the new axiomatic type definition in a non-persistent set.
  _axiomaticTypeDefinitions.appendValue(pTypeDef);
  pTypeDef->AcquireReference();

  // Save the new axiomatic type definition in a persistent set.
#if 0
  _typeDefinitions.appendValue(pTypeDef);
  pTypeDef->AcquireReference();
#endif
}


// Find the aximatic class definition associated with the given class id.
ImplAAFClassDef * ImplAAFMetaDictionary::findAxiomaticClassDefinition(aafUID_constref classId) const // NOT REFERENCE COUNTED!
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
  ImplAAFClassDef* axiomaticClassDefinition;
	if (_axiomaticClassDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&classId)),
                             axiomaticClassDefinition))
  {
    return axiomaticClassDefinition;
  }
  else
  {
    return NULL;
  }
}

// Find the aximatic property definition associated with the given property id.
ImplAAFPropertyDef * ImplAAFMetaDictionary::findAxiomaticPropertyDefinition(aafUID_constref propertyId) const // NOT REFERENCE COUNTED!
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
  ImplAAFPropertyDef* axiomaticPropertyDefinition;
	if (_axiomaticPropertyDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&propertyId)),
                             axiomaticPropertyDefinition))
  {
    return axiomaticPropertyDefinition;
  }
  else
  {
    return NULL;
  }
}

// Find the aximatic type definition associated with the given type id.
ImplAAFTypeDef * ImplAAFMetaDictionary::findAxiomaticTypeDefinition(aafUID_constref typeId) const // NOT REFERENCE COUNTED!
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
  ImplAAFTypeDef* axiomaticTypeDefinition;
	if (_axiomaticTypeDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&typeId)),
                             axiomaticTypeDefinition))
  {
    return axiomaticTypeDefinition;
  }
  else
  {
    return NULL;
  }
}



// These are low-level OMSet tests for containment.
bool ImplAAFMetaDictionary::containsClass(aafUID_constref classId) const
{
  // NOTE: The following type cast is temporary. It should be removed as soon
  // as the OM has a declarative sytax to include the type
  // of the key used in the set. (trr:2000-FEB-26)
  return(_classDefinitions.contains((*reinterpret_cast<const OMObjectIdentification *>(&classId))));
}

bool ImplAAFMetaDictionary::containsType(aafUID_constref typeId)
{
  // NOTE: The following type cast is temporary. It should be removed as soon
  // as the OM has a declarative sytax to include the type
  // of the key used in the set. (trr:2000-FEB-26)
  return(_typeDefinitions.contains((*reinterpret_cast<const OMObjectIdentification *>(&typeId))));
}



//////////////////////////////
//
//////////////////////////////

// Factory function to create an unitialized meta defintion for the 
// given auid.
ImplAAFMetaDefinition * ImplAAFMetaDictionary::pvtCreateMetaDefinition(const aafUID_t & auid)
{

  // Lookup the code class id for the given stored object id.
  const aafClassID_t* id = ImplAAFBaseClassFactory::LookupClassID(auid);
  if (NULL == id)
    return NULL;
  
  // Attempt to create the corresponding storable object.
  ImplAAFRoot *impl = ::CreateImpl(*id);
  if (NULL == impl)
  { // This is a serious programming error. A stored object id was found in the file
    // with a known base class id but no base object could be created.
    ASSERTU(NULL != impl);
    return NULL;
  }

  ImplAAFMetaDefinition *metaDefinition = dynamic_cast<ImplAAFMetaDefinition *>(impl);
  if (NULL == metaDefinition)
  {
    ASSERTU (metaDefinition);
    impl->ReleaseReference();
    return NULL;
  }

  //
  // Initialize the class factory
  //
  metaDefinition->setClassFactory (this);


  // TEMPORARY:
  // Set this object's stored ID.  Be sure to set it to the
  // requested ID, not the instantiated one.  (These will be
  // different if the requested ID is a client-supplied
  // extension class.)
//  metaDefinition->pvtSetSoid (auid);

  return metaDefinition;
}



//Creates a single uninitialized AAF meta definition associated 
  // with a specified stored object id.
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFMetaDictionary::CreateMetaInstance (
    // Stored Object ID of the meta object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in auid
    ImplAAFMetaDefinition ** ppMetaObject)
{
  AAFRESULT hr = AAFRESULT_SUCCESS;

  if (!ppMetaObject) 
    return AAFRESULT_NULL_PARAM;


  // Lookup the class definition for the given classId. If the class
  // definition is one of the "built-in" class definitions then the
  // definition will be created and registered with the dictionary
  // if necessary. (TRR 2000-MAY-16)
  ImplAAFClassDefSP pClassDef;
  hr = dataDictionary()->LookupClassDef (classId, &pClassDef);
  if (AAFRESULT_FAILED (hr))
    return hr;

  if (! pClassDef->pvtIsConcrete ())
	return AAFRESULT_ABSTRACT_CLASS;

  // Create the correct definition object.
  *ppMetaObject = pvtCreateMetaDefinition(classId);
  if (NULL == *ppMetaObject)
  {
    // Return the most likely cause...
    return AAFRESULT_INVALID_CLASS_ID;
  }

  
  // Make sure that we have connected all of the OMProperties
  // to the correct ImplAAFPropertyDefs.
  (*ppMetaObject)->InitializeOMStorable(pClassDef);


  return hr;
}



  //****************
  // LookupClassDef()
  //
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::LookupClassDef(
    aafUID_constref classId,
    ImplAAFClassDef ** ppClassDef)
{
  if (!ppClassDef) return AAFRESULT_NULL_PARAM;

  AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
  // as the OM has a declarative sytax to include the type
  // of the key used in the set. (trr:2000-FEB-26)
  if (_classDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&classId)),
                             *ppClassDef))
  {
    ASSERTU(NULL != *ppClassDef);
    (*ppClassDef)->AcquireReference();
  }
  else
  {
    // no recognized class guid in dictionary
    result = AAFRESULT_NO_MORE_OBJECTS;
  }

  return (result);
}

//****************
// CreateForwardClassReference()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::CreateForwardClassReference(
    aafUID_constref classId)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  // First check to see of forward class reference has already
  // been created.
  if (containsForwardClassReference(classId))
    return AAFRESULT_INVALID_PARAM;

  const OMUniqueObjectIdentification* id = reinterpret_cast<const OMUniqueObjectIdentification*>(&classId);
  // If the class has already been registed then we cannot
  // create a foward reference!
  if (_classDefinitions.contains(*id))
    return AAFRESULT_INVALID_PARAM;

  // It now safe to add the given classId to the set.
  _forwardClassReferences.insert(*id);

  return result;
}

//****************
// IsForwardClassReference()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::HasForwardClassReference(
    aafUID_constref classId,
    aafBoolean_t *pResult)
{
  if (NULL == pResult)
    return AAFRESULT_NULL_PARAM;

  // Call the internal shared implementation.
  *pResult = containsForwardClassReference(classId);

  return AAFRESULT_SUCCESS;
}

//****************
// RegisterClassDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::RegisterClassDef(
    ImplAAFClassDef * pClassDef)
{
  AAFRESULT hr = AAFRESULT_SUCCESS;
  if (NULL == pClassDef)
    return AAFRESULT_NULL_PARAM;
  
  // Get the AUID of the new class to register.
  aafUID_t newAUID;
  hr = pClassDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  hr = PvtRegisterClassDef(pClassDef);
  if (AAFRESULT_SUCCEEDED(hr))
  {  
    bool forwardClassReference = containsForwardClassReference(newAUID);
    if (forwardClassReference)
    {
      // This class is now defined so it can no longer have
      // a forward reference.
      RemoveForwardClassReference(newAUID);
    }

    // Now validate that this classes properties and types
    // are valid.
    hr = pClassDef->CompleteClassRegistration();
    if (AAFRESULT_FAILED(hr))
    {
      // Restore the state of the meta dictionary.
      _classDefinitions.removeValue(pClassDef);
      pClassDef->ReleaseReference();
      
      if (forwardClassReference)
      {
        CreateForwardClassReference(newAUID); // ignore error...
      }
    }
  }

  return(hr);
}


// Private registration method to add the given class definiion
// to the set. 
AAFRESULT ImplAAFMetaDictionary::PvtRegisterClassDef (
  ImplAAFClassDef * pClassDef)
{
  if (NULL == pClassDef)
    return AAFRESULT_NULL_PARAM;

  // Is this class already registered ?
  if (!_classDefinitions.containsValue(pClassDef))
  {
    // This class is not yet registered, add it to the dictionary.
    _classDefinitions.appendValue(pClassDef);
    pClassDef->AcquireReference();
  }

  return(AAFRESULT_SUCCESS);
}

//****************
// GetClassDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::GetClassDefs(
    ImplEnumAAFClassDefs ** ppEnum)
{
  if (NULL == ppEnum)
    return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
  
  ImplEnumAAFClassDefs *theEnum = (ImplEnumAAFClassDefs *)CreateImpl (
	  CLSID_EnumAAFClassDefs);
  
  XPROTECT()
  {
    OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef>* iter = 
      new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef>(_classDefinitions);
    if(iter == 0)
      RAISE(AAFRESULT_NOMEMORY);
	CHECK(theEnum->Initialize(&CLSID_EnumAAFClassDefs,this,iter));
    *ppEnum = theEnum;
  }
  XEXCEPT
  {
    if (theEnum)
    {
      theEnum->ReleaseReference();
      theEnum = 0;
    }
  }
  XEND;
  
  return(AAFRESULT_SUCCESS);
}

//****************
// CountClassDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::CountClassDefs(
    aafUInt32 * pResult)
{
  if (!pResult)
    return AAFRESULT_NULL_PARAM;
  *pResult = _classDefinitions.count();
  return AAFRESULT_SUCCESS;
}

//****************
// RegisterTypeDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::RegisterTypeDef(
    ImplAAFTypeDef * pTypeDef)
{
  if (NULL == pTypeDef)
    return AAFRESULT_NULL_PARAM;
  if (attached() && pTypeDef->attached())
  {
    if (file() != pTypeDef->file())
      return AAFRESULT_OBJECT_ALREADY_ATTACHED;
  }
  
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pTypeDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  if (!containsType(newAUID))
  {
    // We also need to check if the given type is an opaque type. if it is then we
    // need to remove it from the opaqueTypeDefinitions set if the registration 
    // of successful. This makes sense because the type can no longer be opaque.
    ImplAAFTypeDef *pOpaqueType = findOpaqueTypeDefinition(newAUID);
    if (pOpaqueType)
    {
      // Remove the type from the opaque list and release the reference.
      // NOTE: This release may actually delete the opaque type from memory
      // since it is possible that older pOpaqueType is not the same
      // object pointer as the newer pTypeDef pointer eventhough they
      // have the same identifier (newAUID).
      _opaqueTypeDefinitions.removeValue(pOpaqueType);
      pOpaqueType->ReleaseReference();
      pOpaqueType = NULL;
    }

    // This type is not yet registered, add it to the dictionary.
    _typeDefinitions.appendValue(pTypeDef);
    pTypeDef->AcquireReference();
  }

  return (hr);
}

//****************
// LookupTypeDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::LookupTypeDef(
    aafUID_constref typeID,
    ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef)
    return AAFRESULT_NULL_PARAM;

  
  AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
  // as the OM has a declarative sytax to include the type
  // of the key used in the set. (trr:2000-FEB-26)
  if (_typeDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&typeID)),
                             *ppTypeDef))
  {
    ASSERTU(NULL != *ppTypeDef);
    (*ppTypeDef)->AcquireReference();
  }
  else
  {
    // no recognized class guid in dictionary
    result = AAFRESULT_NO_MORE_OBJECTS;
  }

  return (result);
}

//****************
// GetTypeDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::GetTypeDefs(
    ImplEnumAAFTypeDefs ** ppEnum)
{
  if (NULL == ppEnum)
    return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
  
  ImplEnumAAFTypeDefs *theEnum = (ImplEnumAAFTypeDefs *)CreateImpl (
	  CLSID_EnumAAFTypeDefs);
  
  XPROTECT()
  {
    OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>* iter = 
      new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>(_typeDefinitions);
    if(iter == 0)
      RAISE(AAFRESULT_NOMEMORY);
	CHECK(theEnum->Initialize(&CLSID_EnumAAFTypeDefs,this,iter));
    *ppEnum = theEnum;
  }
  XEXCEPT
  {
    if (theEnum)
      {
      theEnum->ReleaseReference();
      theEnum = 0;
      }
  }
  XEND;
  
  return(AAFRESULT_SUCCESS);
}

//****************
// CountTypeDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::CountTypeDefs(
    aafUInt32 * pResult)
{
  if (! pResult)
    return AAFRESULT_NULL_PARAM;
  *pResult = _typeDefinitions.count();
  return AAFRESULT_SUCCESS;
}

//****************
// RegisterOpaqueTypeDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::RegisterOpaqueTypeDef(
    ImplAAFTypeDef * pOpaqueTypeDef)
{
  if (NULL == pOpaqueTypeDef)
    return AAFRESULT_NULL_PARAM;
  
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pOpaqueTypeDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  // NOTE: Behave like all of the other "register" methods: do not
  // fail if the type has already been registered. This is a little
  // ambiguous since the client will not know whether the the given
  // type definition was already public or already opaque...
  // (transdel:2000-APR-03)
  if (!containsType(newAUID) && !findOpaqueTypeDefinition(newAUID))
  {
    // This type is not yet registered, add it to the dictionary.
    _opaqueTypeDefinitions.appendValue(pOpaqueTypeDef);
    pOpaqueTypeDef->AcquireReference();
  }

  return (AAFRESULT_SUCCESS);
}

//****************
// LookupOpaqueTypeDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::LookupOpaqueTypeDef(
    aafUID_constref typeID,
    ImplAAFTypeDef ** ppOpaqueTypeDef)
{
  AAFRESULT status = AAFRESULT_SUCCESS;

  if (! ppOpaqueTypeDef) 
    return AAFRESULT_NULL_PARAM;

  *ppOpaqueTypeDef = findOpaqueTypeDefinition(typeID);
  if (*ppOpaqueTypeDef)
    (*ppOpaqueTypeDef)->AcquireReference ();
  else
    status = AAFRESULT_TYPE_NOT_FOUND;
  
  return (status);
}

//****************
// GetOpaqueTypeDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::GetOpaqueTypeDefs(
    ImplEnumAAFTypeDefs ** ppEnum)
{
  if (NULL == ppEnum)
    return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;

  if (NULL == ppEnum)
    return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
  
  ImplEnumAAFTypeDefs *theEnum = (ImplEnumAAFTypeDefs *)CreateImpl (
	  CLSID_EnumAAFTypeDefs);
  
  XPROTECT()
  {
    OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>* iter = 
      new OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>(_opaqueTypeDefinitions);
    if(iter == 0)
      RAISE(AAFRESULT_NOMEMORY);
	CHECK(theEnum->Initialize(&CLSID_EnumAAFTypeDefs,this,iter));
    *ppEnum = theEnum;
  }
  XEXCEPT
  {
    if (theEnum)
      {
      theEnum->ReleaseReference();
      theEnum = 0;
      }
  }
  XEND;
  
  return(AAFRESULT_SUCCESS);
}

//****************
// CountOpaqueTypeDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::CountOpaqueTypeDefs(
    aafUInt32 * pResult)
{
  if (! pResult)
    return AAFRESULT_NULL_PARAM;
  *pResult = _opaqueTypeDefinitions.count();
  return AAFRESULT_SUCCESS;
}




// Create all of the axiomatic classes as uninitialized objects.
void ImplAAFMetaDictionary::CreateAxiomaticClasses(void)
{
  // Get a pointer to the object model instance.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  aafUInt32 index;
  const ClassDefinition *classDefinition;

  for (index = 0; index < objectModel->countClassDefinitions(); ++index)
  {
    classDefinition = objectModel->classDefinitionAt(index);
    if (classDefinition->axiomatic())
    {
      if (!classDefinition->createDefinition(this))
        throw AAFRESULT_NOMEMORY;
    }
  }
}

// Create all of the axiomatic properties as uninitialized objects.
void ImplAAFMetaDictionary::CreateAxiomaticProperties(void)
{
  // Get a pointer to the object model instance.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  const PropertyDefinition *propertyDefinition;
  aafUInt32 index;

  for (index = 0; index < objectModel->countPropertyDefinitions(); ++index)
  {
    propertyDefinition = objectModel->propertyDefinitionAt(index);
    if (propertyDefinition->axiomatic())
    {
      if (!propertyDefinition->createDefinition(this))
        throw AAFRESULT_NOMEMORY;
    }
  }
}

// Create all of the axiomatic types as uninitialized objects.
void ImplAAFMetaDictionary::CreateAxiomaticTypes(void)
{
  // Get a pointer to the object model instance.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  const TypeDefinition *typeDefinition;
  aafUInt32 index;

  for (index = 0; index < objectModel->countTypeDefinitions(); ++index)
  {
    typeDefinition = objectModel->typeDefinitionAt(index);
    if (typeDefinition->axiomatic() && typeDefinition->concrete())
    {
      if (!typeDefinition->createDefinition(this))
        throw AAFRESULT_NOMEMORY;
    }
  }
}



// Initialize all of the axiomatic classes with their parent and class
// definitions.
void ImplAAFMetaDictionary::InitializeAxiomaticClasses(void)
{
  // Get a pointer to the object model instance.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  const ClassDefinition *classDefinition;
  aafUInt32 index;

  //
  // Finish initializing the each aximatic class definition.
  //
  for (index = 0; index < objectModel->countClassDefinitions(); ++index)
  {
    classDefinition = objectModel->classDefinitionAt(index);
    if (classDefinition->axiomatic())
    {
      if (!classDefinition->initializeDefinition(this))
        throw AAFRESULT_CLASS_NOT_FOUND;
    }
  }
}

// Initialize all of the type definitions with there appropriate class and type
// definitions.
void ImplAAFMetaDictionary::InitializeAxiomaticTypes(void)
{
  // Get a pointer to the object model instance.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  const TypeDefinition *typeDefinition;
  aafUInt32 index;

  //
  // Finish initializing the each aximatic type definition.
  //
  for (index = 0; index < objectModel->countTypeDefinitions(); ++index)
  {
    typeDefinition = objectModel->typeDefinitionAt(index);
    if (typeDefinition->axiomatic() && typeDefinition->concrete())
    {
      if (!typeDefinition->initializeDefinition(this))
        throw AAFRESULT_TYPE_NOT_FOUND;
    }
  }
}

// Initialize all of the property definitions with their type definitions.
void ImplAAFMetaDictionary::InitializeAxiomaticProperties(void)
{
  // Get a pointer to the object model instance.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  const PropertyDefinition *propertyDefinition;
  aafUInt32 index;

  //
  // Finish initializing the each aximatic property definition.
  //
  for (index = 0; index < objectModel->countPropertyDefinitions(); ++index)
  {
    propertyDefinition = objectModel->propertyDefinitionAt(index);
    if (propertyDefinition->axiomatic())
    {
      if (!propertyDefinition->initializeDefinition(this))
        throw AAFRESULT_PROPERTY_NOT_FOUND;
    }
  }
}


// Register all of the axiomatic properties with their corresponding
//  axiomatic class definitions.
void ImplAAFMetaDictionary::RegisterAxiomaticProperties(void)
{
  // Get a pointer to the object model instance.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  const ClassDefinition *classDefinition;
  const PropertyDefinition *propertyDefinition;
  aafUInt32 classIndex, propertyIndex;
  ImplAAFClassDef *pClass = NULL;
  ImplAAFPropertyDef *pProperty = NULL;

  for (classIndex = 0; classIndex < objectModel->countClassDefinitions(); ++classIndex)
  {
    classDefinition = objectModel->classDefinitionAt(classIndex);
    if (classDefinition->axiomatic())
    {
      pClass = findAxiomaticClassDefinition(*classDefinition->id());
      ASSERTU(pClass);

      for (propertyIndex = 0; propertyIndex < classDefinition->propertyCount(); ++propertyIndex)
      {
        propertyDefinition = classDefinition->propertyDefinitionAt(propertyIndex);
        pProperty = findAxiomaticPropertyDefinition(*propertyDefinition->id());
        ASSERTU(pProperty);
        pClass->pvtRegisterExistingPropertyDef(pProperty);
      }
    }
  }
}


// Initialize all of the OMProperties for each aximatic definition.
void ImplAAFMetaDictionary::InitializeAxiomaticOMDefinitions(void)
{
  ImplAAFClassDef *pClassDef;
 
  // Get the class definition for all classes.
  pClassDef = findAxiomaticClassDefinition(AUID_AAFClassDef);
  ASSERTU (pClassDef);

  //
  // Initialize the OM Properties for all of the axiomatic class definitions.
  //
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef> axiomaticClassDefinitions(_axiomaticClassDefinitions, OMBefore);
  while(++axiomaticClassDefinitions)
  {
    ImplAAFClassDef *pClass = axiomaticClassDefinitions.value();
    ASSERTU (pClass);
    if (!pClass)
      throw AAFRESULT_INVALID_OBJ;

    pClass->InitializeOMStorable(pClassDef);
  }

 
  // Get the class definition for all classes.
  pClassDef = findAxiomaticClassDefinition(AUID_AAFPropertyDef);
  ASSERTU (pClassDef);

  //
  // Initialize the OM Properties for all of the axiomatic property definitions.
  //
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPropertyDef> axiomaticPropertyDefinitions(_axiomaticPropertyDefinitions, OMBefore);
  while(++axiomaticPropertyDefinitions)
  {
    ImplAAFPropertyDef *pProperty = axiomaticPropertyDefinitions.value();
    ASSERTU (pProperty);
    if (!pProperty)
      throw AAFRESULT_INVALID_OBJ;

    pProperty->InitializeOMStorable(pClassDef);
  }


  // Get a pointer to the object model instance.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();
  const ClassDefinition *classDefinition;
  const TypeDefinition *typeDefinition;
  aafUID_t id;
  AAFRESULT result;

  //
  // Initialize the OM Properties for all of the axiomatic type definitions.
  //
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef> axiomaticTypeDefinitions(_axiomaticTypeDefinitions, OMBefore);
  while(++axiomaticTypeDefinitions)
  {
    ImplAAFTypeDef *pType = axiomaticTypeDefinitions.value();
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_INVALID_OBJ;

    result = pType->GetAUID(&id);
    ASSERTU (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;
    typeDefinition = objectModel->findTypeDefinition(&id);
    ASSERTU (typeDefinition);
    classDefinition = typeDefinition->classDefinition();
    pClassDef = findAxiomaticClassDefinition(*classDefinition->id());
    ASSERTU (pType);
    if (!pType)
      throw AAFRESULT_CLASS_NOT_FOUND;

    pType->InitializeOMStorable(pClassDef);
  }
  
   
  // Handle special case of initializition the meta dictionary's 
  // OM propreties. TODO: Add InitializeOMStorable method to ImplAAFMetaDictionary.
  pClassDef = findAxiomaticClassDefinition(AUID_AAFMetaDictionary);
  ASSERTU (pClassDef);
  InitializeOMStorable(pClassDef);
}


  
// Complete the registration of the axiomatic class definitions
// This must be called AFTER all other aximatic definitions have
// been initialized and registered.
void ImplAAFMetaDictionary::CompleteAxiomaticClassRegistration(void) // throw AAFRESULT
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef> axiomaticClassDefinitions(_axiomaticClassDefinitions, OMBefore);
  while(++axiomaticClassDefinitions)
  {
    ImplAAFClassDef *pClass = axiomaticClassDefinitions.value();
    ASSERTU (pClass);
    if (!pClass)
      throw AAFRESULT_INVALID_OBJ;

    result = pClass->CompleteClassRegistration();
    if (AAFRESULT_FAILED(result))
      throw result;
  }
}

// Create all of the axiomatic definitions.
void ImplAAFMetaDictionary::CreateAxiomaticDefinitions(void)
{
  CreateAxiomaticClasses();
  CreateAxiomaticTypes();
  CreateAxiomaticProperties();
}

// Initialize all of the axiomatic definitions.
void ImplAAFMetaDictionary::InitializeAxiomaticDefinitions(void)
{
  InitializeAxiomaticClasses();
  InitializeAxiomaticTypes();
  InitializeAxiomaticProperties();

  RegisterAxiomaticProperties();
  InitializeAxiomaticOMDefinitions();
}

  // Initialize only the non-persistent parts of the meta dictionary
AAFRESULT ImplAAFMetaDictionary::Initialize(void)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  try
  {
    InstallAAFObjectModelProcs();

    CreateAxiomaticDefinitions();
    InitializeAxiomaticDefinitions();
  }
  catch (AAFRESULT &rError)
  {
    result = rError;
  }

  return (result);
}

// Create and initialize all of the axiomatic definitions.
AAFRESULT ImplAAFMetaDictionary::InstantiateAxiomaticDefinitions(void)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
	// Make sure that all of the axiomatic objects are in the dictionary.
	// Use only low-level OM methods so that we can eliminate side-effects
	// in the DM.
	OMUniqueObjectIdentification id = {0};
	ImplAAFMetaDictionary * nonConstThis = const_cast<ImplAAFMetaDictionary *>(this);
	
  // Make sure all of the _axiomaticClassDefinitions are in the persistent set.
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef> axiomaticClassDefinitions(_axiomaticClassDefinitions);
  while(++axiomaticClassDefinitions)
  {
    ImplAAFClassDef *pAxiomaticClassDef = axiomaticClassDefinitions.value();
    ASSERTU (pAxiomaticClassDef);
    if (pAxiomaticClassDef)
    {
      id = axiomaticClassDefinitions.identification();
      if (!_classDefinitions.contains(id))
      {
	    nonConstThis->_classDefinitions.appendValue(pAxiomaticClassDef);
      }
      else
      {
	    ImplAAFClassDef* old = nonConstThis->_classDefinitions.replace(pAxiomaticClassDef);
	    if (old != 0)
	      old->ReleaseReference();
      }
      pAxiomaticClassDef->AcquireReference(); // saving another reference...
    }
  }

  // Make sure all of the _axiomaticTypeDefinitions are in the persistent set.
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef> axiomaticTypeDefinitions(_axiomaticTypeDefinitions);
  while(++axiomaticTypeDefinitions)
  {
    ImplAAFTypeDef *pAxiomaticTypeDef = axiomaticTypeDefinitions.value();
    if (pAxiomaticTypeDef)
    {
      id = axiomaticTypeDefinitions.identification();
      if (!_typeDefinitions.contains(id))
      {
	    nonConstThis->_typeDefinitions.appendValue(pAxiomaticTypeDef);
      }
      else
      {
	    ImplAAFTypeDef* old = nonConstThis->_typeDefinitions.replace(pAxiomaticTypeDef);
	    if (old != 0)
	      old->ReleaseReference();
      }
      pAxiomaticTypeDef->AcquireReference(); // saving another reference... 
    }
  }
  return result;
}

AAFRESULT ImplAAFMetaDictionary::MergeBuiltinClassDefs()
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  OMStrongReferenceSetIterator<OMUniqueObjectIdentification, 
                               ImplAAFClassDef>
  iter(_classDefinitions); 

  const AAFObjectModel* pObjectModel = AAFObjectModel::singleton();
  ASSERTU( pObjectModel );

  // For each class def in file...
  while (++iter) {   

    // Get the class def's id and check if the same class def exists in
    // the built in dictionary.
    OMUniqueObjectIdentification id = iter.identification(); 
    ImplAAFClassDef* pFileClassDef = iter.value(); 
    const aafUID_t* uid = reinterpret_cast<aafUID_t*>(&id); 

    const ClassDefinition* pBuiltInModelClassDef = pObjectModel->findClassDefinition( uid );

    if ( pBuiltInModelClassDef && !pBuiltInModelClassDef->isNil() ) {
      
      // The class def exists in both the builtin and file
      // dictionaries. Check that each property that exists in the
      // builtin class def also exists in the files's version of that
      // class def. If it doesn't, then add it to the file's class
      // def.

      unsigned int i;
      for (i = 0; i < pBuiltInModelClassDef->propertyCount(); i++) {

	const PropertyDefinition * pBuiltInModelPropDef = 0;
	pBuiltInModelPropDef = pBuiltInModelClassDef->propertyDefinitionAt(i);

	if ( ! pFileClassDef->PvtIsPropertyDefRegistered( *pBuiltInModelPropDef->id() ) ) {

	  ImplAAFSmartPointer<ImplAAFPropertyDef> spTmp;
	  AAFRESULT hr;
	  hr = pFileClassDef->pvtRegisterPropertyDef( *pBuiltInModelPropDef->id(),
						      pBuiltInModelPropDef->name(),
						      *pBuiltInModelPropDef->typeId(),
						      pBuiltInModelPropDef->required() ? kAAFFalse : kAAFTrue,
						      pBuiltInModelPropDef->uid() ? kAAFTrue : kAAFFalse,
						      &spTmp );
	  
	  ASSERTU( AAFRESULT_SUCCESS == hr );
	  if ( AAFRESULT_SUCCESS != hr ) {
	    result = hr;
	    break;
	  }
	}

      }
    }
  }

  return result;
}

//
// Methods that would be inherited or overriden from ImplAAFStrorable
//

// Associate OMClassDefinition and OMPropertyDefinitions with this object.
void ImplAAFMetaDictionary::InitializeOMStorable(ImplAAFClassDef * pClassDef)
{
  ASSERTU (NULL != pClassDef);
  
  // Install the class definition for this storable.
  setDefinition(pClassDef);
  
  // Make sure all of the properties exist and have property definitions.
  InitOMProperties(pClassDef);
}

// Associate the existing OMProperties with corresponding property definitions from
// the given class definition. NOTE: This call is recursive, it calls itself again
// for the parent class of the given class until current class is a "root" class.
void ImplAAFMetaDictionary::InitOMProperties (ImplAAFClassDef * pClassDef)
{
  ASSERTU (NULL != pClassDef);
  
  OMPropertySetIterator iter(*propertySet(), OMBefore);
  while (++iter)
  {
    ImplAAFPropertyDefSP pPropertyDef;
    pClassDef->LookupPropertyDefbyOMPid(iter.propertyId(), &pPropertyDef);
    OMPropertyDefinition * propertyDefinition = static_cast<OMPropertyDefinition *>((ImplAAFPropertyDef *)pPropertyDef);
    ASSERTU(propertyDefinition);
    OMProperty *property = iter.property();
    property->initialize(propertyDefinition);
  }
}



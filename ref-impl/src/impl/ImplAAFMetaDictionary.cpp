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

#include "ImplAAFObjectCreation.h"
#include "aafErr.h"


#include <assert.h>

#ifndef USE_AAFOBJECT_MODEL
#define USE_AAFOBJECT_MODEL 0
#endif



#if USE_AAFOBJECT_MODEL

#include "AAFObjectModel.h"
#include "AAFObjectModelProcs.h"

#endif

extern "C" const aafClassID_t CLSID_AAFMetaDictionary;
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
    assert(NULL != pMetaDictionary);
    if (NULL != pMetaDictionary)
    {
      // If we created a dictionary then give it a reference to a factory
      // (dictionary) to satisfy the OMStorable class invariant: Every OMStorabe
      // must have a reference to a factory. Since the meta dictionary is not created
      // by the OMClassFactory interface we just set the factory to "itself".
      //
//
//      pMetaDictionary->setClassFactory(pMetaDictionary);

      // NOTE: We will probably need to change the previous
      // line to use a "setMetaClassFactory()" (to be provided by OM)
      // transdel:2000-APR-11  
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
    assert(AAFRESULT_SUCCEEDED(result));
    storable = pMetaObject;
  }

  // Set the class factory for the meta-data objects to this meta dictionary.
  storable->setClassFactory(this);

  return (storable);
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

// Temporary method to set the 
void ImplAAFMetaDictionary::setDataDictionary(ImplAAFDictionary *dataDictionary)
{
  assert(!_dataDictionary);
  _dataDictionary = dataDictionary;
}

ImplAAFDictionary * ImplAAFMetaDictionary::dataDictionary(void) const
{
  assert (NULL != _dataDictionary);
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
  assert(containsForwardClassReference(classId)); // classId must already be a forward reference!
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
}


// Add the given property definition to the set of axiomatic property definitions.
void ImplAAFMetaDictionary::addAxiomaticPropertyDefinition(ImplAAFPropertyDef *pPropertyDef)
{
  // Save the new axiomatic property definition in a non-persistent set.
  _axiomaticPropertyDefinitions.appendValue(pPropertyDef);
}


// Add the given type definition to the set of axiomatic type definitions.
void ImplAAFMetaDictionary::addAxiomaticTypeDefinition(ImplAAFTypeDef *pTypeDef)
{
  // Save the new axiomatic type definition in a non-persistent set.
  _axiomaticTypeDefinitions.appendValue(pTypeDef);
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
bool ImplAAFMetaDictionary::containsClass(aafUID_constref classId)
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
    assert(NULL != impl);
    return NULL;
  }

  ImplAAFMetaDefinition *metaDefinition = dynamic_cast<ImplAAFMetaDefinition *>(impl);
  if (NULL == metaDefinition)
  {
    assert (metaDefinition);
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
  (*ppMetaObject)->InitOMProperties(pClassDef);


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
    assert(NULL != *ppClassDef);
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
    assert(NULL != *ppTypeDef);
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

#if 0
  ImplEnumAAFTypeDefs *theEnum = (ImplEnumAAFTypeDefs *)CreateImpl (CLSID_EnumAAFTypeDefs);
  
  XPROTECT()
  {
    OMSetIterator<OMUniqueObjectIdentification, ImplAAFMetaDictionary::OpaqueTypeDefinition>* iter = 
      new OMSetIterator<OMUniqueObjectIdentification, ImplAAFMetaDictionary::OpaqueTypeDefinition>(_opaqueTypeDefinitions, OMBefore);
    if(iter == 0)
      RAISE(AAFRESULT_NOMEMORY);
	CHECK(theEnum->Initialize(&CLSID_EnumAAFTypeDefs,this, iter));
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
  
#endif
  return (AAFRESULT_NOT_IMPLEMENTED);
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
#if USE_AAFOBJECT_MODEL
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
#endif // #if USE_AAFOBJECT_MODEL
}

// Create all of the axiomatic properties as uninitialized objects.
void ImplAAFMetaDictionary::CreateAxiomaticProperties(void)
{
#if USE_AAFOBJECT_MODEL
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
#endif // #if USE_AAFOBJECT_MODEL
}

// Create all of the axiomatic types as uninitialized objects.
void ImplAAFMetaDictionary::CreateAxiomaticTypes(void)
{
#if USE_AAFOBJECT_MODEL
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
#endif // #if USE_AAFOBJECT_MODEL
}



// Initialize all of the axiomatic classes with their parent and class
// definitions.
void ImplAAFMetaDictionary::InitializeAxiomaticClasses(void)
{
#if USE_AAFOBJECT_MODEL
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
#endif // #if USE_AAFOBJECT_MODEL
}

// Initialize all of the type definitions with there appropriate class and type
// definitions.
void ImplAAFMetaDictionary::InitializeAxiomaticTypes(void)
{
#if USE_AAFOBJECT_MODEL
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
#endif // #if USE_AAFOBJECT_MODEL
}

// Initialize all of the property definitions with their type definitions.
void ImplAAFMetaDictionary::InitializeAxiomaticProperties(void)
{
#if USE_AAFOBJECT_MODEL
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
#endif // #if USE_AAFOBJECT_MODEL
}


// Register all of the axiomatic properties with their corresponding
//  axiomatic class definitions.
void ImplAAFMetaDictionary::RegisterAxiomaticProperties(void)
{
#if USE_AAFOBJECT_MODEL
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
      assert(pClass);

      for (propertyIndex = 0; propertyIndex < classDefinition->propertyCount(); ++propertyIndex)
      {
        propertyDefinition = classDefinition->propertyDefinitionAt(propertyIndex);
        pProperty = findAxiomaticPropertyDefinition(*propertyDefinition->id());
        assert(pProperty);
        pClass->pvtRegisterExistingPropertyDef(pProperty);
      }
    }
  }
#endif // #if USE_AAFOBJECT_MODEL
}


// Initialize all of the OMProperties for each aximatic definition.
void ImplAAFMetaDictionary::InitializeAxiomaticOMProperties(void)
{
#if USE_AAFOBJECT_MODEL
  ImplAAFClassDef *pClassDef;
 
  // Get the class definition for all classes.
  pClassDef = findAxiomaticClassDefinition(AUID_AAFClassDef);
  assert (pClassDef);

  //
  // Initialize the OM Properties for all of the axiomatic class definitions.
  //
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef> axiomaticClassDefinitions(_axiomaticClassDefinitions, OMBefore);
  while(++axiomaticClassDefinitions)
  {
    ImplAAFClassDef *pClass = axiomaticClassDefinitions.value();
    assert (pClass);
    if (!pClass)
      throw AAFRESULT_INVALID_OBJ;

    pClass->InitOMProperties(pClassDef);
  }

 
  // Get the class definition for all classes.
  pClassDef = findAxiomaticClassDefinition(AUID_AAFPropertyDef);
  assert (pClassDef);

  //
  // Initialize the OM Properties for all of the axiomatic property definitions.
  //
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPropertyDef> axiomaticPropertyDefinitions(_axiomaticPropertyDefinitions, OMBefore);
  while(++axiomaticPropertyDefinitions)
  {
    ImplAAFPropertyDef *pProperty = axiomaticPropertyDefinitions.value();
    assert (pProperty);
    if (!pProperty)
      throw AAFRESULT_INVALID_OBJ;

    pProperty->InitOMProperties(pClassDef);
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
    assert (pType);
    if (!pType)
      throw AAFRESULT_INVALID_OBJ;

    result = pType->GetAUID(&id);
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;
    typeDefinition = objectModel->findTypeDefinition(&id);
    assert (typeDefinition);
    classDefinition = typeDefinition->classDefinition();
    pClassDef = findAxiomaticClassDefinition(*classDefinition->id());
    assert (pType);
    if (!pType)
      throw AAFRESULT_CLASS_NOT_FOUND;

    pType->InitOMProperties(pClassDef);
  }
#endif // #if USE_AAFOBJECT_MODEL
}


  
// Complete the registration of the axiomatic class definitions
// This must be called AFTER all other aximatic definitions have
// been initialized and registered.
void ImplAAFMetaDictionary::CompleteAxiomaticClassRegistration(void) // throw AAFRESULT
{
#if USE_AAFOBJECT_MODEL
  AAFRESULT result = AAFRESULT_SUCCESS;
  OMReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef> axiomaticClassDefinitions(_axiomaticClassDefinitions, OMBefore);
  while(++axiomaticClassDefinitions)
  {
    ImplAAFClassDef *pClass = axiomaticClassDefinitions.value();
    assert (pClass);
    if (!pClass)
      throw AAFRESULT_INVALID_OBJ;

    result = pClass->CompleteClassRegistration();
    if (AAFRESULT_FAILED(result))
      throw result;
  }
#endif // #if USE_AAFOBJECT_MODEL
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
  InitializeAxiomaticOMProperties();
  
  CompleteAxiomaticClassRegistration();
}

// Create and initialize all of the axiomatic definitions.
AAFRESULT ImplAAFMetaDictionary::InstantiateAxiomaticDefinitions(void)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  try
  {
#if USE_AAFOBJECT_MODEL
    InstallAAFObjectModelProcs();
#endif // #if USE_AAFOBJECT_MODEL

    CreateAxiomaticDefinitions();
    InitializeAxiomaticDefinitions();
  }
  catch (AAFRESULT &rError)
  {
    result = rError;
  }

  return (result);
}


//
// Meta definition factory methods:
//

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDictionary::CreateClassDef (
      aafUID_constref /*classID*/,
      aafCharacter_constptr pClassName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFClassDef * pParentClass,
      ImplAAFClassDef **ppNewClass)
{
  if (!pClassName || ! pParentClass || !ppNewClass) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefVariableArray (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDef *pElementType,
      ImplAAFTypeDefVariableArray ** ppNewVariableArray)
{
  if (!pTypeName || !pElementType || !ppNewVariableArray) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefFixedArray (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDef *pElementType,
      aafUInt32  /*nElements*/,
      ImplAAFTypeDefFixedArray **pNewFixedArray)
{
  if (!pTypeName || !pElementType || !pNewFixedArray) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDictionary::CreateTypeDefRecord (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDef ** ppMemberTypes,
      aafCharacter_constptr * pMemberNames,
      aafUInt32 /*numMembers*/,
      ImplAAFTypeDefRecord ** ppNewRecord)
{
  if (!pTypeName || !ppMemberTypes || !pMemberNames || !ppNewRecord) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMetaDictionary::CreateTypeDefRename (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDef *pBaseType,
      ImplAAFTypeDefRename ** ppNewRename)
{
  if (!pTypeName || !pBaseType || !ppNewRename) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefString (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDef *pElementType,
      ImplAAFTypeDefString ** ppNewString)
{
  if (!pTypeName || !pElementType || !ppNewString) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefStrongObjRef (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFClassDef * pTargetObjType,
      ImplAAFTypeDefStrongObjRef ** ppNewStrongObjRef)
{
  if (!pTypeName || !pTargetObjType || !ppNewStrongObjRef) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefWeakObjRef (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFClassDef * pTargetObjType,
      aafUID_constptr * pTargetHint,
      aafUInt32 /*targetHintCount*/,
      ImplAAFTypeDefWeakObjRef ** ppNewWeakObjRef)
{
  if (!pTypeName || !pTargetObjType || !pTargetHint || !ppNewWeakObjRef) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefStrongObjRefVector (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDefStrongObjRef * pStrongObjRef,
      ImplAAFTypeDefVariableArray ** ppNewStrongObjRefVector)
{
  if (!pTypeName || !pStrongObjRef || !ppNewStrongObjRefVector) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefWeakObjRefVector (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDefWeakObjRef * pWeakObjRef,
      ImplAAFTypeDefVariableArray ** ppNewWeakObjRefVector)
{
  if (!pTypeName || !pWeakObjRef || !ppNewWeakObjRefVector) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefStrongObjRefSet (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDefStrongObjRef * pStrongObjRef,
      ImplAAFTypeDefSet ** ppNewStrongObjRefSet)
{
  if (!pTypeName || !pStrongObjRef || !ppNewStrongObjRefSet) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFMetaDictionary::CreateTypeDefWeakObjRefSet (
      aafUID_constref /*typeID*/,
      aafCharacter_constptr pTypeName,
      aafCharacter_constptr /*pDescription*/,
      ImplAAFTypeDefWeakObjRef * pWeakObjRef,
      ImplAAFTypeDefSet ** ppNewWeakObjRefSet)
{
  if (!pTypeName || !pWeakObjRef || !ppNewWeakObjRefSet) 
    return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}




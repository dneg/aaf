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
#ifndef USE_AAFOBJECT_MODEL
#define USE_AAFOBJECT_MODEL 0
#endif

#if USE_AAFOBJECT_MODEL



// Enable/disable transitional code...
#define USE_VECTORS_INSTEAD_SETS_FOR_COMPATIBILITY 1


#include "AAFObjectModelProcs.h"
#include "AAFObjectModel.h"

// TEMPORARY: Up-cast to dictionary. This will change for the "two-root" meta dictionary.
#include "ImplAAFDictionary.h"

#include "ImplAAFMetaDictionary.h"
#include "ImplAAFClassDef.h"
#include "ImplAAFTypeDefInt.h"
#include "ImplAAFTypeDefEnum.h"
#include "ImplAAFTypeDefVariableArray.h"
#include "ImplAAFTypeDefFixedArray.h"
#include "ImplAAFTypeDefRecord.h"
#include "ImplAAFTypeDefRename.h"
#include "ImplAAFTypeDefCharacter.h"
#include "ImplAAFTypeDefString.h"
//#include "ImplAAFTypeDefExtEnum.h"
#include "ImplAAFTypeDefStrongObjRef.h"
#include "ImplAAFTypeDefWeakObjRef.h"
#include "ImplAAFTypeDefSet.h"
#include "ImplAAFTypeDefStream.h"
#include "ImplAAFTypeDefIndirect.h"
#include "ImplAAFTypeDefOpaque.h"

#include "ImplAAFBaseClassFactory.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"
#include "ImplAAFObjectCreation.h"
#include "AAFResult.h"

#include <assert.h>




#ifdef __cplusplus
extern "C" {
#endif

bool CreateClassDefinition(const ClassDefinition *, ImplAAFMetaDictionary *);
bool InitializeClassDefinition(const ClassDefinition *, ImplAAFMetaDictionary *);

bool CreatePropertyDefinition(const PropertyDefinition *, ImplAAFMetaDictionary *);
bool InitializePropertyDefinition(const PropertyDefinition *, ImplAAFMetaDictionary *);


bool CreateTypeDefinition(const TypeDefinition *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinition(const TypeDefinition *, ImplAAFMetaDictionary *);


bool CreateTypeDefinitionInteger(const TypeDefinitionInteger *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionInteger(const TypeDefinitionInteger *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionEnumeration(const TypeDefinitionEnumeration *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionEnumeration(const TypeDefinitionEnumeration *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionVariableArray(const TypeDefinitionVariableArray *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionVariableArray(const TypeDefinitionVariableArray *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionFixedArray(const TypeDefinitionFixedArray *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionFixedArray(const TypeDefinitionFixedArray *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionRecord(const TypeDefinitionRecord *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionRecord(const TypeDefinitionRecord *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionRename(const TypeDefinitionRename *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionRename(const TypeDefinitionRename *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionCharacter(const TypeDefinitionCharacter *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionCharacter(const TypeDefinitionCharacter *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionString(const TypeDefinitionString *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionString(const TypeDefinitionString *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionExtendibleEnumeration(const TypeDefinitionExtendibleEnumeration *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionExtendibleEnumeration(const TypeDefinitionExtendibleEnumeration *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionStrongReference(const TypeDefinitionStrongReference *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionStrongReference(const TypeDefinitionStrongReference *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionStrongReferenceSet(const TypeDefinitionStrongReferenceSet *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionStrongReferenceSet(const TypeDefinitionStrongReferenceSet *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionStrongReferenceVector(const TypeDefinitionStrongReferenceVector *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionStrongReferenceVector(const TypeDefinitionStrongReferenceVector *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionWeakReference(const TypeDefinitionWeakReference *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionWeakReference(const TypeDefinitionWeakReference *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionWeakReferenceSet(const TypeDefinitionWeakReferenceSet *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionWeakReferenceSet(const TypeDefinitionWeakReferenceSet *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionWeakReferenceVector(const TypeDefinitionWeakReferenceVector *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionWeakReferenceVector(const TypeDefinitionWeakReferenceVector *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionStream(const TypeDefinitionStream *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionStream(const TypeDefinitionStream *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionIndirect(const TypeDefinitionIndirect *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionIndirect(const TypeDefinitionIndirect *, ImplAAFMetaDictionary *);

bool CreateTypeDefinitionOpaque(const TypeDefinitionOpaque *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionOpaque(const TypeDefinitionOpaque *, ImplAAFMetaDictionary *);


#if USE_VECTORS_INSTEAD_SETS_FOR_COMPATIBILITY
bool CreateTypeDefinitionSetAsVariableArray(const TypeDefinitionSet *, ImplAAFMetaDictionary *);
bool InitializeTypeDefinitionSetAsVariableArray(const TypeDefinitionSet *, ImplAAFMetaDictionary *);
#endif


#ifdef __cplusplus
}
#endif




// Install all of the callback procs into the MetaDictionary.
void InstallAAFObjectModelProcs(void)
{
  bool sInstalled = false;

  if (sInstalled)
  {
    // This is a one-time initialization. The object model call backs
    // are shared by all meta dictionaries.
    return;
  }


  //
  // Install all of the shared callback procs.
  //
  aafUInt32 i;

  // Get the one-and-only meta dictionary.
  const AAFObjectModel *objectModel = AAFObjectModel::singleton();

  // Install the callbacks for the class definitions.
  // NOTE: The first version uses the same functions for 
  // both axiomatic and normal built-in classes (properties,
  // and types).
  for (i = 0; i < objectModel->countClassDefinitions(); ++i)
  {
    const ClassDefinition *classDefinition = objectModel->classDefinitionAt(i);

    const_cast<ClassDefinition *>(classDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateClassDefinition);
    const_cast<ClassDefinition *>(classDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeClassDefinition);
  }

  // Install the callbacks for the property definitions.
  for (i = 0; i < objectModel->countPropertyDefinitions(); ++i)
  {
    const PropertyDefinition * propertyDefinition = objectModel->propertyDefinitionAt(i);

    const_cast<PropertyDefinition *>(propertyDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreatePropertyDefinition);
    const_cast<PropertyDefinition *>(propertyDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializePropertyDefinition);
  }

  // Install the callbacks for the type definitions.
  for (i = 0; i < objectModel->countTypeDefinitions(); ++i)
  {
    const TypeDefinition *typeDefinition;
    typeDefinition = objectModel->typeDefinitionAt(i);
    
    switch (typeDefinition->category())
    {
    case kAAFTypeCatInt:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionInteger);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionInteger);
      break;

    case kAAFTypeCatCharacter:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionCharacter);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionCharacter);
      break;

    case kAAFTypeCatStrongObjRef:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionStrongReference);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionStrongReference);
      break;

    case kAAFTypeCatWeakObjRef:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionWeakReference);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionWeakReference);
      break;

    case kAAFTypeCatRename:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionRename);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionRename);
      break;

    case kAAFTypeCatEnum:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionEnumeration);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionEnumeration);
      break;

    case kAAFTypeCatFixedArray:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionFixedArray);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionFixedArray);
      break;

    case kAAFTypeCatVariableArray:
      if (dynamic_cast<const TypeDefinitionStrongReferenceVector *>(typeDefinition))
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionStrongReferenceVector);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionStrongReferenceVector);
      }
      else if (dynamic_cast<const TypeDefinitionWeakReferenceVector *>(typeDefinition))
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionWeakReferenceVector);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionWeakReferenceVector);
      }
      else
      {
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionVariableArray);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionVariableArray);
      }
      break;

    case kAAFTypeCatSet:
      if (dynamic_cast<const TypeDefinitionStrongReferenceSet*>(typeDefinition))
      {
#if USE_VECTORS_INSTEAD_SETS_FOR_COMPATIBILITY
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionSetAsVariableArray);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionSetAsVariableArray);
#else
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionStrongReferenceSet);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionStrongReferenceSet);
#endif
      }
      else if (dynamic_cast<const TypeDefinitionWeakReferenceSet*>(typeDefinition))
      {
#if USE_VECTORS_INSTEAD_SETS_FOR_COMPATIBILITY
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionSetAsVariableArray);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionSetAsVariableArray);
#else
        const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionWeakReferenceSet);
        const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionWeakReferenceSet);
#endif
      }
      else
      {
        assert (dynamic_cast<const TypeDefinitionStrongReferenceSet*>(typeDefinition) ||
                dynamic_cast<const TypeDefinitionWeakReferenceSet*>(typeDefinition));
      }
      break;

    case kAAFTypeCatRecord:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionRecord);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionRecord);
      break;

    case kAAFTypeCatStream:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionStream);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionStream);
      break;

    case kAAFTypeCatString:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionString);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionString);
      break;

    case kAAFTypeCatExtEnum:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionExtendibleEnumeration);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionExtendibleEnumeration);
      break;

    case kAAFTypeCatIndirect:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionIndirect);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionIndirect);
      break;

    case kAAFTypeCatOpaque:
      const_cast<TypeDefinition *>(typeDefinition)->setCreateDefinitionProc((CreateDefinitionProcType)CreateTypeDefinitionOpaque);
      const_cast<TypeDefinition *>(typeDefinition)->setInitializeDefinitionProc((InitializeDefinitionProcType)InitializeTypeDefinitionOpaque);
      break;

//    case kAAFTypeCatEncrypted:
//      break;

    default:
      // Unknown type!
      assert (0);
      break;
    }
  }

  
  // All of the callback procs have been installed.
  sInstalled = true;
}






//
// ClassDefinition callbacks
//
bool CreateClassDefinition(
  const ClassDefinition *classDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFClassDef *pClass = NULL;

  
  if (classDefinition->axiomatic())
  {
    // Attempt to create the assoication instance.
    pClass = static_cast<ImplAAFClassDef *>(metaDictionary->pvtCreateMetaDefinition(AUID_AAFClassDef));
    assert (pClass);
    if (!pClass)
      throw AAFRESULT_NOMEMORY;

    // Initialize just the id and name. All other properties must be initialized
    // after all of the classes have been created.
//    result = static_cast<ImplAAFMetaDefinition *>(pClass)->Initialize(*classDefinition->id(), classDefinition->name(), NULL);
    result = pClass->SetIdentification(*classDefinition->id());
    if (AAFRESULT_FAILED(result))
    {
      pClass->ReleaseReference();
      pClass = NULL;
      throw result;
    }

    // Save the new axiomatic class definition in a non-persistent set.
    metaDictionary->addAxiomaticClassDefinition(pClass);

    return true;
  }
  else // if (classDefinition->axiomatic())
  {
    return false;
  }
}

bool InitializeClassDefinition(
  const ClassDefinition *classDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFClassDef *pClass = NULL;
  ImplAAFClassDef *pParentClass = NULL;
  aafBoolean_t isConcrete;


  if (classDefinition->axiomatic())
  {
    pClass = metaDictionary->findAxiomaticClassDefinition(*classDefinition->id());
    assert (pClass);
    if (!pClass)
      return false;

    result = pClass->SetName(classDefinition->name());
    assert (AAFRESULT_SUCCEEDED(result));

    // Find and set the parent class definition. (This must also be axiomatic.)
    if (!classDefinition->isRoot())
    {
      pParentClass = metaDictionary->findAxiomaticClassDefinition(*classDefinition->parentId());
      assert (pParentClass);
      result = pClass->SetBootstrapParent(pParentClass);
      assert (AAFRESULT_SUCCEEDED(result));
      result = pClass->SetParent(pParentClass);
      assert (AAFRESULT_SUCCEEDED(result));
    }
    else
    {
      result = pClass->SetBootstrapParent(pClass);
      assert (AAFRESULT_SUCCEEDED(result));
      result = pClass->SetParent(pClass);
      assert (AAFRESULT_SUCCEEDED(result));
    }
    if (AAFRESULT_FAILED(result))
      return false;

    // Is this a concrete class.
    isConcrete = (classDefinition->concrete()) ? kAAFTrue : kAAFFalse;
    pClass->pvtSetIsConcrete(isConcrete); // no result
    return true;
  }
  else // if (classDefinition->axiomatic())
  {
    return false;
  }
}


//
// TEMPORARY:
// Callback procs to create OM Properties for the associated property definition.
// In the DM the ImplAAFPropertyDef is a factory for OM Properties.
//
static OMProperty * CreateOMPropTypeSimple
  (OMPropertyId /*pid*/,
   const wchar_t * /*name*/)
{
  // Let's return 0 so the caller will attempt to use the type def to
  // create the property.
  return 0;
}

static OMProperty * CreateOMPropTypeWeakReference
  (OMPropertyId pid,
   const wchar_t * name)
{
  assert (name);
//  if(pid == PID_SourceReference_SourceID)
//    return new OMSimpleProperty (pid, name, sizeof (aafMobID_t));
//  else
    return new OMSimpleProperty (pid, name, sizeof (aafUID_t));
}

static OMProperty * CreateOMPropTypeWeakReferenceSet
  (OMPropertyId pid,
   const wchar_t * name)
{
  assert (name);
  return new OMSimpleProperty (pid, name, sizeof (aafUID_t));
}

static OMProperty * CreateOMPropTypeWeakReferenceVector
  (OMPropertyId pid,
   const wchar_t * name)
{
  assert (name);
  return new OMSimpleProperty (pid, name, sizeof (aafUID_t));
}

static OMProperty * CreateOMPropTypeStrongReference
  (OMPropertyId pid,
   const wchar_t * name)
{
  assert (name);
  return new OMStrongReferenceProperty<ImplAAFObject> (pid, name);
}

static OMProperty * CreateOMPropTypeStrongReferenceSet
  (OMPropertyId pid,
   const wchar_t * name)
{
  assert (name);
  return new OMStrongReferenceVectorProperty<ImplAAFObject> (pid, name);
}

static OMProperty * CreateOMPropTypeStrongReferenceVector
  (OMPropertyId pid,
   const wchar_t * name)
{
  assert (name);
  return new OMStrongReferenceVectorProperty<ImplAAFObject> (pid, name);
}


static ImplAAFOMPropertyCreateFunc_t SelectOMPropertyFactoryFunction(const TypeDefinition *typeDefinition)
{
  ImplAAFOMPropertyCreateFunc_t pFunc = NULL;

  // TEMPORARY:
  // Assign callback function to create the "appropriate" OMProperty.
  //
  switch (typeDefinition->category())
  {
  case kAAFTypeCatInt:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatCharacter:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatStrongObjRef:
    pFunc = CreateOMPropTypeStrongReference;
    break;

  case kAAFTypeCatWeakObjRef:
    pFunc = CreateOMPropTypeWeakReference;
    break;

  case kAAFTypeCatRename:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatEnum:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatFixedArray:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatVariableArray:
    if (dynamic_cast<const TypeDefinitionStrongReferenceVector *>(typeDefinition))
    {
      pFunc = CreateOMPropTypeStrongReferenceVector;
    }
    else if (dynamic_cast<const TypeDefinitionWeakReferenceVector *>(typeDefinition))
    {
      pFunc = CreateOMPropTypeWeakReferenceVector;
    }
    else
    {
      pFunc = CreateOMPropTypeSimple;
    }
    break;

  case kAAFTypeCatSet:
    if (dynamic_cast<const TypeDefinitionStrongReferenceSet*>(typeDefinition))
    {
      pFunc = CreateOMPropTypeStrongReferenceSet;
    }
    else if (dynamic_cast<const TypeDefinitionWeakReferenceSet*>(typeDefinition))
    {
      pFunc = CreateOMPropTypeWeakReferenceSet;
    }
    else
    {
      assert (dynamic_cast<const TypeDefinitionStrongReferenceSet*>(typeDefinition) ||
              dynamic_cast<const TypeDefinitionWeakReferenceSet*>(typeDefinition));
    }
    break;

  case kAAFTypeCatRecord:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatStream:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatString:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatExtEnum:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatIndirect:
    pFunc = CreateOMPropTypeSimple;
    break;

  case kAAFTypeCatOpaque:
    pFunc = CreateOMPropTypeSimple;
    break;

//    case kAAFTypeCatEncrypted:
//      break;

  default:
    // Unknown type!
    assert (0);
    break;
  }

  return pFunc;
}

//
// PropertyDefinition/ImplAAFPropertyDef callbacks
//
bool CreatePropertyDefinition(
  const PropertyDefinition *propertyDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFPropertyDef *pProperty = NULL;
  AAFRESULT result;

  
  if (propertyDefinition->axiomatic())
  {
    // Attempt to create the assoication instance.
    pProperty = static_cast<ImplAAFPropertyDef *>(metaDictionary->pvtCreateMetaDefinition(AUID_AAFPropertyDef));
    assert (pProperty);
    if (!pProperty)
      throw AAFRESULT_NOMEMORY;

    // Initialize just the id and name. All other properties must be initialized
    // after all of the classes and types have been created.
//    result = static_cast<ImplAAFMetaDefinition *>(pProperty)->Initialize(*propertyDefinition->id(), propertyDefinition->name(), NULL);
    result = pProperty->SetIdentification(*propertyDefinition->id());
    if (AAFRESULT_FAILED(result))
    {
      pProperty->ReleaseReference();
      pProperty = NULL;
      throw result;
    }


    // TEMPORARY:
    // In the DM the ImplAAFPropertyDef is a factory for OM Properties.
    //
    pProperty->SetOMPropCreateFunc(SelectOMPropertyFactoryFunction(propertyDefinition->typeDefinition()));


    // Save the new axiomatic property definition in a non-persistent set.
    metaDictionary->addAxiomaticPropertyDefinition(pProperty);

    return true;
  }
  else // if (propertyDefinition->axiomatic())
  {
    return false;
  }
}


bool InitializePropertyDefinition(
  const PropertyDefinition *propertyDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFPropertyDef *pProperty = NULL;
  ImplAAFTypeDef *pType = NULL;
  const TypeDefinition *typeDefinition = NULL;
  AAFRESULT result;

  
  if (propertyDefinition->axiomatic())
  {
    pProperty = metaDictionary->findAxiomaticPropertyDefinition(*propertyDefinition->id());
    assert (pProperty);

    typeDefinition = propertyDefinition->typeDefinition();
    pType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinition->id());
    assert (pType);

    result = pProperty->pvtInitialize (
      *propertyDefinition->id(),
      propertyDefinition->pid(),
      propertyDefinition->name(),
      pType,
      (propertyDefinition->required()) ? kAAFFalse : kAAFTrue,
      (propertyDefinition->uid()) ? kAAFTrue : kAAFFalse);
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinition callbacks
//
bool CreateTypeDefinition(
  const TypeDefinition *typeDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  const ClassDefinition *classDefinition;
  ImplAAFTypeDef *pType = NULL;
  AAFRESULT result;

  // We cannot create abstract types.
  if (!typeDefinition->concrete())
    return false;

  if (typeDefinition->axiomatic())
  {
    // Attempt to create the assoication type definition instance.
    classDefinition = typeDefinition->classDefinition();
    pType = static_cast<ImplAAFTypeDef *>(metaDictionary->pvtCreateMetaDefinition(*classDefinition->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_NOMEMORY;

    // Initialize just the id. All other properties must be initialized
    // after all of the classes and types have been created.
//    result = static_cast<ImplAAFMetaDefinition *>(pType)->Initialize(*typeDefinition->id(), typeDefinition->name(), NULL);
    result = pType->SetIdentification(*typeDefinition->id());
    if (AAFRESULT_FAILED(result))
    {
      pType->ReleaseReference();
      pType = NULL;
      throw result;
    }

    // Save the new axiomatic type definition in a non-persistent set.
    metaDictionary->addAxiomaticTypeDefinition(pType);

    return true;
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinition(
  const TypeDefinition *typeDefinition, 
  ImplAAFMetaDictionary *metaDictionary)
{
  return false;
}


//
// TypeDefinitionInteger/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionInteger(
  const TypeDefinitionInteger *typeDefinitionInteger, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionInteger->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionInteger, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionInteger(
  const TypeDefinitionInteger *typeDefinitionInteger, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefInt *pType = NULL;
  AAFRESULT result;


  if (typeDefinitionInteger->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefInt *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionInteger->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->Initialize(*typeDefinitionInteger->id(), 
                               typeDefinitionInteger->size(), 
                               (typeDefinitionInteger->signedNumber()) ? kAAFTrue : kAAFFalse,
                               typeDefinitionInteger->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionEnumeration/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionEnumeration(
  const TypeDefinitionEnumeration *typeDefinitionEnumeration, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionEnumeration->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionEnumeration, metaDictionary);
  }
  else
  {
    return false;
  }
}


static void InitializeTypeDefinitionEnumeration_cleanup(aafInt64 *memberValues, aafCharacter_constptr *memberNames)
{
  if (memberValues)
    delete [] memberValues;
  if (memberNames)
    delete [] memberNames;
}

bool InitializeTypeDefinitionEnumeration(
  const TypeDefinitionEnumeration *typeDefinitionEnumeration, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefEnum *pType = NULL;
  ImplAAFTypeDefInt *pElementType = NULL;
  AAFRESULT result;
  aafInt64 *memberValues = NULL;
  aafCharacter_constptr *memberNames = NULL;
  aafUInt32 index;


  if (typeDefinitionEnumeration->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefEnum *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionEnumeration->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    // Find element type.
    const TypeDefinitionInteger *typeDefinitionInteger = typeDefinitionEnumeration->typeDefinition();
    pElementType = dynamic_cast<ImplAAFTypeDefInt *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionInteger->id()));
    assert (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    // Allocate arrays to hold the values and the names.
    memberValues = new aafInt64 [typeDefinitionEnumeration->memberCount()];
    if (NULL == memberValues)
      throw AAFRESULT_NOMEMORY;
    memberNames = new aafCharacter_constptr [typeDefinitionEnumeration->memberCount()];
    if (NULL == memberNames)
    {
      InitializeTypeDefinitionEnumeration_cleanup(memberValues, memberNames);
      throw AAFRESULT_NOMEMORY;
    }

    // Initialize the arrays from the static object model.
    const DefinitionEnumerationMember * member;
    for (index = 0; index < typeDefinitionEnumeration->memberCount(); ++index)
    {
      member = typeDefinitionEnumeration->memberAt(index);
      memberValues[index] = member->value();
      memberNames[index] = member->name();
    }

    result = pType->Initialize (*typeDefinitionEnumeration->id(),
        pElementType,
        memberValues,
        const_cast<aafCharacter **>(memberNames), // THIS CAST SHOULD NOT BE NECESSARY!
        typeDefinitionEnumeration->memberCount(),
        typeDefinitionEnumeration->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
    {
      InitializeTypeDefinitionEnumeration_cleanup(memberValues, memberNames);
      throw result;
    }


    // Register the enumeration size.
    result = pType->RegisterSize(typeDefinitionInteger->size());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
    {
      InitializeTypeDefinitionEnumeration_cleanup(memberValues, memberNames);
      throw result;
    }

   InitializeTypeDefinitionEnumeration_cleanup(memberValues, memberNames);
   return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionVariableArray/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionVariableArray(
  const TypeDefinitionVariableArray *typeDefinitionVariableArray, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionVariableArray->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionVariableArray, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionVariableArray(
  const TypeDefinitionVariableArray *typeDefinitionVariableArray, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefVariableArray *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionVariableArray->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefVariableArray *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionVariableArray->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionVariableArray->elementTypeId());
    assert (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionVariableArray->id(),
                                  pElementType,
                                  typeDefinitionVariableArray->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionFixedArray/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionFixedArray(
  const TypeDefinitionFixedArray *typeDefinitionFixedArray, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionFixedArray->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionFixedArray, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionFixedArray(
  const TypeDefinitionFixedArray *typeDefinitionFixedArray, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefFixedArray *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionFixedArray->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefFixedArray *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionFixedArray->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionFixedArray->elementTypeId());
    assert (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionFixedArray->id(),
                                  pElementType,
                                  typeDefinitionFixedArray->elementCount(),
                                  typeDefinitionFixedArray->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}




//
// TypeDefinition/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionRecord(
  const TypeDefinitionRecord *typeDefinitionRecord, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionRecord->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionRecord, metaDictionary);
  }
  else
  {
    return false;
  }
}



static void InitializeTypeDefinitionRecord_cleanup(
  ImplAAFTypeDef ** memberTypes, 
  aafCharacter_constptr *memberNames, 
  aafUInt32 * memberOffsets)
{
  if (memberTypes)
    delete [] memberTypes;
  if (memberNames)
    delete [] memberNames;
  if (memberOffsets)
    delete [] memberOffsets;
}


bool InitializeTypeDefinitionRecord(
  const TypeDefinitionRecord *typeDefinitionRecord, 
  ImplAAFMetaDictionary *metaDictionary)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFTypeDefRecord *pType = NULL;
  ImplAAFTypeDef ** memberTypes = NULL;
  aafCharacter_constptr * memberNames = NULL;
  aafUInt32 * memberOffsets = NULL;
  aafUInt32 fieldIndex;
  const DefinitionRecordField *field = NULL;


  if (typeDefinitionRecord->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefRecord *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionRecord->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    // allocate arrays to hold memberTypes, memberNames, and memberOffsets.
    memberTypes = new ImplAAFTypeDef * [typeDefinitionRecord->fieldCount()];
    assert (memberTypes);
    if (!memberTypes)
      throw AAFRESULT_NOMEMORY;

    memberNames = new aafCharacter_constptr[typeDefinitionRecord->fieldCount()];
    assert (memberNames);
    if (!memberNames)
    {
      InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);
      throw AAFRESULT_NOMEMORY;
    }

    memberOffsets = new aafUInt32[typeDefinitionRecord->fieldCount()];
    assert (memberOffsets);
    if (!memberOffsets)
    {
      InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);
      throw AAFRESULT_NOMEMORY;
    }

    for (fieldIndex = 0; fieldIndex < typeDefinitionRecord->fieldCount(); ++fieldIndex)
    {
      field = typeDefinitionRecord->fieldAt(fieldIndex);
      memberNames[fieldIndex] = field->name();
      memberOffsets[fieldIndex] = field->fieldOffset();
      memberTypes[fieldIndex] = metaDictionary->findAxiomaticTypeDefinition(*field->typeId());
      assert(memberTypes[fieldIndex]);
      if (!memberTypes[fieldIndex])
        throw AAFRESULT_TYPE_NOT_FOUND;
    }

    // Used the "high-level" Initialize method since the pvtInitialize is obsolete.
    result = pType->Initialize (*typeDefinitionRecord->id(),
                                memberTypes,
        const_cast<aafCharacter **>(memberNames), // THIS CAST SHOULD NOT BE NECESSARY!
                                typeDefinitionRecord->fieldCount(),
                                typeDefinitionRecord->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
    {
      InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);
      throw result;
    }

    result = pType->RegisterMembers (memberOffsets,
                                     typeDefinitionRecord->fieldCount(),
                                     typeDefinitionRecord->recordSize());
    assert (AAFRESULT_SUCCEEDED (result));
    if (AAFRESULT_FAILED(result))
    {
      InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);
      throw result;
    }


    InitializeTypeDefinitionRecord_cleanup(memberTypes, memberNames, memberOffsets);

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionRename/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionRename(
  const TypeDefinitionRename *typeDefinitionRename, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionRename->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionRename, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionRename(
  const TypeDefinitionRename *typeDefinitionRename, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefRename *pType = NULL;
  ImplAAFTypeDef *pBaseType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionRename->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefRename *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionRename->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pBaseType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionRename->renamedTypeId());
    assert (pBaseType);
    if (!pBaseType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->Initialize(*typeDefinitionRename->id(),
                               pBaseType,
                               typeDefinitionRename->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionCharacter/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionCharacter(
  const TypeDefinitionCharacter *typeDefinitionCharacter, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionCharacter->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionCharacter, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionCharacter(
  const TypeDefinitionCharacter *typeDefinitionCharacter, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefCharacter *pType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionCharacter->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefCharacter *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionCharacter->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionCharacter->id(), typeDefinitionCharacter->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionString/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionString(
  const TypeDefinitionString *typeDefinitionString, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionString->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionString, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionString(
  const TypeDefinitionString *typeDefinitionString, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefString *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionString->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefString *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionString->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionString->stringTypeId());
    assert (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionString->id(),
                                  pElementType,
                                  typeDefinitionString->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionExtendibleEnumeration/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionExtendibleEnumeration(
  const TypeDefinitionExtendibleEnumeration *typeDefinitionExtendibleEnumeration, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionExtendibleEnumeration->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionExtendibleEnumeration, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionExtendibleEnumeration(
  const TypeDefinitionExtendibleEnumeration *typeDefinitionExtendibleEnumeration, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionExtendibleEnumeration->axiomatic())
  {
    // There are currently no aximatic extendible enumerators 2000-MAY-06.
    // We will have to implement this method if this changes in the 
    // future.
    assert (0);
    return false;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionStrongReference/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionStrongReference(
  const TypeDefinitionStrongReference *typeDefinitionStrongReference, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStrongReference->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionStrongReference, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionStrongReference(
  const TypeDefinitionStrongReference *typeDefinitionStrongReference, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefStrongObjRef *pType = NULL;
  ImplAAFClassDef *pTarget = NULL;
  AAFRESULT result;
  
  if (typeDefinitionStrongReference->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefStrongObjRef *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStrongReference->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pTarget = metaDictionary->findAxiomaticClassDefinition(*typeDefinitionStrongReference->targetId());
    assert (pTarget);
    if (!pTarget)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionStrongReference->id(),
                                  pTarget,
                                  typeDefinitionStrongReference->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}



#if USE_VECTORS_INSTEAD_SETS_FOR_COMPATIBILITY
//
// TypeDefinitionSetAsVariableArray
//
bool CreateTypeDefinitionSetAsVariableArray(
  const TypeDefinitionSet *typeDefinitionSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefVariableArray *pType = NULL;
  AAFRESULT result;

  // We cannot create abstract types.
  if (!typeDefinitionSet->concrete())
    return false;

  if (typeDefinitionSet->axiomatic())
  {
    // Attempt to create the assoication type definition instance.
    pType = static_cast<ImplAAFTypeDefVariableArray *>(metaDictionary->pvtCreateMetaDefinition(AUID_AAFTypeDefVariableArray));
    assert (pType);
    if (!pType)
      throw AAFRESULT_NOMEMORY;

    // Initialize just the id. All other properties must be initialized
    // after all of the classes and types have been created.
    result = pType->SetIdentification(*typeDefinitionSet->id());
    if (AAFRESULT_FAILED(result))
    {
      pType->ReleaseReference();
      pType = NULL;
      throw result;
    }

    // Save the new axiomatic type definition in a non-persistent set.
    metaDictionary->addAxiomaticTypeDefinition(pType);

    return true;
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionSetAsVariableArray(
  const TypeDefinitionSet *typeDefinitionSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefVariableArray *pType = NULL;
  ImplAAFTypeDefObjectRef *pElementType = NULL;
  AAFRESULT result;

  
  if (typeDefinitionSet->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefVariableArray *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionSet->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = dynamic_cast<ImplAAFTypeDefObjectRef *>
                   (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionSet->elementTypeId()));
    assert (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionSet->id(),
                                  pElementType,
                                  typeDefinitionSet->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}

#endif // #if USE_VECTORS_INSTEAD_SETS_FOR_COMPATIBILITY



//
// TypeDefinitionStrongReferenceSet/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionStrongReferenceSet(
  const TypeDefinitionStrongReferenceSet *typeDefinitionStrongReferenceSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStrongReferenceSet->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionStrongReferenceSet, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionStrongReferenceSet(
  const TypeDefinitionStrongReferenceSet *typeDefinitionStrongReferenceSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefSet *pType = NULL;
  ImplAAFTypeDefStrongObjRef *pElementType = NULL;
  AAFRESULT result;

  
  if (typeDefinitionStrongReferenceSet->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefSet *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStrongReferenceSet->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = dynamic_cast<ImplAAFTypeDefStrongObjRef *>
                   (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStrongReferenceSet->elementTypeId()));
    assert (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionStrongReferenceSet->id(),
                                  pElementType,
                                  typeDefinitionStrongReferenceSet->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionStrongReferenceVector/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionStrongReferenceVector(
  const TypeDefinitionStrongReferenceVector *typeDefinitionStrongReferenceVector, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStrongReferenceVector->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionStrongReferenceVector, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionStrongReferenceVector(
  const TypeDefinitionStrongReferenceVector *typeDefinitionStrongReferenceVector, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStrongReferenceVector->axiomatic())
  {
    // Vectors are still implemented as variable arrays in the DM.
    return InitializeTypeDefinitionVariableArray(typeDefinitionStrongReferenceVector, metaDictionary);
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionWeakReference/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionWeakReference(
  const TypeDefinitionWeakReference *typeDefinitionWeakReference, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionWeakReference->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionWeakReference, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionWeakReference(
  const TypeDefinitionWeakReference *typeDefinitionWeakReference, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefWeakObjRef *pType = NULL;
  ImplAAFClassDef *pTarget = NULL;
  AAFRESULT result;
  
  if (typeDefinitionWeakReference->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefWeakObjRef *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionWeakReference->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pTarget = metaDictionary->findAxiomaticClassDefinition(*typeDefinitionWeakReference->targetId());
    assert (pTarget);
    if (!pTarget)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionWeakReference->id(),
                                  pTarget,
                                  typeDefinitionWeakReference->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionWeakReferenceSet/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionWeakReferenceSet(
  const TypeDefinitionWeakReferenceSet *typeDefinitionWeakReferenceSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionWeakReferenceSet->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionWeakReferenceSet, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionWeakReferenceSet(
  const TypeDefinitionWeakReferenceSet *typeDefinitionWeakReferenceSet, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefSet *pType = NULL;
  ImplAAFTypeDefWeakObjRef *pElementType = NULL;
  AAFRESULT result;

  
  if (typeDefinitionWeakReferenceSet->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefSet *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionWeakReferenceSet->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = dynamic_cast<ImplAAFTypeDefWeakObjRef *>
                   (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionWeakReferenceSet->elementTypeId()));
    assert (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionWeakReferenceSet->id(),
                                  pElementType,
                                  typeDefinitionWeakReferenceSet->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionWeakReferenceVector/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionWeakReferenceVector(
  const TypeDefinitionWeakReferenceVector *typeDefinitionWeakReferenceVector, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionWeakReferenceVector->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionWeakReferenceVector, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionWeakReferenceVector(
  const TypeDefinitionWeakReferenceVector *typeDefinitionWeakReferenceVector, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionWeakReferenceVector->axiomatic())
  {
    // Vectors are still implemented as variable arrays in the DM.
    return InitializeTypeDefinitionVariableArray(typeDefinitionWeakReferenceVector, metaDictionary);
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionStream/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionStream(
  const TypeDefinitionStream *typeDefinitionStream, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionStream->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionStream, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionStream(
  const TypeDefinitionStream *typeDefinitionStream, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefStream *pType = NULL;
  ImplAAFTypeDef *pElementType = NULL;
  AAFRESULT result;
  
  if (typeDefinitionStream->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefStream *>
            (metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStream->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pElementType = metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionStream->elementTypeId());
    assert (pElementType);
    if (!pElementType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionStream->id(),
                                  pElementType,
                                  typeDefinitionStream->name());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionIndirect/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionIndirect(
  const TypeDefinitionIndirect *typeDefinitionIndirect, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionIndirect->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionIndirect, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionIndirect(
  const TypeDefinitionIndirect *typeDefinitionIndirect, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefIndirect *pType = NULL;
  ImplAAFTypeDef *pTypeDefAUID = NULL;
  AAFRESULT result;
  
  if (typeDefinitionIndirect->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefIndirect *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionIndirect->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pTypeDefAUID = metaDictionary->findAxiomaticTypeDefinition(kAAFTypeID_AUID);
    assert (pTypeDefAUID);
    if (!pTypeDefAUID)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionIndirect->id(), 
                                  typeDefinitionIndirect->name(), 
                                  pTypeDefAUID, 
                                  metaDictionary->dataDictionary());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


//
// TypeDefinitionOpaque/ImplAAFTypeDef callbacks
//
bool CreateTypeDefinitionOpaque(
  const TypeDefinitionOpaque *typeDefinitionOpaque, 
  ImplAAFMetaDictionary *metaDictionary)
{
  if (typeDefinitionOpaque->axiomatic())
  {
    return CreateTypeDefinition(typeDefinitionOpaque, metaDictionary);
  }
  else
  {
    return false;
  }
}


bool InitializeTypeDefinitionOpaque(
  const TypeDefinitionOpaque *typeDefinitionOpaque, 
  ImplAAFMetaDictionary *metaDictionary)
{
  ImplAAFTypeDefOpaque *pType = NULL;
  ImplAAFTypeDef *pTypeDefAUID = NULL;
  AAFRESULT result;
  
  if (typeDefinitionOpaque->axiomatic())
  {
    pType = dynamic_cast<ImplAAFTypeDefOpaque *>(metaDictionary->findAxiomaticTypeDefinition(*typeDefinitionOpaque->id()));
    assert (pType);
    if (!pType)
      throw AAFRESULT_TYPE_NOT_FOUND;

    pTypeDefAUID = metaDictionary->findAxiomaticTypeDefinition(kAAFTypeID_AUID);
    assert (pTypeDefAUID);
    if (!pTypeDefAUID)
      throw AAFRESULT_TYPE_NOT_FOUND;

    result = pType->pvtInitialize(*typeDefinitionOpaque->id(), 
                                  typeDefinitionOpaque->name(), 
                                  pTypeDefAUID, 
                                  metaDictionary->dataDictionary());
    assert (AAFRESULT_SUCCEEDED(result));
    if (AAFRESULT_FAILED(result))
      throw result;

    return true;
  }
  else
  {
    return false;
  }
}


#endif // #if USE_AAFOBJECT_MODEL

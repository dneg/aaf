//@doc
//@class    AAFMetaDictionary | Implementation class for AAFMetaDictionary
#ifndef __ImplAAFMetaDictionary_h__
#define __ImplAAFMetaDictionary_h__

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

#include "OMDictionary.h"
#include "OMStrongRefSetProperty.h"

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMReferenceSet.h"
#include "OMIdentitySet.h"

// Forward declarations:
class ImplAAFMetaDefinition;
class ImplAAFMetaDefinition;
class ImplAAFTypeDefVariableArray;
class ImplAAFTypeDefFixedArray;
class ImplAAFTypeDefRecord;
class ImplAAFTypeDefRename;
class ImplAAFTypeDefString;
class ImplAAFTypeDefStrongObjRef;
class ImplAAFTypeDefWeakObjRef;
class ImplAAFTypeDefSet;

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFClassDef> ImplEnumAAFClassDefs;
typedef ImplAAFEnumerator<ImplAAFTypeDef> ImplEnumAAFTypeDefs;

class ImplAAFDictionary;

#include "ImplAAFClassDef.h"
#include "ImplAAFPropertyDef.h"
#include "ImplAAFTypeDef.h"

//
// Indicates the "mode" for creating and initializing any of the axiomatic and
// built-in class definitions.
//
typedef enum _aafObjectCreationMode_e
{
  // Objects need to be created and all properties fully initialized, including all
  // object references.
  kAAFCreatingNewObjects,

  // Objects only need to be created, no properties need to be (or should be)
  // initialized. Only runtime data such as known record offsets need to be
  // initialized.
  kAAFRestoringOldObjects

} aafObjectCreationMode_e;


class ImplAAFMetaDictionary :
  public ImplAAFRoot,
  public OMDictionary
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFMetaDictionary ();

protected:
  virtual ~ImplAAFMetaDictionary ();

public:

  //
  // Factory method to create an instance of ImplAAFMetaDictionary
  //
  static ImplAAFMetaDictionary *CreateMetaDictionary(void);


  //
  // Create an instance of the appropriate derived class, given the
  // class id.  Initializes the OM properties.
  // 
  // This method implements the OMClassFactory interface.
  //
  OMStorable* create(const OMClassId& classId) const;

  //
  // This method implements the required OMStorable interface method
  //
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;




  //
  // Class and type access methods ...


  //****************
  // CreateMetaInstance()
  //
  // Creates a single uninitialized AAF meta definition associated 
  // with a specified stored object id.
  virtual AAFRESULT STDMETHODCALLTYPE 
  CreateMetaInstance (
    // Stored Object ID of the meta object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in pAUID
    ImplAAFMetaDefinition ** ppMetaObject);


  //
  // Add class property and type access methods ...
  //


  //****************
  // LookupClassDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupClassDef
        (// @parm [in,ref] Class Unique ID
         aafUID_constref classId,

         // @parm [out,retval] Class Definition
         ImplAAFClassDef ** ppClassDef);

  //****************
  // CreateForwardClassReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateForwardClassReference 
        (// @parm [in] class identification
         aafUID_constref classId);

  //****************
  // IsForwardClassReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    HasForwardClassReference 
      (// @parm [in] class identification
       aafUID_constref classId,

       // @parm [out] kAAFTrue if the given class identification is a forward reference.
       aafBoolean_t *pResult);

  //****************
  // RegisterClassDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterClassDef
        (// @parm [in] Class Definition
         ImplAAFClassDef * pClassDef);


  //****************
  // GetClassDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetClassDefs
        // @parm [out,retval] Class Definition Enumeration
        (ImplEnumAAFClassDefs ** ppEnum);


  //****************
  // CountClassDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountClassDefs
        // @parm [out, retval] Total number of class definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterTypeDef
        (// @parm [in] Type Definition Object
         ImplAAFTypeDef * pTypeDef);

  //****************
  // LookupTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupTypeDef
        (// @parm [in,ref] Type Unique ID
         aafUID_constref typeID,

         // @parm [out,retval] Type Definition Object
         ImplAAFTypeDef ** ppTypeDef);


  //****************
  // GetTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefs
        // @parm [out,retval] Type Def Enumeration
        (ImplEnumAAFTypeDefs ** ppEnum);


  //****************
  // CountTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountTypeDefs
        // @parm [out, retval] Total number of type definition objects
        (aafUInt32 * pResult);

  //****************
  // RegisterOpaqueTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterOpaqueTypeDef
        (// @parm [in] Type Definition Object
         ImplAAFTypeDef * pOpaqueTypeDef);

  //****************
  // LookupOpaqueTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupOpaqueTypeDef
        (// @parm [in,ref] Type Unique ID
         aafUID_constref typeID,

         // @parm [out,retval] Type Definition Object
         ImplAAFTypeDef ** ppOpaqueTypeDef);

  //****************
  // GetOpaqueTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOpaqueTypeDefs
        // @parm [out,retval] Type Def Enumeration
        (ImplEnumAAFTypeDefs ** ppEnum);


  //****************
  // CountOpaqueTypeDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountOpaqueTypeDefs
        // @parm [out, retval] Total number of type definition objects
        (aafUInt32 * pResult);


public:
  //
  // Methods private to the SDK
  //

  // Private registration method to add the given class definiion
  // to the set. 
  AAFRESULT PvtRegisterClassDef (ImplAAFClassDef * pClassDef);


  // These are low-level OMSet tests for containment.
  bool containsClass(aafUID_constref classId);
  bool containsType(aafUID_constref typeId);
  bool containsForwardClassReference(aafUID_constref classId);

  // If the given classId fromt the set of forward references.
  void RemoveForwardClassReference(aafUID_constref classId);

  // Find the opaque type definition associated with the given type id.
  ImplAAFTypeDef * findOpaqueTypeDefinition(aafUID_constref typeId) const; // NOT REFERENCE COUNTED!


  // Add the given class definition to the set of axiomatic class definitions.
  void addAxiomaticClassDefinition(ImplAAFClassDef *pClassDef);

  // Add the given property definition to the set of axiomatic property definitions.
  void addAxiomaticPropertyDefinition(ImplAAFPropertyDef *pPropertyDef);

  // Add the given type definition to the set of axiomatic type definitions.
  void addAxiomaticTypeDefinition(ImplAAFTypeDef *pTypeDef);

  // Find the aximatic class definition associated with the given class id.
  ImplAAFClassDef * findAxiomaticClassDefinition(aafUID_constref classId) const; // NOT REFERENCE COUNTED!

  // Find the aximatic property definition associated with the given property id.
  ImplAAFPropertyDef * findAxiomaticPropertyDefinition(aafUID_constref propertyId) const; // NOT REFERENCE COUNTED!

  // Find the aximatic type definition associated with the given type id.
  ImplAAFTypeDef * findAxiomaticTypeDefinition(aafUID_constref typeId) const; // NOT REFERENCE COUNTED!

  // Factory function to create an unitialized meta defintion for the 
  // given auid.
  ImplAAFMetaDefinition * pvtCreateMetaDefinition(const aafUID_t & auid);

  // Initialize only the non-persistent parts of the meta dictionary
  AAFRESULT Initialize(void);

  // Create and initialize all of the axiomatic definitions.
  AAFRESULT InstantiateAxiomaticDefinitions(void);

  // Create all of the axiomatic classes as uninitialized objects.
  void CreateAxiomaticClasses(void); // throw AAFRESULT

  // Create all of the axiomatic properties as uninitialized objects.
  void CreateAxiomaticProperties(void); // throw AAFRESULT

  // Create all of the axiomatic types as uninitialized objects.
  void CreateAxiomaticTypes(void); // throw AAFRESULT

  // Initialize all of the axiomatic classes with their parent and class
  // definitions.
  void InitializeAxiomaticClasses(void); // throw AAFRESULT

  // Initialize all of the property definitions with their type definitions.
  void InitializeAxiomaticProperties(void); // throw AAFRESULT

  // Initialize all of the type definitions with there appropriate class and type
  // definitions.
  void InitializeAxiomaticTypes(void); // throw AAFRESULT


  // Register all of the axiomatic properties with their corresponding
  //  axiomatic class definitions.
  void RegisterAxiomaticProperties(void); // throw AAFRESULT

  // Initialize all of the OMProperties for each aximatic definition.
  void InitializeAxiomaticOMDefinitions(void); // throw AAFRESULT
  
  // Complete the registration of the axiomatic class definitions
  // This must be called AFTER all other aximatic definitions have
  // been initialized and registered.
  void CompleteAxiomaticClassRegistration(void); // throw AAFRESULT

  // Create all of the axiomatic definitions.
  void CreateAxiomaticDefinitions(void); // throw AAFRESULT

  // Initialize all of the axiomatic definitions.
  void InitializeAxiomaticDefinitions(void); // throw AAFRESULT
  
  
  //
  // Methods that would be inherited or overriden from ImplAAFStrorable
  //
  
  // Associate OMClassDefinition and OMPropertyDefinitions with this object.
  virtual void InitializeOMStorable(ImplAAFClassDef * pClassDef);

protected:
  // Associate the existing OMProperties with corresponding property definitions from
  // the given class definition. NOTE: This call is recursive, it calls itself again
  // for the parent class of the given class until current class is a "root" class.
  virtual void InitOMProperties (ImplAAFClassDef * pClassDef);

private:

  //
  // Persistent data members.
  //
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFTypeDef> _typeDefinitions;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFClassDef> _classDefinitions;


  //
  // Non-persistent data members.
  //
  OMReferenceSet<OMUniqueObjectIdentification, ImplAAFTypeDef> _opaqueTypeDefinitions;
  OMReferenceSet<OMUniqueObjectIdentification, ImplAAFClassDef> _axiomaticClassDefinitions;
  OMReferenceSet<OMUniqueObjectIdentification, ImplAAFPropertyDef> _axiomaticPropertyDefinitions;
  OMReferenceSet<OMUniqueObjectIdentification, ImplAAFTypeDef> _axiomaticTypeDefinitions;
  OMIdentitySet<OMUniqueObjectIdentification> _forwardClassReferences;

  //
  // Temporary data members used while converting to the "two-roots" containment
  // model.
  //
private:
  ImplAAFDictionary *_dataDictionary;

public:
  void setDataDictionary(ImplAAFDictionary *dataDictionary);
  ImplAAFDictionary * dataDictionary(void) const;
};

#endif // #ifndef __ImplAAFMetaDictionary_h__

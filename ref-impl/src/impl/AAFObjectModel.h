//@doc
//@class    AAFObjectModel | Implementation class for AAFObjectModel
#ifndef __AAFObjectModel_h__
#define __AAFObjectModel_h__

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

#include "AAFTypes.h"

// Forward declarations:
class AAFObjectModel;
class Definition;
class PropertyDefinition;
class ClassDefinition;
class TypeDefinition;




// Type for callback for visitPreOrder and visitPostOrder methods. For visitPreOrder
// return false to skip visit of dependents. For visitPostOrder, returning false
// will stop all visiting any definitions.
typedef bool (*VisitDefinitionProcType)(
  const Definition *definition, void *userData);

// Type for callback to use the given definition to create corresponding objects for "userData".
typedef bool (*CreateDefinitionProcType)(
  const Definition *definition, void *userData);

// Type for callback to use the given definition to create corresponding objects from "userData".
typedef bool (*InitializeDefinitionProcType)(
  const Definition *definition, void *userData);


class AAFObjectModel
{
  // This is a singleton so we need to restrict access to the constructor
  // and destructor.
  // 
  AAFObjectModel ();

public:
  
  // Return the singleton.
  static const AAFObjectModel * singleton(void);

  aafUInt32 countClassDefinitions(void) const;
  // Access the class definitions in the order that they are 
  // initialized from MetaDictionary.h
  const ClassDefinition * classDefinitionAt (aafUInt32 index) const;

  aafUInt32 countPropertyDefinitions(void) const;
  // Access the property definitions in the order that they are 
  // initialized from MetaDictionary.h
  const PropertyDefinition * propertyDefinitionAt (aafUInt32 index) const;

  aafUInt32 countTypeDefinitions(void) const;
  // Access the type definitions in the order that they are 
  // initialized from MetaDictionary.h
  const TypeDefinition * typeDefinitionAt (aafUInt32 index) const;

  aafUInt32 countDefinitions(void) const;


  aafUInt32 countAxiomaticClassDefinitions(void) const;
  aafUInt32 countAxiomaticPropertyDefinitions(void) const;
  aafUInt32 countAxiomaticTypeDefinitions(void) const;

  aafUInt32 countAxiomaticDefinitions(void) const;


  //
  // MetaDictionary search routines:
  //
  const Definition * findDefinition(aafUID_constptr id) const;
  const ClassDefinition * findClassDefinition (aafUID_constptr id) const;
  const PropertyDefinition * findPropertyDefinition (aafUID_constptr id) const;
  const PropertyDefinition * findPropertyDefinition (aafUInt16 pid) const;
  const TypeDefinition * findTypeDefinition (aafUID_constptr id) const;

private:
  void SortClassDefinitions(void);
  void SortPropertyDefinitions(void);
  void SortTypeDefinitions(void);
  void SortDefinitions(void);
  void InitializeClassDefinitions(void);
  void InitializePrivateClassDefinitions(void);
  void InitializePropertyDefinitions(void);
  void InitializeTypeDefinitions(void);
  void InitializeAxiomaticDefinitions(void);
  void InitializeCyclicDefinitions(void);
  void InitializeDefinitions(void);

  friend class AAFObjectModelConstructor; //
};


//
// Abstract base class for all of the meta classes.
//
class Definition
{
public:
  Definition(aafCharacter_constptr name, 
             aafUID_constptr id,
             bool concrete) :
    _name(name),
    _id(id),
    _concrete(concrete),
    _axiomatic(false),
    _cyclic(false),
    _containsCycles(false),
    _createDefinitionProc(0),
    _initializeDefinitionProc(0)
  {}

  aafUID_constptr id(void) const { return _id; }
  aafCharacter_constptr name(void) const { return _name; }
  bool concrete (void) const { return _concrete; }
  bool axiomatic (void) const { return _axiomatic; }
  bool cyclic (void) const { return _cyclic; }
  bool containsCycles (void) const { return _containsCycles; }

  virtual void makeAxiomatic (void) const;

  const AAFObjectModel *objectModel() const;

  virtual void Initialize (void) = 0; // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const = 0;
  virtual bool isNil(void) const = 0;

  // Install the create proc.
  void setCreateDefinitionProc(CreateDefinitionProcType f) { _createDefinitionProc = f; }

  // Install the initialization proc.
  void setInitializeDefinitionProc(InitializeDefinitionProcType f) { _initializeDefinitionProc = f; }

  // Call the create proc with the given userData and return the result.
  bool createDefinition(void *userData) const;

  // Call the initialize proc with the given userData and return the result.
  bool initializeDefinition(void *userData) const;

  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

protected:
  void setAxiomatic (bool axiomatic) { _axiomatic = axiomatic; }
  void setCyclic (bool cyclic) { _cyclic = cyclic; }
  void setContainsCycles (bool containsCycles) { _containsCycles = containsCycles; }

  friend class AAFObjectModel;

private:
  aafCharacter_constptr _name;
  aafUID_constptr _id;
  bool _concrete; // 
  bool _axiomatic; //
  bool _cyclic;
  bool _containsCycles;

  CreateDefinitionProcType _createDefinitionProc;
  InitializeDefinitionProcType _initializeDefinitionProc;
};


//
// class for all of the meta class definitions.
//
class ClassDefinition : public Definition
{
public:
  ClassDefinition(aafCharacter_constptr name, 
                  aafUID_constptr id, 
                  aafUID_constptr parentId, 
                  bool concrete,
                  aafUInt32 propertyCount,
                  PropertyDefinition ** propertyDefinitions) :
    Definition (name, id, concrete),
    _privateClass(false),
    _parentId(parentId),
    _parentClass(0),
    _propertyCount(propertyCount),
    _propertyDefinitions(propertyDefinitions)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual void makeAxiomatic (void) const; // override
  void makePrivateClass(void) const;

  bool privateClass(void) const { return _privateClass; }
  aafUID_constptr parentId(void) const { return _parentId; }
  const ClassDefinition * parentClass (void) const { return _parentClass; }

  aafUInt32 propertyCount(void) const { return _propertyCount; }
  // Access the property definitions in the order that they are 
  // initialized from MetaDictionary.h for this class definition.
  const PropertyDefinition * propertyDefinitionAt (aafUInt32 index) const;
  const PropertyDefinition * findPropertyDefinition (aafUID_constptr id) const;
  const PropertyDefinition * findPropertyDefinition (aafUInt16 pid) const;

  static const ClassDefinition * null(void); // return sentinel for this class.
  virtual bool isNil() const;

  bool isRoot() const; // Is this the root object (InterchangeObject (definition?))

  // Return true if this is the class definition for a uniquely identified object.
  bool isUniquelyIdentified(void) const;

  // Return the unique identifier property for this class definition (if any).
  // Return NULL if this class 
  const PropertyDefinition *uniqueIdentifierProperty(void) const;


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  // NOTE: This method does NOT visit parent the parent class definition.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  // NOTE: This method does NOT visit parent the parent class definition.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;


private:
  void makePropertiesAxiomatic(void) const;
  void setPrivateClass (bool privateClass) { _privateClass = privateClass; }

private:
  bool _privateClass; // true if this class if private to the reference implementation
  aafUID_constptr _parentId;
  const ClassDefinition *_parentClass;

  // The property definitions for this class definition.
  aafUInt32 _propertyCount;
  PropertyDefinition ** _propertyDefinitions;
};


//
// class for all of the meta property definitions.
//
class PropertyDefinition : public Definition
{
public:
  PropertyDefinition(aafCharacter_constptr name,
                     aafUID_constptr id, 
                     aafUInt16 pid, 
                     aafUID_constptr typeId, 
                     bool required, 
                     bool uid,
                     aafUID_constptr classId, 
                     bool concrete) :
    Definition (name, id, concrete),
    _pid(pid),
    _typeId(typeId),
    _required(required),
    _uid(uid),
    _classId(classId),
    _typeDefinition(0),
    _container(0)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual void makeAxiomatic (void) const; // override

  aafUInt16 pid(void) const { return _pid; }
  aafUID_constptr typeId(void) const { return _typeId; }
  bool required(void) const { return _required; }
  bool uid(void) const { return _uid; }
  aafUID_constptr classId(void) const { return _classId; }

  const TypeDefinition * typeDefinition (void) const { return _typeDefinition; }
  const ClassDefinition * container (void) const { return _container; }

  static const PropertyDefinition * null(void); // return sentinel for this class.
  virtual bool isNil() const;


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUInt16 _pid;
  aafUID_constptr _typeId;
  bool _required;
  bool _uid;
  aafUID_constptr _classId;
  const TypeDefinition *_typeDefinition;
  const ClassDefinition *_container;
};



//
// class for all of the meta type definitions.
//
class TypeDefinition : public Definition
{
public:
  TypeDefinition(aafCharacter_constptr name, 
                 aafUID_constptr id,
                 bool concrete) :
    Definition (name, id, concrete)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatUnknown; }

  static const TypeDefinition * null(void); // return sentinel for this class.
  virtual bool isNil(void) const;
};

//
// class for all of the meta type definitions.
//
class TypeDefinitionInteger : public TypeDefinition
{
public:
  TypeDefinitionInteger(aafCharacter_constptr name, 
                        aafUID_constptr id,
                        aafUInt8 size,
                        bool signedNumber) :
    TypeDefinition (name, id, true /*always concrete*/),
    _size(size),
    _signedNumber(signedNumber)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatInt; }

  aafUInt8 size(void) const { return _size; }
  bool signedNumber(void) const { return _signedNumber; }

private:
  aafUInt8 _size;
  bool _signedNumber;
};

//
// class for all of the meta type definitions.
//
class DefinitionEnumerationMember;

class TypeDefinitionEnumeration : public TypeDefinition
{
public:
  TypeDefinitionEnumeration(aafCharacter_constptr name, 
                        aafUID_constptr id,
                        aafUID_constptr typeId,
                        aafUInt32 memberCount,
                        DefinitionEnumerationMember **members) :
    TypeDefinition (name, id, true /*always concrete*/),
    _typeId(typeId),
    _typeDefinition(0),
    _memberCount(memberCount),
    _members(members)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatEnum; }
  virtual void makeAxiomatic (void) const;

  aafUID_constptr typeId(void) const { return _typeId; }
  const TypeDefinitionInteger * typeDefinition(void) const { return _typeDefinition; }
  aafUInt32 memberCount(void) const { return _memberCount; }
  const DefinitionEnumerationMember *memberAt(aafUInt32 index) const;


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUID_constptr _typeId; // must be one of the integer types
  const TypeDefinitionInteger *_typeDefinition; // used to determine the size of an enum value.
  aafUInt32 _memberCount;
  DefinitionEnumerationMember **_members;
};

//
// TypeDefinitionEnumeration Member : objects that represent the elements
// in the enumeration set.
//
class DefinitionEnumerationMember
{
public:
  DefinitionEnumerationMember(aafCharacter_constptr name,
         aafInt64 value,
         aafUID_constptr containerId) :
    _name(name),
    _value(value),
    _containerId(containerId)
  {}

  virtual void Initialize (void); // only the member data of this object

  aafCharacter_constptr name(void) const { return _name; }
  const aafInt64& value(void) const { return _value; }
  aafUID_constptr containerId(void) const { return _containerId; }
  const TypeDefinitionEnumeration * container(void) const;

private:
  aafCharacter_constptr _name;
  aafInt64 _value;
  aafUID_constptr _containerId; // must be an enumeration type.
};


//
// class for all of the meta array type definitions.
//
class TypeDefinitionVariableArray : public TypeDefinition
{
public:
  TypeDefinitionVariableArray(aafCharacter_constptr name, 
                        aafUID_constptr id,
                        aafUID_constptr elementTypeId) :
    TypeDefinition (name, id, true /*always concrete*/),
    _elementTypeId(elementTypeId),
    _elementType(0)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatVariableArray; }
  virtual void makeAxiomatic (void) const;

  aafUID_constptr elementTypeId(void) const { return _elementTypeId; }
  const TypeDefinition * elementType(void) const { return _elementType; }


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUID_constptr _elementTypeId; // used to determine the composition of an element
  const TypeDefinition *_elementType; // 
};

//
// class for all of the fixed array type definitions.
//
class TypeDefinitionFixedArray : public TypeDefinitionVariableArray
{
public:
  TypeDefinitionFixedArray(aafCharacter_constptr name, 
                        aafUID_constptr id,
                        aafUID_constptr elementTypeId,
                        aafUInt32 elementCount) :
    TypeDefinitionVariableArray (name, id, elementTypeId),
    _elementCount(elementCount)
  {}

 virtual const ClassDefinition *classDefinition(void) const;
 virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatFixedArray; }

 aafUInt32 elementCount(void) const { return _elementCount; }

private:
  aafUInt32 _elementCount; // the number of elements in the fixed array.
};

//
// class for all of the meta type definitions.
//
class DefinitionRecordField;
class TypeDefinitionRecord : public TypeDefinition
{
public:
  TypeDefinitionRecord(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUInt32 recordSize,
                       aafUInt32 fieldCount,
                       DefinitionRecordField ** fields) :
    TypeDefinition (name, id, true /*always concrete*/),
    _recordSize(recordSize),
    _fieldCount(fieldCount),
    _fields(fields)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatRecord; }
  virtual void makeAxiomatic (void) const;

  aafUInt32 recordSize(void) const { return _recordSize; }
  aafUInt32 fieldCount(void) const { return _fieldCount; }
  const DefinitionRecordField * fieldAt(aafUInt32 index) const;


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUInt32 _recordSize;
  aafUInt32 _fieldCount;
  DefinitionRecordField **_fields;
};

//
// Class for the fields for a record definition.
//
class DefinitionRecordField
{
public:
  DefinitionRecordField(aafCharacter_constptr name,
         aafUID_constptr typeId,
         aafUID_constptr containerId,
         aafUInt32 fieldOffset) :
    _name(name),
    _typeId(typeId),
    _containerId(containerId),
    _fieldOffset(fieldOffset),
    _typeDefinition(0)
  {}

  virtual void Initialize (void); // only the member data of this object

  aafCharacter_constptr name(void) const { return _name; }
  aafUID_constptr typeId(void) const { return _typeId; }
  aafUID_constptr containerId(void) const { return _containerId; }
  aafUInt32 fieldOffset(void) const { return _fieldOffset; }
  const TypeDefinition *typeDefinition(void) const { return _typeDefinition; }
  const TypeDefinitionRecord * container(void) const;

private:
  aafCharacter_constptr _name;
  aafUID_constptr _typeId; // can be any "fixed lengh" type.
  aafUID_constptr _containerId; // must be an id for a TypeDefinitionRecord
  aafUInt32 _fieldOffset; // the "offsetof" this field within the corresponding record (struct)
  const TypeDefinition *_typeDefinition;
};


//
// class for all of the renamed type definitions.
//
class TypeDefinitionRename : public TypeDefinition
{
public:
  TypeDefinitionRename(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr renamedTypeId) :
    TypeDefinition (name, id, true /*always concrete*/),
    _renamedTypeId(renamedTypeId),
    _renamedType(0)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatRename; }
  virtual void makeAxiomatic (void) const;

  aafUID_constptr renamedTypeId(void) const { return _renamedTypeId; }
  const TypeDefinition * renamedType(void) const { return _renamedType; }


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUID_constptr _renamedTypeId; // 
  const TypeDefinition *_renamedType; // 
};

//
// class for all of the character type definitions.
//
class TypeDefinitionCharacter : public TypeDefinition
{
public:
  TypeDefinitionCharacter(aafCharacter_constptr name, 
                          aafUID_constptr id) :
    TypeDefinition (name, id, true /*always concrete*/)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatCharacter; }
};

//
// class for all of the meta type definitions.
//
class TypeDefinitionString : public TypeDefinition
{
public:
  TypeDefinitionString(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr stringTypeId) :
    TypeDefinition (name, id, true /*always concrete*/),
    _stringTypeId(stringTypeId),
    _stringType(0)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatString; }
  virtual void makeAxiomatic (void) const;

  aafUID_constptr stringTypeId(void) const { return _stringTypeId; }
  const TypeDefinitionCharacter * stringType(void) const { return _stringType; }


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUID_constptr _stringTypeId; // 
  const TypeDefinitionCharacter *_stringType; // 
};

//
// class for all of the meta extendible enumeration definitions.
//
class DefinitionExtendibleEnumerationMember;

class TypeDefinitionExtendibleEnumeration : public TypeDefinition
{
public:
  TypeDefinitionExtendibleEnumeration(aafCharacter_constptr name,
                        aafUID_constptr id,
                        aafUInt32 memberCount,
                        DefinitionExtendibleEnumerationMember **members) :
    TypeDefinition(name, id, true /*always concrete*/),
    _memberCount(memberCount),
    _members(members)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatExtEnum; }

  aafUInt32 memberCount(void) const { return _memberCount; }
  const DefinitionExtendibleEnumerationMember *memberAt(aafUInt32 index) const;

private:
  aafUInt32 _memberCount;
  DefinitionExtendibleEnumerationMember **_members;
};


//
// DefinitionExtendibleEnumerationMember : objects that represent the elements
// in an extendible enumeration list.
//
class DefinitionExtendibleEnumerationMember
{
public:
  DefinitionExtendibleEnumerationMember(aafCharacter_constptr name,
         aafUID_constptr auid,
         aafUID_constptr containerId) :
    _name(name),
    _auid(auid),
    _containerId(containerId)
  {}

  virtual void Initialize (void); // only the member data of this object

  aafCharacter_constptr name(void) const { return _name; }
  aafUID_constptr auid(void) const { return _auid; }
  aafUID_constptr containerId(void) const { return _containerId; }
  const TypeDefinitionExtendibleEnumeration * container(void) const;


private:
  aafCharacter_constptr _name;
  aafUID_constptr _auid;
  aafUID_constptr _containerId; // must be an extendible enumeration type.
};




//
// class for all of the set type definitions.
//
class TypeDefinitionSet : public TypeDefinition
{
protected:
  TypeDefinitionSet(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr elementTypeId,
                       bool concrete) :
    TypeDefinition (name, id, concrete),
    _elementTypeId(elementTypeId),
    _elementType(0)
  {}

public:
  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatSet; }
  virtual void makeAxiomatic (void) const;

  aafUID_constptr elementTypeId(void) const { return _elementTypeId; }
  const TypeDefinition * elementType(void) const { return _elementType; }


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUID_constptr _elementTypeId; // 
  const TypeDefinition *_elementType; // 
};

//
// class for all of the strong reference type definitions.
//
class TypeDefinitionObjectReference : public TypeDefinition
{
protected:
  TypeDefinitionObjectReference(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr targetId,
                       bool concrete) :
    TypeDefinition (name, id, concrete),
    _targetId(targetId),
    _target(0)
  {}

public:
  virtual void Initialize (void); // only the member data of this object
  virtual void makeAxiomatic (void) const;

  aafUID_constptr targetId(void) const { return _targetId; }
  const ClassDefinition * target(void) const { return _target; }


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUID_constptr _targetId; // 
  const ClassDefinition *_target; // 
};


//
// class for all of the strong reference type definitions.
//
class TypeDefinitionStrongReference : public TypeDefinitionObjectReference
{
public:
  TypeDefinitionStrongReference(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr targetId) :
    TypeDefinitionObjectReference (name, id, targetId, true /*always concrete*/)
  {}

  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatStrongObjRef; }
};

//
// class for all of strong reference sets
//
class TypeDefinitionStrongReferenceSet : public TypeDefinitionSet
{
public:
  TypeDefinitionStrongReferenceSet(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr elementTypeId) :
    TypeDefinitionSet (name, id, elementTypeId, true /*always concrete*/)
  {}


  virtual const ClassDefinition *classDefinition(void) const;
};

//
// class for all of strong reference vectors
//
class TypeDefinitionStrongReferenceVector : public TypeDefinitionVariableArray
{
public:
  TypeDefinitionStrongReferenceVector(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr elementTypeId) :
    TypeDefinitionVariableArray (name, id, elementTypeId)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
};

//
// class for all of the weak reference type definitions.
//
class TypeDefinitionWeakReference : public TypeDefinitionObjectReference
{
public:
  TypeDefinitionWeakReference(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr targetId,
                       aafUInt32 targetSetCount,
                       aafUID_constptr * targetSet) :
    TypeDefinitionObjectReference (name, id, targetId, true /*always concrete*/),
    _targetSetCount(targetSetCount),
    _targetSet(targetSet)
  {}

  virtual void Initialize (void);
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatWeakObjRef; }

  virtual void makeAxiomatic (void) const;

  aafUInt32 targetSetCount(void) const { return _targetSetCount; }
  aafUID_constptr targetAt(aafUInt32 index) const;
  
  
private:
  aafUInt32 _targetSetCount;
  aafUID_constptr *_targetSet;
};




//
// class for all of weak reference sets
//
class TypeDefinitionWeakReferenceSet : public TypeDefinitionSet
{
public:
  TypeDefinitionWeakReferenceSet(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr elementTypeId) :
    TypeDefinitionSet (name, id, elementTypeId, true /*always concrete*/)
  {}


  virtual const ClassDefinition *classDefinition(void) const;
};

//
// class for all of weak reference vectors
//
class TypeDefinitionWeakReferenceVector : public TypeDefinitionVariableArray
{
public:
  TypeDefinitionWeakReferenceVector(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr elementTypeId) :
    TypeDefinitionVariableArray (name, id, elementTypeId)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
};

//
// class for all of the stream types
//
class TypeDefinitionStream : public TypeDefinition
{
public:
  TypeDefinitionStream(aafCharacter_constptr name, 
                       aafUID_constptr id,
                       aafUID_constptr elementTypeId = 0) :
    TypeDefinition (name, id, true /*always concrete*/),
    _elementTypeId(elementTypeId),
    _elementType(0)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatStream; }
  virtual void makeAxiomatic (void) const;

  aafUID_constptr elementTypeId(void) const { return _elementTypeId; }
  const TypeDefinition * elementType(void) const { return _elementType; }


  // Walk the tree of definitions pre-order, call the given function for 
  // the current definition before any dependent definitions.
  virtual bool visitPreOrder(VisitDefinitionProcType f, void *userData) const;

  // Walk the tree of definitions post-order, call the given function for 
  // the current definition after any dependent definitions.
  virtual bool visitPostOrder(VisitDefinitionProcType f, void *userData) const;

private:
  aafUID_constptr _elementTypeId; // 
  const TypeDefinition *_elementType; // 
};

//
// class for all of the indirect types
//
class TypeDefinitionIndirect : public TypeDefinition
{
public:
  TypeDefinitionIndirect(aafCharacter_constptr name, 
                        aafUID_constptr id) :
    TypeDefinition (name, id, true /*always concrete*/)
  {}

  virtual void Initialize (void); // only the member data of this object
  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatIndirect; }
};

//
// class for all of the opaque types
//
class TypeDefinitionOpaque : public TypeDefinitionIndirect
{
public:
  TypeDefinitionOpaque(aafCharacter_constptr name, 
                        aafUID_constptr id) :
    TypeDefinitionIndirect (name, id)
  {}

  virtual const ClassDefinition *classDefinition(void) const;
  virtual eAAFTypeCategory_e category(void) const { return kAAFTypeCatOpaque; }
};




#endif // ! __AAFObjectModel_h__

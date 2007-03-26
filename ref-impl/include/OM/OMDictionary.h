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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef OMDICTIONARY_H
#define OMDICTIONARY_H

#include "OMClassFactory.h"
#include "OMStorable.h"
#include "OMRedBlackTree.h"
#include "OMDataTypes.h"
#include "OMType.h"
#include "OMVector.h"


// Abstract meta classes

// {0101010D-0224-0000-060E-2B3402060101}
static const OMObjectIdentification Class_Definition =
{0x0D010101, 0x0224, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0203-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinition =
{0x0D010101, 0x0203, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};

// Concrete meta classes

// {0101010D-0225-0000-060E-2B3402060101}
static const OMObjectIdentification Class_MetaDictionary =
{0x0D010101, 0x0225, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0201-0000-060E-2B3402060101}
static const OMObjectIdentification Class_ClassDefinition =
{0x0D010101, 0x0201, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0202-0000-060E-2B3402060101}
static const OMObjectIdentification Class_PropertyDefinition =
{0x0D010101, 0x0202, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};

// {0101010D-0205-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionStrongObjectReference =
{0x0D010101, 0x0205, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0206-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionWeakObjectReference =
{0x0D010101, 0x0206, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};

// {0101010D-020A-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionSet =
{0x0D010101, 0x020A, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0208-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionFixedArray =
{0x0D010101, 0x0208, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0209-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionVaryingArray =
{0x0D010101, 0x0209, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-020D-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionRecord =
{0x0D010101, 0x020D, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};

// {0101010D-020C-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionStream =
{0x0D010101, 0x020C, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};

// {0101010D-020E-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionRename =
{0x0D010101, 0x020E, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0221-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionIndirect =
{0x0D010101, 0x0221, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0222-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionOpaque =
{0x0D010101, 0x0222, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};

// {0101010D-0223-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionCharacter =
{0x0D010101, 0x0223, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-020B-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionString =
{0x0D010101, 0x020B, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};

// {0101010D-0204-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionInteger =
{0x0D010101, 0x0204, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0207-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionEnumerated =
{0x0D010101, 0x0207, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};
// {0101010D-0220-0000-060E-2B3402060101}
static const OMObjectIdentification Class_TypeDefinitionExtendibleEnumerated =
{0x0D010101, 0x0220, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}};

// Type definitions

// {01010100-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_UInt8 =
{0x01010100, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};
// {01010200-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_UInt16 =
{0x01010200, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};
// {01010300-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_UInt32 =
{0x01010300, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};
// {01010400-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_UInt64 =
{0x01010400, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};
// {01010500-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_Int8 =
{0x01010500, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};
// {01010600-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_Int16 =
{0x01010600, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};
// {01010700-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_Int32 =
{0x01010700, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};
// {01010800-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_Int64 =
{0x01010800, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};
// {01030100-0000-0000-060E-2B3401040101}
static const OMObjectIdentification Type_UniqueObjectIdentification =
{0x01030100, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01}};

#if 1 // HACK4MEIP2
// tjb - the OM shouldn't know about this AAF specific type
static const OMObjectIdentification Type_TransferCharacteristic =
{0x02020102, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
#endif

// Special classes

// {B3B398A5-1C90-11d4-8053-080036210804}
static const OMObjectIdentification Class_Root =
{0xb3b398a5, 0x1c90, 0x11d4, {0x80, 0x53, 0x08, 0x00, 0x36, 0x21, 0x08, 0x04}};

static const OMPropertyId PID_Root_MetaDictionary  = 0x001;
static const OMPropertyId PID_Root_Header          = 0x002;

class OMPropertyDefinition;

class OMReferenceContainerIterator;
typedef OMReferenceContainerIterator ClassDefinitionsIterator;
typedef OMReferenceContainerIterator TypeDefinitionsIterator;

  // @class Dictionary containing definitions of classes, properties and types.
  //        The dictionary is a factory for <c OMstorable> objects.
  //        The dictionary is persistent.
  //   @base public | <c OMClassFactory>
  //   @base public | <c OMStorable>
  //   @cauthor Tim Bingham | Avid Technology, Inc.
class OMDictionary : public OMClassFactory, public OMStorable {
public:

  static OMPropertyDefinition* find(const OMPropertyId propertyId);

  static void insert(const OMPropertyId propertyId,
                     const OMPropertyDefinition* definition);

  static OMPropertyDefinition* remove(const OMPropertyId propertyId);

  static bool contains(const OMPropertyId propertyId);

  virtual void associate(const OMObjectIdentification& id,
                         const OMPropertyId propertyId) = 0;

  virtual void newClass(const OMUniqueObjectIdentification& id,
                        const wchar_t* name,
                        const wchar_t* description,
                        const OMUniqueObjectIdentification& parent,
                        bool isConcrete);

  virtual const OMPropertyDefinition* newProperty(const OMUniqueObjectIdentification& id,
                           const wchar_t* name,
                           const wchar_t* description,
                           OMPropertyId localId,
                           const OMUniqueObjectIdentification& type,
                           bool& isOptional,
                           bool& isUniqueIdentifier,
                           const OMUniqueObjectIdentification& memberOf);

  virtual void newIntegerType(const OMObjectIdentification& id,
                              const wchar_t* name,
                              const wchar_t* description,
                              OMUInt8 size,
                              bool isSigned);

  virtual void newCharacterType(const OMObjectIdentification& id,
                                const wchar_t* name,
                                const wchar_t* description);

  virtual void newStrongReferenceType(
                                 const OMObjectIdentification& id,
                                 const wchar_t* name,
                                 const wchar_t* description,
                                 const OMObjectIdentification& referencedType);

  virtual void newWeakReferenceType(
                                 const OMObjectIdentification& id,
                                 const wchar_t* name,
                                 const wchar_t* description,
                                 const OMObjectIdentification& referencedType,
                                 const OMObjectIdentification* path,
                                 OMUInt32 pathCount);

  virtual void newRenamedType(const OMObjectIdentification& id,
                              const wchar_t* name,
                              const wchar_t* description,
                              const OMObjectIdentification& renamedType);

  virtual void newEnumeratedType(const OMObjectIdentification& id,
                                 const wchar_t* name,
                                 const wchar_t* description,
                                 const OMObjectIdentification& elementType,
                                 const wchar_t** elementNames,
                                 OMInt64* elementValues,
                                 OMUInt32 elementCount);

  virtual void newFixedArrayType(const OMObjectIdentification& id,
                                 const wchar_t* name,
                                 const wchar_t* description,
                                 const OMObjectIdentification& elementType,
                                 OMUInt32 elementCount);

  virtual void newVaryingArrayType(const OMObjectIdentification& id,
                                   const wchar_t* name,
                                   const wchar_t* description,
                                   const OMObjectIdentification& elementType);

  virtual void newSetType(const OMObjectIdentification& id,
                          const wchar_t* name,
                          const wchar_t* description,
                          const OMObjectIdentification& elementType);

  virtual void newRecordType(const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description,
                             const OMObjectIdentification* memberTypes,
                             const wchar_t** memberNames,
                             OMUInt32 memberCount);

  virtual void newStreamType(const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description);

  virtual void newStringType(const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description,
                             const OMObjectIdentification& elementType);

  virtual void newExtendibleEnumeratedType(
                                 const OMObjectIdentification& id,
                                 const wchar_t* name,
                                 const wchar_t* description);

  virtual void newExtendibleEnumeratedTypeElement(
                                 const OMObjectIdentification& elementOf,
                                 const wchar_t* name,
                                 const OMObjectIdentification& value);

  virtual void newIndirectType(const OMObjectIdentification& id,
                               const wchar_t* name,
                               const wchar_t* description);

  virtual void newOpaqueType(const OMObjectIdentification& id,
                             const wchar_t* name,
                             const wchar_t* description);

  virtual void resolve(void);

  static void initialize(void);

  static void finalize(void);

  bool isAbstractMeta(const OMObjectIdentification& id) const;

  bool isConcreteMeta(const OMObjectIdentification& id) const;

  bool isMeta(const OMObjectIdentification& id) const;

  virtual ClassDefinitionsIterator* classDefinitions(void) const = 0;
  
  virtual void typeDefinitions(OMVector<OMType*>& typeDefs) const = 0;
//  virtual TypeDefinitionsIterator* typeDefinitions(void) const = 0;

  static void mapFromKLV(OMPropertyId& id);

  static void mapToKLV(OMPropertyId& id);

  static void mapFromKLV(OMObjectIdentification& id);

  static void mapToKLV(OMObjectIdentification & id);

  virtual bool registerClassDef(const OMUniqueObjectIdentification& classId) = 0;

  virtual bool registerTypeDef(const OMUniqueObjectIdentification& typeId) = 0;
  
private:

  typedef OMRedBlackTree<OMPropertyId,
                         OMPropertyDefinition*> PropertyDefinitionSet;

  static PropertyDefinitionSet* _propertyDefinitions;

};

#endif


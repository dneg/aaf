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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMDictionary

#include "OMDictionary.h"
#include "OMPropertyDefinition.h"

OMDictionary::PropertyDefinitionSet* OMDictionary::_propertyDefinitions;

OMPropertyDefinition* OMDictionary::find(const OMPropertyId propertyId)
{
  TRACE("OMDictionary::find");

  PRECONDITION("Definitions initialized", _propertyDefinitions != 0);
  PRECONDITION("Valid property id", propertyId != 0);

  OMPropertyDefinition* result = 0;
#if defined(OM_DEBUG)
  bool status = 
#endif
  _propertyDefinitions->find(propertyId, result);

  POSTCONDITION("Property definition found", status);
  POSTCONDITION("Valid result", result != 0);
  return result;
}

void OMDictionary::insert(const OMPropertyId propertyId,
                          const OMPropertyDefinition* definition)
{
  TRACE("OMDictionary::insert");

  PRECONDITION("Definitions initialized", _propertyDefinitions != 0);
  PRECONDITION("Valid property id", propertyId != 0);
  PRECONDITION("Valid property definition", definition != 0);
  PRECONDITION("Definition not already present",
                                  !_propertyDefinitions->contains(propertyId));

#if defined(OM_DEBUG)
  bool status =
#endif
  _propertyDefinitions->insert(
                                propertyId,
                                const_cast<OMPropertyDefinition*>(definition));

  POSTCONDITION("Definition not previously present", status);
  POSTCONDITION("Definition present",
                                   _propertyDefinitions->contains(propertyId));
}

OMPropertyDefinition* OMDictionary::remove(const OMPropertyId propertyId)
{
  TRACE("OMDictionary::remove");

  PRECONDITION("Definitions initialized", _propertyDefinitions != 0);
  PRECONDITION("Valid property id", propertyId != 0);
  PRECONDITION("Definition present",
                                   _propertyDefinitions->contains(propertyId));

  OMPropertyDefinition* result = 0;
  bool status = _propertyDefinitions->find(propertyId, result);
  ASSERT("Property definition found", status);

  status = _propertyDefinitions->remove(propertyId);
  ASSERT("Property definition removed", status);

  POSTCONDITION("Definition no longer present",
                                  !_propertyDefinitions->contains(propertyId));
  return result;
}

bool OMDictionary::contains(const OMPropertyId propertyId)
{
  TRACE("OMDictionary::contains");

  PRECONDITION("Definitions initialized", _propertyDefinitions != 0);
  PRECONDITION("Valid property id", propertyId != 0);

  OMPropertyDefinition* d = 0;
  bool result = _propertyDefinitions->find(propertyId, d);

  return result;
}

void OMDictionary::newClass(const OMUniqueObjectIdentification& /* id */,
                            const wchar_t* /* name */,
                            const wchar_t* /* description */,
                            const OMUniqueObjectIdentification& /* parent */,
                            bool /* isConcrete */)
{
  TRACE("OMDictionary::newClass");

  ASSERT("Unimplemented code not reached.", false);
}

const OMPropertyDefinition* OMDictionary::newProperty(const OMUniqueObjectIdentification& /*id*/,
                       const wchar_t* /*name*/,
                       const wchar_t* /*description*/,
                       OMPropertyId /*localId*/,
                       const OMUniqueObjectIdentification& /*type*/,
                       bool& /*isOptional*/,
                       bool& /*isUniqueIdentifier*/,
                       const OMUniqueObjectIdentification& /*memberOf*/)
{
  TRACE("OMDictionary::newProperty");

  ASSERT("Unimplemented code not reached.", false);
  return 0;
}

void OMDictionary::newIntegerType(const OMObjectIdentification& /* id */,
                                  const wchar_t* /* name */,
                                  const wchar_t* /* description */,
                                  OMUInt8 /* size */,
                                  bool /* isSigned */)
{
  TRACE("OMDictionary::newIntegerType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newCharacterType(const OMObjectIdentification& /* id */,
                                    const wchar_t* /* name */,
                                    const wchar_t* /* description */)
{
  TRACE("OMDictionary::newCharacterType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newStrongReferenceType(
                            const OMObjectIdentification& /* id */,
                            const wchar_t* /* name */,
                            const wchar_t* /* description */,
                            const OMObjectIdentification& /* referencedType */)
{
  TRACE("OMDictionary::newStrongReferenceType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newWeakReferenceType(
                            const OMObjectIdentification& /* id */,
                            const wchar_t* /* name */,
                            const wchar_t* /* description */,
                            const OMObjectIdentification& /* referencedType */,
                            const OMObjectIdentification* /* path */,
                            OMUInt32 /* pathCount */)
{
  TRACE("OMDictionary::newWeakReferenceType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newRenamedType(
                               const OMObjectIdentification& /* id */,
                               const wchar_t* /* name */,
                               const wchar_t* /* description */,
                               const OMObjectIdentification& /* renamedType */)
{
  TRACE("OMDictionary::newRenamedType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newEnumeratedType(
                               const OMObjectIdentification& /* id */,
                               const wchar_t* /* name */,
                               const wchar_t* /* description */,
                               const OMObjectIdentification& /* elementType */,
                               const wchar_t** /* elementNames */,
                               OMInt64* /* elementValues */,
                               OMUInt32 /* elementCount */)
{
  TRACE("OMDictionary::newEnumeratedType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newFixedArrayType(
                               const OMObjectIdentification& /* id */,
                               const wchar_t* /* name */,
                               const wchar_t* /* description */,
                               const OMObjectIdentification& /* elementType */,
                               OMUInt32 /* elementCount */)
{
  TRACE("OMDictionary::newFixedArrayType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newVaryingArrayType(
                               const OMObjectIdentification& /* id */,
                               const wchar_t* /* name */,
                               const wchar_t* /* description */,
                               const OMObjectIdentification& /* elementType */)
{
  TRACE("OMDictionary::newVaryingArrayType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newSetType(const OMObjectIdentification& /* id */,
                              const wchar_t* /* name */,
                              const wchar_t* /* description */,
                              const OMObjectIdentification& /* elementType */)
{
  TRACE("OMDictionary::newSetType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newRecordType(
                               const OMObjectIdentification& /* id */,
                               const wchar_t* /* name */,
                               const wchar_t* /* description */,
                               const OMObjectIdentification* /* memberTypes */,
                               const wchar_t** /* memberNames */,
                               OMUInt32 /* memberCount */)
{
  TRACE("OMDictionary::newRecordType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newStreamType(const OMObjectIdentification& /* id */,
                             const wchar_t* /* name */,
                             const wchar_t* /* description */)
{
  TRACE("OMDictionary::newStreamType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newStringType(
                               const OMObjectIdentification& /* id */,
                               const wchar_t* /* name */,
                               const wchar_t* /* description */,
                               const OMObjectIdentification& /* elementType */)
{
  TRACE("OMDictionary::newStringType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newExtendibleEnumeratedType(
                             const OMObjectIdentification& /* id */,
                             const wchar_t* /* name */,
                             const wchar_t* /* description */)
{
  TRACE("OMDictionary::newExtendibleEnumeratedType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newExtendibleEnumeratedTypeElement(
                             const OMObjectIdentification& /* elementOf */,
                             const wchar_t* /* name */,
                             const OMObjectIdentification& /* value */)
{
  TRACE("OMDictionary::newExtendibleEnumeratedTypeElement");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newIndirectType(const OMObjectIdentification& /* id */,
                                   const wchar_t* /* name */,
                                   const wchar_t* /* description */)
{
  TRACE("OMDictionary::newIndirectType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::newOpaqueType(const OMObjectIdentification& /* id */,
                                 const wchar_t* /* name */,
                                 const wchar_t* /* description */)
{
  TRACE("OMDictionary::newOpaqueType");

  ASSERT("Unimplemented code not reached.", false);
}

void OMDictionary::resolve(void)
{
  TRACE("OMDictionary::resolve");

  ASSERT("Unimplemented code not reached.", false);
}

struct _properties_t {
  OMPropertyId _pid;
  const wchar_t* _name;
  const wchar_t* _description;

} _properties[] = {
  {0x0001, L"MetaDictionary", L"MetaDictionary"},
  {0x0002, L"Header",         L"Header"}
};

void OMDictionary::initialize(void)
{
  TRACE("OMDictionary::initialize");
  PRECONDITION("Definitions not initialized", _propertyDefinitions == 0);

  _propertyDefinitions = new PropertyDefinitionSet();
  ASSERT("Valid heap pointer", _propertyDefinitions != 0);

  for (size_t i = 0; i < sizeof(_properties)/sizeof(_properties[0]); i++) {
    OMPropertyDefinition* d =
      new OMBuiltinPropertyDefinition (0,
                                       _properties[i]._name,
                                       _properties[i]._description,
                                       _properties[i]._pid,
                                       false);
    ASSERT("Valid heap pointer", d != 0);
    insert(d->localIdentification(), d);
  }
}

void OMDictionary::finalize(void)
{
  TRACE("OMDictionary::finalize");

  PRECONDITION("Definitions initialized", _propertyDefinitions != 0);

  for (size_t i = 0; i < sizeof(_properties)/sizeof(_properties[0]); i++) {
    if (contains(_properties[i]._pid)) {
      OMPropertyDefinition* d = remove(_properties[i]._pid);
      delete d;
    }
  }
  delete _propertyDefinitions;
}

bool OMDictionary::isAbstractMeta(const OMObjectIdentification& id) const
{
  TRACE("OMDictionary::isAbstractMeta");
  bool result;
  if (id == Class_Definition) {
    result = true;
  } else if (id == Class_TypeDefinition) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool OMDictionary::isConcreteMeta(const OMObjectIdentification& id) const
{
  TRACE("OMDictionary::isConcreteMeta");
  bool result;
  if (id == Class_MetaDictionary) {
    result = true;
  } else if (id == Class_ClassDefinition) {
    result = true;
  } else if (id == Class_PropertyDefinition) {
    result = true;
  } else if (id == Class_TypeDefinitionStrongObjectReference) {
    result = true;
  } else if (id == Class_TypeDefinitionWeakObjectReference) {
    result = true;
  } else if (id == Class_TypeDefinitionSet) {
    result = true;
  } else if (id == Class_TypeDefinitionFixedArray) {
    result = true;
  } else if (id == Class_TypeDefinitionVaryingArray) {
    result = true;
  } else if (id == Class_TypeDefinitionRecord) {
    result = true;
  } else if (id == Class_TypeDefinitionStream) {
    result = true;
  } else if (id == Class_TypeDefinitionRename) {
    result = true;
  } else if (id == Class_TypeDefinitionIndirect) {
    result = true;
  } else if (id == Class_TypeDefinitionOpaque) {
    result = true;
  } else if (id == Class_TypeDefinitionCharacter) {
    result = true;
  } else if (id == Class_TypeDefinitionString) {
    result = true;
  } else if (id == Class_TypeDefinitionInteger) {
    result = true;
  } else if (id == Class_TypeDefinitionEnumerated) {
    result = true;
  } else if (id == Class_TypeDefinitionExtendibleEnumerated) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool OMDictionary::isMeta(const OMObjectIdentification& id) const
{
  TRACE("OMDictionary::isMeta");
  bool result;
  if (isAbstractMeta(id)) {
    result = true;
  } else if (isConcreteMeta(id)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

void OMDictionary::mapFromKLV(OMPropertyId& id)
{
  TRACE("OMDictionary::mapFromKLV");

  if (id == 0x3f06) {        // MXFEssenceContainerData::IndexSID
    id = 0x2b01;             // AAFEssenceData::SampleIndex
  } else if (id == 0x3f07){  // MXFEssenceContainerData::BodySID
    id =  0x2702;            // AAFEssenceData::Data
  }
}

void OMDictionary::mapToKLV(OMPropertyId& id)
{
  TRACE("OMDictionary::mapToKLV");

  if (id == 0x2b01) {        // AAFEssenceData::SampleIndex
    id = 0x3f06;             // MXFEssenceContainerData::IndexSID
  } else if (id == 0x2702) { // AAFEssenceData::Data
    id = 0x3f07;             // MXFEssenceContainerData::BodySID
  }
}

//{04070200-0000-0000-060e-2b3401010102}
static const OMUniqueObjectIdentification kAAFPropID_EssenceData_Data =
{0x04070200, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02}};
//{06010102-0100-0000-060e-2b3401010102}
static const OMUniqueObjectIdentification kAAFPropID_EssenceData_SampleIndex =
{0x06010102, 0x0100, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02}};

// {01030404-0000-0000-060E-2B3401010104}
static const OMUniqueObjectIdentification kAAFPropID_EssenceData_BodySID =
{0x01030404, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x04}};
// {01030405-0000-0000-060E-2B3401010104}
static const OMUniqueObjectIdentification kAAFPropID_EssenceData_IndexSID =
{0x01030405, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x04}};

void OMDictionary::mapFromKLV(OMObjectIdentification& id)
{
  TRACE("OMDictionary::mapFromKLV");

  if (id == kAAFPropID_EssenceData_IndexSID) {
    id = kAAFPropID_EssenceData_SampleIndex;
  } else if (id == kAAFPropID_EssenceData_BodySID) {
    id = kAAFPropID_EssenceData_Data;
  }
}

void OMDictionary::mapToKLV(OMObjectIdentification & id)
{
  TRACE("OMDictionary::mapToKLV");

  if (id == kAAFPropID_EssenceData_SampleIndex) {
    id = kAAFPropID_EssenceData_IndexSID;
  } else if (id == kAAFPropID_EssenceData_Data) {
    id = kAAFPropID_EssenceData_BodySID;
  }
}

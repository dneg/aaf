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
// British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "OMSymbolspace.h"
#include "OMXMLStorage.h"
#include "OMDictionary.h"
#include "OMFile.h"
#include "OMPropertyTable.h"
#include "OMClassDefinition.h"
#include "OMPropertyDefinition.h"
#include "OMType.h"
#include "OMCharacterType.h"
#include "OMEnumeratedType.h"
#include "OMExtendibleEnumeratedType.h"
#include "OMArrayType.h"
#include "OMIndirectType.h"
#include "OMIntegerType.h"
#include "OMOpaqueType.h"
#include "OMRecordType.h"
#include "OMRenamedType.h"
#include "OMSetType.h"
#include "OMStreamType.h"
#include "OMStringType.h"
#include "OMObjectReferenceType.h"
#include "OMSetIterator.h"
#include "OMXMLUtilities.h"
#include "OMVector.h"
#include "OMExceptions.h"
#include "OMAssertions.h"


static const OMUniqueObjectIdentification ClassID_ClassDefinition =
{0x0d010101, 0x0201, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_PropertyDefinition =
{0x0d010101, 0x0202, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionInteger =
{0x0d010101, 0x0204, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionStrongObjectReference =
{0x0d010101, 0x0205, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionWeakObjectReference =
{0x0d010101, 0x0206, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionEnumeration =
{0x0d010101, 0x0207, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionFixedArray =
{0x0d010101, 0x0208, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionVaryingArray =
{0x0d010101, 0x0209, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionSet =
{0x0d010101, 0x020a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionString =
{0x0d010101, 0x020b, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionStream =
{0x0d010101, 0x020c, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionRecord =
{0x0d010101, 0x020d, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionRename =
{0x0d010101, 0x020e, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionExtendibleEnumeration =
{0x0d010101, 0x0220, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionIndirect =
{0x0d010101, 0x0221, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionOpaque =
{0x0d010101, 0x0222, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
static const OMUniqueObjectIdentification ClassID_TypeDefinitionCharacter =
{0x0d010101, 0x0223, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

static const OMPropertyId classDefsTargetPath[3] = {0x0001, 0x0003, 0x0000};
static const OMPropertyId typeDefsTargetPath[3] = {0x0001, 0x0004, 0x0000};


const wchar_t* OMSymbolspace::_baselineURI = 
    L"http://www.aafassociation.org/aafx/v1.1/20090617";
const OMUniqueObjectIdentification OMSymbolspace::_baselineId =
{0x696aa72c, 0x7c41, 0x4fa7, {0x84, 0x99, 0x30, 0xfe, 0x83, 0xe3, 0xdb, 0x12}};
const wchar_t* OMSymbolspace::_baselineVersion = L"1.1";
    

class EnumForReg : public TypeDefForReg
{
public:
    EnumForReg() : TypeDefForReg() {} 
    virtual ~EnumForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return false;
    }
    
    virtual bool dependsOnAType()
    {
        return true;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return &elementTypeId;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return 0;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newEnumeratedType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            elementTypeId, 
            (const wchar_t**)&elementNames.getAt(0), // this assumes that a vector is stored as an array 
            &elementValues.getAt(0),  // this assumes that a vector is stored as an array
            elementNames.count());
    
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMUniqueObjectIdentification elementTypeId;
    OMVector<wchar_t*> elementNames;
    OMVector<OMInt64> elementValues;
};

class FixedArrayForReg : public TypeDefForReg
{
public:
    FixedArrayForReg() : TypeDefForReg() {}
    virtual ~FixedArrayForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return false;
    }
    
    virtual bool dependsOnAType()
    {
        return true;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return &elementTypeId;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return 0;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newFixedArrayType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            elementTypeId, 
            elementCount);
            
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMUniqueObjectIdentification elementTypeId;
    OMUInt32 elementCount;
};

class RecordForReg : public TypeDefForReg
{
public:
    RecordForReg() : TypeDefForReg() {}
    virtual ~RecordForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return true;
    }
    
    virtual bool dependsOnAType()
    {
        return true;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return 0;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return &memberTypeIds;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newRecordType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            &memberTypeIds.getAt(0),
            (const wchar_t**)&memberNames.getAt(0),  // this assumes that a vector is stored as an array
            memberNames.count());
    
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMVector<OMUniqueObjectIdentification> memberTypeIds;
    OMVector<wchar_t*> memberNames;
};

class RenamedForReg : public TypeDefForReg
{
public:
    RenamedForReg() : TypeDefForReg() {}
    virtual ~RenamedForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return false;
    }
    
    virtual bool dependsOnAType()
    {
        return true;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return &renamedTypeId;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return 0;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newRenamedType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            renamedTypeId);
            
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMUniqueObjectIdentification renamedTypeId;
};

class SetForReg : public TypeDefForReg
{
public:
    SetForReg() : TypeDefForReg() {}
    virtual ~SetForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return false;
    }
    
    virtual bool dependsOnAType()
    {
        return true;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return &elementTypeId;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return 0;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newSetType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            elementTypeId);
            
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMUniqueObjectIdentification elementTypeId;
};

class StringForReg : public TypeDefForReg
{
public:
    StringForReg() : TypeDefForReg() {}
    virtual ~StringForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return false;
    }
    
    virtual bool dependsOnAType()
    {
        return true;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return &elementTypeId;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return 0;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newStringType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            elementTypeId);
            
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMUniqueObjectIdentification elementTypeId;
};

class StrongRefForReg : public TypeDefForReg
{
public:
    StrongRefForReg() : TypeDefForReg() {}
    virtual ~StrongRefForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return false;
    }
    
    virtual bool dependsOnAType()
    {
        return false;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return &referencedClassId;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return 0;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newStrongReferenceType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            referencedClassId);
            
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMUniqueObjectIdentification referencedClassId;
};

class VarArrayForReg : public TypeDefForReg
{
public:
    VarArrayForReg() : TypeDefForReg() {}
    virtual ~VarArrayForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return false;
    }
    
    virtual bool dependsOnAType()
    {
        return true;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return &elementTypeId;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return 0;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newVaryingArrayType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            elementTypeId);
            
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMUniqueObjectIdentification elementTypeId;
};

class WeakRefForReg : public TypeDefForReg
{
public:
    WeakRefForReg() : TypeDefForReg() {}
    virtual ~WeakRefForReg() {}
    
    virtual bool hasManyDependencies()
    {
        return false;
    }
    
    virtual bool dependsOnAType()
    {
        return false;
    }
    
    virtual OMUniqueObjectIdentification* dependsOn()
    {
        return &referencedClassId;
    }
    
    virtual OMVector<OMUniqueObjectIdentification>* dependsOnMany()
    {
        return 0;
    }
    
    virtual void registerType(OMSymbolspace* symbolspace, OMDictionary* dictionary)
    {
        dictionary->newWeakReferenceType(
            id, 
            name.c_str(), 
            getOptionalDescription(),
            referencedClassId,
            (const OMObjectIdentification*)&targetSet.getAt(0),  // this assumes that a vector is stored as an array
            targetSet.count());
            
        symbolspace->createSymbolForType(
            id, 
            name.c_str());
    }
    
    OMUniqueObjectIdentification referencedClassId;
    OMVector<OMUniqueObjectIdentification> targetSet;
};



    
OMSymbolspace::OMSymbolspace(OMXMLStorage* store)
: _isInitialised(false), _store(store), _id(nullOMUniqueObjectIdentification), _uri(0), 
    _preferredPrefix(0), _prefix(0), _description(0), _version(0), _uniqueSymbolSuffix(1)
{}

OMSymbolspace::OMSymbolspace(OMXMLStorage* store, OMUniqueObjectIdentification id, const wchar_t* uri, 
    const wchar_t* preferredPrefix, const wchar_t* description)
: _isInitialised(false), _store(store), _id(nullOMUniqueObjectIdentification), _uri(0), 
    _preferredPrefix(0), _prefix(0), _description(0), _version(0), _uniqueSymbolSuffix(1)
{
    TRACE("OMSymbolspace::OMSymbolspace");
    
    initialise(id, uri, preferredPrefix, description);
}

OMSymbolspace::~OMSymbolspace()
{
    TRACE("OMSymbolspace::~OMSymbolspace");
    
    if (_uri != 0)
    {
        delete [] _uri;
    }
    if (_preferredPrefix != 0)
    {
        delete [] _preferredPrefix;
    }
    if (_prefix != 0)
    {
        delete [] _prefix;
    }
    if (_description != 0)
    {
        delete [] _description;
    }
    if (_version != 0)
    {
        delete [] _version;
    }

    OMUInt32 i;
    for (i = 0; i < _propertyDefs.count(); i++)
    {
        delete _propertyDefs.getAt(i);
    }
    OMSetIterator<ExtEnumId, ExtEnumElement*> iterD(_extEnumElements, OMBefore);
    while (++iterD)
    {
        delete iterD.value();
    }
}

bool 
OMSymbolspace::isEmpty()
{
    TRACE("OMSymbolspace::isEmpty");
    
    return _idToSymbol.count() == 0 && _extEnumElements.count() == 0;
}

OMUniqueObjectIdentification 
OMSymbolspace::getId() const
{
    TRACE("OMSymbolspace::getId");
    PRECONDITION("Is initialised", _isInitialised);
    
    return _id;
}

const wchar_t* 
OMSymbolspace::getURI() const
{
    TRACE("OMSymbolspace::getURI");
    PRECONDITION("Is initialised", _isInitialised);
    
    return _uri;
}

const wchar_t* 
OMSymbolspace::getPreferredPrefix() const
{
    TRACE("OMSymbolspace::getPreferredPrefix");
    PRECONDITION("Is initialised", _isInitialised);
    
    return _preferredPrefix;
}
    
const wchar_t* 
OMSymbolspace::getPrefix() const
{
    TRACE("OMSymbolspace::getPrefix");
    PRECONDITION("Is initialised", _isInitialised);
    
    if (_prefix == 0)
    {
        return _preferredPrefix;
    }
    return _prefix;
}

void 
OMSymbolspace::setPrefix(const wchar_t* prefix)
{
    TRACE("OMSymbolspace::setPrefix");
    
    if (_prefix != 0)
    {
        delete [] _prefix;
        _prefix = 0;
    }
    
    if (prefix != 0)
    {
        _prefix = new wchar_t[wcslen(prefix) + 1];
        wcscpy(_prefix, prefix);
    }
}

const wchar_t* 
OMSymbolspace::getDescription() const
{
    TRACE("OMSymbolspace::getDescription");
    PRECONDITION("Is initialised", _isInitialised);
    
    return _description;
}
    
const wchar_t* 
OMSymbolspace::getVersion() const
{
    TRACE("OMSymbolspace::getVersion");
    
    return _version;
}

const wchar_t* 
OMSymbolspace::getMetaDefSymbol(OMUniqueObjectIdentification id) const
{
    TRACE("OMSymbolspace::getMetaDefSymbol");
    
    OMWString* symbol;
    if (_idToSymbol.find(id, &symbol))
    {
        return symbol->c_str();
    }
    
    return 0;
}

OMUniqueObjectIdentification 
OMSymbolspace::getMetaDefId(const wchar_t* symbol) const
{
    TRACE("OMSymbolspace::getMetaDefId");
    PRECONDITION("Valid symbol", symbol != 0);

    OMUniqueObjectIdentification id;
    if (_symbolToId.find(symbol, id))
    {
        return id;
    }
    
    return nullOMUniqueObjectIdentification;
}

OMPropertyId 
OMSymbolspace::getPropertyDefId(const wchar_t* symbol) const
{
    TRACE("OMSymbolspace::getPropertyDefId");
    PRECONDITION("Valid symbol", symbol != 0);

    OMUniqueObjectIdentification id = getMetaDefId(symbol);
    
    OMPropertyId localId;
    if (_idToLocalId.find(id, localId))
    {
        return localId;
    }
    
    return 0x0000;
}

const wchar_t* 
OMSymbolspace::getDefSymbol(OMUniqueObjectIdentification id)
{
    TRACE("OMSymbolspace::getDefSymbol");
    
    OMWString* defSymbol;
    if (_idToDefSymbol.find(id, &defSymbol))
    {
        return (*defSymbol).c_str();
    }
    return 0;
}

OMUniqueObjectIdentification 
OMSymbolspace::getDefId(const wchar_t* definitionSymbol) const
{
    TRACE("OMSymbolspace::getDefId");
    
    OMUniqueObjectIdentification id;
    if (_defSymbolToId.find(definitionSymbol, id))
    {
        return id;
    }
    return nullOMUniqueObjectIdentification;
}

bool 
OMSymbolspace::isKnownExtEnumElement(OMUniqueObjectIdentification elementOf, 
    OMUniqueObjectIdentification value) const
{
    TRACE("OMSymbolspace::isKnownExtEnumElement");

    return _extEnumElements.contains(ExtEnumId(elementOf, value));
}

void 
OMSymbolspace::addClassDef(OMClassDefinition* classDef)
{
    TRACE("OMSymbolspace::addClassDef");
    
    _classDefs.append(classDef);
    createSymbolForClass(classDef->identification(), classDef->name());    
}

void 
OMSymbolspace::addTypeDef(OMType* typeDef)
{
    TRACE("OMSymbolspace::addTypeDef");
    
    _typeDefs.append(typeDef);
    createSymbolForType(typeDef->identification(), typeDef->name());
}

void 
OMSymbolspace::addPropertyDef(OMClassDefinition* classDef, OMPropertyDefinition* propertyDef)
{
    TRACE("OMSymbolspace::addPropertyDef");
    
    PropertyPair* pp = new PropertyPair;
    pp->ownerClassDef = classDef;
    pp->propertyDef = propertyDef;
    
    _propertyDefs.append(pp);
    createSymbolForProperty(propertyDef->identification(), propertyDef->localIdentification(),
        propertyDef->name());    
}

void 
OMSymbolspace::addExtEnumElement(OMUniqueObjectIdentification elementOf, 
    const wchar_t* name, OMUniqueObjectIdentification value)
{
    TRACE("OMSymbolspace::addExtEnumElement");

    if (!_extEnumElements.contains(ExtEnumId(elementOf, value)))
    {
        ExtEnumElement* member = new ExtEnumElement;
        member->elementOf = elementOf;
        member->name = name;
        member->value = value;
        _extEnumElements.insert(ExtEnumId(elementOf, value), member);
    }
}

void 
OMSymbolspace::save()
{
    TRACE("OMSymbolspace::save");
    PRECONDITION("Is initialised", _isInitialised);
    
    getWriter()->writeElementStart(getBaselineURI(), L"Extension");

    wchar_t idUri[XML_MAX_AUID_URI_SIZE];
    auidToURI(_id, idUri);
    getWriter()->writeElementStart(getBaselineURI(), L"Identification");
    getWriter()->writeElementContent(idUri, wcslen(idUri));
    getWriter()->writeElementEnd();

    getWriter()->writeElementStart(getBaselineURI(), L"Symbolspace");
    getWriter()->writeElementContent(_uri, wcslen(_uri));
    getWriter()->writeElementEnd();

    if (_preferredPrefix != 0 && wcslen(_preferredPrefix) > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"PreferredPrefix");
        getWriter()->writeElementContent(_preferredPrefix,
            wcslen(_preferredPrefix));
        getWriter()->writeElementEnd();
    }

    if (_description != 0 && wcslen(_description) > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Description");
        getWriter()->writeElementContent(_description,
            wcslen(_description));
        getWriter()->writeElementEnd();
    }

    if (_classDefs.count() > 0 || _typeDefs.count() > 0 || _propertyDefs.count() > 0 ||
        _extEnumElements.count() > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Definitions");

        OMUInt32 i;
        for (i = 0; i<_classDefs.count(); i++)
        {
            saveClassDef(_classDefs.getAt(i));
        }
        
        for (i = 0; i<_propertyDefs.count(); i++)
        {
            savePropertyDef(_propertyDefs.getAt(i)->ownerClassDef, 
                _propertyDefs.getAt(i)->propertyDef);
        }

        for (i = 0; i<_typeDefs.count(); i++)
        {
            saveTypeDef(_typeDefs.getAt(i));
        }

        OMSetIterator<ExtEnumId, ExtEnumElement*> iter(_extEnumElements, OMBefore);
        while (++iter)
        {
            saveExtEnumElement(iter.value());
        }
        
        getWriter()->writeElementEnd();
    }

    getWriter()->writeElementEnd();
}

void 
OMSymbolspace::restore(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restore");
    
    // make sure classdefs and typedefs target paths are set
    // these are needed when registering extensions MetaDefinitions
    
    // TODO: remove this now that the OMDictionary/ImplAAFMetaDictionary takes care of this
    getClassDefsTag(dictionary);
    getTypeDefsTag(dictionary);
    
    OMUniqueObjectIdentification id;
    OMWString symbolspace;
    OMWString preferredPrefix;
    OMWString description;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"Identification"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid Extension Identification "
                    "value");
            }
            getReader()->getCharacters(data, length);
            uriToAUID(data, &id);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Symbolspace"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid Extension Symbolspace "
                    "value");
            }
            getReader()->getCharacters(data, length);
            symbolspace = data;
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"PreferredPrefix"))
        {
            getReader()->next();
            if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
            {
                const wchar_t* data;
                OMUInt32 length;
                getReader()->getCharacters(data, length);
                preferredPrefix = data;
            }
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Description"))
        {
            getReader()->next();
            if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
            {
                const wchar_t* data;
                OMUInt32 length;
                getReader()->getCharacters(data, length);
                description = data;
            }
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Definitions"))
        {
            while (getReader()->nextElement())
            {
                restoreMetaDictDefinition(dictionary);
            }
            getReader()->moveToEndElement();
        }
        else
        {
            throw OMException("Unknown element in Extension");
        }
    }
    getReader()->moveToEndElement();
    
    initialise(id, symbolspace.c_str(), preferredPrefix.c_str(), description.c_str());
}

void 
OMSymbolspace::registerDeferredDefs(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::registerDeferredDefs");

    OMUInt32 count;
    OMUInt32 i, j;

    // types
    
    bool removed = _typeDefsForRegistration.count() == 0;
    OMUInt32 index = 0;
    bool dependenciesSatified;
    while (_typeDefsForRegistration.count() > 0)
    {
        TypeDefForReg* typeForReg = _typeDefsForRegistration.getAt(index);

        // check if dependencies are satisfied        
        if (typeForReg->hasManyDependencies())
        {
            dependenciesSatified = true;
            OMVector<OMUniqueObjectIdentification>* dependencies = typeForReg->dependsOnMany();
            count = dependencies->count();
            for (j = 0; j < count; j++)
            {
                if ((typeForReg->dependsOnAType() && !dictionary->registerTypeDef(dependencies->getAt(j))) ||
                    (!typeForReg->dependsOnAType() && !dictionary->registerClassDef(dependencies->getAt(j))))
                {
                    dependenciesSatified = false;
                    break;
                }
            }
        }
        else
        {
            dependenciesSatified = true;
            if ((typeForReg->dependsOnAType() && !dictionary->registerTypeDef(*typeForReg->dependsOn())) ||
                (!typeForReg->dependsOnAType() && !dictionary->registerClassDef(*typeForReg->dependsOn())))
            {
                dependenciesSatified = false;
            }
        }
        
        // register type if dependencies are satisfied
        if (dependenciesSatified)
        {
            typeForReg->registerType(this, dictionary);
            _typeDefsForRegistration.removeAt(index);
            delete typeForReg;

            if (index >= _typeDefsForRegistration.count())
            {
                index = 0;
            }
            removed = true;
        }
        else
        {
            index = (index + 1) % _typeDefsForRegistration.count();
            if (index == 0)
            {
                if (removed == false)
                {
                    // no types were removed, which means we have a circular reference
                    break;
                }
                removed = false;
            }
        }
    }
    _typeDefsForRegistration.clear();
    
    if (!removed)
    {
        throw OMException("Found circular type definition references in the Extension Symbolspace");
    }


    // extendible enumeration elements
    
    count = _extEnumElementsForRegistration.count();
    for (i = 0; i < count; i++)
    {
        ExtEnumElement* extEnumElement = _extEnumElementsForRegistration.getAt(i);
     
        dictionary->newExtendibleEnumeratedTypeElement(
            extEnumElement->elementOf,
            extEnumElement->name.c_str(), 
            extEnumElement->value);
        
        addExtEnumElement(
            extEnumElement->elementOf, 
            extEnumElement->name.c_str(), 
            extEnumElement->value);
        
        delete extEnumElement;
    }
    _extEnumElementsForRegistration.clear();

    
    // properties
    
    count = _propertyDefsForRegistration.count();
    for (i = 0; i < count; i++)
    {
        RegisterPropertyPair* rp = _propertyDefsForRegistration.getAt(i);
        
        const OMPropertyDefinition* propertyDef = dictionary->newProperty(
            rp->id,
            rp->name.c_str(),
            rp->description.c_str(),
            rp->localId,
            rp->typeId,
            rp->isOptional,
            rp->isUniqueIdentifier,
            rp->ownerClassId);
        if (propertyDef == NULL)
        {
            throw OMException("Failed to create new property definition");
        }
            
        createSymbolForProperty(
            propertyDef->identification(), 
            propertyDef->localIdentification(), 
            propertyDef->name());

        delete rp;
    }
    _propertyDefsForRegistration.clear();
}

void 
OMSymbolspace::resetForWriting()
{
    TRACE("OMSymbolspace::resetForWriting");
    
    // remove the definitions that must be saved to the document
    // but keep the symbols/identification/ext enum members so that the definitions
    // remain in the same symbolspace when writing
    OMUInt32 i;
    for (i = 0; i < _propertyDefs.count(); i++)
    {
        delete _propertyDefs.getAt(i);
    }
    
    _classDefs.clear();
    _propertyDefs.clear();
    _typeDefs.clear();
}



void 
OMSymbolspace::initialise(OMUniqueObjectIdentification id, const wchar_t* uri, 
    const wchar_t* preferredPrefix, const wchar_t* description)
{
    TRACE("OMSymbolspace::initialise");
    ASSERT("Valid symbolspace id", id != nullOMUniqueObjectIdentification);
    ASSERT("Valid symbolspace uri", uri != 0);
    
    _id = id;
    _uri = new wchar_t[wcslen(uri) + 1];
    wcscpy(_uri, uri);

    if (preferredPrefix != 0 && wcslen(preferredPrefix) > 0)
    {
        _preferredPrefix = new wchar_t[wcslen(preferredPrefix) + 1];
        wcscpy(_preferredPrefix, preferredPrefix);
    }
    if (description != 0 && wcslen(description) > 0)
    {
        _description = new wchar_t[wcslen(description) + 1];
        wcscpy(_description, description);
    }
    
    _isInitialised = true;
}

OMXMLWriter*
OMSymbolspace::getWriter()
{
    TRACE("OMSymbolspace::getWriter");
    
    return _store->getWriter();
}

OMXMLReader*
OMSymbolspace::getReader()
{
    TRACE("OMSymbolspace::getReader");
    
    return _store->getReader();
}

const wchar_t* 
OMSymbolspace::createSymbolForClass(OMUniqueObjectIdentification id, const wchar_t* name)
{
    TRACE("OMSymbolspace::createSymbolForClass");
    PRECONDITION("Valid name", name != 0);

    if (!_idToSymbol.contains(id))
    {
        wchar_t* symbol = createSymbol(name);
        addMetaDefSymbol(id, symbol);
        delete [] symbol;
    }
    
    return getMetaDefSymbol(id);
}

const wchar_t* 
OMSymbolspace::createSymbolForProperty(OMUniqueObjectIdentification id, OMPropertyId localId,
    const wchar_t* name)
{
    TRACE("OMSymbolspace::createSymbolForProperty");
    PRECONDITION("Valid name", name != 0);

    if (!_idToSymbol.contains(id))
    {
        wchar_t* symbol = createSymbol(name);
        addPropertyDefSymbol(id, localId, symbol);
        delete [] symbol;
    }
    
    return getMetaDefSymbol(id);
}

const wchar_t* 
OMSymbolspace::createSymbolForType(OMUniqueObjectIdentification id, const wchar_t* name)
{
    TRACE("OMSymbolspace::createSymbolForType");
    PRECONDITION("Valid name", name != 0);

    if (!_idToSymbol.contains(id))
    {
        wchar_t* symbol = createSymbol(name);
        addMetaDefSymbol(id, symbol);
        delete [] symbol;
    }
    
    return getMetaDefSymbol(id);
}

wchar_t* 
OMSymbolspace::createSymbol(const wchar_t* name)
{
    TRACE("OMSymbolspace::createSymbol");
    PRECONDITION("Valid name", name != 0);
    
    size_t nameLen = wcslen(name);
    if (nameLen == 0)
    {
        wchar_t* symbol = new wchar_t[2];
        symbol[0] = L'_';
        symbol[1] = L'\0';
        return symbol;
    }

    wchar_t* symbol = new wchar_t[nameLen + 1];
    
    const wchar_t* namePtr = name;
    wchar_t* symbolPtr = symbol;
    while (*namePtr != L'\0')
    {
        int codeLen = utf16CodeLen(namePtr);
        if (codeLen == -1) // invalid character
        {
            *symbolPtr = L'_';
        }
        else if (codeLen == 2) // surrogate pair
        {
            *symbolPtr = L'_';
            namePtr++; // this and increment below skips the pair
        }
        else if (namePtr == name) // the first character
        {
            if ((*namePtr >= L'A' && *namePtr <= L'Z') ||
                (*namePtr >= L'a' && *namePtr <= L'z') ||
                (*namePtr == L'_'))
            {
                *symbolPtr = *namePtr;
            }
            else
            {
                *symbolPtr = L'_';
            }
        }
        else if ((*namePtr >= L'A' && *namePtr <= L'Z') ||
            (*namePtr >= L'a' && *namePtr <= L'z') ||
            (*namePtr >= L'0' && *namePtr <= L'9') ||
            (*namePtr == L'.') ||
            (*namePtr == L'-') ||
            (*namePtr == L'_'))
        {
            *symbolPtr = *namePtr;
        }
        else
        {
            *symbolPtr = L'_';
        }
    
        symbolPtr++;
        namePtr++;
    }
    *symbolPtr = L'\0';

    
    // modify the symbol until it is unique
    while (_symbolToId.contains(symbol))
    {
        wchar_t suffix[9];
        std_swprintf(suffix, 9, L"%u", _uniqueSymbolSuffix);
        size_t len = wcslen(symbol);
        wchar_t* newSymbol = new wchar_t[len + 8 + 1];
        wcscpy(&newSymbol[0], symbol);
        delete [] symbol;
        symbol = wcscat(newSymbol, suffix);
        _uniqueSymbolSuffix++;
    }
    
    return symbol;
}

void 
OMSymbolspace::addMetaDefSymbol(OMUniqueObjectIdentification id, const wchar_t* symbol)
{
    TRACE("OMSymbolspace::addMetaDefSymbol");
    PRECONDITION("Symbol is unique", !_symbolToId.contains(symbol));
    PRECONDITION("Identification is unique", !_idToSymbol.contains(id));
    
    _idToSymbol.insert(id, symbol);
    _symbolToId.insert(symbol, id);
}
    
void 
OMSymbolspace::addPropertyDefSymbol(OMUniqueObjectIdentification id, OMPropertyId localId, const wchar_t* symbol)
{
    TRACE("OMSymbolspace::addPropertyDefSymbol");
    PRECONDITION("Symbol is unique", !_symbolToId.contains(symbol));
    PRECONDITION("Identification is unique", !_idToSymbol.contains(id));
    
    _idToSymbol.insert(id, symbol);
    _symbolToId.insert(symbol, id);
    _idToLocalId.insert(id, localId);
}

void 
OMSymbolspace::addDefSymbol(OMUniqueObjectIdentification id, const wchar_t* symbol)
{
    TRACE("OMSymbolspace::addDefSymbol");
    PRECONDITION("Symbol is unique", !_defSymbolToId.contains(symbol));
    PRECONDITION("Identification is unique", !_idToDefSymbol.contains(id));
    
    _idToDefSymbol.insert(id, symbol);
    _defSymbolToId.insert(symbol, id);
}
 
void
OMSymbolspace::saveClassDef(OMClassDefinition* classDef)
{
    TRACE("OMSymbolspace::saveClassDef");

    getWriter()->writeElementStart(getBaselineURI(), L"ClassDefinition");
    
    wchar_t uri[XML_MAX_AUID_URI_SIZE];
    auidToURI(classDef->identification(), uri);
    getWriter()->writeElementStart(getBaselineURI(), L"Identification");
    getWriter()->writeElementContent(uri, wcslen(uri));
    getWriter()->writeElementEnd();

    const wchar_t* symbol = getMetaDefSymbol(classDef->identification());
    getWriter()->writeElementStart(getBaselineURI(), L"Symbol");
    getWriter()->writeElementContent(symbol, wcslen(symbol));
    getWriter()->writeElementEnd();

    const wchar_t* name = classDef->name();
    getWriter()->writeElementStart(getBaselineURI(), L"Name");
    getWriter()->writeElementContent(name, wcslen(name));
    getWriter()->writeElementEnd();

    if (classDef->hasDescription())
    {
        const wchar_t* description = classDef->description();
        getWriter()->writeElementStart(getBaselineURI(), L"Description");
        if (description != 0 && wcslen(description) > 0)
        {
            getWriter()->writeElementContent(description, wcslen(description));
        }
        getWriter()->writeElementEnd();
    }
    
    OMClassDefinition* parentClass = classDef->omParentClass(); 
    if (parentClass != 0)
    {
        wchar_t* idStr = saveMetaDefAUID(parentClass->identification());        
        getWriter()->writeElementStart(getBaselineURI(), L"ParentClass");
        getWriter()->writeElementContent(idStr, wcslen(idStr));
        getWriter()->writeElementEnd();
        delete [] idStr;
    }

    wchar_t boolStr[XML_MAX_BOOL_STRING_SIZE];
    boolToString(classDef->omIsConcrete(), boolStr);
    getWriter()->writeElementStart(getBaselineURI(), L"IsConcrete");
    getWriter()->writeElementContent(boolStr, wcslen(boolStr));
    getWriter()->writeElementEnd();
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::savePropertyDef(OMClassDefinition* ownerClassDef, OMPropertyDefinition* propertyDef)
{
    TRACE("OMSymbolspace::savePropertyDef");

    getWriter()->writeElementStart(getBaselineURI(), L"PropertyDefinition");

    wchar_t uri[XML_MAX_AUID_URI_SIZE];
    auidToURI(propertyDef->identification(), uri);
    getWriter()->writeElementStart(getBaselineURI(), L"Identification");
    getWriter()->writeElementContent(uri, wcslen(uri));
    getWriter()->writeElementEnd();

    const wchar_t* symbol = getMetaDefSymbol(propertyDef->identification());
    getWriter()->writeElementStart(getBaselineURI(), L"Symbol");
    getWriter()->writeElementContent(symbol, wcslen(symbol));
    getWriter()->writeElementEnd();

    const wchar_t* name = propertyDef->name();
    getWriter()->writeElementStart(getBaselineURI(), L"Name");
    getWriter()->writeElementContent(name, wcslen(name));
    getWriter()->writeElementEnd();

    if (propertyDef->hasDescription())
    {
        const wchar_t* description = propertyDef->description();
        getWriter()->writeElementStart(getBaselineURI(), L"Description");
        if (description != 0 && wcslen(description) > 0)
        {
            getWriter()->writeElementContent(description, wcslen(description));
        }
        getWriter()->writeElementEnd();
    }

    const OMType* type = propertyDef->type();
    wchar_t* idStr = saveMetaDefAUID(type->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"Type");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;

    idStr = saveMetaDefAUID(ownerClassDef->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"MemberOf");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;

    wchar_t localIdStr[XML_MAX_INTEGER_STRING_SIZE];
    OMUInt16 localId = propertyDef->localIdentification();
    integerToString((OMByte*)&localId, sizeof(OMUInt16), false, localIdStr, true);
    getWriter()->writeElementStart(getBaselineURI(), L"LocalIdentification");
    getWriter()->writeElementContent(localIdStr, wcslen(localIdStr));
    getWriter()->writeElementEnd();
    
    wchar_t boolStr[XML_MAX_BOOL_STRING_SIZE];
    boolToString(propertyDef->isOptional(), boolStr);
    getWriter()->writeElementStart(getBaselineURI(), L"IsOptional");
    getWriter()->writeElementContent(boolStr, wcslen(boolStr));
    getWriter()->writeElementEnd();

    if (propertyDef->isUniqueIdentifier())
    {
        boolToString(true, boolStr);
        getWriter()->writeElementStart(getBaselineURI(), L"IsUniqueIdentifier");
        getWriter()->writeElementContent(boolStr, wcslen(boolStr));
        getWriter()->writeElementEnd();
    }
    
    
    getWriter()->writeElementEnd();
}


class SaveTypeDefVisitor : public OMTypeVisitor
{
public:
    SaveTypeDefVisitor(OMSymbolspace* symbolspace)
    : _haveSavedType(false)
    {
        _symbolspace = symbolspace;
    }
    virtual ~SaveTypeDefVisitor()
    {}
    
    virtual void visitCharacterType(const OMCharacterType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveCharacterTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitEnumeratedType(const OMEnumeratedType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveEnumeratedTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitExtendibleEnumeratedType(const OMExtendibleEnumeratedType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveExtendibleEnumeratedTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitFixedArrayType(const OMFixedArrayType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveFixedArrayTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitIndirectType(const OMIndirectType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveIndirectTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitIntegerType(const OMIntegerType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveIntegerTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitOpaqueType(const OMOpaqueType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveOpaqueTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitRenamedType(const OMRenamedType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveRenamedTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitRecordType(const OMRecordType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveRecordTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitSetType(const OMSetType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveSetTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitStreamType(const OMStreamType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveStreamTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitStringType(const OMStringType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveStringTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitStrongObjectReferenceType(const OMStrongObjectReferenceType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveStrongObjectReferenceTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitVaryingArrayType(const OMVaryingArrayType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveVaryingArrayTypeDef(type);
            _haveSavedType = true;
        }
    }
    virtual void visitWeakObjectReferenceType(const OMWeakObjectReferenceType* type)
    {
        if (!_haveSavedType)
        {
            _symbolspace->saveWeakObjectReferenceTypeDef(type);
            _haveSavedType = true;
        }
    }
    
private:    
    OMSymbolspace* _symbolspace;
    bool _haveSavedType;
};

void
OMSymbolspace::saveTypeDef(const OMType* typeDef)
{
    TRACE("OMSymbolspace::saveTypeDef");

    SaveTypeDefVisitor visitor(this);
    typeDef->accept(visitor);
}

void
OMSymbolspace::saveCommonTypeDef(const OMType* typeDef)
{
    TRACE("OMSymbolspace::saveCommonTypeDef");

    wchar_t uri[XML_MAX_AUID_URI_SIZE];
    auidToURI(typeDef->identification(), uri);
    getWriter()->writeElementStart(getBaselineURI(), L"Identification");
    getWriter()->writeElementContent(uri, wcslen(uri));
    getWriter()->writeElementEnd();

    const wchar_t* symbol = getMetaDefSymbol(typeDef->identification());
    getWriter()->writeElementStart(getBaselineURI(), L"Symbol");
    getWriter()->writeElementContent(symbol, wcslen(symbol));
    getWriter()->writeElementEnd();

    const wchar_t* name = typeDef->name();
    getWriter()->writeElementStart(getBaselineURI(), L"Name");
    getWriter()->writeElementContent(name, wcslen(name));
    getWriter()->writeElementEnd();

    if (typeDef->hasDescription())
    {
        const wchar_t* description = typeDef->description();
        getWriter()->writeElementStart(getBaselineURI(), L"Description");
        if (description != 0 && wcslen(description) > 0)
        {
            getWriter()->writeElementContent(description, wcslen(description));
        }
        getWriter()->writeElementEnd();
    }
}

void
OMSymbolspace::saveCharacterTypeDef(const OMCharacterType* typeDef)
{
    TRACE("OMSymbolspace::saveCharacterTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionCharacter");
    saveCommonTypeDef(typeDef);
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveEnumeratedTypeDef(const OMEnumeratedType* typeDef)
{
    TRACE("OMSymbolspace::saveEnumeratedTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionEnumeration");

    saveCommonTypeDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t* idStr = saveMetaDefAUID(elementType->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;

    OMUInt32 count = typeDef->elementCount();
    if (count > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Elements");
        
        for (OMUInt32 i = 0; i<count; i++)
        {
            const wchar_t* elementName = typeDef->elementName(i);
            OMInt64 elementValue = typeDef->elementValue(i);

            wchar_t valueStr[XML_MAX_INTEGER_STRING_SIZE];
            integerToString((OMByte*)&elementValue, sizeof(OMInt64), true, valueStr);

            getWriter()->writeElementStart(getBaselineURI(), L"Name");
            getWriter()->writeElementContent(elementName, wcslen(elementName));
            getWriter()->writeElementEnd();
            
            getWriter()->writeElementStart(getBaselineURI(), L"Value");
            getWriter()->writeElementContent(valueStr, wcslen(valueStr));
            getWriter()->writeElementEnd();
        }

        getWriter()->writeElementEnd();
    }
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveExtendibleEnumeratedTypeDef(const OMExtendibleEnumeratedType* typeDef)
{
    TRACE("OMSymbolspace::saveExtendibleEnumeratedTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionExtendibleEnumeration");

    saveCommonTypeDef(typeDef);

    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveFixedArrayTypeDef(const OMFixedArrayType* typeDef)
{
    TRACE("OMSymbolspace::saveFixedArrayTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionFixedArray");

    saveCommonTypeDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t* idStr = saveMetaDefAUID(elementType->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;

    wchar_t elementCountStr[XML_MAX_INTEGER_STRING_SIZE];
    OMUInt32 elementCount = typeDef->elementCount();
    integerToString((OMByte*)&elementCount, sizeof(OMUInt32), false, elementCountStr);
    getWriter()->writeElementStart(getBaselineURI(), L"ElementCount");
    getWriter()->writeElementContent(elementCountStr, wcslen(elementCountStr));
    getWriter()->writeElementEnd();
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveIndirectTypeDef(const OMIndirectType* typeDef)
{
    TRACE("OMSymbolspace::saveIndirectTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionIndirect");
    saveCommonTypeDef(typeDef);
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveIntegerTypeDef(const OMIntegerType* typeDef)
{
    TRACE("OMSymbolspace::saveIntegerTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionInteger");

    saveCommonTypeDef(typeDef);

    wchar_t sizeStr[XML_MAX_INTEGER_STRING_SIZE];
    OMUInt8 size = typeDef->size();
    integerToString((OMByte*)&size, sizeof(OMUInt8), false, sizeStr);
    getWriter()->writeElementStart(getBaselineURI(), L"Size");
    getWriter()->writeElementContent(sizeStr, wcslen(sizeStr));
    getWriter()->writeElementEnd();
    
    wchar_t boolStr[XML_MAX_BOOL_STRING_SIZE];
    boolToString(typeDef->isSigned(), boolStr);
    getWriter()->writeElementStart(getBaselineURI(), L"IsSigned");
    getWriter()->writeElementContent(boolStr, wcslen(boolStr));
    getWriter()->writeElementEnd();
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveOpaqueTypeDef(const OMOpaqueType* typeDef)
{
    TRACE("OMSymbolspace::saveOpaqueTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionOpaque");
    saveCommonTypeDef(typeDef);
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveRecordTypeDef(const OMRecordType* typeDef)
{
    TRACE("OMSymbolspace::saveRecordTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionRecord");

    saveCommonTypeDef(typeDef);

    OMUInt32 count = typeDef->memberCount();
    if (count > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Members");
        
        for (OMUInt32 i = 0; i<count; i++)
        {
            const wchar_t* memberName = typeDef->memberName(i);
            const OMType* memberType = typeDef->memberType(i);

            getWriter()->writeElementStart(getBaselineURI(), L"Name");
            getWriter()->writeElementContent(memberName, wcslen(memberName));
            getWriter()->writeElementEnd();
            
            wchar_t* idStr = saveMetaDefAUID(memberType->identification());        
            getWriter()->writeElementStart(getBaselineURI(), L"Type");
            getWriter()->writeElementContent(idStr, wcslen(idStr));
            getWriter()->writeElementEnd();
            delete [] idStr;
        }

        getWriter()->writeElementEnd();
    }
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveRenamedTypeDef(const OMRenamedType* typeDef)
{
    TRACE("OMSymbolspace::saveRenamedTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionRename");

    saveCommonTypeDef(typeDef);

    const OMType* renamedType = typeDef->renamedType(); 
    wchar_t* idStr = saveMetaDefAUID(renamedType->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"RenamedType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveSetTypeDef(const OMSetType* typeDef)
{
    TRACE("OMSymbolspace::saveSetTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionSet");

    saveCommonTypeDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t* idStr = saveMetaDefAUID(elementType->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveStreamTypeDef(const OMStreamType* typeDef)
{
    TRACE("OMSymbolspace::saveStreamTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionStream");
    saveCommonTypeDef(typeDef);
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveStringTypeDef(const OMStringType* typeDef)
{
    TRACE("OMSymbolspace::saveStringTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionString");

    saveCommonTypeDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t* idStr = saveMetaDefAUID(elementType->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;

    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveStrongObjectReferenceTypeDef(const OMStrongObjectReferenceType* typeDef)
{
    TRACE("OMSymbolspace::saveStrongObjectReferenceTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionStrongObjectReference");

    saveCommonTypeDef(typeDef);

    const OMClassDefinition* referencedClass = typeDef->referencedClass(); 
    wchar_t* idStr = saveMetaDefAUID(referencedClass->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"ReferencedType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;

    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveVaryingArrayTypeDef(const OMVaryingArrayType* typeDef)
{
    TRACE("OMSymbolspace::saveVaryingArrayTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionVariableArray");

    saveCommonTypeDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t* idStr = saveMetaDefAUID(elementType->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveWeakObjectReferenceTypeDef(const OMWeakObjectReferenceType* typeDef)
{
    TRACE("OMSymbolspace::saveWeakObjectReferenceTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionWeakObjectReference");

    saveCommonTypeDef(typeDef);

    const OMClassDefinition* referencedClass = typeDef->referencedClass(); 
    wchar_t* idStr = saveMetaDefAUID(referencedClass->identification());        
    getWriter()->writeElementStart(getBaselineURI(), L"ReferencedType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;

    getWriter()->writeElementStart(getBaselineURI(), L"TargetSet");
    OMUInt32 targetPathCount = typeDef->targetPathElementCount();
    OMUInt32 i;
    for (i = 0; i < targetPathCount; i++)
    {
        const OMUniqueObjectIdentification& element = typeDef->targetPathElement(i);
        idStr = saveMetaDefAUID(element);        
        getWriter()->writeElementStart(getBaselineURI(), L"MetaDefRef");
        getWriter()->writeElementContent(idStr, wcslen(idStr));
        getWriter()->writeElementEnd();
        delete [] idStr;
    }
    getWriter()->writeElementEnd();

    
    getWriter()->writeElementEnd();
}

void 
OMSymbolspace::saveExtEnumElement(const ExtEnumElement* member)
{
    TRACE("OMSymbolspace::saveExtEnumElement");
    
    getWriter()->writeElementStart(getBaselineURI(), L"ExtendibleEnumerationElement");

    wchar_t* idStr = saveMetaDefAUID(member->elementOf);        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementOf");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    delete [] idStr;

    getWriter()->writeElementStart(getBaselineURI(), L"Name");
    getWriter()->writeElementContent(member->name.c_str(), member->name.length());
    getWriter()->writeElementEnd();
    
    wchar_t valueStr[XML_MAX_AUID_URI_SIZE];
    auidToURI(member->value, valueStr);
    getWriter()->writeElementStart(getBaselineURI(), L"Value");
    getWriter()->writeElementContent(valueStr, wcslen(valueStr));
    getWriter()->writeElementEnd();
    
    getWriter()->writeElementEnd();
}


void 
OMSymbolspace::restoreMetaDictDefinition(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreMetaDictDefinition");
    
    const wchar_t* nmspace;
    const wchar_t* localName;
    const OMList<OMXMLAttribute*>* attrs;
    getReader()->getStartElement(nmspace, localName, attrs);
    
    if (getReader()->elementEquals(getBaselineURI(), L"ClassDefinition"))
    {
        restoreClassDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"PropertyDefinition"))
    {
        restorePropertyDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionCharacter"))
    {
        restoreCharacterTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionEnumeration"))
    {
        restoreEnumeratedTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionExtendibleEnumeration"))
    {
        restoreExtEnumeratedTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionFixedArray"))
    {
        restoreFixedArrayTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionIndirect"))
    {
        restoreIndirectTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionInteger"))
    {
        restoreIntTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionOpaque"))
    {
        restoreOpaqueTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionRecord"))
    {
        restoreRecordTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionRename"))
    {
        restoreRenamedTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionSet"))
    {
        restoreSetTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionStream"))
    {
        restoreStreamTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionString"))
    {
        restoreStringTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionStrongObjectReference"))
    {
        restoreStrongObjectReferenceTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionVariableArray"))
    {
        restoreVaryingArrayTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionWeakObjectReference"))
    {
        restoreWeakObjectReferenceTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"ExtendibleEnumerationElement"))
    {
        restoreExtEnumElement(dictionary);
    }
    else
    {
        throw OMException("Unknown definition type encountered");
    }
    
}

bool 
OMSymbolspace::restoreMetaDef(MetaDef* metaDef)
{
    TRACE("OMSymbolspace::restoreMetaDef");
    
    if (getReader()->elementEquals(getBaselineURI(), L"Identification"))
    {
        const wchar_t* data;
        OMUInt32 length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMException("Empty string is invalid MetaDef Identification value");
        }
        getReader()->getCharacters(data, length);
        uriToAUID(data, &metaDef->id);
        getReader()->moveToEndElement();
        
        return true;
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"Symbol"))
    {
        const wchar_t* data;
        OMUInt32 length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMException("Empty string is invalid MetaDef Symbol value");
        }
        getReader()->getCharacters(data, length);
        metaDef->symbol = data;
        getReader()->moveToEndElement();
        
        return true;
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"Name"))
    {
        const wchar_t* data;
        OMUInt32 length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMException("Empty string is invalid MetaDef Name value");
        }
        getReader()->getCharacters(data, length);
        metaDef->name = data;
        getReader()->moveToEndElement();
        
        return true;
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"Description"))
    {
        metaDef->descriptionIsNull = false;
        getReader()->next();
        if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->getCharacters(data, length);
            metaDef->description = data;
        }
        getReader()->moveToEndElement();
        
        return true;
    }
    else 
    {
        return false;
    }
}

void 
OMSymbolspace::restoreClassDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreClassDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification parentClassId = nullOMUniqueObjectIdentification;
    bool isConcrete;
    
    bool isConcreteSet = false;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ParentClass"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid ClassDef ParentClass "
                    "value");
            }
            getReader()->getCharacters(data, length);
            parentClassId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"IsConcrete"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid ClassDef IsConcrete "
                    "value");
            }
            getReader()->getCharacters(data, length);
            boolFromString(data, isConcrete);
            getReader()->moveToEndElement();
            isConcreteSet = true;
        }
        else 
        {
            if (!restoreMetaDef(&metaDef))
            {
                throw OMException("Unknown element in ClassDefinition");
            }
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || !isConcreteSet)
    {
        throw OMException("Incomplete ClassDefinition");
    }
    
    dictionary->newClass(
        metaDef.id, 
        metaDef.name.c_str(), 
        metaDef.getOptionalDescription(), 
        parentClassId, 
        isConcrete);

    createSymbolForClass(
        metaDef.id, 
        metaDef.name.c_str()); 
}

void 
OMSymbolspace::restorePropertyDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restorePropertyDef");
    
    MetaDef metaDef;
    OMPropertyId localId = 0;
    OMUniqueObjectIdentification typeId = nullOMUniqueObjectIdentification;
    OMUniqueObjectIdentification memberOfId = nullOMUniqueObjectIdentification;
    bool isOptional;
    bool isUniqueIdentifier = false;
    
    bool isOptionalSet = false;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"LocalIdentification"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid PropertyDef "
                    "LocalIdentification value");
            }
            getReader()->getCharacters(data, length);
            uint16FromString(data, localId);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Type"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid PropertyDef Type "
                    "value");
            }
            getReader()->getCharacters(data, length);
            typeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"MemberOf"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid PropertyDef MemberOf "
                    "value");
            }
            getReader()->getCharacters(data, length);
            memberOfId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"IsUniqueIdentifier"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid PropertyDef IsUniqueIdentifier "
                    "value");
            }
            getReader()->getCharacters(data, length);
            boolFromString(data, isUniqueIdentifier);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"IsOptional"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid PropertyDef IsOptional "
                    "value");
            }
            getReader()->getCharacters(data, length);
            boolFromString(data, isOptional);
            getReader()->moveToEndElement();
            isOptionalSet = true;
        }
        else 
        {
            if (!restoreMetaDef(&metaDef))
            {
                throw OMException("Unknown element in PropertyDefinition");
            }
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || localId == 0 || typeId == nullOMUniqueObjectIdentification ||
        memberOfId == nullOMUniqueObjectIdentification || !isOptionalSet)
    {
        throw OMException("Incomplete PropertyDefinition");
    }

    // defer registration of property until after all classes have been registered
    RegisterPropertyPair* regPair = new RegisterPropertyPair;
    regPair->ownerClassId = memberOfId;
    regPair->id = metaDef.id;
    regPair->name = metaDef.name.c_str();
    regPair->description = metaDef.getOptionalDescription();
    regPair->localId = localId;
    regPair->typeId = typeId;
    regPair->isOptional = isOptional;
    regPair->isUniqueIdentifier = isUniqueIdentifier;

    _propertyDefsForRegistration.append(regPair);
}

void 
OMSymbolspace::restoreCharacterTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreCharacterTypeDef");
    
    MetaDef metaDef;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (!restoreMetaDef(&metaDef))
        {
            throw OMException("Unknown element in CharacterTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMException("Incomplete CharacterTypeDef");
    }
    
    dictionary->newCharacterType(
        metaDef.id, 
        metaDef.name.c_str(), 
        metaDef.getOptionalDescription());

    createSymbolForType(
        metaDef.id, 
        metaDef.name.c_str()); 
}

void 
OMSymbolspace::restoreEnumeratedTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreEnumeratedTypeDef");
    
    EnumForReg* typeForReg = new EnumForReg();
    _typeDefsForRegistration.append(typeForReg);
    typeForReg->elementTypeId = nullOMUniqueObjectIdentification;
    OMSet<OMWString, OMWString> elementNamesSet;
    OMSet<OMInt64, OMInt64> elementValuesSet;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid Enumerated ElementType "
                    "value");
            }
            getReader()->getCharacters(data, length);
            typeForReg->elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Elements"))
        {
            while (getReader()->nextElement())
            {
                const wchar_t* nmspace;
                const wchar_t* localName;
                const OMList<OMXMLAttribute*>* attrs;
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Name"))
                {
                    throw OMException("Expecting Name element in EnumeratedType Elements");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMException("Invalid Name element in EnumeratedType Elements");
                }
                const wchar_t* data;
                OMUInt32 length;
                getReader()->getCharacters(data, length);
                if (elementNamesSet.contains(data))
                {
                    throw OMException("Duplicate Name value in EnumeratedType Elements");
                }
                elementNamesSet.insert(data, data);
                typeForReg->elementNames.append(wideCharacterStringDup(data));
                getReader()->moveToEndElement();
                
                if (!getReader()->nextElement())
                {
                    throw OMException("Missing matching Value element in EnumeratedType Elements");
                }
                
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Value"))
                {
                    throw OMException("Expecting Value element in EnumeratedType Elements");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMException("Invalid Value element in EnumeratedType Elements");
                }
                getReader()->getCharacters(data, length);
                OMInt64 value;
                int64FromString(data, value);
                if (elementValuesSet.contains(value))
                {
                    throw OMException("Duplicate Value value in EnumeratedType Elements");
                }
                elementValuesSet.insert(value, value);
                typeForReg->elementValues.append(value);
                getReader()->moveToEndElement();
            }
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in EnumeratedTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet() || typeForReg->elementTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMException("Incomplete EnumeratedTypeDef");
    }

}

void 
OMSymbolspace::restoreExtEnumeratedTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreExtEnumeratedTypeDef");
    
    MetaDef metaDef;
    OMSet<OMWString, OMWString> elementNamesSet;
    OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification> elementValuesSet;
    OMVector<wchar_t*> elementNames;
    OMVector<OMUniqueObjectIdentification> elementValues;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (!restoreMetaDef(&metaDef))
        {
            throw OMException("Unknown element in ExtEnumeratedTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMException("Incomplete ExtEnumeratedTypeDef");
    }

    dictionary->newExtendibleEnumeratedType(
        metaDef.id, 
        metaDef.name.c_str(), 
        metaDef.getOptionalDescription());

    createSymbolForType(
        metaDef.id, 
        metaDef.name.c_str());
}

void 
OMSymbolspace::restoreFixedArrayTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreFixedArrayTypeDef");
    
    FixedArrayForReg* typeForReg = new FixedArrayForReg();
    _typeDefsForRegistration.append(typeForReg);
    typeForReg->elementTypeId = nullOMUniqueObjectIdentification;
    
    bool elementCountSet = false;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid FixedArray ElementType "
                    "value");
            }
            getReader()->getCharacters(data, length);
            typeForReg->elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"ElementCount"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid FixedArray ElementCount "
                    "value");
            }
            getReader()->getCharacters(data, length);
            uint32FromString(data, typeForReg->elementCount);
            getReader()->moveToEndElement();
            elementCountSet = true;
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in FixedArrayTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet() || typeForReg->elementTypeId == nullOMUniqueObjectIdentification ||
        !elementCountSet)
    {
        throw OMException("Incomplete FixedArrayTypeDef");
    }

}

void 
OMSymbolspace::restoreIndirectTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreIndirectTypeDef");
    
    MetaDef metaDef;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (!restoreMetaDef(&metaDef))
        {
            throw OMException("Unknown element in IndirectTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMException("Incomplete IndirectTypeDef");
    }

    dictionary->newIndirectType(
        metaDef.id, 
        metaDef.name.c_str(), 
        metaDef.getOptionalDescription());

    createSymbolForType(
        metaDef.id, 
        metaDef.name.c_str());
}

void 
OMSymbolspace::restoreIntTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreIntTypeDef");
    
    MetaDef metaDef;
    OMUInt8 size = 0;
    bool isSigned;
    
    bool isSignedSet = false;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"Size"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid IntTypeDef "
                    "Size value");
            }
            getReader()->getCharacters(data, length);
            uint8FromString(data, size);
            if (size != 1 && size != 2 && size != 4 && size != 8)
            {
                throw OMException("Invalid IntTypeDef size value");
            }
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"IsSigned"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid IntTypeDef "
                    "IsSigned value");
            }
            getReader()->getCharacters(data, length);
            boolFromString(data, isSigned);
            getReader()->moveToEndElement();
            isSignedSet = true;
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMException("Unknown element in IntTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || size == 0 || !isSignedSet)
    {
        throw OMException("Incomplete IntTypeDef");
    }

    dictionary->newIntegerType(
        metaDef.id, 
        metaDef.name.c_str(), 
        metaDef.getOptionalDescription(),
        size,
        isSigned);

    createSymbolForType(
        metaDef.id, 
        metaDef.name.c_str());
}

void 
OMSymbolspace::restoreOpaqueTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreOpaqueTypeDef");
    
    MetaDef metaDef;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (!restoreMetaDef(&metaDef))
        {
            throw OMException("Unknown element in OpaqueTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMException("Incomplete OpaqueTypeDef");
    }

    dictionary->newOpaqueType(
        metaDef.id, 
        metaDef.name.c_str(), 
        metaDef.getOptionalDescription());

    createSymbolForType(
        metaDef.id, 
        metaDef.name.c_str());
}

void 
OMSymbolspace::restoreRecordTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreRecordTypeDef");
    
    RecordForReg* typeForReg = new RecordForReg();
    _typeDefsForRegistration.append(typeForReg);
    OMSet<OMWString, OMWString> memberNamesSet;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"Members"))
        {
            while (getReader()->nextElement())
            {
                const wchar_t* nmspace;
                const wchar_t* localName;
                const OMList<OMXMLAttribute*>* attrs;
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Name"))
                {
                    throw OMException("Expecting Name element in RecordType Members");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMException("Invalid Name element in RecordType Members");
                }
                const wchar_t* data;
                OMUInt32 length;
                getReader()->getCharacters(data, length);
                if (memberNamesSet.contains(data))
                {
                    throw OMException("Duplicate Name value in RecordType Elements");
                }
                memberNamesSet.insert(data, data);
                typeForReg->memberNames.append(wideCharacterStringDup(data));
                getReader()->moveToEndElement();
                
                if (!getReader()->nextElement())
                {
                    throw OMException("Missing matching Type element in RecordType Members");
                }
                
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Type"))
                {
                    throw OMException("Expecting Type element in RecordType Members");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMException("Invalid Type element in RecordType Members");
                }
                getReader()->getCharacters(data, length);
                OMUniqueObjectIdentification typeId = restoreMetaDefAUID(data);
                typeForReg->memberTypeIds.append(typeId);
                getReader()->moveToEndElement();
            }
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in RecordTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet())
    {
        throw OMException("Incomplete RecordTypeDef");
    }

}

void 
OMSymbolspace::restoreRenamedTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreRenamedTypeDef");
    
    RenamedForReg* typeForReg = new RenamedForReg();
    _typeDefsForRegistration.append(typeForReg);
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"RenamedType"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid Renamed RenamedType "
                    "value");
            }
            getReader()->getCharacters(data, length);
            typeForReg->renamedTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in RenamedTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet() || typeForReg->renamedTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMException("Incomplete RenamedTypeDef");
    }
}

void 
OMSymbolspace::restoreSetTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreSetTypeDef");
    
    SetForReg* typeForReg = new SetForReg();
    _typeDefsForRegistration.append(typeForReg);
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid Set ElementType "
                    "value");
            }
            getReader()->getCharacters(data, length);
            typeForReg->elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in SetTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet() || typeForReg->elementTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMException("Incomplete SetTypeDef");
    }
}

void 
OMSymbolspace::restoreStreamTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreStreamTypeDef");
    
    MetaDef metaDef;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (!restoreMetaDef(&metaDef))
        {
            throw OMException("Unknown element in StreamTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMException("Incomplete StreamTypeDef");
    }

    dictionary->newStreamType(
        metaDef.id, 
        metaDef.name.c_str(), 
        metaDef.getOptionalDescription());

    createSymbolForType(
        metaDef.id, 
        metaDef.name.c_str());
}

void 
OMSymbolspace::restoreStringTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreStringTypeDef");
    
    StringForReg* typeForReg = new StringForReg();
    _typeDefsForRegistration.append(typeForReg);
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid String ElementType "
                    "value");
            }
            getReader()->getCharacters(data, length);
            typeForReg->elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in StringTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet() || typeForReg->elementTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMException("Incomplete StringTypeDef");
    }
}

void 
OMSymbolspace::restoreStrongObjectReferenceTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreStrongObjectReferenceTypeDef");
    
    StrongRefForReg* typeForReg = new StrongRefForReg();
    _typeDefsForRegistration.append(typeForReg);
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ReferencedType"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid StrongObjectReference "
                    "ReferencedType value");
            }
            getReader()->getCharacters(data, length);
            typeForReg->referencedClassId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in StrongObjectReferenceTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet() || typeForReg->referencedClassId == nullOMUniqueObjectIdentification)
    {
        throw OMException("Incomplete StrongObjectReferenceTypeDef");
    }
}

void 
OMSymbolspace::restoreVaryingArrayTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreVaryingArrayTypeDef");
    
    VarArrayForReg* typeForReg = new VarArrayForReg();
    _typeDefsForRegistration.append(typeForReg);
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid VariableArray ElementType "
                    "value");
            }
            getReader()->getCharacters(data, length);
            typeForReg->elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in VariableArrayTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet() || typeForReg->elementTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMException("Incomplete VariableArrayTypeDef");
    }
}

void 
OMSymbolspace::restoreWeakObjectReferenceTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreWeakObjectReferenceTypeDef");
    
    WeakRefForReg* typeForReg = new WeakRefForReg();
    _typeDefsForRegistration.append(typeForReg);
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ReferencedType"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid WeakObjectReference "
                    "ReferencedType value");
            }
            getReader()->getCharacters(data, length);
            typeForReg->referencedClassId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"TargetSet"))
        {
            while (getReader()->nextElement())
            {
                const wchar_t* nmspace;
                const wchar_t* localName;
                const OMList<OMXMLAttribute*>* attrs;
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"MetaDefRef"))
                {
                    throw OMException("Expecting MetaDefRef element in WeakObjectReferenceType TargetSet");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMException("Invalid MetaDefRef element in WeakObjectReferenceType TargetSet");
                }
                const wchar_t* data;
                OMUInt32 length;
                getReader()->getCharacters(data, length);
                OMUniqueObjectIdentification id = restoreMetaDefAUID(data);
                getReader()->moveToEndElement();
                
                typeForReg->targetSet.append(id);
            }
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(typeForReg))
        {
            throw OMException("Unknown element in WeakObjectReferenceTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!typeForReg->isSet() || typeForReg->referencedClassId == nullOMUniqueObjectIdentification)
    {
        throw OMException("Incomplete WeakObjectReferenceTypeDef");
    }
}

void 
OMSymbolspace::restoreExtEnumElement(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreExtEnumElement");
    
    ExtEnumElement* extEnumElement = new ExtEnumElement;
    extEnumElement->elementOf = nullOMUniqueObjectIdentification;
    extEnumElement->value = nullOMUniqueObjectIdentification;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementOf"))
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid ExtEnumElement "
                    "ElementOf");
            }
            getReader()->getCharacters(data, length);
            extEnumElement->elementOf = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Name"))
        {
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Invalid Name element in ExtEnumeratedElement");
            }
            const wchar_t* data;
            OMUInt32 length;
            getReader()->getCharacters(data, length);
            extEnumElement->name = data;
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Value"))
        {
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Invalid Value element in ExtEnumeratedElement");
            }
            const wchar_t* data;
            OMUInt32 length;
            getReader()->getCharacters(data, length);
            uriToAUID(data, &extEnumElement->value);
            getReader()->moveToEndElement();
        }
        else
        {
            throw OMException("Unknown element in ExtEnumElement");
        }
    }
    getReader()->moveToEndElement();
    
    if (extEnumElement->elementOf == nullOMUniqueObjectIdentification ||
        extEnumElement->name.length() == 0 ||
        extEnumElement->value == nullOMUniqueObjectIdentification)
    {
        throw OMException("Incomplete ExtEnumElement");
    }

    _extEnumElementsForRegistration.insert(extEnumElement);
}


OMUniqueObjectIdentification
OMSymbolspace::restoreMetaDefAUID(const wchar_t* idStr)
{
    TRACE("OMSymbolspace::restoreMetaDefAUID");
    
    OMUniqueObjectIdentification id = nullOMUniqueObjectIdentification;
    if (isAUIDURI(idStr))
    {
        uriToAUID(idStr, &id);
    }
    else if (isQSymbol(idStr))
    {
        id = _store->getMetaDefIdFromQSymbol(idStr);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMException("Could not retrieve unique id from qualified symbol");
        }
    }
    else
    {
        id = _store->getBaselineMetaDefId(idStr);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMException("Could not retrieve unique id from symbol");
        }
    }
    return id;
}

wchar_t*
OMSymbolspace::saveMetaDefAUID(OMUniqueObjectIdentification id)
{
    TRACE("OMSymbolspace::saveMetaDefAUID");
    
    wchar_t* idStr = 0;
    const wchar_t* symbol = _store->getBaselineMetaDefSymbol(id);
    if (symbol != 0)
    {
        idStr = new wchar_t[wcslen(symbol) + 1];
        wcscpy(idStr, symbol);
    }
    else
    {
        const wchar_t* symbolspace;
        const wchar_t* symbol;
        if (_store->getMetaDefSymbol(id, &symbolspace, &symbol))
        {
            idStr = new wchar_t[wcslen(symbolspace) + 1 + wcslen(symbol) + 1];
            wcscpy(idStr, symbolspace);
            wcscat(idStr, L" ");
            wcscat(idStr, symbol);
        }
        else
        {
            idStr = new wchar_t[XML_MAX_AUID_URI_SIZE];
            auidToURI(id, idStr);
        }
    }
    
    return idStr;
}

// TODO: remove this now that the OMDictionary/ImplAAFMetaDictionary takes care of this
// TODO: cache tags?
OMPropertyTag 
OMSymbolspace::getClassDefsTag(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::getClassDefsTag");

    OMPropertyTable* table = dictionary->file()->referencedProperties();
    return table->insert(classDefsTargetPath);
}

// TODO: remove this now that the OMDictionary/ImplAAFMetaDictionary takes care of this
// TODO: cache tags?
OMPropertyTag 
OMSymbolspace::getTypeDefsTag(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::getTypeDefsTag");

    OMPropertyTable* table = dictionary->file()->referencedProperties();
    return table->insert(typeDefsTargetPath);
}

const wchar_t* 
OMSymbolspace::getBaselineURI()
{
    return _baselineURI;
}

OMSymbolspace::ExtEnumId::ExtEnumId()
: _elementOf(nullOMUniqueObjectIdentification), _value(nullOMUniqueObjectIdentification)
{
    TRACE("OMSymbolspace::ExtEnumId::ExtEnumId()");    
}

OMSymbolspace::ExtEnumId::ExtEnumId(OMUniqueObjectIdentification elementOf, 
    OMUniqueObjectIdentification value)
: _elementOf(elementOf), _value(value)
{
    TRACE("OMSymbolspace::ExtEnumId::ExtEnumId(OMUniqueObjectIdentification. OMUniqueObjectIdentification)");
}

OMSymbolspace::ExtEnumId::ExtEnumId(const ExtEnumId& id)
: _elementOf(id._elementOf), _value(id._value)
{
    TRACE("OMSymbolspace::ExtEnumId::ExtEnumId(ExtEnumId)");    
}

OMSymbolspace::ExtEnumId::~ExtEnumId()
{
    TRACE("OMSymbolspace::ExtEnumId::~ExtEnumId");    
}
        
OMSymbolspace::ExtEnumId& 
OMSymbolspace::ExtEnumId::operator=(const ExtEnumId& rhs)
{
    // MSVC 6.0 didn't like this TRACE - said it couldn't access the private class
    //TRACE("OMSymbolspace::ExtEnumId::operator=");
    
    _elementOf = rhs._elementOf;
    _value = rhs._value;
    
    return *this;
}

bool 
OMSymbolspace::ExtEnumId::operator==(const ExtEnumId& rhs) const
{
    TRACE("OMSymbolspace::ExtEnumId::operator==");
    
    return _elementOf == rhs._elementOf && _value == rhs._value;
}

bool 
OMSymbolspace::ExtEnumId::operator!=(const ExtEnumId& rhs) const
{
    TRACE("OMSymbolspace::ExtEnumId::operator1=");
    
    return !operator==(rhs);
}

bool 
OMSymbolspace::ExtEnumId::operator<(const ExtEnumId& rhs) const
{
    TRACE("OMSymbolspace::ExtEnumId::operator<");
    
    return _elementOf < rhs._elementOf || 
        (_elementOf == rhs._elementOf && _value < rhs._value);
}




#define LITERAL_AUID(l, w1, w2,  b1, b2, b3, b4, b5, b6, b7, b8) \
    {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}

#define ADD_METADEF_SYMBOL(ID, SYMBOL) \
{ \
    ASSERT("Symbol size is less than maximum", wcslen(SYMBOL) < XML_MAX_BASELINE_SYMBOL_SIZE); \
    const OMUniqueObjectIdentification id = ID; \
    ss->addMetaDefSymbol(id, SYMBOL); \
}

#define ADD_PROPERTYDEF_SYMBOL(ID, LOCAL_ID, SYMBOL) \
{ \
    ASSERT("Symbol size is less than maximum", wcslen(SYMBOL) < XML_MAX_BASELINE_SYMBOL_SIZE); \
    const OMUniqueObjectIdentification id = ID; \
    ss->addPropertyDefSymbol(id, LOCAL_ID, SYMBOL); \
}

#define ADD_DEF_SYMBOL_ID(ID, SYMBOL) \
{ \
    ASSERT("Symbol size is less than maximum", wcslen(SYMBOL) < XML_MAX_BASELINE_SYMBOL_SIZE); \
    const OMUniqueObjectIdentification id = ID; \
    ss->addDefSymbol(id, SYMBOL); \
}

#define SET_EXT_ENUM_ID(MEMBEROF) \
{ \
    const OMUniqueObjectIdentification id = MEMBEROF; \
    elementOf = id; \
}


#define ADD_EXT_ENUM_VALUE(VALUE) \
{ \
    const OMUniqueObjectIdentification value = VALUE; \
    ss->addExtEnumElement(elementOf, 0, value); \
}



OMSymbolspace* 
OMSymbolspace::createDefaultExtSymbolspace(OMXMLStorage* store, OMUniqueObjectIdentification id)
{
    TRACE("OMSymbolspace::createDefaultExtSymbolspace");

    wchar_t uri[XML_MAX_AUID_URI_SIZE];
    auidToURI(id, uri);
    
    OMSymbolspace* ss = new OMSymbolspace(store, id, uri, L"this", 
        L"AAF file default extension symbolspace");
    
    return ss;
}

OMSymbolspace* 
OMSymbolspace::createBaselineSymbolspace(OMXMLStorage* store, const wchar_t* uri)
{
    TRACE("OMSymbolspace::createBaselineSymbolspace(OMXMLStorage, wchar_t)");

    if (wcscmp(uri, _baselineURI) == 0)
    {
        return createBaselineSymbolspace(store);
    }
    else
    {
        return 0;
    }
}

OMSymbolspace* 
OMSymbolspace::createBaselineSymbolspace(OMXMLStorage* store)
{
    TRACE("OMSymbolspace::createBaselineSymbolspace(OMXMLStorage)");

    OMSymbolspace* ss = new OMSymbolspace(store, _baselineId, _baselineURI, L"aaf", 
        L"AAF version 1.1 baseline symbolspace");
    ss->_version = wideCharacterStringDup(_baselineVersion);
        
    //
    // Classes
    //
    
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"InterchangeObject");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Component");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0300, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Segment");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0400, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EdgeCode");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0500, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EssenceGroup");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0600, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Event");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0700, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"GPITrigger");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0800, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"CommentMarker");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0900, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Filler");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0A00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"OperationGroup");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0B00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"NestedScope");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0C00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Pulldown");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0D00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ScopeReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0E00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Selector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0F00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Sequence");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SourceReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1100, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SourceClip");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1200, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TextClip");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1300, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"HTMLClip");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1400, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Timecode");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1500, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TimecodeStream");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1600, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TimecodeStream12M");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1700, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Transition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1800, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ContentStorage");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1900, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ControlPoint");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1A00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DefinitionObject");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1B00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DataDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1C00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"OperationDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1D00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ParameterDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1E00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"PluginDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x1F00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"CodecDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ContainerDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2100, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"InterpolationDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2200, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Dictionary");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2300, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EssenceData");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2400, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EssenceDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2500, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"FileDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2600, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"AIFCDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2700, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"PictureDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2800, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"CDCIDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2900, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"RGBADescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2A00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"HTMLDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2B00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TIFFDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2C00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"WAVEDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2D00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"FilmDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2E00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TapeDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x2F00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Preface");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Identification");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3100, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Locator");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3200, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"NetworkLocator");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3300, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TextLocator");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3400, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Package");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3500, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"CompositionPackage");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3600, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MaterialPackage");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3700, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SourcePackage");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3800, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Track");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3900, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EventTrack");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3A00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"StaticTrack");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3B00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TimelineTrack");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3C00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Parameter");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3D00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ConstantValue");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3E00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"VaryingValue");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x3F00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TaggedValue");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"KLVData");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4100, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DescriptiveMarker");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4200, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SoundDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4300, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DataEssenceDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4400, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MultipleDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4500, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DescriptiveClip");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4700, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"AES3PCMDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4800, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"WAVEPCMDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4900, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"PhysicalDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4A00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ImportDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4B00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"RecordingDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4C00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TaggedValueDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4D00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"KLVDataDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4E00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"AuxiliaryDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x4F00, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"RIFFChunk");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x5000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"BWFImportDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x5100, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MPEGVideoDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x5900, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SubDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ClassDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0202, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"PropertyDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0203, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0204, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionInteger");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0205, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionStrongObjectReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0206, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionWeakObjectReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0207, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionEnumeration");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0208, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionFixedArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0209, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionVariableArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x020A, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x020B, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionString");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x020C, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionStream");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x020D, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionRecord");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x020E, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionRename");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0220, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionExtendibleEnumeration");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0221, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionIndirect");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0222, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionOpaque");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0223, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionCharacter");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0224, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MetaDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0225, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MetaDictionary");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DescriptiveObject");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0D010401, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DescriptiveFramework");
    
    
    //
    // Properties
    //
    
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0101, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0101,
        L"ObjectClass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0102,
        L"LinkedGenerationID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04070100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0201,
        L"ComponentDataDefinition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020201, 0x0103, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0202,
        L"ComponentLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0203,
        L"ComponentKLVData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020102, 0x1600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x0204,
        L"ComponentUserComments");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x0205,
        L"ComponentAttributes");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01040901, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0401,
        L"EdgeCodeStart");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0109, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0402,
        L"EdgeCodeFilmFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0102, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x0403,
        L"EdgeCodeFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01030201, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0404,
        L"EdgeCodeHeader");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0601, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0501,
        L"Choices");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0208, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0502,
        L"StillFrame");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0303, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0601,
        L"EventPosition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300404, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0602,
        L"EventComment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300401, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x0801,
        L"ActiveState");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x020A, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0901,
        L"AnnotationSource");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300506, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0b01,
        L"Operation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0602, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0b02,
        L"InputSegments");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x060A, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0b03,
        L"Parameters");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0530050C, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0b04,
        L"BypassOverride");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0206, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0b05,
        L"Rendering");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0607, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0c01,
        L"NestedScopeTracks");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0207, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0d01,
        L"InputSegment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05401001, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0d02,
        L"PulldownKind");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05401001, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0d03,
        L"PulldownDirection");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05401001, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0d04,
        L"PhaseFrame");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0e01,
        L"RelativeScope");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0e02,
        L"RelativeTrack");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0209, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0f01,
        L"SelectedSegment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0608, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0f02,
        L"AlternateSegments");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0609, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1001,
        L"ComponentObjects");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1101,
        L"SourcePackageID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1102,
        L"SourceTrackID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x1103,
        L"ChannelIDs");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x1104,
        L"MonoSourceTrackIDs");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0104, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1201,
        L"StartPosition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020201, 0x0105, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1202,
        L"FadeInLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300501, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1203,
        L"FadeInType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020201, 0x0105, 0x0300, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1204,
        L"FadeOutLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300502, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1205,
        L"FadeOutType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300601, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1401,
        L"BeginAnchor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300602, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1402,
        L"EndAnchor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0105, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1501,
        L"StartTimecode");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040101, 0x0206, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1502,
        L"FramesPerSecond");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040101, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1503,
        L"DropFrame");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040101, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1601,
        L"TimecodeStreamSampleRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04070300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1602,
        L"TimecodeStreamData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040201, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1603,
        L"TimecodeSource");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040101, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1701,
        L"IncludeSync");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0205, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1801,
        L"TransitionOperation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0106, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1802,
        L"CutPoint");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0501, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1901,
        L"Packages");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0502, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1902,
        L"EssenceDataObjects");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0530050D, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1a02,
        L"ControlPointValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x1002, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1a03,
        L"ControlPointTime");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300508, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1a04,
        L"EditHint");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01011503, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1b01,
        L"DefinitionObjectIdentification");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070102, 0x0301, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1b02,
        L"DefinitionObjectName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020301, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1b03,
        L"DefinitionObjectDescription");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300509, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1e01,
        L"OperationDataDefinition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300503, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1e02,
        L"IsTimeWarp");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0401, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1e03,
        L"DegradeTo");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0530050A, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1e06,
        L"OperationCategory");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300504, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1e07,
        L"OperationInputCount");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300505, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1e08,
        L"Bypass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0302, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1e09,
        L"OperationParametersDefined");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0106, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1f01,
        L"ParameterType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0530050B, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1f03,
        L"ParameterDisplayUnits");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200901, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2203,
        L"PluginCategory");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030301, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2204,
        L"PluginVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030301, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2205,
        L"PluginVersionString");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x010A0101, 0x0101, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2206,
        L"DeviceManufacturerName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x020B, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2207,
        L"ManufacturerInfo");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x010A0101, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2208,
        L"ManufacturerID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200902, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2209,
        L"PluginPlatform");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200903, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220a,
        L"MinPlatformVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200904, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220b,
        L"MaxPlatformVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200905, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220c,
        L"Engine");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200906, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220d,
        L"MinEngineVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200907, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220e,
        L"MaxEngineVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200908, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220f,
        L"PluginAPI");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200909, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2210,
        L"MinPluginAPI");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090A, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2211,
        L"MaxPluginAPI");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090B, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2212,
        L"SoftwareOnly");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090C, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2213,
        L"Accelerator");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090D, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2214,
        L"PluginLocators");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090E, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2215,
        L"Authentication");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090F, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2216,
        L"ImplementedClass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0107, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2301,
        L"FileDescriptorClass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0301, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2302,
        L"CodecDataDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x2401,
        L"EssenceIsIdentified");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0503, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2603,
        L"OperationDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0504, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2604,
        L"ParameterDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0505, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2605,
        L"DataDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0506, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2606,
        L"PluginDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0507, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2607,
        L"CodecDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0508, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2608,
        L"ContainerDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0509, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2609,
        L"InterpolationDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x050A, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x260a,
        L"KLVDataDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x050B, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x260b,
        L"TaggedValueDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010106, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2701,
        L"LinkedPackageID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04070200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2702,
        L"EssenceStream");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010102, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2b01,
        L"SampleIndex");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0603, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2f01,
        L"Locators");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0610, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x09),
        0x2f02,
        L"SubDescriptors");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04060101, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3001,
        L"SampleRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04060102, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3002,
        L"EssenceLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0102, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3004,
        L"ContainerFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0103, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3005,
        L"Codec");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3006,
        L"LinkedTrackID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030302, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3101,
        L"AIFCSummary");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010601, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3201,
        L"PictureCompression");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010502, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3202,
        L"StoredHeight");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010502, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3203,
        L"StoredWidth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3204,
        L"SampledHeight");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3205,
        L"SampledWidth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0900, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3206,
        L"SampledXOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0A00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3207,
        L"SampledYOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0B00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3208,
        L"DisplayHeight");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0C00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3209,
        L"DisplayWidth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0D00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x320a,
        L"DisplayXOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0E00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x320b,
        L"DisplayYOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010301, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x320c,
        L"FrameLayout");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x320d,
        L"VideoLineMap");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010101, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x320e,
        L"ImageAspectRatio");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200102, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x320f,
        L"AlphaTransparency");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010201, 0x0101, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3210,
        L"TransferCharacteristic");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010201, 0x0106, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x09),
        0x3219,
        L"ColorPrimaries");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010201, 0x0103, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x321a,
        L"CodingEquations");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04180101, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3211,
        L"ImageAlignmentFactor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010301, 0x0600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3212,
        L"FieldDominance");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04180102, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3213,
        L"ImageStartOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04180103, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3214,
        L"ImageEndOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04050113, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3215,
        L"SignalStandard");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3216,
        L"StoredF2Offset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3217,
        L"DisplayF2Offset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0900, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3218,
        L"ActiveFormatDescriptor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0A00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3301,
        L"ComponentDepth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3302,
        L"HorizontalSubsampling");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3303,
        L"ColorSiting");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3304,
        L"BlackRefLevel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3305,
        L"WhiteRefLevel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3306,
        L"ColorRange");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04180104, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3307,
        L"PaddingBits");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x1000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3308,
        L"VerticalSubsampling");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3309,
        L"AlphaSampleDepth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0A00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x330b,
        L"ReversedByteOrder");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3401,
        L"PixelLayout");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3403,
        L"Palette");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0900, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3404,
        L"PaletteLayout");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010404, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3405,
        L"ScanningDirection");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0B00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3406,
        L"ComponentMaxRef");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0C00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3407,
        L"ComponentMinRef");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0D00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3408,
        L"AlphaMaxRef");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0E00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3409,
        L"AlphaMinRef");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05020103, 0x0101, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3701,
        L"IsUniform");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06080201, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3702,
        L"IsContiguous");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3703,
        L"LeadingLines");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3704,
        L"TrailingLines");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05020103, 0x0102, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3705,
        L"JPEGTableID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030302, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3706,
        L"TIFFSummary");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030302, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3801,
        L"WAVESummary");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0108, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3901,
        L"FilmFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010802, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3902,
        L"FrameRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0103, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3903,
        L"PerforationsPerFrame");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0203, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3904,
        L"FilmAspectRatio");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0106, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3905,
        L"FilmStockManufacturer");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0105, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3906,
        L"FilmStockKind");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0104, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3907,
        L"FilmFormatName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0107, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3908,
        L"FilmBatchNumber");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0101, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3a01,
        L"TapeFormFactor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010401, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3a02,
        L"VideoSignal");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0D010101, 0x0101, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3a03,
        L"TapeFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3a04,
        L"TapeCapacity");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0401, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3a05,
        L"TapeManufacturer");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3a06,
        L"TapeFormulation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0601, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3a07,
        L"TapeBatchNumber");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0501, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3a08,
        L"TapeStock");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3b01,
        L"ByteOrder");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020110, 0x0204, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3b02,
        L"FileLastModified");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3b03,
        L"ContentStorageObject");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0202, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3b04,
        L"Dictionaries");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3b05,
        L"FormatVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0604, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3b06,
        L"IdentificationList");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3b07,
        L"ObjectModelVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01020203, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3b09,
        L"OperationalPattern");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01020210, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3b0a,
        L"EssenceContainers");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01020210, 0x0202, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3b0b,
        L"DescriptiveSchemes");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c01,
        L"ApplicationSupplierName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0301, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c02,
        L"ApplicationName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c03,
        L"ApplicationVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0501, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c04,
        L"ApplicationVersionString");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c05,
        L"ApplicationProductID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020110, 0x0203, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c06,
        L"FileModificationDate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0A00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c07,
        L"ToolkitVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0601, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c08,
        L"ApplicationPlatform");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3c09,
        L"GenerationID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01020101, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x4001,
        L"URL");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01040102, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4101,
        L"LocationName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01011510, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x4401,
        L"PackageID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01030302, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x4402,
        L"PackageName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0605, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4403,
        L"PackageTracks");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020110, 0x0205, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4404,
        L"PackageLastModified");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020110, 0x0103, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4405,
        L"CreationTime");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020102, 0x0C00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4406,
        L"PackageUserComments");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4407,
        L"PackageKLVData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4409,
        L"PackageAttributes");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05010108, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4408,
        L"PackageUsage");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020201, 0x0105, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4501,
        L"DefaultFadeLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300201, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x4502,
        L"DefaultFadeType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300403, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4503,
        L"DefaultFadeEditUnit");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x010A, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x4504,
        L"CompositionRendering");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0203, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4701,
        L"EssenceDescription");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070101, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4801,
        L"TrackID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070102, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4802,
        L"TrackName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0204, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4803,
        L"TrackSegment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01040103, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4804,
        L"EssenceTrackNumber");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300402, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4901,
        L"EventTrackEditRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x010B, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x4902,
        L"EventTrackOrigin");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300405, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4b01,
        L"EditRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0103, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4b02,
        L"Origin");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x010C, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4b03,
        L"MarkIn");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0203, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4b04,
        L"MarkOut");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x010D, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4b05,
        L"UserPosition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0104, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4c01,
        L"ParameterDefinitionReference");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300507, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4d01,
        L"Value");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0105, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4e01,
        L"Interpolation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0606, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4e02,
        L"PointList");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020102, 0x0901, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x5001,
        L"Tag");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020102, 0x0A01, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x5003,
        L"IndirectValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x5101,
        L"KLVDataValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070105, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
        0x6102,
        L"DescribedTrackIDs");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x020C, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x6101,
        L"DescriptiveFrameworkObject");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0101, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d03,
        L"AudioSampleRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
        0x3d02,
        L"Locked");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020101, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3d04,
        L"AudioReferenceLevel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020101, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3d05,
        L"ElectrospatialFormulation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020101, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d07,
        L"ChannelCount");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020303, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
        0x3d01,
        L"QuantizationBits");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020701, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d0c,
        L"DialNorm");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020402, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3d06,
        L"SoundCompression");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04030302, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03),
        0x3e01,
        L"DataEssenceCoding");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x060B, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
        0x3f01,
        L"FileDescriptors");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070106, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x6103,
        L"DescriptiveClipDescribedTrackIDs");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020501, 0x0600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d0d,
        L"Emphasis");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020302, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d0f,
        L"BlockStartOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020501, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d08,
        L"AuxBitsMode");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020501, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d10,
        L"ChannelStatusMode");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020501, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d11,
        L"FixedChannelStatusData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020501, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d12,
        L"UserDataMode");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020501, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d13,
        L"FixedUserData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020302, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d0a,
        L"BlockAlign");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020302, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d0b,
        L"SequenceOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020303, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d09,
        L"AverageBytesPerSecond");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020101, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x3d32,
        L"ChannelAssignment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d29,
        L"PeakEnvelopeVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d2a,
        L"PeakEnvelopeFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d2b,
        L"PointsPerPeakValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0900, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d2c,
        L"PeakEnvelopeBlockSize");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0A00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d2d,
        L"PeakChannels");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0B00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d2e,
        L"PeakFrames");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0C00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d2f,
        L"PeakOfPeaksPosition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0D00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d30,
        L"PeakEnvelopeTimestamp");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0E00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d31,
        L"PeakEnvelopeData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0109, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4d12,
        L"KLVDataType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04090201, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4e11,
        L"MIMEType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04090300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x4e12,
        L"CharSet");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04060802, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x4f01,
        L"ChunkID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04060903, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x4f02,
        L"ChunkLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04070400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x4f03,
        L"ChunkData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020302, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d15,
        L"QltyFileSecurityReport");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020302, 0x0600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d16,
        L"QltyFileSecurityWave");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020502, 0x0101, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d21,
        L"BextCodingHistory");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020502, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d22,
        L"QltyBasicData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020502, 0x0301, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d23,
        L"QltyStartOfModulation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020502, 0x0401, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d24,
        L"QltyQualityEvent");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020502, 0x0501, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d25,
        L"QltyEndOfModulation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020502, 0x0601, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d26,
        L"QltyQualityParameter");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020502, 0x0701, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d27,
        L"QltyOperatorComment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020502, 0x0801, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3d28,
        L"QltyCueSheet");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x060F, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3d33,
        L"UnknownBWFChunks");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x0102, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"SingleSequence");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x0103, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"ConstantBPictureCount");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x0104, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"CodedContentScanning");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x0105, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"LowDelay");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x0106, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"ClosedGOP");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x0107, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"IdenticalGOP");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x0108, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"MaxGOP");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x0109, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"MaxBPictureCount");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x010B, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"BitRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010602, 0x010A, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x0000,
        L"ProfileAndLevel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0008,
        L"ParentClass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0009,
        L"Properties");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000a,
        L"IsConcrete");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000b,
        L"PropertyType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010202, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000c,
        L"IsOptional");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000d,
        L"LocalIdentification");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000e,
        L"IsUniqueIdentifier");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000f,
        L"Size");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0010,
        L"IsSigned");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0900, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0011,
        L"ReferencedType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0A00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0012,
        L"WeakReferencedType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0B00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0013,
        L"TargetSet");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0B00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0014,
        L"ElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0400, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0015,
        L"ElementNames");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0500, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0016,
        L"ElementValues");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0C00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0017,
        L"FixedArrayElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0018,
        L"ElementCount");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0D00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0019,
        L"VariableArrayElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0E00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001a,
        L"SetElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0F00, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001b,
        L"StringElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x1100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001c,
        L"MemberTypes");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0600, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001d,
        L"MemberNames");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x1200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001e,
        L"RenamedType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001f,
        L"ExtendibleEnumerationElementNames");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0020,
        L"ExtendibleEnumerationElementValues");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x1300, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0005,
        L"MetaDefinitionIdentification");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020401, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0006,
        L"MetaDefinitionName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x1401, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0007,
        L"MetaDefinitionDescription");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0700, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0003,
        L"ClassDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0800, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0004,
        L"TypeDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0d010301, 0x0101, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0001,
        L"RootMetaDictionary");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0d010301, 0x0102, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0002,
        L"RootHeader");
    
    
    //
    // Types
    //
    
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt8");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt16");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt32");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt64");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010500, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int8");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int16");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010700, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int32");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010800, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int64");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01012001, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PositionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01012002, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"LengthType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01012003, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"JPEGTableIDType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01012300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PhaseFrameType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01030100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AUID");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01030200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PackageIDType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01040100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Boolean");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01100100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Character");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01100200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UTF16String");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010101, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ProductReleaseType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010102, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TapeFormatType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010103, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"VideoSignalType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010104, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TapeCaseType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010105, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ColorSitingType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010106, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EditHintType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010107, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"FadeType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010108, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"LayoutType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010109, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TCSource");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010A, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PulldownDirectionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010B, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PulldownKindType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010C, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EdgeType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010D, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"FilmType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010E, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"RGBAComponentKind");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010F, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ReferenceType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010120, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AlphaTransparencyType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010121, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"FieldNumber");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010122, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ElectroSpatialFormulation");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010123, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EmphasisType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010124, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AuxBitsModeType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010125, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ChannelStatusModeType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010126, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UserDataModeType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010127, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SignalStandardType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010128, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ScanningDirectionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201012A, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContentScanningType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201012B, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TitleAlignmentType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020101, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationCategoryType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020102, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TransferCharacteristicType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020103, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginCategoryType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020104, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UsageType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020105, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ColorPrimariesType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020106, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"CodingEquationsType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Rational");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ProductVersionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"VersionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"RGBAComponent");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010500, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DateStruct");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TimeStruct");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010700, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TimeStamp");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt8Array");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt8Array12");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int32Array");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int64Array");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010500, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UTF16StringArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AUIDArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010700, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PositionArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010800, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt8Array8");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010900, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt32Array");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010A00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ChannelStatusModeArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010B00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UserDataModeArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04020100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"RGBALayout");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04030100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AUIDSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04030200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt32Set");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04100100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataValue");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04100200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Stream");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04100300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Indirect");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04100400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Opaque");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ClassDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContainerDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010500, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"InterpolationDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PackageWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010700, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010800, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010900, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TypeDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010A00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010B00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"CodecDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010C00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PropertyDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContentStorageStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DictionaryStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EssenceDescriptorStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"NetworkLocatorStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020500, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationGroupStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SegmentStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020700, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SourceClipStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020800, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SourceReferenceStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020900, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ClassDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020A00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"CodecDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020B00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ComponentStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020C00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContainerDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020D00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ControlPointStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020E00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020F00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EssenceDataStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021000, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"IdentificationStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"InterpolationDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"LocatorStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PackageStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TrackStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021500, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021700, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021800, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021900, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PropertyDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021A00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TaggedValueStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021B00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TypeDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021C00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"KLVDataStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021D00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"FileDescriptorStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021E00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"RIFFChunkStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021F00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DescriptiveFrameworkStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05022000, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"KLVDataDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05022100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TaggedValueDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05022200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DescriptiveObjectStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05022600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SubDescriptorStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05030D00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionWeakReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05030E00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterDefinitionWeakReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05030F00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginDefinitionWeakReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05031000, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PropertyDefinitionWeakReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05040100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationDefinitionWeakReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05040200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TypeDefinitionWeakReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05040300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionWeakReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ClassDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"CodecDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContainerDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050500, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EssenceDataStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"InterpolationDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050700, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PackageStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050800, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050900, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050A00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050B00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PropertyDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050C00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TypeDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050D00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"KLVDataDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050E00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TaggedValueDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050F00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DescriptiveObjectStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060100, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ComponentStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060200, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ControlPointStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060300, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"IdentificationStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060400, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"LocatorStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060500, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TrackStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060600, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SegmentStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060700, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SourceReferenceStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060800, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TaggedValueStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060900, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"KLVDataStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060A00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060B00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"FileDescriptorStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060C00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"RIFFChunkStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060D00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DescriptiveObjectStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060E00, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SubDescriptorStrongReferenceVector");
    
    
    //
    // Definition symbols
    //
    
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0C3BEA40, 0xFC05, 0x11D2, 0x8A, 0x29, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_VideoDissolve");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0C3BEA44, 0xFC05, 0x11D2, 0x8A, 0x29, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_SMPTEVideoWipe");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9D2EA890, 0x0968, 0x11D3, 0x8A, 0x38, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_VideoSpeedControl");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9D2EA891, 0x0968, 0x11D3, 0x8A, 0x38, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_VideoRepeat");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xF1DB0F32, 0x8D64, 0x11D3, 0x80, 0xDF, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"OperationDef_Flip");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xF1DB0F34, 0x8D64, 0x11D3, 0x80, 0xDF, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"OperationDef_Flop");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xF1DB0F33, 0x8D64, 0x11D3, 0x80, 0xDF, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"OperationDef_FlipFlop");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x86F5711E, 0xEE72, 0x450C, 0xA1, 0x18, 0x17, 0xCF, 0x3B, 0x17, 0x5D, 0xFF),
        L"OperationDef_VideoPosition");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xF5826680, 0x26C5, 0x4149, 0x85, 0x54, 0x43, 0xD3, 0xC7, 0xA3, 0xBC, 0x09),
        L"OperationDef_VideoCrop");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x2E0A119D, 0xE6F7, 0x4BEE, 0xB5, 0xDC, 0x6D, 0xD4, 0x29, 0x88, 0x68, 0x7E),
        L"OperationDef_VideoScale");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xF2CA330D, 0x8D45, 0x4DB4, 0xB1, 0xB5, 0x13, 0x6A, 0xB0, 0x55, 0x58, 0x6F),
        L"OperationDef_VideoRotate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x21D5C51A, 0x8ACB, 0x46D5, 0x93, 0x92, 0x5C, 0xAE, 0x64, 0x0C, 0x88, 0x36),
        L"OperationDef_VideoCornerPinning");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x14DB900E, 0xD537, 0x49F6, 0x88, 0x9B, 0x01, 0x25, 0x68, 0xFC, 0xC2, 0x34),
        L"OperationDef_VideoAlphaWithinVideoKey");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xE599CB0F, 0xBA5F, 0x4192, 0x93, 0x56, 0x51, 0xEB, 0x19, 0xC0, 0x85, 0x89),
        L"OperationDef_VideoSeparateAlphaKey");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x38FF7903, 0x69E5, 0x476B, 0xBE, 0x5A, 0xEA, 0xFC, 0x20, 0x00, 0xF0, 0x11),
        L"OperationDef_VideoLuminanceKey");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x30A315C2, 0x71E5, 0x4E82, 0xA4, 0xEF, 0x05, 0x13, 0xEE, 0x05, 0x6B, 0x65),
        L"OperationDef_VideoChromaKey");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9D2EA894, 0x0968, 0x11D3, 0x8A, 0x38, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_MonoAudioGain");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9D2EA893, 0x0968, 0x11D3, 0x8A, 0x38, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_MonoAudioPan");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0C3BEA41, 0xFC05, 0x11D2, 0x8A, 0x29, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_MonoAudioDissolve");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x2311BD90, 0xB5DA, 0x4285, 0xAA, 0x3A, 0x85, 0x52, 0x84, 0x87, 0x79, 0xB3),
        L"OperationDef_TwoParameterMonoAudioDissolve");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9BB90DFD, 0x2AAD, 0x49AF, 0xB0, 0x9C, 0x8B, 0xA6, 0xCD, 0x52, 0x81, 0xD1),
        L"OperationDef_VideoOpacity");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x2C50831C, 0x572E, 0x4042, 0xB1, 0xDD, 0x55, 0xED, 0x0B, 0x7C, 0x49, 0xDF),
        L"OperationDef_VideoTitle");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5ABA98F8, 0xF389, 0x471F, 0x8F, 0xEE, 0xDF, 0xDE, 0x7E, 0xC7, 0xF9, 0xBB),
        L"OperationDef_VideoColor");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x1575E350, 0xFCA3, 0x11D2, 0x8A, 0x2A, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_Unknown");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0C3BEA43, 0xFC05, 0x11D2, 0x8A, 0x29, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_VideoFadeToBlack");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0A3C75E0, 0xFD82, 0x11D2, 0x8A, 0x2B, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_PictureWithMate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9D2EA892, 0x0968, 0x11D3, 0x8A, 0x38, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_VideoFrameToMask");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0C3BEA42, 0xFC05, 0x11D2, 0x8A, 0x29, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_StereoAudioDissolve");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9D2EA895, 0x0968, 0x11D3, 0x8A, 0x38, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_StereoAudioGain");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8D896AD0, 0x2261, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"OperationDef_MonoAudioMixdown");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xE4962320, 0x2267, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_Level");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xE4962323, 0x2267, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTEWipeNumber");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA0, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTEReverse");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x72559A80, 0x24D7, 0x11D3, 0x8A, 0x50, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SpeedRatio");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xC573A510, 0x071A, 0x454F, 0xB6, 0x17, 0xAD, 0x6A, 0xE6, 0x90, 0x54, 0xC2),
        L"ParameterDef_PositionOffsetX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x82E27478, 0x1336, 0x4EA3, 0xBC, 0xB9, 0x6B, 0x8F, 0x17, 0x86, 0x4C, 0x42),
        L"ParameterDef_PositionOffsetY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xD47B3377, 0x318C, 0x4657, 0xA9, 0xD8, 0x75, 0x81, 0x1B, 0x6D, 0xC3, 0xD1),
        L"ParameterDef_CropLeft");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5ECC9DD5, 0x21C1, 0x462B, 0x9F, 0xEC, 0xC2, 0xBD, 0x85, 0xF1, 0x40, 0x33),
        L"ParameterDef_CropRight");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8170A539, 0x9B55, 0x4051, 0x9D, 0x4E, 0x46, 0x59, 0x8D, 0x01, 0xB9, 0x14),
        L"ParameterDef_CropTop");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x154BA82B, 0x990A, 0x4C80, 0x91, 0x01, 0x30, 0x37, 0xE2, 0x88, 0x39, 0xA1),
        L"ParameterDef_CropBottom");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8D568129, 0x847E, 0x11D5, 0x93, 0x5A, 0x50, 0xF8, 0x57, 0xC1, 0x00, 0x00),
        L"ParameterDef_ScaleX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8D56812A, 0x847E, 0x11D5, 0x93, 0x5A, 0x50, 0xF8, 0x57, 0xC1, 0x00, 0x00),
        L"ParameterDef_ScaleY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x062CFBD8, 0xF4B1, 0x4A50, 0xB9, 0x44, 0xF3, 0x9E, 0x2F, 0xC7, 0x3C, 0x17),
        L"ParameterDef_Rotation");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x72A3B4A2, 0x873D, 0x4733, 0x90, 0x52, 0x9F, 0x83, 0xA7, 0x06, 0xCA, 0x5B),
        L"ParameterDef_PinTopLeftX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x29E4D78F, 0xA502, 0x4EBB, 0x8C, 0x07, 0xED, 0x5A, 0x03, 0x20, 0xC1, 0xB0),
        L"ParameterDef_PinTopLeftY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xA95296C0, 0x1ED9, 0x4925, 0x84, 0x81, 0x20, 0x96, 0xC7, 0x2E, 0x81, 0x8D),
        L"ParameterDef_PinTopRightX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xCE1757AE, 0x7A0B, 0x45D9, 0xB3, 0xF3, 0x36, 0x86, 0xAD, 0xFF, 0x1E, 0x2D),
        L"ParameterDef_PinTopRightY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x08B2BC81, 0x9B1B, 0x4C01, 0xBA, 0x73, 0xBB, 0xA3, 0x55, 0x4E, 0xD0, 0x29),
        L"ParameterDef_PinBottomLeftX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xC163F2FF, 0xCD83, 0x4655, 0x82, 0x6E, 0x37, 0x24, 0xAB, 0x7F, 0xA0, 0x92),
        L"ParameterDef_PinBottomLeftY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x53BC5884, 0x897F, 0x479E, 0xB8, 0x33, 0x19, 0x1F, 0x86, 0x92, 0x10, 0x0D),
        L"ParameterDef_PinBottomRightX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x812FB15B, 0x0B95, 0x4406, 0x87, 0x8D, 0xEF, 0xAA, 0x1C, 0xFF, 0xC1, 0x29),
        L"ParameterDef_PinBottomRightY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xA2667F65, 0x65D8, 0x4ABF, 0xA1, 0x79, 0x0B, 0x9B, 0x93, 0x41, 0x39, 0x49),
        L"ParameterDef_AlphaKeyInvertAlpha");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x21ED5B0F, 0xB7A0, 0x43BC, 0xB7, 0x79, 0xC4, 0x7F, 0x85, 0xBF, 0x6C, 0x4D),
        L"ParameterDef_LumKeyLevel");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xCBD39B25, 0x3ECE, 0x441E, 0xBA, 0x2C, 0xDA, 0x47, 0x3A, 0xB5, 0xCC, 0x7C),
        L"ParameterDef_LumKeyClip");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xE4962321, 0x2267, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_Amplitude");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xE4962322, 0x2267, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_Pan");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9E610007, 0x1BE2, 0x41E1, 0xBB, 0x11, 0xC9, 0x5D, 0xE9, 0x96, 0x4D, 0x03),
        L"ParameterDef_OutgoingLevel");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x48CEA642, 0xA8F9, 0x455B, 0x82, 0xB3, 0x86, 0xC8, 0x14, 0xB7, 0x97, 0xC7),
        L"ParameterDef_IncomingLevel");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xCB7C0EC4, 0xF45F, 0x4EE6, 0xAE, 0xF0, 0xC6, 0x3D, 0xDB, 0x13, 0x49, 0x24),
        L"ParameterDef_OpacityLevel");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x7B92827B, 0x5AE3, 0x465E, 0xB5, 0xF9, 0x5E, 0xE2, 0x1B, 0x07, 0x08, 0x59),
        L"ParameterDef_TitleText");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xE8EB7F50, 0x602F, 0x4A2F, 0x8F, 0xB2, 0x86, 0xC8, 0x82, 0x6C, 0xCF, 0x24),
        L"ParameterDef_TitleFontName");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01C55287, 0x31B3, 0x4F8F, 0xBB, 0x87, 0xC9, 0x2F, 0x06, 0xEB, 0x7F, 0x5A),
        L"ParameterDef_TitleFontSize");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xDFE86F24, 0x8A71, 0x4DC5, 0x83, 0xA2, 0x98, 0x8F, 0x58, 0x3A, 0xF7, 0x11),
        L"ParameterDef_TitleFontColorR");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xF9F41222, 0x36D9, 0x4650, 0xBD, 0x5A, 0xA1, 0x78, 0x66, 0xCF, 0x86, 0xB9),
        L"ParameterDef_TitleFontColorG");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xF5BA87FA, 0xCF72, 0x4F37, 0xA7, 0x36, 0xD7, 0x09, 0x6F, 0xCB, 0x06, 0xF1),
        L"ParameterDef_TitleFontColorB");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x47C1733F, 0x6AFB, 0x4168, 0x9B, 0x6D, 0x47, 0x6A, 0xDF, 0xBA, 0xE7, 0xAB),
        L"ParameterDef_TitleAlignment");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8B5732C0, 0xBE8E, 0x4332, 0xAA, 0x71, 0x5D, 0x86, 0x6A, 0xDD, 0x77, 0x7D),
        L"ParameterDef_TitleBold");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xE4A3C91B, 0xF96A, 0x4DD4, 0x91, 0xD8, 0x1B, 0xA3, 0x20, 0x00, 0xAB, 0x72),
        L"ParameterDef_TitleItalic");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xA25061DA, 0xDB25, 0x402E, 0x89, 0xFF, 0xA6, 0xD0, 0xEF, 0xA3, 0x94, 0x44),
        L"ParameterDef_TitlePositionX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x6151541F, 0x9D3F, 0x4A0E, 0xA3, 0xF9, 0x24, 0xCC, 0x60, 0xEE, 0xA9, 0x69),
        L"ParameterDef_TitlePositionY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xBE2033DA, 0x723B, 0x4146, 0xAC, 0xE0, 0x32, 0x99, 0xE0, 0xFF, 0x34, 0x2E),
        L"ParameterDef_ColorSlopeR");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x7CA8E01B, 0xC6D8, 0x4B3F, 0xB2, 0x51, 0x28, 0xA5, 0x3E, 0x5B, 0x95, 0x8F),
        L"ParameterDef_ColorSlopeG");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x1AEB007B, 0x3CD5, 0x4814, 0x87, 0xB5, 0xCB, 0xD6, 0xA3, 0xCD, 0xFE, 0x8D),
        L"ParameterDef_ColorSlopeB");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4D1E65E0, 0x85FC, 0x4BB9, 0xA2, 0x64, 0x13, 0xCF, 0x32, 0x0A, 0x85, 0x39),
        L"ParameterDef_ColorOffsetR");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x76F783E4, 0x0BBD, 0x41D7, 0xB0, 0x1E, 0xF4, 0x18, 0xC1, 0x60, 0x2A, 0x6F),
        L"ParameterDef_ColorOffsetG");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x57110628, 0x522D, 0x4B48, 0x8A, 0x28, 0x75, 0x47, 0x7C, 0xED, 0x98, 0x4D),
        L"ParameterDef_ColorOffsetB");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xC2D79C3A, 0x9263, 0x40D9, 0x82, 0x7D, 0x95, 0x3A, 0xC6, 0xB8, 0x88, 0x13),
        L"ParameterDef_ColorPowerR");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x524D52E6, 0x86A3, 0x4F41, 0x86, 0x4B, 0xFB, 0x53, 0xB1, 0x5B, 0x1D, 0x5D),
        L"ParameterDef_ColorPowerG");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5F0CC7DC, 0x907D, 0x4153, 0xBF, 0x00, 0x1F, 0x3C, 0xDF, 0x3C, 0x05, 0xBB),
        L"ParameterDef_ColorPowerB");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0B135705, 0x3312, 0x4D03, 0xBA, 0x89, 0xBE, 0x9E, 0xF4, 0x5E, 0x54, 0x70),
        L"ParameterDef_ColorSaturation");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xF3B9466A, 0x2579, 0x4168, 0xBE, 0xB5, 0x66, 0xB9, 0x96, 0x91, 0x9A, 0x3F),
        L"ParameterDef_ColorCorrectionDescription");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xB0124DBE, 0x7F97, 0x443C, 0xAE, 0x39, 0xC4, 0x9C, 0x1C, 0x53, 0xD7, 0x28),
        L"ParameterDef_ColorInputDescription");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5A9DFC6F, 0x611F, 0x4DB8, 0x8E, 0xFF, 0x3B, 0x9C, 0xDB, 0x6E, 0x12, 0x20),
        L"ParameterDef_ColorViewingDescription");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA1, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTESoft");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA2, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTEBorder");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA3, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTEPosition");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA4, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTEModulator");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA5, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTEShadow");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA6, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTETumble");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA7, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTESpotlight");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA8, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTEReplicationH");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BA9, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTEReplicationV");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9C894BAA, 0x2277, 0x11D3, 0x8A, 0x4C, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_SMPTECheckerboard");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5F1C2560, 0x2415, 0x11D3, 0x8A, 0x4F, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"ParameterDef_PhaseOffset");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030202, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Picture");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x6F3C8CE1, 0x6CEF, 0x11D2, 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"DataDef_LegacyPicture");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x05CBA731, 0x1DAA, 0x11D3, 0x80, 0xAD, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"DataDef_Matte");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x05CBA732, 0x1DAA, 0x11D3, 0x80, 0xAD, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"DataDef_PictureWithMatte");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030202, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Sound");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x78E1EBE1, 0x6CEF, 0x11D2, 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"DataDef_LegacySound");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030201, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Timecode");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x7F275E81, 0x77E5, 0x11D2, 0x80, 0x7F, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"DataDef_LegacyTimecode");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xD2BB2AF0, 0xD234, 0x11D2, 0x89, 0xEE, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12),
        L"DataDef_Edgecode");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030201, 0x1000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_DescriptiveMetadata");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030203, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x05),
        L"DataDef_Auxiliary");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x851419D0, 0x2E4F, 0x11D3, 0x8A, 0x5B, 0x00, 0x50, 0x04, 0x0E, 0xF7, 0xD2),
        L"DataDef_Unknown");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4313B572, 0xD8BA, 0x11D2, 0x80, 0x9B, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"ContainerDef_External");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4B1C1A46, 0x03F2, 0x11D4, 0x80, 0xFB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"ContainerDef_OMF");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4313B571, 0xD8BA, 0x11D2, 0x80, 0x9B, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"ContainerDef_AAF");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x42464141, 0x000D, 0x4D4F, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0xFF),
        L"ContainerDef_AAFMSS");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4B464141, 0x000D, 0x4D4F, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0xFF),
        L"ContainerDef_AAFKLV");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x58464141, 0x000D, 0x4D4F, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0xFF),
        L"ContainerDef_AAFXML");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0101, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_DefinedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0102, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_ExtendedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x017F, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x02),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_50Mbps_PictureOnly");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0201, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_DefinedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0202, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_ExtendedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x027F, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x02),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_50Mbps_PictureOnly");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0301, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_DefinedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0302, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_ExtendedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x037F, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x02),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_40Mbps_PictureOnly");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0401, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_DefinedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0402, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_ExtendedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x047F, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x02),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_40Mbps_PictureOnly");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0501, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_DefinedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0502, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_ExtendedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x057F, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x02),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_625x50I_30Mbps_PictureOnly");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0601, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_DefinedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x0602, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_ExtendedTemplate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0201, 0x067F, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x02),
        L"ContainerDef_MXFGC_Framewrapped_SMPTE_D10_525x5994I_30Mbps_PictureOnly");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x0101, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x0102, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x0201, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x0202, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x0301, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_IECDV_525x5994I_25Mbps_SMPTE322M");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x0302, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_IECDV_525x5994I_25Mbps_SMPTE322M");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x0401, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_IECDV_625x50I_25Mbps_SMPTE322M");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x0402, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_IECDV_625x50I_25Mbps_SMPTE322M");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x3F01, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_IECDV_UndefinedSource_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x3F02, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_IECDV_UndefinedSource_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x4001, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x4002, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x4101, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_625x50I_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x4102, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_25Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x5001, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_525x5994I_50Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x5002, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_525x5994I_50Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x5101, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_625x50I_50Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x5102, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_625x50I_50Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x6001, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_1080x5994I_100Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x6002, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_1080x5994I_100Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x6101, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_1080x50I_100Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x6102, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_1080x50I_100Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x6201, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_720x5994P_100Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x6202, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_720x5994P_100Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x6301, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_720x50P_100Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x6302, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_720x50P_100Mbps");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x7F01, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_DVbased_UndefinedSource");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0202, 0x7F02, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_DVbased_UndefinedSource");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0204, 0x6001, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x02),
        L"ContainerDef_MXFGC_Framewrapped_MPEGES_VideoStream0_SID");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0204, 0x6107, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x02),
        L"ContainerDef_MXFGC_CustomClosedGOPwrapped_MPEGES_VideoStream1_SID");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x0101, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994I_720_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x0102, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994I_720_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x0103, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994I_720_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x0105, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_Uncompressed_625x50I_720_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x0106, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50I_720_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x0107, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Linewrapped_Uncompressed_625x50I_720_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x0119, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_Uncompressed_525x5994P_960_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x011A, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_Uncompressed_525x5994P_960_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x011B, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Linewrapped_Uncompressed_525x5994P_960_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x011D, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_Uncompressed_625x50P_960_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x011E, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_Uncompressed_625x50P_960_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0205, 0x011F, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Linewrapped_Uncompressed_625x50P_960_422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0206, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_Broadcast_Wave_audio_data");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0206, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_Broadcast_Wave_audio_data");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0206, 0x0300, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Framewrapped_AES3_audio_data");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0206, 0x0400, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Clipwrapped_AES3_audio_data");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x020A, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x03),
        L"ContainerDef_MXFGC_Framewrapped_Alaw_Audio");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x020A, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x03),
        L"ContainerDef_MXFGC_Clipwrapped_Alaw_Audio");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x020A, 0x0300, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x03),
        L"ContainerDef_MXFGC_Customwrapped_Alaw_Audio");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0210, 0x6002, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x0A),
        L"ContainerDef_MXFGC_Clipwrapped_AVCbytestream_VideoStream0_SID");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0211, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x0A),
        L"ContainerDef_MXFGC_Framewrapped_VC3");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0211, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x0A),
        L"ContainerDef_MXFGC_Clipwrapped_VC3");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0212, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x0A),
        L"ContainerDef_MXFGC_Framewrapped_VC1");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x0212, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x0A),
        L"ContainerDef_MXFGC_Clipwrapped_VC1");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D010301, 0x027F, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x03),
        L"ContainerDef_MXFGC_Generic_Essence_Multiple_Mappings");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D011301, 0x0101, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x06),
        L"ContainerDef_RIFFWAVE");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D011301, 0x0102, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x07),
        L"ContainerDef_JFIF");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D011301, 0x0104, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x06),
        L"ContainerDef_AIFFAIFC");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0101, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_220X_1080p");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0102, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_145_1080p");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0103, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_220_1080p");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0104, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_36_1080p");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0201, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_220X_1080i");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0202, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_145_1080i");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0203, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_220_1080i");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0204, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_145_1440_1080i");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0301, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_220X_720p");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0302, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_220_720p");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040301, 0x0206, 0x0303, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"ContainerDef_MXFGC_Avid_DNX_145_720p");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5B6C85A3, 0x0EDE, 0x11D3, 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"InterpolationDef_None");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5B6C85A4, 0x0EDE, 0x11D3, 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"InterpolationDef_Linear");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5B6C85A5, 0x0EDE, 0x11D3, 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"InterpolationDef_Constant");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5B6C85A6, 0x0EDE, 0x11D3, 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"InterpolationDef_BSpline");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x15829EC3, 0x1F24, 0x458A, 0x96, 0x0D, 0xC6, 0x5B, 0xB2, 0x3C, 0x2A, 0xA1),
        L"InterpolationDef_Log");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xC09153F7, 0xBD18, 0x4E5A, 0xAD, 0x09, 0xCB, 0xDD, 0x65, 0x4F, 0xA0, 0x01),
        L"InterpolationDef_Power");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D011201, 0x0100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x05),
        L"OPDef_EditProtocol");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0D011201, 0x0200, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x09),
        L"OPDef_Unconstrained");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x3D1DD891, 0xE793, 0x11D2, 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"Platform_Independent");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9FDEF8C1, 0xE847, 0x11D2, 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"Engine_None");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x69C870A1, 0xE793, 0x11D2, 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"PluginAPI_EssenceAccess");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x56905E0B, 0x537D, 0x11D4, 0xA3, 0x6C, 0x00, 0x90, 0x27, 0xDF, 0xCA, 0x6A),
        L"PluginCategory_Codec");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x568FB761, 0x9458, 0x11D2, 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"CodecDef_None");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x90AC17C8, 0xE3E2, 0x4596, 0x9E, 0x9E, 0xA6, 0xDD, 0x1C, 0x70, 0xC8, 0x92),
        L"CodecDef_PCM");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x820F09B1, 0xEB9B, 0x11D2, 0x80, 0x9F, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"CodecDef_WAVE");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4B1C1A45, 0x03F2, 0x11D4, 0x80, 0xFB, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"CodecDef_AIFC");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x18634F8C, 0x3BAB, 0x11D3, 0xBF, 0xD6, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D),
        L"CodecDef_JPEG");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4E84045E, 0x0F29, 0x11D4, 0xA3, 0x59, 0x00, 0x90, 0x27, 0xDF, 0xCA, 0x6A),
        L"CodecDef_CDCI");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4E84045F, 0x0F29, 0x11D4, 0xA3, 0x59, 0x00, 0x90, 0x27, 0xDF, 0xCA, 0x6A),
        L"CodecDef_RGBA");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x6C2A61C2, 0xE7A2, 0x46EE, 0x8D, 0x90, 0x6A, 0x1D, 0x06, 0xE1, 0x5F, 0x41),
        L"CodecDef_VC3");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8EF593F6, 0x9521, 0x4344, 0x9E, 0xDE, 0xB8, 0x4E, 0x8C, 0xFD, 0xC7, 0xDA),
        L"CodecDef_DNxHD");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x1B31F3B1, 0x9450, 0x11D2, 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F),
        L"CodecFlavour_None");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C8A, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_DV_Based_100Mbps_1080x50I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C8B, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_DV_Based_100Mbps_1080x5994I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C8C, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_DV_Based_100Mbps_720x50P");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C8D, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_DV_Based_100Mbps_720x5994P");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C80, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_DV_Based_25Mbps_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C81, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_DV_Based_25Mbps_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C82, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_DV_Based_50Mbps_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C83, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_DV_Based_50Mbps_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C7F, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_IEC_DV_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C7E, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_IEC_DV_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C7D, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_LegacyDV_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C7C, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_LegacyDV_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C84, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_SMPTE_D10_50Mbps_625x50I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C85, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_SMPTE_D10_50Mbps_525x5994I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C86, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_SMPTE_D10_40Mbps_625x50I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C87, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_SMPTE_D10_40Mbps_525x5994I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C88, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_SMPTE_D10_30Mbps_625x50I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xAF4DE587, 0x23D7, 0x4C89, 0xB3, 0x7B, 0xC1, 0xC1, 0x38, 0x70, 0xE7, 0x11),
        L"CodecFlavour_SMPTE_D10_30Mbps_525x5994I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xEFFDB6B4, 0xFE99, 0x4768, 0x88, 0xFE, 0x34, 0x22, 0xA5, 0x76, 0x29, 0x61),
        L"CodecFlavour_VC3_1235");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x21B15F27, 0x2781, 0x4656, 0xAA, 0x1B, 0xDC, 0x5E, 0x63, 0x86, 0x27, 0x38),
        L"CodecFlavour_VC3_1237");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x62F37363, 0xB1D1, 0x4FA0, 0x9F, 0xB7, 0x6E, 0x70, 0x44, 0x37, 0x13, 0x96),
        L"CodecFlavour_VC3_1238");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x1E9B855A, 0x323E, 0x4999, 0xB0, 0xFA, 0x84, 0x44, 0x26, 0x7A, 0x63, 0xA7),
        L"CodecFlavour_VC3_1241");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8B4C29CF, 0xB255, 0x4EF0, 0xBF, 0x79, 0xB5, 0xB6, 0x16, 0x47, 0x92, 0x38),
        L"CodecFlavour_VC3_1242");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xE063FD16, 0x6A70, 0x4128, 0x93, 0x6D, 0xAC, 0x77, 0x6F, 0x26, 0x30, 0xCF),
        L"CodecFlavour_VC3_1243");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xC80D0143, 0xBE86, 0x45FD, 0xAA, 0xCC, 0x7F, 0x61, 0x2B, 0x4B, 0x91, 0x39),
        L"CodecFlavour_VC3_1244");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x47EB10B5, 0x72FA, 0x4DBB, 0x98, 0x01, 0xE0, 0xFE, 0x9A, 0xB8, 0xD9, 0xF0),
        L"CodecFlavour_VC3_1250");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x26CF3984, 0xC716, 0x4315, 0x9D, 0xE7, 0x92, 0x28, 0xB5, 0xC0, 0xF9, 0x22),
        L"CodecFlavour_VC3_1251");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0909CF52, 0x475A, 0x4ABC, 0x9E, 0x13, 0x0D, 0xDB, 0x9D, 0x60, 0xD1, 0x6C),
        L"CodecFlavour_VC3_1252");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x7F5D77DD, 0x5402, 0x45E0, 0x91, 0x28, 0x03, 0x80, 0x16, 0xF5, 0x54, 0x06),
        L"CodecFlavour_VC3_1253");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xA362D3CB, 0xDCEF, 0x4FFB, 0xBB, 0x35, 0xBE, 0x72, 0xA1, 0x65, 0x61, 0xCE),
        L"CodecFlavour_VC3_1254");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xEDB35383, 0x6D30, 0x11D3, 0xA0, 0x36, 0x00, 0x60, 0x94, 0xEB, 0x75, 0xCB),
        L"CompressionDef_AAF_CMPR_FULL_JPEG");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xEDB35391, 0x6D30, 0x11D3, 0xA0, 0x36, 0x00, 0x60, 0x94, 0xEB, 0x75, 0xCB),
        L"CompressionDef_AAF_CMPR_AUNC422");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xEDB35390, 0x6D30, 0x11D3, 0xA0, 0x36, 0x00, 0x60, 0x94, 0xEB, 0x75, 0xCB),
        L"CompressionDef_LegacyDV");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0102, 0x0101, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_SMPTE_D10_50Mbps_625x50I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0102, 0x0102, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_SMPTE_D10_50Mbps_525x5994I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0102, 0x0103, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_SMPTE_D10_40Mbps_625x50I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0102, 0x0104, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_SMPTE_D10_40Mbps_525x5994I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0102, 0x0105, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_SMPTE_D10_30Mbps_625x50I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0102, 0x0106, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_SMPTE_D10_30Mbps_525x5994I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0201, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_IEC_DV_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0201, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_IEC_DV_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0202, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_DV_Based_25Mbps_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0202, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_DV_Based_25Mbps_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0202, 0x0300, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_DV_Based_50Mbps_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0202, 0x0400, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_DV_Based_50Mbps_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0202, 0x0500, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_DV_Based_100Mbps_1080x5994I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0202, 0x0600, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_DV_Based_100Mbps_1080x50I");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0202, 0x0700, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_DV_Based_100Mbps_720x5994P");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x0202, 0x0800, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_DV_Based_100Mbps_720x50P");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x04010202, 0x7100, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x0A),
        L"CompressionDef_VC3_1");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0E040201, 0x0204, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"CompressionDef_Avid_DNxHD_Legacy");
    
    
    //
    // Extendible enumerations
    //
    
    OMUniqueObjectIdentification elementOf;
    
    // OperationCategoryType
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020101, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    
    // TransferCharacteristicType
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020102, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0101, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0102, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0103, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0104, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0105, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0106, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    
    // PluginCategoryType
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020103, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0300, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0400, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    
    // UsageType
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020104, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0500, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0600, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0700, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0800, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0900, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    
    // ColorPrimariesType
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020105, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0301, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x06));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0302, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x06));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0303, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x06));
    
    // CodingEquationsType
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020106, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0202, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0203, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));

    return ss;
}




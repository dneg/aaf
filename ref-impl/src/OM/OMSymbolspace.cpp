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
// Licensor of the AAF Association is British Broadcasting Corporation.
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
#include "OMExtEnumeratedType.h"
#include "OMFixedArrayType.h"
#include "OMIndirectType.h"
#include "OMIntType.h"
#include "OMOpaqueType.h"
#include "OMRecordType.h"
#include "OMRenamedType.h"
#include "OMSetType.h"
#include "OMStreamType.h"
#include "OMStringType.h"
#include "OMStrongObjectReferenceType.h"
#include "OMVariableArrayType.h"
#include "OMWeakObjectReferenceType.h"
#include "OMSetIterator.h"
#include "OMXMLUtilities.h"
#include "OMVector.h"
#include "OMXMLException.h"
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
static const OMUniqueObjectIdentification ClassID_TypeDefinitionVariableArray =
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
    L"http://www.aafassociation.org/aafx/v1.1/20050511";
const OMUniqueObjectIdentification OMSymbolspace::_baselineId =
    {0x342fe22a, 0xa30e, 0x4219, {0xac, 0xc7, 0x5b, 0xe6, 0xda, 0x8b, 0xb9 ,0xa1}};

    
    
OMSymbolspace::OMSymbolspace(OMXMLStorage* store)
: _isInitialised(false), _store(store), _id(nullOMUniqueObjectIdentification), _uri(0), 
    _preferredPrefix(0), _prefix(0), _description(0), _uniqueSymbolSuffix(1)
{}

OMSymbolspace::OMSymbolspace(OMXMLStorage* store, OMUniqueObjectIdentification id, const wchar_t* uri, 
    const wchar_t* preferredPrefix, const wchar_t* description)
: _isInitialised(false), _store(store), _id(nullOMUniqueObjectIdentification), _uri(0), 
    _preferredPrefix(0), _prefix(0), _description(0), _uniqueSymbolSuffix(1)
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

    size_t i;
    for (i = 0; i < _propertyDefs.count(); i++)
    {
        delete _propertyDefs.getAt(i);
    }
    for (i = 0; i < _extEnumExtDefs.count(); i++)
    {
        delete _extEnumExtDefs.getAt(i);
    }
    OMSetIterator<OMUniqueObjectIdentification, 
        OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>* > iter(
            _extEnumValues, OMBefore);
    while (++iter)
    {
        delete iter.value();
    }
}

bool 
OMSymbolspace::isEmpty()
{
    TRACE("OMSymbolspace::isEmpty");
    
    return _idToSymbol.count() == 0 && _extEnumExtDefs.count() == 0;
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
OMSymbolspace::knownExtEnum(OMUniqueObjectIdentification id, 
    OMUniqueObjectIdentification value) const
{
    TRACE("OMSymbolspace::knownExtEnum");

    OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>* values; 
    if (_extEnumValues.find(id, values))
    {
        OMUniqueObjectIdentification tmp;
        if (values->find(value, tmp))
        {
            return true;
        }
    }
    
    return false;
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
OMSymbolspace::addExtEnumExtensions(OMUniqueObjectIdentification id, 
    OMVector<OMWString>& names, OMVector<OMUniqueObjectIdentification>& values)
{
    TRACE("OMSymbolspace::addExtEnumExtensions");
    
    ExtEnumExtGroup* extEnumExt = new ExtEnumExtGroup;
    extEnumExt->typeId = id;
    size_t i;
    for (i = 0; i < names.count(); i++)
    {
        extEnumExt->names.append(names.getAt(i));
        extEnumExt->values.append(values.getAt(i));
    }
    
    _extEnumExtDefs.insert(extEnumExt);
}

void 
OMSymbolspace::save()
{
    TRACE("OMSymbolspace::save");
    PRECONDITION("Is initialised", _isInitialised);
    
    getWriter()->writeElementStart(getBaselineURI(), L"MetaDictionary");

    wchar_t idUri[XML_MAX_AUID_URI_SIZE];
    auidToURI(_id, idUri);
    getWriter()->writeElementStart(getBaselineURI(), L"Identification");
    getWriter()->writeElementContent(idUri, wcslen(idUri));
    getWriter()->writeElementEnd();

    getWriter()->writeElementStart(getBaselineURI(), L"Symbolspace");
    getWriter()->writeElementContent(_uri, wcslen(_uri));
    getWriter()->writeElementEnd();

    if (_preferredPrefix != 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"PreferredPrefix");
        getWriter()->writeElementContent(_preferredPrefix,
            wcslen(_preferredPrefix));
        getWriter()->writeElementEnd();
    }

    if (_description != 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Description");
        getWriter()->writeElementContent(_description,
            wcslen(_description));
        getWriter()->writeElementEnd();
    }

    if (_classDefs.count() > 0 || _typeDefs.count() > 0 || _propertyDefs.count() > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Definitions");

        size_t i;
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

        for (i = 0; i < _extEnumExtDefs.count(); i++)
        {
            saveExtEnumExt(_extEnumExtDefs.getAt(i));
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
    getClassDefsTag(dictionary);
    getTypeDefsTag(dictionary);
    
    OMUniqueObjectIdentification id;
    OMWString symbolspace = 0;
    OMWString preferredPrefix = 0;
    OMWString description = 0;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"Identification"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid MetaDictionary Identification "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            uriToAUID(data, &id);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Symbolspace"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid MetaDictionary Symbolspace "
                    L"value");
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
                size_t length;
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
                size_t length;
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
            throw OMXMLException(L"Unknown element in MetaDictionary");
        }
    }
    getReader()->moveToEndElement();
    
    initialise(id, symbolspace.c_str(), preferredPrefix.c_str(), description.c_str());
}

void 
OMSymbolspace::registerDeferredDefs(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::registerDeferredDefs");

    size_t count = _propertyDefsForRegistration.count();
    size_t i;
    for (i = 0; i < count; i++)
    {
        RegisterPropertyPair* rp = _propertyDefsForRegistration.getAt(i);
        
        int result = dictionary->registerExtPropertyDef(rp->ownerClassId, 
            rp->propertyDef);
        if (result == OM_PROPERTY_REGISTERED_FAILED)
        {
            throw OMXMLException(L"Failed to register property def");
        }

        createSymbolForProperty(rp->propertyDef->identification(), 
            rp->propertyDef->localIdentification(), rp->propertyDef->name());

        if (result == OM_PROPERTY_REGISTERED_ALREADY_REGISTERED)
        {
            delete rp->propertyDef;
        }
        
        delete rp;
    }
    _propertyDefsForRegistration.clear();
    
    
    count = _extEnumExtDefsForRegistration.count();
    for (i = 0; i < count; i++)
    {
        ExtEnumExtGroup* extEnumExt = _extEnumExtDefsForRegistration.getAt(i);
     
        OMVector<const wchar_t*> names;
        for (size_t j = 0; j < extEnumExt->names.count(); j++)
        {
            names.append(extEnumExt->names.getAt(j).c_str());
        }
        bool result = dictionary->registerExtEnumExt(extEnumExt->typeId,
            names, extEnumExt->values);
        if (!result)
        {
            throw OMXMLException(L"Failed to register extendible enumeration extension");
        }
        
        delete extEnumExt;
    }
    _extEnumExtDefsForRegistration.clear();
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

    if (preferredPrefix != 0)
    {
        _preferredPrefix = new wchar_t[wcslen(preferredPrefix) + 1];
        wcscpy(_preferredPrefix, preferredPrefix);
    }
    if (description != 0)
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
OMSymbolspace::createSymbolForClass(OMUniqueObjectIdentification id, 
    const wchar_t* name)
{
    TRACE("OMSymbolspace::createSymbolForClass");
    PRECONDITION("Valid name", name != 0);
    
    wchar_t* symbol = createSymbol(name);
    while (_symbolToId.contains(symbol))
    {
        wchar_t suffix[9];
        std_swprintf(suffix, 9, L"%u", _uniqueSymbolSuffix);
        size_t len = wcslen(symbol);
        wchar_t* newSymbol = new wchar_t[len + 8 + 1];
        wcscpy(newSymbol, symbol);
        delete [] symbol;
        symbol = wcscat(newSymbol, suffix);
        _uniqueSymbolSuffix++;
    }
    addMetaDefSymbol(id, symbol);
    delete [] symbol;
    
    return getMetaDefSymbol(id);
}

const wchar_t* 
OMSymbolspace::createSymbolForProperty(OMUniqueObjectIdentification id, OMPropertyId localId,
    const wchar_t* name)
{
    TRACE("OMSymbolspace::createSymbolForProperty");
    PRECONDITION("Valid name", name != 0);

    wchar_t* symbol = createSymbol(name);
    while (_symbolToId.contains(symbol))
    {
        wchar_t suffix[9];
        std_swprintf(suffix, 9, L"%u", _uniqueSymbolSuffix);
        size_t len = wcslen(symbol);
        wchar_t* newSymbol = new wchar_t[len + 8 + 1 + 1];
        newSymbol[0] = L'_';
        wcscpy(&newSymbol[1], symbol);
        delete [] symbol;
        symbol = wcscat(newSymbol, suffix);
        _uniqueSymbolSuffix++;
    }
    addPropertyDefSymbol(id, localId, symbol);
    delete [] symbol;
    
    return getMetaDefSymbol(id);
}

const wchar_t* 
OMSymbolspace::createSymbolForType(OMUniqueObjectIdentification id, 
    const wchar_t* name)
{
    TRACE("OMSymbolspace::createSymbolForType");
    PRECONDITION("Valid name", name != 0);

    wchar_t* symbol = createSymbol(name);
    while (_symbolToId.contains(symbol))
    {
        wchar_t suffix[9];
        std_swprintf(suffix, 9, L"%u", _uniqueSymbolSuffix);
        size_t len = wcslen(symbol);
        wchar_t* newSymbol = new wchar_t[len + 8 + 1];
        wcscpy(newSymbol, symbol);
        delete [] symbol;
        symbol = wcscat(newSymbol, suffix);
        _uniqueSymbolSuffix++;
    }
    addMetaDefSymbol(id, symbol);
    delete [] symbol;
    
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
OMSymbolspace::addExtEnum(OMUniqueObjectIdentification id, 
    OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>* values)
{
    TRACE("OMSymbolspace::addExtEnum");

    _extEnumValues.insert(id, values);
}

void
OMSymbolspace::saveMetaDef(OMMetaDefinition* metaDef)
{
    TRACE("OMSymbolspace::saveMetaDef");

    wchar_t uri[XML_MAX_AUID_URI_SIZE];
    auidToURI(metaDef->identification(), uri);
    getWriter()->writeElementStart(getBaselineURI(), L"Identification");
    getWriter()->writeElementContent(uri, wcslen(uri));
    getWriter()->writeElementEnd();

    const wchar_t* symbol = getMetaDefSymbol(metaDef->identification());
    getWriter()->writeElementStart(getBaselineURI(), L"Symbol");
    getWriter()->writeElementContent(symbol, wcslen(symbol));
    getWriter()->writeElementEnd();

    const wchar_t* name = metaDef->name();
    getWriter()->writeElementStart(getBaselineURI(), L"Name");
    getWriter()->writeElementContent(name, wcslen(name));
    getWriter()->writeElementEnd();

    const wchar_t* description = metaDef->description();
    if (description != 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Description");
        getWriter()->writeElementContent(description, wcslen(description));
        getWriter()->writeElementEnd();
    }
    
}

void
OMSymbolspace::saveClassDef(OMClassDefinition* classDef)
{
    TRACE("OMSymbolspace::saveClassDef");

    getWriter()->writeElementStart(getBaselineURI(), L"ClassDefinition");
    
    saveMetaDef(classDef);
    
    OMClassDefinition* parentClass = classDef->parentClass(); 
    if (parentClass != 0)
    {
        wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
        saveMetaDefAUID(parentClass->identification(), idStr);        
        getWriter()->writeElementStart(getBaselineURI(), L"ParentClass");
        getWriter()->writeElementContent(idStr, wcslen(idStr));
        getWriter()->writeElementEnd();
    }

    wchar_t boolStr[XML_MAX_BOOL_STRING_SIZE];
    boolToString(classDef->isConcrete(), boolStr);
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

    saveMetaDef(propertyDef);

    const OMType* type = propertyDef->type();
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(type->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"Type");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    saveMetaDefAUID(ownerClassDef->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"MemberOf");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    wchar_t localIdStr[XML_MAX_INTEGER_STRING_SIZE];
    OMUInt16 localId = propertyDef->localIdentification();
    integerToString((OMByte*)&localId, sizeof(OMUInt16), false, localIdStr);
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

void
OMSymbolspace::saveTypeDef(OMType* typeDef)
{
    TRACE("OMSymbolspace::saveTypeDef");

    switch (typeDef->category())
    {
        case OMMetaDefinition::CHARACTER_TYPE:
            saveCharacterTypeDef(dynamic_cast<OMCharacterType*>(typeDef));
            break;
        case OMMetaDefinition::ENUMERATED_TYPE:
            saveEnumeratedTypeDef(dynamic_cast<OMEnumeratedType*>(typeDef));
            break;
        case OMMetaDefinition::EXT_ENUMERATED_TYPE:
            saveExtEnumeratedTypeDef(dynamic_cast<OMExtEnumeratedType*>(typeDef));
            break;
        case OMMetaDefinition::FIXED_ARRAY_TYPE:
            saveFixedArrayTypeDef(dynamic_cast<OMFixedArrayType*>(typeDef));
            break;
        case OMMetaDefinition::INDIRECT_TYPE:
            saveIndirectTypeDef(dynamic_cast<OMIndirectType*>(typeDef));
            break;
        case OMMetaDefinition::INTEGER_TYPE:
            saveIntTypeDef(dynamic_cast<OMIntType*>(typeDef));
            break;
        case OMMetaDefinition::OPAQUE_TYPE:
            saveOpaqueTypeDef(dynamic_cast<OMOpaqueType*>(typeDef));
            break;
        case OMMetaDefinition::RECORD_TYPE:
            saveRecordTypeDef(dynamic_cast<OMRecordType*>(typeDef));
            break;
        case OMMetaDefinition::RENAMED_TYPE:
            saveRenamedTypeDef(dynamic_cast<OMRenamedType*>(typeDef));
            break;
        case OMMetaDefinition::SET_TYPE:
            saveSetTypeDef(dynamic_cast<OMSetType*>(typeDef));
            break;
        case OMMetaDefinition::STREAM_TYPE:
            saveStreamTypeDef(dynamic_cast<OMStreamType*>(typeDef));
            break;
        case OMMetaDefinition::STRING_TYPE:
            saveStringTypeDef(dynamic_cast<OMStringType*>(typeDef));
            break;
        case OMMetaDefinition::STRONG_REF_TYPE:
            saveStrongObjectReferenceTypeDef(dynamic_cast<OMStrongObjectReferenceType*>(typeDef));
            break;
        case OMMetaDefinition::VARIABLE_ARRAY_TYPE:
            saveVariableArrayTypeDef(dynamic_cast<OMVariableArrayType*>(typeDef));
            break;
        case OMMetaDefinition::WEAK_REF_TYPE:
            saveWeakObjectReferenceTypeDef(dynamic_cast<OMWeakObjectReferenceType*>(typeDef));
            break;
        default:
            ASSERT("Valid type category", false);
            break;
    }
    
}

void
OMSymbolspace::saveCharacterTypeDef(OMCharacterType* typeDef)
{
    TRACE("OMSymbolspace::saveCharacterTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionCharacter");
    saveMetaDef(typeDef);
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveEnumeratedTypeDef(OMEnumeratedType* typeDef)
{
    TRACE("OMSymbolspace::saveEnumeratedTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionEnumeration");

    saveMetaDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(elementType->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    OMUInt32 count = typeDef->elementCount();
    if (count > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Elements");
        
        for (OMUInt32 i = 0; i<count; i++)
        {
            wchar_t* elementName = typeDef->elementName(i);
            OMInt64 elementValue = typeDef->elementValue(i);

            wchar_t valueStr[XML_MAX_INTEGER_STRING_SIZE];
            integerToString((OMByte*)&elementValue, sizeof(OMInt64), true, valueStr);

            getWriter()->writeElementStart(getBaselineURI(), L"Name");
            getWriter()->writeElementContent(elementName, wcslen(elementName));
            getWriter()->writeElementEnd();
            
            getWriter()->writeElementStart(getBaselineURI(), L"Value");
            getWriter()->writeElementContent(valueStr, wcslen(valueStr));
            getWriter()->writeElementEnd();
            
            delete [] elementName;
        }

        getWriter()->writeElementEnd();
    }
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveExtEnumeratedTypeDef(OMExtEnumeratedType* typeDef)
{
    TRACE("OMSymbolspace::saveExtEnumeratedTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionExtendibleEnumeration");

    saveMetaDef(typeDef);

    OMUInt32 count = typeDef->elementCount();
    if (count > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Elements");
        
        for (OMUInt32 i = 0; i<count; i++)
        {
            wchar_t* elementName = typeDef->elementName(i);
            OMUniqueObjectIdentification elementValue = typeDef->elementValue(i);

            wchar_t valueStr[XML_MAX_AUID_URI_SIZE];
            auidToURI(elementValue, valueStr);

            getWriter()->writeElementStart(getBaselineURI(), L"Name");
            getWriter()->writeElementContent(elementName, wcslen(elementName));
            getWriter()->writeElementEnd();
            
            getWriter()->writeElementStart(getBaselineURI(), L"Value");
            getWriter()->writeElementContent(valueStr, wcslen(valueStr));
            getWriter()->writeElementEnd();
            
            delete [] elementName;
        }

        getWriter()->writeElementEnd();
    }
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveFixedArrayTypeDef(OMFixedArrayType* typeDef)
{
    TRACE("OMSymbolspace::saveFixedArrayTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionFixedArray");

    saveMetaDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(elementType->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    wchar_t elementCountStr[XML_MAX_INTEGER_STRING_SIZE];
    OMUInt32 elementCount = typeDef->elementCount();
    integerToString((OMByte*)&elementCount, sizeof(OMUInt32), false, elementCountStr);
    getWriter()->writeElementStart(getBaselineURI(), L"ElementCount");
    getWriter()->writeElementContent(elementCountStr, wcslen(elementCountStr));
    getWriter()->writeElementEnd();
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveIndirectTypeDef(OMIndirectType* typeDef)
{
    TRACE("OMSymbolspace::saveIndirectTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionIndirect");
    saveMetaDef(typeDef);
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveIntTypeDef(OMIntType* typeDef)
{
    TRACE("OMSymbolspace::saveIntTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionInteger");

    saveMetaDef(typeDef);

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
OMSymbolspace::saveOpaqueTypeDef(OMOpaqueType* typeDef)
{
    TRACE("OMSymbolspace::saveOpaqueTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionOpaque");
    saveMetaDef(typeDef);
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveRecordTypeDef(OMRecordType* typeDef)
{
    TRACE("OMSymbolspace::saveRecordTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionRecord");

    saveMetaDef(typeDef);

    OMUInt32 count = typeDef->memberCount();
    if (count > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Members");
        
        for (OMUInt32 i = 0; i<count; i++)
        {
            wchar_t* memberName = typeDef->memberName(i);
            OMType* memberType = typeDef->memberType(i);

            getWriter()->writeElementStart(getBaselineURI(), L"Name");
            getWriter()->writeElementContent(memberName, wcslen(memberName));
            getWriter()->writeElementEnd();
            
            wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
            saveMetaDefAUID(memberType->identification(), idStr);        
            getWriter()->writeElementStart(getBaselineURI(), L"Type");
            getWriter()->writeElementContent(idStr, wcslen(idStr));
            getWriter()->writeElementEnd();
            
            delete [] memberName;
        }

        getWriter()->writeElementEnd();
    }
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveRenamedTypeDef(OMRenamedType* typeDef)
{
    TRACE("OMSymbolspace::saveRenamedTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionRename");

    saveMetaDef(typeDef);

    const OMType* renamedType = typeDef->renamedType(); 
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(renamedType->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"RenamedType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveSetTypeDef(OMSetType* typeDef)
{
    TRACE("OMSymbolspace::saveSetTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionSet");

    saveMetaDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(elementType->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveStreamTypeDef(OMStreamType* typeDef)
{
    TRACE("OMSymbolspace::saveStreamTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionStream");
    saveMetaDef(typeDef);
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveStringTypeDef(OMStringType* typeDef)
{
    TRACE("OMSymbolspace::saveStringTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionString");

    saveMetaDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(elementType->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveStrongObjectReferenceTypeDef(OMStrongObjectReferenceType* typeDef)
{
    TRACE("OMSymbolspace::saveStrongObjectReferenceTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionStrongObjectReference");

    saveMetaDef(typeDef);

    const OMClassDefinition* referencedClass = typeDef->referencedClass(); 
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(referencedClass->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"ReferencedClass");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveVariableArrayTypeDef(OMVariableArrayType* typeDef)
{
    TRACE("OMSymbolspace::saveVariableArrayTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionVariableArray");

    saveMetaDef(typeDef);

    const OMType* elementType = typeDef->elementType(); 
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(elementType->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"ElementType");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();
    
    getWriter()->writeElementEnd();
}

void
OMSymbolspace::saveWeakObjectReferenceTypeDef(OMWeakObjectReferenceType* typeDef)
{
    TRACE("OMSymbolspace::saveWeakObjectReferenceTypeDef");

    getWriter()->writeElementStart(getBaselineURI(), L"TypeDefinitionWeakObjectReference");

    saveMetaDef(typeDef);

    const OMClassDefinition* referencedClass = typeDef->referencedClass(); 
    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(referencedClass->identification(), idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"ReferencedClass");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    getWriter()->writeElementStart(getBaselineURI(), L"TargetSet");
    OMVector<OMUniqueObjectIdentification> targetSet;
    typeDef->targetSet(targetSet);
    for (size_t i = 0; i<targetSet.count(); i++)
    {
        saveMetaDefAUID(targetSet.getAt(i), idStr);        
        getWriter()->writeElementStart(getBaselineURI(), L"AUID");
        getWriter()->writeElementContent(idStr, wcslen(idStr));
        getWriter()->writeElementEnd();
    }
    getWriter()->writeElementEnd();

    
    getWriter()->writeElementEnd();
}

void 
OMSymbolspace::saveExtEnumExt(ExtEnumExtGroup* extEnumExt)
{
    TRACE("OMSymbolspace::saveExtEnumExt");
    
    getWriter()->writeElementStart(getBaselineURI(), L"ExtendibleEnumerationExtension");

    wchar_t idStr[XML_MAX_BASELINE_SYMBOL_SIZE];
    saveMetaDefAUID(extEnumExt->typeId, idStr);        
    getWriter()->writeElementStart(getBaselineURI(), L"Extends");
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    getWriter()->writeElementEnd();

    if (extEnumExt->names.count() > 0)
    {
        getWriter()->writeElementStart(getBaselineURI(), L"Elements");
        for (size_t i = 0; i < extEnumExt->names.count(); i++)
        {
            OMWString elementName = extEnumExt->names.getAt(i);
            getWriter()->writeElementStart(getBaselineURI(), L"Name");
            getWriter()->writeElementContent(elementName.c_str(), elementName.length());
            getWriter()->writeElementEnd();
            
            wchar_t valueStr[XML_MAX_AUID_URI_SIZE];
            auidToURI(extEnumExt->values.getAt(i), valueStr);
            getWriter()->writeElementStart(getBaselineURI(), L"Value");
            getWriter()->writeElementContent(valueStr, wcslen(valueStr));
            getWriter()->writeElementEnd();
        }
        getWriter()->writeElementEnd();
    }
    
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
        restoreVariableArrayTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"TypeDefinitionWeakObjectReference"))
    {
        restoreWeakObjectReferenceTypeDef(dictionary);
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"ExtendibleEnumerationExtension"))
    {
        restoreExtEnumExt(dictionary);
    }
    else
    {
        throw OMXMLException(L"Unknown definition type encountered");
    }
    
}

class MetaDef
{
public:
    MetaDef() 
    {
        id = nullOMUniqueObjectIdentification;
    }
    ~MetaDef() 
    {}
    bool isSet()
    {
        return (id != nullOMUniqueObjectIdentification &&
            symbol.length() > 0 && name.length() > 0);
    }
    
    OMUniqueObjectIdentification id;
    OMWString symbol;
    OMWString name;
    OMWString description;
};

bool 
OMSymbolspace::restoreMetaDef(MetaDef* metaDef)
{
    TRACE("OMSymbolspace::restoreMetaDef");
    
    if (getReader()->elementEquals(getBaselineURI(), L"Identification"))
    {
        const wchar_t* data;
        size_t length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMXMLException(L"Empty string is invalid MetaDef Identification value");
        }
        getReader()->getCharacters(data, length);
        uriToAUID(data, &metaDef->id);
        getReader()->moveToEndElement();
        
        return true;
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"Symbol"))
    {
        const wchar_t* data;
        size_t length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMXMLException(L"Empty string is invalid MetaDef Symbol value");
        }
        getReader()->getCharacters(data, length);
        metaDef->symbol = data;
        getReader()->moveToEndElement();
        
        return true;
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"Name"))
    {
        const wchar_t* data;
        size_t length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMXMLException(L"Empty string is invalid MetaDef Name value");
        }
        getReader()->getCharacters(data, length);
        metaDef->name = data;
        getReader()->moveToEndElement();
        
        return true;
    }
    else if (getReader()->elementEquals(getBaselineURI(), L"Description"))
    {
        getReader()->next();
        if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
        {
            const wchar_t* data;
            size_t length;
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
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid ClassDef ParentClass "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            parentClassId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"IsConcrete"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid ClassDef IsConcrete "
                    L"value");
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
                throw OMXMLException(L"Unknown element in ClassDefinition");
            }
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || !isConcreteSet)
    {
        throw OMXMLException(L"Incomplete ClassDefinition");
    }
    
    OMStorable* storable = dictionary->create(ClassID_ClassDefinition);
    OMClassDefinition* classDef = dynamic_cast<OMClassDefinition*>(storable);
    if (!classDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(), 
        parentClassId, isConcrete, getClassDefsTag(dictionary)))
    {
        throw OMXMLException(L"Failed to initialise class definition");
    }
    int result = dictionary->registerExtClassDef(classDef);
    if (result == OM_CLASS_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension class definition");
    }

    createSymbolForClass(classDef->identification(), classDef->name());    

    if (result == OM_CLASS_REGISTERED_ALREADY_REGISTERED)
    {
        delete classDef;
    }
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
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid PropertyDef "
                    L"LocalIdentification value");
            }
            getReader()->getCharacters(data, length);
            uint16FromString(data, localId);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"Type"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid PropertyDef Type "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            typeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"MemberOf"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid PropertyDef MemberOf "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            memberOfId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"IsUniqueIdentifier"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid PropertyDef IsUniqueIdentifier "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            boolFromString(data, isUniqueIdentifier);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"IsOptional"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid PropertyDef IsOptional "
                    L"value");
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
                throw OMXMLException(L"Unknown element in PropertyDefinition");
            }
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || localId == 0 || typeId == nullOMUniqueObjectIdentification ||
        memberOfId == nullOMUniqueObjectIdentification || !isOptionalSet)
    {
        throw OMXMLException(L"Incomplete PropertyDefinition");
    }

    OMStorable* storable = dictionary->create(ClassID_PropertyDefinition);
    OMPropertyDefinition* propertyDef = dynamic_cast<OMPropertyDefinition*>(storable);
    if (!propertyDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(), 
        localId, typeId, isOptional, isUniqueIdentifier))
    {
        throw OMXMLException(L"Failed to initialise property definition");
    }
    RegisterPropertyPair* regPair = new RegisterPropertyPair;
    regPair->ownerClassId = memberOfId;
    regPair->propertyDef = propertyDef;
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
            throw OMXMLException(L"Unknown element in CharacterTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMXMLException(L"Incomplete CharacterTypeDef");
    }
    
    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionCharacter);
    OMCharacterType* typeDef = dynamic_cast<OMCharacterType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str()))
    {
        throw OMXMLException(L"Failed to initialise CharacterTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension CharacterTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
    
}

void 
OMSymbolspace::restoreEnumeratedTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreEnumeratedTypeDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification elementTypeId = nullOMUniqueObjectIdentification;
    OMSet<OMWString, OMWString> elementNamesSet;
    OMSet<OMInt64, OMInt64> elementValuesSet;
    OMVector<wchar_t*> elementNames;
    OMVector<OMInt64> elementValues;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid Enumerated ElementType "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            elementTypeId = restoreMetaDefAUID(data);
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
                    throw OMXMLException(L"Expecting Name element in EnumeratedType Elements");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid Name element in EnumeratedType Elements");
                }
                const wchar_t* data;
                size_t length;
                getReader()->getCharacters(data, length);
                if (elementNamesSet.contains(data))
                {
                    throw OMXMLException(L"Duplicate Name value in EnumeratedType Elements");
                }
                elementNamesSet.insert(data, data);
                elementNames.append(wideCharacterStringDup(data));
                getReader()->moveToEndElement();
                
                if (!getReader()->nextElement())
                {
                    throw OMXMLException(L"Missing matching Value element in EnumeratedType Elements");
                }
                
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Value"))
                {
                    throw OMXMLException(L"Expecting Value element in EnumeratedType Elements");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid Value element in EnumeratedType Elements");
                }
                getReader()->getCharacters(data, length);
                OMInt64 value;
                int64FromString(data, value);
                if (elementValuesSet.contains(value))
                {
                    throw OMXMLException(L"Duplicate Value value in EnumeratedType Elements");
                }
                elementValuesSet.insert(value, value);
                elementValues.append(value);
                getReader()->moveToEndElement();
            }
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in EnumeratedTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || elementTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMXMLException(L"Incomplete EnumeratedTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionEnumeration);
    OMEnumeratedType* typeDef = dynamic_cast<OMEnumeratedType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        elementTypeId, getTypeDefsTag(dictionary),
        elementNames, elementValues))
    {
        throw OMXMLException(L"Failed to initialise EnumeratedTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension EnumeratedTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
    
    size_t count = elementNames.count();
    for (size_t i = 0; i < count; i++)
    {
        delete [] elementNames.getAt(i);
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
        
        if (getReader()->elementEquals(getBaselineURI(), L"Elements"))
        {
            while (getReader()->nextElement())
            {
                const wchar_t* nmspace;
                const wchar_t* localName;
                const OMList<OMXMLAttribute*>* attrs;
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Name"))
                {
                    throw OMXMLException(L"Expecting Name element in ExtEnumeratedType Elements");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid Name element in ExtEnumeratedType Elements");
                }
                const wchar_t* data;
                size_t length;
                getReader()->getCharacters(data, length);
                if (elementNamesSet.contains(data))
                {
                    throw OMXMLException(L"Duplicate Name value in ExtEnumeratedType Elements");
                }
                elementNamesSet.insert(data, data);
                elementNames.append(wideCharacterStringDup(data));
                getReader()->moveToEndElement();
                
                if (!getReader()->nextElement())
                {
                    throw OMXMLException(L"Missing matching Value element in ExtEnumeratedType Elements");
                }
                
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Value"))
                {
                    throw OMXMLException(L"Expecting Value element in ExtEnumeratedType Elements");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid Value element in ExtEnumeratedType Elements");
                }
                getReader()->getCharacters(data, length);
                OMUniqueObjectIdentification value;
                uriToAUID(data, &value);
                if (elementValuesSet.contains(value))
                {
                    throw OMXMLException(L"Duplicate Value value in ExtEnumeratedType Elements");
                }
                elementValuesSet.insert(value, value);
                elementValues.append(value);
                getReader()->moveToEndElement();
            }
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in ExtEnumeratedTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMXMLException(L"Incomplete ExtEnumeratedTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionExtendibleEnumeration);
    OMExtEnumeratedType* typeDef = dynamic_cast<OMExtEnumeratedType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        elementNames, elementValues))
    {
        throw OMXMLException(L"Failed to initialise ExtEnumeratedTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension ExtEnumeratedTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
    
    size_t count = elementNames.count();
    for (size_t i = 0; i < count; i++)
    {
        delete [] elementNames.getAt(i);
    }
}

void 
OMSymbolspace::restoreFixedArrayTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreFixedArrayTypeDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification elementTypeId = nullOMUniqueObjectIdentification;
    OMUInt32 elementCount;
    
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
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid FixedArray ElementType "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"ElementCount"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid FixedArray ElementCount "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            uint32FromString(data, elementCount);
            getReader()->moveToEndElement();
            elementCountSet = true;
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in FixedArrayTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || elementTypeId == nullOMUniqueObjectIdentification ||
        !elementCountSet)
    {
        throw OMXMLException(L"Incomplete FixedArrayTypeDef");
    }
    
    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionFixedArray);
    OMFixedArrayType* typeDef = dynamic_cast<OMFixedArrayType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        elementTypeId, getTypeDefsTag(dictionary), elementCount))
    {
        throw OMXMLException(L"Failed to initialise FixedArrayTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension FixedArrayTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
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
            throw OMXMLException(L"Unknown element in IndirectTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMXMLException(L"Incomplete IndirectTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionIndirect);
    OMIndirectType* typeDef = dynamic_cast<OMIndirectType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str()))
    {
        throw OMXMLException(L"Failed to initialise IndirectTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension IndirectTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
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
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid IntTypeDef "
                    L"Size value");
            }
            getReader()->getCharacters(data, length);
            uint8FromString(data, size);
            if (size != 1 && size != 2 && size != 4 && size != 8)
            {
                throw OMXMLException(L"Invalid IntTypeDef size value");
            }
            getReader()->moveToEndElement();
        }
        else if (getReader()->elementEquals(getBaselineURI(), L"IsSigned"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid IntTypeDef "
                    L"IsSigned value");
            }
            getReader()->getCharacters(data, length);
            boolFromString(data, isSigned);
            getReader()->moveToEndElement();
            isSignedSet = true;
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in IntTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || size == 0 || !isSignedSet)
    {
        throw OMXMLException(L"Incomplete IntTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionInteger);
    OMIntType* typeDef = dynamic_cast<OMIntType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        size, isSigned))
    {
        throw OMXMLException(L"Failed to initialise IntTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension IntTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
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
            throw OMXMLException(L"Unknown element in OpaqueTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMXMLException(L"Incomplete OpaqueTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionOpaque);
    OMOpaqueType* typeDef = dynamic_cast<OMOpaqueType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str()))
    {
        throw OMXMLException(L"Failed to initialise OpaqueTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension OpaqueTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
}

void 
OMSymbolspace::restoreRecordTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreRecordTypeDef");
    
    MetaDef metaDef;
    OMSet<OMWString, OMWString> memberNamesSet;
    OMVector<wchar_t*> memberNames;
    OMVector<OMUniqueObjectIdentification> memberTypeIds;
    
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
                    throw OMXMLException(L"Expecting Name element in RecordType Members");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid Name element in RecordType Members");
                }
                const wchar_t* data;
                size_t length;
                getReader()->getCharacters(data, length);
                if (memberNamesSet.contains(data))
                {
                    throw OMXMLException(L"Duplicate Name value in RecordType Elements");
                }
                memberNamesSet.insert(data, data);
                memberNames.append(wideCharacterStringDup(data));
                getReader()->moveToEndElement();
                
                if (!getReader()->nextElement())
                {
                    throw OMXMLException(L"Missing matching Type element in RecordType Members");
                }
                
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Type"))
                {
                    throw OMXMLException(L"Expecting Type element in RecordType Members");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid Type element in RecordType Members");
                }
                getReader()->getCharacters(data, length);
                OMUniqueObjectIdentification typeId = restoreMetaDefAUID(data);
                memberTypeIds.append(typeId);
                getReader()->moveToEndElement();
            }
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in RecordTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMXMLException(L"Incomplete RecordTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionRecord);
    OMRecordType* typeDef = dynamic_cast<OMRecordType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        memberNames, memberTypeIds, getTypeDefsTag(dictionary)))
    {
        throw OMXMLException(L"Failed to initialise RecordTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension RecordTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
    
    size_t count = memberNames.count();
    for (size_t i = 0; i < count; i++)
    {
        delete [] memberNames.getAt(i);
    }
}

void 
OMSymbolspace::restoreRenamedTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreRenamedTypeDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification renamedTypeId = nullOMUniqueObjectIdentification;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"RenamedType"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid Renamed RenamedType "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            renamedTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in RenamedTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || renamedTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMXMLException(L"Incomplete RenamedTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionRename);
    OMRenamedType* typeDef = dynamic_cast<OMRenamedType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        renamedTypeId, getTypeDefsTag(dictionary)))
    {
        throw OMXMLException(L"Failed to initialise RenamedTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension RenamedTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
}

void 
OMSymbolspace::restoreSetTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreSetTypeDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification elementTypeId = nullOMUniqueObjectIdentification;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid Set ElementType "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in SetTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || elementTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMXMLException(L"Incomplete SetTypeDef");
    }
    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionSet);
    OMSetType* typeDef = dynamic_cast<OMSetType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        elementTypeId, getTypeDefsTag(dictionary)))
    {
        throw OMXMLException(L"Failed to initialise SetTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension SetTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
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
            throw OMXMLException(L"Unknown element in StreamTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet())
    {
        throw OMXMLException(L"Incomplete StreamTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionStream);
    OMStreamType* typeDef = dynamic_cast<OMStreamType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str()))
    {
        throw OMXMLException(L"Failed to initialise StreamTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension StreamTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
}

void 
OMSymbolspace::restoreStringTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreStringTypeDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification elementTypeId = nullOMUniqueObjectIdentification;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid String ElementType "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in StringTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || elementTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMXMLException(L"Incomplete StringTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionString);
    OMStringType* typeDef = dynamic_cast<OMStringType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        elementTypeId, getTypeDefsTag(dictionary)))
    {
        throw OMXMLException(L"Failed to initialise StringTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension StringTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
}

void 
OMSymbolspace::restoreStrongObjectReferenceTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreStrongObjectReferenceTypeDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification referencedClassId = nullOMUniqueObjectIdentification;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ReferencedClass"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid StrongObjectReference "
                    L"ReferencedClass value");
            }
            getReader()->getCharacters(data, length);
            referencedClassId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in StrongObjectReferenceTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || referencedClassId == nullOMUniqueObjectIdentification)
    {
        throw OMXMLException(L"Incomplete StrongObjectReferenceTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionStrongObjectReference);
    OMStrongObjectReferenceType* typeDef = dynamic_cast<OMStrongObjectReferenceType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        referencedClassId, getClassDefsTag(dictionary)))
    {
        throw OMXMLException(L"Failed to initialise StrongObjectReferenceTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension StrongObjectReferenceTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
}

void 
OMSymbolspace::restoreVariableArrayTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreVariableArrayTypeDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification elementTypeId = nullOMUniqueObjectIdentification;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ElementType"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid VariableArray ElementType "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            elementTypeId = restoreMetaDefAUID(data);
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in VariableArrayTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || elementTypeId == nullOMUniqueObjectIdentification)
    {
        throw OMXMLException(L"Incomplete VariableArrayTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionVariableArray);
    OMVariableArrayType* typeDef = dynamic_cast<OMVariableArrayType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        elementTypeId, getTypeDefsTag(dictionary)))
    {
        throw OMXMLException(L"Failed to initialise VariableArrayTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension VariableArrayTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
}

void 
OMSymbolspace::restoreWeakObjectReferenceTypeDef(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreWeakObjectReferenceTypeDef");
    
    MetaDef metaDef;
    OMUniqueObjectIdentification referencedClassId = nullOMUniqueObjectIdentification;
    OMVector<OMUniqueObjectIdentification> targetSet;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"ReferencedClass"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid WeakObjectReference "
                    L"ReferencedClass value");
            }
            getReader()->getCharacters(data, length);
            referencedClassId = restoreMetaDefAUID(data);
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
                if (!getReader()->elementEquals(getBaselineURI(), L"AUID"))
                {
                    throw OMXMLException(L"Expecting AUID element in WeakObjectReferenceType TargetSet");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid AUID element in WeakObjectReferenceType TargetSet");
                }
                const wchar_t* data;
                size_t length;
                getReader()->getCharacters(data, length);
                OMUniqueObjectIdentification id = restoreMetaDefAUID(data);
                getReader()->moveToEndElement();
                
                targetSet.append(id);
            }
            getReader()->moveToEndElement();
        }
        else if (!restoreMetaDef(&metaDef))
        {
            throw OMXMLException(L"Unknown element in WeakObjectReferenceTypeDef");
        }
    }
    getReader()->moveToEndElement();
    
    if (!metaDef.isSet() || referencedClassId == nullOMUniqueObjectIdentification)
    {
        throw OMXMLException(L"Incomplete WeakObjectReferenceTypeDef");
    }

    OMStorable* storable = dictionary->create(ClassID_TypeDefinitionWeakObjectReference);
    OMWeakObjectReferenceType* typeDef = dynamic_cast<OMWeakObjectReferenceType*>(storable);
    if (!typeDef->initialise(metaDef.id, metaDef.name.c_str(), metaDef.description.c_str(),
        referencedClassId, getClassDefsTag(dictionary),
        targetSet))
    {
        throw OMXMLException(L"Failed to initialise WeakObjectReferenceTypeDef");
    }
    int result = dictionary->registerExtTypeDef(typeDef);
    if (result == OM_TYPE_REGISTERED_FAILED)
    {
        throw OMXMLException(L"Failed to register extension WeakObjectReferenceTypeDef");
    }

    createSymbolForType(typeDef->identification(), typeDef->name());    

    if (result == OM_TYPE_REGISTERED_ALREADY_REGISTERED)
    {
        delete typeDef;
    }
}

void 
OMSymbolspace::restoreExtEnumExt(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::restoreExtEnumExt");
    
    OMSet<OMWString, OMWString> elementNamesSet;
    OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification> elementValuesSet;
    ExtEnumExtGroup* extEnumExt = new ExtEnumExtGroup;
    extEnumExt->typeId = nullOMUniqueObjectIdentification;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        
        if (getReader()->elementEquals(getBaselineURI(), L"Extends"))
        {
            const wchar_t* data;
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid ExtEnumExt "
                    L"value");
            }
            getReader()->getCharacters(data, length);
            extEnumExt->typeId = restoreMetaDefAUID(data);
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
                    throw OMXMLException(L"Expecting Name element in ExtEnumExt Elements");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid Name element in ExtEnumExt Elements");
                }
                const wchar_t* data;
                size_t length;
                getReader()->getCharacters(data, length);
                if (elementNamesSet.contains(data))
                {
                    throw OMXMLException(L"Duplicate Name value in ExtEnumExt Elements");
                }
                elementNamesSet.insert(data, data);
                extEnumExt->names.append(data);
                getReader()->moveToEndElement();
                
                if (!getReader()->nextElement())
                {
                    throw OMXMLException(L"Missing matching Value element in ExtEnumExt Elements");
                }
                
                getReader()->getStartElement(nmspace, localName, attrs);
                if (!getReader()->elementEquals(getBaselineURI(), L"Value"))
                {
                    throw OMXMLException(L"Expecting Value element in ExtEnumExt Elements");
                }
                getReader()->next();
                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                {
                    throw OMXMLException(L"Invalid Value element in ExtEnumExt Elements");
                }
                getReader()->getCharacters(data, length);
                OMUniqueObjectIdentification value;
                uriToAUID(data, &value);
                if (elementValuesSet.contains(value))
                {
                    throw OMXMLException(L"Duplicate Value value in ExtEnumExt Elements");
                }
                elementValuesSet.insert(value, value);
                extEnumExt->values.append(value);
                getReader()->moveToEndElement();
            }
            getReader()->moveToEndElement();
        }
        else
        {
            throw OMXMLException(L"Unknown element in ExtEnumExt");
        }
    }
    getReader()->moveToEndElement();
    
    if (extEnumExt->typeId == nullOMUniqueObjectIdentification)
    {
        throw OMXMLException(L"Incomplete ExtEnumExt");
    }

    if (extEnumExt->names.count() == 0)
    {
        delete [] extEnumExt;
    }
    else
    {
        _extEnumExtDefsForRegistration.insert(extEnumExt);
    }
}


OMUniqueObjectIdentification
OMSymbolspace::restoreMetaDefAUID(const wchar_t* idStr)
{
    TRACE("OMSymbolspace::restoreMetaDefAUID");
    
    OMUniqueObjectIdentification id = nullOMUniqueObjectIdentification;
    if (isURI(idStr))
    {
        uriToAUID(idStr, &id);
    }
    else
    {
        id = _store->getBaselineMetaDefId(idStr);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMXMLException(L"Could not retrieve unique id from symbol");
        }
    }
    return id;
}

void 
OMSymbolspace::saveMetaDefAUID(OMUniqueObjectIdentification id, wchar_t* idStr)
{
    TRACE("OMSymbolspace::saveMetaDefAUID");
    
    const wchar_t* symbol = _store->getBaselineMetaDefSymbol(id);
    if (symbol != 0)
    {
        wcsncpy(idStr, symbol, XML_MAX_BASELINE_SYMBOL_SIZE);
    }
    else
    {
        wchar_t uri[XML_MAX_AUID_URI_SIZE];
        auidToURI(id, uri);
        wcsncpy(idStr, uri, XML_MAX_AUID_URI_SIZE);
    }
}

// TODO: cache tags?
OMPropertyTag 
OMSymbolspace::getClassDefsTag(OMDictionary* dictionary)
{
    TRACE("OMSymbolspace::getClassDefsTag");

    OMPropertyTable* table = dictionary->file()->referencedProperties();
    return table->insert(classDefsTargetPath);
}

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

#define SET_EXT_ENUM_ID(ID) \
{ \
    const OMUniqueObjectIdentification tmp = ID; \
    id = tmp; \
}

#define ADD_EXT_ENUM_VALUE(VALUE) \
{ \
    const OMUniqueObjectIdentification value = VALUE; \
    values->insert(value, value); \
}



OMSymbolspace* 
OMSymbolspace::createDefaultExtSymbolspace(OMXMLStorage* store, OMUniqueObjectIdentification id)
{
    TRACE("OMSymbolspace::createDefaultExtSymbolspace");

    wchar_t uri[XML_MAX_AUID_URI_SIZE];
    auidToURI(id, uri);
    
    OMSymbolspace* ss = new OMSymbolspace(store, id, uri, L"aafext", 
        L"AAF file default extension symbolspace");
    
    return ss;
}

OMSymbolspace* 
OMSymbolspace::createV11Symbolspace(OMXMLStorage* store)
{
    TRACE("OMSymbolspace::createV11Symbolspace");

    OMSymbolspace* ss = new OMSymbolspace(store, _baselineId, _baselineURI, L"aaf", 
        L"AAF version 1.1 baseline symbolspace");

        
    //
    // Classes
    //
    
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"InterchangeObject");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0200, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Component");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0300, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Segment");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0400, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EdgeCode");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0500, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EssenceGroup");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0600, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Event");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0700, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"GPITrigger");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0800, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"CommentMarker");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4100, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DescriptiveMarker");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010401, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DescriptiveFramework");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0900, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Filler");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0a00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"OperationGroup");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0b00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"NestedScope");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0c00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Pulldown");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0d00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ScopeReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0e00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Selector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0f00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Sequence");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SourceReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1100, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SourceClip");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1200, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TextClip");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1300, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"HTMLClip");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1400, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Timecode");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1500, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TimecodeStream");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1600, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TimecodeStream12M");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1700, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Transition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1800, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ContentStorage");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1900, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ControlPoint");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1a00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DefinitionObject");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ClassDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1b00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DataDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1c00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"OperationDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1d00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ParameterDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0202, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"PropertyDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0203, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1e00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"PluginDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x1f00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"CodecDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ContainerDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2100, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"InterpolationDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2200, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Dictionary");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2300, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EssenceData");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2400, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EssenceDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2500, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"FileDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2600, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"AIFCDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2700, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"DigitalImageDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2800, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"CDCIDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2900, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"RGBADescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4200, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SoundDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4800, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"PCMDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2a00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"HTMLDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2b00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TIFFDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2c00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"WAVEDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2d00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"FilmDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2e00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TapeDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4900, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"PhysicalDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4a00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ImportDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4b00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"RecordingDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4e00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"AuxiliaryDescriptor");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4c00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TaggedValueDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4d00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"KLVDataDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x2f00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Header");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Identification");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3100, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Locator");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3200, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"NetworkLocator");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3300, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TextLocator");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3400, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Mob");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3500, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"CompositionMob");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3600, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MasterMob");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3700, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"SourceMob");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3800, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MobSlot");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3900, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"EventMobSlot");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3a00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"StaticMobSlot");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3b00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TimelineMobSlot");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3c00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"Parameter");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3d00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"ConstantValue");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3e00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"VaryingValue");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x3f00, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TaggedValue");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x4000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"KLVData");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0204, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionInteger");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0205, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionStrongObjectReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0206, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionWeakObjectReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0207, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionEnumeration");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0208, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionFixedArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0209, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionVariableArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x020a, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x020b, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionString");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x020c, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionStream");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x020d, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionRecord");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x020e, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionRename");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0220, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionExtendibleEnumeration");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0221, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionIndirect");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0222, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionOpaque");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0223, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"TypeDefinitionCharacter");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0224, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MetaDefinition");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0225, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01),
        L"MetaDictionary");



    //
    // Properties
    //
    
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0101, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0101,
        L"ObjClass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0102,
        L"Generation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04070100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0201,
        L"ComponentDataDefinition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020201, 0x0103, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0202,
        L"ComponentLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0203,
        L"ComponentKLVData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020102, 0x1600, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x0204,
        L"ComponentUserComments");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x0205,
        L"ComponentAttributes");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01040901, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0401,
        L"EdgeCodeStart");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0109, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0402,
        L"FilmKind");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0102, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x0403,
        L"CodeFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01030201, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0404,
        L"EdgeCodeHeader");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0601, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0501,
        L"Choices");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0208, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0502,
        L"StillFrame");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0303, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0601,
        L"Position");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300404, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0602,
        L"Comment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300401, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x0801,
        L"ActiveState");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x020a, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0901,
        L"Annotation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070105, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x04),
        0x6102,
        L"DescribedSlots");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x020c, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x6101,
        L"DescriptiveMarkerDescription");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300506, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0B01,
        L"Operation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0602, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0B02,
        L"InputSegments");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x060a, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0B03,
        L"Parameters");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0530050c, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0B04,
        L"BypassOverride");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0206, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0B05,
        L"OperationGroupRendering");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0607, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0C01,
        L"NestedScopeSlots");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0207, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0D01,
        L"InputSegment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05401001, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0D02,
        L"PulldownKind");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05401001, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0D03,
        L"PulldownDirection");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05401001, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0D04,
        L"PhaseFrame");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0E01,
        L"RelativeScope");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0E02,
        L"RelativeSlot");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0209, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0F01,
        L"Selected");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0608, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0F02,
        L"Alternates");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0609, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1001,
        L"Components");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1101,
        L"SourceID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1102,
        L"SourceMobSlotID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x1103,
        L"ChannelIDs");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010103, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x1104,
        L"MonoSourceSlotIDs");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0104, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1201,
        L"StartTime");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020201, 0x0105, 0x0200, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1202,
        L"FadeInLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300501, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1203,
        L"FadeInType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020201, 0x0105, 0x0300, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1204,
        L"FadeOutLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300502, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1205,
        L"FadeOutType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300601, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1401,
        L"BeginAnchor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300602, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1402,
        L"EndAnchor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0105, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1501,
        L"TimecodeStart");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040101, 0x0206, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1502,
        L"FPS");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040101, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1503,
        L"Drop");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040101, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1601,
        L"TimecodeStreamSampleRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04070300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1602,
        L"Source");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040201, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1603,
        L"SourceType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04040101, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1701,
        L"IncludeSync");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0205, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1801,
        L"TransitionOperationGroup");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0106, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1802,
        L"CutPoint");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0501, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1901,
        L"Mobs");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0502, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1902,
        L"ContentStorageEssenceData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0530050d, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1A02,
        L"ControlPointValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x1002, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1A03,
        L"Time");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300508, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1A04,
        L"EditHint");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01011503, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1B01,
        L"DefinitionObjectIdentification");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070102, 0x0301, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1B02,
        L"DefinitionObjectName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020301, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1B03,
        L"DefinitionObjectDescription");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0008,
        L"ParentClass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0009,
        L"Properties");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000A,
        L"IsConcrete");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300509, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1E01,
        L"OperationDefinitionDataDefinition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300503, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1E02,
        L"IsTimeWarp");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0401, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1E03,
        L"DegradeTo");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0530050a, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1E06,
        L"OperationCategory");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300504, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1E07,
        L"NumberInputs");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300505, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x1E08,
        L"Bypass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0302, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1E09,
        L"ParametersDefined");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0106, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1F01,
        L"ParameterDefinitionType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0530050b, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x1F03,
        L"DisplayUnits");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000B,
        L"PropertyDefinitionType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010202, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000C,
        L"IsOptional");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000D,
        L"LocalIdentification");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0600, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000E,
        L"IsUniqueIdentifier");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200901, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2203,
        L"PluginCategory");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030301, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2204,
        L"VersionNumber");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030301, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2205,
        L"VersionString");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x010a0101, 0x0101, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2206,
        L"PluginDefinitionManufacturer");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x020b, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2207,
        L"ManufacturerInfo");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x010a0101, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2208,
        L"PluginDefinitionManufacturerID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200902, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2209,
        L"PluginDefinitionPlatform");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200903, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220A,
        L"MinPlatformVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200904, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220B,
        L"MaxPlatformVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200905, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220C,
        L"Engine");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200906, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220D,
        L"MinEngineVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200907, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220E,
        L"MaxEngineVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200908, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x220F,
        L"PluginAPI");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200909, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2210,
        L"MinPluginAPI");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090a, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2211,
        L"MaxPluginAPI");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090b, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2212,
        L"SoftwareOnly");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090c, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2213,
        L"Accelerator");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090d, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2214,
        L"Locators");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090e, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2215,
        L"Authentication");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0520090f, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2216,
        L"PluginDefinitionDefinitionObject");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0107, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2301,
        L"FileDescriptorClass");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0301, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2302,
        L"CodecDefinitionDataDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x2401,
        L"EssenceIsIdentified");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0503, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2603,
        L"OperationDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0504, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2604,
        L"ParameterDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0505, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2605,
        L"DataDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0506, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2606,
        L"PluginDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0507, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2607,
        L"CodecDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0508, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2608,
        L"ContainerDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0509, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2609,
        L"InterpolationDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x050a, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x260A,
        L"KLVDataDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x050b, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x260B,
        L"TaggedValueDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010106, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2701,
        L"EssenceDataMobID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04070200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2702,
        L"Data");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010102, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2B01,
        L"SampleIndex");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0603, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x2F01,
        L"EssenceDescriptorLocator");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04060101, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3001,
        L"FileDescriptorSampleRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04060102, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3002,
        L"FileDescriptorLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0102, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3004,
        L"ContainerFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0103, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3005,
        L"FileDescriptorCodecDefinition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030302, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3101,
        L"AIFCDescriptorSummary");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010601, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3201,
        L"DigitalImageDescriptorCompression");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010502, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3202,
        L"StoredHeight");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010502, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3203,
        L"StoredWidth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3204,
        L"SampledHeight");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3205,
        L"SampledWidth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0900, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3206,
        L"SampledXOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0a00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3207,
        L"SampledYOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0b00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3208,
        L"DisplayHeight");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0c00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3209,
        L"DisplayWidth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0d00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x320A,
        L"DisplayXOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0e00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x320B,
        L"DisplayYOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010301, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x320C,
        L"FrameLayout");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x320D,
        L"VideoLineMap");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010101, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x320E,
        L"ImageAspectRatio");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200102, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x320F,
        L"AlphaTransparency");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010201, 0x0101, 0x0200, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3210,
        L"TransferCharacteristic");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010201, 0x0106, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x09),
        0x3219,
        L"ColorPrimaries");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010201, 0x0103, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x321A,
        L"CodingEquations");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04180101, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3211,
        L"ImageAlignmentFactor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010301, 0x0600, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3212,
        L"FieldDominance");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04180102, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3213,
        L"FieldStartOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04180103, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3214,
        L"FieldEndOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04050113, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3215,
        L"SignalStandard");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3216,
        L"StoredF2Offset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3217,
        L"DisplayF2Offset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0900, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3218,
        L"ActiveFormatDescriptor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0a00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3301,
        L"ComponentWidth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3302,
        L"HorizontalSubsampling");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x0600, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3303,
        L"ColorSiting");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3304,
        L"BlackReferenceLevel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3305,
        L"WhiteReferenceLevel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3306,
        L"ColorRange");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04180104, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3307,
        L"PaddingBits");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010501, 0x1000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3308,
        L"VerticalSubsampling");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3309,
        L"AlphaSamplingWidth");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0a00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x330B,
        L"ReversedByteOrder");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0600, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3401,
        L"PixelLayout");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3403,
        L"Palette");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0900, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3404,
        L"PaletteLayout");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010404, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3405,
        L"ScanningDirection");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0b00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3406,
        L"ComponentMaxRef");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0c00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3407,
        L"ComponentMinRef");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0d00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3408,
        L"AlphaMaxRef");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010503, 0x0e00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3409,
        L"AlphaMinRef");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0101, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3D03,
        L"SAudioSamplingRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x04),
        0x3D02,
        L"Locked");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020101, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3D04,
        L"AudioRefLevel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020101, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3D05,
        L"ElectroSpatial");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020101, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3D07,
        L"Channels");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020303, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x04),
        0x3D01,
        L"QuantizationBits");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020701, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3D0C,
        L"DialNorm");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020402, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3D06,
        L"SoundDescriptorCompression");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020302, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3D0A,
        L"BlockAlign");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020302, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3D0B,
        L"SequenceOffset");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020303, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3D09,
        L"AverageBPS");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020101, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x3D32,
        L"ChannelAssignment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0600, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D29,
        L"PeakEnvelopeVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D2A,
        L"PeakEnvelopeFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D2B,
        L"PointsPerPeakValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0900, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D2C,
        L"PeakEnvelopeBlockSize");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0a00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D2D,
        L"PeakChannels");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0b00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D2E,
        L"PeakFrames");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0c00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D2F,
        L"PeakOfPeaksPosition");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0d00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D30,
        L"PeakEnvelopeTimestamp");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04020301, 0x0e00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x3D31,
        L"PeakEnvelopeData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05020103, 0x0101, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3701,
        L"IsUniform");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06080201, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3702,
        L"IsContiguous");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3703,
        L"LeadingLines");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010302, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3704,
        L"TrailingLines");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05020103, 0x0102, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3705,
        L"JPEGTableID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030302, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3706,
        L"TIFFDescriptorSummary");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03030302, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3801,
        L"WAVEDescriptorSummary");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0108, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3901,
        L"FilmFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010802, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3902,
        L"FrameRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0103, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3903,
        L"PerforationsPerFrame");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0203, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3904,
        L"FilmAspectRatio");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0106, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3905,
        L"FilmDescriptorManufacturer");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0105, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3906,
        L"FilmDescriptorModel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0104, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3907,
        L"FilmGaugeFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100103, 0x0107, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3908,
        L"FilmBatchNumber");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0101, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3A01,
        L"FormFactor");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04010401, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3A02,
        L"VideoSignal");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x0d010101, 0x0101, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3A03,
        L"TapeFormat");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3A04,
        L"TapeDescriptorLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0401, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3A05,
        L"TapeDescriptorManufacturerID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3A06,
        L"TapeDescriptorModel");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0601, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3A07,
        L"TapeBatchNumber");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04100101, 0x0501, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3A08,
        L"TapeStock");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04090201, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4E11,
        L"MimeType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x04090300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x4E12,
        L"CharSet");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0109, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4D12,
        L"KLVDataType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x3B01,
        L"ByteOrder");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020110, 0x0204, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3B02,
        L"LastModified");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3B03,
        L"Content");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0202, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3B04,
        L"HeaderDictionary");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3B05,
        L"Version");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0604, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3B06,
        L"IdentificationList");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010201, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3B07,
        L"ObjectModelVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01020203, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3B09,
        L"OperationalPattern");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01020210, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3B0A,
        L"EssenceContainers");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01020210, 0x0202, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x05),
        0x3B0B,
        L"DescriptiveSchemes");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C01,
        L"CompanyName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0301, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C02,
        L"ProductName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C03,
        L"ProductVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0501, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C04,
        L"ProductVersionString");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C05,
        L"ProductID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020110, 0x0203, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C06,
        L"Date");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0a00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C07,
        L"ToolkitVersion");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0601, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C08,
        L"IdentificationPlatform");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05200701, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x3C09,
        L"GenerationAUID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01020101, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x4001,
        L"URLString");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01040102, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4101,
        L"TextLocatorName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01011510, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x4401,
        L"MobID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01030302, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x4402,
        L"MobName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0605, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4403,
        L"MobSlots");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020110, 0x0205, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4404,
        L"MobLastModified");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020110, 0x0103, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4405,
        L"CreationTime");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020102, 0x0c00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4406,
        L"MobUserComments");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4407,
        L"MobKLVData");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4409,
        L"MobAttributes");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05010108, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4408,
        L"UsageCode");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020201, 0x0105, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4501,
        L"DefaultFadeLength");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300201, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01),
        0x4502,
        L"DefFadeType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300403, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4503,
        L"DefFadeEditUnit");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x010a, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x08),
        0x4504,
        L"CompositionMobRendering");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0203, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4701,
        L"EssenceDescription");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070101, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4801,
        L"SlotID");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01070102, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4802,
        L"SlotName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0204, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4803,
        L"MobSlotSegment");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x01040103, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4804,
        L"PhysicalTrackNumber");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300402, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4901,
        L"EventMobSlotEditRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300405, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4B01,
        L"TimelineMobSlotEditRate");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0103, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4B02,
        L"SlotOrigin");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x010c, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4B03,
        L"MarkIn");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x0203, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4B04,
        L"MarkOut");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x07020103, 0x010d, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x07),
        0x4B05,
        L"UserPos");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0104, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4C01,
        L"Property");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x05300507, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4D01,
        L"ConstantValueValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0105, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4E01,
        L"Interpolation");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010104, 0x0606, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x4E02,
        L"PointList");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020102, 0x0901, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x5001,
        L"TaggedValueName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020102, 0x0a01, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x5003,
        L"TaggedValueValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010210, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x5101,
        L"KLVDataValue");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x000F,
        L"IntegerSize");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0010,
        L"IsSigned");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0900, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0011,
        L"StrongReferencedType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0a00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0012,
        L"WeakReferencedType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0b00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0013,
        L"ReferenceTargetSet");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0b00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0014,
        L"EnumElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0400, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0015,
        L"EnumElementNames");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0500, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0016,
        L"EnumElementValues");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0c00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0017,
        L"FixedArrayElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0018,
        L"ElementCount");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0d00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0019,
        L"VariableArrayElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0e00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001A,
        L"SetElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0f00, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001B,
        L"StringElementType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x1100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001C,
        L"MemberTypes");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0600, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001D,
        L"MemberNames");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x1200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001E,
        L"RenamedType");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x001F,
        L"ExtEnumElementNames");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03010203, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0020,
        L"ExtEnumElementValues");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x1300, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0005,
        L"MetaDefinitionIdentification");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x03020401, 0x0201, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0006,
        L"MetaDefinitionName");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x1401, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0007,
        L"MetaDefinitionDescription");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0700, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
        0x0003,
        L"ClassDefinitions");
    ADD_PROPERTYDEF_SYMBOL(
        LITERAL_AUID(0x06010107, 0x0800, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02),
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
        LITERAL_AUID(0x01010100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt8");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt16");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt32");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010400, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt64");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010500, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int8");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010600, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int16");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010700, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int32");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01010800, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int64");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01040100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Boolean");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010101, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ProductReleaseType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010102, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TapeFormatType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010103, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"VideoSignalType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010104, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TapeCaseType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010105, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ColorSitingType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010106, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EditHintType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010107, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"FadeType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010108, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"LayoutType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010109, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TCSource");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010a, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PulldownDirectionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010b, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PulldownKindType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010c, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EdgeType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010d, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"FilmType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010e, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"RGBAComponentKind");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x0201010f, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ReferenceType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010120, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AlphaTransparencyType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010121, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"FieldNumber");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010122, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ElectroSpatialFormulation");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010127, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SignalStandardType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02010128, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ScanningDirectionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010800, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt8Array8");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt8Array");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt8Array12");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010900, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt32Array");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int32Array");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010400, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Int64Array");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04020100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"RGBALayout");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010500, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"StringArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010600, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AUIDArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04030100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AUIDSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04030200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UInt32Set");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Rational");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01030100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"AUID");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01030200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"MobIDType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ProductVersionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"VersionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010400, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"RGBAComponent");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010500, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DateStruct");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010600, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TimeStruct");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x03010700, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TimeStamp");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01012001, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PositionType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01012002, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"LengthType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01012003, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"JPEGTableIDType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01012300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PhaseFrameType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01100200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"String");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020101, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationCategoryType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x01100100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Character");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04100200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Stream");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020102, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TransferCharacteristicType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020103, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginCategoryType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020104, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"UsageType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020105, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ColorPrimariesType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x02020106, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"CodingEquationsType");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04100100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataValue");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04010700, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PositionArray");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04100300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Indirect");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x04100400, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"Opaque");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContentStorageStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DictionaryStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EssenceDescriptorStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020400, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"NetworkLocatorStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020500, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationGroupStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020600, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SegmentStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020700, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SourceClipStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020800, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SourceReferenceStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020900, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ClassDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020a00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"CodecDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020b00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ComponentStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020c00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContainerDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020d00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ControlPointStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020e00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05020f00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EssenceDataStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021000, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"IdentificationStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"InterpolationDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"LocatorStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"MobStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021400, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"MobSlotStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021500, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021600, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021700, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021800, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021900, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PropertyDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021a00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TaggedValueStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021b00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TypeDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021c00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"KLVDataStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05021f00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DescriptiveFrameworkStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05022000, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"KLVDataDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05022100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TaggedValueDefinitionStrongReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ClassDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"CodecDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContainerDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050400, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050500, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"EssenceDataStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050600, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"InterpolationDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050700, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"MobStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050800, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050900, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050a00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050b00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PropertyDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050c00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TypeDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050d00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"KLVDataDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05050e00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TaggedValueDefinitionStrongReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ComponentStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ControlPointStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"IdentificationStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060400, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"LocatorStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060500, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"MobSlotStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060600, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SegmentStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060700, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"SourceReferenceStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060800, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TaggedValueStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060900, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"KLVDataStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05060a00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterStrongReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ClassDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ContainerDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010500, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"InterpolationDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010600, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"MobWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010700, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010800, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010900, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TypeDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010a00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010b00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"CodecDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05010c00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PropertyDefinitionWeakReference");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05030d00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionWeakReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05030e00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"ParameterDefinitionWeakReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05030f00, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PluginDefinitionWeakReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05031000, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"PropertyDefinitionWeakReferenceSet");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05040100, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"OperationDefinitionWeakReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05040200, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"TypeDefinitionWeakReferenceVector");
    ADD_METADEF_SYMBOL(
        LITERAL_AUID(0x05040300, 0x0000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01),
        L"DataDefinitionWeakReferenceVector");


    // Definition symbols

    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030202, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Picture");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x6f3c8ce1, 0x6cef, 0x11d2, 0x80, 0x7d, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_LegacyPicture");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x05cba731, 0x1daa, 0x11d3, 0x80, 0xad, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_Matte");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x05cba732, 0x1daa, 0x11d3, 0x80, 0xad, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_PictureWithMatte");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030202, 0x0200, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Sound");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x78e1ebe1, 0x6cef, 0x11d2, 0x80, 0x7d, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_LegacySound");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030201, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_Timecode");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x7f275e81, 0x77e5, 0x11d2, 0x80, 0x7f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"DataDef_LegacyTimecode");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xd2bb2af0, 0xd234, 0x11d2, 0x89, 0xee, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12),
        L"DataDef_Edgecode");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030201, 0x1000, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01),
        L"DataDef_DescriptiveMetadata");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x01030203, 0x0100, 0x0000, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x05),
        L"DataDef_Auxiliary");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x851419d0, 0x2e4f, 0x11d3, 0x8a, 0x5b, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"DataDef_Unknown");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x568fb761, 0x9458, 0x11d2, 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"CodecDef_None");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x90ac17c8, 0xe3e2, 0x4596, 0x9e, 0x9e, 0xa6, 0xdd, 0x1c, 0x70, 0xc8, 0x92),
        L"CodecDef_PCM");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x820f09b1, 0xeb9b, 0x11d2, 0x80, 0x9f, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"CodecDef_WAVE");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4b1c1a45, 0x03f2, 0x11d4, 0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"CodecDef_AIFC");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x18634f8c, 0x3bab, 0x11d3, 0xbf, 0xd6, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d),
        L"CodecDef_JPEG");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4e84045e, 0x0f29, 0x11d4, 0xa3, 0x59, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a),
        L"CodecDef_CDCI");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4e84045f, 0x0f29, 0x11d4, 0xa3, 0x59, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a),
        L"CodecDef_RGBA");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x1b31f3b1, 0x9450, 0x11d2, 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"CodecFlavour_None");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xaf4de587, 0x23d7, 0x4c7c, 0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11),
        L"CodecFlavour_LegacyDV_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xaf4de587, 0x23d7, 0x4c7d, 0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11),
        L"CodecFlavour_LegacyDV_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xaf4de587, 0x23d7, 0x4c7e, 0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11),
        L"CodecFlavour_IEC_DV_625_50");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xaf4de587, 0x23d7, 0x4c7f, 0xb3, 0x7b, 0xc1, 0xc1, 0x38, 0x70, 0xe7, 0x11),
        L"CodecFlavour_IEC_DV_525_60");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4313b572, 0xd8ba, 0x11d2, 0x80, 0x9b, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"ContainerDef_External");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4b1c1a46, 0x03f2, 0x11d4, 0x80, 0xfb, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"ContainerDef_OMF");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4313b571, 0xd8ba, 0x11d2, 0x80, 0x9b, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"ContainerDef_AAF");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x42464141, 0x000d, 0x4d4f, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff),
        L"ContainerDef_AAFMSS");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x4b464141, 0x000d, 0x4d4f, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff),
        L"ContainerDef_AAFKLV");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x58464141, 0x000d, 0x4d4f, 0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff),
        L"ContainerDef_AAFXML");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0d011301, 0x0101, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06),
        L"ContainerDef_RIFFWAVE");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0d011301, 0x0102, 0x0200, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x07),
        L"ContainerDef_JFIF");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0d011301, 0x0104, 0x0100, 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x06),
        L"ContainerDef_AIFFAIFC");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5b6c85a3, 0x0ede, 0x11d3, 0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"InterpolationDef_None");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5b6c85a4, 0x0ede, 0x11d3, 0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"InterpolationDef_Linear");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5b6c85a5, 0x0ede, 0x11d3, 0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"InterpolationDef_Constant");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5b6c85a6, 0x0ede, 0x11d3, 0x80, 0xa9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"InterpolationDef_BSpline");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x15829ec3, 0x1f24, 0x458a, 0x96, 0x0d, 0xc6, 0x5b, 0xb2, 0x3c, 0x2a, 0xa1),
        L"InterpolationDef_Log");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xc09153f7, 0xbd18, 0x4e5a, 0xad, 0x09, 0xcb, 0xdd, 0x65, 0x4f, 0xa0, 0x01),
        L"InterpolationDef_Power");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0c3bea40, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoDissolve");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0c3bea44, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_SMPTEVideoWipe");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9d2ea890, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoSpeedControl");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9d2ea891, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoRepeat");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xf1db0f32, 0x8d64, 0x11d3, 0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"OperationDef_Flip");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xf1db0f34, 0x8d64, 0x11d3, 0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"OperationDef_Flop");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xf1db0f33, 0x8d64, 0x11d3, 0x80, 0xdf, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"OperationDef_FlipFlop");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x86f5711e, 0xee72, 0x450c, 0xa1, 0x18, 0x17, 0xcf, 0x3b, 0x17, 0x5d, 0xff),
        L"OperationDef_VideoPosition");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xf5826680, 0x26c5, 0x4149, 0x85, 0x54, 0x43, 0xd3, 0xc7, 0xa3, 0xbc, 0x09),
        L"OperationDef_VideoCrop");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x2e0a119d, 0xe6f7, 0x4bee, 0xb5, 0xdc, 0x6d, 0xd4, 0x29, 0x88, 0x68, 0x7e),
        L"OperationDef_VideoScale");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xf2ca330d, 0x8d45, 0x4db4, 0xb1, 0xb5, 0x13, 0x6a, 0xb0, 0x55, 0x58, 0x6f),
        L"OperationDef_VideoRotate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x21d5c51a, 0x8acb, 0x46d5, 0x93, 0x92, 0x5c, 0xae, 0x64, 0x0c, 0x88, 0x36),
        L"OperationDef_VideoCornerPinning");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x14db900e, 0xd537, 0x49f6, 0x88, 0x9b, 0x01, 0x25, 0x68, 0xfc, 0xc2, 0x34),
        L"OperationDef_VideoAlphaWithinVideoKey");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xe599cb0f, 0xba5f, 0x4192, 0x93, 0x56, 0x51, 0xeb, 0x19, 0xc0, 0x85, 0x89),
        L"OperationDef_VideoSeparateAlphaKey");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x38ff7903, 0x69e5, 0x476b, 0xbe, 0x5a, 0xea, 0xfc, 0x20, 0x00, 0xf0, 0x11),
        L"OperationDef_VideoLuminanceKey");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x30a315c2, 0x71e5, 0x4e82, 0xa4, 0xef, 0x05, 0x13, 0xee, 0x05, 0x6b, 0x65),
        L"OperationDef_VideoChromaKey");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9d2ea894, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_MonoAudioGain");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9d2ea893, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_MonoAudioPan");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0c3bea41, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_MonoAudioDissolve");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x2311bd90, 0xb5da, 0x4285, 0xaa, 0x3a, 0x85, 0x52, 0x84, 0x87, 0x79, 0xb3),
        L"OperationDef_TwoParameterMonoAudioDissolve");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x1575e350, 0xfca3, 0x11d2, 0x8a, 0x2a, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_Unknown");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0c3bea43, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoFadeToBlack");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0a3c75e0, 0xfd82, 0x11d2, 0x8a, 0x2b, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_PictureWithMate");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9d2ea892, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_VideoFrameToMask");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x0c3bea42, 0xfc05, 0x11d2, 0x8a, 0x29, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_StereoAudioDissolve");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9d2ea895, 0x0968, 0x11d3, 0x8a, 0x38, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_StereoAudioGain");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8d896ad0, 0x2261, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"OperationDef_MonoAudioMixdown");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xe4962320, 0x2267, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_Level");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xe4962323, 0x2267, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEWipeNumber");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba0, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEReverse");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x72559a80, 0x24d7, 0x11d3, 0x8a, 0x50, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SpeedRatio");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xc573a510, 0x071a, 0x454f, 0xb6, 0x17, 0xad, 0x6a, 0xe6, 0x90, 0x54, 0xc2),
        L"ParameterDef_PositionOffsetX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x82e27478, 0x1336, 0x4ea3, 0xbc, 0xb9, 0x6b, 0x8f, 0x17, 0x86, 0x4c, 0x42),
        L"ParameterDef_PositionOffsetY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xd47b3377, 0x318c, 0x4657, 0xa9, 0xd8, 0x75, 0x81, 0x1b, 0x6d, 0xc3, 0xd1),
        L"ParameterDef_CropLeft");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5ecc9dd5, 0x21c1, 0x462b, 0x9f, 0xec, 0xc2, 0xbd, 0x85, 0xf1, 0x40, 0x33),
        L"ParameterDef_CropRight");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8170a539, 0x9b55, 0x4051, 0x9d, 0x4e, 0x46, 0x59, 0x8d, 0x01, 0xb9, 0x14),
        L"ParameterDef_CropTop");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x154ba82b, 0x990a, 0x4c80, 0x91, 0x01, 0x30, 0x37, 0xe2, 0x88, 0x39, 0xa1),
        L"ParameterDef_CropBottom");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8d568129, 0x847e, 0x11d5, 0x93, 0x5a, 0x50, 0xf8, 0x57, 0xc1, 0x00, 0x00),
        L"ParameterDef_ScaleX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x8d56812a, 0x847e, 0x11d5, 0x93, 0x5a, 0x50, 0xf8, 0x57, 0xc1, 0x00, 0x00),
        L"ParameterDef_ScaleY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x062cfbd8, 0xf4b1, 0x4a50, 0xb9, 0x44, 0xf3, 0x9e, 0x2f, 0xc7, 0x3c, 0x17),
        L"ParameterDef_Rotation");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x72a3b4a2, 0x873d, 0x4733, 0x90, 0x52, 0x9f, 0x83, 0xa7, 0x06, 0xca, 0x5b),
        L"ParameterDef_PinTopLeftX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x29e4d78f, 0xa502, 0x4ebb, 0x8c, 0x07, 0xed, 0x5a, 0x03, 0x20, 0xc1, 0xb0),
        L"ParameterDef_PinTopLeftY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xa95296c0, 0x1ed9, 0x4925, 0x84, 0x81, 0x20, 0x96, 0xc7, 0x2e, 0x81, 0x8d),
        L"ParameterDef_PinTopRightX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xce1757ae, 0x7a0b, 0x45d9, 0xb3, 0xf3, 0x36, 0x86, 0xad, 0xff, 0x1e, 0x2d),
        L"ParameterDef_PinTopRightY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x08b2bc81, 0x9b1b, 0x4c01, 0xba, 0x73, 0xbb, 0xa3, 0x55, 0x4e, 0xd0, 0x29),
        L"ParameterDef_PinBottomLeftX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xc163f2ff, 0xcd83, 0x4655, 0x82, 0x6e, 0x37, 0x24, 0xab, 0x7f, 0xa0, 0x92),
        L"ParameterDef_PinBottomLeftY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x53bc5884, 0x897f, 0x479e, 0xb8, 0x33, 0x19, 0x1f, 0x86, 0x92, 0x10, 0x0d),
        L"ParameterDef_PinBottomRightX");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x812fb15b, 0x0b95, 0x4406, 0x87, 0x8d, 0xef, 0xaa, 0x1c, 0xff, 0xc1, 0x29),
        L"ParameterDef_PinBottomRightY");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xa2667f65, 0x65d8, 0x4abf, 0xa1, 0x79, 0x0b, 0x9b, 0x93, 0x41, 0x39, 0x49),
        L"ParameterDef_AlphaKeyInvertAlpha");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x21ed5b0f, 0xb7a0, 0x43bc, 0xb7, 0x79, 0xc4, 0x7f, 0x85, 0xbf, 0x6c, 0x4d),
        L"ParameterDef_LumKeyLevel");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xcbd39b25, 0x3ece, 0x441e, 0xba, 0x2c, 0xda, 0x47, 0x3a, 0xb5, 0xcc, 0x7c),
        L"ParameterDef_LumKeyClip");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xe4962321, 0x2267, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_Amplitude");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0xe4962322, 0x2267, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_Pan");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9e610007, 0x1be2, 0x41e1, 0xbb, 0x11, 0xc9, 0x5d, 0xe9, 0x96, 0x4d, 0x03),
        L"ParameterDef_OutgoingLevel");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x48cea642, 0xa8f9, 0x455b, 0x82, 0xb3, 0x86, 0xc8, 0x14, 0xb7, 0x97, 0xc7),
        L"ParameterDef_IncomingLevel");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba1, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTESoft");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba2, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEBorder");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba3, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEPosition");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba4, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEModulator");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba5, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEShadow");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba6, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTETumble");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba7, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTESpotlight");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba8, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEReplicationH");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894ba9, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTEReplicationV");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9c894baa, 0x2277, 0x11d3, 0x8a, 0x4c, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_SMPTECheckerboard");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x5f1c2560, 0x2415, 0x11d3, 0x8a, 0x4f, 0x00, 0x50, 0x04, 0x0e, 0xf7, 0xd2),
        L"ParameterDef_PhaseOffset");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x3d1dd891, 0xe793, 0x11d2, 0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"Platform_Independent");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x9fdef8c1, 0xe847, 0x11d2, 0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"Engine_None");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x69c870a1, 0xe793, 0x11d2, 0x80, 0x9e, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f),
        L"PluginAPI_EssenceAccess");
    ADD_DEF_SYMBOL_ID(
        LITERAL_AUID(0x56905e0b, 0x537d, 0x11d4, 0xa3, 0x6c, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x6a),
        L"PluginCategory_Codec");


        
    // Extendible enumerations
    
    OMUniqueObjectIdentification id;
    OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>* values;
    
    // OperationCategoryType
    values = new OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>;
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020101, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ss->addExtEnum(id, values);
    
    // TransferCharacteristicType
    values = new OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>;
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020102, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0101, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0102, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0103, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0105, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0106, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ss->addExtEnum(id, values);
    
    // PluginCategoryType
    values = new OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>;
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020103, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0200, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0300, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x0D010102, 0x0101, 0x0400, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ss->addExtEnum(id, values);

    // UsageType
    values = new OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>;
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
    ss->addExtEnum(id, values);

    // ColorPrimariesType
    values = new OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>;
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020105, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0301, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x06));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0302, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x06));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0303, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x06));
    ss->addExtEnum(id, values);

    // CodingEquationsType
    values = new OMSet<OMUniqueObjectIdentification, OMUniqueObjectIdentification>;
    SET_EXT_ENUM_ID(
        LITERAL_AUID(0x02020106, 0x0000, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0201, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0202, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ADD_EXT_ENUM_VALUE(
        LITERAL_AUID(0x04010101, 0x0203, 0x0000, 0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01));
    ss->addExtEnum(id, values);
    
    
    return ss;
}




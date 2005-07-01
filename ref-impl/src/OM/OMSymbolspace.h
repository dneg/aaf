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

#ifndef __OMSYMBOLSPACE_H__
#define __OMSYMBOLSPACE_H__

#include "OMDataTypes.h"
#include "OMWString.h"
#include "OMSet.h"
#include "OMStorable.h"
#include "OMVector.h"


#define XML_MAX_BASELINE_SYMBOL_SIZE                     128


class OMDictionary;
class OMXMLStorage;
class OMXMLReader;
class OMXMLWriter;
class OMMetaDefinition;
class OMClassDefinition;
class OMPropertyDefinition;
class OMType;
class OMCharacterType;
class OMEnumeratedType;
class OMExtEnumeratedType;
class OMFixedArrayType;
class OMIndirectType;
class OMIntType;
class OMOpaqueType;
class OMRecordType;
class OMRenamedType;
class OMSetType;
class OMStreamType;
class OMStringType;
class OMStrongObjectReferenceType;
class OMVariableArrayType;
class OMWeakObjectReferenceType;

class MetaDef;


class OMSymbolspace
{
public:
    OMSymbolspace(OMXMLStorage* store);
    OMSymbolspace(OMXMLStorage* store, OMUniqueObjectIdentification id, const wchar_t* uri, 
        const wchar_t* preferredPrefix, const wchar_t* description);
    ~OMSymbolspace();

    bool isEmpty();
    
    OMUniqueObjectIdentification getId() const;
    const wchar_t* getURI() const;
    const wchar_t* getPreferredPrefix() const;
    const wchar_t* getPrefix() const;
    const wchar_t* getDescription() const;
    
    const wchar_t* getMetaDefSymbol(OMUniqueObjectIdentification id) const;
    OMUniqueObjectIdentification getMetaDefId(const wchar_t* symbol) const;
    OMPropertyId getPropertyDefId(const wchar_t* symbol) const;
    const wchar_t* getDefSymbol(OMUniqueObjectIdentification id);
    OMUniqueObjectIdentification getDefId(const wchar_t* symbol) const;
    bool isKnownExtEnumElement(OMUniqueObjectIdentification elementOf, 
        OMUniqueObjectIdentification value) const;
    
    void addClassDef(OMClassDefinition* classDef);
    void addTypeDef(OMType* typeDef);
    void addPropertyDef(OMClassDefinition* classDef, OMPropertyDefinition* propertyDef);
    void addExtEnumElement(OMUniqueObjectIdentification id, const wchar_t* name,
        OMUniqueObjectIdentification value);
    
    void save();
    void restore(OMDictionary* dictionary);
    void registerDeferredDefs(OMDictionary* dictionary);

    void resetForWriting();
    
    static OMSymbolspace* createDefaultExtSymbolspace(OMXMLStorage* storage, 
        OMUniqueObjectIdentification id);
    static OMSymbolspace* createV11Symbolspace(OMXMLStorage* storage);
    
    static const wchar_t* getBaselineURI();

public:
    // called by OMXMLStorage
    void setPrefix(const wchar_t* prefix);
    
private:
    void initialise(OMUniqueObjectIdentification id, const wchar_t* uri, 
        const wchar_t* preferredPrefix, const wchar_t* description);

    OMXMLWriter* getWriter();
    OMXMLReader* getReader();

    void addMetaDefSymbol(OMUniqueObjectIdentification id, const wchar_t* symbol);
    void addPropertyDefSymbol(OMUniqueObjectIdentification id, OMPropertyId localId, const wchar_t* symbol);
    const wchar_t* createSymbolForClass(OMUniqueObjectIdentification id, const wchar_t* name);
    const wchar_t* createSymbolForProperty(OMUniqueObjectIdentification id, OMPropertyId localId,
        const wchar_t* name);
    const wchar_t* createSymbolForType(OMUniqueObjectIdentification id, const wchar_t* name);
    wchar_t* createSymbol(const wchar_t* name);
    void addDefSymbol(OMUniqueObjectIdentification id, const wchar_t* symbol);

    struct ExtEnumElement
    {
        OMUniqueObjectIdentification elementOf;
        OMWString name;
        OMUniqueObjectIdentification value;
    };
    
    void saveMetaDef(OMMetaDefinition* metaDef);
    void saveClassDef(OMClassDefinition* classDef);
    void savePropertyDef(OMClassDefinition* ownerClassDef, OMPropertyDefinition* propertyDef);
    void saveTypeDef(OMType* typeDef);
    void saveCharacterTypeDef(OMCharacterType* typeDef);
    void saveEnumeratedTypeDef(OMEnumeratedType* typeDef);
    void saveExtEnumeratedTypeDef(OMExtEnumeratedType* typeDef);
    void saveFixedArrayTypeDef(OMFixedArrayType* typeDef);
    void saveIndirectTypeDef(OMIndirectType* typeDef);
    void saveIntTypeDef(OMIntType* typeDef);
    void saveOpaqueTypeDef(OMOpaqueType* typeDef);
    void saveRecordTypeDef(OMRecordType* typeDef);
    void saveRenamedTypeDef(OMRenamedType* typeDef);
    void saveSetTypeDef(OMSetType* typeDef);
    void saveStreamTypeDef(OMStreamType* typeDef);
    void saveStringTypeDef(OMStringType* typeDef);
    void saveStrongObjectReferenceTypeDef(OMStrongObjectReferenceType* typeDef);
    void saveVariableArrayTypeDef(OMVariableArrayType* typeDef);
    void saveWeakObjectReferenceTypeDef(OMWeakObjectReferenceType* typeDef);
    void saveExtEnumElement(ExtEnumElement* value);

    void restoreMetaDictDefinition(OMDictionary* dictionary);
    bool restoreMetaDef(MetaDef* metaDef);
    void restoreClassDef(OMDictionary* dictionary);
    void restorePropertyDef(OMDictionary* dictionary);
    void restoreCharacterTypeDef(OMDictionary* dictionary);
    void restoreEnumeratedTypeDef(OMDictionary* dictionary);
    void restoreExtEnumeratedTypeDef(OMDictionary* dictionary);
    void restoreFixedArrayTypeDef(OMDictionary* dictionary);
    void restoreIndirectTypeDef(OMDictionary* dictionary);
    void restoreIntTypeDef(OMDictionary* dictionary);
    void restoreOpaqueTypeDef(OMDictionary* dictionary);
    void restoreRecordTypeDef(OMDictionary* dictionary);
    void restoreRenamedTypeDef(OMDictionary* dictionary);
    void restoreSetTypeDef(OMDictionary* dictionary);
    void restoreStreamTypeDef(OMDictionary* dictionary);
    void restoreStringTypeDef(OMDictionary* dictionary);
    void restoreStrongObjectReferenceTypeDef(OMDictionary* dictionary);
    void restoreVariableArrayTypeDef(OMDictionary* dictionary);
    void restoreWeakObjectReferenceTypeDef(OMDictionary* dictionary);
    void restoreExtEnumElement(OMDictionary* dictionary);
    
    OMUniqueObjectIdentification restoreMetaDefAUID(const wchar_t* idStr);
    wchar_t* saveMetaDefAUID(OMUniqueObjectIdentification id);
    
    OMPropertyTag getClassDefsTag(OMDictionary* dictionary);
    OMPropertyTag getTypeDefsTag(OMDictionary* dictionary);
    
    static const wchar_t* _baselineURI;
    static const OMUniqueObjectIdentification _baselineId;
    
    bool            _isInitialised;
    OMXMLStorage*   _store;
    
    OMUniqueObjectIdentification    _id;
    wchar_t*        _uri;
    wchar_t*        _preferredPrefix;
    wchar_t*        _prefix;
    wchar_t*        _description;
    
    OMUInt32        _uniqueSymbolSuffix;
    
    // meta-definition symbol/identification which are defined in this symbolspace
    OMSet<OMUniqueObjectIdentification, OMWString>     _idToSymbol; 
    OMSet<OMWString, OMUniqueObjectIdentification>     _symbolToId;
    OMSet<OMUniqueObjectIdentification, OMPropertyId>  _idToLocalId;

    // definition symbol/identification which are defined in this symbolspace
    OMSet<OMUniqueObjectIdentification, OMWString> _idToDefSymbol;
    OMSet<OMWString, OMUniqueObjectIdentification> _defSymbolToId;

    // definitions, including extendible enumeration extensions, which are part
    // of this NON-BASELINE symbolspace and which must be saved to the document
    OMVector<OMClassDefinition*> _classDefs;
    OMVector<OMType*> _typeDefs;
    struct PropertyPair
    {
        OMClassDefinition* ownerClassDef;
        OMPropertyDefinition* propertyDef;
    };
    OMVector<PropertyPair*> _propertyDefs;

    class ExtEnumId
    {
    public:
        ExtEnumId();
        ExtEnumId(OMUniqueObjectIdentification elementOf, 
            OMUniqueObjectIdentification value);
        ExtEnumId(const ExtEnumId& id);
        ~ExtEnumId();
        
        ExtEnumId& operator=(const ExtEnumId& rhs);
        bool operator==(const ExtEnumId& rhs) const;
        bool operator!=(const ExtEnumId& rhs) const;
        bool operator<(const ExtEnumId& rhs) const;
        
    private:
        OMUniqueObjectIdentification _elementOf;
        OMUniqueObjectIdentification _value;
    };
    OMSet<ExtEnumId, ExtEnumElement*> _extEnumElements;

    struct RegisterPropertyPair
    {
        OMUniqueObjectIdentification ownerClassId;
        OMPropertyDefinition* propertyDef;
    };
    // definitions that must be registered last when restoring
    OMVector<RegisterPropertyPair*> _propertyDefsForRegistration;
    OMVector<ExtEnumElement*> _extEnumElementsForRegistration;
    
};


#endif

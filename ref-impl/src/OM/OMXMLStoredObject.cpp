//=---------------------------------------------------------------------=
//
// $Id$
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
// Portions created by British Broadcasting Corporation are
// Copyright 2005, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=

#include "OMXMLStoredObject.h"

#include "OMObjectReference.h"
#include "OMContainerElement.h"

#include "OMDataStream.h"
#include "OMStrongReference.h"
#include "OMStrongReferenceSet.h"
#include "OMStrongReferenceVector.h"
#include "OMWeakReference.h"
#include "OMWeakReferenceSet.h"
#include "OMWeakReferenceVector.h"

#include "OMCachedDiskRawStorage.h"
#include "OMXMLStoredStream.h"
#include "OMPropertySetIterator.h"
#include "OMDataVector.h"
#include "OMArrayType.h"
#include "OMDataContainerIterator.h"
#include "OMDataSet.h"
#include "OMSetType.h"

#include "OMClassFactory.h"
#include "OMClassDefinition.h"

#include "OMUtilities.h"

#include "OMPropertyDefinition.h"

#include "OMXMLUtilities.h"
#include "OMDictionary.h"
#include "OMExceptions.h"
#include "OMRootStorable.h"
#include "OMPropertyTable.h"
#include "OMList.h"
#include "OMListIterator.h"

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

#include "OMIntegerType.h"
#include "OMUniqueObjectIdentType.h"

#include "OMReferenceContainerIter.h"


// class definition identifications
static const OMUniqueObjectIdentification ClassID_Header =
    {0x0d010101, 0x0101, 0x2f00, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

// type definition identifications
static const OMUniqueObjectIdentification TypeID_Boolean =
    {0x01040100, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_DataValue =
    {0x04100100, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_StringArray =
    {0x04010500, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_String =
    {0x01100200, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_MobIDType =
    {0x01030200, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_AUID =
    {0x01030100, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_DateStruct =
    {0x03010500, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_TimeStruct =
    {0x03010600, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_TimeStamp =
    {0x03010700, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_VersionType =
    {0x03010300, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};
static const OMUniqueObjectIdentification TypeID_Rational =
    {0x03010100, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x04, 0x01, 0x01}};

// property definition identifications
static const OMUniqueObjectIdentification PropID_MetaDefinition_Identification =
    {0x06010107, 0x1300, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02}};
static const OMUniqueObjectIdentification PropID_DefinitionObject_Identification =
    {0x01011503, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02}};
static const OMUniqueObjectIdentification PropID_OperationGroup_Parameters =
    {0x06010104, 0x060a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02}};
static const OMUniqueObjectIdentification PropID_Header_Dictionary =
    {0x06010104, 0x0202, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02}};

// property definition local identifications
static const OMPropertyId PropLocalID_Root_MetaDictionary = 0x0001;
static const OMPropertyId PropLocalID_Root_Header = 0x0002;
static const OMPropertyId PropLocalID_MetaDictionary_ClassDefinitions = 0x0003;
static const OMPropertyId PropLocalID_Header_ByteOrder = 0x3B01;
static const OMPropertyId PropLocalID_Header_Dictionary = 0x3B04;
static const OMPropertyId PropLocalID_Header_Version = 0x3B05;
static const OMPropertyId PropLocalID_Header_IdentificationList = 0x3B06;
static const OMPropertyId PropLocalID_Identification_GenerationAUID = 0x3C09;
static const OMPropertyId PropLocalID_DefinitionObject_Identification = 0x1B01;
static const OMPropertyId PropLocalID_Parameter_Definition = 0x4C01;

// global attribute names
static const wchar_t* UniqueId_AttrName = L"uid";
static const wchar_t* StreamRef_AttrName = L"stream";
static const wchar_t* ByteOrder_AttrName = L"byteOrder";
static const wchar_t* ActualType_AttrName = L"actualType";
static const wchar_t* AAFEscape_AttrName = L"escaped";


// property symbols
static const wchar_t* HeaderDictionarySymbol = L"HeaderDictionary";
static const wchar_t* DefObjectIdentificationSymbol = L"DefinitionObjectIdentification"; 


typedef enum
{
    UNKNOWN_CAT = 0,
    CHARACTER_CAT,
    ENUM_CAT,
    EXT_ENUM_CAT,
    FIXED_ARRAY_CAT,
    INDIRECT_CAT,
    INTEGER_CAT,
    OPAQUE_CAT,
    RENAMED_CAT,
    RECORD_CAT,
    SET_CAT,
    STREAM_CAT,
    STRING_CAT,
    STRONG_REF_CAT,
    VAR_ARRAY_CAT,
    WEAK_REF_CAT
} TypeCategory;
    
class TypeCategoryVisitor : public OMTypeVisitor
{
public:
    TypeCategoryVisitor(const OMType* type) 
    : _category(UNKNOWN_CAT), _elementCategory(UNKNOWN_CAT)
    {
        type->accept(*this);
    }
    virtual ~TypeCategoryVisitor() 
    {}
    
    TypeCategory get()
    {
        return _category;
    }
    TypeCategory getElement()
    {
        return _elementCategory;
    }
    
    virtual void visitCharacterType(const OMCharacterType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = CHARACTER_CAT;
        }
        else
        {
            _elementCategory = CHARACTER_CAT;
        }
    }
    virtual void visitEnumeratedType(const OMEnumeratedType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = ENUM_CAT;
        }
        else
        {
            _elementCategory = ENUM_CAT;
        }
    }
    virtual void visitExtendibleEnumeratedType(const OMExtendibleEnumeratedType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = EXT_ENUM_CAT;
        }
        else
        {
            _elementCategory = EXT_ENUM_CAT;
        }
    }
    virtual void visitFixedArrayType(const OMFixedArrayType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = FIXED_ARRAY_CAT;
        }
        else
        {
            _elementCategory = FIXED_ARRAY_CAT;
        }
    }
    virtual void visitIndirectType(const OMIndirectType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = INDIRECT_CAT;
        }
        else
        {
            _elementCategory = INDIRECT_CAT;
        }
    }
    virtual void visitIntegerType(const OMIntegerType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = INTEGER_CAT;
        }
        else
        {
            _elementCategory = INTEGER_CAT;
        }
    }
    virtual void visitOpaqueType(const OMOpaqueType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = OPAQUE_CAT;
        }
        else
        {
            _elementCategory = OPAQUE_CAT;
        }
    }
    virtual void visitRenamedType(const OMRenamedType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = RENAMED_CAT;
        }
        else
        {
            _elementCategory = RENAMED_CAT;
        }
    }
    virtual void visitRecordType(const OMRecordType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = RECORD_CAT;
        }
        else
        {
            _elementCategory = RECORD_CAT;
        }
    }
    virtual void visitSetType(const OMSetType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = SET_CAT;
        }
        else
        {
            _elementCategory = SET_CAT;
        }
    }
    virtual void visitStreamType(const OMStreamType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = STREAM_CAT;
        }
        else
        {
            _elementCategory = STREAM_CAT;
        }
    }
    virtual void visitStringType(const OMStringType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = STRING_CAT;
        }
        else
        {
            _elementCategory = STRING_CAT;
        }
    }
    virtual void visitStrongObjectReferenceType(const OMStrongObjectReferenceType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = STRONG_REF_CAT;
        }
        else
        {
            _elementCategory = STRONG_REF_CAT;
        }
    }
    virtual void visitVaryingArrayType(const OMVaryingArrayType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = VAR_ARRAY_CAT;
        }
        else
        {
            _elementCategory = VAR_ARRAY_CAT;
        }
    }
    virtual void visitWeakObjectReferenceType(const OMWeakObjectReferenceType* type)
    {
        if (_category == UNKNOWN_CAT)
        {
            _category = WEAK_REF_CAT;
        }
        else
        {
            _elementCategory = WEAK_REF_CAT;
        }
    }
    
private:    
    TypeCategory _category;
    TypeCategory _elementCategory;
};



OMXMLStoredObject*
OMXMLStoredObject::openRead(OMDiskRawStorage* storage)
{
    TRACE("OMXMLStoredObject::openRead");
    PRECONDITION("Compatible raw storage access mode", storage->isReadable());
    OMXMLStoredObject* result = new OMXMLStoredObject(
        new OMXMLStorage(storage, OMXMLStorage::READ_MODE), true);
    return result;
}

OMXMLStoredObject*
OMXMLStoredObject::openModify(OMDiskRawStorage* storage)
{
    TRACE("OMXMLStoredObject::openModify");
    PRECONDITION("Compatible raw storage access mode",
        storage->isReadable() && storage->isWritable());
    PRECONDITION("Compatible raw storage", storage->isPositionable());
    OMXMLStoredObject* result = new OMXMLStoredObject(
        new OMXMLStorage(storage, OMXMLStorage::EXISTING_MODIFY_MODE), true);
    return result;
}

OMXMLStoredObject*
OMXMLStoredObject::createWrite(OMDiskRawStorage* storage)
{
    TRACE("OMXMLStoredObject::createWrite");
    PRECONDITION("Compatible raw storage access mode", storage->isWritable());
    OMXMLStoredObject* result = new OMXMLStoredObject(
        new OMXMLStorage(storage, OMXMLStorage::WRITE_MODE), true);
    return result;
}

OMXMLStoredObject*
OMXMLStoredObject::createModify(OMDiskRawStorage* storage)
{
    TRACE("OMXMLStoredObject::createModify");
    PRECONDITION("Compatible raw storage access mode",
                 storage->isReadable() && storage->isWritable());
    PRECONDITION("Compatible raw storage", storage->isPositionable());
    OMXMLStoredObject* result = new OMXMLStoredObject(
        new OMXMLStorage(storage, OMXMLStorage::NEW_MODIFY_MODE), true);
    return result;
}


OMXMLStoredObject::OMXMLStoredObject(OMXMLStorage* store, bool isRoot)
: _store(store), _isRoot(isRoot)
{
    TRACE("OMXMLStoredObject::OMXMLStoredObject");
}

OMXMLStoredObject::~OMXMLStoredObject(void)
{
    TRACE("OMXMLStoredObject::~OMXMLStoredObject");

    if (_isRoot)
    {
        delete _store;
    }
}

OMStoredObject*
OMXMLStoredObject::create(const wchar_t* /* name */)
{
    TRACE("OMXMLStoredObject::create");

    return new OMXMLStoredObject(_store, false);
}

OMStoredObject*
OMXMLStoredObject::open(const wchar_t* /*name*/)
{
    TRACE("OMXMLStoredObject::open");

    return new OMXMLStoredObject(_store, false);
}

void
OMXMLStoredObject::close(void)
{
    TRACE("OMXMLStoredObject::close");
    if (_isRoot && _store->haveWriter())
    {
        getWriter()->synchronize();
    }
}

OMByteOrder
OMXMLStoredObject::byteOrder(void) const
{
    TRACE("OMXMLStoredObject::byteOrder");
    return unspecified;
}

void
OMXMLStoredObject::save(OMFile& file)
{
    TRACE("OMXMLStoredObject::save(OMFile)");

    if (_store->mode() == OMXMLStorage::EXISTING_MODIFY_MODE ||
        _store->mode() == OMXMLStorage::NEW_MODIFY_MODE)
    {
        _store->resetForWriting();
    }
    ASSERT("XML document is set for writing", _store->haveWriter());
    
    try
    {
        getWriter()->writeDocumentStart();
        getWriter()->writeComment(L"Generated by the AAF SDK");
        
        OMVector<OMDataStream*> dataStreams;
        getDataStreams(file.clientRoot(), dataStreams);
        if (dataStreams.count() > 0)
        {
            getWriter()->writeText(L"<!DOCTYPE AAF [\n");
        
            // get unique set of notations
            OMSet<OMUniqueObjectIdentification, OMDataStream*> notations;
            OMUInt32 i;
            for (i = 0; i < dataStreams.count(); i++)
            {
                if (!notations.contains(dataStreams.getAt(i)->type()->identification()))
                {
                    notations.insert(dataStreams.getAt(i)->type()->identification(),
                        dataStreams.getAt(i));
                }
            }
            // write data stream notations
            OMSetIterator<OMUniqueObjectIdentification, OMDataStream*> notationsIter(notations, OMBefore);
            while (++notationsIter)
            {
                getWriter()->writeText(L"<!NOTATION ");
                const wchar_t* notationName = _store->getDataStreamNotationName(
                    notationsIter.value()->type()->identification());
                getWriter()->writeText(notationName);
        
                getWriter()->writeText(L" SYSTEM \"");
                wchar_t uri[XML_MAX_AUID_URI_SIZE];
                auidToURI(notationsIter.key(), uri);
                getWriter()->writeText(uri);
                getWriter()->writeText(L"\">\n");
            }
            
            // write data stream entities
            for (i = 0; i < dataStreams.count(); i++)
            {
                getWriter()->writeText(L"<!ENTITY ");
                const wchar_t* entityName = _store->getDataStreamEntityName(
                    dataStreams.getAt(i));
                getWriter()->writeText(entityName);
        
                getWriter()->writeText(L" SYSTEM \"");
                const wchar_t* entityValue = _store->getDataStreamEntityValue(
                    dataStreams.getAt(i));
                getWriter()->writeText(entityValue);
                getWriter()->writeText(L"\"");
                
                getWriter()->writeText(L" NDATA ");
                const wchar_t* notationName = _store->getDataStreamNotationName(
                    dataStreams.getAt(i)->type()->identification());
                getWriter()->writeText(notationName);
                getWriter()->writeText(L">\n");
            }
            
            getWriter()->writeText(L"]>\n");
        }
        
        OMSymbolspace* extSymbolspace = _store->getDefaultExtSymbolspace();
        if (extSymbolspace == 0)
        {
            OMUniqueObjectIdentification extId = getExtensionSymbolspaceId(file);
            extSymbolspace = _store->createDefaultExtSymbolspace(extId);
        }
        registerExtensions(file, extSymbolspace);
        

        getWriter()->writeElementStart(getBaselineURI(), L"AAF");
        getWriter()->declareNamespace(getBaselineURI(), 0);
        
        bool haveExtensions = false;
        OMSet<OMWString, OMSymbolspace*>& symbolspaces = _store->getSymbolspaces();
        OMSetIterator<OMWString, OMSymbolspace*> iterS(symbolspaces, OMBefore);
        while (++iterS)
        {
            OMSymbolspace* symbolspace = iterS.value();
            if (_store->isBaselineSymbolspace(symbolspace) || !symbolspace->isEmpty())
            {
                getWriter()->declareNamespace(symbolspace->getURI(), symbolspace->getPrefix());
                if (!_store->isBaselineSymbolspace(symbolspace))
                {
                    haveExtensions = true;
                }
            }
        }

        getWriter()->writeAttribute(0, L"version", _store->getBaselineVersion());

        if (haveExtensions)
        {
            getWriter()->writeElementStart(getBaselineURI(), L"Extensions");
            iterS.reset(OMBefore);
            while (++iterS)
            {
                OMSymbolspace* symbolspace = iterS.value();
                if (!_store->isBaselineSymbolspace(symbolspace) && !symbolspace->isEmpty())
                {
                    symbolspace->save();
                }
            }
            getWriter()->writeElementEnd();
        }
        
        file.clientRoot()->save();
        
        getWriter()->writeDocumentEnd();
    }
    catch (OMException& ex)
    {
        if (ex.name() != 0)
        {
            printf("Exception: %s\n", ex.name());
        }
        throw;
    }
}

void
OMXMLStoredObject::save(OMStorable& object)
{
    TRACE("OMXMLStoredObject::save(OMStorable)");

    const wchar_t* symbolspace;
    const wchar_t* symbol;
    if (_store->getMetaDefSymbol(object.classId(), &symbolspace, &symbol))
    {
        getWriter()->writeElementStart(symbolspace, symbol);
    }
    else
    {
        ASSERT("Object class definition registered in MetaDictionary and Symbolspace", false);
    }

    if (_store->haveForwardedObjectSetId())
    {
        wchar_t* id = _store->getForwardedObjectSetId();
        getWriter()->writeAttribute(getBaselineURI(), UniqueId_AttrName, id);
        delete [] id;
    }

    save(*object.propertySet());

    getWriter()->writeElementEnd();
}

void
OMXMLStoredObject::save(const OMStoredObjectIdentification& /*id*/)
{
    TRACE("OMXMLStoredObject::save(OMStoredObjectIdentification)");
    ASSERT("Unimplemented code not reached", false);
}

void
OMXMLStoredObject::save(const OMPropertySet& properties )
{
    TRACE("OMXMLStoredObject::save(OMPropertySet)");

    // write the Header::Dictionary property first
    if (properties.container()->definition()->identification() == ClassID_Header)
    {
        const OMProperty* property = properties.get(PropLocalID_Header_Dictionary);

        const wchar_t* symbolspace;
        const wchar_t* symbol;
        OMUniqueObjectIdentification id =
            property->definition()->identification();
        if (_store->getMetaDefSymbol(id, &symbolspace, &symbol))
        {
            getWriter()->writeElementStart(symbolspace, symbol);
        }
        else
        {
            ASSERT("Property definition registered in MetaDictionary and Symbolspace", false);
        }
        property->save();
        getWriter()->writeElementEnd();
    }
    
    OMPropertySetIterator iterator(properties, OMBefore);
    while (++iterator)
    {
        const OMProperty* property = iterator.property();
        
        // skip the Header::Dictionary property because it has already been written
        if (property->definition()->identification() == PropID_Header_Dictionary)
        {
            continue;
        }
        
        if (!property->isOptional() || property->isPresent())
        {
            // converting older files does not handle extendible enumerations 
            // properly. Properties with invalid ext. enum. values will not be saved.
            TypeCategoryVisitor category(property->type());
            if (category.get() == EXT_ENUM_CAT)
            {
                const OMSimpleProperty* simpleProp = dynamic_cast<const OMSimpleProperty*>
                    (property);
                const OMUniqueObjectIdentification* id = 
                    reinterpret_cast<const OMUniqueObjectIdentification*>(
                        simpleProp->bits());
                const OMExtendibleEnumeratedType* extEnumType = dynamic_cast<const OMExtendibleEnumeratedType*>
                    (property->type());
                if (!extEnumType->isValidValue(*id))
                {
                    ASSERT("Invalid extendible enum. property can be removed",
                        property->isOptional());
                    
                    wchar_t uri[XML_MAX_AUID_URI_SIZE];
                    auidToURI(*id, uri);
                    printf("Invalid extendible enumeration value encountered: %ls. "
                        "The property (%x, '%ls') will not be saved.\n", uri,
                        property->propertyId(), property->name());
                    continue;
                }
            }
            
            if (property->propertyId() != PropLocalID_Root_Header)
            {
                const wchar_t* symbolspace;
                const wchar_t* symbol;
                OMUniqueObjectIdentification id =
                    property->definition()->identification();
                if (_store->getMetaDefSymbol(id, &symbolspace, &symbol))
                {
                    getWriter()->writeElementStart(symbolspace, symbol);
                }
                else
                {
                    ASSERT("Property definition registered in MetaDictionary and Symbolspace", false);
                }
            }

            // handle special case of Header::ByteOrder
            if (property->propertyId() == PropLocalID_Header_ByteOrder)
            {
                saveHeaderByteOrder(property);
            }
            else
            {
                property->save();
            }

            if (property->propertyId() != PropLocalID_Root_Header)
            {
                getWriter()->writeElementEnd();
            }
        }
    }

}

void
OMXMLStoredObject::save(const OMSimpleProperty& property)
{
    TRACE("OMXMLStoredObject::save(OMSimpleProperty)");

    // special processing of MetaDefinition::Identification and 
    // DefinitionObject::Identification so that they are represented
    // as URIs and not as symbols
    if (property.definition()->identification() == PropID_MetaDefinition_Identification ||
        property.definition()->identification() == PropID_DefinitionObject_Identification)
    {
        const OMUniqueObjectIdentification id = 
            *(reinterpret_cast<const OMUniqueObjectIdentification*>(property.bits()));
        wchar_t uri[XML_MAX_AUID_URI_SIZE];
        auidToURI(id, uri);
        getWriter()->writeElementContent(uri, wcslen(uri));
    }
    else
    {
        saveSimpleValue(property.bits(), property.bitsSize(), property.type(), true);
    }
}

void
OMXMLStoredObject::save(const OMDataVector& property)
{
    TRACE("OMXMLStoredObject::save(OMDataVector)");

    // defer processing to saveSimpleValue
    
    OMType* elementType = dynamic_cast<const OMArrayType*>(baseType(property.type()))->elementType();
    OMUInt32 elementInternalSize = elementType->internalSize(); 

    OMByteArray buffer;
    OMDataContainerIterator* iter = property.createIterator();
    while (++(*iter))
    {
        buffer.append(iter->currentElement(), elementInternalSize);
    }
    delete iter;
    
    saveSimpleValue(buffer.bytes(), buffer.size(), property.type(), true);
}

void
OMXMLStoredObject::save(const OMDataSet& property)
{
    TRACE("OMXMLStoredObject::save(OMDataSet)");

    // defer processing to saveSimpleValue
    
    OMType* elementType = dynamic_cast<const OMSetType*>(baseType(property.type()))->elementType();
    OMUInt32 elementInternalSize = elementType->internalSize(); 

    OMByteArray buffer;
    OMDataContainerIterator* iter = property.createIterator();
    while (++(*iter))
    {
        buffer.append(iter->currentElement(), elementInternalSize);
    }
    delete iter;
    
    saveSimpleValue(buffer.bytes(), buffer.size(), property.type(), true);
}

void
OMXMLStoredObject::save(const OMStrongReference& singleton)
{
    TRACE("OMXMLStoredObject::save(OMStrongReference)");
    singleton.reference().save();
}

void
OMXMLStoredObject::save(const OMStrongReferenceVector& vector)
{
    TRACE("OMXMLStoredObject::save(OMStrongReferenceVector)");

    OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
        *vector.iterator();
    while (++iterator)
    {
        OMStrongReferenceVectorElement& element = iterator.value();
        element.save();
    }
    delete &iterator;
}

void
OMXMLStoredObject::save(const OMStrongReferenceSet& set)
{
    TRACE("OMXMLStoredObject::save(OMStrongReferenceSet)");

    OMContainerIterator<OMStrongReferenceSetElement>& iterator = *set.iterator();
    while (++iterator)
    {
        OMStrongReferenceSetElement& element = iterator.value();

        // Forward the DefinitionObject::Identification to the object
        // so that it can be written as the value of the aaf:uid attribute
        // Handle special case of OperationGroup::Parameters set where the key
        // is defined to be the DefinitionObject::Identification property and 
        // this id must not be forwarded
        if (set.keyPropertyId() == PropLocalID_DefinitionObject_Identification &&
            set.definition()->identification() != PropID_OperationGroup_Parameters)
        {
            OMUniqueObjectIdentification id =
                *(reinterpret_cast<OMUniqueObjectIdentification*>(element.identification()));
            wchar_t* idStr = saveAUID(id, DICT_DEF);
            _store->forwardObjectSetId(idStr);
            delete [] idStr;
        }

        element.save();
    }
    delete &iterator;
}

void
OMXMLStoredObject::save(const OMWeakReference& singleton)
{
    TRACE("OMXMLStoredObject::save(OMWeakReference)");

    const OMWeakObjectReferenceType* weakRefType = dynamic_cast<const OMWeakObjectReferenceType*>(
        baseType(singleton.type()));
    saveWeakRef(singleton.identificationBits(), weakRefType);
}

void
OMXMLStoredObject::save(const OMWeakReferenceVector& vector)
{
    TRACE("OMXMLStoredObject::save(OMWeakReferenceVector)");

    OMContainerIterator<OMWeakReferenceVectorElement>& iterator = *vector.iterator();
    while (++iterator)
    {
        const OMWeakObjectReferenceType* weakRefType = dynamic_cast<const OMWeakObjectReferenceType*>(
            baseType((dynamic_cast<const OMArrayType*>(vector.type()))->elementType()));
        const wchar_t* weakRefTypeSymbolspace;
        const wchar_t* weakRefTypeSymbol;
        if (!_store->getMetaDefSymbol(weakRefType->identification(), &weakRefTypeSymbolspace, 
            &weakRefTypeSymbol))
        {
            ASSERT("Weak reference vector element type definition registered in MetaDictionary and Symbolspace", false);
        }
        getWriter()->writeElementStart(weakRefTypeSymbolspace, weakRefTypeSymbol);

        OMWeakReferenceVectorElement& element = iterator.value();
        saveWeakRef(element.identification(), weakRefType);

        getWriter()->writeElementEnd();
    }
    delete &iterator;
}

void
OMXMLStoredObject::save(const OMWeakReferenceSet& set)
{
    TRACE("OMXMLStoredObject::save(OMWeakReferenceSet)");

    OMContainerIterator<OMWeakReferenceSetElement>& iterator = *set.iterator();
    while (++iterator)
    {
        const OMWeakObjectReferenceType* weakRefType = dynamic_cast<const OMWeakObjectReferenceType*>(
            baseType((dynamic_cast<const OMSetType*>(set.type()))->elementType()));
        const wchar_t* weakRefTypeSymbolspace;
        const wchar_t* weakRefTypeSymbol;
        if (!_store->getMetaDefSymbol(weakRefType->identification(), &weakRefTypeSymbolspace, 
            &weakRefTypeSymbol))
        {
            ASSERT("Weak reference vector element type definition registered in MetaDictionary and Symbolspace", false);
        }
        getWriter()->writeElementStart(weakRefTypeSymbolspace, weakRefTypeSymbol);

        OMWeakReferenceSetElement& element = iterator.value();
        saveWeakRef(element.identification(), weakRefType);

        getWriter()->writeElementEnd();
    }
    delete &iterator;
}

void
OMXMLStoredObject::save(const OMPropertyTable* /* table */)
{
    TRACE("OMXMLStoredObject::save(OMPropertyTable)");
    ASSERT("Unimplemented code not reached", false);
}

void
OMXMLStoredObject::save(const OMDataStream& stream)
{
    TRACE("OMXMLStoredObject::save(OMDataStream)");

    const wchar_t* entityName = _store->getDataStreamEntityName((void*)&stream);
    ASSERT("Valid stream entity name", entityName != 0);

    getWriter()->writeAttribute(getBaselineURI(), StreamRef_AttrName, entityName);
    
    if (stream.storedByteOrder() == littleEndian ||
        stream.storedByteOrder() == bigEndian)
    {
        wchar_t byteOrderStr[XML_MAX_BYTE_ORDER_STRING_SIZE];
        byteOrderToString(stream.storedByteOrder(), byteOrderStr);
        getWriter()->writeAttribute(getBaselineURI(), ByteOrder_AttrName, byteOrderStr);
    }
}

OMRootStorable*
OMXMLStoredObject::restore(OMFile& file)
{
    TRACE("OMXMLStoredObject::restore(OMFile)");
    PRECONDITION("XML document is set for reading", _store->haveReader());

    OMRootStorable* root = 0;
    try
    {
        OMFile::OMLoadMode originalLoadMode = file.loadMode();
        file.setLoadMode(OMFile::eagerLoad);

        bool haveRootElement = false;
        while (!haveRootElement && getReader()->next())
        {
            if (getReader()->getEventType() == OMXMLReader::NOTATION_DECL)
            {
                const wchar_t* notationName;
                const wchar_t* publicID;
                const wchar_t* systemID;
                getReader()->getNotationDecl(notationName, publicID, systemID);
                if (!_store->registerDataStreamNotation(notationName, systemID))
                {
                    throw OMException("Failed to register DataStream Notation");
                }
            }
            else if (getReader()->getEventType() == OMXMLReader::UNPARSED_ENTITY_DECL)
            {
                const wchar_t* name;
                const wchar_t* publicID;
                const wchar_t* systemID;
                const wchar_t* notationName;
                getReader()->getUnparsedEntityDecl(name, publicID, systemID, notationName);
                if (!isRelativeURI(systemID) && !isFileURL(systemID))
                {
                    throw OMException("Failed to register DataStream Entity: system ID "
                        "is not a file URL or a relative URI");
                }
                if (!_store->registerDataStreamEntity(name, systemID))
                {
                    throw OMException("Failed to register DataStream Entity");
                }
            }
            else if (getReader()->getEventType() == OMXMLReader::START_PREFIX_MAPPING)
            {
                const wchar_t* prefix;
                const wchar_t* uri;
                getReader()->getStartPrefixMapping(prefix, uri);
                if (prefix != 0 && uri != 0 && wcslen(prefix) > 0 && wcslen(uri) > 0)
                {
                    _store->registerNamespacePrefix(prefix, uri);
                }
            }
            else if (getReader()->getEventType() == OMXMLReader::START_ELEMENT)
            {
                OMClassId id;
                file.rootStore()->restore(id);
                if (id != OMRootStorable::_rootClassId)
                {
                    throw OMException("Invalid root element");
                }
                haveRootElement = true;
            }
        }

        root = new OMRootStorable();
        root->attach(&file);
        root->setStore(file.rootStore());
        
        root->setDictionary(file.dictionary());

        root->setClassFactory(file.classFactory());
        root->restoreContents();
        
        file.setLoadMode(originalLoadMode);
    }
    catch (OMException& ex)
    {
        fprintf(stderr, "Exception: ");
        if (ex.name() != 0)
        {
            fprintf(stderr, "%s\n", ex.name());
        }
        fprintf(stderr, "XML parser position: %ls\n", getReader()->getPositionString());
        throw;
    }

    return root;
}

OMStorable*
OMXMLStoredObject::restoreObject(const OMStrongObjectReference& reference)
{
    TRACE("OMXMLStoredObject::restoreObject");

    const wchar_t* name = reference.name();
    OMProperty* property = reference.property();
    OMStorable* containingObject = property->propertySet()->container();

    OMClassId cid;
    restore(cid);
    OMClassFactory* classFactory = containingObject->classFactory();
    OMStorable* object = classFactory->create(cid);
    ASSERT("Registered class id", object != 0);
    ASSERT("Valid class factory", classFactory == object->classFactory());
#if !defined(OM_NO_VALIDATE_DEFINITIONS)

    ASSERT("Valid class definition", object->definition() != 0);
#endif
    object->attach(containingObject, name);
    object->setStore(this);
    object->restoreContents(); // note: 'this' is deleted in the restoreContents function
    return object;
}

void
OMXMLStoredObject::restore(OMStoredObjectIdentification& id)
{
    TRACE("OMXMLStoredObject::restore(OMStoredObjectIdentification)");

    if (getReader()->elementEquals(getBaselineURI(), L"AAF"))
    {
        id = OMRootStorable::_rootClassId;
    }
    else
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        id = _store->getMetaDefId(nmspace, localName);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMException("Unknown class encountered");
        }
    }
}

void
OMXMLStoredObject::restore(OMPropertySet& properties)
{
    TRACE("OMXMLStoredObject::restore(OMPropertySet)");

    if (_isRoot)
    {
        getReader()->nextElement();
        if (getReader()->getEventType() == OMXMLReader::START_ELEMENT &&
                getReader()->elementEquals(getBaselineURI(), L"Extensions"))
        {
            restoreExtensions(properties.container()->file()->dictionary());
            getReader()->nextElement();
        }

        if (getReader()->getEventType() == OMXMLReader::START_ELEMENT &&
            getReader()->elementEquals(getBaselineURI(), L"Header"))
        {
            createDictionaryMap();
            OMStrongReference* headerRef = dynamic_cast<OMStrongReference*>(
                properties.get(PropLocalID_Root_Header));
            headerRef->reference().restore();
        }
        else
        {
            throw OMException("Header expected");
        }
    }
    else
    {
        while (getReader()->nextElement())
        {
            const wchar_t* nmspace;
            const wchar_t* localName;
            const OMList<OMXMLAttribute*>* attrs;
            getReader()->getStartElement(nmspace, localName, attrs);
            OMPropertyId localId = _store->getPropertyDefId(nmspace, localName);
            if (localId == 0)
            {
                throw OMException("Unknown property encountered");
            }
            OMProperty* property = properties.get(localId);
            if (property->propertyId() == PropLocalID_Header_ByteOrder)
            {
                restoreHeaderByteOrder(property);
            }
            else
            {
                property->restore((OMPropertySize)property->bitsSize());
            }
            
            ASSERT("Reader is positioned on property end element", 
                getReader()->getEventType() == OMXMLReader::END_ELEMENT);
        }
    
        getReader()->moveToEndElement();
    }
}

void
OMXMLStoredObject::restore(OMSimpleProperty& property,
                            OMPropertySize externalSize)
{
    TRACE("OMXMLStoredObject::restore(OMSimpleProperty)");

    const wchar_t* nmspace;
    const wchar_t* localName;
    const OMList<OMXMLAttribute*>* attrs;
    getReader()->getStartElement(nmspace, localName, attrs);
    
    OMByteArray bytes;
    restoreSimpleValue(bytes, attrs, 0, property.type());
 
    // note that a 'DataValue' can have size == 0 and 
    // a set/array not represented using a OMDataVector or OMDataArray 
    // can also have size == 0
    if (bytes.size() > 0) 
    {
        property.setBits(bytes.bytes(), bytes.size());
    }
}

void
OMXMLStoredObject::restore(OMDataVector& property,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMDataVector)");

    // defer to restoreSimpleValue and then append each element
    
    property.clear();

    OMType* elementType = dynamic_cast<const OMArrayType*>(baseType(property.type()))->elementType();
    OMUInt32 elementInternalSize = elementType->internalSize(); 

    const wchar_t* nmspace;
    const wchar_t* localName;
    const OMList<OMXMLAttribute*>* attrs;
    getReader()->getStartElement(nmspace, localName, attrs);
    
    OMByteArray bytes;
    restoreSimpleValue(bytes, attrs, 0, property.type());

    OMUInt32 count = bytes.size() / elementInternalSize;
    OMByte* bytesPtr = bytes.bytes();
    for (OMUInt32 i = 0; i < count; i++)
    {
        property.appendValue(bytesPtr);
        bytesPtr += elementInternalSize;
    }
}

void
OMXMLStoredObject::restore(OMDataSet& property,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMDataSet)");

    // defer to restoreSimpleValue and then insert each element
    
    property.clear();
    
    OMType* elementType = dynamic_cast<const OMSetType*>(baseType(property.type()))->elementType();
    OMUInt32 elementInternalSize = elementType->internalSize(); 

    const wchar_t* nmspace;
    const wchar_t* localName;
    const OMList<OMXMLAttribute*>* attrs;
    getReader()->getStartElement(nmspace, localName, attrs);
    
    OMByteArray bytes;
    restoreSimpleValue(bytes, attrs, 0, property.type());

    OMUInt32 count = bytes.size() / elementInternalSize;
    OMByte* bytesPtr = bytes.bytes();
    for (OMUInt32 i = 0; i < count; i++)
    {
        property.insert(bytesPtr);
        bytesPtr += elementInternalSize;
    }
}

void
OMXMLStoredObject::restore(OMStrongReference& singleton,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMStrongReference)");

    bool haveNextElement = getReader()->nextElement();
    if (!haveNextElement)
    {
        throw OMException("Empty strong reference singleton found");
    }

    wchar_t* name = referenceName(singleton.name(), singleton.propertyId());
    OMStrongObjectReference newReference(&singleton, name);
    singleton.reference() = newReference;
    singleton.reference().restore();
    delete [] name;

    getReader()->nextEndElement();
}

void
OMXMLStoredObject::restore(OMStrongReferenceVector& vector,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMStrongReferenceVector)");

    OMPropertyId vectorId = vector.propertyId();
    const wchar_t* vectorName = vector.name();

    OMUInt32 localKey = 0;
    OMList<OMStrongReferenceVectorElement> elements;
    while (getReader()->nextElement())
    {
        wchar_t* name = elementName(vectorName, vectorId, localKey);

        OMStrongReferenceVectorElement element(&vector, name, localKey);
        element.restore();
        elements.append(element);

        delete [] name;
        localKey++;
    }
    getReader()->moveToEndElement();
    
    vector.setLocalKey(localKey);
    
    if (localKey != 0)
    {
        vector.grow(localKey);
    
        OMListIterator<OMStrongReferenceVectorElement> iter(elements, OMAfter);
        while (--iter)
        {
            localKey--;
            vector.insert(localKey, iter.value());
        }
    }
}

void
OMXMLStoredObject::restore(OMStrongReferenceSet& set,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMStrongReferenceSet)");

    OMPropertyId setId = set.propertyId();
    const wchar_t* setName = set.name();

    OMUInt32 localKey = 0;
    OMKeySize keySize = set.keySize();

    while (getReader()->nextElement())
    {
        wchar_t* name = elementName(setName, setId, localKey);

        OMByte* key = 0;

        // DefinitionObjects will have a aaf:uid attribute
        // Read it so that we can compare it with the DefinitionObject::Identification
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        OMXMLAttribute* idAttr = getReader()->getAttribute(attrs,
                                 getBaselineURI(), UniqueId_AttrName);
        if (idAttr != 0)
        {
            key = new OMByte[sizeof(OMUniqueObjectIdentification)];
            OMUniqueObjectIdentification id = restoreAUID(idAttr->getValue(), DICT_DEF);
            memcpy(key, reinterpret_cast<void*>(&id), sizeof(OMUniqueObjectIdentification));
        }

        // restore the object using a dummy (key and keySize not yet known)
        OMStrongReferenceSetElement dummy(&set, name, localKey, 0, 0);
        dummy.restore();

        // get the key from the property identified to be the key
        // Handle the special case of OperationGroup::Parameters
        OMStorable* storable = dummy.getValue();
        OMProperty* keyProperty = 0;
        if (set.keyPropertyId() == PropLocalID_DefinitionObject_Identification &&
            set.definition()->identification() == PropID_OperationGroup_Parameters)
        {
            keyProperty = storable->findProperty(PropLocalID_Parameter_Definition);
        }
        else
        {
            keyProperty = storable->findProperty(set.keyPropertyId());
        }
        if (keyProperty == 0)
        {
            throw OMException("Could not find property that holds the set key");
        }
        OMUInt32 size = keyProperty->bitsSize();
        if (size != keySize)
        {
            throw OMException("The size of the property identified to be the set key does "
                "not match the expected key size");
        }
        OMByte* bits = new OMByte[size];
        keyProperty->getBits(bits, size);
        if (key != 0)
        {
            if (memcmp(key, bits, size) != 0)
            {
                throw OMException("Key property value does not match aaf:uid attribute value");
            }
            delete [] bits;
        }
        else
        {
            key = bits;
        }
        
        // insert the element into the set
        OMStrongReferenceSetElement element(&set, name, localKey, key, keySize);
        element.setValue(key, storable);
        set.insert(key, element);
        
        delete [] key;
        delete [] name;
        
        localKey++;
    }
    getReader()->moveToEndElement();
}

void
OMXMLStoredObject::restore(OMWeakReference& singleton,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMWeakReference)");

    // TODO: only works for  OMUniqueObjectIdentification
    ASSERT("Supported weak reference key size", singleton.keySize() == sizeof(OMUniqueObjectIdentification));
    
    OMUniqueObjectIdentification id;
    OMPropertyTag tag;
    restoreWeakRef(singleton.propertySet()->container()->file(), 
        singleton.definition()->type(), id, tag);
    
    singleton.setTargetTag(tag);
    singleton.setIdentificationBits(static_cast<void*>(&id), sizeof(OMUniqueObjectIdentification));

    getReader()->moveToEndElement();
}

void
OMXMLStoredObject::restore(OMWeakReferenceVector& vector,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMWeakReferenceVector)");

    // TODO: only works for  OMUniqueObjectIdentification
    ASSERT("Supported weak reference key size", vector.keySize() == sizeof(OMUniqueObjectIdentification));
    
    OMUInt32 index = 0;
    OMList<OMWeakReferenceVectorElement> elements;
    while (getReader()->nextElement())
    {
        OMUniqueObjectIdentification id;
        OMPropertyTag tag;
        restoreWeakRef(vector.propertySet()->container()->file(), 
            vector.definition()->type(), id, tag);
        
        OMWeakReferenceVectorElement element(&vector, static_cast<void*>(&id), 
            sizeof(OMUniqueObjectIdentification), tag);
        element.restore();
        elements.append(element);

        getReader()->moveToEndElement();
        index++;
    }
    getReader()->moveToEndElement();

    vector.setLocalKey(index);
    
    if (index != 0)
    {
        vector.grow(index);
    
        OMListIterator<OMWeakReferenceVectorElement> iter(elements, OMAfter);
        while (--iter)
        {
            index--;
            vector.insert(index, iter.value());
        }
    }
}

void
OMXMLStoredObject::restore(OMWeakReferenceSet& set,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMWeakReferenceSet)");

    // TODO: only works for  OMUniqueObjectIdentification
    ASSERT("Supported weak reference key size", set.keySize() == sizeof(OMUniqueObjectIdentification));
    
    while (getReader()->nextElement())
    {
        OMUniqueObjectIdentification id;
        OMPropertyTag tag;
        restoreWeakRef(set.propertySet()->container()->file(), set.definition()->type(),
            id, tag);
            
        OMWeakReferenceSetElement element(&set, static_cast<void*>(&id), 
            sizeof(OMUniqueObjectIdentification), tag);
        element.restore();
        set.insert(&id, element);

        getReader()->moveToEndElement();
    }
    getReader()->moveToEndElement();
}

void
OMXMLStoredObject::restore(OMPropertyTable*& /* table */)
{
    TRACE("OMXMLStoredObject::restore(OMPropertyTable)");
    ASSERT("Unimplemented code not reached", false);
}

void
OMXMLStoredObject::restore(OMDataStream& stream,
                            OMPropertySize /* externalSize */)
{
    TRACE("OMXMLStoredObject::restore(OMDataStream)");

    const wchar_t* nmspace;
    const wchar_t* localName;
    const OMList<OMXMLAttribute*>* attrs;
    getReader()->getStartElement(nmspace, localName, attrs);

    OMXMLAttribute* byteOrderAttr = getReader()->getAttribute(attrs,
        getBaselineURI(), ByteOrder_AttrName);
    if (byteOrderAttr == 0)
    {
        stream.setStoredByteOrder(unspecified);
    }
    else
    {
        OMByteOrder byteOrder;
        byteOrderFromString(byteOrderAttr->getValue(), &byteOrder);
        stream.setStoredByteOrder(byteOrder);
    }

    OMXMLAttribute* streamAttr = getReader()->getAttribute(attrs, getBaselineURI(), 
        StreamRef_AttrName);
    if (streamAttr == 0)
    {
        throw OMException("DataStream element is missing a aaf:stream attribute");
    }
    const wchar_t* streamFilename = _store->registerDataStreamEntityValue((void*)&stream, 
        _store->getDataStreamEntityValue(streamAttr->getValue()));
    if (streamFilename == 0)
    {
        throw OMException("Could not register filename for DataStream property");
    }

    getReader()->moveToEndElement();

}

OMStoredStream*
OMXMLStoredObject::openStoredStream(const OMDataStream& property)
{
    TRACE("OMXMLStoredObject::openStoredStream");

    const wchar_t* value = _store->getDataStreamEntityValue((void*)&property);
    if (value == 0)
    {
        // Print to stderr because these exceptions are not caught in OM layer
        fprintf(stderr, "Exception thrown \"Opening DataStream property without known filename\"\n");
        throw OMException("Opening DataStream property without known filename");
    }
    
    OMRawStorage* storage = _store->openExistingDataStream(value);
    if (storage == 0)
    {
        // Print to stderr because these exceptions are not caught in OM layer
        fprintf(stderr, "Exception thrown \"Failed to open DataStream\"\n");
        throw OMException("Failed to open DataStream");
    }
    
    return new OMXMLStoredStream(storage);
}

OMStoredStream*
OMXMLStoredObject::createStoredStream(const OMDataStream& property)
{
    TRACE("OMXMLStoredObject::createStoredStream");

    const wchar_t* value = _store->getDataStreamEntityValue((void*)&property);
    if (value == 0)
    {
        // Print to stderr because these exceptions are not caught in OM layer
        fprintf(stderr, "Exception thrown \"Opening DataStream property without known filename\"\n");
        throw OMException("Opening DataStream property without known filename");
    }

    OMRawStorage* storage = _store->openNewDataStream(value);
    if (storage == 0)
    {
        // Print to stderr because these exceptions are not caught in OM layer
        fprintf(stderr, "Exception thrown \"Failed to create DataStream\"\n");
        throw OMException("Failed to create DataStream");
    }
    
    return new OMXMLStoredStream(storage);
}

OMXMLWriter*
OMXMLStoredObject::getWriter()
{
    TRACE("OMXMLStoredObject::getWriter");
    
    return _store->getWriter();
}

OMXMLReader*
OMXMLStoredObject::getReader()
{
    TRACE("OMXMLStoredObject::getReader");
    
    return _store->getReader();
}

const wchar_t*
OMXMLStoredObject::getBaselineURI()
{
    TRACE("OMXMLStoredObject::getBaselineURI");
    
    return _store->getBaselineSymbolspace()->getURI();
}

void
OMXMLStoredObject::saveHeaderByteOrder(const OMProperty* property)
{
    TRACE("OMXMLStoredObject::saveHeaderByteOrder");

    const OMSimpleProperty* simpleProp = dynamic_cast<const OMSimpleProperty*>(property);
    ASSERT("Valid simple property", simpleProp != 0);
    wchar_t byteOrderStr[XML_MAX_BYTE_ORDER_STRING_SIZE];
    headerByteOrderToString(simpleProp->bits(), byteOrderStr);
    
    getWriter()->writeElementContent(byteOrderStr, wcslen(byteOrderStr));
}

void 
OMXMLStoredObject::saveSimpleValue(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveSimpleValue");
    
    switch (TypeCategoryVisitor(type).get())
    {
        case CHARACTER_CAT:
            saveCharacter(internalBytes, internalSize, dynamic_cast<const OMCharacterType*>(type), 
                isElementContent);
            break;
        case ENUM_CAT:
            saveEnum(internalBytes, internalSize, dynamic_cast<const OMEnumeratedType*>(type), 
                isElementContent);
            break;
        case EXT_ENUM_CAT:
            saveExtEnum(internalBytes, internalSize, dynamic_cast<const OMExtendibleEnumeratedType*>(type), 
                isElementContent);
            break;
        case FIXED_ARRAY_CAT:
            saveFixedArray(internalBytes, internalSize, dynamic_cast<const OMFixedArrayType*>(type), 
                isElementContent);
            break;
        case INDIRECT_CAT:
            saveIndirect(internalBytes, internalSize, dynamic_cast<const OMIndirectType*>(type), 
                isElementContent);
            break;
        case INTEGER_CAT:
            saveInteger(internalBytes, internalSize, dynamic_cast<const OMIntegerType*>(type), 
                isElementContent);
            break;
        case OPAQUE_CAT:
            saveOpaque(internalBytes, internalSize, dynamic_cast<const OMOpaqueType*>(type), 
                isElementContent);
            break;
        case RECORD_CAT:
            saveRecord(internalBytes, internalSize, dynamic_cast<const OMRecordType*>(type), 
                isElementContent);
            break;
        case RENAMED_CAT:
            saveRenamed(internalBytes, internalSize, dynamic_cast<const OMRenamedType*>(type), 
                isElementContent);
            break;
        case SET_CAT:
            saveSet(internalBytes, internalSize, dynamic_cast<const OMSetType*>(type), 
                isElementContent);
            break;
        case STRING_CAT:
            saveString(internalBytes, internalSize, dynamic_cast<const OMStringType*>(type), 
                isElementContent);
            break;
        case VAR_ARRAY_CAT:
            saveVariableArray(internalBytes, internalSize, dynamic_cast<const OMVaryingArrayType*>(type), 
                isElementContent);
            break;
        default:
            ASSERT("Valid simple value type category", false);
            break;
    }
}

void 
OMXMLStoredObject::saveCharacter(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMCharacterType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveCharacter");

    if (internalSize != sizeof(wchar_t))
    {
        throw OMException("Invalid character - internal size != sizeof(wchar_t)");
    }
    
    wchar_t c = *(reinterpret_cast<const wchar_t*>(internalBytes));

    if (characterRequiresEscaping(c))
    {
        if (!isElementContent)
        {
            throw OMException("Character requiring escaping is not element content");
        }
        getWriter()->writeAttribute(getBaselineURI(), AAFEscape_AttrName, L"true");
        wchar_t* escapedStr = escapeCharacter(c);
        getWriter()->writeElementContent(escapedStr, wcslen(escapedStr)); 
        delete [] escapedStr;
    }
    else
    {
        wchar_t buffer[2];
        buffer[0] = c;
        buffer[1] = L'\0';
        
        if (isElementContent)
        {
            getWriter()->writeElementContent(buffer, wcslen(buffer));
        }
        else
        {
            getWriter()->writeAttributeContent(buffer);
        }
    }
}

void 
OMXMLStoredObject::saveEnum(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMEnumeratedType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveEnum");
    
    OMInt64 value = 0;
    switch (internalSize)
    {
        case sizeof(OMInt8):
            value = (OMInt64)*((OMInt8*)internalBytes);
            break;
        case sizeof(OMInt16):
            value = (OMInt64)*((OMInt16*)internalBytes);
            break;
        case sizeof(OMInt32):
            value = (OMInt64)*((OMInt32*)internalBytes);
            break;
        case sizeof(OMInt64):
            value = (OMInt64)*(internalBytes);
            break;
        default:
            ASSERT("Valid integer size", false);
            break;
    }
    wchar_t* name = 0;
    if (type->identification() == TypeID_Boolean)
    {
        name = new wchar_t[XML_MAX_BOOL_STRING_SIZE];
        boolToString(value != 0, name); // kAAFFalse = 0, kAAFTrue = 1
    }
    else
    {
        name = type->elementNameFromValue(value);
    }
    ASSERT("Valid name", name != 0);
    
    if (isElementContent)
    {
        getWriter()->writeElementContent(name, wcslen(name));
    }
    else
    {
        getWriter()->writeAttributeContent(name);
    }
    
    delete [] name;
}

void 
OMXMLStoredObject::saveExtEnum(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMExtendibleEnumeratedType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveExtEnum");
    
    const OMUniqueObjectIdentification id = 
        *(reinterpret_cast<const OMUniqueObjectIdentification*>(internalBytes));
    wchar_t* name = type->elementNameFromValue(id);
    ASSERT("Valid name", name != 0);
    
    if (isElementContent)
    {
        getWriter()->writeElementContent(name, wcslen(name));
    }
    else
    {
        getWriter()->writeAttributeContent(name);
    }
    
    delete [] name;
}

void 
OMXMLStoredObject::saveFixedArray(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMFixedArrayType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveFixedArray");

    OMType* elementType = type->elementType();
    const wchar_t* symbolspace;
    const wchar_t* symbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getMetaDefSymbol(id, &symbolspace, &symbol))
    {
        ASSERT("Fixed array element type definition registered in MetaDictionary and Symbolspace", false);
    }

    const OMByte* elementBytes = internalBytes;
    OMUInt32 count = type->elementCount();
    for (OMUInt32 i = 0; i < count; i++)
    {
        getWriter()->writeElementStart(symbolspace, symbol);
        saveSimpleValue(elementBytes, elementType->internalSize(), elementType,
            isElementContent);
        getWriter()->writeElementEnd();
        
        elementBytes += elementType->internalSize();
    }
}

void 
OMXMLStoredObject::saveIndirect(const OMByte* externalBytes, OMUInt32 externalSize, 
    const OMIndirectType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveIndirect");

    OMType* actualType = type->actualType(externalBytes, externalSize);

    OMByteArray actualData;
    OMUInt32 actualSize;
    type->actualSize(externalBytes, externalSize, actualSize);
    actualData.grow(actualSize);
    actualData.setSize(actualSize);
    type->actualData(externalBytes, externalSize, actualData.bytes(), actualSize);
    
    wchar_t* idStr = saveAUID(actualType->identification(), METADICT_DEF);
    getWriter()->writeAttribute(getBaselineURI(), ActualType_AttrName, idStr);
    delete [] idStr;
        
    saveSimpleValue(actualData.bytes(), actualSize, actualType, isElementContent);
}

void 
OMXMLStoredObject::saveInteger(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMIntegerType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveInteger");
    
    wchar_t intStr[XML_MAX_INTEGER_STRING_SIZE];
    integerToString(internalBytes, type->size(), type->isSigned(), intStr);
    
    if (isElementContent)
    {
        getWriter()->writeElementContent(intStr, wcslen(intStr));
    }
    else
    {
        getWriter()->writeAttributeContent(intStr);
    }
    
}

void 
OMXMLStoredObject::saveOpaque(const OMByte* externalBytes, OMUInt32 externalSize, 
    const OMOpaqueType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveOpaque");

    OMUniqueObjectIdentification actualTypeId = type->actualTypeId(externalBytes, 
        externalSize);
    OMByteOrder byteOrder = type->byteOrder(externalBytes, externalSize);

    const OMByte* externalDataBytes;
    OMUInt32 externalDataSize;
    type->externalData(externalBytes, externalSize, externalDataBytes, externalDataSize);
    
    wchar_t* idStr = saveAUID(actualTypeId, METADICT_DEF);
    getWriter()->writeAttribute(getBaselineURI(), ActualType_AttrName, idStr);
    delete [] idStr;
        
    wchar_t byteOrderStr[XML_MAX_BYTE_ORDER_STRING_SIZE];
    byteOrderToString(byteOrder, byteOrderStr);
    getWriter()->writeAttribute(getBaselineURI(), ByteOrder_AttrName, byteOrderStr);

    writeDataInHex(externalDataBytes, externalDataSize, isElementContent);    
}

void 
OMXMLStoredObject::saveRecord(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMRecordType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveRecord");
    
    if (type->identification() == TypeID_AUID)
    {
        const OMUniqueObjectIdentification id = 
            *(reinterpret_cast<const OMUniqueObjectIdentification*>(internalBytes));
        wchar_t* idStr = saveAUID(id, ANY);
        if (isElementContent)
        {
            getWriter()->writeElementContent(idStr, wcslen(idStr));
        }
        else
        {
            getWriter()->writeAttributeContent(idStr);
        }
        delete [] idStr;
    }
    else if (type->identification() == TypeID_MobIDType)
    {
        const OMMaterialIdentification mobId =
            *(reinterpret_cast<const OMMaterialIdentification*>(internalBytes));
        wchar_t uri[XML_MAX_MOBID_URI_SIZE];
        mobIdToURI(mobId, uri);
        if (isElementContent)
        {
            getWriter()->writeElementContent(uri, wcslen(uri));
        }
        else
        {
            getWriter()->writeAttributeContent(uri);
        }
    }
    else if (type->identification() == TypeID_DateStruct)
    {
        wchar_t dateStr[XML_MAX_DATESTRUCT_STRING_SIZE];
        dateStructToString(internalBytes, dateStr);
        if (isElementContent)
        {
            getWriter()->writeElementContent(dateStr, wcslen(dateStr));
        }
        else
        {
            getWriter()->writeAttributeContent(dateStr);
        }
    }
    else if (type->identification() == TypeID_TimeStruct)
    {
        wchar_t timeStr[XML_MAX_TIMESTRUCT_STRING_SIZE];
        timeStructToString(internalBytes, timeStr);
        if (isElementContent)
        {
            getWriter()->writeElementContent(timeStr, wcslen(timeStr));
        }
        else
        {
            getWriter()->writeAttributeContent(timeStr);
        }
    }
    else if (type->identification() == TypeID_TimeStamp)
    {
        wchar_t timeStampStr[XML_MAX_TIMESTAMP_STRING_SIZE];
        timeStampToString(internalBytes, timeStampStr);
        if (isElementContent)
        {
            getWriter()->writeElementContent(timeStampStr, wcslen(timeStampStr));
        }
        else
        {
            getWriter()->writeAttributeContent(timeStampStr);
        }
    }
    else if (type->identification() == TypeID_Rational)
    {
        wchar_t rationalStr[XML_MAX_RATIONAL_STRING_SIZE];
        rationalToString(internalBytes, rationalStr);
        if (isElementContent)
        {
            getWriter()->writeElementContent(rationalStr, wcslen(rationalStr));
        }
        else
        {
            getWriter()->writeAttributeContent(rationalStr);
        }
    }
    else if (type->identification() == TypeID_VersionType)
    {
        wchar_t versionStr[XML_MAX_VERSIONTYPE_STRING_SIZE];
        versionTypeToString(internalBytes, versionStr);
        if (isElementContent)
        {
            getWriter()->writeElementContent(versionStr, wcslen(versionStr));
        }
        else
        {
            getWriter()->writeAttributeContent(versionStr);
        }
    }
    else
    {
        const wchar_t* symbolspace;
        const wchar_t* symbol;
        OMUniqueObjectIdentification id = type->identification();
        if (!_store->getMetaDefSymbol(id, &symbolspace, &symbol))
        {
            ASSERT("Record type definition registered in MetaDictionary and Symbolspace", false);
        }
                    
        const OMByte* memberBytes = internalBytes;
        OMUInt32 count = type->memberCount();
        for (OMUInt32 i = 0; i < count; i++)
        {
            const wchar_t* name = type->memberName(i);
            const OMType* memberType = type->memberType(i);
            
            getWriter()->writeElementStart(symbolspace, name);
            saveSimpleValue(memberBytes, memberType->internalSize(), memberType,
                isElementContent);
            getWriter()->writeElementEnd();
            
            memberBytes += memberType->internalSize();
        }
    }
}

void 
OMXMLStoredObject::saveRenamed(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMRenamedType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveRenamed");

    saveSimpleValue(internalBytes, internalSize, type->renamedType(), isElementContent);    
}

void 
OMXMLStoredObject::saveSet(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMSetType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveSet");

    OMType* elementType = type->elementType();
    const wchar_t* symbolspace;
    const wchar_t* symbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getMetaDefSymbol(id, &symbolspace, &symbol))
    {
        ASSERT("Set element type definition registered in MetaDictionary and Symbolspace", false);
    }

    const OMByte* elementBytes = internalBytes;
    OMUInt32 count = internalSize;
    while (count > 0)
    {
        getWriter()->writeElementStart(symbolspace, symbol);
        saveSimpleValue(elementBytes, elementType->internalSize(), elementType,
            isElementContent);
        getWriter()->writeElementEnd();
        
        count -= elementType->internalSize();
        elementBytes += elementType->internalSize();
    }
}

void 
OMXMLStoredObject::saveString(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMStringType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveString");
    TypeCategoryVisitor category(type);
    
    if (category.getElement() == CHARACTER_CAT)
    {
        OMByteArray modifiedBytes; 
        const wchar_t* str = reinterpret_cast<const wchar_t*>(internalBytes);
        OMUInt32 size = internalSize / sizeof(wchar_t);
        OMUInt32 len = 0;
        while (len < size && str[len] != L'\0')
        {
            len++;
        }
        if (len == size)
        {
            const wchar_t n = L'\0';
            modifiedBytes.append(internalBytes, internalSize);
            modifiedBytes.append(reinterpret_cast<const OMByte*>(&n), sizeof(wchar_t));
            str = reinterpret_cast<const wchar_t*>(modifiedBytes.bytes());
            printf("Invalid string value encountered ('%ls') - string was not null terminated\n", str);
        }
        if (len > 0)
        {
            if (stringRequiresEscaping(str))
            {
                if (!isElementContent)
                {
                    throw OMException("String requiring escaping is not element content");
                }
                getWriter()->writeAttribute(getBaselineURI(), AAFEscape_AttrName, L"true");
                wchar_t* escapedStr = escapeString(str);
                getWriter()->writeElementContent(escapedStr, wcslen(escapedStr)); 
                delete [] escapedStr;
            }
            else
            {
                if (isElementContent)
                {
                    getWriter()->writeElementContent(str, wcslen(str)); 
                }
                else
                {
                    getWriter()->writeAttributeContent(str);
                }
            }
        }
    }
    else if (category.getElement() == INTEGER_CAT)
    {
        const OMByte* bytesPtr = internalBytes;
        OMType* elementType = type->elementType();
        OMUInt32 elementInternalSize = elementType->internalSize();
        OMUInt32 count = internalSize / elementInternalSize;
        for (OMUInt32 i = 0; i < count - 1; i++)
        {
            if (i != 0)
            {
                const wchar_t* space = L" ";
                if (isElementContent)
                {
                    getWriter()->writeElementContent(space, 1); 
                }
                else
                {
                    getWriter()->writeAttributeContent(space);
                }
            }
            saveSimpleValue(bytesPtr, elementInternalSize, elementType, true);
            bytesPtr += elementInternalSize;
        }
    }
    else
    {
        throw OMException("Invalid element type for string type");
    }
}

void 
OMXMLStoredObject::saveVariableArray(const OMByte* internalBytes, OMUInt32 internalSize, 
    const OMVaryingArrayType* type, bool isElementContent)
{
    TRACE("OMXMLStoredObject::saveVariableArray");

    if (type->identification() == TypeID_DataValue)
    {
        writeDataInHex(internalBytes, internalSize, isElementContent);    
    }
    else if (type->identification() == TypeID_StringArray)
    {
        const wchar_t* stringTypeSymbolspace;
        const wchar_t* stringTypeSymbol;
        _store->getMetaDefSymbol(TypeID_String, &stringTypeSymbolspace, &stringTypeSymbol);
        
        OMUInt16 count = 0;
        const OMByte* stringInternalBytes = internalBytes;
        while (count < internalSize)
        {
            OMUInt16 stringCount = 0;
            while (count < internalSize && 
                *((const wchar_t*)&stringInternalBytes[stringCount]) != 0)
            {
                stringCount += sizeof(wchar_t);
                count += sizeof(wchar_t);
            }
            ASSERT("Valid string in string array", count < internalSize && 
                *((const wchar_t*)&stringInternalBytes[stringCount]) == 0);
            
            getWriter()->writeElementStart(stringTypeSymbolspace, stringTypeSymbol);
            if (stringCount > 0)
            {
                if (stringRequiresEscaping((wchar_t*)stringInternalBytes))
                {
                    if (!isElementContent)
                    {
                        throw OMException("String requiring escaping is not element content");
                    }
                    getWriter()->writeAttribute(getBaselineURI(), AAFEscape_AttrName, L"true");
                    wchar_t* escapedStr = escapeString((wchar_t*)stringInternalBytes);
                    getWriter()->writeElementContent(escapedStr, wcslen(escapedStr)); 
                    delete [] escapedStr;
                }
                else
                {
                    if (isElementContent)
                    {
                        getWriter()->writeElementContent((wchar_t*)stringInternalBytes, 
                            stringCount);
                    }
                    else
                    {
                        getWriter()->writeAttributeContent((wchar_t*)stringInternalBytes);
                    }
                }
            }
            getWriter()->writeElementEnd();
            
            stringInternalBytes += stringCount + sizeof(wchar_t);
            count += sizeof(wchar_t);
        }        
    }
    else
    {
        OMType* elementType = type->elementType();
        OMUniqueObjectIdentification id = elementType->identification();
        const wchar_t* symbolspace;
        const wchar_t* symbol;
        if (!_store->getMetaDefSymbol(id, &symbolspace, &symbol))
        {
            ASSERT("Variable array element type definition registered in MetaDictionary and Symbolspace", false);
        }
    
        const OMByte* elementBytes = internalBytes;
        OMUInt32 count = internalSize;
        while (count > 0)
        {
            getWriter()->writeElementStart(symbolspace, symbol);
            saveSimpleValue(elementBytes, elementType->internalSize(), elementType,
                isElementContent);
            getWriter()->writeElementEnd();
            
            count -= elementType->internalSize();
            elementBytes += elementType->internalSize();
        }
    }
}

void
OMXMLStoredObject::writeDataInHex(const OMByte* data, OMUInt32 size, bool isElementContent)
{
    TRACE("OMXMLStoredObject::writeDataInHex");

    const wchar_t* hexDigits = L"0123456789ABCDEF";

    wchar_t buffer[255];
    buffer[254] = 0;
    OMUInt32 bufferPos = 0;
    for (OMUInt32 i = 0; i < size; i++)
    {
        buffer[bufferPos++] = hexDigits[(data[i] & 0xF0) >> 4];
        buffer[bufferPos++] = hexDigits[data[i] & 0x0F];

        if (bufferPos == 254)
        {
            if (isElementContent)
            {
                getWriter()->writeElementContent(buffer, 253);
            }
            else
            {
                getWriter()->writeAttributeContent(buffer);
            }
            bufferPos = 0;
        }
    }
    if (bufferPos > 0)
    {
        buffer[bufferPos] = 0;
        if (isElementContent)
        {
            getWriter()->writeElementContent(buffer, bufferPos);
        }
        else
        {
            getWriter()->writeAttributeContent(buffer);
        }
        bufferPos = 0;
    }
}

void
OMXMLStoredObject::restoreExtensions(OMDictionary* dictionary)
{
    // first-pass: register all QSymbols
    
    while (getReader()->nextElement())
    {
        if (getReader()->elementEquals(getBaselineURI(), L"Extension"))
        {
            OMWString symbolspace;
            while (getReader()->nextElement())
            {
                if (getReader()->elementEquals(getBaselineURI(), L"Symbolspace"))
                {
                    const wchar_t* data;
                    OMUInt32 length;
                    getReader()->next();
                    if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                    {
                        throw OMException("Empty string is invalid Extension::Symbolspace "
                            "value");
                    }
                    getReader()->getCharacters(data, length);
                    symbolspace = data;
                    getReader()->moveToEndElement();
                }
                else if (getReader()->elementEquals(getBaselineURI(), L"Definitions"))
                {
                    while (getReader()->nextElement())
                    {
                        OMWString symbol;
                        OMUniqueObjectIdentification id = nullOMUniqueObjectIdentification;
                        while (getReader()->nextElement())
                        {
                            if (getReader()->elementEquals(getBaselineURI(), L"Identification"))
                            {
                                const wchar_t* data;
                                OMUInt32 length;
                                getReader()->next();
                                if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                                {
                                    throw OMException("Empty string is invalid Extension::Identification "
                                        "value");
                                }
                                getReader()->getCharacters(data, length);
                                uriToAUID(data, &id);
                                getReader()->moveToEndElement();
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
                                symbol = data;
                                getReader()->moveToEndElement();
                            }
                            else
                            {
                                getReader()->skipContent();
                            }
                        }

                        if (symbolspace.length() > 0 && 
                            symbol.length() > 0 && id != nullOMUniqueObjectIdentification)
                        {
                            _store->addMetaDefIdMap(id, symbolspace.c_str(), symbol.c_str());
                        }
                    }
                }
                else
                {
                    getReader()->skipContent();
                }
            }
        }
        else
        {
            getReader()->skipContent();
        }
    }

    
    
    // second-pass: restore the <Extension>s
    
    getReader()->reset();
    while (getReader()->nextElement() && 
        !getReader()->elementEquals(getBaselineURI(), L"Extensions"))
    {}
    
    // remember the symbolspaces so that after all classes defs and types defs have been
    // registered we can register the property defs, which are dependent on the class
    // defs being present, and extdendible enumeration members, which are dependent
    // on the extendible enumeration def being present
    OMVector<OMSymbolspace*> extensionSymbolspaces;
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        if (!getReader()->elementEquals(getBaselineURI(), L"Extension"))
        {
            throw OMException("Unknown element in Extensions - expected MetaDictionary");
        }
        
        OMSymbolspace* symbolspace = _store->createSymbolspace();
        symbolspace->restore(dictionary);
        _store->addSymbolspace(symbolspace);
        extensionSymbolspaces.append(symbolspace);
    }
    getReader()->moveToEndElement();
    
    // now register the property definitions and extendible enumeration extensions 
    OMUInt32 count = extensionSymbolspaces.count();
    for (OMUInt32 i = 0; i < count; i++)
    {
        OMSymbolspace* symbolspace = extensionSymbolspaces.getAt(i);
        symbolspace->registerDeferredDefs(dictionary);
    }
}

void
OMXMLStoredObject::restoreHeaderByteOrder(OMProperty* property)
{
    TRACE("OMXMLStoredObject::restoreHeaderByteOrder");

    OMSimpleProperty* simpleProp = dynamic_cast<OMSimpleProperty*>(property);
    ASSERT("Valid simple property", simpleProp != 0);
    getReader()->next();
    if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
    {
        throw OMException("Invalid Header::ByteOrder value - string is empty");
    }
    const wchar_t* data;
    OMUInt32 length;
    getReader()->getCharacters(data, length);
    
    OMByteArray bytes;
    headerByteOrderFromString(bytes, data);
    simpleProp->setBits(bytes.bytes(), bytes.size());
    
    getReader()->moveToEndElement();
}

void 
OMXMLStoredObject::restoreSimpleValue(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMType* type)
{
    TRACE("OMXMLStoredObject::restoreSimpleValue");

    switch (TypeCategoryVisitor(type).get())
    {
        case CHARACTER_CAT:
            restoreCharacter(bytes, attributes, str, dynamic_cast<const OMCharacterType*>(type)); 
            break;
        case ENUM_CAT:
            restoreEnum(bytes, attributes, str, dynamic_cast<const OMEnumeratedType*>(type)); 
            break;
        case EXT_ENUM_CAT:
            restoreExtEnum(bytes, attributes, str, dynamic_cast<const OMExtendibleEnumeratedType*>(type)); 
            break;
        case FIXED_ARRAY_CAT:
            restoreFixedArray(bytes, attributes, str, dynamic_cast<const OMFixedArrayType*>(type)); 
            break;
        case INDIRECT_CAT:
            restoreIndirect(bytes, attributes, str, dynamic_cast<const OMIndirectType*>(type)); 
            break;
        case INTEGER_CAT:
            restoreInteger(bytes, attributes, str, dynamic_cast<const OMIntegerType*>(type)); 
            break;
        case OPAQUE_CAT:
            restoreOpaque(bytes, attributes, str, dynamic_cast<const OMOpaqueType*>(type)); 
            break;
        case RECORD_CAT:
            restoreRecord(bytes, attributes, str, dynamic_cast<const OMRecordType*>(type)); 
            break;
        case RENAMED_CAT:
            restoreRenamed(bytes, attributes, str, dynamic_cast<const OMRenamedType*>(type)); 
            break;
        case SET_CAT:
            restoreSet(bytes, attributes, str, dynamic_cast<const OMSetType*>(type)); 
            break;
        case STRING_CAT:
            restoreString(bytes, attributes, str, dynamic_cast<const OMStringType*>(type)); 
            break;
        case VAR_ARRAY_CAT:
            restoreVariableArray(bytes, attributes, str, dynamic_cast<const OMVaryingArrayType*>(type)); 
            break;
        default:
            ASSERT("Valid simple value type category", false);
            break;
    }
}

void 
OMXMLStoredObject::restoreCharacter(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMCharacterType* type)
{
    TRACE("OMXMLStoredObject::restoreCharacter");

    bool isElementContent = (attributes != 0 && str == 0);

    bool isEscaped = false;
    if (attributes != 0)
    {
        const OMXMLAttribute* escapeAttr = getReader()->getAttribute(attributes,
            getBaselineURI(), AAFEscape_AttrName);
        if (escapeAttr != 0)
        {
            boolFromString(escapeAttr->getValue(), isEscaped);
        }
    }
    
    const wchar_t* data = str;
    if (str == 0)
    {
        OMUInt32 length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMException("Invalid character value - zero length string for character");
        }
        getReader()->getCharacters(data, length);
    }
    if (data == 0 || wcslen(data) == 0)
    {
        throw OMException("Invalid character value - zero length string for character");
    }

    wchar_t c;
    if (isEscaped)
    {
        c = unescapeCharacter(data);
    }
    else
    {
        if (wcslen(data) > 1)
        {
            throw OMException("Invalid character value - multiple characters present");
        }
        c = *data;
    }
    bytes.append(reinterpret_cast<const OMByte*>(&c), sizeof(wchar_t));
    
    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLStoredObject::restoreEnum(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMEnumeratedType* type)
{
    TRACE("OMXMLStoredObject::restoreEnum");

    bool isElementContent = (attributes != 0 && str == 0);
    
    const wchar_t* data = str;
    if (str == 0)
    {
        OMUInt32 length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMException("Invalid enumeration value - string is empty");
        }
        getReader()->getCharacters(data, length);
    }

    OMInt64 value;
    if (type->identification() == TypeID_Boolean)
    {
        bool boolValue;
        boolFromString(data, boolValue);
        if (boolValue)
        {
            value = 1; // kAAFTrue
        }
        else
        {
            value = 0; // kAAFFalse
        }
    }
    else
    {
        value = type->elementValueFromName(data);
    }
    
    // Can't call internalSize() - no parameters - because it will go to 
    // ImplAAFTypeDef::internalSize() which calls NativeSize() which for extensions 
    // will try de-reference the enum element type which will fail because
    // the OMFile::_root (RootStorable) must still be set
    // Note also that we assume that internalSize(externalBytes, externalBytesSize)
    // allows zero values for the parameters
    switch (type->internalSize(0,0))
    {
        case 1:
            {
                OMInt8 internalValue = (OMInt8)value;
                bytes.append(reinterpret_cast<OMByte*>(&internalValue), sizeof(OMInt8));
            }
            break;
        case 2:
            {
                OMInt16 internalValue = (OMInt16)value;
                bytes.append(reinterpret_cast<OMByte*>(&internalValue), sizeof(OMInt16));    
            }
            break;
        case 4:
            {
                OMInt32 internalValue = (OMInt32)value;
                bytes.append(reinterpret_cast<OMByte*>(&internalValue), sizeof(OMInt32));    
            }
            break;
        case 8:
            {
                bytes.append(reinterpret_cast<OMByte*>(&value), sizeof(OMInt64));    
            }
            break;
        default:
            ASSERT("Valid integer size", false);
            break;
    }
    
    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLStoredObject::restoreExtEnum(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMExtendibleEnumeratedType* type)
{
    TRACE("OMXMLStoredObject::restoreExtEnum");

    bool isElementContent = (attributes != 0 && str == 0);
    
    const wchar_t* data = str;
    if (str == 0)
    {
        OMUInt32 length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMException("Invalid ext enumeration value - string is empty");
        }
        getReader()->getCharacters(data, length);
    }

    OMUniqueObjectIdentification value = type->elementValueFromName(data);

    bytes.append(reinterpret_cast<OMByte*>(&value), sizeof(OMUniqueObjectIdentification));    
    
    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLStoredObject::restoreFixedArray(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMFixedArrayType* type)
{
    TRACE("OMXMLStoredObject::restoreFixedArray");

    OMType* elementType = type->elementType();
    const wchar_t* elementTypeSymbolspace;
    const wchar_t* elementTypeSymbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getMetaDefSymbol(id, &elementTypeSymbolspace, &elementTypeSymbol))
    {
        ASSERT("Fixed array element type definition registered in MetaDictionary and Symbolspace", false);
    }
    
    OMUInt32 count = 0;
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        if (!getReader()->elementEquals(elementTypeSymbolspace, elementTypeSymbol))
        {
            throw OMException("Invalid fixed array element symbol");
        }
        
        restoreSimpleValue(bytes, attrs, 0, elementType);
        
        count++;
    }
    getReader()->moveToEndElement();

    if (count != type->elementCount())
    {
        throw OMException("Fixed array does not have correct number of elements");
    }
    
}

void 
OMXMLStoredObject::restoreIndirect(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMIndirectType* type)
{
    TRACE("OMXMLStoredObject::restoreIndirect");
    PRECONDITION("Valid indirect state", attributes != 0 && str == 0);

    OMByteOrder byteOrder = hostByteOrder();
    OMInteger08Type intType;
    OMUInt32 externalByteOrderSize = intType.externalSize(
        reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder));
    bytes.grow(externalByteOrderSize);
    intType.externalize(reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder), 
        &(bytes.bytes()[bytes.size()]), externalByteOrderSize, byteOrder);
    bytes.setSize(bytes.size() + externalByteOrderSize);
    
    const OMXMLAttribute* typeIdAttr = getReader()->getAttribute(attributes,
        getBaselineURI(), ActualType_AttrName);
    if (typeIdAttr == 0)
    {
        throw OMException("Invalid indirect value - missing 'aaf:actualType' attribute");
    }
    OMUniqueObjectIdentification typeId = restoreAUID(typeIdAttr->getValue(), METADICT_DEF);

    OMUniqueObjectIdentificationType identType;
    OMUInt32 externalIdentSize = identType.externalSize(
        reinterpret_cast<OMByte*>(&typeId), sizeof(typeId));
    bytes.grow(externalIdentSize);
    identType.externalize(reinterpret_cast<OMByte*>(&typeId), sizeof(typeId), 
        &(bytes.bytes()[bytes.size()]), externalIdentSize, byteOrder);
    bytes.setSize(bytes.size() + externalIdentSize);

    OMByteArray actualBytes;    
    OMType* actualType = type->actualType(typeId);
    restoreSimpleValue(actualBytes, attributes, str, actualType);

    OMUInt32 externalBytesSize = actualType->externalSize(actualBytes.bytes(), 
        actualBytes.size());
    bytes.grow(externalBytesSize);
    actualType->externalize(actualBytes.bytes(), actualBytes.size(), 
        &(bytes.bytes()[bytes.size()]), externalBytesSize, byteOrder);
    bytes.setSize(bytes.size() + externalBytesSize);
}

void 
OMXMLStoredObject::restoreInteger(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMIntegerType* type)
{
    TRACE("OMXMLStoredObject::restoreInteger");

    bool isElementContent = (attributes != 0 && str == 0);
    
    const wchar_t* data = str;
    if (str == 0)
    {
        OMUInt32 length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMException("Invalid integer value - string is empty");
        }
        getReader()->getCharacters(data, length);
    }
    
    integerFromString(bytes, data, type->size(), type->isSigned());

    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLStoredObject::restoreOpaque(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMOpaqueType* type)
{
    TRACE("OMXMLStoredObject::restoreOpaque");
    PRECONDITION("Valid indirect state", attributes != 0 && str == 0);

    const OMXMLAttribute* byteOrderAttr = getReader()->getAttribute(attributes,
        getBaselineURI(), ByteOrder_AttrName);
    if (byteOrderAttr == 0)
    {
        throw OMException("Invalid opaque value - missing 'aaf:byteOrder' attribute");
    }
    OMByteOrder byteOrder;
    byteOrderFromString(byteOrderAttr->getValue(), &byteOrder);
    OMInteger08Type intType;
    OMUInt32 externalByteOrderSize = intType.externalSize(
        reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder));
    bytes.grow(externalByteOrderSize);
    intType.externalize(reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder), 
        &(bytes.bytes()[bytes.size()]), externalByteOrderSize, hostByteOrder());
    bytes.setSize(bytes.size() + externalByteOrderSize);
    
    const OMXMLAttribute* typeIdAttr = getReader()->getAttribute(attributes,
        getBaselineURI(), ActualType_AttrName);
    if (typeIdAttr == 0)
    {
        throw OMException("Invalid opaque value - missing 'aaf:actualType' attribute");
    }
    OMUniqueObjectIdentification typeId = restoreAUID(typeIdAttr->getValue(), METADICT_DEF);

    OMUniqueObjectIdentificationType identType;
    OMUInt32 externalIdentSize = identType.externalSize(
        reinterpret_cast<OMByte*>(&typeId), sizeof(typeId));
    bytes.grow(externalIdentSize);
    identType.externalize(reinterpret_cast<OMByte*>(&typeId), sizeof(typeId), 
        &(bytes.bytes()[bytes.size()]), externalIdentSize, hostByteOrder());
    if (byteOrder != hostByteOrder())
    {
        identType.reorder(&(bytes.bytes()[bytes.size()]), externalIdentSize); 
    }
    bytes.setSize(bytes.size() + externalIdentSize);
    
    getReader()->next();
    if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
    {
        const wchar_t* data;
        OMUInt32 length;
        getReader()->getCharacters(data, length);
        byteArrayFromString(bytes, data);
    }
    getReader()->moveToEndElement();
}

void 
OMXMLStoredObject::restoreRecord(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMRecordType* type)
{
    TRACE("OMXMLStoredObject::restoreRecord");

    bool isElementContent = (attributes != 0 && str == 0);

    if (type->identification() == TypeID_AUID)
    {
        const wchar_t* data = str;
        if (str == 0)
        {
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid AUID value");
            }
            getReader()->getCharacters(data, length);
        }
        
        OMUniqueObjectIdentification id = restoreAUID(data, ANY);
        bytes.append(reinterpret_cast<OMByte*>(&id), sizeof(OMUniqueObjectIdentification));

        if (isElementContent)
        {
            getReader()->moveToEndElement();
        }
    }
    else if (type->identification() == TypeID_MobIDType)
    {
        const wchar_t* data = str;
        if (str == 0)
        {
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid MobId value");
            }
            getReader()->getCharacters(data, length);
        }
        
        mobIdFromString(bytes, data);

        if (isElementContent)
        {
            getReader()->moveToEndElement();
        }
    }
    else if (type->identification() == TypeID_DateStruct)
    {
        const wchar_t* data = str;
        if (str == 0)
        {
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid DateStruct value");
            }
            getReader()->getCharacters(data, length);
        }
        
        dateStructFromString(bytes, data);

        if (isElementContent)
        {
            getReader()->moveToEndElement();
        }
    }
    else if (type->identification() == TypeID_TimeStruct)
    {
        const wchar_t* data = str;
        if (str == 0)
        {
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid TimeStruct value");
            }
            getReader()->getCharacters(data, length);
        }
        
        timeStructFromString(bytes, data);

        if (isElementContent)
        {
            getReader()->moveToEndElement();
        }
    }
    else if (type->identification() == TypeID_TimeStamp)
    {
        const wchar_t* data = str;
        if (str == 0)
        {
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid TimeStamp value");
            }
            getReader()->getCharacters(data, length);
        }
        
        timeStampFromString(bytes, data);

        if (isElementContent)
        {
            getReader()->moveToEndElement();
        }
    }
    else if (type->identification() == TypeID_Rational)
    {
        const wchar_t* data = str;
        if (str == 0)
        {
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid Rational value");
            }
            getReader()->getCharacters(data, length);
        }
        
        rationalFromString(bytes, data);

        if (isElementContent)
        {
            getReader()->moveToEndElement();
        }
    }
    else if (type->identification() == TypeID_VersionType)
    {
        const wchar_t* data = str;
        if (str == 0)
        {
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMException("Empty string is invalid VersionType value");
            }
            getReader()->getCharacters(data, length);
        }
        
        versionTypeFromString(bytes, data);

        if (isElementContent)
        {
            getReader()->moveToEndElement();
        }
    }
    else
    {
        OMUInt32 count = type->memberCount();
        for (OMUInt32 i = 0; i < count; i++)
        {
            const wchar_t* memberName = type->memberName(i);
            const OMType* memberType = type->memberType(i);

            if (!getReader()->nextElement())
            {
                throw OMException("Invalid record value - expecting member");
            }
            const wchar_t* nmspace;
            const wchar_t* localName;
            const OMList<OMXMLAttribute*>* attrs;
            getReader()->getStartElement(nmspace, localName, attrs);
            
            if (wcscmp(localName, memberName) != 0)
            {
                throw OMException("Invalid record value - unexpected member");
            }
            
            restoreSimpleValue(bytes, attrs, 0, memberType);
        }            

        if (isElementContent)
        {
            getReader()->nextEndElement();
        }
    }
}

void 
OMXMLStoredObject::restoreRenamed(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMRenamedType* type)
{
    TRACE("OMXMLStoredObject::restoreRenamed");

    restoreSimpleValue(bytes, attributes, str, type->renamedType());
}

void 
OMXMLStoredObject::restoreSet(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMSetType* type)
{
    TRACE("OMXMLStoredObject::restoreSet");

    OMType* elementType = type->elementType();
    const wchar_t* elementTypeSymbolspace;
    const wchar_t* elementTypeSymbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getMetaDefSymbol(id, &elementTypeSymbolspace, &elementTypeSymbol))
    {
        ASSERT("Set element type definition registered in MetaDictionary and Symbolspace", false);
    }
    
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        if (!getReader()->elementEquals(elementTypeSymbolspace, elementTypeSymbol))
        {
            throw OMException("Invalid set element symbol");
        }
        
        restoreSimpleValue(bytes, attrs, 0, elementType);
    }
    getReader()->moveToEndElement();
}

void 
OMXMLStoredObject::restoreString(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMStringType* type) // note: restoring string array result in type == 0
{
    TRACE("OMXMLStoredObject::restoreString");

    bool isElementContent = (attributes != 0 && str == 0);
    TypeCategory elementCat = UNKNOWN_CAT;
    if (type != 0)
    {
        elementCat = TypeCategoryVisitor(type).getElement();
    }
    if (type == 0 || elementCat == CHARACTER_CAT)
    {
        bool isEscaped = false;
        if (attributes != 0)
        {
            const OMXMLAttribute* escapeAttr = getReader()->getAttribute(attributes,
                getBaselineURI(), AAFEscape_AttrName);
            if (escapeAttr != 0)
            {
                boolFromString(escapeAttr->getValue(), isEscaped);
            }
        }
        
        const wchar_t* data = str;
        if (str == 0)
        {
            OMUInt32 length;
            getReader()->next();
            if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
            {
                getReader()->getCharacters(data, length);
            }
        }
        
        if (data == 0)
        {
            wchar_t* nullData = L"\0";    
            bytes.append(reinterpret_cast<OMByte*>(nullData), sizeof(wchar_t));
        }
        else
        {
            const wchar_t* s = data;
            wchar_t* tmp = 0;
            if (isEscaped)
            {
                tmp = unescapeString(data);
                s = tmp;
            }
            bytes.append(reinterpret_cast<const OMByte*>(s), 
                (wcslen(s) + 1) * sizeof(wchar_t));
            if (isEscaped)
            {
                delete [] tmp;
            }
        }
    }        
    else if (elementCat == INTEGER_CAT)
    {
        OMType* elementType = type->elementType();
        OMUInt32 elementInternalSize = elementType->internalSize();
        getReader()->next();
        if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
        {
            const wchar_t* data;
            OMUInt32 length;
            getReader()->getCharacters(data, length);
            
            const wchar_t* dataPtr = data;
            while (*dataPtr != L'\0')
            {
                restoreSimpleValue(bytes, 0, dataPtr, elementType);
                while (*dataPtr != L'\0' && *dataPtr != 0x20)
                {
                    dataPtr++;
                }
                while (*dataPtr != L'\0' && *dataPtr == 0x20)
                {
                    dataPtr++;
                }
            }
            
        }
        for (OMUInt32 i = 0; i < elementInternalSize; i++)
        {
            OMByte nullByte;
            bytes.append(&nullByte, 1);
        }
    }
    else
    {
        throw OMException("Invalid element type for string type");
    }
    
    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLStoredObject::restoreVariableArray(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* str, const OMVaryingArrayType* type)
{
    TRACE("OMXMLStoredObject::restoreVariableArray");

    if (type->identification() == TypeID_DataValue)
    {
        getReader()->next();
        if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
        {
            const wchar_t* data = 0;
            OMUInt32 length;
            getReader()->getCharacters(data, length);
            byteArrayFromString(bytes, data);
        }
        getReader()->moveToEndElement();
    }
    else if (type->identification() == TypeID_StringArray)
    {
        const wchar_t* stringTypeSymbolspace;
        const wchar_t* stringTypeSymbol;
        _store->getMetaDefSymbol(TypeID_String, &stringTypeSymbolspace, &stringTypeSymbol);
        
        while (getReader()->nextElement())
        {
            if (!getReader()->elementEquals(stringTypeSymbolspace, stringTypeSymbol))
            {
                throw OMException("Invalid string array element symbol");
            }
            
            const wchar_t* nmspace;
            const wchar_t* localName;
            const OMList<OMXMLAttribute*>* attrs;
            getReader()->getStartElement(nmspace, localName, attrs);
            restoreString(bytes, attrs, 0, 0);
        }
        getReader()->moveToEndElement();
    }
    else
    {
        OMType* elementType = type->elementType();
        const wchar_t* elementTypeSymbolspace;
        const wchar_t* elementTypeSymbol;
        OMUniqueObjectIdentification id = elementType->identification();
        if (!_store->getMetaDefSymbol(id, &elementTypeSymbolspace, &elementTypeSymbol))
        {
            ASSERT("Variable array element type definition registered in MetaDictionary and Symbolspace", false);
        }
        
        while (getReader()->nextElement())
        {
            const wchar_t* nmspace;
            const wchar_t* localName;
            const OMList<OMXMLAttribute*>* attrs;
            getReader()->getStartElement(nmspace, localName, attrs);
            if (!getReader()->elementEquals(elementTypeSymbolspace, elementTypeSymbol))
            {
                throw OMException("Invalid variable array element symbol");
            }
            
            restoreSimpleValue(bytes, attrs, 0, elementType);
        }
        getReader()->moveToEndElement();
    }
}

const OMType* 
OMXMLStoredObject::baseType(const OMType* type)
{
    TRACE("OMXMLStoredObject::baseType");

    if (TypeCategoryVisitor(type).get() == RENAMED_CAT)
    {
        return baseType((dynamic_cast<const OMRenamedType*>(type))->renamedType());
    }
    else
    {
        return type;
    }
}

void
OMXMLStoredObject::restoreWeakRef(OMFile* file, const OMType* type,
    OMUniqueObjectIdentification& id, OMPropertyTag& tag)
{
    TRACE("OMXMLStoredObject::restoreWeakRef");

    // insert the target path in the property table
    const OMWeakObjectReferenceType* wrType = 0;
    const OMType* bType = baseType(type);
    TypeCategoryVisitor baseCategory(bType);
    if (baseCategory.get() == WEAK_REF_CAT)
    {
        wrType = dynamic_cast<const OMWeakObjectReferenceType*>(bType);
    }
    else if (baseCategory.get() == FIXED_ARRAY_CAT ||
        baseCategory.get() == VAR_ARRAY_CAT)
    {
        const OMArrayType* arrayType = dynamic_cast<const OMArrayType*>(bType);
        wrType = dynamic_cast<const OMWeakObjectReferenceType*>(
            baseType(arrayType->elementType()));
    }
    else if (baseCategory.get() == SET_CAT)
    {
        const OMSetType* setType = dynamic_cast<const OMSetType*>(bType);
        wrType = dynamic_cast<const OMWeakObjectReferenceType*>(
            baseType(setType->elementType()));
    }
    ASSERT("Valid weak object reference type", wrType != 0);
    const OMPropertyId* targetPath = wrType->targetPath();
    OMPropertyTable* table = file->referencedProperties();
    tag = table->insert(targetPath);
    

    // restore the id
    // Pdn: Note that only weak references to objects identified by 
    // OMUniqueObjectIdentification is supported; this limitation
    // needs to be removed here and in the existing SS code
    getReader()->next();
    if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
    {
        const wchar_t* data;
        OMUInt32 length;
        getReader()->getCharacters(data, length);
        if (length == 0)
        {
            throw OMException("Missing weak reference value");
        }
        if (targetPath[0] == PropLocalID_Root_MetaDictionary)
        {
            id = restoreAUID(data, METADICT_DEF);
        }
        else if (targetPath[0] == PropLocalID_Root_Header && targetPath[1] == PropLocalID_Header_Dictionary)
        {
            id = restoreAUID(data, DICT_DEF);
        }
        else
        {
            id = restoreAUID(data, NON_DEF);
        }
    }
    else
    {
        throw OMException("Missing weak reference value");
    }
    getReader()->moveToEndElement();


    // also make sure that referenced MetaDictionary definitions are registered
    if (targetPath[0] == PropLocalID_Root_MetaDictionary)
    {
        if (targetPath[1] == PropLocalID_MetaDictionary_ClassDefinitions)
        {
            if (!file->dictionary()->registerClassDef(id))
            {
                throw OMException("Unknown ClassDefinition: failed to register "
                    "weak referenced ClassDefinition");
            }
        }
        else // TypeDefinitions
        {
            if (!file->dictionary()->registerTypeDef(id))
            {
                throw OMException("Unknown TypeDefinition: failed to register "
                    "weak referenced TypeDefinition");
            }
        }
    }
}

void 
OMXMLStoredObject::saveWeakRef(const void* identificationBits, const OMWeakObjectReferenceType* weakRefType)
{
    TRACE("OMXMLStoredObject::saveWeakRef");

    // save the id
    // TODO: currently assuming identification bits are for a OMUniqueObjectIdentification
    wchar_t* idStr = 0;
    OMUniqueObjectIdentification id = *(static_cast<const OMUniqueObjectIdentification*>(identificationBits));
    const OMPropertyId* targetPath = weakRefType->targetPath();
    if (targetPath[0] == PropLocalID_Root_MetaDictionary)
    {
        idStr = saveAUID(id, METADICT_DEF);
    }
    else if (targetPath[0] == PropLocalID_Root_Header && targetPath[1] == PropLocalID_Header_Dictionary)
    {
        idStr = saveAUID(id, DICT_DEF);
    }
    else
    {
        idStr = saveAUID(id, NON_DEF);
    }
    getWriter()->writeElementContent(idStr, wcslen(idStr));
    delete [] idStr;
}

OMUniqueObjectIdentification 
OMXMLStoredObject::restoreAUID(const wchar_t* idStr, AUIDTargetType targetType)
{
    TRACE("OMXMLStoredObject::restoreAUID");
    
    OMUniqueObjectIdentification id = nullOMUniqueObjectIdentification;
    if (isAUIDURI(idStr))
    {
        uriToAUID(idStr, &id);
    }
    else if (targetType == METADICT_DEF && isQSymbol(idStr))
    {
        id = _store->getMetaDefIdFromQSymbol(idStr);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMException("Could not retrieve unique id from qualified symbol");
        }
    }
    else
    {
        if (targetType == METADICT_DEF || targetType == ANY)
        {
            id = _store->getBaselineMetaDefId(idStr);
        }
        if (id == nullOMUniqueObjectIdentification && 
            (targetType == DICT_DEF || targetType == ANY))
        {
            id = _store->getBaselineDefId(idStr);
        }
        if (id == nullOMUniqueObjectIdentification && 
            targetType == DICT_DEF)
        {
            id = _store->getDefIdFromUIDStr(idStr);
        }
        
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMException("Could not retrieve unique id from symbol");
        }
    }
    
    return id;
}

wchar_t* 
OMXMLStoredObject::saveAUID(OMUniqueObjectIdentification id, AUIDTargetType targetType)
{
    TRACE("OMXMLStoredObject::saveAUID");
    
    wchar_t* idStr = 0;
    if (targetType == METADICT_DEF || targetType == ANY)
    {
        const wchar_t* symbol = _store->getBaselineMetaDefSymbol(id);
        if (symbol != 0)
        {
            idStr = wideCharacterStringDup(symbol);
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
        }
    }
    if (idStr == 0 && (targetType == DICT_DEF || targetType == ANY))
    {
        const wchar_t* symbol = _store->getBaselineDefSymbol(id);
        if (symbol != 0)
        {
            idStr = wideCharacterStringDup(symbol);
        }
        else if (targetType == DICT_DEF)
        {
            const wchar_t* uidStr = _store->getUIDStrFromId(id);
            if (uidStr != 0)
            {
                idStr = wideCharacterStringDup(uidStr);
            }
        }
    }
    if (idStr == 0)
    {
        idStr = new wchar_t[XML_MAX_AUID_URI_SIZE];
        auidToURI(id, idStr);
    }
    
    return idStr;
}
    
OMUniqueObjectIdentification
OMXMLStoredObject::getExtensionSymbolspaceId(OMFile& file)
{
    TRACE("OMXMLStoredObject::getExtensionSymbolspaceId");

    // traverse object tree to /Header/IdentificationList
    const OMPropertyId idListpath[3] = 
        {PropLocalID_Root_Header, PropLocalID_Header_IdentificationList, 0x0000}; 
    OMProperty* idListProp = file.findProperty(idListpath);
    OMStrongReferenceVector* idVector =
        dynamic_cast<OMStrongReferenceVector*>(idListProp);
    OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
        *idVector->iterator();
        
    // if no Identifications are present, then create an id
    if (!++iterator)
    {
        delete &iterator;
        return createUniqueIdentifier();
    }
    
    // position on last identification
    iterator.reset(OMAfter);
    --iterator;
    
    // extract the GenerationAUID property value
    OMStorable* ident = iterator.value().getValue();
    ASSERT("Valid identification storable", ident != 0);
    delete &iterator;
    OMProperty* generation = ident->findProperty(PropLocalID_Identification_GenerationAUID);
    ASSERT("Generation property present", generation != 0);

    OMUniqueObjectIdentification result =
        *(OMUniqueObjectIdentification*)(
            dynamic_cast<OMSimpleProperty*>(generation))->bits();

    return result;
}

void
OMXMLStoredObject::getDataStreams(OMStorable* storable, OMVector<OMDataStream*>& dataStreams)
{
    // iterate through the whole object tree and register each occurence of a DataStream property 
    OMPropertySet* properties = storable->propertySet();
    OMPropertySetIterator propIterator(*properties, OMBefore);
    while (++propIterator)
    {
        OMProperty* property = propIterator.property();
        if (!property->isOptional() || property->isPresent())
        {
            if (dynamic_cast<OMDataStream*>(property) != 0)
            {
                dataStreams.append(dynamic_cast<OMDataStream*>(property));
            }
            else if (dynamic_cast<OMStrongReference*>(property) != 0)
            {
                getDataStreams(dynamic_cast<OMStrongReference*>(property)->reference().getValue(), 
                    dataStreams);
            }
            else if (dynamic_cast<OMStrongReferenceVector*>(property) != 0)
            {
                OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
                    *((dynamic_cast<OMStrongReferenceVector*>(property))->iterator());
                while (++iterator)
                {
                    OMStrongReferenceVectorElement& element = iterator.value();
                    getDataStreams(element.reference().getValue(), dataStreams);
                }
                delete &iterator;
            }
            else if (dynamic_cast<OMStrongReferenceSet*>(property) != 0)
            {
                OMContainerIterator<OMStrongReferenceSetElement>& iterator = 
                    *(dynamic_cast<OMStrongReferenceSet*>(property)->iterator());
                while (++iterator)
                {
                    OMStrongReferenceSetElement& element = iterator.value();
                    getDataStreams(element.reference().getValue(), dataStreams);
                }
                delete &iterator;
            }
        }
    }
}

void
OMXMLStoredObject::registerExtensions(OMFile& file, OMSymbolspace* extSymbolspace)
{
    TRACE("OMXMLStoredObject::registerExtensions");

    OMDictionary* dictionary = file.dictionary();
    
    OMVector<OMClassDefinition*> classDefs;
    ClassDefinitionsIterator* classIter = dictionary->classDefinitions();
    while (++(*classIter))
    {
        OMClassDefinition* classDef = dynamic_cast<OMClassDefinition*>(classIter->currentObject());
        ASSERT("Object is correct type", classDef != 0);
        
        OMSymbolspace* symbolspace = _store->getSymbolspaceForMetaDef(classDef->identification());
        if (symbolspace == 0)
        {
            extSymbolspace->addClassDef(classDef);
        }
        else if (!_store->isBaselineSymbolspace(symbolspace))
        {
            symbolspace->addClassDef(classDef);
        }
        
        PropertyDefinitionsIterator* propIter = classDef->propertyDefinitions();
        while (++(*propIter))
        {
            OMPropertyDefinition* propertyDef = dynamic_cast<OMPropertyDefinition*>(propIter->currentObject());
            ASSERT("Object is correct type", propertyDef != 0);
            
            OMSymbolspace* symbolspace = _store->getSymbolspaceForMetaDef(
                propertyDef->identification());
            if (symbolspace == 0)
            {
                extSymbolspace->addPropertyDef(classDef, propertyDef);
            }
            else if (!_store->isBaselineSymbolspace(symbolspace))
            {
                symbolspace->addPropertyDef(classDef, propertyDef);
            }
        }
        delete propIter;
    }
    delete classIter;
    
//    TypeDefinitionsIterator* typeIter = dictionary->typeDefinitions();
//    while (++(*typeIter))
//    {
//        OMType* typeDef = dynamic_cast<OMType*>(typeIter->currentObject());
//        ASSERT("Object is correct type", typeDef != 0);
    OMVector<OMType*> typeDefs;
    dictionary->typeDefinitions(typeDefs);
    OMUInt32 i;
    for (i = 0; i < typeDefs.count(); i++)
    {
        OMType* typeDef = typeDefs.getAt(i);
        
        OMSymbolspace* typeDefSymbolspace = _store->getSymbolspaceForMetaDef(
            typeDef->identification());

        const OMType* baseTypeDef = baseType(typeDef);
        if (typeDefSymbolspace == 0)
        {
            typeDefSymbolspace = extSymbolspace;
            extSymbolspace->addTypeDef(typeDef);
        }
        else if (!_store->isBaselineSymbolspace(typeDefSymbolspace))
        {
            typeDefSymbolspace->addTypeDef(typeDef);
        }

        if (TypeCategoryVisitor(baseTypeDef).get() == EXT_ENUM_CAT)
        {
            const OMExtendibleEnumeratedType* extEnumTypeDef = 
                dynamic_cast<const OMExtendibleEnumeratedType*>(baseTypeDef);
            OMUniqueObjectIdentification typeId = extEnumTypeDef->identification();
                
            OMUInt32 count = extEnumTypeDef->elementCount();
            for (OMUInt32 j = 0; j < count; j++)
            {
                OMUniqueObjectIdentification value = extEnumTypeDef->elementValue(j);
                if (!_store->isKnownExtEnumElement(typeId, value))
                {
                    const wchar_t* name = extEnumTypeDef->elementName(j);
                    extSymbolspace->addExtEnumElement(typeId, name, value);
                }
            }
        }
    }
//    delete typeIter;
}

void
OMXMLStoredObject::createDictionaryMap()
{
    TRACE("OMXMLStoredObject::createDictionaryMap");

    // first-pass: register all aaf:uid to DefinitionObject:Identification mappings
    
    while (getReader()->nextElement())
    {
        if (getReader()->elementEquals(getBaselineURI(), HeaderDictionarySymbol))
        {
            // move to <Dictionary>
            getReader()->nextElement();
            
            // iterate over DefinitionObject sets
            OMWString symbolspace;
            while (getReader()->nextElement())
            {
                // iterate over DefinitionObjects
                while (getReader()->nextElement())
                {
                    // read the aaf:uid attribute
                    const wchar_t* nmspace;
                    const wchar_t* localName;
                    const OMList<OMXMLAttribute*>* attrs;
                    getReader()->getStartElement(nmspace, localName, attrs);
                    OMXMLAttribute* idAttr = getReader()->getAttribute(attrs,
                                             getBaselineURI(), UniqueId_AttrName);
                    if (idAttr == 0)
                    {
                        throw OMException("Missing aaf:uid attribute on DefinitionObject");
                    }
                    OMWString uidStr = idAttr->getValue();

                    // iterate over the properties and get DefinitionObject::Identification
                    OMUniqueObjectIdentification id = nullOMUniqueObjectIdentification;
                    while (getReader()->nextElement())
                    {
                        if (getReader()->elementEquals(getBaselineURI(), DefObjectIdentificationSymbol))
                        {
                            const wchar_t* data;
                            OMUInt32 length;
                            getReader()->next();
                            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
                            {
                                throw OMException("Empty string is invalid DefinitiobObject::Identification "
                                    "value");
                            }
                            getReader()->getCharacters(data, length);
                            uriToAUID(data, &id);
                            getReader()->moveToEndElement();
                        }
                        else
                        {
                            getReader()->skipContent();
                        }
                    }
                    if (id == nullOMUniqueObjectIdentification)
                    {
                        throw OMException("Missing DefinitionObject::Identification");
                    }

                    if (!isAUIDURI(uidStr.c_str()))
                    {
                        const wchar_t* defSymbol = _store->getDefSymbol(id);
                        if (defSymbol != 0)
                        {
                            if (wcscmp(defSymbol, uidStr.c_str()) != 0)
                            {
                                throw OMException("Invalid aaf:uid attribute value for "
                                    "baseline Definition");
                            }
                        }
                        else
                        {
                            _store->addDefIdMap(id, uidStr.c_str());
                        }
                    }
                }
            }
        }
        else
        {
            getReader()->skipContent();
        }
    }

    
    
    // return the reader to the <Header> element
    getReader()->reset();
    getReader()->nextElement();
    while (getReader()->nextElement() && 
        !getReader()->elementEquals(getBaselineURI(), L"Header"))
    {
        getReader()->skipContent();
    }
    
}

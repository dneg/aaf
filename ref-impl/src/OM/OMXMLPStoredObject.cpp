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
// Portions created by British Broadcasting Corporation are
// Copyright 2005, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=

#include "OMXMLPStoredObject.h"

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
#include "OMXMLPStoredStream.h"
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
#include "OMXMLException.h"
#include "OMRootStorable.h"
#include "OMPropertyTable.h"
#include "OMList.h"
#include "OMListIterator.h"

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

#include "OMIntegerType.h"
#include "OMUniqueObjectIdentType.h"



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



OMXMLPStoredObject*
OMXMLPStoredObject::openRead(OMRawStorage* rawStorage)
{
    TRACE("OMXMLPStoredObject::openRead");
    PRECONDITION("Compatible raw storage access mode", rawStorage->isReadable());
    OMXMLPStoredObject* result = new OMXMLPStoredObject(
        new OMXMLStorage(rawStorage, true), true);
    return result;
}

OMXMLPStoredObject*
OMXMLPStoredObject::openModify(OMRawStorage* ANAME(rawStorage))
{
    TRACE("OMXMLPStoredObject::openModify");
    PRECONDITION("Compatible raw storage access mode",
        rawStorage->isReadable() && rawStorage->isWritable());
    PRECONDITION("Compatible raw storage", rawStorage->isPositionable());
    ASSERT("Unimplemented code not reached", false);
    return 0;
}

OMXMLPStoredObject*
OMXMLPStoredObject::createWrite(OMRawStorage* rawStorage)
{
    TRACE("OMXMLPStoredObject::createWrite");
    PRECONDITION("Compatible raw storage access mode", rawStorage->isWritable());
    OMXMLPStoredObject* result = new OMXMLPStoredObject(
        new OMXMLStorage(rawStorage, false), true);
    return result;
}

OMXMLPStoredObject*
OMXMLPStoredObject::createModify(OMRawStorage* rawStorage)
{
    TRACE("OMXMLPStoredObject::createModify");

    PRECONDITION("Compatible raw storage access mode",
                 rawStorage->isReadable() && rawStorage->isWritable());
    OMXMLPStoredObject* result = new OMXMLPStoredObject(
        new OMXMLStorage(rawStorage, false), true);
    ASSERT("Valid heap pointer", result != 0);
    return result;
}


OMXMLPStoredObject::OMXMLPStoredObject(OMXMLStorage* store, bool isRoot)
: _store(store), _isRoot(isRoot)
{
    TRACE("OMXMLPStoredObject::OMXMLPStoredObject");
}

OMXMLPStoredObject::~OMXMLPStoredObject(void)
{
    TRACE("OMXMLPStoredObject::~OMXMLPStoredObject");

    if (_isRoot)
    {
        delete _store;
    }
}

OMStoredObject*
OMXMLPStoredObject::create(const wchar_t* /* name */)
{
    TRACE("OMXMLPStoredObject::create");

    return new OMXMLPStoredObject(_store, false);
}

OMStoredObject*
OMXMLPStoredObject::open(const wchar_t* /*name*/)
{
    TRACE("OMXMLPStoredObject::open");

    return new OMXMLPStoredObject(_store, false);
}

void
OMXMLPStoredObject::close(void)
{
    TRACE("OMXMLPStoredObject::close");
    if (_isRoot && !_store->isRead())
    {
        getWriter()->synchronize();
    }
}

OMByteOrder
OMXMLPStoredObject::byteOrder(void) const
{
    TRACE("OMXMLPStoredObject::byteOrder");
    return unspecified;
}
#include "utf8.h"
void
OMXMLPStoredObject::save(OMFile& file)
{
    TRACE("OMXMLPStoredObject::save(OMFile)");

    getWriter()->writeDocumentStart();
    getWriter()->writeComment(L"Generated by the AAF-XML prototype");
    getWriter()->writeComment(L"The AAF-XML format is subject to change");
    getWriter()->writeElementStart(getBaselineURI(), L"AAF");
    getWriter()->declareNamespace(getBaselineURI(), 0);
    getWriter()->declareNamespace(getBaselineURI(),
        _store->getBaselineSymbolspace()->getPrefix());

    OMList<OMStorable*> exts;
    OMUniqueObjectIdentification extId = getExtensionSymbolspaceId(file);
    OMSymbolspace* extSymbolspace = _store->createDefaultExtSymbolspace(extId);
    registerExtensions(file, extSymbolspace);

    if (!extSymbolspace->isEmpty())
    {
        getWriter()->declareNamespace(_store->getDefaultExtSymbolspace()->getURI(),
            _store->getDefaultExtSymbolspace()->getPrefix());

        getWriter()->writeElementStart(getBaselineURI(), L"Extensions");
        _store->getDefaultExtSymbolspace()->save();
        getWriter()->writeElementEnd();
    }

    file.clientRoot()->save();

    getWriter()->writeDocumentEnd();
}

void
OMXMLPStoredObject::save(OMStorable& object)
{
    TRACE("OMXMLPStoredObject::save(OMStorable)");

    const wchar_t* symbolspace;
    const wchar_t* symbol;
    if (_store->getSymbol(object.classId(), &symbolspace, &symbol))
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
        getWriter()->writeAttribute(getBaselineURI(), L"id", id);
        delete [] id;
    }

    save(*object.propertySet());

    getWriter()->writeElementEnd();
}

void
OMXMLPStoredObject::save(const OMStoredObjectIdentification& id)
{
    TRACE("OMXMLPStoredObject::save(OMStoredObjectIdentification)");
}

void
OMXMLPStoredObject::save(const OMPropertySet& properties )
{
    TRACE("OMXMLPStoredObject::save(OMPropertySet)");

    OMPropertySetIterator iterator(properties, OMBefore);
    while (++iterator)
    {
        const OMProperty* property = iterator.property();
        if (!property->isOptional() || property->isPresent())
        {
            // converting older files does not handle extendible enumerations 
            // properly. Properties with invalid ext. enum. values will not be saved.
            if (property->type()->category() == 
                OMMetaDefinition::EXT_ENUMERATED_TYPE)
            {
                const OMSimpleProperty* simpleProp = dynamic_cast<const OMSimpleProperty*>
                    (property);
                const OMUniqueObjectIdentification* id = 
                    reinterpret_cast<const OMUniqueObjectIdentification*>(
                        simpleProp->bits());
                const OMExtEnumeratedType* extEnumType = dynamic_cast<const OMExtEnumeratedType*>
                    (property->type());
                if (!extEnumType->isValidValue(*id))
                {
                    ASSERT("Invalid extendible enum. property can be removed",
                        property->isOptional());
                    
                    wchar_t uri[XML_MAX_OID_URI_SIZE];
                    oidToURI(*id, uri);
                    printf("Invalid extendible enumeration value encountered: %ls. "
                        "The property (%x, '%ls') will not be saved.\n", uri,
                        property->propertyId(), property->name());
                    continue;
                }
            }
            
            if (property->propertyId() != 0x0002) // Root::Header
            {
                const wchar_t* symbolspace;
                const wchar_t* symbol;
                OMUniqueObjectIdentification id =
                    property->definition()->identification();
                if (_store->getSymbol(id, &symbolspace, &symbol))
                {
                    getWriter()->writeElementStart(symbolspace, symbol);
                }
                else
                {
                    ASSERT("Property definition registered in MetaDictionary and Symbolspace", false);
                }
            }

            property->save();

            if (property->propertyId() != 0x0002) // Root::Header
            {
                getWriter()->writeElementEnd();
            }
        }
    }

}

void
OMXMLPStoredObject::save(const OMSimpleProperty& property)
{
    TRACE("OMXMLPStoredObject::save(OMSimpleProperty)");
    
    saveSimpleValue(property.bits(), property.bitsSize(), property.type(), true);
}

void
OMXMLPStoredObject::save(const OMDataVector& property)
{
    TRACE("OMXMLPStoredObject::save(OMDataVector)");

    OMType* elementType = dynamic_cast<const OMArrayType*>(property.type())->elementType();
    size_t elementInternalSize = elementType->internalSize(); 

    if (baseType(property.type())->identification() == TypeID_DataValue)
    {
        OMUInt8 buffer[256];
        unsigned int bufferPos = 0;
        OMDataContainerIterator* iter = property.createIterator();
        while (++(*iter))
        {
            buffer[bufferPos++] = *((const OMUInt8*)(iter->currentElement()));
            if (bufferPos == 256)
            {
                writeDataInHex(buffer, bufferPos, true);
            }
        }
        delete iter;
        if (bufferPos > 0)
        {
            writeDataInHex(buffer, bufferPos, true);
        }
    }
    else if (baseType(property.type())->identification() == TypeID_StringArray)
    {
        const wchar_t* stringTypeSymbolspace;
        const wchar_t* stringTypeSymbol;
        _store->getSymbol(TypeID_String, &stringTypeSymbolspace, &stringTypeSymbol);
        
        wchar_t stringBuffer[256];
        stringBuffer[255] = 0;
        unsigned int stringBufferPos = 0;
        wchar_t c = L'\0';
        OMDataContainerIterator* iter = property.createIterator();
        while (++(*iter))
        {
            if (c == L'\0') // c is the previous character at this point
            {
                getWriter()->writeElementStart(stringTypeSymbolspace, stringTypeSymbol);
            }
            
            c = *((const wchar_t*)(iter->currentElement()));
            stringBuffer[stringBufferPos++] = c;
            
            if (stringBufferPos == 255 || (stringBufferPos > 1 && c == L'\0'))
            {
                getWriter()->writeElementContent(stringBuffer, stringBufferPos - 1);
                stringBufferPos = 0;
            }
            
            if (c == L'\0')
            {
                getWriter()->writeElementEnd();
            }
        }
        ASSERT("Valid string array", c == L'\0');
        delete iter;
    }
    else
    {
        OMUniqueObjectIdentification id = elementType->identification();
        const wchar_t* symbolspace;
        const wchar_t* symbol;
        if (!_store->getSymbol(id, &symbolspace, &symbol))
        {
            ASSERT("Data Vector element type definition registered in MetaDictionary and Symbolspace", false);
        }
    
        OMDataContainerIterator* iter = property.createIterator();
        while (++(*iter))
        {
            getWriter()->writeElementStart(symbolspace, symbol);
            saveSimpleValue(iter->currentElement(), elementInternalSize, elementType, true); 
            getWriter()->writeElementEnd();
        }
        delete iter;
    }
}

void
OMXMLPStoredObject::save(const OMDataSet& property)
{
    TRACE("OMXMLPStoredObject::save(OMDataSet)");

    OMType* elementType = dynamic_cast<const OMArrayType*>(property.type())->elementType();
    size_t elementInternalSize = elementType->internalSize(); 
    const wchar_t* symbolspace;
    const wchar_t* symbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getSymbol(id, &symbolspace, &symbol))
    {
        ASSERT("Data set element type definition registered in MetaDictionary and Symbolspace", false);
    }

    OMDataContainerIterator* iter = property.createIterator();
    while (++(*iter))
    {
        getWriter()->writeElementStart(symbolspace, symbol);
        saveSimpleValue(iter->currentElement(), elementInternalSize, elementType, true); 
        getWriter()->writeElementEnd();
    }
    delete iter;
}

void
OMXMLPStoredObject::save(const OMStrongReference& singleton)
{
    TRACE("OMXMLPStoredObject::save(OMStrongReference)");
    singleton.reference().save();
}

void
OMXMLPStoredObject::save(const OMStrongReferenceVector& vector)
{
    TRACE("OMXMLPStoredObject::save(OMStrongReferenceVector)");

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
OMXMLPStoredObject::save(const OMStrongReferenceSet& set)
{
    TRACE("OMXMLPStoredObject::save(OMStrongReferenceSet)");

    OMKeySize keySize = set.keySize();

    OMContainerIterator<OMStrongReferenceSetElement>& iterator = *set.iterator();
    while (++iterator)
    {
        OMStrongReferenceSetElement& element = iterator.value();

        if (keySize == 16)
        {
            OMUniqueObjectIdentification* id =
                reinterpret_cast<OMUniqueObjectIdentification*>(
                    element.identification());
            const wchar_t* definitionSymbol = _store->getDefinitionSymbol(*id);
            if (definitionSymbol != 0)
            {
                _store->forwardObjectSetId(definitionSymbol);
            }
            else
            {
                wchar_t uri[XML_MAX_OID_URI_SIZE];
                oidToURI(*id, uri);
                _store->forwardObjectSetId(uri);
            }
        }
        else if (keySize == 32)
        {
            OMMaterialIdentification* umid =
                reinterpret_cast<OMMaterialIdentification*>(
                    element.identification());
            wchar_t uri[XML_MAX_UMID_URI_SIZE];
            umidToURI(*umid, uri);
            _store->forwardObjectSetId(uri);
        }
        else
        {
            ASSERT("Unimplemented code reached - only AUID or MobID as set id supported", false);
        }

        element.save();
    }
    delete &iterator;
}

void
OMXMLPStoredObject::save(const OMWeakReference& singleton)
{
    TRACE("OMXMLPStoredObject::save(OMWeakReference)");

    OMUniqueObjectIdentification oid = singleton.reference().identification();
    const wchar_t* definitionSymbol = _store->getDefinitionSymbol(oid);
    if (definitionSymbol != 0)
    {
        getWriter()->writeAttribute(getBaselineURI(), L"ref", definitionSymbol);
    }
    else
    {
        wchar_t uri[XML_MAX_OID_URI_SIZE];
        oidToURI(oid, uri);
        getWriter()->writeAttribute(getBaselineURI(), L"ref", uri);
    }
}

void
OMXMLPStoredObject::save(const OMWeakReferenceVector& vector)
{
    TRACE("OMXMLPStoredObject::save(OMWeakReferenceVector)");

    OMContainerIterator<OMWeakReferenceVectorElement>& iterator = *vector.iterator();
    while (++iterator)
    {
        const OMType* weakRefType = baseType( 
            (dynamic_cast<const OMArrayType*>(vector.type()))->elementType());
        const wchar_t* weakRefTypeSymbolspace;
        const wchar_t* weakRefTypeSymbol;
        if (!_store->getSymbol(weakRefType->identification(), &weakRefTypeSymbolspace, 
            &weakRefTypeSymbol))
        {
            ASSERT("Weak reference vector element type definition registered in MetaDictionary and Symbolspace", false);
        }
        getWriter()->writeElementStart(weakRefTypeSymbolspace, weakRefTypeSymbol);

        OMWeakReferenceVectorElement& element = iterator.value();
        OMObjectIdentification oid = element.reference().identification();
        const wchar_t* definitionSymbol = _store->getDefinitionSymbol(oid);
        if (definitionSymbol != 0)
        {
            getWriter()->writeAttribute(getBaselineURI(), L"ref", definitionSymbol);
        }
        else
        {
            wchar_t uri[XML_MAX_OID_URI_SIZE];
            oidToURI(oid, uri);
            getWriter()->writeAttribute(getBaselineURI(), L"ref", uri);
        }

        getWriter()->writeElementEnd();
    }
    delete &iterator;
}

void
OMXMLPStoredObject::save(const OMWeakReferenceSet& set)
{
    TRACE("OMXMLPStoredObject::save(OMWeakReferenceSet)");

    OMContainerIterator<OMWeakReferenceSetElement>& iterator = *set.iterator();
    while (++iterator)
    {
        const OMType* weakRefType = baseType( 
            (dynamic_cast<const OMSetType*>(set.type()))->elementType());
        const wchar_t* weakRefTypeSymbolspace;
        const wchar_t* weakRefTypeSymbol;
        if (!_store->getSymbol(weakRefType->identification(), &weakRefTypeSymbolspace, 
            &weakRefTypeSymbol))
        {
            ASSERT("Weak reference vector element type definition registered in MetaDictionary and Symbolspace", false);
        }
        getWriter()->writeElementStart(weakRefTypeSymbolspace, weakRefTypeSymbol);

        OMWeakReferenceSetElement& element = iterator.value();
        OMObjectIdentification oid = element.reference().identification();
        const wchar_t* definitionSymbol = _store->getDefinitionSymbol(oid);
        if (definitionSymbol != 0)
        {
            getWriter()->writeAttribute(getBaselineURI(), L"ref", definitionSymbol);
        }
        else
        {
            wchar_t uri[XML_MAX_OID_URI_SIZE];
            oidToURI(oid, uri);
            getWriter()->writeAttribute(getBaselineURI(), L"ref", uri);
        }

        getWriter()->writeElementEnd();
    }
    delete &iterator;
}

void
OMXMLPStoredObject::save(const OMPropertyTable* /* table */)
{
    TRACE("OMXMLPStoredObject::save(OMPropertyTable)");
    ASSERT("Unimplemented code not reached", false);
}

void
OMXMLPStoredObject::save(const OMDataStream& stream)
{
    TRACE("OMXMLPStoredObject::save(OMDataStream)");
}

OMRootStorable*
OMXMLPStoredObject::restore(OMFile& file)
{
    TRACE("OMXMLPStoredObject::restore(OMFile)");

    OMRootStorable* root = 0;
    try
    {
        OMFile::OMLoadMode originalLoadMode = file.loadMode();
        file.setLoadMode(OMFile::eagerLoad);

        // TODO read internal DTD
        printf("Skipping internal DTD if exists, %ls\n", getReader()->getPositionString());
        bool haveRootElement = false;
        while (!haveRootElement && getReader()->next())
        {
            if (getReader()->getEventType() == OMXMLReader::START_ELEMENT)
            {
                OMClassId id;
                file.rootStore()->restore(id);
                if (id != OMRootStorable::_rootClassId)
                {
                    throw OMXMLException(L"Invalid root element");
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
    catch (OMXMLException& ex)
    {
        printf("XML Exception: %ls\nXML parser position: %ls\n", ex.getMessage(),
            getReader()->getPositionString());
        OMXMLException newEx(ex.getMessage(), getReader()->getPositionString());
        throw newEx;
    }

    return root;
}

OMStorable*
OMXMLPStoredObject::restoreObject(const OMStrongObjectReference& reference)
{
    TRACE("OMXMLPStoredObject::restoreObject");

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
    // Attach the object.
    object->attach(containingObject, name);
    object->setStore(this);
    object->restoreContents();
    // TODO: find out why 'this' has been deleted and getReader() is invalid
    //ASSERT("Reader is positioned on end element", 
      //  getReader()->getEventType() == OMXMLReader::END_ELEMENT);
    return object;
}

void
OMXMLPStoredObject::restore(OMStoredObjectIdentification& id)
{
    TRACE("OMXMLPStoredObject::restore(OMStoredObjectIdentification)");

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
        id = _store->getId(nmspace, localName);
    }
}

void
OMXMLPStoredObject::restore(OMPropertySet& properties)
{
    TRACE("OMXMLPStoredObject::restore(OMPropertySet)");

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
            OMStrongReference* headerRef = dynamic_cast<OMStrongReference*>(
                properties.get(0x0002));
            headerRef->reference().restore();
        }
        else
        {
            throw OMXMLException(L"Header expected");
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
            OMPropertyId localId = _store->getPropertyId(nmspace, localName);
            if (localId == 0)
            {
                throw OMXMLException(L"Unknown property encountered");
            }
            OMProperty* property = properties.get(localId);
            property->restore(property->bitsSize());

            ASSERT("Reader is positioned on property end element", 
                getReader()->getEventType() == OMXMLReader::END_ELEMENT);
        }
    
        getReader()->moveToEndElement();
    }
}

void
OMXMLPStoredObject::restore(OMSimpleProperty& property,
                            size_t externalSize)
{
    TRACE("OMXMLPStoredObject::restore(OMSimpleProperty)");

    const wchar_t* nmspace;
    const wchar_t* localName;
    const OMList<OMXMLAttribute*>* attrs;
    getReader()->getStartElement(nmspace, localName, attrs);
    
    OMByteArray bytes;
    restoreSimpleValue(bytes, attrs, 0, property.type());
 
    // remove this line?!
    if (bytes.size() > 0)
    {
        property.setBits(bytes.bytes(), bytes.size());
    }
}

void
OMXMLPStoredObject::restore(OMDataVector& property,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMDataVector)");

    property.clear();
    
    const OMArrayType* arrayType = dynamic_cast<const OMArrayType*>(baseType(property.type()));
    if (arrayType->identification() == TypeID_DataValue)
    {
        getReader()->next();
        if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
        {
            const char* data = 0;
            size_t length;
            getReader()->getCharacters(data, length);
            
            OMByteArray bytes;
            byteArrayFromString(bytes, data);
            property.appendValue(bytes.bytes());
        }

        getReader()->moveToEndElement();
    }
    else if (arrayType->identification() == TypeID_StringArray)
    {
        const wchar_t* stringTypeSymbolspace;
        const wchar_t* stringTypeSymbol;
        _store->getSymbol(TypeID_String, &stringTypeSymbolspace, &stringTypeSymbol);
        
        while (getReader()->nextElement())
        {
            if (!getReader()->elementEquals(stringTypeSymbolspace, stringTypeSymbol))
            {
                throw OMXMLException(L"Invalid string array element symbol");
            }
            
            OMByteArray bytes;
            restoreString(bytes, 0, 0, 0);
            property.appendValue(bytes.bytes());
        }

        getReader()->moveToEndElement();
    }
    else
    {
        bool isFixedArray = false;
        OMUInt32 fixedArrayCount = 0;
        if (arrayType->category() == OMMetaDefinition::FIXED_ARRAY_TYPE)
        {
            isFixedArray = true;
            const OMFixedArrayType* fixedArrayType = dynamic_cast<const OMFixedArrayType*>(arrayType);
            fixedArrayCount = fixedArrayType->elementCount();
        }
    
        OMType* elementType = arrayType->elementType();
        const wchar_t* elementTypeSymbolspace;
        const wchar_t* elementTypeSymbol;
        OMUniqueObjectIdentification id = elementType->identification();
        if (!_store->getSymbol(id, &elementTypeSymbolspace, &elementTypeSymbol))
        {
            ASSERT("Array element type definition registered in MetaDictionary and Symbolspace", false);
        }

        while (getReader()->nextElement())
        {
            const wchar_t* nmspace;
            const wchar_t* localName;
            const OMList<OMXMLAttribute*>* attrs;
            getReader()->getStartElement(nmspace, localName, attrs);
            if (!getReader()->elementEquals(elementTypeSymbolspace, elementTypeSymbol))
            {
                throw OMXMLException(L"Invalid array element symbol");
            }
            
            OMByteArray bytes;
            restoreSimpleValue(bytes, attrs, 0, elementType);
            property.appendValue(bytes.bytes());
        }

        if (isFixedArray && property.count() != fixedArrayCount)
        {
            throw OMXMLException(L"Fixed array does not have correct number of elements");
        }

        getReader()->moveToEndElement();
    }
}

void
OMXMLPStoredObject::restore(OMDataSet& property,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMDataSet)");

    property.clear();
    
    const OMSetType* setType = dynamic_cast<const OMSetType*>(baseType(property.type()));
    OMType* elementType = setType->elementType();
    const wchar_t* elementTypeSymbolspace;
    const wchar_t* elementTypeSymbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getSymbol(id, &elementTypeSymbolspace, &elementTypeSymbol))
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
            throw OMXMLException(L"Invalid set element symbol");
        }
        
        OMByteArray bytes;
        restoreSimpleValue(bytes, attrs, 0, elementType);
        property.insert(bytes.bytes());
    }
    getReader()->moveToEndElement();
}

void
OMXMLPStoredObject::restore(OMStrongReference& singleton,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMStrongReference)");

    bool haveNextElement = getReader()->nextElement();
    if (!haveNextElement)
    {
        throw OMXMLException(L"Empty strong reference singleton found");
    }

    wchar_t* name = referenceName(singleton.name(), singleton.propertyId());
    OMStrongObjectReference newReference(&singleton, name);
    singleton.reference() = newReference;
    singleton.reference().restore();
    delete [] name;

    getReader()->nextEndElement();
}

void
OMXMLPStoredObject::restore(OMStrongReferenceVector& vector,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMStrongReferenceVector)");

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
OMXMLPStoredObject::restore(OMStrongReferenceSet& set,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMStrongReferenceSet)");

    OMPropertyId setId = set.propertyId();
    const wchar_t* setName = set.name();

    OMUInt32 localKey = 0;
    ;
    OMKeySize keySize = set.keySize();
    OMByte key[32]; // maxsize(OMMaterialIdentification, OMUniqueObjectIdentification)

    while (getReader()->nextElement())
    {
        wchar_t* name = elementName(setName, setId, localKey);

        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);

        OMXMLAttribute* idAttr = getReader()->getAttribute(attrs,
                                 getBaselineURI(), L"id");
        if (idAttr == 0)
        {
            throw OMXMLException(L"Object in strong reference set is missing a "
                                 L"aaf:id attribute");
        }
        readSetId(key, keySize, idAttr->getValue());

        OMStrongReferenceSetElement element(&set, name, localKey, key, keySize);
        element.restore();
        set.insert(key, element);

        delete [] name;
        localKey++;
    }
    getReader()->moveToEndElement();
}

void
OMXMLPStoredObject::restore(OMWeakReference& singleton,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMWeakReference)");

    OMUniqueObjectIdentification id;
    OMPropertyTag tag;
    restoreWeakRef(singleton.propertySet()->container()->file(), 
        singleton.definition()->type(), id, tag);
    
    OMWeakObjectReference& reference = singleton.reference();
    singleton.setTargetTag(tag);
    reference = OMWeakObjectReference(&singleton, id, tag);
    reference.restore();

    getReader()->moveToEndElement();
}

void
OMXMLPStoredObject::restore(OMWeakReferenceVector&  vector,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMWeakReferenceVector)");

    OMUInt32 index = 0;
    OMList<OMWeakReferenceVectorElement> elements;
    while (getReader()->nextElement())
    {
        OMUniqueObjectIdentification id;
        OMPropertyTag tag;
        restoreWeakRef(vector.propertySet()->container()->file(), 
            vector.definition()->type(), id, tag);
        
        OMWeakReferenceVectorElement element(&vector, id, tag);
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
OMXMLPStoredObject::restore(OMWeakReferenceSet& set,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMWeakReferenceSet)");

    while (getReader()->nextElement())
    {
        OMUniqueObjectIdentification id;
        OMPropertyTag tag;
        restoreWeakRef(set.propertySet()->container()->file(), set.definition()->type(),
            id, tag);
            
        OMWeakReferenceSetElement element(&set, id, tag);
        element.restore();
        set.insert(&id, element);

        getReader()->moveToEndElement();
    }
    getReader()->moveToEndElement();
}

void
OMXMLPStoredObject::restore(OMPropertyTable*& /* table */)
{
    TRACE("OMXMLPStoredObject::restore(OMPropertyTable)");
    ASSERT("Unimplemented code not reached", false);
}

void
OMXMLPStoredObject::restore(OMDataStream& /* stream */,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMDataStream)");
    getReader()->skipContent();
}

OMStoredStream*
OMXMLPStoredObject::openStoredStream(
    const OMDataStream& /* property */)
{
    TRACE("OMXMLPStoredObject::openStoredStream");
    
    OMRawStorage* store = OMCachedDiskRawStorage::openNewModify();
    OMXMLPStoredStream* result = new OMXMLPStoredStream(store);
    return result;
}

OMStoredStream*
OMXMLPStoredObject::createStoredStream(
    const OMDataStream& /* property */)
{
    TRACE("OMXMLPStoredObject::createStoredStream");

    OMRawStorage* store = OMCachedDiskRawStorage::openNewModify();
    OMXMLPStoredStream* result = new OMXMLPStoredStream(store);
    return result;
}

OMXMLWriter*
OMXMLPStoredObject::getWriter()
{
    TRACE("OMXMLPStoredObject::getWriter");
    
    return _store->getWriter();
}

OMXMLReader*
OMXMLPStoredObject::getReader()
{
    TRACE("OMXMLPStoredObject::getReader");
    
    return _store->getReader();
}

const wchar_t*
OMXMLPStoredObject::getBaselineURI()
{
    TRACE("OMXMLPStoredObject::getBaselineURI");
    
    return _store->getBaselineSymbolspace()->getURI();
}

void 
OMXMLPStoredObject::saveSimpleValue(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveSimpleValue");

    switch (type->category())
    {
        case OMMetaDefinition::CHARACTER_TYPE:
            saveCharacter(internalBytes, internalSize, dynamic_cast<const OMCharacterType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::ENUMERATED_TYPE:
            saveEnum(internalBytes, internalSize, dynamic_cast<const OMEnumeratedType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::EXT_ENUMERATED_TYPE:
            saveExtEnum(internalBytes, internalSize, dynamic_cast<const OMExtEnumeratedType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::FIXED_ARRAY_TYPE:
            saveFixedArray(internalBytes, internalSize, dynamic_cast<const OMFixedArrayType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::INDIRECT_TYPE:
            saveIndirect(internalBytes, internalSize, dynamic_cast<const OMIndirectType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::INTEGER_TYPE:
            saveInteger(internalBytes, internalSize, dynamic_cast<const OMIntType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::OPAQUE_TYPE:
            saveOpaque(internalBytes, internalSize, dynamic_cast<const OMOpaqueType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::RECORD_TYPE:
            saveRecord(internalBytes, internalSize, dynamic_cast<const OMRecordType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::RENAMED_TYPE:
            saveRenamed(internalBytes, internalSize, dynamic_cast<const OMRenamedType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::SET_TYPE:
            saveSet(internalBytes, internalSize, dynamic_cast<const OMSetType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::STRING_TYPE:
            saveString(internalBytes, internalSize, dynamic_cast<const OMStringType*>(type), 
                isElementContent);
            break;
        case OMMetaDefinition::VARIABLE_ARRAY_TYPE:
            saveVariableArray(internalBytes, internalSize, dynamic_cast<const OMVariableArrayType*>(type), 
                isElementContent);
            break;
        default:
            ASSERT("Valid simple value type category", false);
            break;
    }
}

void 
OMXMLPStoredObject::saveCharacter(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMCharacterType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveCharacter");
    
    wchar_t buffer[2];
    buffer[0] = *(reinterpret_cast<const wchar_t*>(internalBytes));
    buffer[1] = 0;

    // TODO: AAF escaping for characters not representable in XML, e.g. L'\0'
    if (buffer[0] != L'\0')
    {
        if (isElementContent)
        {
            getWriter()->writeElementContent(buffer, 1);
        }
        else
        {
            getWriter()->writeAttributeContent(buffer);
        }
    }
}

void 
OMXMLPStoredObject::saveEnum(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMEnumeratedType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveEnum");
    
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
    wchar_t* name = type->elementNameFromValue(value);
    
    if (isElementContent)
    {
        getWriter()->writeElementContent(name, lengthOfWideString(name));
    }
    else
    {
        getWriter()->writeAttributeContent(name);
    }
    
    delete [] name;
}

void 
OMXMLPStoredObject::saveExtEnum(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMExtEnumeratedType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveExtEnum");
    
    const OMUniqueObjectIdentification* id = 
        reinterpret_cast<const OMUniqueObjectIdentification*>(internalBytes);
    wchar_t* name = type->elementNameFromValue(*id);
    
    if (isElementContent)
    {
        getWriter()->writeElementContent(name, lengthOfWideString(name));
    }
    else
    {
        getWriter()->writeAttributeContent(name);
    }
    
    delete [] name;
}

void 
OMXMLPStoredObject::saveFixedArray(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMFixedArrayType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveFixedArray");

    OMType* elementType = type->elementType();
    const wchar_t* symbolspace;
    const wchar_t* symbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getSymbol(id, &symbolspace, &symbol))
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
OMXMLPStoredObject::saveIndirect(const OMByte* externalBytes, OMUInt16 externalSize, 
    const OMIndirectType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveIndirect");

    OMType* actualType = type->actualType(externalBytes, externalSize);
    OMByteOrder byteOrder = type->byteOrder(externalBytes, externalSize);

    const OMByte* actualData;
    size_t actualDataSize;
    type->actualData(externalBytes, externalSize, actualData, actualDataSize);
    
    size_t actualInternalSize = actualType->internalSize(actualData, actualDataSize);
    OMByte* actualInternalData = new OMByte[actualInternalSize]; 
    actualType->internalize(actualData, actualDataSize, actualInternalData, 
        actualInternalSize, byteOrder);
        
    wchar_t uri[XML_MAX_OID_URI_SIZE];
    oidToURI(actualType->identification(), uri);
    getWriter()->writeAttribute(getBaselineURI(), L"typeId", uri);
        
    saveSimpleValue(actualInternalData, actualInternalSize, actualType, isElementContent);
        
    delete [] actualInternalData;

}

void 
OMXMLPStoredObject::saveInteger(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMIntType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveInteger");
    
    wchar_t intStr[XML_MAX_INTEGER_STRING_SIZE];
    integerToString(internalBytes, type->size(), type->isSigned(), intStr);
    
    if (isElementContent)
    {
        getWriter()->writeElementContent(intStr, lengthOfWideString(intStr));
    }
    else
    {
        getWriter()->writeAttributeContent(intStr);
    }
    
}

void 
OMXMLPStoredObject::saveOpaque(const OMByte* externalBytes, OMUInt16 externalSize, 
    const OMOpaqueType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveOpaque");

    OMUniqueObjectIdentification actualTypeId = type->actualTypeId(externalBytes, 
        externalSize);
    OMByteOrder byteOrder = type->byteOrder(externalBytes, externalSize);

    const OMByte* actualData;
    size_t actualDataSize;
    type->actualData(externalBytes, externalSize, actualData, actualDataSize);
    
    wchar_t uri[XML_MAX_OID_URI_SIZE];
    oidToURI(actualTypeId, uri);
    getWriter()->writeAttribute(getBaselineURI(), L"typeId", uri);
    
    wchar_t byteOrderStr[XML_MAX_BYTE_ORDER_STRING_SIZE];
    byteOrderToString(byteOrder, &byteOrderStr[0]);
    getWriter()->writeAttribute(getBaselineURI(), L"byteOrder", byteOrderStr);

    writeDataInHex(actualData, actualDataSize, isElementContent);    
        
}

void 
OMXMLPStoredObject::saveRecord(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMRecordType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveRecord");
    
    if (type->identification() == TypeID_AUID)
    {
        const OMUniqueObjectIdentification* id = 
            reinterpret_cast<const OMUniqueObjectIdentification*>(internalBytes);
        wchar_t uri[XML_MAX_OID_URI_SIZE];
        oidToURI(*id, uri);
        if (isElementContent)
        {
            getWriter()->writeElementContent(uri, lengthOfWideString(uri));
        }
        else
        {
            getWriter()->writeAttributeContent(uri);
        }
    }
    else if (type->identification() == TypeID_MobIDType)
    {
        const OMMaterialIdentification* umid =
            reinterpret_cast<const OMMaterialIdentification*>(internalBytes);
        wchar_t uri[XML_MAX_UMID_URI_SIZE];
        umidToURI(*umid, uri);
        if (isElementContent)
        {
            getWriter()->writeElementContent(uri, lengthOfWideString(uri));
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
            getWriter()->writeElementContent(dateStr, lengthOfWideString(dateStr));
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
            getWriter()->writeElementContent(timeStr, lengthOfWideString(timeStr));
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
            getWriter()->writeElementContent(timeStampStr, lengthOfWideString(timeStampStr));
        }
        else
        {
            getWriter()->writeAttributeContent(timeStampStr);
        }
    }
    else
    {
        const wchar_t* symbolspace;
        const wchar_t* symbol;
        OMUniqueObjectIdentification id = type->identification();
        if (!_store->getSymbol(id, &symbolspace, &symbol))
        {
            ASSERT("Record type definition registered in MetaDictionary and Symbolspace", false);
        }
                    
        const OMByte* memberBytes = internalBytes;
        OMUInt32 count = type->memberCount();
        for (OMUInt32 i = 0; i < count; i++)
        {
            wchar_t* name = type->memberName(i);
            OMType* memberType = type->memberType(i);
            
            getWriter()->writeElementStart(symbolspace, name);
            saveSimpleValue(memberBytes, memberType->internalSize(), memberType,
                isElementContent);
            getWriter()->writeElementEnd();
            
            delete [] name;
            memberBytes += memberType->internalSize();
        }
    }
}

void 
OMXMLPStoredObject::saveRenamed(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMRenamedType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveRenamed");

    saveSimpleValue(internalBytes, internalSize, type->renamedType(), isElementContent);    
}

void 
OMXMLPStoredObject::saveSet(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMSetType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveSet");

    OMType* elementType = type->elementType();
    const wchar_t* symbolspace;
    const wchar_t* symbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getSymbol(id, &symbolspace, &symbol))
    {
        ASSERT("Set element type definition registered in MetaDictionary and Symbolspace", false);
    }

    const OMByte* elementBytes = internalBytes;
    OMUInt16 count = internalSize;
    while (count > 0)
    {
        getWriter()->writeElementStart(symbolspace, symbol);
        saveSimpleValue(elementBytes, elementType->internalSize(), elementType,
            isElementContent);
        getWriter()->writeElementEnd();
        
        count -= elementType->internalSize();
    }
}

void 
OMXMLPStoredObject::saveString(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMStringType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveString");
    
    ASSERT("TODO: integer types", type->elementType()->category() == OMMetaDefinition::CHARACTER_TYPE);
    
    OMByteArray modifiedBytes; 
    const wchar_t* str = reinterpret_cast<const wchar_t*>(internalBytes);
    size_t size = internalSize / sizeof(wchar_t);
    size_t count = size;
    while (count > 0 && str[count - 1] != L'\0')
    {
        count--;
    }
    if (count == 0)
    {
        const wchar_t n = L'\0';
        modifiedBytes.append(internalBytes, internalSize);
        modifiedBytes.append(reinterpret_cast<const OMByte*>(&n), sizeof(wchar_t));
        str = reinterpret_cast<const wchar_t*>(modifiedBytes.bytes());
        size++;
        printf("Invalid string value encountered ('%ls') - string was not null terminated\n", str);
    }
    else
    {
        size = count;
    }
        
    if (size > 1)
    {
        // TODO: AAF escaping for characters not representable in XML
        
        // some strings in indirect types where found to not be null terminated
        
        if (isElementContent)
        {
            getWriter()->writeElementContent(str, size - 1); 
        }
        else
        {
            getWriter()->writeAttributeContent(str);
        }
    }
}

void 
OMXMLPStoredObject::saveVariableArray(const OMByte* internalBytes, OMUInt16 internalSize, 
    const OMVariableArrayType* type, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::saveVariableArray");

    if (type->identification() == TypeID_DataValue)
    {
        writeDataInHex(internalBytes, internalSize, isElementContent);    
    }
    else if (type->identification() == TypeID_StringArray)
    {
        const wchar_t* stringTypeSymbolspace;
        const wchar_t* stringTypeSymbol;
        _store->getSymbol(TypeID_String, &stringTypeSymbolspace, &stringTypeSymbol);
        
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
                // TODO: AAF escaping for characters not representable in XML
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
        if (!_store->getSymbol(id, &symbolspace, &symbol))
        {
            ASSERT("Variable array element type definition registered in MetaDictionary and Symbolspace", false);
        }
    
        const OMByte* elementBytes = internalBytes;
        OMUInt16 count = internalSize;
        while (count > 0)
        {
            getWriter()->writeElementStart(symbolspace, symbol);
            saveSimpleValue(elementBytes, elementType->internalSize(), elementType,
                isElementContent);
            getWriter()->writeElementEnd();
            
            count -= elementType->internalSize();
        }
    }
}

void
OMXMLPStoredObject::writeDataInHex(const OMByte* data, size_t size, bool isElementContent)
{
    TRACE("OMXMLPStoredObject::writeDataInHex");

    const wchar_t* hexDigits = L"0123456789ABCDEF";

    wchar_t buffer[255];
    buffer[254] = 0;
    size_t bufferPos = 0;
    for (size_t i = 0; i < size; i++)
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
OMXMLPStoredObject::restoreExtensions(OMDictionary* dictionary)
{
    while (getReader()->nextElement())
    {
        const wchar_t* nmspace;
        const wchar_t* localName;
        const OMList<OMXMLAttribute*>* attrs;
        getReader()->getStartElement(nmspace, localName, attrs);
        if (!getReader()->elementEquals(getBaselineURI(), L"MetaDictionary"))
        {
            throw OMXMLException(L"Unknown element in Extensions - expected MetaDictionary");
        }
        
        OMSymbolspace* symbolspace = _store->createSymbolspace();
        symbolspace->restore(dictionary);
        _store->addSymbolspace(symbolspace);
    }
    getReader()->moveToEndElement();
}

void 
OMXMLPStoredObject::restoreSimpleValue(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMType* type)
{
    TRACE("OMXMLPStoredObject::restoreSimpleValue");

    switch (type->category())
    {
        case OMMetaDefinition::CHARACTER_TYPE:
            restoreCharacter(bytes, attributes, str, dynamic_cast<const OMCharacterType*>(type)); 
            break;
        case OMMetaDefinition::ENUMERATED_TYPE:
            restoreEnum(bytes, attributes, str, dynamic_cast<const OMEnumeratedType*>(type)); 
            break;
        case OMMetaDefinition::EXT_ENUMERATED_TYPE:
            restoreExtEnum(bytes, attributes, str, dynamic_cast<const OMExtEnumeratedType*>(type)); 
            break;
        case OMMetaDefinition::FIXED_ARRAY_TYPE:
            restoreFixedArray(bytes, attributes, str, dynamic_cast<const OMFixedArrayType*>(type)); 
            break;
        case OMMetaDefinition::INDIRECT_TYPE:
            restoreIndirect(bytes, attributes, str, dynamic_cast<const OMIndirectType*>(type)); 
            break;
        case OMMetaDefinition::INTEGER_TYPE:
            restoreInteger(bytes, attributes, str, dynamic_cast<const OMIntType*>(type)); 
            break;
        case OMMetaDefinition::OPAQUE_TYPE:
            restoreOpaque(bytes, attributes, str, dynamic_cast<const OMOpaqueType*>(type)); 
            break;
        case OMMetaDefinition::RECORD_TYPE:
            restoreRecord(bytes, attributes, str, dynamic_cast<const OMRecordType*>(type)); 
            break;
        case OMMetaDefinition::RENAMED_TYPE:
            restoreRenamed(bytes, attributes, str, dynamic_cast<const OMRenamedType*>(type)); 
            break;
        case OMMetaDefinition::SET_TYPE:
            restoreSet(bytes, attributes, str, dynamic_cast<const OMSetType*>(type)); 
            break;
        case OMMetaDefinition::STRING_TYPE:
            restoreString(bytes, attributes, str, dynamic_cast<const OMStringType*>(type)); 
            break;
        case OMMetaDefinition::VARIABLE_ARRAY_TYPE:
            restoreVariableArray(bytes, attributes, str, dynamic_cast<const OMVariableArrayType*>(type)); 
            break;
        default:
            ASSERT("Valid simple value type category", false);
            break;
    }
}

void 
OMXMLPStoredObject::restoreCharacter(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMCharacterType* type)
{
    TRACE("OMXMLPStoredObject::restoreCharacter");

    bool isElementContent = (attributes != 0 && str == 0);
    
    const char* data = str;
    if (str == 0)
    {
        size_t length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMXMLException(L"Invalid character value");
        }
        getReader()->getCharacters(data, length);
    }
    
    wchar_t* wData = convertToWideString(data);
    if (lengthOfWideString(wData) != 1)
    {
        throw OMXMLException(L"Invalid character value - only 1 character allowed");
    }
    bytes.append(reinterpret_cast<OMByte*>(wData), 
        (lengthOfWideString(wData)) * sizeof(wchar_t));
    delete [] wData;
    
    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLPStoredObject::restoreEnum(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMEnumeratedType* type)
{
    TRACE("OMXMLPStoredObject::restoreEnum");

    bool isElementContent = (attributes != 0 && str == 0);
    
    const char* data = str;
    if (str == 0)
    {
        size_t length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMXMLException(L"Invalid enumeration value - string is empty");
        }
        getReader()->getCharacters(data, length);
    }

    wchar_t* wData = convertToWideString(data);
    OMInt64 value = type->elementValueFromName(wData);
    delete [] wData;

    bytes.append(reinterpret_cast<OMByte*>(&value), sizeof(OMInt64));    
    
    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLPStoredObject::restoreExtEnum(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMExtEnumeratedType* type)
{
    TRACE("OMXMLPStoredObject::restoreExtEnum");

    bool isElementContent = (attributes != 0 && str == 0);
    
    const char* data = str;
    if (str == 0)
    {
        size_t length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMXMLException(L"Invalid ext enumeration value - string is empty");
        }
        getReader()->getCharacters(data, length);
    }

    wchar_t* wData = convertToWideString(data);
    OMUniqueObjectIdentification value = type->elementValueFromName(wData);
    delete [] wData;

    bytes.append(reinterpret_cast<OMByte*>(&value), sizeof(OMUniqueObjectIdentification));    
    
    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLPStoredObject::restoreFixedArray(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMFixedArrayType* type)
{
    TRACE("OMXMLPStoredObject::restoreFixedArray");

    OMType* elementType = type->elementType();
    const wchar_t* elementTypeSymbolspace;
    const wchar_t* elementTypeSymbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getSymbol(id, &elementTypeSymbolspace, &elementTypeSymbol))
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
            throw OMXMLException(L"Invalid fixed array element symbol");
        }
        
        restoreSimpleValue(bytes, attrs, 0, elementType);
        
        count++;
    }
    getReader()->moveToEndElement();

    if (count != type->elementCount())
    {
        throw OMXMLException(L"Fixed array does not have correct number of elements");
    }
    
}

void 
OMXMLPStoredObject::restoreIndirect(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMIndirectType* type)
{
    TRACE("OMXMLPStoredObject::restoreIndirect");
    PRECONDITION("Valid indirect state", attributes != 0 && str == 0);

    OMByteOrder byteOrder = hostByteOrder();
    OMInteger08Type intType;
    size_t externalByteOrderSize = intType.externalSize(
        reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder));
    bytes.grow(externalByteOrderSize);
    intType.externalize(reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder), 
        &(bytes.bytes()[bytes.size()]), externalByteOrderSize, byteOrder);
    bytes.setSize(bytes.size() + externalByteOrderSize);
    
    const OMXMLAttribute* typeIdAttr = getReader()->getAttribute(attributes,
        getBaselineURI(), L"typeId");
    if (typeIdAttr == 0)
    {
        throw OMXMLException(L"Invalid indirect value - missing 'aaf:typeId' attribute");
    }
    OMUniqueObjectIdentification typeId;
    char* typeIdStr = convertFromWideString(typeIdAttr->getValue());
    uriToAUID(typeIdStr, &typeId);
    delete [] typeIdStr;
    OMUniqueObjectIdentificationType identType;
    size_t externalIdentSize = identType.externalSize(
        reinterpret_cast<OMByte*>(&typeId), sizeof(typeId));
    bytes.grow(externalIdentSize);
    identType.externalize(reinterpret_cast<OMByte*>(&typeId), sizeof(typeId), 
        &(bytes.bytes()[bytes.size()]), externalIdentSize, byteOrder);
    bytes.setSize(bytes.size() + externalIdentSize);

    OMByteArray actualBytes;    
    OMType* actualType = type->actualType(typeId);
    restoreSimpleValue(actualBytes, attributes, str, actualType);

    size_t externalBytesSize = actualType->externalSize(actualBytes.bytes(), 
        actualBytes.size());
    bytes.grow(externalBytesSize);
    actualType->externalize(actualBytes.bytes(), actualBytes.size(), 
        &(bytes.bytes()[bytes.size()]), externalBytesSize, byteOrder);
    bytes.setSize(bytes.size() + externalBytesSize);
}

void 
OMXMLPStoredObject::restoreInteger(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMIntType* type)
{
    TRACE("OMXMLPStoredObject::restoreInteger");

    bool isElementContent = (attributes != 0 && str == 0);
    
    const char* data = str;
    if (str == 0)
    {
        size_t length;
        getReader()->next();
        if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
        {
            throw OMXMLException(L"Invalid integer value - string is empty");
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
OMXMLPStoredObject::restoreOpaque(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMOpaqueType* type)
{
    TRACE("OMXMLPStoredObject::restoreOpaque");
    PRECONDITION("Valid indirect state", attributes != 0 && str == 0);

    const OMXMLAttribute* byteOrderAttr = getReader()->getAttribute(attributes,
        getBaselineURI(), L"byteOrder");
    if (byteOrderAttr == 0)
    {
        throw OMXMLException(L"Invalid opaque value - missing 'aaf:byteOrder' attribute");
    }
    OMByteOrder byteOrder;
    byteOrderFromString(byteOrderAttr->getValue(), &byteOrder);
    OMInteger08Type intType;
    size_t externalByteOrderSize = intType.externalSize(
        reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder));
    bytes.grow(externalByteOrderSize);
    intType.externalize(reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder), 
        &(bytes.bytes()[bytes.size()]), externalByteOrderSize, byteOrder);
    bytes.setSize(bytes.size() + externalByteOrderSize);
    
    const OMXMLAttribute* typeIdAttr = getReader()->getAttribute(attributes,
        getBaselineURI(), L"typeId");
    if (typeIdAttr == 0)
    {
        throw OMXMLException(L"Invalid opaque value - missing 'aaf:typeId' attribute");
    }
    OMUniqueObjectIdentification typeId;
    char* typeIdStr = convertFromWideString(typeIdAttr->getValue());
    uriToAUID(typeIdStr, &typeId);
    delete [] typeIdStr;
    OMUniqueObjectIdentificationType identType;
    size_t externalIdentSize = identType.externalSize(
        reinterpret_cast<OMByte*>(&typeId), sizeof(typeId));
    bytes.grow(externalIdentSize);
    identType.externalize(reinterpret_cast<OMByte*>(&typeId), sizeof(typeId), 
        &(bytes.bytes()[bytes.size()]), externalIdentSize, byteOrder);
    bytes.setSize(bytes.size() + externalIdentSize);
    
    getReader()->next();
    if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
    {
        const char* data = 0;
        size_t length;
        getReader()->getCharacters(data, length);
        byteArrayFromString(bytes, data);
    }
    getReader()->moveToEndElement();
}

void 
OMXMLPStoredObject::restoreRecord(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMRecordType* type)
{
    TRACE("OMXMLPStoredObject::restoreRecord");

    bool isElementContent = (attributes != 0 && str == 0);

    if (type->identification() == TypeID_AUID)
    {
        const char* data = str;
        if (str == 0)
        {
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid AUID value");
            }
            getReader()->getCharacters(data, length);
        }
        
        auidFromString(bytes, data);

        if (isElementContent)
        {
            getReader()->moveToEndElement();
        }
    }
    else if (type->identification() == TypeID_MobIDType)
    {
        const char* data = str;
        if (str == 0)
        {
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid MobId value");
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
        const char* data = str;
        if (str == 0)
        {
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid DateStruct value");
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
        const char* data = str;
        if (str == 0)
        {
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid TimeStruct value");
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
        const char* data = str;
        if (str == 0)
        {
            size_t length;
            getReader()->next();
            if (getReader()->getEventType() != OMXMLReader::CHARACTERS)
            {
                throw OMXMLException(L"Empty string is invalid TimeStamp value");
            }
            getReader()->getCharacters(data, length);
        }
        
        timeStampFromString(bytes, data);

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
            wchar_t* memberName = type->memberName(i);
            OMType* memberType = type->memberType(i);

            if (!getReader()->nextElement())
            {
                throw OMXMLException(L"Invalid record value - expecting member");
            }
            const wchar_t* nmspace;
            const wchar_t* localName;
            const OMList<OMXMLAttribute*>* attrs;
            getReader()->getStartElement(nmspace, localName, attrs);
            
            if (compareWideString(localName, memberName) != 0)
            {
                throw OMXMLException(L"Invalid record value - unexpected member");
            }
            delete [] memberName;
            
            restoreSimpleValue(bytes, attrs, 0, memberType);
        }            

        if (isElementContent)
        {
            getReader()->nextEndElement();
        }
    }
}

void 
OMXMLPStoredObject::restoreRenamed(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMRenamedType* type)
{
    TRACE("OMXMLPStoredObject::restoreRenamed");

    restoreSimpleValue(bytes, attributes, str, type->renamedType());
}

void 
OMXMLPStoredObject::restoreSet(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMSetType* type)
{
    TRACE("OMXMLPStoredObject::restoreSet");

    OMType* elementType = type->elementType();
    const wchar_t* elementTypeSymbolspace;
    const wchar_t* elementTypeSymbol;
    OMUniqueObjectIdentification id = elementType->identification();
    if (!_store->getSymbol(id, &elementTypeSymbolspace, &elementTypeSymbol))
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
            throw OMXMLException(L"Invalid set element symbol");
        }
        
        restoreSimpleValue(bytes, attrs, 0, elementType);
    }
    getReader()->moveToEndElement();
}

void 
OMXMLPStoredObject::restoreString(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMStringType* type) // note: restoring string array result in type == 0
{
    TRACE("OMXMLPStoredObject::restoreString");

    ASSERT("TODO: integer types", type == 0 || type->elementType()->category() == OMMetaDefinition::CHARACTER_TYPE);
    
    bool isElementContent = (attributes != 0 && str == 0);
    
    const char* data = str;
    if (str == 0)
    {
        size_t length;
        getReader()->next();
        if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
        {
            getReader()->getCharacters(data, length);
        }
    }
    
    if (data == 0)
    {
        wchar_t* wData = L"\0";    
        bytes.append(reinterpret_cast<OMByte*>(wData), sizeof(wchar_t));
    }
    else
    {
        wchar_t* wData = convertToWideString(data);
        bytes.append(reinterpret_cast<OMByte*>(wData), 
            (lengthOfWideString(wData) + 1) * sizeof(wchar_t));
        delete [] wData;
    }
    
    if (isElementContent)
    {
        getReader()->moveToEndElement();
    }
}

void 
OMXMLPStoredObject::restoreVariableArray(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
    const char* str, const OMVariableArrayType* type)
{
    TRACE("OMXMLPStoredObject::restoreVariableArray");

    if (type->identification() == TypeID_DataValue)
    {
        getReader()->next();
        if (getReader()->getEventType() == OMXMLReader::CHARACTERS)
        {
            const char* data = 0;
            size_t length;
            getReader()->getCharacters(data, length);
            byteArrayFromString(bytes, data);
        }
        getReader()->moveToEndElement();
    }
    else if (type->identification() == TypeID_StringArray)
    {
        const wchar_t* stringTypeSymbolspace;
        const wchar_t* stringTypeSymbol;
        _store->getSymbol(TypeID_String, &stringTypeSymbolspace, &stringTypeSymbol);
        
        while (getReader()->nextElement())
        {
            if (!getReader()->elementEquals(stringTypeSymbolspace, stringTypeSymbol))
            {
                throw OMXMLException(L"Invalid string array element symbol");
            }
            
            restoreString(bytes, 0, 0, 0);
        }
        getReader()->moveToEndElement();
    }
    else
    {
        OMType* elementType = type->elementType();
        const wchar_t* elementTypeSymbolspace;
        const wchar_t* elementTypeSymbol;
        OMUniqueObjectIdentification id = elementType->identification();
        if (!_store->getSymbol(id, &elementTypeSymbolspace, &elementTypeSymbol))
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
                throw OMXMLException(L"Invalid variable array element symbol");
            }
            
            restoreSimpleValue(bytes, attrs, 0, elementType);
        }
        getReader()->moveToEndElement();
    }
}

const OMType* 
OMXMLPStoredObject::baseType(const OMType* type)
{
    TRACE("OMXMLPStoredObject::baseType");

    if (type->category() == OMMetaDefinition::RENAMED_TYPE)
    {
        return baseType((dynamic_cast<const OMRenamedType*>(type))->renamedType());
    }
    else
    {
        return type;
    }
}

void
OMXMLPStoredObject::restoreWeakRef(OMFile* file, const OMType* type,
    OMUniqueObjectIdentification& id, OMPropertyTag& tag)
{
    TRACE("OMXMLPStoredObject::restoreWeakRef");

    // Pdn: Note that only weak references to objects identified by 
    // OMUniqueObjectIdentification is supported; this limitation
    // needs to be removed here and in the existing SS code
    
    
    const wchar_t* nmspace;
    const wchar_t* localName;
    const OMList<OMXMLAttribute*>* attrs;
    getReader()->getStartElement(nmspace, localName, attrs);

    OMXMLAttribute* refAttr = getReader()->getAttribute(attrs, getBaselineURI(), 
        L"ref");
    if (refAttr == 0)
    {
        throw OMXMLException(L"Weak reference is missing a aaf:ref attribute");
    }
    id = readRefId(refAttr->getValue());

    const OMWeakObjectReferenceType* wrType = 0;
    const OMType* bType = baseType(type);
    if (bType->category() == OMMetaDefinition::WEAK_REF_TYPE)
    {
        wrType = dynamic_cast<const OMWeakObjectReferenceType*>(bType);
    }
    else if (bType->category() == OMMetaDefinition::FIXED_ARRAY_TYPE ||
        bType->category() == OMMetaDefinition::VARIABLE_ARRAY_TYPE)
    {
        const OMArrayType* arrayType = dynamic_cast<const OMArrayType*>(bType);
        wrType = dynamic_cast<const OMWeakObjectReferenceType*>(
            baseType(arrayType->elementType()));
    }
    else if (bType->category() == OMMetaDefinition::SET_TYPE)
    {
        const OMSetType* setType = dynamic_cast<const OMSetType*>(bType);
        wrType = dynamic_cast<const OMWeakObjectReferenceType*>(
            baseType(setType->elementType()));
    }
    ASSERT("Valid weak object reference type", wrType != 0);
    
    const OMPropertyId* targetPath = wrType->targetPath();
    OMPropertyTable* table = file->referencedProperties();
    tag = table->insert(targetPath);
    
    
    // also make sure that referenced MetaDictionary definitions are registered
    if (targetPath[0] == 0x0001) // MetaDictionary
    {
        if (targetPath[1] == 0x0003) // ClassDefinitions
        {
            if (!file->dictionary()->registerClassDef(id))
            {
                throw OMXMLException(L"Unknown ClassDefinition: failed to register "
                    L"weak referenced ClassDefinition");
            }
        }
        else // TypeDefinitions
        {
            if (!file->dictionary()->registerTypeDef(id))
            {
                throw OMXMLException(L"Unknown TypeDefinition: failed to register "
                    L"weak referenced TypeDefinition");
            }
        }
    }
}

OMUniqueObjectIdentification
OMXMLPStoredObject::getExtensionSymbolspaceId(OMFile& file)
{
    TRACE("OMXMLPStoredObject::getExtensionSymbolspaceId");

    // traverse object tree to /Header/IdentificationList
    const OMPropertyId idListpath[3] = {0x0002, 0x3B06, 0x0}; 
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
    OMProperty* generation = ident->findProperty(0x3C09); // GenerationAUID
    ASSERT("Generation property present", generation != 0);

    OMUniqueObjectIdentification result =
        *(OMUniqueObjectIdentification*)(
            dynamic_cast<OMSimpleProperty*>(generation))->bits();

    return result;
}

void
OMXMLPStoredObject::readSetId(OMByte* key, OMKeySize keySize, const wchar_t* idStr)
{
    TRACE("OMXMLPStoredObject::readSetId");

    if (isURI(idStr))
    {
        if (keySize == 16)
        {
            uriToOID(idStr, reinterpret_cast<OMUniqueObjectIdentification*>(key));
        }
        else if (keySize == 32)
        {
            uriToUMID(idStr, reinterpret_cast<OMMaterialIdentification*>(key));
        }
        else
        {
            ASSERT("Set key size either 16 for AUID or 32 for MobID", false);
        }
    }
    else
    {
        OMUniqueObjectIdentification id = _store->getDefinitionId(idStr);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMXMLException(L"Unknown definition symbol encountered");
        }
        memcpy(key, &id, sizeof(OMUniqueObjectIdentification));
    }
}

OMUniqueObjectIdentification
OMXMLPStoredObject::readRefId(const wchar_t* refStr)
{
    TRACE("OMXMLPStoredObject::readRefId");
    
    // Pdn: Note that only weak references to objects identified by 
    // OMUniqueObjectIdentification is supported; this limitation
    // needs to be removed here and in the existing SS code

    OMUniqueObjectIdentification id;
    if (isURI(refStr))
    {
        uriToOID(refStr, &id);
    }
    else
    {
        id = _store->getDefinitionId(refStr);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMXMLException(L"Unknown definition symbol encountered");
        }
    }

    return id;
}

void
OMXMLPStoredObject::registerExtensions(OMFile& file, OMSymbolspace* extSymbolspace)
{
    TRACE("OMXMLPStoredObject::registerExtensions");

    OMDictionary* dictionary = file.dictionary();
    
    OMVector<OMClassDefinition*> classDefs;
    dictionary->classDefinitions(classDefs);
    size_t i;
    for (i = 0; i < classDefs.count(); i++)
    {
        OMClassDefinition* classDef = classDefs.getAt(i);
        
        bool isExtClassDef = false;
        const wchar_t* symbolspaceURI;
        const wchar_t* symbol;
        if (!_store->getSymbol(classDef->identification(), &symbolspaceURI, &symbol))
        {
            extSymbolspace->addClassDef(classDef);
            isExtClassDef = true;
        }
        
        OMVector<OMPropertyDefinition*> propertyDefs;
        classDef->propertyDefinitions(propertyDefs);
        size_t j;
        for (j = 0; j < propertyDefs.count(); j++)
        {
            OMPropertyDefinition* propertyDef = propertyDefs.getAt(j);
            
            const wchar_t* symbolspaceURI;
            const wchar_t* symbol;
            if (isExtClassDef || 
                !_store->getSymbol(propertyDef->identification(), &symbolspaceURI, &symbol))
            {
                extSymbolspace->addPropertyDef(classDef, propertyDef);
            }
        }
    }
    
    OMVector<OMType*> typeDefs;
    dictionary->typeDefinitions(typeDefs);
    for (i = 0; i < typeDefs.count(); i++)
    {
        OMType* typeDef = typeDefs.getAt(i);
        const wchar_t* symbolspaceURI;
        const wchar_t* symbol;
        if (!_store->getSymbol(typeDef->identification(), &symbolspaceURI, &symbol))
        {
            extSymbolspace->addTypeDef(typeDef);
        }
    }
}



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

void
OMXMLPStoredObject::save(OMFile& file)
{
    TRACE("OMXMLPStoredObject::save(OMFile)");

    getWriter()->writeDocumentStart();
    getWriter()->writeComment(L"Note: the AAF-XML format is subject to change");
    getWriter()->writeElementStart(getBaselineURI(), L"AAF");
    getWriter()->declareNamespace(getBaselineURI(), 0);
    getWriter()->declareNamespace(getBaselineURI(),
        _store->getBaselineSymbolspace()->getPrefix());

    OMList<OMStorable*> exts;
    OMUniqueObjectIdentification generation = getGeneration(file);
    OMSymbolspace* extSymbolspace = _store->createDefaultExtSymbolspace(generation);
    getExtensions(file, extSymbolspace, exts);

    if (exts.count() != 0)
    {
        getWriter()->declareNamespace(_store->getDefaultExtSymbolspace()->getURI(),
            _store->getDefaultExtSymbolspace()->getPrefix());

        getWriter()->writeElementStart(getBaselineURI(), L"Extensions");
        _store->getDefaultExtSymbolspace()->save(exts);
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
            if (property->propertyId() != 0x0002) // Root::Header
            {
                const wchar_t* symbolspace;
                const wchar_t* symbol;
                OMUniqueObjectIdentification id =
                    property->definition()->uniqueIdentification();
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
}

void
OMXMLPStoredObject::save(const OMDataVector& property)
{
    TRACE("OMXMLPStoredObject::save(OMDataVector)");

}

void
OMXMLPStoredObject::save(const OMDataSet& property)
{
    TRACE("OMXMLPStoredObject::save(OMDataSet)");
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
            OMUniqueObjectIdentification* oid =
                reinterpret_cast<OMUniqueObjectIdentification*>(
                    element.identification());
            const wchar_t* definitionSymbol = _store->getDefinitionSymbol(*oid);
            if (definitionSymbol != 0)
            {
                _store->forwardObjectSetId(definitionSymbol);
            }
            else
            {
                wchar_t uri[XML_MAX_OID_URI_SIZE];
                oidToURI(*oid, uri);
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
        OMWeakReferenceVectorElement& element = iterator.value();
        OMObjectIdentification oid = element.reference().identification();

        // TODO: get the symbol for the weak ref type
        getWriter()->writeElementStart(getBaselineURI(), L"Todo-WeakRefTypeSymbol");

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
        OMWeakReferenceSetElement& element = iterator.value();
        OMObjectIdentification oid = element.reference().identification();

        // TODO: get the symbol for the weak ref type
        getWriter()->writeElementStart(getBaselineURI(), L"Todo-WeakRefTypeSymbol");

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
        
        // TODO: restore Extensions
        // root->setClassFactory(file.dictionary());
        root->setDictionary(file.dictionary());

        // restore the Header
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
            // TODO: read Extensions
            printf("Skipping extensions, %ls\n", getReader()->getPositionString());
            getReader()->skipContent();
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
                // TODO: will be removed and replaced with exception
                printf("Skipping unknown property (%d, {%ls, %ls})\n", localId, 
                    nmspace, localName);
                getReader()->skipContent();
            }
            else
            {
                OMProperty* property = properties.get(localId);
                printf("Restoring (%d, {%ls, %ls})\n", localId, nmspace,
                    localName);
                property->restore(property->bitsSize());
            }
        }
    }
}

void
OMXMLPStoredObject::restore(OMSimpleProperty& property,
                            size_t externalSize)
{
    TRACE("OMXMLPStoredObject::restore(OMSimpleProperty)");

    // TODO: remove this stuff
    
    if (externalSize == 0)
    {
        OMByte* data = new OMByte[16];
        memset(data, 0, 16);
        property.setBits(data, 16);
        delete [] data;
    }
    else
    {
        OMByte* data = new OMByte[externalSize];
        if (externalSize == 16 || externalSize == 32)
        {
            memset(data, 1, externalSize);
        }
        else
        {
            memset(data, 0, externalSize);
        }
        property.setBits(data, externalSize);
        delete [] data;
    }

    getReader()->skipContent();
}

void
OMXMLPStoredObject::restore(OMDataVector& /* property */,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMDataVector)");
    getReader()->skipContent();
}

void
OMXMLPStoredObject::restore(OMDataSet& /* property */,
                            size_t /* externalSize */)
{
    TRACE("OMXMLPStoredObject::restore(OMDataSet)");
    getReader()->skipContent();
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

    vector.setLocalKey(localKey);
    vector.grow(localKey);

    OMListIterator<OMStrongReferenceVectorElement> iter(elements, OMAfter);
    while (--iter)
    {
        localKey--;
        vector.insert(localKey, iter.value());
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

        // TODO: This will be removed once we read in simple values
        // this hack ensures the set key equals the property uniquely 
        // identifying the object
        {
            OMStorable* storable = element.getValue();
            OMProperty* keyProperty;
            
            // handle special case for OperationGroup::Parameters
            const OMUniqueObjectIdentification propIDOperationGroupParameters = 
               {0x06010104, 0x060a, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x02}};
            if (set.definition()->uniqueIdentification() == propIDOperationGroupParameters)
            {
                keyProperty = storable->findProperty(0x4c01); // Parameter::Definition
            }
            else
            {
                keyProperty = storable->findProperty(set.keyPropertyId());
            }
            ASSERT("Valid key property for object in strong ref set", keyProperty != 0);
            keyProperty->setBits(key, keySize);
        }

        delete [] name;
        localKey++;
    }
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

    getReader()->nextEndElement();
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

        index++;

        getReader()->nextEndElement();
    }

    vector.setLocalKey(index);
    vector.grow(index);

    OMListIterator<OMWeakReferenceVectorElement> iter(elements, OMAfter);
    while (--iter)
    {
        index--;
        vector.insert(index, iter.value());
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

        getReader()->nextEndElement();
    }
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
    
    OMPropertyTable* table = file->referencedProperties();
    const OMPropertyId* targetPath = type->getTargetPath();
    tag = table->insert(targetPath);
    
    // make sure that referenced MetaDictionary definitions are registered
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
OMXMLPStoredObject::getGeneration(OMFile& file)
{
    TRACE("OMXMLPStoredObject::getGeneration");

    // /Header/IdentificationList
    const OMPropertyId idListpath[3] = {0x0002, 0x3B06, 0x0}; 

    OMProperty* idListProp = file.findProperty(idListpath);
    OMStrongReferenceVector* idVector =
        dynamic_cast<OMStrongReferenceVector*>(idListProp);
    OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
        *idVector->iterator();
    bool haveElements = ++iterator;
    ASSERT("At least one Identification present", haveElements);
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
        OMClassId id = _store->getId(idStr);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMXMLException(L"Unknown definition symbol encountered");
        }
        memcpy(key, &id, sizeof(OMClassId));
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
        id = _store->getId(refStr);
        if (id == nullOMUniqueObjectIdentification)
        {
            throw OMXMLException(L"Unknown definition symbol encountered");
        }
    }

    return id;
}

// NOTE: this function is temporary
void
OMXMLPStoredObject::getExtensions(OMFile& file, OMSymbolspace* extSymbolspace,
                                  OMList<OMStorable*>& exts)
{
    TRACE("OMXMLPStoredObject::getExtensions(OMFile, ...)");

    OMDictionary* dictionary = file.dictionary();

    OMProperty* classDefsProp = dictionary->findProperty(0x0003); // ClassDefinitions
    if (classDefsProp != 0)
    {
        OMStrongReferenceSet* classDefSet =
            dynamic_cast<OMStrongReferenceSet*>(classDefsProp);
        OMContainerIterator<OMStrongReferenceSetElement>& iterator =
            *classDefSet->iterator();
        while (++iterator)
        {
            OMStorable* classDef = iterator.value().getValue();
            getExtensions(classDef, extSymbolspace, exts);
        }
        delete &iterator;
    }

    OMProperty* typeDefsProp = dictionary->findProperty(0x0004); // TypeDefinitions
    if (typeDefsProp != 0)
    {
        OMStrongReferenceSet* typeDefSet =
            dynamic_cast<OMStrongReferenceSet*>(typeDefsProp);
        OMContainerIterator<OMStrongReferenceSetElement>& iterator =
            *typeDefSet->iterator();
        while (++iterator)
        {
            OMStorable* typeDef = iterator.value().getValue();
            getExtensions(typeDef, extSymbolspace, exts);
        }
        delete &iterator;
    }
}

// NOTE: this function is temporary
void
OMXMLPStoredObject::getExtensions(OMStorable* storable, OMSymbolspace* extSymbolspace,
                                  OMList<OMStorable*>& exts)
{
    TRACE("OMXMLPStoredObject::getExtensions(OMStorable, ...)");

    const OMClassId classDefId =
        {0x0d010101, 0x0201, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};
    const OMClassId propertyDefId =
        {0x0d010101, 0x0202, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06, 0x01, 0x01}};

    OMProperty* idProp = storable->findProperty(0x0005);
    ASSERT("MetaDef has Identification property", idProp != 0);
    OMClassId id = *(OMClassId*)(dynamic_cast<OMSimpleProperty*>(idProp))->bits();

    const wchar_t* symbolspaceURI;
    const wchar_t* symbol;
    if (!_store->getSymbol(id, &symbolspaceURI, &symbol))
    {
        exts.append(storable);

        OMProperty* nameProp = storable->findProperty(0x0006); // Name
        ASSERT("MetaDef has Name property", nameProp != 0);
        const wchar_t* name = (wchar_t*)(dynamic_cast<OMSimpleProperty*>(nameProp))->bits();

        if (storable->classId() == classDefId)
        {
            extSymbolspace->createSymbolForClass(id, name);
        }
        else if (storable->classId() == propertyDefId)
        {
            OMProperty* localIdProp = storable->findProperty(0x000D); // LocalIdentification
            ASSERT("PropertyDef has LocalIdentification property", localIdProp != 0);
            OMPropertyId* localId = (OMPropertyId*)(dynamic_cast<OMSimpleProperty*>(localIdProp))->bits();
            extSymbolspace->createSymbolForProperty(id, *localId, name);
        }
        else
        {
            extSymbolspace->createSymbolForType(id, name);
        }
    }

    if (storable->classId() == classDefId)
    {
        OMProperty* propDefsProp = storable->findProperty(0x0009); // Properties
        if (propDefsProp != 0)
        {
            OMStrongReferenceSet* propDefsSet =
                dynamic_cast<OMStrongReferenceSet*>(propDefsProp);
            OMContainerIterator<OMStrongReferenceSetElement>& iterator =
                *propDefsSet->iterator();
            while (++iterator)
            {
                OMStorable* propDef = iterator.value().getValue();
                getExtensions(propDef, extSymbolspace, exts);
            }
            delete &iterator;
        }
    }
}



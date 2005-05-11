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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "OMXMLStorage.h"
#include "OMSetIterator.h"
#include "OMListIterator.h"
#include "OMDiskRawStorage.h"
#include "OMUtilities.h"
#include "OMXMLUtilities.h"
#include "OMXMLException.h"
#include "OMAssertions.h"


OMXMLStorage::OMXMLStorage(OMRawStorage* storage, bool isRead)
: _storage(storage), _objectSetId(0), _baselineSymbolspace(0), _defaultExtSymbolspace(0),
    _dataStreamNotationNameIndex(0), _dataStreamEntityNameIndex(0),
    _dataStreamEntityValueIndex(0)
{
    TRACE("OMXMLStorage::OMXMLStorage");
    
    _isRead = isRead;
    
    if (isRead)
    {
        _xmlWriter = 0;
        _xmlReader = new OMXMLReader(storage);
    }
    else
    {
        _xmlWriter = new OMXMLWriter(storage);
        _xmlReader = 0;
    }
    
    _baselineSymbolspace = OMSymbolspace::createV11Symbolspace(this);
    _symbolspaces.insert(_baselineSymbolspace->getURI(), _baselineSymbolspace);
}

OMXMLStorage::~OMXMLStorage()
{
    TRACE("OMXMLStorage::~OMXMLStorage");

    if (_xmlReader != 0)
    {
        delete _xmlReader;
    }
    if (_xmlWriter != 0)
    {
        delete _xmlWriter;
    }

    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (++iter) 
    {
        delete iter.value();
    }
}

bool 
OMXMLStorage::isRead()
{
    return _isRead;
}

OMXMLWriter* 
OMXMLStorage::getWriter()
{
    TRACE("OMXMLStorage::getWriter");
    PRECONDITION("Is writer", !_isRead);

    return _xmlWriter;
}

OMXMLReader* 
OMXMLStorage::getReader()
{
    TRACE("OMXMLStorage::getReader");
    PRECONDITION("Is reader", _isRead);

    return _xmlReader;
}
    
OMSymbolspace* 
OMXMLStorage::getBaselineSymbolspace() const
{
    TRACE("OMXMLStorage::getBaselineSymbolspace");
    
    return _baselineSymbolspace;
}

OMSymbolspace* 
OMXMLStorage::getDefaultExtSymbolspace() const
{
    TRACE("OMXMLStorage::getBaselineSymbolspace");
    
    return _defaultExtSymbolspace;
}

OMSymbolspace*
OMXMLStorage::createDefaultExtSymbolspace(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::createDefaultExtSymbolspace");
    PRECONDITION("Default symbolspace does not already exist", _defaultExtSymbolspace == 0);
    
    _defaultExtSymbolspace = OMSymbolspace::createDefaultExtSymbolspace(this, id);
    _symbolspaces.insert(_defaultExtSymbolspace->getURI(), _defaultExtSymbolspace);
    
    return _defaultExtSymbolspace;
}

OMSymbolspace* 
OMXMLStorage::createSymbolspace()
{
    TRACE("OMXMLStorage::createSymbolspace");
    
    return new OMSymbolspace(this);
}

void 
OMXMLStorage::addSymbolspace(OMSymbolspace* symbolspace)
{
    TRACE("OMXMLStorage::addSymbolspace");

    if (_symbolspaces.contains(symbolspace->getURI()))
    {
        throw OMXMLException(L"Could not add symbolspace - symbolspace with URI already exists");
    }
    
    _symbolspaces.insert(symbolspace->getURI(), symbolspace);
}

bool 
OMXMLStorage::getMetaDefSymbol(OMUniqueObjectIdentification id, const wchar_t** symbolspaceURI, const wchar_t** symbol) const
{
    TRACE("OMXMLStorage::getMetaDefSymbol");
    
    const wchar_t* sym = 0;
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (sym == 0 && ++iter) 
    {
        if ((sym = iter.value()->getMetaDefSymbol(id)) != 0)
        {
            *symbol = sym;
            *symbolspaceURI = iter.value()->getURI();
        }
    }
    
    return sym != 0;
}

OMUniqueObjectIdentification 
OMXMLStorage::getMetaDefId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getMetaDefId");
    
    OMUniqueObjectIdentification result = nullOMUniqueObjectIdentification;
    OMSymbolspace* symbolspace;
    if (_symbolspaces.find(symbolspaceURI, symbolspace))
    {
        result = symbolspace->getMetaDefId(symbol); 
    }
    
    return result;
}

OMPropertyId 
OMXMLStorage::getPropertyDefId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getPropertyDefId");
    
    OMPropertyId result = 0x0000;
    OMSymbolspace* symbolspace;
    if (_symbolspaces.find(symbolspaceURI, symbolspace))
    {
        result = symbolspace->getPropertyDefId(symbol); 
    }
    
    return result;
}

const wchar_t* 
OMXMLStorage::getDefSymbol(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::getDefSymbol");
    
    return _baselineSymbolspace->getDefSymbol(id);
}

OMUniqueObjectIdentification 
OMXMLStorage::getDefId(const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getDefId");
    
    return _baselineSymbolspace->getDefId(symbol);
}

OMUniqueObjectIdentification 
OMXMLStorage::getBaselineDefId(const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getBaselineDefId");
    
    if (_baselineSymbolspace == 0)
    {
        return nullOMUniqueObjectIdentification;
    }
    
    return _baselineSymbolspace->getDefId(symbol);
}

OMUniqueObjectIdentification 
OMXMLStorage::getBaselineMetaDefId(const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getBaselineMetaDefId");
    
    if (_baselineSymbolspace == 0)
    {
        return nullOMUniqueObjectIdentification;
    }
    
    return _baselineSymbolspace->getMetaDefId(symbol);
}

const wchar_t* 
OMXMLStorage::getBaselineDefSymbol(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::getBaselineDefSymbol");
    
    if (_baselineSymbolspace == 0)
    {
        return 0;
    }
    
    return _baselineSymbolspace->getDefSymbol(id);
}

const wchar_t* 
OMXMLStorage::getBaselineMetaDefSymbol(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::getBaselineMetaDefSymbol");
    
    if (_baselineSymbolspace == 0)
    {
        return 0;
    }
    
    return _baselineSymbolspace->getMetaDefSymbol(id);
}

const wchar_t* 
OMXMLStorage::getDataStreamNotationName(OMUniqueObjectIdentification typeId)
{
    TRACE("OMXMLStorage::getDataStreamNotationName");
    
    OMWString* ret;
    if (_dataStreamNotationNames.find(typeId, &ret))
    {
        return (*ret).c_str();
    }

    char buffer[19];
    sprintf(buffer, "DataStream%x", _dataStreamNotationNameIndex);
    wchar_t* name = convertToWideString(buffer);
    _dataStreamNotationNames.insert(typeId, name); 
    delete [] name;
    _dataStreamNotationNameIndex++;

    if (!_dataStreamNotationNames.find(typeId, &ret))
    {
        ASSERT("Name correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

const wchar_t* 
OMXMLStorage::getDataStreamEntityName(void* ref)
{
    TRACE("OMXMLStorage::getDataStreamEntityName");
    
    OMWString* ret;
    if (_dataStreamEntityNames.find(ref, &ret))
    {
        return (*ret).c_str();
    }

    char buffer[15];
    sprintf(buffer, "stream%x", _dataStreamEntityNameIndex);
    wchar_t* name = convertToWideString(buffer);
    _dataStreamEntityNames.insert(ref, name); 
    delete [] name;
    _dataStreamEntityNameIndex++;
    
    if (!_dataStreamEntityNames.find(ref, &ret))
    {
        ASSERT("Name correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

const wchar_t* 
OMXMLStorage::getDataStreamEntityValue(void* ref, const wchar_t* prefix)
{
    TRACE("OMXMLStorage::getDataStreamEntityValue");
    
    OMWString* ret;
    if (_dataStreamEntityValues.find(ref, &ret))
    {
        return (*ret).c_str();
    }

    char buffer[20];
    sprintf(buffer, "_stream%x", _dataStreamEntityValueIndex);
    wchar_t* value = new wchar_t[20 + lengthOfWideString(prefix)];
    convertToWideString(value, buffer, 20);
    concatenateWideString(value, prefix);
    _dataStreamEntityValues.insert(ref, value); 
    delete [] value;
    _dataStreamEntityValueIndex++;
    
    if (!_dataStreamEntityValues.find(ref, &ret))
    {
        ASSERT("Value correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

const wchar_t* 
OMXMLStorage::registerDataStreamEntityValue(void* ref, const wchar_t* value)
{
    TRACE("OMXMLStorage::registerDataStreamEntityValue");
    
    OMWString* ret;
    if (_dataStreamEntityValues.find(ref, &ret))
    {
        return 0;
    }

    _dataStreamEntityValues.insert(ref, value); 

    if (!_dataStreamEntityValues.find(ref, &ret))
    {
        ASSERT("Value correctly inserted", false);
        return 0;
    }
    return (*ret).c_str();
}

bool 
OMXMLStorage::registerDataStreamEntity(const wchar_t* name, const wchar_t* value)
{
    TRACE("OMXMLStorage::registerDataStreamEntity");
    
    OMWString* ret;
    if (_inputDataStreamEntities.find(name, &ret))
    {
        return false;
    }

    _inputDataStreamEntities.insert(name, value);
    
    return true;
}

const wchar_t* 
OMXMLStorage::getDataStreamEntityValue(const wchar_t* name)
{
    TRACE("OMXMLStorage::getDataStreamEntityValue");
    
    OMWString* ret;
    if (_inputDataStreamEntities.find(name, &ret))
    {
        return (*ret).c_str();
    }
    
    return 0;
}

wchar_t* 
OMXMLStorage::getFilenamePrefixForStream() const
{
    TRACE("OMXMLStorage::getFilenamePrefixForStream");

    wchar_t* result = new wchar_t[1];
    result[0] = L'\0';
    return result;
    
#if 0 // we don't have access to a fileName from the raw storage    
    OMDiskRawStorage* diskStorage = dynamic_cast<OMDiskRawStorage*>(_storage);
    ASSERT("RawStorage is DiskRawStorage", diskStorage != 0);

    const wchar_t* fileName = diskStorage->fileName();
    const wchar_t* startPtr = fileName;    
    const wchar_t* endPtr = fileName;
    const wchar_t* ptr = fileName;
    bool inSuffix = false;
    while (*ptr != L'\0')
    {
#ifdef _WIN32
        if (*ptr == L'\\' || *ptr == L':')
#else
        if (*ptr == L'/')
#endif
        {
            startPtr = ptr + 1;
            endPtr = startPtr;
            inSuffix = false;
        }
        else if (*ptr == L'.')
        {
            endPtr++;
            inSuffix = true;
        }
        else if (!inSuffix)
        {
            endPtr++;
        }
        ptr++;
    }
    ASSERT("Valid end pointer", *endPtr == L'\0' || *endPtr == L'.');
    
    size_t size = endPtr - startPtr;
    if (size == 0)
    {
        return 0;
    }
    wchar_t* result = new wchar_t[size + 1];
    copyWideString(result, startPtr, size);
    result[size] = L'\0';
    
    return result;
#endif
}

wchar_t* 
OMXMLStorage::getFilePathForStream() const
{
    TRACE("OMXMLStorage::getFilePathForStream");

    wchar_t* result = new wchar_t[1];
    result[0] = L'\0';
    return result;
    
#if 0 // we don't have access to a fileName from the raw storage
    OMDiskRawStorage* diskStorage = dynamic_cast<OMDiskRawStorage*>(_storage);
    ASSERT("RawStorage is DiskRawStorage", diskStorage != 0);

    const wchar_t* fileName = diskStorage->fileName();
    const wchar_t* startPtr = fileName;    
    const wchar_t* endPtr = fileName;
    const wchar_t* ptr = fileName;
    while (*ptr != L'\0')
    {
#ifdef _WIN32
        if (*ptr == L'\\' || *ptr == L':')
#else
        if (*ptr == L'/')
#endif
        {
            endPtr = ptr + 1;
        }
        ptr++;
    }
#ifdef _WIN32
    ASSERT("Valid end pointer", *endPtr == *startPtr || *(endPtr - 1) == L'\\' 
        || *(endPtr - 1) == L':'));
#else
    ASSERT("Valid end pointer", *endPtr == *startPtr || *(endPtr - 1) == L'/');
#endif
    
    size_t size = endPtr - startPtr;
    wchar_t* result = new wchar_t[size + 1];
    copyWideString(result, startPtr, size);
    result[size] = L'\0';
    
    return result;
#endif
}

void 
OMXMLStorage::forwardObjectSetId(const wchar_t* id)
{
    TRACE("OMXMLStorage::forwardObjectSetId");
    PRECONDITION("Valid object set id", id != 0);
    
    if (_objectSetId != 0)
    {
        delete [] _objectSetId;
    }
    
    _objectSetId = new wchar_t[lengthOfWideString(id) + 1];
    copyWideString(_objectSetId, id);
}

bool 
OMXMLStorage::haveForwardedObjectSetId()
{
    TRACE("OMXMLStorage::haveForwardedObjectSetId");
    
    return _objectSetId != 0;
}

wchar_t* 
OMXMLStorage::getForwardedObjectSetId()
{
    TRACE("OMXMLStorage::getForwardedObjectSetId");
    PRECONDITION("Have forwarded object set id", _objectSetId != 0);
    
    wchar_t* copy = new wchar_t[lengthOfWideString(_objectSetId) + 1];
    copyWideString(copy, _objectSetId);
    
    delete [] _objectSetId;
    _objectSetId = 0;
    
    return copy;
}



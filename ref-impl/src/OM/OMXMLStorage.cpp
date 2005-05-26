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
#include "OMXMLUtilities.h"
#include "OMXMLException.h"
#include "OMAssertions.h"


OMXMLStorage::OMXMLStorage(OMRawStorage* storage, Mode mode)
: _mode(mode), _storage(storage), _objectSetId(0), _baselineSymbolspace(0), 
    _defaultExtSymbolspace(0), _symbolspacePrefixIndex(0),
    _dataStreamNotationNameIndex(0), _dataStreamEntityNameIndex(0), 
    _dataStreamEntityValueIndex(0)
{
    TRACE("OMXMLStorage::OMXMLStorage");
    
    if (mode == READ_MODE || mode == EXISTING_MODIFY_MODE)
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
    addSymbolspace(_baselineSymbolspace);
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

OMXMLStorage::Mode 
OMXMLStorage::mode()
{
    TRACE("OMXMLStorage::mode");
    
    return _mode;
}

bool 
OMXMLStorage::haveReader()
{
    TRACE("OMXMLStorage::haveReader");
    
    return _xmlReader != 0;
}

bool 
OMXMLStorage::haveWriter()
{
    TRACE("OMXMLStorage::haveWriter");
    
    return _xmlWriter != 0;
}

OMXMLWriter* 
OMXMLStorage::getWriter()
{
    TRACE("OMXMLStorage::getWriter");
    PRECONDITION("Is writer", _xmlWriter != 0);

    return _xmlWriter;
}

OMXMLReader* 
OMXMLStorage::getReader()
{
    TRACE("OMXMLStorage::getReader");
    PRECONDITION("Is reader", _xmlReader != 0);

    return _xmlReader;
}

void 
OMXMLStorage::resetForWriting()
{
    TRACE("OMXMLStorage::resetForWriting");
    PRECONDITION("Is modifiable", _mode == EXISTING_MODIFY_MODE || _mode == NEW_MODIFY_MODE);
    
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (++iter) 
    {
        iter.value()->resetForWriting();
    }
    
    if (_xmlReader != 0)
    {
        delete _xmlReader;
        _xmlReader = 0;
        
        _xmlWriter = new OMXMLWriter(_storage);
    }
    else
    {
        // fill remaining space in file with spaces
        if (_storage->position() < _storage->size())
        {
            wchar_t spaces[1024];
            wmemset(spaces, L' ', 1023);
            spaces[1023] = L'\0';
            
            OMUInt64 remainingSpace = _storage->size() - _storage->position();
            OMUInt64 count = 0;
            while (count < remainingSpace)
            {
                OMUInt32 len = ((remainingSpace - count) > 1023) ? 1023 : 
                    (OMUInt32)(remainingSpace - count);
                if (len < 1023)
                {
                    spaces[len] = L'\0';
                }
                
                _xmlWriter->writeText(spaces);
                
                count += len;
            }
        }
    }
    _xmlWriter->reset();
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
    addSymbolspace(_defaultExtSymbolspace);
    
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
    setUniquePrefix(symbolspace);

    _symbolspaces.insert(symbolspace->getURI(), symbolspace);
}

OMSet<OMWString, OMSymbolspace*>& 
OMXMLStorage::getSymbolspaces()
{
    TRACE("OMXMLStorage::getSymbolspaces");
    
    return _symbolspaces;
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

bool 
OMXMLStorage::knownBaselineExtEnum(OMUniqueObjectIdentification id, 
    OMUniqueObjectIdentification value) const
{
    TRACE("OMXMLStorage::knownBaselineExtEnum");
    
    if (_baselineSymbolspace == 0)
    {
        return false;
    }
    
    return _baselineSymbolspace->knownExtEnum(id, value);
}

OMSymbolspace* 
OMXMLStorage::getSymbolspaceForDef(OMUniqueObjectIdentification id) const
{
    TRACE("OMXMLStorage::getSymbolspaceForDef");

    OMSymbolspace* symbolspace = 0;    
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (symbolspace == 0 && ++iter) 
    {
        if (iter.value()->getDefSymbol(id) != 0)
        {
            symbolspace = iter.value();
        }
    }
    
    return symbolspace;
}

OMSymbolspace* 
OMXMLStorage::getSymbolspaceForMetaDef(OMUniqueObjectIdentification id) const
{
    TRACE("OMXMLStorage::getSymbolspaceForMetaDef");
    
    OMSymbolspace* symbolspace = 0;    
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (symbolspace == 0 && ++iter) 
    {
        if (iter.value()->getMetaDefSymbol(id) != 0)
        {
            symbolspace = iter.value();
        }
    }
    
    return symbolspace;
}

OMSymbolspace* 
OMXMLStorage::getSymbolspaceForExtEnum(OMUniqueObjectIdentification id,
    OMUniqueObjectIdentification value) const
{
    TRACE("OMXMLStorage::getSymbolspaceForExtEnum");
    
    OMSymbolspace* symbolspace = 0;    
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (symbolspace == 0 && ++iter) 
    {
        if (iter.value()->knownExtEnum(id, value))
        {
            symbolspace = iter.value();
        }
    }
    
    return symbolspace;
}

bool 
OMXMLStorage::isBaselineSymbolspace(OMSymbolspace* symbolspace) const
{
    TRACE("OMXMLStorage::isBaselineSymbolspace");
    
    if (_baselineSymbolspace != 0 && symbolspace == _baselineSymbolspace)
    {
        return true;
    }
    
    return false;    
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

    wchar_t name[19];
    std_swprintf(name, 19, L"DataStream%x", _dataStreamNotationNameIndex);
    _dataStreamNotationNames.insert(typeId, name); 
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

    wchar_t name[15];
    std_swprintf(name, 15, L"stream%x", _dataStreamEntityNameIndex);
    _dataStreamEntityNames.insert(ref, name); 
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

    size_t len = 19 + wcslen(prefix);
    wchar_t* value = new wchar_t[len + 1];
    std_swprintf(value, 20, L"_stream%x", _dataStreamEntityValueIndex);
    wcscat(value, prefix);
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

bool 
OMXMLStorage::registerDataStreamNotation(const wchar_t* notationName, 
    const wchar_t* systemId)
{
    TRACE("OMXMLStorage::registerDataStreamNotation");

    if (notationName == 0 || systemId == 0 || !isURI(systemId))
    {
        return false;
    }

    OMUniqueObjectIdentification id;
    uriToAUID(systemId, &id);
    
    OMWString* ret;
    if (_dataStreamNotationNames.find(id, &ret))
    {
        return false;
    }

    _dataStreamNotationNames.insert(id, notationName);
    
    return true;
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
    wcsncpy(result, startPtr, size);
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
    wcsncpy(result, startPtr, size);
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
    
    _objectSetId = new wchar_t[wcslen(id) + 1];
    wcscpy(_objectSetId, id);
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
    
    wchar_t* copy = new wchar_t[wcslen(_objectSetId) + 1];
    wcscpy(copy, _objectSetId);
    
    delete [] _objectSetId;
    _objectSetId = 0;
    
    return copy;
}

void 
OMXMLStorage::registerNamespacePrefix(const wchar_t* prefix, const wchar_t* uri)
{
    TRACE("OMXMLStorage::registerNamespacePrefix");

    if (_namespacesPrefixMap.contains(uri))
    {
        _namespacesPrefixMap.remove(uri);
    }
    _namespacesPrefixMap.insert(uri, prefix);
}


void
OMXMLStorage::setUniquePrefix(OMSymbolspace* symbolspace)
{
    TRACE("OMXMLStorage::setUniquePrefix");

    // use the prefix used in the existing document if it is unique
    OMWString* prefix = 0;
    _namespacesPrefixMap.find(symbolspace->getURI(), &prefix);
    if (prefix != 0 &&
        !_symbolspacesPrefixMap.contains(prefix->c_str()))
    {
        symbolspace->setPrefix(prefix->c_str());
        _symbolspacesPrefixMap.insert(symbolspace->getPrefix(), symbolspace);
    }
    
    // else use the preferred prefix if it is unique
    else if (symbolspace->getPreferredPrefix() != 0 &&
        wcslen(symbolspace->getPreferredPrefix()) > 0 && 
        !_symbolspacesPrefixMap.contains(symbolspace->getPreferredPrefix()))
    {
        symbolspace->setPrefix(symbolspace->getPreferredPrefix());
        _symbolspacesPrefixMap.insert(symbolspace->getPrefix(), symbolspace);
    }

    // else create a unique prefix from the preferred prefix or from 'ns'
    else
    {
        wchar_t* prefix = 0;
        if (symbolspace->getPreferredPrefix() == 0 ||
            wcslen(symbolspace->getPreferredPrefix()) == 0)
        {
            const wchar_t* basePrefix = L"ns";
            prefix = new wchar_t[wcslen(basePrefix) + 1];
            wcscpy(prefix, basePrefix);
        }
        else
        {
            prefix = wideCharacterStringDup(symbolspace->getPreferredPrefix());
        }
        
        while (_symbolspacesPrefixMap.contains(prefix))
        {
            wchar_t suffix[9];
            std_swprintf(suffix, 9, L"%u", _symbolspacePrefixIndex);
            wchar_t* newPrefix = new wchar_t[wcslen(prefix) + 8 + 1];
            wcscpy(newPrefix, prefix);
            delete [] prefix;
            prefix = wcscat(newPrefix, suffix);
            _symbolspacePrefixIndex++;
        }
        
        symbolspace->setPrefix(prefix);
        _symbolspacesPrefixMap.insert(symbolspace->getPrefix(), symbolspace);

        delete [] prefix;
    }
}




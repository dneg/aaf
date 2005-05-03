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
#include "OMUtilities.h"
#include "OMXMLUtilities.h"
#include "OMXMLException.h"
#include "OMAssertions.h"


OMXMLStorage::OMXMLStorage(OMRawStorage* storage, bool isRead)
: _objectSetId(0), _baselineSymbolspace(0), _defaultExtSymbolspace(0),
    _dataStreamNotationNameIndex(0), _dataStreamEntityNameIndex(0),
    _dataStreamEntityValueIndex(0)
{
    TRACE("OMXMLStorage::OMXMLStorage");
    
    _isRead = isRead;
    
    if (isRead)
    {
        _xmlWriter = 0;
        _xmlReader = new OMXMLReader(storage);;
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
OMXMLStorage::getSymbol(OMUniqueObjectIdentification id, const wchar_t** symbolspaceURI, const wchar_t** symbol) const
{
    TRACE("OMXMLStorage::getSymbol");
    
    const wchar_t* sym = 0;
    OMSetIterator<OMWString, OMSymbolspace*> iter(_symbolspaces, OMBefore);
    while (sym == 0 && ++iter) 
    {
        if ((sym = iter.value()->getSymbol(id)) != 0)
        {
            *symbol = sym;
            *symbolspaceURI = iter.value()->getURI();
        }
    }
    
    return sym != 0;
}

OMUniqueObjectIdentification 
OMXMLStorage::getId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getId");
    
    OMUniqueObjectIdentification result = nullOMUniqueObjectIdentification;
    OMSymbolspace* symbolspace;
    if (_symbolspaces.find(symbolspaceURI, symbolspace))
    {
        result = symbolspace->getId(symbol); 
    }
    
    return result;
}

OMPropertyId 
OMXMLStorage::getPropertyId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getPropertyId");
    
    OMPropertyId result = 0x0000;
    OMSymbolspace* symbolspace;
    if (_symbolspaces.find(symbolspaceURI, symbolspace))
    {
        result = symbolspace->getPropertyId(symbol); 
    }
    
    return result;
}

const wchar_t* 
OMXMLStorage::getDefinitionSymbol(OMUniqueObjectIdentification id)
{
    TRACE("OMXMLStorage::getDefinitionSymbol");
    
    return _baselineSymbolspace->getDefinitionSymbol(id);
}

OMUniqueObjectIdentification 
OMXMLStorage::getDefinitionId(const wchar_t* symbol) const
{
    TRACE("OMXMLStorage::getDefinitionId");
    
    return _baselineSymbolspace->getDefinitionId(symbol);
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
OMXMLStorage::getDataStreamEntityValue(void* ref)
{
    TRACE("OMXMLStorage::getDataStreamEntityValue");
    
    OMWString* ret;
    if (_dataStreamEntityValues.find(ref, &ret))
    {
        return (*ret).c_str();
    }

    char buffer[19];
    sprintf(buffer, "stream%x.raw", _dataStreamEntityValueIndex);
    wchar_t* value = convertToWideString(buffer);
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



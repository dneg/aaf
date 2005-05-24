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

#include "OMXMLReader.h"
#include "OMListIterator.h"
#include "OMXMLUtilities.h"
#include "OMAssertions.h"
#include "OMXMLException.h"


#define COPY_STRING(DEST, SRC) \
{ \
    wchar_t* tmp = utf8ToUTF16(SRC); \
    DEST = tmp; \
    delete [] tmp; \
}



OMXMLReader::OMXMLReader(OMRawStorage* storage)
: _eventType(NONE)
{
    TRACE("OMXMLReader::OMXMLReader");
    
#if defined(HAVE_EXPAT)
    _xmlReader = new XMLReaderExpat(new OMXMLIStream(storage));
    _xmlReader->RegisterListener(this);
#else
    ASSERT("Expat library available", false);
#endif
}

OMXMLReader::~OMXMLReader()
{
    TRACE("OMXMLReader::~OMXMLReader");

    cleanUp();    

    if (_xmlReader != 0)
    {
        delete _xmlReader;
    }
}

bool 
OMXMLReader::next()
{
    TRACE("OMXMLReader::next");

    cleanUp();
    return _xmlReader->Next(false);
}

OMXMLReader::EventType 
OMXMLReader::getEventType()
{
    TRACE("OMXMLReader::getEventType");

    return _eventType;
}

bool 
OMXMLReader::nextElement()
{
    TRACE("OMXMLReader::nextElement");

    bool haveNext = true;
    while ((haveNext = next()) && 
        getEventType() != START_ELEMENT && getEventType() != END_ELEMENT)
    {}
    
    if (haveNext && getEventType() == END_ELEMENT)
    {
        return false;
    }
    return haveNext;
}

bool 
OMXMLReader::nextEndElement()
{
    TRACE("OMXMLReader::nextEndElement");

    bool haveNext = true;
    while ((haveNext = next()) && 
        getEventType() != START_ELEMENT && getEventType() != END_ELEMENT)
    {}
    
    if (!haveNext)
    {
        throw OMXMLException(L"Failed to read next end element - reached end of document");
    }
    else if (getEventType() == START_ELEMENT)
    {
        throw OMXMLException(L"Expecting an end element, but found a start element");
    }
    return true;
}

bool 
OMXMLReader::moveToEndElement()
{
    TRACE("OMXMLReader::moveToEndElement");

    if (getEventType() == END_ELEMENT)
    {
        return true;
    }
    
    return nextEndElement();
}

void 
OMXMLReader::skipContent()
{
    TRACE("OMXMLReader::skipContent");
    PRECONDITION("Skip content starting from start element", _eventType == START_ELEMENT);

    bool done = false;
    int count = 1;
    while (!done && count > 0)
    {
        done = !next();
        if (getEventType() == END_ELEMENT)
        {
            count--;
        }
        else if (getEventType() == START_ELEMENT)
        {
            count++;
        }
    }
}

bool 
OMXMLReader::elementEquals(const wchar_t* uri, const wchar_t* localName)
{
    TRACE("OMXMLReader::elementEquals");
    PRECONDITION("Event is start or end element", 
        _eventType == START_ELEMENT || _eventType == END_ELEMENT);
    
    if (wcscmp(_uri.c_str(), uri) == 0 &&
        wcscmp(_localName.c_str(), localName) == 0)
    {
        return true;
    }
    return false;
}

void 
OMXMLReader::getNotationDecl(const wchar_t*& name, const wchar_t*& publicID, 
    const wchar_t*& systemID)
{
    TRACE("OMXMLReader::getNotationDecl");
    PRECONDITION("Valid event", _eventType == NOTATION_DECL);

    name = _name.c_str();
    publicID = _publicID.c_str();
    systemID = _systemID.c_str();
}

void 
OMXMLReader::getUnparsedEntityDecl(const wchar_t*& name, const wchar_t*& publicID, 
    const wchar_t*& systemID, const wchar_t*& notationName)
{
    TRACE("OMXMLReader::getUnparsedEntityDecl");
    PRECONDITION("Valid event", _eventType == UNPARSED_ENTITY_DECL);

    name = _name.c_str();
    publicID = _publicID.c_str();
    systemID = _systemID.c_str();
    notationName = _notationName.c_str();
}

void 
OMXMLReader::getStartElement(const wchar_t*& uri, const wchar_t*& localName, 
    const OMList<OMXMLAttribute*>*& attributes)
{
    TRACE("OMXMLReader::getStartElement");
    PRECONDITION("Valid event", _eventType == START_ELEMENT);

    uri = _uri.c_str();
    localName = _localName.c_str();
    attributes = &_attributes;
}

void 
OMXMLReader::getEndElement(const wchar_t*& uri, const wchar_t*& localName)
{
    TRACE("OMXMLReader::getEndElement");
    PRECONDITION("Valid event", _eventType == END_ELEMENT);

    uri = _uri.c_str();
    localName = _localName.c_str();
}

void 
OMXMLReader::getCharacters(const wchar_t*& data, size_t& length)
{
    TRACE("OMXMLReader::getCharacters");
    PRECONDITION("Valid event", _eventType == CHARACTERS);

    data = _data.c_str();
    length = _data.length();
}

const wchar_t* 
OMXMLReader::getPositionString()
{
    COPY_STRING(_positionString, _xmlReader->GetPositionString().c_str());
    
    return _positionString.c_str();
}

OMXMLAttribute* 
OMXMLReader::getAttribute(const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* nmspace, const wchar_t* localName)
{
    OMXMLAttribute* result = 0;
    
    OMListIterator<OMXMLAttribute*> iter(*attributes, OMBefore);
    while (result == 0 && ++iter)
    {
        OMXMLAttribute* attr = iter.value();
        if (wcscmp(attr->getNamespace(), nmspace) == 0 &&
            wcscmp(attr->getLocalName(), localName) == 0)
        {
            result = attr;
        }
    }
    
    return result;
}

void 
OMXMLReader::NotationDecl(const char* name, const char* publicID, const char* systemID)
{
    TRACE("OMXMLReader::NotationDecl");

    COPY_STRING(_name, name);
    COPY_STRING(_publicID, publicID);
    COPY_STRING(_systemID, systemID);
    
    _eventType = NOTATION_DECL;
}

void 
OMXMLReader::UnparsedEntityDecl(const char* name, const char* publicID, const char* systemID, 
    const char* notationName)
{
    TRACE("OMXMLReader::UnparsedEntityDecl");

    COPY_STRING(_name, name);
    COPY_STRING(_publicID, publicID);
    COPY_STRING(_systemID, systemID);
    COPY_STRING(_notationName, notationName);
    
    _eventType = UNPARSED_ENTITY_DECL;
}

void 
OMXMLReader::StartElement(const char* uri, const char* localName, 
    const XMLAttribute* attributes)
{
    TRACE("OMXMLReader::StartElement");

    COPY_STRING(_uri, uri);
    COPY_STRING(_localName, localName);
    const XMLAttribute* attrPtr = attributes;
    if (attrPtr != 0)
    {
        do
        {
            OMXMLAttribute* attr = new OMXMLAttribute(attrPtr->GetNamespace().c_str(),
                attrPtr->GetLocalName().c_str(), attrPtr->GetValue().c_str());
            _attributes.append(attr);
        }
        while ((attrPtr = attrPtr->Next()) != 0);
    }
    
    _eventType = START_ELEMENT;
}

void 
OMXMLReader::EndElement(const char* uri, const char* localName)
{
    TRACE("OMXMLReader::EndElement");

    COPY_STRING(_uri, uri);
    COPY_STRING(_localName, localName);
    
    _eventType = END_ELEMENT;
}

void 
OMXMLReader::Characters(const char* data, size_t /*length*/)
{
    TRACE("OMXMLReader::Characters");

    COPY_STRING(_data, data);
    
    _eventType = CHARACTERS;
}

void 
OMXMLReader::cleanUp()
{
    TRACE("OMXMLReader::cleanUp");

    OMListIterator<OMXMLAttribute*> iter(_attributes, OMBefore);
    while (++iter)
    {
        OMXMLAttribute* nonRefCopy = iter.value();
        delete nonRefCopy;
    }

    _attributes.clear();
}



OMXMLIStream::OMXMLIStream(OMRawStorage* storage)
: _storage(storage)
{
    TRACE("OMXMLIStream::OMXMLIStream");
    
    _position = 0;
    _size = storage->size();
}

OMXMLIStream::~OMXMLIStream()
{
    TRACE("OMXMLIStream::~OMXMLIStream");
}
    
OMUInt32 
OMXMLIStream::Read(OMByte* data, OMUInt32 count)
{
    TRACE("OMXMLIStream::Read");

    // need to remember (or we could get it each time from the raw storage) the 
    // position because OMStream::read() has a post condition
    // POSTCONDITION("All bytes read", actualByteCount == byteCount);
    OMUInt32 realCount = count;
    if (realCount > (_size - _position))
    {
        realCount = (OMUInt32)(_size - _position);
    }
    if (realCount == 0)
    {
        return 0;
    }
    
    OMUInt32 bytesRead;
    _storage->read(data, realCount, bytesRead);
    _position += bytesRead;
    
    return bytesRead;
}


OMXMLAttribute::OMXMLAttribute(const char* nmspace, const char* localName, const char* value)
{
    TRACE("OMXMLAttribute::OMXMLAttribute");

    if (nmspace != 0)
    {
        COPY_STRING(_nmspace, nmspace);
    }
    if (localName != 0)
    {
        COPY_STRING(_localName, localName);
    }
    if (value != 0)
    {
        COPY_STRING(_value, value);
    }
}

OMXMLAttribute::~OMXMLAttribute()
{
    TRACE("OMXMLAttribute::~OMXMLAttribute");
}

const wchar_t* 
OMXMLAttribute::getNamespace() const
{
    TRACE("OMXMLAttribute::OMXMLAttribute");

    return _nmspace.c_str();
}

const wchar_t* 
OMXMLAttribute::getLocalName() const
{
    TRACE("OMXMLAttribute::OMXMLAttribute");

    return _localName.c_str();
}

const wchar_t* 
OMXMLAttribute::getValue() const
{
    TRACE("OMXMLAttribute::OMXMLAttribute");

    return _value.c_str();
}




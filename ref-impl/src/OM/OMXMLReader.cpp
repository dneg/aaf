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

#include "OMXMLReader.h"
#include "OMListIterator.h"
#include "OMXMLUtilities.h"
#include "OMAssertions.h"
#include "OMExceptions.h"
#include <stdio.h>


OMXMLReader*
OMXMLReader::create(OMRawStorage* xmlStream)
{
    TRACE("OMXMLReader::create");
    
    XML_Expat_Version version = XML_ExpatVersionInfo();
    if (version.major > 1 || 
        (version.major == 1 && version.minor > 95) ||
        (version.major == 1 && version.minor == 95 && version.micro >= 8))
    {
        return new OMXMLReaderExpat(xmlStream);
    }
    else
    {
        fprintf(stderr, "Error: Require Expat version >= 1.95.8\n");
        throw OMException("Require Expat version >= 1.95.8");
    }
}

OMXMLReader::~OMXMLReader()
{
    TRACE("OMXMLReader::~OMXMLReader");
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
        throw OMException("Failed to read next end element - reached end of document");
    }
    else if (getEventType() == START_ELEMENT)
    {
        throw OMException("Expecting an end element, but found a start element");
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
    PRECONDITION("Skip content starting from start element", getEventType() == START_ELEMENT);

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
    PRECONDITION("Event is start or end element", getEventType() == START_ELEMENT || 
        getEventType() == END_ELEMENT);
    
    const wchar_t* eUri;
    const wchar_t* eLocalName;
    if (getEventType() == START_ELEMENT)
    {
        const OMList<OMXMLAttribute*>* attributes;
        getStartElement(eUri, eLocalName, attributes);
    }
    else
    {
        getEndElement(eUri, eLocalName);
    }
    
    return (wcscmp(eUri, uri) == 0 && wcscmp(eLocalName, localName) == 0);
}

OMXMLAttribute* 
OMXMLReader::getAttribute(const OMList<OMXMLAttribute*>* attributes,
    const wchar_t* nmspace, const wchar_t* localName)
{
    TRACE("OMXMLReader::getAttribute");
    
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


OMXMLAttribute::OMXMLAttribute(const wchar_t* nmspace, const wchar_t* localName, 
    const wchar_t* value)
: _nmspace(nmspace), _localName(localName), _value(value)
{
    TRACE("OMXMLAttribute::OMXMLAttribute");
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



#define NAMESPACE_SEPARATOR        0x20
#define READ_CHUNK_SIZE            2048

#define WORK_BUFFER_MIN_SIZE       1024
#define WORK_BUFFER_CLEANUP_SIZE   4096


OMXMLReaderExpat::OMXMLReaderExpat(OMRawStorage* xmlStream)
: _appendData(false), _xmlStream(xmlStream), _parser(0)
{
    TRACE("OMXMLReaderExpat::OMXMLReaderExpat");

    clearEvents();

    _parser = XML_ParserCreateNS(0, NAMESPACE_SEPARATOR);
    XML_SetNotationDeclHandler(_parser, ::expat_NotationDeclHandler);
    XML_SetEntityDeclHandler(_parser, ::expat_EntityDeclHandler);
    XML_SetStartNamespaceDeclHandler(_parser, ::expat_StartNamespaceDeclHandler);
    XML_SetEndNamespaceDeclHandler(_parser, ::expat_EndNamespaceDeclHandler);
    XML_SetStartElementHandler(_parser, ::expat_StartElementHandler);
    XML_SetEndElementHandler(_parser, ::expat_EndElementHandler);
    XML_SetCharacterDataHandler(_parser, ::expat_CharacterDataHandler);
    XML_SetUserData(_parser, this);

    _workBuffer = new wchar_t[WORK_BUFFER_MIN_SIZE];
    _workBufferSize = WORK_BUFFER_MIN_SIZE;

    _readNextChunk = true;
    _status = true;
    _numInBuffer = 0;
}

OMXMLReaderExpat::~OMXMLReaderExpat()
{
    TRACE("OMXMLReaderExpat::~OMXMLReaderExpat");

    XML_ParserFree(_parser);

    if (_workBuffer != 0)
    {
        delete [] _workBuffer;
    }
    
    OMListIterator<OMXMLAttribute*> iter(_attributes, OMBefore);
    while (++iter)
    {
        delete iter.value();
    }
    _attributes.clear();

    OMUInt32 elementCount = _startNmspaceDecls.count();
    for (OMUInt32 i = 0; i < elementCount; i++)
    {
        delete _startNmspaceDecls.getAt(i);
    }
    _startNmspaceDecls.clear();
}

bool 
OMXMLReaderExpat::next()
{
    TRACE("OMXMLReaderExpat::next");

    if (_status == false)
    {
        return _status;
    }

    while (_status && nextEvent() == NONE)
    {
        if (!_readNextChunk)
        {
            int ret = XML_ResumeParser(_parser);

            if (ret == XML_STATUS_ERROR)
            {
                XML_Error errorCode = XML_GetErrorCode(_parser);
                if (errorCode == XML_ERROR_NOT_SUSPENDED)
                {
                    XML_ParsingStatus pStatus;
                    XML_GetParsingStatus(_parser, &pStatus);
                    if (pStatus.parsing == XML_FINISHED)
                    {
                        // finished
                        _status = false;
                    }
                    else
                    {
                        _readNextChunk = true;
                    }
                }
                else
                {
                    throw OMException(getErrorString());
                }
            }
        }

        if (_readNextChunk)
        {
            void* buffer = XML_GetBuffer(_parser, READ_CHUNK_SIZE);
            _numInBuffer = readNextChunk(buffer, READ_CHUNK_SIZE);

            int ret = XML_ParseBuffer(_parser, _numInBuffer, _numInBuffer < READ_CHUNK_SIZE);

            if (ret == XML_STATUS_ERROR)
            {
                throw OMException(getErrorString());
            }

            if (_status)
            {
                XML_ParsingStatus pStatus;
                XML_GetParsingStatus(_parser, &pStatus);
                if (pStatus.parsing == XML_FINISHED)
                {
                    if (_numInBuffer < READ_CHUNK_SIZE)
                    {
                        _status = false;
                    }
                    else
                    {
                        _readNextChunk = true;
                    }
                }
                else
                {
                    _readNextChunk = false;
                }
            }
        }
    }
    
    return _status;
}

void
OMXMLReaderExpat::reset()
{
    TRACE("OMXMLReaderExpat::reset");
    
    XML_ParserFree(_parser);
    
    if (_workBuffer != 0)
    {
        delete [] _workBuffer;
    }
    
    OMListIterator<OMXMLAttribute*> iter(_attributes, OMBefore);
    while (++iter)
    {
        delete iter.value();
    }
    _attributes.clear();
    
    OMUInt32 elementCount = _startNmspaceDecls.count();
    for (OMUInt32 i = 0; i < elementCount; i++)
    {
        delete _startNmspaceDecls.getAt(i);
    }
    _startNmspaceDecls.clear();
    _endNmspaceDecls.clear();
    
    
    clearEvents();

    _parser = XML_ParserCreateNS(0, NAMESPACE_SEPARATOR);
    XML_SetNotationDeclHandler(_parser, ::expat_NotationDeclHandler);
    XML_SetEntityDeclHandler(_parser, ::expat_EntityDeclHandler);
    XML_SetStartNamespaceDeclHandler(_parser, ::expat_StartNamespaceDeclHandler);
    XML_SetEndNamespaceDeclHandler(_parser, ::expat_EndNamespaceDeclHandler);
    XML_SetStartElementHandler(_parser, ::expat_StartElementHandler);
    XML_SetEndElementHandler(_parser, ::expat_EndElementHandler);
    XML_SetCharacterDataHandler(_parser, ::expat_CharacterDataHandler);
    XML_SetUserData(_parser, this);

    _workBuffer = new wchar_t[WORK_BUFFER_MIN_SIZE];
    _workBufferSize = WORK_BUFFER_MIN_SIZE;
    
    _readNextChunk = true;
    _status = true;
    _numInBuffer = 0;
    
    _xmlStream->setPosition(0);
}

OMXMLReaderExpat::EventType 
OMXMLReaderExpat::getEventType()
{
    TRACE("OMXMLReaderExpat::getEventType");

    return _event;
}

void 
OMXMLReaderExpat::getNotationDecl(const wchar_t*& name, const wchar_t*& publicID, 
    const wchar_t*& systemID)
{
    TRACE("OMXMLReaderExpat::getNotationDecl");
    PRECONDITION("Valid event", _event == NOTATION_DECL);

    name = _notationName.c_str();
    publicID = _publicID.c_str();
    systemID = _systemID.c_str();
}

void 
OMXMLReaderExpat::getUnparsedEntityDecl(const wchar_t*& name, const wchar_t*& publicID, 
    const wchar_t*& systemID, const wchar_t*& notationName)
{
    TRACE("OMXMLReaderExpat::getUnparsedEntityDecl");
    PRECONDITION("Valid event", _event == UNPARSED_ENTITY_DECL);

    name = _name.c_str();
    publicID = _publicID.c_str();
    systemID = _systemID.c_str();
    notationName = _notationName.c_str();
}

void 
OMXMLReaderExpat::getStartPrefixMapping(const wchar_t*& prefix, const wchar_t*& uri)
{
    TRACE("OMXMLReaderExpat::getStartPrefixMapping");
    PRECONDITION("Valid event", _event == START_PREFIX_MAPPING);
    PRECONDITION("Prefix mapping data present", _startNmspaceDecls.count() > 0);
    
    QName* qName = _startNmspaceDecls.getAt(0);
    _startNmspaceDecls.removeFirst();
    _prefix = qName->prefix;
    _prefixUri = qName->uri;
    delete qName;
    
    prefix = _prefix.c_str();
    uri = _prefixUri.c_str();
}

void 
OMXMLReaderExpat::getEndPrefixMapping(const wchar_t*& prefix)
{
    TRACE("OMXMLReaderExpat::getEndPrefixMapping");
    PRECONDITION("Valid event", _event == END_PREFIX_MAPPING);
    PRECONDITION("Prefix mapping data present", _endNmspaceDecls.count() > 0);
    
    _prefix = _endNmspaceDecls.getAt(0);
    _endNmspaceDecls.removeFirst();

    prefix = _prefix.c_str();
}

void 
OMXMLReaderExpat::getStartElement(const wchar_t*& uri, const wchar_t*& localName, 
    const OMList<OMXMLAttribute*>*& attributes)
{
    TRACE("OMXMLReaderExpat::getStartElement");
    PRECONDITION("Valid event", _event == START_ELEMENT);

    uri = _uri.c_str();
    localName = _localName.c_str();
    attributes = &_attributes;
}

void 
OMXMLReaderExpat::getEndElement(const wchar_t*& uri, const wchar_t*& localName)
{
    TRACE("OMXMLReaderExpat::getEndElement");
    PRECONDITION("Valid event", _event == END_ELEMENT);

    uri = _uri.c_str();
    localName = _localName.c_str();
}

void 
OMXMLReaderExpat::getCharacters(const wchar_t*& data, OMUInt32& length)
{
    TRACE("OMXMLReaderExpat::getCharacters");
    PRECONDITION("Valid event", _event == CHARACTERS);

    data = _data.c_str();
    length = _data.length();
}

const wchar_t* 
OMXMLReaderExpat::getPositionString()
{
    TRACE("OMXMLReaderExpat::getPositionString");

    int lineNumber = XML_GetCurrentLineNumber(_parser);
    int columnNumber = XML_GetCurrentColumnNumber(_parser);
    
    wchar_t buffer[30];
    std_swprintf(buffer, 30, L"line %d, column %d", lineNumber, columnNumber);
    
    _positionString = buffer;
    
    return _positionString.c_str();
}

void 
OMXMLReaderExpat::registerEvent(EventType event)
{
    TRACE("OMXMLReaderExpat::registerEvent");

    _events.append(event);

    if (event == CHARACTERS)
    {
        _appendData = true;
    }
    else
    {
        _appendData = false;
    }
}


OMXMLReader::EventType 
OMXMLReaderExpat::nextEvent(void)
{
    TRACE("OMXMLReaderExpat::registerEvent");

    _event = NONE;
    if (_events.count() != 0)
    {
        EventType first = _events.getAt(0);
        _events.removeFirst();
        
        if (first == CHARACTERS)
        {
            // merge multiple CHARACTER events into 1
            while (_events.count() > 0 && _events.getAt(0) == CHARACTERS)
            {
                _events.removeFirst();
            }
        }
    
        if (first == CHARACTERS && _events.count() == 0)
        {
            // wait until the next non-CHARACTERS event
            _events.prepend(first);
        }
        else
        {
            _event = first;
        }
    }

    return _event;
}    

void 
OMXMLReaderExpat::clearEvents(void)
{
    TRACE("OMXMLReaderExpat::clearEvents");

    _events.clear();
    _event = NONE;
    _appendData = false;
}    

void 
OMXMLReaderExpat::setCharacterData(const wchar_t* data)
{
    TRACE("OMXMLReaderExpat::setCharacterData");

    if (_appendData)
    {
        wchar_t* tmp = new wchar_t[_data.length() + wcslen(data) + 1];
        wcscpy(tmp, _data.c_str());
        wcscat(tmp, data);
        _data = tmp;
        delete [] tmp;
    }
    else
    {
        _data = data;
    }
}

void 
OMXMLReaderExpat::notationDeclHandler(const XML_Char* notationName, const XML_Char* base,
    const XML_Char* systemId, const XML_Char* publicId)
{
    TRACE("OMXMLReaderExpat::notationDeclHandler");

    wchar_t* workBuffer = getWorkBuffer(xmlStringLen(notationName) + 1);
    OMUInt32 strLen = readCharacters(workBuffer, notationName, 0);
    _notationName = workBuffer;
    
    if (base != 0)
    {
        workBuffer = getWorkBuffer(xmlStringLen(base) + 1);
        strLen = readCharacters(workBuffer, base, 0);
        _base = workBuffer;
    }
    else
    {
        _base = L"";
    }
    
    if (systemId != 0)
    {
        workBuffer = getWorkBuffer(xmlStringLen(systemId) + 1);
        strLen = readCharacters(workBuffer, systemId, 0);
        _systemID = workBuffer;
    }
    else
    {
        _systemID = L"";
    }
    
    if (publicId != 0)
    {
        workBuffer = getWorkBuffer(xmlStringLen(publicId) + 1);
        strLen = readCharacters(workBuffer, publicId, 0);
        _publicID = workBuffer;
    }
    else
    {
        _publicID = L"";
    }
    
    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        XML_Error errorCode = XML_GetErrorCode(_parser);
        if (errorCode != XML_ERROR_SUSPENDED)
        {
            throw OMException(getErrorString());
        }
    }

    registerEvent(NOTATION_DECL);
}

void 
OMXMLReaderExpat::entityDeclHandler(const XML_Char *entityName, 
    int is_parameter_entity, const XML_Char *value, int value_length, 
    const XML_Char *base, const XML_Char *systemId, const XML_Char *publicId, 
    const XML_Char *notationName)
{
    TRACE("OMXMLReaderExpat::entityDeclHandler");

    // only report unparsed (external) entity declarations
    if (is_parameter_entity == 0 && value == 0 && systemId != 0 && notationName != 0)
    {
        wchar_t* workBuffer = getWorkBuffer(xmlStringLen(entityName) + 1);
        OMUInt32 strLen = readCharacters(workBuffer, entityName, 0);
        _name = workBuffer;
        
        workBuffer = getWorkBuffer(xmlStringLen(systemId) + 1);
        strLen = readCharacters(workBuffer, systemId, 0);
        _systemID = workBuffer;
        
        if (publicId != 0)
        {
            workBuffer = getWorkBuffer(xmlStringLen(publicId) + 1);
            strLen = readCharacters(workBuffer, publicId, 0);
            _publicID = workBuffer;
        }
        else
        {
            _publicID = L"";
        }
        
        XML_Status status = XML_StopParser(_parser, true);
        if (status != XML_STATUS_OK)
        {
            XML_Error errorCode = XML_GetErrorCode(_parser);
            if (errorCode != XML_ERROR_SUSPENDED)
            {
                throw OMException(getErrorString());
            }
        }
    
        registerEvent(UNPARSED_ENTITY_DECL);
    }
}

void 
OMXMLReaderExpat::startNamespaceDeclHandler(const XML_Char* prefix, const XML_Char* uri)
{
    TRACE("OMXMLReaderExpat::startNamespaceDeclHandler");

    QName* qName = new QName;
    
    if (prefix != 0)
    {
        wchar_t* workBuffer = getWorkBuffer(xmlStringLen(prefix) + 1);
        readCharacters(workBuffer, prefix, 0);
        qName->prefix = workBuffer;
    }

    if (uri != 0)
    {
        wchar_t* workBuffer = getWorkBuffer(xmlStringLen(uri) + 1);
        readCharacters(workBuffer, uri, 0);
        qName->uri = workBuffer;
    }

    _startNmspaceDecls.append(qName);

    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        XML_Error errorCode = XML_GetErrorCode(_parser);
        if (errorCode != XML_ERROR_SUSPENDED)
        {
            throw OMException(getErrorString());
        }
    }

    registerEvent(START_PREFIX_MAPPING);
}

void 
OMXMLReaderExpat::endNamespaceDeclHandler(const XML_Char* prefix)
{
    TRACE("OMXMLReaderExpat::endNamespaceDeclHandler");

    if (prefix != 0)
    {
        wchar_t* workBuffer = getWorkBuffer(xmlStringLen(prefix) + 1);
        readCharacters(workBuffer, prefix, 0);
        _endNmspaceDecls.append(workBuffer);
    }
    else
    {
        _endNmspaceDecls.append(L"");
    }
    
    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        XML_Error errorCode = XML_GetErrorCode(_parser);
        if (errorCode != XML_ERROR_SUSPENDED)
        {
            throw OMException(getErrorString());
        }
    }

    registerEvent(END_PREFIX_MAPPING);
}

void 
OMXMLReaderExpat::startElementHandler(const XML_Char* name, const XML_Char** atts)
{
    TRACE("OMXMLReaderExpat::startElementHandler");

    wchar_t* workBuffer = getWorkBuffer(xmlStringLen(name) + 1);
    OMUInt32 strLen = readCharacters(workBuffer, name, NAMESPACE_SEPARATOR);
    _uri = workBuffer;

    if (strLen > 0)
    {
        strLen = readCharacters(workBuffer, &(name[strLen + 1]), 0);
    }
    else
    {
        strLen = readCharacters(workBuffer, &(name[strLen]), 0);
    }
    _localName = workBuffer;

    
    OMListIterator<OMXMLAttribute*> iter(_attributes, OMBefore);
    while (++iter)
    {
        delete iter.value();
    }
    _attributes.clear();
    
    if (*atts != 0)
    {
        const XML_Char** attsPtr = atts;
        while (*attsPtr != 0)
        {
            OMWString nmspace;
            OMWString localName;
            OMWString value;
            
            wchar_t* workBuffer = getWorkBuffer(xmlStringLen(*attsPtr) + 1);
            OMUInt32 strLen = readCharacters(workBuffer, *attsPtr, NAMESPACE_SEPARATOR);
            nmspace = workBuffer;
            
            if (strLen > 0)
            {
                strLen = readCharacters(workBuffer, &((*attsPtr)[strLen + 1]), 0);
            }
            else
            {
                strLen = readCharacters(workBuffer, &((*attsPtr)[strLen]), 0);
            }
            localName = workBuffer;

            attsPtr++;

            workBuffer = getWorkBuffer(xmlStringLen(*attsPtr) + 1);
            strLen = readCharacters(workBuffer, *attsPtr, 0);
            value = workBuffer;
            
            _attributes.append(new OMXMLAttribute(nmspace.c_str(), localName.c_str(), value.c_str()));

            attsPtr++;
        }
    }

    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        XML_Error errorCode = XML_GetErrorCode(_parser);
        if (errorCode != XML_ERROR_SUSPENDED)
        {
            throw OMException(getErrorString());
        }
    }

    registerEvent(START_ELEMENT);
}

void 
OMXMLReaderExpat::endElementHandler(const XML_Char* name)
{
    TRACE("OMXMLReaderExpat::endElementHandler");

    wchar_t* workBuffer = getWorkBuffer(xmlStringLen(name) + 1);
    OMUInt32 strLen = readCharacters(workBuffer, name, NAMESPACE_SEPARATOR);
    _uri = workBuffer;

    if (strLen > 0)
    {
        strLen = readCharacters(workBuffer, &(name[strLen + 1]), 0);
    }
    else
    {
        strLen = readCharacters(workBuffer, &(name[strLen]), 0);
    }
    _localName = workBuffer;

    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        XML_Error errorCode = XML_GetErrorCode(_parser);
        if (errorCode != XML_ERROR_SUSPENDED)
        {
            throw OMException(getErrorString());
        }
    }

    registerEvent(END_ELEMENT);
}


void 
OMXMLReaderExpat::characterDataHandler(const XML_Char* s, int len)
{
    TRACE("OMXMLReaderExpat::characterDataHandler");

    // Note: s is not null terminated
    
#if defined(XML_UNICODE_WCHAR_T)
    wchar_t* workBuffer = getWorkBuffer(len + 1);
    memcpy(workBuffer, s, len * sizeof(wchar_t));
    workBuffer[len] = L'\0';
    setCharacterData(workBuffer);
    
#elif defined(XML_UNICODE)
    wchar_t* workBuffer = getWorkBuffer(len + 1);
    const XML_Char* inPtr = s;
    wchar_t* outPtr = workBuffer;
    for (int i = 0; i < len; i++)
    {
        *outPtr = (wchar_t)*inPtr;
        outPtr++;
        inPtr++;
    }
    *outPtr = L'\0';
    setCharacterData(workBuffer);

#else
    char* workBuffer = (char*)getWorkBuffer(len + 1);
    memcpy(workBuffer, s, len * sizeof(char));
    workBuffer[len] = '\0';
    wchar_t* out = utf8ToUTF16(workBuffer);
    setCharacterData(out);
    delete [] out;
#endif

    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        XML_Error errorCode = XML_GetErrorCode(_parser);
        if (errorCode != XML_ERROR_SUSPENDED)
        {
            throw OMException(getErrorString());
        }
    }

    registerEvent(CHARACTERS);
}

OMUInt32 
OMXMLReaderExpat::readNextChunk(void* buffer, OMUInt32 num)
{
    TRACE("OMXMLReaderExpat::readNextChunk");

    OMUInt32 numRead;
    _xmlStream->read((OMByte*)buffer, num, numRead);

    return numRead;
}

OMUInt32 
OMXMLReaderExpat::readCharacters(wchar_t* out, const XML_Char* in, wchar_t terminator)
{
    TRACE("OMXMLReaderExpat::readCharacters(wchar_t*,XML_Char*,wchar_t)");

    OMUInt32 len = 0;
    
#if !defined(XML_UNICODE)

#if defined (XML_UNICODE_WCHAR_T)
    wchar_t* src = in;
#else
    wchar_t* src = utf8ToUTF16(in);
#endif

    if (terminator == L'\0')
    {
        wcscpy(out, src);
        len = wcslen(out);
    }
    else
    {
        wchar_t* atTerminator = wcschr(src, terminator);
        if (atTerminator == 0)
        {
            // no terminator means no data
            len = 0;
            *out = L'\0';
        }
        else
        {
            wcsncpy(out, src, (OMUInt32)(atTerminator - src));
            out[(OMUInt32)(atTerminator - src)] = L'\0';
            len = (OMUInt32)(atTerminator - src);
        }
    }

#if !defined (XML_UNICODE_WCHAR_T)
    delete [] src;
#endif
    
#else // XML_UNICODE
    wchar_t* outPtr = out;
    const XML_Char* inPtr = in;
    *outPtr = (wchar_t)*inPtr;
    while ((wchar_t)(*inPtr) != L'\0' && (wchar_t)(*inPtr) != terminator)
    {
        outPtr++;
        inPtr++;
        len++;
        *outPtr = (wchar_t)*inPtr;
    }

    if ((wchar_t)(*inPtr) == terminator)
    {
        *outPtr = L'\0';
    }
    else if ((wchar_t)(*inPtr) == L'\0')
    {
        // no terminator means no data
        len = 0;
        *out = L'\0';
    }
#endif

    
    return len;
}

OMUInt32 
OMXMLReaderExpat::xmlStringLen(const XML_Char* s) const
{
    TRACE("OMXMLReaderExpat::xmlStringLen");

#if defined (XML_UNICODE_WCHAR_T)
    return wcslen(s);
    
#elif defined (XML_UNICODE)
    OMUInt32 len = 0;
    const XML_Char* sPtr = s;
    while (*sPtr != 0)
    {
        sPtr++;
        len++;
    }
    return len;

#else
    return utf16StrLen(s);
    
#endif
}

wchar_t* 
OMXMLReaderExpat::getWorkBuffer(OMUInt32 size)
{
    TRACE("OMXMLReaderExpat::getWorkBuffer(OMUInt32)");

    if (size > _workBufferSize)
    {
        if (_workBuffer != 0)
        {
            delete [] _workBuffer;
        }
        _workBuffer = new wchar_t[size];
        _workBufferSize = size;
    }
    else
    {
        if (size < WORK_BUFFER_CLEANUP_SIZE && _workBufferSize > WORK_BUFFER_CLEANUP_SIZE)
        {
            if (_workBuffer != 0)
            {
                delete [] _workBuffer;
            }
            _workBuffer = new wchar_t[size];
            _workBufferSize = size;
        }
    }

    return _workBuffer;
}

inline wchar_t* 
OMXMLReaderExpat::getWorkBuffer(void)
{
    TRACE("OMXMLReaderExpat::getWorkBuffer");

    return _workBuffer;
}

OMUInt32 
OMXMLReaderExpat::getWorkBufferSize(void)
{
    TRACE("OMXMLReaderExpat::getWorkBufferSize");

    return _workBufferSize;
}

const char* 
OMXMLReaderExpat::getErrorString()
{
    TRACE("OMXMLReaderExpat::getErrorString");

    strcpy(_errorString, "XMLReader: ");

    const XML_LChar* xmlErrorString = XML_ErrorString(XML_GetErrorCode(_parser));
#if defined (XML_UNICODE_WCHAR_T)
        char* tmp = utf16ToUTF8((wchar_t*)xmlErrorString);
        strcat(_errorString, tmp);
        delete [] tmp;
#else
        strcat(_errorString, xmlErrorString);
#endif
    
    return _errorString;
}



void 
expat_NotationDeclHandler(void* userData, const XML_Char* notationName,
    const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId)
{
    TRACE("::expat_NotationDeclHandler");

    OMXMLReaderExpat* reader = reinterpret_cast<OMXMLReaderExpat*>(userData);
    ASSERT("Valid reader", reader != 0);

    reader->notationDeclHandler(notationName, base, systemId, publicId);
}

void 
expat_EntityDeclHandler(void* userData, const XML_Char *entityName, 
    int is_parameter_entity, const XML_Char *value, int value_length, 
    const XML_Char *base, const XML_Char *systemId, const XML_Char *publicId, 
    const XML_Char *notationName)
{
    TRACE("::expat_EntityDeclHandler");

    OMXMLReaderExpat* reader = reinterpret_cast<OMXMLReaderExpat*>(userData);
    ASSERT("Valid reader", reader != 0);

    reader->entityDeclHandler(entityName, is_parameter_entity, value, value_length,
        base, systemId, publicId, notationName);
}
    
void 
expat_StartNamespaceDeclHandler(void* userData, const XML_Char* prefix,
    const XML_Char* uri)
{
    TRACE("::expat_StartNamespaceDeclHandler");

    OMXMLReaderExpat* reader = reinterpret_cast<OMXMLReaderExpat*>(userData);
    ASSERT("Valid reader", reader != 0);

    reader->startNamespaceDeclHandler(prefix, uri);
}

void 
expat_EndNamespaceDeclHandler(void* userData, const XML_Char *prefix)
{
    TRACE(":;expat_EndNamespaceDeclHandler");

    OMXMLReaderExpat* reader = reinterpret_cast<OMXMLReaderExpat*>(userData);
    ASSERT("Valid reader", reader != 0);

    reader->endNamespaceDeclHandler(prefix);
}

void 
expat_StartElementHandler(void* userData, const XML_Char* name, const XML_Char** atts)
{
    TRACE("::expat_StartElementHandler");

    OMXMLReaderExpat* reader = reinterpret_cast<OMXMLReaderExpat*>(userData);
    ASSERT("Valid reader", reader != 0);

    reader->startElementHandler(name, atts);
}

void 
expat_EndElementHandler(void* userData, const XML_Char* name)
{
    TRACE("::expat_EndElementHandler");

    OMXMLReaderExpat* reader = reinterpret_cast<OMXMLReaderExpat*>(userData);
    ASSERT("Valid reader", reader != 0);

    reader->endElementHandler(name);
}

void 
expat_CharacterDataHandler(void* userData, const XML_Char* s, int len)
{
    TRACE("::expat_CharacterDataHandler");

    OMXMLReaderExpat* reader = reinterpret_cast<OMXMLReaderExpat*>(userData);
    ASSERT("Valid reader", reader != 0);

    reader->characterDataHandler(s, len);
}

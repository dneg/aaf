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


#include <XMLReader.h>
#include <cassert>

using namespace std;


XMLAttribute::XMLAttribute()
: _next(0), _prev(0)
{}

XMLAttribute::XMLAttribute(const char* nmspace, const char* localName, const char* value)
{
    assert(localName != 0);
    assert(value != 0);
    
    if (nmspace == 0)
    {
        _nmspace = "";
    }
    else
    {
        _nmspace = nmspace;
    }
    _localName = localName;
    _value = value;
    _next = 0;
    _prev = 0;
}


XMLAttribute::~XMLAttribute()
{
    if (_next != 0)
    {
        delete _next;
    }
}


void 
XMLAttribute::SetNamespace(const char* nmspace)
{
    if (nmspace == 0)
    {
        _nmspace = "";
    }
    else
    {
        _nmspace = nmspace;
    }
}

    
void 
XMLAttribute::SetLocalName(const char* localName)
{
    _localName = localName;
}


void 
XMLAttribute::SetValue(const char* value)
{
    _value = value;
}


string 
XMLAttribute::GetNamespace(void) const
{
    return _nmspace;
}


string 
XMLAttribute::GetLocalName(void) const
{
    return _localName;
}


string 
XMLAttribute::GetValue(void) const
{
    return _value;
}


void 
XMLAttribute::Append(XMLAttribute* a)
{
    assert(a != 0);

    if (_next != 0)
    {
        _next->Append(a);
    }
    else
    {
        _next = a;
    }
}


const XMLAttribute* 
XMLAttribute::First(void) const
{
    if (_prev != 0)
    {
        return _prev->First();
    }
    
    return this;
}


const XMLAttribute* 
XMLAttribute::Next(void) const
{
    return _next;
}


const XMLAttribute* 
XMLAttribute::Prev(void) const
{
    return _prev;
}


const XMLAttribute* 
XMLAttribute::Get(const char* nmspace, const char* localName) const
{
    assert(localName != 0);
    
    string ns;
    if (nmspace != 0)
    {
        ns = nmspace;
    }
    
    if (_nmspace == ns && _localName == localName)
    {
        return this;
    }
    
    const XMLAttribute* a = 0;
    if (_prev != 0)
    {
        a = _prev->Get(ns.c_str(), localName);
    }
    
    if (a == 0 && _next != 0)
    {
        a = _next->Get(ns.c_str(), localName);
    }
    
    return a;
}

XMLAttribute* 
XMLAttribute::Copy(void) const
{
    XMLAttribute* attributes = new XMLAttribute(_nmspace.c_str(), _localName.c_str(), 
        _value.c_str());
    if (_next != 0)
    {
        attributes->Append(_next->Copy());
    }
    return attributes;
}



XMLReader::XMLReader()
: _appendData(false), _attributes(0)
{
    ClearEvents();
}


XMLReader::~XMLReader()
{
    if (_attributes != 0)
    {
        delete _attributes;
    }
}

void 
XMLReader::RegisterListener(XMLReaderListener* listener)
{
    _listeners.push_back(listener);
}


XMLReader::EventType 
XMLReader::Event(void)
{
    return _event;
}

void 
XMLReader::UnparsedEntityDecl(const char*& name, const char*& publicID, const char*& systemID, 
    const char*& notationName)
{
    assert(_event == UNPARSED_ENTITY_DECL);
    
    name = _name.c_str();
    publicID = _publicID.c_str();
    systemID = _systemID.c_str();
    notationName = _notationName.c_str();
}

void 
XMLReader::StartElement(const char*& uri, const char*& localName, const XMLAttribute*& attributes)
{
    assert(_event == START_ELEMENT);
    
    uri = _uri.c_str();
    localName = _localName.c_str();
    attributes = _attributes;
}


void 
XMLReader::EndElement(const char*& uri, const char*& localName)
{
    assert(_event == END_ELEMENT);
    
    uri = _uri.c_str();
    localName = _localName.c_str();
}


void 
XMLReader::Characters(const char*& data, size_t& length)
{
    assert(_event == CHARACTERS);
    
    data = _data.c_str();
    length = _data.length();
}


void 
XMLReader::RegisterEvent(EventType event)
{
    _events.push(event);

    if (event == CHARACTERS)
    {
        _appendData = true;
    }
    else
    {
        _appendData = false;
    }
}


XMLReader::EventType 
XMLReader::NextEvent(void)
{
    _event = NONE;
    if (_events.size() != 0)
    {
        EventType front = _events.front();
        _events.pop();
    
        if (front == CHARACTERS && _events.size() == 0)
        {
            // wait until the next event
            _events.push(front);
        }
        else if (front != CHARACTERS || _events.front() != CHARACTERS)
        {
            _event = front;
        }
        // else reduce multiple character events to 1 event
    }

    
    switch (_event)
    {
        case NONE:
            // do nothing
            break;
        case NOTATION_DECL:
            {
                vector<XMLReaderListener*>::const_iterator iter;
                for (iter = _listeners.begin(); iter != _listeners.end(); iter++)
                {
                    (*iter)->NotationDecl(_notationName.c_str(), _publicID.c_str(), _systemID.c_str());
                }
            }
            break;
        case UNPARSED_ENTITY_DECL:
            {
                vector<XMLReaderListener*>::const_iterator iter;
                for (iter = _listeners.begin(); iter != _listeners.end(); iter++)
                {
                    (*iter)->UnparsedEntityDecl(_name.c_str(), _publicID.c_str(), _systemID.c_str(), 
                        _notationName.c_str());
                }
            }
            break;
        case START_ELEMENT:
            {
                vector<XMLReaderListener*>::const_iterator iter;
                for (iter = _listeners.begin(); iter != _listeners.end(); iter++)
                {
                    (*iter)->StartElement(_uri.c_str(), _localName.c_str(), _attributes);
                }
            }
            break;
        case END_ELEMENT:
            {
                vector<XMLReaderListener*>::const_iterator iter;
                for (iter = _listeners.begin(); iter != _listeners.end(); iter++)
                {
                    (*iter)->EndElement(_uri.c_str(), _localName.c_str());
                }
            }
            break;
        case CHARACTERS:
            {
                vector<XMLReaderListener*>::const_iterator iter;
                for (iter = _listeners.begin(); iter != _listeners.end(); iter++)
                {
                    (*iter)->Characters(_data.c_str(), _data.length());
                }
            }
            break;
        default:
            assert(false);
            break;
    }
    
    return _event;
}    


void 
XMLReader::ClearEvents(void)
{
    while (_events.size() > 0)
    {
        _events.pop();
    }
    _event = NONE;
    _appendData = false;
}    


void 
XMLReader::SetCharacterData(const char* data, size_t /*len*/)
{
    if (_appendData)
    {
        _data += data;
    }
    else
    {
        _data = data;
    }
}


void 
XMLReader::SetAttributes(XMLAttribute* attributes)
{
    if (_attributes != 0)
    {
        delete _attributes;
    }
    _attributes = attributes;
}


#if defined(HAVE_EXPAT)


#define NAMESPACE_SEPARATOR        0x20
#define READ_CHUNK_SIZE            2048

#define WORK_BUFFER_MIN_SIZE       1024
#define WORK_BUFFER_CLEANUP_SIZE   4096


XMLReaderExpat::XMLReaderExpat(XMLIStream* xmlStream)
: XMLReader(), _xmlStream(xmlStream), _parser(0)
{
    _parser = XML_ParserCreateNS(0, NAMESPACE_SEPARATOR);
    XML_SetEntityDeclHandler(_parser, ::expat_EntityDeclHandler);
    XML_SetStartElementHandler(_parser, ::expat_StartElementHandler);
    XML_SetEndElementHandler(_parser, ::expat_EndElementHandler);
    XML_SetCharacterDataHandler(_parser, ::expat_CharacterDataHandler);
    XML_SetUserData(_parser, this);

    _workBuffer = new char[WORK_BUFFER_MIN_SIZE];
    _workBufferSize = WORK_BUFFER_MIN_SIZE;

    _filePosition = 0;
    _readNextChunk = true;
    _status = true;
    _numInBuffer = 0;
    _ignoreCharacterData = false;

}

XMLReaderExpat::~XMLReaderExpat()
{
    XML_ParserFree(_parser);

    if (_workBuffer != 0)
    {
        delete [] _workBuffer;
    }

    if (_xmlStream != 0)
    {
        delete _xmlStream;
    }
}


bool 
XMLReaderExpat::Next(bool ignoreCharacterData)
{
    if (_status == false)
    {
        return _status;
    }

    _ignoreCharacterData = ignoreCharacterData;
    while (_status && NextEvent() == NONE)
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
                    assert(false);
                }
            }
        }

        if (_readNextChunk)
        {
            void* buffer = XML_GetBuffer(_parser, READ_CHUNK_SIZE);
            _numInBuffer = ReadNextChunk(buffer, READ_CHUNK_SIZE);

            int ret = XML_ParseBuffer(_parser, _numInBuffer, _numInBuffer < READ_CHUNK_SIZE);

            if (ret == XML_STATUS_ERROR)
            {
                /*XML_Error errorCode = */XML_GetErrorCode(_parser);
                assert(false);
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

string 
XMLReaderExpat::GetPositionString(void)
{
    int lineNumber = XML_GetCurrentLineNumber(_parser);
    int columnNumber = XML_GetCurrentColumnNumber(_parser);
    
    char buffer[30];
    sprintf(buffer, "line %d, column %d", lineNumber, columnNumber);
    
    return buffer;
}

void 
XMLReaderExpat::EntityDeclHandler(const XML_Char *entityName, 
    int is_parameter_entity, const XML_Char *value, int value_length, 
    const XML_Char *base, const XML_Char *systemId, const XML_Char *publicId, 
    const XML_Char *notationName)
{
    // only report unparsed (external) entity declarations
    if (is_parameter_entity == 0 && value == 0 && systemId != 0 && notationName != 0)
    {
        char* workBuffer = GetWorkBuffer(XMLStringLen(entityName, 0) + 1);
        OMUInt32 strLen = ReadCharacters(workBuffer, entityName, 0, GetWorkBufferSize());
        _name = workBuffer;
        
        workBuffer = GetWorkBuffer(XMLStringLen(systemId, 0) + 1);
        strLen = ReadCharacters(workBuffer, systemId, 0, GetWorkBufferSize());
        _systemID = workBuffer;
        
        if (publicId != 0)
        {
            workBuffer = GetWorkBuffer(XMLStringLen(publicId, 0) + 1);
            strLen = ReadCharacters(workBuffer, publicId, 0, GetWorkBufferSize());
            _publicID = workBuffer;
        }
        else
        {
            _publicID = "";
        }
        
        XML_Status status = XML_StopParser(_parser, true);
        if (status != XML_STATUS_OK)
        {
            /*XML_Error errorCode = */XML_GetErrorCode(_parser);
            assert(false);
        }
    
        RegisterEvent(UNPARSED_ENTITY_DECL);
    }
}

void 
XMLReaderExpat::StartElementHandler(const XML_Char* name, const XML_Char** atts)
{
    assert(name != 0);

    char* workBuffer = GetWorkBuffer(XMLStringLen(name, 0) + 1);
    OMUInt32 strLen = ReadCharacters(workBuffer, name, NAMESPACE_SEPARATOR, GetWorkBufferSize());
    _uri = workBuffer;
    if (strLen > 0)
    {
        strLen = ReadCharacters(workBuffer, &(name[strLen + 1]), 0, GetWorkBufferSize());
    }
    else
    {
        strLen = ReadCharacters(workBuffer, &(name[strLen]), 0, GetWorkBufferSize());
    }
    _localName = workBuffer;

    if (*atts != 0)
    {
        XMLAttribute* root = new XMLAttribute();

        bool isRoot = true;
        const XML_Char** attsPtr = atts;
        while (*attsPtr != 0)
        {
            XMLAttribute* a = 0;
            if (isRoot)
            {
                a = root;
                isRoot = false;
            }
            else
            {
                a = new XMLAttribute();
                root->Append(a);
            }

            char* workBuffer = GetWorkBuffer(XMLStringLen(*attsPtr, 0) + 1);
            OMUInt32 strLen = ReadCharacters(workBuffer, *attsPtr, NAMESPACE_SEPARATOR, GetWorkBufferSize());
            a->SetNamespace(workBuffer);
            if (strLen > 0)
            {
                strLen = ReadCharacters(workBuffer, &((*attsPtr)[strLen + 1]), 0, GetWorkBufferSize());
            }
            else
            {
                strLen = ReadCharacters(workBuffer, &((*attsPtr)[strLen]), 0, GetWorkBufferSize());
            }
            a->SetLocalName(workBuffer);

            attsPtr++;

            workBuffer = GetWorkBuffer(XMLStringLen(*attsPtr, 0) + 1);
            strLen = ReadCharacters(workBuffer, *attsPtr, 0, GetWorkBufferSize());
            a->SetValue(workBuffer);

            attsPtr++;
        }

        SetAttributes(root);
    }
    else
    {
        SetAttributes(0);
    }

    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        /*XML_Error errorCode = */XML_GetErrorCode(_parser);
        assert(false);
    }

    RegisterEvent(START_ELEMENT);
}

void 
XMLReaderExpat::EndElementHandler(const XML_Char* name)
{
    assert(name != 0);

    char* workBuffer = GetWorkBuffer(XMLStringLen(name, 0) + 1);
    OMUInt32 strLen = ReadCharacters(workBuffer, name, NAMESPACE_SEPARATOR, GetWorkBufferSize());
    _uri = workBuffer;
    if (strLen > 0)
    {
        strLen = ReadCharacters(workBuffer, &(name[strLen + 1]), 0, GetWorkBufferSize());
    }
    else
    {
        strLen = ReadCharacters(workBuffer, &(name[strLen]), 0, GetWorkBufferSize());
    }
    _localName = workBuffer;

    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        XML_Error errorCode = XML_GetErrorCode(_parser);
        if (errorCode != XML_ERROR_SUSPENDED)
        {
            assert(false);
        }
        // combined start/end element brings us here
    }

    RegisterEvent(END_ELEMENT);
}


void 
XMLReaderExpat::CharacterDataHandler(const XML_Char* s, int len)
{
    assert(s != 0);

    if (!_ignoreCharacterData)
    {
        char* workBuffer = GetWorkBuffer(len + 1);
        ReadCharacters(workBuffer, s, len);
        workBuffer[len] = 0;
        SetCharacterData(workBuffer, len);
    }

    XML_Status status = XML_StopParser(_parser, true);
    if (status != XML_STATUS_OK)
    {
        /*XML_Error errorCode = */XML_GetErrorCode(_parser);
        assert(false);
    }

    RegisterEvent(CHARACTERS);
}

OMUInt32 
XMLReaderExpat::ReadNextChunk(void* buffer, OMUInt32 num)
{
    assert(buffer != 0);
    assert(num != 0);

    OMUInt32 numRead = _xmlStream->Read((OMByte*)buffer, num);
    _filePosition += numRead;

    return numRead;
}

OMUInt32 
XMLReaderExpat::ReadCharacters(char* out, const XML_Char* in, char terminator, OMUInt32 maxSize)
{
    assert(out != 0);
    assert(in != 0);


    OMUInt32 size = 0;
    char* outPtr = out;
    const XML_Char* inPtr = in;
    *outPtr = *inPtr;
    while (*inPtr != 0 && (char)(*inPtr) != terminator && size < maxSize)
    {
        outPtr++;
        inPtr++;
        size++;
        *outPtr = (char)*inPtr;
    }

    if ((char)(*inPtr) == terminator)
    {
        *outPtr = 0;
    }
    else if (*inPtr == 0)
    {
        // no terminator means no data
        size = 0;
        *out = 0;
    }
    else
    {
        assert(size == maxSize);
    }

    return size;
}

void 
XMLReaderExpat::ReadCharacters(char* out, const XML_Char* in, OMUInt32 len)
{
    assert(out != 0);
    assert(in != 0);

    OMUInt32 i;
    const XML_Char* inPtr = in;
    char* outPtr = out;
    for (i=0; i<len; i++, outPtr++, inPtr++)
    {
        *outPtr = (char)*inPtr;
    }
}

char* 
XMLReaderExpat::GetWorkBuffer(OMUInt32 size)
{
    if (size > _workBufferSize)
    {
        if (_workBuffer != 0)
        {
            delete [] _workBuffer;
        }
        _workBuffer = new char[size];
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
            _workBuffer = new char[size];
            _workBufferSize = size;
        }
    }

    return _workBuffer;
}

inline char* 
XMLReaderExpat::GetWorkBuffer(void)
{
    return _workBuffer;
}

OMUInt32 
XMLReaderExpat::GetWorkBufferSize(void)
{
    return _workBufferSize;
}

OMUInt32 
XMLReaderExpat::XMLStringLen(const XML_Char* s, XML_Char terminator) const
{
    assert(s != 0);

    OMUInt32 len = 0;
    const XML_Char* sPtr = s;
    while (*sPtr != terminator)
    {
        sPtr++;
        len++;
    }

    return len;
}


void 
expat_EntityDeclHandler(void* userData, const XML_Char *entityName, 
    int is_parameter_entity, const XML_Char *value, int value_length, 
    const XML_Char *base, const XML_Char *systemId, const XML_Char *publicId, 
    const XML_Char *notationName)
{
    XMLReaderExpat* reader = reinterpret_cast<XMLReaderExpat*>(userData);
    assert(reader != 0);

    reader->EntityDeclHandler(entityName, is_parameter_entity, value, value_length,
        base, systemId, publicId, notationName);
}
    
void 
expat_StartElementHandler(void* userData, const XML_Char* name, const XML_Char** atts)
{
    XMLReaderExpat* reader = reinterpret_cast<XMLReaderExpat*>(userData);
    assert(reader != 0);

    reader->StartElementHandler(name, atts);
}

void 
expat_EndElementHandler(void* userData, const XML_Char* name)
{
    XMLReaderExpat* reader = reinterpret_cast<XMLReaderExpat*>(userData);
    assert(reader != 0);

    reader->EndElementHandler(name);
}

void 
expat_CharacterDataHandler(void* userData, const XML_Char* s, int len)
{
    XMLReaderExpat* reader = reinterpret_cast<XMLReaderExpat*>(userData);
    assert(reader != 0);

    reader->CharacterDataHandler(s, len);
}


#endif // HAVE_EXPAT

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

#ifndef __XMLREADER_H__
#define __XMLREADER_H__


#include <OMDataTypes.h>
#include <string>
#include <vector>
#include <queue>


class XMLAttribute;

class XMLReaderListener
{
public:
    virtual void NotationDecl(const char* name, const char* publicID, const char* systemID) = 0;
    virtual void UnparsedEntityDecl(const char* name, const char* publicID, const char* systemID, 
        const char* notationName) = 0;
    virtual void StartElement(const char* uri, const char* localName, 
        const XMLAttribute* attributes) = 0;
    virtual void EndElement(const char* uri, const char* localName) = 0;
    virtual void Characters(const char* data, size_t length) = 0;
};


class XMLReader
{
public:
    XMLReader(void);
    virtual ~XMLReader(void);

    virtual bool Next(bool ignoreCharacterData) = 0;
    virtual std::string GetPositionString(void) = 0;
    
    void RegisterListener(XMLReaderListener* listener);

    enum EventType
    {
        NONE,
        NOTATION_DECL,
        UNPARSED_ENTITY_DECL,
        START_ELEMENT,
        END_ELEMENT,
        CHARACTERS
    };

    EventType Event(void);
    
    void NotationDecl(const char*& name, const char*& publicID, const char*& systemID);
    void UnparsedEntityDecl(const char*& name, const char*& publicID, const char*& systemID, const char*& notationName);
    void StartElement(const char*& uri, const char*& localName, const XMLAttribute*& attributes);
    void EndElement(const char*& uri, const char*& localName);
    void Characters(const char*& data, size_t& length);

    
protected:
    void RegisterEvent(EventType event);
    EventType NextEvent(void);
    void ClearEvents(void);
    void SetCharacterData(const char* data, size_t len);
    void SetAttributes(XMLAttribute* attributes);
    

    std::vector<XMLReaderListener*>     _listeners;
    std::queue<EventType>  _events;
    EventType        _event;
    std::string      _name;
    std::string      _publicID;
    std::string      _systemID;
    std::string      _notationName;
    std::string      _uri;
    std::string      _localName;
    std::string      _qName;
    std::string      _data;
    bool             _appendData;
    size_t           _length;
    std::string      _prefix;
    XMLAttribute*    _attributes;
    
};


class XMLAttribute
{
public:
    XMLAttribute();
    XMLAttribute(const char* nmspace, const char* localName, const char* value);
    ~XMLAttribute();

    void SetNamespace(const char* nmspace);
    void SetLocalName(const char* localName);
    void SetValue(const char* value);
    
    std::string GetNamespace(void) const;
    std::string GetLocalName(void) const;
    std::string GetValue(void) const;

    void Append(XMLAttribute* a);
    const XMLAttribute* First(void) const;
    const XMLAttribute* Next(void) const;
    const XMLAttribute* Prev(void) const;
    const XMLAttribute* Get(const char* nmspace, const char* localName) const;
    
    XMLAttribute* Copy(void) const;
    
private:
    std::string     _nmspace;
    std::string     _localName;
    std::string     _value;
    
    XMLAttribute*   _next;
    XMLAttribute*   _prev;
};



class XMLIStream
{
public:
    XMLIStream(void) {}
    virtual ~XMLIStream(void) {}
    
    virtual OMUInt64 Read(OMByte* data, OMUInt64 count) = 0;
};



#if defined(HAVE_EXPAT)

#include <expat.h>

class XMLReaderExpat : public XMLReader
{
public:
    XMLReaderExpat(XMLIStream* xmlFile);
    virtual ~XMLReaderExpat();

    // from XMLReader
    virtual bool Next(bool ignoreCharacterData);
    virtual std::string GetPositionString(void);
    
public:
    // internal
    void EntityDeclHandler(const XML_Char *entityName, 
        int is_parameter_entity, const XML_Char *value, int value_length, 
        const XML_Char *base, const XML_Char *systemId, const XML_Char *publicId, 
        const XML_Char *notationName);
    void StartElementHandler(const XML_Char* name, const XML_Char** atts);
    void EndElementHandler(const XML_Char* name);
    void CharacterDataHandler(const XML_Char* s, int len);

private:
    
    OMUInt64 ReadNextChunk(void* buffer, OMUInt64 num);
    XML_Parser GetParser(void) const;
    char* GetWorkBuffer(OMUInt64 len);
    char* GetWorkBuffer(void);
    OMUInt64 GetWorkBufferSize(void);
    OMUInt64 XMLStringLen(const XML_Char* s, XML_Char terminator) const;
    OMUInt64 ReadCharacters(char* out, const XML_Char* in, char terminator, OMUInt64 maxSize);
    void ReadCharacters(char* out, const XML_Char* in, OMUInt64 size);
    
    XMLIStream* _xmlStream;
    OMUInt64    _filePosition;
    bool        _ignoreCharacterData;
    XML_Parser  _parser;
    bool        _readNextChunk;
    bool        _status;
    OMUInt64    _numInBuffer;
    char*       _workBuffer;
    OMUInt64    _workBufferSize;
    
};


// expat handlers
void expat_EntityDeclHandler(void* userData, const XML_Char *entityName, 
    int is_parameter_entity, const XML_Char *value, int value_length, 
    const XML_Char *base, const XML_Char *systemId, const XML_Char *publicId, 
    const XML_Char *notationName);
void expat_StartElementHandler(void* userData, const XML_Char* name, const XML_Char** atts);
void expat_EndElementHandler(void* userData, const XML_Char* name);
void expat_CharacterDataHandler(void* userData, const XML_Char* s, int len);

#endif // HAVE_EXPAT

#endif



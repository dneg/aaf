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


#ifndef __OMXMLREADER_H__
#define __OMXMLREADER_H__


#include "XMLReader.h"
#include "OMRawStorage.h"
#include "OMWString.h"
#include "OMList.h"
#include "OMListIterator.h"


class OMXMLIStream;
class OMXMLAttribute;

class OMXMLReader : public XMLReaderListener
{
public:
    OMXMLReader(OMRawStorage* storage);
    virtual ~OMXMLReader();

    enum EventType
    {
        NONE,
        NOTATION_DECL,
        UNPARSED_ENTITY_DECL,
        START_PREFIX_MAPPING,
        END_PREFIX_MAPPING,
        START_ELEMENT,
        END_ELEMENT,
        CHARACTERS
    };

    bool next();
    bool nextElement();
    bool nextEndElement();
    bool moveToEndElement();
    void skipContent();
    EventType getEventType();
    bool elementEquals(const wchar_t* uri, const wchar_t* localName);

    void getNotationDecl(const wchar_t*& name, const wchar_t*& publicID, const wchar_t*& systemID);
    void getUnparsedEntityDecl(const wchar_t*& name, const wchar_t*& publicID, 
        const wchar_t*& systemID, const wchar_t*& notationName);
    void getStartPrefixMapping(const wchar_t*& prefix, const wchar_t*& uri);
    void getEndPrefixMapping(const wchar_t*& prefix);
    void getStartElement(const wchar_t*& uri, const wchar_t*& localName, 
        const OMList<OMXMLAttribute*>*& attributes);
    void getEndElement(const wchar_t*& uri, const wchar_t*& localName);
    void getCharacters(const wchar_t*& data, size_t& length);

    const wchar_t* getPositionString();
    
    OMXMLAttribute* getAttribute(const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* nmspace, const wchar_t* localName);
    
public:
    virtual void NotationDecl(const char* name, const char* publicID, const char* systemID);
    virtual void UnparsedEntityDecl(const char* name, const char* publicID, const char* systemID, 
        const char* notationName);
    virtual void StartPrefixMapping(const char* prefix, const char* uri);
    virtual void EndPrefixMapping(const char* prefix);
    virtual void StartElement(const char* uri, const char* localName, 
        const XMLAttribute* attributes);
    virtual void EndElement(const char* uri, const char* localName);
    virtual void Characters(const char* data, size_t length);

private:
    void cleanUp();

    XMLReader*  _xmlReader;
    
    EventType   _eventType;
    OMWString   _name;
    OMWString   _publicID;
    OMWString   _systemID;
    OMWString   _notationName;
    OMWString   _prefix;
    OMWString   _uri;
    OMWString   _localName;
    OMWString   _data;
    OMList<OMXMLAttribute*>   _attributes;
    OMWString   _positionString;
};


class OMXMLAttribute
{
public:
    friend class OMXMLReader;
    
public:
    ~OMXMLAttribute();

    const wchar_t* getNamespace() const;
    const wchar_t* getLocalName() const;
    const wchar_t* getValue() const;
    
protected:    
    OMXMLAttribute(const char* nmspace, const char* localName, const char* value);

private:
    OMWString       _nmspace;
    OMWString       _localName;
    OMWString       _value;
    
};


class OMXMLIStream : public XMLIStream
{
public:
    OMXMLIStream(OMRawStorage* storage);
    virtual ~OMXMLIStream();
    
    virtual OMUInt32 Read(OMByte* data, OMUInt32 count);
    
private:
    OMRawStorage*   _storage;
    OMUInt64        _position;
    OMUInt64        _size;
};



#endif

//=---------------------------------------------------------------------=
//
// $Id$
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

#include "OMRawStorage.h"
#include "OMWString.h"
#include "OMList.h"
#include "OMListIterator.h"
#include "OMVector.h"


class OMXMLAttribute
{
public:
    OMXMLAttribute(const wchar_t* nmspace, const wchar_t* localName, const wchar_t* value);
    ~OMXMLAttribute();

    const wchar_t* getNamespace() const;
    const wchar_t* getLocalName() const;
    const wchar_t* getValue() const;

private:
    OMWString       _nmspace;
    OMWString       _localName;
    OMWString       _value;
};


class OMXMLReader
{
public:
    static OMXMLReader* create(OMRawStorage* xmlStream);
    
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

    virtual bool next() = 0;
    virtual void reset() = 0;
    
    virtual EventType getEventType() = 0;
    virtual void getNotationDecl(const wchar_t*& name, const wchar_t*& publicID, 
        const wchar_t*& systemID) = 0;
    virtual void getUnparsedEntityDecl(const wchar_t*& name, const wchar_t*& publicID, 
        const wchar_t*& systemID, const wchar_t*& notationName) = 0;
    virtual void getStartPrefixMapping(const wchar_t*& prefix, const wchar_t*& uri) = 0;
    virtual void getEndPrefixMapping(const wchar_t*& prefix) = 0;
    virtual void getStartElement(const wchar_t*& uri, const wchar_t*& localName, 
        const OMList<OMXMLAttribute*>*& attributes) = 0;
    virtual void getEndElement(const wchar_t*& uri, const wchar_t*& localName) = 0;
    virtual void getCharacters(const wchar_t*& data, OMUInt32& length) = 0;

    virtual const wchar_t* getPositionString() = 0;
    
public:
    // utility functions
    bool nextElement();
    bool nextEndElement();
    bool moveToEndElement();
    void skipContent();
    bool elementEquals(const wchar_t* uri, const wchar_t* localName);
    OMXMLAttribute* getAttribute(const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* nmspace, const wchar_t* localName);
};



#include <expat.h>


class OMXMLReaderExpat : public OMXMLReader
{
public:
    OMXMLReaderExpat(OMRawStorage* xmlStream);
    virtual ~OMXMLReaderExpat();

    virtual bool next();
    virtual void reset();
    
    virtual EventType getEventType();
    virtual void getNotationDecl(const wchar_t*& name, const wchar_t*& publicID, 
        const wchar_t*& systemID);
    virtual void getUnparsedEntityDecl(const wchar_t*& name, const wchar_t*& publicID, 
        const wchar_t*& systemID, const wchar_t*& notationName);
    virtual void getStartPrefixMapping(const wchar_t*& prefix, const wchar_t*& uri);
    virtual void getEndPrefixMapping(const wchar_t*& prefix);
    virtual void getStartElement(const wchar_t*& uri, const wchar_t*& localName, 
        const OMList<OMXMLAttribute*>*& attributes);
    virtual void getEndElement(const wchar_t*& uri, const wchar_t*& localName);
    virtual void getCharacters(const wchar_t*& data, OMUInt32& length);

    virtual const wchar_t* getPositionString();

    
public:
    void notationDeclHandler(const XML_Char* notationName, const XML_Char* base,
        const XML_Char* systemId, const XML_Char* publicId);
    void entityDeclHandler(const XML_Char* entityName, 
        int is_parameter_entity, const XML_Char* value, int value_length, 
        const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId, 
        const XML_Char* notationName);
    void startNamespaceDeclHandler(const XML_Char* prefix, const XML_Char* uri);
    void endNamespaceDeclHandler(const XML_Char* prefix);
    void startElementHandler(const XML_Char* name, const XML_Char** atts);
    void endElementHandler(const XML_Char* name);
    void characterDataHandler(const XML_Char* s, int len);
    
private:
    void registerEvent(EventType event);
    EventType nextEvent(void);
    void clearEvents(void);
    void setCharacterData(const wchar_t* data);

    OMUInt32 readNextChunk(void* buffer, OMUInt32 num);
    OMUInt32 readCharacters(wchar_t* out, const XML_Char* in, wchar_t terminator);
    OMUInt32 xmlStringLen(const XML_Char* s) const;

    wchar_t* getWorkBuffer(OMUInt32 len);
    wchar_t* getWorkBuffer(void);
    OMUInt32 getWorkBufferSize(void);

    const char* getErrorString();
    
    OMVector<EventType>  _events;
    EventType        _event;
    OMWString        _name;
    OMWString        _publicID;
    OMWString        _systemID;
    OMWString        _notationName;
    OMWString        _base;
    OMWString        _uri;
    OMWString        _localName;
    OMList<OMXMLAttribute*>   _attributes;
    OMWString        _data;
    OMWString        _prefix;
    OMWString        _prefixUri;

    bool             _appendData;
    
    struct QName
    {
        OMWString uri;
        OMWString prefix;
    };
    OMVector<QName*>    _startNmspaceDecls;
    OMVector<OMWString> _endNmspaceDecls;
    
    OMWString   _positionString;
    char _errorString[512];

    OMRawStorage* _xmlStream;
    XML_Parser  _parser;
    bool        _readNextChunk;
    bool        _status;
    OMUInt32    _numInBuffer;
    wchar_t*    _workBuffer;
    OMUInt32    _workBufferSize;
};

// expat handlers
void expat_NotationDeclHandler(void* userData, const XML_Char* notationName,
    const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId);
void expat_EntityDeclHandler(void* userData, const XML_Char* entityName, 
    int is_parameter_entity, const XML_Char* value, int value_length, 
    const XML_Char* base, const XML_Char* systemId, const XML_Char* publicId, 
    const XML_Char* notationName);
void expat_StartNamespaceDeclHandler(void* userData, const XML_Char* prefix,
    const XML_Char* uri);
void expat_EndNamespaceDeclHandler(void* userData, const XML_Char *prefix);
void expat_StartElementHandler(void* userData, const XML_Char* name, const XML_Char** atts);
void expat_EndElementHandler(void* userData, const XML_Char* name);
void expat_CharacterDataHandler(void* userData, const XML_Char* s, int len);


#endif

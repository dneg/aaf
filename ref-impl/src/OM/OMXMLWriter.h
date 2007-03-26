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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __OMXMLWRITER_H__
#define __OMXMLWRITER_H__


#include "OMDataTypes.h"
#include "OMWString.h"
#include "OMVector.h"
#include "OMSet.h"
#include "OMRawStorage.h"


#define OMXMLWRITER_DEFAULT_NAMESPACE_PREFIX  0
#define OMXMLWRITER_PARENT_ELEMENT_NAMESPACE  0


class OMXMLWriter
{
public:
    static OMXMLWriter* create(OMRawStorage* xmlStream);

    virtual ~OMXMLWriter(void) {}

    virtual void writeDocumentStart(void) = 0;
    virtual void writeDocumentEnd(void) = 0;
    virtual void writeElementStart(const wchar_t* ns, const wchar_t* localName) = 0;
    virtual void declareNamespace(const wchar_t* ns, const wchar_t* prefix) = 0;
    virtual void writeAttribute(const wchar_t* ns, const wchar_t* localName, const wchar_t* value) = 0;
    virtual void writeAttributeStart(const wchar_t* ns, const wchar_t* localName) = 0;
    virtual void writeAttributeContent(const wchar_t* value) = 0;
    virtual void writeAttributeEnd(void) = 0;
    virtual void writeElementContent(const wchar_t* content, OMUInt32 length) = 0;
    virtual void writeElementEnd(void) = 0;
    virtual void writeComment(const wchar_t* comment) = 0;
    virtual void writeProcInstruction(const wchar_t* target, const wchar_t* instruction) = 0;
    virtual void writeText(const wchar_t* text) = 0;

    virtual void synchronize(void) = 0;
    virtual void reset(void) = 0;
};



// simple implementation of the OMXMLWriter interface

// note: character escaping and formating is left up to the application for 
// writeComment and writeProcInstruction

class OMXMLWriterSimple : public OMXMLWriter
{
public:
    enum XMLUTF16ByteOrder {LE, BE};

    OMXMLWriterSimple(OMRawStorage* xmlStream);
    OMXMLWriterSimple(OMRawStorage* xmlStream, XMLUTF16ByteOrder byteOrder);
    virtual ~OMXMLWriterSimple(void);


    virtual void writeDocumentStart(void);
    virtual void writeDocumentEnd(void);
    virtual void writeElementStart(const wchar_t* ns, const wchar_t* localName);
    virtual void declareNamespace(const wchar_t* ns, const wchar_t* prefix);
    virtual void writeAttribute(const wchar_t* ns, const wchar_t* localName, const wchar_t* value);
    virtual void writeAttributeStart(const wchar_t* ns, const wchar_t* localName);
    virtual void writeAttributeContent(const wchar_t* value);
    virtual void writeAttributeEnd(void);
    virtual void writeElementContent(const wchar_t* content, OMUInt32 length);
    virtual void writeElementEnd(void);
    virtual void writeComment(const wchar_t* comment);
    virtual void writeProcInstruction(const wchar_t* target, const wchar_t* instruction);
    virtual void writeText(const wchar_t* text);

    virtual void synchronize(void);
    virtual void reset(void);

private:
    enum XMLEncoding {UTF8, UTF16};
    
    void initialise(OMRawStorage* xmlStream, XMLEncoding encoding, XMLUTF16ByteOrder byteOrder);
    
    const wchar_t* getPrefix(const wchar_t* ns);
    const wchar_t* getNonDefaultNSPrefix(const wchar_t* ns);

    class Element
    {
    public:
        Element(Element* parentElement, const wchar_t* ns, const wchar_t* localName);
        ~Element(void);

        bool addNamespaceDecl(const wchar_t* ns, const wchar_t* prefix);
        OMSet<OMWString, OMWString>* getNamespaceDecls(void);

        const wchar_t* getNamespace(void) const;
        const wchar_t* getLocalName(void) const;
        const wchar_t* getPrefix(const wchar_t* ns) const;
        const wchar_t* getNonDefaultNSPrefix(const wchar_t* ns) const;
        const wchar_t* getPrefix(void) const;
        const wchar_t* getDefaultNamespace(void) const;

    private:
        Element*            _parentElement;
        OMWString           _ns;
        OMWString           _prefix;
        OMWString           _localName;
        OMWString           _defaultNs;
        OMSet<OMWString, OMWString>   _nspaceDecls;
    };

    enum WriteType
    {
        NONE,
        START,
        END,
        ELEMENT_START,
        DELAYED_ELEMENT_START,
        ATTRIBUTE_START,
        ATTRIBUTE_CONTENT,
        ATTRIBUTE_END,
        ELEMENT_CONTENT,
        ELEMENT_END,
        COMMENT,
        PROC_INSTRUCTION
    };

    WriteType               _prevWriteType;
    OMVector<Element*>      _elementStack;
    int                     _level;


private:
    void writeProlog(void);
    void writeName(const wchar_t* data, OMUInt32 length);
    void writeElementData(const wchar_t* data, OMUInt32 length);
    void writeAttributeData(const wchar_t* data, OMUInt32 length);
    void writeCommentData(const wchar_t* data, OMUInt32 length);
    void writeProcInstrData(const wchar_t* data, OMUInt32 length);
    void writeIndent(int level);

    bool validName(const wchar_t* name);
    bool validNamespace(const wchar_t* ns);
    bool validPrefix(const wchar_t* prefix);

    void write(const wchar_t* data, OMUInt32 length);
    void writeRaw(const OMByte* bytes, OMUInt32 size);

private:
    OMRawStorage*       _xmlStream;
    XMLEncoding         _encoding;
    XMLUTF16ByteOrder   _byteOrder;
    bool                _swap;
};


#endif

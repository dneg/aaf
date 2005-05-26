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

#ifndef __XMLWRITER_H__
#define __XMLWRITER_H__

#if defined (_MSC_VER)
  // - 'identifier' : identifier was truncated to 'number' characters in
  //   the debug information
#pragma warning(disable:4786)
#endif

#include <OMDataTypes.h>
#include <vector>
#include <map>
#include <string>



#define XMLWRITER_DEFAULT_NAMESPACE_PREFIX  0
#define XMLWRITER_PARENT_ELEMENT_NAMESPACE  0


// note: character escaping and formating is left up to the application for 
// Comment and ProcInstruction

class XMLWriter
{
public:
    virtual ~XMLWriter(void)
    {}

    virtual void WriteDocumentStart(void) = 0;
    virtual void WriteDocumentEnd(void) = 0;
    virtual void WriteElementStart(const char* ns, const char* localName) = 0;
    virtual void DeclareNamespace(const char* ns, const char* prefix) = 0;
    virtual void WriteAttribute(const char* ns, const char* localName, const char* value) = 0;
    virtual void WriteAttributeStart(const char* ns, const char* localName) = 0;
    virtual void WriteAttributeContent(const char* value) = 0;
    virtual void WriteAttributeEnd(void) = 0;
    virtual void WriteElementContent(const char* content, size_t length) = 0;
    virtual void WriteElementEnd(void) = 0;
    virtual void WriteComment(const char* comment) = 0;
    virtual void WriteProcInstruction(const char* target, const char* instruction) = 0;
    virtual void WriteText(const char* text) = 0;

    virtual void Synchronize(void) = 0;
    virtual void Reset(void) = 0;
};


class XMLOStream
{
public:
    XMLOStream(void) {}
    virtual ~XMLOStream(void) {}
    
    virtual OMUInt32 Write(const OMByte* data, OMUInt32 count) = 0;
    virtual void Synchronize(void) = 0;
    virtual void SetPosition(OMUInt64 position) = 0;
};


class XMLWriterSimple : public XMLWriter
{
public:
    enum XMLUTF16ByteOrder
    {
        LE,
        BE
    };

    XMLWriterSimple(XMLOStream* xmlStream);
    XMLWriterSimple(XMLOStream* xmlStream, XMLUTF16ByteOrder byteOrder);
    virtual ~XMLWriterSimple(void);


    virtual void WriteDocumentStart(void);
    virtual void WriteDocumentEnd(void);
    virtual void WriteElementStart(const char* ns, const char* localName);
    virtual void DeclareNamespace(const char* ns, const char* prefix);
    virtual void WriteAttribute(const char* ns, const char* localName, const char* value);
    virtual void WriteAttributeStart(const char* ns, const char* localName);
    virtual void WriteAttributeContent(const char* value);
    virtual void WriteAttributeEnd(void);
    virtual void WriteElementContent(const char* content, size_t length);
    virtual void WriteElementEnd(void);
    virtual void WriteComment(const char* comment);
    virtual void WriteProcInstruction(const char* target, const char* instruction);
    virtual void WriteText(const char* text);

    virtual void Synchronize(void);
    virtual void Reset(void);

private:
    enum XMLEncoding
    {
        UTF8,
        UTF16
    };
    void Initialise(XMLOStream* xmlStream, XMLEncoding encoding, XMLUTF16ByteOrder byteOrder);
    
    std::string GetPrefix(const char* ns);
    std::string GetNonDefaultNSPrefix(const char* ns);

    class Element
    {
    public:
        Element(Element* parentElement, const char* ns, const char* localName);
        ~Element(void);

        bool AddNamespaceDecl(const char* ns, const char* prefix);
        std::map<std::string, std::string>& GetNamespaceDecls(void);

        std::string GetNamespace(void) const;
        std::string GetLocalName(void) const;
        std::string GetPrefix(const char* ns) const;
        std::string GetNonDefaultNSPrefix(const char* ns) const;
        std::string GetPrefix(void) const;
        std::string GetDefaultNamespace(void) const;

    private:
        Element*            _parentElement;
        std::string         _ns;
        std::string         _prefix;
        std::string         _localName;
        std::string         _defaultNs;
        std::map<std::string, std::string>   _nspaceDecls;
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
    std::vector<Element*>   _elementStack;
    int                     _level;


private:
    void WriteProlog(void);
    void WriteName(const char* data, size_t length);
    void WriteElementData(const char* data, size_t length);
    void WriteAttributeData(const char* data, size_t length);
    void WriteCommentData(const char* data, size_t length);
    void WriteProcInstrData(const char* data, size_t length);
    void WriteIndent(int level);

    bool ValidName(const char* name);
    bool ValidNamespace(const char* ns);
    bool ValidPrefix(const char* prefix);

    void Write(const char* data, size_t length);
    void WriteRaw(const OMByte* bytes, size_t size);

    void ToUTF16(const char* str, OMByte* utf16Bytes, size_t* utf16Len);
    
private:
    XMLOStream*         _xmlStream;
    XMLEncoding         _encoding;
    XMLUTF16ByteOrder   _byteOrder;
    bool                _swap;
};

#endif

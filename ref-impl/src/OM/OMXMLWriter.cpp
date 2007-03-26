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

#include "OMXMLWriter.h"
#include "OMXMLUtilities.h"
#include "OMVectorIterator.h"
#include "OMSetIterator.h"
#include "OMExceptions.h"
#include "OMAssertions.h"


static const wchar_t* g_internalDefaultNamespacePrefix = L" ";
static const wchar_t* g_emptyString = L"";


OMXMLWriter*
OMXMLWriter::create(OMRawStorage* xmlStream)
{
    TRACE("OMXMLWriter::create");
    
    return new OMXMLWriterSimple(xmlStream);
}


OMXMLWriterSimple::OMXMLWriterSimple(OMRawStorage* xmlStream)
{
    TRACE("OMXMLWriterSimple::OMXMLWriterSimple(OMRawStorage)");
    
    initialise(xmlStream, UTF8, LE);
}

OMXMLWriterSimple::OMXMLWriterSimple(OMRawStorage* xmlStream, XMLUTF16ByteOrder byteOrder)
{
    TRACE("OMXMLWriterSimple::OMXMLWriterSimple(OMRawStorage, XMLUTF16ByteOrder)");
    
    initialise(xmlStream, UTF16, byteOrder);
}

OMXMLWriterSimple::~OMXMLWriterSimple()
{
    TRACE("OMXMLWriterSimple::~OMXMLWriterSimple");
    
    OMUInt32 elementCount = _elementStack.count();
    for (OMUInt32 i = 0; i < elementCount; i++)
    {
        writeElementEnd();
    }
}

void
OMXMLWriterSimple::initialise(OMRawStorage* xmlStream, XMLEncoding encoding, 
    XMLUTF16ByteOrder byteOrder)
{
    TRACE("OMXMLWriterSimple::initialise");
    
    _encoding = encoding;
    _byteOrder = byteOrder;

    _swap = false;
    if (encoding == UTF16)
    {
        OMUInt16 test = 0xFFFE;
        OMByte firstByte = *((OMByte*)&test);
        if (firstByte == 0xFF && byteOrder == LE)
        {
            _swap = true;
        }
        else if (byteOrder == BE)
        {
            _swap = true;
        }
    }

    _prevWriteType = NONE;
    _xmlStream = xmlStream;
    _level = 0;
}

void 
OMXMLWriterSimple::writeDocumentStart(void)
{
    TRACE("OMXMLWriterSimple::writeDocumentStart");
    PRECONDITION("Valid state", _prevWriteType == NONE);
    
    writeProlog();
    
    _prevWriteType = START;
}

void 
OMXMLWriterSimple::writeDocumentEnd(void)
{
    TRACE("OMXMLWriterSimple::writeDocumentEnd");

    OMUInt32 elementCount = _elementStack.count();
    for (OMUInt32 i = 0; i < elementCount; i++)
    {
        writeElementEnd();
    }

    _prevWriteType = END;
}

void
OMXMLWriterSimple::writeElementStart(const wchar_t* ns, const wchar_t* localName)
{
    TRACE("OMXMLWriterSimple::writeElementStart");
    PRECONDITION("Valid state", _prevWriteType == START ||
        _prevWriteType == ELEMENT_START ||
        _prevWriteType == DELAYED_ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_CONTENT ||
        _prevWriteType == ELEMENT_END || 
        _prevWriteType == COMMENT ||
        _prevWriteType == PROC_INSTRUCTION);
    
    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        writeAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        write(L">\n", 2);
    }

    Element* parentElement = 0;
    if (_elementStack.count() > 0)
    {
        parentElement = _elementStack.getAt(_elementStack.count() - 1);
    }
    Element* element = new Element(parentElement, ns, localName);
    _elementStack.append(element);

    const wchar_t* prefix = element->getPrefix(ns);
    if (wcslen(prefix) == 0)
    {
        _prevWriteType = DELAYED_ELEMENT_START;
        return;
    }

    writeIndent(_level);
    write(L"<", 1);
    if (wcscmp(prefix, g_internalDefaultNamespacePrefix) != 0)
    {
        writeName(prefix, wcslen(prefix));
        write(L":", 1);
    }
    writeName(localName, wcslen(localName));

    _level++;
    _prevWriteType = ELEMENT_START;
}

void
OMXMLWriterSimple::declareNamespace(const wchar_t* ns, const wchar_t* prefix)
{
    TRACE("OMXMLWriterSimple::declareNamespace");
    PRECONDITION("Valid state", _prevWriteType == DELAYED_ELEMENT_START || 
        _prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END);
    PRECONDITION("Valid prefix", prefix == 0 || validPrefix(prefix));
    PRECONDITION("Valid namespace", validNamespace(ns));

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        writeAttributeEnd();
    }

    ASSERT("Elements count not zero", _elementStack.count() > 0);
    Element* element = _elementStack.getAt(_elementStack.count() - 1);
    bool wasAdded;
    if (prefix == OMXMLWRITER_DEFAULT_NAMESPACE_PREFIX)
    {
        wasAdded = element->addNamespaceDecl(ns, g_internalDefaultNamespacePrefix);
    }
    else
    {
        wasAdded = element->addNamespaceDecl(ns, prefix);
    }

    if (wasAdded)
    {
        if (_prevWriteType == DELAYED_ELEMENT_START)
        {
            ASSERT("Namespace declarations must start with the current element", 
                wcscmp(ns, element->getNamespace()) == 0);
            
            writeIndent(_level);
            write(L"<", 1);
            if (prefix != OMXMLWRITER_DEFAULT_NAMESPACE_PREFIX)
            {
                writeName(prefix, wcslen(prefix));
                write(L":", 1);
            }
            writeName(element->getLocalName(), wcslen(element->getLocalName()));

            _level++;
            _prevWriteType = ELEMENT_START;
        }

        write(L" xmlns", 6);
        if (prefix != OMXMLWRITER_DEFAULT_NAMESPACE_PREFIX)
        {
            write(L":",1);
            writeName(prefix, wcslen(prefix));
        }
        write(L"=\"", 2);
        writeAttributeData(ns, wcslen(ns));
        write(L"\"", 1);
    }
}

void
OMXMLWriterSimple::writeAttribute(const wchar_t* ns, const wchar_t* localName, const wchar_t* value)
{
    TRACE("OMXMLWriterSimple::writeAttribute");
    PRECONDITION("Valid state", _prevWriteType == ELEMENT_START || 
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        writeAttributeEnd();
    }

    write(L" ", 1);

    if (ns != 0)
    {
        const wchar_t* prefix = getNonDefaultNSPrefix(ns);
        ASSERT("Valid prefix", prefix != 0 && wcslen(prefix) > 0);
        writeName(prefix, wcslen(prefix));
        write(L":", 1);
    }

    writeName(localName, wcslen(localName));
    write(L"=\"", 2);
    writeAttributeData(value, wcslen(value));
    write(L"\"", 1);

    _prevWriteType = ATTRIBUTE_END;
}

void 
OMXMLWriterSimple::writeAttributeStart(const wchar_t* ns, const wchar_t* localName)
{
    TRACE("OMXMLWriterSimple::writeAttributeStart");
    PRECONDITION("Valid state", _prevWriteType == ELEMENT_START || 
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END);
        
    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        writeAttributeEnd();
    }

    write(L" ", 1);

    if (ns != 0)
    {
        const wchar_t* prefix = getNonDefaultNSPrefix(ns);
        ASSERT("Valid prefix", prefix != 0 && wcslen(prefix) > 0);
        writeName(prefix, wcslen(prefix));
        write(L":", 1);
    }

    writeName(localName, wcslen(localName));
    write(L"=\"", 2);

    _prevWriteType = ATTRIBUTE_START;
}

void 
OMXMLWriterSimple::writeAttributeContent(const wchar_t* value)
{
    TRACE("OMXMLWriterSimple::writeAttributeContent");
    PRECONDITION("Valid state", _prevWriteType == ATTRIBUTE_START || 
        _prevWriteType == ATTRIBUTE_CONTENT);
    
    writeAttributeData(value, wcslen(value));

    _prevWriteType = ATTRIBUTE_CONTENT;
}

void 
OMXMLWriterSimple::writeAttributeEnd(void)
{
    TRACE("OMXMLWriterSimple::writeAttributeEnd");
    PRECONDITION("Valid state", _prevWriteType == ATTRIBUTE_START || 
        _prevWriteType == ATTRIBUTE_CONTENT);
    
    write(L"\"", 1);

    _prevWriteType = ATTRIBUTE_END;
}

void
OMXMLWriterSimple::writeElementContent(const wchar_t* content, OMUInt32 length)
{
    TRACE("OMXMLWriterSimple::writeElementContent");
    PRECONDITION("Valid state", _prevWriteType == ELEMENT_START ||
        _prevWriteType == ELEMENT_END ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_CONTENT);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        writeAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        write(L">", 1);
    }

    writeElementData(content, length);

    _prevWriteType = ELEMENT_CONTENT;
}

void
OMXMLWriterSimple::writeElementEnd()
{
    TRACE("OMXMLWriterSimple::writeElementEnd");
    PRECONDITION("Valid state", _prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_CONTENT ||
        _prevWriteType == ELEMENT_END || 
        _prevWriteType == COMMENT ||
        _prevWriteType == PROC_INSTRUCTION);

    _level--;

    ASSERT("Elements count not zero", _elementStack.count() > 0);
    Element* element = _elementStack.getAt(_elementStack.count() - 1);
    const wchar_t* prefix = element->getPrefix();
    ASSERT("Valid prefix", prefix != 0 && wcslen(prefix) > 0);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        writeAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        write(L"/>\n", 3);
    }
    else
    {
        if (_prevWriteType != ELEMENT_CONTENT)
        {
            if (_prevWriteType != ELEMENT_END && 
                _prevWriteType != COMMENT &&
                _prevWriteType != PROC_INSTRUCTION)
            {
                write(L"\n", 1);
            }
            writeIndent(_level);
        }

        write(L"</", 2);
        if (wcscmp(prefix, g_internalDefaultNamespacePrefix) != 0)
        {
            writeName(prefix, wcslen(prefix));
            write(L":", 1);
        }
        writeName(element->getLocalName(), wcslen(element->getLocalName()));
        write(L">\n", 2);

    }

    _elementStack.removeLast();
    delete element;

    _prevWriteType = ELEMENT_END;
}

void
OMXMLWriterSimple::writeComment(const wchar_t* comment)
{
    TRACE("OMXMLWriterSimple::writeComment");
    PRECONDITION("Valid state", _prevWriteType == START ||
        _prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_END || 
        _prevWriteType == COMMENT ||
        _prevWriteType == PROC_INSTRUCTION);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        writeAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        write(L">\n", 2);
    }

    writeIndent(_level);
    write(L"<!--", 4);
    writeCommentData(comment, wcslen(comment));
    write(L"-->\n", 4);

    if (_prevWriteType != ELEMENT_END && _prevWriteType != START)
    {
        _prevWriteType = COMMENT;
    }
}

void
OMXMLWriterSimple::writeProcInstruction(const wchar_t* target, const wchar_t* instruction)
{
    TRACE("OMXMLWriterSimple::writeProcInstruction");
    PRECONDITION("Valid state", _prevWriteType == START ||
        _prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_END || 
        _prevWriteType == COMMENT ||
        _prevWriteType == PROC_INSTRUCTION);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        writeAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        write(L">\n", 2);
    }

    writeIndent(_level);
    write(L"<?", 2);
    writeName(target, wcslen(target));
    write(L" ", 1);
    writeProcInstrData(instruction, wcslen(instruction));
    write(L"?>\n", 3);

    if (_prevWriteType != ELEMENT_END && _prevWriteType != START)
    {
        _prevWriteType = PROC_INSTRUCTION;
    }
}

void 
OMXMLWriterSimple::writeText(const wchar_t* text)
{
    TRACE("OMXMLWriterSimple::writeText");

    write(text, wcslen(text));
}

void
OMXMLWriterSimple::synchronize(void)
{
    TRACE("OMXMLWriterSimple::synchronize");

    _xmlStream->synchronize();
}

void
OMXMLWriterSimple::reset(void)
{
    TRACE("OMXMLWriterSimple::reset");

    OMVectorIterator<Element*> iter(_elementStack, OMBefore);
    while (++iter)
    {
        delete iter.value();
    }
    _elementStack.clear();
    
    _prevWriteType = NONE;
    _level = 0;

    _xmlStream->setPosition(0);
}

const wchar_t*
OMXMLWriterSimple::getPrefix(const wchar_t* ns)
{
    TRACE("OMXMLWriterSimple::getPrefix");

    if (_elementStack.count() == 0)
    {
        return g_emptyString;
    }

    Element* element = _elementStack.getAt(_elementStack.count() - 1);
    return element->getPrefix(ns);
}

const wchar_t*
OMXMLWriterSimple::getNonDefaultNSPrefix(const wchar_t* ns)
{
    TRACE("OMXMLWriterSimple::getNonDefaultNSPrefix");

    if (_elementStack.count() == 0)
    {
        return g_emptyString;
    }

    Element* element = _elementStack.getAt(_elementStack.count() - 1);
    return element->getNonDefaultNSPrefix(ns);
}


OMXMLWriterSimple::Element::Element(Element* parentElement, const wchar_t* ns, const wchar_t* localName)
: _parentElement(parentElement), _ns(ns), _localName(localName)
{
    TRACE("OMXMLWriterSimple::Element::Element");
    
    if (parentElement != 0)
    {
        OMSetIterator<OMWString, OMWString> iter(*(parentElement->getNamespaceDecls()), OMBefore);
        while (++iter)
        {
            _nspaceDecls.insert(iter.key(), iter.value());
        }
        _defaultNs = parentElement->getDefaultNamespace();
    }
    _prefix = getPrefix(ns);
}

OMXMLWriterSimple::Element::~Element()
{
    TRACE("OMXMLWriterSimple::Element::~Element");
}

bool
OMXMLWriterSimple::Element::addNamespaceDecl(const wchar_t* ns, const wchar_t* prefix)
{
    TRACE("OMXMLWriterSimple::Element::addNamespaceDecl");
    PRECONDITION("Valid namespace", ns != 0);
    PRECONDITION("Valid prefix", prefix != 0);
    
    if (wcscmp(prefix, g_internalDefaultNamespacePrefix) == 0)
    {
        _defaultNs = ns;

        if (_ns.compare(ns) == 0)
        {
            _prefix = prefix;
        }
    }
    else
    {
        if (_nspaceDecls.contains(ns))
        {
            OMWString* value;
            _nspaceDecls.find(ns, &value);
            if (value->compare(prefix) == 0)
            {
                // decl was already there so it was not added
                return false;
            }
            _nspaceDecls.remove(ns);
        }
        _nspaceDecls.insert(ns, prefix);

        if (_ns.compare(ns) == 0 && _defaultNs.compare(ns) != 0)
        {
            _prefix = prefix;
        }
    }    
    
    return true;
}

OMSet<OMWString, OMWString>* 
OMXMLWriterSimple::Element::getNamespaceDecls(void)
{
    TRACE("OMXMLWriterSimple::Element::getNamespaceDecls");
    
    return &_nspaceDecls;
}

const wchar_t*
OMXMLWriterSimple::Element::getNamespace() const
{
    TRACE("OMXMLWriterSimple::Element::getNamespace");
    
    return _ns.c_str();
}

const wchar_t*
OMXMLWriterSimple::Element::getLocalName() const
{
    TRACE("OMXMLWriterSimple::Element::getLocalName");
    
    return _localName.c_str();
}

const wchar_t* 
OMXMLWriterSimple::Element::getPrefix(const wchar_t* ns) const
{
    TRACE("OMXMLWriterSimple::Element::getPrefix");
    
    if (_defaultNs.compare(ns) == 0)
    {
        return g_internalDefaultNamespacePrefix;
    }
    else
    {
        return getNonDefaultNSPrefix(ns);
    }
}

const wchar_t* 
OMXMLWriterSimple::Element::getNonDefaultNSPrefix(const wchar_t* ns) const
{
    TRACE("OMXMLWriterSimple::Element::getNonDefaultNSPrefix");
    
    if (!_nspaceDecls.contains(ns))
    {
        return g_emptyString;
    }
    
    OMWString* prefix = 0;
    _nspaceDecls.find(ns, &prefix);
    return prefix->c_str();
}

const wchar_t*  
OMXMLWriterSimple::Element::getPrefix(void) const
{
    TRACE("OMXMLWriterSimple::Element::getPrefix");
    
    return _prefix.c_str();
}


const wchar_t* 
OMXMLWriterSimple::Element::getDefaultNamespace() const
{
    TRACE("OMXMLWriterSimple::Element::getDefaultNamespace");
    
    return _defaultNs.c_str();
}


void
OMXMLWriterSimple::writeProlog(void)
{
    TRACE("OMXMLWriterSimple::writeProlog");
    
    if (_encoding == UTF16)
    {
        OMUInt8 bom[2];
        if (_byteOrder == BE)
        {
            bom[0] = 0xFE;
            bom[1] = 0xFF;
        }
        else
        {
            bom[0] = 0xFF;
            bom[1] = 0xFE;
        }
        writeRaw(bom, 2);
    }

    write(L"<?xml version=\"1.0\" encoding=\"", 30);
    if (_encoding == UTF8)
    {
        write(L"UTF-8\"?>\n", 9);
    }
    else
    {
        write(L"UTF-16\"?>\n", 10);
    }
}

void
OMXMLWriterSimple::writeName(const wchar_t* data, OMUInt32 length)
{
    TRACE("OMXMLWriterSimple::writeName");
    
    write(data, length);
}

void
OMXMLWriterSimple::writeElementData(const wchar_t* data, OMUInt32 length)
{
    TRACE("OMXMLWriterSimple::writeElementData");
    
    const wchar_t* lt = L"&lt;";
    const wchar_t* gt = L"&gt;";
    const wchar_t* amp = L"&amp;";
    const wchar_t* lf = L"&#x0D;";
    
    wchar_t buffer[1024];
    unsigned int bufferPos = 0;
    const wchar_t* dataPtr = data;
    for (OMUInt32 i = 0; i<=length; i++)
    {
        if (bufferPos > 1017)
        {
            buffer[bufferPos] = L'\0';
            write(buffer, bufferPos);
            bufferPos = 0;
        }
        
        if (utf16CodeLen(dataPtr) < 0)
        {
            buffer[bufferPos] = *dataPtr;
            bufferPos++;
        }
        else if (utf16CodeLen(dataPtr) == 1)
        {
            if (*dataPtr == L'>')
            {
                memcpy(&(buffer[bufferPos]), gt, 4 * sizeof(wchar_t));
                bufferPos += 4;
            }
            else if (*dataPtr == L'<')
            {
                memcpy(&(buffer[bufferPos]), lt, 4 * sizeof(wchar_t));
                bufferPos += 4;
            } 
            else if (*dataPtr == L'&')
            {
                memcpy(&(buffer[bufferPos]), amp, 5 * sizeof(wchar_t));
                bufferPos += 5;
            }
            else if (*dataPtr == 0x0D)
            {
                memcpy(&(buffer[bufferPos]), lf, 6 * sizeof(wchar_t));
                bufferPos += 6;
            }
            else
            {
                buffer[bufferPos] = *dataPtr;
                bufferPos++;
            }
        }
        else // code len == 2
        {
            buffer[bufferPos] = *dataPtr;
            bufferPos++;
            dataPtr++;
            buffer[bufferPos] = *dataPtr;
            bufferPos++;
        }
        dataPtr++;
    }
    if (bufferPos > 1)
    {
         buffer[bufferPos] = L'\0';
         write(buffer, bufferPos - 1);
    }
}

void
OMXMLWriterSimple::writeAttributeData(const wchar_t* data, OMUInt32 length)
{
    TRACE("OMXMLWriterSimple::writeAttributeData");
    
    const wchar_t* amp = L"&amp;";
    const wchar_t* quot = L"&quot;";
    const wchar_t* apos = L"&apos;";
    const wchar_t* lf = L"&#x0D;";
    
    wchar_t buffer[1024];
    unsigned int bufferPos = 0;
    const wchar_t* dataPtr = data;
    for (OMUInt32 i = 0; i<=length; i++)
    {
        if (bufferPos > 1017)
        {
            buffer[bufferPos] = L'\0';
            write(buffer, bufferPos);
            bufferPos = 0;
        }
        
        if (utf16CodeLen(dataPtr) < 0)
        {
            buffer[bufferPos] = *dataPtr;
            bufferPos++;
        }
        else if (utf16CodeLen(dataPtr) == 1)
        {
            if (*dataPtr == L'\"')
            {
                memcpy(&(buffer[bufferPos]), quot, 6 * sizeof(wchar_t));
                bufferPos += 6;
            }
            else if (*dataPtr == L'\'')
            {
                memcpy(&(buffer[bufferPos]), apos, 6 * sizeof(wchar_t));
                bufferPos += 6;
            }
            else if (*dataPtr == L'&')
            {
                memcpy(&(buffer[bufferPos]), amp, 5 * sizeof(wchar_t));
                bufferPos += 5;
            }
            else if (*dataPtr == 0x0D)
            {
                memcpy(&(buffer[bufferPos]), lf, 6 * sizeof(wchar_t));
                bufferPos += 6;
            }
            else
            {
                buffer[bufferPos] = *dataPtr;
                bufferPos++;
            }
        }
        else // code len == 2
        {
            buffer[bufferPos] = *dataPtr;
            bufferPos++;
            dataPtr++;
            buffer[bufferPos] = *dataPtr;
            bufferPos++;
        }
        dataPtr++;
    }
    if (bufferPos > 1)
    {
         buffer[bufferPos] = L'\0';
         write(buffer, bufferPos - 1);
    }
}

void
OMXMLWriterSimple::writeCommentData(const wchar_t* data, OMUInt32 length)
{
    TRACE("OMXMLWriterSimple::writeCommentData");
    
    write(data, length);
}

void
OMXMLWriterSimple::writeProcInstrData(const wchar_t* data, OMUInt32 length)
{
    TRACE("OMXMLWriterSimple::writeProcInstrData");
    
    write(data, length);
}

void
OMXMLWriterSimple::writeIndent(int level)
{
    TRACE("OMXMLWriterSimple::writeIndent");
    PRECONDITION("Valid level", level >= 0);

    for (int i=0; i<level; i++)
    {
        write(L"  ", 2);
    }
}

bool
OMXMLWriterSimple::validName(const wchar_t* name)
{
    TRACE("OMXMLWriterSimple::validName");

    return (name != 0);
}

bool
OMXMLWriterSimple::validNamespace(const wchar_t* ns)
{
    TRACE("OMXMLWriterSimple::validNamespace");

    return (ns != 0);
}

bool
OMXMLWriterSimple::validPrefix(const wchar_t* prefix)
{
    TRACE("OMXMLWriterSimple::validPrefix");

    return (prefix != 0);
}

void
OMXMLWriterSimple::write(const wchar_t* data, OMUInt32 length)
{
    TRACE("OMXMLWriterSimple::write");

    if (_encoding == UTF8)
    {
        char* utf8Data = utf16ToUTF8(data);
        OMUInt32 numWritten;
        _xmlStream->write((const OMByte*)utf8Data, strlen(utf8Data), numWritten);
        delete [] utf8Data;
    }
    else
    {
        OMUInt16* buffer = new OMUInt16[length];
        
        for (OMUInt32 i = 0; i < length; i++)
        {
            buffer[i] = (OMUInt16)data[i];
            if (_swap)
            {
                OMByte* bytes = (OMByte*)&buffer[i];
                OMByte tmp = bytes[0];
                bytes[0] = bytes[1];
                bytes[1] = tmp;
            }
        }
        OMUInt32 numWritten;
        _xmlStream->write((OMByte*)buffer, length * sizeof(OMUInt16), numWritten);
        
        delete [] buffer;
    }
}

void
OMXMLWriterSimple::writeRaw(const OMByte* bytes, OMUInt32 size)
{
    TRACE("OMXMLWriterSimple::writeRaw");

    OMUInt32 numWritten;
    _xmlStream->write(bytes, size, numWritten);
}





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

#include "XMLWriter.h"
#include <cassert>

using namespace std;


static const char* g_internalDefaultNamespacePrefix = " ";


XMLWriterSimple::XMLWriterSimple(XMLOStream* xmlFile)
{
    Initialise(xmlFile, UTF8, LE);
}

XMLWriterSimple::XMLWriterSimple(XMLOStream* xmlFile, XMLUTF16ByteOrder byteOrder)
{
    Initialise(xmlFile, UTF16, byteOrder);
}

void
XMLWriterSimple::Initialise(XMLOStream* xmlFile, XMLEncoding encoding, 
    XMLUTF16ByteOrder byteOrder)
{
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
    _xmlStream = xmlFile;
    _level = 0;
}

XMLWriterSimple::~XMLWriterSimple()
{
    vector<Element*>::reverse_iterator iter;
    for (iter = _elementStack.rbegin(); iter != _elementStack.rend(); iter++)
    {
        WriteElementEnd();
    }
    
    if (_xmlStream != 0)
    {
        delete _xmlStream;
    }
}

void 
XMLWriterSimple::WriteDocumentStart(void)
{
    assert(_prevWriteType == NONE);
    
    WriteProlog();
    
    _prevWriteType = START;
}

void 
XMLWriterSimple::WriteDocumentEnd(void)
{
    vector<Element*>::reverse_iterator iter;
    for (iter = _elementStack.rbegin(); iter != _elementStack.rend(); iter++)
    {
        WriteElementEnd();
    }
    _elementStack.erase(_elementStack.begin(), _elementStack.end());

    _prevWriteType = END;
}

void
XMLWriterSimple::WriteElementStart(const char* ns, const char* localName)
{
    assert(_prevWriteType == START ||
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
        WriteAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        Write(">\n", 2);
    }
    else if (_prevWriteType == ELEMENT_CONTENT ||
             _prevWriteType == COMMENT ||
             _prevWriteType == PROC_INSTRUCTION)
    {
        WriteElementEnd();
    }

    Element* parentElement = 0;
    if (_elementStack.size() > 0)
    {
        parentElement = _elementStack.back();
    }
    Element* element = new Element(parentElement, ns, localName);
    _elementStack.push_back(element);

    string prefix = element->GetPrefix(ns);
    if (prefix.length() == 0)
    {
        _prevWriteType = DELAYED_ELEMENT_START;
        return;
    }

    WriteIndent(_level);
    Write("<", 1);
    if (prefix.length() != 0 && prefix.compare(g_internalDefaultNamespacePrefix) != 0)
    {
        WriteName(prefix.c_str(), prefix.length());
        Write(":", 1);
    }
    WriteName(localName, strlen(localName));

    _level++;
    _prevWriteType = ELEMENT_START;
}

void
XMLWriterSimple::DeclareNamespace(const char* ns, const char* prefix)
{
    assert(_prevWriteType == DELAYED_ELEMENT_START || 
        _prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END);
    assert(prefix == 0 || ValidPrefix(prefix));
    assert(ValidNamespace(ns));

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        WriteAttributeEnd();
    }

    assert(_elementStack.size() > 0);
    Element* element = _elementStack.back();
    bool wasAdded;
    if (prefix == 0)
    {
        wasAdded = element->AddNamespaceDecl(ns, " ");
    }
    else
    {
        wasAdded = element->AddNamespaceDecl(ns, prefix);
    }

    if (wasAdded)
    {
        if (_prevWriteType == DELAYED_ELEMENT_START)
        {
            WriteIndent(_level);
            Write("<", 1);
            if (prefix != XMLWRITER_DEFAULT_NAMESPACE_PREFIX)
            {
                WriteName(prefix, strlen(prefix));
                Write(":", 1);
            }
            WriteName(element->GetLocalName().c_str(), element->GetLocalName().length());

            _level++;
            _prevWriteType = ELEMENT_START;
        }

        Write(" xmlns", 6);
        if (prefix != XMLWRITER_DEFAULT_NAMESPACE_PREFIX)
        {
            Write(":",1);
            WriteName(prefix, strlen(prefix));
        }
        Write("=\"", 2);
        WriteAttributeData(ns, strlen(ns));
        Write("\"", 1);
    }

}

void
XMLWriterSimple::WriteAttribute(const char* ns, const char* localName, const char* value)
{
    assert(_prevWriteType == ELEMENT_START || 
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        WriteAttributeEnd();
    }

    Write(" ", 1);

    if (ns != 0)
    {
        string prefix = GetNonDefaultNSPrefix(ns);
        assert(prefix.length() > 0);
        WriteName(prefix.c_str(), prefix.length());
        Write(":", 1);
    }

    WriteName(localName, strlen(localName));
    Write("=\"", 2);
    WriteAttributeData(value, strlen(value));
    Write("\"", 1);

    _prevWriteType = ATTRIBUTE_END;
}

void 
XMLWriterSimple::WriteAttributeStart(const char* ns, const char* localName)
{
    assert(_prevWriteType == ELEMENT_START || 
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END);
        
    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        WriteAttributeEnd();
    }

    Write(" ", 1);

    if (ns != 0)
    {
        string prefix = GetNonDefaultNSPrefix(ns);
        assert(prefix.length() > 0);
        WriteName(prefix.c_str(), prefix.length());
        Write(":", 1);
    }

    WriteName(localName, strlen(localName));
    Write("=\"", 2);

    _prevWriteType = ATTRIBUTE_START;
}

void 
XMLWriterSimple::WriteAttributeContent(const char* value)
{
    assert(_prevWriteType == ATTRIBUTE_START || 
        _prevWriteType == ATTRIBUTE_CONTENT);
    
    WriteAttributeData(value, strlen(value));

    _prevWriteType = ATTRIBUTE_CONTENT;
}

void 
XMLWriterSimple::WriteAttributeEnd(void)
{
    assert(_prevWriteType == ATTRIBUTE_START || 
        _prevWriteType == ATTRIBUTE_CONTENT);
    
    Write("\"", 1);

    _prevWriteType = ATTRIBUTE_END;
}

void
XMLWriterSimple::WriteElementContent(const char* content, size_t length)
{
    assert(_prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_CONTENT);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        WriteAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        Write(">", 1);
    }

    WriteElementData(content, length);

    _prevWriteType = ELEMENT_CONTENT;
}

void
XMLWriterSimple::WriteElementEnd()
{
    assert(_prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_CONTENT ||
        _prevWriteType == ELEMENT_END || 
        _prevWriteType == COMMENT ||
        _prevWriteType == PROC_INSTRUCTION);

    _level--;

    assert(_elementStack.size() > 0);
    Element* element = _elementStack.back();
    string prefix = element->GetPrefix();
    assert(prefix.length() != 0);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        WriteAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        Write("/>\n", 3);
    }
    else
    {
        if (_prevWriteType != ELEMENT_CONTENT)
        {
            if (_prevWriteType != ELEMENT_END && 
                _prevWriteType != COMMENT &&
                _prevWriteType != PROC_INSTRUCTION)
            {
                Write("\n", 1);
            }
            WriteIndent(_level);
        }

        Write("</", 2);
        if (prefix.compare(g_internalDefaultNamespacePrefix) != 0)
        {
            WriteName(prefix.c_str(), prefix.length());
            Write(":", 1);
        }
        WriteName(element->GetLocalName().c_str(), element->GetLocalName().length());
        Write(">\n", 2);

    }

    _elementStack.pop_back();
    delete element;

    _prevWriteType = ELEMENT_END;
}

void
XMLWriterSimple::WriteComment(const char* comment)
{
    assert(_prevWriteType == START ||
        _prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_END || 
        _prevWriteType == COMMENT ||
        _prevWriteType == PROC_INSTRUCTION);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        WriteAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        Write(">\n", 2);
    }

    WriteIndent(_level);
    Write("<!--", 4);
    WriteCommentData(comment, strlen(comment));
    Write("-->\n", 4);

    if (_prevWriteType != ELEMENT_END && _prevWriteType != START)
    {
        _prevWriteType = COMMENT;
    }
}

void
XMLWriterSimple::WriteProcInstruction(const char* target, const char* instruction)
{
    assert(_prevWriteType == START ||
        _prevWriteType == ELEMENT_START ||
        _prevWriteType == ATTRIBUTE_START ||
        _prevWriteType == ATTRIBUTE_CONTENT ||
        _prevWriteType == ATTRIBUTE_END ||
        _prevWriteType == ELEMENT_END || 
        _prevWriteType == COMMENT ||
        _prevWriteType == PROC_INSTRUCTION);

    if (_prevWriteType == ATTRIBUTE_START || _prevWriteType == ATTRIBUTE_CONTENT)
    {
        WriteAttributeEnd();
    }

    if (_prevWriteType == ELEMENT_START || _prevWriteType == ATTRIBUTE_END)
    {
        Write(">\n", 2);
    }

    WriteIndent(_level);
    Write("<?", 2);
    WriteName(target, strlen(target));
    Write(" ", 1);
    WriteProcInstrData(instruction, strlen(instruction));
    Write("?>\n", 3);

    if (_prevWriteType != ELEMENT_END && _prevWriteType != START)
    {
        _prevWriteType = PROC_INSTRUCTION;
    }
}

void 
XMLWriterSimple::WriteText(const char* text)
{
    Write(text, strlen(text));
}

void
XMLWriterSimple::Synchronize(void)
{
    _xmlStream->Synchronize();
}

void
XMLWriterSimple::Reset(void)
{
    vector<Element*>::iterator iter;
    for (iter = _elementStack.begin(); iter != _elementStack.end(); iter++)
    {
        delete *iter;
    }
    _elementStack.clear();
    
    _prevWriteType = NONE;
    _level = 0;

    _xmlStream->SetPosition(0);
}

string
XMLWriterSimple::GetPrefix(const char* ns)
{
    if (_elementStack.size() == 0)
    {
        return "";
    }

    Element* element = _elementStack.back();
    return element->GetPrefix(ns);
}

string
XMLWriterSimple::GetNonDefaultNSPrefix(const char* ns)
{
    if (_elementStack.size() == 0)
    {
        return "";
    }

    Element* element = _elementStack.back();
    return element->GetNonDefaultNSPrefix(ns);
}


XMLWriterSimple::Element::Element(Element* parentElement, const char* ns, const char* localName)
: _parentElement(parentElement), _ns(ns), _localName(localName)
{
    if (parentElement != 0)
    {
        _nspaceDecls = parentElement->GetNamespaceDecls();
        _defaultNs = parentElement->GetDefaultNamespace();
    }
    _prefix = GetPrefix(ns);
}

XMLWriterSimple::Element::~Element()
{}

bool
XMLWriterSimple::Element::AddNamespaceDecl(const char* ns, const char* prefix)
{
    assert(ns != 0);
    assert(prefix != 0);
    
    if (strcmp(prefix, g_internalDefaultNamespacePrefix) == 0)
    {
        _defaultNs = ns;

        if (_ns.compare(ns) == 0)
        {
            _prefix = prefix;
        }
    }
    else
    {
        pair<map<string, string>::iterator, bool> result = 
            _nspaceDecls.insert(pair<string, string>(ns, prefix));
        if (!result.second)
        {
            if ((*(result.first)).first.compare(ns) == 0 && 
                (*(result.first)).second.compare(prefix) == 0)
            {
                return false;
            }
            _nspaceDecls.erase(result.first);
            _nspaceDecls.insert(pair<string, string>(ns, prefix));
        }

        if (_ns.compare(ns) == 0 && _defaultNs.compare(ns) != 0)
        {
            _prefix = prefix;
        }
    }    
    
    
    return true;
}

map<string, string>& 
XMLWriterSimple::Element::GetNamespaceDecls(void)
{
    return _nspaceDecls;
}

string
XMLWriterSimple::Element::GetNamespace() const
{
    return _ns;
}

string
XMLWriterSimple::Element::GetLocalName() const
{
    return _localName;
}

string 
XMLWriterSimple::Element::GetPrefix(const char* ns) const
{
    if (_defaultNs.compare(ns) == 0)
    {
        return g_internalDefaultNamespacePrefix;
    }
    else
    {
        map<string, string>::const_iterator result = _nspaceDecls.find(ns);
        if (result == _nspaceDecls.end())
        {
            return "";
        }
        else
        {
            return (*result).second;
        }
    }
}

string 
XMLWriterSimple::Element::GetNonDefaultNSPrefix(const char* ns) const
{
    map<string, string>::const_iterator result = _nspaceDecls.find(ns);
    if (result == _nspaceDecls.end())
    {
        return "";
    }
    else
    {
        return (*result).second;
    }
}

string 
XMLWriterSimple::Element::GetPrefix(void) const
{
    return _prefix;
}


string
XMLWriterSimple::Element::GetDefaultNamespace() const
{
    return _defaultNs;
}

void
XMLWriterSimple::WriteProlog(void)
{
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
        WriteRaw(bom, 2);
    }

    Write("<?xml version=\"1.0\" encoding=\"", 30);
    if (_encoding == UTF8)
    {
        Write("UTF-8\"?>\n", 9);
    }
    else
    {
        Write("UTF-16\"?>\n", 10);
    }
}

void
XMLWriterSimple::WriteName(const char* data, size_t length)
{
    Write(data, length);
}

void
XMLWriterSimple::WriteElementData(const char* data, size_t length)
{
    const char* lt = "&lt;";
    const char* gt = "&gt;";
    const char* amp = "&amp;";
    const char* lf = "&#x0D;";
    
    char buffer[1024];
    unsigned int bufferPos = 0;
    const char* dataPtr = data;
    for (size_t i = 0; i<=length; i++)
    {
        if (bufferPos > 1019)
        {
            Write(buffer, bufferPos);
            bufferPos = 0;
        }
        
        if (*dataPtr == '>')
        {
            memcpy(&(buffer[bufferPos]), gt, 4);
            bufferPos += 4;
        }
        else if (*dataPtr == '<')
        {
            memcpy(&(buffer[bufferPos]), lt, 4);
            bufferPos += 4;
        } 
        else if (*dataPtr == '&')
        {
            memcpy(&(buffer[bufferPos]), amp, 5);
            bufferPos += 5;
        }
        else if (*dataPtr == 0x0D)
        {
            memcpy(&(buffer[bufferPos]), lf, 6);
            bufferPos += 6;
        }
        else
        {
            buffer[bufferPos] = *dataPtr;
            bufferPos++;
        }
        dataPtr++;
    }
    if (bufferPos > 1)
    {
         Write(buffer, bufferPos - 1);
    }
}

void
XMLWriterSimple::WriteAttributeData(const char* data, size_t length)
{
    const char* amp = "&amp;";
    const char* quot = "&quot;";
    const char* apos = "&apos;";
    const char* lf = "&#x0D;";
    
    char buffer[1024];
    unsigned int bufferPos = 0;
    const char* dataPtr = data;
    for (size_t i = 0; i<=length; i++)
    {
        if (bufferPos > 1018)
        {
            Write(buffer, bufferPos);
            bufferPos = 0;
        }
        
        if (*dataPtr == '\"')
        {
            memcpy(&(buffer[bufferPos]), quot, 6);
            bufferPos += 6;
        }
        else if (*dataPtr == '\'')
        {
            memcpy(&(buffer[bufferPos]), apos, 6);
            bufferPos += 6;
        }
        else if (*dataPtr == '&')
        {
            memcpy(&(buffer[bufferPos]), amp, 5);
            bufferPos += 5;
        }
        else if (*dataPtr == 0x0D)
        {
            memcpy(&(buffer[bufferPos]), lf, 6);
            bufferPos += 6;
        }
        else
        {
            buffer[bufferPos] = *dataPtr;
            bufferPos++;
        }
        dataPtr++;
    }
    if (bufferPos > 1)
    {
         Write(buffer, bufferPos - 1);
    }
}

void
XMLWriterSimple::WriteCommentData(const char* data, size_t length)
{
    Write(data, length);
}

void
XMLWriterSimple::WriteProcInstrData(const char* data, size_t length)
{
    Write(data, length);
}

void
XMLWriterSimple::WriteIndent(int level)
{
    assert(level >= 0);

    for (int i=0; i<level; i++)
    {
        Write("  ", 2);
    }
}

bool
XMLWriterSimple::ValidName(const char* name)
{
    return (name != 0);
}

bool
XMLWriterSimple::ValidNamespace(const char* ns)
{
    return (ns != 0);
}

bool
XMLWriterSimple::ValidPrefix(const char* prefix)
{
    return (prefix != 0);
}

void
XMLWriterSimple::Write(const char* data, size_t length)
{
    if (_encoding == UTF8)
    {
        _xmlStream->Write((const OMByte*)data, length);
    }
    else
    {
        OMByte* buffer = new OMByte[(length + 1) * 2];
        size_t utf16Len;
        ToUTF16(data, buffer, &utf16Len);
        if (_swap)
        {
            size_t i;
            for (i = 0; i<utf16Len; i++)
            {
                OMByte tmp;
                tmp = buffer[2 * i];
                buffer[2 * i] = buffer[2 * i + 1];
                buffer[2 * i + 1] = tmp;
            }
        }
        _xmlStream->Write(buffer, utf16Len);
        
        delete [] buffer;
    }
}

void
XMLWriterSimple::WriteRaw(const OMByte* bytes, size_t size)
{
    _xmlStream->Write(bytes, size);
}

void 
XMLWriterSimple::ToUTF16(const char* str, OMByte* utf16Bytes, size_t* utf16Len)
{
    const unsigned char* strPtr = (unsigned char*)str;
    OMUInt16* utf16StrPtr = (OMUInt16*)utf16Bytes;
    while (*strPtr != 0)
    {
        if (*strPtr < 0x80)
        {
            *utf16StrPtr = (OMUInt16)*strPtr;
            utf16StrPtr++;
            strPtr++;
        }
        else if ((*strPtr & 0xE0) == 0xC0)
        {
            if ((*(strPtr + 1) & 0xC0) == 0x80)
            {
                OMUInt16 c = (((OMUInt16)*strPtr) & 0x1F) << 6;
                strPtr++;
                c |= (((OMUInt16)*strPtr) & 0x3F);
                strPtr++;
                *utf16StrPtr = c;
                utf16StrPtr++;
            }
            else
            {
                assert(false);
            }
        }
        else if ((*strPtr & 0xF0) == 0xE0)
        {
            if ((*(strPtr + 1) & 0xC0) == 0x80 && 
                (*(strPtr + 2) & 0xC0) == 0x80)
            {
                OMUInt16 c = (((OMUInt16)*strPtr) & 0x0F) << 12;
                strPtr++;
                c |= (((OMUInt16)*strPtr) & 0x3F) << 6;
                strPtr++;
                c |= (((OMUInt16)*strPtr) & 0x3F);
                strPtr++;
                *utf16StrPtr = c;
                utf16StrPtr++;
            }
            else
            {
                assert(false);
            }
        }
        else if ((*strPtr & 0xF8) == 0xF0)
        {
            if ((*(strPtr + 1) & 0xC0) == 0x80 && 
                (*(strPtr + 2) & 0xC0) == 0x80 && 
                (*(strPtr + 3) & 0xC0) == 0x80)
            {
                OMUInt32 c = (((OMUInt16)*strPtr) & 0x07) << 18;
                strPtr++;
                c |= (((OMUInt16)*strPtr) & 0x3F) << 12;
                strPtr++;
                c |= (((OMUInt16)*strPtr) & 0x3F) << 6;
                strPtr++;
                c |= (((OMUInt16)*strPtr) & 0x3F);
                strPtr++;
                c -= 0x10000;
                *utf16StrPtr = (OMUInt16)(0xD800 | ((c >> 10) & 0x03FF));
                utf16StrPtr++;
                *utf16StrPtr = (OMUInt16)(0xDC00 | (c & 0x03FF));
                utf16StrPtr++;
            }
            else
            {
                assert(false);
            }
        }
        else
        {
            assert(false);
        }
    }
}



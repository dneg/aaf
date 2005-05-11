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

#include "OMXMLWriter.h"
#include "OMUtilities.h"
#include "OMXMLUtilities.h"
#include "OMAssertions.h"


OMXMLOStream::OMXMLOStream(OMRawStorage* storage)
: _storage(storage)
{}

OMXMLOStream::~OMXMLOStream()
{}
    
OMUInt64 
OMXMLOStream::Write(const OMByte* data, OMUInt64 count)
{
    OMUInt32 numWritten;
    _storage->write(data, (OMUInt32)count, numWritten);
    
    return numWritten;
}

void 
OMXMLOStream::Synchronize(void)
{
    _storage->synchronize();
}


OMXMLWriter::OMXMLWriter(OMRawStorage* storage)
{
    _xmlWriter = new XMLWriterSimple(new OMXMLOStream(storage));
}

OMXMLWriter::~OMXMLWriter()
{
    if (_xmlWriter != 0)
    {
        delete _xmlWriter;
    }
}
    
void 
OMXMLWriter::writeDocumentStart()
{
    _xmlWriter->WriteDocumentStart();
}

void 
OMXMLWriter::writeDocumentEnd()
{
    _xmlWriter->WriteDocumentEnd();
}
    
void 
OMXMLWriter::writeElementStart(const wchar_t* ns, const wchar_t* localName)
{
    char* ns8 = convertFromWideString(ns);    
    char* localName8 = convertFromWideString(localName);
    
    _xmlWriter->WriteElementStart(ns8, localName8);
    
    delete [] ns8;
    delete [] localName8;
}

void 
OMXMLWriter::declareNamespace(const wchar_t* ns, const wchar_t* prefix)
{
    char* ns8 = convertFromWideString(ns);    
    char* prefix8 = 0;
    if (prefix != 0)
    {
        prefix8 = convertFromWideString(prefix);
    }
    
    _xmlWriter->DeclareNamespace(ns8, prefix8);
    
    delete [] ns8;
    if (prefix8 != 0)
    {
        delete [] prefix8;
    }
}

void 
OMXMLWriter::writeAttribute(const wchar_t* ns, const wchar_t* localName, const wchar_t* value)
{
    char* ns8 = 0;
    if (ns != 0)
    {
        ns8 = convertFromWideString(ns);
    }
    char* localName8 = convertFromWideString(localName);
    char* value8 = convertFromWideString(value);
    
    _xmlWriter->WriteAttribute(ns8, localName8, value8);
    
    if (ns8 != 0)
    {
        delete [] ns8;
    }
    delete [] localName8;
    delete [] value8;
}

void 
OMXMLWriter::writeAttributeStart(const wchar_t* ns, const wchar_t* localName)
{
    char* ns8 = 0;
    if (ns != 0)
    {
        ns8 = convertFromWideString(ns);
    }
    char* localName8 = convertFromWideString(localName);
    
    _xmlWriter->WriteAttributeStart(ns8, localName8);
    
    if (ns8 != 0)
    {
        delete [] ns8;
    }
    delete [] localName8;
}

void 
OMXMLWriter::writeAttributeContent(const wchar_t* value)
{
    char* value8 = convertFromWideString(value);
    
    _xmlWriter->WriteAttributeContent(value8);
    
    delete [] value8;
}

void 
OMXMLWriter::writeAttributeEnd(void)
{
    _xmlWriter->WriteAttributeEnd();
}

void 
OMXMLWriter::writeElementContent(const wchar_t* content, size_t length)
{
    char* content8 = convertFromWideString(content);    
    
    _xmlWriter->WriteElementContent(content8, strlen(content8));
    
    delete [] content8;
}

void 
OMXMLWriter::writeElementEnd(void)
{
    _xmlWriter->WriteElementEnd();
}

void 
OMXMLWriter::writeComment(const wchar_t* comment)
{
    char* comment8 = convertFromWideString(comment);    
    
    _xmlWriter->WriteComment(comment8);
    
    delete [] comment8;
}

void 
OMXMLWriter::writeProcInstruction(const wchar_t* target, const wchar_t* instruction)
{
    char* target8 = convertFromWideString(target);    
    char* instruction8 = convertFromWideString(instruction);
    
    _xmlWriter->WriteProcInstruction(target8, instruction8);
    
    delete [] target8;
    delete [] instruction8;
}

void 
OMXMLWriter::writeText(const wchar_t* text)
{
    char* text8 = convertFromWideString(text);    
    
    _xmlWriter->WriteText(text8);
    
    delete [] text8;
}

void 
OMXMLWriter::synchronize(void)
{
    _xmlWriter->Synchronize();
}





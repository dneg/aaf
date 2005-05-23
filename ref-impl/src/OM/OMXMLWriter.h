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

#ifndef __OMXMLWRITER_H__
#define __OMXMLWRITER_H__


#include "XMLWriter.h"
#include "OMRawStorage.h"


class OMXMLOStream : public XMLOStream
{
public:
    OMXMLOStream(OMRawStorage* storage);
    virtual ~OMXMLOStream();
    
    virtual OMUInt32 Write(const OMByte* data, OMUInt32 count);
    virtual void Synchronize(void);
    
private:
    OMRawStorage*   _storage;
};


class OMXMLWriter
{
public:
    OMXMLWriter(OMRawStorage* storage);
    ~OMXMLWriter();
    
    void writeDocumentStart();
    void writeDocumentEnd();
    void writeElementStart(const wchar_t* ns, const wchar_t* localName);
    void declareNamespace(const wchar_t* ns, const wchar_t* prefix);
    void writeAttribute(const wchar_t* ns, const wchar_t* localName, const wchar_t* value);
    void writeAttributeStart(const wchar_t* ns, const wchar_t* localName);
    void writeAttributeContent(const wchar_t* value);
    void writeAttributeEnd(void);
    void writeElementContent(const wchar_t* content, size_t length);
    void writeElementEnd(void);
    void writeComment(const wchar_t* comment);
    void writeProcInstruction(const wchar_t* target, const wchar_t* instruction);
    void writeText(const wchar_t* text);
    
    void synchronize(void);
    
private:
    XMLWriter*      _xmlWriter;
};


#endif

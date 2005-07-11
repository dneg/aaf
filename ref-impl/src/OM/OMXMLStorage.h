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

#ifndef __OMXMLSTORAGE_H__
#define __OMXMLSTORAGE_H__

#include "OMDiskRawStorage.h"
#include "OMXMLWriter.h"
#include "OMXMLReader.h"
#include "OMSymbolspace.h"
#include "OMWString.h"
#include "OMSet.h"
#include "OMDataStreamProperty.h"
#include "OMByteArray.h"


class OMMetaDefIdMap
{
public:
    OMMetaDefIdMap();
    ~OMMetaDefIdMap();
    
    void add(OMUniqueObjectIdentification id, const wchar_t* qSymbol);
    void add(OMUniqueObjectIdentification id, const wchar_t* symbolspace,
        const wchar_t* symbol);
    
    OMUniqueObjectIdentification getId(const wchar_t* qSymbol) const;
    const wchar_t* getQSymbol(OMUniqueObjectIdentification id) const;
    
private:
    OMSet<OMUniqueObjectIdentification, OMWString> _idToQSymbol;
    OMSet<OMWString, OMUniqueObjectIdentification> _qSymbolToId;    
};


class OMDefIdMap
{
public:
    OMDefIdMap();
    ~OMDefIdMap();
    
    void add(OMUniqueObjectIdentification id, const wchar_t* uid);
    
    OMUniqueObjectIdentification getId(const wchar_t* uid) const;
    const wchar_t* getUIDStr(OMUniqueObjectIdentification id) const;
    
private:
    OMSet<OMUniqueObjectIdentification, OMWString> _idToUIDStr;
    OMSet<OMWString, OMUniqueObjectIdentification> _uidStrToId;    
};


class OMXMLStorage
{
public:
    enum Mode {READ_MODE, WRITE_MODE, EXISTING_MODIFY_MODE, NEW_MODIFY_MODE};
    
    // note that OMXMLStorage takes ownership of storage
    OMXMLStorage(OMDiskRawStorage* storage, Mode mode);
    ~OMXMLStorage();
    
    Mode mode();
    bool haveReader();
    bool haveWriter();
    OMXMLWriter* getWriter();
    OMXMLReader* getReader();
    
    void resetForWriting();
    
    const wchar_t* getBaselineVersion() const;
    OMSymbolspace* getBaselineSymbolspace() const;
    OMSymbolspace* getDefaultExtSymbolspace() const;
    OMSymbolspace* createDefaultExtSymbolspace(OMUniqueObjectIdentification id);
    OMSymbolspace* createSymbolspace();
    void addSymbolspace(OMSymbolspace* symbolspace);
    OMSet<OMWString, OMSymbolspace*>& getSymbolspaces();

    OMUniqueObjectIdentification getMetaDefIdFromQSymbol(const wchar_t* qSymbol) const;
    void addMetaDefIdMap(OMUniqueObjectIdentification id, const wchar_t* symbolspace, 
        const wchar_t* symbol); 
    
    OMUniqueObjectIdentification getDefIdFromUIDStr(const wchar_t* uidStr) const;
    const wchar_t* getUIDStrFromId(OMUniqueObjectIdentification id) const;
    void addDefIdMap(OMUniqueObjectIdentification id, const wchar_t* uidStr); 
    
    bool getMetaDefSymbol(OMUniqueObjectIdentification id, const wchar_t** symbolspaceURI, const wchar_t** symbol) const;
    OMUniqueObjectIdentification getMetaDefId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const;
    OMPropertyId getPropertyDefId(const wchar_t* symbolspaceURI, const wchar_t* symbol) const;
    const wchar_t* getDefSymbol(OMUniqueObjectIdentification id);
    OMUniqueObjectIdentification getDefId(const wchar_t* symbol) const;
    
    OMUniqueObjectIdentification getBaselineDefId(const wchar_t* symbol) const;
    OMUniqueObjectIdentification getBaselineMetaDefId(const wchar_t* symbol) const;
    const wchar_t* getBaselineDefSymbol(OMUniqueObjectIdentification id);
    const wchar_t* getBaselineMetaDefSymbol(OMUniqueObjectIdentification id);

    OMSymbolspace* getSymbolspaceForDef(OMUniqueObjectIdentification id) const;
    OMSymbolspace* getSymbolspaceForMetaDef(OMUniqueObjectIdentification id) const;
    bool isKnownExtEnumElement(OMUniqueObjectIdentification elementOf,
        OMUniqueObjectIdentification value) const;
    bool isBaselineSymbolspace(OMSymbolspace* symbolspace) const;
        
    const wchar_t* getDataStreamNotationName(OMUniqueObjectIdentification typeId);
    const wchar_t* getDataStreamEntityName(void* ref);
    const wchar_t* getDataStreamEntityValue(void* ref);
    const wchar_t* registerDataStreamEntityValue(void* ref, const wchar_t* value);
    bool registerDataStreamEntity(const wchar_t* name, const wchar_t* value);
    const wchar_t* getDataStreamEntityValue(const wchar_t* name);
    bool registerDataStreamNotation(const wchar_t* notationName, const wchar_t* systemId);
    
    OMRawStorage* openExistingDataStream(const wchar_t* uri);
    OMRawStorage* openNewDataStream(const wchar_t* uri);
    
    void forwardObjectSetId(const wchar_t* id);
    bool haveForwardedObjectSetId();
    wchar_t* getForwardedObjectSetId();

    void registerNamespacePrefix(const wchar_t* prefix, const wchar_t* uri);
    
private:
    void setUniquePrefix(OMSymbolspace* symbolspace);
    void loadBaselineSymbolspace();

    Mode            _mode;
    OMDiskRawStorage*   _storage;
    OMXMLWriter*    _xmlWriter;
    OMXMLReader*    _xmlReader;
    
    wchar_t*        _objectSetId;
    
    OMSymbolspace*  _baselineSymbolspace;
    OMSymbolspace*  _defaultExtSymbolspace;
    
    OMSet<OMWString, OMSymbolspace*> _symbolspaces;
    OMSet<OMWString, OMSymbolspace*> _symbolspacesPrefixMap;
    int             _symbolspacePrefixIndex;

    int             _dataStreamNotationNameIndex;
    int             _dataStreamEntityNameIndex;
    int             _dataStreamEntityValueIndex;
    OMSet<OMUniqueObjectIdentification, OMWString> _dataStreamNotationNames;
    OMSet<void*, OMWString> _dataStreamEntityNames;
    OMSet<void*, OMWString> _dataStreamEntityValues;
    
    OMSet<OMWString, OMWString> _inputDataStreamEntities;
    
    OMSet<OMWString, OMWString> _namespacesPrefixMap;
    
    OMMetaDefIdMap _metaDefIdMap;

    OMDefIdMap _defIdMap;
};


#endif

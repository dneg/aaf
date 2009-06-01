//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
// Portions created by British Broadcasting Corporation are
// Copyright 2005, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMXMLSTOREDOBJECT_H
#define OMXMLSTOREDOBJECT_H

#include "OMStoredObject.h"
#include "OMDataTypes.h"
#include "OMFile.h"
#include "OMXMLStorage.h"
#include "OMList.h"
#include "OMByteArray.h"
#include "OMVector.h"

class OMSimpleProperty;
class OMDataVector;
class OMDataSet;
class OMDataStream;
class OMStrongReference;
class OMStrongReferenceSet;
class OMStrongReferenceVector;
class OMWeakReference;
class OMWeakReferenceSet;
class OMWeakReferenceVector;
class OMStoredStream;

class OMCharacterType;
class OMEnumeratedType;
class OMExtendibleEnumeratedType;
class OMFixedArrayType;
class OMIndirectType;
class OMIntegerType;
class OMOpaqueType;
class OMRecordType;
class OMRenamedType;
class OMSetType;
class OMStreamType;
class OMStringType;
class OMStrongObjectReferenceType;
class OMVaryingArrayType;
class OMWeakObjectReferenceType;

class OMWeakObjectReference;

class OMXMLAttribute;

  // @class In-memory representation of an object persisted in an
  //        eXtensible Markup Language (XML) text file.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMXMLStoredObject : public OMStoredObject {
public:
  // @access Static members.

    // @cmember Open the root <c OMXMLStoredObject> in the disk raw storage
    //          <p storage> for reading only.
    static OMXMLStoredObject* openRead(OMDiskRawStorage* storage);

    // @cmember Open the root <c OMXMLStoredObject> in the raw storage
    //          <p storage> for modification.
    static OMXMLStoredObject* openModify(OMDiskRawStorage* storage);

    // @cmember Create a new root <c OMXMLStoredObject> in the raw storage
    //          <p storage>.
    static OMXMLStoredObject* createWrite(OMDiskRawStorage* storage);

    // @cmember Create a new root <c OMXMLStoredObject> in the raw storage
    //          <p storage>.
    static OMXMLStoredObject* createModify(OMDiskRawStorage* storage);

  // @access Public members.

    // @cmember Destructor.
    virtual ~OMXMLStoredObject(void);

    // @cmember Create a new <c OMXMLStoredObject>, named <p name>,
    //          contained by this <c OMXMLStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
    virtual OMStoredObject* create(const wchar_t* name);

    // @cmember Open an exsiting <c OMXMLStoredObject>, named <p name>,
    //          contained by this <c OMXMLStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
    virtual OMStoredObject* open(const wchar_t* name);

    // @cmember Close this <c OMXMLStoredObject>.
    virtual void close(void);

    // @cmember The byte order of this <c OMXMLStoredObject>.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
    virtual OMByteOrder byteOrder(void) const;

    virtual void save(OMFile& file);

    virtual void save(OMStorable& object);

  // Saving and restoring properties

    // @cmember Save the <c OMStoredObjectIdentification> <p id>
    //          in this <c OMXMLStoredObject>.
    virtual void save(const OMStoredObjectIdentification& id);

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMPropertySet& properties);

    // @cmember Save the <c OMSimpleProperty> <p property> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMSimpleProperty& property);

    // @cmember Save the <c OMDataVector> <p property> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMDataVector& property);

    // @cmember Save the <c OMDataSet> <p property> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMDataSet& property);

    // @cmember Save the <c OMStrongReference> <p singleton> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMStrongReference& singleton);

    // @cmember Save the <c OMStrongReferenceVector> <p vector> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMStrongReferenceVector& vector);

    // @cmember Save the <c OMStrongReferenceSet> <p set> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMStrongReferenceSet& set);

    // @cmember Save the <c OMWeakReference> <p singleton> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMWeakReference& singleton);

    // @cmember Save the <c OMWeakReferenceVector> <p vector> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMWeakReferenceVector& vector);

    // @cmember Save the <c OMWeakReferenceSet> <p set> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMWeakReferenceSet& set);

    // @cmember Save the <c OMPropertyTable> <p table> in this
    //          <c OMXMLStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
    virtual void save(const OMPropertyTable* table);

    // @cmember Save the <c OMDataStream> <p stream> in this
    //          <c OMXMLStoredObject>.
    virtual void save(const OMDataStream& stream);

    virtual OMRootStorable* restore(OMFile& file);

    virtual OMStorable* restoreObject(const OMStrongObjectReference& reference);

    // @cmember Restore the <c OMStoredObjectIdentification>
    //          of this <c OMXMLStoredObject> into <p id>.
    virtual void restore(OMStoredObjectIdentification& id);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMXMLStoredObject>.
    virtual void restore(OMPropertySet& properties);

    // @cmember Restore the <c OMSimpleProperty> <p property> into this
    //          <c OMXMLStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
    virtual void restore(OMSimpleProperty& property,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMDataVector> <p property> into this
    //          <c OMXMLStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
    virtual void restore(OMDataVector& property,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMDataSet> <p property> into this
    //          <c OMXMLStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
    virtual void restore(OMDataSet& property,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMStrongReference> <p singleton> into this
    //          <c OMXMLStoredObject>.
    virtual void restore(OMStrongReference& singleton,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMStrongReferenceVector> <p vector> into this
    //          <c OMXMLStoredObject>.
    virtual void restore(OMStrongReferenceVector& vector,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMStrongReferenceSet> <p set> into this
    //          <c OMXMLStoredObject>.
    virtual void restore(OMStrongReferenceSet& set,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMWeakReference> <p singleton> into this
    //          <c OMXMLStoredObject>.
    virtual void restore(OMWeakReference& singleton,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMWeakReferenceVector> <p vector> into this
    //          <c OMXMLStoredObject>.
    virtual void restore(OMWeakReferenceVector& vector,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMWeakReferenceSet> <p set> into this
    //          <c OMXMLStoredObject>.
    virtual void restore(OMWeakReferenceSet& set,
                         OMPropertySize externalSize);

    // @cmember Restore the <c OMPropertyTable> in this <c OMXMLStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
    virtual void restore(OMPropertyTable*& table);

    // @cmember Restore the <c OMDataStream> <p stream> into this
    //          <c OMXMLStoredObject>.
    virtual void restore(OMDataStream& stream,
                         OMPropertySize externalSize);

  // Stream manipulation

    // @cmember Open the <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMXMLStoredObject>.
    virtual OMStoredStream* openStoredStream(const OMDataStream& property);

    // @cmember Create an <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMXMLStoredObject>.
    virtual OMStoredStream* createStoredStream(const OMDataStream& property);

private:
  // @access Private members.

    // @cmember Constructor.
    OMXMLStoredObject(OMXMLStorage* store, bool isRoot);

    OMXMLWriter* getWriter();
    OMXMLReader* getReader();
    const wchar_t* getBaselineURI();

    void registerExtensions(OMFile& file, OMSymbolspace* extSymbolspace);
    
    void saveHeaderByteOrder(const OMProperty* property);
    
    void saveSimpleValue(const OMByte* internalBytes, OMUInt32 internalSize, const OMType* type,
        bool isElementContent);
    void saveCharacter(const OMByte* externalBytes, OMUInt32 externalSize, const OMCharacterType* type,
        bool isElementContent);
    void saveEnum(const OMByte* internalBytes, OMUInt32 internalSize, const OMEnumeratedType* type,
        bool isElementContent);
    void saveExtEnum(const OMByte* internalBytes, OMUInt32 internalSize, const OMExtendibleEnumeratedType* type,
        bool isElementContent);
    void saveFixedArray(const OMByte* internalBytes, OMUInt32 internalSize, const OMFixedArrayType* type,
        bool isElementContent);
    void saveIndirect(const OMByte* externalBytes, OMUInt32 externalSize, const OMIndirectType* type,
        bool isElementContent);
    void saveInteger(const OMByte* internalBytes, OMUInt32 internalSize, const OMIntegerType* type,
        bool isElementContent);
    void saveOpaque(const OMByte* externalBytes, OMUInt32 externalSize, const OMOpaqueType* type,
        bool isElementContent);
    void saveRecord(const OMByte* internalBytes, OMUInt32 internalSize, const OMRecordType* type,
        bool isElementContent);
    void saveRenamed(const OMByte* internalBytes, OMUInt32 internalSize, const OMRenamedType* type,
        bool isElementContent);
    void saveString(const OMByte* internalBytes, OMUInt32 internalSize, const OMStringType* type,
        bool isElementContent);
    void saveSet(const OMByte* internalBytes, OMUInt32 internalSize, const OMSetType* type,
        bool isElementContent);
    void saveVariableArray(const OMByte* internalBytes, OMUInt32 internalSize, const OMVaryingArrayType* type,
        bool isElementContent);

    void writeDataInHex(const OMByte* data, OMUInt32 size, bool isElementContent);

    
    void restoreExtensions(OMDictionary* dictionary);

    void restoreHeaderByteOrder(OMProperty* property);
    
    void restoreSimpleValue(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMType* type);
    void restoreCharacter(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMCharacterType* type);
    void restoreEnum(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMEnumeratedType* type);
    void restoreExtEnum(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMExtendibleEnumeratedType* type);
    void restoreFixedArray(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMFixedArrayType* type);
    void restoreIndirect(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMIndirectType* type);
    void restoreInteger(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMIntegerType* type);
    void restoreOpaque(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMOpaqueType* type);
    void restoreRecord(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMRecordType* type);
    void restoreRenamed(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMRenamedType* type);
    void restoreSet(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMSetType* type);
    void restoreString(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMStringType* type);
    void restoreVariableArray(OMByteArray& bytes, const OMList<OMXMLAttribute*>* attributes,
        const wchar_t* str, const OMVaryingArrayType* type);

    enum AUIDTargetType
    {
        METADICT_DEF,
        DICT_DEF,
        NON_DEF,
        ANY
    };
    OMUniqueObjectIdentification restoreAUID(const wchar_t* idStr, AUIDTargetType targetType);
    wchar_t* saveAUID(OMUniqueObjectIdentification id, AUIDTargetType targetType);
    
    
    const OMType* baseType(const OMType* type);
    
    void restoreWeakRef(OMFile* file, const OMType* type,
        OMUniqueObjectIdentification& id, OMPropertyTag& tag);
    void saveWeakRef(const void* identificationBits, const OMWeakObjectReferenceType* weakRefType);
    
    OMUniqueObjectIdentification getExtensionSymbolspaceId(OMFile& file);

    void getDataStreams(OMStorable* storable, OMVector<OMDataStream*>& dataStreams);
    
    void createDictionaryMap();

    
    OMXMLStorage*   _store;
    bool            _isRoot;
};

#endif

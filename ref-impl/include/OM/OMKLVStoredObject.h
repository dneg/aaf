//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMKLVSTOREDOBJECT_H
#define OMKLVSTOREDOBJECT_H

#include "OMStoredObject.h"
#include "OMDataTypes.h"
#include "OMFile.h"

class OMSimpleProperty;
class OMDataStream;
class OMStrongReference;
class OMStrongReferenceSet;
class OMStrongReferenceVector;
class OMWeakReference;
class OMWeakReferenceSet;
class OMWeakReferenceVector;
class OMStoredStream;

  // @class In-memory representation of an object persisted in a
  //        SMPTE (Society of Motion Picture and Television Engineers)
  //        Key Length Value (KLV) binary file.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMKLVStoredObject : public OMStoredObject {
public:
  // @access Static members.

    // @cmember Open the root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  static OMKLVStoredObject* openRead(OMRawStorage* rawStorage);

    // @cmember Open the root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  static OMKLVStoredObject* openModify(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMKLVStoredObject* createWrite(OMRawStorage* rawStorage,
                                        const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMKLVStoredObject* createModify(OMRawStorage* rawStorage,
                                         const OMByteOrder byteOrder);

  // @access Public members.

    // @cmember Destructor.
  virtual ~OMKLVStoredObject(void);

    // @cmember Create a new <c OMKLVStoredObject>, named <p name>,
    //          contained by this <c OMKLVStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* create(const wchar_t* name);

    // @cmember Open an exsiting <c OMKLVStoredObject>, named <p name>,
    //          contained by this <c OMKLVStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* open(const wchar_t* name);

    // @cmember Close this <c OMKLVStoredObject>.
  virtual void close(void);

    // @cmember The byte order of this <c OMKLVStoredObject>.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  virtual OMByteOrder byteOrder(void) const;

  virtual void save(OMFile& file);

  virtual void save(OMStorable& object);

  // Saving and restoring properties

    // @cmember Save the <c OMStoredObjectIdentification> <p id>
    //          in this <c OMKLVStoredObject>.
  virtual void save(const OMStoredObjectIdentification& id);

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMPropertySet& properties);

    // @cmember Save the <c OMSimpleProperty> <p property> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMSimpleProperty& property);

    // @cmember Save the <c OMStrongReference> <p singleton> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReference& singleton);

    // @cmember Save the <c OMStrongReferenceVector> <p vector> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReferenceVector& vector);

    // @cmember Save the <c OMStrongReferenceSet> <p set> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReferenceSet& set);

    // @cmember Save the <c OMWeakReference> <p singleton> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReference& singleton);

    // @cmember Save the <c OMWeakReferenceVector> <p vector> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReferenceVector& vector);

    // @cmember Save the <c OMWeakReferenceSet> <p set> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReferenceSet& set);

    // @cmember Save the <c OMPropertyTable> <p table> in this
    //          <c OMKLVStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void save(const OMPropertyTable* table);

    // @cmember Save the <c OMDataStream> <p stream> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMDataStream& stream);

  virtual OMRootStorable* restore(OMFile& file);

  virtual OMStorable* restoreObject(const OMStrongObjectReference& reference);

    // @cmember Restore the <c OMStoredObjectIdentification>
    //          of this <c OMKLVStoredObject> into <p id>.
  virtual void restore(OMStoredObjectIdentification& id);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMKLVStoredObject>.
  virtual void restore(OMPropertySet& properties);

    // @cmember Restore the <c OMSimpleProperty> <p property> into this
    //          <c OMKLVStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMSimpleProperty& property,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReference> <p singleton> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReference& singleton,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceVector> <p vector> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReferenceVector& vector,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceSet> <p set> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReferenceSet& set,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReference> <p singleton> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReference& singleton,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceVector> <p vector> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReferenceVector& vector,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceSet> <p set> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReferenceSet& set,
                       size_t externalSize);

    // @cmember Restore the <c OMPropertyTable> in this <c OMKLVStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void restore(OMPropertyTable*& table);

    // @cmember Restore the <c OMDataStream> <p stream> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMDataStream& stream,
                       size_t externalSize);

  // Stream manipulation

    // @cmember Open the <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMKLVStoredObject>.
  virtual OMStoredStream* openStoredStream(const OMDataStream& property);

    // @cmember Create an <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMKLVStoredObject>.
  virtual OMStoredStream* createStoredStream(const OMDataStream& property);

  // KLV functions

  virtual OMUInt64 length(const OMPropertySet& properties) const;
  virtual void flatSave(const OMPropertySet& properties) const;
  virtual void deepSave(const OMPropertySet& properties) const;
  virtual void streamSave(const OMPropertySet& properties) const;

  virtual void referenceSave(OMStorable* object, OMPropertyId pid) const;

  virtual void flatRestore(const OMPropertySet& properties);
  virtual void deepRestore(const OMPropertySet& properties);
  virtual void streamRestore(OMRawStorage* store);

  virtual void referenceRestore(OMStorable* object, OMPropertyId pid);

  virtual void writeHeaderPartition(void);
  static void writeFooterPartition(OMRawStorage* store);
  static void writePartition(OMRawStorage* store,
                             const OMKLVKey& key,
                             bool reorderBytes);

  static void writePrimerPack(OMRawStorage* store,
                              const OMDictionary* dictionary,
                              bool reorderBytes);

  static void fill(OMRawStorage* store,
                   const OMUInt64& currentPosition,
                   const OMUInt32& KAGSize);

  static void writeKLVKey(OMRawStorage* store, const OMKLVKey& key);

  static void writeKLVLength(OMRawStorage* store, const OMUInt64& length);

  static void writeKLVFill(OMRawStorage* store, const OMUInt64& length);

  static void write(OMRawStorage* store, const OMUInt8& i);
  static void write(OMRawStorage* store, const OMUInt16& i, bool reorderBytes);
  static void write(OMRawStorage* store, const OMUInt32& i, bool reorderBytes);
  static void write(OMRawStorage* store, const OMUInt64& i, bool reorderBytes);
  static void write(OMRawStorage* store,
                    const OMUniqueObjectIdentification& id,
                    bool reorderBytes);

  static void write(OMRawStorage* store,
                    const OMByte* buffer,
                    const OMUInt32& bufferSize);

  static size_t berEncodedSize(const OMUInt64 i);

  static void berEncode(OMByte* berValue,
                        size_t berValueBufferSize,
                        OMUInt32& berValueSize,
                        const OMUInt64& value);

  static OMUInt64 readBerLength(OMRawStorage* store);

  static bool readHeaderPartition(OMRawStorage* store);

  static void readPrimerPack(OMRawStorage* store,
                             OMDictionary* dictionary,
                             bool reorderBytes);

  static void readKLVKey(OMRawStorage* store, OMKLVKey& key);

  static OMUInt64 readKLVLength(OMRawStorage* store);

  static void readKLVFill(OMRawStorage* store);

  static void skipLV(OMRawStorage* store);

  static void read(OMRawStorage* store, OMUInt8& i);
  static void read(OMRawStorage* store, OMUInt16& i, bool reorderBytes);
  static void read(OMRawStorage* store, OMUInt32& i, bool reorderBytes);
  static void read(OMRawStorage* store, OMUInt64& i, bool reorderBytes);
  static void read(OMRawStorage* store,
                    OMUniqueObjectIdentification& id,
                    bool reorderBytes);

  static void read(OMRawStorage* store,
                   OMByte* buffer,
                   const OMUInt32& bufferSize);

    // Object -> instanceId
  static OMUniqueObjectIdentification instanceId(OMStorable* object);

    // instanceId -> object
  static OMStorable* object(const OMUniqueObjectIdentification& instanceId);

  static OMSet<OMStorable*, OMUniqueObjectIdentification>* _objectToInstanceId;

  struct ObjectDirectoryEntry {
    OMStorable* _object;
    OMUInt64 _offset;
    OMUInt8 _flags;
  };

  static OMSet<OMUniqueObjectIdentification,
               ObjectDirectoryEntry>* _instanceIdToObject;

  static OMSet<OMStorable*, OMUniqueObjectIdentification>* objectToInstanceId(
                                                                         void);
  static OMSet<OMUniqueObjectIdentification,
               ObjectDirectoryEntry>* instanceIdToObject(void);

    // Stream -> streamId
  static OMKLVKey streamId(OMDataStream* stream);

    // streamId -> Stream
  static OMDataStream* stream(const OMKLVKey& streamId);

  static OMSet<OMDataStream*, OMKLVKey>* _streamToStreamId;
  static OMSet<OMKLVKey, OMDataStream*>* _streamIdToStream;

  static OMSet<OMDataStream*, OMKLVKey>* streamToStreamId(void);
  static OMSet<OMKLVKey, OMDataStream*>* streamIdToStream(void);

  OMUInt64 save(OMSet<OMUniqueObjectIdentification,
                      ObjectDirectoryEntry>* objectTable);

  void restore(OMSet<OMUniqueObjectIdentification,
                     ObjectDirectoryEntry>* objectTable);

  OMUInt64 saveObjectDirectoryReference(
                                       const OMUniqueObjectIdentification& id);

  void fixupObjectDirectoryReference(OMUInt64 patchOffset,
                                     OMUInt64 patchValue);

  OMUInt64 restoreObjectDirectoryReference(OMUniqueObjectIdentification& id);

  static void convert(OMKLVKey& key, const OMUniqueObjectIdentification& id);

  static void convert(OMUniqueObjectIdentification& id, const OMKLVKey& key);

  static void finalize(void);

  static bool metaDataOnly;

private:
  // @access Private members.

    // @cmember Constructor.
  OMKLVStoredObject(OMRawStorage* s, OMByteOrder byteOrder);

  OMRawStorage* _storage;
  OMByteOrder _byteOrder;
  bool _reorderBytes;
  OMUInt64 _objectDirectory;          // offset of object directory
  OMUInt64 _objectDirectoryReference; // offset object directory reference
};

#endif

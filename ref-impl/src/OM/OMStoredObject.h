/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

// @doc OMINTERNAL
#ifndef OMSTOREDOBJECT_H
#define OMSTOREDOBJECT_H

#include "OMDataTypes.h"
#include "OMFile.h"

#include <stddef.h>

struct IStorage;
struct IStream;

class OMStoredPropertySetIndex;
class OMProperty;
class OMPropertySet;
class OMPropertyTable;
class OMStoredVectorIndex;
class OMStoredSetIndex;
class OMSimpleProperty;
class OMStrongReference;
class OMStrongReferenceVector;
class OMStrongReferenceSet;
class OMWeakReference;
class OMWeakReferenceVector;
class OMWeakReferenceSet;
class OMDataStream;
class OMStoredStream;

  // @class In-memory representation of a persistent object.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStoredObject {
public:
  // @access Static members.

    // @cmember Open the root <c OMStoredObject> in the disk file
    //          <p fileName> for reading only.
    //   @devnote Soon to be obsolete.
  static OMStoredObject* openRead(const wchar_t* fileName);

    // @cmember Open the root <c OMStoredObject> in the disk file
    //          <p fileName> for modification.
    //   @devnote Soon to be obsolete.
  static OMStoredObject* openModify(const wchar_t* fileName);

    // @cmember Create a new root <c OMStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
    //   @devnote Soon to be obsolete.
  static OMStoredObject* createModify(const wchar_t* fileName,
                                      const OMByteOrder byteOrder);

    // @cmember Open the root <c OMStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  static OMStoredObject* openRead(OMRawStorage* rawStorage);

    // @cmember Open the root <c OMStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  static OMStoredObject* openModify(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMStoredObject* createModify(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder);

  // @access Public members.

    // @cmember Destructor.
  ~OMStoredObject(void);

    // @cmember Create a new <c OMStoredObject>, named <p name>,
    //          contained by this <c OMStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  OMStoredObject* create(const wchar_t* name);

    // @cmember Open an exsiting <c OMStoredObject>, named <p name>,
    //          contained by this <c OMStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  OMStoredObject* open(const wchar_t* name);

    // @cmember Close this <c OMStoredObject>.
  void close(void);

    // @cmember The byte order of this <c OMStoredObject>.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  OMByteOrder byteOrder(void) const;

  // Saving and restoring properties

    // @cmember Save the <c OMStoredObjectIdentification> <p id>
    //          in this <c OMStoredObject>.
  void save(const OMStoredObjectIdentification& id);

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMStoredObject>.
  void save(const OMPropertySet& properties);

    // @cmember Save the <c OMSimpleProperty> <p property> in this
    //          <c OMStoredObject>.
  void save(const OMSimpleProperty& property);

    // @cmember Save the <c OMStrongReference> <p singleton> in this
    //          <c OMStoredObject>.
  void save(const OMStrongReference& singleton);

    // @cmember Save the <c OMStrongReferenceVector> <p vector> in this
    //          <c OMStoredObject>.
  void save(const OMStrongReferenceVector& vector);

    // @cmember Save the <c OMStrongReferenceSet> <p set> in this
    //          <c OMStoredObject>.
  void save(const OMStrongReferenceSet& set);

    // @cmember Save the <c OMWeakReference> <p singleton> in this
    //          <c OMStoredObject>.
  void save(const OMWeakReference& singleton);

    // @cmember Save the <c OMWeakReferenceVector> <p vector> in this
    //          <c OMStoredObject>.
  void save(const OMWeakReferenceVector& vector);

    // @cmember Save the <c OMWeakReferenceSet> <p set> in this
    //          <c OMStoredObject>.
  void save(const OMWeakReferenceSet& set);

    // @cmember Save the <c OMPropertyTable> <p table> in this
    //          <c OMStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  void save(const OMPropertyTable* table);

    // @cmember Save the <c OMDataStream> <p stream> in this
    //          <c OMStoredObject>.
  void save(const OMDataStream& stream);

    // @cmember Restore the <c OMStoredObjectIdentification>
    //          of this <c OMStoredObject> into <p id>.
  void restore(OMStoredObjectIdentification& id);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMStoredObject>.
  void restore(OMPropertySet& properties);

    // @cmember Restore the <c OMSimpleProperty> <p property> into this
    //          <c OMStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  void restore(OMSimpleProperty& property, size_t externalSize);

    // @cmember Restore the <c OMStrongReference> <p singleton> into this
    //          <c OMStoredObject>.
  void restore(OMStrongReference& singleton, size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceVector> <p vector> into this
    //          <c OMStoredObject>.
  void restore(OMStrongReferenceVector& vector, size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceSet> <p set> into this
    //          <c OMStoredObject>.
  void restore(OMStrongReferenceSet& set, size_t externalSize);

    // @cmember Restore the <c OMWeakReference> <p singleton> into this
    //          <c OMStoredObject>.
  void restore(OMWeakReference& singleton, size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceVector> <p vector> into this
    //          <c OMStoredObject>.
  void restore(OMWeakReferenceVector& vector, size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceSet> <p set> into this
    //          <c OMStoredObject>.
  void restore(OMWeakReferenceSet& set, size_t externalSize);

    // @cmember Restore the <c OMPropertyTable> in this <c OMStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  void restore(OMPropertyTable*& table);

    // @cmember Restore the <c OMDataStream> <p stream> into this
    //          <c OMStoredObject>.
  void restore(OMDataStream& stream, size_t externalSize);

  // Stream manipulation

    // @cmember Open the <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMStoredObject>.
  OMStoredStream* openStoredStream(const OMDataStream& property);

    // @cmember Create an <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMStoredObject>.
  OMStoredStream* createStoredStream(const OMDataStream& property);

  // Name manipulation.

    // @cmember Compute the name for a stream.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  static wchar_t* streamName(const wchar_t* propertyName,
                             OMPropertyId pid);

    // @cmember Compute the name for an object reference.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  static wchar_t* referenceName(const wchar_t* propertyName,
                                OMPropertyId pid);

    // @cmember Compute the name for a collection.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  static wchar_t* collectionName(const wchar_t* propertyName,
                                 OMPropertyId pid);

    // @cmember Compute the name for an object reference
    //          that is an element of a collection.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  static wchar_t* elementName(const wchar_t* propertyName,
                              OMPropertyId pid,
                              OMUInt32 localKey);

    // @cmember Constructor.
  OMStoredObject(IStorage* s);

    // @cmember Check that the <c OMPropertySet> <p propertySet> is
    //          consistent with the <c OMStoredPropertySetIndex>
    //          propertySetIndex.
  void validate(const OMPropertySet* propertySet,
                const OMStoredPropertySetIndex* propertySetIndex) const;

    // @cmember Save the <c OMStoredVectorIndex> <p vector> in this
    //          <c OMStoredObject>, the vector is named <p vectorName>.
  void save(const OMStoredVectorIndex* vector, const wchar_t* vectorName);

    // @cmember Save the <c OMStoredSetIndex> <p set> in this
    //          <c OMStoredObject>, the set is named <p setName>.
  void save(const OMStoredSetIndex* set, const wchar_t* setName);

    // @cmember Save a single weak reference.
  void save(OMPropertyId propertyId,
            OMStoredForm storedForm,
            const OMUniqueObjectIdentification& id,
            OMPropertyTag tag,
            OMPropertyId keyPropertyId);

    // @cmember Save a collection (vector/set) of weak references.
  void save(const wchar_t* collectionName,
            const OMUniqueObjectIdentification* index,
            size_t count,
            OMPropertyTag tag,
            OMPropertyId keyPropertyId);

  void saveStream(OMPropertyId pid,
                  OMStoredForm storedForm,
                  const wchar_t* name,
                  OMByteOrder byteOrder);

    // @cmember Restore the vector named <p vectorName> into this
    //          <c OMStoredObject>.
  void restore(OMStoredVectorIndex*& vector, const wchar_t* vectorName);

    // @cmember Restore the set named <p setName> into this
    //          <c OMStoredObject>.
  void restore(OMStoredSetIndex*& set, const wchar_t* setName);

    // @cmember Restore a single weak reference.
  void restore(OMPropertyId propertyId,
               OMStoredForm storedForm,
               OMUniqueObjectIdentification& id,
               OMPropertyTag& tag,
               OMPropertyId& keyPropertyId);

    // @cmember Restore a collection (vector/set) of weak references.
  void restore(const wchar_t* collectionName,
               OMUniqueObjectIdentification*& index,
               size_t &count,
               OMPropertyTag& tag,
               OMPropertyId& keyPropertyId);

  void restoreStream(OMPropertyId pid,
                     OMStoredForm storedForm,
                     size_t size,
                     wchar_t** name,
                     OMByteOrder* byteOrder);

    // @cmember Write a property value to this <c OMStoredObject>. The
    //          property value to be written occupies <p size> bytes at
    //          the address <p start>. The property id is <p propertyId>.
    //          The property type is <p type>.
  void write(OMPropertyId propertyId,
             OMStoredForm storedForm,
             void* start,
             size_t size);

    // @cmember Read a property value from this <c OMStoredObject>.
    //          The property value is read into a buffer which occupies
    //          <p size> bytes at the address <p start>. The property id
    //          is <p propertyId>. The property type is <p type>.
  void read(OMPropertyId propertyId,
            OMStoredForm storedForm,
            void* start,
            size_t size);

    // @cmember Open a stream called <p streamName> contained within
    //          this <c OMStoredObject>.
  IStream* openStream(const wchar_t* streamName);

    // @cmember Create a stream called <p streamName> contained within
    //          this <c OMStoredObject>.
  IStream* createStream(const wchar_t* streamName);

    // @cmember Read <p size> bytes from <p stream> into the buffer at
    //          address <p data>.
  void readFromStream(IStream* stream, void* data, size_t size);

    // @cmember Attempt to read <p bytes> bytes from <p stream> into
    //          the buffer at address <p data>. The actual number of
    //          bytes read is returned in <p bytesRead>.
  void readFromStream(IStream* stream,
                      OMByte* data,
                      const OMUInt32 bytes,
                      OMUInt32& bytesRead);

    // @cmember Write <p size> bytes from the buffer at address
    //          <p data> to <p stream>.
  void writeToStream(IStream* stream, void* data, size_t size);

    // @cmember Attempt to write <p bytes> bytes from the buffer at
    //          address <p data> to <p stream>. The actual number of
    //          bytes written is returned in <p bytesWritten>.
  void writeToStream(IStream* stream,
                     const OMByte* data,
                     const OMUInt32 bytes,
                     OMUInt32& bytesWritten);

    // @cmember Read an OMUInt8 from <p stream> into <p i>.
  void readUInt8FromStream(IStream* stream, OMUInt8& i);

    // @cmember Read an OMUInt16 from <p stream> into <p i>.
    //          If <p reorderBytes> is true then the bytes are reordered.
  void readUInt16FromStream(IStream* stream, OMUInt16& i, bool reorderBytes);

    // @cmember Reorder the OMUInt16 <p i>.
  static void reorderUInt16(OMUInt16& i);

    // @cmember Read an OMUInt32 from <p stream> into <p i>.
    //          If <p reorderBytes> is true then the bytes are reordered.
  void readUInt32FromStream(IStream* stream, OMUInt32& i, bool reorderBytes);

    // @cmember Reorder the OMUInt32 <p i>.
  static void reorderUInt32(OMUInt32& i);

    // @cmember Read a UniqueObjectIdentification from <p stream>
    //          into <p id>.
    //          If <p reorderBytes> is true then the bytes are reordered.
  void readUniqueObjectIdentificationFromStream(
                                              IStream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes);

    // @cmember Read a UniqueMaterialIdentification from <p stream>
    //          into <p id>.
    //          If <p reorderBytes> is true then the bytes are reordered.
  void readUniqueMaterialIdentificationFromStream(
                                            IStream* stream,
                                            OMUniqueMaterialIdentification& id,
                                            bool reorderBytes);

    // @cmember Reorder the UniqueObjectIdentification <p id>.
  static void reorderUniqueObjectIdentification(
                                             OMUniqueObjectIdentification& id);

    // @cmember Reorder the UniqueMaterialIdentification <p id>.
  static void reorderUniqueMaterialIdentification(
                                           OMUniqueMaterialIdentification& id);

    // @cmember Size of <p stream> in bytes.
  OMUInt64 streamSize(IStream* stream) const;

    // @cmember Set the size, in bytes, of <p stream>
  void streamSetSize(IStream* stream, const OMUInt64 newSize);

    // @cmember The current position for <f readFromStream()> and
    //          <f writeToStream()>, as an offset in bytes from the
    //          begining of the data stream.
  OMUInt64 streamPosition(IStream* stream) const;

    // @cmember Set the current position for <f readFromStream()> and
    //          <f writeToStream()>, as an offset in bytes from the
    //          begining of the data stream.
  void streamSetPosition(IStream* stream, const OMUInt64 offset);

    // @cmember Close <p stream>.
  void closeStream(IStream*& stream);

    // @cmember The persisted value of <p property> is its name.
    //          Write the property name and enter it into the property index.
  void saveName(const OMProperty& property, const wchar_t* name);

    // @cmember The persisted value of <p property> is its name.
    //          Read (and check) the property name.
  void restoreName(OMProperty& property, const wchar_t* name, size_t size);

  static void mapCharacters(wchar_t* name, size_t nameLength);

  static void mangleName(const wchar_t* clearName,
                         OMPropertyId pid,
                         wchar_t* mangledName,
                         size_t mangledNameSize);

  void writeName(OMPropertyId pid,
                 OMStoredForm storedForm,
                 const wchar_t* name);

  wchar_t* readName(OMPropertyId pid,
                    OMStoredForm storedForm,
                    size_t size);

  static void reorderString(OMCharacter* string,
                            size_t characterCount);

  static void internalizeString(OMCharacter* externalString,
                                wchar_t* internalString,
                                size_t characterCount);

  static void externalizeString(const wchar_t* internalString,
                                OMCharacter* externalString,
                                size_t characterCount);

  static void reorderUInt16Array(OMUInt16* array,
                                 size_t elementCount);

  static void internalizeUInt16Array(OMUInt16* externalArray,
                                     OMUInt16* internalArray,
                                     size_t elementCount);

  static void externalizeUInt16Array(const OMUInt16* internalArray,
                                     OMUInt16* externalArray,
                                     size_t elementCount);

private:
  // @access Private members.

  static OMStoredObject* openFile(const wchar_t* fileName,
                                  const OMFile::OMAccessMode mode);
  static OMStoredObject* createFile(const wchar_t* fileName);
  static OMStoredObject* openFile(OMRawStorage* rawStorage,
                                  const OMFile::OMAccessMode mode);
  static OMStoredObject* createFile(OMRawStorage* rawStorage);

  void create(const OMByteOrder byteOrder);
  void open(const OMFile::OMAccessMode mode);

  void save(OMStoredPropertySetIndex *index);
  OMStoredPropertySetIndex* restore(void);

    // @cmember The stream name for the index of a collection
    //          named <p collectionName>.
  wchar_t* collectionIndexStreamName(const wchar_t* collectionName);

  IStream* createStream(IStorage* storage, const wchar_t* streamName);
  IStream* openStream(IStorage* storage, const wchar_t* streamName);

  IStorage* createStorage(IStorage* storage, const wchar_t* storageName);
  IStorage* openStorage(IStorage* storage,
                        const wchar_t* storageName,
                        const OMFile::OMAccessMode mode);
  void closeStorage(IStorage*& storage);

  void setClass(IStorage* storage, const OMClassId& cid);
  void getClass(IStorage* storage, OMClassId& cid);

  IStorage* _storage;
  OMStoredPropertySetIndex* _index;
  IStream* _properties;
  size_t _offset;

  bool _open;
  OMFile::OMAccessMode _mode;
  OMByteOrder _byteOrder;
  bool _reorderBytes;

#if defined(OM_ENABLE_DEBUG)
  static size_t _openStorages;
  static size_t _openStreams;
#endif
};

#endif

/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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

#include "OMPortability.h"
#include "OMDataTypes.h"
#include "OMFile.h"

#include <stddef.h>

struct IStorage;
struct IStream;

class OMStoredPropertySetIndex;
class OMProperty;
class OMPropertySet;
class OMStoredVectorIndex;
class OMStoredSetIndex;

  // @class In-memory representation of a persistent object.
class OMStoredObject {
public:
  // @access Static members.

    // @cmember Open the root <c OMStoredObject> in the disk file
    //          <p fileName> for reading only.
  static OMStoredObject* openRead(const wchar_t* fileName);

    // @cmember Open the root <c OMStoredObject> in the disk file
    //          <p fileName> for modification.
  static OMStoredObject* openModify(const wchar_t* fileName);

    // @cmember Create a new root <c OMStoredObject> in the disk file
    //          <p fileName>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMStoredObject* createModify(const wchar_t* fileName,
                                      const OMByteOrder byteOrder);

  // @access Public members.

    // @cmember Constructor.
  OMStoredObject(IStorage* s);

    // @cmember Destructor.
  ~OMStoredObject(void);

    // @cmember Create a new <c OMStoredObject>, named <p name>,
    //          contained by this <c OMStoredObject>.
  OMStoredObject* create(const char* name);

    // @cmember Open an exsiting <c OMStoredObject>, named <p name>,
    //          contained by this <c OMStoredObject>.
  OMStoredObject* open(const char* name);

  OMStoredObject* openStoragePath(const char* storagePathName);

    // @cmember Close this <c OMStoredObject>.
  void close(void);

    // @cmember Save the <c OMClassId> <p cid> in this <c OMStoredObject>.
  void save(const OMClassId& cid);

    // @cmember Restore the class id of this <c OMStoredObject>.
  void restore(OMClassId& cid);

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMStoredObject>.
  void save(const OMPropertySet& properties);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMStoredObject>.
  void restore(OMPropertySet& properties);

    // @cmember Check that the <c OMPropertySet> <p propertySet> is
    //          consistent with the <c OMStoredPropertySetIndex>
    //          propertySetIndex.
  void validate(const OMPropertySet* propertySet,
                const OMStoredPropertySetIndex* propertySetIndex) const;

    // @cmember Save the <c OMStoredVectorIndex> <p vector> in this
    //          <c OMStoredObject>, the vector is named <p vectorName>.
  void save(const OMStoredVectorIndex* vector, const char* vectorName);

    // @cmember Save the <c OMStoredSetIndex> <p set> in this
    //          <c OMStoredObject>, the set is named <p setName>.
  void save(const OMStoredSetIndex* set, const char* setName);

    // @cmember Restore the vector named <p vectorName> into this
    //          <c OMStoredObject>.
  void restore(OMStoredVectorIndex*& vector, const char* vectorName);

    // @cmember Restore the set named <p setName> into this
    //          <c OMStoredObject>.
  void restore(OMStoredSetIndex*& set, const char* setName);

    // @cmember Write a property value to this <c OMStoredObject>. The
    //          property value to be written occupies <p size> bytes at
    //          the address <p start>. The property id is <p propertyId>.
    //          The property type is <p type>.
  void write(OMPropertyId propertyId, int type, void* start, size_t size);

    // @cmember Read a property value from this <c OMStoredObject>.
    //          The property value is read into a buffer which occupies
    //          <p size> bytes at the address <p start>. The property id
    //          is <p propertyId>. The property type is <p type>.
  void read(OMPropertyId propertyId, int type, void* start, size_t size);

    // @cmember Open a stream called <p streamName> contained within
    //          this <c OMStoredObject>.
  IStream* openStream(const char* streamName);

    // @cmember Create a stream called <p streamName> contained within
    //          this <c OMStoredObject>.
  IStream* createStream(const char* streamName);

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

    // @cmember Read a UniqueObjectIdentification from <p stream> into <p id>.
    //          If <p reorderBytes> is true then the bytes are reordered.
  void readUniqueObjectIdentificationFromStream(
                                              IStream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes);

    // @cmember Reorder the UniqueObjectIdentification <p id>.
  static void reorderUniqueObjectIdentification(
                                             OMUniqueObjectIdentification& id);

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

  OMByteOrder byteOrder(void) const;

private:
  // @access Private members.

  static OMStoredObject* open(const wchar_t* fileName,
                              const OMFile::OMAccessMode mode);
  static OMStoredObject* create(const wchar_t* fileName);

  void create(const OMByteOrder byteOrder);
  void open(const OMFile::OMAccessMode mode);

  void save(OMStoredPropertySetIndex *index);
  OMStoredPropertySetIndex* restore(void);
  
    // @cmember The stream name for the index of a vector named <p vectorName>.
  char* vectorIndexStreamName(const char* vectorName);

    // @cmember The stream name for the index of a set named <p setName>.
  char* setIndexStreamName(const char* setName);

  IStream* createStream(IStorage* storage, const char* streamName);
  IStream* openStream(IStorage* storage, const char* streamName);

  IStorage* createStorage(IStorage* storage, const char* storageName);
  IStorage* openStorage(IStorage* storage,
                        const char* storageName,
                        const OMFile::OMAccessMode mode);
  void closeStorage(IStorage*& storage);

  void setClass(IStorage* storage, const OMClassId& cid);
  void getClass(IStorage* storage, OMClassId& cid);

  IStorage* _storage;
  OMStoredPropertySetIndex* _index;
  IStream* _indexStream;
  IStream* _propertiesStream;
  size_t _offset;

  bool _open;
  OMFile::OMAccessMode _mode;
  OMByteOrder _byteOrder;
  bool _reorderBytes;
};

#endif

// @doc OMEXTERNAL
#ifndef OMSTOREDOBJECT_H
#define OMSTOREDOBJECT_H

#include "OMPortability.h"
#include "OMTypes.h"

#include <stddef.h>

struct IStorage;
struct IStream;

class OMStoredPropertySetIndex;
class OMProperty;
class OMPropertySet;
class OMStoredVectorIndex;

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
    //          <p fileName>.
  static OMStoredObject* createModify(const wchar_t* fileName);

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

    // @cmember Save the <c OMPropertSet> <p p properties> in this
    //          <c OMStoredObject>.
  void save(const OMPropertySet& properties);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMStoredObject>.
  void restore(OMPropertySet& properties);

    // @cmember Save the <c OMStoredVectorIndex> <p vector> in this
    //          <c OMStoredObject>, the vector is named <p vectorName>.
  void save(const OMStoredVectorIndex* vector, const char* vectorName);

    // @cmember Restore the vector named <p vectorName> into this
    //          <c OMStoredObject>.
  void restore(OMStoredVectorIndex*& vector, const char* vectorName);

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

    // @cmember Write <p size> bytes from the buffer at address
    //          <p data> to <p stream>.
  void writeToStream(IStream* stream, void* data, size_t size);

    // @cmember Read a UInt32 from <p stream> into <p i>. If <p
    //          _reorderBytes> is true then the bytes are reordered.
  void readUInt32FromStream(IStream* stream, OMUInt32& i, bool _reorderBytes);

    // @cmember Reorder the UInt32 <p i>.
  void reorderOMUInt32(OMUInt32& i);

    // @cmember Size of <p stream> in bytes.
  OMUInt64 streamSize(IStream* stream) const;

    // @cmember Set the size, in bytes, of <p stream>
  void streamSetSize(IStream* stream, const OMUInt64 newSize);

    // @cmember The current position for <f readFromStream()> and
    //          <f writeToStream()>, as an offset in bytes from the
    //          begining of the data stream.
    // @this const
  OMUInt64 streamPosition(IStream* stream) const;

    // @cmember Set the current position for <f readFromStream()> and
    //          <f writeToStream()>, as an offset in bytes from the
    //          begining of the data stream.
  void streamSetPosition(IStream* stream, const OMUInt64 offset);

    // @cmember Close <p stream>.
  void closeStream(IStream*& stream);

private:

  static OMStoredObject* open(const wchar_t* fileName,
                              const OMAccessMode mode);
  static OMStoredObject* create(const wchar_t* fileName);

  void create(void);
  void open(const OMAccessMode mode);

  void save(OMStoredPropertySetIndex *index);
  OMStoredPropertySetIndex* restore(void);
  
  char* vectorIndexStreamName(const char* vectorName);

  IStream* createStream(IStorage* storage, const char* streamName);
  IStream* openStream(IStorage* storage, const char* streamName);

  IStorage* createStorage(IStorage* storage, const char* storageName);
  IStorage* openStorage(IStorage* storage,
                        const char* storageName,
                        const OMAccessMode mode);
  void closeStorage(IStorage*& storage);

  void setClass(IStorage* storage, const OMClassId& cid);
  void getClass(IStorage* storage, OMClassId& cid);

  IStorage* _storage;
  OMStoredPropertySetIndex* _index;
  IStream* _indexStream;
  IStream* _propertiesStream;
  size_t _offset;

  bool _open;
  OMAccessMode _mode;
  OMByteOrder _byteOrder;
  bool _reorderBytes;
};

#endif

#ifndef OMSTOREDOBJECT_H
#define OMSTOREDOBJECT_H

#include "OMPortability.h"
#include "OMTypes.h"

struct IStorage;
struct IStream;

class OMStoredPropertySetIndex;
class OMProperty;
class OMPropertySet;
class OMStoredVectorIndex;

class OMStoredObject {
public:
  OMStoredObject(IStorage* s);

  static OMStoredObject* create(const char* fileName);
  static OMStoredObject* open(const char* fileName);
  OMStoredObject* openStoragePath(const char* storagePathName);

  void save(OMProperty* p);
  void restore(OMPropertySet& properties);
  void close(void);
  void write(int pid, int type, void* start, size_t size);
  void read(int pid, int type, void* start, size_t size);
  void saveClassId(const OMClassId& cid);
  OMClassId restoreClassId(void);
  OMStoredObject* createSubStorage(const char* name);
  OMStoredObject* openSubStorage(const char* name);
  void save(const OMStoredVectorIndex* index, const char* vectorName);
  OMStoredVectorIndex* restore(const char* vectorName);
private:

  void create(void);
  void open(void);

  void save(OMStoredPropertySetIndex *index);
  OMStoredPropertySetIndex* restore(void);
  
  char* vectorIndexStreamName(const char* vectorName);

  IStream* createStream(IStorage* storage, const char* streamName);
  IStream* openStream(IStorage* storage, const char* streamName);
  void closeStream(IStream*& stream);
  void writeToStream(IStream* stream, void* data, size_t size);
  void readFromStream(IStream* stream, void* data, size_t size);

  IStorage* createStorage(IStorage* storage, const char* storageName);
  IStorage* openStorage(IStorage* storage, const char* storageName);
  void closeStorage(IStorage*& storage);
  size_t streamOffset(IStream* stream);
  void streamSeek(IStream* stream, size_t offset);

  void setClass(IStorage* storage, const OMClassId& cid);
  void getClass(IStorage* storage, OMClassId& cid);

  IStorage* _storage;
  OMStoredPropertySetIndex* _index;
  IStream* _indexStream;
  IStream* _propertiesStream;
  size_t _offset;

  bool _open;
};

#endif

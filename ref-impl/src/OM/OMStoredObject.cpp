#include "OMStoredObject.h"
#include "OMStoredPropertySetIndex.h"
#include "OMProperty.h"
#include "OMPropertySet.h"
#include "OMStoredVectorIndex.h"

#include "OMAssertions.h"
#include "OMUtilities.h"

OMStoredObject::OMStoredObject(struct IStorage* s)
: _storage(s), _index(0), _indexStream(0), _propertiesStream(0),
  _offset(0), _classId(0), _open(false)
{
}

void OMStoredObject::save(OMProperty* p)
{
  TRACE("OMStoredObject::save(OMProperty*)");
  PRECONDITION("Already open", _open);
  PRECONDITION("Valid property", p != 0);

  p->saveTo(*this);
}

void OMStoredObject::save(OMStoredPropertySetIndex* index)
{
  TRACE("OMStoredObject::save(OMStoredPropertySetIndex*)");
  PRECONDITION("Already open", _open);
  PRECONDITION("Sorted index", index->isSorted());

  // Write version number.
  //
  int version = 1;
  writeToStream(_indexStream, &version, sizeof(version));
  
  // Write count of entries.
  //
  size_t entries = index->entries();
  writeToStream(_indexStream, &entries, sizeof(entries));
  
  // Write entries.
  //
  int pid;
  int type;
  size_t offset;
  size_t length;
  size_t context = 0;
  for (size_t i = 0; i < entries; i++) {
    index->iterate(context, pid, type, offset, length);
    writeToStream(_indexStream, &pid, sizeof(pid));
    writeToStream(_indexStream, &type, sizeof(type));
    writeToStream(_indexStream, &offset, sizeof(length));
    writeToStream(_indexStream, &length, sizeof(length));
  }
  index->clearDirty();
}

OMStoredPropertySetIndex* OMStoredObject::restore(void)
{
  TRACE("OMStoredObject::restore");
  PRECONDITION("Already open", _open);
  PRECONDITION("At start of index stream", streamOffset(_indexStream) == 0);

  // Read version number.
  //
  int version;
  readFromStream(_indexStream, &version, sizeof(version));
  ASSERT("Valid version number", version > 0);
  ASSERT("Recognized version number", version == 1);
  
  // Read count of entries.
  //
  size_t entries;
  readFromStream(_indexStream, &entries, sizeof(size_t));
  OMStoredPropertySetIndex* index = new OMStoredPropertySetIndex(entries);
  ASSERT("Valid number of entries", entries >= 0);
  
  // Read entries.
  //
  int pid;
  int type;
  size_t offset;
  size_t length;
  for (size_t i = 0; i < entries; i++) {
    readFromStream(_indexStream, &pid, sizeof(pid));
    readFromStream(_indexStream, &type, sizeof(type));
    readFromStream(_indexStream, &offset, sizeof(offset));
    readFromStream(_indexStream, &length, sizeof(length));
    index->insert(pid, type, offset, length);
  }
  index->clearDirty();
  
  POSTCONDITION("Sorted index", index->isSorted());
  return index;
}

void OMStoredObject::restore(OMPropertySet& properties)
{
  TRACE("OMStoredObject::restore");
  PRECONDITION("Already open", _open);
  PRECONDITION("At start of properties stream", streamOffset(_propertiesStream) == 0);

  size_t entries = _index->entries();
  
  int pid;
  int type;
  size_t offset;
  size_t length;
  size_t context = 0;
  for (size_t i = 0; i < entries; i++) {
    _index->iterate(context, pid, type, offset, length);
    OMProperty* p = properties.get(pid);
    ASSERT("Valid property", p != 0);
    p->restoreFrom(*this, length);
  }
  
}

void OMStoredObject::create(void)
{
  TRACE("OMStoredObject::create");
  PRECONDITION("Not already open", !_open);

  _index = new OMStoredPropertySetIndex(50);
  _indexStream = createStream(_storage, "property index");
  _propertiesStream = createStream(_storage, "properties");
  _open = true;
}

void OMStoredObject::open(void)
{
  TRACE("OMStoredObject::open");
  PRECONDITION("Not already open", !_open);

  _indexStream = openStream(_storage, "property index");
  _propertiesStream = openStream(_storage, "properties");
  _open = true;
  _index = restore();
}

void OMStoredObject::close(void)
{
  TRACE("OMStoredObject::close");
  PRECONDITION("Already open", _open);

  if (_index->isDirty()) {
    save(_index);
    _index->clearDirty();
  }
  closeStream(_indexStream);
  closeStream(_propertiesStream);
  
  HRESULT resultCode = _storage->Release();
  _storage = 0;
  ASSERT("Reference count is 0.", resultCode == 0);
  _open = false;
}

void OMStoredObject::write(int pid, int type, void* start, size_t size)
{
  TRACE("OMStoredObject::write");

  _index->insert(pid, type, _offset, size);

  // Write property value.
  //
  writeToStream(_propertiesStream, start, size);
  _offset += size;
}

void OMStoredObject::read(int pid, int type, void* start, size_t size)
{
  TRACE("OMStoredObject::read");

  // read property value
  readFromStream(_propertiesStream, start, size);
}

void OMStoredObject::saveClassId(const int cid)
{
  TRACE("OMStoredObject::saveClassId");

  _classId = cid;
  setClass(_storage, _classId);
}

int OMStoredObject::restoreClassId(void)
{
  TRACE("OMStoredObject::restoreClassId");

  getClass(_storage, _classId);
  return _classId;
}

OMStoredObject* OMStoredObject::createSubStorage(const char* name)
{
  TRACE("OMStoredObject::createSubStorage");

  IStorage* newStorage = createStorage(_storage, name);
  OMStoredObject* result = new OMStoredObject(newStorage);
  return result;
}

OMStoredObject* OMStoredObject::openSubStorage(const char* name)
{
  TRACE("OMStoredObject::openSubStorage");

  IStorage* newStorage = openStorage(_storage, name);
  OMStoredObject* result = new OMStoredObject(newStorage);
  return result;
}

void OMStoredObject::save(const OMStoredVectorIndex* index, const char* vectorName)
{
  TRACE("OMStoredObject::save");

  // calculate the stream name for the index
  //
  char* vectorIndexName = vectorIndexStreamName(vectorName);

  // create the stream
  //
  IStream* vectorIndexStream = createStream(_storage, vectorIndexName);

  // write the high water mark
  //
  size_t highWaterMark = index->highWaterMark();
  writeToStream(vectorIndexStream, &highWaterMark, sizeof(highWaterMark));

  // write the count of elements
  //
  size_t entries = index->entries();
  writeToStream(vectorIndexStream, &entries, sizeof(entries));

  // write the element names
  //
  size_t context = 0;
  size_t name;
  for (size_t i = 0; i < entries; i++) {
    index->iterate(context, name);
    writeToStream(vectorIndexStream, &name, sizeof(name));
  }

  // close the stream
  //
  closeStream(vectorIndexStream);
}

OMStoredVectorIndex* OMStoredObject::restore(const char* vectorName)
{
  TRACE("OMStoredObject::restore");
  
  // calculate the stream name for the index
  //
  char* vectorIndexName = vectorIndexStreamName(vectorName);

  // open the stream
  //
  IStream* vectorIndexStream = openStream(_storage, vectorIndexName);

  // read the high water mark
  //
  size_t highWaterMark;
  readFromStream(vectorIndexStream, &highWaterMark, sizeof(highWaterMark));

  // read the count of elements
  //
  size_t entries;
  readFromStream(vectorIndexStream, &entries, sizeof(entries));

  // create an index
  //
  OMStoredVectorIndex* vectorIndex = new OMStoredVectorIndex(entries);

  // read the element names, placing them in the index
  //
  for (size_t i = 0; i < entries; i++) {
    size_t name;
    readFromStream(vectorIndexStream, &name, sizeof(name));
    vectorIndex->insert(i, name);
  }
  return vectorIndex;
}

char* OMStoredObject::vectorIndexStreamName(const char* vectorName)
{
  char* suffix = " index";
  char* vectorIndexName = new char[strlen(vectorName) + strlen(suffix) + 1];
  strcpy(vectorIndexName, vectorName);
  strcat(vectorIndexName, suffix);

  return vectorIndexName;
}

IStream* OMStoredObject::createStream(IStorage* storage, const char* streamName)
{
  TRACE("OMStoredObject::createStream");
  PRECONDITION("Valid storage", storage != 0);

  IStream* stream;
  wchar_t wcStreamName[256];
  convert(wcStreamName, 256, streamName);

  HRESULT resultCode = storage->CreateStream(
    wcStreamName,
    STGM_DIRECT | STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
    0,
    0,
    &stream);
  if (!checkStream(resultCode, streamName)) {
    exit(FAILURE);
  }

  return stream;
}

IStream* OMStoredObject::openStream(IStorage* storage, const char* streamName)
{
  TRACE("OMStoredObject::openStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validString(streamName));
  
  IStream* stream;
  wchar_t wcStreamName[256];
  convert(wcStreamName, 256, streamName);
  
  HRESULT resultCode = storage->OpenStream(
    wcStreamName,
    0,
    STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE,
    0,
    &stream);
  if (!checkStream(resultCode, streamName)) {
    exit(FAILURE);
  }
  
  return stream;
  
}

void OMStoredObject::closeStream(IStream*& stream)
{
  TRACE("OMStoredObject::closeStream");
  PRECONDITION("Valid stream", stream != 0);

  HRESULT resultCode = stream->Release();
  stream = 0;
  ASSERT("Reference count is 0.", resultCode == 0);
}

IStorage* OMStoredObject::createStorage(IStorage* storage, const char* storageName)
{
  TRACE("createStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validString(storageName));

  IStorage* newStorage;
  wchar_t wcStorageName[256];
  convert(wcStorageName, 256, storageName);

  HRESULT resultCode = storage->CreateStorage(
    wcStorageName,
    // STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
    0,
    0,
    &newStorage);
  if (!checkStorage(resultCode, storageName)) {
    newStorage = 0;
  }

  return newStorage;
}

IStorage* OMStoredObject::openStorage(IStorage* storage, const char* storageName)
{
  TRACE("openStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validString(storageName));

  IStorage* newStorage;
  wchar_t wcStorageName[256];
  convert(wcStorageName, 256, storageName);

  HRESULT resultCode = storage->OpenStorage(
    wcStorageName,
    0,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
    0,
    0,
    &newStorage);
  if (!checkStorage(resultCode, storageName)) {
    exit(FAILURE);
  }

  return newStorage;
}

void OMStoredObject::closeStorage(IStorage*& storage)
{
  TRACE("closeStorage");
  PRECONDITION("Valid storage", storage != 0);

  HRESULT resultCode = storage->Release();
  storage = 0;
  ASSERT("Reference count is 0.", resultCode == 0);
}

void OMStoredObject::writeToStream(IStream* stream, void* data, size_t size)
{
  TRACE("OMStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  HRESULT resultCode = stream->Write(data, size, NULL);
  if (!check(resultCode)) {
    exit(FAILURE);
  }
}

void OMStoredObject::readFromStream(IStream* stream, void* data, size_t size)
{
  TRACE("OMStoredObject::readFromStream");

  unsigned long bytesRead;
  HRESULT result = stream->Read(data, size, &bytesRead);
  if (!check(result)) {
    exit(FAILURE);
  }
  ASSERT("Successful read", bytesRead == size);
}

void OMStoredObject::setClass(IStorage* storage, int cid)
{
  TRACE("OMStoredObject::setClass");
  PRECONDITION("Valid storage", storage != 0);

  struct _GUID g = {0,0,0, {0, 0, 0, 0, 0, 0, 0, 0}};
  g.Data4[7] = cid;
  HRESULT resultCode = storage->SetClass(g);
  if (!check(resultCode)) {
    exit(FAILURE);
  }

}

void OMStoredObject::getClass(IStorage* storage, int& cid)
{
  TRACE("OMStoredObject::getClass");

  STATSTG statstg;
  HRESULT result = storage->Stat(&statstg, STATFLAG_NONAME);
  if (!check(result)) {
    exit(FAILURE);
  }
  cid = statstg.clsid.Data4[7];
}

size_t OMStoredObject::streamOffset(IStream* stream)
{
  TRACE("OMStoredObject::streamOffset");

  size_t result;
  LARGE_INTEGER zero = {0, 0};
  ULARGE_INTEGER position;
  HRESULT status = stream->Seek(zero, STREAM_SEEK_CUR, &position);
  if (!check(status)) {
  }
  ASSERT("Small stream", position.HighPart == 0);
  result = position.LowPart;
  return result;
}

void OMStoredObject::streamSeek(IStream* stream, size_t offset)
{
  TRACE("OMStoredObject::streamSeek");

  LARGE_INTEGER newPosition = {0, offset};
  ULARGE_INTEGER oldPosition;
  HRESULT status = stream->Seek(newPosition, STREAM_SEEK_SET, &oldPosition);
  if (!check(status)) {
  }
  ASSERT("Small stream", oldPosition.HighPart == 0);
}

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

// @doc OMEXTERNAL
#include "OMStoredObject.h"
#include "OMStoredPropertySetIndex.h"
#include "OMProperty.h"
#include "OMPropertySet.h"
#include "OMStoredVectorIndex.h"
#include "OMStoredSetIndex.h"
#include "OMDataTypes.h"
#include "OMPropertyTable.h"

#include "OMAssertions.h"
#include "OMUtilities.h"

#include <iostream.h>

#if defined(_MAC) || defined(macintosh)
#include "wintypes.h"
#include <storage.h>
#elif defined(__sgi) || defined(__linux__) || defined (__FreeBSD__)
#include "storage.h"
#else
#include <objbase.h>
#endif

const OMVersion currentVersion = 25;

const size_t indexHeaderSize = sizeof(OMByteOrder) +
                               sizeof(OMVersion) +
                               sizeof(OMPropertyCount);

const size_t indexEntrySize  = sizeof(OMPropertyId) +
                               sizeof(OMStoredForm) +
                               sizeof(OMPropertySize);

#if defined(_MAC) || defined(macintosh) || \
    defined(__sgi) || defined(__linux__) || defined (__FreeBSD__)

// The Macintosh and Unix (SS reference implementation) declarations
// for LARGE_INTEGER and ULARGE_INTEGER don't have a QuadPart.
// On Macintosh this is probably because the HighPart and LowPart
// components on that platform are not in the natural platform order.

static inline OMUInt64 toOMUInt64(const ULARGE_INTEGER &x)
{
  OMUInt64 result = (((OMUInt64)x.HighPart) << 32) + (x.LowPart);
  return result;
}

static inline ULARGE_INTEGER fromOMUInt64(const OMUInt64& x)
{
  ULARGE_INTEGER result;
  result.HighPart = (unsigned long)(((OMUInt64)x) >> 32);
  result.LowPart  = (unsigned long) (0x00000000FFFFFFFF & (OMUInt64)x);
  return result;
}

#else

static inline OMUInt64 toOMUInt64(const ULARGE_INTEGER &x)
{
  OMUInt64 result;
  result = x.QuadPart;
  return result;
}


static inline ULARGE_INTEGER fromOMUInt64(const OMUInt64& x)
{
  ULARGE_INTEGER result;
  result.QuadPart = x;
  return result;
}

#endif

#if defined(_WIN32) && defined(UNICODE)
  typedef wchar_t OMCHAR;
#else
  typedef char OMCHAR;
#endif

#if defined(_WIN32) && defined(UNICODE)
static void convert(wchar_t* wcName, size_t length, const char* name);
#endif

static void convert(char* cName, size_t length, const wchar_t* name);

static void convert(char* cName, size_t length, const char* name);

#if defined(_WIN32) && defined(UNICODE)
static void convert(wchar_t* wcName, size_t length, const wchar_t* name);
#endif

static void formatError(DWORD errorCode);

static void check(HRESULT resultCode);

static void checkFile(HRESULT resultCode, const wchar_t* fileName);

static void checkStream(HRESULT resultCode, const char* streamName);

static void checkStorage(HRESULT resultCode, const char* storageName);

static void printError(const char* prefix, const char* type);

static void printName(const char* name);

static void printName(const wchar_t* name);

  // @mfunc Constructor.
  //   @parm The IStorage for the persistent representation of
  //         this <c OMStoredObject>.
OMStoredObject::OMStoredObject(IStorage* s)
: _storage(s), _index(0), _properties(0),
  _offset(0), _open(false), _mode(OMFile::readOnlyMode),
  _byteOrder(hostByteOrder()), _reorderBytes(false)
{
  TRACE("OMStoredObject::OMStoredObject");
}

  // @mfunc Destructor.
OMStoredObject::~OMStoredObject(void)
{
  TRACE("OMStoredObject::~OMStoredObject");
}

  // @mfunc Open the root <c OMStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMStoredObject> representing the root object in
  //          the disk file.
OMStoredObject* OMStoredObject::openRead(const wchar_t* fileName)
{
  TRACE("OMStoredObject::openRead");
  PRECONDITION("Valid file name", validWideString(fileName));

  OMStoredObject* newStore = OMStoredObject::openFile(fileName,
                                                      OMFile::readOnlyMode);
  newStore->open(OMFile::readOnlyMode);

  return newStore;
}

  // @mfunc Open the root <c OMStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMStoredObject> representing the root object in
  //          the disk file.
OMStoredObject* OMStoredObject::openModify(const wchar_t* fileName)
{
  TRACE("OMStoredObject::openModify");
  PRECONDITION("Valid file name", validWideString(fileName));

  OMStoredObject* newStore = OMStoredObject::openFile(fileName,
                                                      OMFile::modifyMode);
  newStore->open(OMFile::modifyMode);

  return newStore;
}

  // @mfunc Create a new root <c OMStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMStoredObject> representing the root object in
  //          the disk file.
OMStoredObject* OMStoredObject::createModify(const wchar_t* fileName,
                                             const OMByteOrder byteOrder)
{
  TRACE("OMStoredObject::createModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));

  OMStoredObject* newStore = OMStoredObject::createFile(fileName);
  newStore->create(byteOrder);

  return newStore;
}

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMStoredObject::save(const OMPropertySet& properties)
{
  TRACE("OMStoredObject::save(OMPropertySet)");
  PRECONDITION("Already open", _open);
  PRECONDITION("At start of value stream",
                                             streamPosition(_properties) == 0);
  PRECONDITION("At start of value stream", _offset == 0);

  size_t count = properties.count();
  delete _index;
  _index = 0; // for BoundsChecker
  _index = new OMStoredPropertySetIndex(count);
  ASSERT("Valid heap pointer", _index != 0);
  size_t countPresent = properties.countPresent();
  size_t indexSize = indexHeaderSize + (countPresent * indexEntrySize);
  streamSetPosition(_properties, indexSize);
  _offset = indexSize;
  size_t context = 0;
  for (size_t i = 0; i < count; i++) {
    OMProperty* p = 0;
    properties.iterate(context, p);
    ASSERT("Valid property", p != 0);
    if (!p->isOptional() || p->isPresent()) {
      p->save();
    }
  }
#if !defined(OM_DISABLE_VALIDATE)
  validate(&properties, _index);
#endif
  streamSetPosition(_properties, 0);
  save(_index);
  delete _index;
  _index = 0;
  _offset = 0;
  streamSetPosition(_properties, 0);
  POSTCONDITION("At start of properties stream",
                                             streamPosition(_properties) == 0);
  POSTCONDITION("At start of value stream", _offset == 0);
}

void OMStoredObject::save(OMStoredPropertySetIndex* index)
{
  TRACE("OMStoredObject::save(OMStoredPropertySetIndex*)");
  PRECONDITION("Already open", _open);
  PRECONDITION("Valid index", index != 0);
  PRECONDITION("At start of index stream", streamPosition(_properties) == 0);

  // The number of entries in the index.
  //
  OMPropertyCount entries = index->entries();
  ASSERT("Valid index",
                 index->isValid(indexHeaderSize + (entries * indexEntrySize)));

  // Write byte order flag.
  //
  ASSERT("Index in native byte order", _byteOrder == hostByteOrder());
  writeToStream(_properties, &_byteOrder, sizeof(_byteOrder));

  // Write version number.
  //
  OMVersion version = currentVersion;
  writeToStream(_properties, &version, sizeof(version));
  
  // Write count of entries.
  //
  writeToStream(_properties, &entries, sizeof(entries));
  
  // Write entries.
  //
  OMPropertyId propertyId;
  OMStoredForm type;
  OMUInt32 offset;
  OMPropertySize length;
  size_t context = 0;
  for (size_t i = 0; i < entries; i++) {
    index->iterate(context, propertyId, type, offset, length);
    writeToStream(_properties, &propertyId, sizeof(propertyId));
    writeToStream(_properties, &type, sizeof(type));
    writeToStream(_properties, &length, sizeof(length));
  }

  streamSetPosition(_properties, 0);
  POSTCONDITION("At start of index stream", streamPosition(_properties) == 0);
}

OMStoredPropertySetIndex* OMStoredObject::restore(void)
{
  TRACE("OMStoredObject::restore");
  PRECONDITION("Already open", _open);
  PRECONDITION("At start of index stream", streamPosition(_properties) == 0);

  // Read byte order flag.
  //
  readFromStream(_properties, &_byteOrder, sizeof(_byteOrder));
  if (_byteOrder == hostByteOrder()) {
    _reorderBytes = false;
  } else {
    _reorderBytes = true;
  }

  // Read version number.
  //
  OMVersion version;
  readUInt8FromStream(_properties, version);
  ASSERT("Recognized version number", version == currentVersion);
  
  // Read count of entries.
  //
  OMPropertyCount entries;
  readUInt16FromStream(_properties, entries, _reorderBytes);
  OMStoredPropertySetIndex* index = new OMStoredPropertySetIndex(entries);
  ASSERT("Valid heap pointer", index != 0);

  // Read entries.
  //
  OMPropertyId propertyId;
  OMStoredForm type;
  OMUInt32 offset = indexHeaderSize + (entries * indexEntrySize);
  OMPropertySize length;
  for (size_t i = 0; i < entries; i++) {
    readUInt16FromStream(_properties, propertyId, _reorderBytes);
    readUInt16FromStream(_properties, type, _reorderBytes);
    readUInt16FromStream(_properties, length, _reorderBytes);
    index->insert(propertyId, type, offset, length);
    offset = offset + length;
  }
  
  POSTCONDITION("Valid index",
                 index->isValid(indexHeaderSize + (entries * indexEntrySize)));
  return index;
}

  // @mfunc Restore the <c OMPropertySet> <p properties> into
  //        this <c OMStoredObject>.
  //   @parm The <c OMPropertySet> to restore.
void OMStoredObject::restore(OMPropertySet& properties)
{
  TRACE("OMStoredObject::restore");
  PRECONDITION("Already open", _open);

  size_t entries = _index->entries();
  
  OMPropertyId propertyId;
  OMStoredForm type;
  OMUInt32 offset;
  OMPropertySize length;
  size_t context = 0;
  for (size_t i = 0; i < entries; i++) {
    _index->iterate(context, propertyId, type, offset, length);
    OMProperty* p = properties.get(propertyId);
    ASSERT("Valid property", p != 0);
    p->restore(length);
    ASSERT("Property is present", IMPLIES(p->isOptional(), p->isPresent()));
  }
#if !defined(OM_DISABLE_VALIDATE)
  validate(&properties, _index);
#endif
  streamSetPosition(_properties, 0);
  POSTCONDITION("At start of properties stream",
                                       streamPosition(_properties) == 0);
}

OMStoredObject* OMStoredObject::openFile(const wchar_t* fileName,
                                         const OMFile::OMAccessMode mode)
{
  TRACE("OMStoredObject::openFile");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  DWORD openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STGM_DIRECT | STGM_READ      | STGM_SHARE_DENY_WRITE;
  }

  OMCHAR omFileName[256];
  convert(omFileName, 256, fileName);

  HRESULT result;
  IStorage* storage = 0;

  result = StgOpenStorage(
    omFileName,
    0,
    openMode,
    0,
    0,
    &storage);
  checkFile(result, fileName);

  OMStoredObject* newStoredObject = new OMStoredObject(storage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

OMStoredObject* OMStoredObject::createFile(const wchar_t* fileName)
{
  TRACE("OMStoredObject::createFile");
  PRECONDITION("Valid file name", validWideString(fileName));

  OMCHAR omFileName[256];
  convert(omFileName, 256, fileName);

  HRESULT result;
  IStorage* storage = 0;

  result = StgCreateDocfile(
    omFileName,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_FAILIFTHERE,
    0,
    &storage);
  checkFile(result, fileName);

  OMStoredObject* newStoredObject = new OMStoredObject(storage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

static const char* const propertyStreamName = "properties";
 
void OMStoredObject::create(const OMByteOrder byteOrder)
{
  TRACE("OMStoredObject::create");
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));
  PRECONDITION("Not already open", !_open);

  _byteOrder = byteOrder;
  _mode = OMFile::modifyMode;
  _properties = createStream(_storage, propertyStreamName);
  _open = true;
}

void OMStoredObject::open(const OMFile::OMAccessMode mode)
{
  TRACE("OMStoredObject::open");
  PRECONDITION("Not already open", !_open);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  _mode = mode;
  _properties = openStream(_storage, propertyStreamName);
  _open = true;
  _index = restore();
}

  // @mfunc Close this <c OMStoredObject>.
void OMStoredObject::close(void)
{
  TRACE("OMStoredObject::close");
  PRECONDITION("Already open", _open);

  closeStream(_properties);
  
  delete _index;
  _index = 0;

  closeStorage(_storage);

  _open = false;
}

  // @mfunc Write a property value to this <c OMStoredObject>. The
  //        property value to be written occupies <p size> bytes at
  //        the address <p start>. The property id is <p propertyId>.
  //        The property type is <p type>.
  //   @parm The property id.
  //   @parm The property type.
  //   @parm The start address of the property value.
  //   @parm The size of the property value in bytes.
void OMStoredObject::write(OMPropertyId propertyId,
                           OMStoredForm storedForm,
                           void* start,
                           size_t size)
{
  TRACE("OMStoredObject::write");
  PRECONDITION("Valid data", start != 0);
  PRECONDITION("Valid size", size > 0);

  _index->insert(propertyId, storedForm, _offset, size);

  // Write property value.
  //
  writeToStream(_properties, start, size);
  _offset += size;
}

  // @mfunc Read a property value from this <c OMStoredObject>.
  //        The property value is read into a buffer which occupies
  //        <p size> bytes at the address <p start>. The property id
  //        is <p propertyId>. The property type is <p type>.
  //   @parm OMPropertyId | propertyId | The property id.
  //   @parm int | type | The property type.
  //   @parm void* | start | The start address of the buffer to hold the
  //         property value.
  //   @parm size_t | size | The size of the buffer in bytes.
void OMStoredObject::read(OMPropertyId propertyId,
                          OMStoredForm ANAME(storedForm),
                          void* start,
                          size_t size)
{
  TRACE("OMStoredObject::read");
  PRECONDITION("Valid data", start != 0);
  PRECONDITION("Valid size", size > 0);

  // Consistency check - look up propertyId in _index and check that
  // the property type is the expected (passed in) type, and that the
  // property length is the expected (passed in as size) length.
  //
  OMStoredForm actualType;
  OMUInt32 actualOffset;
  OMPropertySize actualLength;
  bool found = _index->find(propertyId,
                            actualType,
                            actualOffset,
                            actualLength);

  ASSERT("Recognized property", found);
  if (!found) {
    // error illegal property for this object
  }
  ASSERT("Matching property types", storedForm == actualType);
  ASSERT("Matching property sizes", size == actualLength);

  // Since random access is not yet supported, check that the stream
  // is synchronized with the index.
  //
  ASSERT("Sequential access",
         actualOffset == streamPosition(_properties));

  // Read property value.
  //
  readFromStream(_properties, start, size);
}

  // @mfunc Size of <p stream> in bytes.
  //   @parm An open stream.
  //   @rdesc The size of <p stream> in bytes
  //   @this const
OMUInt64 OMStoredObject::streamSize(IStream* stream) const
{
  TRACE("OMStoredObject::streamSize");
  PRECONDITION("Valid stream", stream != 0);

  STATSTG statstg;
  HRESULT status = stream->Stat(&statstg, STATFLAG_NONAME);
  check(status);

  OMUInt64 result = toOMUInt64(statstg.cbSize);
  return result;
}

  // @mfunc Set the size, in bytes, of <p stream>
  //   @parm An open stream.
  //   @parm The new size for the stream.
void OMStoredObject::streamSetSize(IStream* stream, const OMUInt64 newSize)
{
  TRACE("OMStoredObject::streamSetSize");

  ULARGE_INTEGER newStreamSize = fromOMUInt64(newSize);
  HRESULT status = stream->SetSize(newStreamSize);
  check(status);
}

  // @mfunc Open a stream called <p streamName> contained within this
  //        <c OMStoredObject>.
  //   @parm The name of the stream to open.
  //   @rdesc An open stream.
IStream* OMStoredObject::openStream(const char* streamName)
{
  TRACE("OMStoredObject::openStream");
  PRECONDITION("Valid stream name", validString(streamName));

  return openStream(_storage, streamName);
}

  // @mfunc Create a stream called <p streamName> contained within
  //        this <c OMStoredObject>.
  //   @parm The name of the stream to create.
  //   @rdesc An open stream.
IStream* OMStoredObject::createStream(const char* streamName)
{
  TRACE("OMStoredObject::createStream");
  PRECONDITION("Valid stream name", validString(streamName));

  return createStream(_storage, streamName);
}

  // @mfunc Save the <c OMClassId> <p cid> in this <c OMStoredObject>.
  //   @parm The <c OMClassId> of this <c OMStoredObject>.
void OMStoredObject::save(const OMClassId& cid)
{
  TRACE("OMStoredObject::save");

  setClass(_storage, cid);
}

  // @mfunc Restore the class id of this <c OMStoredObject>.
  //   @parm The <c OMClassId> of this <c OMStoredObject>.
void OMStoredObject::restore(OMClassId& cid)
{
  TRACE("OMStoredObject::restore");

  getClass(_storage, cid);
}

  // @mfunc Create a new <c OMStoredObject>, named <p name>,
  //        contained by this <c OMStoredObject>.
  //   @parm The name to be used for the new <c OMStoredObject>.
  //   @rdesc A new <c OMStoredObject> contained by this
  //          <c OMStoredObject>.
OMStoredObject* OMStoredObject::create(const char* name)
{
  TRACE("OMStoredObject::create");
  PRECONDITION("Valid name", validString(name));

  IStorage* newStorage = createStorage(_storage, name);
  OMStoredObject* result = new OMStoredObject(newStorage);
  ASSERT("Valid heap pointer", result != 0);
  result->create(_byteOrder);
  return result;
}

  // @mfunc Open an exsiting <c OMStoredObject>, named <p name>,
  //        contained by this <c OMStoredObject>.
  //   @parm The name of the existing <c OMStoredObject>.
  //   @rdesc The existing <c OMStoredObject> contained by this
  //          <c OMStoredObject>.
OMStoredObject* OMStoredObject::open(const char* name)
{
  TRACE("OMStoredObject::open");
  PRECONDITION("Valid name", validString(name));

  IStorage* newStorage = openStorage(_storage, name, _mode);
  OMStoredObject* result = new OMStoredObject(newStorage);
  ASSERT("Valid heap pointer", result != 0);
  result->open(_mode);
  return result;
}

  // @mfunc Check that the <c OMPropertySet> <p propertySet> is
  //        consistent with the <c OMStoredPropertySetIndex>
  //        propertySetIndex.
  //   @parm The <c OMPropertySet> to validate.
  //   @parm The <c OMStoredPropertySetIndex> to validate.
void OMStoredObject::validate(
                        const OMPropertySet* propertySet,
                        const OMStoredPropertySetIndex* propertySetIndex) const
{
  TRACE("OMStoredObject::validate");
  PRECONDITION("Valid property set", propertySet != 0);
  PRECONDITION("Valid property set index", propertySetIndex != 0);

  OMPropertyId propertyId;
  OMStoredForm type;
  OMUInt32 offset;
  OMPropertySize length;
  size_t context;

  // Check that all required properties are present.
  //
  size_t count = propertySet->count();
  context = 0;
  for (size_t j = 0; j < count; j++) {
    OMProperty* p = 0;
    propertySet->iterate(context, p);
    ASSERT("Valid property", p != 0);
    propertyId = p->propertyId();
    if (!p->isOptional()) {
      bool found = propertySetIndex->find(propertyId, type, offset, length);
      ASSERT("Required property present", found);
      if (!found) {
        // error required property missing
      }
    }
  }

  // Check that there are no spurious properties.
  //
  OMPropertyCount entries = propertySetIndex->entries();
  context = 0;
  for (size_t k = 0; k < entries; k++) {
    propertySetIndex->iterate(context, propertyId, type, offset, length);
    bool allowed = propertySet->isAllowed(propertyId);
    ASSERT("Property allowed", allowed);
    if (!allowed) {
      // error illegal property for this object
    }
  }

}

  // @mfunc  Save the <c OMStoredVectorIndex> <p vector> in this
  //         <c OMStoredObject>, the vector is named <p vectorName>.
  //   @parm The <c OMStoredVectorIndex> to save.
  //   @parm The name of the vector.
void OMStoredObject::save(const OMStoredVectorIndex* vector,
                          const char* vectorName)
{
  TRACE("OMStoredObject::save");
  PRECONDITION("Valid vector", vector != 0);
  PRECONDITION("Valid vector name", validString(vectorName));

  // Calculate the stream name for the index.
  //
  char* vectorIndexName = vectorIndexStreamName(vectorName);

  // Create the stream.
  //
  IStream* vectorIndexStream = createStream(_storage, vectorIndexName);

  // Write the count of elements.
  //
  OMUInt32 entries = vector->entries();
  writeToStream(vectorIndexStream, &entries, sizeof(entries));

  // Write the first free key.
  //
  OMUInt32 firstFreeKey = vector->firstFreeKey();
  writeToStream(vectorIndexStream, &firstFreeKey, sizeof(firstFreeKey));

  // Write the last free key.
  //
  OMUInt32 lastFreeKey = vector->lastFreeKey();
  writeToStream(vectorIndexStream, &lastFreeKey, sizeof(lastFreeKey));

  // For each element write the element name.
  //
  size_t context = 0;
  OMUInt32 name;
  for (size_t i = 0; i < entries; i++) {
    vector->iterate(context, name);
    writeToStream(vectorIndexStream, &name, sizeof(name));
  }

  // Close the stream.
  //
  closeStream(vectorIndexStream);

  delete [] vectorIndexName;
}

  // @mfunc  Save the <c OMStoredSetIndex> <p set> in this
  //         <c OMStoredObject>, the set is named <p setName>.
  //   @parm The <c OMStoredSetIndex> to save.
  //   @parm The name of the set.
void OMStoredObject::save(const OMStoredSetIndex* set,
                          const char* setName)
{
  TRACE("OMStoredObject::save");
  PRECONDITION("Valid set", set != 0);
  PRECONDITION("Valid set name", validString(setName));

  // Calculate the stream name for the index.
  //
  char* setIndexName = setIndexStreamName(setName);

  // Create the stream.
  //
  IStream* setIndexStream = createStream(_storage, setIndexName);

  // Write the count of elements.
  //
  OMUInt32 entries = set->entries();
  writeToStream(setIndexStream, &entries, sizeof(entries));

  // Write the first free key.
  //
  OMUInt32 firstFreeKey = set->firstFreeKey();
  writeToStream(setIndexStream, &firstFreeKey, sizeof(firstFreeKey));

  // Write the last free key.
  //
  OMUInt32 lastFreeKey = set->lastFreeKey();
  writeToStream(setIndexStream, &lastFreeKey, sizeof(lastFreeKey));

  // Write the key pid.
  //
  OMPropertyId pid = set->keyPropertyId();
  writeToStream(setIndexStream, &pid, sizeof(pid));

  // Write the key size.
  //
  OMKeySize keySize = set->keySize();
  writeToStream(setIndexStream, &keySize, sizeof(keySize));

  // For each element write the element name, reference count and key.
  //
  size_t context = 0;
  OMUInt32 name;
  OMUInt32 count;
  OMByte* key = new OMByte[keySize];
  ASSERT("Valid heap pointer", key != 0);
  for (size_t i = 0; i < entries; i++) {
    set->iterate(context, name, count, key);
    writeToStream(setIndexStream, &name, sizeof(name));
    writeToStream(setIndexStream, &count, sizeof(count));
    writeToStream(setIndexStream, key, keySize);
  }

  // Close the stream.
  //
  closeStream(setIndexStream);

  delete [] key;
  delete [] setIndexName;
}

  // @mfunc Save the <c OMPropertyTable> <p table> in this
  //        <c OMStoredObject>.
  //   @parm The table to save.
void OMStoredObject::save(const OMPropertyTable* table)
{
  TRACE("OMPropertyTable::save");

  PRECONDITION("Valid property table", table != 0);

  IStream* stream = createStream("referenced properties");

  // byte order
  ASSERT("Index in native byte order", _byteOrder == hostByteOrder());
  writeToStream(stream, &_byteOrder, sizeof(_byteOrder));

  // count of strings
  OMPropertyCount count = table->count();
  writeToStream(stream, &count, sizeof(count));
 
  // count of characters
  OMUInt32 characterCount = 0;
  for (size_t i = 0; i < count; i++) {
    OMUInt32 length = lengthOfWideString(table->valueAt(i));
    characterCount = characterCount + length + 1;
  }
  writeToStream(stream, &characterCount, sizeof(characterCount));

  // sequence of null terminated strings
  for (size_t j = 0; j < count; j++) {
    const wchar_t* internalName = table->valueAt(j);
    size_t characterCount = lengthOfWideString(internalName);
    size_t byteCount = characterCount * sizeof(OMCharacter);
    OMCharacter* externalName = new OMCharacter[characterCount];
    ASSERT("Valid heap pointer", externalName != 0);
    externalizeString(internalName, externalName, characterCount);
    writeToStream(stream, (void*)externalName, byteCount);
    const OMCharacter null = 0;
    writeToStream(stream, (void*)&null, sizeof(null));
    delete [] externalName;
  }

  closeStream(stream);
}

  // @mfunc Save a single weak reference.
  //   @parm The property id.
  //   @parm The property type.
  //   @parm The unique identification of the target.
  //   @parm A tag identifying the collection in which the target resides.
  //   @parm The id of the property whose value is the unique
  //         identifier of objects in the target set.
void OMStoredObject::save(OMPropertyId propertyId,
                          OMStoredForm storedForm,
                          const OMUniqueObjectIdentification& id,
                          OMPropertyTag tag,
                          OMPropertyId keyPropertyId)
{
  TRACE("OMStoredObject::save");

  // tag, key pid, key size, key
  const size_t size = sizeof(tag) +
                      sizeof(OMPropertyId) + sizeof(OMKeySize) + sizeof(id);
  OMByte buffer[size];
  OMByte* p = &buffer[0];
  memcpy(p, &tag, sizeof(tag));
  p += sizeof(tag);
  memcpy(p , &keyPropertyId, sizeof(keyPropertyId));
  p += sizeof(keyPropertyId);
  OMKeySize keySize = sizeof(id);
  memcpy(p, &keySize, sizeof(keySize));
  p += sizeof(keySize);
  memcpy(p, &id, sizeof(id));

  write(propertyId, storedForm, (void *)buffer, size); 
}

  // @mfunc Save a collection (vector/set) of weak references.
  //   @parm The name of the collection.
  //   @parm The unique identifications of the targets.
  //   @parm Count of targets.
  //   @parm A tag identifying the collection in which each of the
  //         targets reside.
  //   @parm The id of the property whose value is the unique
  //         identifier of objects in the target set.
void OMStoredObject::save(const char* collectionName,
                          const OMUniqueObjectIdentification* index,
                          size_t count,
                          OMPropertyTag tag,
                          OMPropertyId keyPropertyId)
{
  TRACE("OMStoredObject::save");

  PRECONDITION("Valid index", IMPLIES(count != 0, index!= 0));
  PRECONDITION("Valid index", IMPLIES(count == 0, index== 0));
  PRECONDITION("Valid collection name", validString(collectionName));

  // Calculate the stream name for the index.
  //
  char* indexName = setIndexStreamName(collectionName);

  // Create the stream.
  //
  IStream* indexStream = createStream(_storage, indexName);

  // Write the count of elements.
  //
  OMUInt32 entries = count;
  writeToStream(indexStream, &entries, sizeof(entries));

  // Write the tag.
  //
  writeToStream(indexStream, &tag, sizeof(tag));

  // Write the key pid.
  //
  writeToStream(indexStream, &keyPropertyId, sizeof(keyPropertyId));

  // Write key size.
  //
  OMKeySize keySize = sizeof(OMUniqueObjectIdentification);
  writeToStream(indexStream, &keySize, sizeof(keySize));

  if (count > 0) {
    // For each element write the element unique identifier
    //
    writeToStream(indexStream,
                  (void *)index,
                  count * sizeof(OMUniqueObjectIdentification));
  }

  // Close the stream.
  //
  closeStream(indexStream);

  delete [] indexName;
}

  // @mfunc Restore the vector named <p vectorName> into this
  //        <c OMStoredObject>.
  //   @parm The name of the vector.
  //   @rdesc The newly restored <c OMStoredVectorIndex>.
void OMStoredObject::restore(OMStoredVectorIndex*& vector,
                             const char* vectorName)
{
  TRACE("OMStoredObject::restore");
  PRECONDITION("Valid vector name", validString(vectorName));

  // Calculate the stream name for the index.
  //
  char* vectorIndexName = vectorIndexStreamName(vectorName);

  // Open the stream.
  //
  IStream* vectorIndexStream = openStream(_storage, vectorIndexName);

  // Read the count of elements.
  //
  OMUInt32 entries;
  readUInt32FromStream(vectorIndexStream, entries, _reorderBytes);

  // Read the first free key.
  //
  OMUInt32 firstFreeKey;
  readUInt32FromStream(vectorIndexStream, firstFreeKey, _reorderBytes);

  // Read the last free key.
  //
  OMUInt32 lastFreeKey;
  readUInt32FromStream(vectorIndexStream, lastFreeKey, _reorderBytes);

  // Create an index.
  //
  OMStoredVectorIndex* vectorIndex = new OMStoredVectorIndex(entries);
  ASSERT("Valid heap pointer", vectorIndex != 0);

  // Set the first free key.
  //
  vectorIndex->setFirstFreeKey(firstFreeKey);

  // Set the last free key.
  //
  vectorIndex->setLastFreeKey(lastFreeKey);

  // Read the element names, placing them in the index.
  //
  for (size_t i = 0; i < entries; i++) {
    OMUInt32 name;
    readUInt32FromStream(vectorIndexStream, name, _reorderBytes);
    vectorIndex->insert(i, name);
  }

  // Close the stream.
  //
  closeStream(vectorIndexStream);

  delete [] vectorIndexName;

  vector = vectorIndex;
}

  // @mfunc Restore the set named <p setName> into this
  //        <c OMStoredObject>.
  //   @parm The name of the set.
  //   @rdesc The newly restored <c OMStoredSetIndex>.
void OMStoredObject::restore(OMStoredSetIndex*& set,
                             const char* setName)
{
  TRACE("OMStoredObject::restore");
  PRECONDITION("Valid set name", validString(setName));

  // Calculate the stream name for the index.
  //
  char* setIndexName = setIndexStreamName(setName);

  // Open the stream.
  //
  IStream* setIndexStream = openStream(_storage, setIndexName);

  // Read the count of elements.
  //
  OMUInt32 entries;
  readUInt32FromStream(setIndexStream, entries, _reorderBytes);

  // Read the first free key.
  //
  OMUInt32 firstFreeKey;
  readUInt32FromStream(setIndexStream, firstFreeKey, _reorderBytes);

  // Read the last free key.
  //
  OMUInt32 lastFreeKey;
  readUInt32FromStream(setIndexStream, lastFreeKey, _reorderBytes);

  // Read the key pid.
  //
  OMPropertyId keyPid;
  readUInt16FromStream(setIndexStream, keyPid, _reorderBytes);

  // Read the key size.
  //
  OMKeySize keySize;
  readUInt8FromStream(setIndexStream, keySize);

  // Create an index.
  //
  OMStoredSetIndex* setIndex = new OMStoredSetIndex(entries, keyPid, keySize);
  ASSERT("Valid heap pointer", setIndex != 0);

  // Set the first free key.
  //
  setIndex->setFirstFreeKey(firstFreeKey);

  // Set the last free key.
  //
  setIndex->setLastFreeKey(lastFreeKey);

  // Read the element names, counts and keys, placing them in the index.
  //
  for (size_t i = 0; i < entries; i++) {
    OMUInt32 name;
    readUInt32FromStream(setIndexStream, name, _reorderBytes);
    OMUInt32 count;
    readUInt32FromStream(setIndexStream, count, _reorderBytes);
    if (keySize == 16) {
      OMUniqueObjectIdentification key;
      readUniqueObjectIdentificationFromStream(setIndexStream,
                                               key,
                                               _reorderBytes);
      setIndex->insert(i, name, count, &key);
    } else if (keySize == 32) {
      OMUniqueMaterialIdentification key;
      readUniqueMaterialIdentificationFromStream(setIndexStream,
                                                 key,
                                                 _reorderBytes);
      setIndex->insert(i, name, count, &key);
    }
  }

  // Close the stream.
  //
  closeStream(setIndexStream);

  delete [] setIndexName;

  set = setIndex;
}

  // @mfunc The stream name for the index of a vector named <p vectorName>.
  //   @parm The vector name.
  //   @rdesc The stream name for the vector index.
char* OMStoredObject::vectorIndexStreamName(const char* vectorName)
{
  TRACE("OMStoredObject::vectorIndexStreamName");
  PRECONDITION("Valid vector name", validString(vectorName));

  char* suffix = " index";
  char* vectorIndexName = new char[strlen(vectorName) + strlen(suffix) + 1];
  ASSERT("Valid heap pointer", vectorIndexName != 0);
  strcpy(vectorIndexName, vectorName);
  strcat(vectorIndexName, suffix);

  return vectorIndexName;
}

  // @mfunc The stream name for the index of a set named <p setName>.
  //   @parm The set name.
  //   @rdesc The stream name for the set index.
char* OMStoredObject::setIndexStreamName(const char* setName)
{
  TRACE("OMStoredObject::setIndexStreamName");
  PRECONDITION("Valid set name", validString(setName));

  char* suffix = " index";
  char* setIndexName = new char[strlen(setName) + strlen(suffix) + 1];
  ASSERT("Valid heap pointer", setIndexName != 0);
  strcpy(setIndexName, setName);
  strcat(setIndexName, suffix);

  return setIndexName;
}

  // @mfunc Restore the <c OMPropertyTable> in this <c OMStoredObject>.
  //   @parm A pointer to the newly restored <c OMPropertyTable> by reference.
void OMStoredObject::restore(OMPropertyTable*& table)
{
  TRACE("OMPropertyTable::restore");

  IStream* stream = openStream("referenced properties");

  // byte order
  OMByteOrder byteOrder;
  readFromStream(stream, &byteOrder, sizeof(byteOrder));
  bool reorderBytes;
  if (byteOrder == hostByteOrder()) {
    reorderBytes = false;
  } else {
    reorderBytes = true;
  }

  // count of strings
  OMPropertyCount count;
  readUInt16FromStream(stream, count, reorderBytes);
  table = new OMPropertyTable();
  ASSERT("Valid heap pointer", table != 0);
 
  if (count > 0) {
    // count of characters
    OMUInt32 totalCharacters;
    readUInt32FromStream(stream, totalCharacters, reorderBytes);

    // sequence of null terminated strings
    OMCharacter* buffer = new OMCharacter[totalCharacters];
    ASSERT("Valid heap pointer", buffer != 0);
    OMUInt32 totalBytes = totalCharacters * sizeof(OMCharacter);
    readFromStream(stream, buffer, totalBytes);
    OMCharacter* externalName = buffer;
    for (size_t i = 0; i < count; i++) {
      size_t characterCount = lengthOfOMString(externalName);
      wchar_t* internalName = new wchar_t[characterCount + 1];
      ASSERT("Valid heap pointer", internalName != 0);
      internalizeString(externalName, internalName, characterCount + 1);
      table->insert(internalName);
      delete [] internalName;
      externalName = externalName + characterCount + 1;
    }
    delete [] buffer;
  }

  closeStream(stream);
}

  // @mfunc Restore a single weak reference.
  //   @parm The property id.
  //   @parm The property type.
  //   @parm The unique identification of the target.
  //   @parm A tag identifying the collection in which the target resides.
  //   @parm The id of the property whose value is the unique
  //         identifier of objects in the target set.
void OMStoredObject::restore(OMPropertyId propertyId,
                             OMStoredForm storedForm,
                             OMUniqueObjectIdentification& id,
                             OMPropertyTag& tag,
                             OMPropertyId& keyPropertyId)
{
  TRACE("OMStoredObject::restore");

  // tag, key pid, key size, key
  const size_t size = sizeof(tag) +
                      sizeof(OMPropertyId) + sizeof(OMKeySize) + sizeof(id);
  OMByte buffer[size];
  OMByte* p = &buffer[0];

  read(propertyId, storedForm, buffer, size);
  memcpy(&tag, p, sizeof(tag));
  p += sizeof(tag);
  memcpy(&keyPropertyId, p, sizeof(keyPropertyId));
  p += sizeof(keyPropertyId);
  OMKeySize keySize;
  memcpy(&keySize, p, sizeof(keySize));
  p += sizeof(keySize);
  memcpy(&id, p, sizeof(id));

  if (byteOrder() != hostByteOrder()) {
	reorderUInt16(tag); // assumes sizeof(tag) == 2
	reorderUInt16(keyPropertyId);
	//reorderUInt32(keySize);
	reorderUniqueObjectIdentification(id);
  }
}

  // @mfunc Restore a collection (vector/set) of weak references.
  //   @parm The name of the collection.
  //   @parm The unique identifications of the targets.
  //   @parm Count of targets.
  //   @parm A tag identifying the collection in which each of the
  //         targets reside.
  //   @parm The id of the property whose value is the unique
  //         identifier of objects in the target set.
void OMStoredObject::restore(const char* collectionName,
                             const OMUniqueObjectIdentification*& index,
                             size_t &count,
                             OMPropertyTag& tag,
                             OMPropertyId& keyPropertyId)
{
  TRACE("OMStoredObject::restore");
  
  // Calculate the stream name for the index.
  //
  char* indexName = setIndexStreamName(collectionName);

  // Open the stream.
  //
  IStream* indexStream = openStream(_storage, indexName);

  // Read the count of elements.
  //
  OMUInt32 entries;
  readUInt32FromStream(indexStream, entries, _reorderBytes);
  count = entries;

  // Read the tag. assumes sizeof(tag) == 2
  //
  readUInt16FromStream(indexStream, tag, _reorderBytes);

  // Read the key pid.
  //
  readUInt16FromStream(indexStream, keyPropertyId, _reorderBytes);

  // Read the key size.
  //
  OMKeySize keySize;
  readUInt8FromStream(indexStream, keySize);

  // Create an index.
  //
  OMUniqueObjectIdentification* collectionIndex = 0;
  if (entries > 0) {
    collectionIndex = new OMUniqueObjectIdentification[entries];
    ASSERT("Valid heap pointer", collectionIndex != 0);

    // Read the element keys, placing them in the index.
    //
    readFromStream(indexStream,
                   collectionIndex,
                   entries * sizeof(OMUniqueObjectIdentification));
    if (_reorderBytes) {
      for (size_t i = 0; i < entries; i++) {
        reorderUniqueObjectIdentification(collectionIndex[i]);
      }
    }
  }

  // Close the stream.
  //
  closeStream(indexStream);

  delete [] indexName;

  index = collectionIndex;
}

IStream* OMStoredObject::createStream(IStorage* storage,
                                      const char* streamName)
{
  TRACE("OMStoredObject::createStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validString(streamName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  DWORD mode = STGM_DIRECT | STGM_READWRITE |
               STGM_SHARE_EXCLUSIVE  | STGM_CREATE;

  IStream* stream = 0;
  OMCHAR omStreamName[256];
  convert(omStreamName, 256, streamName);

  HRESULT resultCode = storage->CreateStream(
    omStreamName,
    mode,
    0,
    0,
    &stream);
  checkStream(resultCode, streamName);

  return stream;
}

IStream* OMStoredObject::openStream(IStorage* storage, const char* streamName)
{
  TRACE("OMStoredObject::openStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validString(streamName));
  
  DWORD mode;
  if (_mode == OMFile::modifyMode) {
    mode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (_mode == OMFile::readOnlyMode) {
    mode = STGM_DIRECT | STGM_READ      | STGM_SHARE_EXCLUSIVE;
  }

  IStream* stream = 0;
  OMCHAR omStreamName[256];
  convert(omStreamName, 256, streamName);
  
  HRESULT resultCode = storage->OpenStream(
    omStreamName,
    0,
    mode,
    0,
    &stream);
  checkStream(resultCode, streamName);

  return stream;
  
}

  // @mfunc Close <p stream>.
  //   @parm The stream to close.
void OMStoredObject::closeStream(IStream*& stream)
{
  TRACE("OMStoredObject::closeStream");
  PRECONDITION("Valid stream", stream != 0);

#if defined(OM_ENABLE_DEBUG)
  HRESULT resultCode = stream->Release();
  ASSERT("Reference count is 0.", resultCode == 0);
#else
  stream->Release();
#endif
  stream = 0;
}

OMByteOrder OMStoredObject::byteOrder(void) const
{
  TRACE("OMStoredObject::byteOrder");

  return _byteOrder;
}

void OMStoredObject::mapCharacters(wchar_t* /* name */, size_t /* nameLength */)
{
  TRACE("OMStoredObject::mapCharacters");

#if 0
  for (size_t i = 0; i < nameLength; i++) {
    name[i] = map[name[i] & 0x7f];
  }
#endif
}

void OMStoredObject::mangleName(const wchar_t* clearName,
                                OMPropertyId pid,
                                wchar_t* mangledName,
                                size_t mangledNameSize)
{
  TRACE("OMStoredObject::mangleName");

  PRECONDITION("Valid pid", pid > 0);

  // Squeeze name to accommodate the pid as "-<pid>"
  //
  size_t maxSize = mangledNameSize - stringSize(pid) - 1 - 1;
  size_t newSize = squeezeWideString(clearName,
                                     lengthOfWideString(clearName),
                                     mangledName,
                                     maxSize);
  ASSERT("Consistent length", newSize == lengthOfWideString(mangledName));
  ASSERT("Consistent length",
                            lengthOfWideString(mangledName) < mangledNameSize);

  // Map out any illegal characters
  //
  mapCharacters(mangledName, newSize);

  // Append "-<pid>"
  //
  mangledName[newSize] = L'-';
  toWideString(pid, &mangledName[newSize+1], stringSize(pid));
}

wchar_t* OMStoredObject::streamName(const wchar_t* propertyName,
                                    OMPropertyId pid)
{
  return referenceName(propertyName, pid);
}

wchar_t* OMStoredObject::referenceName(const wchar_t* propertyName,
                                       OMPropertyId pid)
{
  TRACE("OMStoredObject::referenceName");

  wchar_t* result = new wchar_t[32];
  ASSERT("Valid heap pointer", result != 0);

  mangleName(propertyName, pid, result, 32);

  return result;
}

wchar_t* OMStoredObject::collectionName(const wchar_t* propertyName,
                                        OMPropertyId pid)
{
  TRACE("OMStoredObject::collectionName");

  wchar_t* result = new wchar_t[32];
  ASSERT("Valid heap pointer", result != 0);

  mangleName(propertyName, pid, result, 32 - 10);

  return result;
}

wchar_t* OMStoredObject::elementName(const wchar_t* propertyName,
                                     OMPropertyId pid,
                                     OMUInt32 localKey)
{
  TRACE("OMStoredObject::elementName");

  wchar_t* result = new wchar_t[32];
  ASSERT("Valid heap pointer", result != 0);

  mangleName(propertyName, pid, result, 32 - 10);

  size_t newSize = lengthOfWideString(result);
  concatenateWideString(result, L"{", 1);
  size_t keySize;
  if (localKey != 0) {
    keySize = stringSize(localKey);
    toWideString(localKey, &result[newSize + 1], keySize);
  } else {
    keySize = 1;
    concatenateWideString(result, L"0", 1);
  }
  concatenateWideString(result, L"}", 1);

  return result;
}

void OMStoredObject::writeName(OMPropertyId pid,
                               OMStoredForm storedForm,
                               const wchar_t* name)
{
  TRACE("OMStoredObject::writeName");

  size_t characterCount = lengthOfWideString(name) + 1;
  OMCharacter* buffer = new OMCharacter[characterCount];
  ASSERT("Valid heap pointer", buffer != 0);
  externalizeString(name, buffer, characterCount);
  ASSERT("Native byte order", _byteOrder == hostByteOrder());
  size_t byteCount = characterCount * sizeof(OMCharacter);
  write(pid, storedForm, buffer, byteCount);
  delete [] buffer;
}

wchar_t* OMStoredObject::readName(OMPropertyId pid,
                                  OMStoredForm storedForm,
                                  size_t size)
{
  TRACE("OMStoredObject::readName");

  OMCharacter* propertyName = new OMCharacter[size];
  ASSERT("Valid heap pointer", propertyName != 0);
  read(pid, storedForm, propertyName, size);
  size_t characterCount = size / sizeof(OMCharacter);
  if (_reorderBytes) {
    reorderString(propertyName, characterCount);
  }
  wchar_t* result = new wchar_t[characterCount];
  ASSERT("Valid heap pointer", result!= 0);
  internalizeString(propertyName, result, characterCount);
  delete [] propertyName;
  return result;
}

void OMStoredObject::reorderString(OMCharacter* string,
                                   size_t characterCount)
{
  TRACE("OMStoredObject::reorderString");

  for (size_t i = 0; i < characterCount; i++) {
    reorderUInt16(string[i]);
  }
}

void OMStoredObject::internalizeString(OMCharacter* externalString,
                                       wchar_t* internalString,
                                       size_t characterCount)
{
  TRACE("OMStoredObject::internalizeString");

  for (size_t i = 0; i < characterCount; i++) {
    internalString[i] = externalString[i];
  }
}

void OMStoredObject::externalizeString(const wchar_t* internalString,
                                       OMCharacter* externalString,
                                       size_t characterCount)
{
  TRACE("OMStoredObject::externalizeString");

  for (size_t i = 0; i < characterCount; i++) {
    externalString[i] = internalString[i];
  }
}

IStorage* OMStoredObject::createStorage(IStorage* storage,
                                        const char* storageName)
{
  TRACE("createStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validString(storageName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  DWORD mode = STGM_DIRECT | STGM_READWRITE |
               STGM_SHARE_EXCLUSIVE  | STGM_CREATE;

  IStorage* newStorage = 0;
  OMCHAR omStorageName[256];
  convert(omStorageName, 256, storageName);

  HRESULT resultCode = storage->CreateStorage(
    omStorageName,
    mode,
    0,
    0,
    &newStorage);
  checkStorage(resultCode, storageName);

  return newStorage;
}

IStorage* OMStoredObject::openStorage(IStorage* storage,
                                      const char* storageName,
                                      const OMFile::OMAccessMode mode)
{
  TRACE("openStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validString(storageName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  DWORD openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STGM_DIRECT | STGM_READ      | STGM_SHARE_EXCLUSIVE;
  }

  IStorage* newStorage = 0;
  OMCHAR omStorageName[256];
  convert(omStorageName, 256, storageName);

  HRESULT resultCode = storage->OpenStorage(
    omStorageName,
    0,
    openMode,
    0,
    0,
    &newStorage);
  checkStorage(resultCode, storageName);

  return newStorage;
}

void OMStoredObject::closeStorage(IStorage*& storage)
{
  TRACE("closeStorage");
  PRECONDITION("Valid storage", storage != 0);

#if defined(OM_ENABLE_DEBUG)
  HRESULT resultCode = storage->Release();
  ASSERT("Reference count is 0.", resultCode == 0);
#else
  storage->Release();
#endif
  storage = 0;
}

  // @mfunc Write <p size> bytes from the buffer at address <p data>
  //        to <p stream>.
  //   @parm The stream on which to write.
  //   @parm The buffer to write.
  //   @parm The number of bytes to write.   
void OMStoredObject::writeToStream(IStream* stream, void* data, size_t size)
{
  TRACE("OMStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesWritten;
  HRESULT resultCode = stream->Write(data, size, &bytesWritten);
  check(resultCode);

  ASSERT("Successful write", bytesWritten == size);
}

  // @mfunc Attempt to write <p bytes> bytes from the buffer at
  //        address <p data> to <p stream>. The actual number of
  //        bytes written is returned in <p bytesWritten>.
  //   @parm The stream on which to write.
  //   @parm The buffer to write.
  //   @parm The number of bytes to write
  //   @parm The actual number of bytes written.
void OMStoredObject::writeToStream(IStream* stream,
                                   const OMByte* data,
                                   const OMUInt32 bytes,
                                   OMUInt32& bytesWritten)
{
  TRACE("OMStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT resultCode = stream->Write(data, bytes, &bytesWritten);
  check(resultCode);
}

  // @mfunc Read <p size> bytes from <p stream> into the buffer at
  //        address <p data>.
  //   @parm The stream from which to read.
  //   @parm The buffer into which the bytes are read.
  //   @parm The number of bytes to read.   
void OMStoredObject::readFromStream(IStream* stream, void* data, size_t size)
{
  TRACE("OMStoredObject::readFromStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesRead;
  HRESULT result = stream->Read(data, size, &bytesRead);
  check(result);

  ASSERT("Successful read", bytesRead == size);
}

  // @mfunc Attempt to read <p bytes> bytes from <p stream> into
  //        the buffer at address <p data>. The actual number of
  //        bytes read is returned in <p bytesRead>.
  //   @parm The stream from which to read.
  //   @parm The buffer into which the bytes are read.
  //   @parm The number of bytes to write
  //   @parm The actual number of bytes read.
void OMStoredObject::readFromStream(IStream* stream,
                                    OMByte* data,
                                    const OMUInt32 bytes,
                                    OMUInt32& bytesRead)
{
  TRACE("OMStoredObject::readFromStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT result = stream->Read(data, bytes, &bytesRead);
  check(result);
}

  // @mfunc Read an OMUInt8 from <p stream> into <p i>. If
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt8.
void OMStoredObject::readUInt8FromStream(IStream* stream,
                                         OMUInt8& i)
{
  TRACE("OMStoredObject::readUInt8FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt8));
}

  // @mfunc Read an OMUInt16 from <p stream> into <p i>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt16.
  //   @parm If true then reorder the bytes.
void OMStoredObject::readUInt16FromStream(IStream* stream,
                                          OMUInt16& i,
                                          bool reorderBytes)
{
  TRACE("OMStoredObject::readUInt16FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt16));
  if (reorderBytes) {
    reorderUInt16(i);
  }
}

  // @mfunc Reorder the OMUInt16 <p i>.
  //   @parm The OMUInt16 to reorder.
void OMStoredObject::reorderUInt16(OMUInt16& i)
{
  TRACE("OMStoredObject::reorderUInt16");

  OMUInt8* p = (OMUInt8*)&i;
  OMUInt8 temp;

  temp = p[0];
  p[0] = p[1];
  p[1] = temp;

}

  // @mfunc Read an OMUInt32 from <p stream> into <p i>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt32.
  //   @parm If true then reorder the bytes.
void OMStoredObject::readUInt32FromStream(IStream* stream,
                                          OMUInt32& i,
                                          bool reorderBytes)
{
  TRACE("OMStoredObject::readUInt32FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt32));
  if (reorderBytes) {
    reorderUInt32(i);
  }
}

  // @mfunc Reorder the OMUInt32 <p i>.
  //   @parm The OMUInt32 to reorder.
void OMStoredObject::reorderUInt32(OMUInt32& i)
{
  TRACE("OMStoredObject::reorderUInt32");

  OMUInt8* p = (OMUInt8*)&i;
  OMUInt8 temp;

  temp = p[0];
  p[0] = p[3];
  p[3] = temp;

  temp = p[1];
  p[1] = p[2];
  p[2] = temp;

}

  // @mfunc Read a UniqueObjectIdentification from <p stream> into <p id>.
  //        If <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUniqueObjectIdentification.
  //   @parm If true then reorder the bytes.
void OMStoredObject::readUniqueObjectIdentificationFromStream(
                                              IStream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes)
{
  TRACE("OMStoredObject::UniqueObjectIdentificationFromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &id, sizeof(OMUniqueObjectIdentification));
  if (reorderBytes) {
    reorderUniqueObjectIdentification(id);
  }
}

  // @mfunc Read a UniqueMaterialIdentification from <p stream> into <p id>.
  //        If <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUniqueMaterialIdentification.
  //   @parm If true then reorder the bytes.
void OMStoredObject::readUniqueMaterialIdentificationFromStream(
                                            IStream* stream,
                                            OMUniqueMaterialIdentification& id,
                                            bool reorderBytes)
{
  TRACE("OMStoredObject::UniqueMaterialIdentificationFromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &id, sizeof(OMUniqueMaterialIdentification));
  if (reorderBytes) {
    reorderUniqueMaterialIdentification(id);
  }
}

  // @mfunc Reorder the OMUniqueObjectIdentification <p id>.
  //   @parm The OMUniqueObjectIdentification to reorder.
void OMStoredObject::reorderUniqueObjectIdentification(
                                              OMUniqueObjectIdentification& id)
{
  TRACE("OMStoredObject::reorderUniqueObjectIdentification");

  reorderUInt32(id.Data1);
  reorderUInt16(id.Data2);
  reorderUInt16(id.Data3);
  // no need to swap Data4
}

  // @mfunc Reorder the OMUniqueMaterialIdentification <p id>.
  //   @parm The OMUniqueMaterialIdentification to reorder.
void OMStoredObject::reorderUniqueMaterialIdentification(
                                            OMUniqueMaterialIdentification& id)
{
  TRACE("OMStoredMaterial::reorderUniqueMaterialIdentification");

  // No need to swap
  // SMPTELabel, length, instanceHigh, instanceMid or instanceLow.

  reorderUniqueObjectIdentification(id.material);
}

void OMStoredObject::setClass(IStorage* storage, const OMClassId& cid)
{
  TRACE("OMStoredObject::setClass");
  PRECONDITION("Valid storage", storage != 0);

  GUID g;
  memcpy(&g, &cid, sizeof(GUID));
  HRESULT resultCode = storage->SetClass(g);
  check(resultCode);
}

void OMStoredObject::getClass(IStorage* storage, OMClassId& cid)
{
  TRACE("OMStoredObject::getClass");
  PRECONDITION("Valid storage", storage != 0);

  STATSTG statstg;
  HRESULT result = storage->Stat(&statstg, STATFLAG_NONAME);
  check(result);

  memcpy(&cid, &statstg.clsid, sizeof(OMClassId));
}

  // @mfunc The current position for <f readFromStream()> and
  //        <f writeToStream()>, as an offset in bytes from the begining
  //        of the data stream.
  //   @rdesc The current position for <f readFromStream()> and
  //          <f writeToStream()>, as an offset in bytes from the begining
  //          of the data stream.
  //   @this const
OMUInt64 OMStoredObject::streamPosition(IStream* stream) const
{
  TRACE("OMStoredObject::streamPosition");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt64 result;
  LARGE_INTEGER zero = {0, 0};
  ULARGE_INTEGER position;
  HRESULT status = stream->Seek(zero, STREAM_SEEK_CUR, &position);
  check(status);

  result = toOMUInt64(position);
  return result;
}

  // @mfunc Set the current position for <f readFromStream()> and
  //        <f writeToStream()>, as an offset in bytes from the begining of
  //        the data stream.
  //   @parm The position to use for subsequent calls to readFromStream() and
  //         writeToStream() on this stream. The position is specified as an
  //         offset in bytes from the begining of the data stream.
  //   @this const
void OMStoredObject::streamSetPosition(IStream* stream, const OMUInt64 offset)
{
  TRACE("OMStoredObject::streamSetPosition");
  PRECONDITION("Valid stream", stream != 0);

  ULARGE_INTEGER newPosition = fromOMUInt64(offset);
  ULARGE_INTEGER oldPosition;
  LARGE_INTEGER position;
  memcpy(&position, &newPosition, sizeof(LARGE_INTEGER));
  HRESULT status = stream->Seek(position, STREAM_SEEK_SET, &oldPosition);
  check(status);
}

#if defined(_WIN32) && defined(UNICODE)

static void convert(wchar_t* wcName, size_t length, const char* name)
{
  TRACE("convert");
  PRECONDITION("Valid input name", validString(name));
  PRECONDITION("Valid output buffer", wcName != 0);
  PRECONDITION("Valid output buffer size", length > 0);
  
  ASSERT("Valid program name", validString(getProgramName()));

  size_t status = mbstowcs(wcName, name, length);
  if (status == (size_t)-1) {
    cerr << getProgramName()
      << "Error : Failed to convert \""
      << name
      << "\" to a wide character string."
      << endl;
    // exit(EXIT_FAILURE); // tjb - error
  }
}

#endif

static void convert(char* cName, size_t length, const wchar_t* name)
{
  TRACE("convert");
  ASSERT("Valid program name", validString(getProgramName()));
  PRECONDITION("Valid input name", validWideString(name));
  PRECONDITION("Valid output buffer", cName != 0);
  PRECONDITION("Valid output buffer size", length > 0);

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    cerr << getProgramName()
      << ": Error : Conversion failed."
      << endl;
    // exit(EXIT_FAILURE); // tjb - error
  }
}

static void convert(char* cName, size_t length, const char* name)
{
  TRACE("convert");
  PRECONDITION("Valid input name", validString(name));
  PRECONDITION("Valid output buffer", cName != 0);
  PRECONDITION("Valid output buffer size", length > 0);

  size_t sourceLength = strlen(name);
  if (sourceLength + 1 < length) {
    strncpy(cName, name, sourceLength + 1);
  } else {
    cerr << getProgramName()
      << ": Error : Conversion failed."
      << endl;
    // exit(EXIT_FAILURE); // tjb - error
  }
}

#if defined(_WIN32) && defined(UNICODE)

static void convert(wchar_t* wcName, size_t length, const wchar_t* name)
{
  TRACE("convert");
  PRECONDITION("Valid input name", validWideString(name));
  PRECONDITION("Valid output buffer", wcName != 0);
  PRECONDITION("Valid output buffer size", length > 0);

  size_t sourceLength = lengthOfWideString(name);
  if (sourceLength + 1 < length) {
    copyWideString(wcName, name, sourceLength + 1);
  } else {
    cerr << getProgramName()
      << ": Error : Conversion failed."
      << endl;
    // exit(EXIT_FAILURE); // tjb - error
  }
}

#endif

static void formatError(DWORD errorCode)
{
#if defined(_WIN32) || defined(WIN32)
  OMCHAR buffer[256];

  int status = FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    errorCode,
    LANG_SYSTEM_DEFAULT,
    buffer, 256,
    NULL);

  if (status != 0) {
    char message[256];
    convert(message, 256, buffer);

    int length = strlen(message);
    // zap cr/lf
    if (length >= 2) {
      message[length - 2] = '\0';
    }
    cerr << "Error text = \"" << message << "\"" << endl;
  } else {
    cerr << "Error code = " << hex << errorCode << dec << endl;
  }
#else
  cerr << "Error code = " << hex << errorCode << dec << endl;
#endif
}

static void check(HRESULT resultCode)
{
  TRACE("check");

  ASSERT("Valid program name", validString(getProgramName()));

  if (FAILED(resultCode)) {
    printError(getProgramName(), "Error");
    formatError(resultCode);
  }
  ASSERT("Succeeded", SUCCEEDED(resultCode)); // tjb - error
}

static void checkFile(HRESULT resultCode, const wchar_t* fileName)
{
  TRACE("checkFile");
  PRECONDITION("Valid file name", validWideString(fileName));

  ASSERT("Valid program name", validString(getProgramName()));

  if (FAILED(resultCode)) {
    printError(getProgramName(), "File error");
    printName(fileName);
    formatError(resultCode);
  }
  ASSERT("Succeeded", SUCCEEDED(resultCode)); // tjb - error
}

static void checkStream(HRESULT resultCode, const char* streamName)
{
  TRACE("checkStream");
  PRECONDITION("Valid stream name", validString(streamName));

  ASSERT("Valid program name", validString(getProgramName()));

  if (FAILED(resultCode)) {
    printError(getProgramName(), "Stream error");
    printName(streamName);
    formatError(resultCode);
  }

  ASSERT("Succeeded", SUCCEEDED(resultCode)); // tjb - error
}

static void checkStorage(HRESULT resultCode, const char* storageName)
{
  TRACE("checkStorage");
  PRECONDITION("Valid storage name", validString(storageName));

  ASSERT("Valid program name", validString(getProgramName()));

  if (FAILED(resultCode)) {
    printError(getProgramName(), "Storage error");
    printName(storageName);
    formatError(resultCode);
  }

  ASSERT("Succeeded", SUCCEEDED(resultCode)); // tjb - error
}

static void printError(const char* prefix, const char* type)
{
  cerr << prefix << " : " << type << " : ";
}

static void printName(const char* name)
{
  cerr << "\"" << name << "\" : ";
}

static void printName(const wchar_t* name)
{
  TRACE("printName");
  char cName[256];

  convert(cName, 256, name);
  cerr << "\"" << cName << "\" : ";
}

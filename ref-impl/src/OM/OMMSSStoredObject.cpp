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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMSSStoredObject
#include "OMMSSStoredObject.h"

#include "OMStoredPropertySetIndex.h"
#include "OMProperty.h"
#include "OMPropertySet.h"
#include "OMPropertySetIterator.h"
#include "OMStoredVectorIndex.h"
#include "OMStoredSetIndex.h"
#include "OMDataTypes.h"
#include "OMPropertyTable.h"

#include "OMDiskRawStorage.h"
#include "OMMemoryRawStorage.h"
#include "OMRawStorageLockBytes.h"

#include "OMContainerElement.h"
#include "OMObjectReference.h"
#include "OMStrongReference.h"
#include "OMStrongReferenceSet.h"
#include "OMStrongReferenceVector.h"
#include "OMWeakReference.h"
#include "OMWeakReferenceSet.h"
#include "OMWeakReferenceVector.h"
#include "OMDataStream.h"

#include "OMAssertions.h"
#include "OMExceptions.h"
#include "OMUtilities.h"

#include "OMMSStructuredStorage.h"
#include "OMMSSStoredStream.h"
#include "OMType.h"
#include "OMUniqueObjectIdentType.h"

const OMVersion currentVersion = 32;

const size_t indexHeaderSize = sizeof(OMByteOrder) +
                               sizeof(OMVersion) +
                               sizeof(OMPropertyCount);

const size_t indexEntrySize  = sizeof(OMPropertyId) +
                               sizeof(OMStoredForm) +
                               sizeof(OMPropertySize);

#if defined(OM_UNICODE_APIS)
static void convert(wchar_t* wcName, size_t length, const wchar_t* name);
#else
static void convert(char* cName, size_t length, const wchar_t* name);
#endif

static void check(HRESULT status);

#if defined(OM_DEBUG)
void OMMSSStoredObject::incrementOpenStreamCount(void)
{
  TRACE("OMMSSStoredObject::incrementOpenStreamCount");
  _openStreams = _openStreams + 1;
  if (_openStreams > _maxOpenStreams) {
    _maxOpenStreams = _openStreams;
  }
}

void OMMSSStoredObject::decrementOpenStreamCount(void)
{
  TRACE("OMMSSStoredObject::decrementOpenStreamCount");
  PRECONDITION("Non zero count", _openStreams != 0);
  _openStreams = _openStreams - 1;
}

void OMMSSStoredObject::incrementOpenStorageCount(void)
{
  TRACE("OMMSSStoredObject::incrementOpenStorageCount");
  _openStorages = _openStorages + 1;
  if (_openStorages > _maxOpenStorages) {
    _maxOpenStorages = _openStorages;
  }
}

void OMMSSStoredObject::decrementOpenStorageCount(void)
{
  TRACE("OMMSSStoredObject::decrementOpenStorageCount");
  PRECONDITION("Non zero count", _openStorages != 0);
  _openStorages = _openStorages - 1;
}

size_t OMMSSStoredObject::_openStorages = 0;
size_t OMMSSStoredObject::_maxOpenStorages = 0;
size_t OMMSSStoredObject::_openStreams = 0;
size_t OMMSSStoredObject::_maxOpenStreams = 0;
#endif

  // @mfunc Open the root <c OMMSSStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMMSSStoredObject> representing the root object in
  //          the disk file.
OMMSSStoredObject* OMMSSStoredObject::openRead(const wchar_t* fileName)
{
  TRACE("OMMSSStoredObject::openRead");
  PRECONDITION("Valid file name", validWideString(fileName));

  OMMSSStoredObject* newStore = OMMSSStoredObject::openFile(
                                                         fileName,
                                                         OMFile::readOnlyMode);
  newStore->open(OMFile::readOnlyMode);

  return newStore;
}

  // @mfunc Open the root <c OMMSSStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMMSSStoredObject> representing the root object in
  //          the disk file.
OMMSSStoredObject* OMMSSStoredObject::openModify(const wchar_t* fileName)
{
  TRACE("OMMSSStoredObject::openModify");
  PRECONDITION("Valid file name", validWideString(fileName));

  writeSignature(fileName, nullOMFileSignature);
  OMMSSStoredObject* newStore = OMMSSStoredObject::openFile(
                                                           fileName,
                                                           OMFile::modifyMode);
  newStore->open(OMFile::modifyMode);

  return newStore;
}

  // @mfunc Create a new root <c OMMSSStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMMSSStoredObject> representing the root object in
  //          the disk file.
OMMSSStoredObject* OMMSSStoredObject::createModify(const wchar_t* fileName,
                                                   const OMByteOrder byteOrder)
{
  TRACE("OMMSSStoredObject::createModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));

  OMMSSStoredObject* newStore = OMMSSStoredObject::createFile(fileName);
  newStore->create(byteOrder);

  return newStore;
}

  // @mfunc Open the root <c OMMSSStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMMSSStoredObject> representing the root object.
OMMSSStoredObject* OMMSSStoredObject::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMMSSStoredObject::openRead");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible raw storage access mode", rawStorage->isReadable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());

  OMMSSStoredObject* newStore = OMMSSStoredObject::openFile(
                                                         rawStorage,
                                                         OMFile::readOnlyMode);
  newStore->open(OMFile::readOnlyMode);

  return newStore;
}

  // @mfunc Open the root <c OMMSSStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMMSSStoredObject> representing the root object.
OMMSSStoredObject* OMMSSStoredObject::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMMSSStoredObject::openModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable() &&
                                         rawStorage->isExtendible());

  writeSignature(rawStorage, nullOMFileSignature);
  OMMSSStoredObject* newStore = OMMSSStoredObject::openFile(
                                                           rawStorage,
                                                           OMFile::modifyMode);
  newStore->open(OMFile::modifyMode);

  return newStore;
}

  // @mfunc Create a new root <c OMMSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMMSSStoredObject> representing the root object.
OMMSSStoredObject* OMMSSStoredObject::createWrite(OMRawStorage* rawStorage,
                                                  const OMByteOrder byteOrder)
{
  TRACE("OMMSSStoredObject::createWrite");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isWritable() && rawStorage->isReadable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable() &&
                                         rawStorage->isExtendible());

  OMMSSStoredObject* newStore = OMMSSStoredObject::createFile(rawStorage);
  newStore->create(byteOrder); // mode == modify

  return newStore;
}

  // @mfunc Create a new root <c OMMSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMMSSStoredObject> representing the root object.
OMMSSStoredObject* OMMSSStoredObject::createModify(OMRawStorage* rawStorage,
                                                   const OMByteOrder byteOrder)
{
  TRACE("OMMSSStoredObject::createModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable() &&
                                         rawStorage->isExtendible());

  OMMSSStoredObject* newStore = OMMSSStoredObject::createFile(rawStorage);
  newStore->create(byteOrder);

  return newStore;
}

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true, and the signature is returned
  //        in <p signature>.
  //   @parm The name of the file to check.
  //   @parm If recognized, the file signature.
  //   @rdesc True if the file is recognized, false otherwise.
bool OMMSSStoredObject::isRecognized(const wchar_t* fileName,
                                     OMFileSignature& signature)
{
  TRACE("OMMSSStoredObject::isRecognized");
  PRECONDITION("Valid file name", validWideString(fileName));
  bool result = false;
  OMFileSignature sig;
  FILE* f = wfopen(fileName, L"rb");
  if (f != 0) {
    size_t status = fseek(f, 8, SEEK_SET);
    if (status == 0) {
      status = fread(&sig, sizeof(sig), 1, f);
      if (status == 1) {
        fclose(f);
        if (hostByteOrder() != littleEndian) {
          OMByte* s = reinterpret_cast<OMByte*>(&sig);
          size_t size = sizeof(OMUniqueObjectIdentification);
          OMUniqueObjectIdentificationType::instance()->reorder(s, size);
        }
        if (isRecognized(sig)) {
          result = true;
          signature = sig;
        }
      }
    }
  }
  return result;
}

  // @mfunc Does <p rawStorage> contain a recognized file ?
  //        If so, the result is true, and the signature is returned
  //        in <p signature>.
  //   @parm The <c OMRawStorage> to check.
  //   @parm If recognized, the file signature.
  //   @rdesc True if the <c OMRawStorage> contains a recognized
  //          file, false otherwise.
bool OMMSSStoredObject::isRecognized(OMRawStorage* rawStorage,
                                     OMFileSignature& signature)
{
  TRACE("OMMSSStoredObject::isRecognized");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  bool result = false;
  OMFileSignature sig;
  OMUInt32 count;
  rawStorage->readAt(8,
                     reinterpret_cast<OMByte*>(&sig),
                     sizeof(sig),
                     count);

  if (count == sizeof(sig)) {
    if (hostByteOrder() != littleEndian) {
      OMByte* s = reinterpret_cast<OMByte*>(&sig);
      size_t size = sizeof(OMUniqueObjectIdentification);
      OMUniqueObjectIdentificationType::instance()->reorder(s, size);
    }
    if (isRecognized(sig)) {
      result = true;
      signature = sig;
    }
  }
  return result;
}

  // @mfunc Is <p signature> recognized ?
  //        If so, the result is true.
  //   @parm The signature to check.
  //   @rdesc True if the signature is recognized, false otherwise.
bool OMMSSStoredObject::isRecognized(const OMFileSignature& signature)
{
  TRACE("OMMSSStoredObject::isRecognized");
  bool result = false;
  char tag = ((char)((signature.Data1 & 0xff000000) >> 24));
  if (tag == 'B') {
    result = true;
  }
  return result;
}

  // @mfunc Destructor.
OMMSSStoredObject::~OMMSSStoredObject(void)
{
  TRACE("OMMSSStoredObject::~OMMSSStoredObject");
}

  // @mfunc Create a new <c OMMSSStoredObject>, named <p name>,
  //        contained by this <c OMMSSStoredObject>.
  //   @parm The name to be used for the new <c OMMSSStoredObject>.
  //   @rdesc A new <c OMMSSStoredObject> contained by this
  //          <c OMMSSStoredObject>.
OMStoredObject* OMMSSStoredObject::create(const wchar_t* name)
{
  TRACE("OMMSSStoredObject::create");
  PRECONDITION("Valid name", validWideString(name));

  IStorage* newStorage = createStorage(_storage, name);
  OMMSSStoredObject* result = new OMMSSStoredObject(newStorage);
  ASSERT("Valid heap pointer", result != 0);
  result->create(_byteOrder);
  return result;
}

  // @mfunc Open an exsiting <c OMMSSStoredObject>, named <p name>,
  //        contained by this <c OMMSSStoredObject>.
  //   @parm The name of the existing <c OMMSSStoredObject>.
  //   @rdesc The existing <c OMMSSStoredObject> contained by this
  //          <c OMMSSStoredObject>.
OMStoredObject* OMMSSStoredObject::open(const wchar_t* name)
{
  TRACE("OMMSSStoredObject::open");
  PRECONDITION("Valid name", validWideString(name));

  IStorage* newStorage = openStorage(_storage, name, _mode);
  OMMSSStoredObject* result = new OMMSSStoredObject(newStorage);
  ASSERT("Valid heap pointer", result != 0);
  result->open(_mode);
  return result;
}

  // @mfunc Close this <c OMMSSStoredObject>.
void OMMSSStoredObject::close(void)
{
  TRACE("OMMSSStoredObject::close");

  if (_open) {

    closeStream(_properties);

    delete _index;
    _index = 0;

    closeStorage(_storage);

    _open = false;
  }
}

void OMMSSStoredObject::close(OMFile& file)
{
  TRACE("OMMSSStoredObject::close");

  close();

  if (file.isWritable()) {
    if (file.isValid()) {
      OMFileSignature signature = file.signature();
      OMRawStorage* store = file.rawStorage();
      if (store != 0) {
        writeSignature(store, signature);
      } else {
        const wchar_t* fileName = file.fileName();
        writeSignature(fileName, signature);
      }
    }
  }
}

OMByteOrder OMMSSStoredObject::byteOrder(void) const
{
  TRACE("OMMSSStoredObject::byteOrder");

  return _byteOrder;
}

void OMMSSStoredObject::save(OMFile& file)
{
  TRACE("OMMSSStoredObject::save(OMFile)");
  file.root()->save();
  save(file.referencedProperties());
}

void OMMSSStoredObject::save(OMStorable& object)
{
  TRACE("OMMSSStoredObject::save(OMStorable)");
  save(object.classId());
  save(*object.propertySet());
}

  // @mfunc Save the <c OMStoredObjectIdentification> <p id>
  //        in this <c OMMSSStoredObject>.
  //   @parm The <c OMStoredObjectIdentification> of this
  //         <c OMMSSStoredObject>.
void OMMSSStoredObject::save(const OMStoredObjectIdentification& id)
{
  TRACE("OMMSSStoredObject::save");

  setClass(_storage, id);
}

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMMSSStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMMSSStoredObject::save(const OMPropertySet& properties)
{
  TRACE("OMMSSStoredObject::save(OMPropertySet)");
  PRECONDITION("Already open", _open);
  PRECONDITION("At start of value stream", streamPosition(_properties) == 0);
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

  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    ASSERT("Property has a definition", p->definition() != 0);
    if (!p->isOptional() || p->isPresent()) {
      p->save();
    }
  }

#if !defined(OM_NO_VALIDATE_PROPERTYSETS)
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

  // @mfunc Save the <c OMSimpleProperty> <p property> in this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
void OMMSSStoredObject::save(const OMSimpleProperty& property)
{
  TRACE("OMMSSStoredObject::save");

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = property.storedForm();
  size_t size = property.bitsSize();
  OMByte* bits = property.bits();
  const OMType* propertyType = property.type();

  if (propertyType != 0) { // tjb - temporary, should be ASSERTION below

    ASSERT("Valid property type", propertyType != 0);

    // Allocate buffer for property value
    size_t externalBytesSize = propertyType->externalSize(bits, size);
    OMByte* buffer = new OMByte[externalBytesSize];
    ASSERT("Valid heap pointer", buffer != 0);

    // Externalize property value
    propertyType->externalize(bits,
                              size,
                              buffer,
                              externalBytesSize,
                              byteOrder());

    // Reorder property value
    if (byteOrder() != hostByteOrder()) {
      propertyType->reorder(buffer, externalBytesSize);
    }

    // Write property value
    write(propertyId, storedForm, buffer, externalBytesSize);
    delete [] buffer;

  } else {
    // tjb - temporary, no type information, do it the old way
    //
    write(propertyId, storedForm, bits, size);
  }
}

  // @mfunc Save the <c OMStrongReference> <p singleton> in this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
void OMMSSStoredObject::save(const OMStrongReference& singleton)
{
  TRACE("OMMSSStoredObject::save");

  // Write the index entry.
  //
  wchar_t* name = referenceName(singleton.name(), singleton.propertyId());
  saveName(singleton, name);
  delete [] name;

  singleton.reference().save();
}

  // @mfunc Save the <c OMStrongReferenceVector> <p vector> in this
  //        <c OMMSSStoredObject>.
  //   @parm The vector of strong references to save.
void OMMSSStoredObject::save(const OMStrongReferenceVector& vector)
{
  TRACE("OMMSSStoredObject::save");

  // create a vector index
  //
  size_t count = vector.count();
  OMStoredVectorIndex* index = new OMStoredVectorIndex(count);
  ASSERT("Valid heap pointer", index != 0);
  index->setFirstFreeKey(vector.localKey());
  size_t position = 0;

  // Iterate over the vector saving each element
  //
  OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
                                                            *vector.iterator();
  while (++iterator) {

    OMStrongReferenceVectorElement& element = iterator.value();

    // enter into the index
    //
    index->insert(position, element.localKey());

#if defined(OM_DEBUG)
    wchar_t* name = elementName(vector.name(),
                                vector.propertyId(),
                                element.localKey());
    ASSERT("Consistent names", compareWideString(element.reference().name(),
                                                 name) == 0);
    delete [] name;
    name = 0; // for BoundsChecker
#endif

    // save the object
    //
    element.save();

    position = position + 1;
  }
  delete &iterator;

  // save the vector index
  //
  ASSERT("Valid vector index", index->isValid());
  wchar_t* name = collectionName(vector.name(), vector.propertyId());
  save(index, name);
  delete index;

  // make an entry in the property index
  //
  saveName(vector, name);
  delete [] name;
}

  // @mfunc Save the <c OMStrongReferenceSet> <p set> in this
  //        <c OMMSSStoredObject>.
  //   @parm The set of strong references to save.
void OMMSSStoredObject::save(const OMStrongReferenceSet& set)
{
  TRACE("OMMSSStoredObject::save");

  // create a set index
  //
  size_t count = set.count();
  OMKeySize keySize = set.keySize();
  OMPropertyId keyPropertyId = set.keyPropertyId();
  OMStoredSetIndex* index = new OMStoredSetIndex(count,
                                                 keyPropertyId,
                                                 keySize);
  ASSERT("Valid heap pointer", index != 0);
  index->setFirstFreeKey(set.localKey());
  size_t position = 0;

  // Iterate over the set saving each element. The index entries
  // are written in order of their unique keys.
  //
  OMContainerIterator<OMStrongReferenceSetElement>& iterator = *set.iterator();
  while (++iterator) {

    OMStrongReferenceSetElement& element = iterator.value();

    // enter into the index
    //
    void* key = element.identification();
    index->insert(position,
                  element.localKey(),
                  element.referenceCount(),
                  key);

#if defined(OM_DEBUG)
    wchar_t* name = elementName(set.name(),
                                set.propertyId(),
                                element.localKey());
    ASSERT("Consistent names", compareWideString(element.reference().name(),
                                                 name) == 0);
    delete [] name;
    name = 0; // for BoundsChecker
#endif

    // save the object
    //
    element.save();

    position = position + 1;
  }
  delete &iterator;
  // save the set index
  //
  ASSERT("Valid set index", index->isValid());
  wchar_t* name = collectionName(set.name(), set.propertyId());
  save(index, name);
  delete index;

  // make an entry in the property index
  //
  saveName(set, name);
  delete [] name;
}

  // @mfunc Save the <c OMWeakReference> <p singleton> in this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
void OMMSSStoredObject::save(const OMWeakReference& singleton)
{
  TRACE("OMMSSStoredObject::save");

  OMWeakObjectReference& reference = singleton.reference();

  OMPropertyId propertyId = singleton.propertyId();
  OMStoredForm storedForm = singleton.storedForm();
  const OMUniqueObjectIdentification& id = reference.identification();
  OMPropertyTag tag = singleton.targetTag();
  OMPropertyId keyPid = singleton.keyPropertyId();

  save(propertyId, storedForm, id, tag, keyPid);

  singleton.reference().save();

  singleton.clearTargetTag();
}

  // @mfunc Save the <c OMWeakReferenceVector> <p vector> in this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
void OMMSSStoredObject::save(const OMWeakReferenceVector& vector)
{
  TRACE("OMMSSStoredObject::save");

  OMPropertyTag tag = vector.targetTag();

  // create a vector index
  //
  size_t count = vector.count();
  OMUniqueObjectIdentification* index = 0;
  if (count > 0) {
    index = new OMUniqueObjectIdentification[count];
    ASSERT("Valid heap pointer", index != 0);
  }
  size_t position = 0;

  // Iterate over the vector saving each element. The index entries
  // are written in order of their unique keys.
  //
  OMContainerIterator<OMWeakReferenceVectorElement>& iterator =
                                                            *vector.iterator();
  while (++iterator) {

    OMWeakReferenceVectorElement& element = iterator.value();

    // enter into the index
    //
    index[position] = element.identification();

    // save the object
    //
    element.save();

    position = position + 1;
  }
  delete &iterator;

  // save the vector index
  //
  wchar_t* name = collectionName(vector.name(), vector.propertyId());
  save(name, index, count, tag, vector.keyPropertyId());
  delete [] index;

  // make an entry in the property index
  //
  saveName(vector, name);
  delete [] name;

  vector.clearTargetTag();
}

  // @mfunc Save the <c OMWeakReferenceSet> <p set> in this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
void OMMSSStoredObject::save(const OMWeakReferenceSet& set)
{
  TRACE("OMMSSStoredObject::save");

  OMPropertyTag tag = set.targetTag();

  // create a set index
  //
  size_t count = set.count();
  OMUniqueObjectIdentification* index = 0;
  if (count > 0) {
    index = new OMUniqueObjectIdentification[count];
    ASSERT("Valid heap pointer", index != 0);
  }
  size_t position = 0;

  // Iterate over the set saving each element. The index entries
  // are written in order of their unique keys.
  //
  OMContainerIterator<OMWeakReferenceSetElement>& iterator = *set.iterator();
  while (++iterator) {

    OMWeakReferenceSetElement& element = iterator.value();

    // enter into the index
    //
    index[position] = element.identification();

    // save the object
    //
    element.save();

    position = position + 1;
  }
  delete &iterator;

  // save the set index
  //
  wchar_t* name = collectionName(set.name(), set.propertyId());
  save(name, index, count, tag, set.keyPropertyId());
  delete [] index;

  // make an entry in the property index
  //
  saveName(set, name);
  delete [] name;

  set.clearTargetTag();
}

  // @mfunc Save the <c OMPropertyTable> <p table> in this
  //        <c OMMSSStoredObject>.
  //   @parm The table to save.
void OMMSSStoredObject::save(const OMPropertyTable* table)
{
  TRACE("OMMSSStoredObject::save");

  PRECONDITION("Valid property table", table != 0);

  IStream* stream = createStream(L"referenced properties");

  // byte order
  writeUInt8ToStream(stream, _byteOrder);

  // count of paths
  OMPropertyCount count = table->count();
  writeUInt16ToStream(stream, count, _reorderBytes);

  // count of property ids
  OMUInt32 pidCount = 0;
  for (size_t i = 0; i < count; i++) {
    OMUInt32 length = lengthOfPropertyPath(table->valueAt(i));
    pidCount = pidCount + length + 1;
  }
  writeUInt32ToStream(stream, pidCount, _reorderBytes);

  // sequence of null terminated pids
  for (size_t j = 0; j < count; j++) {
    const OMPropertyId* internalName = table->valueAt(j);
    size_t pidCount = lengthOfPropertyPath(internalName);
    size_t byteCount = pidCount * sizeof(OMPropertyId);
    OMPropertyId* externalName = new OMPropertyId[pidCount];
    ASSERT("Valid heap pointer", externalName != 0);
    externalizeUInt16Array(internalName, externalName, pidCount);
    if (_reorderBytes) {
      reorderUInt16Array(externalName, pidCount);
    }
    writeToStream(stream, (void*)externalName, byteCount);
    OMPropertyId null = 0;
    writeUInt16ToStream(stream, null, _reorderBytes);
    delete [] externalName;
  }

  closeStream(stream);
}

  // @mfunc Save the <c OMDataStream> <p stream> in this
  //        <c OMMSSStoredObject>.
  //   @parm The <c OMDataStream> to save.
void OMMSSStoredObject::save(const OMDataStream& stream)
{
  TRACE("OMMSSStoredObject::save");

  // Use the property name as the stream name
  //
  OMPropertyId propertyId = stream.propertyId();
  OMStoredForm storedForm = stream.storedForm();
  wchar_t* name = streamName(stream.name(), stream.propertyId());
  OMByteOrder byteOrder = stream.storedByteOrder();
  saveStream(propertyId, storedForm, name, byteOrder);
  delete [] name;
}

  // @mfunc Restore the <c OMStoredObjectIdentification>
  //        of this <c OMMSSStoredObject> into <p id>.
  //   @parm The <c OMStoredObjectIdentification> of this
  //         <c OMMSSStoredObject>.
void OMMSSStoredObject::restore(OMStoredObjectIdentification& id)
{
  TRACE("OMMSSStoredObject::restore");

  getClass(_storage, id);
}

  // @mfunc Restore the <c OMPropertySet> <p properties> into
  //        this <c OMMSSStoredObject>.
  //   @parm The <c OMPropertySet> to restore.
void OMMSSStoredObject::restore(OMPropertySet& properties)
{
  TRACE("OMMSSStoredObject::restore");
  PRECONDITION("Already open", _open);

  _index = restore();
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
#if !defined(OM_NO_VALIDATE_PROPERTYSETS)
  validate(&properties, _index);
#endif
  streamSetPosition(_properties, 0);
  POSTCONDITION("At start of properties stream",
                                       streamPosition(_properties) == 0);
}

  // @mfunc Restore the <c OMSimpleProperty> <p property> into this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMMSSStoredObject::restore(OMSimpleProperty& property,
                                 size_t externalSize)
{
  TRACE("OMMSSStoredObject::restore");

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = property.storedForm();
  OMByte* bits = 0;
  const OMType* propertyType = property.type();

  if (propertyType != 0) { // tjb - temporary, should be ASSERTION below

    ASSERT("Valid property type", propertyType != 0);

    // Allocate buffer for property value
    OMByte* buffer = new OMByte[externalSize];
    ASSERT("Valid heap pointer", buffer != 0);

    // Read property value
    read(propertyId, storedForm, buffer, externalSize);

    // Reorder property value
    if (byteOrder() != hostByteOrder()) {
      propertyType->reorder(buffer, externalSize);
    }

    // Internalize property value
    size_t requiredBytesSize = propertyType->internalSize(buffer,
                                                          externalSize);
    property.setSize(requiredBytesSize);
    ASSERT("Property value buffer large enough",
                                         property.size() >= requiredBytesSize);
    bits = property.bits();
    propertyType->internalize(buffer,
                              externalSize,
                              bits,
                              requiredBytesSize,
                              hostByteOrder());
    delete [] buffer;
  } else {
    // tjb - temporary, no type information, do it the old way
    //
    property.setSize(externalSize);
    ASSERT("Property value buffer large enough",
                                              property.size() >= externalSize);
    bits = property.bits();
    read(propertyId, storedForm, bits, externalSize);
  }
}

  // @mfunc Restore the <c OMStrongReference> <p singleton> into this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMMSSStoredObject::restore(OMStrongReference& singleton,
                                size_t externalSize)
{
  TRACE("OMMSSStoredObject::restore");

  wchar_t* name = referenceName(singleton.name(), singleton.propertyId());
  restoreName(singleton, name, externalSize);
  OMStrongObjectReference newReference(&singleton, name);
  delete [] name;
  singleton.reference() = newReference;
  singleton.reference().restore();
}

  // @mfunc Restore the <c OMStrongReferenceVector> <p vector> into this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMMSSStoredObject::restore(OMStrongReferenceVector& vector,
                                size_t externalSize)
{
  TRACE("OMMSSStoredObject::restore");

  OMPropertyId vectorId = vector.propertyId();
  const wchar_t* vectorName = vector.name();

  // restore the index
  //
  OMStoredVectorIndex* vectorIndex = 0;
  wchar_t* name = collectionName(vectorName, vectorId);
  restoreName(vector, name, externalSize);
  restore(vectorIndex, name);
  delete [] name;
  ASSERT("Valid vector index", vectorIndex->isValid());
  vector.setLocalKey(vectorIndex->firstFreeKey());

  // Iterate over the index restoring the elements of the vector
  //
  size_t entries = vectorIndex->entries();
  if (entries > 0) {
    vector.grow(entries); // Set the vector size
    size_t context = 0;
    OMUInt32 localKey;
    for (size_t i = 0; i < entries; i++) {
      vectorIndex->iterate(context, localKey);
      wchar_t* name = elementName(vectorName, vectorId, localKey);
      OMStrongReferenceVectorElement element(&vector, name, localKey);
      element.restore();
      vector.insert(i, element);
      delete [] name;
      name = 0; // for BoundsChecker
    }
  }
  delete vectorIndex;
}

  // @mfunc Restore the <c OMStrongReferenceSet> <p set> into this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMMSSStoredObject::restore(OMStrongReferenceSet& set,
                                size_t externalSize)
{
  TRACE("OMMSSStoredObject::restore");

  OMPropertyId setId = set.propertyId();
  const wchar_t* setName = set.name();

  // restore the index
  //
  OMStoredSetIndex* setIndex = 0;
  wchar_t* name = collectionName(setName, setId);
  restoreName(set, name, externalSize);
  restore(setIndex, name);
  delete [] name;
  ASSERT("Valid set index", setIndex->isValid());
  ASSERT("Consistent key sizes", setIndex->keySize() == set.keySize());
  ASSERT("Consistent key property ids",
                             setIndex->keyPropertyId() == set.keyPropertyId());
  set.setLocalKey(setIndex->firstFreeKey());

  // Iterate over the index restoring the elements of the set.
  // Since the index entries are stored on disk in order of their
  // unique keys this loop is the worst cast order of insertion. This
  // code will eventually be replaced by code that inserts the keys in
  // "binary search" order. That is the middle key is inserted first
  // then (recursively) all the keys below the middle key followed by
  // (recursively) all the keys above the middle key.
  //
  size_t entries = setIndex->entries();
  size_t context = 0;
  OMUInt32 localKey;
  OMUInt32 count;
  OMKeySize keySize = setIndex->keySize();
  OMByte* key = new OMByte[keySize];
  ASSERT("Valid heap pointer", key != 0);

  for (size_t i = 0; i < entries; i++) {
    setIndex->iterate(context, localKey, count, key);
    // Restore the object only if it doesn't already exist in the set.
    // Since the object is uniquely identified by the key, the
    // external copy is identical to the internal one, so we may
    // safely ignore the external one.
    //
    if (!set.contains(key)) {
      wchar_t* name = elementName(setName, setId, localKey);
      OMStrongReferenceSetElement element(&set,
                                          name,
                                          localKey,
                                          count,
                                          key,
                                          keySize);
      element.restore();
      set.insert(key, element);
      delete [] name;
      name = 0; // for BoundsChecker
    }
  }
  delete [] key;
  delete setIndex;
}

  // @mfunc Restore the <c OMWeakReference> <p singleton> into this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMMSSStoredObject::restore(OMWeakReference& singleton,
                                size_t ANAME(externalSize))
{
  TRACE("OMMSSStoredObject::restore");

  ASSERT("Sizes match",
                       (sizeof(OMPropertyTag) +
                        sizeof(OMPropertyId) +
                        sizeof(OMKeySize) +
                        sizeof(OMUniqueObjectIdentification)) == externalSize);

  OMPropertyId propertyId = singleton.propertyId();
  OMStoredForm storedForm = singleton.storedForm();
  OMUniqueObjectIdentification id;
  OMPropertyTag tag;
  OMPropertyId keyPropertyId;
  restore(propertyId, storedForm, id, tag, keyPropertyId);
  ASSERT("Consistent key property ids",
                                   keyPropertyId == singleton.keyPropertyId());

  OMWeakObjectReference& reference = singleton.reference();
  singleton.setTargetTag(tag);
  reference = OMWeakObjectReference(&singleton, id, tag);
  reference.restore();
}

  // @mfunc Restore the <c OMWeakReferenceVector> <p vector> into this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMMSSStoredObject::restore(OMWeakReferenceVector& vector,
                                size_t externalSize)
{
  TRACE("OMMSSStoredObject::restore");

  // restore the index
  //
  OMUniqueObjectIdentification* vectorIndex = 0;
  size_t entries;
  OMPropertyTag tag;
  OMPropertyId keyPropertyId;
  wchar_t* name = collectionName(vector.name(), vector.propertyId());
  restore(name, vectorIndex, entries, tag, keyPropertyId);
  restoreName(vector, name, externalSize);
  delete [] name;

  ASSERT("Valid vector index", IMPLIES(entries != 0, vectorIndex != 0));
  ASSERT("Valid vector index", IMPLIES(entries == 0, vectorIndex == 0));
  ASSERT("Consistent key property ids",
                                      keyPropertyId == vector.keyPropertyId());
  vector.setTargetTag(tag);

  // Iterate over the index restoring the elements of the vector.
  //
  if (entries > 0) {
    vector.grow(entries); // Set the vector size
    for (size_t i = 0; i < entries; i++) {
      OMUniqueObjectIdentification key = vectorIndex[i];
      OMWeakReferenceVectorElement element(&vector, key, tag);
      element.restore();
      vector.insert(i, element);
    }
  }
  delete [] vectorIndex;
}

  // @mfunc Restore the <c OMWeakReferenceSet> <p set> into this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMMSSStoredObject::restore(OMWeakReferenceSet& set,
                                size_t externalSize)
{
  TRACE("OMMSSStoredObject::restore");

  // restore the index
  //
  OMUniqueObjectIdentification* setIndex = 0;
  size_t entries;
  OMPropertyTag tag;
  OMPropertyId keyPropertyId;
  wchar_t* name = collectionName(set.name(), set.propertyId());
  restore(name, setIndex, entries, tag, keyPropertyId);
  restoreName(set, name, externalSize);
  delete [] name;

  ASSERT("Valid set index", IMPLIES(entries != 0, setIndex != 0));
  ASSERT("Valid set index", IMPLIES(entries == 0, setIndex == 0));
  ASSERT("Consistent key property ids", keyPropertyId == set.keyPropertyId());
  set.setTargetTag(tag);

  // Iterate over the index restoring the elements of the set.
  // Since the index entries are stored on disk in order of their
  // unique keys this loop is the worst cast order of insertion. This
  // code will eventually be replaced by code that inserts the keys in
  // "binary search" order. That is the middle key is inserted first
  // then (recursively) all the keys below the middle key followed by
  // (recursively) all the keys above the middle key.
  //
  for (size_t i = 0; i < entries; i++) {
    OMUniqueObjectIdentification key = setIndex[i];
    OMWeakReferenceSetElement element(&set, key, tag);
    element.restore();
    set.insert(&key, element);
   }
  delete [] setIndex;
}

  // @mfunc Restore the <c OMPropertyTable> in this <c OMMSSStoredObject>.
  //   @parm A pointer to the newly restored <c OMPropertyTable> by reference.
void OMMSSStoredObject::restore(OMPropertyTable*& table)
{
  TRACE("OMMSSStoredObject::restore");

  IStream* stream = openStream(L"referenced properties");

  // byte order
  OMByteOrder byteOrder;
  readFromStream(stream, &byteOrder, sizeof(byteOrder));
  bool reorderBytes;
  if (byteOrder == hostByteOrder()) {
    reorderBytes = false;
  } else {
    reorderBytes = true;
  }

  // count of paths
  OMPropertyCount count;
  readUInt16FromStream(stream, count, reorderBytes);
  table = new OMPropertyTable();
  ASSERT("Valid heap pointer", table != 0);

  if (count > 0) {
    // count of property ids
    OMUInt32 totalPids;
    readUInt32FromStream(stream, totalPids, reorderBytes);

    // sequence of null terminated pids
    OMPropertyId* buffer = new OMPropertyId[totalPids];
    ASSERT("Valid heap pointer", buffer != 0);
    OMUInt32 totalBytes = totalPids * sizeof(OMPropertyId);
    readFromStream(stream, buffer, totalBytes);
    OMPropertyId* externalName = buffer;
    for (size_t i = 0; i < count; i++) {
      size_t pidCount = lengthOfPropertyPath(externalName);
      OMPropertyId* internalName = new OMPropertyId[pidCount + 1];
      ASSERT("Valid heap pointer", internalName != 0);
      if (reorderBytes) {
        reorderUInt16Array(externalName, pidCount + 1);
      }
      internalizeUInt16Array(externalName, internalName, pidCount + 1);
      table->insert(internalName);
      delete [] internalName;
      internalName = 0; // for BoundsChecker
      externalName = externalName + pidCount + 1;
    }
    delete [] buffer;
  }

  closeStream(stream);
}

  // @mfunc Restore the <c OMDataStream> <p stream> into this
  //        <c OMMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMMSSStoredObject::restore(OMDataStream& stream,
                                size_t ANAME(externalSize))
{
  TRACE("OMMSSStoredObject::restore");

  OMPropertyId propertyId = stream.propertyId();
  OMStoredForm storedForm = stream.storedForm();
  wchar_t* sName = streamName(stream.name(), propertyId);
  size_t characterCount = lengthOfWideString(sName) + 1;
  size_t size = (characterCount * sizeof(OMCharacter)) + 1;
  ASSERT("Consistent property size", size == externalSize);

  wchar_t* name = 0;
  OMByteOrder bo;
  restoreStream(propertyId, storedForm, size, &name, &bo);
  ASSERT("Consistent property name", compareWideString(name, sName) == 0);
  ASSERT("Valid stored byte order", ((bo == littleEndian) ||
                                     (bo == bigEndian) ||
                                     (bo == unspecified)));
  stream.setStoredByteOrder(bo);
  delete [] name;
  delete [] sName;
}

  // @mfunc Open the <c OMStoredStream> representing the property
  //        <p property> contained within this <c OMMSSStoredObject>.
  //   @parm TBS
  //   @rdesc TBS
OMStoredStream* OMMSSStoredObject::openStoredStream(
                                                  const OMDataStream& property)
{
  TRACE("OMMSSStoredObject::openStoredStream");

  wchar_t* sName = streamName(property.name(), property.propertyId());
  IStream* stream = openStream(sName);
  OMMSSStoredStream* result = new OMMSSStoredStream(stream);
  ASSERT("Valid heap pointer", result != 0);
  delete [] sName;

  return result;
}

  // @mfunc Create an <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMMSSStoredObject>.
  //   @parm TBS
  //   @rdesc TBS
OMStoredStream* OMMSSStoredObject::createStoredStream(
                                                  const OMDataStream& property)
{
  TRACE("OMMSSStoredObject::createStoredStream");

  wchar_t* sName = streamName(property.name(), property.propertyId());
  IStream* stream = createStream(sName);
  OMMSSStoredStream* result = new OMMSSStoredStream(stream);
  ASSERT("Valid heap pointer", result != 0);
  delete [] sName;
  return result;
}

  // @mfunc Constructor.
  //   @parm The IStorage for the persistent representation of
  //         this <c OMMSSStoredObject>.
OMMSSStoredObject::OMMSSStoredObject(IStorage* s)
: _storage(s), _index(0), _properties(0),
  _offset(0), _open(false), _mode(OMFile::readOnlyMode),
  _byteOrder(hostByteOrder()), _reorderBytes(false)
{
  TRACE("OMMSSStoredObject::OMMSSStoredObject");
}

  // @mfunc Check that the <c OMPropertySet> <p propertySet> is
  //        consistent with the <c OMStoredPropertySetIndex>
  //        propertySetIndex.
  //   @parm The <c OMPropertySet> to validate.
  //   @parm The <c OMStoredPropertySetIndex> to validate.
void OMMSSStoredObject::validate(
                        const OMPropertySet* propertySet,
                        const OMStoredPropertySetIndex* propertySetIndex) const
{
  TRACE("OMMSSStoredObject::validate");
  PRECONDITION("Valid property set", propertySet != 0);
  PRECONDITION("Valid property set index", propertySetIndex != 0);

  OMPropertyId propertyId;
  OMStoredForm type;
  OMUInt32 offset;
  OMPropertySize length;
  size_t context;

  // Check that all required properties are present.
  //
  OMPropertySetIterator iterator(*propertySet, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
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
  //         <c OMMSSStoredObject>, the vector is named <p vectorName>.
  //   @parm The <c OMStoredVectorIndex> to save.
  //   @parm The name of the vector.
void OMMSSStoredObject::save(const OMStoredVectorIndex* vector,
                             const wchar_t* vectorName)
{
  TRACE("OMMSSStoredObject::save");
  PRECONDITION("Valid vector", vector != 0);
  PRECONDITION("Valid vector name", validWideString(vectorName));

  // Calculate the stream name for the index.
  //
  wchar_t* vectorIndexName = collectionIndexStreamName(vectorName);

  // Create the stream.
  //
  IStream* vectorIndexStream = createStream(_storage, vectorIndexName);
  delete [] vectorIndexName;

  // Write the count of elements.
  //
  OMUInt32 entries = vector->entries();
  writeUInt32ToStream(vectorIndexStream, entries, _reorderBytes);

  // Write the first free key.
  //
  OMUInt32 firstFreeKey = vector->firstFreeKey();
  writeUInt32ToStream(vectorIndexStream, firstFreeKey, _reorderBytes);

  // Write the last free key.
  //
  OMUInt32 lastFreeKey = vector->lastFreeKey();
  writeUInt32ToStream(vectorIndexStream, lastFreeKey, _reorderBytes);

  // For each element write the element name.
  //
  size_t context = 0;
  OMUInt32 name;
  for (size_t i = 0; i < entries; i++) {
    vector->iterate(context, name);
    writeUInt32ToStream(vectorIndexStream, name, _reorderBytes);
  }

  // Close the stream.
  //
  closeStream(vectorIndexStream);
}

  // @mfunc  Save the <c OMStoredSetIndex> <p set> in this
  //         <c OMMSSStoredObject>, the set is named <p setName>.
  //   @parm The <c OMStoredSetIndex> to save.
  //   @parm The name of the set.
void OMMSSStoredObject::save(const OMStoredSetIndex* set,
                             const wchar_t* setName)
{
  TRACE("OMMSSStoredObject::save");
  PRECONDITION("Valid set", set != 0);
  PRECONDITION("Valid set name", validWideString(setName));

  // Calculate the stream name for the index.
  //
  wchar_t* setIndexName = collectionIndexStreamName(setName);

  // Create the stream.
  //
  IStream* setIndexStream = createStream(_storage, setIndexName);
  delete [] setIndexName;

  // Write the count of elements.
  //
  OMUInt32 entries = set->entries();
  writeUInt32ToStream(setIndexStream, entries, _reorderBytes);

  // Write the first free key.
  //
  OMUInt32 firstFreeKey = set->firstFreeKey();
  writeUInt32ToStream(setIndexStream, firstFreeKey, _reorderBytes);

  // Write the last free key.
  //
  OMUInt32 lastFreeKey = set->lastFreeKey();
  writeUInt32ToStream(setIndexStream, lastFreeKey, _reorderBytes);

  // Write the key pid.
  //
  OMPropertyId pid = set->keyPropertyId();
  writeUInt16ToStream(setIndexStream, pid, _reorderBytes);

  // Write the key size.
  //
  OMKeySize keySize = set->keySize();
  writeUInt8ToStream(setIndexStream, keySize);

  // For each element write the element name, reference count and key.
  //
  size_t context = 0;
  OMUInt32 name;
  OMUInt32 count;
  OMByte* key = new OMByte[keySize];
  ASSERT("Valid heap pointer", key != 0);
  for (size_t i = 0; i < entries; i++) {
    set->iterate(context, name, count, key);
    writeUInt32ToStream(setIndexStream, name, _reorderBytes);
    writeUInt32ToStream(setIndexStream, count, _reorderBytes);
    // tjb - The following ought to be done via the key type and not
    // via the key size
    if (keySize == 16) {
      OMUniqueObjectIdentification* k =
                          reinterpret_cast<OMUniqueObjectIdentification*>(key);
      writeUniqueObjectIdentificationToStream(setIndexStream,
                                              *k,
                                              _reorderBytes);
    } else if (keySize == 32) {
      OMUniqueMaterialIdentification* k =
                        reinterpret_cast<OMUniqueMaterialIdentification*>(key);
      writeUniqueMaterialIdentificationToStream(setIndexStream,
                                                *k,
                                                _reorderBytes);
    }
  }
  delete [] key;

  // Close the stream.
  //
  closeStream(setIndexStream);
}

  // @mfunc Save a single weak reference.
  //   @parm The property id.
  //   @parm The property type.
  //   @parm The unique identification of the target.
  //   @parm A tag identifying the collection in which the target resides.
  //   @parm The id of the property whose value is the unique
  //         identifier of objects in the target set.
void OMMSSStoredObject::save(OMPropertyId propertyId,
                             OMStoredForm storedForm,
                             const OMUniqueObjectIdentification& id,
                             OMPropertyTag tag,
                             OMPropertyId keyPropertyId)
{
  TRACE("OMMSSStoredObject::save");

  // tag, key pid, key size, key
  OMPropertySize size = sizeof(OMPropertyTag) +
                        sizeof(OMPropertyId) +
                        sizeof(OMKeySize) +
                        sizeof(OMUniqueObjectIdentification);
  writeUInt16ToStream(_properties, tag, _reorderBytes);
  writeUInt16ToStream(_properties, keyPropertyId, _reorderBytes);
  OMUniqueObjectIdentification key = id;
  OMKeySize keySize = sizeof(key);
  writeUInt8ToStream(_properties, keySize);
  writeUniqueObjectIdentificationToStream(_properties, key, _reorderBytes);

  // Index entry.
  //
  _index->insert(propertyId, storedForm, _offset, size);
  _offset += size;
}

  // @mfunc Save a collection (vector/set) of weak references.
  //   @parm The name of the collection.
  //   @parm The unique identifications of the targets.
  //   @parm Count of targets.
  //   @parm A tag identifying the collection in which each of the
  //         targets reside.
  //   @parm The id of the property whose value is the unique
  //         identifier of objects in the target set.
void OMMSSStoredObject::save(const wchar_t* collectionName,
                             const OMUniqueObjectIdentification* index,
                             size_t count,
                             OMPropertyTag tag,
                             OMPropertyId keyPropertyId)
{
  TRACE("OMMSSStoredObject::save");

  PRECONDITION("Valid index", IMPLIES(count != 0, index!= 0));
  PRECONDITION("Valid index", IMPLIES(count == 0, index== 0));
  PRECONDITION("Valid collection name", validWideString(collectionName));

  // Calculate the stream name for the index.
  //
  wchar_t* indexName = collectionIndexStreamName(collectionName);

  // Create the stream.
  //
  IStream* indexStream = createStream(_storage, indexName);
  delete [] indexName;

  // Write the count of elements.
  //
  OMUInt32 entries = count;
  writeUInt32ToStream(indexStream, entries, _reorderBytes);

  // Write the tag.
  //
  writeUInt16ToStream(indexStream, tag, _reorderBytes);

  // Write the key pid.
  //
  writeUInt16ToStream(indexStream, keyPropertyId, _reorderBytes);

  // Write key size.
  //
  OMKeySize keySize = sizeof(OMUniqueObjectIdentification);
  writeUInt8ToStream(indexStream, keySize);

  if (count > 0) {
    // For each element write the element unique identifier
    //
    if (_reorderBytes) {
      for (OMUInt32 i = 0; i < entries; i++) {
        OMUniqueObjectIdentification* k =
                          const_cast<OMUniqueObjectIdentification*>(&index[i]);
        reorderUniqueObjectIdentification(*k);
       }
    }
    writeToStream(indexStream,
                  (void *)index,
                  count * sizeof(OMUniqueObjectIdentification));
  }

  // Close the stream.
  //
  closeStream(indexStream);
}

void OMMSSStoredObject::saveStream(OMPropertyId pid,
                                   OMStoredForm storedForm,
                                   const wchar_t* name,
                                   OMByteOrder byteOrder)
{
  TRACE("OMMSSStoredObject::saveStream");

  // Byte order
  //
  writeUInt8ToStream(_properties, byteOrder);

  // Name
  //
  size_t characterCount = lengthOfWideString(name) + 1;
  OMCharacter* buffer = new OMCharacter[characterCount];
  ASSERT("Valid heap pointer", buffer != 0);
  externalizeString(name, buffer, characterCount);
  if (_reorderBytes) {
    reorderString(buffer, characterCount);
  }
  size_t byteCount = characterCount * sizeof(OMCharacter);
  writeToStream(_properties, buffer, byteCount);
  delete [] buffer;

  // Index entry.
  //
  OMPropertySize size = byteCount + 1;
  _index->insert(pid, storedForm, _offset, size);
  _offset += size;
}

  // @mfunc Restore the vector named <p vectorName> into this
  //        <c OMMSSStoredObject>.
  //   @parm The name of the vector.
  //   @rdesc The newly restored <c OMStoredVectorIndex>.
void OMMSSStoredObject::restore(OMStoredVectorIndex*& vector,
                                const wchar_t* vectorName)
{
  TRACE("OMMSSStoredObject::restore");
  PRECONDITION("Valid vector name", validWideString(vectorName));

  // Calculate the stream name for the index.
  //
  wchar_t* vectorIndexName = collectionIndexStreamName(vectorName);

  // Open the stream.
  //
  IStream* vectorIndexStream = openStream(_storage, vectorIndexName);
  delete [] vectorIndexName;

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

  vector = vectorIndex;
}

  // @mfunc Restore the set named <p setName> into this
  //        <c OMMSSStoredObject>.
  //   @parm The name of the set.
  //   @rdesc The newly restored <c OMStoredSetIndex>.
void OMMSSStoredObject::restore(OMStoredSetIndex*& set,
                                const wchar_t* setName)
{
  TRACE("OMMSSStoredObject::restore");
  PRECONDITION("Valid set name", validWideString(setName));

  // Calculate the stream name for the index.
  //
  wchar_t* setIndexName = collectionIndexStreamName(setName);

  // Open the stream.
  //
  IStream* setIndexStream = openStream(_storage, setIndexName);
  delete [] setIndexName;

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
    // tjb - The following ought to be done via the key type and not
    // via the key size
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

  set = setIndex;
}

  // @mfunc Restore a single weak reference.
  //   @parm The property id.
  //   @parm The property type.
  //   @parm The unique identification of the target.
  //   @parm A tag identifying the collection in which the target resides.
  //   @parm The id of the property whose value is the unique
  //         identifier of objects in the target set.
void OMMSSStoredObject::restore(OMPropertyId propertyId,
                                OMStoredForm storedForm,
                                OMUniqueObjectIdentification& id,
                                OMPropertyTag& tag,
                                OMPropertyId& keyPropertyId)
{
  TRACE("OMMSSStoredObject::restore");

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
void OMMSSStoredObject::restore(const wchar_t* collectionName,
                                OMUniqueObjectIdentification*& index,
                                size_t &count,
                                OMPropertyTag& tag,
                                OMPropertyId& keyPropertyId)
{
  TRACE("OMMSSStoredObject::restore");

  // Calculate the stream name for the index.
  //
  wchar_t* indexName = collectionIndexStreamName(collectionName);

  // Open the stream.
  //
  IStream* indexStream = openStream(_storage, indexName);
  delete [] indexName;

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

  index = collectionIndex;
}

void OMMSSStoredObject::restoreStream(OMPropertyId pid,
                                      OMStoredForm storedForm,
                                      size_t size,
                                      wchar_t** name,
                                      OMByteOrder* byteOrder)
{
  TRACE("OMMSSStoredObject::restoreStream");

  size_t characterCount = (size - 1) / sizeof(OMCharacter);
  OMCharacter* buffer = new OMCharacter[characterCount + 1];
  ASSERT("Valid heap pointer", buffer != 0);
  OMByte* property = ((OMByte*)buffer + 1);
  read(pid, storedForm, property, size);
  *byteOrder = *property;
  OMCharacter* externalName = buffer;
  externalName = externalName + 1;
  if (_reorderBytes) {
    reorderString(externalName, characterCount);
  }
  *name = new wchar_t[characterCount];
  ASSERT("Valid heap pointer", *name != 0);
  internalizeString(externalName, *name, characterCount);
  delete [] buffer;
}

  // @mfunc Write a property value to this <c OMMSSStoredObject>. The
  //        property value to be written occupies <p size> bytes at
  //        the address <p start>. The property id is <p propertyId>.
  //        The property type is <p type>.
  //   @parm The property id.
  //   @parm The property type.
  //   @parm The start address of the property value.
  //   @parm The size of the property value in bytes.
void OMMSSStoredObject::write(OMPropertyId propertyId,
                              OMStoredForm storedForm,
                              void* start,
                              size_t size)
{
  TRACE("OMMSSStoredObject::write");
  PRECONDITION("Valid data", start != 0);
  PRECONDITION("Valid size", size > 0);

  _index->insert(propertyId, storedForm, _offset, size);

  // Write property value.
  //
  writeToStream(_properties, start, size);
  _offset += size;
}

  // @mfunc Read a property value from this <c OMMSSStoredObject>.
  //        The property value is read into a buffer which occupies
  //        <p size> bytes at the address <p start>. The property id
  //        is <p propertyId>. The property type is <p type>.
  //   @parm OMPropertyId | propertyId | The property id.
  //   @parm int | type | The property type.
  //   @parm void* | start | The start address of the buffer to hold the
  //         property value.
  //   @parm size_t | size | The size of the buffer in bytes.
void OMMSSStoredObject::read(OMPropertyId propertyId,
                             OMStoredForm ANAME(storedForm),
                             void* start,
                             size_t size)
{
  TRACE("OMMSSStoredObject::read");
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

  // @mfunc Open a stream called <p streamName> contained within this
  //        <c OMMSSStoredObject>.
  //   @parm The name of the stream to open.
  //   @rdesc An open stream.
IStream* OMMSSStoredObject::openStream(const wchar_t* streamName)
{
  TRACE("OMMSSStoredObject::openStream");
  PRECONDITION("Valid stream name", validWideString(streamName));

  return openStream(_storage, streamName);
}

  // @mfunc Create a stream called <p streamName> contained within
  //        this <c OMMSSStoredObject>.
  //   @parm The name of the stream to create.
  //   @rdesc An open stream.
IStream* OMMSSStoredObject::createStream(const wchar_t* streamName)
{
  TRACE("OMMSSStoredObject::createStream");
  PRECONDITION("Valid stream name", validWideString(streamName));

  return createStream(_storage, streamName);
}

  // @mfunc Read <p size> bytes from <p stream> into the buffer at
  //        address <p data>.
  //   @parm The stream from which to read.
  //   @parm The buffer into which the bytes are read.
  //   @parm The number of bytes to read.
void OMMSSStoredObject::readFromStream(IStream* stream,
                                       void* data,
                                       size_t size)
{
  TRACE("OMMSSStoredObject::readFromStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesRead;
  HRESULT status = stream->Read(data, size, &bytesRead);
  check(status);
  ASSERT("IStream::Read() succeeded", SUCCEEDED(status));
  ASSERT("Successful read", bytesRead == size);
}

  // @mfunc Attempt to read <p bytes> bytes from <p stream> into
  //        the buffer at address <p data>. The actual number of
  //        bytes read is returned in <p bytesRead>.
  //   @parm The stream from which to read.
  //   @parm The buffer into which the bytes are read.
  //   @parm The number of bytes to write
  //   @parm The actual number of bytes read.
void OMMSSStoredObject::readFromStream(IStream* stream,
                                       OMByte* data,
                                       const OMUInt32 bytes,
                                       OMUInt32& bytesRead)
{
  TRACE("OMMSSStoredObject::readFromStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT status = stream->Read(data, bytes, &bytesRead);
  check(status);
  ASSERT("IStream::Read() succeeded", SUCCEEDED(status));
}

  // @mfunc Write <p size> bytes from the buffer at address <p data>
  //        to <p stream>.
  //   @parm The stream on which to write.
  //   @parm The buffer to write.
  //   @parm The number of bytes to write.
void OMMSSStoredObject::writeToStream(IStream* stream, void* data, size_t size)
{
  TRACE("OMMSSStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesWritten;
  HRESULT status = stream->Write(data, size, &bytesWritten);
  check(status);
  ASSERT("IStream::Write() succeeded", SUCCEEDED(status));
  ASSERT("Successful write", bytesWritten == size);
}

  // @mfunc Attempt to write <p bytes> bytes from the buffer at
  //        address <p data> to <p stream>. The actual number of
  //        bytes written is returned in <p bytesWritten>.
  //   @parm The stream on which to write.
  //   @parm The buffer to write.
  //   @parm The number of bytes to write
  //   @parm The actual number of bytes written.
void OMMSSStoredObject::writeToStream(IStream* stream,
                                      const OMByte* data,
                                      const OMUInt32 bytes,
                                      OMUInt32& bytesWritten)
{
  TRACE("OMMSSStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT status = stream->Write(data, bytes, &bytesWritten);
  check(status);
  ASSERT("IStream::Write() succeeded", SUCCEEDED(status));
}

  // @mfunc Read an OMUInt8 from <p stream> into <p i>.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt8.
void OMMSSStoredObject::readUInt8FromStream(IStream* stream,
                                            OMUInt8& i)
{
  TRACE("OMMSSStoredObject::readUInt8FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt8));
}

  // @mfunc Write an OMUInt8 from <p i> to <p stream>.
  //   @parm The stream to write to.
  //   @parm The OMUInt8 to write.
void OMMSSStoredObject::writeUInt8ToStream(IStream* stream,
                                           OMUInt8& i)
{
  TRACE("OMMSSStoredObject::writeUInt8ToStream");
  PRECONDITION("Valid stream", stream != 0);

  writeToStream(stream, &i, sizeof(OMUInt8));
}

  // @mfunc Read an OMUInt16 from <p stream> into <p i>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt16.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::readUInt16FromStream(IStream* stream,
                                             OMUInt16& i,
                                             bool reorderBytes)
{
  TRACE("OMMSSStoredObject::readUInt16FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt16));
  if (reorderBytes) {
    reorderUInt16(i);
  }
}

  // @mfunc Write an OMUInt16 from <p i> to <p stream>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream to write to.
  //   @parm The OMUInt16 to write.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::writeUInt16ToStream(IStream* stream,
                                            OMUInt16& i,
                                            bool reorderBytes)
{
  TRACE("OMMSSStoredObject::writeUInt16ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt16 si = i;
  if (reorderBytes) {
    reorderUInt16(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt16));
}

  // @mfunc Reorder the OMUInt16 <p i>.
  //   @parm The OMUInt16 to reorder.
void OMMSSStoredObject::reorderUInt16(OMUInt16& i)
{
  TRACE("OMMSSStoredObject::reorderUInt16");

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
void OMMSSStoredObject::readUInt32FromStream(IStream* stream,
                                             OMUInt32& i,
                                             bool reorderBytes)
{
  TRACE("OMMSSStoredObject::readUInt32FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt32));
  if (reorderBytes) {
    reorderUInt32(i);
  }
}

  // @mfunc Write an OMUInt32 from <p i> to <p stream>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream to write to.
  //   @parm The OMUInt32 to write.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::writeUInt32ToStream(IStream* stream,
                                            OMUInt32& i,
                                            bool reorderBytes)
{
  TRACE("OMMSSStoredObject::writeUInt32ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt32 si = i;
  if (reorderBytes) {
    reorderUInt32(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt32));
}

  // @mfunc Reorder the OMUInt32 <p i>.
  //   @parm The OMUInt32 to reorder.
void OMMSSStoredObject::reorderUInt32(OMUInt32& i)
{
  TRACE("OMMSSStoredObject::reorderUInt32");

  OMUInt8* p = (OMUInt8*)&i;
  OMUInt8 temp;

  temp = p[0];
  p[0] = p[3];
  p[3] = temp;

  temp = p[1];
  p[1] = p[2];
  p[2] = temp;

}

  // @mfunc Read an OMUInt64 from <p stream> into <p i>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt64.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::readUInt64FromStream(IStream* stream,
                                             OMUInt64& i,
                                             bool reorderBytes)
{
  TRACE("OMMSSStoredObject::readUInt64FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt64));
  if (reorderBytes) {
    reorderUInt64(i);
  }
}

  // @mfunc Write an OMUInt64 from <p i> to <p stream>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream to write to.
  //   @parm The OMUInt64 to write.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::writeUInt64ToStream(IStream* stream,
                                            OMUInt64& i,
                                            bool reorderBytes)
{
  TRACE("OMMSSStoredObject::writeUInt64ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt64 si = i;
  if (reorderBytes) {
    reorderUInt64(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt64));
}

  // @mfunc Reorder the OMUInt64 <p i>.
  //   @parm The OMUInt64 to reorder.
void OMMSSStoredObject::reorderUInt64(OMUInt64& i)
{
  TRACE("OMMSSStoredObject::reorderUInt64");

  OMUInt8* p = (OMUInt8*)&i;
  OMUInt8 temp;

  temp = p[0];
  p[0] = p[7];
  p[7] = temp;

  temp = p[1];
  p[1] = p[6];
  p[6] = temp;

  temp = p[2];
  p[2] = p[5];
  p[5] = temp;

  temp = p[3];
  p[3] = p[4];
  p[4] = temp;

}

  // @mfunc Read a UniqueObjectIdentification from <p stream> into <p id>.
  //        If <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUniqueObjectIdentification.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::readUniqueObjectIdentificationFromStream(
                                              IStream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes)
{
  TRACE("OMMSSStoredObject::readUniqueObjectIdentificationFromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &id, sizeof(OMUniqueObjectIdentification));
  if (reorderBytes) {
    reorderUniqueObjectIdentification(id);
  }
}

  // @mfunc Write a UniqueObjectIdentification from <p id> to <p stream>.
  //        If <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream to write to.
  //   @parm The OMUniqueObjectIdentification to write.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::writeUniqueObjectIdentificationToStream(
                                              IStream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes)
{
  TRACE("OMMSSStoredObject::writeUniqueObjectIdentificationToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUniqueObjectIdentification sid = id;
  if (reorderBytes) {
    reorderUniqueObjectIdentification(sid);
  }
  writeToStream(stream, &sid, sizeof(OMUniqueObjectIdentification));
}

  // @mfunc Read a UniqueMaterialIdentification from <p stream> into <p id>.
  //        If <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUniqueMaterialIdentification.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::readUniqueMaterialIdentificationFromStream(
                                            IStream* stream,
                                            OMUniqueMaterialIdentification& id,
                                            bool reorderBytes)
{
  TRACE("OMMSSStoredObject::readUniqueMaterialIdentificationFromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &id, sizeof(OMUniqueMaterialIdentification));
  if (reorderBytes) {
    reorderUniqueMaterialIdentification(id);
  }
}

  // @mfunc Write a UniqueMaterialIdentification from <p id> to <p stream>.
  //        If <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream to write to.
  //   @parm The OMUniqueMaterialIdentification to write.
  //   @parm If true then reorder the bytes.
void OMMSSStoredObject::writeUniqueMaterialIdentificationToStream(
                                            IStream* stream,
                                            OMUniqueMaterialIdentification& id,
                                            bool reorderBytes)
{
  TRACE("OMMSSStoredObject::writeUniqueMaterialIdentificationToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUniqueMaterialIdentification sid = id;
  if (reorderBytes) {
    reorderUniqueMaterialIdentification(sid);
  }
  writeToStream(stream, &sid, sizeof(OMUniqueMaterialIdentification));
}

  // @mfunc Reorder the OMUniqueObjectIdentification <p id>.
  //   @parm The OMUniqueObjectIdentification to reorder.
void OMMSSStoredObject::reorderUniqueObjectIdentification(
                                              OMUniqueObjectIdentification& id)
{
  TRACE("OMMSSStoredObject::reorderUniqueObjectIdentification");

  reorderUInt32(id.Data1);
  reorderUInt16(id.Data2);
  reorderUInt16(id.Data3);
  // no need to swap Data4
}

  // @mfunc Reorder the OMUniqueMaterialIdentification <p id>.
  //   @parm The OMUniqueMaterialIdentification to reorder.
void OMMSSStoredObject::reorderUniqueMaterialIdentification(
                                            OMUniqueMaterialIdentification& id)
{
  TRACE("OMStoredMaterial::reorderUniqueMaterialIdentification");

  // No need to swap
  // SMPTELabel, length, instanceHigh, instanceMid or instanceLow.

  reorderUniqueObjectIdentification(id.material);
}

  // @mfunc Size of <p stream> in bytes.
  //   @parm An open stream.
  //   @rdesc The size of <p stream> in bytes
  //   @this const
OMUInt64 OMMSSStoredObject::streamSize(IStream* stream) const
{
  TRACE("OMMSSStoredObject::streamSize");
  PRECONDITION("Valid stream", stream != 0);

  STATSTG statstg;
  HRESULT status = stream->Stat(&statstg, STATFLAG_NONAME);
  check(status);
  ASSERT("IStream::Stat() succeeded", SUCCEEDED(status));
  OMUInt64 result = toOMUInt64(statstg.cbSize);
  return result;
}

  // @mfunc Set the size, in bytes, of <p stream>
  //   @parm An open stream.
  //   @parm The new size for the stream.
void OMMSSStoredObject::streamSetSize(IStream* stream, const OMUInt64 newSize)
{
  TRACE("OMMSSStoredObject::streamSetSize");

  ULARGE_INTEGER newStreamSize = fromOMUInt64(newSize);
  HRESULT status = stream->SetSize(newStreamSize);
  check(status);
  ASSERT("IStream::SetSize() succeeded", SUCCEEDED(status));
}

  // @mfunc The current position for <f readFromStream()> and
  //        <f writeToStream()>, as an offset in bytes from the begining
  //        of the data stream.
  //   @rdesc The current position for <f readFromStream()> and
  //          <f writeToStream()>, as an offset in bytes from the begining
  //          of the data stream.
  //   @this const
OMUInt64 OMMSSStoredObject::streamPosition(IStream* stream) const
{
  TRACE("OMMSSStoredObject::streamPosition");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt64 result;
  LARGE_INTEGER zero = {0, 0};
  ULARGE_INTEGER position;
  HRESULT status = stream->Seek(zero, STREAM_SEEK_CUR, &position);
  check(status);
  ASSERT("IStream::Seek() succeeded", SUCCEEDED(status));
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
void OMMSSStoredObject::streamSetPosition(IStream* stream,
                                          const OMUInt64 offset)
{
  TRACE("OMMSSStoredObject::streamSetPosition");
  PRECONDITION("Valid stream", stream != 0);

  ULARGE_INTEGER newPosition = fromOMUInt64(offset);
  ULARGE_INTEGER oldPosition;
  LARGE_INTEGER position;
  memcpy(&position, &newPosition, sizeof(LARGE_INTEGER));
  HRESULT status = stream->Seek(position, STREAM_SEEK_SET, &oldPosition);
  check(status);
  ASSERT("IStream::Seek() succeeded", SUCCEEDED(status));
}

  // @mfunc Close <p stream>.
  //   @parm The stream to close.
void OMMSSStoredObject::closeStream(IStream*& stream)
{
  TRACE("OMMSSStoredObject::closeStream");
  PRECONDITION("Valid stream", stream != 0);

#if defined(OM_DEBUG)
  HRESULT status = stream->Release();
  ASSERT("Reference count is 0.", status == 0);
#else
  stream->Release();
#endif
  stream = 0;
#if defined(OM_DEBUG)
  decrementOpenStreamCount();
#endif
}

  // @mfunc The persisted value of <p property> is its name.
  //        Write the property name and enter it into the property index.
  //   @parm The property.
  //   @parm TBS
  //   @this const
void OMMSSStoredObject::saveName(const OMProperty& property,
                                 const wchar_t* name)
{
  TRACE("OMMSSStoredObject::saveName");

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = property.storedForm();
  writeName(propertyId, storedForm, name);
}

  // @mfunc The persisted value of <p property> is its name.
  //        Read (and check) the property name.
  //   @parm The property.
  //   @parm The expected property name.
  //   @parm The (expected) size of the property name.
void OMMSSStoredObject::restoreName(OMProperty& property,
                                    const wchar_t* ANAME(name),
                                    size_t size)
{
  TRACE("OMMSSStoredObject::restoreName");

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = property.storedForm();
  wchar_t* propertyName = readName(propertyId, storedForm, size);
  ASSERT("Consistent property size",
                      size == (lengthOfWideString(name) + 1) *
                              sizeof(OMCharacter));
  ASSERT("Consistent property name",
                  compareWideString(propertyName, name) == 0);
  delete [] propertyName;
}

void OMMSSStoredObject::writeName(OMPropertyId pid,
                                  OMStoredForm storedForm,
                                  const wchar_t* name)
{
  TRACE("OMMSSStoredObject::writeName");

  size_t characterCount = lengthOfWideString(name) + 1;
  OMCharacter* buffer = new OMCharacter[characterCount];
  ASSERT("Valid heap pointer", buffer != 0);
  externalizeString(name, buffer, characterCount);
  if (_reorderBytes) {
    reorderString(buffer, characterCount);
  }
  size_t byteCount = characterCount * sizeof(OMCharacter);
  write(pid, storedForm, buffer, byteCount);
  delete [] buffer;
}

wchar_t* OMMSSStoredObject::readName(OMPropertyId pid,
                                     OMStoredForm storedForm,
                                     size_t size)
{
  TRACE("OMMSSStoredObject::readName");

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

void OMMSSStoredObject::reorderString(OMCharacter* string,
                                      size_t characterCount)
{
  TRACE("OMMSSStoredObject::reorderString");

  for (size_t i = 0; i < characterCount; i++) {
    reorderUInt16(string[i]);
  }
}

void OMMSSStoredObject::internalizeString(OMCharacter* externalString,
                                          wchar_t* internalString,
                                          size_t characterCount)
{
  TRACE("OMMSSStoredObject::internalizeString");

  for (size_t i = 0; i < characterCount; i++) {
    internalString[i] = externalString[i];
  }
}

void OMMSSStoredObject::externalizeString(const wchar_t* internalString,
                                          OMCharacter* externalString,
                                          size_t characterCount)
{
  TRACE("OMMSSStoredObject::externalizeString");

  for (size_t i = 0; i < characterCount; i++) {
    externalString[i] = internalString[i];
  }
}

void OMMSSStoredObject::reorderUInt16Array(OMUInt16* array,
                                           size_t elementCount)
{
  TRACE("OMMSSStoredObject::reorderUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    reorderUInt16(array[i]);
  }
}

void OMMSSStoredObject::internalizeUInt16Array(OMUInt16* externalArray,
                                               OMUInt16* internalArray,
                                               size_t elementCount)
{
  TRACE("OMMSSStoredObject::internalizeUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    internalArray[i] = externalArray[i];
  }
}

void OMMSSStoredObject::externalizeUInt16Array(const OMUInt16* internalArray,
                                               OMUInt16* externalArray,
                                               size_t elementCount)
{
  TRACE("OMMSSStoredObject::externalizeUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    externalArray[i] = internalArray[i];
  }
}

OMMSSStoredObject* OMMSSStoredObject::openFile(const wchar_t* fileName,
                                               const OMFile::OMAccessMode mode)
{
  TRACE("OMMSSStoredObject::openFile");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  DWORD openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STGM_DIRECT | STGM_READ      | STGM_SHARE_DENY_WRITE;
  }

  SSCHAR omFileName[256];
  convert(omFileName, 256, fileName);

  IStorage* storage = 0;

  HRESULT status = StgOpenStorage(
    omFileName,
    0,
    openMode,
    0,
    0,
    &storage);
  check(status);
  ASSERT("StgOpenStorage() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  OMMSSStoredObject* newStoredObject = new OMMSSStoredObject(storage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

OMMSSStoredObject* OMMSSStoredObject::createFile(const wchar_t* fileName)
{
  TRACE("OMMSSStoredObject::createFile");
  PRECONDITION("Valid file name", validWideString(fileName));

  SSCHAR omFileName[256];
  convert(omFileName, 256, fileName);

  IStorage* storage = 0;

  HRESULT status = StgCreateDocfile(
    omFileName,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_FAILIFTHERE,
    0,
    &storage);
  check(status);
  ASSERT("StgCreateDocfile() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  OMMSSStoredObject* newStoredObject = new OMMSSStoredObject(storage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

OMMSSStoredObject* OMMSSStoredObject::openFile(OMRawStorage* rawStorage,
                                               const OMFile::OMAccessMode mode)
{
  TRACE("OMMSSStoredObject::openFile");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  ILockBytes* iLockBytes = new OMRawStorageLockBytes(rawStorage);
  ASSERT("Valid heap pointer", iLockBytes != 0);

  DWORD openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STGM_DIRECT | STGM_READ      | STGM_SHARE_DENY_WRITE;
  }

  IStorage* storage = 0;

  HRESULT status = StgOpenStorageOnILockBytes(
    iLockBytes,
    0,
    openMode,
    0,
    0,
    &storage);
  check(status);
  ASSERT("StgOpenStorageOnILockBytes() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  iLockBytes->Release();
  OMMSSStoredObject* newStoredObject = new OMMSSStoredObject(storage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

OMMSSStoredObject* OMMSSStoredObject::createFile(OMRawStorage* rawStorage)
{
  TRACE("OMMSSStoredObject::createFile");
  PRECONDITION("Valid raw storage", rawStorage != 0);

  ILockBytes* iLockBytes = new OMRawStorageLockBytes(rawStorage);
  ASSERT("Valid heap pointer", iLockBytes != 0);

  IStorage* storage = 0;

  HRESULT status = StgCreateDocfileOnILockBytes(
    iLockBytes,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
    0,
    &storage);
  check(status);
  ASSERT("StgCreateDocfileOnILockBytes() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  iLockBytes->Release();
  OMMSSStoredObject* newStoredObject = new OMMSSStoredObject(storage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

static const wchar_t* const propertyStreamName = L"properties";

void OMMSSStoredObject::create(const OMByteOrder byteOrder)
{
  TRACE("OMMSSStoredObject::create");
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));
  PRECONDITION("Not already open", !_open);

  _byteOrder = byteOrder;
  if (_byteOrder == hostByteOrder()) {
    _reorderBytes = false;
  } else {
    _reorderBytes = true;
  }
  _mode = OMFile::modifyMode;
  _properties = createStream(_storage, propertyStreamName);
  _open = true;
}

void OMMSSStoredObject::open(const OMFile::OMAccessMode mode)
{
  TRACE("OMMSSStoredObject::open");
  PRECONDITION("Not already open", !_open);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  _mode = mode;
  _properties = openStream(_storage, propertyStreamName);
  _open = true;
}

void OMMSSStoredObject::save(OMStoredPropertySetIndex* index)
{
  TRACE("OMMSSStoredObject::save(OMStoredPropertySetIndex*)");
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
  writeUInt8ToStream(_properties, _byteOrder);

  // Write version number.
  //
  OMVersion version = currentVersion;
  writeUInt8ToStream(_properties, version);

  // Write count of entries.
  //
  writeUInt16ToStream(_properties, entries, _reorderBytes);

  // Write entries.
  //
  OMPropertyId propertyId;
  OMStoredForm type;
  OMUInt32 offset;
  OMPropertySize length;
  size_t context = 0;
  for (size_t i = 0; i < entries; i++) {
    index->iterate(context, propertyId, type, offset, length);
    writeUInt16ToStream(_properties, propertyId, _reorderBytes);
    writeUInt16ToStream(_properties, type, _reorderBytes);
    writeUInt16ToStream(_properties, length, _reorderBytes);
  }

  streamSetPosition(_properties, 0);
  POSTCONDITION("At start of index stream", streamPosition(_properties) == 0);
}

OMStoredPropertySetIndex* OMMSSStoredObject::restore(void)
{
  TRACE("OMMSSStoredObject::restore");
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

  // @mfunc The stream name for the index of a collection
  //        named <p collectionName>.
  //   @parm The collection name.
  //   @rdesc The stream name for the collection index.
wchar_t* OMMSSStoredObject::collectionIndexStreamName(
                                                 const wchar_t* collectionName)
{
  TRACE("OMMSSStoredObject::collectionIndexStreamName");
  PRECONDITION("Valid collection name", validWideString(collectionName));

  wchar_t* suffix = L" index";
  size_t length = lengthOfWideString(collectionName) +
                  lengthOfWideString(suffix) + 1;
  wchar_t* indexName = new wchar_t[length];
  ASSERT("Valid heap pointer", indexName != 0);
  copyWideString(indexName,
                 collectionName,
                 lengthOfWideString(collectionName) + 1);
  concatenateWideString(indexName, suffix, lengthOfWideString(suffix) + 1);

  return indexName;
}

IStream* OMMSSStoredObject::createStream(IStorage* storage,
                                         const wchar_t* streamName)
{
  TRACE("OMMSSStoredObject::createStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  DWORD mode = STGM_DIRECT | STGM_READWRITE |
               STGM_SHARE_EXCLUSIVE  | STGM_CREATE;

  IStream* stream = 0;
  SSCHAR omStreamName[256];
  convert(omStreamName, 256, streamName);

  HRESULT status = storage->CreateStream(
    omStreamName,
    mode,
    0,
    0,
    &stream);
  check(status);
  ASSERT("IStorage::CreateStream() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStreamCount();
#endif

  return stream;
}

IStream* OMMSSStoredObject::openStream(IStorage* storage,
                                       const wchar_t* streamName)
{
  TRACE("OMMSSStoredObject::openStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));

  DWORD mode;
  if (_mode == OMFile::modifyMode) {
    mode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (_mode == OMFile::readOnlyMode) {
    mode = STGM_DIRECT | STGM_READ      | STGM_SHARE_EXCLUSIVE;
  }

  IStream* stream = 0;
  SSCHAR omStreamName[256];
  convert(omStreamName, 256, streamName);

  HRESULT status = storage->OpenStream(
    omStreamName,
    0,
    mode,
    0,
    &stream);
  check(status);
  ASSERT("IStorage::OpenStream() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStreamCount();
#endif

  return stream;

}

IStorage* OMMSSStoredObject::createStorage(IStorage* storage,
                                           const wchar_t* storageName)
{
  TRACE("createStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validWideString(storageName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  DWORD mode = STGM_DIRECT | STGM_READWRITE |
               STGM_SHARE_EXCLUSIVE  | STGM_CREATE;

  IStorage* newStorage = 0;
  SSCHAR omStorageName[256];
  convert(omStorageName, 256, storageName);

  HRESULT status = storage->CreateStorage(
    omStorageName,
    mode,
    0,
    0,
    &newStorage);
  check(status);
  ASSERT("IStorage::CreateStorage() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  return newStorage;
}

IStorage* OMMSSStoredObject::openStorage(IStorage* storage,
                                         const wchar_t* storageName,
                                         const OMFile::OMAccessMode mode)
{
  TRACE("openStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validWideString(storageName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  DWORD openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STGM_DIRECT | STGM_READ      | STGM_SHARE_EXCLUSIVE;
  }

  IStorage* newStorage = 0;
  SSCHAR omStorageName[256];
  convert(omStorageName, 256, storageName);

  HRESULT status = storage->OpenStorage(
    omStorageName,
    0,
    openMode,
    0,
    0,
    &newStorage);
  check(status);
  ASSERT("IStorage::OpenStorage() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  return newStorage;
}

void OMMSSStoredObject::closeStorage(IStorage*& storage)
{
  TRACE("closeStorage");
  PRECONDITION("Valid storage", storage != 0);

#if defined(OM_DEBUG)
  HRESULT status = storage->Release();
  ASSERT("Reference count is 0.", status == 0);
#else
  storage->Release();
#endif
  storage = 0;
#if defined(OM_DEBUG)
  decrementOpenStorageCount();
#endif
}

void OMMSSStoredObject::setClass(IStorage* storage, const OMClassId& cid)
{
  TRACE("OMMSSStoredObject::setClass");
  PRECONDITION("Valid storage", storage != 0);

  GUID g;
  memcpy(&g, &cid, sizeof(GUID));
  HRESULT status = storage->SetClass(g);
  check(status);
  ASSERT("IStorage::SetClass() succeeded", SUCCEEDED(status));
}

void OMMSSStoredObject::getClass(IStorage* storage, OMClassId& cid)
{
  TRACE("OMMSSStoredObject::getClass");
  PRECONDITION("Valid storage", storage != 0);

  STATSTG statstg;
  HRESULT status = storage->Stat(&statstg, STATFLAG_NONAME);
  check(status);
  ASSERT("IStorage::Stat() succeeded", SUCCEEDED(status));
  memcpy(&cid, &statstg.clsid, sizeof(OMClassId));
}

  // @mfunc Write the signature to the given raw storage.
  //   @parm The raw storage.
  //   @parm The signature.
void OMMSSStoredObject::writeSignature(OMRawStorage* rawStorage,
                                       const OMFileSignature& signature)
{
  TRACE("OMMSSStoredObject::writeSignature");

  OMFileSignature sig = signature;
  if (hostByteOrder() != littleEndian) {
    OMByte* s = reinterpret_cast<OMByte*>(&sig);
    size_t size = sizeof(OMUniqueObjectIdentification);
    OMUniqueObjectIdentificationType::instance()->reorder(s, size);
  }

  OMUInt32 count;
  rawStorage->writeAt(8,
                      reinterpret_cast<const OMByte*>(&sig),
                      sizeof(sig),
                      count);
  ASSERT("All bytes written", count == sizeof(sig));
}

  // @mfunc Write the signature to the given file.
  //   @parm The file name.
  //   @parm The signature.
void OMMSSStoredObject::writeSignature(const wchar_t* fileName,
                                       const OMFileSignature& signature)
{
  TRACE("OMMSSStoredObject::writeSignature");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMFileSignature sig = signature;

  if (hostByteOrder() != littleEndian) {
    OMByte* s = reinterpret_cast<OMByte*>(&sig);
    size_t size = sizeof(OMUniqueObjectIdentification);
    OMUniqueObjectIdentificationType::instance()->reorder(s, size);
  }

  FILE* f = wfopen(fileName, L"rb+");
  ASSERT("File exists", f != 0);
  size_t status = fseek(f, 8, SEEK_SET);
  ASSERT("Seek succeeded", status == 0);
  status = fwrite(&sig, sizeof(sig), 1, f);
  ASSERT("Write succeeded", status == 1);

  fclose(f);
}

// We may need to convert our arguments, which are always UNICODE
// strings (wchar_t*), to regular character strings (char*), before
// passing them to the Structured Storage API. We always call
// convert() on our arguments before calling the Structured Storage
// API. convert() has two different definitions, depending on
// OM_UNICODE_APIS.
//
#if defined(OM_UNICODE_APIS)

// We have UNICODE Structured Storage APIs - conversion is copy.

static void convert(wchar_t* wcName, size_t length, const wchar_t* name)
{
  TRACE("convert");
  PRECONDITION("Valid input name", validWideString(name));
  PRECONDITION("Valid output buffer", wcName != 0);
  PRECONDITION("Valid output buffer size", length > 0);

  size_t sourceLength = lengthOfWideString(name);
  ASSERT("Output buffer large enough", length > sourceLength + 1);
  copyWideString(wcName, name, sourceLength + 1);
}

#else

// We don't have UNICODE Structured Storage APIs  - convert with wcstombs().

static void convert(char* cName, size_t length, const wchar_t* name)
{
  TRACE("convert");
  PRECONDITION("Valid input name", validWideString(name));
  PRECONDITION("Valid output buffer", cName != 0);
  PRECONDITION("Valid output buffer size", length > 0);

  size_t status = wcstombs(cName, name, length);
  ASSERT("wcstombs() succeeded", status != (size_t)-1);
}

#endif

static void check(HRESULT status)
{
  TRACE("check");

  if (FAILED(status)) {
    throw OMException(status);
  }
}

#if defined(OM_WINDOWS_SS) || defined(OM_MACINTOSH_SS)

// Simple class to ensure COM initialization/uninitialization.

class MSSInitializer {
public:

  MSSInitializer(void)
  {
    CoInitialize(0);
  }

  ~MSSInitializer(void)
  {
    CoUninitialize();
  }

};

// Perform COM initialization during static initialization
//
static MSSInitializer _MSSInitializer;

#endif

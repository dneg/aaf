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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMKLVStoredObject
#include "OMKLVStoredObject.h"

#include "OMCachedDiskRawStorage.h"
#include "OMKLVStoredStream.h"
#include "OMProperty.h"
#include "OMPropertySetIterator.h"
#include "OMStrongRefProperty.h"
#include "OMStrongReference.h"
#include "OMStrongReferenceSet.h"
#include "OMStrongReferenceVector.h"
#include "OMObjectReference.h"
#include "OMContainerElement.h"
#include "OMDataStream.h"
#include "OMIntegerType.h"
#include "OMUniqueObjectIdentType.h"
#include "OMRootStorable.h"
#include "OMVector.h"
#include "OMVectorIterator.h"
#include "OMWeakReference.h"
#include "OMWeakReferenceVector.h"
#include "OMWeakReferenceSet.h"
#include "OMPropertyTable.h"
#include "OMStrongReferenceSetIter.h"
#include "OMClassDefinition.h"
#include "OMPropertyDefinition.h"

#include "OMUtilities.h"

#include "OMRawStorage.h"

#define USETAGTABLE 1
//#define INSTANCEID_DEBUG 1
//#define MAP_CLASSIDS 1

#if defined(INSTANCEID_DEBUG)
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::hex;
using std::setw;
using std::setfill;
using std::dec;
#endif

  // @mfunc Open the root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMKLVStoredObject* OMKLVStoredObject::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMKLVStoredObject::openRead");
  PRECONDITION("Compatible raw storage access mode", rawStorage->isReadable());
  OMKLVStoredObject* result = new OMKLVStoredObject(rawStorage, littleEndian);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Open the root <c OMKLVStoredObject> in the raw storage
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
  //        <p rawStorage> for modification.
OMKLVStoredObject* OMKLVStoredObject::openModify(
                                               OMRawStorage* ANAME(rawStorage))
{
  TRACE("OMKLVStoredObject::openModify");
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create a new root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMKLVStoredObject* OMKLVStoredObject::createWrite(OMRawStorage* rawStorage,
                                                  const OMByteOrder byteOrder)
{
  TRACE("OMKLVStoredObject::createWrite");
  PRECONDITION("Compatible raw storage access mode", rawStorage->isWritable());
  OMKLVStoredObject* result= new OMKLVStoredObject(rawStorage, byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Create a new root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMKLVStoredObject* OMKLVStoredObject::createModify(OMRawStorage* rawStorage,
                                                   const OMByteOrder byteOrder)
{
  TRACE("OMKLVStoredObject::createModify");
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());
  OMKLVStoredObject* result= new OMKLVStoredObject(rawStorage, byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Destructor.
OMKLVStoredObject::~OMKLVStoredObject(void)
{
  TRACE("OMKLVStoredObject::~OMKLVStoredObject");
}

  // @mfunc Create a new <c OMKLVStoredObject>, named <p name>,
  //        contained by this <c OMKLVStoredObject>.
  //   @parm The name to be used for the new <c OMKLVStoredObject>.
  //   @rdesc A new <c OMKLVStoredObject> contained by this
  //          <c OMKLVStoredObject>.
OMStoredObject* OMKLVStoredObject::create(const wchar_t* /* name */)
{
  TRACE("OMKLVStoredObject::create");
  OMStoredObject* result = new OMKLVStoredObject(_storage, _byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Open an exsiting <c OMKLVStoredObject>, named <p name>,
  //        contained by this <c OMKLVStoredObject>.
  //   @parm The name of the existing <c OMKLVStoredObject>.
  //   @rdesc The existing <c OMKLVStoredObject> contained by this
  //          <c OMKLVStoredObject>.
OMStoredObject* OMKLVStoredObject::open(const wchar_t* /* name */)
{
  TRACE("OMKLVStoredObject::open");
  OMStoredObject* result = new OMKLVStoredObject(_storage, _byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Close this <c OMKLVStoredObject>.
void OMKLVStoredObject::close(void)
{
  TRACE("OMKLVStoredObject::close");
}

  // @mfunc The byte order of this <c OMKLVStoredObject>.
  //   @rdesc The byte order.
  //   @this const
OMByteOrder OMKLVStoredObject::byteOrder(void) const
{
  TRACE("OMKLVStoredObject::byteOrder");

  return _byteOrder;
}

#if defined(USETAGTABLE)
static struct {
  OMPropertyTag _tag;
  OMUInt16 _pids[4];
  wchar_t* _path;
} map [] = {
  {0xffff,
   {0x0001, 0x0003},         L"/MetaDictionary/ClassDefinitions"},
  {0xffff,
   {0x0001, 0x0004},         L"/MetaDictionary/TypeDefinitions"},
  {0xffff,
   {0x0002, 0x3b04, 0x2605}, L"/Header/Dictionary/DataDefinitions"},
  {0xffff,
   {0x0002, 0x3b04, 0x2608}, L"/Header/Dictionary/ContainerDefinitions"},
  {0xffff,
   {0x0002, 0x3b04, 0x2607}, L"/Header/Dictionary/CodecDefinitions"},
  {0xffff,
   {0x0002, 0x3b04, 0x2603}, L"/Header/Dictionary/OperationDefinitions"},
  {0xffff,
   {0x0002, 0x3b04, 0x2609}, L"/Header/Dictionary/InterpolationDefinitions"},
  {0xffff,
   {0x0002, 0x3b04, 0x2604}, L"/Header/Dictionary/ParameterDefinitions"},
};

static size_t mapSize = sizeof(map)  / sizeof(map[0]);
static size_t pidsSize = sizeof(map[0]._pids) / sizeof(map[0]._pids[0]);

static void initializeMap(OMFile& file)
{
  for (size_t i = 0; i < mapSize; i++) {
    OMPropertyTag tag = file.referencedProperties()->insert(map[i]._pids);
    map[i]._tag = tag;
  }
}

static OMPropertyTag findTag(const wchar_t* path)
{
  TRACE("findTag");
  OMPropertyTag result =  nullOMPropertyTag;
  for (size_t i = 0; i < mapSize; i++) {
    if (compareWideString(path, map[i]._path) == 0) {
      result = map[i]._tag;
      break;
    }
  }
  POSTCONDITION("", result != nullOMPropertyTag);
  return result;
}
#endif

void OMKLVStoredObject::save(OMFile& file)
{
  TRACE("OMKLVStoredObject::save(OMFile)");

#if defined(INSTANCEID_DEBUG)
  cout << endl;
  for (size_t i = 0; i < mapSize; i++) {
    cout << i << " ";
    for (size_t k = 0; k < pidsSize; k++) {
      cout << hex << setw(4) << setfill('0') << map[i]._pids[k] << " ";
    }
    cout << map[i]._path << endl;
  }
#endif

  // The header partition has already been written,
  // start saving immediately after the header partition
  // and any filler.
  //
  _storage->setPosition(0);
  readHeaderPartition(_storage);
  readKLVFill(_storage);

  OMUInt64 pos = _storage->position();
  _storage->setPosition(pos);

  // Write the primer
  writePrimerPack(_storage, file.dictionary(), _reorderBytes);

  // Save the rest of the file
  file.root()->save();

  // Insert alignment filler
  OMUInt32 bodyPartitionOffset = 0x20000; // Get this from header ?
  OMUInt32 KAGSize = 0x200;
  OMUInt32 fillAlignment;
  if (metaDataOnly) {
    // fill to next KAG
    fillAlignment = KAGSize;
  } else {
    // fill remainder of pre-allocated space
    fillAlignment = bodyPartitionOffset;
  }
  OMUInt64 currentPosition = _storage->position();
  fill(_storage, currentPosition, fillAlignment);

  OMKLVKey bodyPartitionPackKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
     0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x00, 0x00};

  OMKLVKey fillKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
     0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};

  if (!metaDataOnly) {
    // Skip body partition and any following fill
    //
    if (bodyPartitionOffset < _storage->size() ) {
      OMUInt64 savedPosition = _storage->position();
      _storage->setPosition(bodyPartitionOffset);
      // skip body partition
      OMKLVKey k;
      readKLVKey(_storage, k);
      k.octet14 = 0x00;
      if (memcmp(&k, &bodyPartitionPackKey, sizeof(OMKLVKey)) == 0) {
        OMUInt64 length = readKLVLength(_storage);
        OMUInt64 currentPosition = _storage->position();
        _storage->setPosition(currentPosition + length);
        // skip fill
        OMKLVKey fk;
        readKLVKey(_storage, fk);
        if (memcmp(&fk, &fillKey, sizeof(OMKLVKey)) == 0) {
          skipLV(_storage);
        } else {
          _storage->setPosition(currentPosition + length);
        }
      } else {
        _storage->setPosition(savedPosition);
      }
    }
  }

  // Save streams
  if (!metaDataOnly) {
    streamSave(*file.root()->propertySet());
  }
}

void OMKLVStoredObject::save(OMStorable& object)
{
  TRACE("OMKLVStoredObject::save(OMFile)");
  save(object.classId());
  save(*object.propertySet());
}

  // @mfunc Save the <c OMStoredObjectIdentification> <p id>
  //        in this <c OMKLVStoredObject>.
  //   @parm The <t OMStoredObjectIdentification> to save.
void OMKLVStoredObject::save(const OMStoredObjectIdentification& id)
{
  TRACE("OMKLVStoredObject::save(OMStoredObjectIdentification)");

  OMKLVKey k;
  convert(k, id);
  writeKLVKey(_storage, k);
}

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMKLVStoredObject::save(const OMPropertySet& properties)
{
  TRACE("OMKLVStoredObject::save(OMPropertySet)");

  // Length
  OMUInt64 setLength = length(properties);
  writeKLVLength(_storage, setLength);

  // Flat properties
  flatSave(properties);

  // Deep properties
  deepSave(properties);
}

  // @mfunc Save the <c OMSimpleProperty> <p property> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMSimpleProperty> to save.
void OMKLVStoredObject::save(const OMSimpleProperty& property)
{
  TRACE("OMKLVStoredObject::save(OMSimpleProperty)");

  OMPropertySize size = property.bitsSize();
  OMByte* bits = property.bits();
  const OMType* propertyType = property.type();
  ASSERT("Valid property type", propertyType != 0);

  // Allocate buffer for property value
  OMPropertySize externalBytesSize = propertyType->externalSize(bits, size);
  OMByte* buffer = new OMByte[externalBytesSize];
  ASSERT("Valid heap pointer", buffer != 0);

  // Externalize property value
  propertyType->externalize(bits,
                            size,
                            buffer,
                            externalBytesSize,
                            hostByteOrder());

  // Reorder property value
  if (_reorderBytes) {
    propertyType->reorder(buffer, externalBytesSize);
  }

  // size
  write(_storage, externalBytesSize, _reorderBytes);
  // value
  write(_storage, buffer, externalBytesSize);

  delete [] buffer;
}

  // @mfunc Save the <c OMStrongReference> <p singleton> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMKLVStoredObject::save(const OMStrongReference& singleton)
{
  TRACE("OMKLVStoredObject::save(OMStrongReference)");
  singleton.reference().save();
}

  // @mfunc Save the <c OMStrongReferenceVector> <p vector> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMStrongReferenceVector> to save.
void OMKLVStoredObject::save(const OMStrongReferenceVector& vector)
{
  TRACE("OMKLVStoredObject::save(OMStrongReferenceVector)");

  OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
                                                            *vector.iterator();
  while (++iterator) {

    OMStrongReferenceVectorElement& element = iterator.value();

    // save the object
    //
    element.save();

  }
  delete &iterator;
}

  // @mfunc Save the <c OMStrongReferenceSet> <p set> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMKLVStoredObject::save(const OMStrongReferenceSet& set)
{
  TRACE("OMKLVStoredObject::save(OMStrongReferenceSet)");

  OMContainerIterator<OMStrongReferenceSetElement>& iterator = *set.iterator();
  while (++iterator) {
    OMStrongReferenceSetElement& element = iterator.value();

    // Save elements that are sticky or that are referenced.
    if (element.isSticky() || element.referenceCount() > 0) {

      // object
      element.save();
    }
  }
  delete &iterator;
}

  // @mfunc Save the <c OMWeakReference> <p singleton> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMWeakReference> to save.
void OMKLVStoredObject::save(const OMWeakReference& /* singleton */)
{
  TRACE("OMKLVStoredObject::save(OMWeakReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMWeakReferenceVector> <p vector> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMWeakReferenceVector> to save.
void OMKLVStoredObject::save(const OMWeakReferenceVector& /* vector */)
{
  TRACE("OMKLVStoredObject::save(OMWeakReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMWeakReferenceSet> <p set> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMWeakReferenceSet> to save.
void OMKLVStoredObject::save(const OMWeakReferenceSet& /* set */)
{
  TRACE("OMKLVStoredObject::save(OMWeakReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMPropertyTable> <p table> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMPropertyTable> to save.
void OMKLVStoredObject::save(const OMPropertyTable* /* table */)
{
  TRACE("OMKLVStoredObject::save(OMPropertyTable)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMDataStream> <p stream> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMDataStream> to save.
void OMKLVStoredObject::save(const OMDataStream& stream)
{
  TRACE("OMKLVStoredObject::save(OMDataStream)");

  OMDataStream* s = const_cast<OMDataStream*>(&stream);
  OMKLVKey id = streamId(s);

  writeKLVKey(_storage, id);
  OMUInt64 length = stream.size();
  writeKLVLength(_storage, length);

  // value
  stream.setPosition(0);
  for (OMUInt64 i = 0; i < length; i++) {
    OMByte b;
    OMUInt32 x;
    stream.read(&b, 1, x);
    write(_storage, b);
  }
}

OMRootStorable* OMKLVStoredObject::restore(OMFile& file)
{
  TRACE("OMKLVStoredObject::restore(OMFile)");

#if defined(USETAGTABLE)
  initializeMap(file);
#endif

  // Read the header partition
  readHeaderPartition(_storage);

  readKLVFill(_storage);

  // Read the primer
  readPrimerPack(_storage, file.dictionary(), _reorderBytes);

  file.setLoadMode(OMFile::lazyLoad);

  readKLVFill(_storage);

  OMKLVKey fillKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
     0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};

  // For restoring meta objects
  OMDictionary* metaDictionary = file.dictionary();
  // For restoring objects
  const OMClassFactory* dictionary = file.classFactory();

  // restore the root
  //
  OMKLVKey k;
  readKLVKey(_storage, k);
  OMClassId cid;
  convert(cid, k);
  ASSERT("Root object", cid == OMRootStorable::_rootClassId);
  OMRootStorable* root = new OMRootStorable();
  ASSERT("Valid heap pointer", root != 0);
  root->attach(&file);
  root->setStore(file.rootStore());
  root->setClassFactory(metaDictionary);
  flatRestore(*root->propertySet());

  // restore the meta dictionary
  //
  readKLVKey(_storage, k);
  convert(cid, k);

  while (metaDictionary->isMeta(cid)) {
    OMStorable* object = metaDictionary->create(cid);
    ASSERT("Registered class id", object != 0);
    ASSERT("Valid class factory", metaDictionary == object->classFactory());
#if !defined(OM_NO_VALIDATE_DEFINITIONS)
    ASSERT("Valid class definition", object->definition() != 0);
#endif
    // Attach the object.
    // tjb !!! object->attach(containingObject, name);
    // tjb !!! object->setStore(this);
    object->onRestore(file.clientOnSaveContext());

    flatRestore(*object->propertySet());

    readKLVKey(_storage, k);
    convert(cid, k);
  }
  OMProperty* mdp = root->propertySet()->get(0x0001);
  OMStrongReference* mdsr = dynamic_cast<OMStrongReference*>(mdp);
  ASSERT("Valid type", mdsr != 0);
  OMStrongObjectReference& mdr = mdsr->reference();
  OMStorable* mdo = mdr.getValue();
  ASSERT("Valid object", mdo != 0);
  deepRestore(*mdo->propertySet());

  // restore the client root
  //
  root->setClassFactory(dictionary);
  while (memcmp(&k, &fillKey, sizeof(OMKLVKey)) != 0) {
    convert(cid, k);
    OMStorable* object = dictionary->create(cid);
    ASSERT("Registered class id", object != 0);
    ASSERT("Valid class factory", dictionary == object->classFactory());
#if !defined(OM_NO_VALIDATE_DEFINITIONS)
    ASSERT("Valid class definition", object->definition() != 0);
#endif
    // Attach the object.
    // tjb !!! object->attach(containingObject, name);
    // tjb !!! object->setStore(this);
    object->onRestore(file.clientOnSaveContext());

    flatRestore(*object->propertySet());

    readKLVKey(_storage, k);
  }
  OMProperty* hp = root->propertySet()->get(0x0002);
  OMStrongReference* hsr = dynamic_cast<OMStrongReference*>(hp);
  ASSERT("Valid type", hsr != 0);
  OMStrongObjectReference& hr = hsr->reference();
  OMStorable* ho = hr.getValue();
  ASSERT("Valid object", ho != 0);
  deepRestore(*ho->propertySet());

  skipLV(_storage); // This V is fill

  if (!metaDataOnly) {
    streamRestore(_storage);
  }
#if defined(INSTANCEID_DEBUG)

  if (_instanceIdToObject != 0) {

    OMUInt32 n = _instanceIdToObject->count();
    cout << dec << n << " objects" << endl;

    OMSetIterator<OMUniqueObjectIdentification, OMStorable*>
                                      iterator(*_instanceIdToObject, OMBefore);
    while (++iterator) {
      OMStorable* object = iterator.value();
      OMUniqueObjectIdentification id = iterator.key();
      char buffer[OMObjectIdentificationStringBufferSize];
      toString(id, buffer);
      cout << buffer << " -> " << object << endl;
    }
  }
#endif

  return root;
}

OMStorable*
OMKLVStoredObject::restoreObject(const OMStrongObjectReference& reference)
{
  TRACE("OMKLVStoredObject::restoreObject");
  const wchar_t* name = reference.name();
  char* cName = convertWideString(name);
  OMUniqueObjectIdentification id;
  fromString(id, cName);
  delete [] cName;
  OMStorable* result = object(id);
  ASSERT("Object found", result != 0);
    // Attach the object.
  OMProperty* property = reference.property();
  OMStorable* containingObject = property->propertySet()->container();
  result->attach(containingObject, name);
  result->setStore(this);
  return result;
}

  // @mfunc Restore the <c OMStoredObjectIdentification>
  //        of this <c OMKLVStoredObject> into <p id>.
  //   @parm The newly restored <t OMStoredObjectIdentification>.
void OMKLVStoredObject::restore(OMStoredObjectIdentification& /* id */)
{
  TRACE("OMKLVStoredObject::restore(OMStoredObjectIdentification)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMPropertySet> <p properties> into
  //        this <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMPropertySet>.
void OMKLVStoredObject::restore(OMPropertySet& /* properties */)
{
  TRACE("OMKLVStoredObject::restore(OMPropertySet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMSimpleProperty> <p property> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMSimpleProperty>
  //   @parm The external size.
void OMKLVStoredObject::restore(OMSimpleProperty& property,
                                size_t externalSize)
{
  TRACE("OMKLVStoredObject::restore(OMSimpleProperty)");

  const OMType* propertyType = property.type();
  ASSERT("Valid property type", propertyType != 0);

  // Allocate buffer for property value
  OMByte* buffer = new OMByte[externalSize];
  ASSERT("Valid heap pointer", buffer != 0);

  read(_storage, buffer, externalSize);

  // Reorder property value
  if (_reorderBytes) {
    propertyType->reorder(buffer, externalSize);
  }

  // Internalize property value
  size_t requiredBytesSize = propertyType->internalSize(buffer,
                                                        externalSize);

  property.setSize(requiredBytesSize);
  ASSERT("Property value buffer large enough",
                                       property.size() >= requiredBytesSize);
  OMByte* bits = property.bits();
  propertyType->internalize(buffer,
                            externalSize,
                            bits,
                            requiredBytesSize,
                            hostByteOrder());
  delete [] buffer;
}

  // @mfunc Restore the <c OMStrongReference> <p singleton> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMStrongReference>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMStrongReference& singleton ,
                                size_t ANAME(externalSize))
{
  TRACE("OMKLVStoredObject::restore(OMStrongReference)");

  ASSERT("Valid size", externalSize == sizeof(OMUniqueObjectIdentification));
  OMUniqueObjectIdentification id;
  read(_storage, id, _reorderBytes);

  char idString[OMObjectIdentificationStringBufferSize];
  toString(id, idString);

  wchar_t* name = convertString(idString);
  OMStrongObjectReference newReference(&singleton, name, false);
  delete [] name;
  singleton.reference() = newReference;
}

  // @mfunc Restore the <c OMStrongReferenceVector> <p vector> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMStrongReferenceVector>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMStrongReferenceVector& /* vector */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMStrongReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReferenceSet> <p set> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMStrongReferenceSet>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMStrongReferenceSet& /* set */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMStrongReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReference> <p singleton> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMWeakReference>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMWeakReference& singleton,
                                size_t ANAME(externalSize))
{
  TRACE("OMKLVStoredObject::restore(OMWeakReference)");

  ASSERT("Valid size", externalSize == sizeof(OMUniqueObjectIdentification));
  OMUniqueObjectIdentification id;
  read(_storage, id, _reorderBytes);

  OMWeakObjectReference newReference(&singleton, id, 0);
  singleton.reference() = newReference;
}

  // @mfunc Restore the <c OMWeakReferenceVector> <p vector> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMWeakReferenceVector>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMWeakReferenceVector& /* vector */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMWeakReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReferenceSet> <p set> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMWeakReferenceSet>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMWeakReferenceSet& /* set */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMWeakReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMPropertyTable> in this <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMPropertyTable>.
void OMKLVStoredObject::restore(OMPropertyTable*& /* table */)
{
  TRACE("OMKLVStoredObject::restore(OMPropertyTable)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMDataStream> <p stream> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMDataStream>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMDataStream& /* stream */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMDataStream)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Open the <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMKLVStoredObject>.
  //   @parm The <c OMDataStream> to be opened.
  //   @rdesc The newly created <c OMStoredStream>.
OMStoredStream* OMKLVStoredObject::openStoredStream(
                                            const OMDataStream& /* property */)
{
  TRACE("OMKLVStoredObject::openStoredStream");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create an <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMKLVStoredObject>.
  //   @parm The <c OMDataStream> to be created.
  //   @rdesc The newly created <c OMStoredStream>.
OMStoredStream* OMKLVStoredObject::createStoredStream(
                                            const OMDataStream& /* property */)
{
  TRACE("OMKLVStoredObject::createStoredStream");

  OMRawStorage* store = OMCachedDiskRawStorage::openNewModify();
  OMKLVStoredStream* result = new OMKLVStoredStream(store);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

OMUInt64 OMKLVStoredObject::length(const OMPropertySet& properties) const
{
  TRACE("OMKLVStoredObject::length");

  OMUInt64 length = 0;
  length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                  + sizeof(OMUniqueObjectIdentification);
  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    ASSERT("Property has a definition", p->definition() != 0);
    if (!p->isOptional() || p->isPresent()) {
      switch (p->storedForm()) {
      case SF_DATA: {
        OMSimpleProperty* sp = dynamic_cast<OMSimpleProperty*>(p);
        ASSERT("Correct type", sp != 0);
        OMSimpleProperty& property = *sp;
        OMPropertySize size = property.bitsSize();
        OMByte* bits = property.bits();
        const OMType* type = property.type();
        ASSERT("Valid property type", type != 0);
        OMPropertySize s = type->externalSize(bits, size);
        length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize) + s;
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE: {
        length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                        + sizeof(OMUniqueObjectIdentification);
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_VECTOR: {
        length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                        + sizeof(OMUInt32) + sizeof(OMUInt32);
        OMStrongReferenceVector* v = dynamic_cast<OMStrongReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMUInt32 elements = v->count();
        length = length + (elements * sizeof(OMUniqueObjectIdentification));
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_SET: {
        length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                        + sizeof(OMUInt32) + sizeof(OMUInt32);
        OMStrongReferenceSet* s = dynamic_cast<OMStrongReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 elements = s->count();
        length = length + (elements * sizeof(OMUniqueObjectIdentification));
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE: {
        length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                        + sizeof(OMUniqueObjectIdentification);
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_VECTOR: {
        length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                        + sizeof(OMUInt32) + sizeof(OMUInt32);
        OMWeakReferenceVector* v = dynamic_cast<OMWeakReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMUInt32 elements = v->count();
        length = length + (elements * sizeof(OMUniqueObjectIdentification));
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_SET: {
        length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                        + sizeof(OMUInt32) + sizeof(OMUInt32);
        OMWeakReferenceSet* s = dynamic_cast<OMWeakReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 elements = s->count();
        length = length + (elements * sizeof(OMUniqueObjectIdentification));
        break;
      }
      case SF_DATA_STREAM: {
        length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                        + sizeof(OMByteOrder)
                        + sizeof(OMKLVKey);
        break;
      }
      default:
        break;
      }
    }
  }
  return length;
}

void OMKLVStoredObject::flatSave(const OMPropertySet& properties) const
{
  TRACE("OMKLVStoredObject::flatSave");

  referenceSave(properties.container(), 0x3c0a);

  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    ASSERT("Property has a definition", p->definition() != 0);
    if (!p->isOptional() || p->isPresent()) {
      switch (p->storedForm()) {
      case SF_DATA: {
        OMPropertyId id = p->propertyId();
        write(_storage, id, _reorderBytes);
        p->save();
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE: {
        OMStrongReference* sr = dynamic_cast<OMStrongReference*>(p);
        ASSERT("Valid type", sr != 0);
        OMStrongObjectReference& r = sr->reference();
        OMStorable* object = r.getValue();
        ASSERT("Valid object", object != 0);
        OMPropertyId pid = p->propertyId();
        referenceSave(object, pid);
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_VECTOR: {
        OMPropertyId id = p->propertyId();
        write(_storage, id, _reorderBytes);
        OMStrongReferenceVector* v = dynamic_cast<OMStrongReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMUInt32 elementCount = v->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = sizeof(OMUInt32) + sizeof(OMUInt32)
                              + (elementCount * elementSize);
        write(_storage, size, _reorderBytes);
        write(_storage, elementCount, _reorderBytes);
        write(_storage, elementSize, _reorderBytes);
        OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
                                                                *v->iterator();
        while (++iterator) {
          OMStrongReferenceVectorElement& element = iterator.value();
          OMStrongObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = instanceId(object);
          write(_storage, id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_SET: {
        OMPropertyId id = p->propertyId();
        write(_storage, id, _reorderBytes);
        OMStrongReferenceSet* s = dynamic_cast<OMStrongReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 elementCount = s->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = sizeof(OMUInt32) + sizeof(OMUInt32)
                              + (elementCount * elementSize);
        write(_storage, size, _reorderBytes);
        write(_storage, elementCount, _reorderBytes);
        write(_storage, elementSize, _reorderBytes);
        OMContainerIterator<OMStrongReferenceSetElement>& iterator =
                                                                *s->iterator();
        while (++iterator) {
          OMStrongReferenceSetElement& element = iterator.value();
          OMStrongObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = instanceId(object);
          write(_storage, id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE: {
        OMWeakReference* wr = dynamic_cast<OMWeakReference*>(p);
        ASSERT("Valid type", wr != 0);
        OMWeakObjectReference& r = wr->reference();
        OMStorable* object = r.getValue();
        ASSERT("Valid object", object != 0);
        OMPropertyId pid = p->propertyId();
        referenceSave(object, pid);
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_VECTOR: {
        OMPropertyId id = p->propertyId();
        write(_storage, id, _reorderBytes);
        OMWeakReferenceVector* v = dynamic_cast<OMWeakReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMUInt32 elementCount = v->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = sizeof(OMUInt32) + sizeof(OMUInt32)
                              + (elementCount * elementSize);
        write(_storage, size, _reorderBytes);
        write(_storage, elementCount, _reorderBytes);
        write(_storage, elementSize, _reorderBytes);
        OMContainerIterator<OMWeakReferenceVectorElement>& iterator =
                                                                *v->iterator();
        while (++iterator) {
          OMWeakReferenceVectorElement& element = iterator.value();
          OMWeakObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = instanceId(object);
          write(_storage, id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_SET: {
        OMPropertyId id = p->propertyId();
        write(_storage, id, _reorderBytes);
        OMWeakReferenceSet* s = dynamic_cast<OMWeakReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 elementCount = s->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = sizeof(OMUInt32) + sizeof(OMUInt32)
                              + (elementCount * elementSize);
        write(_storage, size, _reorderBytes);
        write(_storage, elementCount, _reorderBytes);
        write(_storage, elementSize, _reorderBytes);
        OMContainerIterator<OMWeakReferenceSetElement>& iterator =
                                                                *s->iterator();
        while (++iterator) {
          OMWeakReferenceSetElement& element = iterator.value();
          OMWeakObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = instanceId(object);
          write(_storage, id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_DATA_STREAM: {
        OMPropertyId id = p->propertyId();
        OMDataStream* stream = dynamic_cast<OMDataStream*>(p);
        ASSERT("Valid type", stream != 0);
        write(_storage, id, _reorderBytes);
        OMPropertySize s = sizeof(OMByteOrder) +
                           sizeof(OMKLVKey);
        write(_storage, s, _reorderBytes);
        OMByteOrder bo = stream->storedByteOrder();
        write(_storage, bo);
        OMKLVKey sid = streamId(stream);
        writeKLVKey(_storage, sid);
        break;
      }
      default:
        break;
      }
    }
  }
}

void OMKLVStoredObject::deepSave(const OMPropertySet& properties) const
{
  TRACE("OMKLVStoredObject::deepSave");

  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    ASSERT("Property has a definition", p->definition() != 0);
    if (!p->isOptional() || p->isPresent()) {
      switch (p->storedForm()) {
      case SF_STRONG_OBJECT_REFERENCE:
      case SF_STRONG_OBJECT_REFERENCE_VECTOR:
      case SF_STRONG_OBJECT_REFERENCE_SET: {
        p->save();
        break;
      }
      default:
        break;
      }
    }
  }
}

void OMKLVStoredObject::streamSave(const OMPropertySet& properties) const
{
  TRACE("OMKLVStoredObject::streamSave");

  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    ASSERT("Property has a definition", p->definition() != 0);
    if (!p->isOptional() || p->isPresent()) {
      switch (p->storedForm()) {
      case SF_STRONG_OBJECT_REFERENCE: {
        OMStrongReference* sr = dynamic_cast<OMStrongReference*>(p);
        ASSERT("Valid type", sr != 0);
        OMStrongObjectReference& r = sr->reference();
        OMStorable* object = r.getValue();
        ASSERT("Valid object", object != 0);
        streamSave(*object->propertySet());
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_VECTOR: {
        OMStrongReferenceVector* v = dynamic_cast<OMStrongReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
                                                                *v->iterator();
        while (++iterator) {
          OMStrongReferenceVectorElement& element = iterator.value();
          OMStrongObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          streamSave(*object->propertySet());
        }
        delete &iterator;
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_SET: {
        OMStrongReferenceSet* s = dynamic_cast<OMStrongReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMContainerIterator<OMStrongReferenceSetElement>& iterator =
                                                                *s->iterator();
        while (++iterator) {
          OMStrongReferenceSetElement& element = iterator.value();
          OMStrongObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          streamSave(*object->propertySet());
        }
        delete &iterator;
        break;
      }
      case SF_DATA_STREAM: {
        p->save();
        break;
      }
      default:
        break;
      }
    }
  }
}

void OMKLVStoredObject::referenceSave(OMStorable* object,
                                      OMPropertyId pid) const
{
  TRACE("OMKLVStoredObject::referenceSave");
  PRECONDITION("Valid object", object != 0);

  OMUniqueObjectIdentification oid = instanceId(object);
  write(_storage, pid, _reorderBytes);
  OMPropertySize s = sizeof(OMUniqueObjectIdentification);
  write(_storage, s, _reorderBytes);
  write(_storage, oid, _reorderBytes);
}

void OMKLVStoredObject::flatRestore(const OMPropertySet& properties)
{
  TRACE("OMKLVStoredObject::flatRestore");

  OMUInt64 setLength = readKLVLength(_storage);

  referenceRestore(properties.container(), 0x3c0a);
  const OMUInt16 overhead = sizeof(OMPropertyId) + sizeof(OMPropertySize);
  setLength = setLength - (overhead + sizeof(OMUniqueObjectIdentification));

  while (setLength > 0) {
    OMPropertyId pid;
    read(_storage, pid, _reorderBytes);
    OMPropertySize length;
    read(_storage, length, _reorderBytes);

    OMProperty* p = properties.get(pid);
    ASSERT("Valid property", p != 0);

    switch (p->storedForm()) {
    case SF_DATA: {
      // p->restore(length);
      OMSimpleProperty* sp = dynamic_cast<OMSimpleProperty*>(p);
      ASSERT("Correct type", sp != 0);
      restore(*sp, length);
      p->setPresent();
      break;
    }
    case SF_STRONG_OBJECT_REFERENCE: {
      // p->restore(length);
      OMStrongReference* sr = dynamic_cast<OMStrongReference*>(p);
      ASSERT("Valid type", sr != 0);
      restore(*sr, length);
      p->setPresent();
      break;
    }
    case SF_STRONG_OBJECT_REFERENCE_VECTOR: {
      OMUInt32 elementCount;
      OMUInt32 elementSize;
      read(_storage, elementCount, _reorderBytes);
      read(_storage, elementSize, _reorderBytes);
      OMStrongReferenceVector* v = dynamic_cast<OMStrongReferenceVector*>(p);
      if (elementCount > 0) {
        v->grow(elementCount);
        for (OMUInt32 i = 0; i < elementCount; i++) {
          OMUniqueObjectIdentification id;
          read(_storage, id, _reorderBytes);

          char idString[OMObjectIdentificationStringBufferSize];
          toString(id, idString);

          wchar_t* name = convertString(idString);
          OMStrongObjectReference newReference(v, name, false);
          OMStrongReferenceVectorElement element(v, name, i);
          element.reference() = newReference;
          delete [] name;
          v->insert(i, element);
        }
      }
      p->setPresent();
      break;
    }
    case SF_STRONG_OBJECT_REFERENCE_SET: {
      OMUInt32 elementCount;
      OMUInt32 elementSize;
      read(_storage, elementCount, _reorderBytes);
      read(_storage, elementSize, _reorderBytes);
      OMStrongReferenceSet* s = dynamic_cast<OMStrongReferenceSet*>(p);
      OMKeySize keySize = s->keySize();
      OMByte* key = new OMByte[keySize];
      ASSERT("Valid heap pointer", key != 0);
      OMUInt32* keyI = (OMUInt32*)key;
      memset(key, 0, keySize);

      for (OMUInt32 i = 0; i < elementCount; i++) {
        *keyI = i + 1; // Temporary unique key 1..N
        OMUniqueObjectIdentification id;
        read(_storage, id, _reorderBytes);

        char idString[OMObjectIdentificationStringBufferSize];
        toString(id, idString);

        wchar_t* name = convertString(idString);
        OMStrongObjectReference newReference(s, name, false);
        OMStrongReferenceSetElement element(s, name, i, key, keySize);
        element.reference() = newReference;
        delete [] name;
        s->insert(key, element);
      }
      delete [] key;
      p->setPresent();
      break;
    }
    case SF_WEAK_OBJECT_REFERENCE: {
      OMWeakReference* wr = dynamic_cast<OMWeakReference*>(p);
      ASSERT("Valid type", wr != 0);
      restore(*wr, length);
      p->setPresent();
      break;
    }
    case SF_WEAK_OBJECT_REFERENCE_VECTOR: {
      OMUInt32 elementCount;
      OMUInt32 elementSize;
      read(_storage, elementCount, _reorderBytes);
      read(_storage, elementSize, _reorderBytes);
      OMWeakReferenceVector* v = dynamic_cast<OMWeakReferenceVector*>(p);
      if (elementCount > 0) {
        v->grow(elementCount);
        for (OMUInt32 i = 0; i < elementCount; i++) {
          OMUniqueObjectIdentification id;
          read(_storage, id, _reorderBytes);

          OMWeakReferenceVectorElement element(v, id, 0);
          v->insert(i, element);
        }
      }
      p->setPresent();
      break;
    }
    case SF_WEAK_OBJECT_REFERENCE_SET: {
      OMUInt32 elementCount;
      OMUInt32 elementSize;
      read(_storage, elementCount, _reorderBytes);
      read(_storage, elementSize, _reorderBytes);
      OMWeakReferenceSet* s = dynamic_cast<OMWeakReferenceSet*>(p);
      if (elementCount > 0) {
        for (OMUInt32 i = 0; i < elementCount; i++) {
          OMUniqueObjectIdentification id;
          read(_storage, id, _reorderBytes);

          OMWeakReferenceSetElement element(s, id, 0);
          s->insert(&id, element);
        }
      }
      p->setPresent();
      break;
    }
    case SF_DATA_STREAM: {
      // tjb -- NYI
      OMDataStream* stream = dynamic_cast<OMDataStream*>(p);
      ASSERT("Valid type", stream != 0);
      OMByteOrder bo;
      read(_storage, bo);
      stream->setStoredByteOrder(bo);
      OMKLVKey sid;
      readKLVKey(_storage, sid);
      //p->setPresent();
      ASSERT("Stream not present", !streamToStreamId()->contains(stream));
      streamToStreamId()->insert(stream, sid);
      ASSERT("Identifier not present", !streamIdToStream()->contains(sid));
      streamIdToStream()->insert(sid, stream);
      break;
    }
    default:
      break;
    }
    setLength = setLength - (overhead + length);
  }
}

void OMKLVStoredObject::deepRestore(const OMPropertySet& properties)
{
  TRACE("OMKLVStoredObject::deepRestore");

  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    ASSERT("Property has a definition", p->definition() != 0);
    if (!p->isOptional() || p->isPresent()) {
      switch (p->storedForm()) {
      case SF_STRONG_OBJECT_REFERENCE: {
        OMStrongReference* sr = dynamic_cast<OMStrongReference*>(p);
        ASSERT("Valid type", sr != 0);
        OMStrongObjectReference& r = sr->reference();
        OMStorable* object = r.getValue();
        ASSERT("Valid object", object != 0);
        deepRestore(*object->propertySet());
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_SET: {
        OMStrongReferenceSet* s = dynamic_cast<OMStrongReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 count = s->count();
        if (count > 0) {
          OMContainerIterator<OMStrongReferenceSetElement>& iterator =
                                                                *s->iterator();
          OMKeySize keySize = s->keySize();
          OMPropertyId keyPid = s->keyPropertyId();
          if (p->propertyId() == 0x0b03) {
            keyPid = 0x4c01;
          }
          OMByte* key = new OMByte[keySize];
          ASSERT("Valid heap pointer", key != 0);
          OMVector<OMUniqueObjectIdentification> objects;
          OMVector<void*> keys;
          objects.grow(count);
          keys.grow(count);

          while (++iterator) {
            OMStrongReferenceSetElement& element = iterator.value();
            OMStrongObjectReference& r = element.reference();
            const wchar_t* name = r.name();
            OMUniqueObjectIdentification id;
            char* cName = convertWideString(name);
            if (isValidObjectIdentificationString(cName)) {
              fromString(id, cName);
              if (instanceIdToObject()->contains(id)) {
                objects.insert(id);
              }

              void* k = new OMByte[keySize];
              memcpy(k, element.identification(), keySize);
              keys.insert(k);

            }
            delete [] cName;
          }

          OMVectorIterator<void*> kiter(keys, OMBefore);
          while (++kiter) {
            void* k = kiter.value();
            s->remove(k);
            //delete [] k;
          }

          OMUInt32 localKey = 0;
          OMVectorIterator<OMUniqueObjectIdentification> viter(objects,
                                                               OMBefore);
          while (++viter) {
            OMUniqueObjectIdentification id = viter.value();
            OMStorable* obj = object(id);
            ASSERT("Valid object", obj != 0);
            OMProperty* kp = obj->propertySet()->get(keyPid);
            ASSERT("Valid property", kp != 0);
            ASSERT("Consistent sizes", keySize == kp->bitsSize());
            kp->getBits(key, keySize);

            if (!s->contains(key)) {
              char idString[OMObjectIdentificationStringBufferSize];
              toString(id, idString);
              wchar_t* name = convertString(idString);

              OMStrongReferenceSetElement element(s,
                                                  name,
                                                  localKey,
                                                  key,
                                                  keySize);
              element.setValue(key, obj);
              s->insert(key, element);
              delete [] name;
            }
            localKey = localKey + 1;
          }
          delete [] key;
          delete &iterator;

          OMContainerIterator<OMStrongReferenceSetElement>& iter =
                                                                *s->iterator();
          while (++iter) {
            OMStrongReferenceSetElement& element = iter.value();
            OMStrongObjectReference& r = element.reference();
            OMStorable* object = r.getValue();
            ASSERT("Valid object", object != 0);
            deepRestore(*object->propertySet());
          }
          delete &iter;

        }
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE: {
        OMWeakReference* wr = dynamic_cast<OMWeakReference*>(p);
        ASSERT("Valid type", wr != 0);
        OMWeakObjectReference& r = wr->reference();
        OMUniqueObjectIdentification id = r.identification();
        OMStorable* obj = object(id);
#if defined(USETAGTABLE)
        if (obj != 0) {
#endif
        ASSERT("Valid object", obj != 0);
        // get key property from object
        OMPropertyId keyPid = wr->keyPropertyId();
        // build new reference from key and object
        OMProperty* kp = obj->propertySet()->get(keyPid);
        ASSERT("Valid property", kp != 0);
        OMUniqueObjectIdentification k;
        ASSERT("Consistent sizes", kp->bitsSize() == sizeof(k));
        kp->getBits(reinterpret_cast<OMByte*>(&k), sizeof(k));
#if defined(USETAGTABLE)
        OMPropertyTag tag = findTag(wr->targetName());
        OMWeakObjectReference newReference(p, k, tag);
        wr->reference() = newReference;
#else
        wr->reference().setValue(id, obj);
#endif
#if defined(USETAGTABLE)
        } else {
          OMPropertyTag tag = findTag(wr->targetName());
          wr->reference().setTargetTag(tag);
        }
#endif
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_VECTOR: {
        OMStrongReferenceVector* v = dynamic_cast<OMStrongReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
                                                                *v->iterator();
        while (++iterator) {
          OMStrongReferenceVectorElement& element = iterator.value();
          OMStrongObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          deepRestore(*object->propertySet());
        }
        delete &iterator;
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_VECTOR: {
        OMWeakReferenceVector* v = dynamic_cast<OMWeakReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMPropertyId keyPid = v->keyPropertyId();
        OMContainerIterator<OMWeakReferenceVectorElement>& iterator =
                                                                *v->iterator();
        while (++iterator) {
          OMWeakReferenceVectorElement& element = iterator.value();
          OMWeakObjectReference& r = element.reference();
          OMUniqueObjectIdentification id = r.identification();
          OMStorable* obj = object(id);
#if defined(USETAGTABLE)
        if (obj != 0) {
#endif
          ASSERT("Valid object", obj != 0);
          OMProperty* kp = obj->propertySet()->get(keyPid);
          ASSERT("Valid property", kp != 0);
          OMUniqueObjectIdentification k;
          ASSERT("Consistent sizes", kp->bitsSize() == sizeof(k));
          kp->getBits(reinterpret_cast<OMByte*>(&k), sizeof(k));
#if defined(USETAGTABLE)
          OMPropertyTag tag = findTag(v->targetName());
          OMWeakObjectReference newReference(v, k, tag);
          r = newReference;
#else
          r.setValue(k, obj);
#endif
#if defined(USETAGTABLE)
        } else {
          OMPropertyTag tag = findTag(v->targetName());
          r.setTargetTag(tag);
        }
#endif
        }
        delete &iterator;
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_SET: {
        OMWeakReferenceSet* s = dynamic_cast<OMWeakReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 count = s->count();
        if (count > 0) {
          OMContainerIterator<OMWeakReferenceSetElement>& iterator =
                                                                *s->iterator();
          OMPropertyId keyPid = s->keyPropertyId();
          OMUniqueObjectIdentification key;
          OMVector<OMUniqueObjectIdentification> objects;

          objects.grow(count);
          while (++iterator) {
            OMWeakReferenceSetElement& element = iterator.value();
            OMWeakObjectReference& r = element.reference();
            OMUniqueObjectIdentification id = r.identification();
            objects.insert(id);
          }
          s->removeAllObjects();
          OMVectorIterator<OMUniqueObjectIdentification> viter(objects,
                                                               OMBefore);
          while (++viter) {
            OMUniqueObjectIdentification id = viter.value();
            OMStorable* obj = object(id);
#if defined(USETAGTABLE)
            if (obj != 0) {
#endif
            ASSERT("Valid object", obj != 0);
            OMProperty* kp = obj->propertySet()->get(keyPid);
            ASSERT("Valid property", kp != 0);
            ASSERT("Consistent sizes", kp->bitsSize() == sizeof(key));
            kp->getBits(reinterpret_cast<OMByte*>(&key), sizeof(key));

            OMWeakReferenceSetElement element(s, key, 0);
#if defined(USETAGTABLE)
            OMPropertyTag tag = findTag(s->targetName());
            OMWeakObjectReference newReference(s, key, tag);
            element.reference() = newReference;
#else
            element.setValue(key, obj);
#endif
            s->insert(&key, element);
#if defined(USETAGTABLE)
            } else {
              OMPropertyTag tag = findTag(s->targetName());
              //element.reference().setTargetTag(tag);
            }
#endif
          }
          delete &iterator;
        }
        break;
      }
      case SF_DATA:
      case SF_DATA_STREAM:
        // tjb -- NYI
        break;
      default:
        break;
      }
    }
  }
}

void OMKLVStoredObject::streamRestore(OMRawStorage* store)
{
  TRACE("OMKLVStoredObject::streamRestore")

  OMKLVKey ClosedFooterPartitionPackKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
     0x0d, 0x01, 0x02, 0x01, 0x01, 0x04, 0x02, 0x00};

  OMKLVKey k;
  readKLVKey(store, k);

  while (memcmp(&k, &ClosedFooterPartitionPackKey, sizeof(OMKLVKey)) != 0) {
    if (streamIdToStream()->contains(k)) {
      OMUInt64 length = readBerLength(store);
      OMDataStream* s = stream(k);
      s->setPosition(0);
      for (OMUInt64 i = 0; i < length; i++) {
        OMByte b;
        OMUInt32 x;
        read(store, b);
        s->write(&b, 1, x);
      }
      s->setPosition(0);
    } else {
      skipLV(store);
    }
    readKLVKey(store, k);
  }
}

void OMKLVStoredObject::referenceRestore(OMStorable* object,
                                         OMPropertyId ANAME(pid))
{
  TRACE("OMKLVStoredObject::referenceRestore");

  OMPropertySize length;
  OMPropertyId rPid;
  read(_storage, rPid, _reorderBytes);
  ASSERT("Property is reference/instance UID", rPid == pid);
  read(_storage, length, _reorderBytes);
  ASSERT("Valid length", length == sizeof(OMUniqueObjectIdentification));
  OMUniqueObjectIdentification iid;
  read(_storage, iid, _reorderBytes);
  ASSERT("Object not present", !objectToInstanceId()->contains(object));
  objectToInstanceId()->insert(object, iid);
  ObjectDirectoryEntry* ep = 0;
  if (instanceIdToObject()->find(iid, &ep)) {
    ASSERT("No previous entry", ep->_object == 0);
    ep->_object = object;
  } else {
    // Root object
    ObjectDirectoryEntry e;
    e._object = object;
    e._offset = 0;
    e._flags = 0;
    ASSERT("Identifier not present", !instanceIdToObject()->contains(iid));
    instanceIdToObject()->insert(iid, e);
  }
  ASSERT("Identifier present", instanceIdToObject()->contains(iid));
}

void OMKLVStoredObject::writeHeaderPartition(void)
{
  OMKLVKey headerPartitionPackKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
     0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x00};

  writePartition(_storage, headerPartitionPackKey, _reorderBytes);
}

void OMKLVStoredObject::writeFooterPartition(OMRawStorage* store)
{
  bool reorderBytes;
  if (hostByteOrder() == bigEndian) {
    reorderBytes = false;
  } else {
    reorderBytes = true;
  }
  OMKLVKey ClosedFooterPartitionPackKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
     0x0d, 0x01, 0x02, 0x01, 0x01, 0x04, 0x02, 0x00};
  writePartition(store, ClosedFooterPartitionPackKey, reorderBytes);
}

OMUInt16 currentMajorVersion = 0xffff;
OMUInt16 currentMinorVersion = 0xfffc;

void OMKLVStoredObject::writePartition(OMRawStorage* store,
                                       const OMKLVKey& key,
                                       bool reorderBytes)
{
  OMKLVKey operationalPattern =
    {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
     0x0d, 0x01, 0x02, 0x01, 0x01, 0x01, 0x09, 0x00};

  OMKLVKey essenceContainers[] = {
    {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01,
     0x0d, 0x01, 0x03, 0x01, 0x02, 0x01, 0x02, 0x01}
  };
  OMUInt32 elementSize = sizeof(OMKLVKey);
  OMUInt32 elementCount = sizeof(essenceContainers) / elementSize;

  writeKLVKey(store, key);
  OMUInt64 sizeOfFixedPortion = 88;
  OMUInt64 length = sizeOfFixedPortion + (elementCount * elementSize);
  writeKLVLength(store, length);
  OMUInt16 majorVersion = currentMajorVersion;
  write(store, majorVersion, reorderBytes);
  OMUInt16 minorVersion = currentMinorVersion;
  write(store, minorVersion, reorderBytes);
  OMUInt32 KAGSize = 0x200;
  write(store, KAGSize, reorderBytes);
  OMUInt64 thisPartition = 0;
  write(store, thisPartition, reorderBytes);
  OMUInt64 previousPartition = 0;
  write(store, previousPartition, reorderBytes);
  OMUInt64 footerPartition = 0;
  write(store, footerPartition, reorderBytes);
  OMUInt64 headerByteCount = 0;
  write(store, headerByteCount, reorderBytes);
  OMUInt64 indexByteCount = 0;
  write(store, indexByteCount, reorderBytes);
  OMUInt32 indexSID = 1;
  write(store, indexSID, reorderBytes);
  OMUInt64 bodyOffset = 0;
  write(store, bodyOffset, reorderBytes);
  OMUInt32 bodySID = 2;
  write(store, bodySID, reorderBytes);
  writeKLVKey(store, operationalPattern);
  write(store, elementCount, reorderBytes);
  write(store, elementSize, reorderBytes);
  for (OMUInt32 i = 0; i < elementCount; i++) {
    writeKLVKey(store, essenceContainers[i]);
  }

  OMUInt64 currentPosition = store->position();
  fill(store, currentPosition, KAGSize);
}

void OMKLVStoredObject::writePrimerPack(OMRawStorage* store,
                                        const OMDictionary* dictionary,
                                        bool reorderBytes)
{
  TRACE("OMKLVStoredObject::writePrimerPack");

  OMKLVKey primerPackKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
     0x0d, 0x01, 0x02, 0x01, 0x01, 0x05, 0x01, 0x00};

  OMUInt32 KAGSize = 0x200;

  OMUInt32 elementCount = 0;
  ClassDefinitionsIterator* classes = dictionary->classDefinitions();
  while (++(*classes)) {
    OMObject* obj = classes->currentObject();
    OMClassDefinition* classDefinition = dynamic_cast<OMClassDefinition*>(obj);
    ASSERT("Object is correct type", classDefinition != 0);
    PropertyDefinitionsIterator*
                           properties = classDefinition->propertyDefinitions();
    while (++(*properties)) {
      OMObject* obj = properties->currentObject();
      OMPropertyDefinition* propertyDefinition =
                                      dynamic_cast<OMPropertyDefinition*>(obj);
      ASSERT("Object is correct type", propertyDefinition != 0);
      OMPropertyId pid = propertyDefinition->localIdentification();
      if (pid >= 0x8000) {
        elementCount = elementCount + 1;
      }
    }
    delete properties;
  }
  delete classes;

  OMUInt32 elementSize = sizeof(OMPropertyId) +
                         sizeof(OMUniqueObjectIdentification);
  writeKLVKey(store, primerPackKey);
  OMUInt64 sizeOfFixedPortion = 8;
  OMUInt64 length = sizeOfFixedPortion + (elementCount * elementSize);
  writeKLVLength(store, length);
  write(store, elementCount, reorderBytes);
  write(store, elementSize, reorderBytes);

  classes = dictionary->classDefinitions();
  while (++(*classes)) {
    OMObject* obj = classes->currentObject();
    OMClassDefinition* classDefinition = dynamic_cast<OMClassDefinition*>(obj);
    ASSERT("Object is correct type", classDefinition != 0);
    PropertyDefinitionsIterator*
                           properties = classDefinition->propertyDefinitions();
    while (++(*properties)) {
      OMObject* obj = properties->currentObject();
      OMPropertyDefinition* propertyDefinition =
                                      dynamic_cast<OMPropertyDefinition*>(obj);
      ASSERT("Object is correct type", propertyDefinition != 0);
      OMPropertyId pid = propertyDefinition->localIdentification();
      if (pid >= 0x8000) {
        write(store, pid, reorderBytes);
        OMUniqueObjectIdentification id =
                                    propertyDefinition->uniqueIdentification();
        OMKLVKey k;
        convert(k, id);
        writeKLVKey(store, k);
      }
    }
    delete properties;
  }
  delete classes;

  OMUInt64 currentPosition = store->position();
  fill(store, currentPosition, KAGSize);
}

void OMKLVStoredObject::fill(OMRawStorage* store,
                             const OMUInt64& currentPosition,
                             const OMUInt32& KAGSize)
{
  TRACE("OMKLVStoredObject::fill");

  OMUInt64 minimumFill = sizeof(OMKLVKey) + sizeof(OMUInt64) + 1;
  OMUInt64 nextPage = (currentPosition / KAGSize) + 1;
  OMUInt64 remainder = (nextPage * KAGSize) - currentPosition;
  if (remainder < minimumFill) {
    remainder = remainder + KAGSize;
  }
  remainder = remainder - minimumFill;
  writeKLVFill(store, remainder);
}

void OMKLVStoredObject::writeKLVKey(OMRawStorage* store, const OMKLVKey& key)
{
  TRACE("OMKLVStoredObject::writeKLVKey");

  OMUInt32 x;
  const OMByte* src = reinterpret_cast<const OMByte*>(&key);
  store->write(src, sizeof(OMKLVKey), x);

  POSTCONDITION("All bytes written", x == sizeof(OMKLVKey));
}

void OMKLVStoredObject::writeKLVLength(OMRawStorage* store,
                                       const OMUInt64& length)
{
  TRACE("OMKLVStoredObject::writeKLVLength");

  OMByte berLength[sizeof(OMUInt64) + 1];
  OMUInt32 berLengthSize;
  berEncode(berLength, sizeof(berLength), berLengthSize, length);
  OMUInt32 x;
  store->write(berLength, berLengthSize, x);

  POSTCONDITION("All bytes written", x == berLengthSize);
}

void OMKLVStoredObject::writeKLVFill(OMRawStorage* store,
                                     const OMUInt64& length)
{
  TRACE("OMKLVStoredObject::writeKLVFill");

  OMKLVKey fillKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
     0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};

  writeKLVKey(store, fillKey);
  writeKLVLength(store, length);
  for (OMUInt64 i = 0; i < length; i++) {
    const OMByte fillPattern[] = "FFFF.FFFC TIM MEC HEHT GEWYRCAN ";
    write(store, fillPattern[i % (sizeof(fillPattern) - 1)]);
  }
}

void OMKLVStoredObject::write(OMRawStorage* store, const OMUInt8& i)
{
  TRACE("OMKLVStoredObject::write");

  OMUInt32 x;
  store->write(&i, sizeof(OMUInt8), x);

  POSTCONDITION("All bytes written", x == sizeof(OMUInt8));
}

void OMKLVStoredObject::write(OMRawStorage* store,
                              const OMUInt16& i,
                              bool reorderBytes)
{
  TRACE("OMKLVStoredObject::write");

  OMUInt16 si = i;
  OMByte* src = reinterpret_cast<OMByte*>(&si);
  if (reorderBytes) {
    OMType::reorderInteger(src, sizeof(OMUInt16));
  }
  OMUInt32 x;
  store->write(src, sizeof(OMUInt16), x);

  POSTCONDITION("All bytes written", x == sizeof(OMUInt16));
}

void OMKLVStoredObject::write(OMRawStorage* store,
                              const OMUInt32& i,
                              bool reorderBytes)
{
  TRACE("OMKLVStoredObject::write");

  OMUInt32 si = i;
  OMByte* src = reinterpret_cast<OMByte*>(&si);
  if (reorderBytes) {
    OMType::reorderInteger(src, sizeof(OMUInt32));
  }
  OMUInt32 x;
  store->write(src, sizeof(OMUInt32), x);

  POSTCONDITION("All bytes written", x == sizeof(OMUInt32));
}

void OMKLVStoredObject::write(OMRawStorage* store,
                              const OMUInt64& i,
                              bool reorderBytes)
{
  TRACE("OMKLVStoredObject::write");

  OMUInt64 si = i;
  OMByte* src = reinterpret_cast<OMByte*>(&si);
  if (reorderBytes) {
    OMInteger64Type::instance()->reorder(src, sizeof(OMUInt64));
  }
  OMUInt32 x;
  store->write(src, sizeof(OMUInt64), x);

  POSTCONDITION("All bytes written", x == sizeof(OMUInt64));
}

void OMKLVStoredObject::write(OMRawStorage* store,
                              const OMUniqueObjectIdentification& id,
                              bool reorderBytes)
{
  TRACE("OMKLVStoredObject::write");

  OMUniqueObjectIdentification sid = id;
  OMByte* src = reinterpret_cast<OMByte*>(&sid);
  if (reorderBytes) {
    OMUniqueObjectIdentificationType::instance()->reorder(
                                         src,
                                         sizeof(OMUniqueObjectIdentification));
  }
  OMUInt32 x;
  store->write(src, sizeof(OMUniqueObjectIdentification), x);

  POSTCONDITION("All bytes written",
                                    x == sizeof(OMUniqueObjectIdentification));
}

void OMKLVStoredObject::write(OMRawStorage* store,
                              const OMByte* buffer,
                              const OMUInt32& bufferSize)
{
  TRACE("OMKLVStoredObject::write");

  OMUInt32 x;
  store->write(buffer, bufferSize, x);

  POSTCONDITION("All bytes written", x == bufferSize);
}

size_t OMKLVStoredObject::berEncodedSize(const OMUInt64 i)
{
  TRACE("OMKLVStoredObject::berEncodedSize");
  PRECONDITION("Non zero", i != 0);

  size_t result = sizeof(i);
  OMUInt64 v = i;
  while (((v & 0xff00000000000000) >> 56) == 0) {
    v = v << 8;
    result = result - 1;
  }
  return result;
}

void OMKLVStoredObject::berEncode(OMByte* berValue,
                                  size_t ANAME(berValueBufferSize),
                                  OMUInt32& berValueSize,
                                  const OMUInt64& value)
{
  TRACE("OMKLVStoredObject::berEncode");
  PRECONDITION("Valid output buffer", berValue != 0);
  PRECONDITION("Valid output buffer size",
                                   berValueBufferSize >= sizeof(OMUInt64) + 1);

  OMByte* p = berValue;
  OMByte b = 0x80 | sizeof(OMUInt64);
  *p++ = b;
  OMUInt64 v = value;
  for (size_t i = 0; i < sizeof(OMUInt64); i++) {
    b = (OMByte)((v & 0xff00000000000000) >> 56);
    *p++ = b;
    v = v << 8;
  }
  berValueSize = sizeof(OMUInt64) + 1;
}

OMUInt64 OMKLVStoredObject::readBerLength(OMRawStorage* store)
{
  TRACE("OMKLVStoredObject::readBerLength");

  OMUInt64 result;
  OMUInt8 b;
  read(store, b);
  if (b == 0x80) {
    // unknown length
    result = 0;
  } else if ((b & 0x80) != 0x80) {
    // short form
    result = b;
  } else {
    // long form
    int length = b & 0x7f;
    result = 0;
    for (int k = 0; k < length; k++) {
      read(store, b);
      result = result << 8;
      result = result + b;
    }
  }
  return result;
}

bool OMKLVStoredObject::readHeaderPartition(OMRawStorage* store)
{
  TRACE("OMKLVStoredObject::readHeaderPartition");

  bool reorderBytes;
  if (hostByteOrder() == bigEndian) {
    reorderBytes = false;
  } else {
    reorderBytes = true;
  }
  OMKLVKey headerPartitionPackKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
     0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x00, 0x00};
  OMKLVKey k;
  bool result = true;
  readKLVKey(store, k);
  k.octet14 = 0x00;
  if (memcmp(&k, &headerPartitionPackKey, sizeof(OMKLVKey)) == 0) {
    readKLVLength(store);
    OMUInt16 majorVersion;
    read(store, majorVersion, reorderBytes);
    if (majorVersion != currentMajorVersion) {
      result = false;
    }
    OMUInt16 minorVersion;
    read(store, minorVersion, reorderBytes);
    if (minorVersion != currentMinorVersion) {
      result = false;
    }
    OMUInt32 KAGSize;
    read(store, KAGSize, reorderBytes);
    OMUInt64 thisPartition;
    read(store, thisPartition, reorderBytes);
    OMUInt64 previousPartition;
    read(store, previousPartition, reorderBytes);
    OMUInt64 footerPartition;
    read(store, footerPartition, reorderBytes);
    OMUInt64 headerByteCount;
    read(store, headerByteCount, reorderBytes);
    OMUInt64 indexByteCount;
    read(store, indexByteCount, reorderBytes);
    OMUInt32 indexSID;
    read(store, indexSID, reorderBytes);
    OMUInt64 bodyOffset;
    read(store, bodyOffset, reorderBytes);
    OMUInt32 bodySID;
    read(store, bodySID, reorderBytes);
    OMKLVKey operationalPattern;
    readKLVKey(store, operationalPattern);
    OMUInt32 elementCount;
    read(store, elementCount, reorderBytes);
    OMUInt32 elementSize;
    read(store, elementSize, reorderBytes);
    OMKLVKey essenceContainer;
    for (OMUInt32 i = 0; i < elementCount; i++) {
      readKLVKey(store, essenceContainer);
    }
  } else {
    result = false;
  }
  return result;
}

void OMKLVStoredObject::readPrimerPack(OMRawStorage* store,
                                       OMDictionary* /* dictionary */,
                                       bool reorderBytes)
{
  TRACE("OMKLVStoredObject::readPrimerPack");

  OMKLVKey primerPackKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
     0x0d, 0x01, 0x02, 0x01, 0x01, 0x05, 0x01, 0x00};

  OMKLVKey k;
  readKLVKey(store, k);
  ASSERT("Primer key", memcmp(&primerPackKey, &k, sizeof(OMKLVKey)) == 0);
  readKLVLength(store);
  OMUInt32 elementCount;
  read(store, elementCount, reorderBytes);
  OMUInt32 elementSize;
  read(store, elementSize, reorderBytes);
  ASSERT("Valid element size",
                       elementSize == sizeof(OMKLVKey) + sizeof(OMPropertyId));
  for (OMUInt32 i = 0; i < elementCount; i++) {
    OMPropertyId pid;
    read(store, pid, reorderBytes);
    OMKLVKey x;
    readKLVKey(store, x);
    OMUniqueObjectIdentification id;
    convert(id, x);
  }
}

void OMKLVStoredObject::readKLVKey(OMRawStorage* store, OMKLVKey& key)
{
  TRACE("OMKLVStoredObject::readKLVKey");

  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&key);
  store->read(dest, sizeof(OMKLVKey), x);

  POSTCONDITION("All bytes read", x == sizeof(OMKLVKey));
}

OMUInt64 OMKLVStoredObject::readKLVLength(OMRawStorage* store)
{
  TRACE("OMKLVStoredObject::readKLVLength");
  OMUInt64 result = readBerLength(store);
  return result;
}

void OMKLVStoredObject::readKLVFill(OMRawStorage* store)
{
  TRACE("OMKLVStoredObject::readKLVFill");
  OMKLVKey fillKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
     0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};

  OMKLVKey k;
  readKLVKey(store, k);
  ASSERT("Fill key", memcmp(&fillKey, &k, sizeof(OMKLVKey)) == 0);
  OMUInt64 length = readKLVLength(store);
  for (OMUInt64 i = 0; i < length; i++) {
    OMByte b;
    read(store, b);
  }
}

void OMKLVStoredObject::skipLV(OMRawStorage* store)
{
  TRACE("OMKLVStoredObject::skipLV");

  OMUInt64 length = readKLVLength(store);
  OMUInt64 position = store->position();
  OMUInt64 newPosition = position + length;
  store->setPosition(newPosition);
}

void OMKLVStoredObject::read(OMRawStorage* store, OMUInt8& i)
{
  TRACE("OMKLVStoredObject::read");
  OMUInt8 result;
  OMUInt32 x;
  store->read(&result, sizeof(OMUInt8), x);
  ASSERT("All bytes read", x == sizeof(OMUInt8));
  i = result;
}

void OMKLVStoredObject::read(OMRawStorage* store,
                             OMUInt16& i,
                             bool reorderBytes)
{
  TRACE("OMKLVStoredObject::read");
  OMUInt16 result;
  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&result);
  store->read(dest, sizeof(OMUInt16), x);
  ASSERT("All bytes read", x == sizeof(OMUInt16));
  if (reorderBytes) {
    OMInteger16Type::instance()->reorder(dest, sizeof(OMUInt16));
  }
  i = result;
}

void OMKLVStoredObject::read(OMRawStorage* store,
                             OMUInt32& i,
                             bool reorderBytes)
{
  TRACE("OMKLVStoredObject::read");
  OMUInt32 result;
  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&result);
  store->read(dest, sizeof(OMUInt32), x);
  ASSERT("All bytes read", x == sizeof(OMUInt32));
  if (reorderBytes) {
    OMInteger32Type::instance()->reorder(dest, sizeof(OMUInt32));
  }
  i = result;
}

void OMKLVStoredObject::read(OMRawStorage* store,
                             OMUInt64& i,
                             bool reorderBytes)
{
  TRACE("OMKLVStoredObject::read");
  OMUInt64 result;
  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&result);
  store->read(dest, sizeof(OMUInt64), x);
  ASSERT("All bytes read", x == sizeof(OMUInt64));
  if (reorderBytes) {
    OMInteger64Type::instance()->reorder(dest, sizeof(OMUInt64));
  }
  i = result;
}

void OMKLVStoredObject::read(OMRawStorage* store,
                             OMUniqueObjectIdentification& id,
                             bool reorderBytes)
{
  TRACE("OMKLVStoredObject::read");
  OMUniqueObjectIdentification result;
  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&result);
  store->read(dest, sizeof(OMUniqueObjectIdentification), x);
  ASSERT("All bytes read", x == sizeof(OMUniqueObjectIdentification));
  if (reorderBytes) {
    OMUniqueObjectIdentificationType::instance()->reorder(
                                         dest,
                                         sizeof(OMUniqueObjectIdentification));
  }
  id = result;
}

void OMKLVStoredObject::read(OMRawStorage* store,
                             OMByte* buffer,
                             const OMUInt32& bufferSize)
{
  TRACE("OMKLVStoredObject::read");
  OMUInt32 x;
  store->read(buffer, bufferSize, x);
  ASSERT("All bytes read", x == bufferSize);
}

OMUniqueObjectIdentification
OMKLVStoredObject::instanceId(OMStorable* object)
{
  TRACE("OMKLVStoredObject::instanceId");
  PRECONDITION("Valid object", object != 0);

  OMUniqueObjectIdentification result;
  if (!objectToInstanceId()->find(object, result)) {
#if defined(INSTANCEID_DEBUG)
    static OMUInt32 seed = 0;
    memset(&result, 0, sizeof(result));
    result.Data1 = ++seed;
#else
    result = createUniqueIdentifier();
#endif
    objectToInstanceId()->insert(object, result);
  }
  return result;
}

OMStorable*
OMKLVStoredObject::object(const OMUniqueObjectIdentification& instanceId)
{
  TRACE("OMKLVStoredObject::object");

  OMStorable* result;
  ObjectDirectoryEntry e;
  if (!instanceIdToObject()->find(instanceId, e)) {
    result = 0;
  } else {
    result = e._object;
  }
  return result;
}

OMSet<OMStorable*, OMUniqueObjectIdentification>*
OMKLVStoredObject::_objectToInstanceId = 0;

OMSet<OMUniqueObjectIdentification, OMKLVStoredObject::ObjectDirectoryEntry>*
OMKLVStoredObject::_instanceIdToObject = 0;

OMSet<OMStorable*, OMUniqueObjectIdentification>*
OMKLVStoredObject::objectToInstanceId(void)
{
  TRACE("OMKLVStoredObject::objectToInstanceId");

  if (_objectToInstanceId == 0) {
    _objectToInstanceId = new OMSet<OMStorable*,
                                    OMUniqueObjectIdentification>();
    ASSERT("Valid heap pointer", _objectToInstanceId != 0);
  }
  return _objectToInstanceId;
}

OMSet<OMUniqueObjectIdentification, OMKLVStoredObject::ObjectDirectoryEntry>*
OMKLVStoredObject::instanceIdToObject(void)
{
  TRACE("OMKLVStoredObject::instanceIdToObject");

  if (_instanceIdToObject == 0) {
    _instanceIdToObject = new OMSet<OMUniqueObjectIdentification,
                                    OMKLVStoredObject::ObjectDirectoryEntry>();
    ASSERT("Valid heap pointer", _instanceIdToObject != 0);
  }
  return _instanceIdToObject;
}

OMSet<OMDataStream*, OMKLVKey>* OMKLVStoredObject::_streamToStreamId = 0;

OMSet<OMKLVKey, OMDataStream*>* OMKLVStoredObject::_streamIdToStream = 0;

OMSet<OMDataStream*, OMKLVKey>* OMKLVStoredObject::streamToStreamId(void)
{
  TRACE("OMKLVStoredObject::streamToStreamId");

  if (_streamToStreamId == 0) {
    _streamToStreamId = new OMSet<OMDataStream*, OMKLVKey>();
    ASSERT("Valid heap pointer", _streamToStreamId != 0);
  }
  return _streamToStreamId;
}

OMSet<OMKLVKey, OMDataStream*>* OMKLVStoredObject::streamIdToStream(void)
{
  TRACE("streamIdToStream");

  if (_streamIdToStream == 0) {
    _streamIdToStream = new OMSet<OMKLVKey, OMDataStream*>();
    ASSERT("Valid heap pointer", _streamIdToStream != 0);
  }
  return _streamIdToStream;
}

OMKLVKey OMKLVStoredObject::streamId(OMDataStream* stream)
{
  TRACE("OMKLVStoredObject::streamId");
  PRECONDITION("Valid stream", stream != 0);

  OMKLVKey result;
  if (!streamToStreamId()->find(stream, result)) {

    OMKLVKey e =
      {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01,
       0x0d, 0x01, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff};

    static OMUInt8 seed = 0;
    e.octet12 = 0x17;   // Item type = GC Data
    e.octet13 = 0x01;   // Essence element count
    e.octet14 = 0x01;   // Essence element type = Unknown data
    e.octet15 = ++seed; // Essence element number
    memcpy(&result, &e, sizeof(result));
    streamToStreamId()->insert(stream, result);
  }
  return result;
}

OMDataStream* OMKLVStoredObject::stream(const OMKLVKey& streamId)
{
  TRACE("OMKLVStoredObject::stream");

  OMDataStream* result;
  if (!streamIdToStream()->find(streamId, result)) {
    result = 0;
  }

  return result;
}

OMUInt64 OMKLVStoredObject::save(OMSet<OMUniqueObjectIdentification,
                                       ObjectDirectoryEntry>* objectTable)
{
  TRACE("OMKLVStoredObject::save");

  OMUInt64 result = _storage->position();

  // {F10296F0-56E0-4d2a-9613-B38A87348746}
  OMUniqueObjectIdentification id =
  { 0xf10296f0, 0x56e0, 0x4d2a,
  { 0x96, 0x13, 0xb3, 0x8a, 0x87, 0x34, 0x87, 0x46 } };
  OMKLVKey k;
  convert(k, id);
  writeKLVKey(_storage, k);
  OMUInt64 entries = objectTable->count();
  const OMUInt8 entrySize = sizeof(OMUniqueObjectIdentification) + // iid
                            sizeof(OMUInt64) +                     // offset
                            sizeof(OMUInt8);                       // flags
  OMUInt64 length = sizeof(OMUInt64) +                     // entry count
                    sizeof(OMUInt8) +                      // entry size
                    (entries * entrySize);                 // entries
  writeKLVLength(_storage, length);

  write(_storage, entries, _reorderBytes);
  write(_storage, entrySize);

  OMSetIterator<OMUniqueObjectIdentification, ObjectDirectoryEntry>
                                      iterator(*_instanceIdToObject, OMBefore);
  while (++iterator) {
    OMUniqueObjectIdentification id = iterator.key();
    ObjectDirectoryEntry e = iterator.value();
    write(_storage, id, _reorderBytes);
    write(_storage, e._offset, _reorderBytes);
    write(_storage, e._flags);
  }
  return result;
}

void OMKLVStoredObject::restore(OMSet<OMUniqueObjectIdentification,
                                      ObjectDirectoryEntry>* objectTable)
{
  TRACE("restore");
  PRECONDITION("Valid metadata directory", objectTable != 0);
  PRECONDITION("Valid metadata directory offset", _objectDirectory != 0);

  OMUInt64 savedPosition = _storage->position();
  _storage->setPosition(_objectDirectory);

  // {F10296F0-56E0-4d2a-9613-B38A87348746}
  OMUniqueObjectIdentification id =
  { 0xf10296f0, 0x56e0, 0x4d2a,
  { 0x96, 0x13, 0xb3, 0x8a, 0x87, 0x34, 0x87, 0x46 } };
  OMKLVKey objectDirectoryKey;
  convert(objectDirectoryKey, id);
  OMKLVKey k;
  readKLVKey(_storage, k);
  ASSERT("Expected key", k == objectDirectoryKey); // tjb - error
  OMUInt64 setLength = readKLVLength(_storage);
  OMUInt64 entries;
  OMUInt8 entrySize;
  ASSERT("Valid length", setLength > sizeof(entries) + sizeof(entrySize));

  read(_storage, entries, _reorderBytes);
  read(_storage, entrySize);
  ASSERT("Valid entry size",
                           entrySize == (sizeof(OMUniqueObjectIdentification) +
                                         sizeof(OMUInt64) +
                                         sizeof(OMUInt8)));
  ASSERT("Consistent length and entry count",
             setLength == sizeof(entries) +
                          sizeof(entrySize) + (entries * entrySize));

  for (OMUInt64 i = 0; i < entries; i++) {
    OMUniqueObjectIdentification id;
    ObjectDirectoryEntry e;

    read(_storage, id, _reorderBytes);
    e._object = 0;
    read(_storage, e._offset, _reorderBytes);
    read(_storage, e._flags);

    objectTable->insert(id, e);
  }

  _storage->setPosition(savedPosition);
}

OMUInt64 OMKLVStoredObject::saveObjectDirectoryReference(
                                        const OMUniqueObjectIdentification& id)
{
  OMPropertySize size = sizeof(OMUniqueObjectIdentification) +
                        sizeof(OMUInt64);
  // pid
  OMPropertyId pid = 0x0003;
  write(_storage, pid, _reorderBytes);
  // size
  write(_storage, size, _reorderBytes);
  // id
  write(_storage, id, _reorderBytes);
  // offset (not yet known, will patch later)
  OMUInt64 patch = _storage->position();
  OMUInt64 offset = 0;
  write(_storage, offset, _reorderBytes);
  return patch;
}

void
OMKLVStoredObject::fixupObjectDirectoryReference(OMUInt64 patchOffset,
                                                 OMUInt64 patchValue)
{
  TRACE("fixupObjectDirectoryReference");
  PRECONDITION("Valid patch offset", patchOffset != 0);
  PRECONDITION("Valid patch value", patchValue!= 0);

  OMUInt64 savedPosition = _storage->position();
  _storage->setPosition(patchOffset);
  write(_storage, patchValue, _reorderBytes);
  _storage->setPosition(savedPosition);
}

OMUInt64
OMKLVStoredObject::restoreObjectDirectoryReference(
                                              OMUniqueObjectIdentification& id)
{
  TRACE("OMKLVStoredObject::restoreObjectDirectoryReference");

  // pid
  OMPropertyId pid;
  read(_storage, pid, _reorderBytes);
  ASSERT("Expected pid", pid == 0x0003);
  // size
  OMPropertySize size;
  read(_storage, size, _reorderBytes);
  ASSERT("Expected size",
            size == (sizeof(OMUniqueObjectIdentification) + sizeof(OMUInt64)));
  // id
  read(_storage, id, _reorderBytes);
  // offset
  OMUInt64 offset;
  read(_storage, offset, _reorderBytes);
  return offset;
}

  // @mfunc Convert the <c OMUniqueObjectIdentification> id
  //        into the <c OMKLVKey> key.
  //   @parm The resulting <c OMKLVKey>.
  //   @parm The input <c OMUniqueObjectIdentification>.
void OMKLVStoredObject::convert(OMKLVKey& key,
                                const OMUniqueObjectIdentification& id)
{
  TRACE("OMKLVStoredObject::convert");

  // Bottom half of key <- top half of id
  //
  key.octet0  = id.Data4[0];
  key.octet1  = id.Data4[1];
  key.octet2  = id.Data4[2];
  key.octet3  = id.Data4[3];
  key.octet4  = id.Data4[4];
  key.octet5  = id.Data4[5];
  key.octet6  = id.Data4[6];
  key.octet7  = id.Data4[7];

  // Top half of key <- bottom half of id
  //
  key.octet8  = (OMByte)((id.Data1 & 0xff000000) >> 24);
  key.octet9  = (OMByte)((id.Data1 & 0x00ff0000) >> 16);
  key.octet10 = (OMByte)((id.Data1 & 0x0000ff00) >>  8);
  key.octet11 = (OMByte)((id.Data1 & 0x000000ff));

  key.octet12 = (OMByte)((id.Data2 & 0xff00) >> 8);
  key.octet13 = (OMByte)((id.Data2 & 0x00ff));

  key.octet14 = (OMByte)((id.Data3 & 0xff00) >> 8);
  key.octet15 = (OMByte)((id.Data3 & 0x00ff));

  // If id is an AAF class AUID, map it to a SMPTE 336M local set key
  //
#if defined(MAP_CLASSIDS)
  OMByte classIdPrefix[] = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06};
  if (memcmp(&key, &classIdPrefix, sizeof(classIdPrefix)) == 0) {
    key.octet5  = 0x53;
  }
#endif
}

  // @mfunc Convert the <c OMKLVKey> key
  //        into the <c OMUniqueObjectIdentification> id.
  //   @parm The resulting <c OMUniqueObjectIdentification>.
  //   @parm The input <c OMKLVKey>.
void OMKLVStoredObject::convert(OMUniqueObjectIdentification& id,
                                const OMKLVKey& key)
{
  TRACE("OMKLVStoredObject::convert");

  // Bottom half of id <- top half of key
  //
  id.Data4[0] = key.octet0;
  id.Data4[1] = key.octet1;
  id.Data4[2] = key.octet2;
  id.Data4[3] = key.octet3;
  id.Data4[4] = key.octet4;
  id.Data4[5] = key.octet5;
  id.Data4[6] = key.octet6;
  id.Data4[7] = key.octet7;

  // Top half of id <- bottom half of key
  //
  OMUInt32 d1 = 0;
  d1 = d1 + (key.octet8  << 24);
  d1 = d1 + (key.octet9  << 16);
  d1 = d1 + (key.octet10 <<  8);
  d1 = d1 + (key.octet11);
  id.Data1 = d1;

  OMUInt16 d2 = 0;
  d2 = d2 + (key.octet12 << 8);
  d2 = d2 + (key.octet13);
  id.Data2 = d2;

  OMUInt16 d3 = 0;
  d3 = d3 + (key.octet14 << 8);
  d3 = d3 + (key.octet15);
  id.Data3 = d3;

  // If key is a SMPTE 336M local set key, map it to an AAF class AUID
  //
#if defined(MAP_CLASSIDS)
  OMByte localSetPrefix[] = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x53};
  if (memcmp(&key, &localSetPrefix, sizeof(localSetPrefix)) == 0) {
    id.Data4[5] = 0x06;
  }
#endif
}

void OMKLVStoredObject::finalize(void)
{
  TRACE("OMKLVStoredObject::finalize");
#if defined(INSTANCEID_DEBUG)

  if (_instanceIdToObject != 0) {

    OMUInt32 n = _instanceIdToObject->count();
    cout << dec << n << " objects" << endl;

    OMSetIterator<OMUniqueObjectIdentification, OMStorable*>
                                      iterator(*_instanceIdToObject, OMBefore);
    while (++iterator) {
      OMStorable* object = iterator.value();
      OMUniqueObjectIdentification id = iterator.key();
      char buffer[OMObjectIdentificationStringBufferSize];
      toString(id, buffer);
      cout << buffer << " -> " << object << endl;
    }
  }
#endif
  if (_objectToInstanceId != 0) {
    _objectToInstanceId->clear();
    delete _objectToInstanceId;
    _objectToInstanceId = 0;
  }

  if (_instanceIdToObject != 0) {
    _instanceIdToObject->clear();
    delete _instanceIdToObject;
    _instanceIdToObject = 0;
  }

  if (_streamToStreamId != 0) {
    _streamToStreamId->clear();
    delete _streamToStreamId;
    _streamToStreamId = 0;
  }

  if (_streamIdToStream != 0) {
    _streamIdToStream->clear();
    delete _streamIdToStream;
    _streamIdToStream = 0;
  }
}

bool OMKLVStoredObject::metaDataOnly = true;

  // @mfunc Constructor.
  //   @parm The <c OMRawStorage> on which this <c OMKLVStoredObject> resides.
  //   @parm TBS
OMKLVStoredObject::OMKLVStoredObject(OMRawStorage* s, OMByteOrder byteOrder)
: _storage(s),
  _byteOrder(byteOrder),
  _reorderBytes(false),
  _objectDirectory(0),
  _objectDirectoryReference(0)
{
  TRACE("OMKLVStoredObject::OMKLVStoredObject");

  if (hostByteOrder() == bigEndian) {
    _reorderBytes = false;
  } else {
    _reorderBytes = true;
  }
}

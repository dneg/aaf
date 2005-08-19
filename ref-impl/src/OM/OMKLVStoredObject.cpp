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

#include "OMMXFStorage.h"
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
#include "OMDataStreamProperty.h"

#include "OMUtilities.h"

#include "OMRawStorage.h"

#define USETAGTABLE 1
//#define INSTANCEID_DEBUG 1

  // @mfunc Open the root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMKLVStoredObject* OMKLVStoredObject::openRead(OMMXFStorage* rawStorage)
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
                                               OMMXFStorage* ANAME(rawStorage))
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
OMKLVStoredObject* OMKLVStoredObject::createWrite(OMMXFStorage* rawStorage,
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
OMKLVStoredObject* OMKLVStoredObject::createModify(OMMXFStorage* rawStorage,
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

  // @mfunc Does <p rawStorage> contain a recognized file ?
  //        If so, the result is true.
  //   @parm The raw storage.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMKLVStoredObject::isRecognized(OMRawStorage* rawStorage)
{
  TRACE("OMKLVStoredObject::isRecognized");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Positionable raw storage", rawStorage->isPositionable());

  bool reorderBytes;
  if (hostByteOrder() == bigEndian) {
    reorderBytes = false;
  } else {
    reorderBytes = true;
  }
  bool result = true;
  OMKLVKey k;
  if (OMMXFStorage::read(rawStorage, k)) {
    k.octet14 = 0x02;
    if (k == ClosedHeaderPartitionPackKey) {
      OMUInt64 length;
      if (OMMXFStorage::readKLVLength(rawStorage, length)) {
        OMUInt16 majorVersion;
        if (OMMXFStorage::read(rawStorage, majorVersion, reorderBytes)) {
          if (majorVersion == currentMajorVersion) {
            OMUInt16 minorVersion;
            if (OMMXFStorage::read(rawStorage, minorVersion, reorderBytes)) {
              if (minorVersion == currentMinorVersion) {
                result = true;
              } else {
                result = false;  // Minor version number doesn't match
              }
            } else {
              result = false; // Couldn't read minor version
            }
          } else {
            result = false; // Major version number doesn't match
          }
        } else {
          result = false; // Couldn't read major version
        }
      } else {
        result = false; // Couldn't read length
      }
    } else {
      result = false; //  Not HeaderPartitionPack key
    }
  } else {
    result = false; // Couldn't read key
  }
  rawStorage->setPosition(0);
  return result;
}

  // @mfunc Does <p file> have an <c OMMXFStorage>.
  //   @parm The <c OMFile>
  //   @rdesc <e bool.true> if the given <c OMFile> has an
  //          <c OMMXFStorage>, <e bool.false> otherwise.
bool OMKLVStoredObject::hasMxfStorage(const OMFile* file)
{
  TRACE("OMKLVStoredObject::hasMxfStorage");
  PRECONDITION("Valid file", file != 0);

  bool result = false;
  OMKLVStoredObject* r = root(file);
  if (r != 0) {
    result = true;
  }
  return result;
}

  // @mfunc The <c OMMXFStorage> associated with <p file>.
  //   @parm The <c OMFile>
  //   @rdesc The <c OMMXFStorage>
OMMXFStorage* OMKLVStoredObject::mxfStorage(const OMFile* file)
{
  TRACE("OMKLVStoredObject::mxfStorage");
  PRECONDITION("Valid file", file != 0);
  PRECONDITION("File has MXF storage", hasMxfStorage(file));

  OMKLVStoredObject* r = root(file);
  OMMXFStorage* result = r->_storage;

  return result;
}

  // @mfunc The root storage of <p file>.
  //   @parm The <c OMFile>
  //   @rdesc The root <c OMKLVStoredObject> if <p file> is KLV encoded,
  //          0 otherwise.
OMKLVStoredObject* OMKLVStoredObject::root(const OMFile* file)
{
  TRACE("OMKLVStoredObject::root");
  OMKLVStoredObject* result = 0;
  OMStoredObject* r = const_cast<OMFile*>(file)->rootStore();
  if (r != 0) {
    result = dynamic_cast<OMKLVStoredObject*>(r);
  }
  return result;
}

  // @mfunc Does <p stream> represent MXF essence ?
  //   @parm The <c OMDataStreamProperty>
  //   @rdesc <e bool.true> if the given <c OMDataStreamProperty> has an
  //          <c OMMXFStorage>, <e bool.false> otherwise.
bool OMKLVStoredObject::isMxfEssence(const OMDataStreamProperty* stream)
{
  TRACE("OMKLVStoredObject::isMxfEssence");
  PRECONDITION("Valid stream", stream != 0);
  bool result = false;
  OMKLVStoredStream* s = mxfStream(stream);
  if (s != 0) {
    result = true;
  }
  return result;
}

  // @mfunc The <c OMKLVStoredStream> associated with <p stream>.
  //   @parm The <c OMDataStreamProperty>
  //   @rdesc The <c OMKLVStoredStream>
OMKLVStoredStream*
OMKLVStoredObject::mxfEssence(const OMDataStreamProperty* stream)
{
  TRACE("OMKLVStoredObject::mxfEssence");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("MXF essence", isMxfEssence(stream));
  OMKLVStoredStream* result = mxfStream(stream);
  return result;
}

  // @mfunc The <c OMKLVStoredStream> associated with <p stream>
  //   @parm The <c OMDataStreamProperty>
  //   @rdesc The <c OMKLVStoredStream> if <p stream> is KLV encoded,
  //          0 otherwise.
OMKLVStoredStream*
OMKLVStoredObject::mxfStream(const OMDataStreamProperty* stream)
{
  TRACE("OMKLVStoredObject::mxfStream");
  PRECONDITION("Valid stream", stream != 0);
  OMStoredStream* s = stream->stream();
  OMKLVStoredStream* result = dynamic_cast<OMKLVStoredStream*>(s);
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

  if (metaDataOnly) {
    // The header partition has already been written,
    // start saving immediately after the header partition
    // and any fill.
    //
    _storage->readHeaderPartition();
    _storage->readKLVFill();
  } else {
    // Write header partition and alignment fill.
    //
    _storage->writeHeaderPartition();
  }

  OMUInt64 pos = _storage->position();
  _storage->setPosition(pos);

  // Write the primer
  writePrimerPack(file.dictionary());

  // Save the rest of the file
  file.root()->save();

  // Save the meta object directory
  _objectDirectory = save(instanceIdToObject());

  // Now we know where it lives, fixup the reference
  fixupObjectDirectoryReference(_objectDirectoryReference, _objectDirectory);

  // Insert alignment fill
  OMUInt32 bodyPartitionOffset = 0x20000; // Get this from header ?
  OMUInt32 KAGSize = 0x100;
  OMUInt32 fillAlignment;
  if (metaDataOnly) {
    // fill to next KAG
    fillAlignment = KAGSize;
  } else {
    // fill remainder of pre-allocated space
    fillAlignment = bodyPartitionOffset;
  }
  OMUInt64 currentPosition = _storage->position();
  _storage->fillAlignK(currentPosition, fillAlignment);

  if (!metaDataOnly) {
    _storage->writeBodyPartition();
  }

  if (!metaDataOnly) {
    // Save streams
    //
    streamSave(*file.root()->propertySet());

    // Write the footer
    //
    OMUInt64 position = _storage->size();
    _storage->setPosition(position);
    _storage->writeFooterPartition();
  }
}

void OMKLVStoredObject::save(OMStorable& object)
{
  TRACE("OMKLVStoredObject::save(OMStorable)");

  OMUniqueObjectIdentification iid = instanceId(&object);
  if (!instanceIdToObject()->contains(iid)) {
    // This object has never been saved
    ObjectDirectoryEntry e;
    e._object = &object;
    e._offset = _storage->position();
    e._flags = 0;
    instanceIdToObject()->insert(iid, e);
  }
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
  _storage->writeKLVKey(k);
}

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMKLVStoredObject::save(const OMPropertySet& properties)
{
  TRACE("OMKLVStoredObject::save(OMPropertySet)");

  // Length
  OMUInt64 setLength = length(properties);
  _storage->writeKLVLength(setLength);

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
  _storage->write(externalBytesSize, _reorderBytes);
  // value
  _storage->write(buffer, externalBytesSize);

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

  _storage->writeKLVKey(id);
  OMUInt64 length = stream.size();
  _storage->writeKLVLength(length);

  // value
  stream.setPosition(0);
  for (OMUInt64 i = 0; i < length; i++) {
    OMByte b;
    OMUInt32 x;
    stream.read(&b, 1, x);
    _storage->write(b);
  }
}

OMRootStorable* OMKLVStoredObject::restore(OMFile& file)
{
  TRACE("OMKLVStoredObject::restore(OMFile)");

#if defined(USETAGTABLE)
  initializeMap(file);
#endif

  // Read the header partition
  _storage->readHeaderPartition();

  _storage->readKLVFill();

  // Read the primer
  readPrimerPack(file.dictionary());

  file.setLoadMode(OMFile::lazyLoad);

  _storage->readKLVFill();

  OMKLVKey fillKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
     0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};
  OMKLVKey objectDirectoryKey =
    {0x96, 0x13, 0xb3, 0x8a, 0x87, 0x34, 0x87, 0x46,
     0xf1, 0x02, 0x96, 0xf0, 0x56, 0xe0, 0x4d, 0x2a};

  // For restoring meta objects
  OMDictionary* metaDictionary = file.dictionary();
  // For restoring objects
  const OMClassFactory* dictionary = file.classFactory();

  // restore the root
  //
  OMKLVKey k;
  _storage->readKLVKey(k);
  OMClassId cid;
  convert(cid, k);
  ASSERT("Root object", cid == OMRootStorable::_rootClassId);
  OMRootStorable* root = new OMRootStorable();
  ASSERT("Valid heap pointer", root != 0);
  root->attach(&file);
  root->setStore(file.rootStore());
  root->setClassFactory(metaDictionary);
  flatRestore(*root->propertySet());

  // restore the meta object directory
  //
  instanceIdToObject()->remove(instanceId(root));
  restore(instanceIdToObject());

  // restore the meta dictionary
  //
  _storage->readKLVKey(k);
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

    _storage->readKLVKey(k);
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
  while (memcmp(&k, &objectDirectoryKey, sizeof(OMKLVKey)) != 0) {
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

    _storage->readKLVKey(k);
  }
  OMProperty* hp = root->propertySet()->get(0x0002);
  OMStrongReference* hsr = dynamic_cast<OMStrongReference*>(hp);
  ASSERT("Valid type", hsr != 0);
  OMStrongObjectReference& hr = hsr->reference();
  OMStorable* ho = hr.getValue();
  ASSERT("Valid object", ho != 0);
  deepRestore(*ho->propertySet());

  _storage->skipLV(); // This V is fill

  if (!metaDataOnly) {
    streamRestore();
  }

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

  _storage->read(buffer, externalSize);

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
  _storage->read(id, _reorderBytes);

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
  _storage->read(id, _reorderBytes);

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

void OMKLVStoredObject::writeProperty(OMPropertyId pid, const OMUInt32& value)
{
  TRACE("OMKLVStoredObject::writeProperty");

  // pid
  _storage->write(pid, _reorderBytes);
  // size
  OMPropertySize size = sizeof(value);
  _storage->write(size, _reorderBytes);
  // value
  _storage->write(value, _reorderBytes);
}

void OMKLVStoredObject::readProperty(const OMPropertyId& ANAME(pid),
                                     OMUInt32& value)
{
  TRACE("OMKLVStoredObject::readProperty");

  // pid
  OMPropertyId p;
  _storage->read(p, _reorderBytes);
  ASSERT("Expected pid", p == pid);
  // size
  OMPropertySize size;
  _storage->read(size, _reorderBytes);
  ASSERT("Expected size", size == sizeof(value));
  // value
  _storage->read(value, _reorderBytes);
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
  // All objects include a hidden instance UID
  length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                  + sizeof(OMUniqueObjectIdentification);

  if (properties.container()->classId() == Class_Root) {
    // Root object includes a hidden reference to the object directory
    length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                    + sizeof(OMUniqueObjectIdentification) + sizeof(OMUInt64);
    // Root object includes a hidden 32-bit version number
    length = length + sizeof(OMPropertyId) + sizeof(OMPropertySize)
                    + sizeof(OMUInt32);
  }

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

  if (properties.container()->classId() == Class_Root) {
    OMKLVStoredObject* This = const_cast<OMKLVStoredObject*>(this);
    OMUniqueObjectIdentification id = {0};
    This->_objectDirectoryReference = This->saveObjectDirectoryReference(id);

    OMPropertyId pid = 0x0004;
    OMUInt32 version = 0x00000004;
    This->writeProperty(pid, version);
  }

  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    ASSERT("Property has a definition", p->definition() != 0);
    if (!p->isOptional() || p->isPresent()) {
      switch (p->storedForm()) {
      case SF_DATA: {
        OMPropertyId id = p->propertyId();
        _storage->write(id, _reorderBytes);
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
        _storage->write(id, _reorderBytes);
        OMStrongReferenceVector* v = dynamic_cast<OMStrongReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMUInt32 elementCount = v->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = sizeof(OMUInt32) + sizeof(OMUInt32)
                              + (elementCount * elementSize);
        _storage->write(size, _reorderBytes);
        _storage->write(elementCount, _reorderBytes);
        _storage->write(elementSize, _reorderBytes);
        OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
                                                                *v->iterator();
        while (++iterator) {
          OMStrongReferenceVectorElement& element = iterator.value();
          OMStrongObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = instanceId(object);
          _storage->write(id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_SET: {
        OMPropertyId id = p->propertyId();
        _storage->write(id, _reorderBytes);
        OMStrongReferenceSet* s = dynamic_cast<OMStrongReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 elementCount = s->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = sizeof(OMUInt32) + sizeof(OMUInt32)
                              + (elementCount * elementSize);
        _storage->write(size, _reorderBytes);
        _storage->write(elementCount, _reorderBytes);
        _storage->write(elementSize, _reorderBytes);
        OMContainerIterator<OMStrongReferenceSetElement>& iterator =
                                                                *s->iterator();
        while (++iterator) {
          OMStrongReferenceSetElement& element = iterator.value();
          OMStrongObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = instanceId(object);
          _storage->write(id, _reorderBytes);
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
        _storage->write(id, _reorderBytes);
        OMWeakReferenceVector* v = dynamic_cast<OMWeakReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMUInt32 elementCount = v->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = sizeof(OMUInt32) + sizeof(OMUInt32)
                              + (elementCount * elementSize);
        _storage->write(size, _reorderBytes);
        _storage->write(elementCount, _reorderBytes);
        _storage->write(elementSize, _reorderBytes);
        OMContainerIterator<OMWeakReferenceVectorElement>& iterator =
                                                                *v->iterator();
        while (++iterator) {
          OMWeakReferenceVectorElement& element = iterator.value();
          OMWeakObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = instanceId(object);
          _storage->write(id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_SET: {
        OMPropertyId id = p->propertyId();
        _storage->write(id, _reorderBytes);
        OMWeakReferenceSet* s = dynamic_cast<OMWeakReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 elementCount = s->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = sizeof(OMUInt32) + sizeof(OMUInt32)
                              + (elementCount * elementSize);
        _storage->write(size, _reorderBytes);
        _storage->write(elementCount, _reorderBytes);
        _storage->write(elementSize, _reorderBytes);
        OMContainerIterator<OMWeakReferenceSetElement>& iterator =
                                                                *s->iterator();
        while (++iterator) {
          OMWeakReferenceSetElement& element = iterator.value();
          OMWeakObjectReference& r = element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = instanceId(object);
          _storage->write(id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_DATA_STREAM: {
        OMPropertyId id = p->propertyId();
        OMDataStream* stream = dynamic_cast<OMDataStream*>(p);
        ASSERT("Valid type", stream != 0);
        _storage->write(id, _reorderBytes);
        OMPropertySize s = sizeof(OMByteOrder) +
                           sizeof(OMKLVKey);
        _storage->write(s, _reorderBytes);
        OMByteOrder bo = stream->storedByteOrder();
        _storage->write(bo);
        OMKLVKey sid = streamId(stream);
        _storage->writeKLVKey(sid);
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
  _storage->write(pid, _reorderBytes);
  OMPropertySize s = sizeof(OMUniqueObjectIdentification);
  _storage->write(s, _reorderBytes);
  _storage->write(oid, _reorderBytes);
}

void OMKLVStoredObject::flatRestore(const OMPropertySet& properties)
{
  TRACE("OMKLVStoredObject::flatRestore");

  OMUInt64 setLength = _storage->readKLVLength();

  referenceRestore(properties.container(), 0x3c0a);
  const OMUInt16 overhead = sizeof(OMPropertyId) + sizeof(OMPropertySize);
  setLength = setLength - (overhead + sizeof(OMUniqueObjectIdentification));

  if (properties.container()->classId() == Class_Root) {
    OMUniqueObjectIdentification id;
    _objectDirectory = restoreObjectDirectoryReference(id);
    setLength = setLength - (overhead + sizeof(OMUniqueObjectIdentification) +
                                        sizeof(OMUInt64));

    OMPropertyId pid = 0x0004;
    OMUInt32 version;
    readProperty(pid, version);

    setLength = setLength - (overhead + sizeof(OMUInt32));
  }

  while (setLength > 0) {
    OMPropertyId pid;
    _storage->read(pid, _reorderBytes);
    OMPropertySize length;
    _storage->read(length, _reorderBytes);

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
      _storage->read(elementCount, _reorderBytes);
      _storage->read(elementSize, _reorderBytes);
      OMStrongReferenceVector* v = dynamic_cast<OMStrongReferenceVector*>(p);
      if (elementCount > 0) {
        v->grow(elementCount);
        for (OMUInt32 i = 0; i < elementCount; i++) {
          OMUniqueObjectIdentification id;
          _storage->read(id, _reorderBytes);

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
      _storage->read(elementCount, _reorderBytes);
      _storage->read(elementSize, _reorderBytes);
      OMStrongReferenceSet* s = dynamic_cast<OMStrongReferenceSet*>(p);
      OMKeySize keySize = s->keySize();
      OMByte* key = new OMByte[keySize];
      ASSERT("Valid heap pointer", key != 0);
      OMUInt32* keyI = (OMUInt32*)key;
      memset(key, 0, keySize);

      for (OMUInt32 i = 0; i < elementCount; i++) {
        *keyI = i + 1; // Temporary unique key 1..N
        OMUniqueObjectIdentification id;
        _storage->read(id, _reorderBytes);

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
      _storage->read(elementCount, _reorderBytes);
      _storage->read(elementSize, _reorderBytes);
      OMWeakReferenceVector* v = dynamic_cast<OMWeakReferenceVector*>(p);
      if (elementCount > 0) {
        v->grow(elementCount);
        for (OMUInt32 i = 0; i < elementCount; i++) {
          OMUniqueObjectIdentification id;
          _storage->read(id, _reorderBytes);

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
      _storage->read(elementCount, _reorderBytes);
      _storage->read(elementSize, _reorderBytes);
      OMWeakReferenceSet* s = dynamic_cast<OMWeakReferenceSet*>(p);
      if (elementCount > 0) {
        for (OMUInt32 i = 0; i < elementCount; i++) {
          OMUniqueObjectIdentification id;
          _storage->read(id, _reorderBytes);

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
      _storage->read(bo);
      stream->setStoredByteOrder(bo);
      OMKLVKey sid;
      _storage->readKLVKey(sid);
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

void OMKLVStoredObject::streamRestore(void)
{
  TRACE("OMKLVStoredObject::streamRestore")

  OMUniqueObjectIdentification sid;
  OMKLVKey k;
  _storage->readKLVKey(k);
  convert(sid, k);

  while (k != ClosedFooterPartitionPackKey) {
    if (_storage->containsStream(sid)) {
      OMUInt64 length = OMMXFStorage::readBerLength(_storage);
      OMDataStream* s = _storage->stream(sid);
      s->setPosition(0);
      for (OMUInt64 i = 0; i < length; i++) {
        OMByte b;
        OMUInt32 x;
        _storage->read(b);
        s->write(&b, 1, x);
      }
      s->setPosition(0);
    } else {
      _storage->skipLV();
    }
    _storage->readKLVKey(k);
    convert(sid, k);
  }
}

void OMKLVStoredObject::referenceRestore(OMStorable* object,
                                         OMPropertyId ANAME(pid))
{
  TRACE("OMKLVStoredObject::referenceRestore");

  OMPropertySize length;
  OMPropertyId rPid;
  _storage->read(rPid, _reorderBytes);
  ASSERT("Property is reference/instance UID", rPid == pid);
  _storage->read(length, _reorderBytes);
  ASSERT("Valid length", length == sizeof(OMUniqueObjectIdentification));
  OMUniqueObjectIdentification iid;
  _storage->read(iid, _reorderBytes);
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

void OMKLVStoredObject::writePrimerPack(const OMDictionary* dictionary)
{
  TRACE("OMKLVStoredObject::writePrimerPack");

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
  _storage->writeKLVKey(primerPackKey);
  OMUInt64 sizeOfFixedPortion = 8;
  OMUInt64 length = sizeOfFixedPortion + (elementCount * elementSize);
  _storage->writeKLVLength(length);
  _storage->write(elementCount, _reorderBytes);
  _storage->write(elementSize, _reorderBytes);

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
        _storage->write(pid, _reorderBytes);
        OMUniqueObjectIdentification id =
                                    propertyDefinition->uniqueIdentification();
        OMKLVKey k;
        convert(k, id);
        _storage->writeKLVKey(k);
      }
    }
    delete properties;
  }
  delete classes;
}

void OMKLVStoredObject::readPrimerPack(OMDictionary* /* dictionary */)
{
  TRACE("OMKLVStoredObject::readPrimerPack");

  OMKLVKey k;
  _storage->readKLVKey(k);
  ASSERT("Primer key", k == primerPackKey);
  _storage->readKLVLength();
  OMUInt32 elementCount;
  _storage->read(elementCount, _reorderBytes);
  OMUInt32 elementSize;
  _storage->read(elementSize, _reorderBytes);
  ASSERT("Valid element size",
                       elementSize == sizeof(OMKLVKey) + sizeof(OMPropertyId));
  for (OMUInt32 i = 0; i < elementCount; i++) {
    OMPropertyId pid;
    _storage->read(pid, _reorderBytes);
    OMKLVKey x;
    _storage->readKLVKey(x);
    OMUniqueObjectIdentification id;
    convert(id, x);
  }
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
  _storage->writeKLVKey(k);
  OMUInt64 entries = objectTable->count();
  const OMUInt8 entrySize = sizeof(OMUniqueObjectIdentification) + // iid
                            sizeof(OMUInt64) +                     // offset
                            sizeof(OMUInt8);                       // flags
  OMUInt64 length = sizeof(OMUInt64) +                     // entry count
                    sizeof(OMUInt8) +                      // entry size
                    (entries * entrySize);                 // entries
  _storage->writeKLVLength(length);

  _storage->write(entries, _reorderBytes);
  _storage->write(entrySize);

  OMSetIterator<OMUniqueObjectIdentification, ObjectDirectoryEntry>
                                      iterator(*_instanceIdToObject, OMBefore);
  while (++iterator) {
    OMUniqueObjectIdentification id = iterator.key();
    ObjectDirectoryEntry e = iterator.value();
    _storage->write(id, _reorderBytes);
    _storage->write(e._offset, _reorderBytes);
    _storage->write(e._flags);
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
  _storage->readKLVKey(k);
  ASSERT("Expected key", k == objectDirectoryKey); // tjb - error
  OMUInt64 setLength = _storage->readKLVLength();
  OMUInt64 entries;
  OMUInt8 entrySize;
  ASSERT("Valid length", setLength > sizeof(entries) + sizeof(entrySize));

  _storage->read(entries, _reorderBytes);
  _storage->read(entrySize);
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

    _storage->read(id, _reorderBytes);
    e._object = 0;
    _storage->read(e._offset, _reorderBytes);
    _storage->read(e._flags);

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
  _storage->write(pid, _reorderBytes);
  // size
  _storage->write(size, _reorderBytes);
  // id
  _storage->write(id, _reorderBytes);
  // offset (not yet known, will patch later)
  OMUInt64 patch = _storage->position();
  OMUInt64 offset = 0;
  _storage->write(offset, _reorderBytes);
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
  _storage->write(patchValue, _reorderBytes);
  _storage->setPosition(savedPosition);
}

OMUInt64
OMKLVStoredObject::restoreObjectDirectoryReference(
                                              OMUniqueObjectIdentification& id)
{
  TRACE("OMKLVStoredObject::restoreObjectDirectoryReference");

  // pid
  OMPropertyId pid;
  _storage->read(pid, _reorderBytes);
  ASSERT("Expected pid", pid == 0x0003);
  // size
  OMPropertySize size;
  _storage->read(size, _reorderBytes);
  ASSERT("Expected size",
            size == (sizeof(OMUniqueObjectIdentification) + sizeof(OMUInt64)));
  // id
  _storage->read(id, _reorderBytes);
  // offset
  OMUInt64 offset;
  _storage->read(offset, _reorderBytes);
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
  OMByte classIdPrefix[] = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x06};
  if (memcmp(&key, &classIdPrefix, sizeof(classIdPrefix)) == 0) {
    key.octet5  = 0x53;
  }
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
  OMByte localSetPrefix[] = {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x53};
  if (memcmp(&key, &localSetPrefix, sizeof(localSetPrefix)) == 0) {
    id.Data4[5] = 0x06;
  }
}

void OMKLVStoredObject::finalize(void)
{
  TRACE("OMKLVStoredObject::finalize");

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
OMKLVStoredObject::OMKLVStoredObject(OMMXFStorage* s, OMByteOrder byteOrder)
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

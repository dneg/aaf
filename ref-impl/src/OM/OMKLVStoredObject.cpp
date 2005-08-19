//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
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
#if 0
#include "OMCachedDiskRawStorage.h"
#else
#include "OMMemoryRawStorage.h"
#endif
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
#include "OMArrayType.h"
#include "OMSetType.h"
#include "OMDataSet.h"
#include "OMDataVector.h"
#include "OMDataContainer.h"
#include "OMDataContainerIterator.h"

#include "OMUtilities.h"

#include "OMRawStorage.h"

#define USETAGTABLE 1
//#define OMONLY 1

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
OMKLVStoredObject* OMKLVStoredObject::openModify(OMMXFStorage* rawStorage)
{
  TRACE("OMKLVStoredObject::openModify");
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());
  OMKLVStoredObject* result = new OMKLVStoredObject(rawStorage, littleEndian);
  ASSERT("Valid heap pointer", result != 0);
  return result;
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
  bool result = false;
  OMUInt64 headerPosition;
  bool foundHeader = OMMXFStorage::findHeader(rawStorage, headerPosition);
  if (foundHeader) {
    rawStorage->setPosition(headerPosition);
    OMKLVKey k;
    if (OMMXFStorage::read(rawStorage, k)) {
      if (OMMXFStorage::isHeader(k)) {
        OMUInt64 length;
        if (OMMXFStorage::readKLVLength(rawStorage, length)) {
          OMUInt16 majorVersion;
          if (OMMXFStorage::read(rawStorage, majorVersion, reorderBytes)) {
            if (majorVersion == currentMajorVersion) {
              OMUInt16 minorVersion;
              if (OMMXFStorage::read(rawStorage, minorVersion, reorderBytes)) {
                if (minorVersion == currentMinorVersion) {
                  result = true;
                }
              }
            }
          }
        }
      }
    }
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

  _storage->open();

  OMUInt64 pos = _storage->primerOffset();
  _storage->setPosition(pos);
  // Write the primer
  writePrimerPack(file.dictionary());
  _storage->fillAlignK(_storage->position(), defaultKAGSize);

  // Save the rest of the file
  file.root()->save();

  // Save the meta object directory
  _storage->saveObjectDirectory();

  // Save streams
  //
  _storage->saveStreams();

  _storage->close();
}

void OMKLVStoredObject::save(OMStorable& object)
{
  TRACE("OMKLVStoredObject::save(OMStorable)");

  OMUInt64 position = _storage->position();
  _storage->enterObject(object, position);

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
  OMUInt64 lengthPosition = _storage->reserveKLVLength();

  // Flat properties
  flatSave(properties);

  // patch length
  _storage->fixupKLVLength(lengthPosition);

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

  if ((propertyType->identification() == Type_UniqueObjectIdentification) ||
      (propertyType->identification() == Type_TransferCharacteristic)) {
    // UniqueObjectIdentification properties are stored
    // as SMPTE universal labels in KLV-encoded files
    const OMUniqueObjectIdentification& id =
        *(OMUniqueObjectIdentification*)bits;
    convert(*(OMKLVKey*)buffer, id);
  } else {
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
  }

  // size
  _storage->write(externalBytesSize, _reorderBytes);
  // value
  _storage->write(buffer, externalBytesSize);

  delete [] buffer;
}

void OMKLVStoredObject::save(const OMDataVector& property)
{
  TRACE("OMKLVStoredObject::save(OMDataVector)");

  const OMType* propertyType = property.type();
  ASSERT("Valid property type", propertyType != 0);
  const OMArrayType* at = dynamic_cast<const OMArrayType*>(propertyType);
  ASSERT("Correct type", at != 0);
  OMType* elementType = at->elementType();
  ASSERT("Fixed size elements", elementType->isFixedSize());
  OMUInt32 elementSize = elementType->internalSize();
  OMUInt32 externalElementSize = elementType->externalSize();
  OMUInt32 elementCount = property.count();

    // Allocate buffer for one element
  OMByte* buffer = new OMByte[externalElementSize];
  ASSERT("Valid heap pointer", buffer != 0);

  // size
  // Doh! 32-bit size and count but 16-bit property size
  OMUInt64 size = externalElementSize * elementCount;
  // ASSERT("Valid size"); // tjb
  OMPropertySize propertySize = static_cast<OMPropertySize>(size);
  propertySize = propertySize + sizeof(OMUInt32) + sizeof(OMUInt32);
  _storage->write(propertySize, _reorderBytes);

  // element count
  _storage->write(elementCount, _reorderBytes);

  // element size
  _storage->write(externalElementSize, _reorderBytes);

  OMDataContainerIterator* it = property.createIterator();
  while (++(*it)) {

    // Get a pointer to the element
    const OMByte* bits = it->currentElement();

    if ((elementType->identification() == Type_UniqueObjectIdentification) ||
        (elementType->identification() == Type_TransferCharacteristic)) {
      // UniqueObjectIdentification properties are stored
      // as SMPTE universal labels in KLV-encoded files
      const OMUniqueObjectIdentification& id =
          *(OMUniqueObjectIdentification*)bits;
      convert(*(OMKLVKey*)buffer, id);
    } else {
      // Externalize element
      elementType->externalize(bits,
                               elementSize,
                               buffer,
                               externalElementSize,
                               hostByteOrder());

      // Reorder element
      if (_reorderBytes) {
        elementType->reorder(buffer, externalElementSize);
      }
    }

    // value
    _storage->write(buffer, externalElementSize);

  }
  delete it;
  delete [] buffer;
}

void OMKLVStoredObject::save(const OMDataSet& property)
{
  TRACE("OMKLVStoredObject::save(OMDataSet)");

  const OMType* propertyType = property.type();
  ASSERT("Valid property type", propertyType != 0);
  const OMSetType* st = dynamic_cast<const OMSetType*>(propertyType);
  ASSERT("Correct type", st != 0);
  OMType* elementType = st->elementType();
  ASSERT("Fixed size elements", elementType->isFixedSize());
  OMUInt32 elementSize = elementType->internalSize();
  OMUInt32 externalElementSize = elementType->externalSize();
  OMUInt32 elementCount = property.count();

    // Allocate buffer for one element
  OMByte* buffer = new OMByte[externalElementSize];
  ASSERT("Valid heap pointer", buffer != 0);

  // size
  // Doh! 32-bit size and count but 16-bit property size
  OMUInt64 size = externalElementSize * elementCount;
  // ASSERT("Valid size"); // tjb
  OMPropertySize propertySize = static_cast<OMPropertySize>(size);
  propertySize = propertySize + sizeof(OMUInt32) + sizeof(OMUInt32);
  _storage->write(propertySize, _reorderBytes);

  // element count
  _storage->write(elementCount, _reorderBytes);

  // element size
  _storage->write(externalElementSize, _reorderBytes);

  OMDataContainerIterator* it = property.createIterator();
  while (++(*it)) {

    // Get a pointer to the element
    const OMByte* bits = it->currentElement();


    if ((elementType->identification() == Type_UniqueObjectIdentification) ||
        (elementType->identification() == Type_TransferCharacteristic)) {
      // UniqueObjectIdentification properties are stored
      // as SMPTE universal labels in KLV-encoded files
      const OMUniqueObjectIdentification& id =
          *(OMUniqueObjectIdentification*)bits;
      convert(*(OMKLVKey*)buffer, id);
    } else {
      // Externalize element
      elementType->externalize(bits,
                               elementSize,
                               buffer,
                               externalElementSize,
                               hostByteOrder());

      // Reorder element
      if (_reorderBytes) {
        elementType->reorder(buffer, externalElementSize);
      }
    }

    // value
    _storage->write(buffer, externalElementSize);

  }
  delete it;
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
void OMKLVStoredObject::save(const OMDataStream& /* stream */)
{
  TRACE("OMKLVStoredObject::save(OMDataStream)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

OMRootStorable* OMKLVStoredObject::restore(OMFile& file)
{
  TRACE("OMKLVStoredObject::restore(OMFile)");

#if defined(USETAGTABLE)
  initializeMap(file);
#endif

  OMUInt64 headerPosition;
  OMMXFStorage::findHeader(_storage, headerPosition);
  _storage->setPosition(headerPosition);

  OMKLVKey k;
  _storage->readKLVKey(k);
  if (OMMXFStorage::isHeader(k)) {
    // Read the header partition
    _storage->readHeaderPartition();
    _storage->readKLVKey(k);
  }
  if (k == fillKey) {
    _storage->readKLVFill();
    _storage->readKLVKey(k);
  }

  // Read the primer
  if (k == primerKey) {
    readPrimerPack(file.dictionary());
  }

  file.setLoadMode(OMFile::lazyLoad);

  _storage->readKLVKey(k);
  if (k == fillKey) {
    _storage->readKLVFill();
    _storage->readKLVKey(k);
  }

  // For restoring meta objects
  OMDictionary* metaDictionary = file.dictionary();
  // For restoring objects
  const OMClassFactory* dictionary = file.classFactory();

  // restore the root
  //
  OMClassId cid;
  convert(cid, k);
  OMRootStorable* root = new OMRootStorable();
  ASSERT("Valid heap pointer", root != 0);
  root->attach(&file);
  root->setStore(file.rootStore());
  root->setClassFactory(metaDictionary);
  // HACK4MEIP2
  if (cid == OMRootStorable::_rootClassId) {
    flatRestore(*root->propertySet());
    _storage->removeObject(*root);
    // restore the meta object directory
    //
    _storage->restoreObjectDirectory();
    _storage->readKLVKey(k);
    convert(cid, k);
  } else {
    // tjb - what ?
  }

  // restore the meta dictionary
  //

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
  OMProperty* mdp = root->propertySet()->get(PID_Root_MetaDictionary);
  OMStrongReference* mdsr = dynamic_cast<OMStrongReference*>(mdp);
  ASSERT("Valid type", mdsr != 0);
  OMStrongObjectReference& mdr = mdsr->reference();
  OMStorable* mdo = mdr.getValue();
  // HACK4MEIP2
  if (mdo != 0) {
    ASSERT("Valid object", mdo != 0);
    deepRestore(*mdo->propertySet());
  } else {
    // tjb - what ?
  }

  // restore the client root
  //
  OMStorable* r = 0;
  root->setClassFactory(dictionary);
  convert(cid, k);
  while (dictionary->isRegistered(cid)) {
    OMStorable* object = dictionary->create(cid);
    ASSERT("Registered class id", object != 0);
    ASSERT("Valid class factory", dictionary == object->classFactory());
#if !defined(OM_NO_VALIDATE_DEFINITIONS)
    ASSERT("Valid class definition", object->definition() != 0);
#endif
    if (r == 0) {
      r = object; // HACK4MEIP2 - First object is root
    }
    // Attach the object.
    // tjb !!! object->attach(containingObject, name);
    // tjb !!! object->setStore(this);
    object->onRestore(file.clientOnSaveContext());

    flatRestore(*object->propertySet());

    _storage->readKLVKey(k);
    convert(cid, k);
  }
  OMProperty* hp = root->propertySet()->get(PID_Root_Header);
  OMStrongReference* hsr = dynamic_cast<OMStrongReference*>(hp);
  ASSERT("Valid type", hsr != 0);
  OMStrongObjectReference& hr = hsr->reference();
  OMStorable* ho = hr.getValue();
  // HACK4MEIP2
  if (ho != 0) {
    ASSERT("Valid object", ho != 0);
    deepRestore(*ho->propertySet());
  } else {
    hr.setValue(r);
    deepRestore(*r->propertySet());
    _storage->removeObject(*r);
  }

  _storage->skipLV(); // This V is fill

  _storage->restoreStreams();

  _storage->clearObjectDirectory();

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
  OMStorable* result = _storage->object(id);
  ASSERT("Object found", result != 0);
  _storage->resolve(id);
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

  if ((propertyType->identification() == Type_UniqueObjectIdentification) ||
      (propertyType->identification() == Type_TransferCharacteristic)) {
    // UniqueObjectIdentification properties are stored
    // as SMPTE universal labels in KLV-encoded files.
    const OMKLVKey* key = (OMKLVKey*)buffer;
    OMUniqueObjectIdentification* id =
                                (OMUniqueObjectIdentification*)property.bits();

    const size_t internalSize = sizeof(OMUniqueObjectIdentification);
    property.setSize(internalSize);
    ASSERT("Property value buffer large enough",
                                              property.size() >= internalSize);
    convert(*id, *key);

  } else {
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
  }

  delete [] buffer;
}

  // @mfunc Restore the <c OMDataVector> <p property> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMDataVector>
  //   @parm The external size.
void OMKLVStoredObject::restore(OMDataVector& property,
                                size_t externalSize)
{
  TRACE("OMKLVStoredObject::restore(OMDataVector)");

  const OMType* propertyType = property.type();
  ASSERT("Valid property type", propertyType != 0);
  const OMArrayType* at = dynamic_cast<const OMArrayType*>(propertyType);
  ASSERT("Correct type", at != 0);
  OMType* elementType = at->elementType();
  ASSERT("Fixed size elements", elementType->isFixedSize());
  OMUInt32 externalElementSize = elementType->externalSize();
  OMUInt32 elementSize = elementType->internalSize();

  // Allocate buffer for one element
  OMByte* buffer = new OMByte[externalElementSize];
  ASSERT("Valid heap pointer", buffer != 0);
  OMByte* value = new OMByte[elementSize];
  ASSERT("Valid heap pointer", value != 0);

  property.clear();
  OMUInt32 elementCount = externalSize / externalElementSize;

  for (OMUInt32 i = 0; i < elementCount; i++) {

    // Read one element
    _storage->read(buffer, externalElementSize);

    if ((elementType->identification() == Type_UniqueObjectIdentification) ||
        (elementType->identification() == Type_TransferCharacteristic)) {
      // UniqueObjectIdentification properties are stored
      // as SMPTE universal labels in KLV-encoded files.
      const OMKLVKey* key = (OMKLVKey*)buffer;
      OMUniqueObjectIdentification* id = (OMUniqueObjectIdentification*)value;
      convert(*id, *key);

    } else {
      // Reorder element
      if (_reorderBytes) {
        elementType->reorder(buffer, externalElementSize);
      }

      // Internalize element
      elementType->internalize(buffer,
                               externalElementSize,
                               value,
                               elementSize,
                               hostByteOrder());
    }

    property.appendValue(value);
  }
  delete [] buffer;
  delete [] value;
}

void OMKLVStoredObject::restore(OMDataSet& property,
                                size_t externalSize)
{
  TRACE("OMKLVStoredObject::restore(OMDataSet)");

  const OMType* propertyType = property.type();
  ASSERT("Valid property type", propertyType != 0);
  const OMSetType* st = dynamic_cast<const OMSetType*>(propertyType);
  ASSERT("Correct type", st != 0);
  OMType* elementType = st->elementType();
  ASSERT("Fixed size elements", elementType->isFixedSize());
  OMUInt32 externalElementSize = elementType->externalSize();
  OMUInt32 elementSize = elementType->internalSize();

  // Allocate buffer for one element
  OMByte* buffer = new OMByte[externalElementSize];
  ASSERT("Valid heap pointer", buffer != 0);
  OMByte* value = new OMByte[elementSize];
  ASSERT("Valid heap pointer", value != 0);

  property.clear();
  OMUInt32 elementCount = externalSize / externalElementSize;

  for (OMUInt32 i = 0; i < elementCount; i++) {

    // Read one element
    _storage->read(buffer, externalElementSize);

    if ((elementType->identification() == Type_UniqueObjectIdentification) ||
        (elementType->identification() == Type_TransferCharacteristic)) {
      // UniqueObjectIdentification properties are stored
      // as SMPTE universal labels in KLV-encoded files.
      const OMKLVKey* key = (OMKLVKey*)buffer;
      OMUniqueObjectIdentification* id = (OMUniqueObjectIdentification*)value;
      convert(*id, *key);

    } else {
      // Reorder element
      if (_reorderBytes) {
        elementType->reorder(buffer, externalElementSize);
      }

      // Internalize element
      elementType->internalize(buffer,
                               externalElementSize,
                               value,
                               elementSize,
                               hostByteOrder());
    }
    property.insert(value);
  }
  delete [] buffer;
  delete [] value;
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

  OMWeakObjectReference<OMUniqueObjectIdentification> newReference(
                                                            &singleton,
                                                            id,
                                                            nullOMPropertyTag);
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

  ASSERT("NYI", false);
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
                                                  const OMDataStream& property)
{
  TRACE("OMKLVStoredObject::createStoredStream");

  OMUInt32 sid = _storage->addStream(const_cast<OMDataStream*>(&property));
  OMKLVStoredStream* result = new OMKLVStoredStream(_storage, sid);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

void OMKLVStoredObject::flatSave(const OMPropertySet& properties) const
{
  TRACE("OMKLVStoredObject::flatSave");

  referenceSave(properties.container(), PID_InterchangeObject_InstanceUID);

  if (properties.container()->classId() == Class_Root) {
    OMKLVStoredObject* This = const_cast<OMKLVStoredObject*>(this);
    OMUniqueObjectIdentification id = _storage->generation();
    This->saveObjectDirectoryReference(id);

    OMPropertyId pid = PID_Root_FormatVersion;
    OMUInt32 version = formatVersion;
    This->writeProperty(pid, version);
  }

  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    OMProperty* p = iterator.property();
    ASSERT("Valid property", p != 0);
    ASSERT("Property has a definition", p->definition() != 0);
    if (!p->isOptional() || p->isPresent()) {
      OMPropertyId id = p->propertyId();
      OMDictionary::mapToKLV(id);
      switch (p->storedForm()) {
      case SF_DATA: {
        _storage->write(id, _reorderBytes);
        p->save();
        break;
      }
      case SF_DATA_VECTOR: {
        _storage->write(id, _reorderBytes);
        p->save();
        break;
      }
      case SF_DATA_SET: {
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
        referenceSave(object, id);
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_VECTOR: {
        _storage->write(id, _reorderBytes);
        OMStrongReferenceVector* v = dynamic_cast<OMStrongReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMUInt32 elementCount = v->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = static_cast<OMPropertySize>(
                                sizeof(OMUInt32) + sizeof(OMUInt32)
                                + (elementCount * elementSize));
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
          OMUniqueObjectIdentification id = _storage->instanceId(object);
          _storage->write(id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_STRONG_OBJECT_REFERENCE_SET: {
        _storage->write(id, _reorderBytes);
        OMStrongReferenceSet* s = dynamic_cast<OMStrongReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 elementCount = s->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = static_cast<OMPropertySize>(
                                sizeof(OMUInt32) + sizeof(OMUInt32)
                                + (elementCount * elementSize));
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
          OMUniqueObjectIdentification id = _storage->instanceId(object);
          _storage->write(id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE: {
        OMWeakReference* wr = dynamic_cast<OMWeakReference*>(p);
        ASSERT("Valid type", wr != 0);
        OMWeakObjectReference<OMUniqueObjectIdentification>& r =
                                                               wr->reference();
        OMStorable* object = r.getValue();
        ASSERT("Valid object", object != 0);
        referenceSave(object, id);
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_VECTOR: {
        _storage->write(id, _reorderBytes);
        OMWeakReferenceVector* v = dynamic_cast<OMWeakReferenceVector*>(p);
        ASSERT("Valid type", v != 0);
        OMUInt32 elementCount = v->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = static_cast<OMPropertySize>(
                                sizeof(OMUInt32) + sizeof(OMUInt32)
                                + (elementCount * elementSize));
        _storage->write(size, _reorderBytes);
        _storage->write(elementCount, _reorderBytes);
        _storage->write(elementSize, _reorderBytes);
        OMContainerIterator<OMWeakReferenceVectorElement>& iterator =
                                                                *v->iterator();
        while (++iterator) {
          OMWeakReferenceVectorElement& element = iterator.value();
          OMWeakObjectReference<OMUniqueObjectIdentification>& r =
                                                           element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = _storage->instanceId(object);
          _storage->write(id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_WEAK_OBJECT_REFERENCE_SET: {
        _storage->write(id, _reorderBytes);
        OMWeakReferenceSet* s = dynamic_cast<OMWeakReferenceSet*>(p);
        ASSERT("Valid type", s != 0);
        OMUInt32 elementCount = s->count();
        OMUInt32 elementSize = sizeof(OMUniqueObjectIdentification);
        OMPropertySize size = static_cast<OMPropertySize>(
                                sizeof(OMUInt32) + sizeof(OMUInt32)
                                + (elementCount * elementSize));
        _storage->write(size, _reorderBytes);
        _storage->write(elementCount, _reorderBytes);
        _storage->write(elementSize, _reorderBytes);
        OMContainerIterator<OMWeakReferenceSetElement>& iterator =
                                                                *s->iterator();
        while (++iterator) {
          OMWeakReferenceSetElement& element = iterator.value();
          OMWeakObjectReference<OMUniqueObjectIdentification>& r =
                                                           element.reference();
          OMStorable* object = r.getValue();
          ASSERT("Valid object", object != 0);
          OMUniqueObjectIdentification id = _storage->instanceId(object);
          _storage->write(id, _reorderBytes);
        }
        delete &iterator;
        break;
      }
      case SF_DATA_STREAM: {
        OMDataStreamProperty* stream = dynamic_cast<OMDataStreamProperty*>(p);
        ASSERT("Valid type", stream != 0);
        _storage->write(id, _reorderBytes);
        OMPropertySize s = sizeof(OMUInt32);
        _storage->write(s, _reorderBytes);
        OMStoredStream* ss = stream->stream();
        ASSERT("Valid stream", ss != 0);
        OMKLVStoredStream* kss = dynamic_cast<OMKLVStoredStream*>(ss);
        ASSERT("Valid type", kss != 0);
        OMUInt32 sid = kss->streamIdentification();
        _storage->write(sid, _reorderBytes);
        // There's no place in the file to put the stored byte order
        // until we have metadata streams.
        ASSERT("Supported stream byte order",
                                  (stream->storedByteOrder() == unspecified) ||
                                  (stream->storedByteOrder() == bigEndian));
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

void OMKLVStoredObject::referenceSave(OMStorable* object,
                                      OMPropertyId pid) const
{
  TRACE("OMKLVStoredObject::referenceSave");
  PRECONDITION("Valid object", object != 0);

  OMUniqueObjectIdentification oid = _storage->instanceId(object);
  _storage->write(pid, _reorderBytes);
  OMPropertySize s = sizeof(OMUniqueObjectIdentification);
  _storage->write(s, _reorderBytes);
  _storage->write(oid, _reorderBytes);
}

void OMKLVStoredObject::flatRestore(const OMPropertySet& properties)
{
  TRACE("OMKLVStoredObject::flatRestore");

  OMUInt64 setLength = _storage->readKLVLength();

  const OMUInt16 overhead = sizeof(OMPropertyId) + sizeof(OMPropertySize);

  while (setLength > 0) {
    OMPropertyId pid;
    _storage->read(pid, _reorderBytes);
    OMDictionary::mapFromKLV(pid);
    OMPropertySize length;
    _storage->read(length, _reorderBytes);

    if (pid == PID_InterchangeObject_InstanceUID) {
      // The UID of this object
      referenceRestore(properties.container(), pid);
    } else if ((pid == PID_Root_ObjectDirectory) &&
               (properties.container()->classId() == Class_Root)) {
      OMUniqueObjectIdentification id; // Not yet used
      _storage->read(id, _reorderBytes);
      OMUInt64 offset;
      _storage->read(offset, _reorderBytes);
      _storage->setObjectDirectoryOffset(offset);
    } else if ((pid == PID_Root_FormatVersion) &&
               (properties.container()->classId() == Class_Root)) {
      OMUInt32 version; // Not yet used
      _storage->read(version, _reorderBytes);
    } else if ((!properties.isAllowed(pid)) && (pid > 0x8000)) { // HACK4MEIP2
      // Dark extension
      OMUInt64 len = length;
      _storage->skipV(len);  // discard value !! tjb
    } else if (pid == 0x3b08) { // Preface::PrimaryPackage
      // HACK4MXFLIB
      // Temporary hack - ignore this property - we don't yet have
      // compiled-in knowlege of its 'type'
      OMUInt64 len = length;
      _storage->skipV(len);
    } else {
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
    case SF_DATA_VECTOR: {
      // p->restore(length);
      OMDataVector* dv = dynamic_cast<OMDataVector*>(p);
      ASSERT("Correct type", dv != 0);
      OMUInt32 entryCount;
      _storage->read(entryCount, _reorderBytes);
      OMUInt32 entrySize;
      _storage->read(entrySize, _reorderBytes);
      size_t externalSize = length - (sizeof(OMUInt32) + sizeof(OMUInt32));
      ASSERT("Consistent size", externalSize == entryCount * entrySize);
      restore(*dv, externalSize);
      p->setPresent();
      break;
    }
    case SF_DATA_SET: {
      // p->restore(length);
      OMDataSet* ds = dynamic_cast<OMDataSet*>(p);
      ASSERT("Correct type", ds != 0);
      OMUInt32 entryCount;
      _storage->read(entryCount, _reorderBytes);
      OMUInt32 entrySize;
      _storage->read(entrySize, _reorderBytes);
      size_t externalSize = length - (sizeof(OMUInt32) + sizeof(OMUInt32));
      ASSERT("Consistent size", externalSize == entryCount * entrySize);
      restore(*ds, externalSize);
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

          OMWeakReferenceVectorElement element(v, id, nullOMPropertyTag);
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

          OMWeakReferenceSetElement element(s, id, nullOMPropertyTag);
          s->insert(&id, element);
        }
      }
      p->setPresent();
      break;
    }
    case SF_DATA_STREAM: {
      OMDataStream* stream = dynamic_cast<OMDataStream*>(p);
      ASSERT("Valid type", stream != 0);
      OMUInt32 sid;
      _storage->read(sid, _reorderBytes);
      // There's no place in the file from which to get the stored byte order
      // until we have metadata streams.
      stream->setStoredByteOrder(bigEndian);
      p->setPresent();
      _storage->associate(stream, sid);
      break;
    }
    default:
      break;
    }
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
#if 0 // HACK4MEIP2
    ASSERT("Property has a definition", p->definition() != 0);
#endif
    if (!p->isOptional() || p->isPresent()) {
      switch (p->storedForm()) {
      case SF_STRONG_OBJECT_REFERENCE: {
        OMStrongReference* sr = dynamic_cast<OMStrongReference*>(p);
        ASSERT("Valid type", sr != 0);
        OMStrongObjectReference& r = sr->reference();
        OMStorable* object = r.getValue();
        // HACK4MEIP2
        if (object != 0) {
          ASSERT("Valid object", object != 0);
          deepRestore(*object->propertySet());
        } else {
          // tjb - what ?
        }
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
          OMVector<OMByte*> keys;
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
              if (_storage->containsObject(id)) {
                objects.insert(id);
              }

              OMByte* k = new OMByte[keySize];
              memcpy(k, element.identification(), keySize);
              keys.insert(k);

            }
            delete [] cName;
          }

          OMVectorIterator<OMByte*> kiter(keys, OMBefore);
          while (++kiter) {
            OMByte* k = kiter.value();
            s->remove(k);
            delete [] k;
          }

          OMUInt32 localKey = 0;
          OMVectorIterator<OMUniqueObjectIdentification> viter(objects,
                                                               OMBefore);
          while (++viter) {
            OMUniqueObjectIdentification id = viter.value();
            OMStorable* obj = _storage->object(id);
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
              _storage->resolve(id);
            } // else key already present
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
        OMWeakObjectReference<OMUniqueObjectIdentification>& r =
                                                               wr->reference();
        OMUniqueObjectIdentification id = r.identification();
        OMStorable* obj = _storage->object(id);
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
        OMWeakObjectReference<OMUniqueObjectIdentification> newReference(p,
                                                                         k,
                                                                         tag);
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
          OMWeakObjectReference<OMUniqueObjectIdentification>& r =
                                                           element.reference();
          OMUniqueObjectIdentification id = r.identification();
          OMStorable* obj = _storage->object(id);
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
          OMWeakObjectReference<OMUniqueObjectIdentification> newReference(
                                                                          v,
                                                                          k,
                                                                          tag);
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
            OMWeakObjectReference<OMUniqueObjectIdentification>& r =
                                                           element.reference();
            OMUniqueObjectIdentification id = r.identification();
            objects.insert(id);
          }
          s->removeAllObjects();
          OMVectorIterator<OMUniqueObjectIdentification> viter(objects,
                                                               OMBefore);
          while (++viter) {
            OMUniqueObjectIdentification id = viter.value();
            OMStorable* obj = _storage->object(id);
#if defined(USETAGTABLE)
            if (obj != 0) {
#endif
            ASSERT("Valid object", obj != 0);
            OMProperty* kp = obj->propertySet()->get(keyPid);
            ASSERT("Valid property", kp != 0);
            ASSERT("Consistent sizes", kp->bitsSize() == sizeof(key));
            kp->getBits(reinterpret_cast<OMByte*>(&key), sizeof(key));

            OMWeakReferenceSetElement element(s, key, nullOMPropertyTag);
#if defined(USETAGTABLE)
            OMPropertyTag tag = findTag(s->targetName());
            OMWeakObjectReference<OMUniqueObjectIdentification> newReference(
                                                                          s,
                                                                          key,
                                                                          tag);
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

void OMKLVStoredObject::referenceRestore(OMStorable* object,
                                         OMPropertyId ANAME(pid))
{
  TRACE("OMKLVStoredObject::referenceRestore");

  ASSERT("Property is reference/instance UID",
         pid == PID_InterchangeObject_InstanceUID);
  OMUniqueObjectIdentification iid;
  _storage->read(iid, _reorderBytes);
  _storage->associate(object, iid);
}

void OMKLVStoredObject::writePrimerPack(const OMDictionary* dictionary)
{
  TRACE("OMKLVStoredObject::writePrimerPack");

  OMUInt32 elementCount = 0;
  OMUInt32 elementSize = sizeof(OMPropertyId) +
                         sizeof(OMUniqueObjectIdentification);
  _storage->writeKLVKey(primerKey);
  OMUInt64 lengthPosition = _storage->reserveKLVLength();
  OMUInt64 elementCountPosition = _storage->reserve(sizeof(elementCount));
  _storage->write(elementSize, _reorderBytes);

  // Instance UID
  _storage->write(PID_InterchangeObject_InstanceUID, _reorderBytes);
  OMKLVKey iuidk;
  convert(iuidk, Property_InterchangeObject_InstanceUID);
  _storage->writeKLVKey(iuidk);
  elementCount = elementCount + 1;

  ClassDefinitionsIterator* classes = dictionary->classDefinitions();
  while (++(*classes)) {
    OMObject* obj = classes->currentObject();
    OMClassDefinition* classDefinition = dynamic_cast<OMClassDefinition*>(obj);
    ASSERT("Object is correct type", classDefinition != 0);
    PropertyDefinitionsIterator*
                           properties = classDefinition->propertyDefinitions();
    while (++(*properties)) {
      elementCount = elementCount + 1;
      OMObject* obj = properties->currentObject();
      OMPropertyDefinition* propertyDefinition =
                                      dynamic_cast<OMPropertyDefinition*>(obj);
      ASSERT("Object is correct type", propertyDefinition != 0);
      OMPropertyId pid = propertyDefinition->localIdentification();
      OMDictionary::mapToKLV(pid);
      _storage->write(pid, _reorderBytes);
#if defined(OMONLY)
      OMUniqueObjectIdentification id = propertyDefinition->identification();
#else
      OMUniqueObjectIdentification id = propertyDefinition->identification();
#endif
      OMDictionary::mapToKLV(id);
      OMKLVKey k;
      convert(k, id);
      _storage->writeKLVKey(k);
    }
    delete properties;
  }
  delete classes;

  // patch length and elementCount
  _storage->fixupKLVLength(lengthPosition);
  _storage->fixup(elementCountPosition, elementCount);
}

void OMKLVStoredObject::readPrimerPack(OMDictionary* /* dictionary */)
{
  TRACE("OMKLVStoredObject::readPrimerPack");

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
    OMDictionary::mapFromKLV(pid);
    OMKLVKey x;
    _storage->readKLVKey(x);
    OMUniqueObjectIdentification id;
    convert(id, x);
    OMDictionary::mapFromKLV(id);
  }
}

void OMKLVStoredObject::saveObjectDirectoryReference(
                                        const OMUniqueObjectIdentification& id)
{
  OMPropertySize size = sizeof(OMUniqueObjectIdentification) +
                        sizeof(OMUInt64);
  // pid
  OMPropertyId pid = PID_Root_ObjectDirectory;
  _storage->write(pid, _reorderBytes);
  // size
  _storage->write(size, _reorderBytes);
  // id
  _storage->write(id, _reorderBytes);
  // offset (not yet known, will patch later)
  OMUInt64 offset = 0;
  _storage->reference(_storage->position(), FUT_OBJECTDIRECTORY);
  _storage->write(offset, _reorderBytes);
}

OMUInt64
OMKLVStoredObject::restoreObjectDirectoryReference(
                                              OMUniqueObjectIdentification& id)
{
  TRACE("OMKLVStoredObject::restoreObjectDirectoryReference");

  // pid
  OMPropertyId pid;
  _storage->read(pid, _reorderBytes);
  ASSERT("Expected pid", pid == PID_Root_ObjectDirectory);
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

  // @mfunc Constructor.
  //   @parm The <c OMRawStorage> on which this <c OMKLVStoredObject> resides.
  //   @parm TBS
OMKLVStoredObject::OMKLVStoredObject(OMMXFStorage* s, OMByteOrder byteOrder)
: _storage(s),
  _byteOrder(byteOrder),
  _reorderBytes(false)
{
  TRACE("OMKLVStoredObject::OMKLVStoredObject");

  if (hostByteOrder() == bigEndian) {
    _reorderBytes = false;
  } else {
    _reorderBytes = true;
  }
}

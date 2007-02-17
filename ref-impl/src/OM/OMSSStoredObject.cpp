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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMSSStoredObject

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMSSStoredObject.h"

#include "OMStoredPropertySetIndex.h"
#include "OMProperty.h"
#include "OMPropertySet.h"
#include "OMPropertySetIterator.h"
#include "OMStoredVectorIndex.h"
#include "OMStoredSetIndex.h"
#include "OMDataTypes.h"
#include "OMPropertyTable.h"
#include "OMRootStorable.h"

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
#include "OMDataVector.h"
#include "OMDataSet.h"
#include "OMArrayType.h"
#include "OMSetType.h"
#include "OMDataContainerIterator.h"

#include "OMAssertions.h"
#include "OMExceptions.h"
#include "OMUtilities.h"

#include "OMMSStructuredStorage.h"
#include "OMSSStoredStream.h"
#include "OMType.h"
#include "OMUniqueObjectIdentType.h"
#include "OMBufferedIStream.h"

#include <stdlib.h>

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

static void checkStatus(HRESULT status);

#if defined(OM_DEBUG)
void OMSSStoredObject::incrementOpenStreamCount(void)
{
  TRACE("OMSSStoredObject::incrementOpenStreamCount");
  _openStreams = _openStreams + 1;
  if (_openStreams > _maxOpenStreams) {
    _maxOpenStreams = _openStreams;
  }
}

void OMSSStoredObject::decrementOpenStreamCount(void)
{
  TRACE("OMSSStoredObject::decrementOpenStreamCount");
  PRECONDITION("Non zero count", _openStreams != 0);
  _openStreams = _openStreams - 1;
}

void OMSSStoredObject::incrementOpenStorageCount(void)
{
  TRACE("OMSSStoredObject::incrementOpenStorageCount");
  _openStorages = _openStorages + 1;
  if (_openStorages > _maxOpenStorages) {
    _maxOpenStorages = _openStorages;
  }
}

void OMSSStoredObject::decrementOpenStorageCount(void)
{
  TRACE("OMSSStoredObject::decrementOpenStorageCount");
  PRECONDITION("Non zero count", _openStorages != 0);
  _openStorages = _openStorages - 1;
}

size_t OMSSStoredObject::_openStorages = 0;
size_t OMSSStoredObject::_maxOpenStorages = 0;
size_t OMSSStoredObject::_openStreams = 0;
size_t OMSSStoredObject::_maxOpenStreams = 0;
#endif


  // @mfunc Open the root <c OMSSStoredObject> using the root storage
  //        <p in_storage>. <p mode> specifies the mode in which
  //		to open the root.
  //   @parm The root storage 
  //   @parm The access mode of the object.
  //   @rdesc An <c OMSSStoredObject> representing the root storage
OMSSStoredObject* OMSSStoredObject::open(IStorage *in_storage, OMFile::OMAccessMode mode)
{
  TRACE("OMSSStoredObject::open");
  PRECONDITION("Valid root storage", in_storage != 0);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));
  //FIXME: add code to check if the root storage can be accessed using the 
  //mode specified
  STATSTG statstg;
  HRESULT status = in_storage->Stat(&statstg, STATFLAG_NONAME);
  checkStatus(status);
  PRECONDITION ("Valid root storage access mode: ", 
  		((mode == OMFile::modifyMode && (statstg.grfMode & (STGM_READWRITE)) ||
		(mode == OMFile::readOnlyMode && (statstg.grfMode & STGM_READ) == 0))));

  OMSSStoredObject* newStore = new OMSSStoredObject(in_storage);
  ASSERT("Valid heap pointer", newStore != 0);

#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  newStore->open(mode);

  return newStore;
}

  // @mfunc Open the root <c OMSSStoredObject> using the root storage
  //        <p in_storage>. The byte order of the newly created root 
  //        is given by <p byteOrder>.
  //   @parm The root storage
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMSSStoredObject> representing the root object 
OMSSStoredObject* OMSSStoredObject::create(IStorage *in_storage,
                                             const OMByteOrder byteOrder)
{
  TRACE("OMSSStoredObject::create");
  PRECONDITION("Valid root storage", in_storage != 0);
  
  //FIXME: add code to check if the root storage can be accessed using the 
  //mode specified
  STATSTG statstg;
  HRESULT status = in_storage->Stat(&statstg, STATFLAG_NONAME);
  checkStatus(status);
  PRECONDITION ("Valid root storage access mode: ", 
  	((statstg.grfMode & STGM_READWRITE) || (statstg.grfMode & STGM_WRITE)));

  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));

  OMSSStoredObject* newStore = new OMSSStoredObject(in_storage);
  ASSERT("Valid heap pointer", newStore != 0);
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  newStore->create(byteOrder);

  return newStore;
}

  // @mfunc Destructor.
OMSSStoredObject::~OMSSStoredObject(void)
{
  TRACE("OMSSStoredObject::~OMSSStoredObject");
}

  // @mfunc Create a new <c OMSSStoredObject>, named <p name>,
  //        contained by this <c OMSSStoredObject>.
  //   @parm The name to be used for the new <c OMSSStoredObject>.
  //   @rdesc A new <c OMSSStoredObject> contained by this
  //          <c OMSSStoredObject>.
OMStoredObject* OMSSStoredObject::create(const wchar_t* name)
{
  TRACE("OMSSStoredObject::create");
  PRECONDITION("Valid name", validWideString(name));

  IStorage* newStorage = createStorage(_storage, name);
  OMSSStoredObject* result = new OMSSStoredObject(newStorage);
  ASSERT("Valid heap pointer", result != 0);
  result->create(_byteOrder);
  return result;
}

  // @mfunc Open an exsiting <c OMSSStoredObject>, named <p name>,
  //        contained by this <c OMSSStoredObject>.
  //   @parm The name of the existing <c OMSSStoredObject>.
  //   @rdesc The existing <c OMSSStoredObject> contained by this
  //          <c OMSSStoredObject>.
OMStoredObject* OMSSStoredObject::open(const wchar_t* name)
{
  TRACE("OMSSStoredObject::open");
  PRECONDITION("Valid name", validWideString(name));

  IStorage* newStorage = openStorage(_storage, name, _mode);
  OMSSStoredObject* result = new OMSSStoredObject(newStorage);
  ASSERT("Valid heap pointer", result != 0);
  result->open(_mode);
  return result;
}

  // @mfunc Close this <c OMSSStoredObject>.
void OMSSStoredObject::close(void)
{
  TRACE("OMSSStoredObject::close");

  if (_open) {

    closeStream(_properties);

    delete _index;
    _index = 0;

    closeStorage(_storage);

    _open = false;
  }
}

OMByteOrder OMSSStoredObject::byteOrder(void) const
{
  TRACE("OMSSStoredObject::byteOrder");

  return _byteOrder;
}

void OMSSStoredObject::save(OMFile& file)
{
  TRACE("OMSSStoredObject::save(OMFile)");
  file.root()->save();
  save(file.referencedProperties());
}

void OMSSStoredObject::save(OMStorable& object)
{
  TRACE("OMSSStoredObject::save(OMStorable)");
  save(object.classId());
  save(*object.propertySet());
}

  // @mfunc Save the <c OMStoredObjectIdentification> <p id>
  //        in this <c OMSSStoredObject>.
  //   @parm The <c OMStoredObjectIdentification> of this
  //         <c OMSSStoredObject>.
void OMSSStoredObject::save(const OMStoredObjectIdentification& id)
{
  TRACE("OMSSStoredObject::save");

  setClass(_storage, id);
}

static const wchar_t* const propertyStreamName = L"properties";

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMSSStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMSSStoredObject::save(const OMPropertySet& properties)
{
  TRACE("OMSSStoredObject::save(OMPropertySet)");

#if defined(OM_DEFER_OPEN)
  if (!_open) {
    PRECONDITION("Not already open", _properties == 0);
#if defined(OM_BUFFERED_STREAMS)
    _properties = openBufferedStream(_storage, propertyStreamName);
#else
    _properties = openStream(_storage, propertyStreamName);
#endif
    _open = true;
  }
#else
  PRECONDITION("Already open", _open);
#endif
  PRECONDITION("At start of value stream", streamPosition(_properties) == 0);
  PRECONDITION("At start of value stream", _offset == 0);

  OMPropertyCount count = properties.countPresent();
  delete _index;
  _index = 0; // for BoundsChecker
  _index = new OMStoredPropertySetIndex(count);
  ASSERT("Valid heap pointer", _index != 0);
  OMPropertyCount countPresent = properties.countPresent();
  size_t size = indexHeaderSize + (countPresent * indexEntrySize);
  ASSERT("Property index not too big", size <= OMPROPERTYOFFSET_MAX );
  OMPropertyOffset indexSize = static_cast<OMPropertyOffset>(size);
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

#if defined(OM_VALIDATE_PROPERTYSETS)
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
  //        <c OMSSStoredObject>.
  //   @parm TBS
void OMSSStoredObject::save(const OMSimpleProperty& property)
{
  TRACE("OMSSStoredObject::save");

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = property.storedForm();
  OMUInt32 bs = property.bitsSize();
  ASSERT("Property value not too big", bs <= OMPROPERTYSIZE_MAX);
  OMPropertySize size = static_cast<OMPropertySize>(bs);
  OMByte* bits = property.bits();
  const OMType* propertyType = property.type();

  if (propertyType != 0) { // tjb - temporary, should be ASSERTION below

    ASSERT("Valid property type", propertyType != 0);

    // Allocate buffer for property value
    OMUInt32 sz = propertyType->externalSize(bits, size);
    ASSERT("Property not too big", sz <= OMPROPERTYSIZE_MAX);
    OMPropertySize externalBytesSize = static_cast<OMPropertySize>(sz);
    OMByte* buffer = new OMByte[externalBytesSize];
    ASSERT("Valid heap pointer", buffer != 0);

	ASSERT( "Zero property length", 0 != externalBytesSize );

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

  // @mfunc Save the <c OMDataVector> <p property> in this
  //        <c OMSSStoredObject>.
  //   @parm TBS
void OMSSStoredObject::save(const OMDataVector& property)
{
  TRACE("OMSSStoredObject::save");

  // Save as if this were an OMSimpleProperty

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = SF_DATA; // != property.storedForm() !
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

  _index->insert(propertyId, storedForm, _offset, propertySize);

  OMDataContainerIterator* it = property.createIterator();
  while (++(*it)) {

    // Get a pointer to the element
    const OMByte* bits = it->currentElement();


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

    // value
    writeToStream(_properties, buffer, externalElementSize);
    _offset += externalElementSize;

  }
  delete it;
  delete [] buffer;
}

void OMSSStoredObject::save(const OMDataSet& property)
{
  TRACE("OMSSStoredObject::save(OMDataSet)");

  // Save as if this were an OMSimpleProperty

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = SF_DATA; // != property.storedForm() !
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

  _index->insert(propertyId, storedForm, _offset, propertySize);

  OMDataContainerIterator* it = property.createIterator();
  while (++(*it)) {

    // Get a pointer to the element
    const OMByte* bits = it->currentElement();


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

    // value
    writeToStream(_properties, buffer, externalElementSize);
    _offset += externalElementSize;

  }
  delete it;
  delete [] buffer;
}

  // @mfunc Save the <c OMStrongReference> <p singleton> in this
  //        <c OMSSStoredObject>.
  //   @parm TBS
void OMSSStoredObject::save(const OMStrongReference& singleton)
{
  TRACE("OMSSStoredObject::save");

  // Write the index entry.
  //
  wchar_t* name = referenceName(singleton.name(), singleton.propertyId());
  saveName(singleton, name);
  delete [] name;

  singleton.reference().save();
}

  // @mfunc Save the <c OMStrongReferenceVector> <p vector> in this
  //        <c OMSSStoredObject>.
  //   @parm The vector of strong references to save.
void OMSSStoredObject::save(const OMStrongReferenceVector& vector)
{
  TRACE("OMSSStoredObject::save");

  // create a vector index
  //
  OMUInt32 count = vector.count();
  OMStoredVectorIndex* index = new OMStoredVectorIndex(count);
  ASSERT("Valid heap pointer", index != 0);
  index->setFirstFreeKey(vector.localKey());
  OMPropertyOffset position = 0;

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
  //        <c OMSSStoredObject>.
  //   @parm The set of strong references to save.
void OMSSStoredObject::save(const OMStrongReferenceSet& set)
{
  TRACE("OMSSStoredObject::save");

  // create a set index
  //
  OMUInt32 count = 0;
  OMContainerIterator<OMStrongReferenceSetElement>& iterator = *set.iterator();
  while (++iterator) {
    OMStrongReferenceSetElement& element = iterator.value();
    // Count the elements that will be saved.
    if (element.isSticky() || element.referenceCount() > 0) {
      count = count + 1;
    }
  }
  OMKeySize keySize = set.keySize();
  OMPropertyId keyPropertyId = set.keyPropertyId();
  OMStoredSetIndex* index = new OMStoredSetIndex(count,
                                                 keyPropertyId,
                                                 keySize);
  ASSERT("Valid heap pointer", index != 0);
  index->setFirstFreeKey(set.localKey());
  OMPropertyOffset position = 0;

  // Iterate over the set saving each element. The index entries
  // are written in order of their unique keys.
  //
  iterator.reset(OMBefore);
  while (++iterator) {

    OMStrongReferenceSetElement& element = iterator.value();

    // Save elements that are sticky or that are referenced.
    if (element.isSticky() || element.referenceCount() > 0) {

      // enter into the index
      //
      void* key = element.identification();
      // Sticky reference counts were previously incorrectly persisted
      // as 0x00000001 instead of 0xffffffff so we add 2 below for
      // compatability.
      OMUInt32 referenceCount = element.referenceCount() + 2;
      index->insert(position,
                    element.localKey(),
                    referenceCount,
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
  //        <c OMSSStoredObject>.
  //   @parm TBS
void OMSSStoredObject::save(const OMWeakReference& singleton)
{
  TRACE("OMSSStoredObject::save");

  OMPropertyId propertyId = singleton.propertyId();
  OMStoredForm storedForm = singleton.storedForm();
  ASSERT("Valid identification size",
                  singleton.keySize() == sizeof(OMUniqueObjectIdentification));
  const OMUniqueObjectIdentification& id =
    *reinterpret_cast<const OMUniqueObjectIdentification*>(
                                               singleton.identificationBits());
  OMPropertyTag tag = singleton.targetTag();
  OMPropertyId keyPid = singleton.keyPropertyId();

  save(propertyId, storedForm, id, tag, keyPid);

  /* Does nothing. Remove, replace? - Alexey
  singleton.reference().save();
  */

  singleton.clearTargetTag();
}

  // @mfunc Save the <c OMWeakReferenceVector> <p vector> in this
  //        <c OMSSStoredObject>.
  //   @parm TBS
void OMSSStoredObject::save(const OMWeakReferenceVector& vector)
{
  TRACE("OMSSStoredObject::save");

  OMPropertyTag tag = vector.targetTag();

  // create a vector index
  //
  OMUInt32 count = vector.count();
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
  //        <c OMSSStoredObject>.
  //   @parm TBS
void OMSSStoredObject::save(const OMWeakReferenceSet& set)
{
  TRACE("OMSSStoredObject::save");

  OMPropertyTag tag = set.targetTag();

  // create a set index
  //
  OMUInt32 count = set.count();
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
  //        <c OMSSStoredObject>.
  //   @parm The table to save.
void OMSSStoredObject::save(const OMPropertyTable* table)
{
  TRACE("OMSSStoredObject::save");

  PRECONDITION("Valid property table", table != 0);

#if defined(OM_BUFFERED_STREAMS)
  IStream* stream = createBufferedStream(_storage, L"referenced properties");
#else
  IStream* stream = createStream(_storage, L"referenced properties");
#endif

  // byte order
  writeUInt8ToStream(stream, _byteOrder);

  // count of paths
  OMPropertyCount count = table->count();
  writeUInt16ToStream(stream, count, _reorderBytes);

  // count of property ids
  OMUInt32 pidCount = 0;
  for (OMPropertyCount i = 0; i < count; i++) {
    OMUInt32 length = lengthOfPropertyPath(table->valueAt(i));
    pidCount = pidCount + length + 1;
  }
  writeUInt32ToStream(stream, pidCount, _reorderBytes);

  // sequence of null terminated pids
  for (OMPropertyCount j = 0; j < count; j++) {
    const OMPropertyId* internalName = table->valueAt(j);
    OMUInt32 pidCount = lengthOfPropertyPath(internalName);
    OMUInt32 byteCount = pidCount * sizeof(OMPropertyId);
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
  //        <c OMSSStoredObject>.
  //   @parm The <c OMDataStream> to save.
void OMSSStoredObject::save(const OMDataStream& stream)
{
  TRACE("OMSSStoredObject::save");

  // Use the property name as the stream name
  //
  OMPropertyId propertyId = stream.propertyId();
  OMStoredForm storedForm = stream.storedForm();
  wchar_t* name = streamName(stream.name(), stream.propertyId());
  OMByteOrder byteOrder = stream.storedByteOrder();
  saveStream(propertyId, storedForm, name, byteOrder);
  delete [] name;
}

OMRootStorable* OMSSStoredObject::restore(OMFile& file)
{
  TRACE("OMSSStoredObject::restore(OMFile)");

  enum OMFile::OMLoadMode savedLoadMode = file.loadMode();
  file.setLoadMode(OMFile::lazyLoad);

  OMPropertyTable* table = 0;
  restore(table);
  file.setReferencedProperties(table);

  OMClassId id;
  file.rootStore()->restore(id);
  ASSERT("Valid root stored object", id == OMRootStorable::_rootClassId);

  OMRootStorable* root = new OMRootStorable();
  ASSERT("Valid heap pointer", root != 0);
  root->attach(&file);
  root->setStore(file.rootStore());
  root->setClassFactory(file.dictionary());

  root->restoreContents();

#if defined(OM_DEBUG)
  OMDictionary *metaDictionary =
#endif
  root->dictionary();
  ASSERT("Consistent dictionaries", metaDictionary == file.dictionary());
  root->setClassFactory(file.classFactory());

  file.setLoadMode(savedLoadMode);
  return root;
}

OMStorable*
OMSSStoredObject::restoreObject(const OMStrongObjectReference& reference)
{
  TRACE("OMSSStoredObject::restoreObject");

  const wchar_t* name = reference.name();
  OMProperty* property = reference.property();
  OMStorable* containingObject = property->propertySet()->container();

  OMClassId cid;
  restore(cid);
  const OMClassFactory* classFactory = containingObject->classFactory();
  OMStorable* object = classFactory->create(cid);
  ASSERT("Registered class id", object != 0);
  ASSERT("Valid class factory", classFactory == object->classFactory());
#if !defined(OM_NO_VALIDATE_DEFINITIONS)
  ASSERT("Valid class definition", object->definition() != 0);
#endif
  // Attach the object.
  object->attach(containingObject, name);
  object->setStore(this);
  object->restoreContents();
  return object;
}

  // @mfunc Restore the <c OMStoredObjectIdentification>
  //        of this <c OMSSStoredObject> into <p id>.
  //   @parm The <c OMStoredObjectIdentification> of this
  //         <c OMSSStoredObject>.
void OMSSStoredObject::restore(OMStoredObjectIdentification& id)
{
  TRACE("OMSSStoredObject::restore");

  getClass(_storage, id);
}

  // @mfunc Restore the <c OMPropertySet> <p properties> into
  //        this <c OMSSStoredObject>.
  //   @parm The <c OMPropertySet> to restore.
void OMSSStoredObject::restore(OMPropertySet& properties)
{
  TRACE("OMSSStoredObject::restore");
#if defined(OM_DEFER_OPEN)
  PRECONDITION("Not already open", !_open);
#else
  PRECONDITION("Already open", _open);
#endif

  _index = restore();
  size_t entries = _index->entries();

  OMPropertyId propertyId;
  OMStoredForm type;
  OMUInt32 offset;
  OMPropertySize length;
  OMUInt16 context = 0;
  for (OMUInt16 i = 0; i < entries; i++) {
    _index->iterate(context, propertyId, type, offset, length);
    OMProperty* p = properties.get(propertyId);
    ASSERT("Valid property", p != 0);
    p->restore(length);
    ASSERT("Property is present", IMPLIES(p->isOptional(), p->isPresent()));
  }
#if defined(OM_VALIDATE_PROPERTYSETS)
  validate(&properties, _index);
#endif
  streamSetPosition(_properties, 0);
  POSTCONDITION("At start of properties stream",
                                       streamPosition(_properties) == 0);
}

  // @mfunc Restore the <c OMSimpleProperty> <p property> into this
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMSimpleProperty& property,
                                 OMPropertySize externalSize)
{
  TRACE("OMSSStoredObject::restore");

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
    OMUInt32 sz = propertyType->internalSize(buffer, externalSize);
    ASSERT("Property not too big", sz <= OMPROPERTYSIZE_MAX);
    OMPropertySize requiredBytesSize = static_cast<OMPropertySize>(sz);
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

  // @mfunc Restore the <c OMDataVector> <p property> into this
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMDataVector& property,
                                 OMPropertySize externalSize)
{
  TRACE("OMSSStoredObject::restore");

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

  for (size_t i = 0; i < elementCount; i++) {

    // Read one element
    readFromStream(_properties, buffer, externalElementSize);

    // Reorder one element
    if (byteOrder() != hostByteOrder()) {
      elementType->reorder(buffer, externalElementSize);
    }

    // Internalize one element
    elementType->internalize(buffer,
                             externalElementSize,
                             value,
                             elementSize,
                             hostByteOrder());

    property.appendValue(value);
  }
  delete [] value;
  delete [] buffer;
}

void OMSSStoredObject::restore(OMDataSet& property,
                                OMPropertySize externalSize)
{
  TRACE("OMSSStoredObject::restore(OMDataSet)");

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

  for (size_t i = 0; i < elementCount; i++) {

    // Read one element
    readFromStream(_properties, buffer, externalElementSize);

    // Reorder one element
    if (byteOrder() != hostByteOrder()) {
      elementType->reorder(buffer, externalElementSize);
    }

    // Internalize one element
    elementType->internalize(buffer,
                             externalElementSize,
                             value,
                             elementSize,
                             hostByteOrder());

    property.insert(value);
  }
  delete [] value;
  delete [] buffer;
}

  // @mfunc Restore the <c OMStrongReference> <p singleton> into this
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMStrongReference& singleton,
                                OMPropertySize externalSize)
{
  TRACE("OMSSStoredObject::restore");

  wchar_t* name = referenceName(singleton.name(), singleton.propertyId());
  restoreName(singleton, name, externalSize);
  OMStrongObjectReference newReference(&singleton, name);
  delete [] name;
  singleton.reference() = newReference;
  singleton.reference().restore();
}

  // @mfunc Restore the <c OMStrongReferenceVector> <p vector> into this
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMStrongReferenceVector& vector,
                                OMPropertySize externalSize)
{
  TRACE("OMSSStoredObject::restore");

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
  OMUInt32 entries = vectorIndex->entries();
  if (entries > 0) {
    vector.grow(entries); // Set the vector size
    OMUInt32 context = 0;
    OMUInt32 localKey;
    for (OMUInt32 i = 0; i < entries; i++) {
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
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMStrongReferenceSet& set,
                                OMPropertySize externalSize)
{
  TRACE("OMSSStoredObject::restore");

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
  OMUInt32 entries = setIndex->entries();
  OMUInt32 context = 0;
  OMUInt32 localKey;
  OMUInt32 count;
  OMKeySize keySize = setIndex->keySize();
  OMByte* key = new OMByte[keySize];
  ASSERT("Valid heap pointer", key != 0);

  for (OMUInt32 i = 0; i < entries; i++) {
    setIndex->iterate(context, localKey, count, key);
    // Restore the object only if it doesn't already exist in the set.
    // Since the object is uniquely identified by the key, the
    // external copy is identical to the internal one, so we may
    // safely ignore the external one.
    //
    if (!set.contains(key)) {
      wchar_t* name = elementName(setName, setId, localKey);
      // Sticky reference counts were previously incorrectly persisted
      // as 0x00000001 instead of 0xffffffff so we subract 2 below for
      // compatability.
      OMStrongReferenceSetElement element(&set,
                                          name,
                                          localKey,
                                          count - 2,
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
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMWeakReference& singleton,
                                OMPropertySize ANAME(externalSize))
{
  TRACE("OMSSStoredObject::restore");

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

  singleton.setIdentificationBits(&id, sizeof(id));
  singleton.setTargetTag(tag);
  
  /* Does nothing. Remove, replace? - Alexey
  reference.restore();
  */
}

  // @mfunc Restore the <c OMWeakReferenceVector> <p vector> into this
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMWeakReferenceVector& vector,
                                OMPropertySize externalSize)
{
  TRACE("OMSSStoredObject::restore");

  // restore the index
  //
  OMUniqueObjectIdentification* vectorIndex = 0;
  OMUInt32 entries;
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
    for (OMUInt32 i = 0; i < entries; i++) {
      OMUniqueObjectIdentification key = vectorIndex[i];
      OMWeakReferenceVectorElement element(&vector, key, tag);
      element.restore();
      vector.insert(i, element);
    }
  }
  delete [] vectorIndex;
}

  // @mfunc Restore the <c OMWeakReferenceSet> <p set> into this
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMWeakReferenceSet& set,
                                OMPropertySize externalSize)
{
  TRACE("OMSSStoredObject::restore");

  // restore the index
  //
  OMUniqueObjectIdentification* setIndex = 0;
  OMUInt32 entries;
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

  // @mfunc Restore the <c OMPropertyTable> in this <c OMSSStoredObject>.
  //   @parm A pointer to the newly restored <c OMPropertyTable> by reference.
void OMSSStoredObject::restore(OMPropertyTable*& table)
{
  TRACE("OMSSStoredObject::restore");

#if defined(OM_BUFFERED_STREAMS)
  IStream* stream = openBufferedStream(_storage, L"referenced properties");
#else
  IStream* stream = openStream(_storage, L"referenced properties");
#endif

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
    for (OMUInt32 i = 0; i < count; i++) {
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
  //        <c OMSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSStoredObject::restore(OMDataStream& stream,
                                OMPropertySize ANAME(externalSize))
{
  TRACE("OMSSStoredObject::restore");

  OMPropertyId propertyId = stream.propertyId();
  OMStoredForm storedForm = stream.storedForm();
  wchar_t* sName = streamName(stream.name(), propertyId);
  size_t count = lengthOfWideString(sName) + 1;
  ASSERT("String not too long", count <= OMUINT16_MAX);
  OMUInt16 characterCount = static_cast<OMUInt16>(count);
  OMPropertySize size = (characterCount * sizeof(OMCharacter)) + 1;
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
  //        <p property> contained within this <c OMSSStoredObject>.
  //   @parm TBS
  //   @rdesc TBS
OMStoredStream* OMSSStoredObject::openStoredStream(
                                                  const OMDataStream& property)
{
  TRACE("OMSSStoredObject::openStoredStream");

  wchar_t* sName = streamName(property.name(), property.propertyId());
  IStream* stream = openStream(sName);
  OMSSStoredStream* result = new OMSSStoredStream(stream);
  ASSERT("Valid heap pointer", result != 0);
  delete [] sName;

  return result;
}

  // @mfunc Create an <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMSSStoredObject>.
  //   @parm TBS
  //   @rdesc TBS
OMStoredStream* OMSSStoredObject::createStoredStream(
                                                  const OMDataStream& property)
{
  TRACE("OMSSStoredObject::createStoredStream");

  wchar_t* sName = streamName(property.name(), property.propertyId());
  IStream* stream = createStream(sName);
  OMSSStoredStream* result = new OMSSStoredStream(stream);
  ASSERT("Valid heap pointer", result != 0);
  delete [] sName;
  return result;
}

  // @mfunc Constructor.
  //   @parm The IStorage for the persistent representation of
  //         this <c OMSSStoredObject>.
OMSSStoredObject::OMSSStoredObject(IStorage* s)
: _storage(s), _index(0), _properties(0),
  _offset(0), _open(false), _mode(OMFile::readOnlyMode),
  _byteOrder(hostByteOrder()), _reorderBytes(false)
{
  TRACE("OMSSStoredObject::OMSSStoredObject");
}

  // @mfunc Check that the <c OMPropertySet> <p propertySet> is
  //        consistent with the <c OMStoredPropertySetIndex>
  //        propertySetIndex.
  //   @parm The <c OMPropertySet> to validate.
  //   @parm The <c OMStoredPropertySetIndex> to validate.
void OMSSStoredObject::validate(
                        const OMPropertySet* propertySet,
                        const OMStoredPropertySetIndex* propertySetIndex) const
{
  TRACE("OMSSStoredObject::validate");
  PRECONDITION("Valid property set", propertySet != 0);
  PRECONDITION("Valid property set index", propertySetIndex != 0);

  OMPropertyId propertyId;
  OMStoredForm type;
  OMUInt32 offset;
  OMPropertySize length;
  OMUInt16 context;

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
  //         <c OMSSStoredObject>, the vector is named <p vectorName>.
  //   @parm The <c OMStoredVectorIndex> to save.
  //   @parm The name of the vector.
void OMSSStoredObject::save(const OMStoredVectorIndex* vector,
                             const wchar_t* vectorName)
{
  TRACE("OMSSStoredObject::save");
  PRECONDITION("Valid vector", vector != 0);
  PRECONDITION("Valid vector name", validWideString(vectorName));

  // Calculate the stream name for the index.
  //
  wchar_t* vectorIndexName = collectionIndexStreamName(vectorName);

  // Create the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  IStream* vectorIndexStream = createBufferedStream(_storage, vectorIndexName);
#else
  IStream* vectorIndexStream = createStream(_storage, vectorIndexName);
#endif
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
  OMUInt32 context = 0;
  OMUInt32 name;
  for (OMUInt32 i = 0; i < entries; i++) {
    vector->iterate(context, name);
    writeUInt32ToStream(vectorIndexStream, name, _reorderBytes);
  }

  // Close the stream.
  //
  closeStream(vectorIndexStream);
}

  // @mfunc  Save the <c OMStoredSetIndex> <p set> in this
  //         <c OMSSStoredObject>, the set is named <p setName>.
  //   @parm The <c OMStoredSetIndex> to save.
  //   @parm The name of the set.
void OMSSStoredObject::save(const OMStoredSetIndex* set,
                             const wchar_t* setName)
{
  TRACE("OMSSStoredObject::save");
  PRECONDITION("Valid set", set != 0);
  PRECONDITION("Valid set name", validWideString(setName));

  // Calculate the stream name for the index.
  //
  wchar_t* setIndexName = collectionIndexStreamName(setName);

  // Create the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  IStream* setIndexStream = createBufferedStream(_storage, setIndexName);
#else
  IStream* setIndexStream = createStream(_storage, setIndexName);
#endif
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
  OMUInt32 context = 0;
  OMUInt32 name;
  OMUInt32 count;
  OMByte* key = new OMByte[keySize];
  ASSERT("Valid heap pointer", key != 0);
  for (OMUInt32 i = 0; i < entries; i++) {
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
void OMSSStoredObject::save(OMPropertyId propertyId,
                             OMStoredForm storedForm,
                             const OMUniqueObjectIdentification& id,
                             OMPropertyTag tag,
                             OMPropertyId keyPropertyId)
{
  TRACE("OMSSStoredObject::save");

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
void OMSSStoredObject::save(const wchar_t* collectionName,
                             const OMUniqueObjectIdentification* index,
                             OMUInt32 count,
                             OMPropertyTag tag,
                             OMPropertyId keyPropertyId)
{
  TRACE("OMSSStoredObject::save");

  PRECONDITION("Valid index", IMPLIES(count != 0, index!= 0));
  PRECONDITION("Valid index", IMPLIES(count == 0, index== 0));
  PRECONDITION("Valid collection name", validWideString(collectionName));

  // Calculate the stream name for the index.
  //
  wchar_t* indexName = collectionIndexStreamName(collectionName);

  // Create the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  IStream* indexStream = createBufferedStream(_storage, indexName);
#else
  IStream* indexStream = createStream(_storage, indexName);
#endif
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

void OMSSStoredObject::saveStream(OMPropertyId pid,
                                   OMStoredForm storedForm,
                                   const wchar_t* name,
                                   OMByteOrder byteOrder)
{
  TRACE("OMSSStoredObject::saveStream");

  // Byte order
  //
  writeUInt8ToStream(_properties, byteOrder);

  // Name
  //
  size_t count = lengthOfWideString(name) + 1;
  ASSERT("String not too long", count <= OMUINT16_MAX);
  OMUInt16 characterCount = static_cast<OMUInt16>(count);
  OMCharacter* buffer = new OMCharacter[characterCount];
  ASSERT("Valid heap pointer", buffer != 0);
  externalizeString(name, buffer, characterCount);
  if (_reorderBytes) {
    reorderString(buffer, characterCount);
  }
  OMPropertySize byteCount = characterCount * sizeof(OMCharacter);
  writeToStream(_properties, buffer, byteCount);
  delete [] buffer;

  // Index entry.
  //
  OMPropertySize size = byteCount + 1;
  _index->insert(pid, storedForm, _offset, size);
  _offset += size;
}

  // @mfunc Restore the vector named <p vectorName> into this
  //        <c OMSSStoredObject>.
  //   @parm The name of the vector.
  //   @rdesc The newly restored <c OMStoredVectorIndex>.
void OMSSStoredObject::restore(OMStoredVectorIndex*& vector,
                                const wchar_t* vectorName)
{
  TRACE("OMSSStoredObject::restore");
  PRECONDITION("Valid vector name", validWideString(vectorName));

  // Calculate the stream name for the index.
  //
  wchar_t* vectorIndexName = collectionIndexStreamName(vectorName);

  // Open the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  IStream* vectorIndexStream = openBufferedStream(_storage, vectorIndexName);
#else
  IStream* vectorIndexStream = openStream(_storage, vectorIndexName);
#endif
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
  for (OMUInt32 i = 0; i < entries; i++) {
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
  //        <c OMSSStoredObject>.
  //   @parm The name of the set.
  //   @rdesc The newly restored <c OMStoredSetIndex>.
void OMSSStoredObject::restore(OMStoredSetIndex*& set,
                                const wchar_t* setName)
{
  TRACE("OMSSStoredObject::restore");
  PRECONDITION("Valid set name", validWideString(setName));

  // Calculate the stream name for the index.
  //
  wchar_t* setIndexName = collectionIndexStreamName(setName);

  // Open the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  IStream* setIndexStream = openBufferedStream(_storage, setIndexName);
#else
  IStream* setIndexStream = openStream(_storage, setIndexName);
#endif
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
  for (OMUInt32 i = 0; i < entries; i++) {
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
void OMSSStoredObject::restore(OMPropertyId propertyId,
                                OMStoredForm storedForm,
                                OMUniqueObjectIdentification& id,
                                OMPropertyTag& tag,
                                OMPropertyId& keyPropertyId)
{
  TRACE("OMSSStoredObject::restore");

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
    reorderUInt16(tag);
    reorderUInt16(keyPropertyId);
    // no need to reorder keySize
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
void OMSSStoredObject::restore(const wchar_t* collectionName,
                                OMUniqueObjectIdentification*& index,
                                OMUInt32 &count,
                                OMPropertyTag& tag,
                                OMPropertyId& keyPropertyId)
{
  TRACE("OMSSStoredObject::restore");

  // Calculate the stream name for the index.
  //
  wchar_t* indexName = collectionIndexStreamName(collectionName);

  // Open the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  IStream* indexStream = openBufferedStream(_storage, indexName);
#else
  IStream* indexStream = openStream(_storage, indexName);
#endif
  delete [] indexName;

  // Read the count of elements.
  //
  OMUInt32 entries;
  readUInt32FromStream(indexStream, entries, _reorderBytes);
  count = entries;

  // Read the tag.
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

void OMSSStoredObject::restoreStream(OMPropertyId pid,
                                      OMStoredForm storedForm,
                                      OMPropertySize size,
                                      wchar_t** name,
                                      OMByteOrder* byteOrder)
{
  TRACE("OMSSStoredObject::restoreStream");

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

  // @mfunc Write a property value to this <c OMSSStoredObject>. The
  //        property value to be written occupies <p size> bytes at
  //        the address <p start>. The property id is <p propertyId>.
  //        The property type is <p type>.
  //   @parm The property id.
  //   @parm The property type.
  //   @parm The start address of the property value.
  //   @parm The size of the property value in bytes.
void OMSSStoredObject::write(OMPropertyId propertyId,
                              OMStoredForm storedForm,
                              void* start,
                              OMPropertySize size)
{
  TRACE("OMSSStoredObject::write");
  PRECONDITION("Valid data", start != 0);
  PRECONDITION("Valid size", size > 0);

  _index->insert(propertyId, storedForm, _offset, size);

  // Write property value.
  //
  writeToStream(_properties, start, size);
  _offset += size;
}

  // @mfunc Read a property value from this <c OMSSStoredObject>.
  //        The property value is read into a buffer which occupies
  //        <p size> bytes at the address <p start>. The property id
  //        is <p propertyId>. The property type is <p type>.
  //   @parm OMPropertyId | propertyId | The property id.
  //   @parm int | type | The property type.
  //   @parm void* | start | The start address of the buffer to hold the
  //         property value.
  //   @parm OMUInt32 | size | The size of the buffer in bytes.
void OMSSStoredObject::read(OMPropertyId ANAME(propertyId),
                             OMStoredForm ANAME(storedForm),
                             void* start,
                             OMPropertySize size)
{
  TRACE("OMSSStoredObject::read");
  PRECONDITION("Valid data", start != 0);
  PRECONDITION("Valid size", size > 0);

#if defined(OM_DEBUG)
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
#endif

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
  //        <c OMSSStoredObject>.
  //   @parm The name of the stream to open.
  //   @rdesc An open stream.
IStream* OMSSStoredObject::openStream(const wchar_t* streamName)
{
  TRACE("OMSSStoredObject::openStream");
  PRECONDITION("Valid stream name", validWideString(streamName));

  return openStream(_storage, streamName);
}

  // @mfunc Create a stream called <p streamName> contained within
  //        this <c OMSSStoredObject>.
  //   @parm The name of the stream to create.
  //   @rdesc An open stream.
IStream* OMSSStoredObject::createStream(const wchar_t* streamName)
{
  TRACE("OMSSStoredObject::createStream");
  PRECONDITION("Valid stream name", validWideString(streamName));

  return createStream(_storage, streamName);
}

  // @mfunc Read <p size> bytes from <p stream> into the buffer at
  //        address <p data>.
  //   @parm The stream from which to read.
  //   @parm The buffer into which the bytes are read.
  //   @parm The number of bytes to read.
void OMSSStoredObject::readFromStream(IStream* stream,
                                       void* data,
                                       OMUInt32 size)
{
  TRACE("OMSSStoredObject::readFromStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", size > 0);

  OMUInt32 bytesRead;
  HRESULT status = stream->Read(data, size, &bytesRead);
  checkStatus(status);
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
void OMSSStoredObject::readFromStream(IStream* stream,
                                       OMByte* data,
                                       const OMUInt32 bytes,
                                       OMUInt32& bytesRead)
{
  TRACE("OMSSStoredObject::readFromStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data buffer", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT status = stream->Read(data, bytes, &bytesRead);
  checkStatus(status);
  ASSERT("IStream::Read() succeeded", SUCCEEDED(status));
}

  // @mfunc Write <p size> bytes from the buffer at address <p data>
  //        to <p stream>.
  //   @parm The stream on which to write.
  //   @parm The buffer to write.
  //   @parm The number of bytes to write.
void OMSSStoredObject::writeToStream(IStream* stream,
                                     void* data,
                                     OMUInt32 size)
{
  TRACE("OMSSStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  OMUInt32 bytesWritten;
  HRESULT status = stream->Write(data, size, &bytesWritten);
  checkStatus(status);
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
void OMSSStoredObject::writeToStream(IStream* stream,
                                      const OMByte* data,
                                      const OMUInt32 bytes,
                                      OMUInt32& bytesWritten)
{
  TRACE("OMSSStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

  HRESULT status = stream->Write(data, bytes, &bytesWritten);
  checkStatus(status);
  ASSERT("IStream::Write() succeeded", SUCCEEDED(status));
}

  // @mfunc Read an OMUInt8 from <p stream> into <p i>.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt8.
void OMSSStoredObject::readUInt8FromStream(IStream* stream,
                                            OMUInt8& i)
{
  TRACE("OMSSStoredObject::readUInt8FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt8));
}

  // @mfunc Write an OMUInt8 from <p i> to <p stream>.
  //   @parm The stream to write to.
  //   @parm The OMUInt8 to write.
void OMSSStoredObject::writeUInt8ToStream(IStream* stream,
                                           OMUInt8& i)
{
  TRACE("OMSSStoredObject::writeUInt8ToStream");
  PRECONDITION("Valid stream", stream != 0);

  writeToStream(stream, &i, sizeof(OMUInt8));
}

  // @mfunc Read an OMUInt16 from <p stream> into <p i>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt16.
  //   @parm If true then reorder the bytes.
void OMSSStoredObject::readUInt16FromStream(IStream* stream,
                                             OMUInt16& i,
                                             bool reorderBytes)
{
  TRACE("OMSSStoredObject::readUInt16FromStream");
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
void OMSSStoredObject::writeUInt16ToStream(IStream* stream,
                                            OMUInt16& i,
                                            bool reorderBytes)
{
  TRACE("OMSSStoredObject::writeUInt16ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt16 si = i;
  if (reorderBytes) {
    reorderUInt16(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt16));
}

  // @mfunc Reorder the OMUInt16 <p i>.
  //   @parm The OMUInt16 to reorder.
void OMSSStoredObject::reorderUInt16(OMUInt16& i)
{
  TRACE("OMSSStoredObject::reorderUInt16");

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
void OMSSStoredObject::readUInt32FromStream(IStream* stream,
                                             OMUInt32& i,
                                             bool reorderBytes)
{
  TRACE("OMSSStoredObject::readUInt32FromStream");
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
void OMSSStoredObject::writeUInt32ToStream(IStream* stream,
                                            OMUInt32& i,
                                            bool reorderBytes)
{
  TRACE("OMSSStoredObject::writeUInt32ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt32 si = i;
  if (reorderBytes) {
    reorderUInt32(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt32));
}

  // @mfunc Reorder the OMUInt32 <p i>.
  //   @parm The OMUInt32 to reorder.
void OMSSStoredObject::reorderUInt32(OMUInt32& i)
{
  TRACE("OMSSStoredObject::reorderUInt32");

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
void OMSSStoredObject::readUInt64FromStream(IStream* stream,
                                             OMUInt64& i,
                                             bool reorderBytes)
{
  TRACE("OMSSStoredObject::readUInt64FromStream");
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
void OMSSStoredObject::writeUInt64ToStream(IStream* stream,
                                            OMUInt64& i,
                                            bool reorderBytes)
{
  TRACE("OMSSStoredObject::writeUInt64ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt64 si = i;
  if (reorderBytes) {
    reorderUInt64(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt64));
}

  // @mfunc Reorder the OMUInt64 <p i>.
  //   @parm The OMUInt64 to reorder.
void OMSSStoredObject::reorderUInt64(OMUInt64& i)
{
  TRACE("OMSSStoredObject::reorderUInt64");

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
void OMSSStoredObject::readUniqueObjectIdentificationFromStream(
                                              IStream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes)
{
  TRACE("OMSSStoredObject::readUniqueObjectIdentificationFromStream");
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
void OMSSStoredObject::writeUniqueObjectIdentificationToStream(
                                              IStream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes)
{
  TRACE("OMSSStoredObject::writeUniqueObjectIdentificationToStream");
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
void OMSSStoredObject::readUniqueMaterialIdentificationFromStream(
                                            IStream* stream,
                                            OMUniqueMaterialIdentification& id,
                                            bool reorderBytes)
{
  TRACE("OMSSStoredObject::readUniqueMaterialIdentificationFromStream");
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
void OMSSStoredObject::writeUniqueMaterialIdentificationToStream(
                                            IStream* stream,
                                            OMUniqueMaterialIdentification& id,
                                            bool reorderBytes)
{
  TRACE("OMSSStoredObject::writeUniqueMaterialIdentificationToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUniqueMaterialIdentification sid = id;
  if (reorderBytes) {
    reorderUniqueMaterialIdentification(sid);
  }
  writeToStream(stream, &sid, sizeof(OMUniqueMaterialIdentification));
}

  // @mfunc Reorder the OMUniqueObjectIdentification <p id>.
  //   @parm The OMUniqueObjectIdentification to reorder.
void OMSSStoredObject::reorderUniqueObjectIdentification(
                                              OMUniqueObjectIdentification& id)
{
  TRACE("OMSSStoredObject::reorderUniqueObjectIdentification");

  reorderUInt32(id.Data1);
  reorderUInt16(id.Data2);
  reorderUInt16(id.Data3);
  // no need to swap Data4
}

  // @mfunc Reorder the OMUniqueMaterialIdentification <p id>.
  //   @parm The OMUniqueMaterialIdentification to reorder.
void OMSSStoredObject::reorderUniqueMaterialIdentification(
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
OMUInt64 OMSSStoredObject::streamSize(IStream* stream) const
{
  TRACE("OMSSStoredObject::streamSize");
  PRECONDITION("Valid stream", stream != 0);

  STATSTG statstg;
  HRESULT status = stream->Stat(&statstg, STATFLAG_NONAME);
  checkStatus(status);
  ASSERT("IStream::Stat() succeeded", SUCCEEDED(status));
  OMUInt64 result = toOMUInt64(statstg.cbSize);
  return result;
}

  // @mfunc Set the size, in bytes, of <p stream>
  //   @parm An open stream.
  //   @parm The new size for the stream.
void OMSSStoredObject::streamSetSize(IStream* stream, const OMUInt64 newSize)
{
  TRACE("OMSSStoredObject::streamSetSize");

  ULARGE_INTEGER newStreamSize = fromOMUInt64(newSize);
  HRESULT status = stream->SetSize(newStreamSize);
  checkStatus(status);
  ASSERT("IStream::SetSize() succeeded", SUCCEEDED(status));
}

  // @mfunc The current position for <f readFromStream()> and
  //        <f writeToStream()>, as an offset in bytes from the begining
  //        of the data stream.
  //   @rdesc The current position for <f readFromStream()> and
  //          <f writeToStream()>, as an offset in bytes from the begining
  //          of the data stream.
  //   @this const
OMUInt64 OMSSStoredObject::streamPosition(IStream* stream) const
{
  TRACE("OMSSStoredObject::streamPosition");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt64 result;
  LARGE_INTEGER zero = {0, 0};
  ULARGE_INTEGER position;
  HRESULT status = stream->Seek(zero, STREAM_SEEK_CUR, &position);
  checkStatus(status);
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
void OMSSStoredObject::streamSetPosition(IStream* stream,
                                          const OMUInt64 offset)
{
  TRACE("OMSSStoredObject::streamSetPosition");
  PRECONDITION("Valid stream", stream != 0);

  ULARGE_INTEGER newPosition = fromOMUInt64(offset);
  ULARGE_INTEGER oldPosition;
  LARGE_INTEGER position;
  memcpy(&position, &newPosition, sizeof(LARGE_INTEGER));
  HRESULT status = stream->Seek(position, STREAM_SEEK_SET, &oldPosition);
  checkStatus(status);
  ASSERT("IStream::Seek() succeeded", SUCCEEDED(status));
}

  // @mfunc Close <p stream>.
  //   @parm The stream to close.
void OMSSStoredObject::closeStream(IStream*& stream)
{
  TRACE("OMSSStoredObject::closeStream");
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

void OMSSStoredObject::writeName(const wchar_t* name)
{
  TRACE("OMSSStoredObject::writeName");

  size_t count = lengthOfWideString(name) + 1;
  ASSERT("String not too long", count <= OMUINT16_MAX);
  OMUInt16 characterCount = static_cast<OMUInt16>(count);
  OMCharacter* buffer = new OMCharacter[characterCount];
  ASSERT("Valid heap pointer", buffer != 0);
  externalizeString(name, buffer, characterCount);
  if (_reorderBytes) {
    reorderString(buffer, characterCount);
  }
  OMPropertySize byteCount = characterCount * sizeof(OMCharacter);
  writeToStream(_properties, buffer, byteCount);
  delete [] buffer;
}

  // @mfunc The persisted value of <p property> is its name.
  //        Write the property name and enter it into the property index.
  //   @parm The property.
  //   @parm TBS
  //   @this const
void OMSSStoredObject::saveName(const OMProperty& property,
                                 const wchar_t* name)
{
  TRACE("OMSSStoredObject::saveName");

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = property.storedForm();
  writeName(propertyId, storedForm, name);
}

  // @mfunc The persisted value of <p property> is its name.
  //        Read (and check) the property name.
  //   @parm The property.
  //   @parm The expected property name.
  //   @parm The (expected) size of the property name.
void OMSSStoredObject::restoreName(OMProperty& property,
                                    const wchar_t* ANAME(name),
                                    OMPropertySize size)
{
  TRACE("OMSSStoredObject::restoreName");

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

void OMSSStoredObject::writeName(OMPropertyId pid,
                                  OMStoredForm storedForm,
                                  const wchar_t* name)
{
  TRACE("OMSSStoredObject::writeName");

  size_t count = lengthOfWideString(name) + 1;
  ASSERT("String not too long", count <= OMUINT16_MAX);
  OMUInt16 characterCount = static_cast<OMUInt16>(count);
  OMCharacter* buffer = new OMCharacter[characterCount];
  ASSERT("Valid heap pointer", buffer != 0);
  externalizeString(name, buffer, characterCount);
  if (_reorderBytes) {
    reorderString(buffer, characterCount);
  }
  OMPropertySize byteCount = characterCount * sizeof(OMCharacter);
  write(pid, storedForm, buffer, byteCount);
  delete [] buffer;
}

wchar_t* OMSSStoredObject::readName(OMPropertyId pid,
                                     OMStoredForm storedForm,
                                     OMPropertySize size)
{
  TRACE("OMSSStoredObject::readName");

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

void OMSSStoredObject::reorderString(OMCharacter* string,
                                      size_t characterCount)
{
  TRACE("OMSSStoredObject::reorderString");

  for (size_t i = 0; i < characterCount; i++) {
    reorderUInt16(string[i]);
  }
}

void OMSSStoredObject::internalizeString(OMCharacter* externalString,
                                          wchar_t* internalString,
                                          size_t characterCount)
{
  TRACE("OMSSStoredObject::internalizeString");

  for (size_t i = 0; i < characterCount; i++) {
    internalString[i] = externalString[i];
  }
}

void OMSSStoredObject::externalizeString(const wchar_t* internalString,
                                          OMCharacter* externalString,
                                          size_t characterCount)
{
  TRACE("OMSSStoredObject::externalizeString");

  for (size_t i = 0; i < characterCount; i++) {
    externalString[i] = internalString[i];
  }
}

void OMSSStoredObject::reorderUInt16Array(OMUInt16* array,
                                           size_t elementCount)
{
  TRACE("OMSSStoredObject::reorderUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    reorderUInt16(array[i]);
  }
}

void OMSSStoredObject::internalizeUInt16Array(OMUInt16* externalArray,
                                               OMUInt16* internalArray,
                                               size_t elementCount)
{
  TRACE("OMSSStoredObject::internalizeUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    internalArray[i] = externalArray[i];
  }
}

void OMSSStoredObject::externalizeUInt16Array(const OMUInt16* internalArray,
                                               OMUInt16* externalArray,
                                               size_t elementCount)
{
  TRACE("OMSSStoredObject::externalizeUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    externalArray[i] = internalArray[i];
  }
}

void OMSSStoredObject::create(const OMByteOrder byteOrder)
{
  TRACE("OMSSStoredObject::create");
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
#if defined(OM_BUFFERED_STREAMS)
  _properties = createBufferedStream(_storage, propertyStreamName);
#else
  _properties = createStream(_storage, propertyStreamName);
#endif
  _open = true;
}

void OMSSStoredObject::open(const OMFile::OMAccessMode mode)
{
  TRACE("OMSSStoredObject::open");
  PRECONDITION("Not already open", !_open);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  _mode = mode;
#if defined(OM_DEFER_OPEN)
#else
#if defined(OM_BUFFERED_STREAMS)
  _properties = openBufferedStream(_storage, propertyStreamName);
#else
  _properties = openStream(_storage, propertyStreamName);
#endif
  _open = true;
#endif
}

void OMSSStoredObject::save(OMStoredPropertySetIndex* index)
{
  TRACE("OMSSStoredObject::save(OMStoredPropertySetIndex*)");
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
  OMUInt16 context = 0;
  for (OMUInt16 i = 0; i < entries; i++) {
    index->iterate(context, propertyId, type, offset, length);
    writeUInt16ToStream(_properties, propertyId, _reorderBytes);
    writeUInt16ToStream(_properties, type, _reorderBytes);
    writeUInt16ToStream(_properties, length, _reorderBytes);
  }

  streamSetPosition(_properties, 0);
  POSTCONDITION("At start of index stream", streamPosition(_properties) == 0);
}

OMStoredPropertySetIndex* OMSSStoredObject::restore(void)
{
  TRACE("OMSSStoredObject::restore");
#if defined(OM_DEFER_OPEN)
  PRECONDITION("Not already open", !_open);
  PRECONDITION("Not already open", _properties == 0);
#if defined(OM_BUFFERED_STREAMS)
  _properties = openBufferedStream(_storage, propertyStreamName);
#else
  _properties = openStream(_storage, propertyStreamName);
#endif
  _open = true;
#else
  PRECONDITION("Already open", _open);
#endif
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
wchar_t* OMSSStoredObject::collectionIndexStreamName(
                                                 const wchar_t* collectionName)
{
  TRACE("OMSSStoredObject::collectionIndexStreamName");
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

IStream* OMSSStoredObject::createStream(IStorage* storage,
                                         const wchar_t* streamName)
{
  TRACE("OMSSStoredObject::createStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  DWORD mode = STGM_DIRECT | STGM_READWRITE |
               STGM_SHARE_EXCLUSIVE  | STGM_CREATE;

  IStream* stream = 0;
  SSCHAR omStreamName[FILENAME_MAX];
  convert(omStreamName, FILENAME_MAX, streamName);

  HRESULT status = storage->CreateStream(
    omStreamName,
    mode,
    0,
    0,
    &stream);
  checkStatus(status);
  ASSERT("IStorage::CreateStream() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStreamCount();
#endif

  return stream;
}

IStream* OMSSStoredObject::openStream(IStorage* storage,
                                       const wchar_t* streamName)
{
  TRACE("OMSSStoredObject::openStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));
  PRECONDITION("Valid mode", (_mode == OMFile::modifyMode) ||
                             (_mode == OMFile::readOnlyMode));

  DWORD mode;
  if (_mode == OMFile::modifyMode) {
    mode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else {
    mode = STGM_DIRECT | STGM_READ      | STGM_SHARE_EXCLUSIVE;
  }

  IStream* stream = 0;
  SSCHAR omStreamName[FILENAME_MAX];
  convert(omStreamName, FILENAME_MAX, streamName);

  HRESULT status = storage->OpenStream(
    omStreamName,
    0,
    mode,
    0,
    &stream);
  checkStatus(status);
  ASSERT("IStorage::OpenStream() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStreamCount();
#endif

  return stream;

}

IStream*
OMSSStoredObject::createBufferedStream(IStorage* storage,
                                        const wchar_t* streamName)
{
  TRACE("OMSSStoredObject::createBufferedStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  IStream* iStream = createStream(storage, streamName);
  ASSERT("Valid IStream", iStream != 0);

  IStream* stream = new OMBufferedIStream(iStream, 1024);
  ASSERT("Valid heap pointer", stream != 0);

  return stream;
}

IStream*
OMSSStoredObject::openBufferedStream(IStorage* storage,
                                      const wchar_t* streamName)
{
  TRACE("OMSSStoredObject::openBufferedStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));

  IStream* iStream = openStream(storage, streamName);
  ASSERT("Valid IStream", iStream != 0);

  IStream* stream = new OMBufferedIStream(iStream, 1024);
  ASSERT("Valid heap pointer", stream != 0);

  return stream;
}

IStorage* OMSSStoredObject::createStorage(IStorage* storage,
                                           const wchar_t* storageName)
{
  TRACE("createStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validWideString(storageName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  DWORD mode = STGM_DIRECT | STGM_READWRITE |
               STGM_SHARE_EXCLUSIVE  | STGM_CREATE;

  IStorage* newStorage = 0;
  SSCHAR omStorageName[FILENAME_MAX];
  convert(omStorageName, FILENAME_MAX, storageName);

  HRESULT status = storage->CreateStorage(
    omStorageName,
    mode,
    0,
    0,
    &newStorage);
  checkStatus(status);
  ASSERT("IStorage::CreateStorage() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  return newStorage;
}

IStorage* OMSSStoredObject::openStorage(IStorage* storage,
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
  } else {
    openMode = STGM_DIRECT | STGM_READ      | STGM_SHARE_EXCLUSIVE;
  }

  IStorage* newStorage = 0;
  SSCHAR omStorageName[FILENAME_MAX];
  convert(omStorageName, FILENAME_MAX, storageName);

  HRESULT status = storage->OpenStorage(
    omStorageName,
    0,
    openMode,
    0,
    0,
    &newStorage);
  checkStatus(status);
  ASSERT("IStorage::OpenStorage() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  return newStorage;
}

void OMSSStoredObject::closeStorage(IStorage*& storage)
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

void OMSSStoredObject::setClass(IStorage* storage, const OMClassId& cid)
{
  TRACE("OMSSStoredObject::setClass");
  PRECONDITION("Valid storage", storage != 0);

  GUID g;
  memcpy(&g, &cid, sizeof(GUID));
  HRESULT status = storage->SetClass(g);
  checkStatus(status);
  ASSERT("IStorage::SetClass() succeeded", SUCCEEDED(status));
}

void OMSSStoredObject::getClass(IStorage* storage, OMClassId& cid)
{
  TRACE("OMSSStoredObject::getClass");
  PRECONDITION("Valid storage", storage != 0);

  STATSTG statstg;
  HRESULT status = storage->Stat(&statstg, STATFLAG_NONAME);
  checkStatus(status);
  ASSERT("IStorage::Stat() succeeded", SUCCEEDED(status));
  memcpy(&cid, &statstg.clsid, sizeof(OMClassId));
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

#if defined(OM_DEBUG)
  size_t status =
#endif
  wcstombs(cName, name, length);
  ASSERT("wcstombs() succeeded", status != (size_t)-1);
}

#endif

static void checkStatus(HRESULT status)
{
  TRACE("checkStatus");

  if (FAILED(status)) {
    throw OMException(status);
  }
}

#if defined(OM_WINDOWS_SS)

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

#endif // !OM_NO_STRUCTURED_STORAGE

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

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMSSStoredObject
// @author Oliver Morgan | wol | Metaglue Corporation | OMSSSStoredObject

// define OM_PERMIT_ZERO_LENGTH to eliminate debug check for zero-length properties on read

#include "OMSSSStoredObject.h"

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

#include "OMSSSStructuredStorage.h"
#include "OMSSSStoredStream.h"

#include "OMType.h"
#include "OMUniqueObjectIdentType.h"

//TODO replace OMBufferedIStream with plain old buffer if needed for SchemaSoft
//#include "OMBufferedIStream.h"

#include <stdlib.h>

typedef int sresult;
#define SUCCEEDED(result) (SSTG_OK==result)
static void checkStatus(sresult status);

// the following consts duplicate those in AAFFileKinds.h
// should be abstracted into OMSignatures.h

// AAF files encoded as structured storage (binary).
// the signature actually stored in all AAF SS (512) files
// note this is not a properly-formed SMPTE label, but this is legacy
const OMUniqueObjectIdentification aafSignature_Aaf_SSBinary = 
{0x42464141, 0x000d, 0x4d4f, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};

// the signature actually stored in all AAF SS (4096) files
// TO BE CONFIRMED
// [060e2b34.0401.0105.0d010101.03020000]
const OMUniqueObjectIdentification aafSignature_Aaf_SSBin_4K = 
{0x0101010d, 0x0203, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x05}};


const OMVersion currentVersion = 32;

const size_t indexHeaderSize = sizeof(OMByteOrder) +
                               sizeof(OMVersion) +
                               sizeof(OMPropertyCount);

const size_t indexEntrySize  = sizeof(OMPropertyId) +
                               sizeof(OMStoredForm) +
                               sizeof(OMPropertySize);

// Determine whether or not UNICODE versions of the APIs are in use.
// Always true for SchemaSoft
#define OM_UNICODE_APIS

// SSCHAR is used in client code for all character (string) arguments
// to functions whose prototype changes when UNICODE is defined.
//
#if defined(OM_UNICODE_APIS)
#define SSCHAR wchar_t
#else
#define SSCHAR char
#endif

#if defined(OM_UNICODE_APIS)
static void convert(wchar_t* wcName, size_t length, const wchar_t* name);
#else
static void convert(char* cName, size_t length, const wchar_t* name);
#endif

static void checkStatus(sresult status);

#if defined(OM_DEBUG)
void OMSSSStoredObject::incrementOpenStreamCount(void)
{
  TRACE("OMSSSStoredObject::incrementOpenStreamCount");
  _openStreams = _openStreams + 1;
  if (_openStreams > _maxOpenStreams) {
    _maxOpenStreams = _openStreams;
  }
}

void OMSSSStoredObject::decrementOpenStreamCount(void)
{
  TRACE("OMSSSStoredObject::decrementOpenStreamCount");
  PRECONDITION("Non zero count", _openStreams != 0);
  _openStreams = _openStreams - 1;
}

void OMSSSStoredObject::incrementOpenStorageCount(void)
{
  TRACE("OMSSSStoredObject::incrementOpenStorageCount");
  _openStorages = _openStorages + 1;
  if (_openStorages > _maxOpenStorages) {
    _maxOpenStorages = _openStorages;
  }
}

void OMSSSStoredObject::decrementOpenStorageCount(void)
{
  TRACE("OMSSSStoredObject::decrementOpenStorageCount");
  PRECONDITION("Non zero count", _openStorages != 0);
  _openStorages = _openStorages - 1;
}

size_t OMSSSStoredObject::_openStorages = 0;
size_t OMSSSStoredObject::_maxOpenStorages = 0;
size_t OMSSSStoredObject::_openStreams = 0;
size_t OMSSSStoredObject::_maxOpenStreams = 0;
#endif

  // @mfunc Perform SchemaSoft Structured Storage specific initialization.
void OMSSSStoredObject::initialize(void)
{
  TRACE("OMSSSStoredObject::initialize");
}

  // @mfunc Perform SchemaSoft Structured Storage specific finalization.
void OMSSSStoredObject::finalize(void)
{
  TRACE("OMSSSStoredObject::finalize");
}

  // @mfunc Open the root <c OMSSSStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMSSSStoredObject> representing the root object in
  //          the disk file.
OMSSSStoredObject* OMSSSStoredObject::openRead(const wchar_t* fileName)
{
  TRACE("OMSSSStoredObject::openRead");
  PRECONDITION("Valid file name", validWideString(fileName));

  OMSSSStoredObject* newStore = OMSSSStoredObject::openFile(
                                                         fileName,
                                                         OMFile::readOnlyMode);
  newStore->open(OMFile::readOnlyMode);

  return newStore;
}

  // @mfunc Open the root <c OMSSSStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMSSSStoredObject> representing the root object in
  //          the disk file.
OMSSSStoredObject* OMSSSStoredObject::openModify(const wchar_t* fileName)
{
  TRACE("OMSSSStoredObject::openModify");
  PRECONDITION("Valid file name", validWideString(fileName));

  OMSSSStoredObject* newStore = OMSSSStoredObject::openFile(
                                                           fileName,
                                                           OMFile::modifyMode);
  newStore->open(OMFile::modifyMode);

  return newStore;
}

  // @mfunc Create a new root <c OMSSSStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMSSSStoredObject> representing the root object in
  //          the disk file.
OMSSSStoredObject* OMSSSStoredObject::createModify(const wchar_t* fileName,
                                                   const OMByteOrder byteOrder,
																									 const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSSStoredObject::createModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));

  OMSSSStoredObject* newStore = OMSSSStoredObject::createFile(fileName,signature);
  newStore->create(byteOrder);

  return newStore;
}

  // @mfunc Open the root <c OMSSSStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMSSSStoredObject> representing the root object.
OMSSSStoredObject* OMSSSStoredObject::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMSSSStoredObject::openRead");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible raw storage access mode", rawStorage->isReadable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());

  OMSSSStoredObject* newStore = OMSSSStoredObject::openFile(
                                                         rawStorage,
                                                         OMFile::readOnlyMode);
  newStore->open(OMFile::readOnlyMode);

  return newStore;
}

  // @mfunc Open the root <c OMSSSStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMSSSStoredObject> representing the root object.
OMSSSStoredObject* OMSSSStoredObject::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMSSSStoredObject::openModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable() &&
                                         rawStorage->isExtendible());

  OMSSSStoredObject* newStore = OMSSSStoredObject::openFile(
                                                           rawStorage,
                                                           OMFile::modifyMode);
  newStore->open(OMFile::modifyMode);

  return newStore;
}

  // @mfunc Create a new root <c OMSSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMSSSStoredObject> representing the root object.
OMSSSStoredObject* OMSSSStoredObject::createModify(OMRawStorage* rawStorage,
                                                   const OMByteOrder byteOrder,
																									 const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSSStoredObject::createModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable() &&
                                         rawStorage->isExtendible());

  OMSSSStoredObject* newStore = OMSSSStoredObject::createFile(rawStorage,signature);
  newStore->create(byteOrder);

  return newStore;
}

  // @mfunc Destructor.
OMSSSStoredObject::~OMSSSStoredObject(void)
{
  TRACE("OMSSSStoredObject::~OMSSSStoredObject");
}

  // @mfunc Create a new <c OMSSSStoredObject>, named <p name>,
  //        contained by this <c OMSSSStoredObject>.
  //   @parm The name to be used for the new <c OMSSSStoredObject>.
  //   @rdesc A new <c OMSSSStoredObject> contained by this
  //          <c OMSSSStoredObject>.
OMStoredObject* OMSSSStoredObject::create(const wchar_t* name)
{
  TRACE("OMSSSStoredObject::create");
  PRECONDITION("Valid name", validWideString(name));

  Storage* newStorage = createStorage(_storage, name);
  OMSSSStoredObject* result = new OMSSSStoredObject(newStorage);
  ASSERT("Valid heap pointer", result != 0);
  result->create(_byteOrder);
  return result;
}

  // @mfunc Open an exsiting <c OMSSSStoredObject>, named <p name>,
  //        contained by this <c OMSSSStoredObject>.
  //   @parm The name of the existing <c OMSSSStoredObject>.
  //   @rdesc The existing <c OMSSSStoredObject> contained by this
  //          <c OMSSSStoredObject>.
OMStoredObject* OMSSSStoredObject::open(const wchar_t* name)
{
  TRACE("OMSSSStoredObject::open");
  PRECONDITION("Valid name", validWideString(name));

  Storage* newStorage = openStorage(_storage, name, _mode);
  OMSSSStoredObject* result = new OMSSSStoredObject(newStorage);
  ASSERT("Valid heap pointer", result != 0);
  result->open(_mode);
  return result;
}

  // @mfunc Close this <c OMSSSStoredObject>.
void OMSSSStoredObject::close(void)
{
  TRACE("OMSSSStoredObject::close");

  if (_open) {

    closeStream(_properties);

    delete _index;
    _index = 0;

		if( _root == 0) 
			closeStorage(_storage);
		else 
			closeStorage(_root);

    _open = false;
  }
}

OMByteOrder OMSSSStoredObject::byteOrder(void) const
{
  TRACE("OMSSSStoredObject::byteOrder");

  return _byteOrder;
}

void OMSSSStoredObject::save(OMFile& file)
{
  TRACE("OMSSSStoredObject::save(OMFile)");
  file.root()->save();
  save(file.referencedProperties());
}

void OMSSSStoredObject::save(OMStorable& object)
{
  TRACE("OMSSSStoredObject::save(OMStorable)");
  save(object.classId());
  save(*object.propertySet());
}

  // @mfunc Save the <c OMStoredObjectIdentification> <p id>
  //        in this <c OMSSSStoredObject>.
  //   @parm The <c OMStoredObjectIdentification> of this
  //         <c OMSSSStoredObject>.
void OMSSSStoredObject::save(const OMStoredObjectIdentification& id)
{
  TRACE("OMSSSStoredObject::save");

  setClass(_storage, id);
}

static const wchar_t* const propertyStreamName = L"properties";

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMSSSStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMSSSStoredObject::save(const OMPropertySet& properties)
{
  TRACE("OMSSSStoredObject::save(OMPropertySet)");

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

#ifdef _DEBUG
	// to permit verification of SSS behavior
	OMUInt64 wol = streamPosition(_properties);
	OMUInt64 wol2 = _offset;
	if( wol || wol2 )
	{
		size_t c = properties.count();
	}
#endif
		
  PRECONDITION("At start of value stream", _offset == 0);

  size_t count = properties.countPresent();
  delete _index;
  _index = 0; // for BoundsChecker
  _index = new OMStoredPropertySetIndex(count);
  ASSERT("Valid heap pointer", _index != 0);
  size_t countPresent = properties.countPresent();
	//StgInfo * pSI;
  //sresult s =::getStreamInfo(_properties,&pSI);
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

	OMUInt64 endoffset = streamPosition(_properties);
	ASSERT("_offset is correct", _offset == endoffset);

#if defined(OM_VALIDATE_PROPERTYSETS)
  validate(&properties, _index);
#endif
  streamSetPosition(_properties, 0);
  save(_index);
  delete _index;
  _index = 0;
  _offset = 0;
  streamSetPosition(_properties, endoffset);
  /*POSTCONDITION("At start of properties stream",
                                             streamPosition(_properties) == 0);
  POSTCONDITION("At start of value stream", _offset == 0); */
}

  // @mfunc Save the <c OMSimpleProperty> <p property> in this
  //        <c OMSSSStoredObject>.
  //   @parm TBS
void OMSSSStoredObject::save(const OMSimpleProperty& property)
{
  TRACE("OMSSSStoredObject::save");

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
void OMSSSStoredObject::save(const OMStrongReference& singleton)
{
  TRACE("OMSSSStoredObject::save");

  // Write the index entry.
  //
  wchar_t* name = referenceName(singleton.name(), singleton.propertyId());
  saveName(singleton, name);
  delete [] name;

  singleton.reference().save();
}

  // @mfunc Save the <c OMStrongReferenceVector> <p vector> in this
  //        <c OMSSSStoredObject>.
  //   @parm The vector of strong references to save.
void OMSSSStoredObject::save(const OMStrongReferenceVector& vector)
{
  TRACE("OMSSSStoredObject::save");

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
  //        <c OMSSSStoredObject>.
  //   @parm The set of strong references to save.
void OMSSSStoredObject::save(const OMStrongReferenceSet& set)
{
  TRACE("OMSSSStoredObject::save");

  // create a set index
  //
  size_t count = 0;
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
  size_t position = 0;

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
void OMSSSStoredObject::save(const OMWeakReference& singleton)
{
  TRACE("OMSSSStoredObject::save");

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
void OMSSSStoredObject::save(const OMWeakReferenceVector& vector)
{
  TRACE("OMSSSStoredObject::save");

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
void OMSSSStoredObject::save(const OMWeakReferenceSet& set)
{
  TRACE("OMSSSStoredObject::save");

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
  //        <c OMSSSStoredObject>.
  //   @parm The table to save.
void OMSSSStoredObject::save(const OMPropertyTable* table)
{
  TRACE("OMSSSStoredObject::save");

  PRECONDITION("Valid property table", table != 0);

#if defined(OM_BUFFERED_STREAMS)
  Stream* stream = createBufferedStream(_storage, L"referenced properties");
#else
  Stream* stream = createStream(_storage, L"referenced properties");
#endif

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
  //        <c OMSSSStoredObject>.
  //   @parm The <c OMDataStream> to save.
void OMSSSStoredObject::save(const OMDataStream& stream)
{
  TRACE("OMSSSStoredObject::save");

  // Use the property name as the stream name
  //
  OMPropertyId propertyId = stream.propertyId();
  OMStoredForm storedForm = stream.storedForm();
  wchar_t* name = streamName(stream.name(), stream.propertyId());
  OMByteOrder byteOrder = stream.storedByteOrder();
  saveStream(propertyId, storedForm, name, byteOrder);
  delete [] name;
}

OMRootStorable* OMSSSStoredObject::restore(OMFile& file)
{
  TRACE("OMSSSStoredObject::restore(OMFile)");

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

  OMDictionary *metaDictionary = root->dictionary();
  ASSERT("Consistent dictionaries", metaDictionary == file.dictionary());
  root->setClassFactory(file.classFactory());

  file.setLoadMode(savedLoadMode);
  return root;
}

OMStorable*
OMSSSStoredObject::restoreObject(const OMStrongObjectReference& reference)
{
  TRACE("OMSSSStoredObject::restoreObject");

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
  //        of this <c OMSSSStoredObject> into <p id>.
  //   @parm The <c OMStoredObjectIdentification> of this
  //         <c OMSSSStoredObject>.
void OMSSSStoredObject::restore(OMStoredObjectIdentification& id)
{
  TRACE("OMSSSStoredObject::restore");

  getClass(_storage, id);
}

  // @mfunc Restore the <c OMPropertySet> <p properties> into
  //        this <c OMSSSStoredObject>.
  //   @parm The <c OMPropertySet> to restore.
void OMSSSStoredObject::restore(OMPropertySet& properties)
{
  TRACE("OMSSSStoredObject::restore");
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
  size_t context = 0;
  for (size_t i = 0; i < entries; i++) {
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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSSStoredObject::restore(OMSimpleProperty& property,
                                 size_t externalSize)
{
  TRACE("OMSSSStoredObject::restore");

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSSStoredObject::restore(OMStrongReference& singleton,
                                size_t externalSize)
{
  TRACE("OMSSSStoredObject::restore");

  wchar_t* name = referenceName(singleton.name(), singleton.propertyId());
  restoreName(singleton, name, externalSize);
  OMStrongObjectReference newReference(&singleton, name);
  delete [] name;
  singleton.reference() = newReference;
  singleton.reference().restore();
}

  // @mfunc Restore the <c OMStrongReferenceVector> <p vector> into this
  //        <c OMSSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSSStoredObject::restore(OMStrongReferenceVector& vector,
                                size_t externalSize)
{
  TRACE("OMSSSStoredObject::restore");

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSSStoredObject::restore(OMStrongReferenceSet& set,
                                size_t externalSize)
{
  TRACE("OMSSSStoredObject::restore");

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSSStoredObject::restore(OMWeakReference& singleton,
                                size_t ANAME(externalSize))
{
  TRACE("OMSSSStoredObject::restore");

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSSStoredObject::restore(OMWeakReferenceVector& vector,
                                size_t externalSize)
{
  TRACE("OMSSSStoredObject::restore");

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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSSStoredObject::restore(OMWeakReferenceSet& set,
                                size_t externalSize)
{
  TRACE("OMSSSStoredObject::restore");

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

  // @mfunc Restore the <c OMPropertyTable> in this <c OMSSSStoredObject>.
  //   @parm A pointer to the newly restored <c OMPropertyTable> by reference.
void OMSSSStoredObject::restore(OMPropertyTable*& table)
{
  TRACE("OMSSSStoredObject::restore");

#if defined(OM_BUFFERED_STREAMS)
  Stream* stream = openBufferedStream(_storage, L"referenced properties");
#else
  Stream* stream = openStream(_storage, L"referenced properties");
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
  //        <c OMSSSStoredObject>.
  //   @parm TBS
  //   @parm TBS
void OMSSSStoredObject::restore(OMDataStream& stream,
                                size_t ANAME(externalSize))
{
  TRACE("OMSSSStoredObject::restore");

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
  //        <p property> contained within this <c OMSSSStoredObject>.
  //   @parm TBS
  //   @rdesc TBS
OMStoredStream* OMSSSStoredObject::openStoredStream(
                                                  const OMDataStream& property)
{
  TRACE("OMSSSStoredObject::openStoredStream");

  wchar_t* sName = streamName(property.name(), property.propertyId());
  Stream* stream = openStream(sName);
  OMSSSStoredStream* result = new OMSSSStoredStream(stream);
  ASSERT("Valid heap pointer", result != 0);
  delete [] sName;

  return result;
}

  // @mfunc Create an <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMSSSStoredObject>.
  //   @parm TBS
  //   @rdesc TBS
OMStoredStream* OMSSSStoredObject::createStoredStream(
                                                  const OMDataStream& property)
{
  TRACE("OMSSSStoredObject::createStoredStream");

  wchar_t* sName = streamName(property.name(), property.propertyId());
  Stream* stream = createStream(sName);
  OMSSSStoredStream* result = new OMSSSStoredStream(stream);
  ASSERT("Valid heap pointer", result != 0);
  delete [] sName;
  return result;
}

  // @mfunc Constructor.
  //   @parm The Storage for the persistent representation of
  //         this <c OMSSSStoredObject>.
OMSSSStoredObject::OMSSSStoredObject(Storage* s)
: _storage(s), _root(0), _index(0), _properties(0),
  _offset(0), _open(false), _mode(OMFile::readOnlyMode),
  _byteOrder(hostByteOrder()), _reorderBytes(false)
{
  TRACE("OMSSSStoredObject::OMSSSStoredObject");
}

  // @mfunc Constructor.
  //   @parm The RootStorage for the persistent representation of
  //         this <c OMSSSStoredObject>.
OMSSSStoredObject::OMSSSStoredObject(RootStorage* s)
:  _root(s), _storage(0), _index(0), _properties(0),
  _offset(0), _open(false), _mode(OMFile::readOnlyMode),
  _byteOrder(hostByteOrder()), _reorderBytes(false)
{
  TRACE("OMSSSStoredObject::OMSSSStoredObject");

	Storage *storage = 0;
	sresult status = ::getStorageFromRoot( _root, &storage );
  checkStatus(status);

  ASSERT("getStorageFromRoot() succeeded", SUCCEEDED(status));
	_storage = storage;
}

  // @mfunc Check that the <c OMPropertySet> <p propertySet> is
  //        consistent with the <c OMStoredPropertySetIndex>
  //        propertySetIndex.
  //   @parm The <c OMPropertySet> to validate.
  //   @parm The <c OMStoredPropertySetIndex> to validate.
void OMSSSStoredObject::validate(
                        const OMPropertySet* propertySet,
                        const OMStoredPropertySetIndex* propertySetIndex) const
{
  TRACE("OMSSSStoredObject::validate");
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
  //         <c OMSSSStoredObject>, the vector is named <p vectorName>.
  //   @parm The <c OMStoredVectorIndex> to save.
  //   @parm The name of the vector.
void OMSSSStoredObject::save(const OMStoredVectorIndex* vector,
                             const wchar_t* vectorName)
{
  TRACE("OMSSSStoredObject::save");
  PRECONDITION("Valid vector", vector != 0);
  PRECONDITION("Valid vector name", validWideString(vectorName));

  // Calculate the stream name for the index.
  //
  wchar_t* vectorIndexName = collectionIndexStreamName(vectorName);

  // Create the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  Stream* vectorIndexStream = createBufferedStream(_storage, vectorIndexName);
#else
  Stream* vectorIndexStream = createStream(_storage, vectorIndexName);
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
  //         <c OMSSSStoredObject>, the set is named <p setName>.
  //   @parm The <c OMStoredSetIndex> to save.
  //   @parm The name of the set.
void OMSSSStoredObject::save(const OMStoredSetIndex* set,
                             const wchar_t* setName)
{
  TRACE("OMSSSStoredObject::save");
  PRECONDITION("Valid set", set != 0);
  PRECONDITION("Valid set name", validWideString(setName));

  // Calculate the stream name for the index.
  //
  wchar_t* setIndexName = collectionIndexStreamName(setName);

  // Create the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  Stream* setIndexStream = createBufferedStream(_storage, setIndexName);
#else
  Stream* setIndexStream = createStream(_storage, setIndexName);
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
void OMSSSStoredObject::save(OMPropertyId propertyId,
                             OMStoredForm storedForm,
                             const OMUniqueObjectIdentification& id,
                             OMPropertyTag tag,
                             OMPropertyId keyPropertyId)
{
  TRACE("OMSSSStoredObject::save");

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
void OMSSSStoredObject::save(const wchar_t* collectionName,
                             const OMUniqueObjectIdentification* index,
                             size_t count,
                             OMPropertyTag tag,
                             OMPropertyId keyPropertyId)
{
  TRACE("OMSSSStoredObject::save");

  PRECONDITION("Valid index", IMPLIES(count != 0, index!= 0));
  PRECONDITION("Valid index", IMPLIES(count == 0, index== 0));
  PRECONDITION("Valid collection name", validWideString(collectionName));

  // Calculate the stream name for the index.
  //
  wchar_t* indexName = collectionIndexStreamName(collectionName);

  // Create the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  Stream* indexStream = createBufferedStream(_storage, indexName);
#else
  Stream* indexStream = createStream(_storage, indexName);
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

void OMSSSStoredObject::saveStream(OMPropertyId pid,
                                   OMStoredForm storedForm,
                                   const wchar_t* name,
                                   OMByteOrder byteOrder)
{
  TRACE("OMSSSStoredObject::saveStream");

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
  //        <c OMSSSStoredObject>.
  //   @parm The name of the vector.
  //   @rdesc The newly restored <c OMStoredVectorIndex>.
void OMSSSStoredObject::restore(OMStoredVectorIndex*& vector,
                                const wchar_t* vectorName)
{
  TRACE("OMSSSStoredObject::restore");
  PRECONDITION("Valid vector name", validWideString(vectorName));

  // Calculate the stream name for the index.
  //
  wchar_t* vectorIndexName = collectionIndexStreamName(vectorName);

  // Open the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  Stream* vectorIndexStream = openBufferedStream(_storage, vectorIndexName);
#else
  Stream* vectorIndexStream = openStream(_storage, vectorIndexName);
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
  //        <c OMSSSStoredObject>.
  //   @parm The name of the set.
  //   @rdesc The newly restored <c OMStoredSetIndex>.
void OMSSSStoredObject::restore(OMStoredSetIndex*& set,
                                const wchar_t* setName)
{
  TRACE("OMSSSStoredObject::restore");
  PRECONDITION("Valid set name", validWideString(setName));

  // Calculate the stream name for the index.
  //
  wchar_t* setIndexName = collectionIndexStreamName(setName);

  // Open the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  Stream* setIndexStream = openBufferedStream(_storage, setIndexName);
#else
  Stream* setIndexStream = openStream(_storage, setIndexName);
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
void OMSSSStoredObject::restore(OMPropertyId propertyId,
                                OMStoredForm storedForm,
                                OMUniqueObjectIdentification& id,
                                OMPropertyTag& tag,
                                OMPropertyId& keyPropertyId)
{
  TRACE("OMSSSStoredObject::restore");

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
void OMSSSStoredObject::restore(const wchar_t* collectionName,
                                OMUniqueObjectIdentification*& index,
                                size_t &count,
                                OMPropertyTag& tag,
                                OMPropertyId& keyPropertyId)
{
  TRACE("OMSSSStoredObject::restore");

  // Calculate the stream name for the index.
  //
  wchar_t* indexName = collectionIndexStreamName(collectionName);

  // Open the stream.
  //
#if defined(OM_BUFFERED_STREAMS)
  Stream* indexStream = openBufferedStream(_storage, indexName);
#else
  Stream* indexStream = openStream(_storage, indexName);
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

void OMSSSStoredObject::restoreStream(OMPropertyId pid,
                                      OMStoredForm storedForm,
                                      size_t size,
                                      wchar_t** name,
                                      OMByteOrder* byteOrder)
{
  TRACE("OMSSSStoredObject::restoreStream");

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

  // @mfunc Write a property value to this <c OMSSSStoredObject>. The
  //        property value to be written occupies <p size> bytes at
  //        the address <p start>. The property id is <p propertyId>.
  //        The property type is <p type>.
  //   @parm The property id.
  //   @parm The property type.
  //   @parm The start address of the property value.
  //   @parm The size of the property value in bytes.
void OMSSSStoredObject::write(OMPropertyId propertyId,
                              OMStoredForm storedForm,
                              void* start,
                              size_t size)
{
  TRACE("OMSSSStoredObject::write");
  PRECONDITION("Valid data", start != 0);
  PRECONDITION("Valid size", size > 0);

  _index->insert(propertyId, storedForm, _offset, size);

  // Write property value.
  //
  writeToStream(_properties, start, size);
  _offset += size;
}

  // @mfunc Read a property value from this <c OMSSSStoredObject>.
  //        The property value is read into a buffer which occupies
  //        <p size> bytes at the address <p start>. The property id
  //        is <p propertyId>. The property type is <p type>.
  //   @parm OMPropertyId | propertyId | The property id.
  //   @parm int | type | The property type.
  //   @parm void* | start | The start address of the buffer to hold the
  //         property value.
  //   @parm size_t | size | The size of the buffer in bytes.
void OMSSSStoredObject::read(OMPropertyId ANAME(propertyId),
                             OMStoredForm ANAME(storedForm),
                             void* start,
                             size_t size)
{
  TRACE("OMSSSStoredObject::read");
  PRECONDITION("Valid data", start != 0);

#ifndef OM_PERMIT_ZERO_LENGTH
    PRECONDITION("Valid size", size > 0);
#endif

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
  //        <c OMSSSStoredObject>.
  //   @parm The name of the stream to open.
  //   @rdesc An open stream.
Stream* OMSSSStoredObject::openStream(const wchar_t* streamName)
{
  TRACE("OMSSSStoredObject::openStream");
  PRECONDITION("Valid stream name", validWideString(streamName));

  return openStream(_storage, streamName);
}

  // @mfunc Create a stream called <p streamName> contained within
  //        this <c OMSSSStoredObject>.
  //   @parm The name of the stream to create.
  //   @rdesc An open stream.
Stream* OMSSSStoredObject::createStream(const wchar_t* streamName)
{
  TRACE("OMSSSStoredObject::createStream");
  PRECONDITION("Valid stream name", validWideString(streamName));

  return createStream(_storage, streamName);
}

  // @mfunc Read <p size> bytes from <p stream> into the buffer at
  //        address <p data>.
  //   @parm The stream from which to read.
  //   @parm The buffer into which the bytes are read.
  //   @parm The number of bytes to read.
void OMSSSStoredObject::readFromStream(Stream* stream,
                                       void* data,
                                       size_t size)
{
  TRACE("OMSSSStoredObject::readFromStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data buffer", data != 0);

#ifndef OM_PERMIT_ZERO_LENGTH
    PRECONDITION("Valid size", size > 0);
#endif

  unsigned long bytesRead = size;
  sresult status = streamRead( stream, data, &bytesRead);
  checkStatus(status);
  ASSERT("SSLib ::streamRead() succeeded", SUCCEEDED(status));
  ASSERT("Successful read", bytesRead == size);
}

  // @mfunc Attempt to read <p bytes> bytes from <p stream> into
  //        the buffer at address <p data>. The actual number of
  //        bytes read is returned in <p bytesRead>.
  //   @parm The stream from which to read.
  //   @parm The buffer into which the bytes are read.
  //   @parm The number of bytes to write
  //   @parm The actual number of bytes read.
void OMSSSStoredObject::readFromStream(Stream* stream,
                                       OMByte* data,
                                       const OMUInt32 bytes,
                                       OMUInt32& bytesRead)
{
  TRACE("OMSSSStoredObject::readFromStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data buffer", data != 0);

#ifndef OM_PERMIT_ZERO_LENGTH
    PRECONDITION("Valid size", bytes > 0);
#endif

	bytesRead = bytes;
  sresult status = streamRead( stream, data, &bytesRead);
  checkStatus(status);
  ASSERT("Stream::Read() succeeded", SUCCEEDED(status));
}

  // @mfunc Write <p size> bytes from the buffer at address <p data>
  //        to <p stream>.
  //   @parm The stream on which to write.
  //   @parm The buffer to write.
  //   @parm The number of bytes to write.
void OMSSSStoredObject::writeToStream(Stream* stream, void* data, size_t size)
{
  TRACE("OMSSSStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", size > 0);

  unsigned long bytesWritten = size;
  sresult status = streamWrite( stream, data, &bytesWritten);
  checkStatus(status);
  ASSERT("Stream::Write() succeeded", SUCCEEDED(status));
  ASSERT("Successful write", bytesWritten == size);
}

  // @mfunc Attempt to write <p bytes> bytes from the buffer at
  //        address <p data> to <p stream>. The actual number of
  //        bytes written is returned in <p bytesWritten>.
  //   @parm The stream on which to write.
  //   @parm The buffer to write.
  //   @parm The number of bytes to write
  //   @parm The actual number of bytes written.
void OMSSSStoredObject::writeToStream(Stream* stream,
                                      const OMByte* data,
                                      const OMUInt32 bytes,
                                      OMUInt32& bytesWritten)
{
  TRACE("OMSSSStoredObject::writeToStream");
  PRECONDITION("Valid stream", stream != 0);
  PRECONDITION("Valid data", data != 0);
  PRECONDITION("Valid size", bytes > 0);

	bytesWritten = bytes;
  sresult status = streamWrite( stream, data,  &bytesWritten);
  checkStatus(status);
  ASSERT("Stream::Write() succeeded", SUCCEEDED(status));
}

  // @mfunc Read an OMUInt8 from <p stream> into <p i>.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt8.
void OMSSSStoredObject::readUInt8FromStream(Stream* stream,
                                            OMUInt8& i)
{
  TRACE("OMSSSStoredObject::readUInt8FromStream");
  PRECONDITION("Valid stream", stream != 0);

  readFromStream(stream, &i, sizeof(OMUInt8));
}

  // @mfunc Write an OMUInt8 from <p i> to <p stream>.
  //   @parm The stream to write to.
  //   @parm The OMUInt8 to write.
void OMSSSStoredObject::writeUInt8ToStream(Stream* stream,
                                           OMUInt8& i)
{
  TRACE("OMSSSStoredObject::writeUInt8ToStream");
  PRECONDITION("Valid stream", stream != 0);

  writeToStream(stream, &i, sizeof(OMUInt8));
}

  // @mfunc Read an OMUInt16 from <p stream> into <p i>. If
  //        <p reorderBytes> is true then the bytes are reordered.
  //   @parm The stream from which to read.
  //   @parm The resulting OMUInt16.
  //   @parm If true then reorder the bytes.
void OMSSSStoredObject::readUInt16FromStream(Stream* stream,
                                             OMUInt16& i,
                                             bool reorderBytes)
{
  TRACE("OMSSSStoredObject::readUInt16FromStream");
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
void OMSSSStoredObject::writeUInt16ToStream(Stream* stream,
                                            OMUInt16& i,
                                            bool reorderBytes)
{
  TRACE("OMSSSStoredObject::writeUInt16ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt16 si = i;
  if (reorderBytes) {
    reorderUInt16(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt16));
}

  // @mfunc Reorder the OMUInt16 <p i>.
  //   @parm The OMUInt16 to reorder.
void OMSSSStoredObject::reorderUInt16(OMUInt16& i)
{
  TRACE("OMSSSStoredObject::reorderUInt16");

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
void OMSSSStoredObject::readUInt32FromStream(Stream* stream,
                                             OMUInt32& i,
                                             bool reorderBytes)
{
  TRACE("OMSSSStoredObject::readUInt32FromStream");
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
void OMSSSStoredObject::writeUInt32ToStream(Stream* stream,
                                            OMUInt32& i,
                                            bool reorderBytes)
{
  TRACE("OMSSSStoredObject::writeUInt32ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt32 si = i;
  if (reorderBytes) {
    reorderUInt32(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt32));
}

  // @mfunc Reorder the OMUInt32 <p i>.
  //   @parm The OMUInt32 to reorder.
void OMSSSStoredObject::reorderUInt32(OMUInt32& i)
{
  TRACE("OMSSSStoredObject::reorderUInt32");

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
void OMSSSStoredObject::readUInt64FromStream(Stream* stream,
                                             OMUInt64& i,
                                             bool reorderBytes)
{
  TRACE("OMSSSStoredObject::readUInt64FromStream");
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
void OMSSSStoredObject::writeUInt64ToStream(Stream* stream,
                                            OMUInt64& i,
                                            bool reorderBytes)
{
  TRACE("OMSSSStoredObject::writeUInt64ToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt64 si = i;
  if (reorderBytes) {
    reorderUInt64(si);
  }
  writeToStream(stream, &si, sizeof(OMUInt64));
}

  // @mfunc Reorder the OMUInt64 <p i>.
  //   @parm The OMUInt64 to reorder.
void OMSSSStoredObject::reorderUInt64(OMUInt64& i)
{
  TRACE("OMSSSStoredObject::reorderUInt64");

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
void OMSSSStoredObject::readUniqueObjectIdentificationFromStream(
                                              Stream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes)
{
  TRACE("OMSSSStoredObject::readUniqueObjectIdentificationFromStream");
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
void OMSSSStoredObject::writeUniqueObjectIdentificationToStream(
                                              Stream* stream,
                                              OMUniqueObjectIdentification& id,
                                              bool reorderBytes)
{
  TRACE("OMSSSStoredObject::writeUniqueObjectIdentificationToStream");
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
void OMSSSStoredObject::readUniqueMaterialIdentificationFromStream(
                                            Stream* stream,
                                            OMUniqueMaterialIdentification& id,
                                            bool reorderBytes)
{
  TRACE("OMSSSStoredObject::readUniqueMaterialIdentificationFromStream");
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
void OMSSSStoredObject::writeUniqueMaterialIdentificationToStream(
                                            Stream* stream,
                                            OMUniqueMaterialIdentification& id,
                                            bool reorderBytes)
{
  TRACE("OMSSSStoredObject::writeUniqueMaterialIdentificationToStream");
  PRECONDITION("Valid stream", stream != 0);

  OMUniqueMaterialIdentification sid = id;
  if (reorderBytes) {
    reorderUniqueMaterialIdentification(sid);
  }
  writeToStream(stream, &sid, sizeof(OMUniqueMaterialIdentification));
}

  // @mfunc Reorder the OMUniqueObjectIdentification <p id>.
  //   @parm The OMUniqueObjectIdentification to reorder.
void OMSSSStoredObject::reorderUniqueObjectIdentification(
                                              OMUniqueObjectIdentification& id)
{
  TRACE("OMSSSStoredObject::reorderUniqueObjectIdentification");

  reorderUInt32(id.Data1);
  reorderUInt16(id.Data2);
  reorderUInt16(id.Data3);
  // no need to swap Data4
}

  // @mfunc Reorder the OMUniqueMaterialIdentification <p id>.
  //   @parm The OMUniqueMaterialIdentification to reorder.
void OMSSSStoredObject::reorderUniqueMaterialIdentification(
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
OMUInt64 OMSSSStoredObject::streamSize(Stream* stream) const
{
  TRACE("OMSSSStoredObject::streamSize");
  PRECONDITION("Valid stream", stream != 0);

  StgInfo* info;
  sresult status = getStreamInfo( stream, &info );
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  OMUInt64 result = info->size;

	status = freeInfo( &info, 1 );
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

	// return result;

	// apparently, info->size isn't updated after write()
	// these four lines get the actual end of stream in sz
	OMUInt64 p = streamPosition( stream );
	sresult ss = streamSeek64( stream, 0, STG_END );
	OMUInt64 sz = streamPosition( stream );
	ss = streamSeek64( stream, p, STG_START );
  return sz;
}


  // @mfunc Set the size, in bytes, of <p stream>
  //   @parm An open stream.
  //   @parm The new size for the stream.
void OMSSSStoredObject::streamSetSize(Stream* stream, const OMUInt64 newSize)
{
  TRACE("OMSSSStoredObject::streamSetSize");
  PRECONDITION("Valid stream", stream != 0);

	sresult status = streamResize( stream, newSize );
	/*pre64 
	// PRECONDITION("Valid Size", newsize<MAXINT32);
	OMUInt32 pos = (OMUInt32)newSize;
	pos -= 1;
	sresult status = streamSeek( stream,  pos, STG_START );

	OMUInt8 newTerm = 0;
	OMUInt32 count = 1;
  status = streamWrite( stream, &newTerm, &count );
	*/
}

  // @mfunc The current position for <f readFromStream()> and
  //        <f writeToStream()>, as an offset in bytes from the begining
  //        of the data stream.
  //   @rdesc The current position for <f readFromStream()> and
  //          <f writeToStream()>, as an offset in bytes from the begining
  //          of the data stream.
  //   @this const
OMUInt64 OMSSSStoredObject::streamPosition(Stream* stream) const
{
  TRACE("OMSSSStoredObject::streamPosition");
  PRECONDITION("Valid stream", stream != 0);

  OMUInt64 result;
	sresult status = streamGetPos64( stream, &result );

	/*pre64
	OMUInt32 pos;
  sresult status = streamGetPos( stream, &pos);

	StgInfo * pSI;
  sresult s = 0;
	s=::getStreamInfo(stream,&pSI);

  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  result = pos;
	*/

  return result;
}

  // @mfunc Set the current position for <f readFromStream()> and
  //        <f writeToStream()>, as an offset in bytes from the begining of
  //        the data stream.
  //   @parm The position to use for subsequent calls to readFromStream() and
  //         writeToStream() on this stream. The position is specified as an
  //         offset in bytes from the begining of the data stream.
  //   @this const
void OMSSSStoredObject::streamSetPosition(Stream* stream,
                                          const OMUInt64 offset)
{
  TRACE("OMSSSStoredObject::streamSetPosition");
  PRECONDITION("Valid stream", stream != 0);

	//pre64 OMUInt32 position = (OMUInt32)offset;
	//pre64 sresult status = streamSeek( stream,  position, STG_START );
	sresult status = streamSeek64( stream, offset, STG_START );

  checkStatus(status);

  ASSERT("streamSetPosition() succeeded", SUCCEEDED(status));
}

  // @mfunc Close <p stream>.
  //   @parm The stream to close.
void OMSSSStoredObject::closeStream(Stream*& stream)
{
  TRACE("OMSSSStoredObject::closeStream");
  PRECONDITION("Valid stream", stream != 0);

	sresult status = SSTG_OK;

	// save position for later test
	OMUInt64 p = streamPosition( stream );

	// seek to the end of the stream
	// SchemaSoft resizes the stream upon close
	status = streamSeek64( stream, 0, STG_END );

	// test position
	OMUInt64 sz = streamPosition( stream );

	if( p != sz )
		OMUInt64 err = sz-p;
		
#if defined(OM_DEBUG)
  status = ::closeStream( &stream );
  ASSERT("Reference count is 0.", status == 0);
#else
  ::closeStream( &stream );
#endif

#if defined(OM_DEBUG)
  decrementOpenStreamCount();
#endif
}


void OMSSSStoredObject::writeName(const wchar_t* name)
{
  TRACE("OMSSSStoredObject::writeName");

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
}

  // @mfunc The persisted value of <p property> is its name.
  //        Write the property name and enter it into the property index.
  //   @parm The property.
  //   @parm TBS
  //   @this const
void OMSSSStoredObject::saveName(const OMProperty& property,
                                 const wchar_t* name)
{
  TRACE("OMSSSStoredObject::saveName");

  OMPropertyId propertyId = property.propertyId();
  OMStoredForm storedForm = property.storedForm();
  writeName(propertyId, storedForm, name);
}

  // @mfunc The persisted value of <p property> is its name.
  //        Read (and check) the property name.
  //   @parm The property.
  //   @parm The expected property name.
  //   @parm The (expected) size of the property name.
void OMSSSStoredObject::restoreName(OMProperty& property,
                                    const wchar_t* ANAME(name),
                                    size_t size)
{
  TRACE("OMSSSStoredObject::restoreName");

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

void OMSSSStoredObject::writeName(OMPropertyId pid,
                                  OMStoredForm storedForm,
                                  const wchar_t* name)
{
  TRACE("OMSSSStoredObject::writeName");

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

wchar_t* OMSSSStoredObject::readName(OMPropertyId pid,
                                     OMStoredForm storedForm,
                                     size_t size)
{
  TRACE("OMSSSStoredObject::readName");

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

void OMSSSStoredObject::reorderString(OMCharacter* string,
                                      size_t characterCount)
{
  TRACE("OMSSSStoredObject::reorderString");

  for (size_t i = 0; i < characterCount; i++) {
    reorderUInt16(string[i]);
  }
}

void OMSSSStoredObject::internalizeString(OMCharacter* externalString,
                                          wchar_t* internalString,
                                          size_t characterCount)
{
  TRACE("OMSSSStoredObject::internalizeString");

  for (size_t i = 0; i < characterCount; i++) {
    internalString[i] = externalString[i];
  }
}

void OMSSSStoredObject::externalizeString(const wchar_t* internalString,
                                          OMCharacter* externalString,
                                          size_t characterCount)
{
  TRACE("OMSSSStoredObject::externalizeString");

  for (size_t i = 0; i < characterCount; i++) {
    externalString[i] = internalString[i];
  }
}

void OMSSSStoredObject::reorderUInt16Array(OMUInt16* array,
                                           size_t elementCount)
{
  TRACE("OMSSSStoredObject::reorderUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    reorderUInt16(array[i]);
  }
}

void OMSSSStoredObject::internalizeUInt16Array(OMUInt16* externalArray,
                                               OMUInt16* internalArray,
                                               size_t elementCount)
{
  TRACE("OMSSSStoredObject::internalizeUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    internalArray[i] = externalArray[i];
  }
}

void OMSSSStoredObject::externalizeUInt16Array(const OMUInt16* internalArray,
                                               OMUInt16* externalArray,
                                               size_t elementCount)
{
  TRACE("OMSSSStoredObject::externalizeUInt16Array");

  for (size_t i = 0; i < elementCount; i++) {
    externalArray[i] = internalArray[i];
  }
}

void OMSSSStoredObject::create(const OMByteOrder byteOrder)
{
  TRACE("OMSSSStoredObject::create");
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

void OMSSSStoredObject::open(const OMFile::OMAccessMode mode)
{
  TRACE("OMSSSStoredObject::open");
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

void OMSSSStoredObject::save(OMStoredPropertySetIndex* index)
{
  TRACE("OMSSSStoredObject::save(OMStoredPropertySetIndex*)");
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

OMStoredPropertySetIndex* OMSSSStoredObject::restore(void)
{
  TRACE("OMSSSStoredObject::restore");
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
wchar_t* OMSSSStoredObject::collectionIndexStreamName(
                                                 const wchar_t* collectionName)
{
  TRACE("OMSSSStoredObject::collectionIndexStreamName");
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

void OMSSSStoredObject::setClass(Storage* storage, const OMClassId& cid)
{
  TRACE("OMSSSStoredObject::setClass");
  PRECONDITION("Valid storage", storage != 0);

  char g[16];
  memcpy(&g, &cid, sizeof(g));

  sresult status = ::storageSetClass( storage, reinterpret_cast<const SSRW_GUID *>(g) );
  checkStatus(status);
  ASSERT("Storage::SetClass() succeeded", SUCCEEDED(status));
}

void OMSSSStoredObject::getClass(Storage* storage, OMClassId& cid)
{
  TRACE("OMSSSStoredObject::getClass");
  PRECONDITION("Valid storage", storage != 0);

  StgInfo* info;
  sresult status = getStorageInfo( storage, &info );
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));

  memcpy(&cid, &(info->clsid), sizeof(OMClassId));

	status = freeInfo( &info, 1 );
  checkStatus(status);
  ASSERT("Succeeded", SUCCEEDED(status));
}

OMSSSStoredObject* OMSSSStoredObject::openFile(const wchar_t* fileName,
                                               const OMFile::OMAccessMode mode)
{
  TRACE("OMSSSStoredObject::openFile");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  StgMode openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STG_RW ;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STG_READ ;
  }

  RootStorage* rootstorage = 0;

  sresult status = openStructuredStorageEx( fileName,
																						openMode,
																						&rootstorage);
  checkStatus(status);
  ASSERT("openStructuredStorage() succeeded", SUCCEEDED(status));

#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  OMSSSStoredObject* newStoredObject = new OMSSSStoredObject(rootstorage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

OMSSSStoredObject* OMSSSStoredObject::createFile(const wchar_t* fileName,
																								 const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSSStoredObject::createFile");
  PRECONDITION("Valid file name", validWideString(fileName));

	// choose sector size based on signature from factory
	unsigned long sectorSize=0;
	if( aafSignature_Aaf_SSBinary == signature ) sectorSize=512;
	else if( aafSignature_Aaf_SSBin_4K == signature ) sectorSize=4096;
	PRECONDITION("Valid Signature", sectorSize!=0 );

  RootStorage* rootstorage = 0;

  sresult status = createStructuredStorageEx( fileName,
																							STG_RW,
																							&rootstorage,
																							sectorSize);
  checkStatus(status);
  ASSERT("StgCreateDocfile() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  OMSSSStoredObject* newStoredObject = new OMSSSStoredObject(rootstorage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

OMSSSStoredObject* OMSSSStoredObject::openFile(OMRawStorage* rawStorage,
                                               const OMFile::OMAccessMode mode)
{
  TRACE("OMSSSStoredObject::openFile");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  StgMode openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STG_RW ;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STG_READ ;
  }

  RootStorage* rootstorage = 0;

  sresult status = SSTG_OK;


  status = openStructuredStorageInOMRawStorage( rawStorage, openMode, &rootstorage );

  checkStatus(status);
  ASSERT("openStructuredStorageInOMRawStorageEx() succeeded", SUCCEEDED(status));

#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  OMSSSStoredObject* newStoredObject = new OMSSSStoredObject(rootstorage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

OMSSSStoredObject* OMSSSStoredObject::createFile(OMRawStorage* rawStorage,
																								 const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSSStoredObject::createFile");
  PRECONDITION("Valid raw storage", rawStorage != 0);

	// choose sector size based on signature from factory
	unsigned long sectorSize=0;
	if( aafSignature_Aaf_SSBinary == signature ) sectorSize=512;
	else if( aafSignature_Aaf_SSBin_4K == signature ) sectorSize=4096;
	PRECONDITION("Valid Signature", sectorSize!=0 );

  RootStorage* rootstorage = 0;

	sresult status = SSTG_OK;

  status = createStructuredStorageInOMRawStorageEx( rawStorage,	STG_RW,	&rootstorage, sectorSize);

  checkStatus(status);
  ASSERT("createStructuredStorageInOMRawStorageEx() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  OMSSSStoredObject* newStoredObject = new OMSSSStoredObject(rootstorage);
  ASSERT("Valid heap pointer", newStoredObject != 0);

  return newStoredObject;
}

Stream* OMSSSStoredObject::createStream(Storage* storage,
                                         const wchar_t* streamName)
{
  TRACE("OMSSSStoredObject::createStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  Stream* stream = 0;
  SSCHAR omStreamName[256];
  convert(omStreamName, 256, streamName);

	// if( the stream already exists i.e. can be opened ) seek to zero

  sresult status = SSTG_OK;

	if( SSTG_OK==::openStream( storage, omStreamName, &stream) )
	{
		status = ::streamSeek( stream, 0L, STG_START );
		ASSERT("SSLib::CreateStream() reopen succeeded", SUCCEEDED(status));
	}
	else if( SSTG_OK==::createStream( storage, omStreamName, &stream) )
	{
		//Note that the assert below as currently wrtten will always succeed
		//ASSERT("SSLib::CreateStream() succeeded", SUCCEEDED(status));
	}

#if defined(OM_DEBUG)
	incrementOpenStreamCount();
#endif



	/* to avoid a SSS bug, fixed by SchemaSoft in 3.01 
	OMByte null = 0;
	OMUInt32 bytesWritten = 1;
	::streamWrite( stream, &null, &bytesWritten);
  ::closeStream( &stream );
  status = ::openStream( storage, omStreamName, &stream );
  checkStatus(status);
	status = ::streamSeek( stream, 0L, STG_START );
  checkStatus(status);
  ASSERT("SSLib::openStream() succeeded", SUCCEEDED(status)); */

  return stream;
}

Stream* OMSSSStoredObject::openStream(Storage* storage,
                                       const wchar_t* streamName)
{
  TRACE("OMSSSStoredObject::openStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));

  Stream* stream = 0;
  SSCHAR omStreamName[256];
  convert(omStreamName, 256, streamName);

  sresult status = ::openStream(
		storage,
    omStreamName,
    &stream);
  checkStatus(status);
	status = ::streamSeek( stream, 0L, STG_START );
  checkStatus(status);
  ASSERT("SSLib ::openStream() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStreamCount();
#endif

  return stream;

}

Stream*
OMSSSStoredObject::createBufferedStream(Storage* storage,
                                        const wchar_t* streamName)
{
  TRACE("OMSSSStoredObject::createBufferedStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  //TODO implement if needed for SchemaSoft
  ASSERT("Unimplemented code not reached", false);
  return 0;
	/*
  Stream* iStream = 0;
  SSCHAR omStreamName[256];
  convert(omStreamName, 256, streamName);

	sresult status = ::createStream(storage, omStreamName, &iStream);
  ASSERT("Valid Stream", iStream != 0);

  Stream* stream = new OMBufferedIStream(iStream, 1024);
  ASSERT("Valid heap pointer", stream != 0);

  return stream;
	*/
}

Stream*
OMSSSStoredObject::openBufferedStream(Storage* storage,
                                      const wchar_t* streamName)
{
  TRACE("OMSSSStoredObject::openBufferedStream");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid stream name", validWideString(streamName));

  //TODO implement if needed for SchemaSoft
  ASSERT("Unimplemented code not reached", false);
  return 0;
	/*
  Stream* iStream = 0;
  SSCHAR omStreamName[256];
  convert(omStreamName, 256, streamName);

	sresult status = ::openStream(storage, omStreamName, &iStream);
  ASSERT("Valid Stream", iStream != 0);

  Stream* stream = new OMBufferedIStream(iStream, 1024);
  ASSERT("Valid heap pointer", stream != 0);

  return stream;
	*/
}

Storage* OMSSSStoredObject::createStorage(Storage* storage,
                                           const wchar_t* storageName)
{
  TRACE("OMSSSStoredObject::createStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validWideString(storageName));
  PRECONDITION("Valid mode", _mode == OMFile::modifyMode);

  Storage* newStorage = 0;
  SSCHAR omStorageName[256];
  convert(omStorageName, 256, storageName);

  sresult status = SSTG_OK;

	// if the storage already exists, reopen
  if( SSTG_OK==::openStorage( storage, omStorageName, &newStorage) )
	{
		//next line always succeeds so removed
		//ASSERT("SSLib::CreateStorage() reopen succeeded", SUCCEEDED(status));
	}
	else if( SSTG_OK==::createStorage( storage, omStorageName, &newStorage) )
	{
		//ASSERT("SSLib::CreateStorage() succeeded", SUCCEEDED(status));
	}

#if defined(OM_DEBUG)
		 incrementOpenStorageCount();
#endif

  return newStorage;
}

Storage* OMSSSStoredObject::openStorage(Storage* storage,
                                         const wchar_t* storageName,
                                         const OMFile::OMAccessMode mode)
{
  TRACE("SOMSSSStoredObject::openStorage");
  PRECONDITION("Valid storage", storage != 0);
  PRECONDITION("Valid storage name", validWideString(storageName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  Storage* newStorage = 0;
  SSCHAR omStorageName[256];
  convert(omStorageName, 256, storageName);

  sresult status = ::openStorage(
		storage,
    omStorageName,
    &newStorage);
  checkStatus(status);
  ASSERT("SSLib ::openStorage() succeeded", SUCCEEDED(status));
#if defined(OM_DEBUG)
  incrementOpenStorageCount();
#endif

  return newStorage;
}

void OMSSSStoredObject::closeStorage(RootStorage*& root)
{
  TRACE("OMSSSStoredObject::closeStorage");
  PRECONDITION("Valid rootstorage", root != 0);
	ASSERT("Valid storage", _storage != 0);

#if defined(OM_DEBUG)
		sresult status = ::closeStructuredStorage(&_root);
		ASSERT("Reference count is 0.", status == 0);
#else
		::closeStructuredStorage(&_root);
#endif
#if defined(OM_DEBUG)
		decrementOpenStorageCount();
#endif
}

void OMSSSStoredObject::closeStorage(Storage*& storage)
{
  TRACE("OMSSSStoredObject::closeStorage");
  PRECONDITION("Valid storage", storage != 0);
	ASSERT("Is not rootstorage", _root == 0);

#if defined(OM_DEBUG)
		sresult status = ::closeStorage(&storage);
		ASSERT("Reference count is 0.", status == 0);
#else
		::closeStorage(&storage);
#endif
#if defined(OM_DEBUG)
		decrementOpenStorageCount();
#endif
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

static void checkStatus(sresult status)
{
  TRACE("checkStatus");

  if (SSTG_OK!=status) {
    throw OMException(status | 0x80000000L);
  }
}

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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMXFStorage

#include "OMMXFStorage.h"

#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMExceptions.h"
#include "OMUtilities.h"
#include "OMSet.h"
#include "OMSetIterator.h"
#include "OMIdentitySet.h"
#include "OMIdentitySetIter.h"
#include "OMType.h"
#include "OMUniqueObjectIdentType.h"
#include "OMDataStreamProperty.h"

#include "OMKLVStoredStream.h" // tjb - we shouldn't depend on these
#include "OMStorable.h"
#include "OMClassFactory.h"

//#define INSTANCEID_DEBUG 1
//#define OMONLY 1

  // @mfunc Constructor.
OMMXFStorage::OMMXFStorage(OMRawStorage* store)
  : OMWrappedRawStorage(store),
  _fixups(),
  _reorderBytes(false),
  _operationalPattern(nullOMKLVKey),
  _essenceContainerLabels(),
  _generation(nullOMUniqueObjectIdentification),
  _objectDirectoryOffset(0),
  _instanceIdToObject(0),
  _objectToInstanceId(0),
  _streamToSid(0),
  _sidToStream(0),
  _maxSid(0),
  _segmentMap(0),
  _segments(0),
  _fileSize(0),
  _primerOffset(0)
{
  TRACE("OMMXFStorage::OMMXFStorage");

  if (hostByteOrder() == bigEndian) {
    _reorderBytes = false;
  } else {
    _reorderBytes = true;
  }

  // Pre-allocate space for the header metadata
  _fileSize = preallocatedMetadataSize;
}

  // @mfunc Destructor.
OMMXFStorage::~OMMXFStorage(void)
{
  TRACE("OMMXFStorage::~OMMXFStorage");

  if (_instanceIdToObject != 0) {
    _instanceIdToObject->clear();
    delete _instanceIdToObject;
    _instanceIdToObject = 0;
  }

  if (_objectToInstanceId != 0) {
    _objectToInstanceId->clear();
    delete _objectToInstanceId;
    _objectToInstanceId = 0;
  }

  if (_streamToSid != 0) {
    _streamToSid->clear();
    delete _streamToSid;
    _streamToSid = 0;
  }

  if (_sidToStream != 0) {
    _sidToStream->clear();
    delete _sidToStream;
    _sidToStream = 0;
  }

  if (_segments != 0) {
//  _segments->clear();
    delete _segments;
    _segments = 0;
  }

  destroySegmentMap();
  destroyFixups();

  size_t count = _partitions.count();
  for (size_t i = 0; i < count; i++) {
    Partition* p = _partitions.valueAt(i);
    delete p;
  }
}

  // @mfunc Open this <c OMMXFStorage>.
void OMMXFStorage::open(void)
{
  TRACE("OMMXFStorage:open");

  // Leave one KAG block of space for the header
  // partition pack and following fill.
  _primerOffset = defaultKAGSize;
}

  // @mfunc Close this <c OMMXFStorage>.
void OMMXFStorage::close(void)
{
  TRACE("OMMXFStorage::close");

  fixup();

  size_t count = _partitions.count();
  Partition* footerPartition = _partitions.valueAt(count - 1);
  OMUInt64 footer = footerPartition->_address;
  OMUInt64 previous = 0;
  for (size_t i = 0; i < count; i++) {
    Partition* p = _partitions.valueAt(i);
    OMUInt64 address = p->_address;
    fixupReference(address + sizeof(OMKLVKey) + 8 + 1 + 16, previous);
    fixupReference(address + sizeof(OMKLVKey) + 8 + 1 + 24, footer);
    previous = address;
  }
  // Set the HeaderByteCount field of the header
  ASSERT("Valid primer offset", _primerOffset < bodyPartitionOffset);
  OMUInt64 metadataSize = bodyPartitionOffset - _primerOffset;
  fixupReference(0 + sizeof(OMKLVKey) + 8 + 1 + 32, metadataSize);

  writeRandomIndex();
}

  // @mfunc Set the operational pattern to <p pattern>.
  //   @parm The operational pattern.
void OMMXFStorage::setOperationalPattern(const OMKLVKey& pattern)
{
  TRACE("OMMXFStorage::setOperationalPattern");
  _operationalPattern = pattern;
}

  // @mfunc Get the operational pattern.
  //   @rdesc The operational pattern.
OMKLVKey OMMXFStorage::operationalPattern(void) const
{
  TRACE("OMMXFStorage::operationalPattern");
  return _operationalPattern;
}

  // @mfunc Add <p label> to the set of essence container labels.
  //   @parm The essence container label.
void OMMXFStorage::addEssenceContainerLabel(const OMKLVKey& label)
{
  TRACE("OMMXFStorage::addEssenceContainerLabel");
  PRECONDITION("Label not present", !containsEssenceContainerLabel(label));

  _essenceContainerLabels.insert(label);
}

  // @mfunc Is <p label> present in the set of essence container labels.
  //   @parm The essence container label.
  //   @rdesc <e bool.true> if <p label> is present,
  //          <e bool.false> otherwise.
bool OMMXFStorage::containsEssenceContainerLabel(const OMKLVKey& label) const
{
  TRACE("OMMXFStorage::containsEssenceContainerLabel");

  bool result;
  if (_essenceContainerLabels.contains(label)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Create an iterator over the set of essence container labels.
  //  @rdesc The newly created iterator.
OMMXFStorage::LabelSetIterator*
OMMXFStorage::essenceContainerLabels(void) const
{
  TRACE("OMMXFStorage::essenceContainerLabels");

  LabelSetIterator* result = new LabelSetIterator(_essenceContainerLabels,
                                                  OMBefore);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Set the file generation.
  //   @parm The file generation.
void
OMMXFStorage::setGeneration(const OMUniqueObjectIdentification& generation)
{
  TRACE("OMMXFStorage::setGeneration");
  _generation = generation;
}

OMUniqueObjectIdentification OMMXFStorage::generation(void) const
{
  TRACE("OMMXFStorage::generation");
  return _generation;
}

bool OMMXFStorage::isHeader(const OMKLVKey& k)
{
  bool result;
  if (memcmp(&OpenHeaderPartitionKey, &k, sizeof(k)) == 0) {
    result = true;
  } else if (memcmp(&OpenCompleteHeaderPartitionKey, &k, sizeof(k)) == 0) {
    result = true;
  } else if (memcmp(&ClosedHeaderPartitionKey, &k, sizeof(k)) == 0) {
    result = true;
  } else if (memcmp(&ClosedCompleteHeaderPartitionKey, &k, sizeof(k)) == 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool OMMXFStorage::isBody(const OMKLVKey& k)
{
  bool result;
  if (memcmp(&OpenBodyPartitionKey, &k, sizeof(k)) == 0) {
    result = true;
  } else if (memcmp(&OpenCompleteBodyPartitionKey, &k, sizeof(k)) == 0) {
    result = true;
  } else if (memcmp(&ClosedBodyPartitionKey, &k, sizeof(k)) == 0) {
    result = true;
  } else if (memcmp(&ClosedCompleteBodyPartitionKey, &k, sizeof(k)) == 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool OMMXFStorage::isFooter(const OMKLVKey& k)
{
  bool result;
  if (memcmp(&FooterKey, &k, sizeof(k)) == 0) {
    result = true;
  } else if (memcmp(&CompleteFooterKey, &k, sizeof(k)) == 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

void OMMXFStorage::writeHeaderPartition(OMUInt32 bodySID,
                                        OMUInt32 indexSID,
                                        OMUInt32 KAGSize)
{
  TRACE("OMMXFStorage::writeHeaderPartition");

  writePartition(ClosedCompleteHeaderPartitionKey, bodySID, indexSID, KAGSize);
  OMUInt64 currentPosition = position();
  fillAlignK(currentPosition, defaultKAGSize);
}

void OMMXFStorage::writeBodyPartition(OMUInt32 bodySID,
                                      OMUInt32 indexSID,
                                      OMUInt32 KAGSize)
{
  TRACE("OMMXFStorage::writeBodyPartition");

  writePartition(ClosedCompleteBodyPartitionKey, bodySID, indexSID, KAGSize);
  OMUInt64 currentPosition = position();
  fillAlignV(currentPosition, KAGSize);
}

void OMMXFStorage::writeFooterPartition(OMUInt32 indexSID,
                                        OMUInt32 KAGSize)
{
  TRACE("OMMXFStorage::writeFooterPartition");

  // Essence (body) not allowed in footer
  writePartition(CompleteFooterKey, 0, indexSID, KAGSize);
  OMUInt64 currentPosition = position();
  fillAlignV(currentPosition, KAGSize);
}

void OMMXFStorage::writePartition(const OMKLVKey& key,
                                  OMUInt32 bodySID,
                                  OMUInt32 indexSID,
                                  OMUInt32 KAGSize)
{
  TRACE("OMMXFStorage::writePartition");

  OMUInt64 currentPosition = position();
  size_t count = _partitions.count();
  size_t i;
  for (i = 0; i < count; i++) {
    Partition* p = _partitions.valueAt(i);
    if (p->_address > currentPosition) {
      break;
    }
  }
  Partition* newPartition = new Partition;
  ASSERT("Valid heap pointer", newPartition != 0);
  newPartition->_address = currentPosition;
  newPartition->_sid = bodySID;
  _partitions.insertAt(newPartition, i);

  OMUInt32 elementSize = sizeof(OMKLVKey);
  LabelSetIterator* iter = essenceContainerLabels();
  OMUInt32 elementCount = iter->count();

  writeKLVKey(key);
  OMUInt64 length = fixedPartitionSize + (elementCount * elementSize);
  writeKLVLength(length);
  OMUInt16 majorVersion = currentMajorVersion;
  write(majorVersion, _reorderBytes);
  OMUInt16 minorVersion = currentMinorVersion;
  write(minorVersion, _reorderBytes);
  write(KAGSize, _reorderBytes);
  OMUInt64 thisPartition = currentPosition;
  write(thisPartition, _reorderBytes);
  OMUInt64 previousPartition = 0;
  write(previousPartition, _reorderBytes);
  OMUInt64 footerPartition = 0;
  write(footerPartition, _reorderBytes);
  OMUInt64 headerByteCount = 0;
  write(headerByteCount, _reorderBytes);
  OMUInt64 indexByteCount = 0;
  write(indexByteCount, _reorderBytes);
  OMUInt32 is = indexSID;
  write(is, _reorderBytes);
  OMUInt64 bodyOffset = 0;
  write(bodyOffset, _reorderBytes);
  OMUInt32 b = bodySID;
  write(b, _reorderBytes);
  writeKLVKey(_operationalPattern);
  write(elementCount, _reorderBytes);
  write(elementSize, _reorderBytes);
  while (++(*iter)) {
    OMKLVKey label = iter->value();
    writeKLVKey(label);
  }
  delete iter;
}

void OMMXFStorage::writeRandomIndex(void)
{
  TRACE("OMMXFStorage::writeRandomIndex");

  OMUInt32 entrySize = sizeof(OMUInt32) + sizeof(OMUInt64);
  writeKLVKey(RandomIndexMetadataKey);
  OMUInt32 count = _partitions.count();
  OMUInt32 length = (count * entrySize) + sizeof(OMUInt32);
  writeKLVLength(length);
  for (size_t i = 0; i < count; i++) {
    Partition* p = _partitions.valueAt(i);
    write(p->_sid, _reorderBytes);
    write(p->_address, _reorderBytes);
  }
  OMUInt32 overallLength = length + sizeof(OMKLVKey) + sizeof(OMUInt64) + 1;
  write(overallLength, _reorderBytes);
}

void OMMXFStorage::readRandomIndex(void)
{
  TRACE("OMMXFStorage::readRandomIndex");
  OMUInt32 entrySize = sizeof(OMUInt32) + sizeof(OMUInt64);
  OMUInt64 length = readKLVLength();
  OMUInt64 entryCount = (length - sizeof(OMUInt32)) / entrySize;
  for (OMUInt32 i = 0; i < entryCount; i++) {
    OMUInt32 sid;
    read(sid, _reorderBytes);
    OMUInt64 offset;
    read(offset, _reorderBytes);
  }
  OMUInt32 overallLength;
  read(overallLength, _reorderBytes);
}

  // @mfunc Write a fill key, a BER encoded length and
  //        <p length> bytes of fill.
  //   @parm The number of bytes of fill to write.
void OMMXFStorage::writeKLVFill(const OMUInt64& length)
{
  TRACE("OMMXFStorage::writeKLVFill");

  writeKLVKey(fillKey);
  writeKLVLength(length);
  for (OMUInt64 i = 0; i < length; i++) {
    const OMByte fillPattern = 0;
    write(fillPattern);
  }
}

  // @cmember Write fill so that the next key is page aligned.
  //   @parm The current position.
  //   @parm The page/KAG size.
void OMMXFStorage::fillAlignK(const OMUInt64& currentPosition,
                              const OMUInt32& KAGSize)
{
  TRACE("OMMXFStorage::fillAlignK");
  PRECONDITION("Valid KAG sise", KAGSize > 0);

  // K fill, L fill
  OMUInt64 overhead = minimumFill;
  OMUInt64 position = currentPosition + overhead;
  OMUInt64 nextPage = (position / KAGSize) + 1;
  OMUInt64 totalSize = (nextPage * KAGSize) - currentPosition;
  ASSERT("Valid total size", totalSize >= overhead);
  OMUInt64 length = totalSize - overhead;
  writeKLVFill(length);
}

  // @mfunc Write fill so that the next value is page aligned.
  //   @parm The current position.
  //   @parm The page/KAG size.
void OMMXFStorage::fillAlignV(const OMUInt64& currentPosition,
                              const OMUInt32& KAGSize)
{
  TRACE("OMMXFStorage::fillAlignV");
  PRECONDITION("Valid KAG sise", KAGSize > 0);

  // K fill, L fill, K next, L next
  OMUInt64 overhead = minimumFill + minimumFill;
  OMUInt64 position = currentPosition + overhead;
  OMUInt64 nextPage = (position / KAGSize) + 1;
  OMUInt64 totalSize = (nextPage * KAGSize) - currentPosition;
  ASSERT("Valid total size", totalSize >= overhead);
  OMUInt64 length = totalSize - overhead;
  writeKLVFill(length);
}

void OMMXFStorage::write(const OMByte* bytes,
                         OMUInt32 byteCount,
                         OMUInt32& bytesWritten)
{
  TRACE("OMMXFStorage::write");
  OMWrappedRawStorage::write(bytes, byteCount, bytesWritten);
}

void OMMXFStorage::write(const OMUInt8& i)
{
  TRACE("OMMXFStorage::write");

  OMUInt32 x;
  write(&i, sizeof(OMUInt8), x);

  POSTCONDITION("All bytes written", x == sizeof(OMUInt8));
}

void OMMXFStorage::write(const OMUInt16& i, bool reorderBytes)
{
  TRACE("OMMXFStorage::write");

  OMUInt16 si = i;
  OMByte* src = reinterpret_cast<OMByte*>(&si);
  if (reorderBytes) {
    OMType::reorderInteger(src, sizeof(OMUInt16));
  }
  OMUInt32 x;
  write(src, sizeof(OMUInt16), x);

  POSTCONDITION("All bytes written", x == sizeof(OMUInt16));
}

void OMMXFStorage::write(const OMUInt32& i, bool reorderBytes)
{
  TRACE("OMMXFStorage::write");

  OMUInt32 si = i;
  OMByte* src = reinterpret_cast<OMByte*>(&si);
  if (reorderBytes) {
    OMType::reorderInteger(src, sizeof(OMUInt32));
  }
  OMUInt32 x;
  write(src, sizeof(OMUInt32), x);

  POSTCONDITION("All bytes written", x == sizeof(OMUInt32));
}

void OMMXFStorage::write(const OMUInt64& i, bool reorderBytes)
{
  TRACE("OMMXFStorage::write");

  OMUInt64 si = i;
  OMByte* src = reinterpret_cast<OMByte*>(&si);
  if (reorderBytes) {
    OMType::reorderInteger(src, sizeof(OMUInt64));
  }
  OMUInt32 x;
  write(src, sizeof(OMUInt64), x);

  POSTCONDITION("All bytes written", x == sizeof(OMUInt64));
}

void OMMXFStorage::write(const OMUniqueObjectIdentification& id,
                         bool reorderBytes)
{
  TRACE("OMMXFStorage::write");

  OMUniqueObjectIdentification sid = id;
  OMByte* src = reinterpret_cast<OMByte*>(&sid);
  if (reorderBytes) {
    OMUniqueObjectIdentificationType::instance()->reorder(
                                         src,
                                         sizeof(OMUniqueObjectIdentification));
  }
  OMUInt32 x;
  write(src, sizeof(OMUniqueObjectIdentification), x);

  POSTCONDITION("All bytes written",
                                    x == sizeof(OMUniqueObjectIdentification));
}

void OMMXFStorage::write(const OMByte* buffer, const OMUInt32& bufferSize)
{
  TRACE("OMMXFStorage::write");

  OMUInt32 x;
  write(buffer, bufferSize, x);

  POSTCONDITION("All bytes written", x == bufferSize);
}

void OMMXFStorage::writeKLVKey(const OMKLVKey& key)
{
  TRACE("OMMXFStorage::writeKLVKey");

  OMUInt32 x;
  const OMByte* src = reinterpret_cast<const OMByte*>(&key);
  write(src, sizeof(OMKLVKey), x);

  POSTCONDITION("All bytes written", x == sizeof(OMKLVKey));
}

void OMMXFStorage::writeKLVLength(const OMUInt8& length)
{
  TRACE("OMMXFStorage::writeKLVLength");

  writeBerLength(sizeof(OMUInt8), length);
}

void OMMXFStorage::writeKLVLength(const OMUInt16& length)
{
  TRACE("OMMXFStorage::writeKLVLength");

  writeBerLength(sizeof(OMUInt16), length);
}

void OMMXFStorage::writeKLVLength(const OMUInt32& length)
{
  TRACE("OMMXFStorage::writeKLVLength");

  writeBerLength(sizeof(OMUInt32), length);
}

void OMMXFStorage::writeKLVLength(const OMUInt64& length)
{
  TRACE("OMMXFStorage::writeKLVLength");

  writeBerLength(sizeof(OMUInt64), length);
}

void OMMXFStorage::writeBerLength(OMUInt32 lengthSize, const OMUInt64& length)
{
  TRACE("OMMXFStorage::writeBerLength");

  PRECONDITION("Valid size", lengthSize > 0);

  OMByte berLength[sizeof(OMUInt64) + 1]; // Max

  berEncode(berLength, sizeof(berLength), lengthSize, length);
  OMUInt32 x;
  write(berLength, lengthSize + 1, x);

  POSTCONDITION("All bytes written", x == (lengthSize + 1));
}

  // @mfunc The minimum size of <p i> when encoded using <f berEncode>.
  //        The returned size includes the BER length byte.
  //   @parm The value to encode.
  //   @rdesc The encoded size.
size_t OMMXFStorage::berEncodedSize(const OMUInt64 i)
{
  TRACE("OMMXFStorage::berEncodedSize");

  const OMUInt64 mask = ((OMUInt64)0xff << 56);
  size_t result;
  if (i != 0) {
    result = sizeof(i);
    OMUInt64 v = i;
    while (((v & mask) >> 56) == 0) {
      v = v << 8;
      result = result - 1;
    }
  } else {
    result = 1;
  }
  result = result + 1; // account for length byte
  return result;
}

  // @mfunc BER encode <p value> into <p berValueBuffer>.
  //   @parm OMByte* | berValueBuffer | The buffer in which to place the BER
  //         encoded value.
  //   @parm size_t | berValueBufferSize | The size of the buffer.
  //   @parm const OMUInt32& | berValueSize | The desired encoding size 1-8
  //         (does not include the BER length byte). e.g for a value that fits
  //         in an OMUInt32, 4 is sufficient.
  //   @parm const OMUInt64& | value | The value to encode.
void OMMXFStorage::berEncode(OMByte* berValueBuffer,
                                  size_t ANAME(berValueBufferSize),
                                  const OMUInt32& berValueSize,
                                  const OMUInt64& value)
{
  TRACE("OMMXFStorage::berEncode");
  PRECONDITION("Valid output buffer", berValueBuffer != 0);
  PRECONDITION("Valid output buffer size",
                              berValueBufferSize >= berEncodedSize(value));
  PRECONDITION("Valid size", berValueSize <= sizeof(OMUInt64));
  PRECONDITION("Valid size", berValueSize > 0);
  PRECONDITION("Valid size", berValueSize >= (berEncodedSize(value) - 1));

  OMByte* p = berValueBuffer;
  OMByte b = 0x80 | (OMByte)berValueSize;
  *p++ = b;
  size_t skip = sizeof(OMUInt64) - berValueSize;
  size_t i;
  OMUInt64 v = value;
  for (i = 0; i < skip; i++) {
    v = v << 8;
  }
  const OMUInt64 mask = ((OMUInt64)0xff << 56);
  for (i = i; i < sizeof(OMUInt64); i++) {
    b = (OMByte)((v & mask) >> 56);
    *p++ = b;
    v = v << 8;
  }
}

void OMMXFStorage::readPartition(OMUInt32& bodySID,
                                 OMUInt32& indexSID,
                                 OMUInt32& KAGSize)
{
  TRACE("OMMXFStorage::readPartition");

  readKLVLength();
  OMUInt16 majorVersion;
  read(majorVersion, _reorderBytes);
  OMUInt16 minorVersion;
  read(minorVersion, _reorderBytes);
  read(KAGSize, _reorderBytes);
  OMUInt64 thisPartition;
  read(thisPartition, _reorderBytes);
  OMUInt64 previousPartition;
  read(previousPartition, _reorderBytes);
  OMUInt64 footerPartition;
  read(footerPartition, _reorderBytes);
  OMUInt64 headerByteCount;
  read(headerByteCount, _reorderBytes);
  OMUInt64 indexByteCount;
  read(indexByteCount, _reorderBytes);
  read(indexSID, _reorderBytes);
  OMUInt64 bodyOffset;
  read(bodyOffset, _reorderBytes);
  read(bodySID, _reorderBytes);
  readKLVKey(_operationalPattern);
  OMUInt32 elementCount;
  read(elementCount, _reorderBytes);
  OMUInt32 elementSize;
  read(elementSize, _reorderBytes);
  OMKLVKey essenceContainer;
  for (OMUInt32 i = 0; i < elementCount; i++) {
    readKLVKey(essenceContainer);
  }
}

void OMMXFStorage::readHeaderPartition(void)
{
  TRACE("OMMXFStorage::readHeaderPartition");

  readKLVLength();
  OMUInt16 majorVersion;
  read(majorVersion, _reorderBytes);
  OMUInt16 minorVersion;
  read(minorVersion, _reorderBytes);
  OMUInt32 KAGSize;
  read(KAGSize, _reorderBytes);
  OMUInt64 thisPartition;
  read(thisPartition, _reorderBytes);
  OMUInt64 previousPartition;
  read(previousPartition, _reorderBytes);
  OMUInt64 footerPartition;
  read(footerPartition, _reorderBytes);
  OMUInt64 headerByteCount;
  read(headerByteCount, _reorderBytes);
  OMUInt64 indexByteCount;
  read(indexByteCount, _reorderBytes);
  OMUInt32 indexSID;
  read(indexSID, _reorderBytes);
  OMUInt64 bodyOffset;
  read(bodyOffset, _reorderBytes);
  OMUInt32 bodySID;
  read(bodySID, _reorderBytes);
  readKLVKey(_operationalPattern);
  OMUInt32 elementCount;
  read(elementCount, _reorderBytes);
  OMUInt32 elementSize;
  read(elementSize, _reorderBytes);
  OMKLVKey essenceContainer;
  for (OMUInt32 i = 0; i < elementCount; i++) {
    readKLVKey(essenceContainer);
    if (!containsEssenceContainerLabel(essenceContainer)) {
      addEssenceContainerLabel(essenceContainer);
     }
  }
}

bool OMMXFStorage::read(const OMRawStorage* store,
                        OMUInt8& i)
{
  TRACE("OMMXFStorage::read");
  bool result;
  OMUInt32 bytesRead;
  OMByte* dest = reinterpret_cast<OMByte*>(&i);
  store->read(dest, sizeof(OMUInt8), bytesRead);
  if (bytesRead == sizeof(OMUInt8)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool OMMXFStorage::read(const OMRawStorage* store,
                        OMUInt16& i,
                        bool reorderBytes)
{
  TRACE("OMMXFStorage::read");
  bool result;
  OMUInt32 bytesRead;
  OMByte* dest = reinterpret_cast<OMByte*>(&i);
  store->read(dest, sizeof(OMUInt16), bytesRead);
  if (bytesRead == sizeof(OMUInt16)) {
    if (reorderBytes) {
      OMType::reorderInteger(dest, sizeof(OMUInt16));
    }
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool OMMXFStorage::read(const OMRawStorage* store, OMKLVKey& key)
{
  TRACE("OMMXFStorage::read");
  bool result;
  OMUInt32 bytesRead;
  OMByte* dest = reinterpret_cast<OMByte*>(&key);
  store->read(dest, sizeof(OMKLVKey), bytesRead);
  if (bytesRead == sizeof(OMKLVKey)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool OMMXFStorage::readKLVLength(const OMRawStorage* store, OMUInt64& length)
{
  TRACE("OMMXFStorage::readKLVLength");
  length = readBerLength(store);
  return true; // tjb
}

void OMMXFStorage::read(OMUInt8& i) const
{
  TRACE("OMMXFStorage::read");
  OMUInt8 result;
  OMUInt32 x;
  read(&result, sizeof(OMUInt8), x);
  ASSERT("All bytes read", x == sizeof(OMUInt8));
  i = result;
}

void OMMXFStorage::read(OMUInt16& i, bool reorderBytes) const
{
  TRACE("OMMXFStorage::read");
  OMUInt16 result;
  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&result);
  read(dest, sizeof(OMUInt16), x);
  ASSERT("All bytes read", x == sizeof(OMUInt16));
  if (reorderBytes) {
    OMType::reorderInteger(dest, sizeof(OMUInt16));
  }
  i = result;
}

void OMMXFStorage::read(OMUInt32& i, bool reorderBytes) const
{
  TRACE("OMMXFStorage::read");
  OMUInt32 result;
  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&result);
  read(dest, sizeof(OMUInt32), x);
  ASSERT("All bytes read", x == sizeof(OMUInt32));
  if (reorderBytes) {
    OMType::reorderInteger(dest, sizeof(OMUInt32));
  }
  i = result;
}

void OMMXFStorage::read(OMUInt64& i, bool reorderBytes) const
{
  TRACE("OMMXFStorage::read");
  OMUInt64 result;
  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&result);
  read(dest, sizeof(OMUInt64), x);
  ASSERT("All bytes read", x == sizeof(OMUInt64));
  if (reorderBytes) {
    OMType::reorderInteger(dest, sizeof(OMUInt64));
  }
  i = result;
}

void OMMXFStorage::read(OMUniqueObjectIdentification& id,
                        bool reorderBytes) const
{
  TRACE("OMMXFStorage::read");
  OMUniqueObjectIdentification result;
  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&result);
  read(dest, sizeof(OMUniqueObjectIdentification), x);
  ASSERT("All bytes read", x == sizeof(OMUniqueObjectIdentification));
  if (reorderBytes) {
    OMUniqueObjectIdentificationType::instance()->reorder(
                                         dest,
                                         sizeof(OMUniqueObjectIdentification));
  }
  id = result;
}

void OMMXFStorage::read(OMByte* buffer, const OMUInt32& bufferSize) const
{
  TRACE("OMMXFStorage::read");
  OMUInt32 x;
  read(buffer, bufferSize, x);
  ASSERT("All bytes read", x == bufferSize);
}

void OMMXFStorage::read(OMByte* bytes,
                        OMUInt32 byteCount,
                        OMUInt32& bytesRead) const
{
  TRACE("OMMXFStorage::read");
  OMWrappedRawStorage::read(bytes, byteCount, bytesRead);
}

void OMMXFStorage::readKLVKey(OMKLVKey& key) const
{
  TRACE("OMMXFStorage::readKLVKey");

  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&key);
  read(dest, sizeof(OMKLVKey), x);

  POSTCONDITION("All bytes read", x == sizeof(OMKLVKey));
}

bool OMMXFStorage::readOuterKLVKey(OMKLVKey& key) const
{
  TRACE("OMMXFStorage::readOuterKLVKey");

  OMUInt32 x;
  OMByte* dest = reinterpret_cast<OMByte*>(&key);
  read(dest, sizeof(OMKLVKey), x);
  ASSERT("Read whole key or nothing", (x == 0) || (x == sizeof(OMKLVKey)));
  bool result = false;
  if (x == sizeof(OMKLVKey)) {
    result = true;
  }
  return result;
}

OMUInt64 OMMXFStorage::readKLVLength(void) const
{
  TRACE("OMMXFStorage::readKLVLength");
  OMUInt64 result = readBerLength(this);
  return result;
}

void OMMXFStorage::readKLVFill(void) const
{
  TRACE("OMMXFStorage::readKLVFill");

  OMUInt64 length = readKLVLength();
  for (OMUInt64 i = 0; i < length; i++) {
    OMByte b;
    read(b);
  }
}

void OMMXFStorage::skipLV(void) const
{
  TRACE("OMMXFStorage::skipLV");

  OMUInt64 length = readKLVLength();
  skipV(length);
}

void OMMXFStorage::skipV(OMUInt64& length) const
{
  TRACE("OMMXFStorage::skipV");

  OMUInt64 pos = position();
  OMUInt64 newPosition = pos + length;
  setPosition(newPosition);
}

OMUInt64 OMMXFStorage::readBerLength(const OMRawStorage* store)
{
  TRACE("OMMXFStorage::readBerLength");

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

OMUniqueObjectIdentification OMMXFStorage::instanceId(OMStorable* object)
{
  TRACE("OMMXFStorage::instanceId");
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
OMMXFStorage::object(const OMUniqueObjectIdentification& instanceId)
{
  TRACE("OMMXFStorage::object");

  OMStorable* result;
  ObjectDirectoryEntry e;
  if (!instanceIdToObject()->find(instanceId, e)) {
    result = 0;
  } else {
    result = e._object;
  }
  return result;
}

void OMMXFStorage::associate(OMStorable* object,
                             const OMUniqueObjectIdentification& instanceId)
{
  TRACE("OMMXFStorage::associate");

  ASSERT("Object not present", !objectToInstanceId()->contains(object));
  objectToInstanceId()->insert(object, instanceId);
  ObjectDirectoryEntry* ep = 0;
  if (instanceIdToObject()->find(instanceId, &ep)) {
    ASSERT("No previous entry", ep->_object == 0);
    ep->_object = object;
  } else {
    // Root object is restored before the object directory
    ObjectDirectoryEntry e;
    e._object = object;
    e._offset = 0;
    e._flags = 0;
    ASSERT("Identifier not present", !containsObject(instanceId));
    instanceIdToObject()->insert(instanceId, e);
  }
  ASSERT("Identifier present", containsObject(instanceId));
}

void OMMXFStorage::enterObject(OMStorable& object, OMUInt64 position)
{
  TRACE("OMMXFStorage::enterObject");

  OMUniqueObjectIdentification iid = instanceId(&object);
  if (!instanceIdToObject()->contains(iid)) {
    // This object has never been saved
    ObjectDirectoryEntry e;
    e._object = &object;
    e._offset = position;
    e._flags = 0;
    instanceIdToObject()->insert(iid, e);
  }
}

void OMMXFStorage::removeObject(OMStorable& object)
{
  TRACE("OMMXFStorage::removeObject");

  PRECONDITION("Object directory exists", _instanceIdToObject != 0);
  PRECONDITION("Object set exists", _objectToInstanceId != 0);

  OMUniqueObjectIdentification k;
  bool found = objectToInstanceId()->find(&object, k);
  ASSERT("Object found", found);

  objectToInstanceId()->remove(&object);
  instanceIdToObject()->remove(k);
}

bool
OMMXFStorage::containsObject(const OMUniqueObjectIdentification& instanceId)
{
  TRACE("OMMXFStorage::containsObject");

  bool result = instanceIdToObject()->contains(instanceId);
  return result;
}

void OMMXFStorage::resolve(const OMUniqueObjectIdentification& instanceId)
{
  TRACE("OMMXFStorage::resolve");
  ASSERT("Identifier present", containsObject(instanceId));
  ObjectDirectoryEntry* ep = 0;
  instanceIdToObject()->find(instanceId, &ep);
  ASSERT("Object directory entry found", ep != 0);
  ep->_flags = 1;
}

OMUInt64 OMMXFStorage::primerOffset(void)
{
  TRACE("OMMXFStorage::primerOffset");
  return _primerOffset;
}

void OMMXFStorage::clearObjectDirectory(void)
{
  TRACE("OMMXFStorage::clearObjectDirectory");

  if (_instanceIdToObject != 0) {
    ObjectDirectoryIterator iterator(*_instanceIdToObject, OMBefore);
    while (++iterator) {
      ObjectDirectoryEntry e = iterator.value();
      if (e._flags != 1) {
        if (e._object != 0) {
          OMClassFactory* factory = e._object->classFactory();
          factory->destroy(e._object);
          e._object = 0;
        }
      }
    }
  }
}

void OMMXFStorage::saveObjectDirectory(void)
{
  TRACE("OMMXFStorage::saveObjectDirectory");

  _objectDirectoryOffset = position();
  definition(position(), FUT_OBJECTDIRECTORY);

  writeKLVKey(objectDirectoryKey);
  OMUInt64 entries = _instanceIdToObject->count();
  const OMUInt8 entrySize = sizeof(OMUniqueObjectIdentification) + // iid
                            sizeof(OMUInt64) +                     // offset
                            sizeof(OMUInt8);                       // flags
  OMUInt64 length = sizeof(OMUInt64) +                     // entry count
                    sizeof(OMUInt8) +                      // entry size
                    (entries * entrySize);                 // entries
  writeKLVLength(length);

  write(entries, _reorderBytes);
  write(entrySize);

  ObjectDirectoryIterator iterator(*_instanceIdToObject, OMBefore);
  while (++iterator) {
    OMUniqueObjectIdentification id = iterator.key();
    ObjectDirectoryEntry e = iterator.value();
    write(id, _reorderBytes);
    write(e._offset, _reorderBytes);
    write(e._flags);
  }
}

void OMMXFStorage::fixupReference(OMUInt64 patchOffset, OMUInt64 patchValue)
{
  TRACE("OMMXFStorage::fixupReference");

  OMUInt64 savedPosition = position();
  setPosition(patchOffset);
  write(patchValue, _reorderBytes);
  setPosition(savedPosition);
}

void OMMXFStorage::restoreObjectDirectory(void)
{
  TRACE("OMMXFStorage::restoreObjectDirectory");
  PRECONDITION("Valid metadata directory", _instanceIdToObject != 0);
  PRECONDITION("Valid metadata directory offset", _objectDirectoryOffset != 0);

  OMUInt64 savedPosition = position();
  setPosition(_objectDirectoryOffset);

  OMKLVKey k;
  readKLVKey(k);
  ASSERT("Expected key", k == objectDirectoryKey); // tjb - error
  OMUInt64 setLength = readKLVLength();
  OMUInt64 entries;
  OMUInt8 entrySize;
  ASSERT("Valid length", setLength > sizeof(entries) + sizeof(entrySize));

  read(entries, _reorderBytes);
  read(entrySize);
  ASSERT("Valid entry size",
                           entrySize == (sizeof(OMUniqueObjectIdentification) +
                                         sizeof(OMUInt64) +
                                         sizeof(OMUInt8)));
  ASSERT("Consistent length and entry count",
             setLength == sizeof(entries) +
                          sizeof(entrySize) + (entries * entrySize));

  for (OMUInt64 i = 0; i < entries; i++) {
    OMUniqueObjectIdentification id;
    OMMXFStorage::ObjectDirectoryEntry e;

    read(id, _reorderBytes);
    e._object = 0;
    read(e._offset, _reorderBytes);
    read(e._flags);

    _instanceIdToObject->insert(id, e);
  }

  setPosition(savedPosition);
}

void OMMXFStorage::setObjectDirectoryOffset(OMUInt64 objectDirectoryOffset)
{
  TRACE("OMMXFStorage::setObjectDirectoryOffset");
  _objectDirectoryOffset = objectDirectoryOffset;
}

OMMXFStorage::ObjectDirectory* OMMXFStorage::instanceIdToObject(void)
{
  TRACE("OMMXFStorage::instanceIdToObject");
  if (_instanceIdToObject == 0) {
    _instanceIdToObject = new ObjectDirectory();
    ASSERT("Valid heap pointer", _instanceIdToObject != 0);
  }
  return _instanceIdToObject;
}

OMMXFStorage::ObjectSet* OMMXFStorage::objectToInstanceId(void)
{
  TRACE("OMMXFStorage::objectToInstanceId");

  if (_objectToInstanceId == 0) {
    _objectToInstanceId = new ObjectSet();
    ASSERT("Valid heap pointer", _objectToInstanceId != 0);
  }
  return _objectToInstanceId;
}

OMUInt32 OMMXFStorage::addStream(OMDataStream* stream)
{
  TRACE("OMMXFStorage::addStream");
#if 1
  // tjb - why don't we unconditionally call associate()
  OMUInt32 result;
  if (!streamToSid()->find(stream, result)) {
    _maxSid = _maxSid + 1;
    result = _maxSid;
    associate(stream, result);
  }
#else
  _maxSid = _maxSid + 1;
  OMUInt32 result = _maxSid;
  associate(stream, result);
#endif
  return result;
}

OMUInt32 OMMXFStorage::streamIdentification(OMDataStream* stream)
{
  TRACE("OMMXFStorage::streamIdentification");
  PRECONDITION("Stream present", streamToSid()->contains(stream));

  OMUInt32 sid;
  streamToSid()->find(stream, sid);
  return sid;
}

OMDataStream* OMMXFStorage::stream(OMUInt32 sid)
{
  TRACE("OMMXFStorage::stream");

  OMDataStream* result;
  if (!sidToStream()->find(sid, result)) {
    result = 0;
  }

  return result;
}

void OMMXFStorage::associate(OMDataStream* stream, OMUInt32 sid)
{
  TRACE("OMMXFStorage::associate");

  PRECONDITION("Stream not present", !streamToSid()->contains(stream));
  PRECONDITION("Identifier not present", !sidToStream()->contains(sid));

  streamToSid()->insert(stream, sid);
  sidToStream()->insert(sid, stream);
  if (sid > _maxSid) {
    _maxSid = sid;
  }
}

bool OMMXFStorage::containsStream(OMUInt32 sid)
{
  TRACE("OMMXFStorage::containsStream");

  return sidToStream()->contains(sid);
}

bool OMMXFStorage::containsStream(OMDataStream* stream)
{
  TRACE("OMMXFStorage::containsStream");

  return streamToSid()->contains(stream);
}

OMUInt64 OMMXFStorage::streamSize(OMUInt32 sid)
{
  TRACE("OMMXFStorage::streamSize");
  OMUInt64 result = 0;

  Stream* s = 0;
  if (segmentMap()->find(sid, s)) {
    // tjb - why would't we know about this stream ?
    result = s->_size;
  }
  return result;
}

void OMMXFStorage::streamSetSize(OMUInt32 /* sid */, OMUInt64 /* newSize */)
{
}

void OMMXFStorage::streamRawWrite(OMUInt32 /* sid */,
                                  OMUInt64 rawPosition,
                                  const OMByte* rawBytes,
                                  OMUInt32 rawByteCount)
{
  TRACE("OMMXFStorage::streamRawWrite");

  PRECONDITION("Valid buffer", rawBytes != 0);
  PRECONDITION("Buffer not empty", rawByteCount != 0);

  OMUInt32 bytesWritten;
  writeAt(rawPosition, rawBytes, rawByteCount, bytesWritten);
  POSTCONDITION("All bytes written", bytesWritten == rawByteCount);
}

void OMMXFStorage::streamFragment(OMUInt32 sid,
                                  OMUInt64 position,
                                  OMUInt32 byteCount,
                                  OMUInt64& rawPosition,
                                  OMUInt32& rawByteCount)
{
  TRACE("OMMXFStorage::streamFragment");

  PRECONDITION("Valid byte count", byteCount != 0);

  Segment* seg = streamSegment(sid, position);
  ASSERT("Valid segment", seg != 0);
  OMUInt64 fragmentSize = (seg->_start + seg->_size) - position;
  rawPosition = (position - seg->_start) + seg->_origin;
  if (byteCount > fragmentSize) {
    rawByteCount = static_cast<OMUInt32>(fragmentSize);
  } else {
    rawByteCount = byteCount;
  }
  POSTCONDITION("Valid position",
                (rawPosition >= seg->_origin) &&
                (rawPosition < (seg->_origin + seg->_size)));
  POSTCONDITION("Valid byte count", rawByteCount != 0);
}

void OMMXFStorage::streamWriteFragment(OMUInt32 sid,
                                       OMUInt64 position,
                                       const OMByte* bytes,
                                       OMUInt32 byteCount,
                                       OMUInt32& bytesWritten)
{
  TRACE("OMMXFStorage::streamWriteFragment");

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Buffer not empty", byteCount != 0);

  // Get largest contiguous fragment
  //
  OMUInt64 rawPosition;
  OMUInt32 rawByteCount;
  streamFragment(sid, position, byteCount, rawPosition, rawByteCount);

  // Write to the fragment
  streamRawWrite(sid, rawPosition, bytes, rawByteCount);
  bytesWritten = rawByteCount;

  // Update stream size
  Stream* s = 0;
  segmentMap()->find(sid, s);
  ASSERT("Stream found", s != 0);
  OMUInt64 newPosition = position + bytesWritten;
  if (newPosition > s->_size) {
    s->_size = newPosition;
  }
}

void OMMXFStorage::streamWriteAt(OMUInt32 sid,
                                 OMUInt64 position,
                                 const OMByte* bytes,
                                 OMUInt32 byteCount,
                                 OMUInt32& bytesWritten)
{
  TRACE("OMMXFStorage::streamWriteAt");

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Buffer not empty", byteCount != 0);

  OMUInt32 writeCount = byteCount;
  OMUInt64 streamBytes = 0;
  Stream* s = 0;
  segmentMap()->find(sid, s);
  if (s != 0) {
    streamBytes = allocatedSize(s);
  }
  if ((position + byteCount) > streamBytes) {
    streamGrow(sid, (position + byteCount) - streamBytes);
  }
  bytesWritten = writeCount;

  const OMByte* p = bytes;
  OMUInt64 pos = position;
  while (writeCount > 0) {
    OMUInt32 w;
    streamWriteFragment(sid, pos, p, writeCount, w);
    writeCount = writeCount - w;
    pos = pos + w;
    p = p + w;
  }
}

void OMMXFStorage::streamRawRead(OMUInt32 /* sid */,
                                 OMUInt64 rawPosition,
                                 OMByte* rawBytes,
                                 OMUInt32 rawByteCount)
{
  TRACE("OMMXFStorage::streamRawRead");

  PRECONDITION("Valid buffer", rawBytes != 0);
  PRECONDITION("Buffer not empty", rawByteCount != 0);

  OMUInt32 bytesRead;
  readAt(rawPosition, rawBytes, rawByteCount, bytesRead);
  POSTCONDITION("All bytes read", bytesRead == rawByteCount);
}

void OMMXFStorage::streamReadFragment(OMUInt32 sid,
                                      OMUInt64 position,
                                      OMByte* bytes,
                                      OMUInt32 byteCount,
                                      OMUInt32& bytesRead)
{
  TRACE("OMMXFStorage::streamReadFragment");

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Buffer not empty", byteCount != 0);

  // Get largest contiguous fragment
  //
  OMUInt64 rawPosition;
  OMUInt32 rawByteCount;
  streamFragment(sid, position, byteCount, rawPosition, rawByteCount);

  // Read from the fragment
  streamRawRead(sid, rawPosition, bytes, rawByteCount);
  bytesRead = rawByteCount;
}

void OMMXFStorage::streamReadAt(OMUInt32 sid,
                                OMUInt64 position,
                                OMByte* bytes,
                                OMUInt32 byteCount,
                                OMUInt32& bytesRead)
{
  TRACE("OMMXFStorage::streamReadAt");

  PRECONDITION("Valid buffer", bytes != 0);
  PRECONDITION("Buffer not empty", byteCount != 0);

  OMUInt32 readCount = byteCount;
  OMUInt64 streamBytes = streamSize(sid);
  if (position > streamBytes) {
    readCount = 0;
  } else if ((position + byteCount) > streamBytes) {
    readCount = static_cast<OMUInt32>(streamBytes - position);
  }
  bytesRead = readCount;

  OMByte* p = bytes;
  OMUInt64 pos = position;
  while (readCount > 0) {
    OMUInt32 r;
    streamReadFragment(sid, pos, p, readCount, r);
    readCount = readCount - r;
    pos = pos + r;
    p = p + r;
  }
}

void OMMXFStorage::streamRestoreSegment(OMUInt32 sid,
                                        OMUInt64 start,
                                        OMUInt64 size,
                                        OMKLVKey label,
                                        OMUInt32 gridSize)
{
  TRACE("OMMXFStorage::streamRestoreSegment");
  Stream* s = 0;
  if (!segmentMap()->find(sid, s)) {
    s = createStream(sid, size, label, gridSize);
    addSegment(s, 0, size, start);
  } else {
    Segment* last = findLastSegment(s);
    ASSERT("Last segment found", last != 0);
    addSegment(s, last->_start + last->_size, size, start);
    s->_size = s->_size + size;
  }
  OMDataStream* sp = stream(sid);
  ASSERT("Found stream", sp != 0);
  OMDataStreamProperty* ds = dynamic_cast<OMDataStreamProperty*>(sp);
  ASSERT("Valid type", ds != 0);
  OMStoredStream* ss = ds->stream();
  ASSERT("Valid stream", ss != 0);
  OMKLVStoredStream* kss = dynamic_cast<OMKLVStoredStream*>(ss);
  ASSERT("Valid type", kss != 0);
  kss->setLabel(label);
  kss->setBlockSize(gridSize);
}

void OMMXFStorage::streamGrow(OMUInt32 sid, OMUInt64 growBytes)
{
  TRACE("OMMXFStorage::streamGrow");
  PRECONDITION("Valid grow bytes", growBytes > 0);

  OMUInt64 start;
  OMUInt64 increment;
  Stream* s = 0;
  if (!segmentMap()->find(sid, s)) {
    // First segment

#if 1
    // tjb we shouldn't know about OMKLVStoredStream here
    OMDataStream* sp = stream(sid);
    ASSERT("Found stream", sp != 0);
    OMDataStreamProperty* ds = dynamic_cast<OMDataStreamProperty*>(sp);
    ASSERT("Valid type", ds != 0);
    OMStoredStream* ss = ds->stream();
    ASSERT("Valid stream", ss != 0);
    OMKLVStoredStream* kss = dynamic_cast<OMKLVStoredStream*>(ss);
    ASSERT("Valid type", kss != 0);
    OMKLVKey label = kss->label();
    OMUInt32 gridSize = kss->blockSize();
#endif
    s = createStream(sid, 0, label, gridSize);
    start = 0;
    increment = (((growBytes - 1) / s->_gridSize) + 1) * s->_gridSize;
    ASSERT("Valid increment", increment >= growBytes);
    _fileSize = _fileSize + s->_gridSize; // For body partition and filler
    addSegment(s, start, increment, _fileSize);
  } else {
    Segment* last = findLastSegment(s);
    ASSERT("Last segment found", last != 0);
    start = last->_start + last->_size;
    increment = (((growBytes - 1) / s->_gridSize) + 1) * s->_gridSize;
    ASSERT("Valid increment", increment >= growBytes);
    if ((last->_origin + last->_size) != _fileSize) {
      // Last segment not at end of file - add a new one
      _fileSize = _fileSize + s->_gridSize; // For body partition and filler
      addSegment(s, start, increment, _fileSize);
    } else {
      // Last segment at end of file - grow it
      last->_size = last->_size + increment;
      _fileSize = _fileSize + increment;
    }
  }
}

void OMMXFStorage::saveStreams(void)
{
  TRACE("OMMXFStorage::saveStreams");

  // We expect to be positioned in the pre-allocated header
  // space just after the last metadata object
  ASSERT("Valid position", position() < bodyPartitionOffset);

  // fill remainder of pre-allocated space
  OMUInt32 fillAlignment = bodyPartitionOffset;
  fillAlignK(position(), fillAlignment);

  // Write header partition and alignment fill.
  //
  setPosition(0);
  writeHeaderPartition(0, 0, defaultKAGSize);

  if (_segments != 0) {
    const Segment* lastFileSegment = _segments->last().value();
    OMUInt64 previous = 0;
    SegmentListIterator sl(*_segments, OMBefore);
    while (++sl) {
      Segment* seg = sl.value();
      ASSERT("Segments in file address order", seg->_origin > previous);
      Stream* s = seg->_stream;
      ASSERT("Valid stream", s != 0);
      OMUInt64 len; // length of valid portion of segment
      if (s->_size > seg->_start + seg->_size) {
        // eos in later segment
        len = seg->_size;
      } else if (s->_size < seg->_start) {
        // eos in earlier segment
        len = 0;
      } else {
        // eos in this segment
        len = s->_size - seg->_start;
      }
      ASSERT("Valid length", len <= seg->_size);

      // For body partition and filler
      OMUInt64 pos = seg->_origin - s->_gridSize + fillBufferZoneSize;

      // Write partition pack
      setPosition(pos);
      if (lastFileSegment == seg  &&  s->_label == IndexTableSegmentKey) {
        // This is the last stream segment in the file
        // and it's a part of an index stream - put it
        // in the footer partition.
        writeFooterPartition(s->_sid, s->_gridSize);
      } else {
        writeBodyPartition(s->_sid, 0, s->_gridSize);
      }

      // Write essence element label
      writeKLVKey(s->_label);
      writeKLVLength(len);

      ASSERT("Consistent origin", seg->_origin == position());

      // Fill end of segment
      OMUInt64 fillSize = seg->_size + fillBufferZoneSize - len;
      OMUInt64 fillEnd = (seg->_origin + seg->_size + fillBufferZoneSize);
      OMUInt64 fillStart = fillEnd - fillSize;
      ASSERT("Can fill", fillSize >= minimumFill);
      setPosition(fillStart);
      writeKLVFill(fillSize - minimumFill);

      // If we wrote index, fill in the index bye count
      if (s->_label == IndexTableSegmentKey) {
        OMUInt64 ibc = seg->_size + fillBufferZoneSize;
        ibc = ibc + sizeof(OMKLVKey) + 8 + 1;
        fixupReference(pos + sizeof(OMKLVKey) + 8 + 1 + 40, ibc);
      }

      previous = seg->_origin;
    }

    if (lastFileSegment->_stream->_label != IndexTableSegmentKey) {
      setPosition(_fileSize + fillBufferZoneSize);
      writePartition(CompleteFooterKey, 0, 0, defaultKAGSize);
    }
  } else {
    setPosition(_fileSize + fillBufferZoneSize);
    writePartition(CompleteFooterKey, 0, 0, defaultKAGSize);
  }
}

OMMXFStorage::SegmentListIterator*
OMMXFStorage::streamSegments(OMUInt32 sid) const
{
  TRACE("OMMXFStorage::streamSegments");

  SegmentListIterator* result = 0;
  Stream* s = 0;
  OMMXFStorage* nonConstThis = const_cast<OMMXFStorage*>(this);
  if (nonConstThis->segmentMap()->find(sid, s)) {
    result = new SegmentListIterator(*(s->_segments), OMBefore);
    ASSERT("Valid heap pointer", result != 0);
  }
  return result;
}

OMSet<OMDataStream*, OMUInt32>*
OMMXFStorage::streamToSid(void)
{
  TRACE("OMMXFStorage::streamToSid");

  if (_streamToSid == 0) {
    _streamToSid = new OMSet<OMDataStream*, OMUInt32>();
    ASSERT("Valid heap pointer", _streamToSid != 0);
  }
  return _streamToSid;
}

OMSet<OMUInt32, OMDataStream*>*
OMMXFStorage::sidToStream(void)
{
  TRACE("OMMXFStorage::sidToStream");

  if (_sidToStream == 0) {
    _sidToStream = new OMSet<OMUInt32, OMDataStream*>();
    ASSERT("Valid heap pointer", _sidToStream != 0);
  }
  return _sidToStream;
}

  // @mfunc Record a reference to <p tag> at <p address>.
  //   @parm The file address of the reference.
  //   @parm The tag.
void OMMXFStorage::reference(OMUInt64 address, OMUInt8 tag)
{
  TRACE("OMMXFStorage::reference");
  PRECONDITION("Valid tag", tag != FUT_UNDEFINED);

  Fixup* f = new Fixup();
  ASSERT("Valid heap pointer", f != 0);
  f->_address = address;
  f->_value = 0;
  f->_tag = tag;
  _fixups.append(f);
}

  // @mfunc Provide a definition for <p tag> of <p value>.
  //   @parm The value to resolve all reference with the same tag.
  //   @parm The tag.
void OMMXFStorage::definition(OMUInt64 value, OMUInt8 tag)
{
  TRACE("OMMXFStorage::definition");
  PRECONDITION("Valid tag", tag != FUT_UNDEFINED);

  FixupListIterator iterator(_fixups, OMBefore);
  while (++iterator) {
    Fixup* f = iterator.value();
    ASSERT("Valid value", f != 0);
    if (f->_tag == tag) {
      f->_value = value;
    }
  }
}

  // @mfunc Apply <c Fixup>s for <p tag>.
  //   @parm The tag.
void OMMXFStorage::fixup(OMUInt8 tag)
{
  TRACE("OMMXFStorage::fixup");

  FixupListIterator iterator(_fixups, OMBefore);
  while (++iterator) {
    Fixup* f = iterator.value();
    ASSERT("Valid value", f != 0);
    ASSERT("Defined", f->_tag != FUT_UNDEFINED);
    if (f->_tag == tag) {
      fixupReference(f->_address, f->_value);
      f->_tag = FUT_RESOLVED;
    }
  }
}

  // @mfunc Apply all <c Fixup>s.
void OMMXFStorage::fixup(void)
{
  TRACE("OMMXFStorage::fixup");

  FixupListIterator iterator(_fixups, OMBefore);
  while (++iterator) {
    Fixup* f = iterator.value();
    ASSERT("Valid value", f != 0);
    ASSERT("Defined", f->_tag != FUT_UNDEFINED);
    if (f->_tag != FUT_RESOLVED) {
      fixupReference(f->_address, f->_value);
      f->_tag = FUT_RESOLVED;
    }
  }
}

  // @mfunc Destroy all <c Fixup>s.
void OMMXFStorage::destroyFixups(void)
{
  TRACE("OMMXFStorage::destroyFixups");

  FixupListIterator iterator(_fixups, OMBefore);
  while (++iterator) {
    Fixup* f = iterator.value();
    ASSERT("Valid value", f != 0);
    ASSERT("Resolved", f->_tag == FUT_RESOLVED);
    delete f;
  }
//_fixups.clear();
}

OMMXFStorage::Stream* OMMXFStorage::createStream(OMUInt32 sid,
                                                 OMUInt64 size,
                                                 OMKLVKey label,
                                                 OMUInt32 gridSize)
{
  TRACE("OMMXFStorage::createStream");
#if defined (OMONLY)
  PRECONDITION("Valid label", label != nullOMKLVKey);
  PRECONDITION("Valid KAG size", gridSize != 0);
  PRECONDITION("Reasonable KAG size", gridSize > 128);
#else
  if (label == nullOMKLVKey) {
    OMKLVKey k = {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01,
                  0x0e, 0x04, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff};
    label = k;
  }
  if (gridSize == 0) {
    gridSize = 0x200;
  }
#endif

  Stream* result = new Stream();
  ASSERT("Valid heap pointer", result != 0);
  result->_segments = new SegmentList();
  ASSERT("Valid heap pointer", result->_segments != 0);
  result->_size = size;
  result->_label = label;
  result->_gridSize = gridSize;
  result->_sid = sid;
  segmentMap()->insert(sid, result);
  return result;
}

OMMXFStorage::Segment*
OMMXFStorage::addSegment(Stream* s,
                         OMUInt64 start,
                         OMUInt64 size,
                         OMUInt64 origin)
{
  TRACE("OMMXFStorage::addSegment");
  PRECONDITION("Valid stream", s != 0);
  PRECONDITION("Valid list", s->_segments != 0);
  PRECONDITION("Valid size", size != 0);

  Segment* result = new Segment();
  ASSERT("Valid heap pointer", result != 0);
  result->_start = start;
  result->_size = size;
  result->_origin = origin;
  result->_stream = s;
  s->_segments->append(result);
  _fileSize = _fileSize + size;

  if (_segments == 0) {
    _segments =  new SegmentList();
     ASSERT("Valid heap pointer", _segments != 0);
  }
  _segments->append(result);
  return result;
}

OMMXFStorage::Segment*
OMMXFStorage::findSegment(Stream* s, OMUInt64 position)
{
  TRACE("OMMXFStorage::findSegment");
  PRECONDITION("Valid stream", s != 0);

  ASSERT("Valid list", s->_segments != 0);
  Segment* result = 0;
  SegmentListIterator iterator(*s->_segments, OMBefore);
  while (++iterator) {
    Segment* c = iterator.value();
    OMUInt64 start = c->_start;
    OMUInt64 size = c->_size;
    if ((position >= start) && (position < start + size)) {
      result = c;
      break;
    }
   }
  return result;
}

OMUInt64 OMMXFStorage::allocatedSize(Stream* s)
{
  TRACE("OMMXFStorage::allocatedSize");
  PRECONDITION("Valid stream", s != 0);
  PRECONDITION("Valid segment list", s->_segments != 0);

  OMUInt64 result = 0;
  SegmentListIterator iterator(*s->_segments, OMBefore);
  while (++iterator) {
    Segment* c = iterator.value();
    result = result + c->_size;
  }
  return result;
}

OMMXFStorage::Segment* OMMXFStorage::findLastSegment(Stream* s)
{
  TRACE("OMMXFStorage::findLastSegment");
  PRECONDITION("Valid stream", s != 0);
  PRECONDITION("Valid segment list", s->_segments != 0);

  SegmentListIterator iterator = s->_segments->last();
  Segment* result = iterator.value();
  POSTCONDITION("Valid result", result != 0);
  return result;
}

OMMXFStorage::Segment*
OMMXFStorage::streamSegment(OMUInt32 sid, OMUInt64 position)
{
  TRACE("OMMXFStorage::streamSegment");

  Stream* s = 0;
  segmentMap()->find(sid, s);
  ASSERT("Stream found", s != 0);
  Segment* result = findSegment(s, position);
  POSTCONDITION("Valid result", result != 0);
  POSTCONDITION("Valid result", position >= result->_start);
  POSTCONDITION("Valid result", position <= result->_start + result->_size);
  return result;
}

OMMXFStorage::SegmentMap* OMMXFStorage::segmentMap(void)
{
  TRACE("OMMXFStorage::segmentMap");
  if (_segmentMap == 0) {
    _segmentMap = new SegmentMap();
    ASSERT("Valid heap pointer", _segmentMap != 0);
  }
  return _segmentMap;
}

void OMMXFStorage::destroySegmentMap(void)
{
  TRACE("OMMXFStorage::destroySegmentMap");

  if (_segmentMap != 0) {
    SegmentMapIterator iter(*_segmentMap, OMBefore);
    while (++iter) {
      Stream* s = iter.value();
      if (s->_segments != 0) {
        SegmentListIterator siter(*s->_segments, OMBefore);
        while (++siter) {
          Segment* seg = siter.value();
          delete seg;
        }
//      s->_segments->clear();
        delete s->_segments;
        s->_segments = 0;
      }
      delete s;
    }
//  _segmentMap->clear();
    delete _segmentMap;
    _segmentMap = 0;
  }
}

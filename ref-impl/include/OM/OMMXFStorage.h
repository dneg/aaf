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
#ifndef OMMXFSTORAGE_H
#define OMMXFSTORAGE_H

#include "OMWrappedRawStorage.h"
#include "OMDataTypes.h"
#include "OMIdentitySet.h"
#include "OMDataStream.h"
#include "OMList.h"
#include "OMListIterator.h"
#include "OMVector.h"

static const OMKLVKey OpenHeaderPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00};

static const OMKLVKey OpenCompleteHeaderPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x03, 0x00};

static const OMKLVKey ClosedHeaderPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x00};

static const OMKLVKey ClosedCompleteHeaderPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x04, 0x00};

static const OMKLVKey OpenBodyPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x01, 0x00};

static const OMKLVKey OpenCompleteBodyPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x03, 0x00};

static const OMKLVKey ClosedBodyPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x02, 0x00};

static const OMKLVKey ClosedCompleteBodyPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x04, 0x00};

static const OMKLVKey FooterKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x04, 0x02, 0x00};

static const OMKLVKey CompleteFooterKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x04, 0x04, 0x00};

static const OMKLVKey primerKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x05, 0x01, 0x00};

static const OMKLVKey fillKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
   0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};

static const OMKLVKey IndexTableSegmentKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x53, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x10, 0x01, 0x00};

static const OMKLVKey RandomIndexMetadataKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x11, 0x01, 0x00};

// Minimum size of a fill KLV triplet (when L not known a priori)
// K (16 bytes)
// L (1 + 8 bytes BER encoded)
// V (0 fill bytes)
static const OMUInt64 minimumFill = sizeof(OMKLVKey) + 1 + sizeof(OMUInt64);

// Coalesced with preceeding fill of any size.
// (value of fillBufferZoneSize must be > minimumFill)
//
static const OMUInt64 fillBufferZoneSize = 32;

static const OMUInt64 bodyPartitionOffset = 0x20000 + fillBufferZoneSize;

static const OMUInt32 defaultKAGSize = 0x100;

static const OMUInt16 currentMajorVersion = 0xffff;
static const OMUInt16 currentMinorVersion = 0xfff8;

// Total size of the fixed-size portions of a partition value
static const OMUInt32 fixedPartitionSize =
  sizeof(OMUInt16) + // Major Version
  sizeof(OMUInt16) + // Minor Version
  sizeof(OMUInt32) + // KAGSize
  sizeof(OMUInt64) + // ThisPartition
  sizeof(OMUInt64) + // PreviousPartition
  sizeof(OMUInt64) + // FooterPartition
  sizeof(OMUInt64) + // HeaderByteCount
  sizeof(OMUInt64) + // IndexByteCount
  sizeof(OMUInt32) + // IndexSID
  sizeof(OMUInt64) + // BodyOffset
  sizeof(OMUInt32) + // BodySID
  sizeof(OMKLVKey) + // Operational Pattern
  sizeof(OMUInt32) + // count of essence container labels
  sizeof(OMUInt32);  // size of each essence container label

static const OMPropertyId PID_InterchangeObject_InstanceUID = 0x3c0a;

static const OMPropertyId PID_Root_ObjectDirectory = 0x003;
static const OMPropertyId PID_Root_FormatVersion   = 0x004;

static const OMUInt32 formatVersion = 0x004;

static const OMKLVKey objectDirectoryKey =
  {0x96, 0x13, 0xb3, 0x8a, 0x87, 0x34, 0x87, 0x46,
   0xf1, 0x02, 0x96, 0xf0, 0x56, 0xe0, 0x4d, 0x2a};

// Fixup tags
static const OMUInt8 FUT_UNDEFINED       = 0x80;
static const OMUInt8 FUT_RESOLVED        = 0x81;

static const OMUInt8 FUT_OBJECTDIRECTORY = 0xff;

class OMStorable;
template <typename Key, typename Element>
class OMSet;
template <typename Key, typename Element>
class OMSetIterator;
template <typename Element>
class OMIdentitySetIterator;

  // @class Class supporting access to the raw bytes of MXF
  //        files supported by the Object Manager.
  //   @base public | <c OMRawStorage>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMMXFStorage : public OMWrappedRawStorage {
public:
  // @access Public members.

    // @cmember Constructor.
  OMMXFStorage(OMRawStorage* store);

    // @cmember Destructor.
  virtual ~OMMXFStorage(void);

    // @cmember Open this <c OMMXFStorage>.
  virtual void open(void);

    // @cmember Close this <c OMMXFStorage>.
  virtual void close(void);

    // @cmember Set the operational pattern to <p pattern>.
  virtual void setOperationalPattern(const OMKLVKey& pattern);

    // @cmember Get the operational pattern.
  virtual OMKLVKey operationalPattern(void) const;

  typedef OMIdentitySet<OMKLVKey> LabelSet;
  typedef OMIdentitySetIterator<OMKLVKey> LabelSetIterator;

    // @cmember Add <p label> to the set of essence container labels.
  virtual void addEssenceContainerLabel(const OMKLVKey& label);

    // @cmember Is <p label> present in the set of essence container labels.
  virtual bool containsEssenceContainerLabel(const OMKLVKey& label) const;

    // @cmember Create an iterator over the set of essence container labels.
  virtual LabelSetIterator* essenceContainerLabels(void) const;

    // @cmember Set the file generation.
  virtual void setGeneration(const OMUniqueObjectIdentification& generation);

  virtual OMUniqueObjectIdentification generation(void) const;

  static bool isHeader(const OMKLVKey& k);

  static bool isBody(const OMKLVKey& k);

  static bool isFooter(const OMKLVKey& k);

  virtual void writeHeaderPartition(OMUInt32 bodySID,
                                    OMUInt32 indexSID,
                                    OMUInt32 KAGSize);
  virtual void writeBodyPartition(OMUInt32 bodySID,
                                  OMUInt32 indexSID,
                                  OMUInt32 KAGSize);
  virtual void writeFooterPartition(OMUInt32 indexSID,
                                    OMUInt32 KAGSize);
  virtual void writePartition(const OMKLVKey& key,
                              OMUInt32 bodySID,
                              OMUInt32 indexSID,
                              OMUInt32 KAGSize);

  virtual void writeRandomIndex(void);

  virtual void readRandomIndex(void);

    // @cmember Write a fill key, a BER encoded length and
    //          <p length> bytes of fill.
  virtual void writeKLVFill(const OMUInt64& length);

    // @cmember Write fill so that the next key is page aligned.
  virtual void fillAlignK(const OMUInt64& currentPosition,
                          const OMUInt32& KAGSize);

    // @cmember Write fill so that the next value is page aligned.
  virtual void fillAlignV(const OMUInt64& currentPosition,
                          const OMUInt32& KAGSize);

  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);
  virtual void write(const OMUInt8& i);
  virtual void write(const OMUInt16& i, bool reorderBytes);
  virtual void write(const OMUInt32& i, bool reorderBytes);
  virtual void write(const OMUInt64& i, bool reorderBytes);
  virtual void write(const OMUniqueObjectIdentification& id,
                     bool reorderBytes);
  virtual void write(const OMByte* buffer,
                     const OMUInt32& bufferSize);

  virtual void writeKLVKey(const OMKLVKey& key);

  virtual void writeKLVLength(const OMUInt8& length);
  virtual void writeKLVLength(const OMUInt16& length);
  virtual void writeKLVLength(const OMUInt32& length);
  virtual void writeKLVLength(const OMUInt64& length);

  virtual void writeBerLength(OMUInt32 lengthSize, const OMUInt64& length);

    // @cmember The minimum size of <p i> when encoded using <f berEncode>.
    //          The returned size includes the BER length byte.
  static size_t berEncodedSize(const OMUInt64 i);

    // @cmember BER encode <p value> into <p berValueBuffer>.
  static void berEncode(OMByte* berValueBuffer,
                        size_t berValueBufferSize,
                        const OMUInt32& berValueSize,
                        const OMUInt64& value);

  virtual void readPartition(OMUInt32& bodySID,
                             OMUInt32& indexSID,
                             OMUInt32& KAGSize);

  virtual void readHeaderPartition(void);

  static bool read(const OMRawStorage* store, OMUInt8& i);
  static bool read(const OMRawStorage* store, OMUInt16& i, bool reorderBytes);
  static bool read(const OMRawStorage* store, OMKLVKey& key);
  static bool readKLVLength(const OMRawStorage* store, OMUInt64& length);

  virtual void read(OMUInt8& i) const;
  virtual void read(OMUInt16& i, bool reorderBytes) const;
  virtual void read(OMUInt32& i, bool reorderBytes) const;
  virtual void read(OMUInt64& i, bool reorderBytes) const;
  virtual void read(OMUniqueObjectIdentification& id, bool reorderBytes) const;
  virtual void read(OMByte* buffer, const OMUInt32& bufferSize) const;
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

  virtual void readKLVKey(OMKLVKey& key) const;

  virtual bool readOuterKLVKey(OMKLVKey& key) const;

  virtual OMUInt64 readKLVLength(void) const;

  virtual void readKLVFill(void) const;

  virtual void skipLV(void) const;

  virtual void skipV(OMUInt64& length) const;

  static OMUInt64 readBerLength(const OMRawStorage* store);

    // Object -> instanceId
  OMUniqueObjectIdentification instanceId(OMStorable* object);

    // instanceId -> object
  OMStorable* object(const OMUniqueObjectIdentification& instanceId);

    // object <-> instanceId
  void associate(OMStorable* object,
                 const OMUniqueObjectIdentification& instanceId);

    // enter object into object directory
  void enterObject(OMStorable& object, OMUInt64 position);

  void removeObject(OMStorable& object);

  bool containsObject(const OMUniqueObjectIdentification& instanceId);

  void resolve(const OMUniqueObjectIdentification& instanceId);

  virtual OMUInt64 primerOffset(void);

  virtual void clearObjectDirectory(void);

  virtual void saveObjectDirectory(void);

  virtual void fixupReference(OMUInt64 patchOffset, OMUInt64 patchValue);

  virtual void restoreObjectDirectory(void);

  virtual void setObjectDirectoryOffset(OMUInt64 objectDirectoryOffset);

  virtual OMUInt32 addStream(OMDataStream* stream);

  virtual OMUInt32 streamIdentification(OMDataStream* stream);

  virtual OMDataStream* stream(OMUInt32 sid);

  virtual void associate(OMDataStream* stream, OMUInt32 sid);

  virtual bool containsStream(OMUInt32 sid);

  virtual bool containsStream(OMDataStream* stream);

  virtual OMUInt64 streamSize(OMUInt32 sid);

  virtual void streamSetSize(OMUInt32 sid, OMUInt64 newSize);

  virtual void streamRawWrite(OMUInt32 sid,
                              OMUInt64 rawPosition,
                              const OMByte* rawBytes,
                              OMUInt32 rawByteCount);

  virtual void streamFragment(OMUInt32 sid,
                              OMUInt64 position,
                              OMUInt32 byteCount,
                              OMUInt64& rawPosition,
                              OMUInt32& rawByteCount);

  virtual void streamWriteFragment(OMUInt32 sid,
                                   OMUInt64 position,
                                   const OMByte* bytes,
                                   OMUInt32 byteCount,
                                   OMUInt32& bytesWritten);

  virtual void streamWriteAt(OMUInt32 sid,
                             OMUInt64 position,
                             const OMByte* bytes,
                             OMUInt32 byteCount,
                             OMUInt32& bytesWritten);

  virtual void streamRawRead(OMUInt32 sid,
                             OMUInt64 rawPosition,
                             OMByte* rawBytes,
                             OMUInt32 rawByteCount);

  virtual void streamReadFragment(OMUInt32 sid,
                                  OMUInt64 position,
                                  OMByte* bytes,
                                  OMUInt32 byteCount,
                                  OMUInt32& bytesRead);

  virtual void streamReadAt(OMUInt32 sid,
                            OMUInt64 position,
                            OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesRead);

  virtual void streamRestoreSegment(OMUInt32 sid,
                                    OMUInt64 start,
                                    OMUInt64 size,
                                    OMKLVKey label,
                                    OMUInt32 gridSize);

  virtual void streamGrow(OMUInt32 sid, OMUInt64 growBytes);

  virtual void saveStreams(void);

  struct Stream;
  struct Segment {
    OMUInt64 _start;   // Stream position of this Segment
    OMUInt64 _size;    // Size of this Segment in bytes
    OMUInt64 _origin;  // File position of this Segment
    Stream* _stream;   // Stream to which this segment belongs
  };
  typedef OMList<Segment*> SegmentList;
  typedef OMListIterator<Segment*> SegmentListIterator;

  struct Stream {
    SegmentList* _segments;
    OMUInt64 _size;
    OMKLVKey _label;
    OMUInt32 _gridSize;
    OMUInt32 _sid;
  };

  virtual SegmentListIterator* streamSegments(OMUInt32 sid) const;

    // @cmember Record a reference to <p tag> at <p address>.
  void reference(OMUInt64 address, OMUInt8 tag);

    // @cmember Provide a definition for <p tag> of <p value>.
  void definition(OMUInt64 value, OMUInt8 tag);

    // @cmember Apply <c Fixup>s for <p tag>.
  void fixup(OMUInt8 tag);

    // @cmember Apply all <c Fixup>s.
  void fixup(void);

    // @cmember Destroy all <c Fixup>s.
  void destroyFixups(void);

private:
  // @access Private members.

  struct ObjectDirectoryEntry {
    OMStorable* _object;
    OMUInt64 _offset;
    OMUInt8 _flags;
  };

  typedef OMSet<OMUniqueObjectIdentification,
                ObjectDirectoryEntry> ObjectDirectory;
  typedef OMSetIterator<OMUniqueObjectIdentification,
                        ObjectDirectoryEntry> ObjectDirectoryIterator;
  typedef OMSet<OMStorable*, OMUniqueObjectIdentification> ObjectSet;

  ObjectDirectory* instanceIdToObject(void);

  ObjectSet* objectToInstanceId(void);

  OMSet<OMDataStream*, OMUInt32>* streamToSid(void);
  OMSet<OMUInt32, OMDataStream*>* sidToStream(void);

  struct Fixup;
  typedef OMList<Fixup*> FixupList;
  typedef OMListIterator<Fixup*> FixupListIterator;

  struct Fixup {
    OMUInt64 _address; // File address
    OMUInt64 _value;   // Value to write to address
    OMUInt8  _tag;     // Identifies the reference
  };

  FixupList _fixups;

  bool _reorderBytes;
  OMKLVKey _operationalPattern;
  LabelSet _essenceContainerLabels;
  OMUniqueObjectIdentification _generation;
  OMUInt64 _objectDirectoryOffset;    // offset of object directory
  ObjectDirectory* _instanceIdToObject;
  ObjectSet* _objectToInstanceId;
  OMSet<OMDataStream*, OMUInt32>* _streamToSid;
  OMSet<OMUInt32, OMDataStream*>* _sidToStream;
  OMUInt32 _maxSid;

  virtual Stream* createStream(OMUInt32 sid,
                               OMUInt64 size,
                               OMKLVKey label,
                               OMUInt32 gridSize);
  virtual Segment* addSegment(Stream* s,
                              OMUInt64 start,
                              OMUInt64 size,
                              OMUInt64 origin);
  virtual Segment* findSegment(Stream* s, OMUInt64 position);
  virtual Segment* findLastSegment(Stream* s);
  virtual OMUInt64 allocatedSize(Stream* s);
  virtual Segment* streamSegment(OMUInt32 sid, OMUInt64 position);

  typedef OMSet<OMUInt32, Stream*> SegmentMap;
  typedef OMSetIterator<OMUInt32, Stream*> SegmentMapIterator;
  virtual SegmentMap* segmentMap(void);
  void destroySegmentMap(void);
  SegmentMap* _segmentMap;
  SegmentList* _segments;
  OMUInt64 _fileSize;

  struct Partition {
    OMUInt64 _address;
    OMUInt32 _sid;
  };
  OMVector<Partition*> _partitions;
  OMUInt64 _primerOffset;
};

#endif

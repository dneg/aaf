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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
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

//#define OM_NEW_STREAM_PARSING
#define OM_FASTER_STREAM_PARSING
#define OM_NEW_STREAM_WRITING

static const OMKLVKey OpenIncompleteHeaderPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00};

static const OMKLVKey IncompleteHeaderPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x00};

static const OMKLVKey OpenHeaderPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x03, 0x00};

static const OMKLVKey HeaderPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x04, 0x00};

static const OMKLVKey OpenIncompleteBodyPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x01, 0x00};

static const OMKLVKey IncompleteBodyPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x02, 0x00};

static const OMKLVKey OpenBodyPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x03, 0x00};

static const OMKLVKey BodyPartitionKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x04, 0x00};

static const OMKLVKey IncompleteFooterKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x04, 0x02, 0x00};

static const OMKLVKey FooterKey =
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

static const OMKLVKey V10IndexTableSegmentKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x53, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x10, 0x00, 0x00};

static const OMKLVKey RandomIndexMetadataKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x11, 0x01, 0x00};

static const OMKLVKey SystemMetadataKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x03, 0x01, 0x04, 0x01, 0x01, 0x00};

static const OMByte EssenceElementPrefix[] =
  {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01};

static const OMByte HeaderPrefix[] =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01,
   0x01, 0x0d, 0x01, 0x02, 0x01, 0x01, 0x02};

static const OMUInt32 RunInLimit = 64 * 1024;

// Minimum size of a fill KLV triplet (when L not known a priori)
// K (16 bytes)
// L (1 + 8 bytes BER encoded)
// V (0 fill bytes)
static const OMUInt64 minimumFill = sizeof(OMKLVKey) + 1 + sizeof(OMUInt64);

// Coalesced with preceeding fill of any size.
// (value of fillBufferZoneSize must be > minimumFill)
//
static const OMUInt64 fillBufferZoneSize = 32;

static const OMUInt32 preallocatedMetadataSize = 0x40000;
static const OMUInt64 bodyPartitionOffset = preallocatedMetadataSize +
                                            fillBufferZoneSize;

static const OMUInt32 defaultKAGSize = 0x100;

static const OMUInt16 currentMajorVersion = 0x0001;
static const OMUInt16 currentMinorVersion = 0x0002;

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

// {01011502-0000-0000-060E-2B3401010101}
static const OMObjectIdentification Property_InterchangeObject_InstanceUID =
{0x01011502, 0x0000, 0x0000, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01}};

static const OMPropertyId PID_Root_ObjectDirectory = 0x003;
static const OMPropertyId PID_Root_FormatVersion   = 0x004;

// Incrementing the following value serves to distinguish between
// files with minor but compatible differences.  Incrementing the
// following value does not invalidate files.
//
// History
//
// 0x005 - First forwards and backwards compatible version
// 0x006 - Use closed and complete keys for body and footer
// 0x007 - Fill in index byte count in the footer
// 0x008 - Add InterchangeObject::InstanceUID to primer
static const OMUInt32 formatVersion = 0x008;

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

  static bool findHeader(const OMRawStorage* store, OMUInt64& headerPosition);

  static bool isHeader(const OMKLVKey& k);

  static bool isBody(const OMKLVKey& k);

  static bool isFooter(const OMKLVKey& k);

  static bool isPartition(const OMKLVKey& k);

  static bool isIndex(const OMKLVKey& k);

  static bool isEssence(const OMKLVKey& k);

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

  virtual void readRandomIndex(OMUInt64 length);

  virtual bool findRandomIndex(OMUInt64 fileSize,
                               OMUInt64& indexPosition);

  virtual bool isRandomIndex(OMUInt64 fileSize, OMUInt32 ripSize);

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

  virtual OMUInt64 reserveKLVLength(void);
  virtual void fixupKLVLength(const OMUInt64 lengthPosition);

  virtual OMUInt64 reserve(OMUInt32 size);
  virtual void fixup(const OMUInt64& patchOffset, const OMUInt8& patchValue);
  virtual void fixup(const OMUInt64& patchOffset, const OMUInt16& patchValue);
  virtual void fixup(const OMUInt64& patchOffset, const OMUInt32& patchValue);
  virtual void fixup(const OMUInt64& patchOffset, const OMUInt64& patchValue);

  virtual void writeBerLength(OMUInt32 lengthSize, const OMUInt64& length);

    // @cmember The minimum size of <p i> when encoded using <f berEncode>.
    //          The returned size includes the BER length byte.
  static size_t berEncodedSize(const OMUInt64 i);

    // @cmember BER encode <p value> into <p berValueBuffer>.
  static void berEncode(OMByte* berValueBuffer,
                        size_t berValueBufferSize,
                        const OMUInt32& berValueSize,
                        const OMUInt64& value);

  virtual void readPartition(OMUInt64 length,
                             OMUInt32& bodySID,
                             OMUInt32& indexSID,
                             OMUInt32& KAGSize);

  virtual void readPartition(OMUInt64 length,
                             OMUInt32& bodySID,
                             OMUInt32& indexSID,
                             OMUInt32& KAGSize,
                             OMUInt64& previousPartition,
                             OMUInt64& thisPartition,
                             OMUInt64& footerPartition);

  virtual void readHeaderPartition(void);

  static bool read(const OMRawStorage* store, OMUInt8& i);
  static bool read(const OMRawStorage* store, OMUInt16& i, bool reorderBytes);
  static bool read(const OMRawStorage* store, OMUInt32& i, bool reorderBytes);
  static bool read(const OMRawStorage* store, OMUInt64& i, bool reorderBytes);
  static bool read(const OMRawStorage* store, OMKLVKey& key);
  static bool readKLVLength(const OMRawStorage* store, OMUInt64& length);
  static bool skipBytes(const OMRawStorage* store, OMUInt64 length);

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

  virtual void skipV(OMUInt64 length) const;

  static OMUInt64 readBerLength(const OMRawStorage* store);


  // A weak reference in MXF file is stored as 16-byte InstanceUID of the
  // referenced object (see SMPTE 377M, Section 5.7.)
  // InstanceUID isn't currently part of the object model and cannot be
  // stored in OMReferenceProperty but it has to be preserved to resolve
  // (i.e. find the referenced object) the references once all the objects
  // were read from the file.
  // The following methods are used to save and access InstanceUID of the
  // referenced object for each restored weak reference property.
  //
    // @cmember Given pointer to a weak reference property return
    //          the InstanceUID of the referenced object.
  bool findReferencedInstanceId(const void* reference,
                                OMUniqueObjectIdentification* id);
    // @cmember Remember the InstanceUID of the object referenced
    //          by the specified weak reference property.
  void associate(const void* reference,
                 const OMUniqueObjectIdentification& referencedInstanceId);


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

  virtual void restoreObjectDirectory(OMUInt64 headerOffset);

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

  virtual void writeStreamAt(OMUInt32 sid,
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

  virtual void readStreamAt(OMUInt32 sid,
                            OMUInt64 position,
                            OMByte* bytes,
                            OMUInt32 byteCount,
                            OMUInt32& bytesRead);

  virtual void streamRestoreSegment(OMUInt32 sid,
                                    OMUInt64 start,
                                    OMUInt64 allocatedSize,
                                    OMUInt64 occupiedSize,
                                    OMKLVKey label,
                                    OMUInt32 gridSize);

  virtual void streamGrow(OMUInt32 sid, OMUInt64 growBytes);

  virtual void saveStreams(void);
  virtual void restoreStreams(void);

  virtual void checkStreams(void);

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

  static bool findPattern(const OMRawStorage* store,
                          OMUInt64 currentPosition,
                          OMUInt64& patternPosition,
                          const OMByte* pattern,
                          OMUInt64 patternSize,
                          OMUInt32 limit);

  void markMetadataStart(OMUInt64 primerKeyPosition);
  void markMetadataEnd(OMUInt64 endKeyPosition);

  void markIndexStart(OMKLVKey key,
                      OMUInt32 sid,
                      OMUInt32 gridSize,
                      OMUInt64 indexKeyPosition);
  void markIndexEnd(OMUInt64 endKeyPosition);

  void markEssenceSegmentStart(OMKLVKey key,
                               OMUInt32 sid,
                               OMUInt32 gridSize,
                               OMUInt64 essenceKeyPosition);
  void markEssenceSegmentEnd(OMUInt64 endKeyPosition);

  void markFill(OMUInt64 fillKeyPosition, OMUInt64 fillEndPosition);

  OMUInt32 _gridSize;

  OMUInt64 _primerPosition;
  OMUInt64 _headerByteCount;

  bool _inIndex;
  OMUInt32 _indexSID;
  OMKLVKey _indexKey;
  OMUInt64 _indexPosition;

  bool _inEssence;
  OMUInt32 _essenceSID;
  OMKLVKey _essenceKey;
  OMUInt64 _essencePosition;

  OMUInt64 _fillStart;
  OMUInt64 _fillEnd;

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
  OMSet<const void*, OMUniqueObjectIdentification>* _referenceToInstanceId;
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
  virtual OMUInt64 validSize(Segment* segment);

    // debugging
  void printPartitions(void);
  void printStreams(void);

  typedef OMSet<OMUInt32, Stream*> SegmentMap;
  typedef OMSetIterator<OMUInt32, Stream*> SegmentMapIterator;
  virtual SegmentMap* segmentMap(void);
  void destroySegmentMap(void);
  SegmentMap* _segmentMap;
  SegmentList* _segments;
  OMUInt64 _fileSize;

  struct Partition {
    OMUInt64 _address;
    OMUInt32 _bodySID;
    OMUInt64 _previous;
    OMUInt32 _indexSID;
  };
  void destroyPartitions(void);
  bool findPartition(OMUInt64 address, OMUInt32& index);
  void addPartition(OMUInt64 address,  OMUInt32 bodySID, OMUInt32 index);
  OMVector<Partition*> _partitions;
  OMUInt64 _primerOffset;
  OMUInt64 _metadataEnd;
};

#endif

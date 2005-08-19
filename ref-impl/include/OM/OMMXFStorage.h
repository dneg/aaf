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

static const OMKLVKey ClosedHeaderPartitionPackKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x02, 0x02, 0x00};

static const OMKLVKey ClosedBodyPartitionPackKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x02, 0x00};

static const OMKLVKey ClosedFooterPartitionPackKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x04, 0x02, 0x00};

static const OMKLVKey primerPackKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x02, 0x05, 0x01, 0x01,
   0x0d, 0x01, 0x02, 0x01, 0x01, 0x05, 0x01, 0x00};

static const OMKLVKey fillKey =
  {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0x01,
   0x03, 0x01, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00};

static const OMUInt32 bodyPartitionOffset = 0x20000;

static const OMUInt32 KAGSize = 0x100;

static const OMUInt16 currentMajorVersion = 0xffff;
static const OMUInt16 currentMinorVersion = 0xfffb;

static const OMPropertyId PID_InterchangeObject_InstanceUID = 0x3c0a;

static const OMPropertyId PID_Root_ObjectDirectory = 0x003;
static const OMPropertyId PID_Root_FormatVersion   = 0x004;

static const OMUInt32 formatVersion = 0x004;

static const OMKLVKey objectDirectoryKey =
  {0x96, 0x13, 0xb3, 0x8a, 0x87, 0x34, 0x87, 0x46,
   0xf1, 0x02, 0x96, 0xf0, 0x56, 0xe0, 0x4d, 0x2a};


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

  struct ObjectDirectoryEntry {
    OMStorable* _object;
    OMUInt64 _offset;
    OMUInt8 _flags;
  };

  typedef OMSet<OMUniqueObjectIdentification,
                ObjectDirectoryEntry> ObjectDirectory;
  typedef OMSetIterator<OMUniqueObjectIdentification,
                        ObjectDirectoryEntry> ObjectDirectoryIterator;

  ObjectDirectory* instanceIdToObject(void);

  typedef OMSet<OMStorable*, OMUniqueObjectIdentification> ObjectSet;

  ObjectSet* objectToInstanceId(void);

private:
  // @access Private members.

  OMKLVKey _operationalPattern;
  LabelSet _essenceContainerLabels;
  ObjectDirectory* _instanceIdToObject;
  ObjectSet* _objectToInstanceId;

};

#endif

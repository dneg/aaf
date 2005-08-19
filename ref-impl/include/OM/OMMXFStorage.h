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

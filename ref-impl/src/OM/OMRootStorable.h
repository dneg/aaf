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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMROOTSTORABLE_H
#define OMROOTSTORABLE_H

#include "OMStorable.h"
#include "OMStrongRefProperty.h"
#include "OMDictionary.h"

  // @class Concrete sub-class of the abstract OMStorable for use as the
  //        root object in a file that may be managed by the Object Manager.
  //   @base public | <c OMStorable>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMRootStorable : public OMStorable {
public:

  OMRootStorable(void);

  OMRootStorable(OMStorable* clientRoot,
                 OMDictionary* dictionary);

  ~OMRootStorable(void);

  // OMStorable overrides

  virtual const OMClassId& classId(void) const;

    // @cmember Save this <c OMRootStorable>.
  virtual void save(void) const;

    // @cmember Close this <c OMRootStorable>.
  virtual void close(void);

    // @cmember Restore the contents of an <c OMRootStorable>.
  virtual void restoreContents(void);

    // @cmember The <c OMFile> in which this <c OMRootStorable> has a
    //          persistent representation.
  virtual OMFile* file(void) const;

    // @cmember Is this <c OMRootStorable> the root of the object
    //          containment hierarchy.
  virtual bool isRoot(void) const;

    // @cmember Is this <c OMRootStorable> associated with an <c OMFile> ?
  virtual bool inFile(void) const;

    // @cmember Is this <c OMRootStorable> a persistent object ?
    //          Persistent objects are associated with a persistent
    //          store (disk file).
  virtual bool persistent(void) const;

  // OMRootStorable functions

  void attach(OMFile* file);

  OMStorable* clientRoot(void) const;

  OMDictionary* dictionary(void) const;

  static const OMClassId _rootClassId;

private:

  void initialize(OMStorable* clientRoot,
                  OMDictionary* dictionary);

  OMStrongReferenceProperty<OMStorable> _clientRoot;
  OMStrongReferenceProperty<OMDictionary> _dictionary;
  OMFile* _file;

};

#endif

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMREFERENCEVECTOR_H
#define OMREFERENCEVECTOR_H

#include "OMVector.h"
#include "OMContainerElement.h"
#include "OMObjectVector.h"

template <typename ReferencedObject>
class OMReferenceVectorIterator;

template <typename Element>
class OMVectorIterator;

  // @class Elastic sequential collections of objects supported by
  //        the Object Manager.
  //        Objects are accessible by index. The order of objects is
  //        determined externally. Duplicate objects are allowed.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @base public | <c OMObjectVector>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMReferenceVector : public OMObjectVector {
public:
  // @access Public members.

    // @cmember Constructor.
  OMReferenceVector(void);

    // @cmember Destructor.
  virtual ~OMReferenceVector(void);

    // @cmember The number of <p ReferencedObject>s in this
    //          <c OMReferenceVector>.
  OMUInt32 count(void) const;

    // @cmember Set the value of this <c OMReferenceVector>
    //          at position <p index> to <p object>.
  ReferencedObject* setValueAt(const ReferencedObject* object,
                               const OMUInt32 index);

    // @cmember Set the value of this <c OMReferenceVector>
    //          at position <p index> to 0.
  ReferencedObject* clearValueAt(const OMUInt32 index);

    // @cmember The value of this <c OMReferenceVector> at position <p index>.
  ReferencedObject* valueAt(const OMUInt32 index) const;

    // @cmember Get the value of this <c OMReferenceVector>
    //          at position <p index> into <p object>.
  void getValueAt(ReferencedObject*& object, const OMUInt32 index) const;

    // @cmember If <p index> is valid, get the value of this
    //          <c OMReferenceVector> at position <p index>
    //          into <p object> and return true, otherwise return false.
  bool find(const OMUInt32 index, ReferencedObject*& object) const;

    // @cmember Append the given <p ReferencedObject> <p object> to
    //          this <c OMReferenceVector>.
  void appendValue(const ReferencedObject* object);

    // @cmember Prepend the given <p ReferencedObject> <p object> to
    //          this <c OMReferenceVector>.
  void prependValue(const ReferencedObject* object);

    // @cmember Insert <p object> into this <c OMReferenceVector>.
  void insert(const ReferencedObject* object);

    // @cmember Insert <p object> into this <c OMReferenceVector>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  void insertAt(const ReferencedObject* object, const OMUInt32 index);

    // @cmember Does this <c OMReferenceVector> contain <p object> ?
  bool containsValue(const ReferencedObject* object) const;

    // @cmember Remove <p object> from this <c OMReferenceVector>.
  void removeValue(const ReferencedObject* object);

    // @cmember Remove the object from this
    //          <c OMReferenceVector> at position <p index>.
    //          Existing objects in this <c OMReferenceVector>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  ReferencedObject* removeAt(const OMUInt32 index);

    // @cmember Remove the last (index == count() - 1) object
    //          from this <c OMReferenceVector>.
  ReferencedObject* removeLast(void);

    // @cmember Remove the first (index == 0) object
    //          from this <c OMReferenceVector>. Existing
    //          objects in this <c OMReferenceVector> are
    //          shifted down one index position.
  ReferencedObject* removeFirst(void);

    // @cmember The index of the <p ReferencedObject*> <p object>.
  OMUInt32 indexOfValue(const ReferencedObject* object) const;

    // @cmember The number of occurrences of <p object> in this
    //          <c OMReferenceVector>.
  OMUInt32 countOfValue(const ReferencedObject* object) const;

    // @cmember Does this <c OMReferenceVector> contain
    //          <p index> ? Is <p index> valid ?
  bool containsIndex(const OMUInt32 index) const;

    // @cmember If this <c OMStrongReferenceProperty> contains <p object>
    //          then place its index in <p index> and return true, otherwise
    //          return false.
  bool findIndex(const ReferencedObject* object, OMUInt32& index) const;

    // @cmember Increase the capacity of this
    //          <c OMReferenceVector> so that it
    //          can contain at least <p capacity> <p ReferencedObject>s
    //          without having to be resized.
  void grow(const OMUInt32 capacity);

    // @cmember Insert <p object> into this <c OMReferenceVector>.
  virtual void insertObject(const OMObject* object);

    // @cmember Does this <c OMReferenceVector> contain <p object> ?
  virtual bool containsObject(const OMObject* object) const;

    // @cmember Remove <p object> from this <c OMReferenceVector>.
  virtual void removeObject(const OMObject* object);

    // @cmember Remove all objects from this <c OMReferenceVector>.
  virtual void removeAllObjects(void);

    // @cmember Create an <c OMReferenceContainerIterator> over this
    //          <c OMReferenceVector>.
  virtual OMReferenceContainerIterator* createIterator(void) const;

    // @cmember Set the value of this <c OMReferenceVector>
    //          at position <p index> to <p object>.
  virtual OMObject* setObjectAt(const OMObject* object,
                                const OMUInt32 index);

    // @cmember The value of this <c OMReferenceVector>
    //          at position <p index>.
  virtual OMObject* getObjectAt(const OMUInt32 index) const;

    // @cmember Append the given <p OMObject> <p object> to
    //          this <c OMReferenceVector>.
  virtual void appendObject(const OMObject* object);

    // @cmember Prepend the given <p OMObject> <p object> to
    //          this <c OMReferenceVector>.
  virtual void prependObject(const OMObject* object);

    // @cmember Remove the object from this
    //          <c OMReferenceVector> at position <p index>.
    //          Existing objects in this <c OMReferenceVector>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  virtual OMObject* removeObjectAt(const OMUInt32 index);

    // @cmember Insert <p object> into this <c OMReferenceVector>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  virtual void insertObjectAt(const OMObject* object, const OMUInt32 index);

private:

  typedef OMVectorElement<ReferencedObject> VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

    // The vector of references.
  OMVector<VectorElement> _vector;

  friend class OMReferenceVectorIterator<ReferencedObject>;

};

#include "OMReferenceVectorT.h"

#endif

/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

// @doc OMEXTERNAL
#ifndef OMWEAKREFVECTORPROPERTY_H
#define OMWEAKREFVECTORPROPERTY_H

#include "OMVector.h"
#include "OMContainerElement.h"
#include "OMWeakReferenceVector.h"

template <typename ReferencedObject>
class OMWeakReferenceVectorIterator;

template <typename Element>
class OMVectorIterator;

  // @class Persistent elastic sequential collections of uniquely identified
  //        weakly referenced (non-contained) objects supported by the
  //        Object Manager.
  //        Objects are accessible by index. The order of objects is
  //        determined externally. Duplicate objects are allowed.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMWeakReferenceVector>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMWeakReferenceVectorProperty : public OMWeakReferenceVector {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceVectorProperty(const OMPropertyId propertyId,
                                const wchar_t* name,
                                const wchar_t* targetName,
                                const OMPropertyId keyPropertyId);

    // @cmember Constructor.
  OMWeakReferenceVectorProperty(const OMPropertyId propertyId,
                                const wchar_t* name,
                                const OMPropertyId keyPropertyId,
                                const OMPropertyId* targetPropertyPath);
    // @cmember Destructor.
  virtual ~OMWeakReferenceVectorProperty(void);

    // @cmember Save this <c OMWeakReferenceVectorProperty>.
  virtual void save(void) const;

    // @cmember Close this <c OMWeakReferenceVectorProperty>.
  virtual void close(void);

    // @cmember Detach this <c OMWeakReferenceVectorProperty>.
  virtual void detach(void);

    // @cmember Restore this <c OMWeakReferenceVectorProperty>, the
    //          external (persisted) size of the
    //          <c OMWeakReferenceVectorProperty> is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The number of <p ReferencedObject>s in this
    //          <c OMWeakReferenceVectorProperty>.
  size_t count(void) const;

    // @cmember Set the value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index> to <p object>.
  ReferencedObject* setValueAt(const ReferencedObject* object,
                               const size_t index);

    // @cmember Set the value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index> to 0.
  ReferencedObject* clearValueAt(const size_t index);

    // @cmember The value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index>.
  ReferencedObject* valueAt(const size_t index) const;

    // @cmember Get the value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index> into <p object>.
  void getValueAt(ReferencedObject*& object, const size_t index) const;

    // @cmember If <p index> is valid, get the value of this
    //          <c OMWeakReferenceVectorProperty> at position <p index>
    //          into <p object> and return true, otherwise return false.
  bool find(const size_t index, ReferencedObject*& object) const;

    // @cmember Append the given <p ReferencedObject> <p object> to
    //          this <c OMWeakReferenceVectorProperty>.
  void appendValue(const ReferencedObject* object);

    // @cmember Prepend the given <p ReferencedObject> <p object> to
    //          this <c OMWeakReferenceVectorProperty>.
  void prependValue(const ReferencedObject* object);

    // @cmember Insert <p object> into this
    //          <c OMWeakReferenceVectorProperty>. This function is
    //          redefined from <c OMContainerProperty> as
    //          <mf OMWeakReferenceVectorProperty::appendValue>.
  void insert(const ReferencedObject* object);

    // @cmember Insert <p object> into this <c OMWeakReferenceVectorProperty>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  void insertAt(const ReferencedObject* object, const size_t index);

    // @cmember Does this <c OMWeakReferenceVectorProperty> contain
    //          <p object> ?
  bool containsValue(const ReferencedObject* object) const;

    // @cmember Remove <p object> from this
    //          <c OMWeakReferenceVectorProperty>.
  void removeValue(const ReferencedObject* object);

    // @cmember Remove the object from this
    //          <c OMWeakReferenceVectorProperty> at position <p index>.
    //          Existing objects in this <c OMWeakReferenceVectorProperty>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  ReferencedObject* removeAt(const size_t index);

    // @cmember Remove the last (index == count() - 1) object
    //          from this <c OMWeakReferenceVectorProperty>.
  ReferencedObject* removeLast(void);

    // @cmember Remove the first (index == 0) object
    //          from this <c OMWeakReferenceVectorProperty>. Existing
    //          objects in this <c OMWeakReferenceVectorProperty> are
    //          shifted down one index position.
  ReferencedObject* removeFirst(void);

    // @cmember The index of the <p ReferencedObject*> <p object>.
  size_t indexOfValue(const ReferencedObject* object) const;

    // @cmember The number of occurrences of <p object> in this
    //          <c OMWeakReferenceVectorProperty>.
  size_t countOfValue(const ReferencedObject* object) const;

    // @cmember Does this <c OMWeakReferenceVectorProperty> contain
    //          <p index> ? Is <p index> valid ?
  bool containsIndex(const size_t index) const;

    // @cmember If this <c OMWeakReferenceProperty> contains <p object>
    //          then place its index in <p index> and return true, otherwise
    //          return false.
  bool findIndex(const ReferencedObject* object, size_t& index) const;

    // @cmember Increase the capacity of this
    //          <c OMWeakReferenceVectorProperty> so that it
    //          can contain at least <p capacity> <p ReferencedObject>s
    //          without having to be resized.
  void grow(const size_t capacity);

  // Optional property interface

    // @cmember Is this <c OMWeakReferenceVectorProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Remove this optional <c OMWeakReferenceVectorProperty>.
  virtual void removeProperty(void);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMWeakReferenceVectorProperty>. The size is given
    //          in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this
    //          <c OMWeakReferenceVectorProperty>. The raw bits are
    //          copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this
    //          <c OMWeakReferenceVectorProperty>. The raw bits are
    //          copied from the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

    // @cmember Insert <p object> into this
    //          <c OMWeakReferenceVectorProperty>.
  virtual void insertObject(const OMObject* object);

    // @cmember Does this <c OMWeakReferenceVectorProperty> contain
    //          <p object> ?
  virtual bool containsObject(const OMObject* object) const;

    // @cmember Remove <p object> from this
    //          <c OMWeakReferenceVectorProperty>.
  virtual void removeObject(const OMObject* object);

    // @cmember Remove all objects from this
    //          <c OMWeakReferenceVectorProperty>.
  virtual void removeAllObjects(void);

    // @cmember Create an <c OMReferenceContainerIterator> over this
    //          <c OMWeakReferenceVectorProperty>.
  virtual OMReferenceContainerIterator* createIterator(void) const;

    // @cmember Set the value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index> to <p object>.
  virtual OMObject* setObjectAt(const OMObject* object,
                                const size_t index);

    // @cmember The value of this <c OMWeakReferenceVectorProperty>
    //          at position <p index>.
  virtual OMObject* getObjectAt(const size_t index) const;

    // @cmember Append the given <p OMObject> <p object> to
    //          this <c OMWeakReferenceVectorProperty>.
  virtual void appendObject(const OMObject* object);

    // @cmember Prepend the given <p OMObject> <p object> to
    //          this <c OMWeakReferenceVectorProperty>.
  virtual void prependObject(const OMObject* object);

    // @cmember Remove the object from this
    //          <c OMWeakReferenceVectorProperty> at position <p index>.
    //          Existing objects in this <c OMWeakReferenceVectorProperty>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  virtual OMObject* removeObjectAt(const size_t index);

    // @cmember Insert <p object> into this <c OMWeakReferenceVectorProperty>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  virtual void insertObjectAt(const OMObject* object, const size_t index);

  virtual OMContainerIterator<OMWeakReferenceVectorElement>*
                                                          iterator(void) const;

  virtual void insert(const size_t index,
                      const OMWeakReferenceVectorElement& element);

  virtual OMPropertyId keyPropertyId(void) const;

  virtual OMPropertyTag targetTag(void) const;

  virtual void setTargetTag(OMPropertyTag targetTag);

  virtual void clearTargetTag(void) const;

private:

  typedef OMWeakReferenceVectorElement VectorElement;

  typedef OMVectorIterator<VectorElement> VectorIterator;

  OMPropertyId* targetPropertyPath(void) const;

    // The vector of references.
  OMVector<VectorElement> _vector;
  OMPropertyTag _targetTag;
  const wchar_t* _targetName;
  OMPropertyId* _targetPropertyPath;
  OMPropertyId _keyPropertyId;

  friend class OMWeakReferenceVectorIterator<ReferencedObject>;

};

#include "OMWeakRefVectorPropertyT.h"

#endif

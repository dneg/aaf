/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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
#ifndef OMWEAKREFSETPROPERTY_H
#define OMWEAKREFSETPROPERTY_H

#include "OMSet.h"
#include "OMContainerElement.h"
#include "OMContainerProperty.h"
#include "OMDataTypes.h"

template <typename ReferencedObject>
class OMWeakReferenceSetIterator;

template <typename Key, typename Element>
class OMSetIterator;

  // @class Persistent sets of uniquely identified weakly referenced
  //        (non-contained) objects supported by the Object Manager.
  //        Objects are accessible by unique identifier (the key).
  //        The objects are not ordered. Duplicates objects are not allowed.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and of <c OMUnique>.
  //   @base public | <c OMContainerProperty>
template <typename ReferencedObject>
class OMWeakReferenceSetProperty :
                                 public OMContainerProperty<ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceSetProperty(const OMPropertyId propertyId,
                             const char* name,
                             const char* targetName);

    // @cmember Destructor.
  virtual ~OMWeakReferenceSetProperty(void);

    // @cmember Save this <c OMWeakReferenceSetProperty>.
  virtual void save(void* clientContext) const;

    // @cmember Close this <c OMWeakReferenceSetProperty>.
  virtual void close(void);

    // @cmember Detach this <c OMWeakReferenceSetProperty>.
  virtual void detach(void);

    // @cmember Restore this <c OMWeakReferenceSetProperty>, the
    //          external (persisted) size of the
    //          <c OMWeakReferenceSetProperty> is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The number of <p ReferencedObject>s in this
    //          <c OMWeakReferenceSetProperty>.
  size_t count(void) const;

    // @cmember Get the size of this <c OMWeakReferenceSetProperty>.
  size_t getSize(void) const;

    // @cmember Insert <p object> into this
    //          <c OMWeakReferenceSetProperty>.
  void insert(const ReferencedObject* object);

    // @cmember If it is not already present, insert <p object> into this
    //          <c OMWeakReferenceSetProperty> and return true,
    //          otherwise return false.
  bool ensurePresent(const ReferencedObject* object);

    // @cmember Append the given <p ReferencedObject> <p object> to
    //          this <c OMWeakReferenceSetProperty>.
  void appendValue(const ReferencedObject* object);

    // @cmember Remove the <p ReferencedObject> identified by
    //          <p identification> from this <c OMWeakReferenceSetProperty>.
  ReferencedObject* remove(const OMUniqueObjectIdentification& identification);

    // @cmember If it is present, remove the <p ReferencedObject> identified by
    //          <p identification> from this <c OMWeakReferenceSetProperty>
    //          and return true, otherwise return false.
  bool ensureAbsent(const OMUniqueObjectIdentification& identification);

    // @cmember Remove <p object> from this
    //          <c OMWeakReferenceSetProperty>.
  void removeValue(const ReferencedObject* object);

    // @cmember If it is present, remove <p object> from this
    //          <c OMWeakReferenceSetProperty> and return true,
    //          otherwise return false.
  bool ensureAbsent(const ReferencedObject* object);

    // @cmember Does this <c OMWeakReferenceSetProperty> contain
    //          <p object> ?
  bool containsValue(const ReferencedObject* object) const;

    // @cmember Does this <c OMWeakReferenceSetProperty> contain a
    //          <p ReferencedObject> identified by <p identification>?
  virtual bool contains(
                     const OMUniqueObjectIdentification& identification) const;

    // @cmember The <p ReferencedObject> in this
    //          <c OMWeakReferenceSetProperty> identified by
    //          <p identification>.
  ReferencedObject* value(
                     const OMUniqueObjectIdentification& identification) const;
  
    // @cmember Find the <p ReferencedObject> in this
    //          <c OMWeakReferenceSetProperty> identified by
    //          <p identification>.  If the object is found it is returned
    //          in <p object> and the result is true. If the element is
    //          not found the result is false.
  virtual bool find(const OMUniqueObjectIdentification& identification,
                    ReferencedObject*& object) const;

  // Optional property interface

    // @cmember Is this <c OMWeakReferenceSetProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Remove this optional <c OMWeakReferenceSetProperty>.
  virtual void remove(void);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMWeakReferenceSetProperty>. The size is given
    //          in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this
    //          <c OMWeakReferenceSetProperty>. The raw bits are
    //          copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this
    //          <c OMWeakReferenceSetProperty>. The raw bits are
    //          copied from the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

private:

  typedef OMWeakReferenceSetElement<ReferencedObject> SetElement;

  typedef OMSetIterator<OMUniqueObjectIdentification, SetElement> SetIterator;

  // The set of references.
  OMSet<OMUniqueObjectIdentification, SetElement> _set;
  OMPropertyTag _targetTag;
  char* _targetName;

  friend class OMWeakReferenceSetIterator<ReferencedObject>;

};

#include "OMWeakRefSetPropertyT.h"

#endif

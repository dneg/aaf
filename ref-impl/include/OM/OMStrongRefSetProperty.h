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
#ifndef OMSTRONGREFSETPROPERTY_H
#define OMSTRONGREFSETPROPERTY_H

#include "OMSet.h"
#include "OMContainerElement.h"
#include "OMContainerProperty.h"

template <typename ReferencedObject>
class OMStrongReferenceSetIterator;

  // @class Persistent sets of uniquely identified strongly referenced
  //        (contained) objects supported by the Object Manager.
  //        Objects are accessible by unique identifier (the key).
  //        The objects are not ordered. Duplicates objects are not allowed.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable> and of <c OMUnique>.
  //   @base public | <c OMContainerProperty>
template <typename ReferencedObject>
class OMStrongReferenceSetProperty :
                                 public OMContainerProperty<ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceSetProperty(const OMPropertyId propertyId,
                               const char* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceSetProperty(void);

    // @cmember Save this <c OMStrongReferenceSetProperty>.
  virtual void save(void* clientContext) const;

    // @cmember Close this <c OMProperty>.
  virtual void close(void);

    // @cmember Restore this <c OMStrongReferenceSetProperty>, the
    //          external (persisted) size of the
    //          <c OMStrongReferenceSetProperty> is <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The number of <p ReferencedObject>s in this
    //          <c OMStrongReferenceSetProperty>.
  size_t count(void) const;

    // @cmember Get the size of this <c OMStrongReferenceSetProperty>.
  size_t getSize(void) const;

  // Set access functions go here

  void insert(const ReferencedObject* object);

    // @cmember Append the given <p ReferencedObject> <p value> to
    //          this <c OMStrongReferenceSetProperty>.
  void appendValue(const ReferencedObject*& value);

    // @cmember Remove <p object> from this
    //          <c OMStrongReferenceSetProperty>.
  void removeValue(const ReferencedObject* object);

    // @cmember Does this <c OMStrongReferenceSetProperty> contain
    //          <p object> ?
  bool containsValue(const ReferencedObject* object) const;

  // Optional property interface

    // @cmember Remove this optional <c OMStrongReferenceSetProperty>.
  virtual void remove(void);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMStrongReferenceSetProperty>. The size is given
    //          in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the raw bits of this
    //          <c OMStrongReferenceSetProperty>. The raw bits are
    //          copied to the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this
    //          <c OMStrongReferenceSetProperty>. The raw bits are
    //          copied from the buffer at address <p bits> which is
    //          <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

private:

  // The set of references.
  OMSet<OMUniqueObjectIdentification,
        OMSetElement<OMStrongObjectReference<ReferencedObject>,
                     ReferencedObject> > _set;

  friend class OMStrongReferenceSetIterator<ReferencedObject>;

};

#include "OMStrongRefSetPropertyT.h"

#endif

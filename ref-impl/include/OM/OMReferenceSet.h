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
#ifndef OREFERENCESET_H
#define OMREFERENCESET_H

#include "OMSet.h"
#include "OMContainerElement.h"
#include "OMObjectSet.h"

template <typename UniqueIdentification, typename ReferencedObject>
class OMReferenceSetIterator;

template <typename UniqueIdentification, typename ReferencedObject>
class OMSetElement;

  // @class Sets of uniquely identified objects supported by the
  //        Object Manager.
  //        Objects are accessible by unique identifier (the key).
  //        The objects are not ordered. Duplicates objects are not allowed.
  //   @tcarg class | ReferencedObject | The type of the referenced objects.
  //   @tcarg class | UniqueIdentification | The type of the unique key
  //          used to identify the referenced objects.
  //   @base public | <c OMReferenceContainer>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename UniqueIdentification, typename ReferencedObject>
class OMReferenceSet : public OMObjectSet {
public:
  // @access Public members.

    // @cmember Constructor.
  OMReferenceSet(void);

    // @cmember Destructor.
  virtual ~OMReferenceSet(void);

    // @cmember The number of <p ReferencedObject>s in this
    //          <c OMReferenceSet>.
  size_t count(void) const;

    // @cmember Insert <p object> into this
    //          <c OMReferenceSet>.
  void insert(const ReferencedObject* object);

    // @cmember If it is not already present, insert <p object> into this
    //          <c OMReferenceSet> and return true, otherwise return false.
  bool ensurePresent(const ReferencedObject* object);

    // @cmember Append the given <p ReferencedObject> <p object> to
    //          this <c OMReferenceSet>.
  void appendValue(const ReferencedObject* object);

    // @cmember Remove the <p ReferencedObject> identified by
    //          <p identification> from this <c OMReferenceSet>.
  ReferencedObject* remove(const UniqueIdentification& identification);

    // @cmember If it is present, remove the <p ReferencedObject> identified by
    //          <p identification> from this <c OMReferenceSet>
    //          and return true, otherwise return false.
  bool ensureAbsent(const UniqueIdentification& identification);

    // @cmember Remove <p object> from this <c OMReferenceSet>.
  void removeValue(const ReferencedObject* object);

    // @cmember If it is present, remove <p object> from this
    //          <c OMReferenceSet> and return true,
    //          otherwise return false.
  bool ensureAbsent(const ReferencedObject* object);

    // @cmember Does this <c OMReferenceSet> contain <p object> ?
  bool containsValue(const ReferencedObject* object) const;

    // @cmember Does this <c OMReferenceSet> contain a
    //          <p ReferencedObject> identified by <p identification>?
  virtual bool contains(const UniqueIdentification& identification) const;

    // @cmember The <p ReferencedObject> in this
    //          <c OMReferenceSet> identified by
    //          <p identification>.
  ReferencedObject* value(const UniqueIdentification& identification) const;

    // @cmember Find the <p ReferencedObject> in this
    //          <c OMReferenceSet> identified by
    //          <p identification>.  If the object is found it is returned
    //          in <p object> and the result is true. If the element is
    //          not found the result is false.
  virtual bool find(const UniqueIdentification& identification,
                    ReferencedObject*& object) const;

  bool isValidIdentification(UniqueIdentification& id) const;

    // @cmember Insert <p object> into this <c OMReferenceSet>.
  virtual void insertObject(const OMObject* object);

    // @cmember Does this <c OMReferenceSet> contain <p object> ?
  virtual bool containsObject(const OMObject* object) const;

    // @cmember Remove <p object> from this <c OMReferenceSet>.
  virtual void removeObject(const OMObject* object);

    // @cmember Remove all objects from this <c OMReferenceSet>.
  virtual void removeAllObjects(void);

    // @cmember Create an <c OMReferenceContainerIterator> over this
    //          <c OMReferenceSet>.
  virtual OMReferenceContainerIterator* createIterator(void) const;

    // @cmember Remove the <c OMObject> identified by <p identification>
    //          from this <c OMReferenceSet>.
  virtual OMObject* remove(void* identification);

    // @cmember Does this <c OMReferenceSet> contain an
    //          <c OMObject> identified by <p identification> ?
  virtual bool contains(void* identification) const;

    // @cmember Find the <c OMObject> in this <c OMReferenceSet>
    //          identified by <p identification>.  If the object is found
    //          it is returned in <p object> and the result is < e bool.true>.
    //          If the object is not found the result is <e bool.false>.
  virtual bool findObject(void* identification, OMObject*& object) const;

private:

  typedef OMSetElement<UniqueIdentification, ReferencedObject> SetElement;

  typedef OMSetIterator<UniqueIdentification, SetElement> SetIterator;

  // The set of references.
  OMSet<UniqueIdentification, SetElement> _set;

  friend class OMReferenceSetIterator<UniqueIdentification,
                                      ReferencedObject>;

};

#include "OMReferenceSetT.h"

#endif

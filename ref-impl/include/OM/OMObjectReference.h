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

// @doc OMINTERNAL
#ifndef OMOBJECTREFERENCE_H
#define OMOBJECTREFERENCE_H

#include "OMDataTypes.h"

class OMProperty;
class OMStorable;

  // @class Persistent references to persistent objects.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMObjectReference {
public:
  // @access Public members.

    // @cmember Constructor.
  OMObjectReference(void);

    // @cmember Constructor.
  OMObjectReference(OMProperty* property);

    // @cmember Copy constructor.
  OMObjectReference(const OMObjectReference&);

    // @cmember Destructor.
  virtual ~OMObjectReference(void);

    // @cmember Is this <c OMObjectReference> void ?
  virtual bool isVoid(void) const;

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide assignment of object references.
  OMObjectReference& operator= (const OMObjectReference& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMObjectReference& rhs) const;

    // @cmember Save this <c OMObjectReference>.
  virtual void save(void) const = 0;

    // @cmember Close this <c OMObjectReference>.
  virtual void close(void) = 0;

    // @cmember Detach this <c OMObjectReference>.
  virtual void detach(void) = 0;

    // @cmember Restore this <c OMObjectReference>.
  virtual void restore(void) = 0;

    // @cmember Get the value of this <c OMObjectReference>.
    //          The value is a pointer to the referenced <c OMStorable>.
  virtual OMStorable* getValue(void) const = 0;

    // @cmember Set the value of this <c OMObjectReference>.
    //          The value is a pointer to the referenced <c OMStorable>.
  virtual OMStorable* setValue(const OMStorable* value) = 0;

    // @cmember The value of this <c OMObjectReference> as a pointer.
    //          This function provides low-level access. If the object exits
    //          but has not yet been loaded then the value returned is 0.
  virtual OMStorable* pointer(void) const;

protected:
  // @access Protected members.

    // @cmember The containing property.
  OMProperty* _property;

    // @cmember A pointer to the actual object.
  OMStorable* _pointer;

};

  // @class Persistent strong references to persistent objects.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMObjectReference>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMStrongObjectReference : public OMObjectReference {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongObjectReference(void);

    // @cmember Constructor.
  OMStrongObjectReference(OMProperty* property, const wchar_t* name);

    // @cmember Copy constructor.
  OMStrongObjectReference(const OMStrongObjectReference&);

    // @cmember Destructor.
  virtual ~OMStrongObjectReference(void);

    // @cmember Is this <c OMStrongObjectReference> void ?
  virtual bool isVoid(void) const;

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide assignment of object references.
  OMStrongObjectReference<ReferencedObject>& operator=
                        (const OMStrongObjectReference<ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMStrongObjectReference<ReferencedObject>& rhs) const;

    // @cmember Save this <c OMStrongObjectReference>.
  virtual void save(void) const;

    // @cmember Close this <c OMStrongObjectReference>.
  virtual void close(void);

    // @cmember Detach this <c OMStrongObjectReference>.
  virtual void detach(void);

    // @cmember Restore this <c OMStrongObjectReference>.
  virtual void restore(void);

    // @cmember Get the value of this <c OMStrongObjectReference>.
    //          The value is a pointer to the referenced <c OMStorable>.
  virtual OMStorable* getValue(void) const;

    // @cmember Set the value of this <c OMStrongObjectReference>.
    //          The value is a pointer to the <c ReferencedObject>.
  virtual OMStorable* setValue(const OMStorable* value);

protected:
  // @access Protected members.

    // @cmember Is this <c OMStrongObjectReference> in the loaded state.
    //          If false there is a persisted representation of this
    //          <c OMStrongObjectReference> that can be loaded.
  bool isLoaded(void) const;

    // @cmember Put this <c OMStrongObjectReference> into the loaded state.
  void setLoaded(void);

    // @cmember Remove this <c OMStrongObjectReference> from the loaded state.
  void clearLoaded(void);

    // @cmember Load the persisted representation of this
    //          <c OMStrongObjectReference>.
  virtual void load(void);

    // @cmember The state of this <c OMStrongObjectReference>. This is false
    //          if a persisted representation of this element exists that
    //          has not yet been loaded, true otherwise.
  bool _isLoaded;

    // @cmember The name of this <c OMStrongObjectReference>.
  wchar_t* _name;

};

template <typename UniqueIdentification, typename ReferencedObject>
class OMStrongReferenceSetProperty;

  // @class Persistent weak references to persistent objects.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @base public | <c OMObjectReference>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMWeakObjectReference : public OMObjectReference {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakObjectReference(void);

    // @cmember Constructor.
  OMWeakObjectReference(OMProperty* property);

    // @cmember Constructor.
  OMWeakObjectReference(
                    OMProperty* property,
                    OMUniqueObjectIdentification identification,
                    OMPropertyTag targetTag);

    // @cmember Copy constructor.
  OMWeakObjectReference(const OMWeakObjectReference&);

    // @cmember Destructor.
  virtual ~OMWeakObjectReference(void);

    // @cmember Is this <c OMWeakObjectReference> void ?
  virtual bool isVoid(void) const;

    // @cmember Assignment.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide assignment of object references.
  OMWeakObjectReference<ReferencedObject>& operator=
                          (const OMWeakObjectReference<ReferencedObject>& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMWeakObjectReference<ReferencedObject>& rhs) const;

    // @cmember Save this <c OMWeakObjectReference>.
  virtual void save(void) const;

    // @cmember Close this <c OMWeakObjectReference>.
  virtual void close(void);

    // @cmember Detach this <c OMWeakObjectReference>.
  virtual void detach(void);

    // @cmember Restore this <c OMWeakObjectReference>.
  virtual void restore(void);

    // @cmember Get the value of this <c OMWeakObjectReference>.
    //          The value is a pointer to the referenced <c OMStorable>.
  virtual OMStorable* getValue(void) const;

    // @cmember Set the value of this <c OMWeakObjectReference>.
    //          The value is a pointer to the referenced <c OMStorable>.
  virtual OMStorable* setValue(const OMStorable* value);

  void setTargetTag(OMPropertyTag targetTag);

  const OMUniqueObjectIdentification& identification(void) const;

private:

  OMStrongReferenceSetProperty<OMUniqueObjectIdentification,
                               ReferencedObject>* set(void) const;

  OMUniqueObjectIdentification _identification;
  OMPropertyTag _targetTag;
  OMStrongReferenceSetProperty<OMUniqueObjectIdentification,
                               ReferencedObject>* _targetSet;

};

#include "OMObjectReferenceT.h"

#endif


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

// @doc OMINTERNAL
#ifndef OMOBJECTREFERENCE_H
#define OMOBJECTREFERENCE_H

#include "OMDataTypes.h"
#include <stddef.h>

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

    // @cmember The value of this <c OMObjectReference> as a pointer.
    //          This function provides low-level access. If the object exits
    //          but has not yet been loaded then the value returned is 0.
  virtual OMStorable* pointer(void) const;

    // @cmember The <c OMProperty> containing this <c OMObjectReference>.
  OMProperty* property(void) const;

protected:
  // @access Protected members.

    // @cmember The containing property.
  OMProperty* _property;

    // @cmember A pointer to the actual object.
  OMStorable* _pointer;

};

  // @class Persistent strong references to persistent objects.
  //   @base public | <c OMObjectReference>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
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
  OMStrongObjectReference& operator= (const OMStrongObjectReference& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMStrongObjectReference& rhs) const;

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
    //          The value is a pointer to the referenced <c OMStorable>.
  virtual OMStorable* setValue(const OMStorable* value);

  virtual const wchar_t* name(void) const;

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

class OMStrongReferenceSet;

  // @class Persistent weak references to persistent objects.
  //   @base public | <c OMObjectReference>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
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
  OMWeakObjectReference& operator= (const OMWeakObjectReference& rhs);

    // @cmember Equality.
    //          This operator provides value semantics for <c OMContainer>.
    //          This operator does not provide equality of object references.
  bool operator== (const OMWeakObjectReference& rhs) const;

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
  virtual OMStorable* setValue(
                            const OMUniqueObjectIdentification& identification,
                            const OMStorable* value);

  void setTargetTag(OMPropertyTag targetTag);

  static OMStrongReferenceSet* targetSet(const OMProperty* property,
                                         OMPropertyTag targetTag);

  const OMUniqueObjectIdentification& identification(void) const;

private:

  OMStrongReferenceSet* set(void) const;

  OMUniqueObjectIdentification _identification;
  OMPropertyTag _targetTag;
  OMStrongReferenceSet* _targetSet;

};

#endif


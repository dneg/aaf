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
#ifndef OMSTOREDOBJECT_H
#define OMSTOREDOBJECT_H

#include "OMDataTypes.h"
#include "OMFile.h"

class OMSimpleProperty;
class OMDataStream;
class OMStrongReference;
class OMStrongReferenceSet;
class OMStrongReferenceVector;
class OMWeakReference;
class OMWeakReferenceSet;
class OMWeakReferenceVector;
class OMStoredStream;

  // @class Abstract base class fo the in-memory representation
  //        of a persistent object.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStoredObject {
public:
  // @access Public members.

    // @cmember Destructor.
  virtual ~OMStoredObject(void);

    // @cmember Create a new <c OMStoredObject>, named <p name>,
    //          contained by this <c OMStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* create(const wchar_t* name) = 0;

    // @cmember Open an exsiting <c OMStoredObject>, named <p name>,
    //          contained by this <c OMStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* open(const wchar_t* name) = 0;

    // @cmember Close this <c OMStoredObject>.
  virtual void close(void) = 0;

    // @cmember The byte order of this <c OMStoredObject>.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  virtual OMByteOrder byteOrder(void) const = 0;

  // Saving and restoring properties

  virtual void save(OMFile& file) = 0;

  virtual void save(OMStorable& object) = 0;

    // @cmember Save the <c OMStoredObjectIdentification> <p id>
    //          in this <c OMStoredObject>.
  virtual void save(const OMStoredObjectIdentification& id) = 0;

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMStoredObject>.
  virtual void save(const OMPropertySet& properties) = 0;

    // @cmember Save the <c OMSimpleProperty> <p property> in this
    //          <c OMStoredObject>.
  virtual void save(const OMSimpleProperty& property) = 0;

    // @cmember Save the <c OMStrongReference> <p singleton> in this
    //          <c OMStoredObject>.
  virtual void save(const OMStrongReference& singleton) = 0;

    // @cmember Save the <c OMStrongReferenceVector> <p vector> in this
    //          <c OMStoredObject>.
  virtual void save(const OMStrongReferenceVector& vector) = 0;

    // @cmember Save the <c OMStrongReferenceSet> <p set> in this
    //          <c OMStoredObject>.
  virtual void save(const OMStrongReferenceSet& set) = 0;

    // @cmember Save the <c OMWeakReference> <p singleton> in this
    //          <c OMStoredObject>.
  virtual void save(const OMWeakReference& singleton) = 0;

    // @cmember Save the <c OMWeakReferenceVector> <p vector> in this
    //          <c OMStoredObject>.
  virtual void save(const OMWeakReferenceVector& vector) = 0;

    // @cmember Save the <c OMWeakReferenceSet> <p set> in this
    //          <c OMStoredObject>.
  virtual void save(const OMWeakReferenceSet& set) = 0;

    // @cmember Save the <c OMPropertyTable> <p table> in this
    //          <c OMStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void save(const OMPropertyTable* table) = 0;

    // @cmember Save the <c OMDataStream> <p stream> in this
    //          <c OMStoredObject>.
  virtual void save(const OMDataStream& stream) = 0;

  virtual OMRootStorable* restore(OMFile& file) = 0;

    // @cmember Restore the <c OMStoredObjectIdentification>
    //          of this <c OMStoredObject> into <p id>.
  virtual void restore(OMStoredObjectIdentification& id) = 0;

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMStoredObject>.
  virtual void restore(OMPropertySet& properties) = 0;

    // @cmember Restore the <c OMSimpleProperty> <p property> into this
    //          <c OMStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMSimpleProperty& property,
                       size_t externalSize) = 0;

    // @cmember Restore the <c OMStrongReference> <p singleton> into this
    //          <c OMStoredObject>.
  virtual void restore(OMStrongReference& singleton,
                       size_t externalSize) = 0;

    // @cmember Restore the <c OMStrongReferenceVector> <p vector> into this
    //          <c OMStoredObject>.
  virtual void restore(OMStrongReferenceVector& vector,
                       size_t externalSize) = 0;

    // @cmember Restore the <c OMStrongReferenceSet> <p set> into this
    //          <c OMStoredObject>.
  virtual void restore(OMStrongReferenceSet& set,
                       size_t externalSize) = 0;

    // @cmember Restore the <c OMWeakReference> <p singleton> into this
    //          <c OMStoredObject>.
  virtual void restore(OMWeakReference& singleton,
                       size_t externalSize) = 0;

    // @cmember Restore the <c OMWeakReferenceVector> <p vector> into this
    //          <c OMStoredObject>.
  virtual void restore(OMWeakReferenceVector& vector,
                       size_t externalSize) = 0;

    // @cmember Restore the <c OMWeakReferenceSet> <p set> into this
    //          <c OMStoredObject>.
  virtual void restore(OMWeakReferenceSet& set,
                       size_t externalSize) = 0;

    // @cmember Restore the <c OMPropertyTable> in this <c OMStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void restore(OMPropertyTable*& table) = 0;

    // @cmember Restore the <c OMDataStream> <p stream> into this
    //          <c OMStoredObject>.
  virtual void restore(OMDataStream& stream,
                       size_t externalSize) = 0;

  // Stream manipulation

    // @cmember Open the <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMStoredObject>.
  virtual OMStoredStream* openStoredStream(const OMDataStream& property) = 0;

    // @cmember Create an <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMStoredObject>.
  virtual OMStoredStream* createStoredStream(const OMDataStream& property) = 0;

  // Name manipulation.

    // @cmember Compute the name for a stream.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  static wchar_t* streamName(const wchar_t* propertyName,
                             OMPropertyId pid);

    // @cmember Compute the name for an object reference.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  static wchar_t* referenceName(const wchar_t* propertyName,
                                OMPropertyId pid);

    // @cmember Compute the name for a collection.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  static wchar_t* collectionName(const wchar_t* propertyName,
                                 OMPropertyId pid);

    // @cmember Compute the name for an object reference
    //          that is an element of a collection.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  static wchar_t* elementName(const wchar_t* propertyName,
                              OMPropertyId pid,
                              OMUInt32 localKey);

protected:
  // @access Protected members.

  static void mapCharacters(wchar_t* name, size_t nameLength);

  static void mangleName(const wchar_t* clearName,
                         OMPropertyId pid,
                         wchar_t* mangledName,
                         size_t mangledNameSize);

};

#endif

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
#ifndef OMKLVSTOREDOBJECT_H
#define OMKLVSTOREDOBJECT_H

#include "OMStoredObject.h"
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

  // @class In-memory representation of an object persisted in a
  //        SMPTE (Society of Motion Picture and Television Engineers)
  //        Key Length Value (KLV) binary file.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMKLVStoredObject : public OMStoredObject {
public:
  // @access Static members.

    // @cmember Open the root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  static OMKLVStoredObject* openRead(OMRawStorage* rawStorage);

    // @cmember Open the root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  static OMKLVStoredObject* openModify(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMKLVStoredObject* createWrite(OMRawStorage* rawStorage,
                                        const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMKLVStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMKLVStoredObject* createModify(OMRawStorage* rawStorage,
                                         const OMByteOrder byteOrder);

  // @access Public members.

    // @cmember Destructor.
  virtual ~OMKLVStoredObject(void);

    // @cmember Create a new <c OMKLVStoredObject>, named <p name>,
    //          contained by this <c OMKLVStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* create(const wchar_t* name);

    // @cmember Open an exsiting <c OMKLVStoredObject>, named <p name>,
    //          contained by this <c OMKLVStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* open(const wchar_t* name);

    // @cmember Close this <c OMKLVStoredObject>.
  virtual void close(void);

    // @cmember The byte order of this <c OMKLVStoredObject>.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  virtual OMByteOrder byteOrder(void) const;

  virtual void save(OMFile& file);

  virtual void save(OMStorable& object);

  // Saving and restoring properties

    // @cmember Save the <c OMStoredObjectIdentification> <p id>
    //          in this <c OMKLVStoredObject>.
  virtual void save(const OMStoredObjectIdentification& id);

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMPropertySet& properties);

    // @cmember Save the <c OMSimpleProperty> <p property> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMSimpleProperty& property);

    // @cmember Save the <c OMStrongReference> <p singleton> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReference& singleton);

    // @cmember Save the <c OMStrongReferenceVector> <p vector> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReferenceVector& vector);

    // @cmember Save the <c OMStrongReferenceSet> <p set> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMStrongReferenceSet& set);

    // @cmember Save the <c OMWeakReference> <p singleton> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReference& singleton);

    // @cmember Save the <c OMWeakReferenceVector> <p vector> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReferenceVector& vector);

    // @cmember Save the <c OMWeakReferenceSet> <p set> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMWeakReferenceSet& set);

    // @cmember Save the <c OMPropertyTable> <p table> in this
    //          <c OMKLVStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void save(const OMPropertyTable* table);

    // @cmember Save the <c OMDataStream> <p stream> in this
    //          <c OMKLVStoredObject>.
  virtual void save(const OMDataStream& stream);

  virtual OMRootStorable* restore(OMFile& file);

  virtual OMStorable* restoreObject(const OMStrongObjectReference& reference);

    // @cmember Restore the <c OMStoredObjectIdentification>
    //          of this <c OMKLVStoredObject> into <p id>.
  virtual void restore(OMStoredObjectIdentification& id);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMKLVStoredObject>.
  virtual void restore(OMPropertySet& properties);

    // @cmember Restore the <c OMSimpleProperty> <p property> into this
    //          <c OMKLVStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMSimpleProperty& property,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReference> <p singleton> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReference& singleton,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceVector> <p vector> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReferenceVector& vector,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceSet> <p set> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMStrongReferenceSet& set,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReference> <p singleton> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReference& singleton,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceVector> <p vector> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReferenceVector& vector,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceSet> <p set> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMWeakReferenceSet& set,
                       size_t externalSize);

    // @cmember Restore the <c OMPropertyTable> in this <c OMKLVStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void restore(OMPropertyTable*& table);

    // @cmember Restore the <c OMDataStream> <p stream> into this
    //          <c OMKLVStoredObject>.
  virtual void restore(OMDataStream& stream,
                       size_t externalSize);

  // Stream manipulation

    // @cmember Open the <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMKLVStoredObject>.
  virtual OMStoredStream* openStoredStream(const OMDataStream& property);

    // @cmember Create an <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMKLVStoredObject>.
  virtual OMStoredStream* createStoredStream(const OMDataStream& property);

private:
  // @access Private members.

    // @cmember Constructor.
  OMKLVStoredObject(OMRawStorage* s, OMByteOrder byteOrder);

  OMRawStorage* _storage;
  OMByteOrder _byteOrder;

};

#endif

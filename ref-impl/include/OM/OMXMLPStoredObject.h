//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
// Portions created by British Broadcasting Corporation are
// Copyright 2005, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#ifndef OMXMLPSTOREDOBJECT_H
#define OMXMLPSTOREDOBJECT_H

#include "OMStoredObject.h"
#include "OMDataTypes.h"
#include "OMFile.h"
#include "OMXMLStorage.h"

class OMSimpleProperty;
class OMDataVector;
class OMDataSet;
class OMDataStream;
class OMStrongReference;
class OMStrongReferenceSet;
class OMStrongReferenceVector;
class OMWeakReference;
class OMWeakReferenceSet;
class OMWeakReferenceVector;
class OMStoredStream;

  // @class In-memory representation of an object persisted in an
  //        eXtensible Markup Language (XML) text file.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMXMLPStoredObject : public OMStoredObject {
public:
  // @access Static members.

    // @cmember Open the root <c OMXMLPStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  static OMXMLPStoredObject* openRead(OMRawStorage* rawStorage);

    // @cmember Open the root <c OMXMLPStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  static OMXMLPStoredObject* openModify(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage>.
  static OMXMLPStoredObject* createWrite(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMXMLPStoredObject> in the raw storage
    //          <p rawStorage>.
  static OMXMLPStoredObject* createModify(OMRawStorage* rawStorage);

  // @access Public members.

    // @cmember Destructor.
  virtual ~OMXMLPStoredObject(void);

    // @cmember Create a new <c OMXMLPStoredObject>, named <p name>,
    //          contained by this <c OMXMLPStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* create(const wchar_t* name);

    // @cmember Open an exsiting <c OMXMLPStoredObject>, named <p name>,
    //          contained by this <c OMXMLPStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* open(const wchar_t* name);

    // @cmember Close this <c OMXMLPStoredObject>.
  virtual void close(void);

    // @cmember The byte order of this <c OMXMLPStoredObject>.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  virtual OMByteOrder byteOrder(void) const;

  virtual void save(OMFile& file);

  virtual void save(OMStorable& object);

  // Saving and restoring properties

    // @cmember Save the <c OMStoredObjectIdentification> <p id>
    //          in this <c OMXMLPStoredObject>.
  virtual void save(const OMStoredObjectIdentification& id);

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMPropertySet& properties);

    // @cmember Save the <c OMSimpleProperty> <p property> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMSimpleProperty& property);

    // @cmember Save the <c OMDataVector> <p property> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMDataVector& property);

    // @cmember Save the <c OMDataSet> <p property> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMDataSet& property);

    // @cmember Save the <c OMStrongReference> <p singleton> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMStrongReference& singleton);

    // @cmember Save the <c OMStrongReferenceVector> <p vector> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMStrongReferenceVector& vector);

    // @cmember Save the <c OMStrongReferenceSet> <p set> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMStrongReferenceSet& set);

    // @cmember Save the <c OMWeakReference> <p singleton> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMWeakReference& singleton);

    // @cmember Save the <c OMWeakReferenceVector> <p vector> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMWeakReferenceVector& vector);

    // @cmember Save the <c OMWeakReferenceSet> <p set> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMWeakReferenceSet& set);

    // @cmember Save the <c OMPropertyTable> <p table> in this
    //          <c OMXMLPStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void save(const OMPropertyTable* table);

    // @cmember Save the <c OMDataStream> <p stream> in this
    //          <c OMXMLPStoredObject>.
  virtual void save(const OMDataStream& stream);

  virtual OMRootStorable* restore(OMFile& file);

  virtual OMStorable* restoreObject(const OMStrongObjectReference& reference);

    // @cmember Restore the <c OMStoredObjectIdentification>
    //          of this <c OMXMLPStoredObject> into <p id>.
  virtual void restore(OMStoredObjectIdentification& id);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMXMLPStoredObject>.
  virtual void restore(OMPropertySet& properties);

    // @cmember Restore the <c OMSimpleProperty> <p property> into this
    //          <c OMXMLPStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMSimpleProperty& property,
                       size_t externalSize);

    // @cmember Restore the <c OMDataVector> <p property> into this
    //          <c OMXMLPStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMDataVector& property,
                       size_t externalSize);

    // @cmember Restore the <c OMDataSet> <p property> into this
    //          <c OMXMLPStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMDataSet& property,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReference> <p singleton> into this
    //          <c OMXMLPStoredObject>.
  virtual void restore(OMStrongReference& singleton,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceVector> <p vector> into this
    //          <c OMXMLPStoredObject>.
  virtual void restore(OMStrongReferenceVector& vector,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceSet> <p set> into this
    //          <c OMXMLPStoredObject>.
  virtual void restore(OMStrongReferenceSet& set,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReference> <p singleton> into this
    //          <c OMXMLPStoredObject>.
  virtual void restore(OMWeakReference& singleton,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceVector> <p vector> into this
    //          <c OMXMLPStoredObject>.
  virtual void restore(OMWeakReferenceVector& vector,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceSet> <p set> into this
    //          <c OMXMLPStoredObject>.
  virtual void restore(OMWeakReferenceSet& set,
                       size_t externalSize);

    // @cmember Restore the <c OMPropertyTable> in this <c OMXMLPStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void restore(OMPropertyTable*& table);

    // @cmember Restore the <c OMDataStream> <p stream> into this
    //          <c OMXMLPStoredObject>.
  virtual void restore(OMDataStream& stream,
                       size_t externalSize);

  // Stream manipulation

    // @cmember Open the <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMXMLPStoredObject>.
  virtual OMStoredStream* openStoredStream(const OMDataStream& property);

    // @cmember Create an <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMXMLPStoredObject>.
  virtual OMStoredStream* createStoredStream(const OMDataStream& property);

private:
  // @access Private members.

    // @cmember Constructor.
  OMXMLPStoredObject(OMXMLStorage* store, bool isRoot);

  OMXMLStorage* _store;
  bool _isRoot;
  
};

#endif

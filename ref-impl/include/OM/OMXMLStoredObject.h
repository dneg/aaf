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
#ifndef OMXMLSTOREDOBJECT_H
#define OMXMLSTOREDOBJECT_H

#include "OMStoredObject.h"
#include "OMDataTypes.h"
#include "OMFile.h"
#include "OMIOStream.h"

class OMSimpleProperty;
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
class OMXMLStoredObject : public OMStoredObject {
public:
  // @access Static members.

    // @cmember Open the root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage> for reading only.
  static OMXMLStoredObject* openRead(OMRawStorage* rawStorage);

    // @cmember Open the root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage> for modification.
  static OMXMLStoredObject* openModify(OMRawStorage* rawStorage);

    // @cmember Create a new root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMXMLStoredObject* createWrite(OMRawStorage* rawStorage,
                                        const OMByteOrder byteOrder);

    // @cmember Create a new root <c OMXMLStoredObject> in the raw storage
    //          <p rawStorage>. The byte order of the newly created root
    //          is given by <p byteOrder>.
  static OMXMLStoredObject* createModify(OMRawStorage* rawStorage,
                                         const OMByteOrder byteOrder);

     // @cmember Is the file named <p fileName> a recognized file ?
     //          If so, the result is true, and the signature is returned
     //          in <p signature>.
  static bool isRecognized(const wchar_t* fileName,
                           OMFileSignature& signature);

     // @cmember Does <p rawStorage> contain a recognized file ?
     //          If so, the result is true, and the signature is returned
     //          in <p signature>.
  static bool isRecognized(OMRawStorage* rawStorage,
                           OMFileSignature& signature);

     // @cmember Is <p signature> recognized ?
  static bool isRecognized(const OMFileSignature& signature);

  // @access Public members.

    // @cmember Destructor.
  virtual ~OMXMLStoredObject(void);

    // @cmember Create a new <c OMXMLStoredObject>, named <p name>,
    //          contained by this <c OMXMLStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* create(const wchar_t* name);

    // @cmember Open an exsiting <c OMXMLStoredObject>, named <p name>,
    //          contained by this <c OMXMLStoredObject>.
    //   @devnote The name argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual OMStoredObject* open(const wchar_t* name);

    // @cmember Close this <c OMXMLStoredObject>.
  virtual void close(void);

  virtual void close(OMFile& file);

    // @cmember The byte order of this <c OMXMLStoredObject>.
    //   @devnote This member function doesn't make sense for all
    //            derived instances of <c OMStoredObject>.
  virtual OMByteOrder byteOrder(void) const;

  virtual void save(OMFile& file);

  virtual void save(OMStorable& object);

  // Saving and restoring properties

    // @cmember Save the <c OMStoredObjectIdentification> <p id>
    //          in this <c OMXMLStoredObject>.
  virtual void save(const OMStoredObjectIdentification& id);

    // @cmember Save the <c OMPropertySet> <p properties> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMPropertySet& properties);

    // @cmember Save the <c OMSimpleProperty> <p property> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMSimpleProperty& property);

    // @cmember Save the <c OMStrongReference> <p singleton> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMStrongReference& singleton);

    // @cmember Save the <c OMStrongReferenceVector> <p vector> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMStrongReferenceVector& vector);

    // @cmember Save the <c OMStrongReferenceSet> <p set> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMStrongReferenceSet& set);

    // @cmember Save the <c OMWeakReference> <p singleton> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMWeakReference& singleton);

    // @cmember Save the <c OMWeakReferenceVector> <p vector> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMWeakReferenceVector& vector);

    // @cmember Save the <c OMWeakReferenceSet> <p set> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMWeakReferenceSet& set);

    // @cmember Save the <c OMPropertyTable> <p table> in this
    //          <c OMXMLStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void save(const OMPropertyTable* table);

    // @cmember Save the <c OMDataStream> <p stream> in this
    //          <c OMXMLStoredObject>.
  virtual void save(const OMDataStream& stream);

    // @cmember Restore the <c OMStoredObjectIdentification>
    //          of this <c OMXMLStoredObject> into <p id>.
  virtual void restore(OMStoredObjectIdentification& id);

    // @cmember Restore the <c OMPropertySet> <p properties> into
    //          this <c OMXMLStoredObject>.
  virtual void restore(OMPropertySet& properties);

    // @cmember Restore the <c OMSimpleProperty> <p property> into this
    //          <c OMXMLStoredObject>.
    //   @devnote The externalSize argument to this member function doesn't
    //            make sense for all derived instances of <c OMStoredObject>.
  virtual void restore(OMSimpleProperty& property,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReference> <p singleton> into this
    //          <c OMXMLStoredObject>.
  virtual void restore(OMStrongReference& singleton,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceVector> <p vector> into this
    //          <c OMXMLStoredObject>.
  virtual void restore(OMStrongReferenceVector& vector,
                       size_t externalSize);

    // @cmember Restore the <c OMStrongReferenceSet> <p set> into this
    //          <c OMXMLStoredObject>.
  virtual void restore(OMStrongReferenceSet& set,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReference> <p singleton> into this
    //          <c OMXMLStoredObject>.
  virtual void restore(OMWeakReference& singleton,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceVector> <p vector> into this
    //          <c OMXMLStoredObject>.
  virtual void restore(OMWeakReferenceVector& vector,
                       size_t externalSize);

    // @cmember Restore the <c OMWeakReferenceSet> <p set> into this
    //          <c OMXMLStoredObject>.
  virtual void restore(OMWeakReferenceSet& set,
                       size_t externalSize);

    // @cmember Restore the <c OMPropertyTable> in this <c OMXMLStoredObject>.
    //   @devnote Does this member function make sense for all
    //            derived instances of <c OMStoredObject> ?
  virtual void restore(OMPropertyTable*& table);

    // @cmember Restore the <c OMDataStream> <p stream> into this
    //          <c OMXMLStoredObject>.
  virtual void restore(OMDataStream& stream,
                       size_t externalSize);

  // Stream manipulation

    // @cmember Open the <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMXMLStoredObject>.
  virtual OMStoredStream* openStoredStream(const OMDataStream& property);

    // @cmember Create an <c OMStoredStream> representing the property
    //          <p stream> contained within this <c OMXMLStoredObject>.
  virtual OMStoredStream* createStoredStream(const OMDataStream& property);

private:
  // @access Private members.

    // @cmember Constructor.
  OMXMLStoredObject(OMRawStorage* s, const OMByteOrder byteOrder);

  OMRawStorage& _store;
  OMIOStream _stream;
  OMByteOrder _byteOrder;

  static OMUInt16 _seed; // For use during save()

  // Binary data formatting

  void output(void);

    // Dump a single byte.
    //
  void print(unsigned char ch);

    // Flush any pending dumped bytes.
    //
  void flush(void);

  static char _table[128];

  char map(int c);

  enum {BYTESPERLINE  = 16}; // Number of dumped bytes to put on a line.
  unsigned char _buffer[BYTESPERLINE];
  int _count;
  int _line;

};

#endif

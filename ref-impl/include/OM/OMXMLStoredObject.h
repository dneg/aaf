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
#ifndef OMXMLSTOREDOBJECT_H
#define OMXMLSTOREDOBJECT_H

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
  OMXMLStoredObject(OMRawStorage* s, OMByteOrder byteOrder);

  OMRawStorage* _store;
  OMByteOrder _byteOrder;

};

#endif

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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMXMLStoredObject
#include "OMXMLStoredObject.h"

#include "OMObjectReference.h"
#include "OMContainerElement.h"

#include "OMDataStream.h"
#include "OMStrongReference.h"
#include "OMStrongReferenceSet.h"
#include "OMStrongReferenceVector.h"
#include "OMWeakReference.h"
#include "OMWeakReferenceSet.h"
#include "OMWeakReferenceVector.h"

#include "OMCachedDiskRawStorage.h"
#include "OMXMLStoredStream.h"
#include "OMPropertySetIterator.h"

#include "OMClassFactory.h"
#include "OMClassDefinition.h"

#include "OMUtilities.h"

#include "OMIOStream.h"

  // @mfunc Open the root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMXMLStoredObject* OMXMLStoredObject::openRead(OMRawStorage* ANAME(rawStorage))
{
  TRACE("OMXMLStoredObject::openRead");
  PRECONDITION("Compatible raw storage access mode", rawStorage->isReadable());
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Open the root <c OMXMLStoredObject> in the raw storage
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
  //        <p rawStorage> for modification.
OMXMLStoredObject* OMXMLStoredObject::openModify(
                                               OMRawStorage* ANAME(rawStorage))
{
  TRACE("OMXMLStoredObject::openModify");
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMXMLStoredObject* OMXMLStoredObject::createWrite(OMRawStorage* rawStorage,
                                                  const OMByteOrder byteOrder)
{
  TRACE("OMXMLStoredObject::createWrite");
  PRECONDITION("Compatible raw storage access mode", rawStorage->isWritable());
  OMXMLStoredObject* result = new OMXMLStoredObject(rawStorage, byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMXMLStoredObject* OMXMLStoredObject::createModify(OMRawStorage* rawStorage,
                                                   const OMByteOrder byteOrder)
{
  TRACE("OMXMLStoredObject::createModify");

  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  OMXMLStoredObject* result = new OMXMLStoredObject(rawStorage, byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Destructor.
OMXMLStoredObject::~OMXMLStoredObject(void)
{
  TRACE("OMXMLStoredObject::~OMXMLStoredObject");

  // TBS tjb
}

  // @mfunc Create a new <c OMXMLStoredObject>, named <p name>,
  //        contained by this <c OMXMLStoredObject>.
  //   @parm The name to be used for the new <c OMXMLStoredObject>.
  //   @rdesc A new <c OMXMLStoredObject> contained by this
  //          <c OMXMLStoredObject>.
OMStoredObject* OMXMLStoredObject::create(const wchar_t* /* name */)
{
  TRACE("OMXMLStoredObject::create");
  OMStoredObject* result = new OMXMLStoredObject(&_store, _byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Open an exsiting <c OMXMLStoredObject>, named <p name>,
  //        contained by this <c OMXMLStoredObject>.
  //   @parm The name of the existing <c OMXMLStoredObject>.
  //   @rdesc The existing <c OMXMLStoredObject> contained by this
  //          <c OMXMLStoredObject>.
OMStoredObject* OMXMLStoredObject::open(const wchar_t* /* name */)
{
  TRACE("OMXMLStoredObject::open");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Close this <c OMXMLStoredObject>.
void OMXMLStoredObject::close(void)
{
  TRACE("OMXMLStoredObject::close");
  _store.synchronize();
}

  // @mfunc The byte order of this <c OMXMLStoredObject>.
  //   @rdesc The byte order.
  //   @this const
OMByteOrder OMXMLStoredObject::byteOrder(void) const
{
  TRACE("OMXMLStoredObject::byteOrder");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return unspecified;
}

void OMXMLStoredObject::save(OMFile& file)
{
  TRACE("OMXMLStoredObject::save(OMFile)");
  _stream << beginl;
  _stream << "<?xml version=\"1.0\"?>" << endl;
  _stream << beginl;
  _stream << "<?OM signature=\"" << file.encoding() << "\"?>" << endl;
  _stream << beginl;
  _stream << "<!DOCTYPE object SYSTEM \"ObjectManager.dtd\">" << endl;
  _stream << beginl;
  _stream << "<!-- This file was produced by a *PROTOTYPE*, both the -->"
          << endl;
  _stream << beginl;
  _stream << "<!-- implementation and the dtd are subject to change. -->"
          << endl;
  file.root()->save();
  //save(file.referencedProperties());
  _stream << endl;
}

void OMXMLStoredObject::save(OMStorable& object)
{
  TRACE("OMXMLStoredObject::save(OMStorable)");
  _stream << indent;
  _stream << beginl;
  _stream << "<object class=\"";
  save(object.classId());
  _stream << "\">" << endl;

  _stream << indent;
  _stream << beginl;
  const wchar_t* name = 0;
#if defined(OM_DEFINITIONS)
  const OMClassDefinition* definition = object.definition();
  if (definition != 0) {
    ASSERT("Valid definition", definition != 0);
    name = definition->name();
  } else {
    name = L"Unknown";
  }
#else
  name = L"Unknown";
#endif
  ASSERT("Valid name", name != 0);
  _stream << "<!-- object of class " << name << " -->" << endl;
  _stream << outdent;

  save(*object.propertySet());

  _stream << beginl;
  _stream << "</object>" << endl;
  _stream << outdent;
}

  // @mfunc Save the <c OMStoredObjectIdentification> <p id>
  //        in this <c OMXMLStoredObject>.
  //   @parm The <t OMStoredObjectIdentification> to save.
void OMXMLStoredObject::save(const OMStoredObjectIdentification& id)
{
  TRACE("OMXMLStoredObject::save(OMStoredObjectIdentification)");
  _stream << id;
}

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMXMLStoredObject::save(const OMPropertySet& properties )
{
  TRACE("OMXMLStoredObject::save(OMPropertySet)");

  _stream << indent;
  _stream << beginl;
  OMUInt32 count = properties.countPresent();
  _stream << "<!-- object contains " << dec << count << " properties  -->"
          << endl;
  _stream << outdent;

  OMPropertySetIterator iterator(properties, OMBefore);
  while (++iterator) {
    const OMProperty* p = iterator.property();
    if (!p->isOptional() || p->isPresent()) {
      _stream << indent;
      _stream << beginl;
      OMUInt16 pid = p->propertyId();
      _stream << "<property identification=\"" << showbase
              << hex << setw(6) << setfill('0') << lowercase << pid
              << "\">" << endl;

      _stream << indent;
      _stream << beginl;
      _stream << "<!-- \"" << p->name() << "\" property -->" << endl;
      _stream << outdent;

      p->save();

      _stream << beginl;
      _stream << "</property>" << endl;
      _stream << outdent;
    } else {
      // Since we are iterating over the properties in this property set
      // instance rather than over the property definitions in the class
      // definition we will omit this comment for optional properties that
      // were added to the class definition after this object instance
      // was created.
      _stream << indent;
      _stream << beginl;
      _stream << "<!-- \"" << p->name() << "\" property not present -->"
              << endl;
      _stream << outdent;
    }
  }
}

  // @mfunc Save the <c OMSimpleProperty> <p property> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMSimpleProperty> to save.
void OMXMLStoredObject::save(const OMSimpleProperty& property)
{
  TRACE("OMXMLStoredObject::save(OMSimpleProperty)");
  _stream << indent;
  _stream << beginl;
  _stream << "<data>" << endl;

  OMUInt16 size = property.size();
  _stream << "<!-- data size is " << dec << size << " bytes -->" << endl;

  OMByte* start = property.bits();
  for (size_t i = 0; i < size; i++) {
    OMByte b = start[i];
    print((char) b);
  }
  flush();

  _stream << beginl;
  _stream << "</data>" << endl;
  _stream << outdent;
}

  // @mfunc Save the <c OMStrongReference> <p singleton> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMXMLStoredObject::save(const OMStrongReference& singleton)
{
  TRACE("OMXMLStoredObject::save(OMStrongReference)");
  singleton.reference().save();
}

  // @mfunc Save the <c OMStrongReferenceVector> <p vector> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMStrongReferenceVector> to save.
void OMXMLStoredObject::save(const OMStrongReferenceVector& vector)
{
  TRACE("OMXMLStoredObject::save(OMStrongReferenceVector)");

  _stream << indent;
  _stream << beginl;
  _stream << "<object-vector>" << endl;

  _stream << indent;
  _stream << beginl;
  OMUInt32 count = vector.count();
  _stream << "<!-- object-vector contains " << dec << count << " objects -->"
          << endl;
  _stream << outdent;

  OMUInt32 ordinal = 0; // tjb - right size ?
  OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
                                                            *vector.iterator();
  while (++iterator) {

    _stream << indent;
    _stream << beginl;
    _stream << "<!-- element " << dec << ordinal << " of " << dec << count
            << " in object-vector \"" << vector.name() << "\" -->" << endl;
    OMStrongReferenceVectorElement& element = iterator.value();

    element.save();

    ordinal = ordinal + 1;
    _stream << outdent;
  }
  delete &iterator;
  _stream << beginl;
  _stream << "</object-vector>" << endl;
  _stream << outdent;
}

  // @mfunc Save the <c OMStrongReferenceSet> <p set> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMXMLStoredObject::save(const OMStrongReferenceSet& set)
{
  TRACE("OMXMLStoredObject::save(OMStrongReferenceSet)");

  _stream << indent;
  _stream << beginl;
  _stream << "<object-set>" << endl;

  _stream << indent;
  _stream << beginl;
  OMUInt32 count = set.count();
  _stream << "<!-- object-set contains " << dec << count << " objects -->"
          << endl;
  _stream << outdent;

  OMKeySize keySize = set.keySize();
  OMUInt32 ordinal = 0;
  OMContainerIterator<OMStrongReferenceSetElement>& iterator = *set.iterator();
  while (++iterator) {

    OMStrongReferenceSetElement& element = iterator.value();

    _stream << indent;
    _stream << beginl;
    _stream << "<!-- element " << dec << ordinal << " of " << dec << count
            << " in object-set \"" << set.name() << "\" -->" << endl;
    _stream << beginl;
    _stream << "<element key=\"";
    void* k = element.identification();
    if (keySize == sizeof(OMUniqueObjectIdentification)) {
      OMUniqueObjectIdentification* id =
                            reinterpret_cast<OMUniqueObjectIdentification*>(k);
      _stream << *id;
    } else if (keySize == sizeof(OMMaterialIdentification)) {
      OMMaterialIdentification* id =
                                reinterpret_cast<OMMaterialIdentification*>(k);
      _stream << *id;
    } else {
      _stream << nullOMUniqueObjectIdentification;
    }
    _stream << "\">" << endl;

    element.save();

    _stream << beginl;
    _stream << "</element>" << endl;
    _stream << outdent;

    ordinal = ordinal + 1;
  }
  _stream << beginl;
  _stream << "</object-set>" << endl;
  _stream << outdent;
  delete &iterator;
}

  // @mfunc Save the <c OMWeakReference> <p singleton> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMWeakReference> to save.
void OMXMLStoredObject::save(const OMWeakReference& singleton)
{
  TRACE("OMXMLStoredObject::save(OMWeakReference)");

  _stream << indent;
  _stream << beginl;
  const char* target = "Weak-References-NYI";
  _stream << "<reference target=\"" << target << "\">" << endl;

  _stream << indent;
  _stream << beginl;
  OMObjectIdentification k = singleton.reference().identification();
  _stream << "<identification guid=\"" << k << "\"/>" << endl;
  _stream << outdent;

  _stream << beginl;
  _stream << "</reference>" << endl;
  _stream << outdent;
}

  // @mfunc Save the <c OMWeakReferenceVector> <p vector> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMWeakReferenceVector> to save.
void OMXMLStoredObject::save(const OMWeakReferenceVector& vector)
{
  TRACE("OMXMLStoredObject::save(OMWeakReferenceVector)");

  _stream << indent;
  _stream << beginl;
  const char* target = "Weak-References-NYI";
  _stream << "<reference-vector target=\"" << target << "\">" << endl;

  _stream << indent;
  _stream << beginl;
  OMUInt32 count = vector.count();
  _stream << "<!-- reference-vector references " << dec << count
          << " objects -->" << endl;
  _stream << outdent;

  OMContainerIterator<OMWeakReferenceVectorElement>& iterator =
                                                            *vector.iterator();
  while (++iterator) {

    OMWeakReferenceVectorElement& element = iterator.value();

    OMObjectIdentification key = element.identification();

    _stream << indent;
    _stream << beginl;
    _stream << "<identification guid=\"" << key << "\"/>" << endl;
    _stream << outdent;
  }
  _stream << beginl;
  _stream << "</reference-vector>" << endl;
  _stream << outdent;
  delete &iterator;
}

  // @mfunc Save the <c OMWeakReferenceSet> <p set> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMWeakReferenceSet> to save.
void OMXMLStoredObject::save(const OMWeakReferenceSet& set)
{
  TRACE("OMXMLStoredObject::save(OMWeakReferenceSet)");

  _stream << indent;
  _stream << beginl;
  const char* target = "Weak-References-NYI";
  _stream << "<reference-set target=\"" << target << "\">" << endl;

  _stream << indent;
  _stream << beginl;
  OMUInt32 count = set.count();
  _stream << "<!-- reference-set references " << dec << count
          << " objects -->" << endl;
  _stream << outdent;

  OMContainerIterator<OMWeakReferenceSetElement>& iterator = *set.iterator();
  while (++iterator) {

    OMWeakReferenceSetElement& element = iterator.value();

    OMObjectIdentification key = element.identification();

    _stream << indent;
    _stream << beginl;
    _stream << "<identification guid=\"" << key << "\"/>" << endl;
    _stream << outdent;
  }
  _stream << beginl;
  _stream << "</reference-set>" << endl;
  _stream << outdent;
  delete &iterator;
}

  // @mfunc Save the <c OMPropertyTable> <p table> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMPropertyTable> to save.
void OMXMLStoredObject::save(const OMPropertyTable* /* table */)
{
  TRACE("OMXMLStoredObject::save(OMPropertyTable)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMDataStream> <p stream> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMDataStream> to save.
void OMXMLStoredObject::save(const OMDataStream& stream)
{
  TRACE("OMXMLStoredObject::save(OMDataStream)");
  _stream << indent;
  _stream << beginl;
  _stream << "<stream>" << endl;

  OMUInt64 size = stream.size();
  _stream << "<!-- stream size is " << dec << size << " bytes -->" << endl;

  stream.setPosition(0);
  for (size_t i = 0; i < size; i++) {
    OMByte b;
    OMUInt32 x;
    stream.read(&b, 1, x);
    print((char) b);
  }
  flush();

  _stream << beginl;
  _stream << "</stream>" << endl;
  _stream << outdent;
}

OMRootStorable* OMXMLStoredObject::restore(OMFile& /* file */)
{
  TRACE("OMXMLStoredObject::restore(OMFile)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  OMRootStorable* result = 0;
  return result;
}

OMStorable*
OMXMLStoredObject::restoreObject(const OMStrongObjectReference& reference)
{
  TRACE("OMXMLStoredObject::restoreObject");

  const wchar_t* name = reference.name();
  OMProperty* property = reference.property();
  OMStorable* containingObject = property->propertySet()->container();

  OMClassId cid;
  restore(cid);
  const OMClassFactory* classFactory = containingObject->classFactory();
  OMStorable* object = classFactory->create(cid);
  ASSERT("Registered class id", object != 0);
  ASSERT("Valid class factory", classFactory == object->classFactory());
#if !defined(OM_NO_VALIDATE_DEFINITIONS)
  ASSERT("Valid class definition", object->definition() != 0);
#endif
  // Attach the object.
  object->attach(containingObject, name);
  object->setStore(this);
  object->restoreContents();
  return object;
}

  // @mfunc Restore the <c OMStoredObjectIdentification>
  //        of this <c OMXMLStoredObject> into <p id>.
  //   @parm The newly restored <t OMStoredObjectIdentification>.
void OMXMLStoredObject::restore(OMStoredObjectIdentification& /* id */)
{
  TRACE("OMXMLStoredObject::restore(OMStoredObjectIdentification)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMPropertySet> <p properties> into
  //        this <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMPropertySet>.
void OMXMLStoredObject::restore(OMPropertySet& /* properties */)
{
  TRACE("OMXMLStoredObject::restore(OMPropertySet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMSimpleProperty> <p property> into this
  //        <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMSimpleProperty>
  //   @parm The external size.
void OMXMLStoredObject::restore(OMSimpleProperty& /* property */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLStoredObject::restore(OMSimpleProperty)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReference> <p singleton> into this
  //        <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMStrongReference>.
  //   @parm The external size.
void OMXMLStoredObject::restore(OMStrongReference& /* singleton */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLStoredObject::restore(OMStrongReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReferenceVector> <p vector> into this
  //        <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMStrongReferenceVector>.
  //   @parm The external size.
void OMXMLStoredObject::restore(OMStrongReferenceVector& /* vector */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLStoredObject::restore(OMStrongReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReferenceSet> <p set> into this
  //        <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMStrongReferenceSet>.
  //   @parm The external size.
void OMXMLStoredObject::restore(OMStrongReferenceSet& /* set */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLStoredObject::restore(OMStrongReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReference> <p singleton> into this
  //        <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMWeakReference>.
  //   @parm The external size.
void OMXMLStoredObject::restore(OMWeakReference& /* singleton */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLStoredObject::restore(OMWeakReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReferenceVector> <p vector> into this
  //        <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMWeakReferenceVector>.
  //   @parm The external size.
void OMXMLStoredObject::restore(OMWeakReferenceVector& /* vector */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLStoredObject::restore(OMWeakReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReferenceSet> <p set> into this
  //        <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMWeakReferenceSet>.
  //   @parm The external size.
void OMXMLStoredObject::restore(OMWeakReferenceSet& /* set */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLStoredObject::restore(OMWeakReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMPropertyTable> in this <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMPropertyTable>.
void OMXMLStoredObject::restore(OMPropertyTable*& /* table */)
{
  TRACE("OMXMLStoredObject::restore(OMPropertyTable)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMDataStream> <p stream> into this
  //        <c OMXMLStoredObject>.
  //   @parm The newly restored <c OMDataStream>.
  //   @parm The external size.
void OMXMLStoredObject::restore(OMDataStream& /* stream */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLStoredObject::restore(OMDataStream)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Open the <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMXMLStoredObject>.
  //   @parm The <c OMDataStream> to be opened.
  //   @rdesc The newly created <c OMStoredStream>.
OMStoredStream* OMXMLStoredObject::openStoredStream(
                                            const OMDataStream& /* property */)
{
  TRACE("OMXMLStoredObject::openStoredStream");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create an <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMXMLStoredObject>.
  //   @parm The <c OMDataStream> to be created.
  //   @rdesc The newly created <c OMStoredStream>.
OMStoredStream* OMXMLStoredObject::createStoredStream(
                                            const OMDataStream& /* property */)
{
  TRACE("OMXMLStoredObject::createStoredStream");

  OMRawStorage* store = OMCachedDiskRawStorage::openNewModify();
  OMXMLStoredStream* result = new OMXMLStoredStream(store);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Constructor.
  //   @parm The <c OMRawStorage> on which this <c OMXMLStoredObject> resides.
  //   @parm TBS
OMXMLStoredObject::OMXMLStoredObject(OMRawStorage* s,
                                     const OMByteOrder byteOrder)
: _store(*s),
  _stream(s),
  _byteOrder(byteOrder),
  _count(0),
  _line(0)
{
  TRACE("OMXMLStoredObject::OMXMLStoredObject");
}

void OMXMLStoredObject::output(void)
{
  int i;

  _stream << "<!-- ";
  OMUInt32 address = _line * BYTESPERLINE;
  _stream << dec << setw(8) << setfill(' ') << address;
  _stream << " -->  ";
  _line++;
  
  for (i = 0; i < _count; i++) {
    _stream << hex << setw(2) << setfill('0') << uppercase << _buffer[i];
  }
  
  for (i = _count; i < BYTESPERLINE; i++) {
    _stream << "  ";
  }
  
  _stream << "  <!-- ";
  for (i = 0; i < _count; i++) {
    int c = (unsigned char)_buffer[i];
    _stream.put(map(c));
  }
  _stream << " -->" << endl;
}

void OMXMLStoredObject::print(unsigned char ch)
{ 
  _buffer[_count++] = ch;
  
  if ( _count == BYTESPERLINE) {
    output();
    _count = 0;
  }
}

void OMXMLStoredObject::flush(void)
{
  if (_count > 0) {
    output();
  }
  _count = 0;
  _line = 0;

}

// Interpret values 0x00 - 0x7f as ASCII characters.
//
char OMXMLStoredObject::_table[128] = {
'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
' ',  '!',  '"',  '#',  '$',  '%',  '&', '\'',
'(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
'8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',
'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
'X',  'Y',  'Z',  '[', '\\',  ']',  '^',  '_',
'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
'x',  'y',  'z',  '{',  '|',  '}',  '~',  '.'};

char OMXMLStoredObject::map(int c)
{
  char result;
  if (c < 0x80) {
    result = _table[c & 0x7f];
  } else {
    result = '.';
  }
  return result;
}

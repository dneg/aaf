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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMKLVStoredObject
#include "OMKLVStoredObject.h"

#include "OMRawStorage.h"

  // @mfunc Open the root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMKLVStoredObject* OMKLVStoredObject::openRead(OMRawStorage* ANAME(rawStorage))
{
  TRACE("OMKLVStoredObject::openRead");
  PRECONDITION("Compatible raw storage access mode", rawStorage->isReadable());
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Open the root <c OMKLVStoredObject> in the raw storage
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
  //        <p rawStorage> for modification.
OMKLVStoredObject* OMKLVStoredObject::openModify(
                                               OMRawStorage* ANAME(rawStorage))
{
  TRACE("OMKLVStoredObject::openModify");
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create a new root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMKLVStoredObject* OMKLVStoredObject::createWrite(OMRawStorage* rawStorage,
                                                  const OMByteOrder byteOrder)
{
  TRACE("OMKLVStoredObject::createWrite");
  PRECONDITION("Compatible raw storage access mode", rawStorage->isWritable());
  OMKLVStoredObject* result= new OMKLVStoredObject(rawStorage, byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Create a new root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMKLVStoredObject* OMKLVStoredObject::createModify(OMRawStorage* rawStorage,
                                                   const OMByteOrder byteOrder)
{
  TRACE("OMKLVStoredObject::createModify");
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());
  OMKLVStoredObject* result= new OMKLVStoredObject(rawStorage, byteOrder);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Destructor.
OMKLVStoredObject::~OMKLVStoredObject(void)
{
  TRACE("OMKLVStoredObject::~OMKLVStoredObject");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Create a new <c OMKLVStoredObject>, named <p name>,
  //        contained by this <c OMKLVStoredObject>.
  //   @parm The name to be used for the new <c OMKLVStoredObject>.
  //   @rdesc A new <c OMKLVStoredObject> contained by this
  //          <c OMKLVStoredObject>.
OMStoredObject* OMKLVStoredObject::create(const wchar_t* /* name */)
{
  TRACE("OMKLVStoredObject::create");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Open an exsiting <c OMKLVStoredObject>, named <p name>,
  //        contained by this <c OMKLVStoredObject>.
  //   @parm The name of the existing <c OMKLVStoredObject>.
  //   @rdesc The existing <c OMKLVStoredObject> contained by this
  //          <c OMKLVStoredObject>.
OMStoredObject* OMKLVStoredObject::open(const wchar_t* /* name */)
{
  TRACE("OMKLVStoredObject::open");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Close this <c OMKLVStoredObject>.
void OMKLVStoredObject::close(void)
{
  TRACE("OMKLVStoredObject::close");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc The byte order of this <c OMKLVStoredObject>.
  //   @rdesc The byte order.
  //   @this const
OMByteOrder OMKLVStoredObject::byteOrder(void) const
{
  TRACE("OMKLVStoredObject::byteOrder");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return unspecified;
}

void OMKLVStoredObject::save(OMFile& /* file */)
{
  TRACE("OMKLVStoredObject::save(OMFile)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMKLVStoredObject::save(OMStorable& /* object */)
{
  TRACE("OMKLVStoredObject::save(OMFile)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMStoredObjectIdentification> <p id>
  //        in this <c OMKLVStoredObject>.
  //   @parm The <t OMStoredObjectIdentification> to save.
void OMKLVStoredObject::save(const OMStoredObjectIdentification& /* id */)
{
  TRACE("OMKLVStoredObject::save(OMStoredObjectIdentification)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMKLVStoredObject::save(const OMPropertySet& /* properties */)
{
  TRACE("OMKLVStoredObject::save(OMPropertySet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMSimpleProperty> <p property> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMSimpleProperty> to save.
void OMKLVStoredObject::save(const OMSimpleProperty& /* property */)
{
  TRACE("OMKLVStoredObject::save(OMSimpleProperty)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMStrongReference> <p singleton> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMKLVStoredObject::save(const OMStrongReference& /* singleton */)
{
  TRACE("OMKLVStoredObject::save(OMStrongReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMStrongReferenceVector> <p vector> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMStrongReferenceVector> to save.
void OMKLVStoredObject::save(const OMStrongReferenceVector& /* vector */)
{
  TRACE("OMKLVStoredObject::save(OMStrongReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMStrongReferenceSet> <p set> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMKLVStoredObject::save(const OMStrongReferenceSet& /* set */)
{
  TRACE("OMKLVStoredObject::save(OMStrongReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMWeakReference> <p singleton> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMWeakReference> to save.
void OMKLVStoredObject::save(const OMWeakReference& /* singleton */)
{
  TRACE("OMKLVStoredObject::save(OMWeakReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMWeakReferenceVector> <p vector> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMWeakReferenceVector> to save.
void OMKLVStoredObject::save(const OMWeakReferenceVector& /* vector */)
{
  TRACE("OMKLVStoredObject::save(OMWeakReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMWeakReferenceSet> <p set> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMWeakReferenceSet> to save.
void OMKLVStoredObject::save(const OMWeakReferenceSet& /* set */)
{
  TRACE("OMKLVStoredObject::save(OMWeakReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMPropertyTable> <p table> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMPropertyTable> to save.
void OMKLVStoredObject::save(const OMPropertyTable* /* table */)
{
  TRACE("OMKLVStoredObject::save(OMPropertyTable)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMDataStream> <p stream> in this
  //        <c OMKLVStoredObject>.
  //   @parm The <c OMDataStream> to save.
void OMKLVStoredObject::save(const OMDataStream& /* stream */)
{
  TRACE("OMKLVStoredObject::save(OMDataStream)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStoredObjectIdentification>
  //        of this <c OMKLVStoredObject> into <p id>.
  //   @parm The newly restored <t OMStoredObjectIdentification>.
void OMKLVStoredObject::restore(OMStoredObjectIdentification& /* id */)
{
  TRACE("OMKLVStoredObject::restore(OMStoredObjectIdentification)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMPropertySet> <p properties> into
  //        this <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMPropertySet>.
void OMKLVStoredObject::restore(OMPropertySet& /* properties */)
{
  TRACE("OMKLVStoredObject::restore(OMPropertySet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMSimpleProperty> <p property> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMSimpleProperty>
  //   @parm The external size.
void OMKLVStoredObject::restore(OMSimpleProperty& /* property */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMSimpleProperty)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReference> <p singleton> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMStrongReference>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMStrongReference& /* singleton */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMStrongReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReferenceVector> <p vector> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMStrongReferenceVector>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMStrongReferenceVector& /* vector */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMStrongReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReferenceSet> <p set> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMStrongReferenceSet>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMStrongReferenceSet& /* set */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMStrongReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReference> <p singleton> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMWeakReference>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMWeakReference& /* singleton */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMWeakReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReferenceVector> <p vector> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMWeakReferenceVector>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMWeakReferenceVector& /* vector */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMWeakReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReferenceSet> <p set> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMWeakReferenceSet>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMWeakReferenceSet& /* set */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMWeakReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMPropertyTable> in this <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMPropertyTable>.
void OMKLVStoredObject::restore(OMPropertyTable*& /* table */)
{
  TRACE("OMKLVStoredObject::restore(OMPropertyTable)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMDataStream> <p stream> into this
  //        <c OMKLVStoredObject>.
  //   @parm The newly restored <c OMDataStream>.
  //   @parm The external size.
void OMKLVStoredObject::restore(OMDataStream& /* stream */,
                                size_t /* externalSize */)
{
  TRACE("OMKLVStoredObject::restore(OMDataStream)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Open the <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMKLVStoredObject>.
  //   @parm The <c OMDataStream> to be opened.
  //   @rdesc The newly created <c OMStoredStream>.
OMStoredStream* OMKLVStoredObject::openStoredStream(
                                            const OMDataStream& /* property */)
{
  TRACE("OMKLVStoredObject::openStoredStream");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create an <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMKLVStoredObject>.
  //   @parm The <c OMDataStream> to be created.
  //   @rdesc The newly created <c OMStoredStream>.
OMStoredStream* OMKLVStoredObject::createStoredStream(
                                            const OMDataStream& /* property */)
{
  TRACE("OMKLVStoredObject::createStoredStream");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Constructor.
  //   @parm The <c OMRawStorage> on which this <c OMKLVStoredObject> resides.
  //   @parm TBS
OMKLVStoredObject::OMKLVStoredObject(OMRawStorage* s, OMByteOrder byteOrder)
: _storage(s),
  _byteOrder(byteOrder)
{
  TRACE("OMKLVStoredObject::OMKLVStoredObject");
}

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
#include "OMXMLStoredObject.h"

  // @mfunc Open the root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMXMLStoredObject* OMXMLStoredObject::openRead(OMRawStorage* /* rawStorage */)
{
  TRACE("OMXMLStoredObject::openRead");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Open the root <c OMXMLStoredObject> in the raw storage
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
  //        <p rawStorage> for modification.
OMXMLStoredObject* OMXMLStoredObject::openModify(
                                                OMRawStorage* /* rawStorage */)
{
  TRACE("OMXMLStoredObject::openModify");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMXMLStoredObject* OMXMLStoredObject::createModify(
                                             OMRawStorage* /* rawStorage */,
                                             const OMByteOrder /* byteOrder */)
{
  TRACE("OMXMLStoredObject::createModify");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Destructor.
OMXMLStoredObject::~OMXMLStoredObject(void)
{
  TRACE("OMXMLStoredObject::~OMXMLStoredObject");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Create a new <c OMXMLStoredObject>, named <p name>,
  //        contained by this <c OMXMLStoredObject>.
  //   @parm The name to be used for the new <c OMXMLStoredObject>.
  //   @rdesc A new <c OMXMLStoredObject> contained by this
  //          <c OMXMLStoredObject>.
OMStoredObject* OMXMLStoredObject::create(const wchar_t* /* name */)
{
  TRACE("OMXMLStoredObject::create");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
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
  ASSERT("Unimplemented code not reached", false); // tjb TBS
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
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMXMLStoredObject::save(OMStorable& object)
{
  TRACE("OMXMLStoredObject::save(OMFile)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMStoredObjectIdentification> <p id>
  //        in this <c OMXMLStoredObject>.
  //   @parm The <t OMStoredObjectIdentification> to save.
void OMXMLStoredObject::save(const OMStoredObjectIdentification& /* id */)
{
  TRACE("OMXMLStoredObject::save(OMStoredObjectIdentification)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMXMLStoredObject::save(const OMPropertySet& /* properties */)
{
  TRACE("OMXMLStoredObject::save(OMPropertySet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMSimpleProperty> <p property> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMSimpleProperty> to save.
void OMXMLStoredObject::save(const OMSimpleProperty& /* property */)
{
  TRACE("OMXMLStoredObject::save(OMSimpleProperty)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMStrongReference> <p singleton> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMXMLStoredObject::save(const OMStrongReference& /* singleton */)
{
  TRACE("OMXMLStoredObject::save(OMStrongReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMStrongReferenceVector> <p vector> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMStrongReferenceVector> to save.
void OMXMLStoredObject::save(const OMStrongReferenceVector& /* vector */)
{
  TRACE("OMXMLStoredObject::save(OMStrongReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMStrongReferenceSet> <p set> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMXMLStoredObject::save(const OMStrongReferenceSet& /* set */)
{
  TRACE("OMXMLStoredObject::save(OMStrongReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMWeakReference> <p singleton> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMWeakReference> to save.
void OMXMLStoredObject::save(const OMWeakReference& /* singleton */)
{
  TRACE("OMXMLStoredObject::save(OMWeakReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMWeakReferenceVector> <p vector> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMWeakReferenceVector> to save.
void OMXMLStoredObject::save(const OMWeakReferenceVector& /* vector */)
{
  TRACE("OMXMLStoredObject::save(OMWeakReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMWeakReferenceSet> <p set> in this
  //        <c OMXMLStoredObject>.
  //   @parm The <c OMWeakReferenceSet> to save.
void OMXMLStoredObject::save(const OMWeakReferenceSet& /* set */)
{
  TRACE("OMXMLStoredObject::save(OMWeakReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
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
void OMXMLStoredObject::save(const OMDataStream& /* stream */)
{
  TRACE("OMXMLStoredObject::save(OMDataStream)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
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
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Constructor.
  //   @parm The <c OMRawStorage> on which this <c OMXMLStoredObject> resides.
OMXMLStoredObject::OMXMLStoredObject(OMRawStorage* s)
: _storage(s)
{
  TRACE("OMXMLStoredObject::OMXMLStoredObject");
}

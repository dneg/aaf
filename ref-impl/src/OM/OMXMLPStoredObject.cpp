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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMXMLPStoredObject
#include "OMXMLPStoredObject.h"

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
#include "OMXMLPStoredStream.h"
#include "OMPropertySetIterator.h"
#include "OMDataVector.h"
#include "OMArrayType.h"
#include "OMDataContainerIterator.h"
#include "OMDataSet.h"
#include "OMSetType.h"

#include "OMClassFactory.h"
#include "OMClassDefinition.h"

#include "OMUtilities.h"

#include "OMPropertyDefinition.h"

#include "OMXMLUtilities.h"


  // @mfunc Open the root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMXMLPStoredObject* OMXMLPStoredObject::openRead(OMRawStorage* ANAME(rawStorage))
{
  TRACE("OMXMLPStoredObject::openRead");
  PRECONDITION("Compatible raw storage access mode", rawStorage->isReadable());
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Open the root <c OMXMLPStoredObject> in the raw storage
  //   @parm The raw storage in which to open the file.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
  //        <p rawStorage> for modification.
OMXMLPStoredObject* OMXMLPStoredObject::openModify(
                                               OMRawStorage* ANAME(rawStorage))
{
  TRACE("OMXMLPStoredObject::openModify");
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMXMLPStoredObject* OMXMLPStoredObject::createWrite(OMRawStorage* rawStorage)
{
  TRACE("OMXMLPStoredObject::createWrite");
  PRECONDITION("Compatible raw storage access mode", rawStorage->isWritable());
  OMXMLPStoredObject* result = new OMXMLPStoredObject(
    new OMXMLStorage(rawStorage, false), true);
  return result;
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the file.
  //   @parm The desired byte ordering for the new file.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMXMLPStoredObject* OMXMLPStoredObject::createModify(OMRawStorage* rawStorage)
{
  TRACE("OMXMLPStoredObject::createModify");

  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  OMXMLPStoredObject* result = new OMXMLPStoredObject(
    new OMXMLStorage(rawStorage, false), true);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Destructor.
OMXMLPStoredObject::~OMXMLPStoredObject(void)
{
  TRACE("OMXMLPStoredObject::~OMXMLPStoredObject");

  if (_isRoot)
  {
    delete _store;
  }
}

  // @mfunc Create a new <c OMXMLPStoredObject>, named <p name>,
  //        contained by this <c OMXMLPStoredObject>.
  //   @parm The name to be used for the new <c OMXMLPStoredObject>.
  //   @rdesc A new <c OMXMLPStoredObject> contained by this
  //          <c OMXMLPStoredObject>.
OMStoredObject* OMXMLPStoredObject::create(const wchar_t* /* name */)
{
  TRACE("OMXMLPStoredObject::create");
  OMStoredObject* result = new OMXMLPStoredObject(_store, false);
  return result;
}

  // @mfunc Open an exsiting <c OMXMLPStoredObject>, named <p name>,
  //        contained by this <c OMXMLPStoredObject>.
  //   @parm The name of the existing <c OMXMLPStoredObject>.
  //   @rdesc The existing <c OMXMLPStoredObject> contained by this
  //          <c OMXMLPStoredObject>.
OMStoredObject* OMXMLPStoredObject::open(const wchar_t* /* name */)
{
  TRACE("OMXMLPStoredObject::open");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Close this <c OMXMLPStoredObject>.
void OMXMLPStoredObject::close(void)
{
  TRACE("OMXMLPStoredObject::close");
  if (_isRoot && !_store->isRead())
  {
    _store->getWriter()->synchronize();
  }
}

  // @mfunc The byte order of this <c OMXMLPStoredObject>.
  //   @rdesc The byte order.
  //   @this const
OMByteOrder OMXMLPStoredObject::byteOrder(void) const
{
  TRACE("OMXMLPStoredObject::byteOrder");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return unspecified;
}

void OMXMLPStoredObject::save(OMFile& file)
{
  TRACE("OMXMLPStoredObject::save(OMFile)");
  _store->getWriter()->writeDocumentStart();
  
  file.root()->save();
  
  _store->getWriter()->writeDocumentEnd();
}

void OMXMLPStoredObject::save(OMStorable& object)
{
  TRACE("OMXMLPStoredObject::save(OMStorable)");
  
  if (object.isRoot())
  {
      _store->getWriter()->writeComment(L"Note: the AAF-XML format is subject to change");
      _store->getWriter()->writeElementStart(_store->getBaselineSymbolspace(), L"AAF");
      _store->getWriter()->declareNamespace(_store->getBaselineSymbolspace(), 0);
      _store->getWriter()->declareNamespace(_store->getBaselineSymbolspace(), 
        _store->getBaselinePrefix());
  }
  else
  {
      const wchar_t* symbolspace;
      const wchar_t* symbol;
      if (_store->getSymbol(object.classId(), &symbolspace, &symbol))
      {
          _store->getWriter()->writeElementStart(symbolspace, symbol);
      }
      else
      {
          // this will be removed
          _store->getWriter()->writeElementStart(_store->getBaselineSymbolspace(), 
            L"Todo-Object");
      }
  }

  if (_store->haveForwardedObjectSetId())
  {
      wchar_t* id = _store->getForwardedObjectSetId();
      _store->getWriter()->writeAttribute(_store->getBaselineSymbolspace(), L"id", id);
      delete [] id;
  }
  
  save(*object.propertySet());
  
  _store->getWriter()->writeElementEnd();
}

  // @mfunc Save the <c OMStoredObjectIdentification> <p id>
  //        in this <c OMXMLPStoredObject>.
  //   @parm The <t OMStoredObjectIdentification> to save.
void OMXMLPStoredObject::save(const OMStoredObjectIdentification& id)
{
  TRACE("OMXMLPStoredObject::save(OMStoredObjectIdentification)");
}

  // @mfunc Save the <c OMPropertySet> <p properties> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMPropertySet> to save.
void OMXMLPStoredObject::save(const OMPropertySet& properties )
{
  TRACE("OMXMLPStoredObject::save(OMPropertySet)");

    OMPropertySetIterator iterator(properties, OMBefore);
    while (++iterator)
    {
        const OMProperty* property = iterator.property();
        if (property->propertyId() == 0x0001)
        {
            _store->getWriter()->writeElementStart(_store->getBaselineSymbolspace(), 
                L"Extensions");
            _store->getWriter()->writeComment(L"Todo");
            _store->getWriter()->writeElementEnd();
            continue;
        }
        
        if (!property->isOptional() || property->isPresent())
        {
            if (property->propertyId() != 0x0002) // != Root::Header
            {
                const wchar_t* symbolspace;
                const wchar_t* symbol;
                OMUniqueObjectIdentification id = 
                    property->definition()->uniqueIdentification();
                if (_store->getSymbol(id, &symbolspace, &symbol))
                {
                    _store->getWriter()->writeElementStart(symbolspace, symbol);
                }
                else
                {
                    // this will be removed
                    _store->getWriter()->writeElementStart(_store->getBaselineSymbolspace(), 
                        L"Todo-Property");
                }
            }
            
            property->save();
            
            if (property->propertyId() != 0x0002) // != Root::Header
            {
                _store->getWriter()->writeElementEnd();
            }
        }
    }
  
}

  // @mfunc Save the <c OMSimpleProperty> <p property> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMSimpleProperty> to save.
void OMXMLPStoredObject::save(const OMSimpleProperty& property)
{
  TRACE("OMXMLPStoredObject::save(OMSimpleProperty)");
}

  // @mfunc Save the <c OMDataVector> <p property> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMDataVector> to save.
void OMXMLPStoredObject::save(const OMDataVector& property)
{
  TRACE("OMXMLPStoredObject::save(OMDataVector)");

}

void OMXMLPStoredObject::save(const OMDataSet& property)
{
  TRACE("OMXMLPStoredObject::save(OMDataSet)");
}

  // @mfunc Save the <c OMStrongReference> <p singleton> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMXMLPStoredObject::save(const OMStrongReference& singleton)
{
  TRACE("OMXMLPStoredObject::save(OMStrongReference)");
  singleton.reference().save();
}

  // @mfunc Save the <c OMStrongReferenceVector> <p vector> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMStrongReferenceVector> to save.
void OMXMLPStoredObject::save(const OMStrongReferenceVector& vector)
{
  TRACE("OMXMLPStoredObject::save(OMStrongReferenceVector)");

    OMContainerIterator<OMStrongReferenceVectorElement>& iterator =
        *vector.iterator();
    while (++iterator)
    {
        OMStrongReferenceVectorElement& element = iterator.value();
        element.save();
    }
    delete &iterator;
}

  // @mfunc Save the <c OMStrongReferenceSet> <p set> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMStrongReference> to save.
void OMXMLPStoredObject::save(const OMStrongReferenceSet& set)
{
  TRACE("OMXMLPStoredObject::save(OMStrongReferenceSet)");

    OMKeySize keySize = set.keySize();

    OMContainerIterator<OMStrongReferenceSetElement>& iterator = *set.iterator();
    while (++iterator)
    {
        OMStrongReferenceSetElement& element = iterator.value();
        
        if (keySize == sizeof(OMUniqueObjectIdentification))
        {
            OMUniqueObjectIdentification* oid = 
                reinterpret_cast<OMUniqueObjectIdentification*>(
                    element.identification());
            const wchar_t* stringId = _store->getStringId(*oid);
            if (stringId != 0)
            {
                _store->forwardObjectSetId(stringId);
            }
            else
            {
                wchar_t uri[XML_MAX_OID_URI_SIZE];
                oidToURI(*oid, uri);
                _store->forwardObjectSetId(uri);
            }
        }
        else if (keySize == sizeof(OMMaterialIdentification))
        {
            OMMaterialIdentification* umid = 
                reinterpret_cast<OMMaterialIdentification*>(
                    element.identification());
            wchar_t uri[XML_MAX_UMID_URI_SIZE];
            umidToURI(*umid, uri);
            _store->forwardObjectSetId(uri);
        }
        else
        {
            ASSERT("Unimplemented code reached - only AUID or MobID as set id supported", false);
        }
            
        element.save();
    }
    delete &iterator;
}

  // @mfunc Save the <c OMWeakReference> <p singleton> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMWeakReference> to save.
void OMXMLPStoredObject::save(const OMWeakReference& singleton)
{
  TRACE("OMXMLPStoredObject::save(OMWeakReference)");

    OMUniqueObjectIdentification oid = singleton.reference().identification();
    const wchar_t* stringId = _store->getStringId(oid);
    if (stringId != 0)
    {
        _store->getWriter()->writeAttribute(_store->getBaselineSymbolspace(), L"ref", 
            stringId);
    }
    else
    {
        wchar_t uri[XML_MAX_OID_URI_SIZE];
        oidToURI(oid, uri);
        _store->getWriter()->writeAttribute(_store->getBaselineSymbolspace(), L"ref", uri);
    }
}

  // @mfunc Save the <c OMWeakReferenceVector> <p vector> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMWeakReferenceVector> to save.
void OMXMLPStoredObject::save(const OMWeakReferenceVector& vector)
{
  TRACE("OMXMLPStoredObject::save(OMWeakReferenceVector)");

    OMContainerIterator<OMWeakReferenceVectorElement>& iterator = *vector.iterator();
    while (++iterator)
    {
        OMWeakReferenceVectorElement& element = iterator.value();
        OMObjectIdentification oid = element.reference().identification();

        _store->getWriter()->writeElementStart(_store->getBaselineSymbolspace(), L"Todo-WeakRefTypeSymbol");
        
        const wchar_t* stringId = _store->getStringId(oid);
        if (stringId != 0)
        {
            _store->getWriter()->writeAttribute(_store->getBaselineSymbolspace(), L"ref", 
                stringId);
        }
        else
        {
            wchar_t uri[XML_MAX_OID_URI_SIZE];
            oidToURI(oid, uri);
            _store->getWriter()->writeAttribute(_store->getBaselineSymbolspace(), L"ref", uri);
        }
        
        _store->getWriter()->writeElementEnd();
    }
    delete &iterator;
}

  // @mfunc Save the <c OMWeakReferenceSet> <p set> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMWeakReferenceSet> to save.
void OMXMLPStoredObject::save(const OMWeakReferenceSet& set)
{
  TRACE("OMXMLPStoredObject::save(OMWeakReferenceSet)");

    OMContainerIterator<OMWeakReferenceSetElement>& iterator = *set.iterator();
    while (++iterator)
    {
        OMWeakReferenceSetElement& element = iterator.value();
        OMObjectIdentification oid = element.reference().identification();

        _store->getWriter()->writeElementStart(_store->getBaselineSymbolspace(), L"Todo-WeakRefTypeSymbol");
        
        const wchar_t* stringId = _store->getStringId(oid);
        if (stringId != 0)
        {
            _store->getWriter()->writeAttribute(_store->getBaselineSymbolspace(), L"ref", 
                stringId);
        }
        else
        {
            wchar_t uri[XML_MAX_OID_URI_SIZE];
            oidToURI(oid, uri);
            _store->getWriter()->writeAttribute(_store->getBaselineSymbolspace(), L"ref", uri);
        }
        
        _store->getWriter()->writeElementEnd();
    }
    delete &iterator;
}

  // @mfunc Save the <c OMPropertyTable> <p table> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMPropertyTable> to save.
void OMXMLPStoredObject::save(const OMPropertyTable* /* table */)
{
  TRACE("OMXMLPStoredObject::save(OMPropertyTable)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Save the <c OMDataStream> <p stream> in this
  //        <c OMXMLPStoredObject>.
  //   @parm The <c OMDataStream> to save.
void OMXMLPStoredObject::save(const OMDataStream& stream)
{
  TRACE("OMXMLPStoredObject::save(OMDataStream)");
}

OMRootStorable* OMXMLPStoredObject::restore(OMFile& /* file */)
{
  TRACE("OMXMLPStoredObject::restore(OMFile)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  OMRootStorable* result = 0;
  return result;
}

OMStorable*
OMXMLPStoredObject::restoreObject(const OMStrongObjectReference& reference)
{
  TRACE("OMXMLPStoredObject::restoreObject");

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
  //        of this <c OMXMLPStoredObject> into <p id>.
  //   @parm The newly restored <t OMStoredObjectIdentification>.
void OMXMLPStoredObject::restore(OMStoredObjectIdentification& /* id */)
{
  TRACE("OMXMLPStoredObject::restore(OMStoredObjectIdentification)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMPropertySet> <p properties> into
  //        this <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMPropertySet>.
void OMXMLPStoredObject::restore(OMPropertySet& /* properties */)
{
  TRACE("OMXMLPStoredObject::restore(OMPropertySet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMSimpleProperty> <p property> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMSimpleProperty>
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMSimpleProperty& /* property */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMSimpleProperty)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMDataVector> <p property> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMDataVector>
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMDataVector& /* property */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMDataVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

void OMXMLPStoredObject::restore(OMDataSet& /* property */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMDataSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReference> <p singleton> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMStrongReference>.
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMStrongReference& /* singleton */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMStrongReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReferenceVector> <p vector> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMStrongReferenceVector>.
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMStrongReferenceVector& /* vector */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMStrongReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMStrongReferenceSet> <p set> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMStrongReferenceSet>.
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMStrongReferenceSet& /* set */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMStrongReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReference> <p singleton> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMWeakReference>.
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMWeakReference& /* singleton */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMWeakReference)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReferenceVector> <p vector> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMWeakReferenceVector>.
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMWeakReferenceVector& /* vector */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMWeakReferenceVector)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMWeakReferenceSet> <p set> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMWeakReferenceSet>.
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMWeakReferenceSet& /* set */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMWeakReferenceSet)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMPropertyTable> in this <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMPropertyTable>.
void OMXMLPStoredObject::restore(OMPropertyTable*& /* table */)
{
  TRACE("OMXMLPStoredObject::restore(OMPropertyTable)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Restore the <c OMDataStream> <p stream> into this
  //        <c OMXMLPStoredObject>.
  //   @parm The newly restored <c OMDataStream>.
  //   @parm The external size.
void OMXMLPStoredObject::restore(OMDataStream& /* stream */,
                                size_t /* externalSize */)
{
  TRACE("OMXMLPStoredObject::restore(OMDataStream)");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
}

  // @mfunc Open the <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMXMLPStoredObject>.
  //   @parm The <c OMDataStream> to be opened.
  //   @rdesc The newly created <c OMStoredStream>.
OMStoredStream* OMXMLPStoredObject::openStoredStream(
                                            const OMDataStream& /* property */)
{
  TRACE("OMXMLPStoredObject::openStoredStream");
  ASSERT("Unimplemented code not reached", false); // tjb TBS
  return 0;
}

  // @mfunc Create an <c OMStoredStream> representing the property
  //        <p stream> contained within this <c OMXMLPStoredObject>.
  //   @parm The <c OMDataStream> to be created.
  //   @rdesc The newly created <c OMStoredStream>.
OMStoredStream* OMXMLPStoredObject::createStoredStream(
                                            const OMDataStream& /* property */)
{
  TRACE("OMXMLPStoredObject::createStoredStream");

  OMRawStorage* store = OMCachedDiskRawStorage::openNewModify();
  OMXMLPStoredStream* result = new OMXMLPStoredStream(store);
  ASSERT("Valid heap pointer", result != 0);
  return result;
}

  // @mfunc Constructor.
  //   @parm The <c OMRawStorage> on which this <c OMXMLPStoredObject> resides.
  //   @parm TBS
OMXMLPStoredObject::OMXMLPStoredObject(OMXMLStorage* store, bool isRoot)
: _store(store), _isRoot(isRoot)
{
  TRACE("OMXMLPStoredObject::OMXMLPStoredObject");
}



/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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

// @doc OMEXTERNAL
#include "OMPropertyBase.h"

#include "OMProperty.h"

#include "OMStorable.h"
#include "OMType.h"
#include "OMUtilities.h"
#include "OMPropertyDefinition.h"

#include <memory.h>

// class OMProperty

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The stored form of this <c OMProperty>.
  //   @parm The name of this <c OMProperty>.
OMProperty::OMProperty(const OMPropertyId propertyId,
                       const int storedForm,
                       const char* name)
: _propertyId(propertyId), _storedForm(storedForm), _name(name),
  _propertySet(0), _definition(0), _type(0),
  // _isOptional(false),
  // BobT: make optional by default, to hack around problem where
  // props may be restored before they're initialized by DM.
  _isOptional(true),
  _isPresent(false)
{
  TRACE("OMProperty::OMProperty");

  PRECONDITION("Valid name", validString(_name)); 
}

  // @mfunc Temporary pseudo-constructor for clients which provide
  //         a type definition.
  //   @parm The property id.
  //   @parm The name of this <c OMProperty>.
  //   @parm The type of this <c OMProperty>.
  //   @parm True if this is an optional property, false otherwise.
void OMProperty::initialize(const OMPropertyId propertyId,
                            const char* name,
                            OMType* type,
                            const bool isOptional)
{
  TRACE("OMProperty::initialize");

  PRECONDITION("Valid property name", validString(name));
  // PRECONDITION("Valid type", type != 0);

  // Temporary consistency checks
  ASSERT("Consistent property id", propertyId == OMProperty::propertyId());
  ASSERT("Consistent property name", strcmp(name, OMProperty::name()) == 0);
  ASSERT("Not initialized by property definition", _definition == 0);

  _type = type;
  _isOptional = isOptional;
}

  // @mfunc Temporary pseudo-constructor for clients which provide
  //        a property definition.
  //   @parm The definition of this <c OMProperty>.
void OMProperty::initialize(const OMPropertyDefinition* definition)
{
  TRACE("OMProperty::initialize");

  PRECONDITION("Valid property definition", definition != 0);
  _definition = definition;

  // Temporary consistency checks
  ASSERT("Consistent property id",
                            _propertyId == _definition->localIdentification());
  ASSERT("Consistent property name", strcmp(_name, _definition->name()) == 0);
  // ASSERT("Consistent property optionality",
  //                                 _isOptional == _definition->isOptional());
  ASSERT("Not initialized by type", _type == 0);
  _isOptional = _definition->isOptional();
}

  // @mfunc Destructor.
OMProperty::~OMProperty(void)
{
  TRACE("OMProperty::~OMProperty");
}

  // @mfunc Close this <c OMProperty>.
void OMProperty::close(void)
{
  TRACE("OMProperty::close");

  // nothing to do for most descendants of OMProperty
}

  // @mfunc Detach this <c OMProperty>.
void OMProperty::detach(void)
{
  TRACE("OMProperty::detach");

  // nothing to do for most descendants of OMProperty
}

  // @mfunc The <c OMPropertyDefinition> defining this <c OMProperty>.
  //   @rdesc The defining <c OMPropertyDefinition>.
  //   @this const 
const OMPropertyDefinition* OMProperty::definition(void) const
{
  TRACE("OMProperty::definition");

  const OMPropertyDefinition* result = _definition;
  POSTCONDITION("Valid result", result != 0);
  return result;
}

  // @mfunc The name of this <c OMProperty>.
  //   @rdesc The property name.
  //   @this const
const char* OMProperty::name(void) const
{
  TRACE("OMProperty::name");

  return _name;
}

  // @mfunc The property id of this <c OMProperty>.
  //   @rdesc The property id.
  //   @this const
OMPropertyId OMProperty::propertyId(void) const
{
  TRACE("OMProperty::propertyId");

  return _propertyId;
}

  // @mfunc The <c OMPropertySet> containing this <c OMProperty>.
  //   @rdesc The containing <c OMPropertySet>.
  //   @this const
const OMPropertySet* OMProperty::propertySet(void) const
{
  TRACE("OMProperty::propertySet");

  return _propertySet;
}

  // @mfunc Inform this <c OMProperty> that it is a member of
  //        the <c OMPropertySet> <p propertySet>.
  //   @parm The <c OMPropertySet> of which this <c OMProperty> is
  //   a member.
void OMProperty::setPropertySet(const OMPropertySet* propertySet)
{
  TRACE("OMProperty::setPropertySet");
  PRECONDITION("Valid property set", propertySet != 0);

  _propertySet = propertySet;
}

  // @mfunc The address of this <c OMProperty> object. This function
  //        is defined so that descendants may override "operator &" to
  //        obtain the address of the contained property value. This
  //        function can then be used to obtain the address of this
  //        <c OMProperty>.
  //   @rdesc The address of this <c OMProperty>.
OMProperty* OMProperty::address(void)
{
  TRACE("OMProperty::address");

  return this;
}

  // @mfunc Write this property to persistent store, performing
  //        any necessary externalization and byte reordering.
  //   @parm The property id.
  //   @parm The stored form to use for this property.
  //   @parm The bytes of this property in internal (in memory) form.
  //   @parm The actual size of the bytes of this property.
  //   @this const
void OMProperty::write(OMPropertyId propertyId,
                       int storedForm,
                       OMByte* internalBytes,
                       size_t internalBytesSize) const
{
  TRACE("OMProperty::write");

  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size", internalBytesSize > 0);

  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* container = _propertySet->container();
  ASSERT("Valid container", container != 0);
  ASSERT("Container is persistent", container->persistent());
  OMStoredObject* store = container->store();
  ASSERT("Valid stored object", store != 0);

  // BobT: temporarily remove this precondition to allow either
  // _definition or _type to be set (but not both).
  // PRECONDITION("Valid property definition", _definition != 0);

  const OMType* type = _type;
  if (_definition != 0) {
    type = _definition->type();
    PRECONDITION("Both _type and _definition not set", _type == 0);
  }
  if (type != 0) { // tjb - temporary, should be ASSERTION below

    ASSERT("Valid property type", type != 0);
 
    // Allocate buffer for property value
    size_t externalBytesSize = type->externalSize(internalBytes,
                                                  internalBytesSize);
    OMByte* buffer = new OMByte[externalBytesSize];
    ASSERT("Valid heap pointer", buffer != 0);

    // Externalize property value
    type->externalize(internalBytes,
                      internalBytesSize,
                      buffer,
                      externalBytesSize,
                      store->byteOrder());
  
    // Reorder property value
    if (store->byteOrder() != hostByteOrder()) {
      type->reorder(buffer, externalBytesSize);
    }

    // Write property value
    store->write(propertyId, storedForm, buffer, externalBytesSize);
    delete [] buffer;

  } else {
    // tjb - temporary, no type information, do it the old way
    //
    store->write(propertyId, storedForm, internalBytes, internalBytesSize);
  }
}

  // @mfunc Read this property from persistent store, performing
  //        any necessary byte reordering and internalization.
  //   @parm The property id.
  //   @parm The stored from to use for this property.
  //   @parm The buffer in which to place the internal (in memory)
  //         form of the bytes of this property.
  //   @parm The size of the buffer in which to place the internal
  //         (in memory) form of the bytes of this property.
  //   @parm The size of the external (on disk) form of the bytes of
  //         this propery.
  //   @this const
void OMProperty::read(OMPropertyId propertyId,
                      int storedForm,
                      OMByte* internalBytes,
                      size_t internalBytesSize,
                      size_t externalBytesSize)
{
  TRACE("OMProperty::read");

  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size", internalBytesSize > 0);
  PRECONDITION("Valid external bytes size", externalBytesSize > 0);

  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  // BobT: temporarily remove this precondition to allow either
  // _definition or _type to be set (but not both).
  // PRECONDITION("Valid property definition", _definition != 0);

  const OMType* type = _type;
  if (_definition != 0) {
    type = _definition->type();
    PRECONDITION("Both _type and _definition not set", _type == 0);
  }

  if (type != 0) { // tjb - temporary, should be ASSERTION below

    ASSERT("Valid property type", type != 0);

    // Allocate buffer for property value
    OMByte* buffer = new OMByte[externalBytesSize];
    ASSERT("Valid heap pointer", buffer != 0);

    // Read property value
    store->read(propertyId, storedForm, buffer, externalBytesSize);

    // Reorder property value
    if (store->byteOrder() != hostByteOrder()) {
      type->reorder(buffer, externalBytesSize);
    }

    // Internalize property value
    size_t requiredBytesSize = type->internalSize(buffer, externalBytesSize);
    ASSERT("Property value buffer large enough",
                                       internalBytesSize >= requiredBytesSize);

    type->internalize(buffer,
                      externalBytesSize,
                      internalBytes,
                      requiredBytesSize,
                      hostByteOrder());
    delete [] buffer;
  } else {
    // tjb - temporary, no type information, do it the old way
    //
    ASSERT("Property value buffer large enough",
                                       internalBytesSize >= externalBytesSize);
    store->read(propertyId, storedForm, internalBytes, externalBytesSize);
  }
  setPresent();
}

  // @mfunc Set the bit that indicates that this optional <c OMProperty>
  //        is present.
void OMProperty::setPresent(void)
{
  TRACE("OMProperty::setPresent");

  _isPresent = true;
}

  // @mfunc Clear the bit that indicates that this optional <c OMProperty>
  //        is present.
void OMProperty::clearPresent(void)
{
  TRACE("OMProperty::clearPresent");
  PRECONDITION("Property is optional", isOptional());

  _isPresent = false;
}

// class OMSimpleProperty

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMSimpleProperty>.
OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const char* name)
: OMProperty(propertyId, SF_DATA, name), _size(0), _bits(0)
{
  TRACE("OMSimpleProperty::OMSimpleProperty");
}

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMSimpleProperty>.
  //   @parm The size of this <c OMSimpleProperty>.
OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const char* name,
                                   size_t valueSize)
: OMProperty(propertyId, SF_DATA, name),
  _size(valueSize),
  _bits(0)
{
  TRACE("OMSimpleProperty::OMSimpleProperty");
  PRECONDITION("Valid size", (valueSize > 0));

  _bits = new unsigned char[valueSize];
  ASSERT("Valid heap pointer", _bits != 0);

  for (size_t i = 0; i < valueSize; i++) {
    _bits[i] = 0;
  }

  POSTCONDITION("Valid bits", _bits != 0 );
}

  // @mfunc Destructor.
OMSimpleProperty::~OMSimpleProperty(void)
{
  TRACE("OMSimpleProperty::~OMSimpleProperty");

  delete [] _bits;
}

  // @mfunc The size of this <c OMSimpleProperty>.
  //   @rdesc The property size in bytes.
  //   @this const
size_t OMSimpleProperty::size(void) const
{
  TRACE("OMSimpleProperty::size");

  return _size;
}

  // @mfunc Get the value of this <c OMSimpleProperty>.
  //   @parm The buffer to receive the property value.
  //   @parm The size of the buffer.
  //   @this const
void OMSimpleProperty::get(void* value, size_t valueSize) const
{
  TRACE("OMSimpleProperty::get");
  PRECONDITION("Valid data buffer", value != 0);
  PRECONDITION("Valid size", valueSize >= _size);
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  memcpy(value, _bits, _size);
}

  // @mfunc Set the value of this <c OMSimpleProperty>.
  //   @parm The address of the property value.
  //   @parm The size of the value.
  //   @this const
void OMSimpleProperty::set(const void* value, size_t valueSize) 
{
  TRACE("OMSimpleProperty::set");
  PRECONDITION("Valid data buffer", value != 0);
  PRECONDITION("Valid size", valueSize > 0);

  if (valueSize != _size) {
    delete [] _bits;
    _bits = 0; // for BoundsChecker
    _bits = new unsigned char[valueSize];
    ASSERT("Valid heap pointer", _bits != 0);
    _size = valueSize;
  }
  memcpy(_bits, value, _size);
  setPresent();
}

  // @mfunc Save this <c OMSimpleProperty>.
  //   @parm Client context for callbacks.
  //   @this const
void OMSimpleProperty::save(void* /* clientContext */) const
{
  TRACE("OMSimpleProperty::save");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  write(_propertyId, _storedForm, _bits, _size);
}

  // @mfunc Restore this <c OMSimpleProperty>, the external (persisted)
  //        size of the <c OMSimpleProperty> is <p externalSize>.
  //   @parm The size of the <c OMSimpleProperty>.
void OMSimpleProperty::restore(size_t externalSize)
{
  TRACE("OMSimpleProperty::restore");
  ASSERT("Sizes match", externalSize == _size);

  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  read(_propertyId, _storedForm, _bits, _size, externalSize);
}

  // @mfunc Is this an optional property ? 
  //   @rdesc True if this property is optional, false otherwise.
  //   @this const
bool OMProperty::isOptional(void) const
{
  TRACE("OMProperty::isOptional");
  return _isOptional;
}

  // @mfunc Is this optional property present ?
  //   @rdesc True if this property is present, false otherwise.
  //   @this const
bool OMProperty::isPresent(void) const
{
  TRACE("OMProperty::isPresent");

  PRECONDITION("Property is optional", isOptional());
  return _isPresent;
}

  // @mfunc Remove this optional <c OMProperty>.
void OMProperty::remove(void)
{
  TRACE("OMProperty::remove");
  PRECONDITION("Property is optional", isOptional());
  PRECONDITION("Optional property is present", isPresent());
  clearPresent();
  POSTCONDITION("Optional property no longer present", !isPresent());
}

  // @mfunc The size of the raw bits of this
  //        <c OMSimpleProperty>. The size is given in bytes.
  //   @rdesc The size of the raw bits of this
  //          <c OMSimpleProperty> in bytes.
  //   @this const
size_t OMSimpleProperty::bitsSize(void) const
{
  TRACE("OMSimpleProperty::bitsSize");

  return _size;
}

  // @mfunc Get the raw bits of this <c OMSimpleProperty>.
  //        The raw bits are copied to the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @parm The address of the buffer into which the raw bits are copied.
  //   @parm The size of the buffer.
  //   @this const
void OMSimpleProperty::getBits(OMByte* bits, size_t bitsSize) const
{
  TRACE("OMSimpleProperty::getBits");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", bitsSize >= _size);

  memcpy(bits, _bits, _size);
}

  // @mfunc Set the raw bits of this <c OMSimpleProperty>.
  //        The raw bits are copied from the buffer at address <p bits> which
  //        is <p size> bytes in size.
  //   @parm The address of the buffer from which the raw bits are copied.
  //   @parm The size of the buffer.
void OMSimpleProperty::setBits(const OMByte* bits, size_t size)
{
  TRACE("OMSimpleProperty::setBits");
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", size > 0);

  set(bits, size);
}

// class OMStringProperty

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMStringProperty>.
OMStringProperty::OMStringProperty(const OMPropertyId propertyId,
                                   const char* name)
: OMCharacterStringProperty<char>(propertyId, name)
{
  TRACE("OMStringProperty::OMStringProperty");
  PRECONDITION("Valid name", validString(name));
}

  // @mfunc Destructor.
OMStringProperty::~OMStringProperty(void)
{
  TRACE("OMStringProperty::~OMStringProperty");
}

  // @mfunc Assignment operator.
  //   @parm The new value for this <c OMStringProperty>.
  //   @rdesc The <c OMStringProperty> resulting from the assignment.
OMStringProperty& OMStringProperty::operator = (const char* value)
{
  TRACE("OMStringProperty::operator =");
  PRECONDITION("Valid value", value != 0);

  assign(value);
  return *this;
}

// class OMWideStringProperty

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMWideStringProperty>.
OMWideStringProperty::OMWideStringProperty(const OMPropertyId propertyId,
                                           const char* name)
: OMCharacterStringProperty<wchar_t>(propertyId, name)
{
  TRACE("OMWideStringProperty::OMWideStringProperty");
  PRECONDITION("Valid name", validString(name));
}

  // @mfunc Destructor.
OMWideStringProperty::~OMWideStringProperty(void)
{
  TRACE("OMWideStringProperty::~OMWideStringProperty");
}

  // @mfunc Assignment operator.
  //   @parm The new value for this <c OMWideStringProperty>.
  //   @rdesc The <c OMWideStringProperty> resulting from the assignment.
OMWideStringProperty& OMWideStringProperty::operator = (const wchar_t* value)
{
  TRACE("OMWideStringProperty::operator =");
  PRECONDITION("Valid string", validWideString(value));

  assign(value);
  return *this;
}

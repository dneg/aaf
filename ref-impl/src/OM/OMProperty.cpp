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

// @doc OMEXTERNAL

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
                       const OMStoredForm storedForm,
                       const wchar_t* name)
: _propertyId(propertyId), _storedForm(storedForm), _name(name), _cName(0),
  _propertySet(0), _definition(0),
  // _isOptional(false),
  // BobT: make optional by default, to hack around problem where
  // props may be restored before they're initialized by DM.
  _isOptional(true),
  _isPresent(false)
{
  TRACE("OMProperty::OMProperty");

  PRECONDITION("Valid name", validWideString(_name)); 
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
  ASSERT("Consistent property name",
                           compareWideString(_name, _definition->name()) == 0);
  // ASSERT("Consistent property optionality",
  //                                 _isOptional == _definition->isOptional());
  _isOptional = _definition->isOptional();
}

  // @mfunc Destructor.
OMProperty::~OMProperty(void)
{
  TRACE("OMProperty::~OMProperty");

  delete [] _cName;
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

  if (_cName == 0) {
    OMProperty* nonConstThis = const_cast<OMProperty*>(this);
    nonConstThis->_cName = convertWideString(_name);
  }
  return _cName;
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

  // @mfunc The type of this <c OMProperty>.
  //   @rdesc The type.
  //   @this const
const OMType* OMProperty::type(void) const
{
  TRACE("OMProperty::type");

  // PRECONDITION("Valid property definition", _definition != 0);

  const OMType* result = 0;
  if (_definition != 0) {
    result = _definition->type();
  }
  return result;
}

// class OMSimpleProperty

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMSimpleProperty>.
OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const wchar_t* name)
: OMProperty(propertyId, SF_DATA, name), _size(0), _bits(0)
{
  TRACE("OMSimpleProperty::OMSimpleProperty");
}

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The name of this <c OMSimpleProperty>.
  //   @parm The size of this <c OMSimpleProperty>.
OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const wchar_t* name,
                                   size_t valueSize)
: OMProperty(propertyId, SF_DATA, name),
  _size(0),
  _bits(0)
{
  TRACE("OMSimpleProperty::OMSimpleProperty");
  PRECONDITION("Valid size", (valueSize > 0));

  setSize(valueSize);
  for (size_t i = 0; i < _size; i++) {
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

  // @mfunc Set the size of this <c OMSimpleProperty> to <p newSize> bytes.
  //   @parm The new property size in bytes.
void OMSimpleProperty::setSize(size_t newSize)
{
  TRACE("OMSimpleProperty::setSize");

  PRECONDITION("Valid size", newSize > 0);

  if (newSize != _size) {
    delete [] _bits;
    _bits = 0; // for BoundsChecker
    _bits = new unsigned char[newSize];
    ASSERT("Valid heap pointer", _bits != 0);
    _size = newSize;
  }
}

  // @mfunc Write this property to persistent store, performing
  //        any necessary externalization and byte reordering.
  //   @this const
void OMSimpleProperty::write(void) const
{
  TRACE("OMSimpleProperty::write");

  PRECONDITION("Valid internal bytes", _bits != 0);
  PRECONDITION("Valid internal bytes size", _size > 0);

  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* container = _propertySet->container();
  ASSERT("Valid container", container != 0);
  ASSERT("Container is persistent", container->persistent());
  OMStoredObject* store = container->store();
  ASSERT("Valid stored object", store != 0);

  const OMType* propertyType = type();

  if (propertyType != 0) { // tjb - temporary, should be ASSERTION below

    ASSERT("Valid property type", propertyType != 0);
 
    // Allocate buffer for property value
    size_t externalBytesSize = propertyType->externalSize(_bits,
                                                          _size);
    OMByte* buffer = new OMByte[externalBytesSize];
    ASSERT("Valid heap pointer", buffer != 0);

    // Externalize property value
    propertyType->externalize(_bits,
                              _size,
                              buffer,
                              externalBytesSize,
                              store->byteOrder());
  
    // Reorder property value
    if (store->byteOrder() != hostByteOrder()) {
      propertyType->reorder(buffer, externalBytesSize);
    }

    // Write property value
    store->write(_propertyId, _storedForm, buffer, externalBytesSize);
    delete [] buffer;

  } else {
    // tjb - temporary, no type information, do it the old way
    //
    store->write(_propertyId, _storedForm, _bits, _size);
  }
}

  // @mfunc Read this property from persistent store, performing
  //        any necessary byte reordering and internalization.
  //   @parm The size of the external (on disk) form of the bytes of
  //         this propery.
  //   @this const
void OMSimpleProperty::read(size_t externalBytesSize)
{
  TRACE("OMSimpleProperty::read");

  PRECONDITION("Valid external bytes size", externalBytesSize > 0);

  OMStoredObject* store = _propertySet->container()->store();
  ASSERT("Valid store", store != 0);

  const OMType* propertyType = type();

  if (propertyType != 0) { // tjb - temporary, should be ASSERTION below

    ASSERT("Valid property type", propertyType != 0);

    // Allocate buffer for property value
    OMByte* buffer = new OMByte[externalBytesSize];
    ASSERT("Valid heap pointer", buffer != 0);

    // Read property value
    store->read(_propertyId, _storedForm, buffer, externalBytesSize);

    // Reorder property value
    if (store->byteOrder() != hostByteOrder()) {
      propertyType->reorder(buffer, externalBytesSize);
    }

    // Internalize property value
    size_t requiredBytesSize = propertyType->internalSize(buffer,
                                                          externalBytesSize);
    setSize(requiredBytesSize);
    ASSERT("Property value buffer large enough", _size >= requiredBytesSize);

    propertyType->internalize(buffer,
                              externalBytesSize,
                              _bits,
                              requiredBytesSize,
                              hostByteOrder());
    delete [] buffer;
  } else {
    // tjb - temporary, no type information, do it the old way
    //
    setSize(externalBytesSize);
    ASSERT("Property value buffer large enough", _size >= externalBytesSize);
    store->read(_propertyId, _storedForm, _bits, externalBytesSize);
  }
  setPresent();
}

  // @mfunc Get the value of this <c OMSimpleProperty>.
  //   @parm The buffer to receive the property value.
  //   @parm size_t | valueSize | The size of the buffer.
  //   @this const
void OMSimpleProperty::get(void* value, size_t ANAME(valueSize)) const
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

  setSize(valueSize);
  memcpy(_bits, value, _size);
  setPresent();
}

  // @mfunc Save this <c OMSimpleProperty>.
  //   @this const
void OMSimpleProperty::save(void) const
{
  TRACE("OMSimpleProperty::save");
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  write();
}

  // @mfunc Restore this <c OMSimpleProperty>, the external (persisted)
  //        size of the <c OMSimpleProperty> is <p externalSize>.
  //   @parm The size of the <c OMSimpleProperty>.
void OMSimpleProperty::restore(size_t externalSize)
{
  TRACE("OMSimpleProperty::restore");
  ASSERT("Sizes match", externalSize == _size);

  read(externalSize);
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

  // @mfunc Is this <c OMProperty> void ?
  //   @rdesc True if this <c OMProperty> is void, false otherwise.
  //   @this const
bool OMProperty::isVoid(void) const
{
  TRACE("OMProperty::isVoid");

  // most descendants of OMProperty are always void
  return true;
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

  // @mfunc The value of this <c OMProperty> as an <c OMStorable>.
  //        If this <c OMProperty> does not represent an <c OMStorable>
  //        then the value returned is 0.
  //   @rdesc Always 0.
  //   @this const
OMStorable* OMProperty::storable(void) const
{
  TRACE("OMProperty::storable");

  return 0;
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
  //   @parm size_t | bitsSize | The size of the buffer.
  //   @this const
void OMSimpleProperty::getBits(OMByte* bits, size_t ANAME(bitsSize)) const
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

// @doc OMEXTERNAL
#include "OMProperty.h"

#include "OMStorable.h"
#include "OMType.h"
#include "OMUtilities.h"

#include <memory.h>

// class OMProperty

OMProperty::OMProperty(const OMPropertyId propertyId,
                       const int storedForm,
                       const char* name)
: _propertyId(propertyId), _storedForm(storedForm), _name(name), _type(0),
  // _isOptional(false),
  // BobT: make optional by default, to hack around problem where
  // props may be restored before they're initialized by DM.
  _isOptional(true),
  _isPresent(false)
{
  TRACE("OMProperty::OMProperty");

  PRECONDITION("Valid name", validString(_name)); 
}

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

  _type = type;
  _isOptional = isOptional;
}

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

  // @mfunc Detach the <c OMStorable> object with the given
  //        <p key> from this <c OMProperty>. This
  //        <c OMProperty> must no longer attempt
  //        to access the <c OMStorable> with the given <p key>.
  //   @parm The <c OMStoredObject> to detach from this
  //         <c OMProperty>.
  //   @parm A key identifying the <c OMStorable>.
void OMProperty::detach(const OMStorable* object, const size_t key)
{
  TRACE("OMProperty::detach");

  PRECONDITION("Valid object", object != 0);
  PRECONDITION("Valid key", key == 0);
  // nothing to do for most descendants of OMProperty
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

  if (_type != 0) { // tjb - temporary, should be PRECONDITION below

    PRECONDITION("Valid property type", _type != 0);

    // Allocate buffer for property value
    size_t externalBytesSize = _type->externalSize(internalBytes,
                                                   internalBytesSize);
    OMByte* buffer = new OMByte[externalBytesSize];
    ASSERT("Valid heap pointer", buffer != 0);

    // Externalize property value
    _type->externalize(internalBytes,
                       internalBytesSize,
                       buffer,
                       externalBytesSize,
                       store->byteOrder());
  
    // Reorder property value
    if (store->byteOrder() != hostByteOrder()) {
      _type->reorder(buffer, externalBytesSize);
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

  if (_type != 0) { // tjb - temporary, should be PRECONDITION below

    PRECONDITION("Valid property type", _type != 0);

    // Allocate buffer for property value
    OMByte* buffer = new OMByte[externalBytesSize];
    ASSERT("Valid heap pointer", buffer != 0);

    // Read property value
    store->read(propertyId, storedForm, buffer, externalBytesSize);

    // Reorder property value
    if (store->byteOrder() != hostByteOrder()) {
      _type->reorder(buffer, externalBytesSize);
    }

    // Internalize property value
    size_t requiredBytesSize = _type->internalSize(buffer, externalBytesSize);
    ASSERT("Property value buffer large enough",
                                       internalBytesSize >= requiredBytesSize);

    _type->internalize(buffer,
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

// @doc OMINTERNAL

// class OMSimpleProperty

OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const char* name)
: OMProperty(propertyId, SF_DATA, name), _size(0), _bits(0)
{
  TRACE("OMSimpleProperty::OMSimpleProperty");
}

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

OMSimpleProperty::~OMSimpleProperty(void)
{
  TRACE("OMSimpleProperty::~OMSimpleProperty");

  delete [] _bits;
}

size_t OMSimpleProperty::size(void) const
{
  TRACE("OMSimpleProperty::size");

  return _size;
}

void OMSimpleProperty::get(void* value, size_t valueSize) const
{
  TRACE("OMSimpleProperty::get");
  PRECONDITION("Valid data buffer", value != 0);
  PRECONDITION("Valid size", valueSize >= _size);
  PRECONDITION("Optional property is present",
                                           IMPLIES(isOptional(), isPresent()));

  memcpy(value, _bits, _size);
}

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
  //   @this const
void OMSimpleProperty::save(void) const
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

// class OMCollectionProperty

OMCollectionProperty::OMCollectionProperty(const OMPropertyId propertyId,
                                           const int storedForm,
                                           const char* name)
: OMProperty(propertyId, storedForm, name)
{
  TRACE("OMCollectionProperty::OMCollectionProperty");
  PRECONDITION("Valid name", validString(name));
}

OMCollectionProperty::~OMCollectionProperty(void)
{
  TRACE("OMCollectionProperty::~OMCollectionProperty");
}

// class OMStringProperty

OMStringProperty::OMStringProperty(const OMPropertyId propertyId,
                                   const char* name)
: OMCharacterStringProperty<char>(propertyId, name)
{
  TRACE("OMStringProperty::OMStringProperty");
  PRECONDITION("Valid name", validString(name));
}

OMStringProperty::~OMStringProperty(void)
{
  TRACE("OMStringProperty::~OMStringProperty");
}

OMStringProperty& OMStringProperty::operator = (const char* value)
{
  TRACE("OMStringProperty::operator =");
  PRECONDITION("Valid value", value != 0);

  assign(value);
  return *this;
}

// class OMWideStringProperty

OMWideStringProperty::OMWideStringProperty(const OMPropertyId propertyId,
                                           const char* name)
: OMCharacterStringProperty<wchar_t>(propertyId, name)
{
  TRACE("OMWideStringProperty::OMWideStringProperty");
  PRECONDITION("Valid name", validString(name));
}

OMWideStringProperty::~OMWideStringProperty(void)
{
  TRACE("OMWideStringProperty::~OMWideStringProperty");
}

OMWideStringProperty& OMWideStringProperty::operator = (const wchar_t* value)
{
  TRACE("OMWideStringProperty::operator =");
  PRECONDITION("Valid string", validWideString(value));

  assign(value);
  return *this;
}

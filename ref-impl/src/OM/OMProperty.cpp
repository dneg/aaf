// @doc OMEXTERNAL
#include "OMProperty.h"

#include "OMStorable.h"

#include <memory.h>

// class OMProperty

OMProperty::OMProperty(const OMPropertyId propertyId,
                       const int type,
                       const char* name)
: _propertyId(propertyId), _type(type), _name(name)
{
  TRACE("OMProperty::OMProperty");
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
const OMPropertyId OMProperty::propertyId(void) const
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
  // nothing to do for most descendants of OMProperty
}

int OMProperty::typeId(void) const
{
  return _type;
}

// @doc OMINTERNAL

// class OMSimpleProperty

OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const char* name)
: OMProperty(propertyId, TID_DATA, name), _size(0), _bits(0)
{
  TRACE("OMSimpleProperty::OMSimpleProperty");
}

OMSimpleProperty::OMSimpleProperty(const OMPropertyId propertyId,
                                   const char* name,
                                   size_t valueSize)
: OMProperty(propertyId, TID_DATA, name),
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
}

  // @mfunc Save this <c OMSimpleProperty>.
  //   @this const
void OMSimpleProperty::save(void) const
{
  TRACE("OMSimpleProperty::save");

  ASSERT("Valid property set", _propertySet != 0);
  OMStorable* container = _propertySet->container();
  ASSERT("Valid container", container != 0);
  ASSERT("Container is persistent", container->persistent());
  OMStoredObject* s = container->store();

  s->write(_propertyId, _type, _bits, _size);
}

void OMSimpleProperty::restoreFrom(OMStoredObject& s, size_t size)
{
  TRACE("OMSimpleProperty::restoreFrom");
  ASSERT("Sizes match", size == _size);

  s.read(_propertyId, _type, _bits, _size);
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
  PRECONDITION("Valid bits", bits != 0);
  PRECONDITION("Valid size", bitsSize >= _size);

  memcpy(bits, _bits, _size);
}

// class OMCollectionProperty

OMCollectionProperty::OMCollectionProperty(const OMPropertyId propertyId,
                                           const int type,
                                           const char* name)
: OMProperty(propertyId, type, name)
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

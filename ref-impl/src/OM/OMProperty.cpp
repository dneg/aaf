// @doc

#include "OMProperty.h"

#include "OMStorable.h"

#include <memory.h>

// class OMProperty

OMProperty::OMProperty(int pid, const char* name)
: _pid(pid), _type(TID_NONE), _name(name)
{
}

OMProperty::OMProperty(int pid, const int type, const char* name)
: _pid(pid), _type(type), _name(name)
{
}

OMProperty::~OMProperty(void)
{
}

  // @mfunc The name of this <c OMProperty>.
  //   @rdesc The property name.
  //   @this const
const char* OMProperty::name(void) const
{
  return _name;
}

  // @mfunc The property id of this <c OMProperty>.
  //   @rdesc The property id.
  //   @this const
int OMProperty::pid(void) const
{
  return _pid;
}

  // @mfunc Inform this <c OMProperty> that it is a property of
  //        the <c OMStorable> object <p containingObject>.
  //   @parm The <c OMStorable> object of which this <c OMProperty> is
  //   a property.
void OMProperty::setContainingObject(const OMStorable* containingObject)
{
  _containingObject = containingObject;
}

  // @mfunc The address of this <c OMProperty> object. This function
  //        is defined so that descendants may override "operator &".
  //   @rdesc The address of this <c OMProperty>.
OMProperty* OMProperty::address(void)
{
  return this;
}

// class OMSimpleProperty

OMSimpleProperty::OMSimpleProperty(int pid, const char* name)
: OMProperty(pid, name), _size(0), _bits(0)
{
}

OMSimpleProperty::OMSimpleProperty(int pid, const char* name, size_t valueSize)
: OMProperty(pid, name), _size(valueSize), _bits(new unsigned char[valueSize])
{
  PRECONDITION("Valid size", (valueSize > 0));

  for (size_t i = 0; i < valueSize; i++) {
    _bits[i] = 0;
  }

  POSTCONDITION("Valid bits", _bits != 0 );
}

OMSimpleProperty::~OMSimpleProperty(void)
{
  delete _bits;
}

size_t OMSimpleProperty::size(void) const
{
  return _size;
}

void OMSimpleProperty::get(void* value, size_t valueSize) const
{
  PRECONDITION("Valid size", valueSize >= _size);
  memcpy(value, _bits, _size);
}

void OMSimpleProperty::set(const void* value, size_t valueSize) 
{
  if (valueSize != _size) {
    delete _bits;
    _bits = new unsigned char[valueSize];
    _size = valueSize;
  }
  memcpy(_bits, value, _size);
}

void OMSimpleProperty::saveTo(OMStoredObject& s) const
{
  s.write(_pid, _type, _bits, _size);
}


// class OMCollectionProperty

OMCollectionProperty::OMCollectionProperty(int pid,
                                           const int type,
                                           const char* name)
: OMProperty(pid, type, name)
{
}

OMCollectionProperty::~OMCollectionProperty(void)
{
}

// class OMStringProperty

OMStringProperty::OMStringProperty(int pid, const char* name)
: OMCharacterStringProperty<char>(pid, name)
{
}

OMStringProperty::~OMStringProperty(void)
{
}

OMStringProperty& OMStringProperty::operator = (const char* value)
{
  assign(value);
  return *this;
}

// class OMWideStringProperty

OMWideStringProperty::OMWideStringProperty(int pid, const char* name)
: OMCharacterStringProperty<wchar_t>(pid, name)
{
}

OMWideStringProperty::~OMWideStringProperty(void)
{
}

OMWideStringProperty& OMWideStringProperty::operator = (const wchar_t* value)
{
  assign(value);
  return *this;
}


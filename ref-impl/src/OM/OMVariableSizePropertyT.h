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
#ifndef OMVARIABLESIZEPROPERTYT_H
#define OMVARIABLESIZEPROPERTYT_H

#include "OMAssertions.h"
#include "OMPropertySet.h"
#include "OMStorable.h"

template<typename PropertyType>
OMVariableSizeProperty<PropertyType>::OMVariableSizeProperty(
                                                 const OMPropertyId propertyId,
                                                 const char* name)
: OMSimpleProperty(propertyId, name)
{
  TRACE("OMVariableSizeProperty<PropertyType>::OMVariableSizeProperty");
}

template<typename PropertyType>
OMVariableSizeProperty<PropertyType>::~OMVariableSizeProperty(void)
{
  TRACE("OMVariableSizeProperty<PropertyType>::~OMVariableSizeProperty");
}

  // @mfunc Get the value of this <c OMVariableSizeProperty>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm A pointer to a <p PropertyType>
  //   @parm The size of the <p value>
  //   @this const
template<typename PropertyType>
void OMVariableSizeProperty<PropertyType>::getValue(PropertyType* value,
                                                    size_t valueSize) const
{
  TRACE("OMVariableSizeProperty<PropertyType>::getValue");
  PRECONDITION("Valid size", valueSize >= size());

  get(value, valueSize);
}

  // @mfunc Set the value of this <c OMVariableSizeProperty>. The
  //        value is set by copying <p valueSize> bytes from the
  //        address <p value> into the <c OMVariableSizeProperty>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm A pointer to an array of <p PropertyType>s
  //   @parm The size of the array <p value> in bytes
template<typename PropertyType>
void OMVariableSizeProperty<PropertyType>::setValue(const PropertyType* value,
                                                    size_t valueSize)
{
  TRACE("OMVariableSizeProperty<PropertyType>::setValue");

  set(value, valueSize);
}

  // @mfunc Set the value of this <c OMVariableSizeProperty>. The
  //        value is set by copying <p elementCount> elements from the
  //        address <p value> into the <c OMVariableSizeProperty>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm A pointer to an array of <p PropertyType>s
  //   @parm The number of element in the array <p value>
template<typename PropertyType>
void OMVariableSizeProperty<PropertyType>::setElementValues(
                                                     const PropertyType* value,
                                                     size_t elementCount)
{
  TRACE("OMVariableSizeProperty<PropertyType>::setElementValues");
  PRECONDITION("Valid value", value != 0);
  PRECONDITION("Valid count", elementCount > 0);

  setValue(value, elementCount * sizeof(PropertyType));

}

  // @mfunc Get the value of the item at position <p index> in this
  //        <c OMVariableSizeProperty>. The value is obtained by copying
  //        a single item of type PropertyType from this
  //        <c OMVariableSizeProperty> at position <p index>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm The location that is to receive the value copied out of
  //         this <c OMVariableSizeProperty>.
  //   @parm The index of the value to get.
  //   @this const
template<typename PropertyType>
void OMVariableSizeProperty<PropertyType>::getValueAt(PropertyType* value,
                                                      const size_t index) const
{
  TRACE("OMVariableSizeProperty<PropertyType>::getValueAt");
  PRECONDITION("Valid index", (index >= 0) && (index < count()));
  PRECONDITION("Valid value", value != 0);

  *value = ((PropertyType*)_bits)[index];
}

  // @mfunc Set the value of the item at position <p index> in this
  //        <c OMVariableSizeProperty>. The value is set by copying
  //        a single item of type PropertyType into this
  //        <c OMVariableSizeProperty> at position <p index>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm A pointer to the new value that is to be copied into
  //         this <c OMVariableSizeProperty>.
  //   @parm The index of the value to set.
template<typename PropertyType>
void OMVariableSizeProperty<PropertyType>::setValueAt(
                                                     const PropertyType* value,
                                                     const size_t index)
{
  TRACE("OMVariableSizeProperty<PropertyType>::setValueAt");
  PRECONDITION("Valid index", (index >= 0) && (index < count()));
  PRECONDITION("Valid value", value != 0);

  ((PropertyType*)_bits)[index] = *value;
}

  // @mfunc Set the value of the item at the last position in this
  //        <c OMVariableSizeProperty>. The <c OMVariableSizeProperty>
  //        is first increased in size by one item. The value is then
  //        set by copying a single item of type PropertyType into this
  //        <c OMVariableSizeProperty> at the last position.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm A pointer to the new value that is to be copied into
  //         this <c OMVariableSizeProperty>.
template<typename PropertyType>
void OMVariableSizeProperty<PropertyType>::appendValue(
                                                     const PropertyType* value)
{
  TRACE("OMVariableSizeProperty<PropertyType>::prependValue");
  PRECONDITION("Valid value", value != 0);

  size_t oldCount = count();
  size_t newCount = oldCount + 1;
  PropertyType* buffer = new PropertyType[newCount];
  ASSERT("Valid heap pointer", buffer != 0);
  if (oldCount > 0) {
    copyElementsToBuffer(buffer, oldCount);
  }
  buffer[newCount - 1] = *value;
  setElementValues(buffer, newCount);
}

  // @mfunc Set the value of the item at the first position in this
  //        <c OMVariableSizeProperty>. The <c OMVariableSizeProperty>
  //        is first increased in size by one item and all existing items
  //        are moved up by on position. The value is then
  //        set by copying a single item of type PropertyType into this
  //        <c OMVariableSizeProperty> at the first position.
  //   @parm A pointer to the new value that is to be copied into
  //         this <c OMVariableSizeProperty>.
template<typename PropertyType>
void OMVariableSizeProperty<PropertyType>::prependValue(
                                                     const PropertyType* value)
{
  TRACE("OMVariableSizeProperty<PropertyType>::prependValue");
  PRECONDITION("Valid value", value != 0);

  size_t oldCount = count();
  size_t newCount = oldCount + 1;
  PropertyType* buffer = new PropertyType[newCount];
  ASSERT("Valid heap pointer", buffer != 0);
  if (oldCount > 0) {
    copyElementsToBuffer(&buffer[1], oldCount);
  }
  buffer[0] = *value;
  setElementValues(buffer, newCount);
}

  // @mfunc Get the value of this <c OMVariableSizeProperty>.  The
  //        value is obtained by copying the value from the
  //        <c OMVariableSizeProperty>. The buffer is at address
  //        <p buffer> and is <p bufferSize> bytes in size.
  //        Copying only takes place if the buffer is large enough.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm A pointer to the buffer.
  //   @parm The size of the buffer.
  //   @rdesc <e bool.true> if the value was successfully copied <e
  //          bool.false> otherwise.
  //   @this const
template<typename PropertyType>
bool OMVariableSizeProperty<PropertyType>::copyToBuffer(
                                                       PropertyType* buffer,
                                                       size_t bufferSize) const
{
  TRACE("OMVariableSizeProperty<PropertyType>::copyToBuffer");
  PRECONDITION("Valid buffer", buffer != 0);
  PRECONDITION("Valid buffer size", bufferSize > 0);

  bool result;
  if (bufferSize >= size()) {
    getValue(buffer, bufferSize);
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Get the value of this <c OMVariableSizeProperty>.  The
  //        value is obtained by copying the value from the
  //        <c OMVariableSizeProperty>. The buffer is at address
  //        <p buffer> and is <p elementCount> elements in size.
  //        Copying only takes place if the buffer is large enough.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm A pointer to the buffer.
  //   @parm The number of elements in the buffer.
  //   @rdesc <e bool.true> if the value was successfully copied <e
  //          bool.false> otherwise.
  //   @this const
template<typename PropertyType>
bool OMVariableSizeProperty<PropertyType>::copyElementsToBuffer(
                                                     PropertyType* buffer,
                                                     size_t elementCount) const
{
  TRACE("OMVariableSizeProperty<PropertyType>::copyElementsToBuffer");
  PRECONDITION("Valid buffer", buffer != 0);
  PRECONDITION("Valid count", elementCount > 0);

  return copyToBuffer(buffer, elementCount * sizeof(PropertyType));
}

  // @mfunc Restore this <c OMVariableSizeProperty>, the external
  //        (persisted) size of the <c OMVariableSizeProperty> is
  //        <p externalSize>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @parm The external (persisted) size of the <c OMVariableSizeProperty>.
template<typename PropertyType>
void OMVariableSizeProperty<PropertyType>::restore(size_t externalSize)
{
  TRACE("OMVariableSizeProperty<PropertyType>::restore");

  if (externalSize != _size) {
    delete [] _bits;
    _bits = 0;  // for BoundsChecker
    _bits = new unsigned char[externalSize];
    ASSERT("Valid heap pointer", _bits != 0);
    _size = externalSize;
  }
  read(_propertyId, _storedForm, _bits, _size, externalSize);
}

  // @mfunc The number of items in this this <c OMVariableSizeProperty>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @rdesc The number of items in this <c OMVariableSizeProperty>.
  //   @this const 
template<typename PropertyType>
size_t OMVariableSizeProperty<PropertyType>::count(void) const
{
  TRACE("OMVariableSizeProperty<PropertyType>::count");
  return size() / sizeof(PropertyType);
}

#endif

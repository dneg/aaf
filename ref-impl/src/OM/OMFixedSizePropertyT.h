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
#ifndef OMFIXEDSIZEPROPERTYT_H
#define OMFIXEDSIZEPROPERTYT_H

#include "OMAssertions.h"

template<typename PropertyType>
OMFixedSizeProperty<PropertyType>::OMFixedSizeProperty(
                                                 const OMPropertyId propertyId,
                                                 const wchar_t* name)
: OMSimpleProperty(propertyId, name, sizeof(PropertyType))
{
  TRACE("OMFixedSizeProperty<PropertyType>::OMFixedSizeProperty");
}

template<typename PropertyType>
OMFixedSizeProperty<PropertyType>::~OMFixedSizeProperty(void)
{
  TRACE("OMFixedSizeProperty<PropertyType>::~OMFixedSizeProperty");
}

  // @mfunc Get the value of this <c OMFixedSizeProperty>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @parm A value of <p PropertyType> by reference.
  //   @this const
template<typename PropertyType>
void OMFixedSizeProperty<PropertyType>::getValue(PropertyType& value) const
{
  TRACE("OMFixedSizeProperty<PropertyType>::getValue");
  ASSERT("Sizes match", size() == sizeof(PropertyType));

  get(&value, size());
}

  // @mfunc Set the value of this <c OMFixedSizeProperty>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @parm A value of <p PropertyType> by reference.
template<typename PropertyType>
void OMFixedSizeProperty<PropertyType>::setValue(const PropertyType& value)
{
  TRACE("OMFixedSizeProperty<PropertyType>::setValue");
  ASSERT("Sizes match", size() == sizeof(PropertyType));

  set(&value, size());
}

  // @mfunc Assignment operator.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @parm A value of <p PropertyType> by reference.
  //   @rdesc A value of <c OMFixedSizeProperty> by reference.
template<typename PropertyType>
OMFixedSizeProperty<PropertyType>&
OMFixedSizeProperty<PropertyType>::operator = (const PropertyType& value)
{
  TRACE("OMFixedSizeProperty<PropertyType>::operator =");

  setValue(value);
  return *this;
}

  // @mfunc Type conversion. Convert an <c OMFixedSizeProperty>
  //        into a <p PropertyType>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @rdesc The result of the conversion as a value of type
  //          <p PropertyType>.
  //   @this const
template<typename PropertyType>
OMFixedSizeProperty<PropertyType>::operator PropertyType() const
{
  TRACE("OMFixedSizeProperty<PropertyType>::operator PropertyType");

  PropertyType result;
  getValue(result);
  return result;
}

  // @mfunc "Address of" operator.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @rdesc Pointer to a <p PropertyType>
template<typename PropertyType>
PropertyType* OMFixedSizeProperty<PropertyType>::operator &(void)
{
  return reinterpret_cast<PropertyType*>(_bits);
}

  // @mfunc Convert this <c OMFixedSizeProperty> into a const
  //        reference to a <p PropertyType>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @rdesc Const reference to a <p PropertyType>.
  //   @this const
template<typename PropertyType>
const PropertyType& OMFixedSizeProperty<PropertyType>::reference(void) const
{
  return *reinterpret_cast<const PropertyType*>(_bits);
}

  // @mfunc Restore this <c OMFixedSizeProperty>, the external (persisted)
  //        size of the <c OMFixedSizeProperty> is <p externalSize>.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @parm The external (persisted) size of the <c OMFixedSizeProperty>.
template<typename PropertyType>
void OMFixedSizeProperty<PropertyType>::restore(size_t externalSize)
{
  TRACE("OMFixedSizeProperty<PropertyType>::restore");

  read(externalSize);
}

#endif

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
#ifndef OMVARIABLESIZEPROPERTY_H
#define OMVARIABLESIZEPROPERTY_H

#include "OMPropertyBase.h"

  // @class Variable size simple (data) persistent
  //        properties supported by the Object Manager.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type.
  //   @base public | <c OMSimpleProperty>
template <typename PropertyType>
class OMVariableSizeProperty : public OMSimpleProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMVariableSizeProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMVariableSizeProperty(void);

    // @cmember Get the value of this <c OMVariableSizeProperty>.
  void getValue(PropertyType* value, size_t valueSize) const;

    // @cmember Set the value of this <c OMVariableSizeProperty>. The
    //          value is set by copying <p valueSize> bytes from the
    //          address <p value> into the <c OMVariableSizeProperty>.
  void setValue(const PropertyType* value, size_t valueSize);

    // @cmember Set the value of this <c OMVariableSizeProperty>. The
    //          value is set by copying <p elementCount> elements from the
    //          address <p value> into the <c OMVariableSizeProperty>.
  void setElementValues(const PropertyType* value, size_t elementCount);

    // @cmember Get the value of the item at position <p index> in this
    //          <c OMVariableSizeProperty>. The value is obtained by copying
    //          a single item of type PropertyType from this
    //          <c OMVariableSizeProperty> at position <p index>.
  void getValueAt(PropertyType* value, const size_t index) const;

    // @cmember Set the value of the item at position <p index> in this
    //          <c OMVariableSizeProperty>. The value is set by copying
    //          a single item of type PropertyType into this
    //          <c OMVariableSizeProperty> at position <p index>.
  void setValueAt(const PropertyType* value, const size_t index);

    // @cmember Set the value of the item at the last position in this
    //          <c OMVariableSizeProperty>. The <c OMVariableSizeProperty>
    //          is first increased in size by one item. The value is then
    //          set by copying a single item of type PropertyType into this
    //          <c OMVariableSizeProperty> at the last position.
  void appendValue(const PropertyType* value);

    // @cmember Set the value of the item at the first position in this
    //          <c OMVariableSizeProperty>. The <c OMVariableSizeProperty>
    //          is first increased in size by one item and all existing items
    //          are moved up by on position. The value is then
    //          set by copying a single item of type PropertyType into this
    //          <c OMVariableSizeProperty> at the first position.
  void prependValue(const PropertyType* value);

    // @cmember Get the value of this <c OMVariableSizeProperty>.  The
    //          value is obtained by copying the value from the
    //          <c OMVariableSizeProperty>. The buffer is at address
    //          <p buffer> and is <p bufferSize> bytes in size.
    //          Copying only takes place if the buffer is large enough.
  bool copyToBuffer(PropertyType* buffer, size_t bufferSize) const;

    // @cmember Get the value of this <c OMVariableSizeProperty>.  The
    //          value is obtained by copying the value from the
    //          <c OMVariableSizeProperty>. The buffer is at address
    //          <p buffer> and is <p elementCount> elements in size.
    //          Copying only takes place if the buffer is large enough.
  bool copyElementsToBuffer(PropertyType* buffer, size_t elementCount) const;

    // @cmember Restore this <c OMVariableSizeProperty>, the external
    //          (persisted) size of the <c OMVariableSizeProperty> is
    //          <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember The number of items in this this <c OMVariableSizeProperty>.
  size_t count(void) const;

};

#include "OMVariableSizePropertyT.h"

#endif

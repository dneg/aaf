//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMFIXEDSIZEPROPERTY_H
#define OMFIXEDSIZEPROPERTY_H

#include "OMProperty.h"

  // @class Fixed size simple (data) persistent
  //        properties supported by the Object Manager.
  //   @tcarg class | PropertyType | The type of the property. This
  //          can be any type with well defined copy and assignment
  //          semantics.
  //   @base public | <c OMSimpleProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename PropertyType>
class OMFixedSizeProperty : public OMSimpleProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMFixedSizeProperty(const OMPropertyId propertyId, const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMFixedSizeProperty(void);

    // @cmember Get the value of this <c OMFixedSizeProperty>.
  void getValue(PropertyType& value) const;

    // @cmember Set the value of this <c OMFixedSizeProperty>.
  void setValue(const PropertyType& value);

    // @cmember Assignment operator.
  OMFixedSizeProperty<PropertyType>& operator = (const PropertyType& value);

    // @cmember Type conversion. Convert an <c OMFixedSizeProperty>
    //          into a <p PropertyType>.
  operator PropertyType() const;

    // @cmember "Address of" operator.
  PropertyType* operator &(void);

    // @cmember Convert this <c OMFixedSizeProperty> into a const
    //          reference to a <p PropertyType>.
  const PropertyType& reference(void) const;

    // @cmember Restore this <c OMFixedSizeProperty>, the external
    //          (persisted) size of the <c OMFixedSizeProperty> is
    //          <p externalSize>.
  virtual void restore(size_t externalSize);

};

#include "OMFixedSizePropertyT.h"

#endif

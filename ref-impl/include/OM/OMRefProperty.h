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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMREFERENCEPROPERTY_H
#define OMREFERENCEPROPERTY_H

#include "OMProperty.h"

class OMObject;

  // @class Abstract base class for persistent reference properties
  //        supported by the Object Manager.
  //   @base public | <c OMProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMReferenceProperty : public OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMReferenceProperty(const OMPropertyId propertyId,
                      const OMStoredForm storedForm,
                      const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMReferenceProperty(void);

  // Direct property access interface

    // @cmember The size of the raw bits of this
    //          <c OMReferenceProperty>. The size is given in bytes.
  virtual size_t bitsSize(void) const;

    // @cmember Get the value of this <c OMReferenceProperty>.
  virtual OMObject* getObject(void) const = 0;

    // @cmember set the value of this <c OMReferenceProperty>.
  virtual OMObject* setObject(const OMObject* object) = 0;

    // @cmember Clear the value of this <c OMReferenceProperty>.
  virtual OMObject* clearObject(void) = 0;

};

#endif

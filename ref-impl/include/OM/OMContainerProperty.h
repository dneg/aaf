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
#ifndef OMCONTAINERPROPERTY_H
#define OMCONTAINERPROPERTY_H

#include "OMProperty.h"

class OMObject;
class OMReferenceContainer;
class OMReferenceContainerIterator;

  // @class Abstract base class for persistent object reference container
  //        properties supported by the Object Manager.
  //   @base public | <c OMProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMContainerProperty : public OMProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMContainerProperty(const OMPropertyId propertyId,
                      const OMStoredForm storedForm,
                      const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMContainerProperty(void);

    // @cmember Convert to <c OMReferenceContainer>.
  virtual OMReferenceContainer* referenceContainer(void) = 0;

    // @cmember The current local key.
  OMUInt32 localKey(void) const;

    // @cmember Set the current local key. Used on restore to restart
    //          local key assignment.
  void setLocalKey(OMUInt32 newLocalKey);

protected:
  // @access Protected members.

    // @cmember Compute the name of an element in this <c OMContainter>
    //          given the element's <p localKey>.
  wchar_t* elementName(OMUInt32 localKey);

    // @cmember Obtain the next available local key.
  OMUInt32 nextLocalKey(void);

  virtual const wchar_t* storedName(void) const;

private:
  // @access Private members.

    // @cmember The next available local key.
    //
  OMUInt32 _localKey;

};

#endif

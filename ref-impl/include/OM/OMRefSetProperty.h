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
#ifndef OMREFSETPROPERTY_H
#define OMREFSETPROPERTY_H

#include "OMContainerProperty.h"
#include "OMObjectSet.h"

  // @class Abstract base class for persistent object reference set
  //        properties supported by the Object Manager.
  //   @base public | <c OMContainerProperty>
  //   @base public | <c OMObjectSet>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMReferenceSetProperty : public OMContainerProperty,
                               public OMObjectSet {
public:
  // @access Public members.

    // @cmember Constructor.
  OMReferenceSetProperty(const OMPropertyId propertyId,
                         const OMStoredForm storedForm,
                         const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMReferenceSetProperty(void);

    // @cmember Convert to <c OMReferenceContainer>.
  virtual OMReferenceContainer* referenceContainer(void);

};

#endif


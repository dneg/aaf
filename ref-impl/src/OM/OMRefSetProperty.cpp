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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMReferenceSetProperty

#include "OMRefSetProperty.h"

#include "OMAssertions.h"

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The stored form of this property.
  //   @parm The name of this property.
OMReferenceSetProperty::OMReferenceSetProperty(const OMPropertyId propertyId,
                                               const OMStoredForm storedForm,
                                               const wchar_t* name)
: OMContainerProperty(propertyId, storedForm, name)
{
  TRACE("OMReferenceSetProperty::OMReferenceSetProperty");
}

  // @mfunc Destructor.
OMReferenceSetProperty::~OMReferenceSetProperty(void)
{
  TRACE("OMReferenceSetProperty::~OMReferenceSetProperty");
}

  // @mfunc Convert to <c OMReferenceContainer>.
  //   @rdesc The <c OMReferenceContainer> interface implemented by
  //          this <c OMReferenceSetProperty>
OMReferenceContainer* OMReferenceSetProperty::referenceContainer(void)
{
  TRACE("OMReferenceSetProperty::referenceContainer");
  return this;
}

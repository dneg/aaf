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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMReferenceProperty

#include "OMRefProperty.h"

#include "OMAssertions.h"

OMReferenceProperty::OMReferenceProperty(const OMPropertyId propertyId,
                                         const OMStoredForm storedForm,
                                         const wchar_t* name)
: OMProperty(propertyId, storedForm, name)
{
  TRACE("OMReferenceProperty::OMReferenceProperty");
}

OMReferenceProperty::~OMReferenceProperty(void)
{
  TRACE("OMReferenceProperty::~OMReferenceProperty");
}

  // @mfunc The size of the raw bits of this <c OMReferenceProperty>. The
  //        size is given in bytes.
  //   @rdesc The size of the raw bits of this <c OMReferenceProperty>
  //          in bytes.
  //   @this const
size_t OMReferenceProperty::bitsSize(void) const
{
  TRACE("OMReferenceProperty::bitsSize");
  OBSOLETE("other methods on class OMReferenceProperty");

  return sizeof(OMStorable*);
}

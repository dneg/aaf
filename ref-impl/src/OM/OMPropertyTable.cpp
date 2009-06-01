//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMPropertyTable

#include "OMPropertyTable.h"

#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMStoredObject.h"

  // @mfunc Constructor.
OMPropertyTable::OMPropertyTable(void)
{
  TRACE("OMPropertyTable::OMPropertyTable");
}

  // @mfunc Destructor.
OMPropertyTable::~OMPropertyTable(void)
{
  TRACE("OMPropertyTable::~OMPropertyTable");

  OMUInt16 elements = count();
  for (OMUInt16 i = 0; i < elements; i++) {
    OMPropertyId* p = _vector.valueAt(i);
    delete [] p;
  }
}

  // @mfunc If <p propertyPath> is not already present then insert
  //        it (by copying) into the table and return its tag,
  //        otherwise just return its tag. Tags are allocated
  //        sequentially.
  //   @parm The property path to insert.
  //   @rdesc The assigned index.
OMPropertyTag OMPropertyTable::insert(const OMPropertyId* propertyPath)
{
  TRACE("OMPropertyTable::insert");

  PRECONDITION("Valid property path", validPropertyPath(propertyPath));
  PRECONDITION("Room in table", count() < OMUINT16_MAX);

  OMPropertyTag result = nullOMPropertyTag;
  bool found = false;
  OMUInt16 elements = count();
  for (OMUInt16 i = 0; i < elements; i++) {
    if (comparePropertyPath(_vector.valueAt(i), propertyPath) == 0) {
     result = i;
     found = true;
     break;
    }
  }

  if (!found) {
    _vector.append(savePropertyPath(propertyPath));
    result = elements;
  }

  POSTCONDITION("Valid result", isValid(result));
  return result;
}

  // @mfunc The property path corresponding to <p tag> in the table.
  //   @parm The index.
  //   @rdesc The property path.
  //   @this const
const OMPropertyId* OMPropertyTable::valueAt(OMPropertyTag tag) const
{
  TRACE("OMPropertyTable::valueAt");

  PRECONDITION("Valid tag", isValid(tag));

  return _vector.valueAt(tag);
}

  // @mfunc The count of entries in the table.
  //   @rdesc The count of entries.
  //   @this const
OMUInt16 OMPropertyTable::count(void) const
{
  TRACE("OMPropertyTable::count");

  OMUInt16 result = static_cast<OMUInt16>(_vector.count());
  return result;
}

  // @mfunc Is <p tag> valid ?
  //   @parm The tag to check.
  //   @rdesc True if the tag is valid, false otherwise.
  //   @this const
bool OMPropertyTable::isValid(OMPropertyTag tag) const
{
  TRACE("OMPropertyTable::isValid");

  bool result;

  if ((tag < count()) && (tag != nullOMPropertyTag)) {
    result = true;
  } else {
    result = false;
  }

  return result;
}

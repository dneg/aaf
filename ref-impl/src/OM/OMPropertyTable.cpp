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

  size_t elements = _vector.count();
  for (size_t i = 0; i < elements; i++) {
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

  OMPropertyTag result;
  bool found = false;
  size_t elements = _vector.count();
  for (size_t i = 0; i < elements; i++) {
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
size_t OMPropertyTable::count(void) const
{
  TRACE("OMPropertyTable::count");

  return _vector.count();
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

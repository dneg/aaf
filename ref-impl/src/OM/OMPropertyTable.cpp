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

// @doc OMINTERNAL
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
    char* p = _vector.valueAt(i);
    delete [] p;
  }
}

  // @mfunc If <p propertyName> is not already present then insert
  //        it (by copying) into the table and return its index,
  //        otherwise just return its index.
  //   @parm The property name to insert.
  //   @rdesc The assigned index.
OMUInt32 OMPropertyTable::insert(const char* propertyName)
{
  TRACE("OMPropertyTable::insert");

  PRECONDITION("Valid property name", validString(propertyName));

  OMUInt32 result;
  bool found = false;
  size_t elements = _vector.count();
  for (size_t i = 0; i < elements; i++) {
    if (strcmp(_vector.valueAt(i), propertyName) == 0) {
     result = i;
     found = true;
     break;
    }
  }

  if (!found) {
    _vector.append(saveString(propertyName));
    result = elements;
  }

  return result;
}

  // @mfunc The property name at position <p index> in the table.
  //   @parm The index.
  //   @rdesc The property name.
  //   @this const
const char* OMPropertyTable::valueAt(OMUInt32 index) const
{
  TRACE("OMPropertyTable::valueAt");

  PRECONDITION("Valid index", index < _vector.count());
  return _vector.valueAt(index);
}

  // @mfunc The count of entries in the table.
  //   @rdesc The count of entries.
  //   @this const
size_t OMPropertyTable::count(void) const
{
  TRACE("OMPropertyTable::valueAt");

  return _vector.count();
}

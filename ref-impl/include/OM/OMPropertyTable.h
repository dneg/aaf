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

// @doc OMINTERNAL
#ifndef OMPROPERTYTABLE_H
#define OMPROPERTYTABLE_H

#include "OMDataTypes.h"

#include "OMVector.h"

  // @class Persistent tables of property instance names.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMPropertyTable {
public:
  // @access Public members.

    // @cmember Constructor.
  OMPropertyTable(void);

    // @cmember Destructor.
  ~OMPropertyTable(void);

    // @cmember If <p propertyPath> is not already present then insert
    //          it (by copying) into the table and return its tag,
    //          otherwise just return its tag. Tags are allocated
    //          sequentially.
  OMPropertyTag insert(const OMPropertyId* propertyPath);

    // @cmember The property path corresponding to <p tag> in the table.
  const OMPropertyId* valueAt(OMPropertyTag tag) const;

    // @cmember The count of entries in the table.
  size_t count(void) const;

    // @cmember Is <p tag> valid ?
  bool isValid(OMPropertyTag tag) const;

private:

  OMVector<OMPropertyId*> _vector;

};

#endif

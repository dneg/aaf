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
#ifndef OMDEFINITION_H
#define OMDEFINITION_H

#include "OMDataTypes.h"
#include <stddef.h>

class OMType;

  // @class Abstract base class used to define persistent entities
  //        supported by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMDefinition {
public:
  // @access Public members.

    // @cmember Destructor.
  virtual ~OMDefinition(void) {}

    // @cmember The unique identification of the entity defined by
    //          this <c OMDefinition>.
  virtual const OMUniqueObjectIdentification& identification(void) const = 0;

    // @cmember The name of the entity defined by
    //          this <c OMDefinition>.
  virtual const wchar_t* name(void) const = 0;

};

#endif

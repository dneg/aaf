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
#ifndef OMOBJECTSET_H
#define OMOBJECTSET_H

#include "OMReferenceContainer.h"

  // @class Abstract base class for persistent object reference set
  //        properties supported by the Object Manager.
  //   @base public | <c OMReferenceContainer>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMObjectSet : public OMReferenceContainer {
public:
  // @access Public members.

    // @cmember Remove the <c OMObject> identified by <p identification>
    //          from this <c OMObjectSet>.
  virtual OMObject* remove(void* identification) = 0;

    // @cmember Does this <c OMObjectSet> contain an
    //          <c OMObject> identified by <p identification> ?
  virtual bool contains(void* identification) const = 0;

    // @cmember Find the <c OMObject> in this <c OMObjectSet>
    //          identified by <p identification>.  If the object is found
    //          it is returned in <p object> and the result is < e bool.true>.
    //          If the object is not found the result is <e bool.false>.
  virtual bool findObject(void* identification, OMObject*& object) const = 0;

};

#endif


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
#ifndef OMOBJECTVECTOR_H
#define OMOBJECTVECTOR_H

#include "OMReferenceContainer.h"

  // @class Abstract base class for elastic sequential collections of objects
  //        supported by the Object Manager.
  //        Objects are accessible by index. The order of objects is
  //        determined externally. Duplicate objects are allowed.
  //   @base public | <c OMReferenceContainer>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMObjectVector : public OMReferenceContainer {
public:
  // @access Public members.

    // @cmember Set the value of this <c OMObjectVector>
    //          at position <p index> to <p object>.
  virtual OMObject* setObjectAt(const OMObject* object,
                                const size_t index) = 0;

    // @cmember The value of this <c OMObjectVector>
    //          at position <p index>.
  virtual OMObject* getObjectAt(const size_t index) const = 0;

    // @cmember Append the given <p OMObject> <p object> to
    //          this <c OMObjectVector>.
  virtual void appendObject(const OMObject* object) = 0;

    // @cmember Prepend the given <p OMObject> <p object> to
    //          this <c OMObjectVector>.
  virtual void prependObject(const OMObject* object) = 0;

    // @cmember Remove the object from this
    //          <c OMObjectVector> at position <p index>.
    //          Existing objects in this <c OMObjectVector>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  virtual OMObject* removeObjectAt(const size_t index) = 0;

    // @cmember Insert <p object> into this <c OMObjectVector>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  virtual void insertObjectAt(const OMObject* object, const size_t index) = 0;

};

#endif

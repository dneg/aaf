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
#ifndef OMSTRONGREFERENCEVECTOR_H
#define OMSTRONGREFERENCEVECTOR_H

#include "OMRefVectorProperty.h"

template <typename Element>
class OMContainerIterator;
class OMStrongReferenceVectorElement;

  // @class Persistent elastic sequential collections of strongly
  //        referenced (contained) objects supported by the Object Manager.
  //        Objects are accessible by index. The order of objects is
  //        determined externally. Duplicate objects are allowed.
  //   @base public | <c OMReferenceVectorProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStrongReferenceVector : public OMReferenceVectorProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceVector(const OMPropertyId propertyId,
                          const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceVector(void);

  virtual OMContainerIterator<OMStrongReferenceVectorElement>*
                                                      iterator(void) const = 0;

  virtual void insert(const size_t index,
                      const OMStrongReferenceVectorElement& element) = 0;

    // @cmember Increase the capacity of this
    //          <c OMStrongReferenceVector> so that it
    //          can contain at least <p capacity> <p OMObject>s
    //          without having to be resized.
  virtual void grow(const size_t capacity) = 0;

};

#endif

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
#ifndef OMCONTAINER_H
#define OMCONTAINER_H

#include <stddef.h>

  // @class Abstract base class for collections of elements.
  //   @tcarg class | Element | The type of an <c OMContainer> element.
  //          This type must support = and ==.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename Element>
class OMContainer {
public:
  // @access Public members.

    // @cmember The number of elements in this <c OMContainer>.
    //          <mf OMContainer::count> returns the actual number
    //          of elements in the <c OMContainer>.
  virtual size_t count(void) const = 0;

    // @cmember Remove all elements from this <c OMContainer>.
  virtual void clear(void) = 0;

};

#endif

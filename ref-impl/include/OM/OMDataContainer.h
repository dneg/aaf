//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef OMDATACONTAINER_H
#define OMDATACONTAINER_H

class OMDataContainerIterator;

  // @class Abstract base class for data (i.e. non-object) containers.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMDataContainer {
public:

  virtual size_t count(void) const = 0;

  virtual void clear(void) = 0;

  virtual OMDataContainerIterator* createIterator(void) const = 0;

};

#endif

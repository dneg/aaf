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
#ifndef OMWEAKREFERENCE_H
#define OMWEAKREFERENCE_H

#include "OMRefProperty.h"

class OMWeakObjectReference;

  // @class Persistent weak reference (pointer to shared object)
  //        properties supported by the Object Manager.
  //   @base public | <c OMReferenceProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMWeakReference : public OMReferenceProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReference(const OMPropertyId propertyId,
                  const wchar_t* name);

    // @cmember Destructor.
  ~OMWeakReference(void);

  virtual OMWeakObjectReference& reference(void) const = 0;

  virtual OMPropertyId keyPropertyId(void) const = 0;

  virtual OMPropertyTag targetTag(void) const = 0;

  virtual void setTargetTag(OMPropertyTag targetTag) = 0;

  virtual void clearTargetTag(void) const = 0;

  virtual const wchar_t* targetName(void) const = 0;

};

#endif

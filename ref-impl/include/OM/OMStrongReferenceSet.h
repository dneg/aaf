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
#ifndef OMSTRONGREFERENCESET_H
#define OMSTRONGREFERENCESET_H

#include "OMRefSetProperty.h"

template <typename Element>
class OMContainerIterator;
class OMStrongReferenceSetElement;

  // @class Persistent sets of uniquely identified strongly referenced
  //        (contained) objects supported by the Object Manager.
  //        Objects are accessible by unique identifier (the key).
  //        The objects are not ordered. Duplicates objects are not allowed.
  //   @base public | <c OMSReferenceSetProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStrongReferenceSet : public OMReferenceSetProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceSet(const OMPropertyId propertyId,
                       const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceSet(void);

  virtual OMContainerIterator<OMStrongReferenceSetElement>*
                                                     iterator(void) const  = 0;

  virtual void insert(void* key,
                      const OMStrongReferenceSetElement& element) = 0;

  virtual void find(void* key, OMStrongReferenceSetElement& element) const = 0;

  virtual bool find(void* key,
                    OMStrongReferenceSetElement** element) const = 0;

  virtual void find(void* key, OMStorable*& object) const = 0;

  virtual OMKeySize keySize(void) const = 0;

  virtual OMPropertyId keyPropertyId(void) const = 0;

};

#endif

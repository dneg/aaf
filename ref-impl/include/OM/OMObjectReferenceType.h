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

// @doc OMEXTERNAL
#ifndef OMOBJECTREFERENCETYPE_H
#define OMOBJECTREFERENCETYPE_H

#include "OMType.h"
#include "OMSingleton.h"
#include "OMClassDefinition.h"
#include "OMVector.h"

// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMObjectReferenceType
class OMObjectReferenceType : public OMType {
public:

  virtual const OMUniqueObjectIdentification& referencedType(void) const = 0;

  virtual OMClassDefinition* referencedClass(void) const = 0;
};

// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMStrongObjectReferenceType
class OMStrongObjectReferenceType : public OMObjectReferenceType {
public:

  virtual OMType::Tag tag(void) const;

};

// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMWeakObjectReferenceType
class OMWeakObjectReferenceType : public OMObjectReferenceType {
public:

  virtual OMType::Tag tag(void) const;

  virtual OMUInt32 targetPathElementCount(void) const = 0;

  virtual const OMUniqueObjectIdentification& targetPathElement(OMUInt32 index) const = 0;

  virtual const OMPropertyId* targetPath(void) const = 0;
};


#endif

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

#ifndef OMDICTIONARY_H
#define OMDICTIONARY_H

#include "OMClassFactory.h"
#include "OMStorable.h"
#include "OMRedBlackTree.h"

class OMPropertyDefinition;

  // @class Dictionary containing definitions of classes, properties and types.
  //        The dictionary is a factory for <c OMstorable> objects.
  //        The dictionary is persistent.
  //   @base public | <c OMClassFactory>
  //   @base public | <c OMStorable>
  //   @cauthor Tim Bingham | Avid Technology, Inc.
class OMDictionary : public OMClassFactory, public OMStorable {
public:

  static OMPropertyDefinition* find(const OMPropertyId propertyId);

  static void insert(const OMPropertyId propertyId,
                     const OMPropertyDefinition* definition);

  static OMPropertyDefinition* remove(const OMPropertyId propertyId);

  static bool contains(const OMPropertyId propertyId);

  static void initialize(void);

  static void finalize(void);

private:

  typedef OMRedBlackTree<OMPropertyId,
                         OMPropertyDefinition*> PropertyDefinitionSet;

  static PropertyDefinitionSet* _propertyDefinitions;

};

#endif


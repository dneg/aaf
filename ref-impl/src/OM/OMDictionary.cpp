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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMDictionary

#include "OMDictionary.h"
#include "OMPropertyDefinition.h"

OMDictionary::PropertyDefinitionSet OMDictionary::_propertyDefinitions;

OMPropertyDefinition* OMDictionary::find(const OMPropertyId propertyId)
{
  TRACE("OMDictionary::find");

  PRECONDITION("Valid property id", propertyId != 0);

  OMPropertyDefinition* result = 0;
  bool status = _propertyDefinitions.find(propertyId, result);

  POSTCONDITION("Property definition found", status);
  POSTCONDITION("Valid result", result != 0);
  return result;
}

void OMDictionary::insert(const OMPropertyId propertyId,
                          const OMPropertyDefinition* definition)
                          
{
  TRACE("OMDictionary::insert");

  PRECONDITION("Valid property id", propertyId != 0);
  PRECONDITION("Valid property definition", definition != 0);
  PRECONDITION("Definition not already present",
                                   !_propertyDefinitions.contains(propertyId));

  bool status = _propertyDefinitions.insert(
                                propertyId,
                                const_cast<OMPropertyDefinition*>(definition));

  POSTCONDITION("Definition not previously present", status);
  POSTCONDITION("Definition present",
                                    _propertyDefinitions.contains(propertyId));
}

OMPropertyDefinition* OMDictionary::remove(const OMPropertyId propertyId)
{
  TRACE("OMDictionary::remove");

  PRECONDITION("Valid property id", propertyId != 0);
  PRECONDITION("Definition present",
                                    _propertyDefinitions.contains(propertyId));

  OMPropertyDefinition* result = 0;
  bool status = _propertyDefinitions.find(propertyId, result);
  ASSERT("Property definition found", status);

  status = _propertyDefinitions.remove(propertyId);
  ASSERT("Property definition removed", status);

  POSTCONDITION("Definition no longer present",
                                   !_propertyDefinitions.contains(propertyId));
  return result;
}

bool OMDictionary::contains(const OMPropertyId propertyId)
{
  TRACE("OMDictionary::contains");

  OMPropertyDefinition* d = 0;
  bool result = _propertyDefinitions.find(propertyId, d);

  return result;
}

struct {
  OMPropertyId _pid;
  wchar_t* _name;
} _properties[] = {
  {0x0001, L"MetaDictionary"},
  {0x0002, L"Header"}
};

void OMDictionary::initialize(void)
{
  TRACE("OMDictionary::initialize");

  for (size_t i = 0; i < sizeof(_properties)/sizeof(_properties[0]); i++) {
    OMPropertyDefinition* d =
      new OMBuiltinPropertyDefinition (0,
                                       _properties[i]._name,
                                       _properties[i]._pid,
                                       false);
    ASSERT("Valid heap pointer", d != 0);
    insert(d->localIdentification(), d);
  }
}

void OMDictionary::finalize(void)
{
  TRACE("OMDictionary::finalize");

  for (size_t i = 0; i < sizeof(_properties)/sizeof(_properties[0]); i++) {
    if (contains(_properties[i]._pid)) {
      OMPropertyDefinition* d = remove(_properties[i]._pid);
      delete d;
    }
  }
}

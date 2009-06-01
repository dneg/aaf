//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMPROPERTYDEFINITION_H
#define OMPROPERTYDEFINITION_H

#include "OMDataTypes.h"
#include "OMDefinition.h"

#include <stddef.h>

class OMClassDefinition;
class OMType;

  // @class Abstract base class used to define persistent properties
  //        supported by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMPropertyDefinition : public OMDefinition {
public:
  // @access Public members.

    // @cmember Destructor.
  virtual ~OMPropertyDefinition(void) {}

    // @cmember The type of the <c OMProperty> defined by this
    //          <c OMPropertyDefinition>.
  virtual const OMType* type(void) const = 0;

    // @cmember The locally unique identification of the <c OMProperty>
    //          defined by this <c OMPropertyDefinition>.
  virtual OMPropertyId localIdentification(void) const = 0;

    // @cmember Is the <c OMProperty> defined by this
    //          <c OMPropertyDefinition> optional?
  virtual bool isOptional(void) const = 0;

  virtual OMClassDefinition* containingClass(void) const = 0;

    // @cmember Does the Is the <c OMProperty> defined by this
    //          <c OMPropertyDefinition> uniquely identify the object?
  virtual bool isUniqueIdentifier(void) const = 0;
  

};

  // @class Definitions of persistent properties supported by
  //        the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
  //   @devnote This is a temporary class and will be merged into
  //            <c OMPropertyDefinition>. This will require changes
  //            to code in Object Manager clients.
class OMBuiltinPropertyDefinition : public OMPropertyDefinition {
public:
  // @access Public members.

    // @cmember Constructor.
  OMBuiltinPropertyDefinition(const OMType* type,
                              const wchar_t* name,
                              const wchar_t* description,
                              const OMPropertyId propertyId,
                              const bool isOptional);

    // @cmember Destructor.
  ~OMBuiltinPropertyDefinition(void);

    // @cmember The type of the <c OMProperty> defined by this
    //          <c OMBuiltinPropertyDefinition>.
  virtual const OMType* type(void) const;

    // @cmember The unique identification of the <c OMProperty> defined by
    //          this <c OMPropertyDefinition>.
  virtual const OMUniqueObjectIdentification& identification(void) const;

    // @cmember The name of the <c OMProperty> defined by this
    //          <c OMBuiltinPropertyDefinition>.
  virtual const wchar_t* name(void) const;

    // @cmember Does this <c OMBuiltinPropertyDefinition> have a description.
  virtual bool hasDescription(void) const;

    // @cmember The description of the <c OMProperty> defined by
    //          this <c OMBuiltinPropertyDefinition>.
    //          @precondition <f hasDefinition()>
  virtual const wchar_t* description(void) const;

    // @cmember The locally unique identification of the <c OMProperty>
    //          defined by this <c OMBuiltinPropertyDefinition>.
  virtual OMPropertyId localIdentification(void) const;

    // @cmember Is the <c OMProperty> defined by this
    //          <c OMBuiltinPropertyDefinition> optional?
  virtual bool isOptional(void) const;

  virtual OMClassDefinition* containingClass(void) const;

    // @cmember Is the <c OMProperty> defined by this
    //          <c OMBuiltinPropertyDefinition> predefined
    //          with respect to a well known baseline?
  virtual bool isPredefined(void) const;

    // @cmember Does the <c OMProperty> defined by this
    //          <c OMBuiltinPropertyDefinition> unique identify an object?
  virtual bool isUniqueIdentifier(void) const;
  
private:
  // @access Private members.

  const OMType* _type;
  const wchar_t* _name;
  const OMPropertyId _propertyId;
  const bool _isOptional;

};

#endif

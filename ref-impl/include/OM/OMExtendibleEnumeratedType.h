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
#ifndef OMEXTENDIBLEENUMERATEDTYPE_H
#define OMEXTENDIBLEENUMERATEDTYPE_H

#include "OMType.h"

// @author Tim Bingham | tjb | Avid Technology, Inc. |
// OMExtendibleEnumeratedType
class OMExtendibleEnumeratedType : public OMType {
public:

  virtual OMType::Tag tag(void) const;

  virtual OMUInt32 elementCount(void) const = 0;

  virtual const wchar_t* elementName(OMUInt32 index) const = 0;

  virtual OMUniqueObjectIdentification elementValue(OMUInt32 index) const = 0;

  virtual wchar_t* elementNameFromValue(OMUniqueObjectIdentification value) const = 0;

  virtual OMUniqueObjectIdentification elementValueFromName(const wchar_t* name) const = 0;

  virtual bool isValidValue(OMUniqueObjectIdentification value) const = 0;

  virtual bool registerElement(const wchar_t* name, OMUniqueObjectIdentification value) = 0;
    
};

#endif

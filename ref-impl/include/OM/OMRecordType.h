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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMRECORDTYPE_H
#define OMRECORDTYPE_H

#include "OMType.h"

#include "OMDataTypes.h"

class OMTypeVisitor;

// @author Tim Bingham | tjb | Avid Technology, Inc. | OMRecordType
class OMRecordType : public OMType {
public:

  virtual OMType::Tag tag(void) const;

  virtual OMUInt32 memberCount(void) const = 0;

  virtual const wchar_t* memberName(OMUInt32 index) const = 0;

  virtual const OMType* memberType(OMUInt32 index) const = 0;

  virtual void accept(OMTypeVisitor& visitor) const;

};

#endif

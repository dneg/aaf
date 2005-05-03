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
#ifndef OMRECORDTYPE_H
#define OMRECORDTYPE_H

#include "OMType.h"
#include "OMVector.h"

class OMRecordType : virtual public OMType
{
public:
    virtual OMUInt32 memberCount(void) const = 0;
    virtual wchar_t* memberName(OMUInt32 index) const = 0;
    virtual OMType* memberType(OMUInt32 index) const = 0;
    virtual bool initialise(const OMUniqueObjectIdentification& id, const wchar_t* name,
        const wchar_t* description, OMVector<wchar_t*>& memberNames, 
        OMVector<OMUniqueObjectIdentification>& memberTypeIds, 
        OMPropertyTag typeDefsTag) = 0;
};


#endif

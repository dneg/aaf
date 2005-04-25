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
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __OMINDIRECTTYPE_H__
#define __OMINDIRECTTYPE_H__


#include "OMType.h"

class OMIndirectType : virtual public OMType
{
public:
    virtual OMUniqueObjectIdentification actualTypeId(const OMByte* externalBytes, 
        size_t externalSize) const = 0;
    virtual OMType* actualType(const OMByte* externalBytes, size_t externalSize) const = 0;
    virtual OMByteOrder byteOrder(const OMByte* externalBytes, size_t externalSize) const = 0;
    virtual void actualData(const OMByte* externalBytes, size_t externalSize,
        const OMByte*& actualBytes, size_t& actualBytesSize) const = 0;
};


#endif


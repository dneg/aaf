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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#ifndef OMUNIQUEOBJECTIDENTTYPE_H
#define OMUNIQUEOBJECTIDENTTYPE_H

#include "OMType.h"
#include "OMSingleton.h"

class OMUniqueObjectIdentificationType;

  // @class Type definition for OMUniqueObjectIdentification.
  //   @base public | <c OMType>
  //   @base public | <c OMSingleton>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMUniqueObjectIdentificationType :
                         public OMType,
                         public OMSingleton<OMUniqueObjectIdentificationType> {
public:

  OMUniqueObjectIdentificationType(void);

  virtual ~OMUniqueObjectIdentificationType(void);

  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const;

  virtual size_t externalSize(const OMByte* internalBytes,
                              size_t internalBytesSize) const;

  virtual void externalize(const OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(const OMByte* externalBytes,
                              size_t externalSize) const;

  virtual void internalize(const OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;

};

#endif

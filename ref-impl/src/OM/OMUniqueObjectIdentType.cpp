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

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. |
//         OMUniqueObjectIdentificationType

#include "OMUniqueObjectIdentType.h"

#include "OMAssertions.h"
#include "OMIntegerType.h"

OMUniqueObjectIdentificationType::OMUniqueObjectIdentificationType(void)
{
  TRACE("OMUniqueObjectIdentificationType::OMUniqueObjectIdentificationType");
}

OMUniqueObjectIdentificationType::~OMUniqueObjectIdentificationType(void)
{
  TRACE("OMUniqueObjectIdentificationType::~OMUniqueObjectIdentificationType");
}

void OMUniqueObjectIdentificationType::reorder(
                                         OMByte* externalBytes,
                                         size_t ANAME(externalBytesSize)) const
{
  TRACE("OMUniqueObjectIdentificationType::reorder");

  ASSERT("Sizes match",
                    externalBytesSize == sizeof(OMUniqueObjectIdentification));

  OMUniqueObjectIdentification* p =
                reinterpret_cast<OMUniqueObjectIdentification*>(externalBytes);
  OMInteger32Type::instance()->reorder((OMByte*)&p->Data1, 4);
  OMInteger16Type::instance()->reorder((OMByte*)&p->Data2, 2);
  OMInteger16Type::instance()->reorder((OMByte*)&p->Data3, 2);
  // no need to swap Data4
}

size_t OMUniqueObjectIdentificationType::externalSize(
                                         const OMByte* NNAME(internalBytes),
                                         size_t ANAME(internalBytesSize)) const
{
  TRACE("OMUniqueObjectIdentificationType::externalSize");

  ASSERT("Sizes match",
                    internalBytesSize == sizeof(OMUniqueObjectIdentification));
  return sizeof(OMUniqueObjectIdentification);
}

void OMUniqueObjectIdentificationType::externalize(
                                            const OMByte* internalBytes,
                                            size_t internalBytesSize,
                                            OMByte* externalBytes,
                                            size_t NNAME(externalBytesSize),
                                            OMByteOrder NNAME(byteOrder)) const
{
  TRACE("OMUniqueObjectIdentificationType::externalize");

  ASSERT("Sizes match",
                    internalBytesSize == sizeof(OMUniqueObjectIdentification));
  copy(internalBytes, externalBytes, internalBytesSize);
}

size_t OMUniqueObjectIdentificationType::internalSize(
                                         const OMByte* NNAME(externalBytes),
                                         size_t ANAME(externalBytesSize)) const
{
  TRACE("OMUniqueObjectIdentificationType::internalSize");

  ASSERT("Sizes match",
                    externalBytesSize == sizeof(OMUniqueObjectIdentification));
  return sizeof(OMUniqueObjectIdentification);
}

void OMUniqueObjectIdentificationType::internalize(
                                            const OMByte* externalBytes,
                                            size_t externalBytesSize,
                                            OMByte* internalBytes,
                                            size_t NNAME(internalBytesSize),
                                            OMByteOrder NNAME(byteOrder)) const
{
  TRACE("OMUniqueObjectIdentificationType::internalize");

  ASSERT("Sizes match",
                    externalBytesSize == sizeof(OMUniqueObjectIdentification));
  copy(externalBytes, internalBytes, externalBytesSize);
}

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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
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
#include "OMDictionary.h"
#include "OMIntegerType.h"

OMUniqueObjectIdentificationType::OMUniqueObjectIdentificationType(void)
: OMBuiltinDefinition(Type_UniqueObjectIdentification,
                      L"UniqueObjectIdentification")
{
  TRACE("OMUniqueObjectIdentificationType::OMUniqueObjectIdentificationType");
}

OMUniqueObjectIdentificationType::~OMUniqueObjectIdentificationType(void)
{
  TRACE("OMUniqueObjectIdentificationType::~OMUniqueObjectIdentificationType");
}

bool OMUniqueObjectIdentificationType::isFixedSize(void) const
{
  return true;
}

void OMUniqueObjectIdentificationType::reorder(
                                       OMByte* externalBytes,
                                       OMUInt32 ANAME(externalBytesSize)) const
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

OMUInt32 OMUniqueObjectIdentificationType::externalSize(void) const
{
  return sizeof(OMUniqueObjectIdentification);
}

OMUInt32 OMUniqueObjectIdentificationType::externalSize(
                                       const OMByte* NNAME(internalBytes),
                                       OMUInt32 ANAME(internalBytesSize)) const
{
  TRACE("OMUniqueObjectIdentificationType::externalSize");

  ASSERT("Sizes match",
                    internalBytesSize == sizeof(OMUniqueObjectIdentification));
  return sizeof(OMUniqueObjectIdentification);
}

void OMUniqueObjectIdentificationType::externalize(
                                            const OMByte* internalBytes,
                                            OMUInt32 internalBytesSize,
                                            OMByte* externalBytes,
                                            OMUInt32 NNAME(externalBytesSize),
                                            OMByteOrder NNAME(byteOrder)) const
{
  TRACE("OMUniqueObjectIdentificationType::externalize");

  ASSERT("Sizes match",
                    internalBytesSize == sizeof(OMUniqueObjectIdentification));
  copy(internalBytes, externalBytes, internalBytesSize);
}

OMUInt32 OMUniqueObjectIdentificationType::internalSize(
                                       const OMByte* NNAME(externalBytes),
                                       OMUInt32 ANAME(externalBytesSize)) const
{
  TRACE("OMUniqueObjectIdentificationType::internalSize");

  ASSERT("Sizes match",
                    externalBytesSize == sizeof(OMUniqueObjectIdentification));
  return sizeof(OMUniqueObjectIdentification);
}

OMUInt32 OMUniqueObjectIdentificationType::internalSize(void) const
{
  TRACE("OMUniqueObjectIdentificationType::internalSize");
  return sizeof(OMUniqueObjectIdentification);
}

void OMUniqueObjectIdentificationType::internalize(
                                            const OMByte* externalBytes,
                                            OMUInt32 externalBytesSize,
                                            OMByte* internalBytes,
                                            OMUInt32 NNAME(internalBytesSize),
                                            OMByteOrder NNAME(byteOrder)) const
{
  TRACE("OMUniqueObjectIdentificationType::internalize");

  ASSERT("Sizes match",
                    externalBytesSize == sizeof(OMUniqueObjectIdentification));
  copy(externalBytes, internalBytes, externalBytesSize);
}

const OMUniqueObjectIdentification&
OMUniqueObjectIdentificationType::identification(void) const
{
  return OMBuiltinDefinition::identification();
}

const wchar_t* OMUniqueObjectIdentificationType::name(void) const
{
  return OMBuiltinDefinition::name();
}

bool OMUniqueObjectIdentificationType::hasDescription(void) const
{
  TRACE("OMUniqueObjectIdentificationType::hasDescription");
  return OMBuiltinDefinition::hasDescription();
}

const wchar_t* OMUniqueObjectIdentificationType::description(void) const
{
  TRACE("OMUniqueObjectIdentificationType::description");
  return OMBuiltinDefinition::description();
}

bool OMUniqueObjectIdentificationType::isPredefined(void) const
{
  TRACE("OMUniqueObjectIdentificationType::isPredefined");
  return OMBuiltinDefinition::isPredefined();
}

OMUInt32 OMUniqueObjectIdentificationType::memberCount(void) const
{
  TRACE("OMUniqueObjectIdentificationType::memberCount");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

const wchar_t* OMUniqueObjectIdentificationType::memberName(
                                                    OMUInt32 /* index */) const
{
  TRACE("OMUniqueObjectIdentificationType::memberName");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

const OMType* OMUniqueObjectIdentificationType::memberType(
                                                    OMUInt32 /* index */) const
{
  TRACE("OMUniqueObjectIdentificationType::memberType");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

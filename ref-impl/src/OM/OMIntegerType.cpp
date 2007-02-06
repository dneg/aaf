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
#include "OMIntegerType.h"

#include "OMDictionary.h"
#include "OMAssertions.h"

// class OMIntegerType
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMIntegerType

OMType::Tag OMIntegerType::tag(void) const
{
  return OMTTInteger;
}

bool OMIntegerType::isFixedSize(void) const
{
  return true;
}

void OMIntegerType::reorder(OMByte* externalBytes,
                            OMUInt32 ANAME(externalBytesSize)) const
{
  TRACE("OMIntegerType::reorder");

  ASSERT("Consistent sizes", externalBytesSize == size());
  if (size() > 1) {
    reorderInteger(externalBytes, size());
  }
}

OMUInt32 OMIntegerType::externalSize(const OMByte* ANAME(internalBytes),
                                     OMUInt32 ANAME(internalBytesSize)) const
{
  TRACE("OMIntegerType::externalSize");

  PRECONDITION("Valid internal bytes", internalBytes != 0);

  ASSERT("Consistent sizes", internalBytesSize == size());
  return size();
}

OMUInt32 OMIntegerType::externalSize(void) const
{
  return size();
}

void OMIntegerType::externalize(const OMByte* internalBytes,
                                OMUInt32 ANAME(internalBytesSize),
                                OMByte* externalBytes,
                                OMUInt32 externalBytesSize,
                                OMByteOrder NNAME(byteOrder)) const
{
  TRACE("OMIntegerType::externalize");
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size",
          internalBytesSize >= internalSize(externalBytes, externalBytesSize));
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size",
          externalBytesSize >= externalSize(internalBytes, internalBytesSize));

  ASSERT("Consistent sizes", internalBytesSize == size());
  ASSERT("Consistent sizes", externalBytesSize == size());
  copy(internalBytes, externalBytes, externalBytesSize);
}

OMUInt32 OMIntegerType::internalSize(const OMByte* ANAME(externalBytes),
                                     OMUInt32 ANAME(externalBytesSize)) const
{
  TRACE("OMIntegerType::internalSize");

  PRECONDITION("Valid external bytes", externalBytes != 0);

  ASSERT("Consistent sizes", externalBytesSize == size());
  return size();
}

OMUInt32 OMIntegerType::internalSize(void) const
{
  TRACE("OMIntegerType::internalSize");
  return size();
}

void OMIntegerType::internalize(const OMByte* externalBytes,
                                OMUInt32 ANAME(externalBytesSize),
                                OMByte* internalBytes,
                                OMUInt32 internalBytesSize,
                                OMByteOrder NNAME(byteOrder)) const
{
  TRACE("OMIntegerType::internalize");
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size",
          internalBytesSize >= internalSize(externalBytes, externalBytesSize));
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size",
          externalBytesSize >= externalSize(internalBytes, internalBytesSize));

  ASSERT("Consistent sizes", externalBytesSize == size());
  ASSERT("Consistent sizes", internalBytesSize == size());
  copy(externalBytes, internalBytes, internalBytesSize);
}

void OMIntegerType::accept(OMTypeVisitor& /* visitor */) const
{
  TRACE("OMIntegerType::accept");
  ASSERT("Unimplemented code not reached", false); // tjb - not yet
}

OMIntegerType::~OMIntegerType(void)
{
  TRACE("OMIntegerType::~OMIntegerType");
}

// class OMBuiltinIntegerType

OMBuiltinIntegerType::OMBuiltinIntegerType(
                            const OMStoredObjectIdentification& identification,
                            const wchar_t* name)
: OMBuiltinDefinition(identification, name)
{
  TRACE("OMBuiltinIntegerType::OMBuiltinIntegerType");
}

OMBuiltinIntegerType::~OMBuiltinIntegerType(void)
{
  TRACE("OMBuiltinIntegerType::~OMBuiltinIntegerType");
}

const OMUniqueObjectIdentification&
OMBuiltinIntegerType::identification(void) const
{
  return OMBuiltinDefinition::identification();
}

const wchar_t* OMBuiltinIntegerType::name(void) const
{
  return OMBuiltinDefinition::name();
}

bool OMBuiltinIntegerType::hasDescription(void) const
{
  TRACE("OMBuiltinIntegerType::hasDescription");
  return OMBuiltinDefinition::hasDescription();
}

const wchar_t* OMBuiltinIntegerType::description(void) const
{
  TRACE("OMBuiltinIntegerType::description");
  return OMBuiltinDefinition::description();
}

bool OMBuiltinIntegerType::isPredefined(void) const
{
  TRACE("OMBuiltinIntegerType::isPredefined");
  return OMBuiltinDefinition::isPredefined();
}

// class OMInteger08Type
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger08Type

OMInteger08Type::OMInteger08Type(void)
: OMBuiltinIntegerType(Type_Int8, L"Int8")
{
  TRACE("OMInteger08Type::OMInteger08Type");
}

OMInteger08Type::~OMInteger08Type(void)
{
  TRACE("OMInteger08Type::~OMInteger08Type");
}

OMUInt8 OMInteger08Type::size(void) const
{
  TRACE("OMInteger08Type::size");

  return 1;
}

bool OMInteger08Type::isSigned(void) const
{
  TRACE("OMInteger08Type::isSigned");
  return false;
}

// class OMInteger16Type
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger16Type

OMInteger16Type::OMInteger16Type(void)
: OMBuiltinIntegerType(Type_Int16, L"Int16")
{
  TRACE("OMInteger16Type::OMInteger16Type");
}

OMInteger16Type::~OMInteger16Type(void)
{
  TRACE("OMInteger16Type::~OMInteger16Type");
}

OMUInt8 OMInteger16Type::size(void) const
{
  TRACE("OMInteger16Type::size");

  return 2;
}


bool OMInteger16Type::isSigned(void) const
{
  TRACE("OMInteger16Type::isSigned");
  return false;
}

// class OMInteger32Type
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger32Type

OMInteger32Type::OMInteger32Type(void)
: OMBuiltinIntegerType(Type_Int32, L"Int32")
{
  TRACE("OMInteger32Type::OMInteger32Type");
}

OMInteger32Type::~OMInteger32Type(void)
{
  TRACE("OMInteger32Type::~OMInteger32Type");
}

OMUInt8 OMInteger32Type::size(void) const
{
  TRACE("OMInteger32Type::size");

  return 4;
}

bool OMInteger32Type::isSigned(void) const
{
  TRACE("OMInteger32Type::isSigned");
  return false;
}

// class OMInteger64Type
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger64Type

OMInteger64Type::OMInteger64Type(void)
: OMBuiltinIntegerType(Type_Int64, L"Int64")
{
  TRACE("OMInteger64Type::OMInteger64Type");
}

OMInteger64Type::~OMInteger64Type(void)
{
  TRACE("OMInteger64Type::~OMInteger64Type");
}

OMUInt8 OMInteger64Type::size(void) const
{
  TRACE("OMInteger64Type::size");

  return 8;
}

bool OMInteger64Type::isSigned(void) const
{
  TRACE("OMInteger64Type::isSigned");
  return false;
}

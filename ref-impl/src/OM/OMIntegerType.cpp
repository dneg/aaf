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
#include "OMIntegerType.h"

#include "OMAssertions.h"

// class OMIntegerType
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMIntegerType

void OMIntegerType::reorder(OMByte* externalBytes,
                            size_t ANAME(externalBytesSize)) const
{
  TRACE("OMIntegerType::reorder");

  ASSERT("Consistent sizes", externalBytesSize == size());
  if (size() > 1) {
    reorderInteger(externalBytes, size());
  }
}

size_t OMIntegerType::externalSize(const OMByte* ANAME(internalBytes),
                                   size_t ANAME(internalBytesSize)) const
{
  TRACE("OMIntegerType::externalSize");

  PRECONDITION("Valid internal bytes", internalBytes != 0);

  ASSERT("Consistent sizes", internalBytesSize == size());
  return size();
}

void OMIntegerType::externalize(const OMByte* internalBytes,
                                size_t ANAME(internalBytesSize),
                                OMByte* externalBytes,
                                size_t externalBytesSize,
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

size_t OMIntegerType::internalSize(const OMByte* ANAME(externalBytes),
                                   size_t ANAME(externalBytesSize)) const
{
  TRACE("OMIntegerType::internalSize");

  PRECONDITION("Valid external bytes", externalBytes != 0);

  ASSERT("Consistent sizes", externalBytesSize == size());
  return size();
}

void OMIntegerType::internalize(const OMByte* externalBytes,
                                size_t ANAME(externalBytesSize),
                                OMByte* internalBytes,
                                size_t internalBytesSize,
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

OMIntegerType::OMIntegerType(void)
{
  TRACE("OMIntegerType::OMIntegerType");
}

OMIntegerType::~OMIntegerType(void)
{
  TRACE("OMIntegerType::~OMIntegerType");
}

// class OMInteger08Type
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger08Type

OMInteger08Type::OMInteger08Type(void)
{
  TRACE("OMInteger08Type::OMInteger08Type");
}

OMInteger08Type::~OMInteger08Type(void)
{
  TRACE("OMInteger08Type::~OMInteger08Type");
}

size_t OMInteger08Type::size(void) const
{
  TRACE("OMInteger08Type::size");

  return 1;
}

// class OMInteger16Type
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger16Type

OMInteger16Type::OMInteger16Type(void)
{
  TRACE("OMInteger16Type::OMInteger16Type");
}

OMInteger16Type::~OMInteger16Type(void)
{
  TRACE("OMInteger16Type::~OMInteger16Type");
}

size_t OMInteger16Type::size(void) const
{
  TRACE("OMInteger16Type::size");

  return 2;
}

// class OMInteger32Type
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger32Type

OMInteger32Type::OMInteger32Type(void)
{
  TRACE("OMInteger32Type::OMInteger32Type");
}

OMInteger32Type::~OMInteger32Type(void)
{
  TRACE("OMInteger32Type::~OMInteger32Type");
}

size_t OMInteger32Type::size(void) const
{
  TRACE("OMInteger32Type::size");

  return 4;
}

// class OMInteger64Type
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMInteger64Type

OMInteger64Type::OMInteger64Type(void)
{
  TRACE("OMInteger64Type::OMInteger64Type");
}

OMInteger64Type::~OMInteger64Type(void)
{
  TRACE("OMInteger64Type::~OMInteger64Type");
}

size_t OMInteger64Type::size(void) const
{
  TRACE("OMInteger64Type::size");

  return 8;
}

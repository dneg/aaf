/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

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

size_t OMIntegerType::externalSize(OMByte* ANAME(internalBytes),
                                   size_t ANAME(internalBytesSize)) const
{
  TRACE("OMIntegerType::externalSize");

  PRECONDITION("Valid internal bytes", internalBytes != 0);

  ASSERT("Consistent sizes", internalBytesSize == size());
  return size();
}

void OMIntegerType::externalize(OMByte* internalBytes,
                                size_t ANAME(internalBytesSize),
                                OMByte* externalBytes,
                                size_t externalBytesSize,
                                OMByteOrder /* byteOrder */) const
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

size_t OMIntegerType::internalSize(OMByte* ANAME(externalBytes),
                                   size_t ANAME(externalBytesSize)) const
{
  TRACE("OMIntegerType::internalSize");

  PRECONDITION("Valid external bytes", externalBytes != 0);

  ASSERT("Consistent sizes", externalBytesSize == size());
  return size();
}

void OMIntegerType::internalize(OMByte* externalBytes,
                                size_t ANAME(externalBytesSize),
                                OMByte* internalBytes,
                                size_t internalBytesSize,
                                OMByteOrder /* byteOrder */) const
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

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

void OMUniqueObjectIdentificationType::reorder(OMByte* externalBytes,
                                               size_t externalBytesSize) const
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
                                         OMByte* /* internalBytes */,
                                         size_t ANAME(internalBytesSize)) const
{
  TRACE("OMUniqueObjectIdentificationType::externalSize");

  ASSERT("Sizes match",
                    internalBytesSize == sizeof(OMUniqueObjectIdentification));
  return sizeof(OMUniqueObjectIdentification);
}

void OMUniqueObjectIdentificationType::externalize(
                                             OMByte* internalBytes,
                                             size_t internalBytesSize,
                                             OMByte* externalBytes,
                                             size_t /*externalBytesSize*/,
                                             OMByteOrder /* byteOrder */) const
{
  TRACE("OMUniqueObjectIdentificationType::externalize");

  ASSERT("Sizes match",
                    internalBytesSize == sizeof(OMUniqueObjectIdentification));
  copy(internalBytes, externalBytes, internalBytesSize);
}

size_t OMUniqueObjectIdentificationType::internalSize(
                                         OMByte* /* externalBytes */,
                                         size_t ANAME(externalBytesSize)) const
{
  TRACE("OMUniqueObjectIdentificationType::internalSize");

  ASSERT("Sizes match",
                    externalBytesSize == sizeof(OMUniqueObjectIdentification));
  return sizeof(OMUniqueObjectIdentification);
}

void OMUniqueObjectIdentificationType::internalize(
                                             OMByte* externalBytes,
                                             size_t externalBytesSize,
                                             OMByte* internalBytes,
                                             size_t /* internalBytesSize */,
                                             OMByteOrder /* byteOrder */) const
{
  TRACE("OMUniqueObjectIdentificationType::internalize");

  ASSERT("Sizes match",
                    externalBytesSize == sizeof(OMUniqueObjectIdentification));
  copy(externalBytes, internalBytes, externalBytesSize);
}

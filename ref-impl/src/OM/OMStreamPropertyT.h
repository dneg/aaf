/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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
#ifndef OMSTREAMPROPERTYT_H
#define OMSTREAMPROPERTYT_H

#include "OMStreamProperty.h"

template <typename Element>
OMStreamProperty<Element>::OMStreamProperty(const OMPropertyId propertyId,
                                            const char* name)
  : OMDataStreamProperty(propertyId, name)
{
}

template <typename Element>
OMStreamProperty<Element>::~OMStreamProperty(void)
{
}

template <typename Element>
void OMStreamProperty<Element>::readElements(OMUInt64 index,
                                             OMUInt32 elementCount,
                                             Element* elements) const
{
  TRACE("OMStreamProperty<Element>::readElements");

  setIndex(index);
  readElements(elementCount, elements);
}

template <typename Element>
void OMStreamProperty<Element>::writeElements(OMUInt64 index,
                                              OMUInt32 elementCount,
                                              const Element* elements)
{
  TRACE("OMStreamProperty<Element>::writeElements");

  setIndex(index);
  writeElements(elementCount, elements);
}

template <typename Element>
void OMStreamProperty<Element>::readElement(OMUInt64 index,
                                            Element* element) const
{
  TRACE("OMStreamProperty<Element>::readElement");

  readElements(index, 1, element);
}

template <typename Element>
void OMStreamProperty<Element>::writeElement(OMUInt64 index,
                                             const Element* element)
{
  TRACE("OMStreamProperty<Element>::writeElement");

  writeElements(index, 1, element);
}

template <typename Element>
void OMStreamProperty<Element>::readElements(OMUInt32 elementCount,
                                             Element* elements) const
{
  TRACE("OMStreamProperty<Element>::readElements");

  OMUInt32 byteCount = elementCount * sizeof(Element);
  OMUInt32 actualByteCount;
  OMByte* bytes = reinterpret_cast<OMByte*>(elements);
  read(bytes, byteCount, actualByteCount);
  // reorder
  // internalize
  POSTCONDITION("All bytes read", actualByteCount == byteCount);
}

template <typename Element>
void OMStreamProperty<Element>::writeElements(OMUInt32 elementCount,
                                              const Element* elements)
{
  TRACE("OMStreamProperty<Element>::writeElements");
  
  OMUInt32 byteCount = elementCount * sizeof(Element);
  OMUInt32 actualByteCount;
  const OMByte* bytes = reinterpret_cast<const OMByte*>(elements);
  // externalize
  // reorder
  write(bytes, byteCount, actualByteCount);

  POSTCONDITION("All bytes written", actualByteCount == byteCount);
}

template <typename Element>
void OMStreamProperty<Element>::readElement(Element* element) const
{
  TRACE("OMStreamProperty<Element>::readElement");

  readElements(index(), 1, element);
}

template <typename Element>
void OMStreamProperty<Element>::writeElement(const Element* element)
{
  TRACE("OMStreamProperty<Element>::writeElement");

  writeElements(index(), 1, &element);
}

template <typename Element>
void OMStreamProperty<Element>::appendElements(OMUInt32 elementCount,
                                               const Element* elements)
{
  TRACE("OMStreamProperty<Element>::appendElements");

  writeElements(elementCount(), elementCount, elements);
}

template <typename Element>
void OMStreamProperty<Element>::appendElement(const Element element)
{
  TRACE("OMStreamProperty<Element>::appendElement");

  writeElements(elementCount(), 1, &element);
}

template <typename Element>
OMUInt64 OMStreamProperty<Element>::index(void) const
{
  TRACE("OMStreamProperty<Element>::index");

  OMUInt64 currentPosition = position();
  OMUInt64 result = currentPosition / sizeof(Element);

  return result;
}

template <typename Element>
void OMStreamProperty<Element>::setIndex(const OMUInt64 newIndex) const
{
  TRACE("OMStreamProperty<Element>::setIndex");

  OMUInt64 newPosition = newIndex * sizeof(Element);
  setPosition(newPosition);
}

template <typename Element>
OMUInt64 OMStreamProperty<Element>::elementCount(void) const
{
  TRACE("OMStreamProperty<Element>::elementCount");

  OMUInt64 streamSize = size();
  OMUInt64 result = streamSize / sizeof(Element);

  return result;
}


#endif

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
#ifndef OMSTREAMPROPERTYT_H
#define OMSTREAMPROPERTYT_H

#include "OMStreamProperty.h"

  // @mfunc Constructor.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The property id.
  //   @parm The name of this <c OMStreamProperty>.
template <typename Element>
OMStreamProperty<Element>::OMStreamProperty(const OMPropertyId propertyId,
                                            const wchar_t* name)
  : OMDataStreamProperty(propertyId, name)
{
}

  // @mfunc Destructor.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
template <typename Element>
OMStreamProperty<Element>::~OMStreamProperty(void)
{
}

  // @mfunc Read <p elementCount> <p Element>s, starting at <p index>,
  //        from this <c OMStreamProperty> into <p elements>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The index from which to read the elements.
  //   @parm The element count.
  //   @parm The elements.
  //   @this const
template <typename Element>
void OMStreamProperty<Element>::readElements(OMUInt64 index,
                                             OMUInt32 count,
                                             Element* elements) const
{
  TRACE("OMStreamProperty<Element>::readElements");

  PRECONDITION("Valid element count", count > 0);
  PRECONDITION("Valid buffer", elements != 0);
  PRECONDITION("Valid index", index < elementCount());
  PRECONDITION("Valid element count", (index + count) <= elementCount());

  setIndex(index);
  readElements(count, elements);
}

  // @mfunc Write <p elementCount> <p Element>s, starting at <p index>,
  //        to this <c OMStreamProperty> from <p elements>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The index at which to write the elements.
  //   @parm The element count.
  //   @parm The elements.
template <typename Element>
void OMStreamProperty<Element>::writeElements(OMUInt64 index,
                                              OMUInt32 elementCount,
                                              const Element* elements)
{
  TRACE("OMStreamProperty<Element>::writeElements");

  PRECONDITION("Valid element count", elementCount > 0);
  PRECONDITION("Valid buffer", elements != 0);

  setIndex(index);
  writeElements(elementCount, elements);
}

  // @mfunc Read a single <p Element>, at <p index>,
  //        from this <c OMStreamProperty> into <p element>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The index from which to read the element.
  //   @parm The element.
  //   @this const
template <typename Element>
void OMStreamProperty<Element>::readElement(OMUInt64 index,
                                            Element* element) const
{
  TRACE("OMStreamProperty<Element>::readElement");

  PRECONDITION("Valid buffer", element != 0);
  PRECONDITION("Valid index", index < elementCount());

  readElements(index, 1, element);
}

  // @mfunc Write a single <p Element>s, at <p index>,
  //        to this <c OMStreamProperty> from <p element>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The index at which to write the element.
  //   @parm The element.
template <typename Element>
void OMStreamProperty<Element>::writeElement(OMUInt64 index,
                                             const Element* element)
{
  TRACE("OMStreamProperty<Element>::writeElement");

  PRECONDITION("Valid buffer", element != 0);

  writeElements(index, 1, element);
}

  // @mfunc Read <p elementCount> <p Element>s from the current position in
  //        this <c OMStreamProperty> into <p elements>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The element count.
  //   @parm The elements.
  //   @this const
template <typename Element>
void OMStreamProperty<Element>::readElements(OMUInt32 elementCount,
                                             Element* elements) const
{
  TRACE("OMStreamProperty<Element>::readElements");

  PRECONDITION("Valid element count", elementCount > 0);
  PRECONDITION("Valid buffer", elements != 0);

  const OMType* elementType = type(); // Temporary, _element_ type !
  OMUInt32 actualElementCount;
  readTypedElements(elementType,
                    sizeof(Element),
                    (OMByte*)elements,
                    elementCount,
                    actualElementCount);
  POSTCONDITION("All elements read", actualElementCount == elementCount);
}

  // @mfunc Write <p elementCount> <p Element>s to the current position in
  //        this <c OMStreamProperty> from <p elements>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The element count.
  //   @parm The elements.
template <typename Element>
void OMStreamProperty<Element>::writeElements(OMUInt32 elementCount,
                                              const Element* elements)
{
  TRACE("OMStreamProperty<Element>::writeElements");
  
  PRECONDITION("Valid element count", elementCount > 0);
  PRECONDITION("Valid buffer", elements != 0);

  const OMType* elementType = type(); // Temporary, _element_ type !
  OMUInt32 actualElementCount;
  writeTypedElements(elementType,
                     sizeof(Element),
                     (OMByte*)elements,
                     elementCount,
                     actualElementCount);
  POSTCONDITION("All elements written", actualElementCount == elementCount);
}

  // @mfunc Read a single <p Element> from the current position in
  //        this <c OMStreamProperty> into <p element>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The element.
  //   @this const
template <typename Element>
void OMStreamProperty<Element>::readElement(Element* element) const
{
  TRACE("OMStreamProperty<Element>::readElement");

  PRECONDITION("Valid buffer", element != 0);

  readElements(index(), 1, element);
}

  // @mfunc Write a single <p Element> to the current position in
  //        this <c OMStreamProperty> from <p element>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The element.
template <typename Element>
void OMStreamProperty<Element>::writeElement(const Element* element)
{
  TRACE("OMStreamProperty<Element>::writeElement");

  PRECONDITION("Valid buffer", element != 0);

  writeElements(index(), 1, &element);
}

  // @mfunc Write <p elementCount> <p Element>s to the end of this
  //        <c OMStreamProperty> from <p elements>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The element count.
  //   @parm The elements.
template <typename Element>
void OMStreamProperty<Element>::appendElements(OMUInt32 elementCount,
                                               const Element* elements)
{
  TRACE("OMStreamProperty<Element>::appendElements");

  PRECONDITION("Valid element count", elementCount > 0);
  PRECONDITION("Valid buffer", elements != 0);

  writeElements(elementCount(), elementCount, elements);
}

  // @mfunc Write a single <p Element> to the end of
  //        this <c OMStreamProperty>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The element.
template <typename Element>
void OMStreamProperty<Element>::appendElement(const Element* element)
{
  TRACE("OMStreamProperty<Element>::appendElement");

  writeElements(elementCount(), 1, element);
}

  // @mfunc The index of the current <p Element>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @rdesc The current index.
  //   @this const
template <typename Element>
OMUInt64 OMStreamProperty<Element>::index(void) const
{
  TRACE("OMStreamProperty<Element>::index");

  OMUInt64 currentPosition = position();
  OMUInt64 result = currentPosition / sizeof(Element);

  return result;
}

  // @mfunc Make the <p Element> at <p newIndex> the current one.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The new value for the current index.
  //   @this const
template <typename Element>
void OMStreamProperty<Element>::setIndex(const OMUInt64 newIndex) const
{
  TRACE("OMStreamProperty<Element>::setIndex");

  OMUInt64 newPosition = newIndex * sizeof(Element);
  setPosition(newPosition);
}

  // @mfunc The count of <p Element>s in this <c OMStreamProperty>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @rdesc The element count.
  //   @this const
template <typename Element>
OMUInt64 OMStreamProperty<Element>::elementCount(void) const
{
  TRACE("OMStreamProperty<Element>::elementCount");

  OMUInt64 streamSize = size();
  OMUInt64 result = streamSize / sizeof(Element);

  return result;
}

  // @mfunc Set the count of <p Element>s in this <c OMStreamProperty>
  //        to <p newElementCount>.
  //   @tcarg class | Element | The type of an <c OMStreamProperty> element.
  //   @parm The new element count.
template <typename Element>
void OMStreamProperty<Element>::setElementCount(OMUInt64 newElementCount)
{
  TRACE("OMStreamProperty<Element>::elementCount");

  OMUInt64 newSize = newElementCount * sizeof(Element);
  
  setSize(newSize);
}

#endif

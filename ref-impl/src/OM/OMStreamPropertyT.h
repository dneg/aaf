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
void OMStreamProperty<Element>::readElements(size_t startElement,
                                             size_t elementCount,
                                             Element* elements) const
{
}

template <typename Element>
void OMStreamProperty<Element>::writeElements(size_t startElement,
                                              size_t elementCount,
                                              const Element* elements)
{
}

template <typename Element>
void OMStreamProperty<Element>::readElements(size_t elementCount,
                                             Element* elements) const
{
}

template <typename Element>
void OMStreamProperty<Element>::writeElements(size_t elementCount,
                                              const Element* elements)
{
}

template <typename Element>
void OMStreamProperty<Element>::readElement(Element* element) const
{
}

template <typename Element>
void OMStreamProperty<Element>::writeElement(const Element* element)
{
}

template <typename Element>
void OMStreamProperty<Element>::appendElements(size_t elementCount,
                                               const Element* elements)
{
}

template <typename Element>
void OMStreamProperty<Element>::appendElement(const Element* element)
{
}

template <typename Element>
size_t OMStreamProperty<Element>::position(void) const
{
}

template <typename Element>
void OMStreamProperty<Element>::setPosition(size_t newPosition)
{
}

template <typename Element>
size_t OMStreamProperty<Element>::elementCount(void) const
{
}


#endif

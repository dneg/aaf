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
#ifndef OMSTREAMPROPERTY_H
#define OMSTREAMPROPERTY_H

#include "OMDataTypes.h"

#include "OMDataStreamProperty.h"

template <typename Element>
class OMStreamProperty : public OMDataStreamProperty {
public:

  OMStreamProperty(const OMPropertyId propertyId, const char* name);

  virtual ~OMStreamProperty(void);
 
    // All reads/and writes advance the current element

// Random access in chunks of Elements

    // Read elementCount Elements, starting at index,
    // from the stream into elements.
  void readElements(OMUInt64 index,
                    OMUInt32 elementCount,
                    Element* elements) const;

    // Write elementCount Elements, starting at index,
    // to the stream from elements.
  void writeElements(OMUInt64 index,
                     OMUInt32 elementCount,
                     const Element* elements);

// Random access one Element at a time.

    // Read elementCount Elements, starting at index,
    // from the stream into elements.
  void readElement(OMUInt64 index,
                   Element* element) const;

    // Write elementCount Elements, starting at index,
    // to the stream from elements.
  void writeElement(OMUInt64 index,
                    const Element* element);

// Sequential access in chunks of Elements

    // Read elementCount Elements from the stream into elements.
  void readElements(OMUInt32 elementCount,
                    Element* elements) const;

    // Write elementCount Elements to the stream from elements.
  void writeElements(OMUInt32 elementCount,
                     const Element* elements);

// Sequential access one Element at a time.

    // Read the current Element into element.
  void readElement(Element* element) const;

    // Write an Element at the current position.
  void writeElement(const Element* element);

// Extend in chunks of Elements

    // Write elementCount Elements to the end of the stream from elements.
  void appendElements(OMUInt32 elementCount,
                      const Element* elements);

// Extend one Element at a time

    // Write an element at the end of the stream.
  void appendElement(const Element element);

// Stream positioning and count of elements

    // The index of the current element.
  OMUInt64 index(void) const;

    // Make the element at newIndex the current one.
  void setIndex(const OMUInt64 newIndex) const;

    // The count of elements in the stream.
  OMUInt64 elementCount(void) const;

    // Set the count of elements in the stream.
  void setElementCount(OMUInt64 newElementCount);

};

#include "OMStreamPropertyT.h"

#endif

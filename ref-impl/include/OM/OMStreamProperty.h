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

// General access

    // Read elementCount Elements, starting at startElement,
    // from the stream into elements.
  void readElements(size_t startElement,
                    size_t elementCount,
                    Element* elements) const;

    // Write elementCount Elements, starting at startElement,
    // to the stream from elements.
  void writeElements(size_t startElement,
                     size_t elementCount,
                     const Element* elements);

// Sequential access in chunks of Elements

    // Read elementCount Elements from the stream into elements.
  void readElements(size_t elementCount,
                    Element* elements) const;

    // Write elementCount Elements to the stream from elements.
  void writeElements(size_t elementCount,
                     const Element* elements);

// Sequential access one Element at a time.

    // Read the current Element into element.
  void readElement(Element* element) const;

    // Write an Element at the current position.
  void writeElement(const Element* element);

// Extend in chunks of Elements

    // Write elementCount Elements to the end of the stream from elements.
  void appendElements(size_t elementCount,
                      const Element* elements);

// Extend one Element at a time

    // Write an element at the end of the stream.
  void appendElement(const Element* element);


// Stream positioning and count of elements

    // The ordinal of the current element.
  size_t position(void) const;

    // Make the element at ordinal newPosition the currentOne.
  void setPosition(size_t newPosition);

    // The count of elements in the stream.
  size_t elementCount(void) const;

};

#include "OMStreamPropertyT.h"

#endif

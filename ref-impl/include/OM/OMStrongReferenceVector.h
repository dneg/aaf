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

// @doc OMINTERNAL
#ifndef OMSTRONGREFERENCEVECTOR_H
#define OMSTRONGREFERENCEVECTOR_H

#include "OMRefVectorProperty.h"

template <typename Element>
class OMContainerIterator;
class OMStrongReferenceVectorElement;

  // @class Persistent elastic sequential collections of strongly
  //        referenced (contained) objects supported by the Object Manager.
  //        Objects are accessible by index. The order of objects is
  //        determined externally. Duplicate objects are allowed.
  //   @base public | <c OMReferenceVectorProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStrongReferenceVector : public OMReferenceVectorProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceVector(const OMPropertyId propertyId,
                          const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceVector(void);

  virtual OMContainerIterator<OMStrongReferenceVectorElement>*
                                                      iterator(void) const = 0;

  virtual void insert(const size_t index,
                      const OMStrongReferenceVectorElement& element) = 0;

    // @cmember Increase the capacity of this
    //          <c OMStrongReferenceVector> so that it
    //          can contain at least <p capacity> <p OMObject>s
    //          without having to be resized.
  virtual void grow(const size_t capacity) = 0;

};

#endif

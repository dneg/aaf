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

// @doc OMINTERNAL
#ifndef OMVECTOR_H
#define OMVECTOR_H

#include "OMContainer.h"

#include <stddef.h>

  // @class Elastic sequential collections of elements accessible by
  //        index.  The order of elements is determined externally.
  //        Duplicate elements are allowed.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @base public | <c OMContainer>
template <typename Element>
class OMVector : public OMContainer<Element> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMVector();

    // @cmember Destructor.
  virtual ~OMVector(void);

    // @cmember Insert <p value> into this <c OMVector>.
  virtual void insert(const Element value);

    // @cmember Does this <c OMVector> contain <p value> ?
  virtual bool containsValue(const Element value) const;

    // @cmember The number of elements in this <c OMVector>.
    //          <mf OMVector::count> returns the actual number
    //          of elements in the <c OMVector>.
  size_t count(void) const;

    // @cmember The capacity of this <c OMVector>.
    //          <mf OMVector::capacity> returns the potential
    //          number of elements in the <c OMVector>.
  virtual size_t capacity(void) const;

    // @cmember Increase the capacity of this <c OMVector> so that it
    //          can contain at least <p capacity> elements without
    //          having to be resized.
  virtual void grow(const size_t capacity);

    // @cmember Free any unused capacity in this <c OMVector> while
    //          ensuring that it can contain at least <p capacity>
    //          elements.
  virtual void shrink(const size_t capacity);

    // @cmember Is this <c OMVector> full ?
  virtual bool full(void) const;

    // @cmember Is this <c OMVector> empty ?
  virtual bool empty(void) const;

    // @cmember Set the value of the <p Element> at
    //          position <p index> in this <c OMVector>.
    //          The existing <p Element> at <p index> is replaced.
  void setAt(const Element value, const size_t index);

    // @cmember Get the value of the <p Element> at
    //          position <p index> in this <c OMVector>.
  void getAt(Element& value, const size_t index) const;

    // @cmember Get the value of the <p Element> at
    //          position <p index> in this <c OMVector>.
  Element& getAt(const size_t index) const;

    // @cmember Insert <p value> into this <c OMVector> at
    //          position <p index>. Existing values in this
    //          <c OMVector> at <p index> and higher are
    //          shifted up one index position.
  virtual void insertAt(const Element value, const size_t index);

    // @cmember Append the given <p Element> <p value> to
    //          this <c OMVector>. The new element is added after
    //          the last element currently in this <c OMVector>.
  void append(const Element value);

    // @cmember Prepend the given <p Element> <p value> to
    //          this <c OMVector>. The new element is added before
    //          the first element currently in this <c OMVector>.
    //          Existing values in this <c OMVector> are shifted
    //          up one index position.
  void prepend(const Element value);

    // @cmember Remove the value from this <c OMVector> at
    //          position <p index>. Existing values in this
    //          <c OMVector> at <p index> + 1 and higher are
    //          shifted down on index position.
  virtual void removeAt(const size_t index);

    // @cmember Remove the last (index == count() - 1) element
    //          from this <c OMVector>.
  void removeLast(void);

    // @cmember Remove the first (index == 0) element
    //          from this <c OMVector>. Existing values in this
    //          <c OMVector> are shifted down one index position.
  void removeFirst(void);

private:
  // @access Private members.

  Element* _vector; // The representation of the vector
  size_t _capacity; // Potential number of elements
  size_t _count;    // Actual number of elements

    // @cmember Calculate the next valid capacity higher
    //          than <p capacity>.
  static size_t nextHigherCapacity(size_t capacity);

};

#include "OMVectorT.h"

#endif

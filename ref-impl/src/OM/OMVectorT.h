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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMVector
#ifndef OMVECTORT_H
#define OMVECTORT_H

#include "OMAssertions.h"

  // @mfunc Constructor.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
template <typename Element>
OMVector<Element>::OMVector()
: _vector(0), _capacity(0), _count(0)
{
  TRACE("OMVector<Element>::OMVector");
}

  // @mfunc Destructor.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
template <typename Element>
OMVector<Element>::~OMVector(void)
{
  TRACE("OMVector<Element>::""~OMVector");

  delete [] _vector;
  _vector = 0;
}

  // @mfunc Insert <p value> into this <c OMVector>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The Element to insert. A value of type <p Element> by value.
template <typename Element>
void OMVector<Element>::insert(const Element value)
{
  TRACE("OMVector<Element>::insert");

  insertAt(value, count());
}

  // @mfunc Does this <c OMVector> contain <p value> ?
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The Element to search for. A value of type <p Element> by value.
  //   @this const
template <typename Element>
bool OMVector<Element>::containsValue(const Element value) const
{
  TRACE("OMVector<Element>::containsValue");

  bool result = false;

  for (size_t i = 0; i < _count; i++) {
    if (_vector[i] == value) {
      result = true;
      break;
    }
  }
  return result;
}

  // @mfunc The number of elements in this <c OMVector>.
  //        <mf OMVector::count> returns the actual number
  //        of elements in the <c OMVector>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @rdesc The number of elements in this <c OMVector>.
  //   @this const
template <typename Element>
size_t OMVector<Element>::count(void) const
{
  TRACE("OMVector<Element>::count");

  return _count;
}

  // @mfunc Remove <p value> from this <c OMVector>.
  //        In the case of duplicate values, the one with the lowest
  //        index is removed.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //  @parm The Element to remove. A value of type <p Element> by value.
template <typename Element>
void OMVector<Element>::removeValue(const Element value)
{
  TRACE("OMVector<Element>::removeValue");

  PRECONDITION("Value is present", containsValue(value));

  size_t index = indexOfValue(value);

  removeAt(index);

}

  // @mfunc The capacity of this <c OMVector>.
  //        <mf OMVector::capacity> returns the potential
  //        number of elements in the <c OMVector>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @rdesc The capacity of this <c OMVector>.
  //   @this const
template <typename Element>
size_t OMVector<Element>::capacity(void) const
{
  TRACE("OMVector<Element>::capacity");

  return _capacity;
}

  // @mfunc Increase the capacity of this <c OMVector> so that it
  //        can contain at least <p capacity> elements without
  //        having to be resized.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The new capacity.
template <typename Element>
void OMVector<Element>::grow(size_t capacity)
{
  TRACE("OMVector<Element>::grow");
  PRECONDITION("Valid capacity", capacity > 0);

  // Calculate the new capacity
  //
  size_t newCapacity = nextHigherCapacity(capacity);

  if (newCapacity > _capacity) {

    _capacity = newCapacity;

    // Save old array
    //
    Element* oldVector = _vector;

    // Allocate new array
    //
    _vector = new Element[_capacity];
    ASSERT("Valid heap pointer", _vector != 0);

    // Copy over all elements from the old array
    //
    for (size_t i = 0; i < _count; i++) {
      _vector[i] = oldVector[i];
    }

    // Delete the old array
    //
    delete [] oldVector;
  }
  POSTCONDITION("Size properly increased", _capacity >= capacity);
}

  // @mfunc Free any unused capacity in this <c OMVector> while
  //        ensuring that it can contain at least <p capacity>
  //        elements.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The new capacity.
template <typename Element>
void OMVector<Element>::shrink(size_t capacity)
{
  TRACE("OMVector<Element>::shrink");

  // Calculate the new capacity
  //
  size_t newCapacity = nextHigherCapacity(capacity);

  if (newCapacity < _capacity) {

    _capacity = newCapacity;

    // Save old array
    //
    Element* oldVector = _vector;

    if (_capacity > 0) {

      // Allocate new array
      //
      _vector = new Element[_capacity];
      ASSERT("Valid heap pointer", _vector != 0);

      // Copy over all elements from the old array
      //
      for (size_t i = 0; i < _count; i++) {
        _vector[i] = oldVector[i];
      }

    } else {
     _vector = 0;
    }

    // Delete the old array
    //
    delete [] oldVector;
  }
  // POSTCONDITION("Size properly decreased", _capacity <= oldCapacity);
}

  // @mfunc Is this <c OMVector> full ?
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @rdesc True if this <c OMVector> is full, false otherwise ?
  //   @this const
template <typename Element>
bool OMVector<Element>::full(void) const
{
  TRACE("OMVector<Element>::full");

  bool result;
  if (count() == capacity()) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Is this <c OMVector> empty ?
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @rdesc True if this <c OMVector> is empty, false otherwise ?
  //   @this const
template <typename Element>
bool OMVector<Element>::empty(void) const
{
  TRACE("OMVector<Element>::empty");

  bool result;
  if (count() == 0) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Set the value of the <p Element> at
  //        position <p index> in this <c OMVector>.
  //        The existing <p Element> at <p index> is replaced.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The new value.
  //   @parm The index.
template <typename Element>
void OMVector<Element>::setAt(const Element value, const size_t index)
{
  TRACE("OMVector<Element>::setAt");
  PRECONDITION("Valid index", index <  _count);

  _vector[index] = value;

  POSTCONDITION("Element properly inserted", _vector[index] == value);
}

  // @mfunc Get the value of the <p Element> at
  //        position <p index> in this <c OMVector>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The value obtained. By reference.
  //   @parm The index.
  //   @this const
template <typename Element>
void OMVector<Element>::getAt(Element& value, const size_t index) const
{
  TRACE("OMVector<Element>::getAt");
  PRECONDITION("Valid index", index < _count);

  value = _vector[index];
}

  // @mfunc Get the value of the <p Element> at
  //        position <p index> in this <c OMVector>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The index.
  //   @this const
template <typename Element>
Element& OMVector<Element>::getAt(const size_t index) const
{
  TRACE("OMVector<Element>::getAt");
  PRECONDITION("Valid index", index < _count);

  return _vector[index];
}

  // @mfunc The value of the <p Element> at
  //        position <p index> in this <c OMVector>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The index.
  //   @this const
template <typename Element>
Element& OMVector<Element>::valueAt(const size_t index) const
{
  TRACE("OMVector<Element>::valueAt");
  PRECONDITION("Valid index", index < _count);

  return _vector[index];
}

  // @mfunc Insert <p value> into this <c OMVector> at
  //        position <p index>. Existing values in this
  //        <c OMVector> at <p index> and higher are
  //        shifted up one index position.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The value to insert.
  //   @parm The index.
template <typename Element>
void OMVector<Element>::insertAt(const Element value, const size_t index)
{
  TRACE("OMVector<Element>::insertAt");
  PRECONDITION("Valid index", index <= _count);
  SAVE(_count, size_t);

  // Make space for at least one more element
  //
  grow(_count + 1);

  // Shuffle up existing elements, if any and if necessary
  //
  for (size_t i = _count; i > index; i--) {
    _vector[i] = _vector[i - 1];
  }

  _vector[index] = value;
  _count = _count + 1;

  POSTCONDITION("Element properly inserted", _vector[index] == value);
  POSTCONDITION("One more element", _count == OLD(_count) + 1);
}

  // @mfunc Append the given <p Element> <p value> to
  //        this <c OMVector>. The new element is added after
  //        the last element currently in this <c OMVector>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The value to append.
template <typename Element>
void OMVector<Element>::append(const Element value)
{
  TRACE("OMVector<Element>::append");

  insertAt(value, count());
}

  // @mfunc Prepend the given <p Element> <p value> to
  //        this <c OMVector>. The new element is added before
  //        the first element currently in this <c OMVector>.
  //        Existing values in this <c OMVector> are shifted
  //        up one index position.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The value to prepend.
template <typename Element>
void OMVector<Element>::prepend(const Element value)
{
  TRACE("OMVector<Element>::prepend");

  insertAt(value, 0);
}

  // @mfunc Remove the value from this <c OMVector> at
  //        position <p index>. Existing values in this
  //        <c OMVector> at <p index> + 1 and higher are
  //        shifted down on index position.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The index of the value to be removed.
template <typename Element>
void OMVector<Element>::removeAt(const size_t index)
{
  TRACE("OMVector<Element>::removeAt");
  PRECONDITION("Valid index", index < _count);
  SAVE(_count, size_t);

  // Shuffle down existing elements
  //
  for (size_t i = index; i < _count - 1; i++) {
    _vector[i] = _vector[i + 1];
  }

  _count = _count - 1;

  // Trim execss capacity
  //
  shrink(_count);

  POSTCONDITION("One less element", _count == OLD(_count) - 1);
}

  // @mfunc Remove the last (index == count() - 1) element
  //         from this <c OMVector>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
template <typename Element>
void OMVector<Element>::removeLast(void)
{
  TRACE("OMVector<Element>::removeLast");
  PRECONDITION("Not empty", !empty());

  removeAt(count() - 1);
}

  // @mfunc Remove the first (index == 0) element
  //        from this <c OMVector>. Existing values in this
  //        <c OMVector> are shifted down one index position.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
template <typename Element>
void OMVector<Element>::removeFirst(void)
{
  TRACE("OMVector<Element>::removeFirst");
  PRECONDITION("Not empty", !empty());

  removeAt(0);
}

  // @mfunc Remove all elements from this <c OMVector>.
  //        from this <c OMVector>. Existing values in this
  //        <c OMVector> are shifted down one index position.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
template <typename Element>
void OMVector<Element>::clear(void)
{
  TRACE("OMVector<Element>::clear");

  size_t elementCount = count();
  for (size_t i = 0; i < elementCount; i++) {
    removeLast();
  }
  POSTCONDITION("All elements removed", count() == 0);
  POSTCONDITION("Empty", capacity() == 0);
}

  // @mfunc The index of the element with value <p value>.
  //        In the case of duplicate values, lowest index is returned.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The value for which the index is to be found.
  //   @this const
template <typename Element>
size_t OMVector<Element>::indexOfValue(const Element value) const
{
  TRACE("OMVector<Element>::indexOfValue");

  PRECONDITION("Value is present", containsValue(value));

  size_t result;

  for (size_t i = 0; i < _count; i++) {
    if (_vector[i] == value) {
      result = i;
      break;
    }
  }
  return result;
}

  // @mfunc The number of elements with value <p value>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The value for which the index is to be found.
  //   @this const
template <typename Element>
size_t OMVector<Element>::countValue(const Element value) const
{
  TRACE("OMVector<Element>::countValue");

  size_t result = 0;

  for (size_t i = 0; i < _count; i++) {
    if (_vector[i] == value) {
      result = result + 1;
    }
  }
  return result;
}

  // @mfunc Calculate the next valid capacity higher than <p capacity>.
  //   @tcarg class | Element | The type of an <c OMVector> element.
  //          This type must support operator = and operator ==.
  //   @parm The desired capaciy.
template <typename Element>
size_t OMVector<Element>::nextHigherCapacity(size_t capacity)
{
  TRACE("OMVector<Element>::nextHigherCapacity");

  // The existing capacity is doubled until doubling is not possible.
  // Once doubling is no longer possible the capacity is set to the
  // maximum index value.
  // If size_t is, for example, 32 bits this allows a maximum capacity
  // of 4G vector elements. This is an architectural limit.
  // All vectors with less than 2G elements have a capacity that is an
  // even power of two.
  // Doubling the vector capacity gives O(lg N) performance in the
  // common case of creating a vector from scratch using a sequence
  // of append operations.
  //
  const size_t OMSIZE_T_MAX = ~(size_t)0;
  const size_t OMSIZE_T_MASK = ~(OMSIZE_T_MAX >> 1); // set only the msb

  size_t result;
  if (capacity == 0) {
    result = 0;
  } else if (capacity < OMSIZE_T_MASK) {
    ASSERT("Non-zero capacity", capacity > 0);
    ASSERT("Capacity can be doubled", capacity < OMSIZE_T_MASK);

    size_t mask = OMSIZE_T_MASK;
    size_t oldMask;

    do {
      oldMask = mask;
      mask = mask >> 1;
    } while (!(mask & capacity));

    if (mask == capacity) {
      result = capacity; // exact power of 2
    } else {
      result = oldMask; // next higher power
    }
  } else {
    result = OMSIZE_T_MAX;
  }
  POSTCONDITION("Valid result", result >= capacity);
  return result;
}

#endif

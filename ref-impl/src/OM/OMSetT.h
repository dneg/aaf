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
#ifndef OMSETT_H
#define OMSETT_H

#include "OMAssertions.h"

template<typename Key, typename Element>
OMSet<Key, Element>::OMSet()
{
}

template<typename Key, typename Element>
OMSet<Key, Element>::~OMSet()
{
}

  // @mfunc Insert <p value> into this <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Element> to insert.
template <typename Key, typename Element>
void OMSet<Key, Element>::insert(const Element value)
{
  TRACE("OMSet<Key, Element>::insert");

  _tree.insert(value.identification() ,value);

}

  // @mfunc Does this <c OMSet> contain <p value> ?
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Element> for which to search.
  //   @rdesc True if this <c OMSet> contains <p value>, false otherwise.
  //   @this const
template <typename Key, typename Element>
bool OMSet<Key, Element>::containsValue(const Element value) const
{
  TRACE("OMSet<Key, Element>::containsValue");

  bool result = _tree.contains(value.identification());

  return result;
}

  // @mfunc Does this <c OMSet> contain an <p Element>
  //        identified by <p key>?
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key> for which to search.
  //   @rdesc True if this <c OMSet> contains an <p Element>
  //          identified by <p key>, false otherwise.
  //   @this const
template <typename Key, typename Element>
bool OMSet<Key, Element>::contains(const Key key) const
{
  TRACE("OMSet<Key, Element>::contains");

  bool result = _tree.contains(key);

  return result;
}

  // @mfunc Find the <p Element> in this <c OMSet> identified by
  //        <p key>.  If the element is found it is returned in
  //        <p value> and the result is true. If the element is
  //        not found the result is false.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key> for which to search.
  //   @parm The value that was found, if any.
  //   @rdesc True if this <c OMSet> contains an <p Element>
  //          identified by <p key>, false otherwise.
  //   @this const
template <typename Key, typename Element>
bool OMSet<Key, Element>::find(const Key key, Element& value) const
{
  TRACE("OMSet<Key, Element>::find");

  bool result = _tree.find(key, value);

  return result;
}

  // @mfunc The number of elements in this <c OMSet>.
  //        <mf OMSet::count> returns the actual number
  //        of elements in the <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @rdesc The count of elements in this <c OMSet>.
  //   @this const
template <typename Key, typename Element>
size_t OMSet<Key, Element>::count(void) const
{
  TRACE("OMSet<Key, Element>::count");

  size_t result = _tree.count();

  return result;
}

  // @mfunc Append the given <p Element> <p value> to
  //        this <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Element> to append.
template <typename Key, typename Element>
void OMSet<Key, Element>::append(const Element value)
{
  TRACE("OMSet<Key, Element>::append");

  insert(value);
}

  // @mfunc Remove the <p Element> with <p Key> <p key> from this <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Key> of the <p Element> to remove.
template <typename Key, typename Element>
void OMSet<Key, Element>::remove(const Key key)
{
  TRACE("OMSet<Key, Element>::remove");

  _tree.remove(key);
}

  // @mfunc Remove <p value> from this <c OMSet>.
  //   @tcarg class | Element | The type of an <c OMSet> element.
  //          This type must support operator = and operator==.
  //          Instances of this type must be able to return a unique
  //          value of type <p Key> to identify themselves through a
  //          function with the signature
  //          const Key Element::identification(void) const.
  //   @tcarg class | Key  | The type of the unique key used to identify
  //          elements. This type must support operator =, operator !=
  //          and operator <lt>.
  //   @parm The <p Element> to remove.
template <typename Key, typename Element>
void OMSet<Key, Element>::removeValue(const Element value)
{
  TRACE("OMSet<Key, Element>::removeValue");

  _tree.remove(value.identification());
}

#endif

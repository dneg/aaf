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
#include "OMContainerElement.h"

#include "OMAssertions.h"

// class OMStrongReferenceVectorElement

  // @mfunc Constructor.
OMStrongReferenceVectorElement::OMStrongReferenceVectorElement(void)
: OMContainerElement<OMStrongObjectReference>(),
  _localKey(0)
{
  TRACE("OMStrongReferenceVectorElement::OMStrongReferenceVectorElement");
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> (a vector property) that contains this
  //         <c OMStrongReferenceVectorElement>.
  //   @parm The name of this <c OMStrongReferenceVectorElement>.
  //   @parm The local key of this <c OMStrongReferenceVectorElement> within
  //         it's vector.
OMStrongReferenceVectorElement::OMStrongReferenceVectorElement(
                                                          OMProperty* property,
                                                          const wchar_t* name,
                                                          OMUInt32 localKey)
: OMContainerElement<OMStrongObjectReference>(
                                      OMStrongObjectReference(property, name)),
  _localKey(localKey)
{
  TRACE("OMStrongReferenceVectorElement::OMStrongReferenceVectorElement");
}

  // @mfunc Copy constructor.
  //   @parm The <c OMStrongReferenceVectorElement> to copy.
OMStrongReferenceVectorElement::OMStrongReferenceVectorElement(
                                     const OMStrongReferenceVectorElement& rhs)
: OMContainerElement<OMStrongObjectReference>(rhs),
  _localKey(rhs._localKey)
{
  TRACE("OMStrongReferenceVectorElement::OMStrongReferenceVectorElement");
}

  // @mfunc Destructor.
OMStrongReferenceVectorElement::~OMStrongReferenceVectorElement(void)
{
  TRACE("OMStrongReferenceVectorElement::~OMStrongReferenceVectorElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMStrongReferenceVectorElement> to be assigned.
  //   @rdesc The <c OMStrongReferenceVectorElement> resulting from
  //          the assignment.
OMStrongReferenceVectorElement&
OMStrongReferenceVectorElement::operator= (
                                     const OMStrongReferenceVectorElement& rhs)
{
  TRACE("OMStrongReferenceVectorElement::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }

  OMContainerElement<OMStrongObjectReference>::operator=(rhs);
  _localKey = rhs._localKey;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMStrongReferenceVectorElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise.
bool OMStrongReferenceVectorElement::operator== (
                               const OMStrongReferenceVectorElement& rhs) const
{
  TRACE("OMStrongReferenceVectorElement::operator==");

  bool result = OMContainerElement<OMStrongObjectReference>::operator==(rhs);
  if (result) {
    if (_localKey != rhs._localKey) {
      result = false;
    }
  }
  return result;
}

  // @mfunc Set the value of this <c OMOMStrongReferenceVectorElement>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to previous <p ReferencedObject>, if any.
OMStorable* OMStrongReferenceVectorElement::setValue(const OMStorable* value)
{
  TRACE("OMStrongReferenceVectorElement::setValue");
  OBSOLETE("OMContainerElement<ObjectReference>::reference");

  OMStorable* result = _reference.setValue(value);
  return result;
}

  // @mfunc The local key of this <c OMStrongReferenceVectorElement>.
  //   @rdesc The local key of this <c OMStrongReferenceVectorElement>.
  //   @this const
OMUInt32 OMStrongReferenceVectorElement::localKey(void) const
{
  TRACE("OMStrongReferenceVectorElement::localKey");

  return  _localKey;
}

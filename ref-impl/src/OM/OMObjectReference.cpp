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
#include "OMObjectReference.h"

// class OMObjectReference
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMObjectReference

  // @mfunc Constructor.
OMObjectReference::OMObjectReference(void)
: _property(0),
  _pointer(0)
{
  TRACE("OMObjectReference::OMObjectReference");
  POSTCONDITION("void", isVoid());
}

  // @mfunc Constructor.
  //   @parm The <c OMProperty> that contains this <c OMObjectReference>.
OMObjectReference::OMObjectReference(OMProperty* property)
: _property(property),
  _pointer(0)
{
  TRACE("OMObjectReference::OMObjectReference");

  POSTCONDITION("non-void", !isVoid());
}

  // @mfunc Copy constructor.
  //   @parm The <c OMObjectReference> to copy.
OMObjectReference::OMObjectReference(const OMObjectReference& rhs)
: _property(rhs._property),
  _pointer(rhs._pointer)
{
  TRACE("OMObjectReference::OMObjectReference");
}

  // @mfunc Destructor.
OMObjectReference::~OMObjectReference(void)
{
  TRACE("OMObjectReference::~OMObjectReference");
}

  // @mfunc Is this <c OMObjectReference> void ?
  //   @rdesc True if this <c OMObjectReference> is void,
  //          false otherwise. 
  //   @this const
bool OMObjectReference::isVoid(void) const
{
  bool result;
  if ((_property == 0) && (_pointer == 0)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @parm The <c OMObjectReference> to be assigned.
  //   @rdesc The <c OMObjectReference> resulting from the assignment. 
OMObjectReference& OMObjectReference::operator= (const OMObjectReference& rhs)
{
  TRACE("OMObjectReference::operator=");

  if (*this == rhs) {
    return *this; // early return !
  }
  _property = rhs._property;
  _pointer = rhs._pointer;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide equality of object references.
  //   @parm The <c OMObjectReference> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
  //   @this const
bool OMObjectReference::operator== (const OMObjectReference& rhs) const
{
  bool result;

  if ((_property == rhs._property) && (_pointer == rhs._pointer)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc The value of this <c OMObjectReference> as a pointer.
  //        This function provides low-level access. If the object exits
  //        but has not yet been loaded then the value returned is 0.
  //   @rdesc A pointer to the referenced object, if any, otherwise 0.
  //   @this const
OMStorable* OMObjectReference::pointer(void) const
{
  TRACE("OMObjectReference::pointer");

  return _pointer;
}

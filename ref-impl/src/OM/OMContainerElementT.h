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

// class OMContainerElement<ObjectReference, ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
OMContainerElement<ObjectReference, ReferencedObject>::OMContainerElement(void)
  : _localKey(0), _reference()
{
  TRACE("OMContainerElement<ObjectReference,"
                           "ReferencedObject>::OMContainerElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMProperty> (a container property) that contains this
  //         <c OMContainerElement>.
  //   @parm The name of this <c OMContainerElement>.
  //   @parm The local key of this <c OMContainerElement> within
  //         it's container.
template <typename ObjectReference, typename ReferencedObject>
OMContainerElement<ObjectReference, ReferencedObject>::OMContainerElement(
                                                          OMProperty* property,
                                                          const char* name,
                                                          OMUInt32 localKey)
  : _localKey(localKey), _reference(property, name)
{
  TRACE("OMContainerElement<ObjectReference,"
                           "ReferencedObject>::OMContainerElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMContainerElement> to copy.
template <typename ObjectReference, typename ReferencedObject>
OMContainerElement<ObjectReference, ReferencedObject>::OMContainerElement(
              const OMContainerElement<ObjectReference, ReferencedObject>& rhs)
  : _localKey(rhs._localKey), _reference(rhs._reference)
{
  TRACE("OMContainerElement<ObjectReference,"
                           "ReferencedObject>::OMContainerElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
OMContainerElement<ObjectReference, ReferencedObject>::~OMContainerElement(
                                                                          void)
{
  TRACE("OMContainerElement<ObjectReference,"
                           "ReferencedObject>::~OMContainerElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMContainerElement> to be assigned.
  //   @rdesc The <c OMContainerElement> resulting from the assignment. 
template <typename ObjectReference, typename ReferencedObject>
OMContainerElement<ObjectReference, ReferencedObject>&
OMContainerElement<ObjectReference, ReferencedObject>::operator= (
              const OMContainerElement<ObjectReference, ReferencedObject>& rhs)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  _localKey = rhs._localKey;
  _reference = rhs._reference;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMContainer>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMContainerElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename ObjectReference, typename ReferencedObject>
bool OMContainerElement<ObjectReference, ReferencedObject>::operator== (
        const OMContainerElement<ObjectReference, ReferencedObject>& rhs) const
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::operator==");

  bool result;
  if ((_localKey == rhs._localKey) && (_reference == rhs._reference)) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Save this <c OMContainerElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm Client context for callbacks.
template <typename ObjectReference, typename ReferencedObject>
void OMContainerElement<ObjectReference, ReferencedObject>::save(
                                                           void* clientContext)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::save");

  _reference.save(clientContext);
}

  // @mfunc Close this <c OMContainerElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
void OMContainerElement<ObjectReference, ReferencedObject>::close(void)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::close");

  _reference.close();
}

  // @mfunc Detach this <c OMContainerElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
void OMContainerElement<ObjectReference, ReferencedObject>::detach(void)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::detach");

  _reference.detach();
}

  // @mfunc Restore this <c OMContainerElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
void OMContainerElement<ObjectReference, ReferencedObject>::restore(void)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::restore");

  _reference.restore();
}

  // @mfunc Get the value of this <c OMContainerElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename ObjectReference, typename ReferencedObject>
ReferencedObject*
OMContainerElement<ObjectReference, ReferencedObject>::getValue(void) const
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::getValue");

  return _reference.getValue();
}

  // @mfunc Set the value of this <c OMContainerElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to previous <p ReferencedObject>, if any.
template <typename ObjectReference, typename ReferencedObject>
ReferencedObject*
OMContainerElement<ObjectReference, ReferencedObject>::setValue(
                                                 const ReferencedObject* value)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::setValue");

  return _reference.setValue(value);
}

  // @mfunc The local key of this <c OMContainerElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc The local key of this <c OMContainerElement>.
  //   @this const
template <typename ObjectReference, typename ReferencedObject>
OMUInt32
OMContainerElement<ObjectReference, ReferencedObject>::localKey(void) const
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::localKey");

  return  _localKey;
}

// class OMVectorElement<ObjectReference, ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
OMVectorElement<ObjectReference, ReferencedObject>::OMVectorElement(void)
  : OMContainerElement<ObjectReference, ReferencedObject>()
{
  TRACE("OMVectorElement<ObjectReference, ReferencedObject>::OMVectorElement");
}
  
  // @mfunc Constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMProperty> (a vector property) that contains this
  //         <c OMVectorElement>.
  //   @parm The name of this <c OMVectorElement>.
  //   @parm The local key of this <c OMVectorElement> within
  //         it's vector.
template <typename ObjectReference, typename ReferencedObject>
OMVectorElement<ObjectReference, ReferencedObject>::OMVectorElement(
                                                          OMProperty* property,
                                                          const char* name,
                                                          OMUInt32 localKey)
  : OMContainerElement<ObjectReference,
                       ReferencedObject>(property, name, localKey)
{
  TRACE("OMVectorElement<ObjectReference, ReferencedObject>::OMVectorElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMVectorElement> to copy.
template <typename ObjectReference, typename ReferencedObject>
OMVectorElement<ObjectReference, ReferencedObject>::OMVectorElement(
                 const OMVectorElement<ObjectReference, ReferencedObject>& rhs)
  : OMContainerElement<ObjectReference, ReferencedObject>(rhs)
{
  TRACE("OMVectorElement<ObjectReference, ReferencedObject>::OMVectorElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
OMVectorElement<ObjectReference, ReferencedObject>::~OMVectorElement(void)
{
  TRACE(
       "OMVectorElement<ObjectReference, ReferencedObject>::~OMVectorElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMVectorElement> to be assigned.
  //   @rdesc The <c OMVectorElement> resulting from the assignment. 
template <typename ObjectReference, typename ReferencedObject>
OMVectorElement<ObjectReference, ReferencedObject>&
OMVectorElement<ObjectReference, ReferencedObject>::operator= (
                 const OMVectorElement<ObjectReference, ReferencedObject>& rhs)
{
  TRACE("OMVectorElement<ObjectReference, ReferencedObject>::operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  OMContainerElement<ObjectReference, ReferencedObject>::operator=(rhs);
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMVectorElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename ObjectReference, typename ReferencedObject>
bool OMVectorElement<ObjectReference, ReferencedObject>::operator== (
           const OMVectorElement<ObjectReference, ReferencedObject>& rhs) const
{
  TRACE("OMVectorElement<ObjectReference, ReferencedObject>::operator==");

  bool result =
        OMContainerElement<ObjectReference, ReferencedObject>::operator==(rhs);
  return result;
}

// class OMSetElement<ObjectReference, ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
OMSetElement<ObjectReference, ReferencedObject>::OMSetElement(void)
  : OMContainerElement<ObjectReference, ReferencedObject>(),
    _identification(nullOMUniqueObjectIdentification),
    _referenceCount(1/*tjb*/)
{
  TRACE("OMSetElement<ObjectReference, ReferencedObject>::OMSetElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMProperty> (a set property) that contains this
  //         <c OMSetElement>.
  //   @parm The name of this <c OMSetElement>.
  //   @parm The local key of this <c OMSetElement> within
  //         it's set.
  //   @parm The unique key of this <c OMSetElement>.
template <typename ObjectReference, typename ReferencedObject>
OMSetElement<ObjectReference, ReferencedObject>::OMSetElement(
                                   OMProperty* property,
                                   const char* name,
                                   OMUInt32 localKey,
                                   OMUniqueObjectIdentification identification)
  : OMContainerElement<ObjectReference,
                       ReferencedObject>(property, name, localKey),
    _identification(identification),
    _referenceCount(1/*tjb*/)
{
  TRACE("OMSetElement<ObjectReference, ReferencedObject>::OMSetElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMSetElement> to copy.
template <typename ObjectReference, typename ReferencedObject>
OMSetElement<ObjectReference, ReferencedObject>::OMSetElement(
                    const OMSetElement<ObjectReference, ReferencedObject>& rhs)
  : OMContainerElement<ObjectReference, ReferencedObject>(rhs),
    _identification(rhs._identification),
    _referenceCount(rhs._referenceCount)
{
  TRACE("OMSetElement<ObjectReference, ReferencedObject>::OMSetElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
OMSetElement<ObjectReference, ReferencedObject>::~OMSetElement(void)
{
  TRACE("OMSetElement<ObjectReference, ReferencedObject>::~OMSetElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMSetElement> to be assigned.
  //   @rdesc The <c OMSetElement> resulting from the assignment. 
template <typename ObjectReference, typename ReferencedObject>
OMSetElement<ObjectReference, ReferencedObject>&
OMSetElement<ObjectReference, ReferencedObject>::operator= (
                    const OMSetElement<ObjectReference, ReferencedObject>& rhs)
{
  TRACE("OMSetElement<ObjectReference, ReferencedObject>::operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  OMContainerElement<ObjectReference, ReferencedObject>::operator=(rhs);
  _identification = rhs._identification;
  _referenceCount = rhs._referenceCount;

  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMSetElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename ObjectReference, typename ReferencedObject>
bool OMSetElement<ObjectReference, ReferencedObject>::operator== (
              const OMSetElement<ObjectReference, ReferencedObject>& rhs) const
{
  TRACE("OMSetElement<ObjectReference, ReferencedObject>::operator==");

  bool result;

  if (_identification == rhs._identification) {
    result = true;
  } else {
    result = false;
  }

  ASSERT("Consistent",
                      IMPLIES(result, _referenceCount == rhs._referenceCount));

#if defined (OM_ENABLE_DEBUG)
  bool check =
        OMContainerElement<ObjectReference, ReferencedObject>::operator==(rhs);
#endif
  ASSERT("Consistent", IMPLIES(result, check));

  return result;
}

  // @mfunc The unique key of this <c OMSetElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc  The unique key of this <c OMSetElement>.
template <typename ObjectReference, typename ReferencedObject>
const OMUniqueObjectIdentification
OMSetElement<ObjectReference, ReferencedObject>::identification(void) const
{
  TRACE("OMSetElement<ObjectReference, ReferencedObject>::identification");

  return _identification;
}

  // @mfunc The count of weak references to this <c OMSetElement>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc The count of weak references.
template <typename ObjectReference, typename ReferencedObject>
OMUInt32
OMSetElement<ObjectReference, ReferencedObject>::referenceCount(void) const
{
  TRACE("OMSetElement<ObjectReference, ReferencedObject>::referenceCount");

  return _referenceCount;
}

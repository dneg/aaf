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

// class OMVectorElement<ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>::OMVectorElement(void)
  : _pointer(0)
{
  TRACE("OMVectorElement<ReferencedObject>::OMVectorElement");
}
  
  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>::OMVectorElement(
                                               const ReferencedObject* pointer)
  : _pointer(const_cast<ReferencedObject*>(pointer))
{
  TRACE("OMVectorElement<ReferencedObject>::OMVectorElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMVectorElement> to copy.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>::OMVectorElement(
                                  const OMVectorElement<ReferencedObject>& rhs)
{
  TRACE("OMVectorElement<ReferencedObject>::OMVectorElement");

  _pointer = rhs._pointer;
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>::~OMVectorElement(void)
{
  TRACE("OMVectorElement<ReferencedObject>::~OMVectorElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMVectorElement> to be assigned.
  //   @rdesc The <c OMVectorElement> resulting from the assignment. 
template <typename ReferencedObject>
OMVectorElement<ReferencedObject>&
OMVectorElement<ReferencedObject>::operator= (
                                  const OMVectorElement<ReferencedObject>& rhs)
{
  TRACE("OMVectorElement<ReferencedObject>::operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  _pointer = rhs._pointer;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMVector>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMVectorElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename ReferencedObject>
bool OMVectorElement<ReferencedObject>::operator== (
                            const OMVectorElement<ReferencedObject>& rhs) const
{
  TRACE("OMVectorElement<ReferencedObject>::operator==");

  bool result;
  if (_pointer == rhs._pointer) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Get the value of this <c OMVectorElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @rdesc A pointer to the <p ReferencedObject>.
  //   @this const
template <typename ReferencedObject>
ReferencedObject* OMVectorElement<ReferencedObject>::getValue(void) const
{
  TRACE("OMVectorElement<ReferencedObject>::getValue");

  // No lazy loding for non-persistent objects
  return pointer();
}

  // @mfunc Set the value of this <c OMVectorElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm A pointer to the new <p ReferencedObject>.
  //   @rdesc A pointer to previous <p ReferencedObject>, if any.
template <typename ReferencedObject>
ReferencedObject* OMVectorElement<ReferencedObject>::setValue(
                                                 const ReferencedObject* value)
{
  TRACE("OMVectorElement<ReferencedObject>::setValue");

  ReferencedObject* oldPointer = _pointer;
  _pointer = const_cast<ReferencedObject*>(value);
  return oldPointer;
}

  // @mfunc The value of this <c OMVectorElement> as a pointer.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @rdesc A pointer to the <p ReferencedObject>, if loaded.
  //   @this const
template <typename ReferencedObject>
ReferencedObject* OMVectorElement<ReferencedObject>::pointer(void) const
{
  TRACE("OMVectorElement<ReferencedObject>::pointer");

  return _pointer;
}

// class OMSetElement<UniqueIdentification, ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement(void)
  : OMVectorElement<ReferencedObject>(0)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm A pointer to a <p ReferencedObject>.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement(
                                               const ReferencedObject* pointer)
  : OMVectorElement<ReferencedObject>(pointer)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMSetElement> to copy.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement(
                                     const OMSetElement<UniqueIdentification,
                                                        ReferencedObject>& rhs)
  : OMVectorElement<ReferencedObject>(rhs)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::OMSetElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>::~OMSetElement(void)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::~OMSetElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMSet>.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMSetElement> to be assigned.
  //   @rdesc The <c OMSetElement> resulting from the assignment. 
template <typename UniqueIdentification, typename ReferencedObject>
OMSetElement<UniqueIdentification, ReferencedObject>&
OMSetElement<UniqueIdentification, ReferencedObject>::operator= (
                                     const OMSetElement<UniqueIdentification,
                                                        ReferencedObject>& rhs)
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  OMVectorElement<ReferencedObject>::operator=(rhs);
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMSet>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @parm The <c OMSetElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename UniqueIdentification, typename ReferencedObject>
bool OMSetElement<UniqueIdentification, ReferencedObject>::operator== (
                               const OMSetElement<UniqueIdentification,
                                                  ReferencedObject>& rhs) const
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::operator==");

  bool result = OMVectorElement<ReferencedObject>::operator==(rhs);

  return result;
}

  // @mfunc The unique key of this <c OMSetElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced object.
  //   @rdesc  The unique key of this <c OMSetElement>.
template <typename UniqueIdentification, typename ReferencedObject>
const UniqueIdentification
OMSetElement<UniqueIdentification, ReferencedObject>::identification(
                                                                    void) const
{
  TRACE("OMSetElement<UniqueIdentification, ReferencedObject>::"
                                                             "identification");
  ReferencedObject* object = getValue();
  ASSERT("Valid object", object!= 0);
  return object->identification();
}

// class OMContainerElement<ObjectReference, ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ObjectReference, typename ReferencedObject>
OMContainerElement<ObjectReference, ReferencedObject>::OMContainerElement(void)
  : _reference()
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::"
                                                         "OMContainerElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <p ObjectReference> for this <c OMContainerElement>.
template <typename ObjectReference, typename ReferencedObject>
OMContainerElement<ObjectReference, ReferencedObject>::OMContainerElement(
                                              const ObjectReference& reference)
  : _reference(reference)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::"
                                                         "OMContainerElement");
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
  : _reference(rhs._reference)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::"
                                                         "OMContainerElement");
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
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::"
                                                        "~OMContainerElement");
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
  if (_reference == rhs._reference) {
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
template <typename ObjectReference, typename ReferencedObject>
void OMContainerElement<ObjectReference, ReferencedObject>::save(void)
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::save");

  _reference.save();
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

  // @mfunc The value of this <c OMContainerElement> as a pointer.
  //        This function provides low-level access. If the object exits
  //        but has not yet been loaded then the value returned is 0.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc A pointer to the <p ReferencedObject>, if loaded.
template <typename ObjectReference, typename ReferencedObject>
ReferencedObject*
OMContainerElement<ObjectReference, ReferencedObject>::pointer(void) const
{
  TRACE("OMContainerElement<ObjectReference, ReferencedObject>::pointer");

  return _reference.pointer();
}


// class OMStrongReferenceVectorElement<ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ReferencedObject>
OMStrongReferenceVectorElement<ReferencedObject>::
                                           OMStrongReferenceVectorElement(void)
  : OMContainerElement<OMStrongObjectReference<ReferencedObject>,
                       ReferencedObject>(), _localKey(0)
{
  TRACE("OMStrongReferenceVectorElement<ReferencedObject>::"
                                             "OMStrongReferenceVectorElement");
}
  
  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMProperty> (a vector property) that contains this
  //         <c OMStrongReferenceVectorElement>.
  //   @parm The name of this <c OMStrongReferenceVectorElement>.
  //   @parm The local key of this <c OMStrongReferenceVectorElement> within
  //         it's vector.
template <typename ReferencedObject>
OMStrongReferenceVectorElement<ReferencedObject>::
                                                OMStrongReferenceVectorElement(
                                                          OMProperty* property,
                                                          const char* name,
                                                          OMUInt32 localKey)
  : OMContainerElement<OMStrongObjectReference<ReferencedObject>,
                       ReferencedObject>(
                    OMStrongObjectReference<ReferencedObject>(property, name)),
    _localKey(localKey)
{
  TRACE("OMStrongReferenceVectorElement<ReferencedObject>::"
                                             "OMStrongReferenceVectorElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceVectorElement> to copy.
template <typename ReferencedObject>
OMStrongReferenceVectorElement<ReferencedObject>::
                                                OMStrongReferenceVectorElement(
                   const OMStrongReferenceVectorElement<ReferencedObject>& rhs)
  : OMContainerElement<OMStrongObjectReference<ReferencedObject>,
                       ReferencedObject>(rhs), _localKey(rhs._localKey)
{
  TRACE("OMStrongReferenceVectorElement<ReferencedObject>::"
                                             "OMStrongReferenceVectorElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ReferencedObject>
OMStrongReferenceVectorElement<ReferencedObject>::
                                          ~OMStrongReferenceVectorElement(void)
{
  TRACE("OMStrongReferenceVectorElement<ReferencedObject>::"
                                            "~OMStrongReferenceVectorElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceVectorElement> to be assigned.
  //   @rdesc The <c OMStrongReferenceVectorElement> resulting from
  //          the assignment. 
template <typename ReferencedObject>
OMStrongReferenceVectorElement<ReferencedObject>&
OMStrongReferenceVectorElement<ReferencedObject>::operator= (
                   const OMStrongReferenceVectorElement<ReferencedObject>& rhs)
{
  TRACE("OMStrongReferenceVectorElement<ReferencedObject>::operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  OMContainerElement<OMStrongObjectReference<ReferencedObject>,
                     ReferencedObject>::operator=(rhs);
  _localKey = rhs._localKey;
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceVectorElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename ReferencedObject>
bool OMStrongReferenceVectorElement<ReferencedObject>::operator== (
             const OMStrongReferenceVectorElement<ReferencedObject>& rhs) const
{
  TRACE("OMStrongReferenceVectorElement<ReferencedObject>::operator==");

  bool result = OMContainerElement<OMStrongObjectReference<ReferencedObject>,
                                   ReferencedObject>::operator==(rhs);
  if (result) {
    if (_localKey != rhs._localKey) {
      result = false;
    }
  }
  return result;
}

  // @mfunc The local key of this <c OMStrongReferenceVectorElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc The local key of this <c OMStrongReferenceVectorElement>.
  //   @this const
template <typename ReferencedObject>
OMUInt32
OMStrongReferenceVectorElement<ReferencedObject>::localKey(void) const
{
  TRACE("OMStrongReferenceVectorElement<ReferencedObject>::localKey");

  return  _localKey;
}

// class OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                              OMStrongReferenceSetElement(void)
  : OMStrongReferenceVectorElement<ReferencedObject>(),
    _referenceCount(1/*tjb*/)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                "OMStrongReferenceSetElement");

  memset(&_identification, 0, sizeof(_identification));
}

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMProperty> (a set property) that contains this
  //         <c OMStrongReferenceSetElement>.
  //   @parm The name of this <c OMStrongReferenceSetElement>.
  //   @parm The local key of this <c OMStrongReferenceSetElement> within
  //         it's set.
  //   @parm The unique key of this <c OMStrongReferenceSetElement>.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                   OMStrongReferenceSetElement(
                                   OMProperty* property,
                                   const char* name,
                                   OMUInt32 localKey,
                                   OMUInt32 referenceCount,
                                   UniqueIdentification identification)
  : OMStrongReferenceVectorElement<ReferencedObject>(property, name, localKey),
    _identification(identification),
    _referenceCount(referenceCount)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                "OMStrongReferenceSetElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceSetElement> to copy.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                   OMStrongReferenceSetElement(
                  const OMStrongReferenceSetElement<UniqueIdentification,
                                                    ReferencedObject>& rhs)
  : OMStrongReferenceVectorElement<ReferencedObject>(rhs),
    _identification(rhs._identification),
    _referenceCount(rhs._referenceCount)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                "OMStrongReferenceSetElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                             ~OMStrongReferenceSetElement(void)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                               "~OMStrongReferenceSetElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceSetElement> to be assigned.
  //   @rdesc The <c OMStrongReferenceSetElement> resulting from
  //          the assignment. 
template <typename UniqueIdentification, typename ReferencedObject>
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>&
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                                    operator= (
                  const OMStrongReferenceSetElement<UniqueIdentification,
                                                    ReferencedObject>& rhs)
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                                  "operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  OMStrongReferenceVectorElement<ReferencedObject>::operator=(rhs);
  _identification = rhs._identification;
  _referenceCount = rhs._referenceCount;

  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMStrongReferenceSetElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename UniqueIdentification, typename ReferencedObject>
bool
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                                   operator== (
            const OMStrongReferenceSetElement<UniqueIdentification,
                                              ReferencedObject>& rhs) const
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                                 "operator==");

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
             OMStrongReferenceVectorElement<ReferencedObject>::operator==(rhs);
#endif
  ASSERT("Consistent", IMPLIES(result, check));

  return result;
}

  // @mfunc The unique key of this <c OMStrongReferenceSetElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc  The unique key of this <c OMStrongReferenceSetElement>.
template <typename UniqueIdentification, typename ReferencedObject>
const UniqueIdentification
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                     identification(void) const
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                             "identification");

  return _identification;
}

  // @mfunc The count of weak references to this
  //        <c OMStrongReferenceSetElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc The count of weak references.
template <typename UniqueIdentification, typename ReferencedObject>
OMUInt32
OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::
                                                     referenceCount(void) const
{
  TRACE("OMStrongReferenceSetElement<UniqueIdentification, ReferencedObject>::"
                                                             "referenceCount");

  return _referenceCount;
}

// class OMWeakReferenceVectorElement<ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ReferencedObject>
OMWeakReferenceVectorElement<ReferencedObject>::
                                             OMWeakReferenceVectorElement(void)
  : OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                       ReferencedObject>()
{
  TRACE("OMWeakReferenceVectorElement<ReferencedObject>::"
                                               "OMWeakReferenceVectorElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMProperty> (a set property) that contains this
  //         <c OMWeakReferenceVectorElement>.
  //   @parm The unique key of this <c OMWeakReferenceVectorElement>.
  //   @parm A tag identifying the <c OMStrongReferenceVectorProperty>
  //         in which the target resides.
template <typename ReferencedObject>
OMWeakReferenceVectorElement<ReferencedObject>::OMWeakReferenceVectorElement(
                                   OMProperty* property,
                                   OMUniqueObjectIdentification identification,
                                   OMPropertyTag targetTag)
  : OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                       ReferencedObject>(
  OMWeakObjectReference<ReferencedObject>(property, identification, targetTag))
{
  TRACE("OMWeakReferenceVectorElement<ReferencedObject>::"
                                               "OMWeakReferenceVectorElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMWeakReferenceVectorElement> to copy.
template <typename ReferencedObject>
OMWeakReferenceVectorElement<ReferencedObject>::OMWeakReferenceVectorElement(
                    const OMWeakReferenceVectorElement<ReferencedObject>& rhs)
  : OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                       ReferencedObject>(rhs)
{
  TRACE("OMWeakReferenceVectorElement<ReferencedObject>::"
                                               "OMWeakReferenceVectorElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ReferencedObject>
OMWeakReferenceVectorElement<ReferencedObject>::
                                            ~OMWeakReferenceVectorElement(void)
{
  TRACE("OMWeakReferenceVectorElement<ReferencedObject>::"
                                              "~OMWeakReferenceVectorElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMWeakReferenceVectorElement> to be assigned.
  //   @rdesc The <c OMWeakReferenceVectorElement> resulting from
  //          the assignment. 
template <typename ReferencedObject>
OMWeakReferenceVectorElement<ReferencedObject>&
OMWeakReferenceVectorElement<ReferencedObject>::operator= (
                     const OMWeakReferenceVectorElement<ReferencedObject>& rhs)
{
  TRACE("OMWeakReferenceVectorElement<ReferencedObject>::operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                     ReferencedObject>::operator=(rhs);
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMVector>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMWeakReferenceVectorElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename ReferencedObject>
bool OMWeakReferenceVectorElement<ReferencedObject>::operator== (
               const OMWeakReferenceVectorElement<ReferencedObject>& rhs) const
{
  TRACE("OMWeakReferenceVectorElement<ReferencedObject>::operator==");

  bool result = OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                                   ReferencedObject>::operator==(rhs);

  return result;
}

  // @mfunc The unique key of this <c OMWeakReferenceVectorElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc  The unique key of this <c OMWeakReferenceVectorElement>.
template <typename ReferencedObject>
const OMUniqueObjectIdentification
OMWeakReferenceVectorElement<ReferencedObject>::identification(void) const
{
  TRACE("OMWeakReferenceVectorElement<ReferencedObject>::identification");

  return _reference.identification();
}

// class OMWeakReferenceSetElement<ReferencedObject>

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ReferencedObject>
OMWeakReferenceSetElement<ReferencedObject>::OMWeakReferenceSetElement(void)
  : OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                       ReferencedObject>()
{
  TRACE("OMWeakReferenceSetElement<ReferencedObject>::"
                                                  "OMWeakReferenceSetElement");
}

  // @mfunc Constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMProperty> (a set property) that contains this
  //         <c OMWeakReferenceSetElement>.
  //   @parm The unique key of this <c OMWeakReferenceSetElement>.
  //   @parm A tag identifying the <c OMStrongReferenceSetProperty>
  //         in which the target resides.
template <typename ReferencedObject>
OMWeakReferenceSetElement<ReferencedObject>::OMWeakReferenceSetElement(
                                   OMProperty* property,
                                   OMUniqueObjectIdentification identification,
                                   OMPropertyTag targetTag)
  : OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                       ReferencedObject>(
  OMWeakObjectReference<ReferencedObject>(property, identification, targetTag))
{
  TRACE("OMWeakReferenceSetElement<ReferencedObject>::"
                                                  "OMWeakReferenceSetElement");
}

  // @mfunc Copy constructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMWeakReferenceSetElement> to copy.
template <typename ReferencedObject>
OMWeakReferenceSetElement<ReferencedObject>::OMWeakReferenceSetElement(
                    const OMWeakReferenceSetElement<ReferencedObject>& rhs)
  : OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                       ReferencedObject>(rhs)
{
  TRACE("OMWeakReferenceSetElement<ReferencedObject>::"
                                                  "OMWeakReferenceSetElement");
}

  // @mfunc Destructor.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
template <typename ReferencedObject>
OMWeakReferenceSetElement<ReferencedObject>::~OMWeakReferenceSetElement(void)
{
  TRACE("OMWeakReferenceSetElement<ReferencedObject>::"
                                                 "~OMWeakReferenceSetElement");
}

  // @mfunc Assignment.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide assignment of object references.
  //   @tcarg class | ObjectReference  | The type of the contained object
  //          reference 
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMWeakReferenceSetElement> to be assigned.
  //   @rdesc The <c OMWeakReferenceSetElement> resulting from
  //          the assignment. 
template <typename ReferencedObject>
OMWeakReferenceSetElement<ReferencedObject>&
OMWeakReferenceSetElement<ReferencedObject>::operator= (
                        const OMWeakReferenceSetElement<ReferencedObject>& rhs)
{
  TRACE("OMWeakReferenceSetElement<ReferencedObject>::operator=");

  if (*this == rhs) {
	return *this; // early return !
  }

  OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                     ReferencedObject>::operator=(rhs);
  return *this;
}

  // @mfunc Equality.
  //        This operator provides value semantics for <c OMSet>.
  //        This operator does not provide equality of object references.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @parm The <c OMWeakReferenceSetElement> to be compared.
  //   @rdesc True if the values are the same, false otherwise. 
template <typename ReferencedObject>
bool OMWeakReferenceSetElement<ReferencedObject>::operator== (
                const OMWeakReferenceSetElement<ReferencedObject>& rhs) const
{
  TRACE("OMWeakReferenceSetElement<ReferencedObject>::operator==");

  bool result = OMContainerElement<OMWeakObjectReference<ReferencedObject>,
                                   ReferencedObject>::operator==(rhs);

  return result;
}

  // @mfunc The unique key of this <c OMWeakReferenceSetElement>.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          object. This type must be a descendant of <c OMStorable>.
  //   @rdesc  The unique key of this <c OMWeakReferenceSetElement>.
template <typename ReferencedObject>
const OMUniqueObjectIdentification
OMWeakReferenceSetElement<ReferencedObject>::identification(void) const
{
  TRACE("OMWeakReferenceSetElement<ReferencedObject>::identification");

  return _reference.identification();
}

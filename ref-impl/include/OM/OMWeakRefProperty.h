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

// @doc OMEXTERNAL
#ifndef OMWEAKREFPROPERTY_H
#define OMWEAKREFPROPERTY_H

#include "OMDataTypes.h"
#include "OMProperty.h"
#include "OMObjectReference.h"

  // @class Persistent weak reference (pointer to shared object)
  //        properties supported by the Object Manager.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (pointed to) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMReferenceProperty>
template <typename ReferencedObject>
class OMWeakReferenceProperty : public OMReferenceProperty<ReferencedObject> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWeakReferenceProperty(const OMPropertyId propertyId,
                          const wchar_t* name,
                          const wchar_t* targetName,
                          const OMPropertyId keyPropertyId);

    // @cmember Destructor.
  virtual ~OMWeakReferenceProperty(void);

    // @cmember Get the value of this <c OMWeakReferenceProperty>.
  virtual void getValue(ReferencedObject*& object) const;

    // @cmember set the value of this <c OMWeakReferenceProperty>.
  virtual ReferencedObject* setValue(const ReferencedObject* object);

    // @cmember Assignment operator.
  OMWeakReferenceProperty<ReferencedObject>& operator =
                                              (const ReferencedObject* value);

    // @cmember Dereference operator.
  ReferencedObject* operator -> (void);

    // @cmember Dereference operator.
  const ReferencedObject* operator -> (void) const;

    // @cmember Type conversion. Convert an
    //          <c OMWeakReferenceProperty> into a pointer to the
    //          referenced (pointed to) <p ReferencedObject>.
  operator ReferencedObject*() const;

    // @cmember Save this <c OMWeakReferenceProperty>.
  virtual void save(void) const;

    // @cmember close this <c OMWeakReferenceProperty>.
  virtual void close(void);

    // @cmember Restore this <c OMWeakReferenceProperty>, the external
    //          (persisted) size of the <c OMWeakReferenceProperty> is
    //          <p externalSize>.
  virtual void restore(size_t externalSize);

    // @cmember Is this <c OMWeakReferenceProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Get the raw bits of this <c OMWeakReferenceProperty>. The
    //          raw bits are copied to the buffer at address <p bits>
    //          which is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this <c OMWeakReferenceProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

private:

  OMPropertyTag targetTag(void) const;

  OMPropertyId* targetPropertyPath(void) const;

  void clearTargetTag(void) const;

  OMWeakObjectReference<ReferencedObject> _reference;
  OMPropertyTag _targetTag;
  const wchar_t* _targetName;
  OMPropertyId* _targetPropertyPath;
  OMPropertyId _keyPropertyId;

};

#include "OMWeakRefPropertyT.h"

#endif

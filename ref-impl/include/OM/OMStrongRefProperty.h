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
#ifndef OMSTRONGREFPROPERTY_H
#define OMSTRONGREFPROPERTY_H

#include "OMDataTypes.h"
#include "OMProperty.h"
#include "OMRefProperty.h"
#include "OMObjectReference.h"

  // @class Persistent strong reference (contained object)
  //        properties supported by the Object Manager.
  //   @tcarg class | ReferencedObject | The type of the referenced
  //          (contained) object. This type must be a descendant of
  //          <c OMStorable>.
  //   @base public | <c OMReferenceProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
template <typename ReferencedObject>
class OMStrongReferenceProperty : public OMReferenceProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStrongReferenceProperty(const OMPropertyId propertyId,
                            const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMStrongReferenceProperty(void);

    // @cmember Get the value of this <c OMStrongReferenceProperty>.
  virtual void getValue(ReferencedObject*& object) const;

    // @cmember Set the value of this <c OMStrongReferenceProperty>.
  virtual ReferencedObject* setValue(const ReferencedObject* object);

    // @cmember Clear the value of this <c OMStrongReferenceProperty>.
  virtual ReferencedObject* clearValue(void);

    // @cmember Assignment operator.
  OMStrongReferenceProperty<ReferencedObject>& operator =
                                              (const ReferencedObject* value);

    // @cmember Dereference operator.
  ReferencedObject* operator -> (void);

    // @cmember Dereference operator.
  const ReferencedObject* operator -> (void) const;

    // @cmember Type conversion. Convert an
    //          <c OMStrongReferenceProperty> into a pointer to the
    //          referenced (contained) <p ReferencedObject>.
  operator ReferencedObject*() const;

    // @cmember Save this <c OMStrongReferenceProperty>.
  virtual void save(void) const;

    // @cmember Close this <c OMProperty>.
  virtual void close(void);

    // @cmember Detach this <c OMProperty>.
  virtual void detach(void);

    // @cmember Restore this <c OMStrongReferenceProperty>, the external
    //          (persisted) size of the <c OMStrongReferenceProperty>
    //          is <p externalSize>.
  virtual void restore(size_t externalSize);

  // Optional property interface

    // @cmember Is this <c OMStrongReferenceProperty> void ?
  virtual bool isVoid(void) const;

    // @cmember Remove this optional <c OMStrongReferenceProperty>.
  virtual void removeProperty(void);

    // @cmember Get the raw bits of this <c OMStrongReferenceProperty>. The
    //          raw bits are copied to the buffer at address <p bits>
    //          which is <p size> bytes in size.
  virtual void getBits(OMByte* bits, size_t size) const;

    // @cmember Set the raw bits of this <c OMStrongReferenceProperty>. The raw
    //          bits are copied from the buffer at address <p bits> which
    //          is <p size> bytes in size.
  virtual void setBits(const OMByte* bits, size_t size);

    // @cmember Get the value of this <c OMStrongReferenceProperty>.
  virtual OMObject* getObject(void) const;

    // @cmember set the value of this <c OMStrongReferenceProperty>.
  virtual OMObject* setObject(const OMObject* object);

    // @cmember The value of this <c OMStrongReferenceProperty>
    //          as an <c OMStorable>.
  virtual OMStorable* storable(void) const;

private:

  OMStrongObjectReference<ReferencedObject> _reference;

};

#include "OMStrongRefPropertyT.h"

#endif

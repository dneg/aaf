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
#ifndef OMREFVECTORPROPERTY_H
#define OMREFVECTORPROPERTY_H

#include "OMContainerProperty.h"

  // @class Abstract base class for persistent object reference vector
  //        properties supported by the Object Manager.
  //   @base public | <c OMContainerProperty>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMReferenceVectorProperty : public OMContainerProperty {
public:
  // @access Public members.

    // @cmember Constructor.
  OMReferenceVectorProperty(const OMPropertyId propertyId,
                            const OMStoredForm storedForm,
                            const wchar_t* name);

    // @cmember Destructor.
  virtual ~OMReferenceVectorProperty(void);

    // @cmember Set the value of this <c OMReferenceVectorProperty>
    //          at position <p index> to <p object>.
  virtual OMObject* setObjectAt(const OMObject* object,
                                const size_t index) = 0;

    // @cmember The value of this <c OMReferenceVectorProperty>
    //          at position <p index>.
  virtual OMObject* getObjectAt(const size_t index) const = 0;

    // @cmember Append the given <p OMObject> <p object> to
    //          this <c OMReferenceVectorProperty>.
  virtual void appendObject(const OMObject* object) = 0;

    // @cmember Prepend the given <p OMObject> <p object> to
    //          this <c OMReferenceVectorProperty>.
  virtual void prependObject(const OMObject* object) = 0;

    // @cmember Remove the object from this
    //          <c OMReferenceVectorProperty> at position <p index>.
    //          Existing objects in this <c OMReferenceVectorProperty>
    //          at <p index> + 1 and higher are shifted down one index
    //          position.
  virtual OMObject* removeObjectAt(const size_t index) = 0;

    // @cmember Insert <p object> into this <c OMReferenceVectorProperty>
    //          at position <p index>. Existing objects at <p index> and
    //          higher are shifted up one index position.
  virtual void insertObjectAt(const OMObject* object, const size_t index) = 0;

};

#endif


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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMContainerProperty

#include "OMContainerProperty.h"

#include "OMAssertions.h"
#include "OMStoredObject.h"

#include <stdio.h>

  // @mfunc Constructor.
  //   @parm The property id.
  //   @parm The stored form of this property.
  //   @parm The name of this property.
OMContainerProperty::OMContainerProperty(const OMPropertyId propertyId,
                                         const OMStoredForm storedForm,
                                         const wchar_t* name)
: OMProperty(propertyId, storedForm, name), _localKey(0)
{
  TRACE("OMContainerProperty::OMContainerProperty");
}

  // @mfunc Destructor.
OMContainerProperty::~OMContainerProperty(void)
{
  TRACE("OMContainerProperty::~OMContainerProperty");
}

  // @mfunc Compute the name of an element in this <c OMContainer>
  //        given the element's <p localKey>.
  //   @parm The element's local key.
wchar_t* OMContainerProperty::elementName(OMUInt32 localKey)
{
  TRACE("OMContainerProperty::elementName");

  wchar_t* name = OMStoredObject::elementName(_name, propertyId(), localKey);
  return name;
}

  // @mfunc Obtain the next available local key.
  //   @rdesc The next available local key.
OMUInt32 OMContainerProperty::nextLocalKey(void)
{
  return _localKey++;
}

  // @mfunc The current local key.
  //   @rdesc The current local key.
  //   @this const

OMUInt32 OMContainerProperty::localKey(void) const
{
  return _localKey;
}

  // @mfunc Set the current local key. Used on restore to restart
  //        local key assignment.
  //   @parm The new local key.
void OMContainerProperty::setLocalKey(OMUInt32 newLocalKey)
{
  _localKey = newLocalKey;
}

const wchar_t* OMContainerProperty::storedName(void) const
{
  TRACE("OMContainerProperty::storedName");

  if (_storedName == 0) {
    OMContainerProperty* p = const_cast<OMContainerProperty*>(this);
    p->_storedName = OMStoredObject::collectionName(_name, propertyId());
  }
  return _storedName;
}

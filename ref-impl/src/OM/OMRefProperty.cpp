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

#include "OMRefProperty.h"

#include "OMAssertions.h"

OMReferenceProperty::OMReferenceProperty(const OMPropertyId propertyId,
                                         const OMStoredForm storedForm,
                                         const wchar_t* name)
: OMProperty(propertyId, storedForm, name)
{
  TRACE("OMReferenceProperty::OMReferenceProperty");
}

OMReferenceProperty::~OMReferenceProperty(void)
{
  TRACE("OMReferenceProperty::~OMReferenceProperty");
}

  // @mfunc The size of the raw bits of this <c OMReferenceProperty>. The
  //        size is given in bytes.
  //   @rdesc The size of the raw bits of this <c OMReferenceProperty>
  //          in bytes.
  //   @this const
size_t OMReferenceProperty::bitsSize(void) const
{
  TRACE("OMReferenceProperty::bitsSize");
  OBSOLETE("other methods on class OMReferenceProperty");

  return sizeof(OMStorable*);
}

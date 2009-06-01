//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFGetFileBits_h__
#include "ImplAAFGetFileBits.h"
#endif

#include "OMAssertions.h"
#include <string.h>


ImplAAFGetFileBits::ImplAAFGetFileBits ()
  : _rep (0)
{}


ImplAAFGetFileBits::~ImplAAFGetFileBits ()
{}

void ImplAAFGetFileBits::Initialize
        (OMRawStorage * prs)
{
  ASSERTU (prs);
  ASSERTU (!_rep);
  _rep = prs;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFGetFileBits::ReadAt (
      aafMemPtr_t  buf,
      aafUInt32  bufSize,
      aafUInt64  position)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  if ( ! buf)
	return AAFRESULT_NULL_PARAM;

  if ((bufSize + position) > _rep->size())
	return AAFRESULT_OFFSET_SIZE;

  aafUInt32 bytesRead;
  _rep->readAt (position, buf, bufSize, bytesRead);
  ASSERTU (bytesRead == bufSize);

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFGetFileBits::GetSize (
      aafUInt64 * pSize)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  if ( ! pSize)
	return AAFRESULT_NULL_PARAM;

  *pSize = _rep->size ();
  return AAFRESULT_SUCCESS;
}

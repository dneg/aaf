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

#ifndef __ImplAAFSetFileBits_h__
#include "ImplAAFSetFileBits.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFSetFileBits::ImplAAFSetFileBits ()
  : _rep (0)
{}


ImplAAFSetFileBits::~ImplAAFSetFileBits ()
{}

void ImplAAFSetFileBits::Initialize
        (OMRawStorage * prs)
{
  assert (prs);
  assert (!_rep);
  _rep = prs;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSetFileBits::WriteAt (
      aafMemPtr_t  buf,
      aafUInt32  bufSize,
      aafUInt64  position)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  if ( ! buf)
	return AAFRESULT_NULL_PARAM;

  aafUInt64 s = _rep->extent ();
  if (s < (position + bufSize))
	{
	  AAFRESULT hr;
	  hr = SetSize (position + bufSize);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	}

  aafUInt32 bytesWritten;
  _rep->writeAt (position, buf, bufSize, bytesWritten);
  if (bytesWritten != bufSize)
	return AAFRESULT_INVALID_PARAM;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFSetFileBits::SetSize (
      aafUInt64 size)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  // BobT Hack!  Some implementations may not understand 64-bit
  // integer sizes, so we'll only allow 32-bit sizes...
  if (size > ((unsigned long) ~0))
	return AAFRESULT_DATA_SIZE;

  _rep->extend (size);
  aafUInt64 s = _rep->extent ();
  if (s < size)
	return AAFRESULT_DATA_SIZE;
  else
	return AAFRESULT_SUCCESS;
}

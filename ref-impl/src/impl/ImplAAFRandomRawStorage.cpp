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

#ifndef __ImplAAFRandomRawStorage_h__
#include "ImplAAFRandomRawStorage.h"
#endif

#include "OMRawStorage.h"

#include <assert.h>
#include <string.h>


ImplAAFRandomRawStorage::ImplAAFRandomRawStorage ()
{}


ImplAAFRandomRawStorage::~ImplAAFRandomRawStorage ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomRawStorage::ReadAt (
      aafUInt64  position,
	  aafMemPtr_t  buf,
	  aafUInt32  bufSize,
	  aafUInt32 *  pNumRead)
{
  if (!GetOMStorage())
	return AAFRESULT_NOT_INITIALIZED;

  if (! buf)
	return AAFRESULT_NULL_PARAM;

  if (! pNumRead)
	return AAFRESULT_NULL_PARAM;

  if (! GetOMStorage()->isReadable())
	return AAFRESULT_NOT_READABLE;

  // Creation if ImplAAFRandomRawStorages in ImplAAFModule.cpp
  // guarantees that this storage is positionable.
  assert (GetOMStorage()->isPositionable ());

  GetOMStorage()->readAt (position, buf, bufSize, *pNumRead);
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomRawStorage::WriteAt (
      aafUInt64  position,
	  aafMemConstPtr_t  buf,
	  aafUInt32  bufSize,
	  aafUInt32 *  pNumWritten)
{
  if (!GetOMStorage())
	return AAFRESULT_NOT_INITIALIZED;

  if (! buf)
	return AAFRESULT_NULL_PARAM;

  if (! pNumWritten)
	return AAFRESULT_NULL_PARAM;

  if (! GetOMStorage()->isWritable())
	return AAFRESULT_NOT_WRITEABLE;

  // Creation if ImplAAFRandomRawStorages in ImplAAFModule.cpp
  // guarantees that this storage is positionable.
  assert (GetOMStorage()->isPositionable ());

  if (GetOMStorage()->extent() < (position + bufSize))
	{
	  GetOMStorage()->extend (position + bufSize);
	  if (GetOMStorage()->extent() < (position + bufSize))
		return AAFRESULT_SMALLBUF;
	}

  GetOMStorage()->writeAt (position, buf, bufSize, *pNumWritten);
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomRawStorage::GetSize (
      aafUInt64 * pSize)
{
  if (!GetOMStorage())
	return AAFRESULT_NOT_INITIALIZED;

  if (! pSize)
	return AAFRESULT_NULL_PARAM;
  
  // Creation if ImplAAFRandomRawStorages in ImplAAFModule.cpp
  // guarantees that this storage is positionable.
  assert (GetOMStorage()->isPositionable ());

  assert (pSize);
  *pSize = GetOMStorage()->size();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomRawStorage::IsExtendable (
      aafBoolean_t * pResult)
{
  if (!GetOMStorage())
	return AAFRESULT_NOT_INITIALIZED;

  if (! pResult)
	return AAFRESULT_NULL_PARAM;

  *pResult = GetOMStorage()->isExtendible () ?
	kAAFTrue : kAAFFalse;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomRawStorage::GetExtent (
      aafUInt64 * pResult)
{
  if (!GetOMStorage())
	return AAFRESULT_NOT_INITIALIZED;

  if (! pResult)
	return AAFRESULT_NULL_PARAM;

  // Creation if ImplAAFRandomRawStorages in ImplAAFModule.cpp
  // guarantees that this storage is positionable.
  assert (GetOMStorage()->isPositionable ());

  *pResult = GetOMStorage()->extent ();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomRawStorage::SetExtent (
      aafUInt64 extent)
{
  if (!GetOMStorage())
	return AAFRESULT_NOT_INITIALIZED;

  if (! GetOMStorage()->isExtendible ())
	return AAFRESULT_OPERATION_NOT_PERMITTED;

  // BobT Hack!  Some implementations may not understand 64-bit
  // integer sizes, so we'll only allow 32-bit sizes...
  if (extent > ((unsigned long) ~0))
	return AAFRESULT_DATA_SIZE;

  GetOMStorage()->extend (extent);
  // See if extend() succeded
  if (GetOMStorage()->extent() < extent)
	return AAFRESULT_SMALLBUF;

  return AAFRESULT_SUCCESS;
}

//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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

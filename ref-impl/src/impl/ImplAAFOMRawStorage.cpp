//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"
#include "AAFResult.h"

#include <assert.h>

#include "ImplAAFOMRawStorage.h"

#include "OMExceptions.h"

ImplAAFOMRawStorage::ImplAAFOMRawStorage (IAAFRawStorage * rep)
  : _rep (rep),
    _randRep (0),
    _position(0)
{
  assert (rep);
  _rep->AddRef ();

  AAFRESULT hr;
  hr = _rep->QueryInterface(IID_IAAFRandomRawStorage, (void **)&_randRep);
  if (AAFRESULT_FAILED (hr))
	_randRep = 0;
}


ImplAAFOMRawStorage::~ImplAAFOMRawStorage ()
{
  assert (_rep);
  _rep->Release ();
  _rep = 0;

  if (_randRep)
	{
	  _randRep->Release ();
	  _randRep = 0;
	}
}


bool ImplAAFOMRawStorage::isReadable(void) const
{
  assert (_rep);
  aafBoolean_t r;
  AAFRESULT hr;
  hr = _rep->IsReadable (&r);
  assert (AAFRESULT_SUCCEEDED (hr));
  return r ? true : false;
}


void ImplAAFOMRawStorage::read(OMByte* bytes,
							   OMUInt32 byteCount,
							   OMUInt32& bytesRead) const
{
  assert (_rep);
  AAFRESULT hr;

  // If raw storage is positionable (is IAAFRandomRawStorage) read bytes
  // at the position specified by the _position member. Otherwise read
  // at the current storage position which in this case always matches
  // the _position value.
  if (_randRep)
  {
    hr = _randRep->ReadAt (_position, bytes, byteCount, &bytesRead);
  }
  else
  {
    hr = _rep->Read (bytes, byteCount, &bytesRead);
  }
  assert (AAFRESULT_SUCCEEDED (hr));

  ImplAAFOMRawStorage* nonConstThis = const_cast<ImplAAFOMRawStorage*>(this);
  nonConstThis->_position += bytesRead;
}


void ImplAAFOMRawStorage::readAt(OMUInt64 position,
								 OMByte* bytes,
								 OMUInt32 byteCount,
								 OMUInt32& bytesRead) const
{
  assert (bytes);
  assert (_randRep);
  AAFRESULT hr;
  hr = _randRep->ReadAt (position, bytes, byteCount, &bytesRead);
  assert (AAFRESULT_SUCCEEDED (hr));

  ImplAAFOMRawStorage* nonConstThis = const_cast<ImplAAFOMRawStorage*>(this);
  nonConstThis->_position = position + bytesRead;
}


bool ImplAAFOMRawStorage::isWritable(void) const
{
  assert (_rep);
  aafBoolean_t r;
  AAFRESULT hr;
  hr = _rep->IsWriteable (&r);
  assert (AAFRESULT_SUCCEEDED (hr));
  return r ? true : false;
}


void ImplAAFOMRawStorage::write(const OMByte* bytes,
								OMUInt32 byteCount,
								OMUInt32& bytesWritten)
{
  assert (_rep);
  AAFRESULT hr;

  // If raw storage is positionable (is IAAFRandomRawStorage) write bytes
  // at the position specified by the _position member. Otherwise write
  // at the current storage position which in this case always matches
  // the _position value.
  if (_randRep)
  {
    hr = _randRep->WriteAt (_position, bytes, byteCount, &bytesWritten);
  }
  else
  {
    hr = _rep->Write (bytes, byteCount, &bytesWritten);
  }
  assert (AAFRESULT_SUCCEEDED (hr));
  _position += bytesWritten;
}


void ImplAAFOMRawStorage::writeAt(OMUInt64 position,
								  const OMByte* bytes,
								  OMUInt32 byteCount,
								  OMUInt32& bytesWritten)
{
  assert (bytes);
  assert (_randRep);
  AAFRESULT hr;
  hr = _randRep->WriteAt (position, bytes, byteCount, &bytesWritten);
  assert (AAFRESULT_SUCCEEDED (hr));
  _position = position + bytesWritten;
}


bool ImplAAFOMRawStorage::isExtendible(void) const
{
  assert (_rep);

  // If not an AAFRandomRawStorage, it's definitely not extendible.
  if (! _randRep)
	return false;

  AAFRESULT hr;
  aafBoolean_t r;
  hr = _randRep->IsExtendable (&r);
  assert (AAFRESULT_SUCCEEDED (hr));
  return r ? true : false;
}


OMUInt64 ImplAAFOMRawStorage::extent(void) const
{
  assert (_randRep);
  AAFRESULT hr;
  aafUInt64 result;
  hr = _randRep->GetExtent (&result);
  assert (AAFRESULT_SUCCEEDED (hr));
  return result;
}


void ImplAAFOMRawStorage::extend(OMUInt64 newSize)
{
  assert (_randRep);
  AAFRESULT hr;
  hr = _randRep->SetExtent (newSize);
  assert (AAFRESULT_SUCCEEDED (hr));
}


OMUInt64 ImplAAFOMRawStorage::size(void) const
{
  assert (_randRep);
  AAFRESULT hr;
  OMUInt64 result;
  hr = _randRep->GetSize (&result);
  assert (AAFRESULT_SUCCEEDED (hr));
  return result;
}


bool ImplAAFOMRawStorage::isPositionable(void) const
{
  assert (_rep);
  // If not an AAFRandomRawStorage, it's not positionable.
  if (_randRep)
	return true;
  else
	return false;
}


OMUInt64 ImplAAFOMRawStorage::position(void) const
{
  assert (_rep);
  return _position;
}


void ImplAAFOMRawStorage::setPosition(OMUInt64 newPosition) const
{
  assert (_randRep);

  // The _position value is used by read() and write() methods to
  // adjust the storage current position before reading or writing.
  ImplAAFOMRawStorage* nonConstThis = const_cast<ImplAAFOMRawStorage*>(this);
  nonConstThis->_position = newPosition;
}


void ImplAAFOMRawStorage::synchronize(void)
{
  assert (_rep);
  AAFRESULT hr;
  hr = _rep->Synchronize ();
  assert (AAFRESULT_SUCCEEDED (hr));
}

// ImplAAFOMCachedRawStorage methods

ImplAAFOMCachedRawStorage::ImplAAFOMCachedRawStorage(IAAFRawStorage* rep,
                                                     aafUInt32  pageCount,
                                                     aafUInt32  pageSize,
                                                     OMCachePageAllocator* allocator )
: OMBaseCachedDiskRawStorage(pageSize, pageCount, getRawStorageSize(rep), allocator),
  _rep(rep),
  _randRep(0)
{
  assert (rep);
  _rep->AddRef ();

  AAFRESULT hr;
  hr = _rep->QueryInterface(IID_IAAFRandomRawStorage, (void **)&_randRep);
  if (AAFRESULT_FAILED (hr))
	_randRep = 0;
}

ImplAAFOMCachedRawStorage::~ImplAAFOMCachedRawStorage()
{
  assert (_rep);
  _rep->Release ();
  _rep = 0;

  if (_randRep)
	{
	  _randRep->Release ();
	  _randRep = 0;
	}
}

bool ImplAAFOMCachedRawStorage::isReadable(void) const
{
  assert (_rep);
  aafBoolean_t r;
  AAFRESULT hr;
  hr = _rep->IsReadable (&r);
  if (AAFRESULT_FAILED (hr)) throw OMException(hr);
  return r ? true : false;
}

bool ImplAAFOMCachedRawStorage::isWritable(void) const
{
  assert (_rep);
  aafBoolean_t r;
  AAFRESULT hr;
  hr = _rep->IsWriteable (&r);
  if (AAFRESULT_FAILED (hr)) throw OMException(hr);
  return r ? true : false;
}

bool ImplAAFOMCachedRawStorage::isExtendible(void) const
{
  assert (_rep);

  // If not an AAFRandomRawStorage, it's definitely not extendible.
  if (! _randRep)
	return false;

  AAFRESULT hr;
  aafBoolean_t r;
  hr = _randRep->IsExtendable (&r);
  if (AAFRESULT_FAILED (hr)) throw OMException(hr);
  return r ? true : false;
}

bool ImplAAFOMCachedRawStorage::isPositionable(void) const
{
  assert (_rep);
  // If not an AAFRandomRawStorage, it's not positionable.
  if (_randRep)
	return true;
  else
	return false;
}

void ImplAAFOMCachedRawStorage::synchronize(void)
{
  if (isWritable()) {
    flush();
  }
  assert (_rep);
  AAFRESULT hr;
  hr = _rep->Synchronize ();
  if (AAFRESULT_FAILED (hr)) throw OMException(hr);
}

void ImplAAFOMCachedRawStorage::rawReadAt(OMUInt64 position,
                                          OMUInt32 byteCount,
                                          OMByte* destination)
{
  assert(_randRep);
  assert(byteCount != 0);

  OMUInt32 bytesRead = 0;
  AAFRESULT hr = _randRep->ReadAt(position, destination, byteCount, &bytesRead);
  if (AAFRESULT_FAILED(hr) || (bytesRead != byteCount)) {
    throw OMException(hr);
  }
}


void ImplAAFOMCachedRawStorage::rawWriteAt(OMUInt64 position,
                                           OMUInt32 byteCount,
                                           const OMByte* source)
{
  assert(_randRep);
  assert(byteCount != 0);

  OMUInt32 bytesWritten = 0;
  AAFRESULT hr = _randRep->WriteAt(position, source, byteCount, &bytesWritten);
  if (AAFRESULT_FAILED(hr) || (bytesWritten != byteCount)) {
    throw OMException(hr);
  }
}


/*static*/ aafUInt64 ImplAAFOMCachedRawStorage::getRawStorageSize(
    IAAFRawStorage* pRawStorage )
{
  assert (pRawStorage);

  aafUInt64 rawStorageSize = 0;

  IAAFRandomRawStorage* pRandomRawStorage = 0;
  AAFRESULT hr = pRawStorage->QueryInterface(IID_IAAFRandomRawStorage,
                                             (void **)&pRandomRawStorage);

  if (pRandomRawStorage)
  {
    pRandomRawStorage->GetSize(&rawStorageSize);

    pRandomRawStorage->Release();
    pRandomRawStorage = 0;
  }


  return rawStorageSize;
}

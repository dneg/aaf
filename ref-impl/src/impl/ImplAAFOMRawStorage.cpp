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

#include "AAF.h"
#include "AAFResult.h"

#include "OMAssertions.h"

#include "ImplAAFOMRawStorage.h"

#include "OMExceptions.h"

ImplAAFOMRawStorage::ImplAAFOMRawStorage (IAAFRawStorage * rep)
  : _rep (rep),
    _randRep (0),
    _position(0)
{
  ASSERTU (rep);
  _rep->AddRef ();

  AAFRESULT hr;
  hr = _rep->QueryInterface(IID_IAAFRandomRawStorage, (void **)&_randRep);
  if (AAFRESULT_FAILED (hr))
	_randRep = 0;
}


ImplAAFOMRawStorage::~ImplAAFOMRawStorage ()
{
  ASSERTU (_rep);
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
  ASSERTU (_rep);
  aafBoolean_t r;
  AAFRESULT hr;
  hr = _rep->IsReadable (&r);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  return r ? true : false;
}


void ImplAAFOMRawStorage::read(OMByte* bytes,
							   OMUInt32 byteCount,
							   OMUInt32& bytesRead) const
{
  ASSERTU (_rep);
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
  ASSERTU (AAFRESULT_SUCCEEDED (hr));

  ImplAAFOMRawStorage* nonConstThis = const_cast<ImplAAFOMRawStorage*>(this);
  nonConstThis->_position += bytesRead;
}


void ImplAAFOMRawStorage::readAt(OMUInt64 position,
								 OMByte* bytes,
								 OMUInt32 byteCount,
								 OMUInt32& bytesRead) const
{
  ASSERTU (bytes);
  ASSERTU (_randRep);
  AAFRESULT hr;
  hr = _randRep->ReadAt (position, bytes, byteCount, &bytesRead);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));

  ImplAAFOMRawStorage* nonConstThis = const_cast<ImplAAFOMRawStorage*>(this);
  nonConstThis->_position = position + bytesRead;
}


bool ImplAAFOMRawStorage::isWritable(void) const
{
  ASSERTU (_rep);
  aafBoolean_t r;
  AAFRESULT hr;
  hr = _rep->IsWriteable (&r);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  return r ? true : false;
}


void ImplAAFOMRawStorage::write(const OMByte* bytes,
								OMUInt32 byteCount,
								OMUInt32& bytesWritten)
{
  ASSERTU (_rep);
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
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  _position += bytesWritten;
}


void ImplAAFOMRawStorage::writeAt(OMUInt64 position,
								  const OMByte* bytes,
								  OMUInt32 byteCount,
								  OMUInt32& bytesWritten)
{
  ASSERTU (bytes);
  ASSERTU (_randRep);
  AAFRESULT hr;
  hr = _randRep->WriteAt (position, bytes, byteCount, &bytesWritten);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  _position = position + bytesWritten;
}


bool ImplAAFOMRawStorage::isExtendible(void) const
{
  ASSERTU (_rep);

  // If not an AAFRandomRawStorage, it's definitely not extendible.
  if (! _randRep)
	return false;

  AAFRESULT hr;
  aafBoolean_t r;
  hr = _randRep->IsExtendable (&r);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  return r ? true : false;
}


OMUInt64 ImplAAFOMRawStorage::extent(void) const
{
  ASSERTU (_randRep);
  AAFRESULT hr;
  aafUInt64 result;
  hr = _randRep->GetExtent (&result);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  return result;
}


void ImplAAFOMRawStorage::extend(OMUInt64 newSize)
{
  ASSERTU (_randRep);
  AAFRESULT hr;
  hr = _randRep->SetExtent (newSize);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
}


OMUInt64 ImplAAFOMRawStorage::size(void) const
{
  ASSERTU (_randRep);
  AAFRESULT hr;
  OMUInt64 result;
  hr = _randRep->GetSize (&result);
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
  return result;
}


bool ImplAAFOMRawStorage::isPositionable(void) const
{
  ASSERTU (_rep);
  // If not an AAFRandomRawStorage, it's not positionable.
  if (_randRep)
	return true;
  else
	return false;
}


OMUInt64 ImplAAFOMRawStorage::position(void) const
{
  ASSERTU (_rep);
  return _position;
}


void ImplAAFOMRawStorage::setPosition(OMUInt64 newPosition) const
{
  ASSERTU (_randRep);

  // The _position value is used by read() and write() methods to
  // adjust the storage current position before reading or writing.
  ImplAAFOMRawStorage* nonConstThis = const_cast<ImplAAFOMRawStorage*>(this);
  nonConstThis->_position = newPosition;
}


void ImplAAFOMRawStorage::synchronize(void)
{
  ASSERTU (_rep);
  AAFRESULT hr;
  hr = _rep->Synchronize ();
  ASSERTU (AAFRESULT_SUCCEEDED (hr));
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
  ASSERTU (rep);
  _rep->AddRef ();

  AAFRESULT hr;
  hr = _rep->QueryInterface(IID_IAAFRandomRawStorage, (void **)&_randRep);
  if (AAFRESULT_FAILED (hr))
	_randRep = 0;
}

ImplAAFOMCachedRawStorage::~ImplAAFOMCachedRawStorage()
{
  ASSERTU (_rep);
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
  ASSERTU (_rep);
  aafBoolean_t r;
  AAFRESULT hr;
  hr = _rep->IsReadable (&r);
  if (AAFRESULT_FAILED (hr)) throw OMException(hr);
  return r ? true : false;
}

bool ImplAAFOMCachedRawStorage::isWritable(void) const
{
  ASSERTU (_rep);
  aafBoolean_t r;
  AAFRESULT hr;
  hr = _rep->IsWriteable (&r);
  if (AAFRESULT_FAILED (hr)) throw OMException(hr);
  return r ? true : false;
}

bool ImplAAFOMCachedRawStorage::isExtendible(void) const
{
  ASSERTU (_rep);

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
  ASSERTU (_rep);
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
  ASSERTU (_rep);
  AAFRESULT hr;
  hr = _rep->Synchronize ();
  if (AAFRESULT_FAILED (hr)) throw OMException(hr);
}

void ImplAAFOMCachedRawStorage::rawReadAt(OMUInt64 position,
                                          OMUInt32 byteCount,
                                          OMByte* destination)
{
  ASSERTU(_randRep);
  ASSERTU(byteCount != 0);

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
  ASSERTU(_randRep);
  ASSERTU(byteCount != 0);

  OMUInt32 bytesWritten = 0;
  AAFRESULT hr = _randRep->WriteAt(position, source, byteCount, &bytesWritten);
  if (AAFRESULT_FAILED(hr) || (bytesWritten != byteCount)) {
    throw OMException(hr);
  }
}


/*static*/ aafUInt64 ImplAAFOMCachedRawStorage::getRawStorageSize(
    IAAFRawStorage* pRawStorage )
{
  ASSERTU (pRawStorage);

  aafUInt64 rawStorageSize = 0;

  IAAFRandomRawStorage* pRandomRawStorage = 0;
  pRawStorage->QueryInterface(IID_IAAFRandomRawStorage,
                                             (void **)&pRandomRawStorage);

  if (pRandomRawStorage)
  {
    pRandomRawStorage->GetSize(&rawStorageSize);

    pRandomRawStorage->Release();
    pRandomRawStorage = 0;
  }


  return rawStorageSize;
}

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

#include "AAF.h"
#include <assert.h>

#include "ImplAAFOMRawStorage.h"

ImplAAFOMRawStorage::ImplAAFOMRawStorage (IAAFRawStorage * rep)
  : _rep (rep),
	_randRep (0)
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
  hr = _rep->Read (bytes, byteCount, &bytesRead);
  assert (AAFRESULT_SUCCEEDED (hr));
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
  hr = _rep->Write (bytes, byteCount, &bytesWritten);
  assert (AAFRESULT_SUCCEEDED (hr));
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


void ImplAAFOMRawStorage::synchronize(void)
{
  assert (_rep);
  AAFRESULT hr;
  hr = _rep->Synchronize ();
  assert (AAFRESULT_SUCCEEDED (hr));
}

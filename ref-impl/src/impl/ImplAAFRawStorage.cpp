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

#ifndef __ImplAAFRawStorage_h__
#include "ImplAAFRawStorage.h"
#endif

#include "OMRawStorage.h"

#include <assert.h>
#include <string.h>


ImplAAFRawStorage::ImplAAFRawStorage ()
  : _rep (0),
	_access (kAAFFileAccess_none)
{}


ImplAAFRawStorage::~ImplAAFRawStorage ()
{
  if (_rep)
	{
	  delete _rep;
	  _rep = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRawStorage::IsReadable
        (aafBoolean_t *  pResult)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  if (! pResult)
	return AAFRESULT_NULL_PARAM;

  // We need to cache our own pvtIsReadable data in case this storage
  // is on memory, and has not yet been opened.
  *pResult = (_rep->isReadable() && pvtIsReadable()) ?
	kAAFTrue : kAAFFalse;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRawStorage::Read
        (aafMemPtr_t  buf,
		 aafUInt32  bufSize,
         aafUInt32 *  pNumRead)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  if (! buf)
	return AAFRESULT_NULL_PARAM;

  if (! pNumRead)
	return AAFRESULT_NULL_PARAM;

  aafBoolean_t readable = kAAFFalse;
  AAFRESULT hr = IsReadable (&readable);
  if (AAFRESULT_FAILED (hr))
	return hr;
  if (! readable)
	return AAFRESULT_NOT_READABLE;

  _rep->read (buf, bufSize, *pNumRead);
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRawStorage::IsWriteable
        (aafBoolean_t *  pResult)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  if (! pResult)
	return AAFRESULT_NULL_PARAM;

  // We need to cache our own pvtIsWriteable data in case this storage
  // is on memory, and has not yet been opened.
  *pResult = _rep->isWritable() && pvtIsWriteable() ?
	kAAFTrue : kAAFFalse;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRawStorage::Write
        (aafMemConstPtr_t  buf,
         aafUInt32  bufSize,
		 aafUInt32 * pNumWritten)
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;

  if (! buf)
	return AAFRESULT_NULL_PARAM;

  if (! pNumWritten)
	return AAFRESULT_NULL_PARAM;

  aafBoolean_t writeable = kAAFFalse;
  AAFRESULT hr = IsWriteable (&writeable);
  if (AAFRESULT_FAILED (hr))
	return hr;
  if (! writeable)
	return AAFRESULT_NOT_WRITEABLE;

  _rep->write (buf, bufSize, *pNumWritten);

  if (*pNumWritten < bufSize)
	// Note!  This violates one tenet, that if a method fails, no
	// action is taken.  However, this failure notice is given *after*
	// some bytes have been written...
	return AAFRESULT_SMALLBUF;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRawStorage::Synchronize ()
{
  if (! _rep)
	return AAFRESULT_NOT_INITIALIZED;
  _rep->synchronize ();
  return AAFRESULT_SUCCESS;
}


void ImplAAFRawStorage::Initialize
  (OMRawStorage * prs,
   aafFileAccess_t access)
{
  assert (! _rep);
  assert(prs);
  _rep = prs;
  _access = access;
}


OMRawStorage * ImplAAFRawStorage::GetOMStorage()
{
  return _rep;
}


bool ImplAAFRawStorage::pvtIsReadable () const
{
  return (kAAFFileAccess_read == _access ||
		  kAAFFileAccess_modify == _access);
}


bool ImplAAFRawStorage::pvtIsWriteable () const
{
  return (kAAFFileAccess_write == _access ||
		  kAAFFileAccess_modify == _access);
}


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
   aafFileAccess_e access)
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


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

#include "ImplAAFGetFileBits.h"
#include "ImplAAFSetFileBits.h"
#include "ImplAAFRandomFile.h"
#include "ImplAAFObjectCreation.h"
#include "OMFile.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFGetFileBits;
extern "C" const aafClassID_t CLSID_AAFSetFileBits;

ImplAAFRandomFile::ImplAAFRandomFile ()
{}


ImplAAFRandomFile::~ImplAAFRandomFile ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomFile::GetFileBits (
      ImplAAFGetFileBits ** ppGetFileBits)
{
  if (! ppGetFileBits)
	return AAFRESULT_NULL_PARAM;

  if ((! IsOpen()) && (! IsClosed()))
	// never been opened
	return AAFRESULT_BADOPEN;

  if (IsOpen())
	// opened, but not closed
	return AAFRESULT_BADCLOSE;
	
  if (!IsClosed())
	// opened, but not closed
	return AAFRESULT_BADCLOSE;
  
  // Note that 'writeable' is from the point of view of the AAF file;
  // the GetFileBits operation (which reads bits) should only succeed
  // for AAF files which are to be written.
  if (!IsWriteable ())
	return AAFRESULT_NOT_WRITEABLE;

  assert (IsClosed() && IsWriteable());

  OMRawStorage * os = RawStorage ();
  assert (os);

  ImplAAFGetFileBits * pResult =
	(ImplAAFGetFileBits*)CreateImpl (CLSID_AAFGetFileBits);
  assert (pResult);
  pResult->Initialize (os);
  assert (ppGetFileBits);
  *ppGetFileBits = pResult;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomFile::SaveAsFile (
      ImplAAFFile * /*pDestFile*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRandomFile::SetFileBits (
      ImplAAFSetFileBits ** ppSetFileBits)
{
  if (! ppSetFileBits)
	return AAFRESULT_NULL_PARAM;

  if (IsOpen())
	return AAFRESULT_BADOPEN;
	
  // We only care that this file has never been opened; note that
  // isClosed() means that this file was opened at one time, which to
  // us is a BADOPEN.
  if (IsClosed())
	return AAFRESULT_BADOPEN;
  
  // Note that 'readable' is from the point of view of the AAF file;
  // the SetFileBits operation (which writes bits) should only succeed
  // for AAF files which are to be read.
  if (! IsReadable ())
	return AAFRESULT_NOT_READABLE;

  assert (!IsOpen() && !IsClosed() && IsReadable());

  OMRawStorage * os = RawStorage ();
  assert (os);

  ImplAAFSetFileBits * pResult =
	(ImplAAFSetFileBits*)CreateImpl (CLSID_AAFSetFileBits);
  assert (pResult);
  pResult->Initialize (os);
  assert (ppSetFileBits);
  *ppSetFileBits = pResult;
  return AAFRESULT_SUCCESS;
}

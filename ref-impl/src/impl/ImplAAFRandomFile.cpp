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
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
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

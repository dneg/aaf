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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=




#ifndef __ImplAAFNetworkLocator_h__
#include "ImplAAFNetworkLocator.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "AAFResult.h"

#include <wchar.h>


ImplAAFNetworkLocator::ImplAAFNetworkLocator ()
: _path(PID_NetworkLocator_URLString, L"URLString")
{
  _persistentProperties.put(_path.address());
  _path = L"";
}


ImplAAFNetworkLocator::~ImplAAFNetworkLocator ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFNetworkLocator::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


// Override from AAFLocator
AAFRESULT STDMETHODCALLTYPE
ImplAAFNetworkLocator::GetPath (aafCharacter *  pPathBuf,
								aafUInt32    bufSize)
{
  bool stat;
  if (! pPathBuf)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  stat = _path.copyToBuffer(pPathBuf, bufSize);
  if (! stat)
	{
	  return AAFRESULT_SMALLBUF;
	}
  return AAFRESULT_SUCCESS;
}



// Override from AAFLocator
AAFRESULT STDMETHODCALLTYPE
ImplAAFNetworkLocator::GetPathBufLen (aafUInt32 *  pLen)
{
  if (! pLen)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pLen = _path.size();
  return AAFRESULT_SUCCESS;
}



// Override from AAFLocator
AAFRESULT STDMETHODCALLTYPE
ImplAAFNetworkLocator::SetPath (const aafCharacter *  pPathBuf)
{
  if (! pPathBuf)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (wcslen(pPathBuf)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
	  return AAFRESULT_BAD_SIZE;
	}

	_path = pPathBuf;

	return(AAFRESULT_SUCCESS); 
}




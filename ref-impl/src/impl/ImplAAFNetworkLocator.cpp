/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFNetworkLocator_h__
#include "ImplAAFNetworkLocator.h"
#endif

#include "AAFResult.h"

#include <assert.h>


ImplAAFNetworkLocator::ImplAAFNetworkLocator ()
: _path(PID_NETWORKLOCATOR_URLSTRING, "URLString")
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
ImplAAFNetworkLocator::GetPath (aafWChar *  pPathBuf,
								aafInt32    bufSize)
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
ImplAAFNetworkLocator::GetPathBufLen (aafInt32 *  pLen)
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
ImplAAFNetworkLocator::SetPath (aafWChar *  pPathBuf)
{
  if (! pPathBuf)
	{
	  return AAFRESULT_NULL_PARAM;
	}

	_path = pPathBuf;

	return(AAFRESULT_SUCCESS); 
}


extern "C" const aafClassID_t CLSID_AAFNetworkLocator;

OMDEFINE_STORABLE(ImplAAFNetworkLocator, CLSID_AAFNetworkLocator);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFNetworkLocator::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFNetworkLocator, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}


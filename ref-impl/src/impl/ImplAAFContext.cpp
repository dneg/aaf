
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/






#ifndef __ImplAAFSession_h__
#include "ImplAAFSession.h"
#endif

#include "OMUtilities.h"

#include "ImplAAFObjectCreation.h"
#include "ImplAAFFile.h"
#include "AAFResult.h"

#include <assert.h>

#if defined(__MWERKS__)
#include <wstring.h>	// include wcslen declaration.
#endif

extern "C" const aafClassID_t CLSID_AAFFile;

ImplAAFSession::ImplAAFSession ()
{}


ImplAAFSession::~ImplAAFSession ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSession::EndSession ()
{
  return AAFRESULT_SUCCESS;
}

//****************
// CreateFile()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFSession::CreateFile (aafWChar *  pwFilePath,
							aafFileRev_t  rev,
							ImplAAFFile ** ppFile)
{
  ImplAAFRoot	*pRoot;
  char *  pbFilePath;

  if (! pwFilePath)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (! ppFile)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  pRoot = CreateImpl(CLSID_AAFFile);
  if (!pRoot)
	{
	  return(0x80004005L);	// TODO: change this to AAFRESULT_FAILED
	}

  *ppFile = static_cast<ImplAAFFile*>(pRoot);

  assert (pwFilePath);
  pbFilePath = new char [wcslen (pwFilePath) + 1];
  assert (pbFilePath);
  OMUwc2sb(pbFilePath, pwFilePath);
  (*ppFile)->Create((unsigned char *) pbFilePath, this, rev);
  delete[] pbFilePath;

  return(AAFRESULT_SUCCESS);
}


//****************
// OpenReadFile()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFSession::OpenReadFile(aafWChar *  pwFilePath,
							 ImplAAFFile ** ppFile)
{
  ImplAAFRoot	*pRoot;
  char *  pbFilePath;

  if (! pwFilePath)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (! ppFile)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  pRoot = CreateImpl(CLSID_AAFFile);
  if (!pRoot)
	{
	  return(0x80004005L);	// TODO: change this to AAFRESULT_FAILED
	}

  assert (pwFilePath);
  *ppFile = static_cast<ImplAAFFile*>(pRoot);
  pbFilePath = new char [wcslen (pwFilePath) + 1];
  assert (pbFilePath);
  OMUwc2sb(pbFilePath, pwFilePath);
  (*ppFile)->OpenRead((unsigned char *) pbFilePath, this);
  delete[] pbFilePath;

  return(AAFRESULT_SUCCESS);
}


//****************
// OpenModifyFile()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFSession::OpenModifyFile (aafWChar *  pwFilePath,
								ImplAAFFile ** ppFile)
{
  AAFRESULT hr;
  ImplAAFRoot	*pRoot;
  char *  pbFilePath;

  if (! pwFilePath)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (! ppFile)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  pRoot = CreateImpl(CLSID_AAFFile);
  if (!pRoot)
	{
	  return(0x80004005L);	// TODO: change this to AAFRESULT_FAILED
	}

  *ppFile = static_cast<ImplAAFFile*>(pRoot);
  assert (pwFilePath);
  pbFilePath = new char [wcslen (pwFilePath) + 1];
  assert (pbFilePath);
  OMUwc2sb(pbFilePath, pwFilePath);
  hr = (*ppFile)->OpenModify((unsigned char *) pbFilePath, this);
  delete[] pbFilePath;

  return(hr);
}

ImplAAFFile *ImplAAFSession::GetTopFile()
{
	return(_topFile);
}

void ImplAAFSession::SetTopFile(ImplAAFFile *file)
{
	_topFile = file;
}

OMLSession ImplAAFSession::GetContainerSession(void)
{
	return((OMLSession)NULL);
}

aafProductIdentification_t *ImplAAFSession::GetDefaultIdent(void)
{
	return(_defaultIdent);
}

  //***********************************************************
  // METHOD NAME: SetCurrentIdentification()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSession | SetCurrentIdentification |
  // Sets the object which identifies the creator of the file.
  // @end
  // 
AAFRESULT STDMETHODCALLTYPE
ImplAAFSession::SetDefaultIdentification (aafProductIdentification_t * pIdent)
{
  if (! pIdent)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  _defaultIdent = pIdent;

  if ((_defaultIdent != 0) && (_defaultIdent->productName != 0))
	{
	  setProgramName((const char *)_defaultIdent->productName);
	}
  else
	{
	  setProgramName("Unknown");
	}

  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFSession::GetIdentification (aafProductIdentification_t * pIdent)
{
  if (! pIdent)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  //
  // BobT: Horrible hack! We want to return the identification to the
  // user, but our aafProductIdentification_t struct only contains
  // pointers!  Fix it later; for now we'll return pointers to
  // internal data (DANGEROUS!) to the caller...
  //
  assert (_defaultIdent);
  *pIdent = *_defaultIdent;

  return (AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFSession::BeginSession (
    aafProductIdentification_t  *ident
  )
{
  return SetDefaultIdentification(ident);
}

extern "C" const aafClassID_t CLSID_AAFSession;

OMDEFINE_STORABLE(ImplAAFSession, CLSID_AAFSession);

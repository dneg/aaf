
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
  (*ppFile)->Create(pwFilePath, this, rev);

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
  (*ppFile)->OpenRead(pwFilePath, this);

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
  hr = (*ppFile)->OpenModify(pwFilePath, this);

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

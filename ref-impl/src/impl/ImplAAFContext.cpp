
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

// single instance of this class; initialized by first call to GetInstance().
/*static*/ ImplAAFSession * ImplAAFSession::_singleton; // = 0;

/*static*/ ImplAAFSession * ImplAAFSession::GetInstance ()
{
  if (! _singleton)
	{
	  _singleton = new ImplAAFSession;
	}
  return _singleton;
}


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

// extern "C" const aafClassID_t CLSID_AAFSession;
// CLSID for AAFSession
// {F0C10891-3073-11D2-804A-006008143E6F
const CLSID CLSID_AAFSession = { 0xF0C10891, 0x3073, 0x11D2, { 0x80, 0x4A, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

OMDEFINE_STORABLE(ImplAAFSession, CLSID_AAFSession);


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

#include "AAFFile.h"
#include "ImplAAFFile.h"

#include <assert.h>


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
    ImplAAFSession::CreateFile
        (aafDataBuffer_t  filePath,   //@parm [in] File path [replace with object later]
		 aafFileRev_t  rev,   //@parm [in] File revision to create
         ImplAAFFile ** file)  //@parm [out] Current AAF file
  {
	  AAFFile	*apiFile;
		  
	AAFFile::CreateObject(&apiFile);
	  *file = static_cast<ImplAAFFile*>(apiFile->GetRepObject());
	  (*file)->Create(filePath, this, rev);
	  return(AAFRESULT_SUCCESS);
  }

  //****************
  // OpenReadFile()
  //
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSession::OpenReadFile
        (aafDataBuffer_t  filePath,   //@parm [in] File path [replace with object later]
		 ImplAAFFile ** file)  //@parm [out] Current AAF file
  {
	  AAFFile	*apiFile;
		  
	AAFFile::CreateObject(&apiFile);
	  *file = static_cast<ImplAAFFile*>(apiFile->GetRepObject());
	  (*file)->OpenRead(filePath, this);
	  return(AAFRESULT_SUCCESS);
  }

  //****************
  // OpenModifyFile()
  //
  AAFRESULT STDMETHODCALLTYPE
	  ImplAAFSession::OpenModifyFile
        (aafDataBuffer_t  filePath,   //@parm [in] File path [replace with object later]
		 ImplAAFFile ** file)  //@parm [out] Current AAF file
  {
	  AAFFile	*apiFile;
		  
	AAFFile::CreateObject(&apiFile);
	  *file = static_cast<ImplAAFFile*>(apiFile->GetRepObject());
	  (*file)->OpenModify(filePath, this);
	return(AAFRESULT_SUCCESS);
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
ImplAAFSession::SetDefaultIdentification (
    // @parm aafProductIdentification | ident | [in] a struct from which it is initialized
    aafProductIdentification_t  *ident
  )
  {
	  _defaultIdent = ident;

    if ((_defaultIdent != 0) && (_defaultIdent->productName != 0)) {
      setProgramName(_defaultIdent->productName);
    } else {
      setProgramName("Unknown");
    }

	  return(AAFRESULT_SUCCESS);
  }


AAFRESULT STDMETHODCALLTYPE
ImplAAFSession::GetIdentification (
    aafProductIdentification_t  *ident)
{
  // BobT: this should check for null, and return the appropriate
  // error code.
  assert (ident);
  
  //
  // BobT: Horrible hack! We want to return the identification to the
  // user, but our aafProductIdentification_t struct only contains
  // pointers!  Fix it later; for now we'll return pointers to
  // internal data (DANGEROUS!) to the caller...
  //
  assert (_defaultIdent);
  *ident = *_defaultIdent;

  return (AAFRESULT_SUCCESS);
}

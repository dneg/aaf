
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#include "Container.h"


#ifndef __ImplAAFFile_h__
#include "ImplAAFFile.h"
#endif

#include "AAFTypes.h"
#include "AAFMedia.h"
#include "AAFDataDef.h"
#include "AAFSession.h"
#include "ImplAAFSession.h"
#include "ImplAAFHeader.h"
#include "AAFUtils.h"
#include "aafErr.h"
//#include "aafDefs.h"

#include <assert.h>


ImplAAFFile::ImplAAFFile ()
: _pContainer (0), _fmt(kAAFiMedia)
{}


ImplAAFFile::~ImplAAFFile ()
{}


void ImplAAFFile::InitContainer (void * pContainer)
{
//  assert (! _pContainer);
  _pContainer = pContainer;
  assert (_pContainer);
}


struct  IAAFObject;

void * ImplAAFFile::GetContainer ()
{
  assert (_pContainer);
  return _pContainer;
}

  //***********************************************************
  // METHOD NAME: Close()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFFile | Close |
  // Closes an AAF file, saving the result.
  // @end
  AAFRESULT STDMETHODCALLTYPE
	  ImplAAFFile::Close ()
  {
	
#if FULL_TOOLKIT
	AAFFile *tstFile;
  clearBentoErrors();
  aafAssert((_topMedia == NULL) || (_closeMediaProc != NULL), 
				  this, OM_ERR_MEDIA_CANNOT_CLOSE);
#endif
  
  XPROTECT(this)
	{
#if FULL_TOOLKIT
	  if (_closeMediaProc != NULL)
		{
		  while (_topMedia != NULL)
			CHECK((*_closeMediaProc) (_topMedia));
		}
		
	  if (this == _session->_topFile)
		_session->_topFile = _prevFile;
	  else
		{
		  tstFile = _session->_topFile;
		  while (tstFile && tstFile->_prevFile != NULL)
			{
			  if (tstFile->_prevFile == this)
				 {
					tstFile->_prevFile = _prevFile;
					break;
				 }
			  else
					tstFile = tstFile->_prevFile;
			}
		}
#endif

	  if (_fmt == kAAFiMedia)
		{
		  switch(_openType)
			{
			case kOmCreate:
			case kOmModify:
				_container->SetHead(_head);
				break;
			
			default:
			  break;
			}
		
	  }
		else
		{
#if FULL_TOOLKIT
			if(_rawFileDesc != NULL)
				AAFFree(_rawFileDesc);
#endif
		}

#ifdef AAF_ERROR_TRACE
	  if(_stackTrace != NULL)
	  {
			AAFFree(_stackTrace);
		}
	  _stackTraceSize = 0;
	  _stackTrace = NULL;
#endif
	  if (_fmt == kAAFiMedia)
		{
#if FULL_TOOLKIT
		  if (_BentoErrorNumber)
			_container->OMLAbortContainer();
		  else
#endif
			  _container->OMLCloseContainer();
#if FULL_TOOLKIT
		  if (_BentoErrorRaised)
			{
				if(_BentoErrorNumber == CM_err_BadWrite)
				{
					RAISE(OM_ERR_CONTAINERWRITE);
				}
				else
				{
					RAISE(OM_ERR_BADCLOSE);
				}
			}
#endif
	  }
	  _cookie = 0;
	}
  XEXCEPT
  {
  }
	XEND

///	  omOptFree(NULL, file);

	return(AAFRESULT_SUCCESS);
  }

  //***********************************************************
  // METHOD NAME: GetRevision()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFFile | GetRevision |
  // Get the revision of the ciurrent AAF file.
  // @end
  // 
	AAFRESULT STDMETHODCALLTYPE
		ImplAAFFile::GetRevision (
    // @parm aafFileRev_t * | rev | [out] Revision of the current file
    aafFileRev_t *  rev
  )
  {
		*rev = kAAFRev1;
		return(AAFRESULT_SUCCESS);
  }

/************************
 * Function: InternOpenFile	(INTERNAL)
 *
 * 	Open a file, given a container use mode and type.   This
 *		is the function which takes care of most of the operations
 *		involved in opening the file.   The wrapper routines 
 *		omfsOpenFile() and omfsModifyFile() should be called instead
 *		by client applications.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *		OM_ERR_BAD_SESSION - Session handle is invalid.
 *		OM_ERR_BADHEAD - Head object is invalid or missing.
 *		OM_ERR_NOTAAFFILE - The file is not an AAF file.
 *		OM_ERR_BADOPEN - The file is an AAF file, but fails to open.
 */
AAFRESULT ImplAAFFile::InternOpenFile(aafDataBuffer_t stream, 
								   ImplAAFSession * session,
								   OMLContainerUseMode useMode, 
								   openType_t type)
{
	OMLRefCon        	myRefCon = NULL;
	ImplAAFHeader		*head;
	aafErr_t      		status, finalStatus = OM_ERR_NONE;
	aafInt32			errnum;

	if (session == NULL)
	  return(OM_ERR_BAD_SESSION);	

#if FULL_TOOLKIT
	if(!session->isValidSession())
		  return(OM_ERR_BAD_SESSION);	
#endif

#if FULL_TOOLKIT
	status = InitFileHandle(session, kAAFiMedia);
	if (status != OM_ERR_NONE)
		return (status);
#endif

	XPROTECT(this)
	{
#if FULL_TOOLKIT
		clearBentoErrors();
#endif
		_openType = type;

		if (stream == NULL) 
		  RAISE(OM_ERR_NULL_PARAM);
		
		myRefCon = createRefConForMyHandlers(session->GetContainerSession(), 
											 (char *) stream, (GetUpdatingTargetType)NULL, this);
#if FULL_TOOLKIT
		aafCheckBentoRaiseError(this, OM_ERR_BADSESSIONMETA);
#endif
		
		/*
		 * Open the container
		 */
#if FULL_TOOLKIT
		_BentoErrorNumber = 0;
		session->ResetBentoError();
#endif
		_container = new OMContainer;
		_container->OMLOpenContainer(session->GetContainerSession(), myRefCon, "AAF", 
									useMode);
		if (_container == NULL)
		{
#if FULL_TOOLKIT
			if(_BentoErrorRaised)
				errnum = _BentoErrorNumber;
			else
				errnum = session->GetBentoError();
			if (errnum== CM_err_BadMagicBytes)
			{
				RAISE(OM_ERR_NOTAAFFILE);
			}
			else if(session->GetBentoError() == CM_err_GenericMessage)
			{
				RAISE(OM_ERR_FILE_NOT_FOUND);
			}
			else
			{
				RAISE(OM_ERR_BADOPEN);
			}
#endif
		}
		
		/* This is a little bit of a catch-22, since we're setting
		 * the rev "before" we can verify that the HEAD object is a head
		 * object.   But, we need head object to get the rev! */
		head = new ImplAAFHeader();
//!!!		this, (OMLObject)_container->cmFindObject(1));
//!!!		head->Load(this);
		_head = head;
		/* We now use datakinds in the whole API, not just 2.x
		 */
#if FULL_TOOLKIT
		_head->DatakindLookup(NODATAKIND, &_nilKind, &status);
		CHECK(status);
		_head->DatakindLookup(PICTUREKIND, &_pictureKind, &status);
		CHECK(status);
		_head->DatakindLookup(SOUNDKIND, &_soundKind, &status);
		CHECK(status);

		/* Register any other properties and types in the file in the cache */
		CHECK(_head->UpdateLocalTypesProps(_container));
		_session->HandleOpenCallback(this);
#endif
	}
	XEXCEPT
	{
	  finalStatus = XCODE();
	  NO_PROPAGATE(); /* Rely on finalStatus variable, as file will be NULL */

		/* Reset to previous state before returning with error */
		if (session && this)
		{
			delete head;

#ifdef AAF_ERROR_TRACE
		  if(_stackTrace != NULL)
			 {
				AAFFree(_stackTrace);
			 }
		  _stackTraceSize = 0;
		  _stackTrace = NULL;
#endif
		  if(_container != NULL)
			 {
				_container->OMLAbortContainer();
			 }
		  else if(myRefCon != NULL)
			 OMLFree((OMContainer *)NULL, myRefCon, session->GetContainerSession());

		  /* Set to previous file or NULL */
		  session->SetTopFile(_prevFile);
		  _cookie = 0;
//		  omOptFree(NULL, file);
		}
 // 		file = NULL;
	}
	XEND;

	return (finalStatus);
}

/************************
 * Function: Create
 *
 * 	Create an AAF file referencing the given session, of the
 *		given revision.
 *
 * Argument Notes:
 *		Stream -- Must be of a type which matches the Bento file handler.
 *		ie:	a SFReply record pointer for omfmacsf.c
 *				or a path string for aafansic.c
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
AAFRESULT ImplAAFFile::Create(
			aafDataBuffer_t		stream, 
			ImplAAFSession		*session, 
			aafFileRev_t		rev)
{
	OMLRefCon        myRefCon;
	ImplAAFHeader *     head;
	aafErr_t		status;

	XPROTECT((ImplAAFFile*)NULL)
	{
		if (stream == NULL) 
		  RAISE(OM_ERR_NULL_PARAM);

		if (session == NULL)
		  RAISE(OM_ERR_BAD_SESSION);	

#if FULL_TOOLKIT
		if(!session->isValidSession())
		  RAISE(OM_ERR_BAD_SESSION);	

		CHECK(InitFileHandle(session, kAAFiMedia));	
		clearBentoErrors();
#endif
		_setrev = rev;
		_openType = kOmCreate;
		
		myRefCon = createRefConForMyHandlers(session->GetContainerSession(), 
											 (char *) stream, (GetUpdatingTargetType)NULL, this);
#if FULL_TOOLKIT
		aafCheckBentoRaiseError(this, OM_ERR_BADSESSIONMETA);
#endif
		
		_container = new OMContainer;
		_container->OMLOpenNewContainer(session->GetContainerSession(), myRefCon,
												"AAF",
												(OMLContainerUseMode) kOMLWriting,
												1, 0, 0);
#if FULL_TOOLKIT
		aafCheckBentoRaiseError(this, OM_ERR_BADOPEN);
#endif
		if(_container == NULL)
			RAISE(OM_ERR_BADOPEN);
	
		head = new ImplAAFHeader();		//!!!this, (OMLObject)_container->cmFindObject(1));
		_head = head;
		if (head == NULL)
		  RAISE(OM_ERR_BADHEAD);

		_head->AddIdentificationObject(session->GetDefaultIdent());
#if FULL_TOOLKIT
		aafCheckBentoRaiseError(this, OM_ERR_BADHEAD);
	
		_head = head;
		/* We now use datakinds in the whole API, not just 2.x
		 */
		_head->DatakindLookup(NODATAKIND, &_nilKind, &status);
		CHECK(status);
		_head->DatakindLookup(PICTUREKIND, &_pictureKind, &status);
		CHECK(status);
		_head->DatakindLookup(SOUNDKIND, &_soundKind, &status);
		CHECK(status);
#endif
	}
	XEXCEPT
	  {
		 /* Reset to previous state before returning with error */
		 if (session && this)
		{
			/* Added by MT: you need to save the pointer before deleting
			 * the  object
			 */
			ImplAAFFile *pSavedAddr = _prevFile;

			if (session->GetTopFile())
				AAFFree(session->GetTopFile());
			     
			/* Set to previous this or NULL */
			session->SetTopFile(pSavedAddr);
		}
	  }
	XEND;
	
	return (OM_ERR_NONE);
}

/************************
 * Function: OpenRead
 *
 * 	Opens an AAF file read-only.
 *
 * Argument Notes:
 *		Stream -- Must be of a type which matches the Bento file handler.
 *		ie:	a SFReply record pointer for omfmacsf.c
 *				or a path string for aafansic.c
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *		OM_ERR_BAD_SESSION - Session handle is invalid.
 *		OM_ERR_BADHEAD - Head object is invalid or missing.
 *		OM_ERR_NOTAAFFILE - The file is not an AAF file.
 *		OM_ERR_BADOPEN - The file is an AAF file, but fails to open.
 */
AAFRESULT ImplAAFFile::OpenRead(
			aafDataBuffer_t		stream, 
			ImplAAFSession *	session)
{
	return (InternOpenFile(stream, session, (OMLContainerUseMode) 0, 
							   kOmOpenRead));
}


/************************
 * Function: omfsModifyFile
 *
 * 	Opens an AAF file for update (read-write).
 *
 * Argument Notes:
 *		Stream -- Must be of a type which matches the Bento file handler.
 *		ie:	a SFReply record pointer for omfmacsf.c
 *				or a path string for aafansic.c
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *		OM_ERR_BAD_SESSION - Session handle is invalid.
 *		OM_ERR_BADHEAD - Head object is invalid or missing.
 *		OM_ERR_NOTAAFFILE - The file is not an AAF file.
 *		OM_ERR_BADOPEN - The file is an AAF file, but fails to open.
 */
AAFRESULT ImplAAFFile::OpenModify(
			aafDataBuffer_t		stream, 
			ImplAAFSession *	session)
{
	aafInt32	numIdent;
	
	XPROTECT((ImplAAFFile*)NULL)
	{
		CHECK(InternOpenFile(stream, session,
				 (OMLContainerUseMode) kOMLReuseFreeSpace, kOmModify));
		CHECK(_head->SetToolkitRevisionCurrent());
		
		/* NOTE: If modifying an existing file WITHOUT an IDNT object, add a
		 * dummy IDNT object to indicate that this program was not the creator.
		 */
		CHECK(_head->GetNumIdentifications(&numIdent));
		if(numIdent == 0)
		{
			_head->AddIdentificationObject((aafProductIdentification_t *)NULL);
		}
		/* Now, always add the information from THIS application */
		_head->AddIdentificationObject(session->GetDefaultIdent());
	}
	XEXCEPT
	  {
	  }
	XEND;
  
	return (OM_ERR_NONE);
}

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "ImplAAFFile.h"

#include "OMFile.h"
#include "OMUtilities.h"
#include "ImplAAFClassFactory.h"

#include "ImplAAFSession.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDataDef.h"

#include "ImplAAFObjectCreation.h"

#include <assert.h>

// local function for simplifying error handling.
inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw r;
}

inline void checkExpression(bool test, AAFRESULT r)
{
  if (!test)
    throw r;
}


extern "C" const aafClassID_t CLSID_AAFHeader;

AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Initialize ()
{
	if (_initialized)
	{
		return AAFRESULT_ALREADY_INITIALIZED;
	}

	// Create the class factory for base classes.
	_classFactory = new ImplAAFClassFactory;
	if (NULL == _classFactory)
		return AAFRESULT_NOMEMORY;

	_initialized = AAFTrue;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::OpenExistingRead (wchar_t * pFileName,
							   aafUInt32 modeFlags)
{
	ImplAAFSession * pS;
	AAFRESULT stat = AAFRESULT_SUCCESS;

	// Validate parameters and preconditions.
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (_open)
		return AAFRESULT_ALREADY_OPEN;

	if (_file)
		return AAFRESULT_ALREADY_OPEN;

	if (! pFileName)
		return AAFRESULT_NULL_PARAM;

	if (modeFlags)
		return AAFRESULT_BAD_FLAGS;
	

	pS = ImplAAFSession::GetInstance();
	assert (pS);

	// Save the mode flags for now. They are not currently (2/4/1999) used by the
	// OM to open the doc file. Why do we return an error if modeFlags != 0?
	_modeFlags = modeFlags;

	try
	{
		// Ask the OM to open the file.
		_file = OMFile::openRead(pFileName, _classFactory);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

		// Get the byte order
		_byteOrder = _file->byteOrder();

		// Restore the header.
		OMStorable* head = _file->restore();
		_head = dynamic_cast<ImplAAFHeader *>(head);
		checkExpression(NULL != _head, AAFRESULT_BADHEAD);
		
		// Initialize the mob lookup tables.
		checkResult(_head->LoadMobTables());
		
		_open = AAFTrue;
		_openType = kOmOpenRead;
	}
	catch (AAFRESULT &r)
	{
		stat = r;

		// Cleanup after failure.
		if (_file)
		{
			_file->close();
			_file = 0;
		}

		if (NULL == _head)
		{
			_head->ReleaseReference();
			_head = 0;
		}
	}
	
	return stat;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::OpenExistingModify (wchar_t * pFileName,
								 aafUInt32 modeFlags,
								 aafProductIdentification_t * pIdent)
{
	ImplAAFSession * pS;
	AAFRESULT stat = AAFRESULT_SUCCESS;


	// Validate parameters and preconditions.
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (_open)
		return AAFRESULT_ALREADY_OPEN;

	if (! pFileName)
		return AAFRESULT_NULL_PARAM;

	if (! pIdent)
		return AAFRESULT_NULL_PARAM;

	if (modeFlags)
		return AAFRESULT_BAD_FLAGS;

	memcpy (&_ident, pIdent, sizeof (_ident));

	pS = ImplAAFSession::GetInstance();
	assert (pS);

	
	// Save the mode flags for now. They are not currently (2/4/1999) used by the
	// OM to open the doc file. Why do we return an error if modeFlags != 0?
	_modeFlags = modeFlags;

	try 
	{
		// Ask the OM to open the file.
		_file = OMFile::openModify(pFileName, _classFactory);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

		// Get the byte order
		_byteOrder = _file->byteOrder();

		// Restore the header.
		OMStorable* head = _file->restore();
		_head = dynamic_cast<ImplAAFHeader *>(head);
		checkExpression(NULL != _head, AAFRESULT_BADHEAD);
		
		// Initialize the mob lookup tables.
		checkResult(_head->LoadMobTables());

		checkResult(_head->SetToolkitRevisionCurrent());
	  
		// NOTE: If modifying an existing file WITHOUT an IDNT object, add a
		// dummy IDNT object to indicate that this program was not the creator.
		//
		aafInt32	numIdent = 0;
		checkResult(_head->GetNumIdentifications(&numIdent));
		if(numIdent == 0)
		{
			_head->AddIdentificationObject((aafProductIdentification_t *)NULL);
		}
		// Now, always add the information from THIS application */
		_head->AddIdentificationObject(&_ident);
		

		_open = AAFTrue;
		_openType = kOmModify;
	}
	catch (AAFRESULT &rc)
	{
		stat = rc;

		// Cleanup after failure.
		if (_file)
		{
			_file->close();
			_file = 0;
		}

		if (NULL == _head)
		{
			_head->ReleaseReference();
			_head = 0;
		}
	}


	return stat;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::OpenNewModify (wchar_t * pFileName,
							aafUInt32 modeFlags,
							aafProductIdentification_t * pIdent)
{
	ImplAAFSession * pS;
	AAFRESULT stat = AAFRESULT_SUCCESS;

	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (_open)
		return AAFRESULT_ALREADY_OPEN;

	if (! pFileName)
		return AAFRESULT_NULL_PARAM;

	if (modeFlags)
		return AAFRESULT_BAD_FLAGS;


	pS = ImplAAFSession::GetInstance();
	assert (pS);

	memcpy (&_ident, pIdent, sizeof (_ident));

	try
	{
		// Create the header for the OM manager to use as the root
		// for the file.
		_head = dynamic_cast<ImplAAFHeader*>(CreateImpl(CLSID_AAFHeader));
		checkExpression(NULL != _head, AAFRESULT_BADHEAD);
		
		// Add the ident to the header.
		checkResult(_head->AddIdentificationObject(&_ident));
		  
		// Set the byte order
		_byteOrder = hostByteOrder();
		_head->SetByteOrder(_byteOrder);

		// Attempt to create the file.
		_file = OMFile::createModify(pFileName, _classFactory, _byteOrder, _head);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

		_open = AAFTrue;
		_openType = kOmCreate;
		GetRevision(&_setrev);
	}
	catch (AAFRESULT &rc)
	{
		stat = rc;

		// Cleanup after failure.
		if (_file)
		{
			_file->close();
			_file = 0;
		}

		if (NULL == _head)
		{
			_head->ReleaseReference();
			_head = 0;
		}
	}

	return stat;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::OpenTransient (aafProductIdentification_t * /*pIdent*/)
{

	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;


	if (_open)
		return AAFRESULT_ALREADY_OPEN;

	return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Save ()
{
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;


	if (!_open)
		return AAFRESULT_NOT_OPEN;

	// If any new modes are added then the following line will
	// have to be updated.
	if (kOmCreate == _openType || kOmModify == _openType) {
	  _file->save();
	} else {
	  return AAFRESULT_WRONG_OPENMODE;
	}


	// Record the fact that this file was modified
	_head->SetModified();


	// BobT Hack!  We don't have Save() hooked up yet; Close() will
	// save, so I'll do something extremely ugly and just return without
	// complaint.
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::SaveAs (wchar_t * /*pFileName*/,
					 aafUInt32 /*modeFlags*/)
{
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (!_open)
		return AAFRESULT_NOT_OPEN;

	return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Revert ()
{
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;


	if (!_open)
		return AAFRESULT_NOT_OPEN;

	return AAFRESULT_NOT_IMPLEMENTED;
}


ImplAAFFile::ImplAAFFile () :
		_cookie(0),
		_file(0),
		_classFactory(NULL),
		_byteOrder(0),
		_openType(kOmUndefined),
		_head(NULL),
		_semanticCheckEnable(AAFFalse),
		_nilKind(0),
		_pictureKind(0),
		_soundKind(0),
		_initialized(AAFFalse),
		_open(AAFFalse),
		_modeFlags(0)
{
	memset (&_ident, 0, sizeof (_ident));
}


ImplAAFFile::~ImplAAFFile ()
{
	InternalReleaseObjects();

	// cleanup the container.
	if (_classFactory)
	{
		delete _classFactory;
		_classFactory = NULL;
	}

	// cleanup the OM File.
	if (_file)
	{
		delete _file;
		_file = NULL;
	}

}

void ImplAAFFile::InternalReleaseObjects()
{
	if (0 != _soundKind)
	{
		_soundKind->ReleaseReference();
		_soundKind = 0;
	}

	if (0 != _pictureKind)
	{
		_pictureKind->ReleaseReference();
		_pictureKind = 0;
	}

	if (0 != _nilKind)
	{
		_nilKind->ReleaseReference();
		_nilKind = 0;
	}
}


//***********************************************************
// METHOD NAME: Close()
//
// DESCRIPTION:
// @mfunc AAFRESULT | AAFFile | Close |
// Closes an AAF file, saving the result.
// @end
//
// TODO: the FULL_TOOLKIT code closes all associated media files
// and removes this file node from the session's file list.
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Close ()
{
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (!_open)
		return AAFRESULT_NOT_OPEN;


	// Release all of the pointers that we created or copied
	// during the create or open methods.
	InternalReleaseObjects();


	// Close the OM file.
	_file->close();


	// Whenever a file is created or opened a new container is
	// created.  If we don't want to leak the container object
	// and any objects in the associated OMFile object we had
	// better delete the container object here.
	delete _file;
	_file = 0;
  
	// Release the last reference to the header of the file. 
	// We need to release the header after the file is closed so
	// that the OMFile object within the container can safely
	// use its reference to its root (a.k.a. header).
	if (0 != _head)
	{
		_head->ReleaseReference();
		_head = 0;
	}

	_cookie = 0;
	_open = AAFFalse;
	_openType = kOmUndefined;


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
ImplAAFFile::GetRevision
(
 // @parm aafFileRev_t * | rev | [out] Revision of the current file
 aafFileRev_t *  rev
 )
{
  *rev = kAAFRev1;
  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::GetHeader (ImplAAFHeader ** header)
{
  *header = _head;

  // We are returning a copy of the reference counted object.
  if (_head)
	_head->AcquireReference();

  return(AAFRESULT_SUCCESS);
}

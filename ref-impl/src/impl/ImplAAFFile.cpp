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

#include "ImplAAFFile.h"

#include "OMFile.h"
#include "OMUtilities.h"
#include "OMClassFactory.h"

#include "ImplAAFDataDef.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"
#include "ImplAAFHeader.h"
#include "ImplAAFRandomRawStorage.h"
#include "ImplAAFIdentification.h"

#include "AAFStoredObjectIDs.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFOMRawStorage.h"

#include "AAFFileSignatures.h"
#include "AAFFileMode.h"
#include "AAFFileKinds.h"

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFIdentification>
  ImplAAFIdentificationSP;
typedef ImplAAFSmartPointer<ImplAAFRandomRawStorage>
  ImplAAFRandomRawStorageSP;

#include <assert.h>

// For IAAFRawStorage
#include "AAF.h"

// AAF file signature.
//static const OMFileSignature aafFileSignature  =
//{0x42464141,
//0xff0d, 0x4d4f,
//{0x06, 0x0e, 0x2b, 0x34, 0x01, 0x01, 0x01, 0xff}};


static const aafUID_t kNullAuid = { 0 };
static const aafProductIdentification_t kNullIdent = { 0 };

//
// File Format Version
//
// Update this when incompatible changes are made to AAF file format
// version. 
//
//    0 : Tue Jan 11 17:08:26 EST 2000
//        Initial Release version.
//
static const aafUInt32 sCurrentAAFObjectModelVersion = 0;


static const aafUID_t kNullFileKind = { 0 };


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

inline int equalUID(const aafUID_t & a, const aafUID_t & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}

//
// Returns true if all flags set are defined flags; returns false if
// any flags set are *not* defined.
//
static bool areAllModeFlagsDefined (aafUInt32 modeFlags)
{
  static const aafUInt32 kAllFlags =
	AAF_FILE_MODE_EAGER_LOADING |
	AAF_FILE_MODE_REVERTABLE |
	AAF_FILE_MODE_UNBUFFERED |
	AAF_FILE_MODE_RECLAIMABLE |
	AAF_FILE_MODE_USE_LARGE_SS_SECTORS |
	AAF_FILE_MODE_CLOSE_FAIL_DIRTY |
	AAF_FILE_MODE_DEBUG0_ON |
	AAF_FILE_MODE_DEBUG1_ON;

  if (modeFlags & (~kAllFlags))
	{
	  // at least one flag is not defined
	  return false;
	}
  else
	{
	  return true;
	}
}


extern "C" const aafClassID_t CLSID_AAFDictionary;

AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Initialize ()
{
	if (_initialized)
	{
		return AAFRESULT_ALREADY_INITIALIZED;
	}

	// Create the class factory for base classes.
	_factory = ImplAAFDictionary::CreateDictionary();
	if (NULL == _factory)
		return AAFRESULT_NOMEMORY;

	// Create the class factory for meta classes.
	_metafactory = ImplAAFMetaDictionary::CreateMetaDictionary();
	if (NULL == _metafactory)
		return AAFRESULT_NOMEMORY;

	// The following is temporary code. (transdel:2000-APR-11)
	_factory->setMetaDictionary(_metafactory);
	_metafactory->setDataDictionary(_factory);
	
#if 0
	// Experimental: Create and initialize all of the axiomatic definitions.
	// This must be done before another other definitions or data objects
	// can be created.
	AAFRESULT result = _metafactory->InstantiateAxiomaticDefinitions();
	assert(AAFRESULT_SUCCEEDED(result));
#else
    // Initialize only the non-persistent parts of the meta dictionary
	AAFRESULT result = _metafactory->Initialize();
	assert(AAFRESULT_SUCCEEDED(result));
#endif

	_initialized = kAAFTrue;

	return result;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::OpenExistingRead (const aafCharacter * pFileName,
							   aafUInt32 modeFlags)
{
	AAFRESULT stat = AAFRESULT_SUCCESS;

	// Validate parameters and preconditions.
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (IsOpen())
		return AAFRESULT_ALREADY_OPEN;

	if (_file)
		return AAFRESULT_ALREADY_OPEN;

	if (! pFileName)
		return AAFRESULT_NULL_PARAM;

	if (! areAllModeFlagsDefined (modeFlags))
	  return AAFRESULT_BAD_FLAGS;

	if (modeFlags)
	  return AAFRESULT_NOT_IN_CURRENT_VERSION;

	// Save the mode flags for now. They are not currently (2/4/1999) used by the
	// OM to open the doc file. Why do we return an error if modeFlags != 0?
	//
	// Answer: because none of them are implemented yet.
	_modeFlags = modeFlags;

	try
	{
		// Ask the OM to open the file.
		_file = OMFile::openExistingRead(pFileName,
										 _factory,
										 0,
										 OMFile::lazyLoad,
										 _metafactory);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

        // Check the file's signature.
        OMFileSignature sig = _file->signature();
		const OMFileSignature aafFileSignature =
		  *reinterpret_cast<const OMFileSignature *>
		  (&aafFileSignatureAafSSBinaryGUID);
        checkExpression(sig == aafFileSignature, AAFRESULT_NOT_AAF_FILE);

		// Restore the meta dictionary, it should be the same object
		// as _metafactory
		OMDictionary* mf = _file->dictionary();
		assert(mf == _metafactory);

		// Make sure all definitions are present in the meta dictionary
		ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(mf);
		assert(d);
		d->InstantiateAxiomaticDefinitions();

		// Get the byte order
		OMByteOrder byteOrder = _file->byteOrder();
		if (byteOrder == littleEndian) {
			_byteOrder = 0x4949; // 'II'
		} else { // bigEndian
			_byteOrder = 0x4d4d; // 'MM'
		}

		// Restore the header.
		bool regWasEnabled = _factory->SetEnableDefRegistration (false);
		OMStorable* head = _file->restore();
		_head = dynamic_cast<ImplAAFHeader *>(head);
		_head->SetFile(this);
		
		// Check for file format version.
		if (_head->IsObjectModelVersionPresent())
		  {
			// If property isn't present, the default version is 0,
			// which is always (supposed to be) legible.  If it is
			// present, find out the version number to determine if
			// the file is legible.
			if (_head->GetObjectModelVersion() >
				sCurrentAAFObjectModelVersion)
			  {
				// File version is higher than the version understood
				// by this toolkit.  Therefore this file cannot be read.
				return AAFRESULT_FILEREV_DIFF;
			  }
		  }

		// Now that the file is open and the header has been
		// restored, complete the initialization of the
		// dictionary. We obtain the dictionary via the header
		// to give the persistence mechanism a chance to work.
		ImplAAFDictionary* dictionary = 0;
		HRESULT hr = _head->GetDictionary(&dictionary);
		if (hr != AAFRESULT_SUCCESS)
		  return hr;
		_factory->SetEnableDefRegistration (regWasEnabled);
		dictionary->InitBuiltins();
		dictionary->ReleaseReference();
		dictionary = 0;
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
ImplAAFFile::OpenExistingModify (const aafCharacter * pFileName,
								 aafUInt32 modeFlags,
								 aafProductIdentification_t * pIdent)
{
	AAFRESULT stat = AAFRESULT_SUCCESS;


	// Validate parameters and preconditions.
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (IsOpen())
		return AAFRESULT_ALREADY_OPEN;

	if (_file)
		return AAFRESULT_ALREADY_OPEN;

	if (! pFileName)
		return AAFRESULT_NULL_PARAM;

	if (! pIdent)
		return AAFRESULT_NULL_PARAM;

	if (! areAllModeFlagsDefined (modeFlags))
	  return AAFRESULT_BAD_FLAGS;

	if (modeFlags)
	  return AAFRESULT_NOT_IN_CURRENT_VERSION;
	
	// Save the mode flags for now. They are not currently (2/4/1999) used by the
	// OM to open the doc file. Why do we return an error if modeFlags != 0?
	//
	// Answer: because none of them are implemented yet.
	_modeFlags = modeFlags;

	try 
	{
		// Ask the OM to open the file.
		_file = OMFile::openExistingModify(pFileName,
										   _factory,
										   0,
										   OMFile::lazyLoad,
										   _metafactory);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

		// Restore the meta dictionary, it should be the same object
		// as _metafactory
		OMDictionary* mf = _file->dictionary();
		assert(mf == _metafactory);

		// Make sure all definitions are present in the meta dictionary
		ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(mf);
		assert(d);
		d->InstantiateAxiomaticDefinitions();

		// Get the byte order
		OMByteOrder byteOrder = _file->byteOrder();
		if (byteOrder == littleEndian) {
			_byteOrder = 0x4949; // 'II'
		} else { // bigEndian
			_byteOrder = 0x4d4d; // 'MM'
		}

		// Restore the header.
		bool regWasEnabled = _factory->SetEnableDefRegistration (false);
		OMStorable* head = _file->restore();
		_head = dynamic_cast<ImplAAFHeader *>(head);
		checkExpression(NULL != _head, AAFRESULT_BADHEAD);
		// Check for file format version.
		if (_head->IsObjectModelVersionPresent())
		  {
			// If property isn't present, the default version is 0,
			// which is always (supposed to be) legible.  If it is
			// present, find out the version number to determine if
			// the file is legible.
			if (_head->GetObjectModelVersion() >
				sCurrentAAFObjectModelVersion)
			  {
				// File version is higher than the version understood
				// by this toolkit.  Therefore this file cannot be read.
				return AAFRESULT_FILEREV_DIFF;
			  }
		  }

		// Now that the file is open and the header has been
		// restored, complete the initialization of the
		// dictionary. We obtain the dictionary via the header
		// to give the persistence mechanism a chance to work.
		ImplAAFDictionary* dictionary = 0;
		HRESULT hr = _head->GetDictionary(&dictionary);
		if (hr != AAFRESULT_SUCCESS)
		  return hr;
		_factory->SetEnableDefRegistration (regWasEnabled);
		dictionary->InitBuiltins();
		dictionary->ReleaseReference();
		dictionary = 0;

		checkResult(_head->SetToolkitRevisionCurrent());
	  
		// NOTE: If modifying an existing file WITHOUT an IDNT object, add a
		// dummy IDNT object to indicate that this program was not the creator.
		//
		aafUInt32	numIdent = 0;
		checkResult(_head->CountIdentifications(&numIdent));
		if(numIdent == 0)
		{
			_head->AddIdentificationObject((aafProductIdentification_t *)NULL);
		}
		// Now, always add the information from THIS application */
		_head->AddIdentificationObject(pIdent);
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
ImplAAFFile::OpenNewModify (const aafCharacter * pFileName,
							aafUInt32 modeFlags,
							aafProductIdentification_t * pIdent)
{
	ImplAAFContentStorage	*pCStore = NULL;
	AAFRESULT stat = AAFRESULT_SUCCESS;
	aafVersionType_t		theVersion = { 1, 0 };

	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (IsOpen())
		return AAFRESULT_ALREADY_OPEN;

	if (_file)
		return AAFRESULT_ALREADY_OPEN;

	if (! pFileName)
		return AAFRESULT_NULL_PARAM;

	if (! pIdent)
		return AAFRESULT_NULL_PARAM;

	if (! areAllModeFlagsDefined (modeFlags))
	  return AAFRESULT_BAD_FLAGS;

	if (modeFlags)
	  return AAFRESULT_NOT_IN_CURRENT_VERSION;

	try
	{
	  // Create the header for the OM manager to use as the root
	  // for the file. Use the OMClassFactory interface for this 
	  // object because the dictionary has not been associated with
	  // a header yet (Chicken 'n Egg problem).
	  const OMClassId& soid =
		*reinterpret_cast<const OMClassId *>(&AUID_AAFHeader);
	  _head = static_cast<ImplAAFHeader *>(_factory->create(soid));
	  checkExpression(NULL != _head, AAFRESULT_BADHEAD);
		
		// Make sure the header is initialized with our previously created
		// dictionary.
		_head->SetDictionary(_factory);

		// Set the file format version.
		//
		// BobT Fri Jan 21 14:37:43 EST 2000: the default behavior is
		// that if the version isn't present, it's assumed to Version
		// 0.  Therefore if the current version is 0, don't write out
		// the property.  We do this so that hackers examining written
		// files won't know that a mechanism exists to mark future
		// incompatible versions, and so will work harder to make any
		// future changes compatible.
		if (sCurrentAAFObjectModelVersion)
		  {
			_head->SetObjectModelVersion(sCurrentAAFObjectModelVersion);
		  }

		// Add the ident to the header.
		checkResult(_head->AddIdentificationObject(pIdent));
		  
		// Set the byte order
		OMByteOrder byteOrder = hostByteOrder();
		if (byteOrder == littleEndian) {
			_byteOrder = 0x4949; // 'II'
		} else { // bigEndian
			_byteOrder = 0x4d4d; // 'MM'
		}
		_head->SetByteOrder(_byteOrder);
		_head->SetFileRevision (theVersion);

		//JeffB!!! We must decide whether def-only files have a content storage
		checkResult(_head->GetContentStorage(&pCStore));
		pCStore->ReleaseReference(); // need to release this pointer!
		pCStore = 0;

		// Attempt to create the file.
		const OMFileSignature aafFileSignature =
		  *reinterpret_cast<const OMFileSignature *>
		  (&aafFileSignatureAafSSBinaryGUID);
		_file = OMFile::openNewModify(pFileName,
									  _factory,
									  0,
									  byteOrder,
									  _head,
									  aafFileSignature,
									  _metafactory);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

		// Restore the meta dictionary, it should be the same object
		// as _metafactory
		OMDictionary* mf = _file->dictionary();
		assert(mf == _metafactory);

		// Make sure all definitions are present in the meta dictionary
		ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(mf);
		assert(d);
		d->InstantiateAxiomaticDefinitions();

		// Now that the file is open and the header has been
		// restored, complete the initialization of the
		// dictionary. We obtain the dictionary via the header
		// to give the persistence mechanism a chance to work.
		_factory->InitializeOMStorable
		    (_factory->GetBuiltinDefs()->cdDictionary());
		ImplAAFDictionary* dictionary = 0;
		HRESULT hr = _head->GetDictionary(&dictionary);
		if (hr != AAFRESULT_SUCCESS)
		  return hr;
		dictionary->InitBuiltins();

		dictionary->ReleaseReference();
		dictionary = 0;
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
ImplAAFFile::OpenTransient (aafProductIdentification_t * pIdent)
{

	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (IsOpen())
		return AAFRESULT_ALREADY_OPEN;

	if (! pIdent)
		return AAFRESULT_NULL_PARAM;

	return AAFRESULT_NOT_IN_CURRENT_VERSION;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::CreateAAFFileOnRawStorage
  (IAAFRawStorage * pRawStorage,
   aafFileExistence_t existence,
   aafFileAccess_t access,
   aafUID_constptr pFileKind,
   aafUInt32 modeFlags,
   aafProductIdentification_constptr pIdent)
{
  if (! _initialized)
	return AAFRESULT_NOT_INITIALIZED;

  if (! pRawStorage)
	return AAFRESULT_NULL_PARAM;

  if (IsOpen())
	return AAFRESULT_ALREADY_OPEN;

  if (! areAllModeFlagsDefined (modeFlags))
	return AAFRESULT_BAD_FLAGS;

  if (modeFlags)
	return AAFRESULT_NOT_IN_CURRENT_VERSION;
	
  AAFRESULT hr;
  aafBoolean_t b = kAAFFalse;
  switch (access)
	{
	case kAAFFileAccess_read:
	  if (kAAFFileExistence_new == existence)
		return AAFRESULT_INVALID_PARAM;
	  b = kAAFFalse;
	  hr = pRawStorage->IsReadable (&b);
	  if (kAAFFalse == b)
		return AAFRESULT_NOT_READABLE;
	  break;
	case kAAFFileAccess_write:
	  if (kAAFFileExistence_existing == existence)
		return AAFRESULT_INVALID_PARAM;
	  b = kAAFFalse;
	  hr = pRawStorage->IsWriteable (&b);
	  if (kAAFFalse == b)
		return AAFRESULT_NOT_WRITEABLE;
	  break;
	case kAAFFileAccess_modify:
	  b = kAAFFalse;
	  hr = pRawStorage->IsWriteable (&b);
	  if (kAAFFalse == b)
		return AAFRESULT_NOT_WRITEABLE;
	  b = kAAFFalse;
	  hr = pRawStorage->IsReadable (&b);
	  if (kAAFFalse == b)
		return AAFRESULT_NOT_READABLE;
	  break;
	default:
	  return AAFRESULT_INVALID_PARAM;
	}

  switch (existence)
	{
	case kAAFFileExistence_new:
	  if (! pFileKind)
		return AAFRESULT_NULL_PARAM;

	  // Check to see if signature specifies Binary Structured
	  // Storage; if so, raw storage must be at least readable, even
	  // for write-only files.
	  if (equalUID (*pFileKind, aafFileKindAafSSBinary))
		{
		  b = kAAFFalse;
		  hr = pRawStorage->IsReadable (&b);
		  if (kAAFFalse == b)
			return AAFRESULT_NOT_READABLE;
		}
	  break;
	case kAAFFileExistence_existing:
	  break;
	default:
	  return AAFRESULT_INVALID_PARAM;
	}

  _access = access;
  _existence = existence;

  AAFRESULT stat = AAFRESULT_SUCCESS;
  try
	{
	  // Create the OM storage...
	  OMRawStorage * pOMStg =
		new ImplAAFOMRawStorage (pRawStorage);
	  assert (pOMStg);

	  if (kAAFFileExistence_new == existence) // new
		{
		  if (! pIdent)
			return AAFRESULT_NULL_PARAM;

		  // Create the header for the OM manager to use as the root
		  // for the file. Use the OMClassFactory interface for this
		  // object because the dictionary has not been associated
		  // with a header yet (Chicken 'n Egg problem).
		  const OMClassId& soid =
			*reinterpret_cast<const OMClassId *>(&AUID_AAFHeader);
		  _head = static_cast<ImplAAFHeader *>(_factory->create(soid));
		  checkExpression(NULL != _head, AAFRESULT_BADHEAD);
		
		  // Make sure the header is initialized with our previously
		  // created dictionary.
		  _head->SetDictionary(_factory);

		  // Set the file format version.
		  //
		  // BobT Fri Jan 21 14:37:43 EST 2000: the default behavior
		  // is that if the version isn't present, it's assumed to
		  // Version 0.  Therefore if the current version is 0, don't
		  // write out the property.  We do this so that hackers
		  // examining written files won't know that a mechanism
		  // exists to mark future incompatible versions, and so will
		  // work harder to make any future changes compatible.
		  if (sCurrentAAFObjectModelVersion)
			{
			  _head->SetObjectModelVersion
				(sCurrentAAFObjectModelVersion);
			}

		  // Add the ident to the header.
		  checkResult(_head->AddIdentificationObject(pIdent));
		  
		  // Set the byte order
		  OMByteOrder byteOrder = hostByteOrder();
		  if (byteOrder == littleEndian)
			{
			  _byteOrder = 0x4949; // 'II'
			}
		  else
			{ // bigEndian
			  _byteOrder = 0x4d4d; // 'MM'
			}
		  _head->SetByteOrder(_byteOrder);
		  aafVersionType_t theVersion = { 1, 0 };
		  _head->SetFileRevision (theVersion);

		  //JeffB!!! We must decide whether def-only files have a
		  //content storage
		  ImplAAFContentStorage * pCStore = 0;
		  checkResult(_head->GetContentStorage(&pCStore));
		  pCStore->ReleaseReference(); // need to release this pointer!
		  pCStore = 0;

		  // Attempt to create the file.
		  const OMFileSignature aafFileSignature =
			*reinterpret_cast<const OMFileSignature *> (pFileKind);

		  if (kAAFFileAccess_read == access)
			{
			  // Can't open an new file for read only.
			  return AAFRESULT_INVALID_PARAM;
			}
		  if (kAAFFileAccess_modify == access)
			{
			  // NewModify
			  if (! OMFile::compatibleRawStorage (pOMStg,
												  OMFile::modifyMode,
												  aafFileSignature))
				return AAFRESULT_INVALID_PARAM;
												  
			  _file = OMFile::openNewModify (pOMStg,
											 _factory,
											 0,
											 byteOrder,
											 _head,
											 aafFileSignature,
											 _metafactory);
			}
		  else // write-only
			{
			  // NewWrite
			  if (! OMFile::compatibleRawStorage (pOMStg,
												  OMFile::writeOnlyMode,
												  aafFileSignature))
				return AAFRESULT_INVALID_PARAM;
												  
			  _file = OMFile::openNewWrite (pOMStg,
											_factory,
											0,
											byteOrder,
											_head,
											aafFileSignature,
											_metafactory);
			}
		}
	  else if (kAAFFileExistence_existing == existence)
		{
		  if (kAAFFileAccess_write == access)
			{
			  // Can't open an existing file for write only.
			  return AAFRESULT_INVALID_PARAM;
			}
		  if (kAAFFileAccess_modify == access)
			{
			  // ExistingModify
			  if (! pIdent)
				return AAFRESULT_NULL_PARAM;
			  _preOpenIdent = *pIdent;

			  _file = OMFile::openExistingModify (pOMStg,
												  _factory,
												  0,
												  OMFile::lazyLoad,
												  _metafactory);
			}
		  else // read-only
			{
			  // ExistingRead
			  _file = OMFile::openExistingRead (pOMStg,
												_factory,
												0,
												OMFile::lazyLoad,
												_metafactory);
			}
		} // endif new/existing
	  else
		{
		  return AAFRESULT_INVALID_PARAM;
		}
	}

  catch (AAFRESULT &r)
	{
	  stat = r;
	}
  return stat;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Open ()
{
  if (IsOpen() || IsClosed())
	return AAFRESULT_ALREADY_OPEN;

  if (! _file)
	return AAFRESULT_NOT_INITIALIZED;

  AAFRESULT stat = AAFRESULT_SUCCESS;

  try
	{
	  _file->open ();
	  // _isOpen = kAAFTrue;

	  if (kAAFFileExistence_new == _existence) // new
		{
		  // Now that the file is open and the header has been
		  // restored, complete the initialization of the
		  // dictionary. We obtain the dictionary via the header
		  // to give the persistence mechanism a chance to work.
		  _factory->InitializeOMStorable
		      (_factory->GetBuiltinDefs()->cdDictionary());
		  ImplAAFDictionary* dictionary = 0;
		  HRESULT hr = _head->GetDictionary(&dictionary);
		  if (hr != AAFRESULT_SUCCESS)
			return hr;
		  dictionary->InitBuiltins();
		  dictionary->ReleaseReference ();
		  dictionary = 0;
		}
	  else if (kAAFFileExistence_existing == _existence) // existing
		{
		  // Check the file's signature.
		  OMFileSignature sig = _file->signature();
		  const OMFileSignature aafFileSignature  =
			*reinterpret_cast<const OMFileSignature *>
			(&aafFileSignatureAafSSBinaryGUID);
		  checkExpression(sig == aafFileSignature, AAFRESULT_NOT_AAF_FILE);

		// Restore the meta dictionary, it should be the same object
		// as _metafactory
		OMDictionary* mf = _file->dictionary();
		assert(mf == _metafactory);

		// Make sure all definitions are present in the meta dictionary
		ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(mf);
		assert(d);
		d->InstantiateAxiomaticDefinitions();

		  // Get the byte order
		  OMByteOrder byteOrder = _file->byteOrder();
		  if (byteOrder == littleEndian)
			{
			  _byteOrder = 0x4949; // 'II'
			}
		  else
			{ // bigEndian
			  _byteOrder = 0x4d4d; // 'MM'
			}

		  // Restore the header.
		  bool regWasEnabled = _factory->SetEnableDefRegistration (false);
		  OMStorable* head = _file->restore();
		  _head = dynamic_cast<ImplAAFHeader *>(head);
		  _head->SetFile(this);
		
		  // Check for file format version.
		  if (_head->IsObjectModelVersionPresent())
			{
			  // If property isn't present, the default version is 0,
			  // which is always (supposed to be) legible.  If it is
			  // present, find out the version number to determine if
			  // the file is legible.
			  if (_head->GetObjectModelVersion() >
				  sCurrentAAFObjectModelVersion)
				{
				  // File version is higher than the version
				  // understood by this toolkit.  Therefore this file
				  // cannot be read.
				  return AAFRESULT_FILEREV_DIFF;
				}
			}
		  // Now that the file is open and the header has been
		  // restored, complete the initialization of the
		  // dictionary. We obtain the dictionary via the header
		  // to give the persistence mechanism a chance to work.
		  ImplAAFDictionary* dictionary = 0;
		  HRESULT hr = _head->GetDictionary(&dictionary);
		  if (hr != AAFRESULT_SUCCESS)
			return hr;
		  dictionary->ReleaseReference ();
		  dictionary = 0;
		  _factory->SetEnableDefRegistration (regWasEnabled);

		  if (IsWriteable())
			{
			  // NOTE: If modifying an existing file WITHOUT an IDNT
			  // object, add a dummy IDNT object to indicate that this
			  // program was not the creator.
			  //
			  aafUInt32	numIdent = 0;
			  checkResult(_head->CountIdentifications(&numIdent));
			  if(numIdent == 0)
				{
				  _head->AddIdentificationObject
					((aafProductIdentification_t *)NULL);
				}
			  // Now, always add the information from THIS application
			  _head->AddIdentificationObject(&_preOpenIdent);
			}
		}
	  else
		{
		  assert (0);
		}
	}

  catch (AAFRESULT &r)
	{
	  stat = r;
	}
  return stat;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Save ()
{
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (!IsOpen())
		return AAFRESULT_NOT_OPEN;

	if (! _file)
		return AAFRESULT_NOT_OPEN;
	if (! _file->isOpen())
		return AAFRESULT_NOT_OPEN;

	// If any new modes are added then the following line will
	// have to be updated.
	if (IsWriteable ()) {
	  // Assure no registration of def objects in dictionary during
	  // save operation
	  ImplAAFDictionarySP dictSP;
	  AAFRESULT hr = _head->GetDictionary(&dictSP);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	  dictSP->AssureClassPropertyTypes ();
	  bool regWasEnabled = dictSP->SetEnableDefRegistration (false);

	  // OMFile::save() allows us to pass a client context to be
	  // regurgitated to the OMStorable::onSave() callback.  We'll use
	  // it to pass the AUID of the latest generation.
	  ImplAAFIdentificationSP pLatestIdent;
	  hr = _head->GetLastIdentification (&pLatestIdent);
	  if (AAFRESULT_FAILED (hr)) return hr;
	  aafUID_t latestGen;
	  hr = pLatestIdent->GetGenerationID (&latestGen);
	  if (AAFRESULT_FAILED (hr)) return hr;
	  _file->saveFile(&latestGen);

	  dictSP->SetEnableDefRegistration (regWasEnabled);

	} else {
	  return AAFRESULT_WRONG_OPENMODE;
	}


	// Record the fact that this file was modified
	_head->SetModified();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::SaveCopyAs (ImplAAFFile * pDestFile)
{
  if (! pDestFile)
	return AAFRESULT_NULL_PARAM;
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Revert ()
{
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (!IsOpen())
		return AAFRESULT_NOT_OPEN;

        assert(_file);
        _file->revert();

	return AAFRESULT_SUCCESS;
}


ImplAAFFile::ImplAAFFile () :
		_cookie(0),
		_file(0),
		_factory(NULL),
		_metafactory(NULL),
		_byteOrder(0),
		// _openType(kOmUndefined),
		_head(NULL),
		_semanticCheckEnable(kAAFFalse),
		_initialized(kAAFFalse),
		// _isOpen(kAAFFalse),
		_existence (0),
		_access (kAAFFileAccess_none),
		_preOpenIdent(kNullIdent)
{
}


ImplAAFFile::~ImplAAFFile ()
{
	InternalReleaseObjects();

	// cleanup the container.
	if (_factory)
	{
		_factory->ReleaseReference();
		_factory = NULL;
	}

	if (_metafactory)
	{
		_metafactory->ReleaseReference();
		_metafactory = NULL;
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
}


//***********************************************************
// METHOD NAME: Close()
//
// DESCRIPTION:
// @mfunc AAFRESULT | AAFFile | Close |
// Closes an AAF file, saving the result.
// @end
//
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::Close ()
{
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (!IsOpen())
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
	//
	// BobT: Keep this around longer!  We'll need GetBits later...
	// delete _file;
	// _file = 0;
  
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
	// _isOpen = kAAFFalse;
	// BobT: don't set this, so we'll remember what open type this
	// file had
	// _openType = kOmUndefined;

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
  if (! header)
	return AAFRESULT_NULL_PARAM;

  if (!IsOpen())
	return AAFRESULT_NOT_OPEN;

  *header = _head;

  // We are returning a copy of the reference counted object.
  if (_head)
	_head->AcquireReference();

  return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFFile::GetDictionary (ImplAAFDictionary ** ppDictionary) const
{
	if (! ppDictionary)
	{
		return AAFRESULT_NULL_PARAM;
	}

	return(_head->GetDictionary(ppDictionary));
}

OMFile * ImplAAFFile::omFile (void)
{
  assert (IsOpen());
  assert (_file);
  return _file;
}


bool ImplAAFFile::IsReadable () const
{
  assert (_file);
  return _file->isReadable();
}

bool ImplAAFFile::IsWriteable () const
{
  assert (_file);
  return _file->isWritable();
}

bool ImplAAFFile::IsOpen () const
{
  if (!_file)
	return false;
  assert (_file);
  return _file->isOpen();
}

bool ImplAAFFile::IsClosed () const
{
  if (!_file)
	return false;
  assert (_file);
  return _file->isClosed();
}

OMRawStorage * ImplAAFFile::RawStorage ()
{
  OMRawStorage * r = 0;
  assert (_file);
  r = _file->rawStorage ();
  assert (r);
  return r;
}

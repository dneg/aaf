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
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFFile.h"

#include "OMFile.h"
#include "OMUtilities.h"
#include "OMClassFactory.h"
#include "OMMemoryRawStorage.h"

#include "OMSSStoredObjectFactory.h"
#include "OMMS_SSStoredObjectFactory.h"
#ifdef USE_LIBGSF
#include "OMGSF_SSStoredObjectFactory.h"
#endif
#include "OMSS_SSStoredObjectFactory.h"
#include "OMXMLStoredObjectFactory.h"
#include "OMKLVStoredObjectFactory.h"

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

extern "C" const aafClassID_t CLSID_AAFRandomRawStorage;

static const aafProductIdentification_t kNullIdent = { 0 };

//
// File Format Version
//
// Update this when incompatible changes are made to AAF file format
// version. 
//
//    kAAFRev1 : Tue Jan 11 17:08:26 EST 2000
//        Initial Release version.
//
//    kAAFRev2 : Wed May 19 19:18:00 EST 2004
//        UInt32Set and AUIDSet types added to built-in model as part
//        of AAF v1.1. This is not compatible with older toolkits.
//
static const aafFileRev_t sCurrentAAFObjectModelVersion = kAAFRev2;


// FileKind from the point of view of the OM
#define ENCODING(x) *reinterpret_cast<const OMStoredObjectEncoding*>(&x)

// these are the installation defaults set in AAFFileKinds.h
#define AAF512Encoding ENCODING(kAAFFileKind_Aaf512Binary)
#define AAF4KEncoding ENCODING(kAAFFileKind_Aaf4KBinary)


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
	AAF_FILE_MODE_LAZY_LOADING |
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

//
// Returns true if all flags set are supported flags; returns false if
// any flags set are *not* supported.
//
static bool areAllModeFlagsSupported (aafUInt32 modeFlags)
{
	//NOTE: Lazy loading and large sector support included
  static const aafUInt32 kSupportedFlags =
	AAF_FILE_MODE_USE_LARGE_SS_SECTORS | AAF_FILE_MODE_LAZY_LOADING;

  if (modeFlags & (~kSupportedFlags))
	{
	  // at least one flag is not supported
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
	OMFile::OMLoadMode	loadMode = OMFile::eagerLoad;	// The default behavior
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

	if (! areAllModeFlagsSupported (modeFlags))
	  return AAFRESULT_NOT_IN_CURRENT_VERSION;

    if( modeFlags & AAF_FILE_MODE_LAZY_LOADING)
	{
		  loadMode = OMFile::lazyLoad;
	}

	// Save the mode flags for now. They are not currently (2/4/1999) used by the
	// OM to open the doc file. Why do we return an error if modeFlags != 0?
	//
	// Answer: because none of them are implemented yet.
	_modeFlags = modeFlags;

	//NOTE: Depending on LARGE sectors flag check encoding 
	if (modeFlags & AAF_FILE_MODE_USE_LARGE_SS_SECTORS)
	{
    	if (!OMFile::hasFactory(AAF4KEncoding))
    	  return AAFRESULT_FILEKIND_NOT_REGISTERED;
	}
	else
	{
    	if (!OMFile::hasFactory(AAF512Encoding))
    	  return AAFRESULT_FILEKIND_NOT_REGISTERED;
	}

	try
	{
		// Ask the OM to open the file.
		_file = OMFile::openExistingRead(pFileName,
										 _factory,
										 0,
										 loadMode,
										 _metafactory);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

		// Restore the meta dictionary, it should be the same object
		// as _metafactory
		OMDictionary* mf = _file->dictionary();
		assert(mf == _metafactory);

		// Make sure all definitions are present in the meta dictionary
		ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(mf);
		assert(d);
		checkResult( d->InstantiateAxiomaticDefinitions() );

		// Make sure properties that exist in builtin class
		// definitions but not the file's class definition,
		// are merged to the file's class definition.
		checkResult( d->MergeBuiltinClassDefs() );

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
				  static_cast<aafUInt32>(sCurrentAAFObjectModelVersion))
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

		if (NULL != _head)
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
	OMFile::OMLoadMode	loadMode = OMFile::eagerLoad;	// The default behavior
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

	if (! areAllModeFlagsSupported (modeFlags))
	  return AAFRESULT_NOT_IN_CURRENT_VERSION;

	if( modeFlags & AAF_FILE_MODE_LAZY_LOADING)
	{
		loadMode = OMFile::lazyLoad;
	}

	// Save the mode flags for now. They are not currently (2/4/1999) used by the
	// OM to open the doc file. Why do we return an error if modeFlags != 0?
	//
	// Answer: because none of them are implemented yet.
	_modeFlags = modeFlags;

	//NOTE: Depending on LARGE sectors flag set encoding 
	if (modeFlags & AAF_FILE_MODE_USE_LARGE_SS_SECTORS)
	{
    	if (!OMFile::hasFactory(AAF4KEncoding))
      		return AAFRESULT_FILEKIND_NOT_REGISTERED;
	}
	else
	{
    	if (!OMFile::hasFactory(AAF512Encoding))
      		return AAFRESULT_FILEKIND_NOT_REGISTERED;
	}


	try 
	{
		// Ask the OM to open the file.
		_file = OMFile::openExistingModify(pFileName,
										   _factory,
										   0,
										   loadMode,
										   _metafactory);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

		// Restore the meta dictionary, it should be the same object
		// as _metafactory
		OMDictionary* mf = _file->dictionary();
		assert(mf == _metafactory);

		// Make sure all definitions are present in the meta dictionary
		ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(mf);
		assert(d);
		checkResult( d->InstantiateAxiomaticDefinitions() );

		// Make sure properties that exist in builtin class
		// definitions but not the file's class definition,
		// are merged to the file's class definition.
		checkResult( d->MergeBuiltinClassDefs() );

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
				  static_cast<aafUInt32>(sCurrentAAFObjectModelVersion))
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

		if (NULL != _head)
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
	aafVersionType_t		theVersion = { 1, 1 };

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

	if (! areAllModeFlagsSupported (modeFlags))
	  return AAFRESULT_NOT_IN_CURRENT_VERSION;

	// modeFlags only in RawStorage API
	// remove when implemented in NamedFile API
	//if( modeFlags )
	 // return AAFRESULT_NOT_IN_CURRENT_VERSION;

	//NOTE: check LARGE_SECTORS flag
	if (modeFlags & AAF_FILE_MODE_USE_LARGE_SS_SECTORS)
	{
    	if (!OMFile::hasFactory(AAF4KEncoding))
    	  return AAFRESULT_FILEKIND_NOT_REGISTERED;
	}
	else
	{
    	if (!OMFile::hasFactory(AAF512Encoding))
    	  return AAFRESULT_FILEKIND_NOT_REGISTERED;
	}

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
		OMStoredObjectEncoding aafFileEncoding;
		

		//NOTE: Depending on LARGE sectors flag set encoding 
		if (modeFlags & AAF_FILE_MODE_USE_LARGE_SS_SECTORS)
		{
			aafFileEncoding	= AAF4KEncoding;
		}
		else
		{
			aafFileEncoding	= AAF512Encoding;
		}
		

		_file = OMFile::openNewModify(pFileName,
									  _factory,
									  0,
									  byteOrder,
									  _head,
									  aafFileEncoding,
									  _metafactory);
		checkExpression(NULL != _file, AAFRESULT_INTERNAL_ERROR);

		// Restore the meta dictionary, it should be the same object
		// as _metafactory
		OMDictionary* mf = _file->dictionary();
		assert(mf == _metafactory);

		// Make sure all definitions are present in the meta dictionary
		ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(mf);
		assert(d);
		checkResult( d->InstantiateAxiomaticDefinitions() );

		// Make sure properties that exist in builtin class
		// definitions but not the file's class definition,
		// are merged to the file's class definition.
		checkResult( d->MergeBuiltinClassDefs() );

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

		if (NULL != _head)
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

	OMRawStorage * pOMRawStg = 0;

	try {
		//
		// Create a memory backed ImplAAFRawStorage
		//

		pOMRawStg = OMMemoryRawStorage::openNewModify ();
		if (!pOMRawStg) {
			throw AAFRESULT_NOMEMORY;
		}

		//
		// Create a file on top of the memory backed storage. 
		//

		_access = kAAFFileAccess_modify;
		_existence = kAAFFileExistence_new;

		// The following code fragment originated in
		// CreateAAFFileOnRawStorage().  It should probably be
		// factored out and made reusable.  Similar code
		// fragments appear in other ImplAAFFile methods as
		// well.

		// start of CreateAAFFileOnRawStorage fragment

		// Create and init the header;
		const OMClassId& soid =
		  *reinterpret_cast<const OMClassId *>(&AUID_AAFHeader);
		_head = static_cast<ImplAAFHeader *>(_factory->create(soid));
		checkExpression(NULL != _head, AAFRESULT_BADHEAD);
		_head->SetDictionary(_factory);
		if (sCurrentAAFObjectModelVersion) {
		  _head->SetObjectModelVersion(sCurrentAAFObjectModelVersion);
		}
		
		// Set the byte order
		OMByteOrder byteOrder = hostByteOrder();
		if (byteOrder == littleEndian) {
		  _byteOrder = 0x4949; // 'II'
		}
		else { // bigEndian
		  _byteOrder = 0x4d4d; // 'MM'
		}
		_head->SetByteOrder(_byteOrder);
		// FIXME FIXME - Hardcoded version!!!
		aafVersionType_t theVersion = { 1, 1 };
		_head->SetFileRevision (theVersion);
		
		ImplAAFContentStorage * pCStore = 0;
 	        checkResult(_head->GetContentStorage(&pCStore));
		pCStore->ReleaseReference();
		pCStore = 0;
		
		// Attempt to create the file.
		const OMStoredObjectEncoding aafFileEncoding = AAF512Encoding;
		  
		_file = OMFile::openNewModify (pOMRawStg,
					       _factory,
					       0,
					       byteOrder,
					       _head,
					       aafFileEncoding,
					       _metafactory);

		Open();

		// end of CreateAAFFileOnRawStorage fragment						  

		// Add the ident to the header.
		checkResult( _head->AddIdentificationObject(pIdent) );

	}
	catch ( AAFRESULT& ex ) {
		if ( pOMRawStg ) {
			delete pOMRawStg;
		}

		return ex;
	}
	return AAFRESULT_SUCCESS;
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
 	OMFile::OMLoadMode	loadMode = OMFile::eagerLoad;	// The default behavior

  if (! _initialized)
	return AAFRESULT_NOT_INITIALIZED;

  if (! pRawStorage)
	return AAFRESULT_NULL_PARAM;

  if (IsOpen())
	return AAFRESULT_ALREADY_OPEN;

  if (! areAllModeFlagsDefined (modeFlags))
	return AAFRESULT_BAD_FLAGS;

	if (! areAllModeFlagsSupported (modeFlags))
	  return AAFRESULT_NOT_IN_CURRENT_VERSION;

  if (modeFlags & AAF_FILE_MODE_LAZY_LOADING)
      loadMode = OMFile::lazyLoad;

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

      if (!OMFile::hasFactory(ENCODING(*pFileKind)))
        return AAFRESULT_FILEKIND_NOT_REGISTERED;

	  break;
	case kAAFFileExistence_existing:
		// In this case, pFileKind selects the implementation used to read the existing file.
		// A NULL pFileKind argument was acceptable in previous toolkits when this selection was
		// not available.  Now, treat pFileKind as DontCare.
	  if (! pFileKind)
		  pFileKind = &kAAFFileKind_DontCare;

		// can specify DontCare or a required FileKind on open existing
      if (!equalUID(*pFileKind,kAAFFileKind_DontCare) && !OMFile::hasFactory(ENCODING(*pFileKind)))
	      return AAFRESULT_FILEKIND_NOT_REGISTERED;

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
		  aafVersionType_t theVersion = { 1, 1 };
		  _head->SetFileRevision (theVersion);

		  //JeffB!!! We must decide whether def-only files have a
		  //content storage
		  ImplAAFContentStorage * pCStore = 0;
		  checkResult(_head->GetContentStorage(&pCStore));
		  pCStore->ReleaseReference(); // need to release this pointer!
		  pCStore = 0;

		  // Attempt to create the file.
		  const OMStoredObjectEncoding aafFileEncoding = ENCODING(*pFileKind);
		  // replaces const OMStoredObjectEncoding aafFileEncoding = *reinterpret_cast<const OMStoredObjectEncoding*> (pFileKind);

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
												  aafFileEncoding))
				return AAFRESULT_INVALID_PARAM;
												  
			  _file = OMFile::openNewModify (pOMStg,
											 _factory,
											 0,
											 byteOrder,
											 _head,
											 aafFileEncoding,
											 _metafactory);
			}
		  else // write-only
			{
			  // NewWrite
			  if (! OMFile::compatibleRawStorage (pOMStg,
												  OMFile::writeOnlyMode,
												  aafFileEncoding))
				return AAFRESULT_INVALID_PARAM;
												  
			  _file = OMFile::openNewWrite (pOMStg,
											_factory,
											0,
											byteOrder,
											_head,
											aafFileEncoding,
											_metafactory);
			}
		}
	  else if (kAAFFileExistence_existing == existence) // Existing
		{
		  if (kAAFFileAccess_write == access)
			{
			  // Can't open an existing file for write only.
			  return AAFRESULT_INVALID_PARAM;
			}

		  const OMStoredObjectEncoding aafFileEncoding = ENCODING(*pFileKind);

		  if (kAAFFileAccess_modify == access)
			{
			  // ExistingModify
			  if (! pIdent)
				return AAFRESULT_NULL_PARAM;
			  _preOpenIdent = *pIdent;

			  _file = OMFile::openExistingModify (pOMStg,
												  _factory,
												  0,
												  loadMode,
													aafFileEncoding,
												  _metafactory);
			}
		  else // read-only
			{
			  // ExistingRead
			  _file = OMFile::openExistingRead (pOMStg,
												_factory,
												0,
												loadMode,
												aafFileEncoding,
												_metafactory);

			  // Make sure properties that exist in builtin class
			  // definitions but not the file's class definition,
			  // are merged to the file's class definition.
			  ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(_metafactory);
			  assert(d);
			  checkResult( d->MergeBuiltinClassDefs() );

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
		// Restore the meta dictionary, it should be the same object
		// as _metafactory
		OMDictionary* mf = _file->dictionary();
		assert(mf == _metafactory);

		// Make sure all definitions are present in the meta dictionary
		ImplAAFMetaDictionary* d = dynamic_cast<ImplAAFMetaDictionary*>(mf);
		assert(d);
		checkResult( d->InstantiateAxiomaticDefinitions() );

		// Make sure properties that exist in builtin class
		// definitions but not the file's class definition,
		// are merged to the file's class definition.
		checkResult( d->MergeBuiltinClassDefs() );

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
				  static_cast<aafUInt32>(sCurrentAAFObjectModelVersion))
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
		  _factory->SetEnableDefRegistration (regWasEnabled);
		  dictionary->InitBuiltins();
		  dictionary->ReleaseReference ();
		  dictionary = 0;

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
  // Save all objects in this file to pDestFile.  Implemented by
  // cloning the source header's content store and
  // _identificationList.  The source identification objects are
  // appended to pDstFile's existing identification list. The
  // destination header's _byteOrder, _lastModified, _dictionary,
  // _fileRef, and _objectModelVersion are not cloned - they all take
  // care of themselves.  The destination dictionary is updated as a
  // side effect of cloning the content store and ident list.

  if (! pDestFile)
	return AAFRESULT_NULL_PARAM;

  if ( this == pDestFile ) {
    // The call is redundant.  Just save the file.
    return Save();
  }

  try {
    ImplAAFSmartPointer<ImplAAFHeader> spDstHeader;
    checkResult( pDestFile->GetHeader(&spDstHeader) );

    ImplAAFSmartPointer<ImplAAFContentStorage> spDstContentStore;
    checkResult( spDstHeader->GetContentStorage(&spDstContentStore) );

    aafUInt32 numDstMobs = 0;
    checkResult( spDstContentStore->CountMobs(0, &numDstMobs) );

    aafUInt32 numDstEssenceData = 0;
    checkResult( spDstContentStore->CountEssenceData(&numDstEssenceData) );

    if ( numDstMobs || numDstEssenceData ) {
      throw AAFRESULT_OPERATION_NOT_PERMITTED;
    }

    ImplAAFSmartPointer<ImplAAFDictionary> spDstDict;
    checkResult( spDstHeader->GetDictionary(&spDstDict) );

    // Merge the dictionary
    {
      ImplAAFSmartPointer<ImplAAFDictionary> spSrcDict;
      checkResult( GetDictionary(&spSrcDict) );

      checkResult( spSrcDict->MergeTo( spDstDict ) );
    }

    // Clone the content store.
    {
      ImplAAFSmartPointer<ImplAAFContentStorage> spSrcContentStore;
      checkResult( _head->GetContentStorage( &spSrcContentStore ) );

      OMStorable* pNewDstStorable = spSrcContentStore->shallowCopy(static_cast<ImplAAFDictionary*>(spDstDict));
      ImplAAFContentStorage* pNewDstStorage = dynamic_cast<ImplAAFContentStorage*>(pNewDstStorable);
      if ( !pNewDstStorage ) {
		throw AAFRESULT_BAD_TYPE;
      }
      
      spDstHeader->SetContentStorage( pNewDstStorage );

      spSrcContentStore->deepCopyTo( pNewDstStorable, 0 );
      pNewDstStorage->onCopy( 0 );

    }

    // Clone the ident list.
    {
      aafUInt32 numSrcIdents = 0;
      checkResult( _head->CountIdentifications(&numSrcIdents) );
      
      unsigned int i;
      for( i = 0; i < numSrcIdents; ++i ) {

	ImplAAFSmartPointer<ImplAAFIdentification> spSrcIdent;
	checkResult( _head->GetIdentificationAt( i, &spSrcIdent ) );
	
	OMStorable* pNewDstStorable = spSrcIdent->shallowCopy(static_cast<ImplAAFDictionary*>(spDstDict));
	ImplAAFIdentification* pNewDstIdent = dynamic_cast<ImplAAFIdentification*>(pNewDstStorable);
	if ( !pNewDstIdent ) {
	  throw AAFRESULT_BAD_TYPE;
	}
	
	checkResult( spDstHeader->AppendIdentification( pNewDstIdent ) );

	pNewDstIdent->onCopy( 0 );
       	spSrcIdent->deepCopyTo( pNewDstIdent, 0 );

      }
    }

  }
  catch( const HRESULT& ex ) {
    return ex;
  }

  return pDestFile->Save();
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
// Get the revision of the current AAF file.
// @end
// 
AAFRESULT STDMETHODCALLTYPE
ImplAAFFile::GetRevision (aafFileRev_t *  rev)
{
	if (! _initialized)
		return AAFRESULT_NOT_INITIALIZED;

	if (! rev)
		return AAFRESULT_NULL_PARAM;

	ImplAAFHeader* header;
	AAFRESULT hr = GetHeader(&header);
	if (hr != AAFRESULT_SUCCESS)
		  return hr;

	aafUInt32 ObjectModelVersion = 0;
	if (header->IsObjectModelVersionPresent())
			ObjectModelVersion = header->GetObjectModelVersion();
	header->ReleaseReference();
	header=0;

	*rev = ObjectModelVersion;

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

void ImplAAFFile::removeFactories(void)
{
  OMFile::removeAllFactories();
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


// FileKinds from the point of view of the OM
#define AAFM512Encoding ENCODING(kAAFFileKind_AafM512Binary)
#define AAFS512Encoding ENCODING(kAAFFileKind_AafS512Binary)
#define AAFG512Encoding ENCODING(kAAFFileKind_AafG512Binary)
#define AAFS4KEncoding ENCODING(kAAFFileKind_AafS4KBinary)
#define AAFM4KEncoding ENCODING(kAAFFileKind_AafM4KBinary)
#define AAFG4KEncoding ENCODING(kAAFFileKind_AafG4KBinary)

// these are only prototype
#define AAFXMLEncoding ENCODING(kAAFFileKind_AafXmlText)
#define AAFKLVEncoding ENCODING(kAAFFileKind_AafKlvBinary)

// signatures from the point of view of the OM
#define Signature_SSBin_512 ENCODING(kAAFSignature_Aaf512Binary)
#define Signature_SSBin_4K ENCODING(kAAFSignature_Aaf4KBinary)

void ImplAAFFile::registerFactories(void)
{
	// these are in order of preference for the installation

#if defined( OS_WINDOWS )
// DEFAULT is Schemasoft 512

	assert( equalUID( kAAFFileKind_Aaf512Binary, kAAFFileKind_AafS512Binary));
	assert( equalUID( kAAFFileKind_Aaf4KBinary, kAAFFileKind_AafS4KBinary));

	OMFile::registerFactory(AAFS512Encoding,
                          new OMSS_SSStoredObjectFactory(AAFS512Encoding,
                                                       Signature_SSBin_512,
                                                       L"AAF-S",
                                                       L"AAF Schemasoft SS"));
	OMFile::registerFactory(AAFM512Encoding,
                          new OMMS_SSStoredObjectFactory(AAFM512Encoding,
                                                       Signature_SSBin_512,
                                                       L"AAF-M",
                                                       L"AAF Microsoft SS"));
	OMFile::registerFactory(AAFS4KEncoding,
                          new OMSS_SSStoredObjectFactory(AAFS4KEncoding,
                                                       Signature_SSBin_4K,
                                                       L"AAF-S4K",
                                                       L"AAF Schemasoft 4K"));
	OMFile::registerFactory(AAFM4KEncoding,
                          new OMMS_SSStoredObjectFactory(AAFM4KEncoding,
                                                       Signature_SSBin_4K,
                                                       L"AAF-M4K",
                                                       L"AAF Microsoft 4K"));

#elif defined( OS_UNIX )

// libgsf is available on all UNIX platforms
#ifdef USE_LIBGSF

	// If LIBGSF support is explicitly requested, GSF SS 512 is the default
	assert( equalUID( kAAFFileKind_Aaf512Binary, kAAFFileKind_AafG512Binary));
	assert( equalUID( kAAFFileKind_Aaf4KBinary, kAAFFileKind_AafG4KBinary));

	OMFile::registerFactory(AAFG512Encoding,
                          new OMGSF_SSStoredObjectFactory(AAFG512Encoding,
                                                       Signature_SSBin_512,
                                                       L"AAF-G",
                                                       L"AAF GSF SS"));
	OMFile::registerFactory(AAFG4KEncoding,
                          new OMGSF_SSStoredObjectFactory(AAFG4KEncoding,
                                                       Signature_SSBin_4K,
                                                       L"AAF-G4K",
                                                       L"AAF GSF 4K"));
#else

// The Schemasoft precompiled library is only available on these platforms
#if defined( OS_DARWIN ) || defined( OS_IRIX ) || defined( OS_LINUX ) || defined( OS_SOLARIS )

	// DEFAULT is SchemaSoft 512
	assert( equalUID( kAAFFileKind_Aaf512Binary, kAAFFileKind_AafS512Binary));
	assert( equalUID( kAAFFileKind_Aaf4KBinary, kAAFFileKind_AafS4KBinary));

	OMFile::registerFactory(AAFS512Encoding, 
                          new OMSS_SSStoredObjectFactory(AAFS512Encoding,
                                                       Signature_SSBin_512,
                                                       L"AAF-S",
                                                       L"AAF Schemasoft SS"));
	OMFile::registerFactory(AAFS4KEncoding,
                          new OMSS_SSStoredObjectFactory(AAFS4KEncoding,
                                                       Signature_SSBin_4K,
                                                       L"AAF-S4K",
                                                       L"AAF Schemasoft 4K"));
#else
#error No SS implementation available on platform
#endif // list of SchemaSoft library platforms

#endif // USE_LIBGSF

#endif // OS_WINDOWS,OS_UNIX
}

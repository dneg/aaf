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

////////////////////////////////////////////////////////////////////////////////
// @doc
//
// @module AAFMODULE.CPP - AAF Factory functions for AAF DLL Module |
//
// Implementation of DLL Exports
// 
// @end
//
// Tom Ransdell, Avid AAF Development Team
// Plugin manager call added by Jeffrey Bedell.
//
////////////////////////////////////////////////////////////////////////////////

#include "ImplAAFModule.h"

#include "AAFPrivate.h"
#include "AAFResult.h"
#include "ImplAAFPluginManager.h"
#include "ImplAAFFile.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFRandomRawStorage.h"

#include "OMRawStorage.h"
#include "OMDiskRawStorage.h"
#include "OMMemoryRawStorage.h"

#include "ImplAAFSmartPointer.h"

typedef ImplAAFSmartPointer<ImplAAFFile> ImplAAFFileSP;

extern "C" const aafClassID_t CLSID_AAFFile;
extern "C" const aafClassID_t CLSID_AAFRandomFile;
extern "C" const aafClassID_t CLSID_AAFRawStorage;
extern "C" const aafClassID_t CLSID_AAFRandomRawStorage;

#include <assert.h>


// The first release of AAF (v1.0) does not fully support read/modify/write
// (a.k.a. edit-in-place).
#ifndef READMODIFYWRITE_SUPPORTED
#define READMODIFYWRITE_SUPPORTED 0
#endif


// Define this to 1 to use raw storage implementations of the
// traditional AAFFileOpenXXX(filename) methods.
#ifndef USE_RAW_STORAGE
#define USE_RAW_STORAGE 0
#endif // ! USE_RAW_STORAGE


#if USE_RAW_STORAGE
#include "AAFFileKinds.h"
#include "AAF.h"
#endif


//***********************************************************
//
// AAFLoad()
//
// This is just a stub since the dll will already be loaded
// if the client code has linked directly to the export library.
//
STDAPI ImplAAFLoad(const char * /*dllname*/)
{
  return S_OK;
}



//***********************************************************
//
// AAFUnload()
//
// This is just a stub since the dll will be unloaded
// when the client code terminates if the client code has 
// linked directly to the export library.
//
STDAPI ImplAAFUnload()
{
  return S_OK;
}



//***********************************************************
//
// AAFFileOpenExistingRead()
//
// Creates an object associated with with an existing
// filesystem file that contains data which is only to be read.
// Does the following:
// - Opens the existing named file in the filesystem for reading.
// - Associates an object with that filesystem file.
// - Places the object into the Open-read-only state.
// - This AAFFile object then can be used as the root of the
//   containment tree representing all AAF objects contained within
//   the file.
//
// Succeeds if:
// - The pFileName argument is valid.
// - Only valid flags have been specified.
// - A valid combination of flags has been specified.
// - The named file exists in the filesystem.
// - The named filesystem file is readable.
// - The named file represents itself as a valid AAF file.  Even if
//   this succeeds, it is not guaranteed that the named file is in
//   fact a valid AAF file.
//
// This function will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - the pFileName pointer arg is NULL.
//
// AAFRESULT_BAD_FLAGS
//   - one or more illegal flags were specified.
//
// AAFRESULT_BAD_FLAG_COMBINATION
//   - illegal combination of otherwise valid flags was specified.
//
// AAFRESULT_FILE_NOT_FOUND
//   - The named file does not exist in the filesystem.
//
// AAFRESULT_NOT_READABLE
//   - The named file cannot be read.
//
// AAFRESULT_NOT_AAF_FILE
//   - The named file does not claim to be a valid AAF file.
// 
STDAPI ImplAAFFileOpenExistingRead (
  // Null-terminated string containing name of filesystem file to be
  // opened for reading.  Filename must be in a form that would be
  // acceptable to StgOpenStorage() for this platform.
  /*[in, string]*/ const aafCharacter *  pFileName,

  // File open mode flags.  May be any of the following ORed together.
  // All other bits must be set to zero.
  //  - kAAFFileModeUnbuffered - to indicate buffered mode.  Default
  //    is buffered.
  /*[in]*/ aafUInt32  modeFlags,

  // Pointer to buffer to receive pointer to new file.
  /*[out]*/ ImplAAFFile ** ppFile)
{
#if USE_RAW_STORAGE
  IAAFRawStorage * pRawStg = 0;
  AAFRESULT hr = AAFCreateRawStorageDisk
	(pFileName,
	 kAAFFileExistence_existing,
	 kAAFFileAccess_read,
	 &pRawStg);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  hr = ImplAAFCreateAAFFileOnRawStorage
		(pRawStg,
		 kAAFFileExistence_existing,
		 kAAFFileAccess_read,
		 &aafFileKindAafSSBinary,
		 modeFlags,
		 0,
		 ppFile);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  assert (ppFile);
		  assert (*ppFile);
		  hr = (*ppFile)->Open ();
		}
	}
  if (pRawStg)
	{
	  pRawStg->Release ();
	}
  return hr;

#else // ! USE_RAW_STORAGE

  HRESULT hr = S_OK;
  ImplAAFFile * pFile = 0;

  if (!pFileName || !ppFile)
    return AAFRESULT_NULL_PARAM;

  // Initialize the out parameter.
  *ppFile = 0;

  //
  // For backwards compatibility with existing client code
  // the first checked in version of this function is implemented
  // the same as the old client code which this function is 
  // intended to replace...
  // 

  // Create an instance of an uninitialized file object.
  pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFFile));
	if(!pFile)
		hr = AAFRESULT_NOMEMORY;
  else
  {
    // Make sure the file is initialized (not open yet...)
    hr = pFile->Initialize();
    if (SUCCEEDED(hr))
    {
      // Attempt to open the file read only.
      hr = pFile->OpenExistingRead(pFileName, modeFlags);
      if (SUCCEEDED(hr))
      {
       *ppFile = pFile;
       pFile = 0;
      }
    }

    // Cleanup the file if it could not be initialized and opened.
    if (FAILED(hr) && pFile)
      pFile->ReleaseReference();
  }

  return hr;

#endif // USE_RAW_STORAGE
}


//***********************************************************
//
// AAFFileOpenExistingModify()
//
// Creates an object associated with with an existing
// filesystem file that contains data which is to be read and
// written.  Associates the given identification with it.
// Does the following:
// - Opens the existing named file in the filesystem for reading and
//   writing.
// - Associates an object with that filesystem file.
// - Places the object into the Open-read-write  state.
// - This AAFFile object then can be used as the root of the
//   containment tree representing all AAF objects contained within
//   the file.
//
// Succeeds if:
// - This object is currently Closed.
// - The pFileName argument is valid.
// - The pIdent argument is valid.
// - Only valid flags have been specified.
// - A valid combination of flags has been specified.
// - The named file exists in the filesystem.
// - The named filesystem file is readable.
// - The named filesystem file is writable.
// - The named file represents itself as a valid AAF file.  Even if
//   this succeeds, it is not guaranteed that the named file is in
//   fact a valid AAF file.
//
// This function will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - either pFileName or pIdent pointer argument is NULL.
//
// AAFRESULT_BAD_FLAGS
//   - one or more illegal flags were specified.
//
// AAFRESULT_BAD_FLAG_COMBINATION
//   - illegal combination of otherwise valid flags was specified.
//
// AAFRESULT_FILE_NOT_FOUND
//   - The named file does not exist in the filesystem.
//
// AAFRESULT_NOT_READABLE
//   - The named file cannot be read.
//
// AAFRESULT_NOT_WRITEABLE
//   - The named file cannot be written.
//
// AAFRESULT_NOTAAFFILE
//   - The named file is not a valid AAF file.
// 

#if ! READMODIFYWRITE_SUPPORTED
	
STDAPI ImplAAFFileOpenExistingModify (
  const aafCharacter *,
  aafUInt32,
  aafProductIdentification_t *,
  ImplAAFFile **)
{
	return AAFRESULT_NOT_IN_CURRENT_VERSION;
}

#else // #if ! READMODIFYWRITE_SUPPORTED	

STDAPI ImplAAFFileOpenExistingModify (
  // Null-terminated string containing name of filesystem file to be
  // opened for modification.  Filename must be in a form that would
  // be acceptable to StgOpenStorage() for this platform.
  /*[in, string]*/ const aafCharacter *  pFileName,

  // File open mode flags.  May be any of the following ORed together.
  // All other bits must be set to zero.
  //  - kAAFFileModeUnbuffered - to indicate unbuffered mode.
  //    Default is buffered.
  //  - kAAFFileModeRevertable - to indicate that Revert is possible
  //    on this file (for all changes except those to essence).
  /*[in]*/ aafUInt32  modeFlags,

  // Identification of the application which is modifying this file.
  /*[in]*/ aafProductIdentification_t *  pIdent,

  // Pointer to buffer to receive pointer to new file.
  /*[out]*/ ImplAAFFile ** ppFile)
{
#if USE_RAW_STORAGE
  IAAFRawStorage * pRawStg = 0;
  AAFRESULT hr = AAFCreateRawStorageDisk
	(pFileName,
	 kAAFFileExistence_existing,
	 kAAFFileAccess_modify,
	 &pRawStg);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  hr = ImplAAFCreateAAFFileOnRawStorage
		(pRawStg,
		 kAAFFileExistence_existing,
		 kAAFFileAccess_modify,
		 &aafFileKindAafSSBinary,
		 modeFlags,
		 pIdent,
		 ppFile);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  assert (ppFile);
		  assert (*ppFile);
		  hr = (*ppFile)->Open ();
		}
	}
  if (pRawStg)
	{
	  pRawStg->Release ();
	}
  return hr;

#else // ! USE_RAW_STORAGE

  HRESULT hr = S_OK;
  ImplAAFFile * pFile = 0;

  if (!pFileName || !pIdent || !ppFile)
    return AAFRESULT_NULL_PARAM;

  // Initialize the out parameter.
  *ppFile = 0;

  //
  // For backwards compatibility with existing client code
  // the first checked in version of this function is implemented
  // the same as the old client code which this function is 
  // intended to replace...
  // 

  // Create an instance of an uninitialized file object.
  pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFFile));
	if(!pFile)
		hr = AAFRESULT_NOMEMORY;
  else
  {
    // Make sure the file is initialized (not open yet...)
    hr = pFile->Initialize();
    if (SUCCEEDED(hr))
    {
      // Attempt to open the file for modification.
      hr = pFile->OpenExistingModify(pFileName, modeFlags, pIdent);
      if (SUCCEEDED(hr))
      {
        *ppFile = pFile;
        pFile = 0;
      }
    }

    // Cleanup the file if it could not be initialized and opened.
    if (FAILED(hr) && pFile)
      pFile->ReleaseReference();
  }

  return hr;
#endif // USE_RAW_STORAGE
}

#endif // #else // #if ! READMODIFYWRITE_SUPPORTED	



//***********************************************************
//
// AAFFileOpenNewModify()
//
// Creates an object associated with with a new
// filesystem file that contains data which is to be read and
// written.  Associates the given identification with it.
// Does the following:
// - Creates a new file in the filesystem with the given name.
// - Opens the filesystem file for reading and writing.
// - Associates this object with that filesystem file.
// - Places this object into the Open state.
// - This AAFFile object then can be used as the root of the
//   containment tree representing all AAF objects contained within
//   the file.
//
// Succeeds if:
// - The pFileName argument is valid.
// - The pIdent argument is valid.
// - Only valid flags have been specified.
// - A valid combination of flags has been specified.
// - The named file does not exists in the filesystem.
// - There is sufficient space in the filesystem to create the file.
//
// This function will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - either pFileName or pIdent pointer argument is NULL.
//
// AAFRESULT_BAD_FLAGS
//   - one or more illegal flags were specified.
//
// AAFRESULT_BAD_FLAG_COMBINATION
//   - illegal combination of otherwise valid flags was specified.
//
// AAFRESULT_FILE_EXISTS
//   - The named file already exists in the filesystem.
//
// AAFRESULT_INSUFFICIENT_SPACE
//   - There is insufficient space in the filesystem to create this
//     file.
// 
STDAPI ImplAAFFileOpenNewModify (
  // Null-terminated string containing name of filesystem file to be
  // opened for modification.  Filename must be in a form that would
  // be acceptable to StgOpenStorage() for this platform.
  /*[in, string]*/ const aafCharacter *  pFileName,

  // File open mode flags.  May be any of the following ORed together.
  // All other bits must be set to zero.
  //  - kAAFFileModeUnbuffered - to indicate unbuffered mode.
  //    Default is buffered.
  //  - kAAFFileModeRevertable - to indicate that Revert is possible
  //    on this file (for all changes except those to essence).
  /*[in]*/ aafUInt32  modeFlags,

  // Identification of the application which is creating this file.
  /*[in]*/ aafProductIdentification_t *  pIdent,

  // Pointer to buffer to receive pointer to new file.
  /*[out]*/ ImplAAFFile ** ppFile)
{
#if USE_RAW_STORAGE
  IAAFRawStorage * pRawStg;
  AAFRESULT hr = AAFCreateRawStorageDisk
	(pFileName,
	 kAAFFileExistence_new,
	 kAAFFileAccess_modify,
	 &pRawStg);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  hr = ImplAAFCreateAAFFileOnRawStorage
		(pRawStg,
		 kAAFFileExistence_new,
		 kAAFFileAccess_modify,
		 &aafFileKindAafSSBinary,
		 modeFlags,
		 pIdent,
		 ppFile);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  assert (ppFile);
		  assert (*ppFile);
		  hr = (*ppFile)->Open ();
		}
	}
  if (pRawStg)
	{
	  pRawStg->Release ();
	}
  return hr;

#else // ! USE_RAW_STORAGE

  HRESULT hr = S_OK;
  ImplAAFFile * pFile = 0;

  if (!pFileName || !pIdent || !ppFile)
    return AAFRESULT_NULL_PARAM;

  // Initialize the out parameter.
  *ppFile = 0;

  //
  // For backwards compatibility with existing client code
  // the first checked in version of this function is implemented
  // the same as the old client code which this function is 
  // intended to replace...
  // 

  // Create an instance of an uninitialized file object.
  pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFFile));
	if(!pFile)
		hr = AAFRESULT_NOMEMORY;
  else
  {
    // Make sure the file is initialized (not open yet...)
    hr = pFile->Initialize();
    if (SUCCEEDED(hr))
    {
      // Attempt to open a new file for modification.
      hr = pFile->OpenNewModify(pFileName, modeFlags, pIdent);
      if (SUCCEEDED(hr))
      {
        *ppFile = pFile;
        pFile = 0;
      }
    }

    // Cleanup the file if it could not be initialized and opened.
    if (FAILED(hr) && pFile)
      pFile->ReleaseReference();
  }

  return hr;
#endif // USE_RAW_STORAGE
}



//***********************************************************
//
// AAFFileOpenTransient()
//
// Creates an object associated with with a transient file,
// meaning that it is not associated with any filesystem file but
// may still be used to contain AAF objects as if it was associated
// with a filesystem file.  Associates the given identification with
// this file.
//
// Transient files are never considered Revertable.
//
// NOTE! Stub only.   Implementation not yet added.
//
// Succeeds if:
// - This object has already been Initialize()d.
// - The pIdent argument is valid.
// - This object is currently closed.
//
// This function will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NOT_INITIALIZED
//   - This object has not yet had Initialize() called on it.
//
// AAFRESULT_ALREADY_OPEN
//   - This object is already open.
//
// AAFRESULT_NULL_PARAM
//   - the pIdent pointer argument is NULL.
// 
STDAPI ImplAAFFileOpenTransient (
  // Identification which is to be associated with this file.
  /*[in]*/ aafProductIdentification_t *  pIdent,

  // Pointer to buffer to receive pointer to new file.
  /*[out]*/ ImplAAFFile ** ppFile)
{
#if USE_RAW_STORAGE
  IAAFRawStorage * pRawStg;
  AAFRESULT hr = AAFCreateRawStorageMemory
	(kAAFFileAccess_modify,
	 &pRawStg);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  hr = ImplAAFCreateAAFFileOnRawStorage
		(pRawStg,
		 kAAFFileExistence_new,
		 kAAFFileAccess_modify,
		 &aafFileKindAafSSBinary,
		 0,
		 pIdent,
		 ppFile);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  assert (ppFile);
		  assert (*ppFile);
		  hr = (*ppFile)->Open ();
		}
	}
  if (pRawStg)
	{
	  pRawStg->Release ();
	}
  return hr;

#else // ! USE_RAW_STORAGE

  HRESULT hr = S_OK;
	const aafClassID_t& fileID = *reinterpret_cast<const aafClassID_t *>(&CLSID_AAFFile); 
  ImplAAFFile * pFile = 0;

  if (!pIdent || !ppFile)
    return AAFRESULT_NULL_PARAM;

  // Initialize the out parameter.
  *ppFile = 0;

  //
  // For backwards compatibility with existing client code
  // the first checked in version of this function is implemented
  // the same as the old client code which this function is 
  // intended to replace...
  // 

  // Create an instance of an uninitialized file object.
  pFile = static_cast<ImplAAFFile *>(::CreateImpl(fileID));
	if(!pFile)
		hr = AAFRESULT_NOMEMORY;
  else
  {
    // Make sure the file is initialized (not open yet...)
    hr = pFile->Initialize();
    if (SUCCEEDED(hr))
    {
      // Attempt to open a new transient file.
      hr = pFile->OpenTransient(pIdent);
      if (SUCCEEDED(hr))
      {
        *ppFile = pFile;
        pFile = 0;
      }
    }

    // Cleanup the file if it could not be initialized and opened.
    if (FAILED(hr) && pFile)
      pFile->ReleaseReference();
  }

  return hr;
#endif // USE_RAW_STORAGE
}

//***********************************************************
//
// AAFFileIsAAFFile()
//
// This function is implemented in aaflib. This stub is here only
// because the dodo generated CAAFModule.cpp insists on it. This stub
// is not and should not be called.
//
STDAPI ImplAAFFileIsAAFFile (
  const aafCharacter *  /* pFileName */,
  aafUID_t * /* pAAFFileKind */,
  aafBool *  /* pFileIsAAFFile */)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

//***********************************************************
//
// AAFLoadPluginManager()
//
// Returns a instance of the singleton plugin manager.
//
// This function will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - the pIdent pointer argument is NULL.
// 
STDAPI ImplAAFGetPluginManager (
  /*[out]*/ ImplAAFPluginManager ** ppManager)
{
	HRESULT					hr = S_OK;
	ImplAAFPluginManager	*implMgr;
	
	if (!ppManager)
		return AAFRESULT_NULL_PARAM;
	
	// Initialize the out parameter.
	*ppManager = 0;
	
	implMgr = ImplAAFPluginManager::GetPluginManager();
  if (!implMgr)
    hr = AAFRESULT_NOMEMORY;
  else
    *ppManager = implMgr;
	
	return hr;
}


STDAPI
ImplAAFCreateRawStorageMemory
  (aafFileAccess_t access,
   ImplAAFRawStorage ** ppNewRawStorage)
{
  if (! ppNewRawStorage)
	return AAFRESULT_NULL_PARAM;

  OMRawStorage * stg = 0;
  stg = OMMemoryRawStorage::openNewModify ();

  assert (stg);
  ImplAAFRawStorage * prs = 0;
  if (stg->isPositionable ())
	prs = static_cast<ImplAAFRawStorage *>
	  (::CreateImpl(CLSID_AAFRandomRawStorage));
  else
	prs = static_cast<ImplAAFRawStorage *>
	  (::CreateImpl(CLSID_AAFRawStorage));

  if(!prs)
	{
	  delete stg;
	  return AAFRESULT_NOMEMORY;
	}

  prs->Initialize (stg, access);

  assert (ppNewRawStorage);
  *ppNewRawStorage = prs;
  return AAFRESULT_SUCCESS;
}


STDAPI
ImplAAFCreateRawStorageDisk
  (aafCharacter_constptr pFilename,
   aafFileExistence_t existence,
   aafFileAccess_t access,
   ImplAAFRawStorage ** ppNewRawStorage)
{
  if (! pFilename)
	return AAFRESULT_NULL_PARAM;

  if (! ppNewRawStorage)
	return AAFRESULT_NULL_PARAM;

  OMRawStorage * stg = 0;

  if (kAAFFileExistence_new == existence)
	{
	  switch (access)
		{
		case kAAFFileAccess_read:
		  return AAFRESULT_WRONG_OPENMODE;		  
		  break;
		case kAAFFileAccess_write:
		case kAAFFileAccess_modify:
		  stg = OMDiskRawStorage::openNewModify (pFilename);
		  break;
		default:
		  return AAFRESULT_WRONG_OPENMODE;		  
		}
	}
  else if (kAAFFileExistence_existing == existence)
	{
	  switch (access)
		{
		case kAAFFileAccess_read:
		  stg = OMDiskRawStorage::openExistingRead (pFilename);
		  break;
		case kAAFFileAccess_write:
		case kAAFFileAccess_modify:
		  stg = OMDiskRawStorage::openExistingModify (pFilename);
		  break;
		default:
		  return AAFRESULT_WRONG_OPENMODE;		  
		}
	}
  else
	{
	  return AAFRESULT_WRONG_OPENMODE;
	}

  assert (stg);
  ImplAAFRawStorage * prs = 0;
  if (stg->isPositionable ())
	prs = static_cast<ImplAAFRawStorage *>
	  (::CreateImpl(CLSID_AAFRandomRawStorage));
  else
	prs = static_cast<ImplAAFRawStorage *>
	  (::CreateImpl(CLSID_AAFRawStorage));

  if(!prs)
	{
	  delete stg;
	  return AAFRESULT_NOMEMORY;
	}

  prs->Initialize (stg, access);

  assert (ppNewRawStorage);
  *ppNewRawStorage = prs;
  return AAFRESULT_SUCCESS;
}


STDAPI
ImplAAFCreateAAFFileOnRawStorage
  (IAAFRawStorage * pRawStorage,
   aafFileExistence_t existence,
   aafFileAccess_t access,
   aafUID_constptr pFileKind,
   aafUInt32 modeFlags,
   aafProductIdentification_constptr pIdent,
   ImplAAFFile ** ppNewFile)
{
  if (! pRawStorage)
	return AAFRESULT_NULL_PARAM;

  if (! ppNewFile)
	return AAFRESULT_NULL_PARAM;

#if ! READMODIFYWRITE_SUPPORTED

  if ((kAAFFileExistence_existing == existence) &&
      (kAAFFileAccess_modify == access))
  {
    return AAFRESULT_NOT_IN_CURRENT_VERSION;
  }

#endif

  HRESULT hr = S_OK;
  ImplAAFFileSP pFile;

  // Create an instance of an uninitialized file object.  Check the
  // raw storage to determine if this should be an AAFFile, or an
  // AAFRandomFile.  If the RawStorage is a RandomRawStorage, create
  // an AAFRandomFile.
  IAAFRandomRawStorage * prrs = 0;
  hr = pRawStorage->QueryInterface (IID_IAAFRandomRawStorage,
									(void **) &prrs);
  if (SUCCEEDED (hr))
	pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFRandomFile));
  else
	pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFFile));

  if (prrs)
	{
	  prrs->Release ();
	  prrs = 0;
	}

  if(!pFile)
	return AAFRESULT_NOMEMORY;

  // Smart pointer had already incremented reference count, so bump it
  // back down to 1 here...
  pFile->ReleaseReference ();

  // Make sure the file is initialized (not open yet...)
  hr = pFile->Initialize();
  if (SUCCEEDED(hr))
	{
	  // Attempt to open the file for modification.
	  hr = pFile->CreateAAFFileOnRawStorage(pRawStorage,
											existence,
											access,
											pFileKind,
											modeFlags,
											pIdent);
	  if (SUCCEEDED(hr))
		{
		  *ppNewFile = pFile;
		  (*ppNewFile)->AcquireReference ();
		}
	}

  return hr;
}

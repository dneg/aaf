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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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
#include "AAFFileKinds.h"
#include "AAFFileMode.h"
#include "AAFSDKBuild.h"

#include "ImplAAFPluginManager.h"
#include "ImplAAFContext.h"
#include "ImplAAFFile.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFRandomRawStorage.h"
#include "ImplAAFOMRawStorage.h"
#include "ImplEnumAAFFileEncodings.h"
#include "ImplAAFCachePageAllocator.h"
#include "ImplAAFOMCachePageAllocator.h"

#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMDiskRawStorage.h"
#include "OMCachedDiskRawStorage.h"
#include "OMMemoryRawStorage.h"
#include "OMStoredObjectFactory.h"
#include "OMCachePageAllocator.h"

#include "ImplAAFSmartPointer.h"
#include "AAFUtils.h"

typedef ImplAAFSmartPointer<ImplAAFFile> ImplAAFFileSP;

extern "C" const aafClassID_t CLSID_AAFFile;
extern "C" const aafClassID_t CLSID_AAFRandomFile;
extern "C" const aafClassID_t CLSID_AAFRawStorage;
extern "C" const aafClassID_t CLSID_AAFRandomRawStorage;
extern "C" const aafClassID_t CLSID_EnumAAFFileEncodings;
extern "C" const aafClassID_t CLSID_AAFCachePageAllocator;

#include "OMAssertions.h"


// Define this to 1 to use raw storage implementations of the
// traditional AAFFileOpenXXX(filename) methods.
#ifndef USE_RAW_STORAGE
#define USE_RAW_STORAGE 0
#endif // ! USE_RAW_STORAGE


#if USE_RAW_STORAGE
#include "AAF.h"
#endif

extern const aafUID_t *mapStructuredStorageFileKind_DefaultToActual(const aafUID_t *);

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
	  const aafUID_t* pFileKind;
	  if (modeFlags & AAF_FILE_MODE_USE_LARGE_SS_SECTORS)
	  	pFileKind = &kAAFFileKind_Aaf4KBinary;
    else
	  	pFileKind = &kAAFFileKind_Aaf512Binary;

	  hr = ImplAAFCreateAAFFileOnRawStorage
		(pRawStg,
		 kAAFFileExistence_existing,
		 kAAFFileAccess_read,
		 pFileKind,
		 modeFlags,
		 0,
		 ppFile);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  ASSERTU (ppFile);
		  ASSERTU (*ppFile);
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

  // Check that the file is an AAF file
  aafUID_t fileKind;
  aafBool isAnAAFFile;
  hr = ImplAAFFileIsAAFFile(pFileName, &fileKind, &isAnAAFFile);
  if (FAILED(hr))
    return hr;

  if (isAnAAFFile == kAAFFalse)
    return AAFRESULT_NOT_AAF_FILE;

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
	{
		hr = AAFRESULT_NOMEMORY;
	}
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
	  const aafUID_t* pFileKind;
	  if (modeFlags & AAF_FILE_MODE_USE_LARGE_SS_SECTORS)
	  	pFileKind = &kAAFFileKind_Aaf4KBinary;
		else
	  	pFileKind = &kAAFFileKind_Aaf512Binary;

	  hr = ImplAAFCreateAAFFileOnRawStorage
		(pRawStg,
		 kAAFFileExistence_existing,
		 kAAFFileAccess_modify,
		 pFileKind,
		 modeFlags,
		 pIdent,
		 ppFile);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  ASSERTU (ppFile);
		  ASSERTU (*ppFile);
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

  // Check that the file is an AAF file
  aafUID_t fileKind;
  aafBool isAnAAFFile;
  hr = ImplAAFFileIsAAFFile(pFileName, &fileKind, &isAnAAFFile);
  if (FAILED(hr))
    return hr;

  if (isAnAAFFile == kAAFFalse)
    return AAFRESULT_NOT_AAF_FILE;

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
  IAAFRawStorage * pRawStg = 0;
  AAFRESULT hr = AAFCreateRawStorageDisk
	(pFileName,
	 kAAFFileExistence_new,
	 kAAFFileAccess_modify,
	 &pRawStg);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  const aafUID_t* pFileKind;
	  if (modeFlags & AAF_FILE_MODE_USE_LARGE_SS_SECTORS)
	  	pFileKind = &kAAFFileKind_Aaf4KBinary;
		else
	  	pFileKind = &kAAFFileKind_Aaf512Binary;

	  hr = ImplAAFCreateAAFFileOnRawStorage
		(pRawStg,
		 kAAFFileExistence_new,
		 kAAFFileAccess_modify,
		 pFileKind,
		 modeFlags,
		 pIdent,
		 ppFile);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  ASSERTU (ppFile);
		  ASSERTU (*ppFile);
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
// AAFFileOpenNewModifyEx()
//
STDAPI ImplAAFFileOpenNewModifyEx (
  // Null-terminated string containing name of filesystem file to be
  // opened for modification.  Filename must be in a form that would
  // be acceptable to StgOpenStorage() for this platform.
  /*[in, string]*/ const aafCharacter *  pFileName,

	// the FileKind to create. Must be one of the constants defined in
	// include/AAFFileKinds.h, or 0
	// a stored object factory must have been registered by ImplAAFFile.cpp
  /*[in]*/ aafUID_constptr pFileKind,

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

  // this implementation requires the OM RAW_STORAGE apis
  // an implementation using OM NAMED_FILE apis is possible, however it
  // would require additional method on IAAFFile::OpenNewModifyEx()
  // which is not felt to be worthwhile
  // #if USE_RAW_STORAGE
  IAAFRawStorage * pRawStg = 0;
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
	 pFileKind,
	 modeFlags,
	 pIdent,
	 ppFile);
      if (AAFRESULT_SUCCEEDED (hr))
	{
	  ASSERTU (ppFile);
	  ASSERTU (*ppFile);
	  hr = (*ppFile)->Open ();
	}
    }
  if (pRawStg)
    {
      pRawStg->Release ();
    }
  
  return hr;
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
  IAAFRawStorage * pRawStg = 0;
  AAFRESULT hr = AAFCreateRawStorageMemory
	(kAAFFileAccess_modify,
	 &pRawStg);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  const aafUID_t* pFileKind;
	  if (modeFlags & AAF_FILE_MODE_USE_LARGE_SS_SECTORS)
	  	pFileKind = &kAAFFileKind_Aaf4KBinary;
		else
	  	pFileKind = &kAAFFileKind_Aaf512Binary;

	  hr = ImplAAFCreateAAFFileOnRawStorage
		(pRawStg,
		 kAAFFileExistence_new,
		 kAAFFileAccess_modify,
		 pFileKind,
		 0,
		 pIdent,
		 ppFile);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  ASSERTU (ppFile);
		  ASSERTU (*ppFile);
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
STDAPI ImplAAFFileIsAAFFile (
  const aafCharacter *  pFileName,
  aafUID_t * pAAFFileKind,
  aafBool *  pFileIsAAFFile)
{
  if (pFileName == 0)
    return AAFRESULT_NULL_PARAM;

  if (pAAFFileKind == 0)
    return AAFRESULT_NULL_PARAM;

  if (pFileIsAAFFile == 0)
    return AAFRESULT_NULL_PARAM;

  // Crude file existance check.  May be better done in 
  // OMSSStoredObjectFactory::isRecognized(OMRawStorage* rawStorage) but
  // reporting the right HRESULT is difficult from there.
  // There may also be another reason for failing to open the file than
  // non-existance. e.g. permissions problems.
  // A better description of the problem may be "Cannot read from file."
  FILE* f = wfopen(pFileName, L"rb");
  if(f == 0)
    return AAFRESULT_FILE_NOT_FOUND;
  
  fclose(f);
 
  HRESULT hr = S_OK;

  OMStoredObjectEncoding encoding;
  bool recognized = OMFile::isRecognized(pFileName, encoding);
  if (recognized) {
    *pAAFFileKind = *reinterpret_cast<aafUID_t*>(&encoding);
    *pFileIsAAFFile = kAAFTrue;
  } else {
    *pFileIsAAFFile = kAAFFalse;
  }
  return hr;
}

//***********************************************************
//
// AAFRawStorageIsAAFFile()
//
//
STDAPI ImplAAFRawStorageIsAAFFile (
  IAAFRawStorage * /* pRawStorage */,
  aafUID_t * /* pAAFFileKind */,
  aafBool *  /* pRawStorageIsAAFFile */)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


//***********************************************************
//
// ImplAAFFileIsAAFFileKind()
//
STDAPI ImplAAFFileIsAAFFileKind (
  aafCharacter_constptr  pFileName,
  aafUID_constptr pAAFFileKind_in,
  aafBool *  pFileIsAAFFile)
{
  if (pFileName == 0)
    return AAFRESULT_NULL_PARAM;

  const aafUID_t *pAAFFileKind = mapStructuredStorageFileKind_DefaultToActual(pAAFFileKind_in);
  if (pAAFFileKind == 0)
    return AAFRESULT_NULL_PARAM;

  if (pFileIsAAFFile == 0)
    return AAFRESULT_NULL_PARAM;

  // Crude file existance check.  May be better done in 
  // OMSSStoredObjectFactory::isRecognized(OMRawStorage* rawStorage) but
  // reporting the right HRESULT is difficult from there.
  // There may also be another reason for failing to open the file than
  // non-existance. e.g. permissions problems.
  // A better description of the problem may be "Cannot read from file."
  FILE* f = wfopen(pFileName, L"rb");
  if(f == 0)
    return AAFRESULT_FILE_NOT_FOUND;
  
  fclose(f);
 
  HRESULT hr = S_OK;
  aafBool is_file_kind = kAAFFalse;

  const OMStoredObjectEncoding* p_om_encoding =
      reinterpret_cast<const OMStoredObjectEncoding*>(pAAFFileKind);

  const bool has_factory = OMFile::hasFactory (*p_om_encoding);
  if (has_factory)
  {
    OMStoredObjectFactory* p_factory =
        OMFile::findFactory (*p_om_encoding);

    // Does the factory recognize this file?
    if( p_factory->isRecognized( pFileName ) )
    {
      is_file_kind = kAAFTrue;
    }
    else
    {
      is_file_kind = kAAFFalse;
    }

    hr = S_OK;
  }
  else
  {
    is_file_kind = false;
    hr = AAFRESULT_FILEKIND_NOT_REGISTERED;
  }


  if( hr == S_OK )
  {
    *pFileIsAAFFile = is_file_kind;
  }


  return hr;
}


//***********************************************************
//
// ImplAAFRawStorageIsAAFFileKind()
//
//
STDAPI ImplAAFRawStorageIsAAFFileKind (
  IAAFRawStorage *  pRawStorage,
  aafUID_constptr pAAFFileKind,
  aafBool *  pRawStorageIsAAFFile)
{
  if (pRawStorage == 0)
    return AAFRESULT_NULL_PARAM;

  if (pAAFFileKind == 0)
    return AAFRESULT_NULL_PARAM;

  if (pRawStorageIsAAFFile == 0)
    return AAFRESULT_NULL_PARAM;

  CHECK_CLIENT_IMPLEMENTED_QI(pRawStorage, IID_IAAFRawStorage);

  HRESULT hr = S_OK;
  aafBool is_file_kind = kAAFFalse;


  // Obtain OM representation of pRawStorage.
  IAAFRoot* p_root_object = 0;
  hr = pRawStorage->QueryInterface( IID_IAAFRoot,
                               reinterpret_cast<void**>(&p_root_object));
  ASSERTU(p_root_object != 0);

  ImplAAFRoot* p_impl_root_object = 0;
  p_root_object->GetImplRep( reinterpret_cast<void**>(&p_impl_root_object) );
  ASSERTU(p_impl_root_object != 0);

  p_root_object->Release();
  p_root_object = 0;

  ImplAAFRawStorage* p_impl_raw_storage =
                        dynamic_cast<ImplAAFRawStorage*>(p_impl_root_object);
  ASSERTU(p_impl_raw_storage != 0);

  OMRawStorage*  p_om_raw_storage = p_impl_raw_storage->GetOMStorage();
  ASSERTU(p_om_raw_storage != 0);


  const OMStoredObjectEncoding* p_om_encoding =
      reinterpret_cast<const OMStoredObjectEncoding*>(pAAFFileKind);

  const bool has_factory = OMFile::hasFactory (*p_om_encoding);
  if (has_factory)
  {
    OMStoredObjectFactory* p_factory =
        OMFile::findFactory (*p_om_encoding);

    // Does the factory recognize this file?
    if( p_factory->isRecognized( p_om_raw_storage ) )
    {
      is_file_kind = kAAFTrue;
    }
    else
    {
      is_file_kind = kAAFFalse;
    }

    hr = S_OK;
  }
  else
  {
    is_file_kind = false;
    hr = AAFRESULT_FILEKIND_NOT_REGISTERED;
  }


  if( hr == S_OK )
  {
    *pRawStorageIsAAFFile = is_file_kind;
  }


  return hr;
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

  ASSERTU (stg);
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

  ASSERTU (ppNewRawStorage);
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

  ASSERTU (stg);
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

  ASSERTU (ppNewRawStorage);
  *ppNewRawStorage = prs;
  return AAFRESULT_SUCCESS;
}

STDAPI
ImplAAFCreateRawStorageCachedDisk
  (aafCharacter_constptr pFilename,
   aafFileExistence_t existence,
   aafFileAccess_t access,
   aafUInt32  pageCount,
   aafUInt32  pageSize,
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
		  stg = OMCachedDiskRawStorage::openNewModify (pFilename, pageSize, pageCount);
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
		  stg = OMCachedDiskRawStorage::openExistingRead (pFilename, pageSize, pageCount);
		  break;
		case kAAFFileAccess_write:
		case kAAFFileAccess_modify:
		  stg = OMCachedDiskRawStorage::openExistingModify (pFilename, pageSize, pageCount);
		  break;
		default:
		  return AAFRESULT_WRONG_OPENMODE;		  
		}
	}
  else
	{
	  return AAFRESULT_WRONG_OPENMODE;
	}

  ASSERTU (stg);
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

  ASSERTU (ppNewRawStorage);
  *ppNewRawStorage = prs;
  return AAFRESULT_SUCCESS;
}

// not public
STDAPI
ImplAAFCreateBuiltinCachePageAllocator
  (ImplAAFCachePageAllocator ** ppCachePageAllocator)
{
  ImplAAFCachePageAllocator* pImplAllocator = static_cast<ImplAAFCachePageAllocator*>(::CreateImpl(CLSID_AAFCachePageAllocator));
  if (!pImplAllocator)
	  return AAFRESULT_NOMEMORY;

  *ppCachePageAllocator = pImplAllocator;
  return AAFRESULT_SUCCESS;
}

// not public
STDAPI
AAFCreateBuiltinCachePageAllocator
  (aafUInt32  /* pageCount */,
   aafUInt32  /* pageSize */,
   IAAFCachePageAllocator ** ppCachePageAllocator)
{
  ImplAAFCachePageAllocator* pImplAllocator = 0;
  HRESULT hr = ImplAAFCreateBuiltinCachePageAllocator(&pImplAllocator);
  if (!(AAFRESULT_SUCCEEDED (hr)))
    return hr;

  IUnknown *pUnknown = static_cast<IUnknown *>(pImplAllocator->GetContainer());
  ASSERTU(pUnknown);
  IAAFCachePageAllocator* pNewAllocator = 0;
  hr = pUnknown->QueryInterface(IID_IAAFCachePageAllocator, (void **)&pNewAllocator);
  ASSERTU (SUCCEEDED (hr));
  pImplAllocator->ReleaseReference();
  *ppCachePageAllocator = pNewAllocator;
  return AAFRESULT_SUCCESS;
}

STDAPI
ImplAAFCreateRawStorageCached
  (IAAFRawStorage * pRawStorage,
   aafUInt32  pageCount,
   aafUInt32  pageSize,
   ImplAAFRawStorage ** ppNewRawStorage)
{
  if (! pRawStorage)
	return AAFRESULT_NULL_PARAM;
  if (! ppNewRawStorage)
	return AAFRESULT_NULL_PARAM;

  IAAFCachePageAllocator * pCachePageAllocator = 0;
  HRESULT hr = AAFCreateBuiltinCachePageAllocator(pageSize, pageCount, &pCachePageAllocator);
  if (!AAFRESULT_SUCCEEDED(hr))
    return hr;

  return ImplAAFCreateRawStorageCached2(pRawStorage,
                                        pageCount,
                                        pageSize,
                                        pCachePageAllocator,
                                        ppNewRawStorage);
}

STDAPI
ImplAAFCreateRawStorageCached2
  (IAAFRawStorage * pRawStorage,
   aafUInt32  pageCount,
   aafUInt32  pageSize,
   IAAFCachePageAllocator * pCachePageAllocator,
   ImplAAFRawStorage ** ppNewRawStorage)
{
  if (! pRawStorage)
	return AAFRESULT_NULL_PARAM;
  if (! pCachePageAllocator)
	return AAFRESULT_NULL_PARAM;
  if (! ppNewRawStorage)
	return AAFRESULT_NULL_PARAM;

  CHECK_CLIENT_IMPLEMENTED_QI(pRawStorage, IID_IAAFRawStorage);
  CHECK_CLIENT_IMPLEMENTED_QI(pCachePageAllocator, IID_IAAFCachePageAllocator);

  HRESULT hr;
  aafBoolean_t isReadable;
  hr = pRawStorage->IsReadable(&isReadable);
  if (FAILED(hr)) return hr;

  aafBoolean_t isWriteable;
  hr = pRawStorage->IsWriteable(&isWriteable);
  if (FAILED(hr)) return hr;

  aafFileAccess_t access = kAAFFileAccess_none;
  if (isReadable && isWriteable)
    access = kAAFFileAccess_modify;
  else if (isReadable)
    access = kAAFFileAccess_read;
  else if (isWriteable)
    access = kAAFFileAccess_write;

  hr = pCachePageAllocator->Initialize(pageCount, pageSize);
  if (FAILED(hr)) return hr;

  OMCachePageAllocator* pAllocator = new ImplAAFOMCachePageAllocator(pCachePageAllocator, pageCount, pageSize);
  ASSERTU(pAllocator);
  ImplAAFOMCachedRawStorage* pStg = new ImplAAFOMCachedRawStorage(pRawStorage,
                                                                  pageCount,
                                                                  pageSize,
                                                                  pAllocator);
  ASSERTU(pStg);


  ImplAAFRawStorage * prs = static_cast<ImplAAFRawStorage *>
	  (::CreateImpl(CLSID_AAFRandomRawStorage));
  prs->Initialize (pStg, access);
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

  CHECK_CLIENT_IMPLEMENTED_QI(pRawStorage, IID_IAAFRawStorage);

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

STDAPI
ImplAAFSetProgressCallback
  (IAAFProgress * pProgress)
{
  if (pProgress == 0)
    return AAFRESULT_NULL_PARAM;

  AAFRESULT	hr = AAFRESULT_SUCCESS;

  CHECK_CLIENT_IMPLEMENTED_QI(pProgress, IID_IAAFProgress);

  ImplAAFContext	*context = ImplAAFContext::GetInstance();
  ASSERTU(context != 0);
  hr = context->SetProgressCallback(pProgress);
  return hr;
}

STDAPI
ImplAAFGetFileEncodings
  (ImplEnumAAFFileEncodings** ppFileEncodings)
{
  if( ppFileEncodings == 0 )
  {
    return AAFRESULT_NULL_PARAM;
  }


  HRESULT   hr = AAFRESULT_SUCCESS;


  ImplEnumAAFFileEncodings* p_enum_encodings =
    static_cast<ImplEnumAAFFileEncodings*>
      (::CreateImpl(CLSID_EnumAAFFileEncodings));

  if( p_enum_encodings == 0 )
  {
    hr = AAFRESULT_NOMEMORY;
  }


  if( SUCCEEDED( hr ) )
  {
    *ppFileEncodings = p_enum_encodings;
  }


  return hr;
}

extern const char AAFReferenceImplementationIdent[];
const char AAFReferenceImplementationIdent[] = "@(#) " AAF_SDK_RELEASE; // for Linux and Irix
const aafProductVersion_t AAFReferenceImplementationVersion = {AAF_MAJOR_VERSION, AAF_MINOR_VERSION, AAF_MAINT_RELEASE, AAF_PATCH_LEVEL, AAF_RELEASE_STAGE};


STDAPI
ImplAAFGetLibraryVersion
  (aafProductVersion_t *  pVersion)
{
  if (pVersion == 0)
  {
    return AAFRESULT_NULL_PARAM;
  }


  HRESULT   hr = AAFRESULT_SUCCESS;
  *pVersion = AAFReferenceImplementationVersion;
  return hr;
}


STDAPI
ImplAAFGetStaticLibraryVersion
  (aafProductVersion_t *  pVersion)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

extern "C" const char * AAFGetLibraryPath();

// The size of a buffer, in bytes, needed to hold s when properly
// terminated and converted to aafCharacters.
static size_t bufferByteSize(const char* s)
{
  ASSERTU(s != 0);
  size_t result = strlen(s) + 1; // characters needed
  result = result * sizeof(aafCharacter); // bytes needed
  return result;
}

STDAPI ImplAAFGetLibraryPathNameBufLen
  (aafUInt32 *  pBufSize)
{
  if (pBufSize == 0)
	return AAFRESULT_NULL_PARAM;

  const char* path = AAFGetLibraryPath();
  ASSERTU(path != 0);
  *pBufSize = bufferByteSize(path);
  return AAFRESULT_SUCCESS;
}

STDAPI ImplAAFGetLibraryPathName
   (aafCharacter *  pLibraryPathName,
    aafUInt32  bufSize)
{
  if (pLibraryPathName == 0)
	return AAFRESULT_NULL_PARAM;

  const char* path = AAFGetLibraryPath();
  ASSERTU(path != 0);
  if (bufferByteSize(path) > bufSize)
	return AAFRESULT_SMALLBUF;

  convertStringToWideString(pLibraryPathName, path, bufSize / sizeof(aafCharacter));
  return AAFRESULT_SUCCESS;
}

STDAPI ImplAAFResultToTextBufLen (
    AAFRESULT  /* result */,
    aafUInt32 *   /* pResultTextSize */)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

STDAPI ImplAAFResultToText (
    AAFRESULT  /* result */,
    aafCharacter *  /* pResultText */,
    aafUInt32  /* resultTextSize */)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

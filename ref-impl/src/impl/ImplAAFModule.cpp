/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

////////////////////////////////////////////////////////////////////////////////
// @doc
//
// @module AAFMODULE.CPP - AAF Factory functions for AAF DLL Module |
//
// Implementation of DLL Exports
// 
// @end
//
// Copyright (c)1998-1999 Avid Technologies, Inc. All Rights Reserved
//
// Tom Ransdell, Avid AAF Development Team
// Plugin manager call added by Jeffrey Bedell.
//
////////////////////////////////////////////////////////////////////////////////

#include "ImplAAFModule.h"

#include "AAFRoot.h"
#include "AAFResult.h"
#include "ImplAAFPluginManager.h"
#include "ImplAAFFile.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_AAFFile;

#include <assert.h>




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
  HRESULT hr = S_OK;
  ImplAAFFile * pFile = NULL;

  if (NULL == pFileName || NULL == ppFile)
    return AAFRESULT_NULL_PARAM;

  // Initialize the out parameter.
  *ppFile = NULL;

  //
  // For backwards compatibility with existing client code
  // the first checked in version of this function is implemented
  // the same as the old client code which this function is 
  // intended to replace...
  // 

  // Create an instance of an uninitialized file object.
  pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFFile));
	if(NULL == pFile)
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
      }
    }
  }

  return hr;
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
  HRESULT hr = S_OK;
  ImplAAFFile * pFile = NULL;

  if (NULL == pFileName || NULL == pIdent || NULL == ppFile)
    return AAFRESULT_NULL_PARAM;

  // Initialize the out parameter.
  *ppFile = NULL;

  //
  // For backwards compatibility with existing client code
  // the first checked in version of this function is implemented
  // the same as the old client code which this function is 
  // intended to replace...
  // 

  // Create an instance of an uninitialized file object.
  pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFFile));
	if(NULL == pFile)
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
      }
    }
  }

  return hr;
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
  HRESULT hr = S_OK;
  ImplAAFFile * pFile = NULL;

  if (NULL == pFileName || NULL == pIdent || NULL == ppFile)
    return AAFRESULT_NULL_PARAM;

  // Initialize the out parameter.
  *ppFile = NULL;

  //
  // For backwards compatibility with existing client code
  // the first checked in version of this function is implemented
  // the same as the old client code which this function is 
  // intended to replace...
  // 

  // Create an instance of an uninitialized file object.
  pFile = static_cast<ImplAAFFile *>(::CreateImpl(CLSID_AAFFile));
	if(NULL == pFile)
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
      }
    }
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
  HRESULT hr = S_OK;
	const aafClassID_t& fileID = *reinterpret_cast<const aafClassID_t *>(&CLSID_AAFFile); 
  ImplAAFFile * pFile = NULL;

  if (NULL == pIdent || NULL == ppFile)
    return AAFRESULT_NULL_PARAM;

  // Initialize the out parameter.
  *ppFile = NULL;

  //
  // For backwards compatibility with existing client code
  // the first checked in version of this function is implemented
  // the same as the old client code which this function is 
  // intended to replace...
  // 

  // Create an instance of an uninitialized file object.
  pFile = static_cast<ImplAAFFile *>(::CreateImpl(fileID));
	if(NULL == pFile)
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
      }
    }
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
	
	if (NULL == ppManager)
		return AAFRESULT_NULL_PARAM;
	
	// Initialize the out parameter.
	*ppManager = NULL;
	
	implMgr = ImplAAFPluginManager::GetPluginManager();
    *ppManager = implMgr;
	
	return hr;
}



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

#include "AAFPrivate.h"
#include "AAFResult.h"
#include "ImplAAFPluginManager.h"
#include "ImplAAFFile.h"
#include "ImplAAFRawStorage.h"
#include "ImplEnumAAFFileEncodings.h"
//#include "ImplAAFObjectCreation.h"

#include "AAFTypes.h"

class ImplAAFFile;
class ImplAAFPluginManager;

//***********************************************************
//
// AAFLoad()
//
// This is just a stub since the dll will already be loaded
// if the client code has linked directly to the export library.
//
STDAPI ImplAAFLoad(const char * /*dllname*/);




//***********************************************************
//
// AAFUnload()
//
// This is just a stub since the dll will be unloaded
// when the client code terminates if the client code has 
// linked directly to the export library.
//
STDAPI ImplAAFUnload();



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
  /*[out]*/ ImplAAFFile ** ppFile);


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
  /*[out]*/ ImplAAFFile ** ppFile);



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
  /*[out]*/ ImplAAFFile ** ppFile);

//***********************************************************
//
// AAFFileOpenNewModifyEx()
//
// Please see AAF.h for comments.
//
STDAPI ImplAAFFileOpenNewModifyEx (
  const aafCharacter *  pFileName,
  aafUID_constptr  pFileKind,
  aafUInt32  modeFlags,
  aafProductIdentification_t *  pIdent,
  ImplAAFFile ** ppFile);



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
  /*[out]*/ ImplAAFFile ** ppFile);

//***********************************************************
//
// AAFFileIsAAFFile()
//
// Please see AAF.h for comments.
//
STDAPI ImplAAFFileIsAAFFile (
  const aafCharacter *  pFileName,
  aafUID_t * pAAFFileKind,
  aafBool *  pFileIsAAFFile);

//***********************************************************
//
// AAFRawStorageIsAAFFile()
//
// Please see AAF.h for comments.
//
STDAPI ImplAAFRawStorageIsAAFFile (
  IAAFRawStorage * pRawStorage,
  aafUID_t * pAAFFileKind,
  aafBool *  pRawStorageIsAAFFile);

//***********************************************************
//
// ImplAAFFileIsAAFFileKind()
//
STDAPI ImplAAFFileIsAAFFileKind (
  aafCharacter_constptr  pFileName,
  aafUID_constptr pAAFFileKind,
  aafBool *  pFileIsAAFFile);

//***********************************************************
//
// ImplAAFRawStorageIsAAFFileKind()
//
//
STDAPI ImplAAFRawStorageIsAAFFileKind (
  IAAFRawStorage *  /*pRawStorage*/,
  aafUID_constptr /*pAAFFileKind*/,
  aafBool *  /*pRawStorageIsAAFFile*/);

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
  /*[out]*/ ImplAAFPluginManager ** ppManager);


STDAPI
ImplAAFCreateRawStorageMemory
  (aafFileAccess_t  access,
   ImplAAFRawStorage ** ppNewRawStorage);

STDAPI
ImplAAFCreateRawStorageDisk
  (aafCharacter_constptr  pFilename,
   aafFileExistence_t  existence,
   aafFileAccess_t  access,
   ImplAAFRawStorage ** ppNewRawStorage);

STDAPI
ImplAAFCreateRawStorageCachedDisk
  (aafCharacter_constptr pFilename,
   aafFileExistence_t existence,
   aafFileAccess_t access,
   aafUInt32  pageCount,
   aafUInt32  pageSize,
   ImplAAFRawStorage ** ppNewRawStorage);

STDAPI
ImplAAFCreateRawStorageCached
  (IAAFRawStorage * pRawStorage,
   aafUInt32  pageCount,
   aafUInt32  pageSize,
   ImplAAFRawStorage ** ppNewRawStorage);

STDAPI
ImplAAFCreateRawStorageCached2
  (IAAFRawStorage * pRawStorage,
   aafUInt32  pageCount,
   aafUInt32  pageSize,
   IAAFCachePageAllocator * pCachePageAllocator,
   ImplAAFRawStorage ** ppNewRawStorage);

STDAPI
ImplAAFCreateAAFFileOnRawStorage
  (IAAFRawStorage * pRawStorage,
   aafFileExistence_t  existence,
   aafFileAccess_t  access,
   aafUID_constptr  pFileKind,
   aafUInt32  modeFlags,
   aafProductIdentification_constptr  pIdent,
   ImplAAFFile ** ppNewFile);

STDAPI
ImplAAFSetProgressCallback
  (IAAFProgress * pProgress);

//***********************************************************
//
// ImplAAFGetFileEncodings()
//
// Returns an enumeration of file encodings supported by this version
// of the library.
//
// This method will succeed if the following are true:
// - The ppFileEncodings pointer is valid.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppFileEncodings arg is NULL.
// 
STDAPI
ImplAAFGetFileEncodings
  (ImplEnumAAFFileEncodings** ppFileEncodings);

//***********************************************************
//
// ImplAAFSetDiagnosticOutput()
//
// Makes the specified stream the AAF Toolkit's diagnostic output stream
//
// This method will succeed if the following are true:
// - The pOutput pointer is valid.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pOutput arg is NULL.
// 
STDAPI
ImplAAFSetDiagnosticOutput
  (IAAFDiagnosticOutput* pOutput);

//***********************************************************
//
// ImplAAFGetLibraryVersion()
//
// Return the version of the AAF library currently
// running on this machine\, which implements these interfaces.
// 
// Succeeds if all of the following are true:
// - the pVersion pointer is valid.
// 
// If this method fails nothing is written to *pVersion.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect\, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pVersion is null.
//
STDAPI
ImplAAFGetLibraryVersion
  (aafProductVersion_t *  pVersion);

extern const aafProductVersion_t AAFReferenceImplementationVersion;

// The real AAFGetStaticLibraryVersion is implemented in aaflib.
// This function is here for two reasons:
// 1.  The dodo machinery requires it
// 2.  If someone were to link directly to the AAF DLL (rather than load
//        via aaflib's functionality), then they have the corresponding 
//        function.
// Thus, this function is implemented with the second case in mind.  
// However, this function has no meaning in that context, thus NOT_IMPLEMENTED
// is always returned.
STDAPI ImplAAFGetStaticLibraryVersion
  (aafProductVersion_t *  pVersion);

STDAPI ImplAAFGetLibraryPathNameBufLen
  (aafUInt32 *  pBufSize);

STDAPI ImplAAFGetLibraryPathName
   (aafCharacter *  pLibraryPathName,
    aafUInt32  bufSize);

STDAPI ImplAAFResultToTextBufLen (
    AAFRESULT  result,
    aafUInt32 *   pResultTextSize);

STDAPI ImplAAFResultToText (
    AAFRESULT  result,
    aafCharacter *  pResultText,
    aafUInt32  resultTextSize);

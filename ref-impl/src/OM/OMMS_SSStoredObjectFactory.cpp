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
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#include "OMMS_SSStoredObjectFactory.h"

#include "OMRawStorageLockBytes.h"
#include "OMSSStoredObject.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"
#include "OMMSStructuredStorage.h"

#include "OMExceptions.h"

static void checkStatus(HRESULT status);

  // @mfunc Constructor.
OMMS_SSStoredObjectFactory::OMMS_SSStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMSSStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMMS_SSStoredObjectFactory::OMMS_SSStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
  PRECONDITION("Valid signature",
                                signature != nullOMUniqueObjectIdentification);
}

  // @mfunc Destructor.
OMMS_SSStoredObjectFactory::~OMMS_SSStoredObjectFactory(void)
{
  TRACE("OMMS_SSStoredObjectFactory::~OMMS_SSStoredObjectFactory");
}

  // @mfunc Initialize this <c OMMS_SSStoredObjectFactory>.
void OMMS_SSStoredObjectFactory::initialize(void)
{
  TRACE("OMMS_SSStoredObjectFactory::initialize");
  OMMSSInitialize();
}

  // @mfunc Finalize this <c OMMS_SSStoredObjectFactory>.
void OMMS_SSStoredObjectFactory::finalize(void)
{
  TRACE("OMMS_SSStoredObjectFactory::finalize");
  OMMSSFinalize();
}

/*
* MS VC++ versions prior to version 7.0 do not define STGOPTIONS (unless a
* recent Microsoft platform SDK was installed).  The reliable way to test
* whether STGOPTIONS is defined is to test for the STGOPTIONS_VERSION macro.
*/
#ifdef OM_USE_STORAGE_EX
#ifndef STGOPTIONS_VERSION
typedef void STGOPTIONS;
#endif // STGOPTIONS_VERSION
#endif // OM_USE_STORAGE_EX

OMStoredObject*
OMMS_SSStoredObjectFactory::openFile (OMRawStorage* rawStorage,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMMS_SSStoredObjectFactory::openFile");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  ILockBytes* iLockBytes = new OMRawStorageLockBytes(rawStorage);
  ASSERT("Valid heap pointer", iLockBytes != 0);

  DWORD openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STGM_DIRECT | STGM_READ      | STGM_SHARE_DENY_WRITE;
  }

  IStorage* storage = 0;

#ifndef OM_USE_STORAGE_EX

  HRESULT status = StgOpenStorageOnILockBytes(
    iLockBytes,
    0,
    openMode,
    0,
    0,
    &storage);
  checkStatus(status);
  ASSERT("StgOpenStorageOnILockBytes() succeeded", SUCCEEDED(status));
#else // OM_USE_STORAGE_EX

	OM_STGOPTIONS stgoptions = { 1, 0, 0, NULL };

	// since there is no StgOpenStorageOnILockBytesEx(), try ordinary...expect it to fail interestingly
  HRESULT status = StgOpenStorageOnILockBytes(
    iLockBytes,
    0,
    openMode,
    0,
    0,
    &storage);
  checkStatus(status);
  ASSERT("StgOpenStorageOnILockBytes() succeeded", SUCCEEDED(status));

#endif //OM_USE_STORAGE_EX

  iLockBytes->Release();
  
  return OMSSStoredObject::open(storage, mode);
}

OMStoredObject* 
OMMS_SSStoredObjectFactory::createFile (OMRawStorage* rawStorage,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
	TRACE("OMMS_SSStoredObjectFactory::createFile");
	PRECONDITION("Valid raw storage", rawStorage != 0);

	// choose sector size based on signature from factory
	unsigned long sectorSize=0;
	if( OMSignature_Aaf_SSBin_512 == signature ) sectorSize=512;
	else if( OMSignature_Aaf_SSBin_4K == signature ) sectorSize=4096;
	PRECONDITION("Valid Signature", sectorSize!=0 );

	IStorage* storage = 0;

#ifndef OM_USE_STORAGE_EX

	ILockBytes* iLockBytes = new OMRawStorageLockBytes(rawStorage);
	ASSERT("Valid heap pointer", iLockBytes != 0);

	HRESULT status = StgCreateDocfileOnILockBytes(
		iLockBytes,
		STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
		0,
		&storage);
	checkStatus(status);
	iLockBytes->Release();
	ASSERT("StgCreateDocfileOnILockBytes() succeeded", SUCCEEDED(status));
#else // OM_USE_STORAGE_EX

	OM_STGOPTIONS stgoptions = { 1, 0, 0, NULL };
	stgoptions.ulSectorSize = sectorSize;

	/*
	The StgOpenStorageOnILockBytes API should work for 4k-sector files.  You
	can either preload a 12k file (you need 3 sectors for an empty docfile
	-- the header, FAT, and directory containing the root entry), or you can
	call StgCreateStorageEx to create and immediately release the root
	storage and re-open with StgOpenStorageOnILockBytes.
	*/

	HRESULT status;

	ILockBytes* iLockBytes = new OMRawStorageLockBytes(rawStorage);
	ASSERT("Valid heap pointer", iLockBytes != 0);

	status = StgCreateDocfileOnILockBytesEx(
				iLockBytes,
				STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
				STGFMT_DOCFILE,
				0, // or could be FILE_FLAG_NO_BUFFERING
				&stgoptions,
				0,
				IID_IStorage,
				(void**)&storage);

	checkStatus(status);

	ASSERT("StgOpenStorageOnILockBytes() succeeded", SUCCEEDED(status));
#endif //OM_USE_STORAGE_EX

  return OMSSStoredObject::create(storage, byteOrder);
}


OMStoredObject*
OMMS_SSStoredObjectFactory::openFile (const wchar_t* fileName,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMMS_SSStoredObjectFactory::openFile");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  DWORD openMode;
  if (mode == OMFile::modifyMode) {
    openMode = STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
  } else if (mode == OMFile::readOnlyMode) {
    openMode = STGM_DIRECT | STGM_READ      | STGM_SHARE_DENY_WRITE;
  }

  SSCHAR omFileName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, FILENAME_MAX);
#else
  copyWideString (omFileName, fileName, FILENAME_MAX);
#endif

  IStorage* storage = 0;

#ifndef OM_USE_STORAGE_EX

  HRESULT status = StgOpenStorage(
    omFileName,
    0,
    openMode,
    0,
    0,
    &storage);
  checkStatus(status);
  ASSERT("StgOpenStorage() succeeded", SUCCEEDED(status));

#else // OM_USE_STORAGE_EX

	OM_STGOPTIONS stgoptions = { 1, 0, 0, NULL };

  HRESULT status = StgOpenStorageEx(
							omFileName,
							openMode,
							STGFMT_DOCFILE,
							0,
							(STGOPTIONS *) &stgoptions,	//save to refer to STGOPTIONS here as this code is only used under windows
							0,
							IID_IStorage,
							(void **) &storage);

  checkStatus(status);
  ASSERT("StgOpenStorageEx() succeeded", SUCCEEDED(status));
#endif //OM_USE_STORAGE_EX

  return OMSSStoredObject::open(storage, mode);
}

OMStoredObject*
OMMS_SSStoredObjectFactory::createFile (const wchar_t* fileName,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
  TRACE("OMMS_SSStoredObjectFactory::createFile");
  PRECONDITION("Valid file name", validWideString(fileName));

// choose sector size based on signature from factory
	unsigned long sectorSize=0;
	if( OMSignature_Aaf_SSBin_512 == signature ) sectorSize=512;
	else if( OMSignature_Aaf_SSBin_4K == signature ) sectorSize=4096;
	PRECONDITION("Valid Signature", sectorSize!=0 );

  SSCHAR omFileName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, FILENAME_MAX);
#else
  copyWideString (omFileName, fileName, FILENAME_MAX);
#endif

  IStorage* storage = 0;

#ifndef OM_USE_STORAGE_EX

  HRESULT status = StgCreateDocfile(
    omFileName,
    STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_FAILIFTHERE,
    0,
    &storage);
  checkStatus(status);
  ASSERT("StgCreateDocfile() succeeded", SUCCEEDED(status));
#else // OM_USE_STORAGE_EX

	OM_STGOPTIONS stgoptions = { 1, 0, 0, NULL };
	stgoptions.ulSectorSize = sectorSize;

	HRESULT status = StgCreateStorageEx(
						omFileName,
						STGM_DIRECT | STGM_READWRITE | STGM_SHARE_EXCLUSIVE ,
						STGFMT_DOCFILE,
						0, // or could be FILE_FLAG_NO_BUFFERING
						(STGOPTIONS *) &stgoptions,	//save to refer to STGOPTIONS here as this code is only used under windows
						0,
						IID_IStorage,
						(void **) &storage);

  checkStatus(status);
  ASSERT("StgCreateStorageEx() succeeded", SUCCEEDED(status));
#endif //OM_USE_STORAGE_EX

  return OMSSStoredObject::create(storage, byteOrder);
}

static void checkStatus(HRESULT status)
{
  TRACE("checkStatus");

  if (FAILED(status)) {
    throw OMException(status);
  }
}

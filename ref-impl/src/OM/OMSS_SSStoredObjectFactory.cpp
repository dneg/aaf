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
#include "OMSS_SSStoredObjectFactory.h"

#include "OMSSStoredObject.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"
#include "OMSSStructuredStorage.h"

#include "OMExceptions.h"

static void checkStatus(HRESULT status);

  // @mfunc Constructor.
OMSS_SSStoredObjectFactory::OMSS_SSStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMSSStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMSS_SSStoredObjectFactory::OMSS_SSStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
  PRECONDITION("Valid signature",
                                signature != nullOMUniqueObjectIdentification);
}

  // @mfunc Destructor.
OMSS_SSStoredObjectFactory::~OMSS_SSStoredObjectFactory(void)
{
  TRACE("OMSS_SSStoredObjectFactory::~OMSS_SSStoredObjectFactory");
}

  // @mfunc Initialize this <c OMSS_SSStoredObjectFactory>.
void OMSS_SSStoredObjectFactory::initialize(void)
{
  TRACE("OMSS_SSStoredObjectFactory::initialize");
}

  // @mfunc Finalize this <c OMSS_SSStoredObjectFactory>.
void OMSS_SSStoredObjectFactory::finalize(void)
{
  TRACE("OMSS_SSStoredObjectFactory::finalize");
}

OMStoredObject*
OMSS_SSStoredObjectFactory::openFile (OMRawStorage* rawStorage,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMSS_SSStoredObjectFactory::openFile");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  IStorage* storage = 0;

  HRESULT status = OMSSIStorage::StgOpenStorageInOMRawStorage(
												rawStorage,
												mode,
												(void **)&storage);
  checkStatus(status);
  ASSERT("StgOpenStorageInOMRawStorage() succeeded", SUCCEEDED(status));

  return OMSSStoredObject::open(storage, mode);
}

OMStoredObject* 
OMSS_SSStoredObjectFactory::createFile (OMRawStorage* rawStorage,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
	TRACE("OMSS_SSStoredObjectFactory::createFile");
	PRECONDITION("Valid raw storage", rawStorage != 0);

	// choose sector size based on signature from factory
	unsigned long sectorSize=0;
	if( OMSignature_Aaf512Binary == signature ) sectorSize=512;
	else if( OMSignature_Aaf4KBinary == signature ) sectorSize=4096;
	PRECONDITION("Valid Signature", sectorSize!=0 );

	IStorage* storage = 0;

    HRESULT status = OMSSIStorage::StgCreateStorageInOMRawStorage(
												rawStorage,
												OMFile::modifyMode,
												(void **)&storage,
												sectorSize);
	checkStatus(status);
	ASSERT("StgCreateStorageInOMRawStorage() succeeded", SUCCEEDED(status));

	return OMSSStoredObject::create(storage, byteOrder);
}


OMStoredObject*
OMSS_SSStoredObjectFactory::openFile (const wchar_t* fileName,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMSS_SSStoredObjectFactory::openFile");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  SSCHAR omFileName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, FILENAME_MAX);
#else
  copyWideString (omFileName, fileName, FILENAME_MAX);
#endif

  IStorage* storage = 0;

  HRESULT status = OMSSIStorage::StgOpenStorageEx(
												omFileName,
												mode,
												(void **)&storage);
  checkStatus(status);
  ASSERT("StgOpenStorageEx() succeeded", SUCCEEDED(status));

  return OMSSStoredObject::open(storage, mode);
}

OMStoredObject*
OMSS_SSStoredObjectFactory::createFile (const wchar_t* fileName,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSS_SSStoredObjectFactory::createFile");
  PRECONDITION("Valid file name", validWideString(fileName));

// choose sector size based on signature from factory
	unsigned long sectorSize=0;
	if( OMSignature_Aaf512Binary == signature ) sectorSize=512;
	else if( OMSignature_Aaf4KBinary == signature ) sectorSize=4096;
	PRECONDITION("Valid Signature", sectorSize!=0 );

  SSCHAR omFileName[FILENAME_MAX];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, FILENAME_MAX);
#else
  copyWideString (omFileName, fileName, FILENAME_MAX);
#endif

  IStorage* storage = 0;

  HRESULT status = OMSSIStorage::StgCreateStorageEx(
											omFileName,
											OMFile::modifyMode,
											(void **)&storage,
											sectorSize);
  checkStatus(status);
  ASSERT("StgCreateStorageEx() succeeded", SUCCEEDED(status));

  return OMSSStoredObject::create(storage, byteOrder);
}

static void checkStatus(HRESULT status)
{
  TRACE("checkStatus");

  if (FAILED(status)) {
    throw OMException(status);
  }
}

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
#include "OMSSxMSSStoredObjectFactory.h"

#include "OMMSSStoredObject.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"
#include "OMSSSStructuredStorage.h"

#include "OMExceptions.h"

static void checkStatus(HRESULT status);

  // @mfunc Constructor.
OMSSxMSSStoredObjectFactory::OMSSxMSSStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMMSSStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMSSxMSSStoredObjectFactory::OMSSxMSSStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
  PRECONDITION("Valid signature",
                                signature != nullOMUniqueObjectIdentification);
}

  // @mfunc Destructor.
OMSSxMSSStoredObjectFactory::~OMSSxMSSStoredObjectFactory(void)
{
  TRACE("OMSSxMSSStoredObjectFactory::~OMSSxMSSStoredObjectFactory");
}

  // @mfunc Initialize this <c OMSSxMSSStoredObjectFactory>.
void OMSSxMSSStoredObjectFactory::initialize(void)
{
  TRACE("OMSSxMSSStoredObjectFactory::initialize");
}

  // @mfunc Finalize this <c OMSSxMSSStoredObjectFactory>.
void OMSSxMSSStoredObjectFactory::finalize(void)
{
  TRACE("OMSSxMSSStoredObjectFactory::finalize");
}

OMStoredObject*
OMSSxMSSStoredObjectFactory::openFile (OMRawStorage* rawStorage,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMSSxMSSStoredObjectFactory::openFile");
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

  return OMMSSStoredObject::open(storage, mode);
}

OMStoredObject* 
OMSSxMSSStoredObjectFactory::createFile (OMRawStorage* rawStorage,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
	TRACE("OMSSxMSSStoredObjectFactory::createFile");
	PRECONDITION("Valid raw storage", rawStorage != 0);

	// choose sector size based on signature from factory
	unsigned long sectorSize=0;
	if( OMSignature_Aaf_SSBin_512 == signature ) sectorSize=512;
	else if( OMSignature_Aaf_SSBin_4K == signature ) sectorSize=4096;
	PRECONDITION("Valid Signature", sectorSize!=0 );

	IStorage* storage = 0;

    HRESULT status = OMSSIStorage::StgCreateStorageInOMRawStorage(
												rawStorage,
												OMFile::modifyMode,
												(void **)&storage,
												sectorSize);
	checkStatus(status);
	ASSERT("StgCreateStorageInOMRawStorage() succeeded", SUCCEEDED(status));

	return OMMSSStoredObject::create(storage, byteOrder);
}


OMStoredObject*
OMSSxMSSStoredObjectFactory::openFile (const wchar_t* fileName,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMSSxMSSStoredObjectFactory::openFile");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  SSCHAR omFileName[256];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, 256);
#else
  copyWideString (omFileName, fileName, 256);
#endif

  IStorage* storage = 0;

  HRESULT status = OMSSIStorage::StgOpenStorageEx(
												omFileName,
												mode,
												(void **)&storage);
  checkStatus(status);
  ASSERT("StgOpenStorageEx() succeeded", SUCCEEDED(status));

  return OMMSSStoredObject::open(storage, mode);
}

OMStoredObject*
OMSSxMSSStoredObjectFactory::createFile (const wchar_t* fileName,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSxMSSStoredObjectFactory::createFile");
  PRECONDITION("Valid file name", validWideString(fileName));

// choose sector size based on signature from factory
	unsigned long sectorSize=0;
	if( OMSignature_Aaf_SSBin_512 == signature ) sectorSize=512;
	else if( OMSignature_Aaf_SSBin_4K == signature ) sectorSize=4096;
	PRECONDITION("Valid Signature", sectorSize!=0 );

  SSCHAR omFileName[256];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, 256);
#else
  copyWideString (omFileName, fileName, 256);
#endif

  IStorage* storage = 0;

  HRESULT status = OMSSIStorage::StgCreateStorageEx(
											omFileName,
											OMFile::modifyMode,
											(void **)&storage,
											sectorSize);
  checkStatus(status);
  ASSERT("StgCreateStorageEx() succeeded", SUCCEEDED(status));

  return OMMSSStoredObject::create(storage, byteOrder);
}

static void checkStatus(HRESULT status)
{
  TRACE("checkStatus");

  if (FAILED(status)) {
    throw OMException(status);
  }
}

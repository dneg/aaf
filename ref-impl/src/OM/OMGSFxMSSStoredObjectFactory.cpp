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
#include "OMGSFxMSSStoredObjectFactory.h"

#include "OMMSSStoredObject.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"
#include "OMGSFStructuredStorage.h"
#include "OMExceptions.h"

static void checkStatus(HRESULT status);

  // @mfunc Constructor.
OMGSFxMSSStoredObjectFactory::OMGSFxMSSStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMMSSStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMGSFxMSSStoredObjectFactory::OMGSFxMSSStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
  PRECONDITION("Valid signature",
                                signature != nullOMUniqueObjectIdentification);
}

  // @mfunc Destructor.
OMGSFxMSSStoredObjectFactory::~OMGSFxMSSStoredObjectFactory(void)
{
  TRACE("OMGSFxMSSStoredObjectFactory::~OMGSFxMSSStoredObjectFactory");
}

  // @mfunc Initialize this <c OMGSFxMSSStoredObjectFactory>.
void OMGSFxMSSStoredObjectFactory::initialize(void)
{
  TRACE("OMGSFxMSSStoredObjectFactory::initialize");
  OMGSFInitialize();
}

  // @mfunc Finalize this <c OMGSFxMSSStoredObjectFactory>.
void OMGSFxMSSStoredObjectFactory::finalize(void)
{
  TRACE("OMGSFxMSSStoredObjectFactory::finalize");
  OMGSFFinalize();
}

  // @mfunc Can a file be created successfully on the given
  //        <c OMRawStorage> and accessed successfully in the mode
  //        specified by <p accessMode> ?
  //   @parm The <c OMRawStorage>.
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMGSFxMSSStoredObjectFactory::compatibleRawStorage(
                                         const OMRawStorage* rawStorage,
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMGSFxMSSStoredObjectFactory::compatibleRawStorage");

  return false;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMGSFxMSSStoredObjectFactory::compatibleNamedFile(
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMGSFxMSSStoredObjectFactory::compatibleNamedFile");

  bool result = true;

  // Read/Write GSF Structured Storage files are not supported.
  // But currently there is no method to create write-only named files.
  // So the following code is commented out until it GSF support improves.
  // if (accessMode == OMFile::modifyMode) {
  //   result = false;
  // }
  return result;
}

OMStoredObject*
OMGSFxMSSStoredObjectFactory::openFile (OMRawStorage* rawStorage,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMGSFxMSSStoredObjectFactory::openFile");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));
  ASSERT("Unimplemented code to open GSF SS using raw storage not reached", 0);
  return 0;
}

OMStoredObject*
OMGSFxMSSStoredObjectFactory::createFile (OMRawStorage* rawStorage,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
	TRACE("OMGSFxMSSStoredObjectFactory::createFile");
	PRECONDITION("Valid raw storage", rawStorage != 0);

	ASSERT("Unimplemented code to create GSF file on raw storage not reached", 0);
	return 0;
}


OMStoredObject*
OMGSFxMSSStoredObjectFactory::openFile (const wchar_t* fileName,
                                     const OMFile::OMAccessMode mode)
{
  TRACE("OMGSFxMSSStoredObjectFactory::openFile");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid mode", (mode == OMFile::modifyMode) ||
                             (mode == OMFile::readOnlyMode));

  if (mode == OMFile::modifyMode) {
  	ASSERT("Unimplemented GSF open in Read Modify mode not reached", false);
		return 0;
  }
  SSCHAR omFileName[256];
#ifndef OM_UNICODE_APIS
  convertWideStringToString(omFileName, fileName, 256);
#else
  copyWideString (omFileName, fileName, 256);
#endif
  IStorage* storage = 0;

  HRESULT status = OMGSFIStorage::StgOpenStorageEx(
										omFileName,
										OMFile::readOnlyMode,
										(void **)&storage);
  checkStatus(status);
  ASSERT("StgOpenStorageEx() succeeded", SUCCEEDED(status));

  return OMMSSStoredObject::open(storage, mode);
}

OMStoredObject*
OMGSFxMSSStoredObjectFactory::createFile (const wchar_t* fileName,
                                  const OMByteOrder byteOrder,
   		                          const OMUniqueObjectIdentification& signature)
{
  TRACE("OMGSFxMSSStoredObjectFactory::createFile");
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

  HRESULT status = OMGSFIStorage::StgCreateStorageEx(
											omFileName,
											OMFile::writeOnlyMode,
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

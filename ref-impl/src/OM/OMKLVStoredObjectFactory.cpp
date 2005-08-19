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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMMSSStoredObjectFactory
#include "OMKLVStoredObjectFactory.h"

#include "OMKLVStoredObject.h"
#include "OMUtilities.h"
#include "OMAssertions.h"
#include "OMDiskRawStorage.h"
#include "OMMXFStorage.h"

  // @mfunc Constructor.
OMKLVStoredObjectFactory::OMKLVStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMKLVStoredObjectFactory::OMKLVStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
  PRECONDITION("Valid signature",
                                signature != nullOMUniqueObjectIdentification);
}

  // @mfunc Destructor.
OMKLVStoredObjectFactory::~OMKLVStoredObjectFactory(void)
{
  TRACE("OMKLVStoredObjectFactory::~OMKLVStoredObjectFactory");
}

  // @mfunc Initialize this <c OMKLVStoredObjectFactory>.
void OMKLVStoredObjectFactory::initialize(void)
{
  TRACE("OMKLVStoredObjectFactory::initialize");
}

  // @mfunc Finalize this <c OMKLVStoredObjectFactory>.
void OMKLVStoredObjectFactory::finalize(void)
{
  TRACE("OMKLVStoredObjectFactory::finalize");
}

  // @mfunc Open the root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the <c OMKLVStoredObject>.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMStoredObject*
OMKLVStoredObjectFactory::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMKLVStoredObjectFactory::openRead");
  OMMXFStorage* storage = new OMMXFStorage(rawStorage);
  ASSERT("Valid heap pointer", storage != 0);
  return OMKLVStoredObject::openRead(storage);
}

  // @mfunc Open the root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMKLVStoredObject>.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMStoredObject*
OMKLVStoredObjectFactory::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMKLVStoredObjectFactory::openModify");
  OMMXFStorage* storage = new OMMXFStorage(rawStorage);
  ASSERT("Valid heap pointer", storage != 0);
  return OMKLVStoredObject::openModify(storage);
}

  // @mfunc Create a new root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMKLVStoredObject>.
  //   @parm The desired byte ordering for the new <c OMKLVStoredObject>.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMStoredObject*
OMKLVStoredObjectFactory::createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder)
{
  TRACE("OMKLVStoredObjectFactory::createWrite");

  OMMXFStorage* storage = new OMMXFStorage(rawStorage);
  ASSERT("Valid heap pointer", storage != 0);
  OMKLVStoredObject* result = OMKLVStoredObject::createWrite(storage,
                                                             byteOrder);
  return result;
}

  // @mfunc Create a new root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMKLVStoredObject>.
  //   @parm The desired byte ordering for the new <c OMKLVStoredObject>.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMStoredObject*
OMKLVStoredObjectFactory::createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMKLVStoredObjectFactory::createModify");

  OMMXFStorage* storage = new OMMXFStorage(rawStorage);
  ASSERT("Valid heap pointer", storage != 0);
  OMKLVStoredObject* result = OMKLVStoredObject::createModify(storage,
                                                              byteOrder);
  return result;
}

  // @mfunc Open the root <c OMKLVStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMKLVStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMKLVStoredObjectFactory::openRead(const wchar_t* NNAME(fileName))
{
  TRACE("OMKLVStoredObjectFactory::openRead");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Open the root <c OMKLVStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMKLVStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMKLVStoredObjectFactory::openModify(const wchar_t* NNAME(fileName))
{
  TRACE("OMKLVStoredObjectFactory::openModify");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Create a new root <c OMKLVStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMKLVStoredObject>.
  //   @rdesc An <c OMKLVStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMKLVStoredObjectFactory::createModify(const wchar_t* NNAME(fileName),
                                       const OMByteOrder NNAME(byteOrder))
{
  TRACE("OMKLVStoredObjectFactory::creatModify");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Create a new root <c OMKLVStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMKLVStoredObject>.
  //   @rdesc An <c OMKLVStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMKLVStoredObjectFactory::createWrite(const wchar_t* NNAME(fileName),
                                      const OMByteOrder NNAME(byteOrder))
{
  TRACE("OMKLVStoredObjectFactory::creatWrite");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true.
  //   @parm The name of the file.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMKLVStoredObjectFactory::isRecognized(const wchar_t* fileName)
{
  TRACE("OMKLVStoredObjectFactory::isRecognized");
  bool result;
  OMRawStorage* rawStorage = OMDiskRawStorage::openExistingRead(fileName);
  if (rawStorage != 0) {
    result = isRecognized(rawStorage);
    delete rawStorage;
  } else {
    result = false;
  }
  return result;
}

  // @mfunc Does <p rawStorage> contain a recognized file ?
  //        If so, the result is true.
  //   @parm The raw storage.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMKLVStoredObjectFactory::isRecognized(OMRawStorage* rawStorage)
{
  TRACE("OMKLVStoredObjectFactory::isRecognized");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Positionable raw storage", rawStorage->isPositionable());

  bool result = OMKLVStoredObject::isRecognized(rawStorage);
  return result;
}

  // @mfunc Can a file be created successfully on the given
  //        <c OMRawStorage> and accessed successfully in the mode
  //        specified by <p accessMode> ?
  //   @parm The <c OMRawStorage>.
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMKLVStoredObjectFactory::compatibleRawStorage(
                                  const OMRawStorage* NNAME(rawStorage),
                                  const OMFile::OMAccessMode NNAME(accessMode))
{
  TRACE("OMKLVStoredObjectFactory::compatibleRawStorage");

  // tjb -- missing checks ?
  bool result = true;
  return result;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMKLVStoredObjectFactory::compatibleNamedFile(
                                  const OMFile::OMAccessMode NNAME(accessMode))
{
  TRACE("OMKLVStoredObjectFactory::compatibleNamedFile");

  // Directly accessed named files are not supported regardless of the
  // access mode. Named files are supported via a disk file based
  // implementation of OMRawStorage.
  bool result = false;
  return result;
}

  // @mfunc Perform any necessary actions when <p file> is closed.
  //   @parm The <c OMFile>
void OMKLVStoredObjectFactory::close(OMFile* file)
{
  TRACE("OMKLVStoredObjectFactory::close");

  PRECONDITION("Valid file", file != 0);

  OMRawStorage* store = file->rawStorage();
  if (store != 0) {
    store->synchronize();
  }

  if (OMKLVStoredObject::hasMxfStorage(file)) {
    OMMXFStorage* store = OMKLVStoredObject::mxfStorage(file);
    ASSERT("Valid store", store != 0);
    delete store;
  }
}

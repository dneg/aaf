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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#include "OMSSSStoredObjectFactory.h"

#include "OMSSSStoredObject.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"

#include <stdio.h>

  // @mfunc Constructor.
OMSSSStoredObjectFactory::OMSSSStoredObjectFactory(
                                        const OMStoredObjectEncoding& encoding,
																				const OMUniqueObjectIdentification& signature,
                                        const wchar_t* name,
                                        const wchar_t* description)
: OMStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMSSSStoredObjectFactory::OMSSSStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
  PRECONDITION("Valid signature",
                                signature != nullOMUniqueObjectIdentification);
}

  // @mfunc Destructor.
OMSSSStoredObjectFactory::~OMSSSStoredObjectFactory(void)
{
  TRACE("OMSSSStoredObjectFactory::~OMSSSStoredObjectFactory");
}

  // @mfunc Initialize this <c OMSSSStoredObjectFactory>.
void OMSSSStoredObjectFactory::initialize(void)
{
  TRACE("OMSSSStoredObjectFactory::initialize");
  OMSSSStoredObject::initialize();
}

  // @mfunc Finalize this <c OMSSSStoredObjectFactory>.
void OMSSSStoredObjectFactory::finalize(void)
{
  TRACE("OMSSSStoredObjectFactory::finalize");
  OMSSSStoredObject::finalize();
}

  // @mfunc Open the root <c OMSSSStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the <c OMSSSStoredObject>.
  //   @rdesc An <c OMSSSStoredObject> representing the root object.
OMStoredObject*
OMSSSStoredObjectFactory::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMSSSStoredObjectFactory::openRead");
  return OMSSSStoredObject::openRead(rawStorage);
}

  // @mfunc Open the root <c OMSSSStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMSSSStoredObject>.
  //   @rdesc An <c OMSSSStoredObject> representing the root object.
OMStoredObject*
OMSSSStoredObjectFactory::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMSSSStoredObjectFactory::openModify");

  writeSignature(rawStorage, nullOMUniqueObjectIdentification);
  return OMSSSStoredObject::openModify(rawStorage);
}

  // @mfunc Create a new root <c OMSSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMSSSStoredObject>.
  //   @parm The desired byte ordering for the new <c OMSSSStoredObject>.
  //   @rdesc An <c OMSSSStoredObject> representing the root object.
OMStoredObject*
OMSSSStoredObjectFactory::createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder)
{
  TRACE("OMSSSStoredObjectFactory::createWrite");
  ASSERT("Unimplemented code not reached", false);
  // MSS writable files must also be readable
	return 0;
}

  // @mfunc Create a new root <c OMSSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMSSSStoredObject>.
  //   @parm The desired byte ordering for the new <c OMSSSStoredObject>.
  //   @rdesc An <c OMSSSStoredObject> representing the root object.
OMStoredObject*
OMSSSStoredObjectFactory::createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMSSSStoredObjectFactory::createModify");
  return OMSSSStoredObject::createModify(rawStorage, byteOrder, signature());
}

  // @mfunc Open the root <c OMSSSStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMSSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMSSSStoredObjectFactory::openRead(const wchar_t* fileName)
{
  TRACE("OMSSSStoredObjectFactory::openRead");
  return OMSSSStoredObject::openRead(fileName);
}

  // @mfunc Open the root <c OMSSSStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMSSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMSSSStoredObjectFactory::openModify(const wchar_t* fileName)
{
  TRACE("OMSSSStoredObjectFactory::openModify");

  writeSignature(fileName, nullOMUniqueObjectIdentification);
  return OMSSSStoredObject::openModify(fileName);
}

  // @mfunc Create a new root <c OMSSSStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMSSSStoredObject>.
  //   @rdesc An <c OMSSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMSSSStoredObjectFactory::createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMSSSStoredObjectFactory::createModify");
  return OMSSSStoredObject::createModify(fileName, byteOrder, signature());
}

  // @mfunc Create a new root <c OMSSSStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMSSSStoredObject>.
  //   @rdesc An <c OMSSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMSSSStoredObjectFactory::createWrite(const wchar_t* /* fileName */,
                                      const OMByteOrder /* byteOrder */)
{
  TRACE("OMSSSStoredObjectFactory::createWrite");
  ASSERT("Unimplemented code not reached", false);
  // MSS writable files must also be readable
  return 0;
}

// Structured Storage signature.
//
static const OMByte MSSSignature[] =
{0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1};

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true.
  //   @parm The name of the file.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMSSSStoredObjectFactory::isRecognized(const wchar_t* fileName)
{
  TRACE("OMSSSStoredObjectFactory::isRecognized");
  PRECONDITION("Valid file name", validWideString(fileName));

  bool result = false;
  FILE* f = wfopen(fileName, L"rb");
  if (f != 0) {
    OMByte sssig[8];
    size_t status = fread(&sssig, sizeof(sssig), 1, f);
    if (status == 1) {
      if (memcmp(sssig, MSSSignature, sizeof(MSSSignature)) == 0) {
        OMUniqueObjectIdentification sig;
        OMByte* s = reinterpret_cast<OMByte*>(&sig);
        status = fread(s, sizeof(sig), 1, f);
        if (status == 1) {
          if (hostByteOrder() != littleEndian) {
            size_t size = sizeof(OMUniqueObjectIdentification);
            OMUniqueObjectIdentificationType::instance()->reorder(s, size);
          }
          // If we created this file we wrote a signature
          // Do they match ?
          if (sig == signature()) {
            result = true;
          }
        }
      }
    }
    fclose(f);
  }
  return result;
}

  // @mfunc Does <p rawStorage> contain a recognized file ?
  //        If so, the result is true.
  //   @parm The raw storage.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMSSSStoredObjectFactory::isRecognized(OMRawStorage* rawStorage)
{
  TRACE("OMSSSStoredObjectFactory::isRecognized");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Positionable raw storage", rawStorage->isPositionable());

  bool result = false;

  OMUInt32 count;
  OMByte sssig[8];
  rawStorage->readAt(0, sssig, sizeof(sssig), count);
  if (count == sizeof(sssig)) {
    if (memcmp(sssig, MSSSignature, sizeof(MSSSignature)) == 0) {
      OMUniqueObjectIdentification sig;
      OMByte* s = reinterpret_cast<OMByte*>(&sig);
      rawStorage->readAt(8, s, sizeof(sig), count);
      if (count == sizeof(sig)) {
        if (hostByteOrder() != littleEndian) {
          size_t size = sizeof(OMUniqueObjectIdentification);
          OMUniqueObjectIdentificationType::instance()->reorder(s, size);
        }
        // If we created this file we wrote a signature
        // Do they match ?
        if (sig == signature()) {
          result = true;
        }
      }
    }
  }
  rawStorage->setPosition(0);
  return result;
}

  // @mfunc Can a file be created successfully on the given
  //        <c OMRawStorage> and accessed successfully in the mode
  //        specified by <p accessMode> ?
  //   @parm The <c OMRawStorage>.
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMSSSStoredObjectFactory::compatibleRawStorage(
                                         const OMRawStorage* rawStorage,
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMSSSStoredObjectFactory::compatibleRawStorage");

  bool result = true;

  // Write only Structured Storage files are not permitted.
  if ((accessMode == OMFile::writeOnlyMode) && (!rawStorage->isReadable())) {
    result = false;
  }

  return result;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMSSSStoredObjectFactory::compatibleNamedFile(
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMSSSStoredObjectFactory::compatibleNamedFile");

  bool result = true;

  // Write only Structured Storage files are not
  // permitted. Note that support for directly accessed named files
  // may be withdrawn in future and replaced by a disk file based
  // implementation of OMRawStorage.
  if (accessMode == OMFile::writeOnlyMode) {
    result = false;
  }
  return result;
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p fileName> is closed.
  //   @parm The file name.
void OMSSSStoredObjectFactory::close(const wchar_t* fileName,
                                     bool isWritable)
{
  TRACE("OMSSSStoredObjectFactory::close");

  if (isWritable) {
    writeSignature(fileName, signature());
  }
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p rawStorage> is closed.
  //   @parm The <c OMRawStorage>
void OMSSSStoredObjectFactory::close(OMRawStorage* rawStorage,
                                     bool isWritable )
{
  TRACE("OMSSSStoredObjectFactory::close");

  if (isWritable) {
    writeSignature(rawStorage, signature());
  }
}

  // @mfunc Write the signature to the given raw storage.
  //   @parm The raw storage.
  //   @parm The signature.
void OMSSSStoredObjectFactory::writeSignature(
                                       OMRawStorage* rawStorage,
                                       const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSSStoredObjectFactory::writeSignature");

  OMUniqueObjectIdentification sig = signature;
  if (hostByteOrder() != littleEndian) {
    OMByte* s = reinterpret_cast<OMByte*>(&sig);
    size_t size = sizeof(OMUniqueObjectIdentification);
    OMUniqueObjectIdentificationType::instance()->reorder(s, size);
  }

  OMUInt32 count;
  rawStorage->writeAt(8,
                      reinterpret_cast<const OMByte*>(&sig),
                      sizeof(sig),
                      count);
  ASSERT("All bytes written", count == sizeof(sig));
}

  // @mfunc Write the signature to the given file.
  //   @parm The file name.
  //   @parm The signature.
void OMSSSStoredObjectFactory::writeSignature(
                                       const wchar_t* fileName,
                                       const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSSStoredObjectFactory::writeSignature");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMUniqueObjectIdentification sig = signature;

  if (hostByteOrder() != littleEndian) {
    OMByte* s = reinterpret_cast<OMByte*>(&sig);
    size_t size = sizeof(OMUniqueObjectIdentification);
    OMUniqueObjectIdentificationType::instance()->reorder(s, size);
  }

  FILE* f = wfopen(fileName, L"rb+");
  ASSERT("File exists", f != 0);
  size_t status = fseek(f, 8, SEEK_SET);
  ASSERT("Seek succeeded", status == 0);
  status = fwrite(&sig, sizeof(sig), 1, f);
  ASSERT("Write succeeded", status == 1);

  fclose(f);
}

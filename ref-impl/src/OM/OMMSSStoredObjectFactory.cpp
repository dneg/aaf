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
#include "OMMSSStoredObjectFactory.h"

#include "OMMSSStoredObject.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"

#include <stdio.h>

  // @mfunc Constructor.
OMMSSStoredObjectFactory::OMMSSStoredObjectFactory(
                                        const OMStoredObjectEncoding& encoding,
                                        const wchar_t* name,
                                        const wchar_t* description)
: OMStoredObjectFactory(encoding, name, description)
{
  TRACE("OMMSSStoredObjectFactory::OMMSSStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
}

  // @mfunc Destructor.
OMMSSStoredObjectFactory::~OMMSSStoredObjectFactory(void)
{
  TRACE("OMMSSStoredObjectFactory::~OMMSSStoredObjectFactory");
}

  // @mfunc Initialize this <c OMMSSStoredObjectFactory>.
void OMMSSStoredObjectFactory::initialize(void)
{
  TRACE("OMMSSStoredObjectFactory::initialize");
  OMMSSStoredObject::initialize();
}

  // @mfunc Finalize this <c OMMSSStoredObjectFactory>.
void OMMSSStoredObjectFactory::finalize(void)
{
  TRACE("OMMSSStoredObjectFactory::finalize");
  OMMSSStoredObject::finalize();
}

  // @mfunc Open the root <c OMMSSStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the <c OMMSSStoredObject>.
  //   @rdesc An <c OMMSSStoredObject> representing the root object.
OMStoredObject*
OMMSSStoredObjectFactory::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMMSSStoredObjectFactory::openRead");
  return OMMSSStoredObject::openRead(rawStorage);
}

  // @mfunc Open the root <c OMMSSStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMMSSStoredObject>.
  //   @rdesc An <c OMMSSStoredObject> representing the root object.
OMStoredObject*
OMMSSStoredObjectFactory::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMMSSStoredObjectFactory::openModify");

  writeSignature(rawStorage, nullOMStoredObjectEncoding);
  return OMMSSStoredObject::openModify(rawStorage);
}

  // @mfunc Create a new root <c OMMSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMMSSStoredObject>.
  //   @parm The desired byte ordering for the new <c OMMSSStoredObject>.
  //   @rdesc An <c OMMSSStoredObject> representing the root object.
OMStoredObject*
OMMSSStoredObjectFactory::createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder)
{
  TRACE("OMMSSStoredObjectFactory::createWrite");
  return OMMSSStoredObject::createWrite(rawStorage, byteOrder);
}

  // @mfunc Create a new root <c OMMSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMMSSStoredObject>.
  //   @parm The desired byte ordering for the new <c OMMSSStoredObject>.
  //   @rdesc An <c OMMSSStoredObject> representing the root object.
OMStoredObject*
OMMSSStoredObjectFactory::createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMMSSStoredObjectFactory::createModify");
  return OMMSSStoredObject::createModify(rawStorage, byteOrder);
}

  // @mfunc Open the root <c OMMSSStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMMSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMMSSStoredObjectFactory::openRead(const wchar_t* fileName)
{
  TRACE("OMMSSStoredObjectFactory::openRead");
  return OMMSSStoredObject::openRead(fileName);
}

  // @mfunc Open the root <c OMMSSStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMMSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMMSSStoredObjectFactory::openModify(const wchar_t* fileName)
{
  TRACE("OMMSSStoredObjectFactory::openModify");

  writeSignature(fileName, nullOMStoredObjectEncoding);
  return OMMSSStoredObject::openModify(fileName);
}

  // @mfunc Create a new root <c OMMSSStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMMSSStoredObject>.
  //   @rdesc An <c OMMSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMMSSStoredObjectFactory::createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMMSSStoredObjectFactory::creatModify");
  return OMMSSStoredObject::createModify(fileName, byteOrder);
}

  // @mfunc Create a new root <c OMMSSStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMMSSStoredObject>.
  //   @rdesc An <c OMMSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMMSSStoredObjectFactory::createWrite(const wchar_t* /* fileName */,
                                      const OMByteOrder /* byteOrder */)
{
  TRACE("OMMSSStoredObjectFactory::creatWrite");
  ASSERT("Unimplemented code not reached", false);
  // MSS writable files must also be readable
//return OMMSSStoredObject::createWrite(fileName, byteOrder);
  return 0;
}

// Microsoft Structured Storage signature.
//
static const OMByte MSSSignature[] =
{0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1};

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true.
  //   @parm The name of the file.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMMSSStoredObjectFactory::isRecognized(const wchar_t* fileName)
{
  TRACE("OMMSSStoredObjectFactory::isRecognized");
  PRECONDITION("Valid file name", validWideString(fileName));

  bool result = false;
  FILE* f = wfopen(fileName, L"rb");
  if (f != 0) {
    OMByte sssig[8];
    size_t status = fread(&sssig, sizeof(sssig), 1, f);
    if (status == 1) {
      if (memcmp(sssig, MSSSignature, sizeof(MSSSignature)) == 0) {
        OMStoredObjectEncoding sig;
        OMByte* s = reinterpret_cast<OMByte*>(&sig);
        status = fread(s, sizeof(sig), 1, f);
        if (status == 1) {
          if (hostByteOrder() != littleEndian) {
            size_t size = sizeof(OMUniqueObjectIdentification);
            OMUniqueObjectIdentificationType::instance()->reorder(s, size);
          }
          // If we created this file we wrote a signature whose value is the
          // encoding. Do they match ?
          if (sig == encoding()) {
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
OMMSSStoredObjectFactory::isRecognized(OMRawStorage* rawStorage)
{
  TRACE("OMMSSStoredObjectFactory::isRecognized");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Positionable raw storage", rawStorage->isPositionable());

  bool result = false;

  OMUInt32 count;
  OMByte sssig[8];
  rawStorage->readAt(0, sssig, sizeof(sssig), count);
  if (count == sizeof(sssig)) {
    if (memcmp(sssig, MSSSignature, sizeof(MSSSignature)) == 0) {
      OMStoredObjectEncoding sig;
      OMByte* s = reinterpret_cast<OMByte*>(&sig);
      rawStorage->readAt(8, s, sizeof(sig), count);
      if (count == sizeof(sig)) {
        if (hostByteOrder() != littleEndian) {
          size_t size = sizeof(OMUniqueObjectIdentification);
          OMUniqueObjectIdentificationType::instance()->reorder(s, size);
        }
        // If we created this file we wrote a signature whose value is the
        // encoding. Do they match ?
        if (sig == encoding()) {
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
bool OMMSSStoredObjectFactory::compatibleRawStorage(
                                         const OMRawStorage* rawStorage,
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMMSSStoredObjectFactory::compatibleRawStorage");

  bool result = true;

  // Write only Microsoft Structured Storage files are not permitted.
  if ((accessMode == OMFile::writeOnlyMode) && (!rawStorage->isReadable())) {
    result = false;
  }

  return result;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMMSSStoredObjectFactory::compatibleNamedFile(
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMMSSStoredObjectFactory::compatibleNamedFile");

  bool result = true;

  // Write only Microsoft Structured Storage files are not
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
void OMMSSStoredObjectFactory::close(const wchar_t* fileName,
                                     bool isWritable)
{
  TRACE("OMMSSStoredObjectFactory::close");

  if (isWritable) {
    // The encoding is used as the signature.
    writeSignature(fileName, encoding());
  }
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p rawStorage> is closed.
  //   @parm The <c OMRawStorage>
void OMMSSStoredObjectFactory::close(OMRawStorage* rawStorage,
                                     bool isWritable )
{
  TRACE("OMMSSStoredObjectFactory::close");

  if (isWritable) {
    // The encoding is used as the signature.
    writeSignature(rawStorage, encoding());
  }
}

  // @mfunc Write the signature to the given raw storage.
  //   @parm The raw storage.
  //   @parm The signature.
void OMMSSStoredObjectFactory::writeSignature(
                                       OMRawStorage* rawStorage,
                                       const OMStoredObjectEncoding& signature)
{
  TRACE("OMMSSStoredObjectFactory::writeSignature");

  OMStoredObjectEncoding sig = signature;
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
void OMMSSStoredObjectFactory::writeSignature(
                                       const wchar_t* fileName,
                                       const OMStoredObjectEncoding& signature)
{
  TRACE("OMMSSStoredObjectFactory::writeSignature");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStoredObjectEncoding sig = signature;

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

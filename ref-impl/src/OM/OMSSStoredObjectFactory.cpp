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

#ifndef OM_NO_STRUCTURED_STORAGE

#include "OMSSStoredObjectFactory.h"

#include "OMSSStoredObject.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMRawStorage.h"
#include "OMUniqueObjectIdentType.h"

  // @mfunc Constructor.
OMSSStoredObjectFactory::OMSSStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMSSStoredObjectFactory::OMSSStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
  PRECONDITION("Valid signature",
                                signature != nullOMUniqueObjectIdentification);
}

  // @mfunc Destructor.
OMSSStoredObjectFactory::~OMSSStoredObjectFactory(void)
{
  TRACE("OMSSStoredObjectFactory::~OMSSStoredObjectFactory");
}

  // @mfunc Open the root <c OMSSStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the <c OMSSStoredObject>.
  //   @rdesc An <c OMSSStoredObject> representing the root object.
OMStoredObject*
OMSSStoredObjectFactory::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMSSStoredObjectFactory::openRead");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible raw storage access mode", rawStorage->isReadable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable());

  return openFile(rawStorage, OMFile::readOnlyMode);
}

  // @mfunc Open the root <c OMSSStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMSSStoredObject>.
  //   @rdesc An <c OMSSStoredObject> representing the root object.
OMStoredObject*
OMSSStoredObjectFactory::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMSSStoredObjectFactory::openModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable() &&
                                         rawStorage->isExtendible());

  writeSignature(rawStorage, nullOMUniqueObjectIdentification);
  return openFile(rawStorage, OMFile::modifyMode);
}

  // @mfunc Create a new root <c OMSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMSSStoredObject>.
  //   @parm The desired byte ordering for the new <c OMSSStoredObject>.
  //   @rdesc An <c OMSSStoredObject> representing the root object.
OMStoredObject*
OMSSStoredObjectFactory::createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder)
{
  TRACE("OMSSStoredObjectFactory::createWrite");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Create a new root <c OMSSStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMSSStoredObject>.
  //   @parm The desired byte ordering for the new <c OMSSStoredObject>.
  //   @rdesc An <c OMSSStoredObject> representing the root object.
OMStoredObject*
OMSSStoredObjectFactory::createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMSSStoredObjectFactory::createModify");

  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));
  PRECONDITION("Compatible raw storage access mode",
                         rawStorage->isReadable() && rawStorage->isWritable());
  PRECONDITION("Compatible raw storage", rawStorage->isPositionable() &&
                                         rawStorage->isExtendible());

  return createFile(rawStorage, byteOrder, signature());
}

  // @mfunc Open the root <c OMSSStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMSSStoredObjectFactory::openRead(const wchar_t* fileName)
{
  TRACE("OMSSStoredObjectFactory::openRead");
  PRECONDITION("Valid file name", validWideString(fileName));

  return openFile(fileName, OMFile::readOnlyMode);
}

  // @mfunc Open the root <c OMSSStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMSSStoredObjectFactory::openModify(const wchar_t* fileName)
{
  TRACE("OMSSStoredObjectFactory::openModify");
  PRECONDITION("Valid file name", validWideString(fileName));

  writeSignature(fileName, nullOMUniqueObjectIdentification);
  return openFile(fileName, OMFile::modifyMode);
}

  // @mfunc Create a new root <c OMSSStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMSSStoredObject>.
  //   @rdesc An <c OMSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMSSStoredObjectFactory::createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMSSStoredObjectFactory::createModify");
  PRECONDITION("Valid file name", validWideString(fileName));
  PRECONDITION("Valid byte order",
                      (byteOrder == littleEndian) || (byteOrder == bigEndian));

  return createFile(fileName, byteOrder, signature());
}

  // @mfunc Create a new root <c OMSSStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMSSStoredObject>.
  //   @rdesc An <c OMSSStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMSSStoredObjectFactory::createWrite(const wchar_t* /* fileName */,
                                      const OMByteOrder /* byteOrder */)
{
  TRACE("OMSSStoredObjectFactory::createWrite");
  ASSERT("Unimplemented code not reached", false);
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
OMSSStoredObjectFactory::isRecognized(const wchar_t* fileName)
{
  TRACE("OMSSStoredObjectFactory::isRecognized");
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
            OMUInt32 size = static_cast<OMUInt32>(sizeof(sig));
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
OMSSStoredObjectFactory::isRecognized(OMRawStorage* rawStorage)
{
  TRACE("OMSSStoredObjectFactory::isRecognized");
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
          OMUInt32 size = static_cast<OMUInt32>(sizeof(sig));
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
bool OMSSStoredObjectFactory::compatibleRawStorage(
                                         const OMRawStorage* rawStorage,
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMSSStoredObjectFactory::compatibleRawStorage");

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
bool OMSSStoredObjectFactory::compatibleNamedFile(
                                         const OMFile::OMAccessMode accessMode)
{
  TRACE("OMSSStoredObjectFactory::compatibleNamedFile");

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
void OMSSStoredObjectFactory::close(const wchar_t* fileName,
                                     bool isWritable)
{
  TRACE("OMSSStoredObjectFactory::close");

  if (isWritable) {
    writeSignature(fileName, signature());
  }
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p rawStorage> is closed.
  //   @parm The <c OMRawStorage>
void OMSSStoredObjectFactory::close(OMRawStorage* rawStorage,
                                     bool isWritable )
{
  TRACE("OMSSStoredObjectFactory::close");

  if (isWritable) {
    writeSignature(rawStorage, signature());
  }
}

  // @mfunc Perform any necessary actions when <p file> is closed.
  //   @parm The <c OMFile>
void OMSSStoredObjectFactory::close(OMFile* file)
{
  TRACE("OMSSStoredObjectFactory::close");

  PRECONDITION("Valid file", file != 0);

  const bool isWritable = file->isWritable();
  OMRawStorage* store = file->rawStorage();
  if (store != 0) {
    if (isWritable) {
      this->writeSignature(store, this->signature());
    }
    store->synchronize();
  } else if (isWritable) {
    const wchar_t* fileName = file->fileName();
    this->writeSignature(fileName, this->signature());
  }
}

  // @mfunc Write the signature to the given raw storage.
  //   @parm The raw storage.
  //   @parm The signature.
void OMSSStoredObjectFactory::writeSignature(
                                 OMRawStorage* rawStorage,
                                 const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSStoredObjectFactory::writeSignature");

  OMUniqueObjectIdentification sig = signature;
  if (hostByteOrder() != littleEndian) {
    OMByte* s = reinterpret_cast<OMByte*>(&sig);
    OMUInt32 size = static_cast<OMUInt32>(sizeof(sig));
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
void OMSSStoredObjectFactory::writeSignature(
                                 const wchar_t* fileName,
                                 const OMUniqueObjectIdentification& signature)
{
  TRACE("OMSSStoredObjectFactory::writeSignature");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMUniqueObjectIdentification sig = signature;

  if (hostByteOrder() != littleEndian) {
    OMByte* s = reinterpret_cast<OMByte*>(&sig);
    OMUInt32 size = static_cast<OMUInt32>(sizeof(sig));
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

#endif // !OM_NO_STRUCTURED_STORAGE

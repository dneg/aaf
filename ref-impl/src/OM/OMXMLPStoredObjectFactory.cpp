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
#include "OMXMLPStoredObjectFactory.h"

#include "OMXMLPStoredObject.h"
#include "OMUtilities.h"
#include "OMAssertions.h"
#include "OMRawStorage.h"
#include "OMDiskRawStorage.h"

#include <ctype.h>

  // @mfunc Constructor.
OMXMLPStoredObjectFactory::OMXMLPStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMXMLPStoredObjectFactory::OMXMLPStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
}

  // @mfunc Destructor.
OMXMLPStoredObjectFactory::~OMXMLPStoredObjectFactory(void)
{
  TRACE("OMXMLPStoredObjectFactory::~OMXMLPStoredObjectFactory");
}

  // @mfunc Initialize this <c OMXMLPStoredObjectFactory>.
void OMXMLPStoredObjectFactory::initialize(void)
{
  TRACE("OMXMLPStoredObjectFactory::initialize");
}

  // @mfunc Finalize this <c OMXMLPStoredObjectFactory>.
void OMXMLPStoredObjectFactory::finalize(void)
{
  TRACE("OMXMLPStoredObjectFactory::finalize");
}

  // @mfunc Open the root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMStoredObject*
OMXMLPStoredObjectFactory::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMXMLPStoredObjectFactory::openRead");
  return OMXMLPStoredObject::openRead(rawStorage);
}

  // @mfunc Open the root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMStoredObject*
OMXMLPStoredObjectFactory::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMXMLPStoredObjectFactory::openModify");
  return OMXMLPStoredObject::openModify(rawStorage);
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMXMLPStoredObject>.
  //   @parm The desired byte ordering for the new <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMStoredObject*
OMXMLPStoredObjectFactory::createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder NNAME(byteOrder))
{
  TRACE("OMXMLPStoredObjectFactory::createWrite");
  return OMXMLPStoredObject::createWrite(rawStorage);
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMXMLPStoredObject>.
  //   @parm The desired byte ordering for the new <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMStoredObject*
OMXMLPStoredObjectFactory::createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder NNAME(byteOrder))
{
  TRACE("OMXMLPStoredObjectFactory::createModify");
  return OMXMLPStoredObject::createModify(rawStorage);
}

  // @mfunc Open the root <c OMXMLPStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLPStoredObjectFactory::openRead(const wchar_t* /* fileName */)
{
  TRACE("OMXMLPStoredObjectFactory::openRead");
  ASSERT("Unimplemented code not reached", false);
//return OMXMLPStoredObject::openRead(fileName);
  return 0;
}

  // @mfunc Open the root <c OMXMLPStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLPStoredObjectFactory::openModify(const wchar_t* /* fileName */)
{
  TRACE("OMXMLPStoredObjectFactory::openModify");
  ASSERT("Unimplemented code not reached", false);
//return OMXMLPStoredObject::openModify(fileName);
  return 0;
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLPStoredObjectFactory::createModify(const wchar_t* NNAME(fileName),
                                       const OMByteOrder NNAME(byteOrder))
{
  TRACE("OMXMLPStoredObjectFactory::createModify");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLPStoredObjectFactory::createWrite(const wchar_t* /* fileName */,
                                      const OMByteOrder /* byteOrder */)
{
  TRACE("OMXMLPStoredObjectFactory::creatWrite");
  ASSERT("Unimplemented code not reached", false);
//return OMXMLPStoredObject::createWrite(fileName, byteOrder);
  return 0;
}

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true.
  //   @parm The name of the file.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMXMLPStoredObjectFactory::isRecognized(const wchar_t* fileName)
{
  TRACE("OMXMLPStoredObjectFactory::isRecognized");
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
OMXMLPStoredObjectFactory::isRecognized(OMRawStorage* rawStorage)
{
  TRACE("OMXMLPStoredObjectFactory::isRecognized");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Positionable raw storage", rawStorage->isPositionable());

  char signature[] = "<?XML VERSION=\"1.0\"?>"
  "<?OM SIGNATURE=\"{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}\"?>";
  char* p = strchr(signature, '{');
  ASSERT("Found place holder", p != 0);
  toString(encoding(), p);
  size_t bufferSize = strlen(signature) + 1;
  char* buffer = new char[bufferSize];
  ASSERT("Valid heap pointer", buffer != 0);
  bool result = readSignature(rawStorage, buffer, bufferSize);
  if (result) {
    if (strcmp(signature, buffer) != 0) {
      result = false;
    }
  }
  delete [] buffer;
  rawStorage->setPosition(0);
  return result;
}

  // @mfunc Can a file be created successfully on the given
  //        <c OMRawStorage> and accessed successfully in the mode
  //        specified by <p accessMode> ?
  //   @parm The <c OMRawStorage>.
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMXMLPStoredObjectFactory::compatibleRawStorage(
                                  const OMRawStorage* NNAME(rawStorage),
                                  const OMFile::OMAccessMode NNAME(accessMode))
{
  TRACE("OMXMLPStoredObjectFactory::compatibleRawStorage");

  // tjb -- missing checks ?
  bool result = true;
  return result;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMXMLPStoredObjectFactory::compatibleNamedFile(
                                  const OMFile::OMAccessMode NNAME(accessMode))
{
  TRACE("OMXMLPStoredObjectFactory::compatibleNamedFile");

  // Directly accessed named files are not supported regardless of the
  // access mode. Named files are supported via a disk file based
  // implementation of OMRawStorage.
  bool result = false;
  return result;
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p fileName> is closed.
  //   @parm The file name.
void OMXMLPStoredObjectFactory::close(const wchar_t* /* fileName */,
                                     bool /* isWritable */)
{
  TRACE("OMXMLPStoredObjectFactory::close");
  // Nothing to do.
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p rawStorage> is closed.
  //   @parm The <c OMRawStorage>
void OMXMLPStoredObjectFactory::close(OMRawStorage* /* rawStorage */,
                                     bool /* isWritable */)
{
  TRACE("OMXMLPStoredObjectFactory::close");
  // Nothing to do.
}

  // @mfunc Read the signature from the given raw storage.
  //   @parm TBS
  //   @parm TBS
  //   @parm TBS
bool OMXMLPStoredObjectFactory::readSignature(OMRawStorage* rawStorage,
                                             char* signature,
                                             size_t signatureSize)
{
  TRACE("OMXMLPStoredObjectFactory::readSignature");
  size_t index = 0;
  while (index < signatureSize - 1) {
    unsigned char ch;
    OMUInt32 bytesRead;
    rawStorage->read(reinterpret_cast<OMByte*>(&ch), 1, bytesRead);
    if (bytesRead != 1) {
      break;
    }
    int c = ch;
    if (isprint(c)) {
      signature[index++] = toupper(ch);
    }
  }
  bool result;
  if (index == (signatureSize - 1)) {
  signature[index] = 0;
    result = true;
  } else {
    result = false;
  }
  return result;
}

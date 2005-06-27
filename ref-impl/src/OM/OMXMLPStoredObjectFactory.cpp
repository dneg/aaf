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
#include "OMDiskRawStorage.h"
#include "OMExceptions.h"


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
OMXMLPStoredObjectFactory::openRead(OMRawStorage* /* rawStorage */)
{
  TRACE("OMXMLPStoredObjectFactory::openRead");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Open the root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMStoredObject*
OMXMLPStoredObjectFactory::openModify(OMRawStorage* /* rawStorage */)
{
  TRACE("OMXMLPStoredObjectFactory::openModify");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMXMLPStoredObject>.
  //   @parm The desired byte ordering for the new <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMStoredObject*
OMXMLPStoredObjectFactory::createWrite(OMRawStorage* /* rawStorage */,
                                      const OMByteOrder /* byteOrder */)
{
  TRACE("OMXMLPStoredObjectFactory::createWrite");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMXMLPStoredObject>.
  //   @parm The desired byte ordering for the new <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object.
OMStoredObject*
OMXMLPStoredObjectFactory::createModify(OMRawStorage* /* rawStorage */,
                                       const OMByteOrder /* byteOrder */)
{
  TRACE("OMXMLPStoredObjectFactory::createModify");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Open the root <c OMXMLPStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLPStoredObjectFactory::openRead(const wchar_t* fileName )
{
  TRACE("OMXMLPStoredObjectFactory::openRead");
  OMDiskRawStorage* storage = OMDiskRawStorage::openExistingRead(fileName);
  return OMXMLPStoredObject::openRead(storage);
}

  // @mfunc Open the root <c OMXMLPStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLPStoredObjectFactory::openModify(const wchar_t* fileName)
{
  TRACE("OMXMLPStoredObjectFactory::openModify");
  OMDiskRawStorage* storage = OMDiskRawStorage::openExistingModify(fileName);
  return OMXMLPStoredObject::openModify(storage);
}

  // @mfunc Create a new root <c OMXMLPStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMXMLPStoredObject>.
  //   @rdesc An <c OMXMLPStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLPStoredObjectFactory::createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMXMLPStoredObjectFactory::createModify");
  OMDiskRawStorage* storage = OMDiskRawStorage::openNewModify(fileName);
  return OMXMLPStoredObject::createModify(storage);
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

  bool isRecog = false;
  OMXMLReader* reader = 0;
  try
  {
    reader = OMXMLReader::create(rawStorage);
    if (reader->nextElement() &&
          reader->elementEquals(OMSymbolspace::getBaselineURI(), L"AAF"))
    {
        isRecog = true;
    }
  }
  catch (...)
  {}

  if (reader != 0)
  {
    delete reader;
  }
  
  rawStorage->setPosition(0);
  
  return isRecog;
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
  // raw storage is not supported
  return false;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMXMLPStoredObjectFactory::compatibleNamedFile(
                                  const OMFile::OMAccessMode NNAME(accessMode))
{
  TRACE("OMXMLPStoredObjectFactory::compatibleNamedFile");

  return true;
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
bool OMXMLPStoredObjectFactory::readSignature(OMRawStorage* /* rawStorage */,
                                             char* /* signature */,
                                             size_t /* signatureSize */)
{
  TRACE("OMXMLPStoredObjectFactory::readSignature");
  ASSERT("Unimplemented code not reached", false);
  return false;
}

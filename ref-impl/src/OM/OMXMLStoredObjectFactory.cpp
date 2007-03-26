//=---------------------------------------------------------------------=
//
// $Id$
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
#include "OMXMLStoredObjectFactory.h"

#include "OMXMLStoredObject.h"
#include "OMUtilities.h"
#include "OMAssertions.h"
#include "OMDiskRawStorage.h"
#include "OMExceptions.h"


#include <ctype.h>

  // @mfunc Constructor.
OMXMLStoredObjectFactory::OMXMLStoredObjectFactory(
                                 const OMStoredObjectEncoding& encoding,
                                 const OMUniqueObjectIdentification& signature,
                                 const wchar_t* name,
                                 const wchar_t* description)
: OMStoredObjectFactory(encoding, signature, name, description)
{
  TRACE("OMXMLStoredObjectFactory::OMXMLStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
}

  // @mfunc Destructor.
OMXMLStoredObjectFactory::~OMXMLStoredObjectFactory(void)
{
  TRACE("OMXMLStoredObjectFactory::~OMXMLStoredObjectFactory");
}

  // @mfunc Initialize this <c OMXMLStoredObjectFactory>.
void OMXMLStoredObjectFactory::initialize(void)
{
  TRACE("OMXMLStoredObjectFactory::initialize");
}

  // @mfunc Finalize this <c OMXMLStoredObjectFactory>.
void OMXMLStoredObjectFactory::finalize(void)
{
  TRACE("OMXMLStoredObjectFactory::finalize");
}

  // @mfunc Open the root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage> for reading only.
  //   @parm The raw storage in which to open the <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMStoredObject*
OMXMLStoredObjectFactory::openRead(OMRawStorage* /* rawStorage */)
{
  TRACE("OMXMLStoredObjectFactory::openRead");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Open the root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMStoredObject*
OMXMLStoredObjectFactory::openModify(OMRawStorage* /* rawStorage */)
{
  TRACE("OMXMLStoredObjectFactory::openModify");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMXMLStoredObject>.
  //   @parm The desired byte ordering for the new <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMStoredObject*
OMXMLStoredObjectFactory::createWrite(OMRawStorage* /* rawStorage */,
                                      const OMByteOrder /* byteOrder */)
{
  TRACE("OMXMLStoredObjectFactory::createWrite");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMXMLStoredObject>.
  //   @parm The desired byte ordering for the new <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMStoredObject*
OMXMLStoredObjectFactory::createModify(OMRawStorage* /* rawStorage */,
                                       const OMByteOrder /* byteOrder */)
{
  TRACE("OMXMLStoredObjectFactory::createModify");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Open the root <c OMXMLStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMXMLStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLStoredObjectFactory::openRead(const wchar_t* fileName )
{
  TRACE("OMXMLStoredObjectFactory::openRead");
#if !defined(HAVE_EXPAT)
    ASSERT("Expat library available", false);
    return 0;
#else

  OMDiskRawStorage* storage = OMDiskRawStorage::openExistingRead(fileName);
  return OMXMLStoredObject::openRead(storage);
#endif  
}

  // @mfunc Open the root <c OMXMLStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMXMLStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLStoredObjectFactory::openModify(const wchar_t* fileName)
{
  TRACE("OMXMLStoredObjectFactory::openModify");
#if !defined(HAVE_EXPAT)
    ASSERT("Expat library available", false);
    return 0;
#else

  OMDiskRawStorage* storage = OMDiskRawStorage::openExistingModify(fileName);
  return OMXMLStoredObject::openModify(storage);
#endif  
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLStoredObjectFactory::createModify(const wchar_t* fileName,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMXMLStoredObjectFactory::createModify");
#if !defined(HAVE_EXPAT)
    ASSERT("Expat library available", false);
    return 0;
#else

  OMDiskRawStorage* storage = OMDiskRawStorage::openNewModify(fileName);
  return OMXMLStoredObject::createModify(storage);
#endif  
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLStoredObjectFactory::createWrite(const wchar_t* /* fileName */,
                                      const OMByteOrder /* byteOrder */)
{
  TRACE("OMXMLStoredObjectFactory::creatWrite");
  ASSERT("Unimplemented code not reached", false);
  return 0;
}

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true.
  //   @parm The name of the file.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMXMLStoredObjectFactory::isRecognized(const wchar_t* fileName)
{
  TRACE("OMXMLStoredObjectFactory::isRecognized");
  
#if !defined(HAVE_EXPAT)
    return false;
#else

  bool result;
  OMRawStorage* rawStorage = OMDiskRawStorage::openExistingRead(fileName);
  if (rawStorage != 0) {
    result = isRecognized(rawStorage);
    delete rawStorage;
  } else {
    result = false;
  }
  return result;
#endif  
}

  // @mfunc Does <p rawStorage> contain a recognized file ?
  //        If so, the result is true.
  //   @parm The raw storage.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMXMLStoredObjectFactory::isRecognized(OMRawStorage* rawStorage)
{
  TRACE("OMXMLStoredObjectFactory::isRecognized");
  PRECONDITION("Valid raw storage", rawStorage != 0);
  PRECONDITION("Positionable raw storage", rawStorage->isPositionable());

#if !defined(HAVE_EXPAT)
    return false;
#else

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
#endif  
}

  // @mfunc Can a file be created successfully on the given
  //        <c OMRawStorage> and accessed successfully in the mode
  //        specified by <p accessMode> ?
  //   @parm The <c OMRawStorage>.
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMXMLStoredObjectFactory::compatibleRawStorage(
                                  const OMRawStorage* NNAME(rawStorage),
                                  const OMFile::OMAccessMode NNAME(accessMode))
{
  TRACE("OMXMLStoredObjectFactory::compatibleRawStorage");
  // raw storage is not supported
  return false;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMXMLStoredObjectFactory::compatibleNamedFile(
                                  const OMFile::OMAccessMode NNAME(accessMode))
{
  TRACE("OMXMLStoredObjectFactory::compatibleNamedFile");

  return true;
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p fileName> is closed.
  //   @parm The file name.
void OMXMLStoredObjectFactory::close(const wchar_t* /* fileName */,
                                     bool /* isWritable */)
{
  TRACE("OMXMLStoredObjectFactory::close");
  // Nothing to do.
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p rawStorage> is closed.
  //   @parm The <c OMRawStorage>
void OMXMLStoredObjectFactory::close(OMRawStorage* /* rawStorage */,
                                     bool /* isWritable */)
{
  TRACE("OMXMLStoredObjectFactory::close");
  // Nothing to do.
}

  // @mfunc Read the signature from the given raw storage.
  //   @parm TBS
  //   @parm TBS
  //   @parm TBS
bool OMXMLStoredObjectFactory::readSignature(OMRawStorage* /* rawStorage */,
                                             char* /* signature */,
                                             OMUInt32 /* signatureSize */)
{
  TRACE("OMXMLStoredObjectFactory::readSignature");
  ASSERT("Unimplemented code not reached", false);
  return false;
}

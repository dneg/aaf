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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
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

  // @mfunc Constructor.
OMXMLStoredObjectFactory::OMXMLStoredObjectFactory(
                                        const OMStoredObjectEncoding& encoding,
                                        const wchar_t* name)
: OMStoredObjectFactory(encoding, name)
{
  TRACE("OMXMLStoredObjectFactory::OMXMLStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
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
OMXMLStoredObjectFactory::openRead(OMRawStorage* rawStorage)
{
  TRACE("OMXMLStoredObjectFactory::openRead");
  return OMXMLStoredObject::openRead(rawStorage);
}

  // @mfunc Open the root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMStoredObject*
OMXMLStoredObjectFactory::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMXMLStoredObjectFactory::openModify");
  return OMXMLStoredObject::openModify(rawStorage);
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMXMLStoredObject>.
  //   @parm The desired byte ordering for the new <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMStoredObject*
OMXMLStoredObjectFactory::createWrite(OMRawStorage* rawStorage,
                                      const OMByteOrder byteOrder)
{
  TRACE("OMXMLStoredObjectFactory::createWrite");
  return OMXMLStoredObject::createWrite(rawStorage, byteOrder);
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the raw storage
  //        <p rawStorage>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The raw storage in which to create the <c OMXMLStoredObject>.
  //   @parm The desired byte ordering for the new <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object.
OMStoredObject*
OMXMLStoredObjectFactory::createModify(OMRawStorage* rawStorage,
                                       const OMByteOrder byteOrder)
{
  TRACE("OMXMLStoredObjectFactory::createModify");
  return OMXMLStoredObject::createModify(rawStorage, byteOrder);
}

  // @mfunc Open the root <c OMXMLStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMXMLStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLStoredObjectFactory::openRead(const wchar_t* /* fileName */)
{
  TRACE("OMXMLStoredObjectFactory::openRead");
  ASSERT("Unimplemented code not reached", false);
//return OMXMLStoredObject::openRead(fileName);
  return 0;
}

  // @mfunc Open the root <c OMXMLStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMXMLStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLStoredObjectFactory::openModify(const wchar_t* /* fileName */)
{
  TRACE("OMXMLStoredObjectFactory::openModify");
  ASSERT("Unimplemented code not reached", false);
//return OMXMLStoredObject::openModify(fileName);
  return 0;
}

  // @mfunc Create a new root <c OMXMLStoredObject> in the disk file
  //        <p fileName>. The byte order of the newly created root
  //        is given by <p byteOrder>.
  //   @parm The name of the file to create. The file must not exist.
  //   @parm The desired byte ordering for the new <c OMXMLStoredObject>.
  //   @rdesc An <c OMXMLStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMXMLStoredObjectFactory::createModify(const wchar_t* /* fileName */,
                                       const OMByteOrder /* byteOrder */)
{
  TRACE("OMXMLStoredObjectFactory::creatModify");
  ASSERT("Unimplemented code not reached", false);
//return OMXMLStoredObject::createModify(fileName, byteOrder);
  return 0;
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
//return OMXMLStoredObject::createWrite(fileName, byteOrder);
  return 0;
}

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true.
  //   @parm The name of the file.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMXMLStoredObjectFactory::isRecognized(const wchar_t* /* fileName */)
{
  TRACE("OMXMLStoredObjectFactory::isRecognized");
//ASSERT("Unimplemented code not reached", false);
  return false;
}

  // @mfunc Does <p rawStorage> contain a recognized file ?
  //        If so, the result is true.
  //   @parm The raw storage.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMXMLStoredObjectFactory::isRecognized(OMRawStorage* /* rawStorage */)
{
  TRACE("OMXMLStoredObjectFactory::isRecognized");
//ASSERT("Unimplemented code not reached", false);
  return false;
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

  // tjb -- missing checks ?
  bool result = true;
  return result;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMXMLStoredObjectFactory::compatibleNamedFile(
                                   const OMFile::OMAccessMode /* accessMode */)
{
  TRACE("OMXMLStoredObjectFactory::compatibleNamedFile");

  // Directly accessed named files are not supported regardless of the
  // access mode. Named files are supported via a disk file based
  // implementation of OMRawStorage.
  bool result = false;
  return result;
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p fileName> is closed.
  //   @parm The file name.
void OMXMLStoredObjectFactory::close(const wchar_t* /* fileName */)
{
  TRACE("OMXMLStoredObjectFactory::close");
  // Nothing to do.
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p rawStorage> is closed.
  //   @parm The <c OMRawStorage>
void OMXMLStoredObjectFactory::close(OMRawStorage* /* rawStorage */)
{
  TRACE("OMXMLStoredObjectFactory::close");
  // Nothing to do.
}

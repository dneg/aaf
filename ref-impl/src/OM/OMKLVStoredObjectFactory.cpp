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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
#include "OMKLVStoredObjectFactory.h"

#include "OMKLVStoredObject.h"
#include "OMUtilities.h"
#include "OMAssertions.h"

  // @mfunc Constructor.
OMKLVStoredObjectFactory::OMKLVStoredObjectFactory(
                                        const OMStoredObjectEncoding& encoding,
                                        const wchar_t* name,
                                        const wchar_t* description)
: OMStoredObjectFactory(encoding, name, description)
{
  TRACE("OMKLVStoredObjectFactory::OMKLVStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
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
  return OMKLVStoredObject::openRead(rawStorage);
}

  // @mfunc Open the root <c OMKLVStoredObject> in the raw storage
  //        <p rawStorage> for modification.
  //   @parm The raw storage in which to open the <c OMKLVStoredObject>.
  //   @rdesc An <c OMKLVStoredObject> representing the root object.
OMStoredObject*
OMKLVStoredObjectFactory::openModify(OMRawStorage* rawStorage)
{
  TRACE("OMKLVStoredObjectFactory::openModify");
  return OMKLVStoredObject::openModify(rawStorage);
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
  return OMKLVStoredObject::createWrite(rawStorage, byteOrder);
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
  return OMKLVStoredObject::createModify(rawStorage, byteOrder);
}

  // @mfunc Open the root <c OMKLVStoredObject> in the disk file
  //        <p fileName> for reading only.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMKLVStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMKLVStoredObjectFactory::openRead(const wchar_t* /* fileName */)
{
  TRACE("OMKLVStoredObjectFactory::openRead");
  ASSERT("Unimplemented code not reached", false);
//return OMKLVStoredObject::openRead(fileName);
  return 0;
}

  // @mfunc Open the root <c OMKLVStoredObject> in the disk file
  //        <p fileName> for modification.
  //   @parm The name of the file to open. The file must already exist.
  //   @rdesc An <c OMKLVStoredObject> representing the root object in
  //          the disk file.
OMStoredObject*
OMKLVStoredObjectFactory::openModify(const wchar_t* /* fileName */)
{
  TRACE("OMKLVStoredObjectFactory::openModify");
  ASSERT("Unimplemented code not reached", false);
//return OMKLVStoredObject::openModify(fileName);
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
OMKLVStoredObjectFactory::createModify(const wchar_t* /* fileName */,
                                       const OMByteOrder /* byteOrder */)
{
  TRACE("OMKLVStoredObjectFactory::creatModify");
  ASSERT("Unimplemented code not reached", false);
//return OMKLVStoredObject::createModify(fileName, byteOrder);
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
OMKLVStoredObjectFactory::createWrite(const wchar_t* /* fileName */,
                                      const OMByteOrder /* byteOrder */)
{
  TRACE("OMKLVStoredObjectFactory::creatWrite");
  ASSERT("Unimplemented code not reached", false);
//return OMKLVStoredObject::createWrite(fileName, byteOrder);
  return 0;
}

  // @mfunc Is the file named <p fileName> a recognized file ?
  //        If so, the result is true.
  //   @parm The name of the file.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMKLVStoredObjectFactory::isRecognized(const wchar_t* /* fileName */)
{
  TRACE("OMKLVStoredObjectFactory::isRecognized");
//ASSERT("Unimplemented code not reached", false);
  return false;
}

  // @mfunc Does <p rawStorage> contain a recognized file ?
  //        If so, the result is true.
  //   @parm The raw storage.
  //   @rdesc True if the file is recognized, false otherwise.
bool
OMKLVStoredObjectFactory::isRecognized(OMRawStorage* /* rawStorage */)
{
  TRACE("OMKLVStoredObjectFactory::isRecognized");
//ASSERT("Unimplemented code not reached", false);
  return false;
}

  // @mfunc Can a file be created successfully on the given
  //        <c OMRawStorage> and accessed successfully in the mode
  //        specified by <p accessMode> ?
  //   @parm The <c OMRawStorage>.
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMKLVStoredObjectFactory::compatibleRawStorage(
                                   const OMRawStorage* /* rawStorage */,
                                   const OMFile::OMAccessMode /* accessMode */)
{
  TRACE("OMKLVStoredObjectFactory::compatibleRawStorage");
  bool result = false;
  ASSERT("Unimplemented code not reached", false);
  return result;
}

  // @mfunc Can a file be created successfully as a named file and
  //        accessed successfully in the mode specified by <p accessMode> ?
  //   @parm The <t OMAccessMode>.
  //   @rdesc True if the file can be created, false otherwise.
bool OMKLVStoredObjectFactory::compatibleNamedFile(
                                   const OMFile::OMAccessMode /* accessMode */)
{
  TRACE("OMKLVStoredObjectFactory::compatibleNamedFile");
  bool result = false;
  ASSERT("Unimplemented code not reached", false);
  return result;
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p fileName> is closed.
  //   @parm The file name.
void OMKLVStoredObjectFactory::close(const wchar_t* /* fileName */,
                                     bool /* isWritable */)
{
  TRACE("OMKLVStoredObjectFactory::close");
  ASSERT("Unimplemented code not reached", false);
}

  // @mfunc Perform any necessary actions when the file
  //        contained in <p rawStorage> is closed.
  //   @parm The <c OMRawStorage>
void OMKLVStoredObjectFactory::close(OMRawStorage* /* rawStorage */,
                                     bool /* isWritable */)
{
  TRACE("OMKLVStoredObjectFactory::close");
  ASSERT("Unimplemented code not reached", false);
}

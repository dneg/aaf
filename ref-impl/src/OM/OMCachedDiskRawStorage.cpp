//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMCachedDiskRawStorage
#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMUtilities.h"
#include "OMExceptions.h"
#include "OMDiskRawStorage.h"

#include "OMCachedDiskRawStorage.h"

  // @mfunc Constructor.
OMBaseCachedDiskRawStorage::OMBaseCachedDiskRawStorage(OMUInt32 pageSize,
                                                       OMUInt32 pageCount,
                                                       OMUInt64 size)
: OMCachedRawStorage(pageSize, pageCount, size),
  _validSize(size)
{
  TRACE("OMCachedDiskRawStorage::OMCachedDiskRawStorage");
}

  // @mfunc Constructor.
OMBaseCachedDiskRawStorage::OMBaseCachedDiskRawStorage(
                                               OMUInt32 pageSize,
                                               OMUInt32 pageCount,
                                               OMUInt64 size,
                                               OMCachePageAllocator* allocator)
: OMCachedRawStorage(pageSize, pageCount, size, allocator),
  _validSize(size)
{
  TRACE("OMCachedDiskRawStorage::OMCachedDiskRawStorage");
}

  // @mfunc Destructor.
OMBaseCachedDiskRawStorage::~OMBaseCachedDiskRawStorage(void)
{
  TRACE("OMBaseCachedDiskRawStorage::~OMBaseCachedDiskRawStorage");
}

  // @mfunc Read a page or partial page without using the cache.
  //   @parm TBS
  //   @parm TBS
  //   @parm TBS
void OMBaseCachedDiskRawStorage::readPage(OMUInt64 position,
                                          OMUInt32 byteCount,
                                          OMByte* destination)
{
  TRACE("OMBaseCachedDiskRawStorage::readPage");
  PRECONDITION("Valid destination", destination != 0);

  OMUInt64 streamSize = _validSize;
  if (position < streamSize) {
    OMUInt64 remaining = streamSize - position;
    OMUInt32 readSize;
    if (remaining < byteCount) {
      readSize = static_cast<OMUInt32>(remaining);
    } else {
      readSize = byteCount;
    }
    ASSERT("Valid read size", readSize != 0);
    rawReadAt(position, readSize, destination);
  }
}

  // @mfunc Write a page or partial page without using the cache.
  //   @parm TBS
  //   @parm TBS
  //   @parm TBS
void OMBaseCachedDiskRawStorage::writePage(OMUInt64 position,
                                           OMUInt32 byteCount,
                                           const OMByte* source)
{
  TRACE("OMBaseCachedDiskRawStorage::writePage");
  PRECONDITION("Valid source", source != 0);
  PRECONDITION("Stream is writable", isWritable());

  OMUInt64 streamSize = extent();
  OMUInt64 remaining = streamSize - position;
  OMUInt32 writeSize;
  if (remaining < byteCount) {
    writeSize = static_cast<OMUInt32>(remaining);
  } else {
    writeSize = byteCount;
  }
  ASSERT("Valid write size", writeSize != 0);
  rawWriteAt(position, writeSize, source);
  OMUInt64 newValidSize = position + writeSize;
  if (newValidSize > _validSize) {
    _validSize = newValidSize;
  }
}

  // @mfunc Create an <c OMCachedDiskRawStorage> object by opening an existing
  //        file for read-only access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @parm TBS.
  //   @parm TBS.
  //   @rdesc The newly created <c OMCachedDiskRawStorage> object.
OMCachedDiskRawStorage*
OMCachedDiskRawStorage::openExistingRead(const wchar_t* fileName,
                                         OMUInt32 pageSize,
                                         OMUInt32 pageCount)
{
  TRACE("OMCachedDiskRawStorage::openExistingRead");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openExistingRead(fileName);

  OMCachedDiskRawStorage* result = new OMCachedDiskRawStorage(
                                                          file,
                                                          OMFile::readOnlyMode,
                                                          pageSize,
                                                          pageCount);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMCachedDiskRawStorage> object by opening an existing
  //        file for modify access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @parm TBS.
  //   @parm TBS.
  //   @rdesc The newly created <c OMCachedDiskRawStorage> object.
OMCachedDiskRawStorage*
OMCachedDiskRawStorage::openExistingModify(const wchar_t* fileName,
                                           OMUInt32 pageSize,
                                           OMUInt32 pageCount)
{
  TRACE("OMCachedDiskRawStorage::openExistingModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openExistingModify(fileName);

  OMCachedDiskRawStorage* result = new OMCachedDiskRawStorage(
                                                            file,
                                                            OMFile::modifyMode,
                                                            pageSize,
                                                            pageCount);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMCachedDiskRawStorage> object by creating a new
  //        file for modify access, the file is named <p fileName>.
  //        The file must not already exist.
  //   @parm The file name.
  //   @parm TBS.
  //   @parm TBS.
  //   @rdesc The newly created <c OMCachedDiskRawStorage> object.
OMCachedDiskRawStorage*
OMCachedDiskRawStorage::openNewModify(const wchar_t* fileName,
                                      OMUInt32 pageSize,
                                      OMUInt32 pageCount)
{
  TRACE("OMCachedDiskRawStorage::openNewModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  OMStream* file = OMStream::openNewModify(fileName);

  OMCachedDiskRawStorage* result = new OMCachedDiskRawStorage(
                                                            file,
                                                            OMFile::modifyMode,
                                                            pageSize,
                                                            pageCount);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMCachedDiskRawStorage> object by creating a new
  //        temporary file for modify access.
  //   @parm TBS.
  //   @parm TBS.
  //   @rdesc The newly created <c OMCachedDiskRawStorage> object.
OMCachedDiskRawStorage*
OMCachedDiskRawStorage::openNewModify(OMUInt32 pageSize,
                                      OMUInt32 pageCount)
{
  TRACE("OMCachedDiskRawStorage::openNewModify");

  OMStream* file = OMStream::openNewModify();

  OMCachedDiskRawStorage* result = new OMCachedDiskRawStorage(
                                                            file,
                                                            OMFile::modifyMode,
                                                            pageSize,
                                                            pageCount);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Destructor.
OMCachedDiskRawStorage::~OMCachedDiskRawStorage(void)
{
  TRACE("OMCachedDiskRawStorage::~OMCachedDiskRawStorage");

  synchronize();
  delete _file;
  _file = 0;
}

  // @mfunc Is it possible to read from this <c OMCachedDiskRawStorage> ?
bool OMCachedDiskRawStorage::isReadable(void) const
{
  TRACE("OMCachedDiskRawStorage::isReadable");
  bool result;

  if ((_mode == OMFile::modifyMode) || (_mode == OMFile::readOnlyMode)) {
    result = true;
  } else {
    result = false;
}
  return result;
}

  // @mfunc Is it possible to write to this <c OMCachedDiskRawStorage> ?
bool OMCachedDiskRawStorage::isWritable(void) const
{
  TRACE("OMCachedDiskRawStorage::isWritable");

  bool result;
  if ((_mode == OMFile::modifyMode) || (_mode == OMFile::writeOnlyMode)) {
    result = true;
  } else {
    result = false;
}
  return result;
}

  // @mfunc May this <c OMCachedDiskRawStorage> be changed in size ?
bool OMCachedDiskRawStorage::isExtendible(void) const
{
  TRACE("OMCachedDiskRawStorage::isExtendible");

  return true;
}

  // @mfunc May the current position, for <f read()> and <f write()>,
  //        of this <c OMCachedDiskRawStorage> be changed ?
bool OMCachedDiskRawStorage::isPositionable(void) const
{
  TRACE("OMCachedDiskRawStorage::isPositionable");

  return true;
}

  // @mfunc Synchronize this <c OMCachedDiskRawStorage> with its external
  //        representation.
void OMCachedDiskRawStorage::synchronize(void)
{
  TRACE("OMCachedDiskRawStorage::synchronize");

  if (isWritable()) {
    flush();
    _file->synchronize();
  }
}

  // @mfunc Read bytes without using the cache.
  //   @parm TBS
  //   @parm TBS
  //   @parm TBS
void OMCachedDiskRawStorage::rawReadAt(OMUInt64 position,
                                       OMUInt32 byteCount,
                                       OMByte* destination)
{
  TRACE("OMCachedDiskRawStorage::rawReadAt");
  PRECONDITION("Valid destination", destination != 0);

  _file->setPosition(position);
  OMUInt32 br;
  _file->read(destination, byteCount, br);
  ASSERT("All bytes read", br == byteCount);
}

  // @mfunc Write bytes without using the cache.
  //   @parm TBS
  //   @parm TBS
  //   @parm TBS
void OMCachedDiskRawStorage::rawWriteAt(OMUInt64 position,
                                        OMUInt32 byteCount,
                                        const OMByte* source)
{
  TRACE("OMCachedDiskRawStorage::rawWriteAt");
  PRECONDITION("Valid source", source != 0);
  PRECONDITION("Stream is writable", isWritable());

  _file->setPosition(position);
  OMUInt32 bw;
  _file->write(source, byteCount, bw);
  ASSERT("All bytes written", bw == byteCount);
}

  // @mfunc Constructor.
OMCachedDiskRawStorage::OMCachedDiskRawStorage(OMStream* file,
                                               OMFile::OMAccessMode accessMode,
                                               OMUInt32 pageSize,
                                               OMUInt32 pageCount)
: OMBaseCachedDiskRawStorage(pageSize, pageCount, file->size()),
  _file(file),
  _mode(accessMode)
{
  TRACE("OMCachedDiskRawStorage::OMCachedDiskRawStorage");
  PRECONDITION("Valid file", _file != 0);
}

/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

// @doc OMINTERNAL
#include "OMRawStorage.h"
#include "OMAssertions.h"
#include "OMUtilities.h"

#include <limits.h>
#include <errno.h>

#include "OMDiskRawStorage.h"

  // @mfunc Create an <c OMDiskRawStorage> object by opening an existing
  //        file for read-only access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMDiskRawStorage> object.
OMDiskRawStorage*
OMDiskRawStorage::openExistingRead(const wchar_t* fileName)
{
  TRACE("OMDiskRawStorage::openExistingRead");

  PRECONDITION("Valid file name", validWideString(fileName));

  FILE* file = wfopen(fileName, L"rb");
  ASSERT("File successfully opened", file != 0); // tjb - error

  OMDiskRawStorage* result = new OMDiskRawStorage(file, OMFile::readOnlyMode);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMDiskRawStorage> object by opening an existing
  //        file for modify access, the file is named <p fileName>.
  //        The file must already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMDiskRawStorage> object.
OMDiskRawStorage*
OMDiskRawStorage::openExistingModify(const wchar_t* fileName)
{
  TRACE("OMDiskRawStorage::openExistingModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  FILE* file = wfopen(fileName, L"r+b");
  ASSERT("File successfully opened", file != 0); // tjb - error

  OMDiskRawStorage* result = new OMDiskRawStorage(file, OMFile::modifyMode);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Create an <c OMDiskRawStorage> object by creating a new
  //        file for modify access, the file is named <p fileName>.
  //        The file must note already exist.
  //   @parm The file name.
  //   @rdesc The newly created <c OMDiskRawStorage> object.
OMDiskRawStorage*
OMDiskRawStorage::openNewModify(const wchar_t* fileName)
{
  TRACE("OMDiskRawStorage::openNewModify");

  PRECONDITION("Valid file name", validWideString(fileName));

  FILE* file = wfopen(fileName, L"wb");
  ASSERT("File successfully opened", file != 0); // tjb - error

  OMDiskRawStorage* result = new OMDiskRawStorage(file, OMFile::modifyMode);
  ASSERT("Valid heap pointer", result != 0);

  return result;
}

  // @mfunc Constructor.
  //   @parm The file.
  //   @parm The access mode.
OMDiskRawStorage::OMDiskRawStorage(FILE* file,
                                   OMFile::OMAccessMode accessMode)
: _file(file),
  _mode(accessMode)
{
  TRACE("OMDiskRawStorage::OMDiskRawStorage");

  PRECONDITION("Valid file", _file != 0);
  PRECONDITION("Valid mode", (_mode == OMFile::readOnlyMode)  ||
                             (_mode == OMFile::writeOnlyMode) ||
                             (_mode == OMFile::modifyMode));
}

  // @mfunc Destructor.
OMDiskRawStorage::~OMDiskRawStorage(void)
{
  TRACE("OMDiskRawStorage::~OMDiskRawStorage");

  PRECONDITION("Valid file", _file != 0);
  fclose(_file);
  _file = 0;
}

  // @mfunc Attempt to read the number of bytes given by <p byteCount>
  //        from this <c OMDiskRawStorage> at <p offset> into the buffer
  //        at address <p bytes>. The actual number of bytes read is
  //        returned in <p bytesRead>. Reading from offsets greater than
  //        <mf OMRawStorage::size> causes <p bytesRead> to be less than
  //        <p byteCount>. Reading bytes that have never been written
  //        returns undefined data in <p bytes>.
  //   @parm The offset from which to read.
  //   @parm The buffer into which the bytes are to be read.
  //   @parm The number of bytes to read.
  //   @parm The number of bytes actually read.
  //   @this const
void OMDiskRawStorage::readAt(OMUInt64 offset,
                              OMByte* bytes,
                              OMUInt32 byteCount,
                              OMUInt32& bytesRead) const
{
  TRACE("OMDiskRawStorage::readAt");

  PRECONDITION("Valid mode", (_mode == OMFile::modifyMode) ||
                             (_mode == OMFile::readOnlyMode));

  setPosition(offset);
  size_t actualByteCount = fread(bytes, 1, byteCount, _file);

  bytesRead = actualByteCount;
}

  // @mfunc Attempt to write the number of bytes given by <p byteCount>
  //        to this <c OMDiskRawStorage> at <p offset> from the buffer
  //        at address <p bytes>. The actual number of bytes written is
  //        returned in <p bytesWritten>. Writing to offsets greater than
  //        <mf OMRawStorage::size> causes this <c OMDiskRawStorage>
  //        to be extended, however such extension can fail, causing
  //        <p bytesWritten> to be less than <p byteCount>.
  //   @parm The offset at which to write.
  //   @parm The buffer from which the bytes are to be written.
  //   @parm The number of bytes to write.
  //   @parm The actual number of bytes written.
void OMDiskRawStorage::writeAt(OMUInt64 offset,
                               const OMByte* bytes,
                               OMUInt32 byteCount,
                               OMUInt32& bytesWritten)
{
  TRACE("OMDiskRawStorage::writeAt");

  PRECONDITION("Valid mode", (_mode == OMFile::modifyMode) ||
                             (_mode == OMFile::writeOnlyMode));

  setPosition(offset);
  size_t actualByteCount = fwrite(bytes, 1, byteCount, _file);

  bytesWritten = actualByteCount;
}

  // @mfunc The current size of this <c OMDiskRawStorage> in bytes.
  //   @rdesc The current size of this <c OMDiskRawSrorage> in bytes.
  //   @this const
OMUInt64 OMDiskRawStorage::size(void) const
{
  TRACE("OMDiskRawStorage::size");

  long int seekStatus = fseek(_file, 0, SEEK_END);
  ASSERT("Successful seek", seekStatus == 0); // tjb - error

  errno = 0;
  long int position = ftell(_file);
  ASSERT("Successful tell", IMPLIES(position == -1L, errno == 0));

  OMUInt64 result = position;
  return result;
}

  // @mfunc Set the size of this <c OMDiskRawStorage> to <p newSize> bytes.
  //        If <p newSize> is greater than <mf OMRawStorage::size>
  //        then this <c OMDiskRawStorage> is extended. If <p newSize>
  //        is less than <mf OMRawStorage::size> then this
  //        <c OMDiskRawStorage> is truncated.
  //   @parm The new size of this <c OMDiskRawSrorage> in bytes.
  //   @devnote There is no ANSI way of truncating a file in place.
void OMDiskRawStorage::setSize(OMUInt64 newSize)
{
  TRACE("OMDiskRawStorage::setSize");

  PRECONDITION("Valid mode", (_mode == OMFile::modifyMode) ||
                             (_mode == OMFile::writeOnlyMode));

  OMUInt64 currentSize = size();

  if (newSize > currentSize) {

#if 1
    // Extend by writing a single byte.
	//
    OMByte nullByte = 0;
    OMUInt32 bytesWritten = 0;

    writeAt(newSize - 1, &nullByte, 1, bytesWritten);
    ASSERT("Successful write", bytesWritten == 1);
#else
    // Harbison and Steele (3rd Ed. page 305) claim that
    // fseek(_file, positive, SEEK_END) extends the file,
    // but this is not true in a least one implementation
    // (Microsoft VC++ 6.0).
    OMUInt64 additionalBytes = newSize - currentSize;

    ASSERT("Supported offset", additionalBytes <= LONG_MAX); // tjb - limit
    long int liAdditionalBytes = static_cast<long int>(additionalBytes);

    int seekStatus = fseek(_file, liAdditionalBytes, SEEK_END);
    ASSERT("Successful seek", seekStatus == 0); // tjb - error
#endif

    ASSERT("Size properly changed", size() == newSize);
  }
  // else no ANSI way to truncate the file in place
}

  // @mfunc Set the position for subsequent reads and writes
  //        to <p newPosition>.
  //   @parm The new position.
  //   @devnote fseek takes a long int for offset this may not be sufficient
  //            for 64-bit offsets.
void OMDiskRawStorage::setPosition(OMUInt64 newPosition) const
{
  TRACE("OMDiskRawStorage::setPosition");

  ASSERT("Supported position", newPosition <= LONG_MAX); // tjb - limit
  long int liNewPosition = static_cast<long int>(newPosition);

  int seekStatus = fseek(_file, liNewPosition, SEEK_SET);
  ASSERT("Successful seek", seekStatus == 0); // tjb - error
}

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
#ifndef OMDISKRAWSTORAGE_H
#define OMDISKRAWSTORAGE_H

#include "OMRawStorage.h"
#include "OMFile.h"

#include <stdio.h>

  // @class Class supporting access to the raw bytes of disk
  //        files supported by the Object Manager.
  //
  //        This is an Object Manager built-in implementation of the
  //        <c OMRawStorage> interface. The implementation uses
  //        ANSI file functions only.
  //
  //   @base public | OMRawStorage
class OMDiskRawStorage : public OMRawStorage {
public:
  // @access Static members.

    // @cmember Create an <c OMDiskRawStorage> object by opening an existing
    //          file for read-only access, the file is named <p fileName>.
    //          The file must already exist.
  static OMDiskRawStorage* openExisitingRead(const wchar_t* fileName);

    // @cmember Create an <c OMDiskRawStorage> object by opening an existing
    //          file for modify access, the file is named <p fileName>.
    //          The file must already exist.
  static OMDiskRawStorage* openExisitingModify(const wchar_t* fileName);

    // @cmember Create an <c OMDiskRawStorage> object by creating a new
    //          file for modify access, the file is named <p fileName>.
    //          The file must note already exist.
  static OMDiskRawStorage* openNewModify(const wchar_t* fileName);

  // @access Public members.

    // @cmember Constructor.
  OMDiskRawStorage(FILE* file, OMFile::OMAccessMode accessMode);

    // @cmember Destructor.
  virtual ~OMDiskRawStorage(void);

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from this <c OMDiskRawStorage> at <p offset> into the buffer
    //          at address <p bytes>. The actual number of bytes read is
    //          returned in <p bytesRead>. Reading from offsets greater than
    //          <mf OMRawStorage::size> causes <p bytesRead> to be less than
    //          <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
  virtual void readAt(OMUInt64 offset,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to this <c OMDiskRawStorage> at <p offset> from the buffer
    //          at address <p bytes>. The actual number of bytes written is
    //          returned in <p bytesWritten>. Writing to offsets greater than
    //          <mf OMRawStorage::size> causes this <c OMDiskRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
  virtual void writeAt(OMUInt64 offset,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

    // @cmember The current size of this <c OMDiskRawStorage> in bytes.
  virtual OMUInt64 size(void) const;

    // @cmember Set the size of this <c OMDiskRawStorage> to <p newSize> bytes.
    //          If <p newSize> is greater than <mf OMRawStorage::size>
    //          then this <c OMDiskRawStorage> is extended. If <p newSize>
    //          is less than <mf OMRawStorage::size> then this
    //          <c OMDiskRawStorage> is truncated.
  virtual void setSize(OMUInt64 newSize);

private:
  // @access Private members.

  FILE* _file;
  OMFile::OMAccessMode _mode;

};

#endif

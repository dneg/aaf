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
#ifndef OMMAPPEDFILERAWSTORAGE_H
#define OMMAPPEDFILERAWSTORAGE_H

#include "OMFile.h"

#include "OMRawStorage.h"

#include <stdio.h>

  // @class Class supporting access to the raw bytes of memory mapped disk
  //        files supported by the Object Manager.
  //
  //        This is an Object Manager built-in implementation of the
  //        <c OMRawStorage> interface.
  //
  //   @base public | <c OMRawStorage>
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMMappedFileRawStorage : public OMRawStorage {
public:
  // @access Static members.

    // @cmember Create an <c OMMappedFileRawStorage> object by opening
    //          an existing file for read-only access, the file is named
    //          <p fileName>.
    //          The file must already exist.
  static OMMappedFileRawStorage* openExistingRead(const wchar_t* fileName);

    // @cmember Create an <c OMMappedFileRawStorage> object by opening
    //          an existing file for modify access, the file is named
    //          <p fileName>.
    //          The file must already exist.
  static OMMappedFileRawStorage* openExistingModify(const wchar_t* fileName);

    // @cmember Create an <c OMMappedFileRawStorage> object by creating a new
    //          file for modify access, the file is named <p fileName>.
    //          The file must not already exist.
  static OMMappedFileRawStorage* openNewModify(const wchar_t* fileName);

  // @access Public members.

    // @cmember Destructor.
  virtual ~OMMappedFileRawStorage(void);

    // @cmember Is it possible to read from this <c OMMappedFileRawStorage> ?
  virtual bool isReadable(void) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from the current position in this <c OMMappedFileRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMMappedFileRawStorage::size> causes <p bytesRead>
    //          to be less than <p byteCount>.
    //          Reading bytes that have never been written returns undefined
    //          data in <p bytes>.
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from offset <p position> in this <c OMMappedFileRawStorage>
    //          into the buffer at address <p bytes>.
    //          The actual number of bytes read is returned in <p bytesRead>.
    //          Reading from positions greater than
    //          <mf OMMappedFileRawStorage::size> causes <p bytesRead>
    //          to be less than <p byteCount>.
    //          Reading bytes that have never been written returns undefined
    //          data in <p bytes>.
    //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 possition,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

    // @cmember Is it possible to write to this <c OMMappedFileRawStorage> ?
  virtual bool isWritable(void) const;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to the current position in this <c OMMappedFileRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMMappedFileRawStorage::size> causes this
    //          <c OMMappedFileRawStorage> to be extended, however such
    //          extension can fail, causing <p bytesWritten> to be less than
    //          <p byteCount>.
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

    // @cmember May this <c OMMappedFileRawStorage> be changed in size ?
  virtual bool isSizeable(void) const;

    // @cmember The current size of this <c OMMappedFileRawStorage> in bytes.
    //          precondition - isSizeable()
  virtual OMUInt64 size(void) const;

    // @cmember Set the size of this <c OMMappedFileRawStorage> to <p newSize>
    //          bytes. If <p newSize> is greater than
    //          <mf OMMappedFileRawStorage::size>
    //          then this <c OMMappedFileRawStorage> is extended.
    //          If <p newSize> is less than <mf OMMappedFileRawStorage::size>
    //          then this <c OMMappedFileRawStorage> is truncated.
    //          Truncation may also result in the current position for
    //          <f read()> and <f write()> being set to
    //          <mf OMMappedFileRawStorage::size>.
    //          precondition - isSizeable()
  virtual void setSize(OMUInt64 newSize);

    // @cmember May the current position, for <f read()> and <f write()>,
    //          of this <c OMMappedFileRawStorage> be changed ?
  virtual bool isPositionable(void) const;

    // @cmember The current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMMappedFileRawStorage>.
    //          precondition - isPositionable()
  virtual OMUInt64 position(void) const;

    // @cmember Set the current position for <f read()> and <f write()>, as an
    //          offset in bytes from the beginning of this
    //          <c OMMappedFileRawStorage>.
    //          precondition - isPositionable()
  virtual void setPosition(OMUInt64 newPosition);

    // @cmember Synchronize this <c OMMappedFileRawStorage> with its external
    //          representation.
  virtual void synchronize(void);

private:
  // @access Private members.

    // @cmember none | OMMappedFileRawStorage | accessMode | Constructor.
  OMMappedFileRawStorage(/* TBS */ OMFile::OMAccessMode accessMode);

};

#endif

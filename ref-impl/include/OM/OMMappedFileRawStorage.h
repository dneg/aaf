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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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
  virtual void readAt(OMUInt64 position,
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

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to offset <p position> in this <c OMMappedFileRawStorage>
    //          from the buffer at address <p bytes>.
    //          The actual number of bytes written is returned in
    //          <p bytesWritten>.
    //          Writing to positions greater than
    //          <mf OMMappedFileRawStorage::size> causes this
    //          <c OMMappedFileRawStorage> to be extended, however such
    //          extension can fail, causing <p bytesWritten> to be less than
    //          <p byteCount>.
    //          @precondition <f isWritable()> && <f isPositionable()>
    //   @devnote How is failure to extend indicated ?
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

    // @cmember May this <c OMMappedFileRawStorage> be changed in size ?
  virtual bool isExtendible(void) const;

    // @cmember The current extent of this <c OMMappedFileRawStorage> in bytes.
    //          The <f extent()> is the allocated size, while the <f size()>
    //          is the valid size.
    //          precondition - isPositionable()
  virtual OMUInt64 extent(void) const;

    // @cmember Set the size of this <c OMMappedFileRawStorage> to <p newSize>
    //          bytes. If <p newSize> is greater than
    //          <mf OMMappedFileRawStorage::size>
    //          then this <c OMMappedFileRawStorage> is extended.
    //          If <p newSize> is less than <mf OMMappedFileRawStorage::size>
    //          then this <c OMMappedFileRawStorage> is truncated.
    //          Truncation may also result in the current position for
    //          <f read()> and <f write()> being set to
    //          <mf OMMappedFileRawStorage::size>.
    //          precondition - isExtendible()
  virtual void extend(OMUInt64 newSize);

    // @cmember The current size of this <c OMMappedFileRawStorage> in bytes.
    //          The <f size()> is the valid size, while the <f extent()>
    //          is the allocated size.
    //          precondition - isPositionable()
  virtual OMUInt64 size(void) const;

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
  virtual void setPosition(OMUInt64 newPosition) const;

    // @cmember Synchronize this <c OMMappedFileRawStorage> with its external
    //          representation.
  virtual void synchronize(void);

private:
  // @access Private members.

    // @cmember none | OMMappedFileRawStorage | accessMode | Constructor.
  OMMappedFileRawStorage(/* TBS */ OMFile::OMAccessMode accessMode);

};

#endif

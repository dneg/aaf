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
#ifndef OMRAWSTORAGE_H
#define OMRAWSTORAGE_H

#include "OMDataTypes.h"

  // @class Abstract base class supporting access to the raw bytes of
  //        files supported by the Object Manager.
  //
  //        Object Manager clients implement this interface to allow files
  //        to be stored in locations not known to the Object Manager.
  //
  //        For example, Object Manager clients may wish to store files in
  //        a proprietary file system, this could be done by providing an
  //        implementation of this interface for that file system.
  //
  //        Additionally a number of built-in implementations of this
  //        interface are provided -
  //
  //          <c OMDiskRawStorage>   - an implementation of <c OMRawStorage>
  //                                   for disk files. Uses ANSI file
  //                                   functions only.
  //
  //          <c OMMemoryRawStorage> - an implementation of <c OMRawStorage>
  //                                   that stores the file in memory.
  //
class OMRawStorage {
public:
  // @access Public members.

    // @cmember Destructor.
  virtual ~OMRawStorage(void) {}

    // @cmember Attempt to read the number of bytes given by <p byteCount>
    //          from this <c OMRawStorage> at <p offset> into the buffer
    //          at address <p bytes>. The actual number of bytes read is
    //          returned in <p bytesRead>. Reading from offsets greater than
    //          <mf OMRawStorage::size> causes <p bytesRead> to be less than
    //          <p byteCount>. Reading bytes that have never been written
    //          returns undefined data in <p bytes>.
  virtual void readAt(OMUInt64 offset,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const = 0;

    // @cmember Attempt to write the number of bytes given by <p byteCount>
    //          to this <c OMRawStorage> at <p offset> from the buffer
    //          at address <p bytes>. The actual number of bytes written is
    //          returned in <p bytesWritten>. Writing to offsets greater than
    //          <mf OMRawStorage::size> causes this <c OMRawStorage>
    //          to be extended, however such extension can fail, causing
    //          <p bytesWritten> to be less than <p byteCount>.
  virtual void writeAt(OMUInt64 offset,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten) = 0;

    // @cmember The current size of this <c OMRawStorage> in bytes.
    //   @devnote This method does not make sense for <c OMRawStorage>
    //            implemented on a stream
  virtual OMUInt64 size(void) const = 0;

    // @cmember Set the size of this <c OMRawStorage> to <p newSize> bytes.
    //          If <p newSize> is greater than <mf OMRawStorage::size>
    //          then this <c OMRawStorage> is extended. If <p newSize>
    //          is less than <mf OMRawStorage::size> then this
    //          <c OMRawStorage> is truncated.
    //   @devnote This method does not make sense for <c OMRawStorage>
    //            implemented on a stream.
    //   @devnote How is failure to extend indicated ?
  virtual void setSize(OMUInt64 newSize) = 0;

};

#endif

#ifndef __ImplAAFOMRawStorage_h__
#define __ImplAAFOMRawStorage_h__

/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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

#include "OMRawStorage.h"

struct IAAFRawStorage;
struct IAAFRandomRawStorage;

class ImplAAFOMRawStorage : public OMRawStorage
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFOMRawStorage (IAAFRawStorage * rep);

  virtual ~ImplAAFOMRawStorage ();

public:

  // Implementations of OMRawStorage pure virtual methods.

  // @cmember Is it possible to read from this <c OMRawStorage> ?
  virtual bool isReadable(void) const;

  // @cmember Attempt to read the number of bytes given by <p byteCount>
  //          from the current position in this <c OMRawStorage>
  //          into the buffer at address <p bytes>.
  //          The actual number of bytes read is returned in <p bytesRead>.
  //          Reading from positions greater than
  //          <mf OMRawStorage::size> causes <p bytesRead> to be less
  //          than <p byteCount>. Reading bytes that have never been written
  //          returns undefined data in <p bytes>.
  //          @precondition <f isReadable()>
  virtual void read(OMByte* bytes,
                    OMUInt32 byteCount,
                    OMUInt32& bytesRead) const;

  // @cmember Attempt to read the number of bytes given by <p byteCount>
  //          from offset <p position> in this <c OMRawStorage>
  //          into the buffer at address <p bytes>.
  //          The actual number of bytes read is returned in <p bytesRead>.
  //          Reading from positions greater than
  //          <mf OMRawStorage::size> causes <p bytesRead> to be less
  //          than <p byteCount>. Reading bytes that have never been written
  //          returns undefined data in <p bytes>.
  //          @precondition <f isReadable()> && <f isPositionable()>
  virtual void readAt(OMUInt64 position,
                      OMByte* bytes,
                      OMUInt32 byteCount,
                      OMUInt32& bytesRead) const;

  // @cmember Is it possible to write to this <c OMRawStorage> ?
  virtual bool isWritable(void) const;

  // @cmember Attempt to write the number of bytes given by <p byteCount>
  //          to the current position in this <c OMRawStorage>
  //          from the buffer at address <p bytes>.
  //          The actual number of bytes written is returned in
  //          <p bytesWritten>.
  //          Writing to positions greater than
  //          <mf OMRawStorage::size> causes this <c OMRawStorage>
  //          to be extended, however such extension can fail, causing
  //          <p bytesWritten> to be less than <p byteCount>.
  //          @precondition <f isWritable()>
  //   @devnote How is failure to extend indicated ?
  virtual void write(const OMByte* bytes,
                     OMUInt32 byteCount,
                     OMUInt32& bytesWritten);

  // @cmember Attempt to write the number of bytes given by <p byteCount>
  //          to offset <p position> in this <c OMRawStorage>
  //          from the buffer at address <p bytes>.
  //          The actual number of bytes written is returned in
  //          <p bytesWritten>.
  //          Writing to positions greater than
  //          <mf OMRawStorage::size> causes this <c OMRawStorage>
  //          to be extended, however such extension can fail, causing
  //          <p bytesWritten> to be less than <p byteCount>.
  //          @precondition <f isWritable()> && <f isPositionable()>
  //   @devnote How is failure to extend indicated ?
  virtual void writeAt(OMUInt64 position,
                       const OMByte* bytes,
                       OMUInt32 byteCount,
                       OMUInt32& bytesWritten);

  // @cmember May this <c OMRawStorage> be changed in size ?
  //          An implementation of <c OMRawStorage> for disk files
  //          would most probably return true. An implemetation
  //          for network streams would return false. An implementation
  //          for fixed size contiguous memory files (avoiding copying)
  //          would return false.
  virtual bool isExtendible(void) const;

    // @cmember The current extent of this <c OMRawStorage> in bytes.
    //          @precondition <f isPositionable()>
  virtual OMUInt64 extent(void) const;

  // @cmember Set the size of this <c OMRawStorage> to <p newSize> bytes.
  //          If <p newSize> is greater than <mf OMRawStorage::size>
  //          then this <c OMRawStorage> is extended. If <p newSize>
  //          is less than <mf OMRawStorage::size> then this
  //          <c OMRawStorage> is truncated. Truncation may also result
  //          in the current position for <f read()> and <f write()>
  //          being set to <mf OMRawStorage::size>.
  //          @precondition <f isExtendible()>
  //   @devnote How is failure to extend indicated ?
  virtual void extend(OMUInt64 newSize);

  // @cmember The current size of this <c OMRawStorage> in bytes.
  //          @precondition <f isPositionable()>
  virtual OMUInt64 size(void) const;

    // @cmember May the current position, for <f read()> and <f write()>,
    //          of this <c OMRawStorage> be changed ?
    //          An implementation of <c OMRawStorage> for disk files
    //          would most probably return true. An implemetation
    //          for network streams would return false. An implementation
    //          for memory files would return true.
  virtual bool isPositionable(void) const;

  // @cmember Synchronize this <c OMRawStorage> with its external
  //          representation.
  //          An implementation of <c OMRawStorage> for disk files would
  //          most probably implement this virtual function as a flush.
  //          This virtual function would probably be implemented as a
  //          noop in implemetations for network streams and for memory
  //          files.
  virtual void synchronize(void);


private:
  IAAFRawStorage * _rep;
  IAAFRandomRawStorage * _randRep;
};

#endif // ! __ImplAAFOMRawStorage_h__

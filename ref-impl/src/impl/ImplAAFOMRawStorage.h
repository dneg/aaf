#ifndef __ImplAAFOMRawStorage_h__
#define __ImplAAFOMRawStorage_h__

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

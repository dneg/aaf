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

// @doc OMEXTERNAL
#ifndef OMTYPE_H
#define OMTYPE_H

#include "OMDataTypes.h"

#include <stddef.h>

  // @class Abstract base class describing the data types that may be
  //        assumed by persistent properties supported by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMType {
public:
  // @access Public members.

    // @cmember Reorder (swap) the given <p externalBytes> according to the
    //          data type described by this <c OMType>. The number of
    //          bytes is given by <p externalBytesSize>. Object Manager clients
    //          must provide a suitable implementation of this virtual
    //          function for the data type being described. This class
    //          provides static functions to aid in such an implementation.
    //          The bytes to be swapped are assumed to be in external form.
  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const = 0;

    // @cmember The size, in bytes, of an entity described by
    //          <p internalBytes>, <p internalBytesSize>
    //          and this <c OMType> when persisted.
    //          Object Manager clients must provide a suitable
    //          implementation of this virtual function for the data
    //          type being described. This class provides static
    //          functions to aid in such an implementation.
  virtual size_t externalSize(const OMByte* internalBytes,
                              size_t internalBytesSize) const = 0;

    // @cmember Convert the given <p internalBytes> from internal
    //          (in memory) representation to external (persisted)
    //          representation, according to the data type described
    //          by this <c OMType>. The number of bytes is given by
    //          <p internalBytesSize>. Together <p externalBytes>
    //          and <p externalBytesSize> define a buffer that must
    //          be of appropriate size to receive the converted
    //          value. Note that the resulting external value may be
    //          smaller than, larger than, or the same size as the
    //          original internal value depending on the relationship
    //          between <mf OMType::internalSize> and
    //          <mf OMType::externalSize>.
    //          The value is in the <p byteOrder> specified.
    //          Object Manager clients must provide a suitable
    //          implementation of this virtual function for the data
    //          type being described. This class provides static
    //          functions to aid in such an implementation.
  virtual void externalize(const OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const = 0;


    // @cmember The size, in bytes, of an entity described by
    //          <p externalBytes>, <p externalBytesSize>
    //          and this <c OMType> when in memory.
    //          Object Manager clients must provide a suitable
    //          implementation of this virtual function for the data
    //          type being described. This class provides static
    //          functions to aid in such an implementation.
  virtual size_t internalSize(const OMByte* externalBytes,
                              size_t externalSize) const = 0;

    // @cmember Convert the given <p externalBytes> from external
    //          (persisted) representation to internal (in memory)
    //          representation, according to the data type described
    //          by this <c OMType>. The number of bytes is given by
    //          <p externalBytesSize>. Together <p internalBytes>
    //          and <p internalBytesSize> define a buffer that must
    //          be of appropriate size to receive the converted
    //          value. Note that the resulting internal value may be
    //          smaller than, larger than, or the same size as the
    //          original external value depending on the relationship
    //          between <mf OMType::externalSize> and
    //          <mf OMType::internalSize>.
    //          The value is in the <p byteOrder> specified.
    //          Object Manager clients must provide a suitable
    //          implementation of this virtual function for the data
    //          type being described. This class provides static
    //          functions to aid in such an implementation.
  virtual void internalize(const OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const = 0;

  // @access Static members.

    // @cmember Reorder (swap) the integer described by <p bytes> and
    //          <p bytesSize>. This static function is provided to aid
    //          Object Manager clients in providing suitable
    //          implementations of the virtual functions in this class.
    //          In particular, Object Manager clients may wish to use
    //          this function when implementing <mf OMType::reorder>.
  static void reorderInteger(OMByte* bytes,
                             size_t bytesSize);

    // @cmember Expand, by padding with leading zeros, the value decribed
    //          by <p inputBytes> and <p inputBytesSize> into the buffer
    //          described by <p outputBytes> and <p outputBytesSize>. The
    //          value is in the <p byteOrder> specified. This static function
    //          is provided to aid Object Manager clients in providing
    //          suitable implementations of the virtual functions in this
    //          class. In particular, Object Manager clients may wish to use
    //          this function when implementing <mf OMType::internalize> and
    //          <mf OMType::externalize>.
  static void expand(const OMByte* inputBytes,
                     size_t inputBytesSize,
                     OMByte* outputBytes,
                     size_t outputBytesSize,
                     OMByteOrder byteOrder);

    // @cmember Contract, by discarding leading bytes, the value decribed
    //          by <p inputBytes> and <p inputBytesSize> into the buffer
    //          described by <p outputBytes> and <p outputBytesSize>. The
    //          value is in the <p byteOrder> specified. This static function
    //          is provided to aid Object Manager clients in providing
    //          suitable implementations of the virtual functions in this
    //          class. In particular, Object Manager clients may wish to use
    //          this function when implementing <mf OMType::internalize> and
    //          <mf OMType::externalize>.
  static void contract(const OMByte* inputBytes,
                       size_t inputBytesSize,
                       OMByte* outputBytes,
                       size_t outputBytesSize,
                       OMByteOrder byteOrder);

    // @cmember Copy the value decribed by <p inputBytes> and <p bytesSize>
    //          into the buffer described by <p outputBytes> and
    //          <p bytesSize>. This static function is provided to aid
    //          Object Manager clients in providing suitable implementations
    //          of the virtual functions in this class. In particular,
    //          Object Manager clients may wish to use this function when
    //          implementing <mf OMType::internalize> and
    //          <mf OMType::externalize>.
  static void copy(const OMByte* inputBytes,
                   OMByte* outputBytes,
                   size_t bytesSize);

};

#endif

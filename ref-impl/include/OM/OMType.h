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

// @doc OMEXTERNAL
#ifndef OMTYPE_H
#define OMTYPE_H

#include "OMDataTypes.h"

#include <stddef.h>

  // @class Abstract base class describing the data types that may be
  //        assumed by persistent properties supported by the Object Manager.
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
  virtual size_t externalSize(OMByte* internalBytes,
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
  virtual void externalize(OMByte* internalBytes,
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
  virtual size_t internalSize(OMByte* externalBytes,
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
  virtual void internalize(OMByte* externalBytes,
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
  static void expand(OMByte* inputBytes,
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
  static void contract(OMByte* inputBytes,
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
  static void copy(OMByte* inputBytes,
                   OMByte* outputBytes,
                   size_t bytesSize);

};

#endif

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
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

#include "OMDefinition.h"
#include "OMDataTypes.h"

class OMTypeVisitor;

  // @class Abstract base class describing the data types that may be
  //        assumed by persistent properties supported by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMType : public OMDefinition {
public:
  // @access Public members.

  enum Tag {
    OMTTUnknown               =  0,
    OMTTInteger               =  1,
    OMTTCharacter             =  2,
    OMTTStrongObjectReference =  3,
    OMTTWeakObjectReference   =  4,
    OMTTRename                =  5,
    OMTTEnumerated            =  6,
    OMTTFixedArray            =  7,
    OMTTVaryingArray          =  8,
    OMTTSet                   =  9,
    OMTTRecord                = 10,
    OMTTStream                = 11,
    OMTTString                = 12,
    OMTTExtendibleEnumerated  = 13,
    OMTTIndirect              = 14,
    OMTTOpaque                = 15};

  virtual OMType::Tag tag(void) const = 0;

    // @cmember Are all instances of this type the same size ?
    //          The <f externalSize()> and <f internalSize()> of
    //          fixed size types may be determined independently
    //          of a specific instance of the type. Note that a fixed
    //          size type may have differing internal and external
    //          sizes and, if so, must be externalized and internalized.
  virtual bool isFixedSize(void) const = 0;

    // @cmember Reorder (swap) the given <p externalBytes> according to the
    //          data type described by this <c OMType>. The number of
    //          bytes is given by <p externalBytesSize>. Object Manager clients
    //          must provide a suitable implementation of this virtual
    //          function for the data type being described. This class
    //          provides static functions to aid in such an implementation.
    //          The bytes to be swapped are assumed to be in external form.
  virtual void reorder(OMByte* externalBytes,
                       OMUInt32 externalBytesSize) const = 0;

    // @cmember The size, in bytes, of an entity described by
    //          <p internalBytes>, <p internalBytesSize>
    //          and this <c OMType> when persisted.
    //          Object Manager clients must provide a suitable
    //          implementation of this virtual function for the data
    //          type being described. This class provides static
    //          functions to aid in such an implementation.
  virtual OMUInt32 externalSize(const OMByte* internalBytes,
                                OMUInt32 internalBytesSize) const = 0;

    // @cmember The size, in bytes, of an entity of this <c OMType>
    //          when persisted. Object Manager clients must provide
    //          a suitable implementation of this virtual function
    //          for the data type being described.
    //          @precondition <f isFixedSize()>
  virtual OMUInt32 externalSize(void) const = 0;

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
                           OMUInt32 internalBytesSize,
                           OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByteOrder byteOrder) const = 0;


    // @cmember The size, in bytes, of an entity described by
    //          <p externalBytes>, <p externalBytesSize>
    //          and this <c OMType> when in memory.
    //          Object Manager clients must provide a suitable
    //          implementation of this virtual function for the data
    //          type being described. This class provides static
    //          functions to aid in such an implementation.
  virtual OMUInt32 internalSize(const OMByte* externalBytes,
                                OMUInt32 externalSize) const = 0;

    // @cmember The size, in bytes, of an entity of this <c OMType>
    //          when in memory. Object Manager clients must provide
    //          a suitable implementation of this virtual function
    //          for the data type being described.
    //          @precondition <f isFixedSize()>
  virtual OMUInt32 internalSize(void) const = 0;

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
                           OMUInt32 externalBytesSize,
                           OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByteOrder byteOrder) const = 0;

  // @access Static members.

    // @cmember Reorder (swap) the integer described by <p bytes> and
    //          <p bytesSize>. This static function is provided to aid
    //          Object Manager clients in providing suitable
    //          implementations of the virtual functions in this class.
    //          In particular, Object Manager clients may wish to use
    //          this function when implementing <mf OMType::reorder>.
  static void reorderInteger(OMByte* bytes,
                             OMUInt32 bytesSize);

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
                     OMUInt32 inputBytesSize,
                     OMByte* outputBytes,
                     OMUInt32 outputBytesSize,
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
                       OMUInt32 inputBytesSize,
                       OMByte* outputBytes,
                       OMUInt32 outputBytesSize,
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
                   OMUInt32 bytesSize);

  virtual void accept(OMTypeVisitor& visitor) const = 0;

};

#endif

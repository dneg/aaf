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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMType

#include "OMType.h"

#include "OMAssertions.h"

#include <string.h>

  // @mfunc Reorder (swap) the integer described by <p bytes> and
  //        <p bytesSize>. This static function is provided to aid
  //        Object Manager clients in providing suitable
  //        implementations of the virtual functions in this class.
  //        In particular, Object Manager clients may wish to use
  //        this function when implementing <mf OMType::reorder>.
  //   @parm The buffer containing the bytes to reorder.
  //   @parm The size of the buffer.
void OMType::reorderInteger(OMByte* bytes,
                            size_t bytesSize)
{
  TRACE("OMType::reorderInteger");
  PRECONDITION("Valid size", (bytesSize == 2) ||
                             (bytesSize == 4) ||
                             (bytesSize == 8));


  size_t bytesToSwap = bytesSize / 2;
  for (size_t i = 0; i < bytesToSwap; i++) {
    OMByte temp = bytes[i];
    bytes[i] = bytes[bytesSize - i - 1];
    bytes[bytesSize - i - 1] = temp;
  }

}

  // @mfunc Expand, by padding with leading zeros, the value decribed
  //        by <p inputBytes> and <p inputBytesSize> into the buffer
  //        described by <p outputBytes> and <p outputBytesSize>. The
  //        value is in the <p byteOrder> specified. This static function
  //        is provided to aid Object Manager clients in providing
  //        suitable implementations of the virtual functions in this
  //        class. In particular, Object Manager clients may wish to use
  //        this function when implementing <mf OMType::internalize> and
  //        <mf OMType::externalize>.
  //   @parm The buffer containing the input bytes.
  //   @parm The size of the input buffer.
  //   @parm The buffer to receive the output bytes.
  //   @parm The size of the output buffer.
  //   @parm The byte order.
void OMType::expand(OMByte* inputBytes,
                    size_t inputBytesSize,
                    OMByte* outputBytes,
                    size_t outputBytesSize,
                    OMByteOrder byteOrder)
{
  TRACE("OMType::expand");
  PRECONDITION("Valid input bytes", inputBytes != 0);
  PRECONDITION("Valid output bytes", outputBytes != 0);
  PRECONDITION("Valid output size", outputBytesSize > 0);
  PRECONDITION("Valid sizes", outputBytesSize > inputBytesSize);

  OMByte* ip = inputBytes;
  OMByte* op = outputBytes;
  size_t padSize = outputBytesSize - inputBytesSize;

  if (byteOrder == littleEndian) {
    // copy valid bytes
    memcpy(op, ip, inputBytesSize);
    op = op + inputBytesSize;
    // zero fill pad bytes (assumes sign extension not wanted)
    memset(op, 0, padSize); 
  } else { // byteOrder == bigEndian
    // zero fill pad bytes (assumes sign extension not wanted)
    memset(op, 0, padSize); 
    op = op + padSize;
    // copy valid bytes
    memcpy(op, ip, inputBytesSize);
  }
}

  // @mfunc Contract, by discarding leading bytes, the value decribed
  //        by <p inputBytes> and <p inputBytesSize> into the buffer
  //        described by <p outputBytes> and <p outputBytesSize>. The
  //        value is in the <p byteOrder> specified. This static function
  //        is provided to aid Object Manager clients in providing
  //        suitable implementations of the virtual functions in this
  //        class. In particular, Object Manager clients may wish to use
  //        this function when implementing <mf OMType::internalize> and
  //        <mf OMType::externalize>.
  //   @parm The buffer containing the input bytes.
  //   @parm The size of the input buffer.
  //   @parm The buffer to receive the output bytes.
  //   @parm The size of the output buffer.
  //   @parm The byte order.
void OMType::contract(OMByte* inputBytes,
                      size_t inputBytesSize,
                      OMByte* outputBytes,
                      size_t outputBytesSize,
                      OMByteOrder byteOrder)
{
  TRACE("OMType::contract");
  PRECONDITION("Valid input bytes", inputBytes != 0);
  PRECONDITION("Valid output bytes", outputBytes != 0);
  PRECONDITION("Valid output size", outputBytesSize > 0);
  PRECONDITION("Valid sizes", outputBytesSize < inputBytesSize);

  OMByte* ip = inputBytes;
  OMByte* op = outputBytes;
  size_t trimSize = inputBytesSize - outputBytesSize;

  if (byteOrder == littleEndian) {
    // copy valid bytes
    memcpy(op, ip, outputBytesSize);
    // ignore trimmed bytes (nop)
  } else { // byteOrder == bigEndian
    // ignore trimmed bytes
    ip = ip + trimSize;
    // copy valid bytes
    memcpy(op, ip, outputBytesSize);
  }
}

  // @mfunc Copy the value decribed by <p inputBytes> and <p bytesSize>
  //        into the buffer described by <p outputBytes> and
  //        <p bytesSize>. This static function is provided to aid
  //        Object Manager clients in providing suitable implementations
  //        of the virtual functions in this class. In particular,
  //        Object Manager clients may wish to use this function when
  //        implementing <mf OMType::internalize> and
  //        <mf OMType::externalize>.
  //   @parm The buffer containing the input bytes.
  //   @parm The buffer to receive the output bytes.
  //   @parm The size of the input and output buffers.
void OMType::copy(OMByte* inputBytes,
                  OMByte* outputBytes,
                  size_t bytesSize)
{
  TRACE("OMType::copy");
  PRECONDITION("Valid input bytes", inputBytes != 0);
  PRECONDITION("Valid output bytes", outputBytes != 0);
  PRECONDITION("Valid buffer size", bytesSize > 0);

  OMByte* ip = inputBytes;
  OMByte* op = outputBytes;

  for (size_t i = 0; i < bytesSize; i++) {
    *op++ = *ip++;
  }
}

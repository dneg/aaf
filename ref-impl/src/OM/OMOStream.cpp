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

#if defined(OM_ENABLE_DEBUG)

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMOStream

#include "OMOStream.h"

  // @mfunc Insert a character string.
  //   @parm The string to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (const char* string)
{
  return put(string);
}

  // @mfunc Insert an OMUInt32 in decimal.
  //   @parm The OMUInt32 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMUInt32 i)
{
  return put(i);
}

  // @mfunc Insert a pointer in hex.
  //   @parm The pointer to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (void* p)
{
  return put(p);
}

  // @mfunc Put a new line.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::endLine(void)
{
  return putLine();
}

  // @mfunc Insert (call) a manipulator.
  //   @parm The manipulator to insert (call).
  //   @rdesc The modified <c OMOStream>
OMOStream&
OMOStream::operator << (OMOStream& (*manipulator)(OMOStream&))
{
  return(*manipulator)(*this);
}

 // @func <c OMOStream> end of line manipulator.
 //   @parm The <c OMOStream> in which to inset the new line.
  //   @rdesc The modified <c OMOStream>
OMOStream& endl(OMOStream& s)
{
  return s.endLine();
}

 // @globalv Global <c OMOStream> for Object Manager logging.
 //          Debug use only.
OMOStream omlog;

// @devnote If your platform doesn't have iostream.h you'll need to
//          implement the following functions differently.

#include <iostream.h>

  // @mfunc Put a character string.
  //   @parm The character string to be written.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(const char* string)
{
  cerr << string;
  return *this;
}

  // @mfunc Put an OMUInt32 in decimal.
  //   @parm The OMUInt32 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMUInt32 i)
{
  cerr << i;
  return *this;
}

  // @mfunc Put a pointer in hex.
  //   @parm The pointer to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(void* p)
{
  cerr << p;
  return *this;
}

  // @mfunc Put a new line.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::putLine(void)
{
  cerr << endl;
  return *this;
}

#endif

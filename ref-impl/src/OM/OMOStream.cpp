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

  // @mfunc Insert an OMInt32 in decimal.
  //   @parm The OMInt32 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMInt32 i)
{
  return put(i);
}

  // @mfunc Insert an OMUInt16 in decimal.
  //   @parm The OMUInt16 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMUInt16 i)
{
  return put(i);
}

  // @mfunc Insert an OMInt16 in decimal.
  //   @parm The OMInt16 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMInt16 i)
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

// @devnote If your platform doesn't have <iostream> you'll need to
//          implement the following functions differently.

#if !defined(OM_OUTPUT_TO_DEBUGGER)

// Diagnostic output to cerr

#include <iostream>
using std::cerr;
using std::endl;

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

  // @mfunc Put an OMInt32 in decimal.
  //   @parm The OMInt32 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMInt32 i)
{
  cerr << i;
  return *this;
}

  // @mfunc Put an OMUInt16 in decimal.
  //   @parm The OMUInt16 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMUInt16 i)
{
  cerr << i;
  return *this;
}

  // @mfunc Put an OMInt16 in decimal.
  //   @parm The OMInt16 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMInt16 i)
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

#else

// Diagnostic output to debugger
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "OMUtilities.h"

#if defined (OM_OS_WINDOWS)

#include <windows.h>

static void debugPrint(const char* string)
{
  wchar_t s[256];
  convertStringToWideString(s, string, sizeof(s)/sizeof(s[0]));
  OutputDebugString(s);
}

#elif defined(OM_OS_MACOS) || defined(OM_OS_MACOSX)

#include <MacTypes.h>

static char buffer[256];
static size_t current = 0;

static void debugFlush(void)
{
  unsigned char p[256];
  copyCToPString(p, current + 1, buffer);
  DebugStr(p);
  current = 0;
}

static void debugPrint(const char c)
{
  if (current == sizeof(buffer)) {
    debugFlush();
    buffer[current++] = c;
  } else if (c == '\n') {
    debugFlush();
  } else {
    buffer[current++] = c;
  }
}

static void debugPrint(const char* string)
{
  size_t i = 0;
  while (string[i] != 0) {
    debugPrint(string[i++]);
  }
}

#endif

OMOStream& OMOStream::put(const char* string)
{
  debugPrint(string);
  return *this;
}

OMOStream& OMOStream::put(OMUInt32 i)
{
  ostringstream s;
  s << i << ends;
  string buffer = s.str();
  debugPrint(buffer.c_str());
  return *this;
}

OMOStream& OMOStream::put(OMInt32 i)
{
  ostringstream s;
  s << i << ends;
  string buffer = s.str();
  debugPrint(buffer.c_str());
  return *this;
}

OMOStream& OMOStream::put(OMUInt16 i)
{
  ostringstream s;
  s << i << ends;
  string buffer = s.str();
  debugPrint(buffer.c_str());
  return *this;
}

OMOStream& OMOStream::put(OMInt16 i)
{
  ostringstream s;
  s << i << ends;
  string buffer = s.str();
  debugPrint(buffer.c_str());
  return *this;
}

OMOStream& OMOStream::put(void* p)
{
  ostringstream s;
  s << p << ends;
  string buffer = s.str();
  debugPrint(buffer.c_str());
  return *this;
}

OMOStream& OMOStream::putLine(void)
{
  debugPrint("\n");
  return *this;
}

#endif


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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMOStream

#include "OMOStream.h"

#include "OMUtilities.h"

#include <sstream>
#include <string>
using namespace std;

static void format(ostringstream& s,
                   int base,
                   int& width);

  // @mfunc Constructor.
OMOStream::OMOStream(void)
: _base(10),
  _width(0)
{
}

  // @mfunc Insert a character string.
  //   @parm The string to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (const char* string)
{
  return put(string);
}

  // @mfunc Insert an OMUInt64.
  //   @parm The OMUInt64 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMUInt64 i)
{
  return put(i);
}

  // @mfunc Insert an OMInt64.
  //   @parm The OMInt64 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMInt64 i)
{
  return put(i);
}

  // @mfunc Insert an OMUInt32.
  //   @parm The OMUInt32 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMUInt32 i)
{
  return put(i);
}

  // @mfunc Insert an OMInt32.
  //   @parm The OMInt32 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMInt32 i)
{
  return put(i);
}

  // @mfunc Insert an OMUInt16.
  //   @parm The OMUInt16 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMUInt16 i)
{
  return put(i);
}

  // @mfunc Insert an OMInt16.
  //   @parm The OMInt16 to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (OMInt16 i)
{
  return put(i);
}

  // @mfunc Insert a pointer.
  //   @parm The pointer to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (void* p)
{
  return put(p);
}

  // @mfunc Insert a double.
  //   @parm The double to insert.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::operator << (double d)
{
  return put(d);
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
OMOStream& OMOStream::operator << (OMOStream& (*manipulator)(OMOStream&))
{
  return(*manipulator)(*this);
}

OMOStream& OMOStream::operator << (const OMOStreamManipulator& m)
{
  return m._f(*this, m._i);
}

OMOStream& OMOStream::dec(void)
{
  _base = 10;
  return *this;
}

OMOStream& OMOStream::hex(void)
{
  _base = 16;
  return *this;
}

OMOStream& OMOStream::setw(int n)
{
  _width = n;
  return *this;
}

 // @func <c OMOStream> end of line manipulator.
 //   @parm The <c OMOStream> in which to inset the new line.
  //   @rdesc The modified <c OMOStream>
OMOStream& endl(OMOStream& s)
{
  return s.endLine();
}

OMOStream& dec(OMOStream& s)
{
  return s.dec();
}

OMOStream& hex(OMOStream& s)
{
  return s.hex();
}

OMOStream& set_w(OMOStream& s, int n)
{
  return s.setw(n);
}

#if defined(_MSC_VER) && (_MSC_VER <= 1200)
// Missing from VC++ version 6.0 and earlier

ostream& operator << (ostream& s, __int64 i);

ostream& operator << (ostream& s, unsigned __int64 i);

ostream& operator << (ostream& s, __int64 i)
{
  s << (OMInt32)i; // tjb - hack
  return s;
}

ostream& operator << (ostream& s, unsigned __int64 i)
{
  s << (OMUInt32)i; // tjb - hack
  return s;
}

#endif

  // @mfunc Put an OMUInt64.
  //   @parm The OMUInt64 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMUInt64 i)
{
  ostringstream s;
  format(s, _base, _width);
  s << i << ends;
  string buffer = s.str();
  put(buffer.c_str());
  return *this;
}

  // @mfunc Put an OMInt64.
  //   @parm The OMInt64 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMInt64 i)
{
  ostringstream s;
  format(s, _base, _width);
  s << i << ends;
  string buffer = s.str();
  put(buffer.c_str());
  return *this;
}

  // @mfunc Put an OMUInt32.
  //   @parm The OMUInt32 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMUInt32 i)
{
  ostringstream s;
  format(s, _base, _width);
  s << i << ends;
  string buffer = s.str();
  put(buffer.c_str());
  return *this;
}

  // @mfunc Put an OMInt32.
  //   @parm The OMInt32 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMInt32 i)
{
  ostringstream s;
  format(s, _base, _width);
  s << i << ends;
  string buffer = s.str();
  put(buffer.c_str());
  return *this;
}

  // @mfunc Put an OMUInt16.
  //   @parm The OMUInt16 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMUInt16 i)
{
  ostringstream s;
  format(s, _base, _width);
  s << i << ends;
  string buffer = s.str();
  put(buffer.c_str());
  return *this;
}

  // @mfunc Put an OMInt16.
  //   @parm The OMInt16 to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(OMInt16 i)
{
  ostringstream s;
  format(s, _base, _width);
  s << i << ends;
  string buffer = s.str();
  put(buffer.c_str());
  return *this;
}

  // @mfunc Put a pointer.
  //   @parm The pointer to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(void* p)
{
  ostringstream s;
  format(s, _base, _width);
  s << p << ends;
  string buffer = s.str();
  put(buffer.c_str());
  return *this;
}

  // @mfunc Put a double.
  //   @parm The double to write.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMOStream::put(double d)
{
  ostringstream s;
  format(s, _base, _width);
  s << d << ends;
  string buffer = s.str();
  put(buffer.c_str());
  return *this;
}

#if defined(_MSC_VER)

// Disable warning from pragma below.
//
#pragma warning(disable:4073)

// Ensure that this compilation unit (in particular the global
// OMOStream objects below) is initialized after the run-time library
// and before any client code.
//
#pragma init_seg(lib)

#endif

 // @globalv Global <c OMDiagnosticStream> for Object Manager logging.
OMDiagnosticStream omlog;

OMStandardOutputStream omout;

// @devnote If your platform doesn't have <lt>iostream<gt> you'll need to
//          implement the <mf OMOStream::put> and <mf OMOStream::putLine>
//          functions differently.

// Diagnostic output to cerr

#include <iostream>
using namespace std;

OMStandardDiagnosticStream::OMStandardDiagnosticStream(void)
{
}

  // @mfunc Put a character string.
  //   @parm The character string to be written.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMStandardDiagnosticStream::put(const char* string)
{
  cerr << string;
  return *this;
}

  // @mfunc Put a new line.
  //   @rdesc The modified <c OMOStream>
OMOStream& OMStandardDiagnosticStream::putLine(void)
{
  cerr << endl;
  return *this;
}

OMStandardOutputStream::OMStandardOutputStream(void)
{
}

OMOStream& OMStandardOutputStream::put(const char* string)
{
  cout << string;
  return *this;
}

OMOStream& OMStandardOutputStream::putLine(void)
{
  cout << endl;
  return *this;
}

// Diagnostic output to debugger

#if defined (OM_OS_WINDOWS)

#include <windows.h>

static void debugPrint(const char* string)
{
  wchar_t s[256];
  convertStringToWideString(s, string, sizeof(s)/sizeof(s[0]));
  OutputDebugString(s);
}

#elif defined(OM_OS_MACOSX)

#include <CoreServices/CoreServices.h>

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

#else

static void debugPrint(const char* string)
{
  cerr << string;
}

#endif

OMDebuggerDiagnosticStream::OMDebuggerDiagnosticStream(void)
{
}

OMOStream& OMDebuggerDiagnosticStream::put(const char* string)
{
  debugPrint(string);
  return *this;
}

OMOStream& OMDebuggerDiagnosticStream::putLine(void)
{
  debugPrint("\n");
  return *this;
}

OMDiagnosticStream::OMDiagnosticStream(void)
{
  initialize();
//TRACE("OMDiagnosticStream::OMDiagnosticStream");
//POSTCONDITION("Valid stream", _stream != 0);
}

OMDiagnosticStream::OMDiagnosticStream(OMOStream* stream)
: _stream(stream)
{
//TRACE("OMDiagnosticStream::OMDiagnosticStream");
//POSTCONDITION("Valid stream", _stream != 0);
}

OMDiagnosticStream::~OMDiagnosticStream(void)
{
  delete _stream; // We own _stream
  _stream = 0;
}

void OMDiagnosticStream::initialize(void)
{
#if defined(OM_OUTPUT_TO_DEBUGGER)
  _stream = new OMDebuggerDiagnosticStream();
#else
  _stream = new OMStandardDiagnosticStream();
#endif
}

void OMDiagnosticStream::setStream(OMOStream* stream)
{
//TRACE("OMDiagnosticStream::setStream");
//PRECONDITION("Valid stream", stream != 0);

  delete _stream; // We own _stream
  _stream = stream;
}

OMOStream& OMDiagnosticStream::put(const char* string)
{
//TRACE("OMDiagnosticStream::put");
  (*_stream) << string;
  return *this;
}

OMOStream& OMDiagnosticStream::putLine(void)
{
//TRACE("OMDiagnosticStream::putLine");
  (*_stream) << endl;
  return *this;
}

static void format(ostringstream& s,
                   int base,
                   int& width)
{
  if (width != 0) {
    s.width(width);
    width = 0;
  }
  if (base != 10) {
    s.setf(ios::hex,ios::basefield);
  }
}

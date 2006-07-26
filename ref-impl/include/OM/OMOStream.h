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
#ifndef OMOSTREAM_H
#define OMOSTREAM_H

#include "OMDataTypes.h"

class OMOStreamManipulator;

  // @class Simple, platform independent, text output stream
  //        for diagnostic (debug only) use by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMOStream {
public:
  // @access Public members.

    // @cmember Constructor.
  OMOStream(void);

    // @cmember Insert a character string.
  OMOStream& operator << (const char* string);

    // @cmember Insert an OMUInt64.
  OMOStream& operator << (OMUInt64 i);

    // @cmember Insert an OMInt64.
  OMOStream& operator << (OMInt64 i);

    // @cmember Insert an OMUInt32.
  OMOStream& operator << (OMUInt32 i);

    // @cmember Insert an OMInt32.
  OMOStream& operator << (OMInt32 i);

    // @cmember Insert an OMUInt16.
  OMOStream& operator << (OMUInt16 i);

    // @cmember Insert an OMInt16.
  OMOStream& operator << (OMInt16 i);

    // @cmember Insert a pointer.
  OMOStream& operator << (void* p);

    // @cmember Insert a double.
  OMOStream& operator << (double d);

    // @cmember Insert a new line.
  OMOStream& endLine(void);

    // @cmember Insert (call) a manipulator.
  OMOStream& operator << (OMOStream& (*manipulator)(OMOStream&));

    // @cmember Insert (call) a manipulator.
  OMOStream& operator << (const OMOStreamManipulator& m);

  OMOStream& dec(void);

  OMOStream& hex(void);

  OMOStream& setw(int n);

protected:
  // @access Protected members.

    // @cmember Put a character string.
  virtual OMOStream& put(const char* string) = 0;

    // @cmember Put a new line.
  virtual OMOStream& putLine(void) = 0;

    // @cmember Put an OMUInt64.
  OMOStream& put(OMUInt64 i);

    // @cmember Put an OMInt64.
  OMOStream& put(OMInt64 i);

    // @cmember Put an OMUInt32.
  OMOStream& put(OMUInt32 i);

    // @cmember Put an OMInt32.
  OMOStream& put(OMInt32 i);

    // @cmember Put an OMUInt16.
  OMOStream& put(OMUInt16 i);

    // @cmember Put an OMInt16.
  OMOStream& put(OMInt16 i);

    // @cmember Put a pointer.
  OMOStream& put(void* p);

    // @cmember Put a double.
  OMOStream& put(double d);

private:
  int _base;
  int _width;
};

class OMOStreamManipulator {
public:
  OMOStreamManipulator(OMOStream& (*f)(OMOStream&, int), int i);

  OMOStream& (*_f)(OMOStream&, int);
  int _i;
};

inline OMOStreamManipulator::OMOStreamManipulator(
                                              OMOStream& (*f)(OMOStream&, int),
                                              int i)
: _f(f),
  _i(i)
{
}

 // @func <c OMOStream> end of line manipulator.
 //   @parm The <c OMOStream> in which to inset the new line.
 //   @rdesc The modified <c OMOStream>.
OMOStream& endl(OMOStream& s);

OMOStream& dec(OMOStream& s);

OMOStream& hex(OMOStream& s);

OMOStream& set_w(OMOStream& s, int n);

inline OMOStreamManipulator setw(int n)
{
  return OMOStreamManipulator(set_w, n);
}

// Diagnostic stream which outputs to standard error/cerr
//
class OMStandardDiagnosticStream : public OMOStream {
public:

  OMStandardDiagnosticStream(void);

protected:

  virtual OMOStream& put(const char* string);

  virtual OMOStream& putLine(void);

};

// Diagnostic stream which outputs to standard out/cout
//
class OMStandardOutputStream : public OMOStream {
public:

  OMStandardOutputStream(void);

protected:

  virtual OMOStream& put(const char* string);

  virtual OMOStream& putLine(void);

};

// Diagnostic stream which outputs to the debugger
//
class OMDebuggerDiagnosticStream : public OMOStream {
public:

  OMDebuggerDiagnosticStream(void);

protected:

  virtual OMOStream& put(const char* string);

  virtual OMOStream& putLine(void);

};

// Run-time configurable diagnostic stream
//
class OMDiagnosticStream : public OMOStream {
public:

  OMDiagnosticStream(void);

  OMDiagnosticStream(OMOStream* stream);

  virtual ~OMDiagnosticStream(void);

    // Set the disgnostic stream, relinquishing ownership
  void setStream(OMOStream* stream);

protected:

  void initialize(void);

  virtual OMOStream& put(const char* string);

  virtual OMOStream& putLine(void);

private:
  OMOStream* _stream; // Owned by this
};

 // @globalv Global <c OMDiagnosticStream> for Object Manager logging.
extern OMDiagnosticStream omlog;

extern OMStandardOutputStream omout;

#endif

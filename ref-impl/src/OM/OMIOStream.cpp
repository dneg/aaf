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

// @doc OMINTERNAL
#include "OMIOStream.h"

#include "OMDataTypes.h"
#include "OMRawStorage.h"
#include "OMUtilities.h"
#include "OMAssertions.h"

#include <iostream.h>
#if defined(OM_OS_WINDOWS)
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#if defined(OM_OS_WINDOWS)
#define NEWLINE "\r\n"
#elif defined(OM_OS_MACOS)
#define NEWLINE "\r"
#elif defined(OM_OS_UNIX)
#define NEWLINE "\n"
#endif

static void print(strstream& s, const OMObjectIdentification& id);

static void print(strstream& s, const OMMaterialIdentification& id);

static void format(strstream& s,
                   bool& showBase,
                   size_t base,
                   bool uppercase,
                   char fill,
                   int& width);

OMIOStream::OMIOStream(OMRawStorage* rawStorage)
: _store(rawStorage),
  _showBase(false),
  _base(10),
  _uppercase(false),
  _fill(0),
  _width(0),
  _newLine(NEWLINE)
{
}

OMIOStream::~OMIOStream(void)
{
}

void OMIOStream::put(char c)
{
  char buffer[2];
  buffer[0] = c;
  buffer[1] = 0;
  write(buffer);
}

OMIOStream& OMIOStream::operator << (const char* string)
{
  write(string);
  return *this;
}

OMIOStream& OMIOStream::operator << (const wchar_t* string)
{
  char* s = convertWideString(string);
  write(s);
  delete [] s;
  return *this;
}

OMIOStream& OMIOStream::operator << (const OMUInt8 i)
{
  strstream s;
  format(s, _showBase, _base, _uppercase, _fill, _width);
  s << (int)i << ends;
  char* buffer = s.str();
  write(buffer);
  delete [] buffer;
  return *this;
}

OMIOStream& OMIOStream::operator << (const OMUInt16 i)
{
  strstream s;
  format(s, _showBase, _base, _uppercase, _fill, _width);
  s << i << ends;
  char* buffer = s.str();
  write(buffer);
  delete [] buffer;
  return *this;
}

OMIOStream& OMIOStream::operator << (const OMUInt32 i)
{
  strstream s;
  format(s, _showBase, _base, _uppercase, _fill, _width);
  s << i << ends;
  char* buffer = s.str();
  write(buffer);
  delete [] buffer;
  return *this;
}

OMIOStream& OMIOStream::operator << (const OMUInt64 i)
{
  // TBS assumes small
  OMUInt32 x = (OMUInt32)i;
  *this << x;
  return *this;
}

OMIOStream& OMIOStream::operator << (const OMObjectIdentification& id)
{
  TRACE("OMIOStream::operator << (const OMObjectIdentification& id)");

  strstream s;
  format(s, _showBase, _base, _uppercase, _fill, _width);

  s.setf(ios::hex,ios::basefield);
  s.fill('0');
 
  print(s, id);

  s << ends;
  char* buffer = s.str();
  write(buffer);
  delete [] buffer;
  return *this;
}

OMIOStream& OMIOStream::operator << (const OMMaterialIdentification& id)
{
  TRACE("OMIOStream::operator << (const OMMaterialIdentification& id)");

  strstream s;
  format(s, _showBase, _base, _uppercase, _fill, _width);

  s.setf(ios::hex,ios::basefield);
  s.fill('0');
 
  print(s, id);

  s << ends;
  char* buffer = s.str();
  write(buffer);
  delete [] buffer;
  return *this;
}

OMIOStream& OMIOStream::operator << (OMIOStream& (*manipulator)(OMIOStream&))
{
  return (*manipulator)(*this);
}

OMIOStream& OMIOStream::operator << (const OMIOStreamManipulator& m)
{
  return m._f(*this, m._i);
}

size_t OMIOStream::_level = 0;

OMIOStream& OMIOStream::beginl(void)
{
  for (size_t i = 1; i < _level; i++) {
    *this << "  ";
  }
  return *this;
}

OMIOStream& OMIOStream::endl(void)
{
  write(_newLine);
  flush();
  return *this;
}

OMIOStream& OMIOStream::indent(void)
{
  _level = _level + 1;
  return *this;
}

OMIOStream& OMIOStream::outdent(void)
{
  _level = _level - 1;
  return *this;
}

OMIOStream& OMIOStream::flush(void)
{
  _store->synchronize();
  return *this;
}

OMIOStream& OMIOStream::showbase(void)
{
  _showBase = true;
  return *this;
}

OMIOStream& OMIOStream::dec(void)
{
  _base = 10;
  return *this;
}

OMIOStream& OMIOStream::hex(void)
{
  _base = 16;
  return *this;
}

OMIOStream& OMIOStream::uppercase(void)
{
  _uppercase = true;
  return *this;
}

OMIOStream& OMIOStream::lowercase(void)
{
  _uppercase = false;
  return *this;
}

OMIOStream& OMIOStream::setfill(int c)
{
  _fill = c;
  return *this;
}

OMIOStream& OMIOStream::setw(int n)
{
  _width = n;
  return *this;
}

void OMIOStream::write(const char* string)
{
  OMUInt32 size;
  _store->write(reinterpret_cast<const OMByte*>(string), strlen(string), size);
}

OMIOStream& beginl(OMIOStream& s)
{
  return s.beginl();
}

OMIOStream& endl(OMIOStream& s)
{
  return s.endl();
}

OMIOStream& indent(OMIOStream& s)
{
  return s.indent();
}

OMIOStream& outdent(OMIOStream& s)
{
  return s.outdent();
}

OMIOStream& flush(OMIOStream& s)
{
  return s.flush();
}

OMIOStream& showbase(OMIOStream& s)
{
  return s.showbase();
}

OMIOStream& dec(OMIOStream& s)
{
  return s.dec();
}

OMIOStream& hex(OMIOStream& s)
{
  return s.hex();
}

OMIOStream& uppercase(OMIOStream& s)
{
  return s.uppercase();
}

OMIOStream& lowercase(OMIOStream& s)
{
  return s.lowercase();
}

OMIOStream& set_fill(OMIOStream& s, int c)
{
  return s.setfill(c);
}

OMIOStream& set_w(OMIOStream& s, int n)
{
  return s.setw(n);
}

static void print(strstream& s, const OMObjectIdentification& id)
{
  OMByte* bp = (OMByte *)&id.Data4;
  OMByte pb1 = *bp++;
  OMByte pb2 = *bp++;

  s << "{";
  s.width(8);
  s << id.Data1 << "-";
  s.width(4);
  s << id.Data2 << "-";
  s.width(4);
  s << id.Data3 << "-";
  s.width(2);
  s << (int)pb1;
  s.width(2);
  s << (int)pb2 << "-";
  for (int i = 2; i <= 7; i++) {
    s.width(2);
    s << (int)id.Data4[i];
  }
  s << "}";
}

static void print(strstream& s, const OMMaterialIdentification& id)
{
  s << "{";
  for (size_t i = 0; i < sizeof(id.SMPTELabel); i++) {
    s.width(2);
    s << (int)id.SMPTELabel[i];
  }
  s << "-";
  s.width(2);
  s << (int)id.length << "-";
  s.width(2);
  s << (int)id.instanceHigh << "-";
  s.width(2);
  s << (int)id.instanceMid << "-";
  s.width(2);
  s << (int)id.instanceLow << "-";
  print(s, id.material);
  s << "}";
}

static void format(strstream& s,
                   bool& showBase,
                   size_t base,
                   bool uppercase,
                   char fill,
                   int& width)
{
  s.setf(ios::internal);
  if (fill != 0) {
    s.fill(fill);
  }
  if (width != 0) {
    s.width(width);
    width = 0;
  }
  if (base != 10) {
    if (uppercase) {
      s.setf(ios::uppercase);
    }
    if (showBase) {
      s.setf(ios::showbase);
      showBase = 0;
    }
    s.setf(ios::hex,ios::basefield);
  }
}

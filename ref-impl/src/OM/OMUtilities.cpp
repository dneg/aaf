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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMUtilities

#include "OMUtilities.h"

#include "OMAssertions.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

OMByteOrder hostByteOrder(void)
{
  TRACE("hostByteOrder");

  OMInt16 word = 0x1234;
  OMInt8  byte = *((OMInt8*)&word);
  OMByteOrder result;

  ASSERT("Valid byte order", ((byte == 0x12) || (byte == 0x34)));

  if (byte == 0x12) {
    result = bigEndian;
  } else {
    result = littleEndian;
  }
  return result;
}

size_t lengthOfWideString(const wchar_t* string)
{
  TRACE("lengthOfWideString");

  PRECONDITION("Valid string", validWideString(string));
  const wchar_t* p = string;
  size_t length = 0;
  while (*p != 0) {
    ++length;
    ++p;
  }
  return length;
}

wchar_t* copyWideString(wchar_t* destination,
                        const wchar_t* source,
                        const size_t length)
{
  TRACE("lengthOfWideString");

  PRECONDITION("Valid destination", destination != 0);
  PRECONDITION("Valid source string", validWideString(source));
  PRECONDITION("Valid length", length > 0);

  wchar_t* d = destination;
  const wchar_t* s = source;
  size_t i = 0;

  for (i = 0; ((i < length) && (*s != 0)); i++) {
    *d++ = *s++;
  }
  for (i = i; i < length; i++) {
    *d++ = 0;
  }
  return destination;
}

wchar_t* concatenateWideString(wchar_t* destination,
                               const wchar_t* source,
                               const size_t length)
{
  TRACE("concatenateWideString");

  PRECONDITION("Valid destination", validWideString(destination));
  PRECONDITION("Valid source", validWideString(source));
  PRECONDITION("Valid length", length > 0); // Different to strncat()

  wchar_t* d = destination;
  while (*d != 0) {
    ++d;
  }

  const wchar_t* s = source;
  for (size_t i = 0; ((i < length) && (*s != 0)); i++) {
    *d++ = *s++;
  }
  *d = 0;
  return destination;
}

wchar_t* saveWideString(const wchar_t* string)
{
  TRACE("saveWideString");

  PRECONDITION("Valid string", validWideString(string));
  size_t length = lengthOfWideString(string) + 1;
  wchar_t* result = new wchar_t[length];
  ASSERT("Valid heap pointer", result != 0);
  copyWideString(result, string, length);
  return result;
}

// if string1 >  string2 then result =  1
// if string1 == string2 then result =  0
// if string1 <  string2 then result = -1
int compareWideString(const wchar_t* string1, const wchar_t* string2)
{
  TRACE("compareWideString");

  PRECONDITION("Valid string", validWideString(string1));
  PRECONDITION("Valid string", validWideString(string2));
  int result = 0;
  const wchar_t* s1 = string1;
  const wchar_t* s2 = string2;
  while (*s1 == *s2) {
    if (*s1 == 0) {
      break;
    }
    ++s1;
    ++s2;
  }

  if ((*s1 == 0) && (*s2 == 0)) {
    result = 0;
  } else if (*s1 > *s2) {
    result = 1;
  } else if (*s1 < *s2) {
    result = -1;
  }
  return result;
}

char* convertWideString(const wchar_t* string)
{
  TRACE("convertWideString");

  PRECONDITION("Valid string", validWideString(string));
  size_t length = lengthOfWideString(string);
  char* result = new char[length + 1];
  ASSERT("Valid heap pointer", result != 0);
  size_t status = wcstombs(result, string, length + 1);
  ASSERT("Successful conversion", status != (size_t)-1);
  return result;
}

wchar_t* findWideCharacter(const wchar_t* string, wchar_t character)
{
  const wchar_t* p = string;
  while(*p != 0) {
    if (*p == character) {
      break;
    }
    p++;
  }
  wchar_t* result;
  if (*p == 0) {
    result = 0;
  } else {
    result = const_cast<wchar_t*>(p);
  }
  return result;
}

size_t countWideCharacter(const wchar_t* string, wchar_t character)
{
  size_t result = 0;
  wchar_t* element = const_cast<wchar_t*>(string);
  wchar_t* end = findWideCharacter(element, character);
  while (end != 0) {
    result = result + 1;
    element = ++end;
    end = findWideCharacter(element, character);
  }
  return result;
}

char* saveString(const char* string)
{
  TRACE("saveString");

  PRECONDITION("Valid string", validString(string));
  size_t length = strlen(string) + 1;
  char* result = new char[length];
  ASSERT("Valid heap pointer", result != 0);
  strcpy(result, string);
  return result;
}

size_t stringSize(OMUInt32 i)
{
  TRACE("stringSize");

  PRECONDITION("Valid integer", i > 0);

  size_t result = 8;
  while (((i & 0xf0000000) >> 28) == 0) {
    ASSERT("Valid result", result != 0);
    result = result - 1;
    i = i << 4;
  }
  return result;
}

void toWideString(OMUInt32 i, wchar_t* result, size_t ANAME(resultSize))
{
  TRACE("toWideString");

  PRECONDITION("Valid integer", i > 0);
  PRECONDITION("Valid result buffer", result != 0);
  PRECONDITION("Output string large enough", resultSize >= stringSize(i));

  // Skip leading zeros
  //
  size_t digits = 8;
  int nibble;
  while ((nibble = ((i & 0xf0000000) >> 28)) == 0) {
    ASSERT("Valid digits", digits > 0);
    digits = digits - 1;
    i = i << 4;
  }

  // Convert remaining digits to hex characters
  //
  wchar_t* hexDigits = L"0123456789abcdef";
  wchar_t* p = result;
  while (digits > 0) {
    ASSERT("Valid nibble", ((nibble >= 0) && (nibble <= 15)));
    *p++ = hexDigits[nibble];
    i = i << 4;
    nibble = ((i & 0xf0000000) >> 28);
    ASSERT("Valid digits", digits > 0);
    digits = digits - 1;
  }
  *p = 0;
}

size_t squeezeWideString(const wchar_t* clearName,
                         size_t clearNameSize,
                         wchar_t* squeezedName,
                         size_t squeezedNameSize)
{
  TRACE("squeezeWideString");

  PRECONDITION("Valid input name", validWideString(clearName));
  PRECONDITION("Valid input name size", clearNameSize > 0);
  PRECONDITION("Valid result buffer", squeezedName != 0);
  PRECONDITION("Valid result buffer size", squeezedNameSize > 0);

  size_t result;
  if (clearNameSize <= squeezedNameSize) {
    // It fits, just copy
    copyWideString(squeezedName, clearName, clearNameSize + 1);
    result = clearNameSize;
  } else {
    // Too big, squeeze it
    wchar_t ch;
    size_t half = squeezedNameSize / 2;
    for (size_t i = 0; i < squeezedNameSize; i++) {
      if (i < half) {         // Copy from bottom half
        ch = clearName[i];
      } else if (i == half) { // Show characters omitted
        ch = L'-';
      } else {                // Copy from top half
        ch = clearName[clearNameSize - (squeezedNameSize - i)];
      }
      ASSERT("Valid character", ch != 0);
      squeezedName[i] = ch;
    }
    squeezedName[squeezedNameSize] = 0;
    result = squeezedNameSize;
  }
  return result;
}

size_t lengthOfOMString(const OMCharacter* string)
{
  const OMCharacter* p = string;
  size_t length = 0;
  while (*p != 0) {
    ++length;
    ++p;
  }
  return length;
}

  // Manipulation of property paths (eventually these will be
  // intantiations of templates shared with the wide character
  // functions).

bool validPropertyPath(const OMPropertyId* path)
{
  TRACE("validPropertyPath");

  bool result;
  if (path == 0) {
    result = false;
  } else {
    if (lengthOfPropertyPath(path) == 0) {
      result = false;
    } else {
      result = true;
    }
  }
  return result;
}

size_t lengthOfPropertyPath(const OMPropertyId* path)
{
  TRACE("lengthOfPropertyPath");
  // PRECONDITION("Valid property path", validPropertyPath(path));
  PRECONDITION("Valid property path", path != 0);

  const OMPropertyId* p = path;
  size_t result = 0;
  while (*p != 0) {
    ++result;
    ++p;
  }
  return result;
}

OMPropertyId* savePropertyPath(const OMPropertyId* path)
{
  TRACE("savePropertyPath");
  PRECONDITION("Valid property path", validPropertyPath(path));

  size_t length = lengthOfPropertyPath(path) + 1;
  OMPropertyId* result = new OMPropertyId[length];
  ASSERT("Valid heap pointer", result != 0);
  for (size_t i = 0; i < length; i++) {
    result[i] = path[i];
  }

  return result;
}

int comparePropertyPath(const OMPropertyId* path1, const OMPropertyId* path2)
{
  TRACE("comparePropertyPath");

  PRECONDITION("Valid property path", validPropertyPath(path1));
  PRECONDITION("Valid property path", validPropertyPath(path2));
  int result = 0;
  const OMPropertyId* p1 = path1;
  const OMPropertyId* p2 = path2;
  while (*p1 == *p2) {
    if (*p1 == 0) {
      break;
    }
    ++p1;
    ++p2;
  }

  if ((*p1 == 0) && (*p2 == 0)) {
    result = 0;
  } else if (*p1 > *p2) {
    result = 1;
  } else if (*p1 < *p2) {
    result = -1;
  }
  return result;
}

// _wfopen() is in the W32 API on Windows 95, 98 and ME but with an
// implementation that always fails. By default we use _wfopen() when
// compiled on/for the W32 API, this can be overridden by defining
// NO_W32_WFOPEN.

#if !defined(NO_W32_WFOPEN)
#if defined(_WIN32) || defined(WIN32)
#define W32_WFOPEN
#endif
#endif

// Just like ANSI fopen() except for wchar_t* file names and modes.
//
FILE* wfopen(const wchar_t* fileName, const wchar_t* mode)
{
  TRACE("wfopen");
  ASSERT("Valid file name", fileName != 0);
  ASSERT("Valid mode", mode != 0);

  FILE* result = 0;
#if defined(W32_WFOPEN)
  result = _wfopen(fileName, mode);
#else
  char cFileName[FILENAME_MAX];
  size_t status = wcstombs(cFileName, fileName, FILENAME_MAX);
  ASSERT("Convert succeeded", status != (size_t)-1);

  char cMode[FILENAME_MAX];
  status = wcstombs(cMode, mode, FILENAME_MAX);
  ASSERT("Convert succeeded", status != (size_t)-1);

  result = fopen(cFileName, cMode);
#endif
  return result;
}

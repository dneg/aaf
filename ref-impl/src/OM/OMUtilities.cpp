/***********************************************************************
*
*              Copyright (c) 1998-1999 Avid Technology, Inc.
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

#include "OMUtilities.h"

#include "OMAssertions.h"

#include <iostream.h>
#include <string.h>
#include <stdio.h>

static char programName[FILENAME_MAX] = "Object Manager";

void setProgramName(const char* name)
{
  TRACE("setProgramName");

  PRECONDITION("Valid program name", validString(name));

  size_t size = strlen(name) + 1;
  if (size >= FILENAME_MAX) {
    size = FILENAME_MAX - 1;
  }
  strncpy(programName, name, size);
  programName[size] = '\0';
}

const char* getProgramName(void)
{
  return programName;
}

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

// Same as strlen(), but for wide characters.
//
size_t lengthOfWideString(const wchar_t* string)
{
  const wchar_t* p = string;
  size_t length = 0;
  while (*p != 0) {
    ++length;
    ++p;
  }
  return length;
}

// Same as strncpy(), but for wide characters.
//
wchar_t* copyWideString(wchar_t* destination,
                        const wchar_t* source,
                        const size_t length)
{
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

size_t lengthOfOMWideString(const OMWideCharacter* string)
{
  const OMWideCharacter* p = string;
  size_t length = 0;
  while (*p != 0) {
    ++length;
    ++p;
  }
  return length;
}

#include "OMUtilities.h"

#include "OMAssertions.h"

#include <iostream.h>
#include <string.h>
#include <stdio.h>

char programName[FILENAME_MAX] = "Object Manager";

void setProgramName(const char* name)
{
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

size_t wideStringLength(const wchar_t* string)
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
wchar_t* wideStringCopy(wchar_t* destination,
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

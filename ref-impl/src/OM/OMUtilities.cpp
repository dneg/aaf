#include <iostream.h>

#include "OMUtilities.h"

#include "OMAssertions.h"

const char* programName = "Unknown program";

void setProgramName(const char* name)
{
  programName = name;
}

const char* getProgramName(void)
{
  return programName;
}

ByteOrder hostByteOrder(void)
{
  OMInt16 word = 0x1234;
  OMInt8  byte = *((OMInt8*)&word);
  ByteOrder result;

  ASSERT("Valid byte order", ((byte == 0x12) || (byte == 0x34)));

  if (byte == 0x12) {
    result = bigEndian;
  } else {
    result = littleEndian;
  }
  return result;
}


#include "OMUtilities.h"

#include "OMAssertions.h"

#include <iostream.h>
#include <assert.h>
#include <string.h>

#if !defined(OMSTANDALONE)
#include "AAFTypes.h"
#endif

char* programName = 0;

void setProgramName(const char* name)
{
  PRECONDITION("Valid program name", validString(name));

  size_t size = strlen(name) + 1;
  delete [] programName;
  programName = new char[size];
  strncpy(programName, name, size);
}

const char* getProgramName(void)
{
  if (programName == 0) {
    setProgramName("Unknown Program");
  }
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


#if !defined(OMSTANDALONE)
void OMUwc2sb (char * pbString,
			   const aafWChar * pwString)
{
  assert (pwString);
  assert (pbString);
  while (*pwString)
	{
	  // first make sure this char will fit
	  assert (0 == (*pwString & ~0xFF));
	  *pbString++ = (char) *pwString++;
	}
  *pbString = (char) *pwString;
}

void OMUsb2wc (aafWChar * pwString,
			   const char * pbString)
{
  assert (pwString);
  assert (pbString);
  while (*pbString)
	{
	  *pwString++ = (char) *pbString++;
	}
  *pwString = (char) *pbString;
}
#endif

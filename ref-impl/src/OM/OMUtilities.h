#ifndef OMUTILITIES_H
#define OMUTILITIES_H

#include "AAFTypes.h"

#include "OMTypes.h"

void setProgramName(const char* name);

const char* getProgramName(void);

ByteOrder hostByteOrder(void);

//
// Converts wide character string (pwString) to a single-byte string
// (pbString).  It is the caller's responsibility to allocate the
// destination memory.
//
void OMUwc2sb (char * pbString,
			   const aafWChar * pwString);

//
// Converts single-byte string (pbString) to a wide character string
// (pwString).  It is the caller's responsibility to allocate the
// destination memory.
//
void OMUsb2wc (aafWChar * pwString,
			   const char * pbString);

#endif

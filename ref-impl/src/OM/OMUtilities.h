#ifndef OMUTILITIES_H
#define OMUTILITIES_H

#if !defined(OMSTANDALONE)
#include "AAFTypes.h"
#endif

#include "OMTypes.h"

void setProgramName(const char* name);

const char* getProgramName(void);

ByteOrder hostByteOrder(void);

#if !defined(OMSTANDALONE)
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

#endif

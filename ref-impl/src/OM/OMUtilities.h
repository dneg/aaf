#ifndef OMUTILITIES_H
#define OMUTILITIES_H

#include "OMDataTypes.h"

#include <stddef.h>

void setProgramName(const char* name);

const char* getProgramName(void);

OMByteOrder hostByteOrder(void);

size_t wideStringLength(const wchar_t* string);

wchar_t* wideStringCopy(wchar_t* destination,
                        const wchar_t* source,
                        const size_t length);

#endif

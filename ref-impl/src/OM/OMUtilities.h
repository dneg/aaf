#ifndef OMUTILITIES_H
#define OMUTILITIES_H

#include "OMDataTypes.h"

#include <stddef.h>

void setProgramName(const char* name);

const char* getProgramName(void);

OMByteOrder hostByteOrder(void);

size_t lengthOfWideString(const wchar_t* string);

wchar_t* copyWideString(wchar_t* destination,
                        const wchar_t* source,
                        const size_t length);

size_t lengthOfOMWideString(const OMWideCharacter* string);

#endif

#ifndef OMUTILITIES_H
#define OMUTILITIES_H

#include <stdlib.h>

#include "OMPortability.h"

void setProgramName(const char* name);

void convert(wchar_t* wcName, size_t length, const char* name);

void convert(char* cName, size_t length, wchar_t* name);

void formatError(DWORD errorCode);

void printError(const char* prefix, const char* type);

void printName(const char* name);

int check(HRESULT resultCode);

int checkFile(HRESULT resultCode, const char* fileName);

int checkStream(HRESULT resultCode, const char* streamName);

int checkStorage(HRESULT resultCode, const char* storageName);

#endif

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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
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
#include "OMPortability.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

OMByteOrder hostByteOrder(void)
{
  TRACE("hostByteOrder");

  OMUInt32 longWord = 0x12345678;
  OMUInt8  byte = *((OMInt8*)&longWord);
  OMByteOrder result;

  ASSERT("Valid byte order", ((byte == 0x12) || (byte == 0x78)));

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
                        size_t length)
{
  TRACE("copyWideString");

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

wchar_t* copyWideString(wchar_t* destination,
                        const wchar_t* source)
{
  TRACE("copyWideString");

  PRECONDITION("Valid destination", destination != 0);
  PRECONDITION("Valid source string", validWideString(source));

  wchar_t* d = destination;
  const wchar_t* s = source;

  while(*s != 0) {
    *d++ = *s++;
  }
  *d = 0;
  return destination;
}

wchar_t* concatenateWideString(wchar_t* destination,
                               const wchar_t* source,
                               size_t length)
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

wchar_t* concatenateWideString(wchar_t* destination,
                               const wchar_t* source)
{
  TRACE("concatenateWideString");

  PRECONDITION("Valid destination", validWideString(destination));
  PRECONDITION("Valid source", validWideString(source));

  wchar_t* d = destination;
  while (*d != 0) {
    ++d;
  }

  const wchar_t* s = source;
  while(*s != 0) {
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
int compareWideString(const wchar_t* string1,
                      const wchar_t* string2,
                      size_t length)
{
  TRACE("compareWideString");

  PRECONDITION("Valid string", validWideString(string1));
  PRECONDITION("Valid string", validWideString(string2));
  int result = 0;
  const wchar_t* s1 = string1;
  const wchar_t* s2 = string2;
  for (size_t i = 0; i < length; i++) {
    if (*s1 != *s2) {
      if (*s1 > *s2) {
        result = 1;
      } else {
        result = -1;
      } 
      break;
    } else if (*s1 == 0) {
      result = 0;
      break;
    }
    ++s1;
    ++s2;
  }
  return result;
}

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

void convertWideStringToString(char* /* result */,
                               const wchar_t* /* string */,
                               size_t /* resultSize */)
{
  TRACE("convertWideStringToString");
  ASSERT("Unimplemented code not reached", false);
}

void convertStringToWideString(wchar_t* result,
                               const char* string,
                               size_t resultSize)
{
  TRACE("convertStringToWideString");

  PRECONDITION("Valid string", string != 0);
  size_t length = strlen(string);
  if (length > (resultSize - 1)) {
    length = resultSize - 1;
  }
  size_t status  = mbstowcs(result, string, length);
  ASSERT("Successful conversion", status != (size_t)-1);
  result[length] = 0;
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

wchar_t* convertString(const char* string)
{
  TRACE("convertString");

  PRECONDITION("Valid string", validString(string));
  size_t length = strlen(string);
  wchar_t* result = new wchar_t[length + 1];
  ASSERT("Valid heap pointer", result != 0);
  size_t status = mbstowcs(result, string, length + 1);
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

void copyCToPString(unsigned char* pString, size_t size, const char* string)
{
  size_t maxLength = size - 1;
  size_t length = strlen(string);
  if (length > maxLength) {
    length = maxLength;
  }
  memcpy(&pString[1], string, length);
  pString[0] = length;
}

void copyPToCString(char* string, size_t size, const unsigned char* pString)
{
  size_t maxLength = size - 1;
  size_t length = pString[0];
  if (length > maxLength) {
    length = maxLength;
  }
  memcpy(string, &pString[1], length);
  string[length] = 0;
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

bool validString(const char* string)
{
  bool result = true;

  if (string == 0) {
    // bad pointer
    result = false;
  } else {
    size_t length = strlen(string);
    if (length == 0) {
      // bad length
      result = false;
    } else {
      for (size_t i = 0; i < length; i++) {
        int c = (unsigned char)string[i];
        if (!isprint(c)) {
          // bad character
          result = false;
          break;
        }
      }
    }
  }

  return result;
}

bool validWideString(const wchar_t* string)
{
  return (string != 0) /* && (lengthOfWideString(string) > 0) */;
}

static const char digits[] = "0123456789ABCDEF"; 

void toString(const OMUInt8&i, char* is)
{
  TRACE("toString");
  PRECONDITION("Valid buffer", is != 0);

  char* op = is;
  *op++ = digits[ (i & 0xf0) >> 4 ];
  *op++ = digits[ (i & 0x0f) ];
  *op++ = '\0';
}

void toString(const OMUInt16&i, char* is)
{
  TRACE("toString");
  PRECONDITION("Valid buffer", is != 0);

  char* op = is;
  OMUInt8 b0 = (OMUInt8)((i & 0xff00) >> 8);
  OMUInt8 b1 = (OMUInt8)((i & 0x00ff));
  toString(b0, op); op = op + 2;
  toString(b1, op); op = op + 2;
}

void toString(const OMUInt32&i, char* is)
{
  TRACE("toString");
  PRECONDITION("Valid buffer", is != 0);

  char* op = is;
  OMUInt16 w0 = (OMUInt16)((i & 0xffff0000) >> 16);
  OMUInt16 w1 = (OMUInt16)((i & 0x0000ffff));
  toString(w0, op); op = op + 4;
  toString(w1, op); op = op + 4;
}

void toString(const OMObjectIdentification& id, char* idString)
{
  TRACE("toString");
  PRECONDITION("Valid buffer", idString != 0);

  char* op = idString;

  *op++ = '{'; 

  OMUInt32* lp = (OMUInt32 *)&id;

  // long word
  toString(*lp++, op); op = op + 8;

  OMUInt16* wp = (OMUInt16 *)lp;

  // word
  *op++ = '-';
  toString(*wp++, op); op = op + 4;
  // word
  *op++ = '-';
  toString(*wp++, op); op = op + 4;

  OMByte* bp = (OMByte *)wp;

  // pseudo-word
  *op++ = '-';
  toString(*bp++, op); op = op + 2;
  toString(*bp++, op); op = op + 2;

  // bytes
  *op++ = '-';
  toString(*bp++, op); op = op + 2;
  toString(*bp++, op); op = op + 2;
  toString(*bp++, op); op = op + 2;
  toString(*bp++, op); op = op + 2;
  toString(*bp++, op); op = op + 2;
  toString(*bp++, op); op = op + 2;

  *op++ = '}'; 
  *op = '\0'; 
}

void fromString(OMUInt8& i, const char* is)
{
  OMByte b = *is++;
  b = b - 0x30;
  if (b > 9) b = b - 7;
  i = b;
  i = i << 4;
  b = *is;
  b = b - 0x30;
  if (b > 9) b = b - 7;
  i = i + b;
}

void fromString(OMUInt16& i, const char* is)
{
  const char* p = is;
  OMUInt8 b;
  fromString(b, p); p = p + 2;
  i = b;
  i = i << 8;
  fromString(b, p);
  i = i + b;
}

void fromString(OMUInt32& i, const char* is)
{
  const char* p = is;
  OMUInt16 w;
  fromString(w, p); p = p + 4;
  i = w;
  i = i << 16;
  fromString(w, p);
  i = i + w;
}

void fromString(OMObjectIdentification& id, const char* idString)
{
  TRACE("fromString");
  PRECONDITION("Valid id string", isValidObjectIdentificationString(idString));

  const char* p = idString;
  ++p; // {
  // long word
  fromString(id.Data1, p); p = p + 8;
  ++p; // -
  // word
  fromString(id.Data2, p); p = p + 4;
  ++p; // -
  // word
  fromString(id.Data3, p); p = p + 4;
  ++p; // -
  // pseudo-word
  OMUInt16 x;
  fromString(x, p); p = p + 4;
  id.Data4[0] = (OMUInt8)((x & 0xff00) >> 8);
  id.Data4[1] = (OMUInt8)(x & 0x00ff);
  ++p; // -
  // bytes
  fromString(id.Data4[2], p); p = p + 2;
  fromString(id.Data4[3], p); p = p + 2;
  fromString(id.Data4[4], p); p = p + 2;
  fromString(id.Data4[5], p); p = p + 2;
  fromString(id.Data4[6], p); p = p + 2;
  fromString(id.Data4[7], p); p = p + 2;
  ++p; // }
  POSTCONDITION("End of string", *p == 0);
}

bool isValidObjectIdentificationString(const char* idString)
{
  TRACE("isValidObjectIdentificationString");
  PRECONDITION("Non-void string", idString != 0);

  bool result = true;
  if (!validString(idString)) {
    result = false;
  } else if (strlen(idString) != OMObjectIdentificationStringBufferSize - 1) {
    result = false;
  }

  return result;
}

void checkTypes(void)
{
  TRACE("checkTypes");

  ASSERT("Correct size for OMInt8",   sizeof(OMInt8)   == 1);
  ASSERT("Correct size for OMInt16",  sizeof(OMInt16)  == 2);
  ASSERT("Correct size for OMInt32",  sizeof(OMInt32)  == 4);
  ASSERT("Correct size for OMInt64",  sizeof(OMInt64)  == 8);

  ASSERT("Correct size for OMUInt8",  sizeof(OMUInt8)  == 1);
  ASSERT("Correct size for OMUInt16", sizeof(OMUInt16) == 2);
  ASSERT("Correct size for OMUInt32", sizeof(OMUInt32) == 4);
  ASSERT("Correct size for OMUInt64", sizeof(OMUInt64) == 8);
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

#if defined(OM_OS_WINDOWS)

#include <windows.h>

typedef enum WindowsKind {
  wkError,       // error/unknown
  wk3_1,         // Win32s
  wkConsumer,    // Windows 95/98/Me
  wkProfessional // Windows NT/2000/XP
} WindowsKind;

static WindowsKind getWindowsKind(void)
{
  WindowsKind result = wkError;

  DWORD version = GetVersion();
  BYTE majorVersion = (BYTE)(version        & 0x000000ff);
  BYTE minorVersion = (BYTE)((version >> 8) & 0x000000ff);
  if (version < 0x80000000) {
    result = wkProfessional;
  } else if (majorVersion < 4) {
    result = wk3_1;
  } else {
    result = wkConsumer;
  }
  return result;
}

#endif

// _wfopen() and _wremove() are in the W32 API on Windows 95, 98 and
// ME but with an implementation that always fails. So we only call
// them if getWindowsKind() == wkProfessional.

// Just like ANSI fopen() except for wchar_t* file names and modes.
//
FILE* wfopen(const wchar_t* fileName, const wchar_t* mode)
{
  TRACE("wfopen");
  ASSERT("Valid file name", fileName != 0);
  ASSERT("Valid mode", mode != 0);

  FILE* result = 0;
#if defined(OM_OS_WINDOWS)
  if (getWindowsKind() == wkProfessional) {
    result = _wfopen(fileName, mode);
  } else {
#endif
    char cFileName[FILENAME_MAX];
    size_t status = wcstombs(cFileName, fileName, FILENAME_MAX);
    ASSERT("Convert succeeded", status != (size_t)-1);

    char cMode[FILENAME_MAX];
    status = wcstombs(cMode, mode, FILENAME_MAX);
    ASSERT("Convert succeeded", status != (size_t)-1);

    result = fopen(cFileName, cMode);
#if defined(OM_OS_WINDOWS)
  }
#endif
  return result;
}

// Just like ANSI remove() except for wchar_t* file names.
int wremove(const wchar_t* fileName)
{
  TRACE("wremove");
  ASSERT("Valid file name", fileName != 0);

  int result = 0;
#if defined(OM_OS_WINDOWS)
  if (getWindowsKind() == wkProfessional) {
    result = _wremove(fileName);
  } else {
#endif
    char cFileName[FILENAME_MAX];
    size_t status = wcstombs(cFileName, fileName, FILENAME_MAX);
    ASSERT("Convert succeeded", status != (size_t)-1);

    result = remove(cFileName);
#if defined(OM_OS_WINDOWS)
  }
#endif
  return result;
}

#if defined(OM_OS_WINDOWS)
#define OM_USE_COM_CREATEUUID
#elif defined(OM_OS_MACOS)
#define OM_USE_OM_CREATEUUID
#elif defined(OM_OS_UNIX) && defined(__linux__)
#define OM_USE_LIBUUID_CREATEUUID
#elif defined(OM_OS_UNIX) && defined(__MACH__)
#define OM_USE_CF_CREATEUUID
#elif defined(OM_OS_UNIX)
#define OM_USE_OM_CREATEUUID
#elif defined(OM_OS_MACOSX)
#define OM_USE_OM_CREATEUUID
#endif

#if defined(OM_USE_COM_CREATEUUID)

#include <objbase.h>

OMUniqueObjectIdentification createUniqueIdentifier(void)
{
  OMUniqueObjectIdentification result = {0};
  GUID u;
  CoCreateGuid(&u);
  memcpy(&result, &u, sizeof(OMUniqueObjectIdentification));
  return result;
}

#elif defined(OM_USE_OM_CREATEUUID)

#include <time.h>

OMUniqueObjectIdentification createUniqueIdentifier(void)
{
  OMUniqueObjectIdentification result = {0};
  // {FFFFFFFF-3B78-47ec-98DD-68AC60D2539E}
  static OMUniqueObjectIdentification id =
    {0xffffffff, 0x3b78, 0x47ec,
    {0x98, 0xdd, 0x68, 0xac, 0x60, 0xd2, 0x53, 0x9e}};

  if (id.Data1 == 0xffffffff) {
    OMUInt32 ticks = clock();
    time_t timer = time(0);
    id.Data1 += timer + ticks;
  }
  ++id.Data1;
  result = id;
  return result;
}

#elif defined(OM_USE_CF_CREATEUUID)

#include <CoreServices/CoreServices.h>

OMUniqueObjectIdentification createUniqueIdentifier(void)
{
  OMUniqueObjectIdentification result = {0};
  CFUUIDRef ur = CFUUIDCreate(kCFAllocatorDefault);
  CFUUIDBytes ub = CFUUIDGetUUIDBytes(ur);
  memcpy(&result, &ub, sizeof(OMUniqueObjectIdentification));
  CFRelease(ur);
  return result;
}

#elif defined(OM_USE_LIBUUID_CREATEUUID)

#include <uuid/uuid.h>

OMUniqueObjectIdentification createUniqueIdentifier(void)
{
  OMUniqueObjectIdentification result = {0};
  uuid_t u;
  uuid_generate(u);
  memcpy(&result, &u, sizeof(OMUniqueObjectIdentification));
  return result;
}

#endif


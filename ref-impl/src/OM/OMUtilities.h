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
#ifndef OMUTILITIES_H
#define OMUTILITIES_H

#include "OMDataTypes.h"

#include <stdio.h>
#include <stddef.h>

// @module OMUtilities | Utility functions including error handling,
//         obtaining information about the host computer, wide character
//         string manipulation, property path manipulation and accessing
//         disk files with wide character names.
//   @mauthor Tim Bingham | tjb | Avid Technology, Inc.

  // @func Get the byte order used on the host computer.
  //   @rdesc The host byte order.
OMByteOrder hostByteOrder(void);

  // ISO C Amendment 1 equivalents.
  //
  // wcscmp()  <-> compareWideString(const wchar_t*, const wchar_t*)
  // wcscpy()  <-> copyWideString(wchar_t*, const wchar_t*)
  // wcscat()  <-> concatenateWideString(wchar_t*, const wchar_t*)
  //
  // wcsncmp() <-> compareWideString(const wchar_t*, const wchar_t*, size_t)
  // wcsncpy() <-> copyWideString(wchar_t*, const wchar_t*, size_t)
  // wcsncat() <-> concatenateWideString(wchar_t*, const wchar_t*, size_t)
  //
  // wcslen()  <-> lengthOfWideString()
  // wcschr()  <-> findWideCharacter()
  //
  // none      <-> saveWideString()
  // none      <-> countWideCharacter()

  // @func The length of the wide character string <p string> in characters
  //       excluding the terminating null character. Same as strlen(),
  //       but for wide characters.
  //   @parm The wide character string.
  //   @rdesc The wide character string length in characters.
size_t lengthOfWideString(const wchar_t* string);

  // @func Copy a wide character string. Same as strncpy(), but for wide
  //       characters. Exactly <p length> characters are always written to
  //       the <p destination> buffer. The <p destination> buffer must be
  //       at least <p length> characters in size. If the buffer is too
  //       small this error is not detected. If <p length> is greater than
  //       the length of <p source> then then <p destination> is padded
  //       with nulls and <p destination> will be properly null terminated.
  //       If <p length> is less than the length of <p source> only length
  //       characters will be copied and <p destination> will not be properly
  //       null terminated. If the <p source> and <p destination> wide
  //       character strings overlap this error is not detected,.
  //   @parm The destination buffer.
  //   @parm The wide character string to copy.
  //   @parm The number of characters to copy.
  //   @rdesc The resulting wide character string.
wchar_t* copyWideString(wchar_t* destination,
                        const wchar_t* source,
                        size_t length);

wchar_t* copyWideString(wchar_t* destination,
                        const wchar_t* source);

  // @func Concatenate wide character strings. Same as strncat(), but for wide
  //       characters. Append up to <p length> characters from <p source>
  //       to the end of <p destination>. If the null character that terminates
  //       <p source> is encountered before <p length> characters have been
  //       copied, then the null character is copied but no more. If no null
  //       character appears among the first <p length> characters of
  //       <p source>, then the first <p length> characters are copied and a
  //       null character is supplied to terminate <p destination>, that is,
  //       <p length> + 1 characters in all are written.  
  //   @parm The destination buffer.
  //   @parm The wide character string to copy.
  //   @parm The number of characters to copy.
  //   @rdesc The resulting wide character string.
wchar_t* concatenateWideString(wchar_t* destination,
                               const wchar_t* source,
                               size_t length);

wchar_t* concatenateWideString(wchar_t* destination,
                               const wchar_t* source);

  // @func Save a wide character string. Same as saveString() but for wide
  //       characters.
  //   @parm The wide character string to save.
  //   @rdesc The saved wide character string.
wchar_t* saveWideString(const wchar_t* string);

int compareWideString(const wchar_t* string1,
                      const wchar_t* string2,
                      size_t length);

int compareWideString(const wchar_t* string1, const wchar_t* string2);

  // @func Convert a wide character string to a character string. Similar to
  //       wcstombs() except that the result is always a properly terminated,
  //       but possibly truncated, character string.
  //   @parm The resulting character string.
  //   @parm The wide character string to convert.
  //   @parm The size, in characters, of the result buffer.
void convertWideStringToString(char* result,
                               const wchar_t* string,
                               size_t resultSize);

  // @func Convert a character string to a wide character string. Similar to
  //       mbstowcs() except that the result is always a properly terminated,
  //       but possibly truncated, character string.
  //   @parm The resulting wide character string.
  //   @parm The character string to convert.
  //   @parm The size, in wide characters, of the result buffer.
void convertStringToWideString(wchar_t* result,
                               const char* string,
                               size_t resultSize);

char* convertWideString(const wchar_t* string);

wchar_t* convertString(const char* string);

wchar_t* findWideCharacter(const wchar_t* string, wchar_t character);

size_t countWideCharacter(const wchar_t* string, wchar_t character);

  // C strings <-> Pascal strings

// Convert C string to Pascal string, truncate if too long.
//
void copyCToPString(unsigned char* pString, size_t size, const char* string);

// Convert Pascal string to C string, truncate if too long.
//
void copyPToCString(char* string, size_t size, const unsigned char* pString);

  // @func The number of characters needed to represent <p i> as a
  //       hexadecimal string without leading zeros.
  //   @parm A non-zero unsigned integer.
  //   @rdesc The number of characters.
size_t stringSize(OMUInt32 i);

  // @func Convert <p i> hexadecimal string without leading zeros.
  //   @parm A non-zero unsigned integer.
  //   @parm The resulting string.
  //   @parm The size, in characters, of the result buffer.
void toWideString(OMUInt32 i, wchar_t* result, size_t resultSize);

   // @func Squeeze a string to fit within a given size, omitting characters
   //       from the center if necessary.
  //   @parm The string to be squeezed.
  //   @parm The length of the string to be squeezed.
  //   @parm The resulting string.
  //   @parm The size, in characters, of the result buffer.
  //   @rdesc The size, in characters, of the resulting string.
size_t squeezeWideString(const wchar_t* clearName,
                         size_t clearNameSize,
                         wchar_t* squeezedName,
                         size_t squeezedNameSize);

  // @func Save a character string.
  //   @parm The character string to save.
  //   @rdesc The saved character string.
char* saveString(const char* string);

  // @func Is the given string valid ? Use <f validString> in
  //       expressions passed to the assertion macros
  //       <f PRECONDITION>, <f POSTCONDITION> and <f ASSERT>.
  //   @parm The string to check for validity.
  //   @rdesc True if the string is valid, false otherwise.
bool validString(const char* string);

  // @func Is the given wchar_t string valid ? Use <f validWideString> in
  //       expressions passed to the assertion macros
  //       <f PRECONDITION>, <f POSTCONDITION> and <f ASSERT>.
  //   @parm The wchar_t string to check for validity.
  //   @rdesc True if the wchar_t string is valid, false otherwise.
bool validWideString(const wchar_t* string);

  // Conversions to strings

  // Convert an OMUInt8 to a hexadecimal string like this -
  // XX
void toString(const OMUInt8&i, char* is);

  // Convert an OMUInt16 to a hexadecimal string like this -
  // XXXX
void toString(const OMUInt16&i, char* is);

  // Convert an OMUInt32 to a hexadecimal string like this -
  // XXXXXXXX
void toString(const OMUInt32&i, char* is);

  // Convert an OMObjectIdentification to a string like this -
  //{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
void toString(const OMObjectIdentification& id, char* idString);

  // Caller allocated buffers must be at least this big -
  //
const size_t OMUInt8StringBufferSize = 3;
const size_t OMUInt16StringBufferSize = 5;
const size_t OMUInt32StringBufferSize = 9;
const size_t OMObjectIdentificationStringBufferSize = 39;

  // Conversions from strings

  // Parse an OMUInt8 from a hexadecimal string like this -
  // XX
void fromString(OMUInt8& i, const char* is);

  // Parse an OMUInt16 from a hexadecimal string like this -
  // XXXX
void fromString(OMUInt16& i, const char* is);

  // Parse an OMUInt32 from a hexadecimal string like this -
  // XXXXXXXX
void fromString(OMUInt32& i, const char* is);

  // Parse an OMObjectIdentification from a string like this -
  //{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
void fromString(OMObjectIdentification& id, const char* idString);

  // Does idString represent a vaild OMObjectIdentification ?
bool isValidObjectIdentificationString(const char* idString);

  // Check of types

void checkTypes(void);

  // Manipulation of property paths

bool validPropertyPath(const OMPropertyId* path);

size_t lengthOfPropertyPath(const OMPropertyId* path);

OMPropertyId* savePropertyPath(const OMPropertyId* path);

int comparePropertyPath(const OMPropertyId* path1, const OMPropertyId* path2);

  // @func Open a named file. Just like ANSI fopen() except for
  //       wchar_t* file names and modes.
  //   @parm The file name.
  //   @parm The mode.
  //   @rdesc An ANSI FILE* 
FILE* wfopen(const wchar_t* fileName, const wchar_t* mode);

  // @func Remove the named file. Just like ANSI remove() except for
  //       wchar_t* file names.
  //   @parm The file name.
  //   @rdesc 0 if the file is successfuly deleted -1 otherwise. 
int wremove(const wchar_t* fileName);

// Generating unique identifiers

OMUniqueObjectIdentification createUniqueIdentifier(void);

#endif

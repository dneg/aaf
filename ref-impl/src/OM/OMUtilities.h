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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
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

char* convertWideString(const wchar_t* string);

wchar_t* findWideCharacter(const wchar_t* string, wchar_t character);

size_t countWideCharacter(const wchar_t* string, wchar_t character);

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

size_t lengthOfOMString(const OMCharacter* string);

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

#endif

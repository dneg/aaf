/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

// @doc OMEXTERNAL
#ifndef OMUTILITIES_H
#define OMUTILITIES_H

#include "OMDataTypes.h"

#include <stddef.h>

  // @func Set the name of the program to be use in error messages.
  //   @parm The program name.
void setProgramName(const char* name);

  // @func Get the name of the program to be used in error messages.
  //   @rdesc The program name.
const char* getProgramName(void);

  // @func Get the byte order used on the host computer.
  //   @rdesc The host byte order.
OMByteOrder hostByteOrder(void);

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
                        const size_t length);

  // @func Save a wide character string. Same as saveString() but for wide
  //       characters.
  //   @parm The wide character string to save.
  //   @rdesc The saved wide character string.
wchar_t* saveWideString(const wchar_t* string);

int compareWideString(const wchar_t* string1, const wchar_t* string2);

char* convertWideString(const wchar_t* string);

  // @func Save a character string.
  //   @parm The character string to save.
  //   @rdesc The saved character string.
char* saveString(const char* string);

size_t lengthOfOMString(const OMCharacter* string);

#endif

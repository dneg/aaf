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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMAssertions

#include "OMAssertions.h"

class OMAssertionViolation {
public:

  OMAssertionViolation() {}

  ~OMAssertionViolation() {}

  OMAssertionViolation(const OMAssertionViolation&) {}

};

#if defined (OM_ENABLE_DEBUG)

#include <ctype.h>

#include "OMOStream.h"
#include "OMUtilities.h"

#if defined(OM_ENABLE_STACK_TRACE)
#include "OMStackTrace.h"
#endif


void reportAssertionViolation(char* assertionKind,
                              char* assertionName,
                              char* expressionString,
                              char* routineName,
                              char* fileName,
                              OMUInt32 lineNumber)
{
  omlog << assertionKind
        << " \"" << assertionName << "\" violated in routine \""
        << routineName  << "\"." << endl;
  omlog << "The violation occurred at line " << lineNumber
        << " in file \"" << fileName << "\"." << endl;
  omlog << "The condition \"" << expressionString << "\" was false." << endl;

#if defined(OM_ENABLE_STACK_TRACE)
  printStackTrace(omlog);
#endif

#if defined(OM_EXIT_ON_ASSERT)
  exit(EXIT_FAILURE);
#elif defined(OM_ABORT_ON_ASSERT)
  abort();
#elif defined(OM_CONTINUE_ON_ASSERT)
  // Continue execution.
#else
  throw OMAssertionViolation();
#endif
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

bool validOMString(const OMCharacter* string)
{
  return (string != 0) /* && (lengthOfOMString(string) > 0) */;
}

#if defined (OM_ENABLE_TRACE)

void trace(const char* routineName)
{
  omlog << "Enter \"" << routineName << "\"." << endl;
}

#endif

#if defined (OM_ENABLE_OBSOLETE)

void obsolete(const char* routineName, const char* newRoutineName)
{
  omlog << "The routine \""
        << routineName
        << "\" is obsolete. Please use \""
        << newRoutineName << "\" instead." << endl;
}

#endif

#if defined(OM_ENABLE_DEBUG)

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

#endif

#endif

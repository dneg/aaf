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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMAssertions

#include "OMAssertions.h"

#if defined (OM_ENABLE_DEBUG)

#include <ctype.h>

#include "OMOStream.h"
#include "OMUtilities.h"

#if defined(OM_STACK_TRACE_ON_ASSERT)
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

#if defined(OM_STACK_TRACE_ON_ASSERT)
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

#if defined (OM_TRACE)

void trace(const char* routineName)
{
  omlog << "Enter \"" << routineName << "\"." << endl;
}

#endif

#if defined (OM_OBSOLETE)

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

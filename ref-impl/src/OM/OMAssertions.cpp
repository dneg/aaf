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

#if defined (OM_DEBUG)

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
}

void assertionViolation(char* assertionKind,
                        char* assertionName,
                        char* expressionString,
                        char* routineName,
                        char* fileName,
                        OMUInt32 lineNumber)
{
  reportAssertionViolation(assertionKind,
                           assertionName,
                           expressionString,
                           routineName,
                           fileName,
                           lineNumber);
  handleAssertionViolation();
}

void handleAssertionViolation(void)
{
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

#endif

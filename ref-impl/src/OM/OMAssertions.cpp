//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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


void reportAssertionViolation(const char* assertionKind,
                              const char* assertionName,
                              const char* expressionString,
                              const char* routineName,
                              const char* fileName,
                              OMUInt32 lineNumber)
{
  omlog << assertionKind
        << " \"" << assertionName << "\" violated in routine \""
        << routineName  << "\"." << endl;
  omlog << "The violation occurred at line " << lineNumber
        << " in file \"" << fileName << "\"." << endl;
  omlog << "The condition \"" << expressionString << "\" was false." << endl;
}

void assertionViolation(const char* assertionKind,
                        const char* assertionName,
                        const char* expressionString,
                        const char* routineName,
                        const char* fileName,
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

#ifndef OMASSERTIONS_H
#define OMASSERTIONS_H

#include "OMPortability.h"

#include <stddef.h>

#define SUCCESS (1)
#define FAILURE (2)

#if defined (OM_ENABLE_DEBUG)

static char* currentRoutineName = "unknown";

void reportAssertionFailure(char* kind,
                            char* name,
                            char* expressionString,
                            char* routine,
                            char* fileName,
                            size_t lineNumber);

#if defined(OM_ENABLE_TRACE)

#define TRACE(routine) \
  currentRoutineName = routine; \
  cerr << "Enter \"" << currentRoutineName << "\"." << endl;

#else

#define TRACE(routine) currentRoutineName = routine;

#endif

#define PRECONDITION(name, expression) \
  (expression) \
    ? (void)0  \
    : reportAssertionFailure("Precondition", name, #expression, \
                             currentRoutineName, __FILE__, __LINE__)

#define POSTCONDITION(name, expression) \
  (expression) \
    ? (void)0  \
    : reportAssertionFailure("Postcondition", name, #expression, \
                             currentRoutineName, __FILE__, __LINE__)

#define ASSERT(name, expression) \
  (expression) \
    ? (void)0  \
    : reportAssertionFailure("Assertion",    name, #expression, \
                             currentRoutineName, __FILE__, __LINE__)

#define INVARIANT() checkInvariant();

#define IMPLIES(a, b)          (!(a) || (b))

#else

#define TRACE(name)

#define PRECONDITION(name, expression)

#define POSTCONDITION(name, expression)

#define ASSERT(name, expression)

#define INVARIANT()

#define IMPLIES(a, b)

#endif

bool validString(const char* string);

bool validWideString(const wchar_t* string);

#endif

#include "OMAssertions.h"

class OMAssertionViolation {
public:

  OMAssertionViolation() {}

  ~OMAssertionViolation() {}

  OMAssertionViolation(const OMAssertionViolation&) {}

};

#if defined (OM_ENABLE_DEBUG)

#include <string.h>

#include <iostream.h>
#include <stdlib.h>

#include "OMUtilities.h"

#if defined(OM_ENABLE_STACK_TRACE)
#include "OMStackTrace.h"
#endif


void reportAssertionViolation(char* assertionKind,
                              char* assertionName,
                              char* expressionString,
                              char* routineName,
                              char* fileName,
                              size_t lineNumber)
{
  cerr << assertionKind
       << " \"" << assertionName << "\" violated in routine \""
       << routineName  << "\"." << endl;
  cerr << "The violation occurred at line " << lineNumber
       << " in file \"" << fileName << "\"." << endl;
  cerr << "The condition \"" << expressionString << "\" was false." << endl;

#if defined(OM_ENABLE_STACK_TRACE)
  printStackTrace(cerr);
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
  return ((string != 0) && (strlen(string) > 0));
}

bool validWideString(const wchar_t* string)
{
  return (string != 0) /* && (lengthOfWideString(string) > 0) */;
}

bool validOMWideString(const OMWideCharacter* string)
{
  return (string != 0) /* && (lengthOfOMWideString(string) > 0) */;
}

#if defined (OM_ENABLE_TRACE)

void trace(const char* routineName)
{
  cerr << "Enter \"" << routineName << "\"." << endl;
}

#endif

#endif

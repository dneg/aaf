// @doc OMINTERNAL
#ifndef OMASSERTIONS_H
#define OMASSERTIONS_H

#include "OMPortability.h"

#include <stddef.h>

  // @class OMAssertionViolation | Object Manager assertion
  //        violation. An instance of this opaque class is thrown
  //        when an assertion violation occurs.
class OMAssertionViolation;

#if defined (OM_ENABLE_DEBUG)

#include "OMDataTypes.h"

  // @func Report an assertion violation. Used to implement the
  //       <f PRECONDITION>, <f POSTCONDITION> and <f ASSERT> macros.
  //   @parm The kind of assertion - PRECONDITION, POSTCONDITION or
  //         ASSERT.
  //   @parm The name of the assertion. The assertion name is a
  //         description of the assertion that makes sense from the
  //         point of view of someone reading the source text. The
  //         name comprises a portion of the message that is printed
  //         by this routine.
  //   @parm The expression, as a text string, that was found not to
  //         be true.
  //   @parm The name of the routine in which the assertion violation
  //         occurred.
  //   @parm The name of the source file in which the assertion
  //         violation occurred.
  //   @parm The line number at which the assertion violation
  //         occurred.
void reportAssertionViolation(char* assertionKind,
                              char* assertionName,
                              char* expressionString,
                              char* routineName,
                              char* fileName,
                              size_t lineNumber);

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

  // @func Is the given OMWideCharacter string valid ? Use
  //       <f validOMWideString> in expressions passed to the
  //       assertion macros <f PRECONDITION>, <f POSTCONDITION>
  //       and <f ASSERT>.
  //   @parm The OMWideCharacter string to check for validity.
  //   @rdesc True if the OMWideCharacter string is valid, false otherwise.
bool validOMWideString(const OMWideCharacter* string);

#if defined(OM_ENABLE_TRACE)

  // @func Output routine tracing information.
  //   @parm The name of the routine.
void trace(const char* routineName);

  // @func Print routine tracing information (when enabled with
  //       OM_ENABLE_DEBUG and OM_ENABLE_TRACE). The routine name
  //       provided is used by other assertions.
  //   @parm The routine name. For the most explicit output, names of
  //         member functions should be prefixed with the class name,
  //         as in className::functionName.
#define TRACE(routine) \
  char* currentRoutineName = routine; \
  trace(currentRoutineName);

#else

#define TRACE(routine) \
  char* currentRoutineName; \
  currentRoutineName = routine;

#endif

  // @func Assert (when enabled with OM_ENABLE_DEBUG) that the
  //       precondition described by <p name> and <p expression> is
  //       true. An invocation of this macro must be preceeded by an
  //       invocation of the <f TRACE> macro.
  //   @parm The name of the precondition. The precondition name is a
  //         description of the precondition that makes sense from the
  //         point of view of someone reading the source text. The
  //         name comprises a portion of the message that is printed
  //         if the precondition is violated.
  //   @parm The precondition expression. The expression should be
  //         free of side effects.
#define PRECONDITION(name, expression) \
  (expression) \
    ? (void)0  \
    : reportAssertionViolation("Precondition", name, #expression, \
                               currentRoutineName, __FILE__, __LINE__)

  // @func Assert (when enabled with OM_ENABLE_DEBUG) that the
  //       postcondition described by <p name> and <p expression> is
  //       true. An invocation of this macro must be preceeded by an
  //       invocation of the <f TRACE> macro.
  //   @parm The name of the postcondition. The postcondition name is a
  //         description of the postcondition that makes sense from the
  //         point of view of someone reading the source text. The
  //         name comprises a portion of the message that is printed
  //         if the postcondition is violated.
  //   @parm The postcondition expression. The expression should be
  //         free of side effects.
#define POSTCONDITION(name, expression) \
  (expression) \
    ? (void)0  \
    : reportAssertionViolation("Postcondition", name, #expression, \
                               currentRoutineName, __FILE__, __LINE__)

  // @func Assert (when enabled with OM_ENABLE_DEBUG) that the
  //       condition described by <p name> and <p expression> is
  //       true. An invocation of this macro must be preceeded by an
  //       invocation of the <f TRACE> macro.
  //   @parm The name of the condition. The condition name is a
  //         description of the condition that makes sense from the
  //         point of view of someone reading the source text. The
  //         name comprises a portion of the message that is printed
  //         if the condition is violated.
  //   @parm The condition expression. The expression should be
  //         free of side effects.
#define ASSERT(name, expression) \
  (expression) \
    ? (void)0  \
    : reportAssertionViolation("Assertion",    name, #expression, \
                               currentRoutineName, __FILE__, __LINE__)

  // @func Assert (when enabled with OM_ENABLE_DEBUG) that the
  //       invariant for the class of the current object is true.
#define INVARIANT() \
  checkInvariant();

  // @func Boolean implication - use <f IMPLIES> in construction of other
  //       assertions. Read 'IMPLIES(a, b)' as 'a => b', or
  //       'a implies b'. 'ASSERT("...", IMPLIES(a, b))' is the
  //       expression form of 'if (a) ASSERT("...", b)'. However,
  //       IMPLIES() hides the 'if' statement so that it can be
  //       'compiled away'.
  //   @parm An expression.
  //   @parm An expression.
#define IMPLIES(a, b) \
  (!(a) || (b))

#else

#define TRACE(name)

#define PRECONDITION(name, expression)

#define POSTCONDITION(name, expression)

#define ASSERT(name, expression)

#define INVARIANT()

#define IMPLIES(a, b)

#endif

#endif

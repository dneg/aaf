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

  // @func Is the given OMCharacter string valid ? Use
  //       <f validOMString> in expressions passed to the
  //       assertion macros <f PRECONDITION>, <f POSTCONDITION>
  //       and <f ASSERT>.
  //   @parm The OMCharacter string to check for validity.
  //   @rdesc True if the OMCharacter string is valid, false otherwise.
bool validOMString(const OMCharacter* string);

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

inline void noTrace(const char* /* routine */) {}

#define TRACE(routine) \
  char* currentRoutineName; \
  currentRoutineName = routine; \
  noTrace(currentRoutineName);

#endif

#if defined(OM_ENABLE_OBSOLETE)

  // @func Output a message indicating that the <p routineName>
  //       is obsolete and that <p newRoutineName> should be used instead
  //   @parm The name of the obsolete routine.
  //   @parm The name of the routine that should be called instead.
void obsolete(const char* routineName, const char* newRoutineName);

  // @func Print a message (when enabled with OM_ENABLE_DEBUG and
  //       OM_ENABLE_OBSOLETE) indicating that the current routine
  //       is obsolete and that <p newRoutineName> should be used instead.
  //       OBSOLETE is provided to aid clients in migrating from one
  //       Object Manager version to the next. Routines are made obsolete
  //       before they are removed. 
  //   @parm The name of the routine that should be called instead.
#define OBSOLETE(newRoutineName) \
  obsolete(currentRoutineName, newRoutineName);

#else

#define OBSOLETE(newRoutineName)

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

  // @func Evaluate <p expression> for each element,
  //       <p start> .. <p elementCount> of a collection. Use
  //       <p index> as the name of the index. The <p expression>
  //       is most usefully one of the assertion macros such as
  //       <f PRECONDITION>, <f POSTCONDITION> or <f ASSERT>.
  //   @parm The index name.
  //   @parm The starting index.
  //   @parm The number of elements.
  //   @parm The expression to evaluate for each element.
#define FOREACH(index, start, elementCount, expression) \
  { for (size_t index = start; index < start + elementCount; index++) \
    {expression;} }

  // @func Universal quantifier. Evaluate <p expression> for all
  //       elements, 0 .. <p elementCount> of a collection. Use
  //       <p index> as the name of the index. The <p expression>
  //       is most usefully one of the assertion macros such as
  //       <f PRECONDITION>, <f POSTCONDITION> or <f ASSERT>.
  //   @parm The index name.
  //   @parm The number of elements in the collection.
  //   @parm The expression to evaluate for each element.
#define FORALL(index, elementCount, expression) \
        FOREACH(index, 0, elementCount, expression) 

  // @func Define a name only when assertions are enabled. Use to
  //       avoid compiler warnings.
  //   @parm The name to (conditionally) define.
#define ANAME(name) \
  name

  // @func Save the value of a variable on entry to a routine for
  //       later retrieval in the postcondition with <f OLD>.
  //   @parm The name of the variable to save.
  //   @parm The type of the variable.
#define SAVE(name, type) \
  SAVE_EXPRESSION(name, name, type)

  // @func Save the value of an expression on entry to a routine for
  //       later retrieval in the postcondition with <f OLD>.
  //   @parm The name of the saved expression.
  //   @parm The expression to save.
  //   @parm The type of the expression.
#define SAVE_EXPRESSION(name, expression, type) \
  type oldValueOf##name = expression

  // @func Retrieve the value of a variable or expression saved on
  //       entry to a routine with <f SAVE> or with <f SAVE_EXPRESSION>.
  //       For use in postconditions.
  //   @parm The name of the saved variable or expression.
#define OLD(name) \
  oldValueOf##name

#else

#define TRACE(name)

#define OBSOLETE(newRoutineName)

#define PRECONDITION(name, expression)

#define POSTCONDITION(name, expression)

#define ASSERT(name, expression)

#define INVARIANT()

#define IMPLIES(a, b)

#define FOREACH(index, start, elementCount, expression)

#define FORALL(index, elementCount, expression)

#define ANAME(name)

#define SAVE(name, type)

#define SAVE_EXPRESSION(name, expression, type)

#define OLD(name)

#endif

#endif

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
#ifndef OMASSERTIONS_H
#define OMASSERTIONS_H


#include <stddef.h>

// @module OMAssertions | Functions and macros to implement run-time
//         monitoring of assertions.
//
// References ...
//
// [1] "Object Oriented Software Construction", Bertrand Meyer,
// 1997 Prentice Hall PTR, ISBN 0-13-629155-4
//
//   @mauthor Tim Bingham | tjb | Avid Technology, Inc.

  // @class OMAssertionViolation | Object Manager assertion
  //        violation. An instance of this class is thrown
  //        when an assertion violation occurs.
class OMAssertionViolation {
public:

  OMAssertionViolation() {}

  ~OMAssertionViolation() {}

  OMAssertionViolation(const OMAssertionViolation&) {}

};

  // @func Never define a name. Use to avoid compiler warnings.
  //   @parm The name not to define.
#define NNAME(name)

#if defined(OM_DEBUG)

#include "OMDataTypes.h"

  // @func Report an assertion violation. Used to implement the
  //       <f PRECONDITION>, <f POSTCONDITION> and <f ASSERT> macros.
  //   @parm The kind of assertion - PRECONDITION, POSTCONDITION or
  //         ASSERT.
  //   @parm The name of the assertion. The assertion name is a
  //         description of the assertion that makes sense from the
  //         internal point of view (that of someone reading the source
  //         text). The name comprises a portion of the message that is
  //         printed by this routine. The message that is printed makes
  //         sense from the external point of view.
  //   @parm The expression, as a text string, that was found not to
  //         be true.
  //   @parm The name of the routine in which the assertion violation
  //         occurred.
  //   @parm The name of the source file in which the assertion
  //         violation occurred.
  //   @parm The line number at which the assertion violation
  //         occurred.
void reportAssertionViolation(const char* assertionKind,
                              const char* assertionName,
                              const char* expressionString,
                              const char* routineName,
                              const char* fileName,
                              OMUInt32 lineNumber);

void assertionViolation(const char* assertionKind,
                        const char* assertionName,
                        const char* expressionString,
                        const char* routineName,
                        const char* fileName,
                        OMUInt32 lineNumber);

void handleAssertionViolation(void);

#if defined(OM_TRACE)

  // @func Output routine tracing information.
  //   @parm The name of the routine.
void trace(const char* routineName);

  // @func Print routine tracing information (when enabled with
  //       OM_DEBUG and OM_TRACE). The routine name
  //       provided is used by other assertions.
  //   @parm The routine name. For the most explicit output, names of
  //         member functions should be prefixed with the class name,
  //         as in className::functionName.
#define TRACE(routine) \
  const char* currentRoutineName = routine; \
  trace(currentRoutineName);

#else

inline void noTrace(const char* NNAME(routine)) {}

#define TRACE(routine) \
  const char* currentRoutineName; \
  currentRoutineName = routine; \
  noTrace(currentRoutineName);

#endif

#if defined(OM_OBSOLETE)

  // @func Output a message indicating that the <p routineName>
  //       is obsolete and that <p newRoutineName> should be used instead
  //   @parm The name of the obsolete routine.
  //   @parm The name of the routine that should be called instead.
void obsolete(const char* routineName, const char* newRoutineName);

  // @func Print a message (when enabled with OM_DEBUG and
  //       OM_OBSOLETE) indicating that the current routine
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

  // @func Assert (when enabled with OM_DEBUG) that the
  //       precondition described by <p name> and <p expression> is
  //       true. An invocation of this macro must be preceeded by an
  //       invocation of the <f TRACE> macro.
  //   @parm The name of the precondition. The precondition name is a
  //         description of the precondition that makes sense from the
  //         internal point of view (that of someone reading the source
  //         text). The name comprises a portion of the message that is
  //         printed if the precondition is violated. The message that
  //         is printed makes sense from the external point of view.
  //   @parm The precondition expression. The expression should be
  //         free of side effects.
#define PRECONDITION(name, expression) \
  (expression) \
    ? (void)0  \
    : assertionViolation("Precondition", name, #expression, \
                         currentRoutineName, __FILE__, __LINE__)

  // @func Assert (when enabled with OM_DEBUG) that the
  //       postcondition described by <p name> and <p expression> is
  //       true. An invocation of this macro must be preceeded by an
  //       invocation of the <f TRACE> macro.
  //   @parm The name of the postcondition. The postcondition name is a
  //         description of the postcondition that makes sense from the
  //         internal point of view (that of someone reading the source
  //         text). The name comprises a portion of the message that is
  //         printed if the postcondition is violated. The message that
  //         is printed makes sense from the external point of view.
  //   @parm The postcondition expression. The expression should be
  //         free of side effects.
#define POSTCONDITION(name, expression) \
  (expression) \
    ? (void)0  \
    : assertionViolation("Postcondition", name, #expression, \
                         currentRoutineName, __FILE__, __LINE__)

  // @func Assert (when enabled with OM_DEBUG) that the
  //       condition described by <p name> and <p expression> is
  //       true. An invocation of this macro must be preceeded by an
  //       invocation of the <f TRACE> macro.
  //   @parm The name of the condition. The condition name is a
  //         description of the condition that makes sense from the
  //         internal point of view (that of someone reading the source
  //         text). The name comprises a portion of the message that is
  //         printed if the condition is violated. The message that
  //         is printed makes sense from the external point of view.
  //   @parm The condition expression. The expression should be
  //         free of side effects.
#define ASSERT(name, expression) \
  (expression) \
    ? (void)0  \
    : assertionViolation("Assertion", name, #expression, \
                         currentRoutineName, __FILE__, __LINE__)

  // @func Assert (when enabled with OM_DEBUG) that the
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

  // @func Declare a return result only when assertions are enabled. Use to
  //       avoid compiler warnings. Note trailing "=" - invoke without ";".
  //   @parm The return result to (conditionally) define.
#define ARESULT(result) \
  result =

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

  // @func Assert (when enabled with OM_DEBUG) that the
  //       condition <p expression> is true.
  //       Please don't use this macro in new code. It is intended
  //       only as a quick replacement, in existing code, for the ISO
  //       assert() macro from <assert.h>. In new code, please use
  //       <f ASSERT>, or one of the other macros defined here.
  //   @parm The condition expression. The expression should be
  //         free of side effects.
#define ASSERTU(expression) \
  (expression) \
    ? (void)0  \
    : assertionViolation( \
        "Assertion", \
        "Unknown (assertion name not supplied by developer)", \
        #expression, \
        "Unknown (routine name not supplied by developer)", \
        __FILE__, \
        __LINE__)

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

#define ARESULT(result)

#define ANAME(name)

#define SAVE(name, type)

#define SAVE_EXPRESSION(name, expression, type)

#define OLD(name)

#define ASSERTU(expression)

#endif

#endif

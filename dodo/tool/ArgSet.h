#ifndef _ArgSet_h_
#define _ArgSet_h_

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

#ifndef _Vect_h_
#include "Vect.h"
#endif

#ifndef _ArgDef_h_
#include "ArgDef.h"
#endif


struct ArgSet
{
  //
  // Default ctor, dtor, copy ctor, op= are OK
  //


  void AppendArg
	(const ArgDef & src);
  //
  // Appends the given argument.
  //
  //********


  int GetNumArgs () const;
  //
  // Gets the number of args defined.
  //
  //********


  ArgDef GetArg
	(int index) const;
  //
  // Returns a copy of the given argument.  Index is zero-based.
  //
  //********


  enum eInitArgsStat
  {
    kInitNoError = 0,	// all OK
    kInitArgsTooFew,	// too few args
    kInitArgsTooMany,	// too many args
    kInitNoStartParen,	// starting paren missing
    kInitNoEndParen		// ending paren missing
  };
  //
  // Status for InitArgs()

  eInitArgsStat InitArgs
	(TextStream & text,
	 int numExpected);
  //
  // Initializes this ArgSet using the given text.  Args are assumed
  // to be of the form:
  //
  // (arg1,arg2,arg3,...)
  //
  // where the string must begin with an open-parenthesis.  Args will
  // be consumed until a c close-parenthesis is found.  White space
  // preceding arguments is discarded, while white space inside
  // arguments is assumed to be part of the argument.  Commas separate
  // arguments.  Commas to be part of an argument can be escaped with
  // the backslash ('\') character.
  //
  //********


  static const char * InitErrString
	(eInitArgsStat stat);
  //
  // Translates eInitArgsStat into an error string.
  //
  //********


  static void Test ();
  //
  // Unit test.  assert()s if an error is found.
  //
  //********


private:
  Vector<ArgDef> _args;
};


#endif // ! _ArgSet_h_

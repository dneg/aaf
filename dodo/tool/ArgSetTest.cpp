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

#include "ArgSet.h"
#include "ArgSet.h"
#ifndef _ArgSet_h_
#error - improper include guard
#endif

#include <assert.h>
#include <string.h>


void ArgSet::Test ()
{
  //
  // ctor
  //
  ArgSet as;
  assert (0 == as._args.size());

  //
  // AppendArg(), GetNumArgs(), GetArg()
  //
  ArgDef ad;
  TextStream input;
  assert (0 == as._args.size());
  assert (0 == as.GetNumArgs());
  input = "here's one.,";
  assert (ArgDef::kInitNoError == ad.Init (input));
  as.AppendArg (ad);
  assert (1 == as.GetNumArgs());
  assert (1 == as._args.size());
  TextStream ts;
  ts = as._args[0].GetText();
  assert (ts.Snoop ("here's one."));
  ts = as.GetArg(0).GetText();
  assert (ts.Snoop ("here's one."));

  //
  // InitArgs()
  //
  eInitArgsStat stat;

  input = "(arg1,arg2)";
  stat = as.InitArgs (input, 3);
  assert (kInitArgsTooFew == stat);

  input = "(arg1,arg2)";
  stat = as.InitArgs (input, 1);
  assert (kInitArgsTooMany == stat);

  input = "arg1,arg2)";
  stat = as.InitArgs (input, 1);
  assert (kInitNoStartParen == stat);

  input = "(arg1,arg2";
  stat = as.InitArgs (input, 2);
  assert (kInitNoEndParen == stat);

  // input = "(arg1,arg2,arg3,arg4,arg5,arg6)";
  input = ("(arg1,arg2,arg3,\n"
	   "    arg4,	arg5,          arg6) hey");
  stat = as.InitArgs (input, 6);
  assert (kInitNoError == stat);
  assert (6 == as.GetNumArgs());

  ts = as.GetArg(0).GetText();
  assert (ts.Expect("arg1"));
  assert (0 == ts.GetLength());
  ts = as.GetArg(1).GetText();
  assert (ts.Expect("arg2"));
  assert (0 == ts.GetLength());
  ts = as.GetArg(2).GetText();
  assert (ts.Expect("arg3"));
  assert (0 == ts.GetLength());
  ts = as.GetArg(3).GetText();
  assert (ts.Expect("arg4"));
  assert (0 == ts.GetLength());
  ts = as.GetArg(4).GetText();
  assert (ts.Expect("arg5"));
  assert (0 == ts.GetLength());
  ts = as.GetArg(5).GetText();
  assert (ts.Expect("arg6"));
  assert (0 == ts.GetLength());

  input = "(arg1,arg2)";
  stat = as.InitArgs (input, 0);
  assert (kInitNoError == stat);
  assert (input.Expect ("(arg1,arg2)"));
  assert (0 == input.GetLength());
  assert (0 == as.GetNumArgs());
}

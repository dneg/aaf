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

#include "MacroDef.h"
#include "MacroDef.h"
#ifndef _MacroDef_h_
#error - include guard not properly set.
#endif

#ifndef _ArgSet_h_
#include "ArgSet.h"
#endif

#include <assert.h>
#include <string.h>


void MacroDef::Test ()
{
  //
  // ctor
  //
  MacroDef md;
  assert (0 == md._numArgs);
  assert (0 == md._name.GetLength());
  assert (0 == md._definition.GetLength());

  //
  // Init()
  //
  TextStream name = "mac";
  md.Init (name, 2);
  assert (2 == md._numArgs);
  assert (3 == md._name.GetLength());
  assert (md._name.Snoop("mac"));

  //
  // AppendMacroExpansion()
  //
  TextStream exp;
  eAppendStat aStat;
  TextStream ts;

  exp = "arg %03 too great";
  aStat = md.AppendMacroExpansion (exp);
  assert (kAppendArgRange == aStat);

  exp = "arg %x illegal";
  aStat = md.AppendMacroExpansion (exp);
  assert (kAppendBadArg == aStat);

  md.Init (name, 2);	// re-init to clear out old macro def crap
  assert (0 == md._definition.GetLength());
  exp = "Second=%02, First=%01";
  aStat = md.AppendMacroExpansion (exp);
  assert (kAppendNoError == aStat);
  assert (0 != md._definition.GetLength());
  ts = md._definition;
  assert (ts.GetLength());
  assert (ts.Expect ("Second=%02, First=%01"));
  assert (! ts.GetLength());


  //
  // IsMacro()
  //
  TextStream input;
  bool stat;

  input = "amac";
  stat = md.IsMacro (input);
  assert (! stat);

  input = "macro";
  stat = md.IsMacro (input);
  assert (stat);

  input = "macanudo";
  stat = md.IsMacro (input);
  assert (stat);

  input = "madness";
  stat = md.IsMacro (input);
  assert (! stat);

  //
  // InvokeMacro()
  //
  TextStream output;
  eInvokeMacroStat iStat;
  SourceInfo si ("foo", 2);
  ArgSet as;
  ArgSet::eInitArgsStat iasStat;

  TextStream argText;
  argText = "(ONE!,TWO!,THREE!)";
  iasStat = as.InitArgs (argText, 3);
  assert (ArgSet::kInitNoError == iasStat);
  assert (0 == output.GetLength());

  iStat = md.InvokeMacro(as, output, si);
  assert (kInvokeWrongArgCount == iStat);
  assert (0 == output.GetLength());

  MacroDef md2;
  iStat = md2.InvokeMacro(as, output, si);
  assert (kInvokeNotInited == iStat);

  md2.Init (name, 2);
  iStat = md2.InvokeMacro(as, output, si);
  assert (kInvokeWrongArgCount == iStat);
  assert (0 == output.GetLength());

  argText = "(ONE!,TWO!)";
  iasStat = as.InitArgs (argText, 2);
  assert (ArgSet::kInitNoError == iasStat);
  assert (0 == output.GetLength());
  iStat = md.InvokeMacro(as, output, si);
  assert (kInvokeNoError == iStat);
  assert (0 != output.GetLength());

  stat = output.Expect ("Second=TWO!, First=ONE!");
  assert (stat);
  assert (0 == output.GetLength());

  //
  // GetNumArgs()
  //
  assert (2 == md2.GetNumArgs());
  md2.Init (name, 0);
  assert (0 == md2.GetNumArgs());
  assert (2 == md.GetNumArgs());
}

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

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

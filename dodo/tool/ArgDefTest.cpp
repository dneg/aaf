/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "ArgDef.h"
#include "ArgDef.h"
#ifndef _ArgDef_h_
#error - improper include guard
#endif

#include <assert.h>
#include <string.h>


void ArgDef::Test ()
{
  //
  // Init(), GetText()
  //
  ArgDef ad;
  TextStream ts;
  TextStream input;
  eInitStat stat;
  assert (0 == ad._rep.GetLength());

  input = "something";
  stat = ad.Init (input);
  assert (kInitNotTerminated == stat);

  input = "proper)";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("proper"));
  assert (0 == ts.GetLength());
  assert (input.Expect (")"));
  assert (0 == input.GetLength());

  input = "(paren in arg)";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("(paren in arg"));
  assert (0 == ts.GetLength());
  assert (input.Expect (")"));
  assert (0 == input.GetLength());

  input = "middle arg,yup)";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("middle arg"));
  assert (0 == ts.GetLength());
  assert (input.Expect ("yup)"));
  assert (0 == input.GetLength());

  input = "comma in\\, arg, here";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("comma in, arg"));
  assert (0 == ts.GetLength());
  assert (input.Expect (" here"));
  assert (0 == input.GetLength());

  input = "  \t\t\n\t  White\tspace in here.,";
  stat = ad.Init (input);
  assert (kInitNoError == stat);
  ts = ad.GetText ();
  assert (0 != ts.GetLength());
  assert (ts.Expect ("White\tspace in here."));
  assert (0 == ts.GetLength());
  assert (0 == input.GetLength());

  input = "bad escape\\";
  stat = ad.Init (input);
  assert (kInitBadEscape == stat);
}

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

#include "MacroSet.h"
#include "MacroSet.h"
#ifndef _MacroSet_h_
#error - improperly formed include guard
#endif

#include <assert.h>
#include <string.h>


void MacroSet::Test ()
{
  //
  // ctor
  //
  MacroSet ms;
  assert (0 == ms._macs.size());

  //
  // Append(), GetNumMacros()
  //
  MacroDef md0;
  MacroDef md1;
  MacroDef md2;

  md0.Init ("zero-arg", 0);
  md1.Init ("1-arg", 1);
  md2.Init ("two-args!", 2);

  assert (0 == md0.GetNumArgs());
  assert (1 == md1.GetNumArgs());
  assert (2 == md2.GetNumArgs());

  TextStream ts;

  MacroDef::eAppendStat mdStat;

  ts = "Zero-arg macro expansion.";
  mdStat = md0.AppendMacroExpansion (ts);
  assert (MacroDef::kAppendNoError == mdStat);
  ts = "One-arg macro: it is %01, isn't it?";
  mdStat = md1.AppendMacroExpansion (ts);
  assert (MacroDef::kAppendNoError == mdStat);
  ts = "Two-arg macro: second=%02, first=%01!!!";
  mdStat = md2.AppendMacroExpansion (ts);
  assert (MacroDef::kAppendNoError == mdStat);

  assert (0 == ms._macs.size());
  assert (0 == ms.GetNumMacros());
  ms.Append (md1);
  assert (1 == ms._macs.size());
  assert (1 == ms.GetNumMacros());
  ms.Append (md2);
  assert (2 == ms._macs.size());
  assert (2 == ms.GetNumMacros());
  ms.Append (md0);
  assert (3 == ms._macs.size());
  assert (3 == ms.GetNumMacros());


  //
  // GetMacro()
  //
  assert (1 == ms.GetMacro(0).GetNumArgs());
  assert (0 == ms.GetMacro(2).GetNumArgs());
  assert (2 == ms.GetMacro(1).GetNumArgs());


  //
  // SearchMacro()
  //
  bool stat;
  MacroDef found;

  ts = "zero-arf! arf! arf!";
  stat = ms.SearchMacroName (ts, found);
  assert (!stat);

  ts = "zero-arg! arg! arg!";
  stat = ms.SearchMacroName (ts, found);
  assert (stat);
  assert (0 == found.GetNumArgs());

  ts = "xzero-arg! arg! arg!";
  stat = ms.SearchMacroName (ts, found);
  assert (!stat);

  ts = "1-arf! arf! arf!";
  stat = ms.SearchMacroName (ts, found);
  assert (!stat);

  ts = "1-arg! arg! arg!";
  stat = ms.SearchMacroName (ts, found);
  assert (stat);
  assert (1 == found.GetNumArgs());

  ts = "x1-arg! arg! arg!";
  stat = ms.SearchMacroName (ts, found);
  assert (!stat);

  ts = "two-arfs! arf! arf!";
  stat = ms.SearchMacroName (ts, found);
  assert (!stat);

  ts = "two-args! arg! arg!";
  stat = ms.SearchMacroName (ts, found);
  assert (stat);
  assert (2 == found.GetNumArgs());

  ts = "xtwo-args! arg! arg!";
  stat = ms.SearchMacroName (ts, found);
  assert (!stat);


  //
  // AddMacros()
  //
  FILE * fp = fopen ("test.dat", "w");
  assert (fp);
  fprintf (fp,
		   "#startm FirstStreamMacro 1\n"
		   "macro 1 arg:%%01 body\n"
		   "#endm\n");
  fclose (fp);

  ts = "#import test.dat\n";
  assert (3 == ms._macs.size());
  ms.AddMacros (ts);
  assert (4 == ms._macs.size());


  //
  // ApplyMacros()
  //
  TextStream input;
  TextStream output;
  TextStream bitbucket;

  input = ("This is where the FirstStreamMacro(hey!) might be placed"
		   "for 1-arg(dope) operation into zero-arg heyday. Hey!"
		   "two-args!(fitz,\n"
		   "	hugh)is barmy!\n");
  stat = ms.ApplyMacros (input, output);
  assert (stat);
  ts = output;
  stat = ms.ApplyMacros (ts, bitbucket);
  assert (!stat);

  ts = ("This is where the macro 1 arg:hey! body\n might be placed"
	"for One-arg macro: it is dope, isn't it? operation into"
	" Zero-arg macro expansion. heyday. Hey!"
	"Two-arg macro: second=hugh, first=fitz!!!is barmy!\n");
  assert (output.GetLength());
  stat = output.Expect (ts);
  assert (stat);
  assert (!output.GetLength());
}

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

#include "TextLine.h"
#include "TextLine.h"
#ifndef _TextLine_h_
#error - nasty include guard
#endif

#include <assert.h>
#include <string.h>


void TextLine::Test ()
{
  //
  // ctor
  //
  TextLine tl;
  assert (0 == tl._pStorage);
  assert (0 == tl._pData);
  assert (0 == tl._numAllocated);

  //
  // Init()
  //
  tl.Init ("howdy");
  assert (tl._pStorage);
  assert (tl._pData);
  assert (tl._pData >= tl._pStorage);
  assert (tl._numAllocated);
  assert (tl._numAllocated > strlen ("howdy"));
  assert (0 == strcmp (tl._pData, "howdy"));

  //
  // copy ctor
  //
  TextLine tl2 = tl;
  assert (tl2._pStorage);
  assert (tl2._pData);
  assert (tl2._pData >= tl._pStorage);
  assert (tl2._numAllocated);
  assert (tl2._numAllocated > strlen ("howdy"));
  assert (0 == strcmp (tl2._pData, "howdy"));

  //
  // op=
  //
  TextLine tl3;
  tl3 = tl;
  assert (tl3._pStorage);
  assert (tl3._pData);
  assert (tl3._pData >= tl._pStorage);
  assert (tl3._numAllocated);
  assert (tl3._numAllocated > strlen ("howdy"));
  assert (0 == strcmp (tl3._pData, "howdy"));

  //
  // GetLength()
  //
  assert (5 == tl.GetLength());

  //
  // Append()
  //
  bool stat;

  stat = tl.Append ('!');
  assert (stat);
  assert (0 == strcmp (tl._pData, "howdy!"));
  stat = tl.Append ('\\');
  assert (stat);
  assert (0 == strcmp (tl._pData, "howdy!\\"));
  stat = tl.Append ('\n');
  assert (stat);
  assert (0 == strcmp (tl._pData, "howdy!\\\n"));
  // we should be able to add chars after escaped newline
  stat = tl.Append (' ');
  assert (stat);
  assert (0 == strcmp (tl._pData, "howdy!\\\n "));
  stat = tl.Append ('\n');
  assert (stat);
  assert (0 == strcmp (tl._pData, "howdy!\\\n \n"));
  // we should not be able to add chars after non-escaped newline
  stat = tl.Append ('x');
  assert (! stat);
  assert (0 == strcmp (tl._pData, "howdy!\\\n \n"));
  stat = tl.Append ('y');
  assert (! stat);
  assert (0 == strcmp (tl._pData, "howdy!\\\n \n"));

  //
  // Consume()
  //
  char c;

  stat = tl2.Consume (c);
  assert (stat);
  assert ('h' == c);
  stat = tl2.Consume (c);
  assert (stat);
  assert ('o' == c);
  stat = tl2.Consume (c);
  assert (stat);
  assert ('w' == c);
  stat = tl2.Consume (c);
  assert (stat);
  assert ('d' == c);
  stat = tl2.Consume (c);
  assert (stat);
  assert ('y' == c);
  stat = tl2.Consume (c);
  assert (!stat);
  stat = tl2.Consume (c);
  assert (!stat);
  assert (! tl2.GetLength());

  //
  // Set/GetSourceInfo()
  //
  SourceInfo si1 ("this-file", 42);

  SourceInfo si2;
  si2 = tl.GetSourceInfo ();
  assert (0 == si2.GetLineNumber());
  assert (0 == si2.GetFileName());

  tl.SetSourceInfo (si1);
  si2 = tl.GetSourceInfo ();
  assert (42 == si2.GetLineNumber());
  assert (0 == strcmp ("this-file", si2.GetFileName()));
}

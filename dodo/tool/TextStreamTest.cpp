/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "TextStream.h"
#include "TextStream.h"
#ifndef _TextStream_h_
#error - bogus include guard
#endif

#include <assert.h>
#include <string.h>
#include <stdio.h>


void TextStream::Test ()
{
  //
  // ctor, GetLength()
  //
  TextStream ts1;
  assert (0 == ts1._pStorage);
  assert (0 == ts1._pData);
  assert (0 == ts1._numAllocated);
  assert (0 == ts1._cachedLen);
  assert (0 == ts1.GetLength());

  char * str = "What's up with this?\nI just want to know.\n";
  TextStream ts2 (str);
  assert (strlen (str) == ts2._cachedLen);
  assert (strlen (str) == ts2.GetLength());
  assert (0 == strcmp (str, ts2._pData));

  //
  // copy ctor
  //
  TextStream ts3 = ts2;
  assert (strlen (str) == ts3._cachedLen);
  assert (strlen (str) == ts3.GetLength());
  assert (0 == strcmp (str, ts3._pData));
  
  //
  // Clear()
  //
  ts3.Clear ();
  assert (0 == ts3._pStorage);
  assert (0 == ts3._pData);
  assert (0 == ts3._numAllocated);
  assert (0 == ts3._cachedLen);
  assert (0 == ts3.GetLength());

  //
  // op=
  //
  ts3 = ts2;
  assert (strlen (str) == ts3._cachedLen);
  assert (strlen (str) == ts3.GetLength());
  assert (0 == strcmp (str, ts3._pData));

  //
  // Append(char)
  //
  assert (0 == ts1._pStorage);
  assert (0 == ts1._pData);
  assert (0 == ts1._numAllocated);
  assert (0 == ts1.GetLength());
  ts1.Append ('H');
  assert (ts1._pStorage);
  assert (ts1._numAllocated);
  assert (1 == ts1.GetLength());
  assert (ts1._pData);
  assert (1 == strlen (ts1._pData));
  int oldAlloc = ts1._numAllocated;
  ts1.Append ('i');
  assert (oldAlloc == ts1._numAllocated);
  assert (0 == strcmp ("Hi", ts1._pData));

  //
  // Append(TextStream)
  //
  ts2 = TextStream ("Hello, ");
  ts3 = TextStream ("there.");
  ts2.Append (ts3);
  assert (0 == strcmp ("Hello, there.", ts2._pData));

  //
  // Append(FILE*)
  //
  FILE * fp = fopen ("test.dat", "w");
  SourceInfo si;
  assert (fp);
  fprintf (fp, "This is some kinda test, ya know?\n");
  fclose (fp);
  fp = fopen ("test.dat", "r");
  assert (fp);
  ts3.Append (fp, si);
  fclose (fp);
  assert (0 == strcmp ("there.This is some kinda test, ya know?\n", ts3._pData));

  //
  // Consume()
  //
  char c;
  bool stat;
  stat = ts1.Consume (c);
  assert (stat);
  assert ('H' == c);
  assert (1 == ts1.GetLength());
  stat = ts1.Consume (c);
  assert (stat);
  assert ('i' == c);
  assert (0 == ts1.GetLength());
  stat = ts1.Consume (c);
  assert (!stat);
  assert (! c);

  //
  // Snoop(), Expect()
  //
  assert (! ts3.Snoop ("there.."));
  assert (ts3.Snoop ("there."));
  assert (ts3.Snoop ("there."));
  assert (! ts3.Expect ("there.."));
  assert (ts3.Expect ("there."));
  assert (! ts3.Expect ("there."));
  assert (ts3.Expect ("This is"));
}

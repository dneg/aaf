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

#include "SourceInfo.h"
#include "SourceInfo.h"
#ifndef _SourceInfo_h_
#error - bad include guard
#endif

#include <assert.h>
#include <string.h>


void SourceInfo::Test ()
{
  //
  // ctor
  //
  SourceInfo si0;
  assert (0 == si0._fileName);
  assert (0 == si0._lineNumber);

  SourceInfo si1 ("hey", 2);
  assert (si1._fileName);
  assert (0 == strcmp (si1._fileName, "hey"));
  assert (2 == si1._lineNumber);

  //
  // copy ctor
  //
  SourceInfo si2 = si1;
  assert (si2._fileName);
  assert (0 == strcmp (si2._fileName, "hey"));
  assert (2 == si2._lineNumber);

  //
  // op=
  //
  SourceInfo si3 ("you", 17);
  si2 = si3;
  assert (si3._fileName);
  assert (0 == strcmp (si3._fileName, "you"));
  assert (17 == si3._lineNumber);

  //
  // GetFileName()
  //
  assert (si3.GetFileName() == si3._fileName);
  assert (0 == strcmp (si3.GetFileName(), si3._fileName));

  //
  // GetLineNumber()
  //
  assert (si3.GetLineNumber() == si3._lineNumber);
  assert (17 == si3.GetLineNumber());
}

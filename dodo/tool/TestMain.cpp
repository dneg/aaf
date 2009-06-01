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

#ifndef _SourceInfo_h_
#include "SourceInfo.h"
#endif

#ifndef _TextLine_h_
#include "TextLine.h"
#endif

#ifndef _TextStream_h_
#include "TextStream.h"
#endif

#ifndef _ArgDef_h_
#include "ArgDef.h"
#endif

#ifndef _ArgSet_h_
#include "ArgSet.h"
#endif

#ifndef _MacroDef_h_
#include "MacroDef.h"
#endif

#ifndef _MacroSet_h_
#include "MacroSet.h"
#endif

#include <iostream>
using std::cout;
using std::endl;

#include <stdlib.h>


int main ()
{
  cout << "Testing SourceInfo..." << endl;
  SourceInfo::Test ();
  cout << "Testing TextLine..." << endl;
  TextLine::Test ();
  cout << "Testing TextStream..." << endl;
  TextStream::Test ();
  cout << "Testing ArgDef..." << endl;
  ArgDef::Test ();
  cout << "Testing ArgSet..." << endl;
  ArgSet::Test ();
  cout << "Testing MacroDef..." << endl;
  MacroDef::Test ();
  cout << "Testing MacroSet..." << endl;
  MacroSet::Test ();
  cout << "done." << endl;

  return 0;
}

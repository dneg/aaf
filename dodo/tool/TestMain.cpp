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

#include <iostream.h>
#include <stdlib.h>


void main ()
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

  exit (0);
}

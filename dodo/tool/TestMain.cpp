/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

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

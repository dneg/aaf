#ifndef _ArgDef_h_
#define _ArgDef_h_
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef _TextStream_h_
#include "TextStream.h"
#endif


struct ArgDef
{
  //
  // default ctor, dtor, copy ctor, op= are OK
  //

  enum eInitStat
  {
    kInitNoError = 0,	// all OK
    kInitNotTerminated,	// improper termination (no ')' or ',')
    kInitBadEscape		// improper use of '\' character
  };
  //
  // Status for Init()

  eInitStat Init
	(TextStream & text);
  //
  // Initializes this arg using the given source text.
  //
  //********


  TextStream GetText () const;
  //
  // Returns the text of this argument.
  //
  //********


  static void Test ();
  //
  // Unit test.  assert()s if an error is found.
  //
  //********


private:
  TextStream _rep;
};


#endif // ! _ArgDef_h_

#ifndef _MacroDef_h_
#define _MacroDef_h_

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

#ifndef _ArgSet_h_
#include "ArgSet.h"
#endif


struct MacroDef
{
  MacroDef ();
  //
  // default ctor
  //
  //********

  //
  // Default dtor, copy ctor, op= are OK
  //

  void Init
  (const TextStream & name,	// [in] this macro's name
   int numArgs);			// [in] number of arguments this macro expects
  //
  // Initializes an empty macro definition.  It will have the given
  // macro name, and will expect the given number of macro arguments.
  //
  // If Init() is called on an already initialized MacroDef, the
  // previous contents of the MacroDef will be deleted.
  //
  // Requires that name be non-empty.
  //
  //********


  //
  // Data accessors
  //
  enum eAppendStat
  {
    kAppendNoError,	// all went well
    kAppendArgRange,	// illegal argument value given
    kAppendBadArg	// illegal argument text given
  };
  eAppendStat AppendMacroExpansion
	(TextStream & text);
  //
  // Appends the given text to the stored macro expansion.
  //
  //********


  bool IsMacro
	(TextStream & testName) const;
  //
  // Returns true if the first part of text matches this macro's
  // entire name, and consumes that text from text.  Returns false
  // otherwise, and consumes no text.
  //
  //********


  enum eInvokeMacroStat
  {
    kInvokeNoError,			// all went well
    kInvokeWrongArgCount,	// improper number of args provided
    kInvokeNotInited		// this MacroDef has not yet been
							// initialized
  };
  //
  // Status for InvokeMacro()

  eInvokeMacroStat InvokeMacro
  (const ArgSet & args,				// [in] list of string macro
									// arguments
   TextStream & outText,			// [out] buffer into which
									// expansion is placed
   const SourceInfo & si) const;	// [in] Source Info to be placed
									// into output stream
  //
  // Invokes this macro using the given arguments.  Places result into
  // outText.
  //
  // Returns:
  //  eInvokeMacroStat describing success or failure of this
  //  operation.
  //
  //********


  static const char * InvErrString
	(eInvokeMacroStat stat);
  //
  // Translates eInvokeMacroStat into an error string.
  //
  //********


  int GetNumArgs () const;
  //
  // Returns:
  //  the number of arguments expected by this macro
  //
  //********


  void dump (FILE* fp) const;
  //
  // Dumps contents of this thing to given file.
  //
  //********


  char GetInitial () const;
  //
  // Returns the initial character of this macro's name.  Returns '\0'
  // if it has not yet been initialized.
  //
  //********


  static void Test ();
  //
  // Unit test.  Will assert() if a problem is found.
  //
  //********


private:
  int			_numArgs;
  TextStream	_name;
  TextStream	_definition;
};


#endif // ! _MacroDef_h_

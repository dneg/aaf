#ifndef _MacroSet_h_
#define _MacroSet_h_

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef _MacroDef_h_
#include "MacroDef.h"
#endif

struct TextStream;

struct MacroSet
{
  //
  // Default ctor, dtor, copy ctor, op= are OK
  //


  void Append
	(const MacroDef & src);		// [in] macro to append
  //
  // Appends the given macro to our list.
  //
  //********


  int GetNumMacros () const;
  //
  // Gets the number of macros defined.
  //
  //********


  MacroDef GetMacro
	(int index) const;
  //
  // Returns a copy of the given macro.  Index is zero-based.
  //
  //********


  bool SearchMacroName
	(TextStream & input,
	 MacroDef & foundMacro) const;
  //
  // Searches through all known macros for one whose entire name
  // matches the beginning of the given input.  If found, consumes the
  // macro's name from the input, copies that macro into foundMacro,
  // and returns true; if not found, consumes nothing and returns
  // false.
  //
  //********


  void AddMacros
	(TextStream & input,			// [in] input text from which
									// macros are taken
	 bool ignoreComments = false);	// [in] if true, will ignore all
									// text outside of comments
  //
  // Reads input, following #import statements, consuming text and
  // adding the macros defined therein to this object.
  //
  //********


  bool ApplyMacros
	(TextStream & input,			// [in] input text to which macros
									// are applied
	 TextStream & output);			// [out] output text to which
									// parsed text is written
  //
  // Applies these macros to input stream and writes results to
  // output.
  //
  // Returns:
  //  true if any macro substitutions were made
  //  returns false if no changes were made.
  //
  //********


  void dump (FILE* fp) const;
  //
  // Dumps contents of this thing to given file.
  //
  //********


  static void Test ();
  //
  // Unit test.  assert()s if an error is found.
  //
  //********


private:
  //
  // Helper methods
  //

  void import
	(TextStream & input,
	 bool ignoreText);

  TextStream readName
	(TextStream & input);

  int readCount
	(TextStream & input);

  TextStream readDefinition
	(TextStream & input);

  void readMacro
	(TextStream & input);

  static void discardLine
	(TextStream & input);


  static void err_exit
	(const char * msg,
	const TextStream & lineInfo);

  static void err_exit1
	(const char * msg1,
	 const char * msg2,
	 const TextStream & lineInfo);


  Vector<MacroDef> _macs;
};


#endif // ! _MacroSet_h_

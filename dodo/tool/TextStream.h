#ifndef _TextStream_h_
#define _TextStream_h_
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef _Vect_h_
#include "Vect.h"
#endif

#ifndef _TextLine_h_
#include "TextLine.h"
#endif

#include <stdio.h>


struct TextStream
{
  TextStream
	(const char * src = 0);
  //
  // ctor to make one of these from a C-style null-terminated string,
  // and caching the given fileName and starting line number.
  //
  //********


  ~TextStream();
  TextStream (const TextStream & src);
  TextStream operator= (const TextStream & src);
  //
  // dtor, copy ctor, op=
  //
  //********


  void Clear ();
  //
  // clears the contents of this TextStream.
  //
  //********


  int GetLength () const;
  //
  // Returns the number of characters contained in this TextStream.
  //
  //********


  void Append
	(char c);		// [in] char to append to this stream
  //
  // Appends the character to the end of this text stream.
  //
  //********


  void Append
	(const TextStream & txt);	// [in] text to append to this stream
  //
  // Appends the contents of the given text stream to the end of this
  // text stream.
  //
  //********


  void Append
	(FILE * fp,					// [in] char to append to this stream
	 const SourceInfo & si);	// [in] filename/line number to be
								// cached 
  //
  // Appends the character to the end of this text stream.
  //
  //********


  bool Consume
	(char & c);		// [out] destination for char to be retrieved
  //
  // Consumes one character from beginning of this text stream.
  //
  // Returns:
  //  true if successful, false if stream is empty.
  //
  //********


  bool Expect
	(const TextStream & key);	// [in] text to compare
  //
  // Compares entirety of key to the beginning of this text.  If it
  // matches, the text matching the key is consumed and true is
  // returned.  Otherwise, nothing is consumed and false is returned.
  //
  //********


  bool Snoop
	(const TextStream & key) const;	// [in] text to compare
  //
  // Compares entirety of key to the beginning of this text.  If it
  // matches,  true is returned.  Otherwise false is returned.  In
  // neither case is any text consumed.
  //
  //********


  SourceInfo GetCurSourceInfo () const;
  //
  // Returns:
  //  current source information with reference to the original source file.
  //
  //********


  void SetCurSourceInfo (const SourceInfo & si);
  //
  // Sets the source info of the last line in this stream to the given
  // info.
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

  char *		_pStorage;
  int			_numAllocated;

  char *		_pData;
  int			_cachedLen;

  SourceInfo	_startSi;
};

#endif // !  _TextStream_h_

#ifndef _SourceInfo_h_
#define _SourceInfo_h_
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

struct SourceInfo
{
  SourceInfo
	(const char * filename = 0,
	 int linenum = 0);
  //
  // Creates one of these with the given filename and line number.
  //
  //********

  ~SourceInfo ();
  SourceInfo (const SourceInfo & src);
  SourceInfo operator= (const SourceInfo & src);


  const char * GetFileName () const;
  //
  // Returns a pointer to this object's filename.
  //
  // DANGER!!! This returns a pointer to data inside this object!
  // Make sure this object's lifetime is at least as long as your
  // caching of the returned pointer!
  //
  //********


  int GetLineNumber () const;
  //
  // Returns this object's line number.
  //
  //********


  static void Test ();
  //
  // Unit test.  assert()s if a problem is found.
  //
  //********


private:

  void allocate (const char * name);
  //
  // private helper to allocate stuff for one of these
  //
  //********

  char * _fileName;
  int    _lineNumber;
};

#endif // ! _SourceInfo_h_

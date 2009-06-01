#ifndef _SourceInfo_h_
#define _SourceInfo_h_
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

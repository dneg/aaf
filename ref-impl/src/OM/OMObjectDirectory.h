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

// @doc OMINTERNAL
#ifndef OMOBJECTDIRECTORY_H
#define OMOBJECTDIRECTORY_H

#include <stddef.h>

class OMStorable;

  // @class Debug only data structure for tracking objects by name.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMObjectDirectory {
public:

  OMObjectDirectory(void);

  ~OMObjectDirectory(void);

  //
  //
  bool lookup(const wchar_t* name, const OMStorable*& p) const;

  //
  //
  void insert(const wchar_t* name, const OMStorable* p);

  // Count of valid entries.
  //
  size_t count(void) const;

  // Object at index.
  //
  OMStorable* object(size_t index) const;

  // Name of obect at index.
  //
  const wchar_t* name(size_t index) const;

private:

  struct TableEntry {
    OMStorable* _object;
    wchar_t* _name;
  };
  size_t _capacity;        // Number of potential entries.
  size_t _current;         // Index of first unoccupied entry.
  TableEntry* _table;   // Dynamically allocated array.

};

#endif

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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMObjectDirectory

#include "OMObjectDirectory.h"
#include "OMAssertions.h"
#include "OMUtilities.h"

#define OM_OBJECT_DIRECTORY_CAPACITY  (5000)

OMObjectDirectory::OMObjectDirectory(void)
: _capacity(OM_OBJECT_DIRECTORY_CAPACITY), _current(0)
{
  TRACE("OMObjectDirectory::OMObjectDirectory");

  _table = new TableEntry[_capacity];
  ASSERT("Valid heap pointer", _table != 0);
  for (size_t i = 0; i < _capacity; i++) {
    _table[i]._object = 0;
    _table[i]._name = 0;
  }
}

OMObjectDirectory::~OMObjectDirectory(void)
{
  for (size_t i = 0; i < _current; i++) {
    delete [] _table[i]._name;
  }
  delete [] _table;
  _table = 0;
}

bool OMObjectDirectory::lookup(const wchar_t* name, const OMStorable*& p) const
{
  TRACE("OMObjectDirectory::lookup");
  PRECONDITION("Valid name to look up", validWideString(name));
  bool result = false;

  for (size_t i = 0; i < _current; i++) {
    int status = compareWideString(name, _table[i]._name);
    if (status == 0) {
      result = true;
      p = _table[i]._object;
      break;
    }
  }
  return result;
}

void OMObjectDirectory::insert(const wchar_t* name, const OMStorable* p)
{
  TRACE("OMObjectDirectory::insert");

  if (_current < _capacity) {
    wchar_t* n = saveWideString(name);
    _table[_current]._object = const_cast<OMStorable *>(p);
    _table[_current]._name = n;
    _current++;
  } else {
    ASSERT("Table not full", false);
  }
}

size_t OMObjectDirectory::count(void) const
{
  return _current;
}

OMStorable* OMObjectDirectory::object(size_t index) const
{
  TRACE("OMObjectDirectory::object");
  PRECONDITION("Valid index", index < count());

  return _table[index]._object;
}

const wchar_t* OMObjectDirectory::name(size_t index) const
{
  TRACE("OMObjectDirectory::name");
  PRECONDITION("Valid index", index < count());

  return _table[index]._name;
}
